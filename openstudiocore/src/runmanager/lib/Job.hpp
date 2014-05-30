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

#ifndef RUNMANAGER_LIB_JOB_HPP
#define RUNMANAGER_LIB_JOB_HPP

#include "RunManagerAPI.hpp"
#include "ProcessCreator.hpp"
#include "AdvancedStatus.hpp"
#include "TreeStatus.hpp"

#include "../../utilities/core/UUID.hpp"
#include "FileInfo.hpp"
#include "JobParam.hpp"
#include "ToolInfo.hpp"
#include "JobErrors.hpp"
#include "JobType.hpp"

class QThread;

namespace openstudio {
namespace runmanager {

namespace detail {
  class Job_Impl;
}
  
  class JobFactory;

  class MergedJobResults;
  
  /// A handle to a job that must be created with the JobFactory class.
  /// Jobs are reference counted objects can may be safely copied
  class RUNMANAGER_API Job 
  {
    public:
      Job(const Job &temp);

      friend class JobFactory;

      /// Return the UUID of the Job
      UUID uuid() const;

      /// Return the JobType of the Job
      JobType jobType() const;

      /// Connect a Qt signal/slot pair through to the underlying Job_Impl object
      /// that this object is a handle to.
      /// Available slots:
      ///   - started(const openstudio::UUID &id) - emitted when job is started
      ///   - finished(const openstudio::UUID &id, const openstudio::runmanager::JobErrors &errors) - emitted when job has finished
      ///   - outputFileChanged(const openstudio::UUID &id, const openstudio::runmanager::FileInfo& file) - emitted
      ///          when any output file created by job has been modified
      ///   - outputDataAdded(const openstudio::UUID &id, const std::string &data) - emitted when raw data
      ///          is send to stdout, does not wait for whole lines
      ///   - stateChanged(const openstudio::UUID &id) - emitted when the state (running) or description of the job has changed
      ///   - statusChanged(const openstudio::runmanager::AdvancedStatus &) - emitted when detailed status information is available
      ///   - childrenChanged(const openstudio::UUID &id) - emitted after the list of children has changed
      ///   - parentChanged(const openstudio::UUID &id) - emitted after the parent has changed
      ///   - treeChanged() - emitted if any of the jobs in the job tree (this one or any children) have changed in any way
      ///   - uuidChanged(const openstudio::UUID &oldId, const openstudio::UUID &newId) - emitted if the UUID of this job changed
      /// \param[in] signal signal to connect to
      /// \param[in] receiver signal receiver
      /// \param[in] method receiving slot
      /// \param[in] type Type of connection to make
      bool connect(const char *signal, const QObject *receiver, const char *method, 
         Qt::ConnectionType type = Qt::QueuedConnection) const;

      /// Pass a disconnect call through to the underlying Job_Impl object
      /// \param[in] signal signal to disconnect from
      /// \param[in] receiver signal receiver to disconnect 
      /// \param[in] method receiving slot to disconnect
      ///
      /// \sa Job::connect
      bool disconnect(const char *signal=nullptr, const QObject *receiver=nullptr, const char *method=nullptr);

      /// Begin execution of the job. Note that execution does not necessarily begin immediately.
      /// Check isRunning or handle the started() signal to know when the job state has changed
      ///
      /// \param[in] t_pc ProcessCreator to use if any process needs to be executed by the job
      void start(const std::shared_ptr<ProcessCreator> &t_pc);

      /** Begin execution of the job, used for restarting a job that was left running on a remote 
       *  server.
       *  \param[in] t_pc process creator
       *  \param[in] t_remoteid the id of the process which should already be running on a remote 
       *  server */
      void start(const std::shared_ptr<ProcessCreator> &t_pc, int t_remoteid, int t_remoteTaskNumber);

      /// Synchronously block until the Job has been finished. 
      /// \param[in] t_msecs Number of milliseconds to wait for
      /// if -1, wait indefinitely
      void waitForFinished(int t_msecs = -1);

      /// Return true if the job is currently running.
      bool running() const;

      /// Return true if the job is running remotely. running() will also return true in this state
      bool runningRemotely() const;

      /// Return true if the job is out of date
      bool outOfDate() const;

      /// Set the canceled state of the job
      /// The job will continue processing if it has already begun,
      /// but will refuse to restart processing if the job becomes out of date later
      void setCanceled(bool t_canceled = true);

      /// Returns the canceled state of the job
      bool canceled() const;

