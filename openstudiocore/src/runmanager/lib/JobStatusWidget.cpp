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

#include "JobStatusWidget.hpp"
#include "Job.hpp"
#include "RunManager.hpp"

#include "../../utilities/core/Application.hpp"

#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>

namespace openstudio {
namespace runmanager {

    namespace detail{
    // JobItem implementation

    JobItem::JobItem(const Job &t_job, int t_col)
      : m_job(t_job), m_col(t_col)
    {
//      m_job.connect(SIGNAL(started(const openstudio::UUID &)), this, SLOT(stateChanged()), Qt::QueuedConnection);
//      m_job.connect(SIGNAL(finished(const openstudio::UUID &, const openstudio::runmanager::JobErrors&)), this, SLOT(stateChanged()), Qt::QueuedConnection);
      m_job.connect(SIGNAL(stateChanged(const openstudio::UUID &)), this, SLOT(stateChanged()), Qt::QueuedConnection);
      m_job.connect(SIGNAL(statusChanged(const openstudio::runmanager::AdvancedStatus &)), this,
          SLOT(statusChanged(const openstudio::runmanager::AdvancedStatus &)), Qt::QueuedConnection);
      m_job.connect(SIGNAL(outputFileChanged(const openstudio::UUID &, const openstudio::runmanager::FileInfo &)), this, 
          SLOT(jobOutputFileChanged(const openstudio::UUID &, const openstudio::runmanager::FileInfo &)), Qt::QueuedConnection);

      setEditable(false);
      updateDescription();

      updateOutputFiles();
    }

    bool JobItem::operator<(const QStandardItem &qsi) const
    {
      if (m_col == 1)
      {
        try {
          const JobItem &ji = dynamic_cast<const JobItem &>(qsi);
          return m_job.index() < ji.m_job.index();
        } catch (const std::bad_cast &) {
          // somehow it's not a JobItem??
        }
      }

      // We aren't column 1, or the types mismatched
      // use the parent one
      return this->QStandardItem::operator<(qsi);
    }

    Job JobItem::getJob()
    {
      return m_job;
    }

    void JobItem::jobOutputFileChanged(const openstudio::UUID &, const openstudio::runmanager::FileInfo &t_fi)
    {
      updateOutputFile(t_fi);
    }




    QStringList JobItem::columnHeaders()
    { 
      QStringList l;
      l.push_back("Job Description");
      l.push_back("Warnings");
      l.push_back("Errors");
      l.push_back("Last Run");
      l.push_back("Status");
  //    l.push_back("Output Directory");
  //    l.push_back("Weather File");

      return l;
    }

    void JobItem::updateDescription()
    {
      
      switch (m_col)
      {
        case 0:
          if (m_job.running())
          {
            setForeground(QBrush(Qt::green));
          } else {
            setForeground(QBrush());
          }
          setText(toQString(m_job.description()));
          break;
        case 1:
          if (!m_job.outOfDate())
          {
            setText(QString::number(m_job.errors().warnings().size()));
          } else {
            setText("");
          }
          break;
        case 2:
          if (!m_job.outOfDate())
          {
            setText(QString::number(m_job.errors().errors().size()));
          } else {
            setText("");
          }
          break;
        case 3:
          if (!m_job.running())
          {
            if (m_job.outOfDate())
            {
              setText("Out Of Date");
            } else {
              boost::optional<openstudio::DateTime> dt = m_job.lastRun();

              if (dt)
              {
                std::stringstream ss;
                ss << dt->date() << " " << dt->time();
                setText(toQString(ss.str()));
              } else {
                setText("N/A");
              }
            }
          } else {
            setText("");
          }
          break;
        case 4:
          setText(toQString(m_job.status().toString()));
          break;
          
//        case 5:
  //        setText(toQString(m_job.outdir().external_file_string()));
    //      break;
    //    case 6:
    //      try 
   //       {
    //        setText(toQString(m_job.detailedDescription()));
     //     } catch (const std::exception &) {
     //       setText("");
     //     }
         
      };

      
    }

    void JobItem::stateChanged()
    {
      updateDescription();
    }

    void JobItem::statusChanged(const openstudio::runmanager::AdvancedStatus &)
    {
      updateDescription();
    }

