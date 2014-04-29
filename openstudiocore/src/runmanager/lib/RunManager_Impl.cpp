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

#include "RunManager_Impl.hpp"
#include "Configuration.hpp"
#include <utilities/core/Application.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/ApplicationPathHelpers.hpp>
#include <boost/bind.hpp>
#include <runmanager/lib/runmanagerdatabase.hxx>
#include "JobFactory.hpp"
#include "Workflow.hpp"
#include <QFileInfo>
#include <QDateTime>
#include <QMessageBox>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <utilities/core/System.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/bcl/BCLMeasure.hpp>
#include <QtConcurrentRun>
#include <QFuture>
#include <OpenStudio.hxx>

#include <QJsonDocument>
#include <QJsonParseError>
#include "RubyJobUtils.hpp"
#include "WorkItem.hpp"
#include "JSONWorkflowOptions.hpp"
#include "JobFactory.hpp"
#include <ruleset/OSArgument.hpp>
#include <boost/bind.hpp>

namespace openstudio {
namespace runmanager {
namespace detail {

  // WorkflowItem implementation

  WorkflowItem::WorkflowItem(const Job &t_job, int t_col)
    : m_job(t_job), m_col(t_col)
  {
    m_job.connect(SIGNAL(treeChanged(const openstudio::UUID &)), this, SLOT(stateChanged()), Qt::QueuedConnection);

    setEditable(false);

    updateDescription();
  }

  bool WorkflowItem::operator<(const QStandardItem &qsi) const
  {
    if (m_col == 1)
    {
      try {
        const WorkflowItem &ji = dynamic_cast<const WorkflowItem &>(qsi);
        return m_job.index() < ji.m_job.index();
      } catch (const std::bad_cast &) {
        // somehow it's not a WorkflowItem??
      }
    }

    // We aren't column 1, or the types mismatched
    // use the parent one
    return this->QStandardItem::operator<(qsi);
  }

  Job WorkflowItem::getJob()
  {
    return m_job;
  }


  QStringList WorkflowItem::columnHeaders()
  {
    QStringList l;
    l.push_back("Workflow Description");
    l.push_back("Index");
    l.push_back("Warnings");
    l.push_back("Errors");
    l.push_back("Last Run");
    l.push_back("Status");
    l.push_back("Output Directory");
    l.push_back("Weather File");

    return l;
  }

  void WorkflowItem::updateDescription()
  {
    
    switch (m_col)
    {
      case 0:
        if (m_job.treeRunning())
        {
          setForeground(QBrush(Qt::green));
        } else {
          setForeground(QBrush());
        }
        setText(toQString(m_job.treeDescription()));
        break;
      case 1:
        setText(QString::number(m_job.index()));
        break;
      case 2:
        setText(QString::number(m_job.treeErrors().warnings().size()));
        break;
      case 3:
        setText(QString::number(m_job.treeErrors().errors().size()));
        break;
      case 4:
        if (!m_job.treeRunning())
        {
          if (m_job.treeOutOfDate())
          {
            setText("Out Of Date");
          } else {
            boost::optional<openstudio::DateTime> dt = m_job.treeLastRun();

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
      case 5:
        setText(toQString(m_job.treeStatus().valueDescription()));
        break;
      case 6:
        setText(toQString(m_job.outdir().native()));
        break;
      case 7:
        {
          std::vector<std::string> descs = m_job.treeDetailedDescription();
          if (!descs.empty())
          {
            setText(toQString(descs[0]));
          } else {
            setText("");
          }
        }
    };
    
  }

  void WorkflowItem::stateChanged()
  {
    updateDescription();
  }



  class RunManager_Impl::DBHolder
  {
    public:
      DBHolder(const openstudio::path &DB)
        : m_db("sqlite3", "database=" + toString(DB)),
          m_config(initConfigOptions(m_db, DB)),
          m_loading(false),
          m_dbPath(DB),
          m_configOptions(new ConfigOptions(toConfigOptions(m_db, m_config)))
      {

      }

      ConfigOptions getConfigOptions()
      {
        // we know that the m_configOptions object is not going to be modified
        // at worst its shared_ptr will be swapped out inside of
        // setConfigOptions. Therefore we can safely get our own copy of the shared_ptr
        // and return a copy of the object
        boost::shared_ptr<ConfigOptions> co = m_configOptions;

        return *m_configOptions;
      }

      void setLoading(bool loading)
      {
        // If the system is loading, we don't want to persist jobs that are queued during load
        QMutexLocker l(&m_mutex);
        m_loading = loading;
      }

      bool isLoading() const
      {
        QMutexLocker l(&m_mutex);
        return m_loading;
      }

      void setConfigOptions(const ConfigOptions &t_co)
      {
        QMutexLocker l(&m_mutex);
        updateConfiguration(m_db, m_config, t_co);
        m_db.commit();
        m_configOptions = boost::shared_ptr<ConfigOptions>(new ConfigOptions(t_co));
      }

      void setRemoteProcessId(const openstudio::UUID &t_uuid, int t_remoteId, int t_remoteTaskId)
      {
        QMutexLocker l(&m_mutex);
        if (m_loading) return; // we are currently loading, don't persist that which we are loading

        RunManagerDB::RemoteJob rj(m_db);
        rj.uuid = toString(t_uuid);
        rj.remoteId = t_remoteId;
        rj.remoteTaskId = t_remoteTaskId;
        rj.update();
        m_db.commit();

      }

      void clearRemoteProcessId(const openstudio::UUID &t_uuid, int t_remoteId, int t_remoteTaskId)
      {
        QMutexLocker l(&m_mutex);
        std::vector<RunManagerDB::RemoteJob> jobs
          = litesql::select<RunManagerDB::RemoteJob>(m_db,
              RunManagerDB::RemoteJob::Uuid == toString(t_uuid)).all();
        for (std::vector<RunManagerDB::RemoteJob>::iterator itr = jobs.begin();
            itr != jobs.end();
            ++itr)
        {
          itr->del();
        }
        m_db.commit();
      }

      std::map<openstudio::UUID, std::pair<int, int> > loadRemoteJobs()
      {
        std::map<openstudio::UUID, std::pair<int, int> > retvals;

        std::vector<RunManagerDB::RemoteJob> remotejobs
          = litesql::select<RunManagerDB::RemoteJob>(m_db).all();

        for (std::vector<RunManagerDB::RemoteJob>::iterator itr = remotejobs.begin();
             itr != remotejobs.end();
             ++itr)
        {
          std::string uuidstr = itr->uuid;
          openstudio::UUID uuid = toUUID(uuidstr);
          int remoteId = itr->remoteId;
          int remoteTaskId = itr->remoteTaskId;

          if (uuid.isNull())
          {
            LOG(Error, "Error loading job uuid: " << uuidstr);
          } else {
            retvals[uuid] = std::make_pair(remoteId, remoteTaskId);
          }
        }

        return retvals;
      }

      std::vector<openstudio::runmanager::Job> loadJobs()
      {
        QMutexLocker l(&m_mutex);
        return loadJobsImpl(false, "");
      }

      std::vector<openstudio::runmanager::Workflow> loadWorkflows()
      {
        QMutexLocker l(&m_mutex);
        std::vector<openstudio::runmanager::Job> jobs = loadJobsImpl(true, "");

        // ETH@20120216 - Workflow constructor is now explicit. Replacing
        // std::vector<openstudio::runmanager::Workflow> workflows(jobs.begin(), jobs.end());
        std::vector<runmanager::Workflow> workflows;
        for (unsigned i = 0, n = jobs.size(); i < n; ++i) {
          workflows.push_back(Workflow(jobs[i]));
        }
        return workflows;
      }

      openstudio::runmanager::Workflow loadWorkflowByName(const std::string &t_name)
      {
        QMutexLocker l(&m_mutex);
        std::vector<openstudio::runmanager::Job> jobs = loadJobsImpl(true, "");

        for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
             itr != jobs.end();
             ++itr)
        {
          try {
            if (itr->jobParams().get("workflowname").children.at(0).value == t_name)
            {
              return Workflow(*itr);
            }
          } catch (const std::exception &) {
            // continue
          }
        }

        throw std::runtime_error("No workflow with the name " + t_name);
      }

      void deleteWorkflowByName(const std::string &t_name)
      {
        QMutexLocker l(&m_mutex);
        m_db.begin();
        std::vector<openstudio::runmanager::Job> jobs = loadJobsImpl(true, "");

        for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
             itr != jobs.end();
             ++itr)
        {
          try {
            if (itr->jobParams().get("workflowname").children.at(0).value == t_name)
            {
              LOG(Debug, "WorkflowFound " << t_name << " with uuid " << toString(itr->uuid()) << " deleting" );
              deleteJobRecursiveInternal(*itr);
            }
          } catch (const std::exception &) {
            // continue
          }
        }
        m_db.commit();
      }
      
      void deleteWorkflows()
      {
        QMutexLocker l(&m_mutex);
        m_db.begin();
        std::vector<openstudio::runmanager::Job> jobs = loadJobsImpl(true, "");

        for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
             itr != jobs.end(); 
             ++itr)
        {
          deleteJobRecursiveInternal(*itr);
        }

        m_db.commit();
      }

      void deleteJobTree(const openstudio::runmanager::Job &t_job)
      {
        QMutexLocker l(&m_mutex);
        m_db.begin();
        deleteJobRecursiveInternal(t_job);
        m_db.commit();
      }

      void deleteJobRecursiveInternal(const openstudio::runmanager::Job &t_job)
      {
        std::vector<openstudio::runmanager::Job> children = t_job.children();

        for (std::vector<openstudio::runmanager::Job>::const_iterator itr = children.begin();
             itr != children.end();
             ++itr)
        {
          deleteJobRecursiveInternal(*itr);
        }

        if (t_job.finishedJob())
        {
          deleteJobRecursiveInternal(*t_job.finishedJob());
        }

        deleteJobInternal(t_job);
      }

      bool workflowExists(const std::string &t_key)
      {
        QMutexLocker l(&m_mutex);
        std::vector<openstudio::runmanager::Job> jobs = loadJobsImpl(true, t_key);

        for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
             itr != jobs.end();
             ++itr)
        {
          if (itr->jobParams().get("workflowkey").children.at(0).value == t_key)
          {
            return true;
          }
        }

        return false;
      }

      openstudio::runmanager::Workflow loadWorkflow(const std::string &t_key)
      {
        QMutexLocker l(&m_mutex);
        std::vector<openstudio::runmanager::Job> jobs = loadJobsImpl(true, t_key);

        for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
             itr != jobs.end();
             ++itr)
        {
          if (itr->jobParams().get("workflowkey").children.at(0).value == t_key)
          {
            return openstudio::runmanager::Workflow(*itr);
          }
        }

        throw std::runtime_error("Workflow with key " + t_key + " does not exist");
      }


      void deleteJobInternal(const openstudio::runmanager::Job &t_job)
      {
        std::vector<RunManagerDB::Job> jobs
          = litesql::select<RunManagerDB::Job>(m_db,
              RunManagerDB::Job::Uuid == toString(t_job.uuid())).all();
        for (std::vector<RunManagerDB::Job>::iterator itr = jobs.begin();
            itr != jobs.end();
            ++itr)
        {
          itr->del();
        }

        deleteJobFiles<RunManagerDB::JobFileInfo, RunManagerDB::RequiredFile>(t_job);
        deleteJobFiles<RunManagerDB::OutputFileInfo, RunManagerDB::OutputRequiredFile>(t_job);
        deleteJobTools(t_job);
        deleteJobParams(t_job);
        deleteJobStatus(t_job.uuid());
      }


      void deleteJob(const openstudio::runmanager::Job &t_job)
      {
        QMutexLocker l(&m_mutex);
        m_db.begin();
        deleteJobInternal(t_job);
        m_db.commit();
      }

