/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

    connect(ui.actionRaisePriority, &QAction::triggered, this, &RunManagerStatusWidget::raiseJobPriority);
    connect(ui.actionLowerPriority, &QAction::triggered, this, &RunManagerStatusWidget::lowerJobPriority);
    connect(ui.actionRerun, &QAction::triggered, this, &RunManagerStatusWidget::rerunJob);
    connect(ui.actionShowWarnings, &QAction::triggered, this, &RunManagerStatusWidget::showJobWarnings);
    connect(ui.actionGoToOutput, &QAction::triggered, this, &RunManagerStatusWidget::emitGoToOutput);
    connect(ui.actionDelete, &QAction::triggered, this, &RunManagerStatusWidget::deleteJob);
    connect(ui.actionOpenOutputFolder, &QAction::triggered, this, &RunManagerStatusWidget::openOutputFolder);


    ui.actionSep->setSeparator(true);

    connect(ui.treeView, &QTreeView::activated, ui.actionGoToOutput, &QAction::trigger);
    connect(ui.completedTreeView, &QTreeView::activated, ui.actionGoToOutput, &QAction::trigger);

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
    connect(static_cast<QStandardItemModel *>(ui.treeView->model()), &QStandardItemModel::itemChanged, this, &RunManagerStatusWidget::itemChanged);
    connect(ui.treeView->model(), &QAbstractItemModel::rowsInserted, this, &RunManagerStatusWidget::rowInserted);
    connect(ui.treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &RunManagerStatusWidget::jobTreeSelectionChanged);
    connect(ui.completedTreeView->selectionModel(), &QItemSelectionModel::selectionChanged,
      this, &RunManagerStatusWidget::completedJobTreeSelectionChanged);

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


