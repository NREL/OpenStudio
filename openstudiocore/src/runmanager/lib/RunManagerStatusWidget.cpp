/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include "RunManagerStatusWidget.hpp"
#include "RunManager.hpp"
#include "RunManager_Impl.hpp"
#include "Job.hpp"

#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

namespace openstudio {
namespace runmanager {

  RunManagerStatusWidget::RunManagerStatusWidget(QWidget *parent)
    : QWidget(parent)
  {
    ui.setupUi(this);

    ui.actionGoToOutput->setVisible(false);

    connect(ui.actionRaisePriority, SIGNAL(triggered()), this, SLOT(raiseJobPriority()));
    connect(ui.actionLowerPriority, SIGNAL(triggered()), this, SLOT(lowerJobPriority()));
    connect(ui.actionRerun, SIGNAL(triggered()), this, SLOT(rerunJob()));
    connect(ui.actionShowWarnings, SIGNAL(triggered()), this, SLOT(showJobWarnings()));
    connect(ui.actionGoToOutput, SIGNAL(triggered()), this, SLOT(emitGoToOutput()));
    connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(deleteJob()));
    connect(ui.actionOpenOutputFolder, SIGNAL(triggered()), this, SLOT(openOutputFolder()));


    ui.actionSep->setSeparator(true);

    connect(ui.treeView, SIGNAL(activated(const QModelIndex &)), ui.actionGoToOutput, SLOT(trigger()));
    connect(ui.completedTreeView, SIGNAL(activated(const QModelIndex &)), ui.actionGoToOutput, SLOT(trigger()));

    clearActions();
  }

  RunManagerStatusWidget::~RunManagerStatusWidget()
  {
  }

  void RunManagerStatusWidget::deleteJob()
  {
    std::vector<Job> jobs = m_runmanager->getJobs(getSelectedRows());

    for (const auto & job : jobs)
    {
      if (!job.parent())
      {
        m_runmanager->remove(job);
      }
    }

  }

  void RunManagerStatusWidget::openOutputFolder()
  {
    QModelIndex i = getCurrentIndex();
    if (i.isValid())
    {
      openstudio::runmanager::Job j = m_runmanager->getJob(i);
      openstudio::path p = j.outdir();
      QString url = toQString(p.native());
      QUrl qurl1 = QUrl::fromLocalFile(url);

      LOG(Debug, "Opening dir: " << openstudio::toString(qurl1.toString()));

      if (!boost::filesystem::exists(p))
      {
        QMessageBox::critical(this, "Unable to launch viewer", "File: " + toQString(p) + " does not exist.");
      } else if (!QDesktopServices::openUrl(qurl1)) {
        QMessageBox::critical(this, "Unable to launch viewer", "No operating system registered viewer for: " + toQString(p));
      }
    }
  }

  void RunManagerStatusWidget::pausedToggled(bool t_paused)
  {
    if (t_paused) {
      ui.actionPause->setToolTip("Start Queue Processing");
    } else {
      ui.actionPause->setToolTip("Pause Queue Processing");
    }
  }

  void RunManagerStatusWidget::setRunManager(const RunManager &t_runmanager)
  {
    m_runmanager = t_runmanager;

    ui.actionPause->setChecked(m_runmanager->paused());
    pausedToggled(m_runmanager->paused());
    m_runmanager->connect(SIGNAL(pausedChanged(bool)), ui.actionPause, SLOT(setChecked(bool)));
    m_runmanager->connect(ui.actionPause, SIGNAL(toggled(bool)), SLOT(setPaused(bool)));
    m_runmanager->connect(SIGNAL(pausedChanged(bool)), this, SLOT(pausedToggled(bool)));

    ui.treeView->setModel(m_runmanager->getQItemModel());
    ui.completedTreeView->setModel(m_runmanager->getQItemModel());
    connect(ui.treeView->model(), SIGNAL(itemChanged(QStandardItem *)), 
        this, SLOT(itemChanged(QStandardItem *)));
    connect(ui.treeView->model(), SIGNAL(rowsInserted( const QModelIndex &, int, int)),
        this, SLOT(rowInserted()));
    connect(ui.treeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        this, SLOT(jobTreeSelectionChanged(const QItemSelection &)));
    connect(ui.completedTreeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        this, SLOT(completedJobTreeSelectionChanged(const QItemSelection &)));

    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui.treeView->model());

    for (int i = 0; i < model->rowCount(); ++i)
    {
      itemChanged(model->item(i));
    }