      template<typename Itr>
        void deleteJobs(Itr begin, const Itr &end)
        {
          QMutexLocker l(&m_mutex);

          m_db.begin();
          while (begin != end)
          {
            std::vector<RunManagerDB::Job> jobs
              = litesql::select<RunManagerDB::Job>(m_db,
                  RunManagerDB::Job::Uuid == toString(begin->uuid())).all();

            for (std::vector<RunManagerDB::Job>::iterator itr = jobs.begin();
                itr != jobs.end();
                ++itr)
            {
              itr->del();
            }

            deleteJobFiles<RunManagerDB::JobFileInfo, RunManagerDB::RequiredFile>(*begin);
            deleteJobFiles<RunManagerDB::OutputFileInfo, RunManagerDB::OutputRequiredFile>(*begin);
            deleteJobTools(*begin);
            deleteJobParams(*begin);
            deleteJobStatus(begin->uuid());
            ++begin;
          }

          m_db.commit();
        }

      std::string persistWorkflow(openstudio::runmanager::Workflow t_workflow /*we want a copy*/)
      {
        Job j = t_workflow.create();

        std::string key = j.jobParams().get("workflowkey").children.at(0).value;
        
        m_db.begin();
        if (!workflowExists(key))
        {
          persistJobRecursive(j, true);
          // workflow did not exist, is now added
        }
        m_db.commit();

        return key;
      }

      void persistJobTree(const openstudio::runmanager::Job &t_job)
      {
        m_db.begin();
        persistJobRecursive(t_job, false);
        m_db.commit();
      }

      void persistJobTrees(const std::vector<openstudio::runmanager::Job> &t_jobs)
      {
        m_db.begin();

        for (std::vector<openstudio::runmanager::Job>::const_iterator itr = t_jobs.begin();
             itr != t_jobs.end();
             ++itr)
        {
          persistJobRecursive(*itr, false);
        }

        m_db.commit();
      }

      void persistJobRecursive(const openstudio::runmanager::Job &t_job, bool isWorkflow)
      {
        persistJob(t_job, isWorkflow);
        boost::optional<openstudio::runmanager::Job> fj = t_job.finishedJob();
        if (fj)
        {
          persistJobRecursive(*fj, isWorkflow);
        }

        std::vector<openstudio::runmanager::Job> children = t_job.children();
        for (std::vector<openstudio::runmanager::Job>::const_iterator itr = children.begin();
             itr != children.end();
             ++itr)
        {
          persistJobRecursive(*itr, isWorkflow);
        }

      }

      void persistJob(const openstudio::runmanager::Job &t_job, bool isWorkflow = false)
      {
        QMutexLocker l(&m_mutex);
        if (m_loading) return; // we are currently loading, don't persist that which we are loading

        RunManagerDB::Job j(m_db);
        j.uuid = toString(t_job.uuid());
        j.isFinishedJob = false;
        j.isWorkflowJob = isWorkflow;
        if (t_job.parent())
        {
          j.parentUuid = toString(t_job.parent()->uuid());
          boost::optional<Job> finishedJob = t_job.parent()->finishedJob();

          if (finishedJob)
          {
            if (*finishedJob == t_job)
            {
              j.isFinishedJob = true;
            }
          }
        }
        j.index = t_job.index();
        j.jobType = t_job.jobType().valueName();
        j.update();


        persistJobFiles<RunManagerDB::JobFileInfo, RunManagerDB::RequiredFile>(t_job);
        persistJobTools(t_job);
        persistJobParams(t_job);
        persistJobStatusInternal(t_job);
      }


      static std::map<openstudio::UUID, std::pair<openstudio::DateTime, JobErrors> > loadJobStatus(const std::vector<RunManagerDB::JobStatus> &t_statuses,
          const std::vector<RunManagerDB::JobErrors> &t_errors)
      {
        std::map<openstudio::UUID, std::pair<openstudio::DateTime, JobErrors> > ret;

        for (std::vector<RunManagerDB::JobStatus>::const_iterator itr = t_statuses.begin();
             itr != t_statuses.end();
             ++itr)
        {
          JobErrors err;
          std::string lastRun = itr->lastRun;
          int resultValue = itr->result;
          err.result = ruleset::OSResultValue(resultValue);

          if (err.result == ruleset::OSResultValue::NA) {
            err.numNAs = 1;
          }
          
          ret[openstudio::toUUID(itr->jobUuid)] = std::make_pair(openstudio::DateTime(lastRun), err);
        }

        for (std::vector<RunManagerDB::JobErrors>::const_iterator itr = t_errors.begin();
             itr != t_errors.end();
             ++itr)
        {
          int errortype = itr->errorType;
          ret[openstudio::toUUID(itr->jobUuid)].second.addError(ErrorType(errortype), itr->value);
        }

        return ret;
      }


      void deleteJobStatus(const openstudio::UUID &t_uuid)
      {
        // delete the old
        std::vector<RunManagerDB::JobErrors> errors 
          = litesql::select<RunManagerDB::JobErrors>(m_db,
              RunManagerDB::JobErrors::JobUuid == toString(t_uuid)).all();
        for (std::vector<RunManagerDB::JobErrors>::iterator itr = errors.begin();
            itr != errors.end();
            ++itr)
        {
          itr->del();
        }

        std::vector<RunManagerDB::JobStatus> status 
          = litesql::select<RunManagerDB::JobStatus>(m_db,
              RunManagerDB::JobStatus::JobUuid == toString(t_uuid)).all();
        for (std::vector<RunManagerDB::JobStatus>::iterator itr = status.begin();
            itr != status.end();
            ++itr)
        {
          itr->del();
        }
      }

      void persistJobStatus(const openstudio::UUID &t_uuid, const JobErrors &t_errors, const boost::optional<openstudio::DateTime> &t_lastRun,
          const Files &t_files)
      {
        m_db.begin();
        persistJobStatusInternal(t_uuid, t_errors, t_lastRun, t_files);
        m_db.commit();
      }


      void persistJobStatusInternal(const openstudio::UUID &t_uuid, const JobErrors &t_errors, const boost::optional<openstudio::DateTime> &t_lastRun,
          const Files &t_files)
      {
        LOG(Debug, "(" << openstudio::toString(m_dbPath) << ") Persisting job status for " << openstudio::toString(t_uuid));
        deleteJobStatus(t_uuid);
        deleteJobFiles<RunManagerDB::OutputFileInfo, RunManagerDB::OutputRequiredFile>(t_uuid);

        LOG(Debug, "Deleted previous data job status for " << openstudio::toString(t_uuid));

        persistJobFiles<RunManagerDB::OutputFileInfo, RunManagerDB::OutputRequiredFile>(t_uuid, t_files.files());

        if (!t_lastRun)
        {
          LOG(Debug, "Not persisting, job has not actually finished " << openstudio::toString(t_uuid));
          // Job has not been run, nothing to persist besides deleting the old one
          return;
        }

        RunManagerDB::JobStatus db_js = RunManagerDB::JobStatus(m_db);
        db_js.jobUuid = openstudio::toString(t_uuid);
        db_js.result = static_cast<int>(t_errors.result.value());

        if (t_lastRun)
        {
          db_js.lastRun = t_lastRun->toString();
        }

        db_js.update();


        std::vector<std::pair<runmanager::ErrorType, std::string> > errors = t_errors.allErrors;

        for (std::vector<std::pair<runmanager::ErrorType, std::string> >::const_iterator itr = errors.begin();
             itr != errors.end();
             ++itr)
        {
          RunManagerDB::JobErrors j(m_db);
          j.jobUuid = toString(t_uuid);
          j.errorType = static_cast<int>(itr->first.value());
          j.value = itr->second;
          j.update();
        }

        LOG(Debug, "Done persisting job, committing " << openstudio::toString(t_uuid));
      }

      void persistJobStatusInternal(const openstudio::runmanager::Job &t_job)
      {
        persistJobStatusInternal(t_job.uuid(), t_job.errors(), t_job.lastRun(), openstudio::runmanager::Files(t_job.outputFiles()));
      }

      void persistJobStatus(const openstudio::runmanager::Job &t_job)
      {
        persistJobStatus(t_job.uuid(), t_job.errors(), t_job.lastRun(), openstudio::runmanager::Files(t_job.outputFiles()));
      }


    private:
      mutable QMutex m_mutex;
      RunManagerDB::RunManagerDatabase m_db;
      RunManagerDB::ConfigOptions m_config;
      bool m_loading;
      openstudio::path m_dbPath;
      boost::shared_ptr<openstudio::runmanager::ConfigOptions> m_configOptions;

