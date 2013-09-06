/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef OPENSTUDIO_RUNMANAGER_RUNMANAGER_IMPL_HPP__
#define OPENSTUDIO_RUNMANAGER_RUNMANAGER_IMPL_HPP__

#include <QObject>
#include <utilities/core/Path.hpp>
#include <utilities/core/UUID.hpp>
#include <QStandardItemModel>
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>
#include <QDateTime>
#include "Job.hpp"
#include "ConfigOptions.hpp"
#include "LocalProcessCreator.hpp"
#include "SLURMManager.hpp"
#include "Workflow.hpp"
#include "RunManagerStatus.hpp"

namespace openstudio {
namespace runmanager {
  class RunManager;

namespace detail {
  /// WorkflowItem used for providing data about a job tree through the
  /// QStandardItemModel interface
  /// Uses signals and slots to track the state of the Job tree and
  /// update the model as changes occur.
  class WorkflowItem : public QObject, public QStandardItem
  {
    Q_OBJECT;

    public:
      WorkflowItem(const Job &, int col);
      virtual ~WorkflowItem() {}

      Job getJob();

      static const int numColumns = 8;
      static QStringList columnHeaders();

      virtual bool operator<(const QStandardItem &qsi) const;

    private slots:
      void stateChanged();

    private:
      void updateDescription();

      Job m_job;
      int m_col;
  };

  /// Implementation details of the openstudio::runmanager::RunManager class.
  class RunManager_Impl : public QThread
  {
    Q_OBJECT;

    public:
      /// \param DB path to database object with configuration and queue data
      RunManager_Impl(const openstudio::path &DB, bool t_paused=false, bool t_initui=true, bool t_tempdb=false);
      virtual ~RunManager_Impl();

      /// \param job Job to check out of data status for
      /// \returns true if the passed in job is out of date
      bool outOfDate(const openstudio::runmanager::Job &job) const;

      /// Queue a job and all children up for processing
      /// \param t_job job to queue up for processing
      /// \param force true if the job runnable state should be forced to true after queuing
      /// \param[in] t_basePath Path by which relative paths in this job should be evaluated. If empty,
      ///                       the path of the runmanager db is used.
      /// \returns true if the job was successfully added, false if there was a conflict with
      ///          workflowkeys
      bool enqueue(const openstudio::runmanager::Job &t_job, bool force, const openstudio::path &t_path);

      /// Queue a vector of jobs and all children up for processing
      /// \param t_jobs jobs to queue up for processing
      /// \param force true if the job runnable state should be forced to true after queuing
      /// \param[in] t_basePath Path by which relative paths in this job should be evaluated. If empty,
      ///                       the path of the runmanager db is used.
      /// \returns false if any of the jobs failed to be added due to a conflict with workflowkeys 
      ///         
      bool enqueue(const std::vector<openstudio::runmanager::Job> &t_job, bool force, const openstudio::path &t_path);

      /// Remove the given job and all child jobs from the queue
      void remove(openstudio::runmanager::Job job);


      /// \param t_job job to raise the priority (list order) of
      void raisePriority(const openstudio::runmanager::Job &t_job);

      /// \param t_job job to lower the priority (list order) of
      void lowerPriority(const openstudio::runmanager::Job &t_job);

      /// Raise the priority of the given jobs
      void raisePriority(const std::vector<Job> &t_jobs);

      /// Lower the priority of the given jobs
      void lowerPriority(const std::vector<Job> &t_jobs);

      /// Load all of the jobs from the given database file into the current RunManager, enqueuing them
      void loadJobs(const openstudio::path &t_path);

      /// merge existing job trees
      void updateJobs(const std::vector<Job> &t_jobs);

      /// merge / update the given job, setting the base path to the given t_path
      void updateJob(const Job &t_job, const openstudio::path &t_path);

      /// merge / update the given job
      void updateJob(const Job &t_job);

      /// Update all out of date jobs to be runnable again
      void setOutOfDateRunnable();

      /// \returns a QAbstractItemModel pointer appropriate for attaching to a Item viewer,
      ///          such as a tree view or list view
      QAbstractItemModel *getQItemModel();

      /// \returns all known jobs in queue
      std::vector<openstudio::runmanager::Job> getJobs() const;

      /// \returns Job matching t_uuid or throws if not found
      openstudio::runmanager::Job getJob(const openstudio::UUID &t_uuid) const;

      /// \returns Job matching t_index or throws if not found
      openstudio::runmanager::Job getJob(const QModelIndex &t_index) const;

      /// \returns the QModelIndex for the given job in the QAbstractItemModel
      QModelIndex getIndex(const Job &t_job) const;

      /// \returns a vector of all jobs contained in the QModelIndexList provided
      std::vector<openstudio::runmanager::Job> getJobs(const QModelIndexList &t_indexes) const;

      /// \returns QAbstractItemModel row number t_job is on or throws if not found
      int getRow(const Job &t_job) const;

      /// Show the configuration GUI to the user
      /// \param[in] parent QWidget parent for the configuration UI
      void showConfigGui(QWidget *parent);