    void JobItem::updateOutputFiles()
    {
      std::vector<FileInfo> fis = m_job.outputFiles();

      std::for_each(fis.begin(), fis.end(), std::bind(&JobItem::updateOutputFile, this, std::placeholders::_1));
    }

    void JobItem::updateOutputFile(const openstudio::runmanager::FileInfo &t_fi)
    {
      QStandardItem *fileparent = nullptr;

      for (int i = 0; i < rowCount(); ++i)
      {
        if (child(i)->text() == "Output Files")
        {
          fileparent = child(i);
          break;
        }
      }

      if (!fileparent)
      {
        QList<QStandardItem*> row;
        row.push_back(new QStandardItem("Output Files"));
        row.back()->setEditable(false);
        insertRow(0, row);
        fileparent = row.front();
      }

      for (int i = 0; i < fileparent->rowCount(); ++i)
      {
        FileItem *file = dynamic_cast<FileItem *>(fileparent->child(i,0));

        assert(file && "Child is not a FileItem object");

        if (file->fileInfo().filename == t_fi.filename)
        {
          file->setFileInfo(t_fi);
          return;
        }
      }

      // We did not find the file in the existing list
      QList<QStandardItem*> row;
      row.push_back(new FileItem(t_fi));
      fileparent->appendRow(row);
    }

    // FileItem implementation

    FileItem::FileItem(const openstudio::runmanager::FileInfo &t_fi)
      : m_info(t_fi)
    {
      setEditable(false);
      updateDisplay();
    }

    void FileItem::setFileInfo(const openstudio::runmanager::FileInfo &t_fi)
    {
      m_info = t_fi;
      updateDisplay();
    }

    FileInfo FileItem::fileInfo()
    {
      return m_info;
    }

    void FileItem::updateDisplay()
    {
      std::stringstream ss;
      ss << m_info.filename;
      if (m_info.exists) {
        ss << " (" << m_info.lastModified.date() << " " << m_info.lastModified.time() << ")";
      } else {
        ss << " (removed)";
      }
      setText(toQString(ss.str()));
    }
  } // detail

  // rest of the stuff 

  JobStatusWidget::JobStatusWidget(openstudio::runmanager::RunManager t_runmanager, QWidget *parent)
    : QWidget(parent), m_runmanager(t_runmanager)
  {
    ui.setupUi(this);

    if (Application::instance().isDefaultInstance())
    {
      QIcon icon = QIcon(":/images/rm_16.png");
      icon.addPixmap(QPixmap(":/images/rm_32.png"));
      icon.addPixmap(QPixmap(":/images/rm_48.png"));
      icon.addPixmap(QPixmap(":/images/rm_64.png"));
      icon.addPixmap(QPixmap(":/images/rm_128.png"));
      icon.addPixmap(QPixmap(":/images/rm_256.png"));
      setWindowIcon(icon);
    }

    this->setWindowIconText(tr("OpenStudio Job Status"));
    this->setWindowTitle(tr("OpenStudio Job Status"));

    clearWorkflow();

    connect(ui.actionRerun, SIGNAL(triggered()), this, SLOT(rerunJob()));
    connect(ui.actionShowWarnings, SIGNAL(triggered()), this, SLOT(showJobWarnings()));
    connect(ui.actionCleanupOutput, SIGNAL(triggered()), this, SLOT(cleanupJobOutput()));
    connect(ui.actionOpenEditFile, SIGNAL(triggered()), this, SLOT(openEditFile()));
    connect(ui.actionOpenDirectory, SIGNAL(triggered()), this, SLOT(openDirectory()));

    m_runmanager.connect(SIGNAL(statsChanged()), this, SLOT(runmanagerStatsChanged()), Qt::QueuedConnection);

    m_model.setHorizontalHeaderLabels(detail::JobItem::columnHeaders());

    ui.treeView->setModel(m_runmanager.getQItemModel());

    for (int i = 0; i < 15; ++i)
    {
      ui.treeView->setColumnHidden(i, i!=0);
    }

    ui.treeJobs->setModel(&m_model);

    connect(ui.treeJobs->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        this, SLOT(jobTreeSelectionChanged(const QItemSelection &)));
    connect(ui.treeJobs, SIGNAL(activated(const QModelIndex &)), 
        this, SLOT(itemActivated(const QModelIndex &)));
    connect(&m_model, SIGNAL(rowsInserted(const QModelIndex &, int, int)),
        this, SLOT(rowsInserted(const QModelIndex &, int, int)));
    connect(ui.treeView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
        this, SLOT(workflowTreeSelectionChanged(const QItemSelection &)));

    ui.actionSep->setSeparator(true);

    ui.horizontalLayout_3->setStretch(0, 1);
    ui.horizontalLayout_3->setStretch(1, 2);


    runmanagerStatsChanged();
    resizeCols();
  }