      std::vector<openstudio::runmanager::Job> loadJobsImpl(bool isWorkflow, const std::string &workflowkey)
      {
        LOG(Trace, "Loading jobs with workflow key: " << workflowkey);
        std::set<openstudio::UUID> jobidstolookup;

        if (!workflowkey.empty())
        {
          std::vector<RunManagerDB::JobParam> childparam
            = litesql::select<RunManagerDB::JobParam>(m_db,
                RunManagerDB::JobParam::Value == workflowkey).all();


          std::deque<openstudio::UUID> parentstoscan;

          for (std::vector<RunManagerDB::JobParam>::const_iterator itr = childparam.begin();
               itr != childparam.end();
               ++itr)
          {
            std::string uuidstr = itr->jobUuid;
            LOG(Trace, "Job with keyfound: " << workflowkey << " " << uuidstr);
            parentstoscan.push_back(openstudio::toUUID(uuidstr));
          }

          while (!parentstoscan.empty())
          {
            openstudio::UUID jobid = parentstoscan.front();
            parentstoscan.pop_front();
            jobidstolookup.insert(jobid);

            std::vector<RunManagerDB::Job> jobs
              = litesql::select<RunManagerDB::Job>(m_db,
                  RunManagerDB::Job::ParentUuid == toString(jobid)).all();

            for (std::vector<RunManagerDB::Job>::const_iterator itr = jobs.begin();
                 itr != jobs.end();
                 ++itr)
            {
              std::string uuidstr = itr->uuid;
              parentstoscan.push_back(openstudio::toUUID(uuidstr));
            }

          }
        }


        QElapsedTimer et;
        et.start();

        std::vector<RunManagerDB::Job> jobs
          = litesql::select<RunManagerDB::Job>(m_db, RunManagerDB::Job::IsWorkflowJob == isWorkflow).orderBy(RunManagerDB::Job::Index).all();



        std::map<openstudio::UUID, Tools> allTools;
        std::map<openstudio::UUID, JobParams> allJobParams;
        std::map<openstudio::UUID, Files> allFiles;
        std::map<openstudio::UUID, Files> allOutputFiles;
        std::map<openstudio::UUID, std::pair<openstudio::DateTime, JobErrors> > allStatus;

        if (!isWorkflow)
        {
          std::vector<RunManagerDB::JobFileInfo> files = litesql::select<RunManagerDB::JobFileInfo>(m_db).all();
          std::vector<RunManagerDB::RequiredFile> requiredfiles = litesql::select<RunManagerDB::RequiredFile>(m_db).all();

          QFuture<std::map<openstudio::UUID, Files> > futurefiles = QtConcurrent::run(boost::bind(&loadJobFiles<RunManagerDB::JobFileInfo, RunManagerDB::RequiredFile>, boost::ref(files), boost::ref(requiredfiles)));

          std::vector<RunManagerDB::JobToolInfo> tools = litesql::select<RunManagerDB::JobToolInfo>(m_db).all();
          std::vector<RunManagerDB::JobParam> params = litesql::select<RunManagerDB::JobParam>(m_db).all();

          QFuture<std::map<openstudio::UUID, JobParams> > futureparams = QtConcurrent::run(boost::bind(&loadJobParams, boost::ref(params)));

          std::vector<RunManagerDB::JobStatus> status = litesql::select<RunManagerDB::JobStatus>(m_db).all();
          std::vector<RunManagerDB::JobErrors> errors = litesql::select<RunManagerDB::JobErrors>(m_db).all();

          std::vector<RunManagerDB::OutputFileInfo> outputfiles = litesql::select<RunManagerDB::OutputFileInfo>(m_db).all();
          std::vector<RunManagerDB::OutputRequiredFile> outputrequiredfiles = litesql::select<RunManagerDB::OutputRequiredFile>(m_db).all();

          QFuture<std::map<openstudio::UUID, Files> > futureoutputfiles = QtConcurrent::run(boost::bind(&loadJobFiles<RunManagerDB::OutputFileInfo, RunManagerDB::OutputRequiredFile>, boost::ref(outputfiles), boost::ref(outputrequiredfiles)));

          LOG(Info, "Time to load all RunManager data: " << et.restart() << " sizes: " << tools.size() << " " << params.size() << " " << outputfiles.size() << " " << outputrequiredfiles.size() << " " << files.size() << " " << requiredfiles.size() << " " << jobs.size() << " " << status.size() << " " << errors.size());


          allStatus = loadJobStatus(status, errors);
          LOG(Info, "Time to parse allStatus: " << et.restart());
          allTools = loadJobTools(tools);
          LOG(Info, "Time to parse alltools: " << et.restart());
//          allJobParams = loadJobParams(params);
          allJobParams = futureparams;
          LOG(Info, "Time to parse allJobParams: " << et.restart());
//          allFiles = loadJobFiles(files, requiredfiles);
          allFiles = futurefiles;
          LOG(Info, "Time to parse allFiles: " << et.restart());
          allOutputFiles = futureoutputfiles;
//          allOutputFiles = loadJobFiles(outputfiles, outputrequiredfiles);
          LOG(Info, "Time to parse allOutputFiles: " << et.restart());
        } else {
          for (std::vector<RunManagerDB::Job>::const_iterator itr = jobs.begin();
               itr != jobs.end();
               ++itr)
          {
            std::string uuidstr = itr->uuid;
            openstudio::UUID uuid = openstudio::toUUID(itr->uuid);

            std::vector<RunManagerDB::JobToolInfo> tools = litesql::select<RunManagerDB::JobToolInfo>(m_db,
              RunManagerDB::JobToolInfo::JobUuid == uuidstr).all();
            std::vector<RunManagerDB::JobParam> params = litesql::select<RunManagerDB::JobParam>(m_db,
              RunManagerDB::JobParam::JobUuid == uuidstr).all();
            //std::vector<RunManagerDB::OutputFileInfo> outputfiles = litesql::select<RunManagerDB::OutputFileInfo>(m_db,
            //  RunManagerDB::OutputFileInfo::JobUuid == uuidstr).all();
            std::vector<RunManagerDB::JobFileInfo> files = litesql::select<RunManagerDB::JobFileInfo>(m_db,
              RunManagerDB::JobFileInfo::JobUuid == uuidstr).all();

            std::vector<RunManagerDB::RequiredFile> requiredfiles;
            for (std::vector<RunManagerDB::JobFileInfo>::const_iterator fileitr = files.begin();
                 fileitr != files.end();
                 ++fileitr)
            {
              std::vector<RunManagerDB::RequiredFile> part = litesql::select<RunManagerDB::RequiredFile>(m_db,
                  RunManagerDB::RequiredFile::ParentId == fileitr->id).all();
              requiredfiles.insert(requiredfiles.end(), part.begin(), part.end());
            }


            LOG(Info, "Time to load for uuid: " << uuidstr << " " << et.restart());

            allTools[uuid] = loadJobTools(tools)[uuid];
            LOG(Info, "Time to parse tools for uuid: " << uuidstr << " " << et.restart());
            allJobParams[uuid] = loadJobParams(params)[uuid];
            LOG(Info, "Time to parse params for uuid: " << uuidstr << " " << et.restart());
            allFiles[uuid] = loadJobFiles(files, requiredfiles)[uuid];
            LOG(Info, "Time to parse files for uuid: " << uuidstr << " " << et.restart());
          }
        }

        std::list<std::pair<RunManagerDB::Job, openstudio::runmanager::Job> > loadedjobs;

        int index = 0;
        for (std::vector<RunManagerDB::Job>::iterator itr = jobs.begin();
             itr != jobs.end();
             ++itr)
        {
          std::string uuidstr = itr->uuid;
          openstudio::UUID uuid = toUUID(uuidstr);

          if (uuid.isNull())
          {
            LOG(Error, "Error loading job uuid: " << uuidstr);
          } else {
            if (!isWorkflow
                || (isWorkflow && workflowkey.empty())
                || (isWorkflow && !workflowkey.empty() && jobidstolookup.count(uuid) != 0))
            {
              boost::optional<openstudio::DateTime> dt;
              JobErrors e;
              Files f;

              std::map<openstudio::UUID, std::pair<openstudio::DateTime, JobErrors> >::const_iterator statusitr = allStatus.find(uuid);

              if (statusitr != allStatus.end())
              {
                const std::pair<openstudio::DateTime, JobErrors> &status = statusitr->second;
                dt = status.first;
                e = status.second;
                f = allOutputFiles[uuid];
              }


              // only add the job if it's one that was found with our workflow key search
              Job j = JobFactory::createJob(
                  JobType(itr->jobType),
                  allTools[uuid],
                  allJobParams[uuid],
                  allFiles[uuid],
                  std::vector<URLSearchPath>(),
                  true,
                  uuid,
                  JobState(dt, e, f, AdvancedStatus(AdvancedStatusEnum::Idle), openstudio::path())
                  );

              loadedjobs.push_back(std::make_pair(*itr, j));

              j.setIndex(index);
              ++index;
            }
          }
        }

        bool didsomething = true;
        std::vector<Job> returnedjobs;
        std::map<UUID,Job> completedjobs;
        while (!loadedjobs.empty() && didsomething)
        {
          didsomething = false;

          // Reconcile parent / child relationships and move jobs to a different list when they can be
          for (std::list<std::pair<RunManagerDB::Job, Job> >::iterator itr = loadedjobs.begin();
               itr != loadedjobs.end();
               ++itr)
          {
            std::string parentuuidstr = itr->first.parentUuid;

            if (parentuuidstr.empty())
            {
              returnedjobs.push_back(itr->second);
              completedjobs.insert(std::map<UUID,Job>::value_type(itr->second.uuid(),itr->second));
              didsomething = true;
              itr = loadedjobs.erase(itr);
            } else {
              openstudio::UUID parentuuid = toUUID(parentuuidstr);

              if (parentuuid.isNull())
              {
                LOG(Error, "Error loading parent uuid: " << parentuuidstr);
              } else {

                std::map<UUID,Job>::iterator itr2 = completedjobs.find(parentuuid);
                if (itr2 != completedjobs.end()) {
                  if (itr->first.isFinishedJob.value())
                  {
                    LOG(Trace, "Successfully loaded finished job: " << toString(itr->second.uuid()));
                    itr2->second.setFinishedJob(itr->second);
                    didsomething = true;
                    completedjobs.insert(std::map<UUID,Job>::value_type(itr->second.uuid(),itr->second));
                    itr = loadedjobs.erase(itr);
                  } else {
                    LOG(Trace, "Successfully loaded child job: " << toString(itr->second.uuid()));
                    itr2->second.addChild(itr->second);
                    didsomething = true;
                    completedjobs.insert(std::map<UUID,Job>::value_type(itr->second.uuid(),itr->second));
                    itr = loadedjobs.erase(itr);
                  }
                }
              }
            }

            if (didsomething)
            {
              // it's just easier this way. We moved a job out of the list, so let's restart
              // and avoid iterating past the end of the list
              break;
            }
          }
        }

        if (!didsomething)
        {
          LOG(Error, "Job loading loop failed to successfully load all jobs in DB");
        }

        return returnedjobs;

      }

      static std::map<openstudio::UUID, JobParams> loadJobParams(const std::vector<RunManagerDB::JobParam> &t_params)
      {
        std::map<openstudio::UUID, std::list<std::pair<RunManagerDB::JobParam, JobParam> > > allloadedparams;

        for (std::vector<RunManagerDB::JobParam>::const_iterator itr = t_params.begin();
             itr != t_params.end();
             ++itr)
        {
          allloadedparams[openstudio::toUUID(itr->jobUuid)].push_back(std::make_pair(*itr, JobParam(itr->value)));
        }

        std::map<openstudio::UUID, JobParams> retval;

        for (std::map<openstudio::UUID, std::list<std::pair<RunManagerDB::JobParam, JobParam> > >::iterator allitr = allloadedparams.begin();
             allitr != allloadedparams.end();
             ++allitr)
        {
          std::list<std::pair<RunManagerDB::JobParam, JobParam> > completedparams;
          std::list<std::pair<RunManagerDB::JobParam, JobParam> > &loadedparams(allitr->second);

          std::vector<std::pair<int, JobParam*> > paramlookup; // yes I'm using a pointer for this temporary storage where I don't want a copy

          // Loop through all of the loaded params as many times as it takes to empty them all out
          bool didsomething = true;
          while (!loadedparams.empty() && didsomething)
          {
            didsomething = false;

            for (std::list<std::pair<RunManagerDB::JobParam, JobParam> >::iterator itr = loadedparams.begin();
                itr != loadedparams.end();
                ++itr)
            {
              // If it has no parent we know it's a top level param
              if (itr->first.parentId == 0)
              {
                completedparams.push_back(*itr);
                paramlookup.push_back(std::make_pair(itr->first.id, &completedparams.back().second));
                didsomething = true;
                loadedparams.erase(itr);
              } else {
                // look for parent
                for (std::vector<std::pair<int, JobParam*> >::iterator itr2 = paramlookup.begin();
                    itr2 != paramlookup.end();
                    ++itr2)
                {
                  if (itr2->first == itr->first.parentId)
                  {
                    itr2->second->children.push_back(itr->second);
                    paramlookup.push_back(std::make_pair(itr->first.id, &itr2->second->children.back()));
                    didsomething = true;
                    loadedparams.erase(itr);
                    break; // break out of find parent loop
                  }
                }
              }

              if (didsomething)
              {
                // We did something, let's break from this loop
                // and restart the search
                break;
              }
            }

          }

          if (!didsomething)
          {
            LOG(Error, "Error loading job params, params left unhandled");
          }

          JobParams params;

          for (std::list<std::pair<RunManagerDB::JobParam, JobParam> >::iterator itr2 = completedparams.begin();
              itr2 != completedparams.end();
              ++itr2)
          {
            params.append(itr2->second);
          }

          retval[allitr->first] = params;
        }

        return retval;
      }

      void deleteJobParams(const openstudio::runmanager::Job &t_job)
      {
        // delete the old
        std::vector<RunManagerDB::JobParam> tools
          = litesql::select<RunManagerDB::JobParam>(m_db,
              RunManagerDB::JobParam::JobUuid == toString(t_job.uuid())).all();
        for (std::vector<RunManagerDB::JobParam>::iterator itr = tools.begin();
            itr != tools.end();
            ++itr)
        {
          itr->del();
        }
      }

      void persistJobParams(const openstudio::runmanager::Job &t_job)
      {
        // add the new
        std::vector<JobParam> params = t_job.params();

        openstudio::path basePath = t_job.getBasePath();

        if (!basePath.empty() && basePath != boost::filesystem::initial_path<openstudio::path>()
            && basePath != m_dbPath.parent_path())
        {
          // as a special case, we want to preserve the basePath if it has been set
          JobParams jps(params);
          jps.append("job_t_basePath", openstudio::toString(basePath));
          params = jps.params();
        }

        for (std::vector<JobParam>::const_iterator itr = params.begin();
            itr != params.end();
            ++itr)
        {
          persistJobParamsImpl(t_job, *itr);
        }
      }

      void persistJobParamsImpl(const openstudio::runmanager::Job &t_job,
          const openstudio::runmanager::JobParam &t_param,
          boost::optional<int> parentid = boost::optional<int>())
      {
        RunManagerDB::JobParam p(m_db);
        p.jobUuid = toString(t_job.uuid());

        if (parentid)
        {
          p.parentId = *parentid;
        }

        p.value = t_param.value;
        p.update();


        for (std::vector<JobParam>::const_iterator itr = t_param.children.begin();
             itr != t_param.children.end();
             ++itr)
        {
          persistJobParamsImpl(t_job, *itr, boost::optional<int>(p.id));
        }
      }

