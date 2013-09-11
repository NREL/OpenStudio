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

#ifndef OPENSTUDIO_RUNMANAGER_RUNMANAGER_HPP__
#define OPENSTUDIO_RUNMANAGER_RUNMANAGER_HPP__

#include <runmanager/lib/ConfigOptions.hpp>
#include <runmanager/lib/RunManagerAPI.hpp>

#include <utilities/core/Path.hpp>
#include <utilities/core/UUID.hpp>

#include <QAbstractItemModel>

#include <boost/shared_ptr.hpp>

namespace openstudio{
  namespace model
  {
    class Model;
  }

namespace runmanager {
namespace detail {
  class RunManager_Impl;
}

  class Job;
  class Workflow;
  class RunManagerStatus;
  class RunManager;


  /// Handle to a RunManagerStatus dialog
  /// when the last copy of the handle goes out of scope, the dialog is 
  /// destroyed.
  class RUNMANAGER_API RunManagerStatusHandle
  {
    friend class RunManager;

    public:
      RunManagerStatusHandle()
      {
      }

    private:
      RunManagerStatusHandle(const boost::shared_ptr<RunManagerStatus> &t_statusUI)
        : m_statusUI(t_statusUI)
      {
      }
           
      boost::shared_ptr<RunManagerStatus> m_statusUI;
  };

  /// A handle to an underlying RunManager_Impl object, can be copied and passed
  /// around freely. If two RunManager objects are created using the same DB path, 
  /// the same RunManager is loaded and shared between the two objects.
  /// This prevents any issues between file sharing for the DB's and ensures
  /// that each instantiation receives the same signals
  class RUNMANAGER_API RunManager
  {
    public:
      /// Construct a RunManager using a new temporary database file for this one use only
      /// \param[in] t_paused If true the processor is started in paused mode.
      /// \param[in] t_initializeui If true the application's UI subsystem is initialized. Used in a standalone
      ///                           application that utilized RunManager
      RunManager(bool t_paused = false, bool t_initializeui = true);

      /// Construct a RunManager from a given database file path
      /// \param[in] DB The file location for the database storing prefs and job queue
      /// \param[in] t_new If true, the database file is truncated and started from scratch
      /// \param[in] t_paused If true the processor is started in paused mode.
      /// \param[in] t_initializeui If true the application's UI subsystem is initialized. Used in a standalone
      ///                           application that utilized RunManager
      RunManager(const openstudio::path &DB, bool t_new = false, bool t_paused = false, bool t_initializeui=true);
      ~RunManager();

      /// Return tue if the given job is out of date
      /// \todo This should probably be removed in favor of openstudio::runmanager::Job::isOutOfDate
      bool outOfDate(const openstudio::runmanager::Job &job) const;

      /// Load all of the jobs from the given database file into the current RunManager, enqueuing them
      void loadJobs(const openstudio::path &t_db);

      /// Load all of the jobs from the given JSON string, updating job trees
      void updateJobs(const std::string &t_json, bool t_externallyManaged);

      /// Load all of the jobs from the given JSON structure represented by a QVariant,
      /// updating job trees
      void updateJobs(const QVariant &t_variant, const VersionString &t_version, bool t_externallyManaged);

      /// update job trees
      void updateJobs(const std::vector<Job> &t_jobs);

      /// update job tree
      void updateJob(const Job &t_job);

      /// update job tree
      void updateJob(const Job &t_job, const openstudio::path &t_dir);

      std::string jobsToJson() const;

      std::vector<Job> jobsForExport() const;

      /// Queue up a job (and all children) for processing
      /// \param[in] job Job to enqueue
      /// \param[in] force Process job even if it is out of date
      /// \param[in] t_basePath Path by which relative paths in this job should be evaluated. If not provided,
      ///                       the path of the runmanager db is used.
      void enqueue(const openstudio::runmanager::Job &job, bool force, const openstudio::path &t_basePath = openstudio::path());

      /// Queue up a job (and all children) for processing
      /// \param[in] jobs Vector of Jobs to enqueue
      /// \param[in] force Process job even if it is out of date
      /// \param[in] t_basePath Path by which relative paths in this job should be evaluated. If not provided,
      ///                       the path of the runmanager db is used.
      void enqueue(const std::vector<openstudio::runmanager::Job> &jobs, bool force, const openstudio::path &t_basePath = openstudio::path());

      /// Remove the given job and all child jobs from the queue
      void remove(const openstudio::runmanager::Job &job);

      /// Return a QAbstractItemModel which represents the RunManager work queue. The
      /// model is appropriate for using with a QTreeView widget
      QAbstractItemModel *getQItemModel();

      /// Return a vector of all Jobs the RunManager knows about
      std::vector<openstudio::runmanager::Job> getJobs() const;

      /// \return true if there is still work pending
      bool workPending() const;

      /// \return true if the queue is paused
      bool paused() const;

      /// set paused state
      void setPaused(bool t_paused = true);