  JobStatusWidget::~JobStatusWidget()
  {
  }

  void JobStatusWidget::workflowTreeSelectionChanged(const QItemSelection &i)
  {
    QModelIndexList il = i.indexes();

    if (il.empty())
    {
      clearWorkflow();
    } else {
      runmanager::Job j = m_runmanager.getJob(il.front());
      setWorkflow(j);
    }
  }

  std::vector<std::pair<bool, QAction *> > JobStatusWidget::publicActions()
  {
    std::vector<std::pair<bool, QAction *> > acts;
    acts.push_back(std::make_pair(false, ui.actionRerun));
    acts.push_back(std::make_pair(false, ui.actionShowWarnings));
    acts.push_back(std::make_pair(false, ui.actionCleanupOutput));
    acts.push_back(std::make_pair(false, ui.actionOpenEditFile));
    acts.push_back(std::make_pair(false, ui.actionOpenDirectory));
    return acts;
  }


  void JobStatusWidget::rowsInserted(const QModelIndex &i, int start, int end)
  {
    QStandardItem *rootitem = m_model.itemFromIndex(i);
    while (start <= end)
    {
      QStandardItem *item = rootitem?rootitem->child(start):m_model.item(start);
      ui.treeJobs->setExpanded(item->index(), true);

      detail::JobItem *ji = dynamic_cast<detail::JobItem *>(item);
      if (!ji)
      {
        ui.treeJobs->setFirstColumnSpanned(start, i, true);
      }

      rowsInserted(item->index(), 0, item->rowCount() - 1);

      ++start;

    }
  }

  void JobStatusWidget::runmanagerStatsChanged()
  {
    std::map<std::string, double> stats = m_runmanager.statistics();
    ui.lblJobsInQueue->setText(QString::number(stats["Number of Jobs"]));
    ui.lblLocalJobsRunning->setText(QString::number(stats["Locally Running Jobs"]));
    ui.lblRemoteJobsRunning->setText(QString::number(stats["Remotely Running Jobs"]));
    ui.lblFailedJobs->setText(QString::number(stats["Failed Jobs"]));
    ui.lblCompletedJobs->setText(QString::number(stats["Completed Jobs"]));
  }

  void JobStatusWidget::outputDataAdded(const openstudio::UUID &t_jobid, const std::string &t_data)
  {
    QTextCursor cursor = ui.txtStandardOut->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(openstudio::toQString(t_data));
  }

  void JobStatusWidget::setWorkflow(const openstudio::runmanager::Job &t_job)
  {
    clearWorkflow();
    m_job = t_job;

    t_job.connect(SIGNAL(treeChanged(const openstudio::UUID &)), this, SLOT(updateDetails()));
    addJob(t_job);
    updateDetails();
    QModelIndex mi = m_runmanager.getIndex(t_job);
    ui.treeView->setCurrentIndex(mi);

    ui.treeJobs->setCurrentIndex(m_model.index(0,0));
    jobSelectionChanged(t_job);
  }


  void JobStatusWidget::updateDetails()
  {
    if (m_job)
    {
      ui.lblWorkflowErrors->setText(QString::number(m_job->treeErrors().errors().size()));
      ui.lblWorkflowWarnings->setText(QString::number(m_job->treeErrors().warnings().size()));
      try {
        ui.txtWeatherFile->setText(toQString(m_job->treeDetailedDescription().at(0)));
      } catch (const std::exception &) {
        // no description available
      }
      ui.txtOutputDirectory->setText(toQString(m_job->outdir().native()));
    }
  }

  bool JobStatusWidget::jobIndexLessThan(const Job& lhs, const Job &rhs)
  {
    return lhs.index() < rhs.index();
  }
 