      /// Returns the current set of ConfigOptions
      ConfigOptions getConfigOptions() const;

      /// Update the configuration options. Called after the showConfigGui is closed, or by the user
      void setConfigOptions(const ConfigOptions &co);

      /// Returns true if the RunManager queue has work left to be done that is knows about.
      /// \returns true if any job is runnable
      bool workPending() const;

      /// \returns true if work processing has been paused
      bool paused() const;

      /// Block until there is no more work pending.
      /// \param[in] t_msecs Max milliseconds to wait, or -1 to wait forever
      void waitForFinished(int m_secs = -1);

      /// Registers the internal meta types used during job processing
      static void registerMetaTypes();

      /// Returns the path of the loaded db file
      openstudio::path dbPath() const;

      /// Sets the password to use when making a SLURM connection.
      /// Passwords are not persisted.
      void setSLURMPassword(const std::string &t_pass);

      /// Persist a workflow to the database
      /// \returns the key the workflow is stored under
      std::string persistWorkflow(const Workflow &_wf);

      /// \returns a vector of all worklows in the database
      std::vector<Workflow> loadWorkflows();

      /// \returns a specific workflow
      Workflow loadWorkflow(const std::string &key);

      /// \deletes a workflow from the database
      void deleteWorkflows();

      /// \returns a pointer to the RunManagerStatus, creates one if it does not exist already
      boost::shared_ptr<runmanager::RunManagerStatus> getStatusDialog(const RunManager &t_rm);

      void hideStatusDialog();

      /// Clear all jobs from the runmanager
      void clearJobs();

      Workflow loadWorkflowByName(const std::string &t_name) const;

      void deleteWorkflowByName(const std::string &t_name);


    public slots:
      /// \param[in] t_state set the paused state to t_state
      void setPaused(bool t_state);

      /// \returns a set of named statistics regarding the job queue
      std::map<std::string, double> statistics() const;

    signals:
      /// Emitted when the job's state has changed
      void pausedChanged(bool);

      /// Stats changed
      void statsChanged();

    protected:
      virtual void run();

    private slots:
      // Slots used for monitoring the status of Jobs that this RunManager is tracking
      void treeStateChanged(const openstudio::UUID &t_uuid);
      void processQueue();
      void remoteProcessStarted(const openstudio::UUID &t_uuid, int t_remoteId, int t_remoteTaskNumber);
      void remoteProcessFinished(const openstudio::UUID &t_uuid, int t_remoteId, int t_remoteTaskNumber);
      void jobFinished(const openstudio::UUID &t_uuid, const openstudio::runmanager::JobErrors &t_errors, 
          const openstudio::DateTime &t_lastRun, const std::vector<openstudio::runmanager::FileInfo> &t_outputFiles);

    private:
      REGISTER_LOGGER("openstudio.runmanager.RunManager");


      class DBHolder;

      QTimer m_timer;


      /// \param t_job job to raise the priority (list order) of
      void raisePriorityInternal(const openstudio::runmanager::Job &t_job);

      /// \param t_job job to lower the priority (list order) of
      void lowerPriorityInternal(const openstudio::runmanager::Job &t_job);

      static bool jobIndexLessThan(const Job& lhs, const Job &rhs);

      /// Generates and returns the statistics from a deque of jobs
      static std::map<std::string, double> generateStatistics(const std::deque<runmanager::Job> &t_jobs);

      /// Returns the WorkflowItem pointer for the given job, or 0 if it does not exist
      WorkflowItem *getWorkflowItem(const Job &t_job) const;

      WorkflowItem *getWorkflowItemImpl(const Job &t_job, const QModelIndex &parent) const;

      bool enqueueImpl(openstudio::runmanager::Job t_job, bool force, const openstudio::path &t_path);


      mutable QMutex m_mutex;
      mutable QMutex m_activate_mutex;
      QWaitCondition m_waitCondition;

      boost::shared_ptr<DBHolder> m_dbholder;

      std::deque<openstudio::runmanager::Job> m_queue;
      std::set<std::string> m_workflowkeys;
      std::set<openstudio::UUID> m_jobuuids;

      QStandardItemModel m_model; //< Data model for passing to Qt data viewer widgets

      openstudio::path m_dbfile;
      volatile bool m_processingQueue;
      volatile bool m_workPending;
      volatile bool m_paused;
      volatile bool m_continue;

      std::string m_SLURMPassword;

      boost::shared_ptr<LocalProcessCreator> m_localProcessCreator;
      boost::shared_ptr<SLURMManager> m_remoteProcessCreator;

      boost::weak_ptr<runmanager::RunManagerStatus> m_statusUI;

      std::map<std::string, double> m_statistics;

      bool m_temporaryDB;

      int m_lastRunning;
      int m_lastRunningRemotely;
      int m_lastRunningLocally;
      QDateTime m_lastStatistics;


      /// Debugging tool for printing the current queue to standard out
      void print_queue() const;

      void setContinue(bool t_cont);
      bool getContinue() const;

  };
}
}
}

#endif