      /// Update all out of date jobs to be runnable again
      /// Jobs do not automatically restart as soon as they are out of date
      void setOutOfDateRunnable();

      /// Block until the RunManager has finished processing all work.
      /// This method is generally only recommended for a non-gui scenario.
      void waitForFinished(int m_secs = -1);

      /// Return the job that is represented by the given openstudio::UUID
      /// throw an std::out_of_range if a job with the UUID does not exist
      /// \param[in] t_uuid UUID to look up
      openstudio::runmanager::Job getJob(const openstudio::UUID &t_uuid) const;

      /// Return the job at the current QModelIndex
      /// throw an std::out_of_range if the job could not be found
      /// \param[in] t_index QModelIndex to look up
      openstudio::runmanager::Job getJob(const QModelIndex &t_index) const;

      /// \returns a vector of all jobs contained in the QModelIndexList provided
      std::vector<openstudio::runmanager::Job> getJobs(const QModelIndexList &t_indexes) const;

      /// \returns the QModelIndex for the given job in the QAbstractItemModel
      QModelIndex getIndex(const Job &t_job) const;

      /// Return a copy of the current openstudio::runmanager::ConfigOptions object
      openstudio::runmanager::ConfigOptions getConfigOptions() const;

      /// Update the configuration options in one atomic step
      void setConfigOptions(const openstudio::runmanager::ConfigOptions &co);

      /// Launch the openstudio::runmanager::Configuration UI for choosing configuration options
      /// \param t_parent The parent widget for the created Configuration UI
      void showConfigGui(QWidget *t_parent);

      /// Raise the priority of the given job
      void raisePriority(const Job &t_job);

      /// Lower the priority of the given job
      void lowerPriority(const Job &t_job);

      /// Raise the priority of the given jobs
      void raisePriority(const std::vector<Job> &t_jobs);

      /// Lower the priority of the given jobs
      void lowerPriority(const std::vector<Job> &t_jobs);

      /// Launch the openstudio::runmanager::Configuration UI. This method creates one that
      /// is parentless
      void showConfigGui();

      /// Show a status dialog of the processes taking place if one is not already open. A handle to the dialog is returned.
      /// When the last copy of the handle goes out of scope the dialog is destroyed.
      RunManagerStatusHandle showStatusDialog();

      /// Hide the status dialog
      void hideStatusDialog();

      /// Return the file location this RunManager DB was created from
      openstudio::path dbPath() const;

      /// Return default path to use for the RunManager settings/queue database
      /// something such as $HOME/.runmanager_default_db
      static openstudio::path defaultDBLocation();

      /** Connect a Qt SLOT to a signal in the underlying RunManager_Impl object
       *  available signals:
       *    - pausedChanged(bool)
       *  \param signal signal on RunManager_Impl to connect to
       *  \param receiver object receiving signal
       *  \param method slot on receiver to connect to
       *  \param type type of Qt connection */
      bool connect(const char *signal, 
          const QObject *receiver, const char *method, 
          Qt::ConnectionType type = Qt::QueuedConnection) const;

      /// Connect a Qt SIGNAL to a slot in the underlying RunManager_Impl object
      /// available slots:
      ///   - setPaused(bool t_state);
      /// \param sender object that SIGNAL is comming from
      /// \param signal signal on object to connect
      /// \param method slot on RunManager_Impl to connect to
      /// \param type type of Qt connection to make
      bool connect(const QObject *sender, const char *signal, const char *method, 
          Qt::ConnectionType type = Qt::QueuedConnection) const;

      /// Disconnects signal or signals from the underlying RunManager_Impl object
      bool disconnect( const char * signal = 0, const QObject * receiver = 0, const char * method = 0);

      /// \returns a set of named statistics regarding the job queue
      std::map<std::string, double> statistics() const;

      /// Sets the password to use when making a SLURM connection.
      /// Passwords are not persisted.
      void setSLURMPassword(const std::string &t_pass);

      /// Persist a workflow to the database
      /// \returns the key the workflow is stored under
      std::string persistWorkflow(const Workflow &_wf);

      /// \returns a vector of all worklows in the database
      std::vector<Workflow> loadWorkflows() const;

      /// \returns a specific workflow 
      Workflow loadWorkflow(const std::string &key) const;

      Workflow loadWorkflowByName(const std::string &t_name) const;

      /// deletes the workflows from the database
      void deleteWorkflows();

      void deleteWorkflowByName(const std::string &t_name);

      /// safely clears all jobs from the runmanager
      void clearJobs();

      /// perform some model simplifications that reduce the simulation time
      /// with accuracy being an acceptable loss
      ///
      /// \todo move this into a more formalized location at some point
      static void simplifyModelForPerformance(openstudio::model::Model &t_model);

    private:
      REGISTER_LOGGER("openstudio.runmanager.RunManager");

      boost::shared_ptr<detail::RunManager_Impl> m_impl;
      RunManagerStatusHandle m_handle;

      struct DB_Handler;
      static DB_Handler &get_db_handler();
      static openstudio::path generateTempPathName();

  };

}
}

#endif