  void JobStatusWidget::clearWorkflow()
  {
    m_job = boost::none;
    jobSelectionCleared();

    m_model.removeRows(0, m_model.rowCount());

    ui.lblWorkflowErrors->setText("");
    ui.lblWorkflowWarnings->setText("");
    ui.txtWeatherFile->setText("");
    ui.txtOutputDirectory->setText("");

  }

  void JobStatusWidget::jobSelectionChanged(const openstudio::runmanager::Job &t_job)
  {
    if (m_outputjob) {
      m_outputjob->disconnect(nullptr,this, nullptr);
    }

    t_job.connect(SIGNAL(outputDataAdded(const openstudio::UUID &, const std::string &)), 
        this, SLOT(outputDataAdded(const openstudio::UUID &, const std::string &)));


    ui.txtStandardOut->setPlainText(openstudio::toQString(t_job.getOutput()));
    m_outputjob = t_job;

    ui.txtStandardOut->setEnabled(true);

    LOG(Debug, "Output updated to show Job: " << toString(t_job.uuid()));
  }

  void JobStatusWidget::jobSelectionCleared()
  {
    ui.txtStandardOut->setPlainText("");
    ui.txtStandardOut->setEnabled(false);

  }

  void JobStatusWidget::openDirectory()
  {
    QModelIndex i = ui.treeJobs->currentIndex();
    if (i.isValid())
    {
      /// \todo should this be done here, or in the runmanager? I can see arguments for both
      ///       it would be easy to move into runmanager if desired

      try {
        LOG(Debug, "Attempting to get file for openDirectory");
        openstudio::path p = getFile(i);
        LOG(Debug, "Attempting to open path for: " << toString(p));

        if (boost::filesystem::is_regular_file(p))
        {
          openFile(p.parent_path() );
        } else {
          openFile(p);
        }
      } catch (const std::exception &e) {
        LOG(Error, "Error opening directory for path: " << e.what());
        QMessageBox::critical(this, "Unable to open selected directory", "Error: " + toQString(std::string(e.what())));
       }

  //    openFile(p);
    }
  }

  std::vector<openstudio::path> JobStatusWidget::getFiles(const QModelIndexList &t_indexes) const
  {
    std::vector<openstudio::path> ret;

    for (QModelIndexList::const_iterator itr = t_indexes.begin();
         itr != t_indexes.end();
         ++itr)
    {
      try {
        ret.push_back(getFile(*itr));
      } catch (const std::range_error &) {
        // one of the indexes was not a file
      } catch (const std::out_of_range &) {
        // one of the indexes was not a file
      }
    }

    return ret;
  }

  openstudio::path JobStatusWidget::getFile(const QModelIndex &t_index) const
  {
    if (t_index.isValid())
    {
      detail::FileItem *fi = dynamic_cast<detail::FileItem *>(m_model.itemFromIndex(t_index));
      if (!fi)
      {
        detail::JobItem *ji = dynamic_cast<detail::JobItem *>(m_model.itemFromIndex(t_index));

        if (!ji) {
          throw std::out_of_range("Unknown index in runmanager");
        }

        return ji->getJob().outdir();
      } else {
        return fi->fileInfo().fullPath;
      }
    } else {
      throw std::out_of_range("Unknown index in runmanager");
    }
  }


  void JobStatusWidget::itemActivated(const QModelIndex &t_index)
  {
    try {
      openstudio::path p = getFile(t_index);
      openFile(p);
    } catch (const std::runtime_error &) {
      // no file to open
    } catch (const std::out_of_range &) {
      // no file to open
    }
  }

  openstudio::runmanager::Job JobStatusWidget::getJob(const QModelIndex &t_index) const
  {
    if (t_index.isValid())
    {
      detail::JobItem *ji = dynamic_cast<detail::JobItem *>(m_model.itemFromIndex(t_index));

      if (ji)
      {
        return ji->getJob();
      }
    }
          
    throw std::out_of_range("Unknown index in runmanager");
  }