      /// Return last time that the Job was run (set at the "start" time)
      /// \return empty value if the Job has never been run.
      boost::optional<openstudio::DateTime> lastRun() const;

      /// Return a user friendly description of the job.
      std::string description() const;

      /// Return a vector of all output files generated by the Job. The list 
      /// may change as the job generates more output files.
      std::vector<FileInfo> outputFiles() const;

      /// Return a vector of all input files the job was created with, with no base path application
      std::vector<FileInfo> rawInputFiles() const;

      /// Return a vector of all input files the job was created with
      std::vector<FileInfo> inputFiles() const;

      /// Return a vector of all params this job was created with
      std::vector<JobParam> params() const;

      /// Return a JobParams object for the params in this job
      JobParams jobParams() const;

      /// Return a vector of all tools this job was created with
      std::vector<ToolInfo> tools() const;

      /// Return a detailed description of the job
      std::string detailedDescription() const;

      /// Return the set of all input files available to this job
      Files allInputFiles() const;

      /// Return the set of all files (input and output) from this job and its
      /// dependencies
      Files allFiles() const;

      /// Get all stdout output generated by the Job so far
      std::string getOutput() const;

      /// Return an object representing the errors and warnings that this job
      /// has generated
      JobErrors errors() const;

      /// Cleanup the output generated by this job
      /// removing any output files that have been created
      void cleanup();

      /// Present the warnings and errors generated by this job to the user
      void showErrors() const;

      /// Present the warnings and errors generated by this job and all children jobs to the user
      void showTreeErrors() const;

      /// Set the runnable state of the job
      /// \param[in] force Forces the job to be runnable even if it is not out of date
      ///                  or has been marked as canceled
      void setRunnable(bool force = false);

      /// \returns true if the job is runnable. 
      bool runnable() const;

      /// \returns true if the job can be run remotely
      bool remoteRunnable() const;

      /// \returns details status information about the Job
      AdvancedStatus status() const;

      /// Adds a new child job to this job. All child jobs
      /// will be executed in parallel after the parent finishes
      /// Any child added will be reparented to this Job
      void addChild(const Job &t_job);

      /// Deparents the provided Job from this Job.
      /// \return false if t_job is not a child of this Job
      bool removeChild(const Job &t_job);

      /// Returns a vector of all children that this job has.
      std::vector<Job> children() const;

      /// Returns true if any child process is out of date
      bool childrenOutOfDate() const;

      /// Returns true if any child process's tree is out of date
      bool childrenTreesOutOfDate() const;

      /// Returns the most recent DateTime that any child was run
      boost::optional<openstudio::DateTime> childrenLastRun() const;

      /// Returns true if any child is currently running
      bool childrenRunning() const;

      /// Returns true if any child's tree is currently running
      bool childrenTreesRunning() const;

      /// \returns the job finished job. 
      /// \sa setFinishedJob
      boost::optional<Job> finishedJob() const;

      /// Sets the "finishedJob" This is a job that executes after
      /// the parent and all parallel children have finished executing.
      void setFinishedJob(const Job &t_job);

      /// \returns the job's parent, if it exists
      boost::optional<Job> parent() const;

      /// \returns the Job's index in the work queue. Or -1 if unset
      int index() const;

      /// \param[in] index Set the job's index in the work queue
      void setIndex(int index);

      /// \returns true if the Jobs' uuids are equal
      bool operator==(const Job &rhs) const;

      /// \returns true if this Job's UUID is < rhs.uuid()
      bool operator<(const Job &rhs) const;

      /// \returns the base path for this process execution
      openstudio::path getBasePath() const;

      /// Sets the base path by which relative paths will be evaluated from this job
      void setBasePath(const openstudio::path &t_basePath);

      /// Sets the base path for this job and all child jobs
      void setBasePathRecursive(const openstudio::path &t_basePath);

      /// \returns the output directory that the Job is using
      openstudio::path outdir() const;

      /// \returns the time the job was started
      boost::optional<openstudio::DateTime> startTime() const;

      /// \returns the time the job completed
      boost::optional<openstudio::DateTime> endTime() const;

      /// return all the specific params this job has appended onto the list of
      /// params acquired from all dependencies
      JobParams allParams() const;

      
      /// \returns true if this job or any job under it is running
      bool treeRunning() const;

      /// \returns a string describing the job tree from this point down
      std::string treeDescription() const;