      static std::map<openstudio::UUID, Tools> loadJobTools(const std::vector<RunManagerDB::JobToolInfo> &t_tools)
      {
        std::map<openstudio::UUID, Tools> foundTools;

        for (std::vector<RunManagerDB::JobToolInfo>::const_iterator itr = t_tools.begin();
             itr != t_tools.end();
             ++itr)
        {
          boost::optional<int> major;
          boost::optional<int> minor;
          boost::optional<int> build;

          if (itr->majorVersion >= 0)
          {
            major = itr->majorVersion;
          }

          if (itr->minorVersion >= 0)
          {
            minor = itr->minorVersion;
          }

          if (itr->buildVersion >= 0)
          {
            build = itr->buildVersion;
          }

          ToolVersion tv(major,minor,build);

          std::string regexstr = itr->outFileFilter;

          ToolInfo ti(
              itr->name,
              tv,
              toPath(itr->localBinPath),
              toPath(itr->remoteArchive),
              toPath(itr->remoteExe),
              boost::regex(regexstr)
              );


          foundTools[openstudio::toUUID(itr->jobUuid)].append(ti);
        }

        return foundTools;
      }


      void deleteJobTools(const openstudio::runmanager::Job &t_job)
      {
        // delete the old
        std::vector<RunManagerDB::JobToolInfo> tools
          = litesql::select<RunManagerDB::JobToolInfo>(m_db,
              RunManagerDB::JobToolInfo::JobUuid == toString(t_job.uuid())).all();
        for (std::vector<RunManagerDB::JobToolInfo>::iterator itr = tools.begin();
            itr != tools.end();
            ++itr)
        {
          itr->del();
        }
      }

      void persistJobTools(const openstudio::runmanager::Job &t_job)
      {
        // Add the new
        std::vector<ToolInfo> jobTools = t_job.tools();

        for (std::vector<ToolInfo>::const_iterator itr = jobTools.begin();
             itr != jobTools.end();
             ++itr)
        {
          RunManagerDB::JobToolInfo j(m_db);
          j.jobUuid = toString(t_job.uuid());
          j.name = itr->name;
          j.localBinPath = toString(itr->localBinPath);
          j.remoteArchive = toString(itr->remoteArchive);
          j.remoteExe = toString(itr->remoteExe);
          j.outFileFilter = boost::lexical_cast<std::string>(itr->outFileFilter);

          if (itr->version.getMajor())
          {
            j.majorVersion = *itr->version.getMajor();
          } else {
            j.majorVersion = -1;
          }

          if (itr->version.getMinor())
          {
            j.minorVersion = *itr->version.getMinor();
          } else {
            j.minorVersion = -1;
          }


          if (itr->version.getBuild())
          {
            j.buildVersion = *itr->version.getBuild();
          } else {
            j.buildVersion = -1;
          }


          j.update();
        }
      }

      template<typename JobFileType, typename RequiredFileType>
      static std::map<openstudio::UUID, Files> loadJobFiles(const std::vector<JobFileType> &t_files, const std::vector<RequiredFileType> &t_requiredFiles)
      {
        std::map<int, std::list<std::pair<QUrl, openstudio::path> > > requiredFiles;

        for (typename std::vector<RequiredFileType>::const_iterator itr = t_requiredFiles.begin();
             itr != t_requiredFiles.end();
             ++itr)
        {
          requiredFiles[itr->parentId].push_back(std::make_pair(QUrl::fromUserInput(toQString(itr->from)), toPath(itr->to)));
        }

        std::map<openstudio::UUID, Files> ret;

        for (typename std::vector<JobFileType>::const_iterator itr = t_files.begin();
             itr != t_files.end();
             ++itr)
        {
          openstudio::path fullpath = itr->fullPath.value().empty()?openstudio::path():toPath(itr->fullPath);

          DateTime dt;
          if (!fullpath.empty() && boost::filesystem::exists(fullpath))
          {
            //get the current date time, not the stored one
            dt = toDateTime(QFileInfo(toQString(fullpath)).lastModified());
          }

          FileInfo f(
              itr->fileName,
              dt,
              itr->key,
              itr->fullPath.value().empty()?openstudio::path():toPath(itr->fullPath)
              );

          const std::list<std::pair<QUrl, openstudio::path> > &theseRequiredFiles = requiredFiles[itr->id];

          for (typename std::list<std::pair<QUrl, openstudio::path> >::const_iterator itr2 = theseRequiredFiles.begin();
               itr2 != theseRequiredFiles.end();
               ++itr2)
          {
            try {
              f.addRequiredFile(itr2->first, itr2->second);
            } catch (const std::runtime_error &) {
              LOG(Error, "Error loading runmanager database, db was imported from a previous version and has a required file conflict: '" << openstudio::toString(itr2->first.toString()) << "' to '" << openstudio::toString(itr2->second));
              throw;
            }
          }

          ret[openstudio::toUUID(itr->jobUuid)].append(f);
        }

        return ret;
      }

      template<typename JobFileType, typename RequiredFileType>
      void deleteJobFiles(const openstudio::runmanager::Job &t_job)
      {
        deleteJobFiles<JobFileType, RequiredFileType>(t_job.uuid());
      }

      template<typename JobFileType, typename RequiredFileType>
      void deleteJobFiles(const openstudio::UUID &t_uuid)
      {
        // delete the old
        std::vector<JobFileType> files
          = litesql::select<JobFileType>(m_db,
              JobFileType::JobUuid == toString(t_uuid)).all();

        for (typename std::vector<JobFileType>::iterator itr = files.begin();
            itr != files.end();
            ++itr)
        {
          std::vector<RequiredFileType> requiredFiles
            = litesql::select<RequiredFileType>(m_db, RequiredFileType::ParentId == itr->id).all();

          for (typename std::vector<RequiredFileType>::iterator itr2 = requiredFiles.begin();
               itr2 != requiredFiles.end();
               ++itr2)
          {
            itr2->del();
          }

          itr->del();
        }
      }

      template<typename JobFileType, typename RequiredFileType>
      void persistJobFiles(const openstudio::runmanager::Job &t_job)
      {
        persistJobFiles<JobFileType, RequiredFileType>(t_job.uuid(), t_job.inputFiles());
      }

      template<typename JobFileType, typename RequiredFileType>
      void persistJobFiles(const openstudio::UUID &t_uuid, const std::vector<FileInfo> &t_files)
      {
        // Add the new
        for (std::vector<FileInfo>::const_iterator itr = t_files.begin();
             itr != t_files.end();
             ++itr)
        {
          JobFileType f(m_db);
          LOG(Debug, "Persisting JobFile " << toString(itr->fullPath));
          f.jobUuid = toString(t_uuid);
          if (!itr->fullPath.empty())
          {
            f.fullPath = toString(itr->fullPath);
          }
          f.fileName = itr->filename;
          f.lastModified = boost::lexical_cast<std::string>(itr->lastModified);
          f.key = itr->key;
          f.update();

          std::vector<std::pair<QUrl, openstudio::path> > jobRequiredFiles = itr->requiredFiles;

          for (std::vector<std::pair<QUrl, openstudio::path> >::const_iterator itr2 =
                   jobRequiredFiles.begin();
               itr2 != jobRequiredFiles.end();
               ++itr2)
          {
            LOG(Debug, "Persisting RequiredFile " << toString(itr2->first.toString()) << " to " << toString(itr2->second));
            RequiredFileType rf(m_db);
            int id = f.id;
            rf.parentId = id;
            rf.from = toString(itr2->first.toString());
            rf.to = toString(itr2->second);
            rf.update();
          }
        }
      }

      static ConfigOptions toConfigOptions(RunManagerDB::RunManagerDatabase t_db, const RunManagerDB::ConfigOptions &db_co)
      {
        ConfigOptions co;

        co.setMaxLocalJobs(db_co.maxLocalJobs);
        co.setDefaultIDFLocation(toPath(db_co.defaultIDFLocation));
        co.setDefaultEPWLocation(toPath(db_co.defaultEPWLocation));
        std::string outdir = db_co.outputLocation;
        boost::trim(outdir);
        if (!outdir.empty())
        {
          co.setOutputLocation(toPath(outdir));
        }

        co.setSimpleName(db_co.simpleName);

        co.setMaxSLURMJobs(db_co.maxSLURMJobs);
        co.setSLURMHost(db_co.slurmHost);
        co.setSLURMUserName(db_co.slurmUserName);

        co.setSLURMMaxTime(db_co.slurmMaxTime);
        co.setSLURMPartition(db_co.slurmPartition);
        co.setSLURMAccount(db_co.slurmAccount);

        std::vector<RunManagerDB::ToolLocations> vers = litesql::select<RunManagerDB::ToolLocations>(t_db).all();

        for (std::vector<RunManagerDB::ToolLocations>::const_iterator itr = vers.begin();
             itr != vers.end();
             ++itr)
        {
          boost::optional<int> major;
          boost::optional<int> minor;
          boost::optional<int> build;

          if (itr->majorVer > -1)
          {
            major = itr->majorVer;
          }

          if (itr->minorVer > -1)
          {
            minor = itr->minorVer;
          }

          if (itr->buildVer > -1)
          {
            build = itr->buildVer;
          }

          co.setToolLocation(ToolVersion(major, minor, build),
              ToolLocationInfo(ToolType(itr->toolType), toPath(itr->path), toPath(itr->linuxBinaryArchive)));
        }

        return co;
      }

      static void updateConfiguration(RunManagerDB::RunManagerDatabase &t_db, RunManagerDB::ConfigOptions &db_co, const ConfigOptions &t_co)
      {
        std::vector<RunManagerDB::ToolLocations> dbvers = litesql::select<RunManagerDB::ToolLocations>(t_db).all();
        for (std::vector<RunManagerDB::ToolLocations>::iterator itr = dbvers.begin();
             itr != dbvers.end();
             ++itr)
        {
          itr->del();
        }


        std::vector<std::pair<ToolVersion, ToolLocationInfo> > vers =
          t_co.getToolLocations();

        for (std::vector<std::pair<ToolVersion, ToolLocationInfo> >::const_iterator itr = vers.begin();
             itr != vers.end();
             ++itr)
        {
          RunManagerDB::ToolLocations v(t_db);
          v.toolType = itr->second.toolType.valueName();
          v.path = toString(itr->second.binaryDir);
          v.majorVer = itr->first.getMajor()?itr->first.getMajor().get():-1;
          v.minorVer = itr->first.getMinor()?itr->first.getMinor().get():-1;
          v.buildVer = itr->first.getBuild()?itr->first.getBuild().get():-1;
          v.linuxBinaryArchive = toString(itr->second.linuxBinaryArchive);
          v.update();
        }

        db_co.defaultIDFLocation = toString(t_co.getDefaultIDFLocation());
        db_co.defaultEPWLocation = toString(t_co.getDefaultEPWLocation());

        db_co.outputLocation = toString(t_co.getOutputLocation());
        db_co.simpleName = t_co.getSimpleName();

        db_co.maxLocalJobs = t_co.getMaxLocalJobs();

        db_co.maxSLURMJobs = t_co.getMaxSLURMJobs();
        db_co.slurmHost = t_co.getSLURMHost();
        db_co.slurmUserName = t_co.getSLURMUserName();

        db_co.slurmMaxTime = t_co.getSLURMMaxTime();
        db_co.slurmPartition = t_co.getSLURMPartition();
        db_co.slurmAccount = t_co.getSLURMAccount();

        db_co.update();
      }