  std::vector<openstudio::runmanager::Job> JobStatusWidget::getJobs(const QModelIndexList &t_indexes) const
  {
    std::vector<openstudio::runmanager::Job> ret;

    for (QModelIndexList::const_iterator itr = t_indexes.begin();
         itr != t_indexes.end();
         ++itr)
    {
      try {
        ret.push_back(getJob(*itr));
      } catch (const std::range_error &) {
        // one of the indexes was not a job
      } catch (const std::out_of_range &) {
        // one of the indexes was not a job
      }
    }

    // now we need to sort them based on index()
    std::sort(ret.begin(), ret.end(), 
      std::function<bool (const Job &, const Job &)>(&JobStatusWidget::jobIndexLessThan));

    return ret;
  }


  void JobStatusWidget::jobTreeSelectionChanged(const QItemSelection &)
  {
    QModelIndexList selection = ui.treeJobs->selectionModel()->selectedRows();

    bool multipleselection = selection.size() > 1;
    std::vector<Job> jobs = getJobs(selection);
    bool hasjobs = !jobs.empty();
    bool hasfiles = !getFiles(selection).empty();

    if (hasjobs)
    {
      ui.actionRerun->setEnabled(true);
      ui.actionShowWarnings->setEnabled(true);
      ui.actionCleanupOutput->setEnabled(true);
      ui.actionOpenDirectory->setEnabled(true);
    } else {
      ui.actionRerun->setEnabled(false);
      ui.actionShowWarnings->setEnabled(false);
      ui.actionCleanupOutput->setEnabled(false);
      ui.actionOpenDirectory->setEnabled(false);
    }

    if (hasfiles)
    {
      ui.actionOpenEditFile->setEnabled(true);
    } else {
      ui.actionOpenEditFile->setEnabled(false);
    }

    if (multipleselection)
    {
      ui.actionShowWarnings->setEnabled(false);
      ui.actionCleanupOutput->setEnabled(false);
      ui.actionOpenDirectory->setEnabled(false);

      emit jobSelectionCleared();
    } else {
      //ui.actionShowWarnings->setEnabled(true);
      //ui.actionCleanupOutput->setEnabled(true);
      //ui.actionOpenDirectory->setEnabled(true);

      if (hasjobs)
      {
        emit jobSelectionChanged(getJob(selection.front()));
      }
    }
  }

  void JobStatusWidget::addJob(openstudio::runmanager::Job job)
  {
    LOG(Info, "Adding Job to tree: " << toString(job.uuid()) << " " << job.description());

    boost::optional<Job> parent = job.parent();
    QStandardItem *parentitem = nullptr;

    if (parent)
    {
      parentitem = getJobItem(*parent);

      if (!parentitem)
      {
        LOG(Info, "Unable to enqueue job (" << toString(job.uuid()) << ") parent missing (" << toString(parent->uuid()) <<")");
        throw std::runtime_error("Unable to add a job to runmanager whose parent does not already exist");
      }
    }

    if (getJobItem(job) == nullptr)
    {

      QList<QStandardItem*> cols;
      for (int i = 0; i < detail::JobItem::numColumns; ++i)
      {
        cols.push_back(new detail::JobItem(job, i));
      }

      if (parentitem)
      {
        parentitem->appendRow(cols);
      } else {
        m_model.appendRow(cols);
        resizeCols();
      }
    }

    std::vector<openstudio::runmanager::Job> children = job.children();

    for (std::vector<openstudio::runmanager::Job>::const_iterator itr = children.begin();
         itr != children.end();
         ++itr)
    {
      addJob(*itr);
    }

    boost::optional<openstudio::runmanager::Job> finishedjob = job.finishedJob();

    if (finishedjob)
    {
      addJob(*finishedjob);
    }

  }



  detail::JobItem *JobStatusWidget::getJobItem(const Job &t_job) const
  {
    return getJobItemImpl(t_job, QModelIndex());
  }

  detail::JobItem *JobStatusWidget::getJobItemImpl(const Job &t_job, const QModelIndex &parent) const
  {
    if (parent.isValid())
    {
      QStandardItem *item = m_model.itemFromIndex(parent);

      if (item)
      {
        detail::JobItem *j = dynamic_cast<detail::JobItem *>(item);
        if (j && j->getJob() == t_job)
        {
          return j;
        }
      }
    }

    const int rowcount = m_model.rowCount(parent);

    for (int i = 0; i < rowcount; ++i)
    {
      detail::JobItem *j = getJobItemImpl(t_job, m_model.index(i, 0, parent));
      if (j)
      {
        return j;
      }
    }

    return nullptr;
  }