      /// \returns true if this job or any job under it is out of date
      bool treeOutOfDate() const;

      /// \returns the amalgamation of all warnings / errors reported by this job and all child jobs
      JobErrors treeErrors() const;

      /// \returns the overall status of the job tree
      openstudio::runmanager::TreeStatusEnum treeStatus() const;

      /// \returns the concatenation of the job tree's detailed description fields
      std::vector<std::string> treeDetailedDescription() const; 

      /** \returns all input and output files for all jobs in this job tree. */
      Files treeAllFiles() const;

      /// \returns all output files generated by this job and all of its (grand)children jobs
      Files treeOutputFiles() const;

      /// \returns the time the tree was completed running. Only returns a value if all jobs in the tree have a 
      ///          last run time and returns the latest of the returned values
      boost::optional<openstudio::DateTime> treeLastRun() const;

      /// Sets the runnable state for this job and all child jobs
      void setTreeRunnable(bool force = false);

      /// Set the canceled state of this job and all child jobs
      void setTreeCanceled(bool t_canceled = true);


      /// Return true if this job was run before t_rhs
      /// This function provides a higher resolution comparison than 
      /// lhs.lastRun() < rhs.lastRun()
      /// \param[in] t_rhs the job to compare to.
      bool ranBefore(const openstudio::runmanager::Job &t_rhs) const;

      /// Merge the t_job into the current job, if possible.
      /// If not possible, throw MergeJobError with description as to why.
      /// accepts NullJobs automatically, passes off 
      /// to mergeJobImpl if otherwise.
      void mergeJob(const openstudio::runmanager::Job &t_job);

      /// Returns the history of the status changes of the job
      std::vector<std::pair<boost::posix_time::ptime, AdvancedStatus> > history() const;

      /// Requests that a job stops
      void requestStop();

      /// Swap the internals of this job with rhs.
      void swap(Job &rhs);

  
      /// \returns a re-created Job tree from the given JSON string.
      ///
      /// \throws std::exception if any part of the parsing fails, with description
      ///         of where and why.
      ///
      /// \sa Job::toJSON for more information
      static Job fromJSON(const std::string &t_json);

      /// \returns a snapshot of the complete status of this job and all children
      ///
      /// All paths and tools are left as-is, as if the job were being persisted in
      /// the runmanager database. 
      /// 
      /// Main uses for this function are transporting the status of a job tree
      /// across a network and for display locally and for persisting of complex
      /// workflows.
      /// 
      /// With care to use relative paths, or care to re-create the job file 
      /// structure on two computers it could be used to begin a job tree
      /// on one computer and continue it on another.
      ///
      /// \sa openstudio::runmanager::Workflow::create for information on creating
      ///     a Job tree from a Workflow 
      ///
      /// \sa openstudio::runmanager::Workflow::Workflow for information on
      ///     creating a Workflow from a Job tree
      std::string toJSON() const;

      /// Update this job tree with the details from the other job tree. Throws an error
      /// if the UUID's do not match
      ///
      /// Specifically, the static content: input files, params and tools remain static
      /// but the state, output files, last runtime are updated.
      void updateJob(const Job &t_other, bool t_allowUUIDUpdate);

      Job &operator=(Job rhs);

      void moveToThread(QThread* targetThread);

      /// \returns true if the job is flagged as externally managed
      bool externallyManaged() const;

      /// sets this job (and children) as being externally managed
      void makeExternallyManaged();

      /// \returns all output files relative to the rundir
      Files relativeOutputFiles() const;

      // send job state and file output signals as if the job had gone from no state
      // to the current state
      void sendSignals();

      /// Sets the advancedstatus of the current job. Only allowed on externally managed jobs
      void setStatus(const AdvancedStatus &t_status);

      /// \returns true if the job has merged jobs
      bool hasMergedJobs() const;

      /// \returns the breakout of completed merged jobs
      std::vector<MergedJobResults> mergedJobResults() const;

    protected:
      Job(const std::shared_ptr<detail::Job_Impl> &t_impl);

    private:
      std::shared_ptr<detail::Job_Impl> m_impl;
  };

  /** \relates Job */
  typedef boost::optional<Job> OptionalJob;
}
}


namespace std {
  template<> 
  inline void swap(openstudio::runmanager::Job &lhs, openstudio::runmanager::Job &rhs)
  {
    lhs.swap(rhs);
  }
}

#endif // RUNMANAGER_LIB_JOB_HPP