      static RunManagerDB::ConfigOptions initConfigOptions(RunManagerDB::RunManagerDatabase &db, const openstudio::path &t_path)
      {
        try {
          db.create();
        } catch (const litesql::SQLError &) {
          // schema already exists - nothing to do
        }

        if (db.needsUpgrade()) {
          db.upgrade();
        }

        try {
          RunManagerDB::MetaData md = litesql::select<RunManagerDB::MetaData>(db).one();
          int version = md.version;
          if (version > 3)
          {
            throw std::runtime_error("Unsupported RunManagerDB version");
          }
        } catch (const litesql::NotFound &) {
          RunManagerDB::MetaData md = RunManagerDB::MetaData(db);
          md.version = 3;
          md.openStudioVersion = openStudioVersion();
          md.originalPath = openstudio::toString(t_path);
          md.update();
        }

        try {
          return litesql::select<RunManagerDB::ConfigOptions>(db).one();
        } catch (const litesql::NotFound &) {
          RunManagerDB::ConfigOptions db_co = RunManagerDB::ConfigOptions(db);
          updateConfiguration(db, db_co, ConfigOptions());
          return db_co;
        }
      }

  };



  // RunManager_Impl implementation

  RunManager_Impl::RunManager_Impl(const openstudio::path &DB, bool t_paused, bool t_initui, bool t_temporaryDB, bool t_useStatusGUI)
    : m_useStatusGUI(t_useStatusGUI && t_initui),
      m_dbholder(new DBHolder(DB)),
      m_dbfile(DB),
      m_processingQueue(false),
      m_workPending(false), m_paused(t_paused), m_continue(true),
      m_localProcessCreator(new LocalProcessCreator()),
      //m_remoteProcessCreator(new SLURMManager()),
      m_temporaryDB(t_temporaryDB),
      m_lastRunning(0),
      m_lastRunningRemotely(0),
      m_lastRunningLocally(0),
      m_lastStatistics(QDateTime::currentDateTime())
  {
    if (!t_initui && t_useStatusGUI)
    {
      LOG(Warn, "User requested to not initialize UI but left status GUI enabled. Disabling status GUI");
    }

    LOG(Info, "Creating RunManager with file: " << toString(DB));
    RunManager_Impl::registerMetaTypes();

    LOG(Info, "Loading config options");
    ConfigOptions co = m_dbholder->getConfigOptions();

    // make sure a QApplication exists, it is required for the
    // Q-Model related code to work properly
    openstudio::Application::instance().application(t_initui);

    m_model.setHorizontalHeaderLabels(WorkflowItem::columnHeaders());

    std::vector<Job> loadedjobs = m_dbholder->loadJobs();
    const std::map<openstudio::UUID, std::pair<int, int> > remotejobs = m_dbholder->loadRemoteJobs();


    LOG(Info, "Queueing Jobs");
    m_dbholder->setLoading(true);

    enqueue(loadedjobs, false, m_dbfile.parent_path());
    for (std::vector<Job>::iterator itr = loadedjobs.begin();
         itr != loadedjobs.end();
         ++itr)
    {
      //std::map<openstudio::UUID, std::pair<int, int> >::const_iterator itr2 = remotejobs.find(itr->uuid());
      //if (itr2 != remotejobs.end())
      //{
      //  try {
      //    m_remoteProcessCreator->activate(); // make sure we have an active connection
      //    itr->start(m_remoteProcessCreator, itr2->second.first, itr2->second.second);
      //  } catch (const std::exception &e) {
      //    QMessageBox::information(0, "Error loading remote job", toQString(std::string("Error: ") + e.what() + " could not restore remote job: " + itr->description()));
      //  }
      //}
    }

    m_dbholder->setLoading(false);


    LOG(Info, "Starting Runmanager");
    start();

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(processQueue()));
    m_timer.start(1000);


    processQueue();
    LOG(Info, "Runmanager Started");
  }

  RunManager_Impl::~RunManager_Impl()
  {
    setPaused(true);

    setContinue(false);
    processQueue();
    wait();


    QMutexLocker lock(&m_mutex);
    std::deque<Job> q = m_queue;
    m_queue.clear();
    for (std::deque<Job>::iterator itr = q.begin();
         itr != q.end();
         ++itr)
    {
      itr->setCanceled(true);
    }

    for (std::deque<Job>::iterator itr = q.begin();
         itr != q.end();
         ++itr)
    {
      itr->requestStop();
    }


    lock.unlock();

    for (std::deque<Job>::iterator itr = q.begin();
         itr != q.end();
         ++itr)
    {
      itr->waitForFinished();
    }
    q.clear();

    lock.relock();
//    m_model.removeRows(0, m_model.rowCount());
    m_dbholder.reset();

    if (m_temporaryDB)
    {
      boost::filesystem::remove(m_dbfile);
    }

  }

  void RunManager_Impl::remoteProcessStarted(const openstudio::UUID &t_uuid, int t_remoteId, int t_remoteTaskId)
  {
    m_dbholder->setRemoteProcessId(t_uuid, t_remoteId, t_remoteTaskId);
  }

  void RunManager_Impl::remoteProcessFinished(const openstudio::UUID &t_uuid, int t_remoteId, int t_remoteTaskId)
  {
    m_dbholder->clearRemoteProcessId(t_uuid, t_remoteId, t_remoteTaskId);
  }

  void RunManager_Impl::registerMetaTypes()
  {
    qRegisterMetaType<openstudio::UUID>("openstudio::UUID");
    qRegisterMetaType<openstudio::runmanager::AdvancedStatus>("openstudio::runmanager::AdvancedStatus");
    qRegisterMetaType<openstudio::runmanager::FileInfo>("openstudio::runmanager::FileInfo");
    qRegisterMetaType<std::vector<openstudio::runmanager::FileInfo> >("std::vector<openstudio::runmanager::FileInfo>");
    qRegisterMetaType<openstudio::runmanager::FileInfo>("openstudio::runmanager::Files");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<QProcess::ExitStatus>("QProcess::ExitStatus");
    qRegisterMetaType<QProcess::ProcessError>("QProcess::ProcessError");
    qRegisterMetaType<openstudio::runmanager::JobErrors>("openstudio::runmanager::JobErrors");
  }


  bool RunManager_Impl::outOfDate(const openstudio::runmanager::Job &job) const
  {
    return job.outOfDate();
  }

  void RunManager_Impl::waitForFinished(int t_msecs)
  {
    if (t_msecs < 0)
    {
      t_msecs = INT_MAX;
    }

    boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();

    while ( workPending()
            && (boost::posix_time::microsec_clock::universal_time() - start).total_milliseconds() < t_msecs)
    {
      openstudio::System::msleep(10);
    }
  }

  void RunManager_Impl::setOutOfDateRunnable()
  {
    QMutexLocker lock(&m_mutex);
    std::deque<Job>::iterator itr = m_queue.begin();
    std::deque<Job>::iterator end = m_queue.end();

    while (itr != end)
    {
      if (itr->outOfDate())
      {
        itr->setRunnable();
      }

      ++itr;
    }
  }

  bool RunManager_Impl::workPending() const
  {
    // See if there are any job cleanups left to do first
    openstudio::Application::instance().processEvents(1);

    QMutexLocker lock(&m_mutex);
    std::deque<openstudio::runmanager::Job> queue = m_queue;
    lock.unlock();

    for (std::deque<openstudio::runmanager::Job>::const_iterator itr = queue.begin();
         itr != queue.end();
         ++itr)
    {
      if (itr->running() || itr->runnable())
      {
//        LOG(Debug, "workPending true: " << itr->running() << " " << itr->runnable());
        return true;
      }
    }

//    LOG(Debug, "workPending false: ");
    return false;
  }

  bool RunManager_Impl::paused() const
  {
    return m_paused;
  }

  void RunManager_Impl::setPaused(bool t_paused)
  {
    const bool changed = (t_paused != m_paused);
    if (changed) {
      m_paused = t_paused;
      LOG(Info, "RunManager queue paused state changed " << t_paused);
      emit pausedChanged(m_paused);
    }

    processQueue();
  }

  boost::shared_ptr<RunManagerStatus> RunManager_Impl::getStatusDialog(const RunManager &t_rm)
  {
    LOG(Info, "Getting status dialog");

    boost::shared_ptr<RunManagerStatus> ui = m_statusUI.lock();

    if (!ui)
    {
      ui = boost::shared_ptr<RunManagerStatus>(new RunManagerStatus(0,0,t_rm));
    }

    ui->open();
    ui->activateWindow();

    m_statusUI = ui;

    return ui;
  }

  void RunManager_Impl::hideStatusDialog()
  {
    LOG(Info, "Hiding status dialog");

    boost::shared_ptr<RunManagerStatus> ui = m_statusUI.lock();

    if (ui)
    {
      ui->hide();
    }
  }

  ruleset::OSArgument makeArg(const std::string &arg_name, const QVariant &arg_value)
  {
    switch (arg_value.type())
    {
      case QVariant::UInt:
      case QVariant::Int:
      case QVariant::LongLong:
      case QVariant::ULongLong:
        {
          ruleset::OSArgument arg = ruleset::OSArgument::makeIntegerArgument(arg_name);
          arg.setValue(arg_value.toInt());
          return arg;
        }

      case QVariant::Double:
        {
          ruleset::OSArgument arg = ruleset::OSArgument::makeDoubleArgument(arg_name);
          arg.setValue(arg_value.toDouble());
          return arg;
        }

      case QVariant::Bool:
        {
          ruleset::OSArgument arg = ruleset::OSArgument::makeBoolArgument(arg_name);
          arg.setValue(arg_value.toBool());
          return arg;
        }

      case QVariant::String:
        {
          ruleset::OSArgument arg = ruleset::OSArgument::makeStringArgument(arg_name);
          arg.setValue(openstudio::toString(arg_value.toString()));
          return arg;
        }


      default:
        throw std::runtime_error(std::string("Unhandled Type: ") + arg_value.typeName());
    }
    throw std::runtime_error("Unknown error building OSArgument");
  }

  bool setArgValue(std::map<std::string, ruleset::OSArgument> &argument_map, const std::string &wf_arg_name, const QVariant &wf_arg_value)
  {
    std::map<std::string, ruleset::OSArgument>::iterator itr = argument_map.find(wf_arg_name);

    ruleset::OSArgument arg = makeArg(wf_arg_name, wf_arg_value);

    if (itr == argument_map.end())
    {
      argument_map.insert(std::make_pair(wf_arg_name, arg));
    } else {
      itr->second = arg.clone();
    }

    return true; // is there really a failure case here?
  }

  openstudio::runmanager::Job RunManager_Impl::runWorkflow(const std::string &t_json, const openstudio::path &t_basePath, const openstudio::path &t_runPath,
      const openstudio::runmanager::Tools &t_tools, const openstudio::runmanager::JSONWorkflowOptions &t_options)
  {
    QJsonParseError parseError;
    QByteArray byteArray(t_json.c_str(), t_json.length());
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray, &parseError);
    if( QJsonParseError::NoError != parseError.error) {
      LOG_FREE_AND_THROW("openstudio.Json","Error parsing JSON: " + toString(parseError.errorString()));
    }
    return runWorkflow(jsonDoc.toVariant(), t_basePath, t_runPath, t_tools, t_options);
  }

  openstudio::runmanager::Job RunManager_Impl::runWorkflow(const openstudio::path &t_jsonPath, const openstudio::path &t_basePath, const openstudio::path &t_runPath,
      const openstudio::runmanager::Tools &t_tools, const openstudio::runmanager::JSONWorkflowOptions &t_options)
  {
    QFile file(toQString(t_jsonPath));
    if (file.open(QFile::ReadOnly)) {
      QJsonParseError parseError;
      QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &parseError);
      file.close();
      if( QJsonParseError::NoError != parseError.error) {
        LOG_FREE_AND_THROW("openstudio.Json","Error parsing JSON: " + toString(parseError.errorString()));
      }
      return runWorkflow(jsonDoc.toVariant(), t_basePath, t_runPath, t_tools, t_options);
    }

    LOG_FREE_AND_THROW("openstudio.Json","Could not open file " << toString(t_jsonPath) << " for reading.");
  }

  openstudio::runmanager::Job RunManager_Impl::runWorkflow(const QVariant &t_variant, const openstudio::path &t_basePath, const openstudio::path &t_runPath,
      const openstudio::runmanager::Tools &t_tools, const openstudio::runmanager::JSONWorkflowOptions &t_options)
  {
    if (t_variant.isNull())
    {
      throw std::runtime_error("Provided JSON was empty");
    }

    QVariantMap data = t_variant.toMap();

    QVariantMap data_point_json = data["data_point"].toMap();
    QVariantMap analysis_json = data["analysis"].toMap();

    LOG(Debug, "Parsing Analysis JSON input & Applying Measures");
    // by hand for now, go and get the information about the measures;
    if (!analysis_json.empty()) {
      LOG(Debug, "Loading baseline model");

      openstudio::path baseline_model_path;

      QVariantMap analysis_seed_json = analysis_json["seed"].toMap();
      if (!analysis_seed_json.empty()) {
        LOG(Debug, openstudio::toString(analysis_json["seed"].toString()));

        // Some reason this hash is not indifferent access;
        if (!analysis_seed_json["path"].isNull()) {

          // This last(2) needs to be cleaned up.  Why don't we know the path of the file;
          baseline_model_path = completeAndNormalize(t_basePath / openstudio::toPath(analysis_seed_json["path"].toString()));
          if (!boost::filesystem::exists(baseline_model_path)) {
            throw std::runtime_error("Seed model " + openstudio::toString(baseline_model_path) + " did not exist");
          }
        } else {
          throw std::runtime_error("No seed model path in JSON defined");
        }
      } else {
        throw std::runtime_error("No seed model block");
      }

      openstudio::path weather_file_path;

      if (!analysis_json["weather_file"].isNull()) {
        if (!analysis_json["weather_file"].toMap()["path"].isNull()) {
          weather_file_path = completeAndNormalize(t_basePath / openstudio::toPath(analysis_json["weather_file"].toMap()["path"].toString()));
          if (!boost::filesystem::exists(weather_file_path)) {
            throw std::runtime_error("Could not find weather file for simulation " + openstudio::toString(weather_file_path) );
          }
        } else {
          throw std::runtime_error("No weather file path defined");
        }
      } else {
        throw std::runtime_error("No weather file block defined");
      }


      QVariantMap analysis_problem_json = analysis_json["problem"].toMap();

      // iterate over the workflow and grab the measures;
      if (!analysis_problem_json["workflow"].isNull() )
      {
        std::vector<RubyJobBuilder> measures;
        std::vector<RubyJobBuilder> energyplusmeasures;

        QVariantList workflows = analysis_problem_json["workflow"].toList();

        for (QVariantList::const_iterator wfitr = workflows.begin();
             wfitr != workflows.end();
             ++wfitr)
        {

          QVariantMap wf = wfitr->toMap();
          // process the measure

          openstudio::path measure_path = openstudio::completeAndNormalize(t_basePath / toPath(wf["bcl_measure_directory"].toString()));
          std::string measure_name = toString(wf["bcl_measure_class_name_ADDME"].toString());
          std::string workflow_name = toString(wf["name"].toString());

          /*
          cannot be done without embedded ruby 
          arguments = measure.arguments(@model);

          /// Create argument map and initialize all the arguments;
          argument_map = OpenStudio::Ruleset::OSArgumentMap.new;
          arguments.each do |v| {
            argument_map[v.name] = v.clone;
          }
          */

          LOG(Debug, "iterate over arguments for workflow item " << workflow_name );

          QVariantList wf_arguments = wf["arguments"].toList();

          std::map<std::string, ruleset::OSArgument> argument_map;


          for (QVariantList::const_iterator wf_arg_itr = wf_arguments.begin();
               wf_arg_itr != wf_arguments.end();
               ++wf_arg_itr)
          {
            QVariantMap wf_arg = wf_arg_itr->toMap();
            QVariant wf_arg_value = wf_arg["value"];
            std::string wf_arg_name = openstudio::toString(wf_arg["name"].toString());

            if (!wf_arg_value.isNull()) {
              LOG(Debug, "Setting argument value " << wf_arg_name << " to " << toString(wf_arg_value.toString()));

              // v = argument_map[toQString(wf_arg_name)];

              // no way to test this without embedded ruby interpreter
              // if (not v) { throw std::runtime_error("Could not find argument map in measure" ); }

              bool value_set = setArgValue(argument_map, wf_arg_name, wf_arg_value);

              if (!value_set)
              {
                throw std::runtime_error("Could not set argument " + wf_arg_name + " of value " + openstudio::toString(wf_arg_value.toString()) + " on model");
              }
            } else {
              if (t_options.throwOnNoWorkflowVariable)  { throw std::runtime_error("Value for argument " + wf_arg_name + " not set in argument list" ); }
              LOG(Debug, "Value for argument " << wf_arg_name << " not set in argument list therefore will use default");
              break;
            }
          }

          LOG(Debug, "iterate over variables for workflow item " << workflow_name );

          QVariantList wf_variables = wf["variables"].toList();

          for (QVariantList::const_iterator wf_var_itr = wf_variables.begin();
               wf_var_itr != wf_variables.end();
               ++wf_var_itr)
          {
            QVariantMap wf_var = wf_var_itr->toMap();
            QString variable_uuid = wf_var["uuid"].toString(); // this is what the variable value is set to

            if (!wf_var["argument"].isNull()) 
            {
              std::string variable_name = toString(wf_var["argument"].toMap()["name"].toString());

              if (variable_name.empty())
              {
                throw std::runtime_error("no variable name set for the argument object");
              }

              /*
               * Since Nick did not provide any example of a datapoint json, this must not be critical for this work
               
              // Get the value from the data point json that was set via R / Problem Formulation;
              if (!data_point_json["data_point"].isNull()) {
                if (!data_point_json["data_point"].toMap()["set_variable_values"].isNull()) {
                  QVariant variable_value = data_point_json["data_point"].toMap()["set_variable_values"].toMap()[variable_uuid];
                  if (!variable_value.isNull()) {
                    LOG(Debug, "Setting variable " << variable_name << " to " << openstudio::toString(variable_value.toString()));

                    bool value_set = setArgValue(argument_map, variable_name, variable_value);

                    if (!value_set)
                    {
                      throw std::runtime_error("Could not set variable " + variable_name + " of value " + openstudio::toString(variable_value.toString()) + " on model");
                    }
                  } else {
                    if (t_options.throwOnNoWorkflowVariable)  { 
                      throw std::runtime_error("Value for variable " + variable_name + ":" + openstudio::toString(variable_uuid) + " not set in datapoint object" ); 
                    }

                    LOG(Debug, "Value for variable " << variable_name << ":" << openstudio::toString(variable_uuid) << " not set in datapoint object");
                    break;
                  }
                } else {
                  throw std::runtime_error("No block for set_variable_values in data point record");
                }
              } else {
                throw std::runtime_error("No block for data_point in data_point record");
              }
              */
            } else {
              throw std::runtime_error("Variable is defined but no argument is present");
            }
          }

          if (wf["measure_type"].toString() == "RubyMeasure") {
            BCLMeasure measure(measure_path);

            measures.push_back(RubyJobBuilder(measure, argument_map, openstudio::toPath("")));
          } else if (wf["measure_type"].toString() == "EnergyPlusMeasure") {
            BCLMeasure measure(measure_path);

            energyplusmeasures.push_back(RubyJobBuilder(measure, argument_map, openstudio::toPath("")));
          }

        }


        Workflow wf;

        for (std::vector<RubyJobBuilder>::iterator itr = measures.begin();
             itr != measures.end();
             ++itr)
        {
          itr->setIncludeDir(getOpenStudioRubyIncludePath());
          wf.addJob(itr->toWorkItem());
        }

        wf.addJob(openstudio::runmanager::JobType::ModelToIdf);
        wf.addJob(openstudio::runmanager::JobType::ExpandObjects);

        for (std::vector<RubyJobBuilder>::iterator itr = energyplusmeasures.begin();
             itr != energyplusmeasures.end();
             ++itr)
        {
          itr->setIncludeDir(getOpenStudioRubyIncludePath());
          wf.addJob(itr->toWorkItem());
        }

        if (t_options.flatOutDir)
        {
          JobParams params;
          params.append(JobParam("flatoutdir"));
          wf.add(params);
        }

        wf.addJob(openstudio::runmanager::JobType::EnergyPlus);
        wf.add(t_tools);

        openstudio::runmanager::Job j = wf.create(t_runPath, baseline_model_path, weather_file_path);
        if (t_options.optimizeJobTree)
        {
          JobFactory::optimizeJobTree(j);
        }


        enqueue(j, true, openstudio::path());
        return j;
      } else {
        throw std::runtime_error("JSON does not contain a workflow object");
      }
    } else {
      throw std::runtime_error("JSON does not contain an analysis object");
    }
  }


  bool RunManager_Impl::enqueue(const openstudio::runmanager::Job &job, bool force, const openstudio::path &basePath)
  {
    boost::optional<openstudio::runmanager::Job> result = enqueueImpl(job,force,basePath);
    if (!result)
    {
      m_dbholder->persistJobTree(job);

      processQueue();
      return true;
    }

    return false;
  }

  boost::optional<openstudio::runmanager::Job> RunManager_Impl::enqueueOrReturnExisting(
      const openstudio::runmanager::Job &job,
      bool force,
      const openstudio::path &path)
  {
    boost::optional<Job> result = enqueueImpl(job,force,path);
    if (!result) {
      m_dbholder->persistJobTree(job);
      processQueue();
    }
    return result;
  }

  bool RunManager_Impl::enqueue(const std::vector<openstudio::runmanager::Job> &t_jobs, bool force, const openstudio::path &t_basePath)
  {
    std::vector<openstudio::runmanager::Job> successfullJobs;

    for(std::vector<openstudio::runmanager::Job>::const_iterator itr = t_jobs.begin();
        itr != t_jobs.end();
        ++itr)
    {
      if (enqueueImpl(*itr, force, t_basePath))
      {
        successfullJobs.push_back(*itr);
      }
    }

    m_dbholder->persistJobTrees(successfullJobs);
    processQueue();

    return successfullJobs.size() == t_jobs.size();
  }


  boost::optional<openstudio::runmanager::Job> RunManager_Impl::enqueueImpl(openstudio::runmanager::Job job, bool force, const openstudio::path &t_basePath)
  {
    boost::optional<Job> result;
    bool addedSignal = false;

    UUID uuid = job.uuid();
    LOG(Info, "Enqueing Job: " << toString(uuid) << " " << job.description());

    if (job.getBasePath().empty() || job.getBasePath() == boost::filesystem::initial_path<openstudio::path>())
    {
      // Set basePath iff it has not been explicitly set before
      job.setBasePath(t_basePath);
    }

    QMutexLocker lock(&m_mutex);


    JobParams params = job.jobParams();
    if (params.has("workflowkey") && !params.get("workflowkey").children.empty())
    {
      std::string key = params.get("workflowkey").children.at(0).value;
      if (!key.empty() && m_workflowkeys.find(key) != m_workflowkeys.end())
      {
        // ETH@20121107 - This seems like an inefficient search. Is the most-parental job in the queue
        // with the same workflow key guranteed to have the same uuid as job? If so, could just search
        // in m_queue for uuid.
        for(std::deque<openstudio::runmanager::Job>::iterator itr = m_queue.begin();
            itr != m_queue.end();
            ++itr)
        {
          if (!itr->parent())
          {
            // only try if it doesn't have a parent - if it's a top level job
            JobParams params2 = itr->jobParams();
            if (params2.has("workflowkey") && !params2.get("workflowkey").children.empty())
            {
              std::string queuekey = params2.get("workflowkey").children.at(0).value;
              if (key == queuekey)
              {
                //Hey look, we found a workflow with the same key, so this is the one we need to
                //restart and *not* add the passed in one
                LOG(Info, "An existing job with the workflowkey of " << key << " exists in the queue, not adding new job, restarting existing job");
                itr->setTreeRunnable(false);
                itr->setRunnable(force);
                result = *itr;
                return result;
              }

            }
          }
        }
      }

      m_workflowkeys.insert(key);
    }


    boost::optional<Job> parent = job.parent();

    if (m_jobuuids.find(uuid) == m_jobuuids.end())
    {
      m_jobuuids.insert(uuid);

      if (!parent)
      {
        //we are the top level job, set up the state of the runnable-ness
        job.setTreeRunnable(false);
        job.setRunnable(force);
        job.setTreeCanceled(false);
        addedSignal = true;
      }

      // If the item does not already exist in queue, create it and
      // create the corresponding QStandardItem model object
      job.setIndex(m_queue.size());
      m_queue.push_back(job);

      if (!parent && m_useStatusGUI)
      {
        // Only top level jobs get displayed in the model
        QList<QStandardItem*> cols;
        for (int i = 0; i < WorkflowItem::numColumns; ++i)
        {
          cols.push_back(new WorkflowItem(job, i));
        }

        m_model.appendRow(cols);
      }

      if (!parent)
      {
        // is a top-level job, add connection for changes
        job.connect(SIGNAL(treeChanged(const openstudio::UUID &)), this,
            SLOT(treeStateChanged(const openstudio::UUID &)), Qt::QueuedConnection);
      }

      job.connect(SIGNAL(remoteProcessStarted(const openstudio::UUID &, int, int)), this,
          SLOT(remoteProcessStarted(const openstudio::UUID &, int, int)), Qt::QueuedConnection);
      job.connect(SIGNAL(remoteProcessFinished(const openstudio::UUID &, int, int)), this,
          SLOT(remoteProcessFinished(const openstudio::UUID &, int, int)), Qt::QueuedConnection);
      job.connect(SIGNAL(finishedExt(const openstudio::UUID &, const openstudio::runmanager::JobErrors &, const openstudio::DateTime &, const std::vector<openstudio::runmanager::FileInfo> &)), this,
          SLOT(jobFinished(const openstudio::UUID &, const openstudio::runmanager::JobErrors &, const openstudio::DateTime &, const std::vector<openstudio::runmanager::FileInfo> &)), Qt::QueuedConnection);
    }

    lock.unlock();

    if (job.externallyManaged())
    {
      job.sendSignals();
    }

    std::vector<openstudio::runmanager::Job> children = job.children();

    for (std::vector<openstudio::runmanager::Job>::const_iterator itr = children.begin();
        itr != children.end();
        ++itr)
    {
      enqueueImpl(*itr, force, t_basePath);
    }

    boost::optional<openstudio::runmanager::Job> finishedjob = job.finishedJob();

    if (finishedjob)
    {
      enqueueImpl(*finishedjob, force, t_basePath);
    }

    if (addedSignal)
    {
      emit jobTreeAdded(job.uuid());
    }

    return result;
  }

  void RunManager_Impl::jobFinished(const openstudio::UUID &t_uuid, const openstudio::runmanager::JobErrors &t_errors, 
      const openstudio::DateTime &t_lastRun, const std::vector<openstudio::runmanager::FileInfo> &t_files)
  {
    m_dbholder->persistJobStatus(t_uuid, t_errors, t_lastRun, runmanager::Files(t_files));
  }

  openstudio::path RunManager_Impl::dbPath() const
  {
    return m_dbfile;
  }


  void RunManager_Impl::remove(openstudio::runmanager::Job job)
  {
    LOG(Info, "Removing job: " << toString(job.uuid()));

    std::vector<Job> toremove;

    QMutexLocker lock(&m_mutex);


    try {
      std::string key = job.jobParams().get("workflowkey").children.at(0).value;
      if (!key.empty())
      {
        m_workflowkeys.erase(key);
      }
    } catch (...) {
      // nothing to be done, no key
    }

    m_jobuuids.erase(job.uuid());

    std::vector<Job> children = job.children();

    for (std::vector<Job>::const_iterator itr = children.begin();
         itr != children.end();
         ++itr)
    {
      toremove.push_back(*itr);
    }

    if (job.finishedJob())
    {
      toremove.push_back(*job.finishedJob());
    }

    std::deque<openstudio::runmanager::Job>::iterator itr =
      std::find(m_queue.begin(), m_queue.end(), job);

    if (itr != m_queue.end())
    {
      itr->requestStop();
      m_queue.erase(itr);
    }

    WorkflowItem *item = getWorkflowItem(job);

    if (item)
    {
      m_model.removeRow(item->index().row(), item->index().parent());
    }

    m_dbholder->deleteJob(job);

    lock.unlock();

    job.waitForFinished();

    for (std::vector<Job>::const_iterator itr = toremove.begin();
         itr != toremove.end();
         ++itr)
    {
      remove(*itr);
    }
  }


  void RunManager_Impl::treeStateChanged(const openstudio::UUID &)
  {
    processQueue();
  }


  QAbstractItemModel *RunManager_Impl::getQItemModel()
  {
    return &m_model;
  }

  QModelIndex RunManager_Impl::getIndex(const Job &t_job) const
  {
    WorkflowItem *i = getWorkflowItem(t_job);
    if (i) {
      return i->index();
    } else {
      return QModelIndex();
    }
  }


  WorkflowItem *RunManager_Impl::getWorkflowItem(const Job &t_job) const
  {
    return getWorkflowItemImpl(t_job, QModelIndex());
  }

  WorkflowItem *RunManager_Impl::getWorkflowItemImpl(const Job &t_job, const QModelIndex &parent) const
  {
    if (parent.isValid())
    {
      QStandardItem *item = m_model.itemFromIndex(parent);

      if (item)
      {
        WorkflowItem *j = dynamic_cast<WorkflowItem *>(item);
        if (j && j->getJob() == t_job)
        {
          return j;
        }
      }
    }

    const int rowcount = m_model.rowCount(parent);

    for (int i = 0; i < rowcount; ++i)
    {
      WorkflowItem *j = getWorkflowItemImpl(t_job, m_model.index(i, 0, parent));
      if (j)
      {
        return j;
      }
    }

    return 0;
  }

  int RunManager_Impl::getRow(const Job &t_job) const
  {
    for (int i = 0; i < m_model.rowCount();  ++i)
    {
      WorkflowItem *j = dynamic_cast<WorkflowItem *>(m_model.item(i));
      if (j && j->getJob() == t_job)
      {
        return i;
      }
    }

    throw std::out_of_range("Job not found: " + toString(t_job.uuid()));
  }

  openstudio::runmanager::Job RunManager_Impl::getJob(const openstudio::UUID &t_uuid) const
  {
    QMutexLocker lock(&m_mutex);

    std::deque<Job>::const_iterator itr = std::find_if(m_queue.begin(),
                                                       m_queue.end(),
                                                       boost::bind(uuidEquals<Job,UUID>,_1,t_uuid));
    if (itr != m_queue.end()) {
      return *itr;
    }

    throw std::out_of_range("Unknown uuid in runmanager: " + toString(t_uuid));
  }

  void RunManager_Impl::print_queue() const
  {
    QMutexLocker lock(&m_mutex);
    std::deque<openstudio::runmanager::Job>::const_iterator itr = m_queue.begin(), end = m_queue.end();

    std::cout << "--- queue ---" << std::endl;
    while (itr != end)
    {
      std::cout << itr->description() << std::endl;
      ++itr;
    }
    std::cout << "--- end queue ---" << std::endl;
  }

  openstudio::runmanager::Job RunManager_Impl::getJob(const QModelIndex &t_index) const
  {
    if (t_index.isValid())
    {
      WorkflowItem *ji = dynamic_cast<WorkflowItem *>(m_model.itemFromIndex(t_index));

      if (ji)
      {
        return ji->getJob();
      }
    }

    throw std::out_of_range("Unknown index in runmanager");
  }

  bool RunManager_Impl::jobIndexLessThan(const Job& lhs, const Job &rhs)
  {
    return lhs.index() < rhs.index();
  }

  std::vector<openstudio::runmanager::Job> RunManager_Impl::getJobs(const QModelIndexList &t_indexes) const
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
      boost::function<bool (const Job &, const Job &)>(&RunManager_Impl::jobIndexLessThan));

    return ret;
  }

  void RunManager_Impl::raisePriority(const openstudio::runmanager::Job &t_job)
  {
    QMutexLocker lock(&m_mutex);

    // raisePriority for this parent job and all its children
    raisePriorityInternal(t_job);
  }

  void RunManager_Impl::raisePriorityInternal(const openstudio::runmanager::Job &t_job)
  {
    /* me */
    std::deque<openstudio::runmanager::Job>::iterator itr = std::find(m_queue.begin(), m_queue.end(), t_job);

    if (itr != m_queue.end() && itr != m_queue.begin())
    {
      std::deque<openstudio::runmanager::Job>::iterator toswap(itr);
      --toswap;

      int itrindex = itr->index();
      int toswapindex = toswap->index();

      itr->setIndex(toswapindex);
      toswap->setIndex(itrindex);

      std::swap(*itr, *toswap);
    }

    /* then children */


    std::vector<openstudio::runmanager::Job> vec = t_job.children();
    for (std::vector<openstudio::runmanager::Job>::const_iterator itr = vec.begin();
         itr != vec.end();
         ++itr)
    {
      raisePriorityInternal(*itr);
    }

    if (t_job.finishedJob())
    {
      raisePriorityInternal(*t_job.finishedJob());
    }

    //print_queue();
  }

  void RunManager_Impl::lowerPriority(const openstudio::runmanager::Job &t_job)
  {
    QMutexLocker lock(&m_mutex);
    lowerPriorityInternal(t_job);
  }

  void RunManager_Impl::lowerPriorityInternal(const openstudio::runmanager::Job &t_job)
  {
    /* children */
    if (t_job.finishedJob())
    {
      raisePriorityInternal(*t_job.finishedJob());
    }

    std::vector<openstudio::runmanager::Job> vec = t_job.children();
    std::vector<openstudio::runmanager::Job>::const_reverse_iterator vecend
      = vec.rend();
    for (std::vector<openstudio::runmanager::Job>::const_reverse_iterator itr = vec.rbegin();
         itr != vecend;
         ++itr)
    {
      raisePriorityInternal(*itr);
    }

    /* then me */
    std::deque<openstudio::runmanager::Job>::iterator itr = std::find(m_queue.begin(), m_queue.end(), t_job);

    if (itr != m_queue.end())
    {
      std::deque<openstudio::runmanager::Job>::iterator toswap(itr);
      ++toswap;
      if (toswap != m_queue.end())
      {
        int itrindex = itr->index();
        int toswapindex = toswap->index();

        itr->setIndex(toswapindex);
        toswap->setIndex(itrindex);

        std::swap(*itr, *toswap);
      }
    }

    //print_queue();
  }

  std::vector<openstudio::runmanager::Job> RunManager_Impl::getJobs() const
  {
    QMutexLocker lock(&m_mutex);
    return std::vector<openstudio::runmanager::Job>(m_queue.begin(), m_queue.end());
  }

  void RunManager_Impl::setConfigOptions(const ConfigOptions &co)
  {
    QMutexLocker lock(&m_mutex);
    m_dbholder->setConfigOptions(co);

//    if (!m_SLURMPassword.empty())
//    {
//      m_remoteProcessCreator->setConfiguration(
//        SSHCredentials(co.getSLURMHost(), co.getSLURMUserName(), m_SLURMPassword),
//        SLURMConfigOptions(co.getSLURMMaxTime(), co.getSLURMPartition(), co.getSLURMAccount()));
//    } else {
//      m_remoteProcessCreator->setConfiguration(
//       SSHCredentials(co.getSLURMHost(), co.getSLURMUserName()),
//        SLURMConfigOptions(co.getSLURMMaxTime(), co.getSLURMPartition(), co.getSLURMAccount()));
//    }

  }

  void RunManager_Impl::showConfigGui(QWidget *parent)
  {
    Configuration cf(parent,Qt::WindowFlags(Qt::Dialog | Qt::WindowTitleHint),m_dbholder->getConfigOptions());
    if (cf.exec() == QDialog::Accepted)
    {
      setConfigOptions(cf.getConfigOptions());
    }
  }


  void RunManager_Impl::raisePriority(const std::vector<Job> &t_jobs)
  {
    QMutexLocker lock(&m_mutex);

    if (!t_jobs.empty())
    {
      if (t_jobs.front().index() > 0)
      {
        for (std::vector<Job>::const_iterator itr = t_jobs.begin();
             itr != t_jobs.end();
             ++itr)
        {
          raisePriorityInternal(*itr);
        }
      }
    }
  }

  void RunManager_Impl::lowerPriority(const std::vector<Job> &t_jobs)
  {
    QMutexLocker lock(&m_mutex);

    if (!t_jobs.empty())
    {
      if (t_jobs.back().index() < static_cast<int>(m_queue.size()))
      {
        for (std::vector<Job>::const_reverse_iterator itr = t_jobs.rbegin();
             itr != t_jobs.rend();
             ++itr)
        {
          lowerPriorityInternal(*itr);
        }
      }
    }
  }


  ConfigOptions RunManager_Impl::getConfigOptions() const
  {
    return m_dbholder->getConfigOptions();
  }

  void RunManager_Impl::processQueue()
  {

    if (m_activate_mutex.tryLock())
    {
      // we are the only ones trying to test the slurmmanager for activation
      QMutexLocker lock(&m_mutex);
      const ConfigOptions config = getConfigOptions();
      const int maxremotejobs = config.getSLURMHost().empty()?0:config.getMaxSLURMJobs();
      bool paused = m_paused;

      if (maxremotejobs > 0 && !paused)
      {
        std::deque<runmanager::Job> jobs(m_queue);
        lock.unlock();

        bool remoterunnable = false;

        for (std::deque<runmanager::Job>::const_iterator itr = jobs.begin();
             itr != jobs.end();
             ++itr)
        {
          if (itr->remoteRunnable())
          {
            remoterunnable = true;
            break;
          }
        }

//        if (remoterunnable)
//        {
//          try {
//            m_remoteProcessCreator->activate(); // make sure we have an active connection
//          } catch (...) {
//          }
//        }
      }

      m_activate_mutex.unlock();
    }

    m_waitCondition.wakeAll();
  }

  std::map<std::string, double> RunManager_Impl::generateStatistics(const std::deque<runmanager::Job> &t_jobs)
  {
    int locallyrunningjobs = 0;
    int remotelyrunningjobs = 0;
    int runningjobs = 0;
    int runningworkflows = 0;
    int secondsjobsrunning = 0;
    int totaljobs = 0;
    int totalworkflows = 0;
    int completedjobs = 0;
    int totalwarnings = 0;
    int totalerrors = 0;
    int failedjobs = 0;
    int successfuljobs = 0;

    for (std::deque<runmanager::Job>::const_iterator itr = t_jobs.begin();
         itr != t_jobs.end();
         ++itr)
    {
      ++totaljobs;
      if (!itr->parent())
      {
        ++totalworkflows;
        if (itr->childrenRunning())
        {
          ++runningworkflows;
        }
      }


      if (itr->running())
      {
        if (itr->runningRemotely())
        {
          ++remotelyrunningjobs;
        } else {
          ++locallyrunningjobs;
        }

        ++runningjobs;
      } else {
        boost::optional<openstudio::DateTime> start = itr->startTime();
        boost::optional<openstudio::DateTime> end = itr->endTime();
        if (end && start
            && *end >= *start)
        {
          ++completedjobs;

          openstudio::runmanager::JobErrors errors = itr->errors();
          if (errors.result != ruleset::OSResultValue::Fail)
          {
            ++successfuljobs;
          } else {
            ++failedjobs;
          }

          totalerrors += errors.errors().size();
          totalwarnings += errors.warnings().size();

          secondsjobsrunning += (*end - *start).totalSeconds();
        }
      }
    }

    std::map<std::string, double> stats;
    stats["Total Errors"] = totalerrors;
    stats["Total Warnings"] = totalwarnings;
    stats["Number of Jobs"] = totaljobs;
    stats["Number of Workflows"] = totalworkflows;
    stats["Running Jobs"] = runningjobs;
    stats["Running Workflows"] = runningworkflows;
    stats["Average Time to Complete Job"] = double(secondsjobsrunning) / double(completedjobs);
    stats["Completed Jobs"] = completedjobs;
    stats["Failed Jobs"] = failedjobs;
    stats["Successful Jobs"] = successfuljobs;
    stats["Locally Running Jobs"] = locallyrunningjobs;
    stats["Remotely Running Jobs"] = remotelyrunningjobs;

    return stats;
  }

  std::map<std::string, double> RunManager_Impl::statistics() const
  {
    QMutexLocker lock(&m_mutex);
    return m_statistics;
  }