  void JobStatusWidget::resizeCols()
  {
    for (int i = 0; i < ui.treeJobs->model()->columnCount(); ++i)
    {
      ui.treeJobs->resizeColumnToContents(i);
    }
  }


  void JobStatusWidget::cancelResumeJob()
  {
    std::vector<Job> jobs = getJobs(ui.treeJobs->selectionModel()->selectedRows());

    for (auto & job : jobs)
    {
      job.setCanceled(!job.canceled());
    }
  }

  void JobStatusWidget::rerunJob()
  {
    std::vector<Job> jobs = getJobs(ui.treeJobs->selectionModel()->selectedRows());

    for (auto & job : jobs)
    {
      job.setTreeRunnable(false);
      job.setRunnable(true);
    }
  }

  void JobStatusWidget::cleanupJobOutput()
  {
    QModelIndex i = ui.treeJobs->currentIndex();
    if (i.isValid())
    {
      openstudio::runmanager::Job j = getJob(i);
      j.cleanup();
    }
  }

  void JobStatusWidget::openEditFile()
  {
    std::vector<openstudio::path> files = getFiles(ui.treeJobs->selectionModel()->selectedRows());

    for (std::vector<openstudio::path>::const_iterator itr = files.begin();
         itr != files.end();
         ++itr)
    {
      openFile(*itr);
    }

  }


  // Note: the following code was taken from the QDesktopServices implementation
  // and adapted to not call "startDetached" so that it is possible to monitor the outcome
  // of the attempted URL launch.
#if defined(Q_OS_MAC)
  bool openDocument(const QUrl &file)
  {
    if (!file.isValid())
      return false;
    // LSOpen does not work in this case, use QProcess open instead.
    QProcess process;
    process.start(QLatin1String("open"), QStringList() << file.toLocalFile());
    
    if (!process.waitForFinished(500))
    {
      process.kill();
      return false;
    }

    return process.exitCode() == 0;
  }
#elif defined(Q_OS_X11)
  inline static bool launch(const QUrl &url, const QString &client)
  {
    QProcess process;
    process.start(client + QLatin1Char(' ') + QString::fromLatin1(url.toEncoded().constData()));
    
    if (!process.waitForFinished(500))
    {
      process.kill();
      return false;
    }

    return process.exitCode() == 0;
  }

  static bool openDocument(const QUrl &url)
  {
    if (!url.isValid())
      return false;
    if (launch(url, QLatin1String("xdg-open")))
      return true;
    if (qgetenv("DESKTOP_SESSION") == "gnome" && launch(url, QLatin1String("gnome-open"))) {
      return true;
    } else {
      if (!qgetenv("KDE_FULL_SESSION").isEmpty() && launch(url, QLatin1String("kfmclient exec")))
        return true;
    }
    if (launch(url, QLatin1String("firefox")))
      return true;
    if (launch(url, QLatin1String("mozilla")))
      return true;
    if (launch(url, QLatin1String("netscape")))
      return true;
    if (launch(url, QLatin1String("opera")))
      return true;
    return false;
  }


#else
  bool openDocument(const QUrl &url)
  {
    return QDesktopServices::openUrl(url);
  }

#endif

  void JobStatusWidget::openFile(const openstudio::path &p)
  {
    QString url = toQString(p.native());
    QUrl qurl1 = QUrl::fromLocalFile(url);

    LOG(Debug, "Opening file: " << openstudio::toString(qurl1.toString()));

    if (!boost::filesystem::exists(p))
    {
      QMessageBox::critical(this, "Unable to launch viewer", "File: " + toQString(p) + " does not exist.");
    } else if (!openDocument(qurl1)) {
      QMessageBox::critical(this, "Unable to launch viewer", "No operating system registered viewer for: " + toQString(p));
    }
  }


  void JobStatusWidget::showJobWarnings()
  {
    QModelIndex i = ui.treeJobs->currentIndex();
    if (i.isValid())
    {
      openstudio::runmanager::Job j = getJob(i);
      j.showErrors();
    }
  }


} // runmanager
} // openstudio