    resizeCols();
  }

  std::vector<std::pair<bool, QAction *> > RunManagerStatusWidget::publicActions()
  {
    std::vector<std::pair<bool, QAction *> > acts;
    acts.push_back(std::make_pair(true, ui.actionPause));
    acts.push_back(std::make_pair(false, ui.actionRerun));
    acts.push_back(std::make_pair(false, ui.actionLowerPriority));
    acts.push_back(std::make_pair(false, ui.actionRaisePriority));
    acts.push_back(std::make_pair(false, ui.actionShowWarnings));
    acts.push_back(std::make_pair(false, ui.actionOpenOutputFolder));
    acts.push_back(std::make_pair(false, ui.actionDelete));
    return acts;
  }

  void RunManagerStatusWidget::setGoToOutputEnabled(bool enabled)
  {
    ui.actionGoToOutput->setVisible(enabled);
  }

  void RunManagerStatusWidget::itemChanged(QStandardItem *i)
  {
    detail::WorkflowItem *wi = dynamic_cast<detail::WorkflowItem *>(i);

    if (wi)
    {
      Job j = wi->getJob();

      if (j.treeStatus() == TreeStatusEnum::Running || j.treeStatus() == TreeStatusEnum::Waiting || j.runnable())
      {
        ui.treeView->setRowHidden(i->row(), QModelIndex(), false);
        ui.completedTreeView->setRowHidden(i->row(), QModelIndex(), true);
      } else {
        ui.treeView->setRowHidden(i->row(), QModelIndex(), true);
        ui.completedTreeView->setRowHidden(i->row(), QModelIndex(), false);
      }
    }
  }


  QModelIndex RunManagerStatusWidget::getCurrentIndex()
  {
    if (ui.treeView->currentIndex().isValid())
    {
      return ui.treeView->currentIndex();
    } else {
      return ui.completedTreeView->currentIndex();
    }
  }

  QModelIndexList RunManagerStatusWidget::getSelectedRows()
  {
    QModelIndexList selection = ui.treeView->selectionModel()->selectedRows();
    if (selection.count() > 0)
    {
      return selection;
    } else {
      return ui.completedTreeView->selectionModel()->selectedRows();
    }
  }


  void RunManagerStatusWidget::raiseJobPriority()
  {
    m_runmanager->raisePriority(m_runmanager->getJobs(getSelectedRows()));
  }

  void RunManagerStatusWidget::emitGoToOutput()
  {
    emit goToOutput();
  }

  void RunManagerStatusWidget::completedJobTreeSelectionChanged(const QItemSelection &)
  {
    QModelIndexList selection = ui.completedTreeView->selectionModel()->selectedRows();
    if (selection.count() > 0)
    {
      ui.treeView->selectionModel()->reset();
      updateActions(selection);
    } else {
      clearActions();
    }
  }

  void RunManagerStatusWidget::jobTreeSelectionChanged(const QItemSelection &)
  {
    QModelIndexList selection = ui.treeView->selectionModel()->selectedRows();
    if (selection.count() > 0)
    {
      ui.completedTreeView->selectionModel()->reset();
      updateActions(selection);
    } else {
      clearActions();
    }
  }

  void RunManagerStatusWidget::clearActions()
  {
    ui.actionRaisePriority->setEnabled(false);
    ui.actionLowerPriority->setEnabled(false);
    ui.actionRerun->setEnabled(false);
    ui.actionShowWarnings->setEnabled(false);
    ui.actionGoToOutput->setEnabled(false);
    ui.actionDelete->setEnabled(false);
  }

  void RunManagerStatusWidget::updateActions(const QModelIndexList &selection)
  {
    bool multipleselection = selection.size() > 1;
    //std::vector<Job> jobs = m_runmanager->getJobs(selection);

    ui.actionRaisePriority->setEnabled(true);
    ui.actionLowerPriority->setEnabled(true);
    ui.actionRerun->setEnabled(true);
    ui.actionShowWarnings->setEnabled(true);
    ui.actionGoToOutput->setEnabled(true);
    ui.actionDelete->setEnabled(true);

    if (multipleselection)
    {
      ui.actionShowWarnings->setEnabled(false);
      emit jobSelectionCleared();
    } else {
      ui.actionShowWarnings->setEnabled(true);
      emit jobSelectionChanged(m_runmanager->getJob(selection.front()));
    }
  }


  void RunManagerStatusWidget::rowInserted()
  {
    QStandardItemModel *model = dynamic_cast<QStandardItemModel *>(ui.treeView->model());

    for (int i = 0; i < model->rowCount(); ++i)
    {
      itemChanged(model->item(i));
    }

    if (ui.treeView->model()->rowCount() < 10 || ui.completedTreeView->model()->rowCount() < 10)
    {
      resizeCols();
    }
  }

  void RunManagerStatusWidget::resizeCols()
  {
    LOG(Debug, "resizeCols called");
    for (int i = 0; i < ui.treeView->model()->columnCount(); ++i)
    {
      ui.treeView->resizeColumnToContents(i);
    }
    for (int i = 0; i < ui.completedTreeView->model()->columnCount(); ++i)
    {
      ui.completedTreeView->resizeColumnToContents(i);
    }
  }


  void RunManagerStatusWidget::cancelResumeJob()
  {
    std::vector<Job> jobs = m_runmanager->getJobs(getSelectedRows());

    for (auto & job : jobs)
    {
      job.setTreeCanceled(!job.canceled());
    }
  }

  void RunManagerStatusWidget::rerunJob()
  {
    std::vector<Job> jobs = m_runmanager->getJobs(getSelectedRows());

    for (auto & job : jobs)
    {
      // Only the top level needs to be forced the others will see themselves as
      // out of date after the top level is re-run
      job.setTreeRunnable(false);
      job.setRunnable(true);
    }
  }

  void RunManagerStatusWidget::cleanupJobOutput()
  {
    QModelIndex i = getCurrentIndex();
    if (i.isValid())
    {
      openstudio::runmanager::Job j = m_runmanager->getJob(i);
      j.cleanup();
    }
  }


  void RunManagerStatusWidget::showJobWarnings()
  {
    QModelIndex i = getCurrentIndex();
    if (i.isValid())
    {
      openstudio::runmanager::Job j = m_runmanager->getJob(i);
      j.showTreeErrors();
    }
  }

  void RunManagerStatusWidget::lowerJobPriority()
  {
    m_runmanager->lowerPriority(m_runmanager->getJobs(getSelectedRows()));
  }


} // runmanager
} // openstudio