//  void RunManager_Impl::setSLURMPassword(const std::string &t_pass)
//  {
//    m_SLURMPassword = t_pass;
//    ConfigOptions co = m_dbholder->getConfigOptions();
//    if (!m_SLURMPassword.empty())
//    {
//      m_remoteProcessCreator->setConfiguration(
//        SSHCredentials(co.getSLURMHost(), co.getSLURMUserName(), m_SLURMPassword),
//        SLURMConfigOptions(co.getSLURMMaxTime(), co.getSLURMPartition(), co.getSLURMAccount()));
//    } else {
//      m_remoteProcessCreator->setConfiguration(
//        SSHCredentials(co.getSLURMHost(), co.getSLURMUserName()),
//        SLURMConfigOptions(co.getSLURMMaxTime(), co.getSLURMPartition(), co.getSLURMAccount()));
//    }
//  }

  std::string RunManager_Impl::persistWorkflow(const Workflow &t_wf)
  {
    return m_dbholder->persistWorkflow(t_wf);
  }

  std::vector<Workflow> RunManager_Impl::loadWorkflows()
  {
    return m_dbholder->loadWorkflows();
  }

  Workflow RunManager_Impl::loadWorkflow(const std::string &key)
  {
    return m_dbholder->loadWorkflow(key);
  }

  Workflow RunManager_Impl::loadWorkflowByName(const std::string &t_name) const
  {
    return m_dbholder->loadWorkflowByName(t_name);
  }

  void RunManager_Impl::deleteWorkflowByName(const std::string &t_name)
  {
    m_dbholder->deleteWorkflowByName(t_name);
  }


  void RunManager_Impl::deleteWorkflows()
  {
    m_dbholder->deleteWorkflows();
  }

  void RunManager_Impl::setContinue(bool t_cont)
  {
    QMutexLocker lock(&m_mutex);
    m_continue = t_cont;
  }

  bool RunManager_Impl::getContinue() const
  {
    QMutexLocker lock(&m_mutex);
    return m_continue;
  }

  void RunManager_Impl::loadJobs(const openstudio::path &t_path)
  {
    DBHolder db(t_path);

    std::vector<openstudio::runmanager::Job> jobs = db.loadJobs();

    for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
         itr != jobs.end();
         ++itr)
    {
      // add top level jobs, children will get added automatically
      if (!itr->parent())
      {
        enqueue(*itr, true, t_path.parent_path());
      }
    }
  }

  void RunManager_Impl::updateJob(const Job &t_job)
  {
    try {
      Job j = getJob(t_job.uuid());
      m_dbholder->deleteJobTree(t_job);
      j.updateJob(t_job, false);
      m_dbholder->persistJobTree(t_job);
    } catch (const std::out_of_range &) {
      // job didn't exist
      enqueue(t_job, true, m_dbfile.parent_path());
    }

    openstudio::Application::instance().processEvents();
  }

  /// update job tree, and be willing to change the UUID in the process
  void RunManager_Impl::updateJob(const openstudio::UUID &t_uuid, const Job &t_job)
  {

    try {
      Job j = getJob(t_uuid);
      m_dbholder->deleteJobTree(t_job);
      j.updateJob(t_job, true); // update the old one to have the features of the new one
      m_dbholder->persistJobTree(t_job);
    } catch (const std::out_of_range &) {
      // uuid didn't exist, we want to throw this back out
      throw;
    }
  }


  void RunManager_Impl::updateJob(const Job &t_job, const openstudio::path &t_path)
  {
    try {
      Job j = getJob(t_job.uuid());
      m_dbholder->deleteJobTree(t_job);
      j.updateJob(t_job, false);      
      m_dbholder->persistJobTree(t_job);
      j.setBasePathRecursive(t_path);
    } catch (const std::out_of_range &) {
      // job didn't exist
      enqueue(t_job, true, t_path);
    }

    openstudio::Application::instance().processEvents(1);
  }

  void RunManager_Impl::updateJobs(const std::vector<Job> &t_jobTrees)
  {
    LOG(Info, "Updating jobs: " << t_jobTrees.size());
    for (std::vector<Job>::const_iterator itr = t_jobTrees.begin();
         itr != t_jobTrees.end();
         ++itr)
    {
      updateJob(*itr);
    }
  }

  void RunManager_Impl::clearJobs()
  {
    QMutexLocker lock(&m_mutex);
    m_workflowkeys.clear();
    m_jobuuids.clear();

    std::deque<Job> q = m_queue;
    m_queue.clear();
    for (std::deque<Job>::iterator itr = q.begin();
         itr != q.end();
         ++itr)
    {
      itr->setCanceled(true);
    }

    for (std::deque<Job>::iterator itr = q.begin();
         itr != q.end();
         ++itr)
    {
      itr->requestStop();
    }

    m_dbholder->deleteJobs(q.begin(), q.end());
    m_model.removeRows(0, m_model.rowCount());
    lock.unlock();

    for (std::deque<Job>::iterator itr = q.begin();
         itr != q.end();
         ++itr)
    {
      itr->waitForFinished();
    }
    openstudio::Application::instance().processEvents(1);
  }



  void RunManager_Impl::run()
  {
    while (getContinue())
    {
      //openstudio::Application::instance().processEvents(1);
      QMutexLocker lock(&m_mutex);

      if (m_continue) // need to check m_continue while inside of the lock
      {
        // If we are to continue, we can safely wait on the condition
        m_waitCondition.wait(&m_mutex);
      } else {
        continue;
      }

      //LOG(Info, boost::posix_time::microsec_clock::local_time() << " kicking off new jobs ");

      bool statschanged = false;
      std::map<std::string, double> oldstats = m_statistics;

      if (!m_paused && !m_processingQueue && m_continue)
      {
        std::deque<openstudio::runmanager::Job> queue(m_queue);


        lock.unlock();

        m_processingQueue = true;


        int running = std::count_if(queue.begin(), queue.end(), boost::bind(&openstudio::runmanager::Job::running, _1));
        int runningRemotely
          = std::count_if(queue.begin(), queue.end(), boost::bind(&openstudio::runmanager::Job::runningRemotely, _1));
        int runningLocally = running - runningRemotely;

        //LOG(Info, boost::posix_time::microsec_clock::local_time() << " kicking off new jobs runningremotely: " << runningRemotely << " runningLocally " << runningLocally);

        std::deque<Job>::iterator itr = queue.begin();
        std::deque<Job>::iterator end = queue.end();


        ConfigOptions config = getConfigOptions();

        const int maxremotejobs = config.getSLURMHost().empty()?0:config.getMaxSLURMJobs();
        const int maxlocaljobs = config.getMaxLocalJobs();

        // Make sure we have as many running as we should have
        while ((runningLocally < maxlocaljobs || runningRemotely < maxremotejobs) && itr != end)
        {
          boost::optional<Job> parent = itr->parent();

          if (itr->runnable())
          {
//            if (runningRemotely < maxremotejobs && m_remoteProcessCreator->hasConnection() && itr->remoteRunnable())
//            {
//              LOG(Info, "Starting job remotely: " << toString(itr->uuid()) << " " << itr->description() );
//              itr->start(m_remoteProcessCreator);
//              ++runningRemotely;
//            } else if (runningLocally < maxlocaljobs) {
            if (runningLocally < maxlocaljobs) {
              LOG(Info, "Starting job locally: " << toString(itr->uuid()) << " " << itr->description() );
              itr->start(m_localProcessCreator);
              ++runningLocally;
            }
          }

          ++itr;
        }


        if ((running != m_lastRunning
             || runningRemotely != m_lastRunningRemotely
             || runningLocally != m_lastRunningLocally)
            && m_lastStatistics.addSecs(1) < QDateTime::currentDateTime())
        {
          std::map<std::string, double> stats = generateStatistics(queue);


          if (stats != oldstats)
          {
            statschanged = true;
          }

          m_lastRunning = running;
          m_lastRunningRemotely = runningRemotely;
          m_lastRunningLocally = runningLocally;
          m_lastStatistics = QDateTime::currentDateTime();

          lock.relock();
          m_statistics = stats;
          lock.unlock();
        }


        m_processingQueue = false;

        
        //LOG(Info, boost::posix_time::microsec_clock::local_time() << " done kicking off new jobs runningremotely: " << runningRemotely << " runningLocally " << runningLocally);

      } else {
        std::deque<openstudio::runmanager::Job> queue(m_queue);

        lock.unlock();

        std::map<std::string, double> stats = generateStatistics(queue);

        if (stats != oldstats)
        {
          statschanged = true;
        }


        lock.relock();
        m_statistics = stats;
        lock.unlock();
      }

      if (statschanged)
      {
        emit statsChanged();
      }

    }
  }

}
}
}

