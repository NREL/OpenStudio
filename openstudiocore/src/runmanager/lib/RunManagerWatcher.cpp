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

#include "RunManagerWatcher.hpp"
#include "../../utilities/time/DateTime.hpp"
#include "../../utilities/core/Assert.hpp"

#include "JobErrors.hpp"
#include "FileInfo.hpp"
#include "Job.hpp"
#include "AdvancedStatus.hpp"
#include "RubyJobUtils.hpp"
#include "MergedJobResults.hpp"

namespace openstudio {
namespace runmanager {

  RunManagerWatcher::RunManagerWatcher(const RunManager &t_runManager)
    : m_runManager(t_runManager)
  {
    bool connected = m_runManager.connect(SIGNAL(pausedChanged(bool)), this, SLOT(pausedChanged(bool)));
    OS_ASSERT(connected);

    connected = m_runManager.connect(SIGNAL(jobTreeAdded(const openstudio::UUID &)), 
        this, SLOT(jobTreeAdded(const openstudio::UUID &)));
    OS_ASSERT(connected);

    connected = m_runManager.connect(SIGNAL(statsChanged()), this, SLOT(statsChanged()));
    OS_ASSERT(connected);

    std::vector<openstudio::runmanager::Job> jobs = m_runManager.getJobs();

    for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
         itr != jobs.end();
         ++itr)
    {
      if (!itr->parent())
      {
        hookUpSignals(*itr);
      }
    }
  }

  Job RunManagerWatcher::getJob(const openstudio::UUID &t_uuid)
  {
    return m_runManager.getJob(t_uuid);
  }

  void RunManagerWatcher::hookUpSignals(const openstudio::runmanager::Job &t_job)
  {
    bool connected = t_job.connect(SIGNAL(started(const openstudio::UUID &)), this, SLOT(started(const openstudio::UUID &)));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(finished(const openstudio::UUID &, const openstudio::runmanager::JobErrors&)), 
        this, SLOT(finished(const openstudio::UUID &, const openstudio::runmanager::JobErrors&)));
    OS_ASSERT(connected);

    connected = t_job.connect(
            SIGNAL(finishedExt(const openstudio::UUID &, const openstudio::runmanager::JobErrors&, const openstudio::DateTime &, const std::vector<openstudio::runmanager::FileInfo> &)), 
        this, SLOT(finishedExt(const openstudio::UUID &, const openstudio::runmanager::JobErrors&, const openstudio::DateTime &, const std::vector<openstudio::runmanager::FileInfo> &)));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(outputFileChanged(const openstudio::UUID &, const openstudio::runmanager::FileInfo&)), 
        this, SLOT(outputFileChanged(const openstudio::UUID &, const openstudio::runmanager::FileInfo&)));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(stateChanged(const openstudio::UUID &)), this, SLOT(stateChanged(const openstudio::UUID &)));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(outputDataAdded(const openstudio::UUID &, const std::string &)), 
        this, SLOT(outputDataAdded(const openstudio::UUID &, const std::string &)));
    OS_ASSERT(connected);


    connected = t_job.connect(SIGNAL(statusChanged(const openstudio::runmanager::AdvancedStatus &)), 
        this, SLOT(statusChanged(const openstudio::runmanager::AdvancedStatus &)));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(childrenChanged(const openstudio::UUID &)), this, SLOT(childrenChanged(const openstudio::UUID &)));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(parentChanged(const openstudio::UUID &)), this, SLOT(parentChanged(const openstudio::UUID &)));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(treeChanged(const openstudio::UUID &)), this, SLOT(treeChanged(const openstudio::UUID &)));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(uuidChanged(const openstudio::UUID &, const openstudio::UUID &)), this, SLOT(uuidChanged(const openstudio::UUID &, const openstudio::UUID &)));
    OS_ASSERT(connected);

    std::vector<openstudio::runmanager::Job> children = t_job.children();

    for (std::vector<openstudio::runmanager::Job>::const_iterator itr = children.begin();
         itr != children.end();
         ++itr)
    {
      hookUpSignals(*itr);
    }

    if (t_job.finishedJob())
    {
      hookUpSignals(*(t_job.finishedJob()));
    }
  }

  void RunManagerWatcher::runManagerJobTreeAddedInternal(const openstudio::runmanager::Job &t_job)
  {
    hookUpSignals(t_job);
    runManagerJobTreeAdded(t_job);
  }


  void RunManagerWatcher::jobFinishedExtInternal(const openstudio::UUID &t_id, const openstudio::runmanager::JobErrors& t_errors, 
      const openstudio::DateTime &t_lastRun, const std::vector<openstudio::runmanager::FileInfo> &t_outputfiles)
  {
    jobFinishedExt(t_id, t_errors, t_lastRun, t_outputfiles);

    try {
      Job job = m_runManager.getJob(t_id);

      boost::optional<openstudio::runmanager::Job> parent = job;
      while (parent->parent())
      {
        parent = parent->parent();
      }

      TreeStatusEnum status = parent->treeStatus();
      if (status == TreeStatusEnum::Finished || status == TreeStatusEnum::Failed || status == TreeStatusEnum::Canceled)
      {
        LOG(Debug, "Calling treeFinished");
        treeFinished(*parent);
      }

      if (job.hasMergedJobs())
      {
        std::vector<MergedJobResults> mergedJobs = job.mergedJobResults();
        openstudio::runmanager::JobType jobtype = job.jobType();
        openstudio::DateTime lastRun = t_lastRun;
        openstudio::UUID mergedIntoJobId = t_id;
        JobParams inputParams = job.params();

        std::vector<JobParams> params;

        if (jobtype == JobType::Ruby)
        {
          RubyJobBuilder rjb(inputParams);
          std::vector<RubyJobBuilder> mergedRubyJobs = rjb.mergedJobs();
          OS_ASSERT(mergedRubyJobs.size() + 1 == mergedJobs.size());
          params.push_back(inputParams);

          for (std::vector<RubyJobBuilder>::const_iterator itr = mergedRubyJobs.begin();
               itr != mergedRubyJobs.end();
               ++itr)
          {
            params.push_back(itr->toParams());
          }

        }

        OS_ASSERT(params.empty() || params.size() == mergedJobs.size());

        for (size_t pos = 0; pos < mergedJobs.size(); ++pos)
        {
          openstudio::UUID jobId = mergedJobs[pos].uuid;
          JobErrors errors = mergedJobs[pos].errors;
          Files outputFiles = mergedJobs[pos].outputFiles;

          bool isMergedJob = true;

          jobFinishedDetails(jobId, jobtype, lastRun, errors, outputFiles, params.empty()?inputParams:params.at(pos),
              isMergedJob, mergedIntoJobId);
        }
      } else {
        // no merged jobs
        jobFinishedDetails(t_id, job.jobType(), t_lastRun, t_errors, Files(job.outputFiles()), JobParams(job.params()),
            false, openstudio::UUID());
      }
    } catch (const std::runtime_error &e) {
      LOG(Error, "Error extracting job finished details " << e.what());
    }
  }

  void RunManagerWatcher::started(const openstudio::UUID &id)
  {
    jobStarted(id);
  }

  void RunManagerWatcher::finished(const openstudio::UUID &id, const openstudio::runmanager::JobErrors& errors)
  {
    jobFinished(id, errors);
  }

  void RunManagerWatcher::finishedExt(const openstudio::UUID &id, const openstudio::runmanager::JobErrors& errors, 
      const openstudio::DateTime &lastRun, const std::vector<openstudio::runmanager::FileInfo> &outputfiles)
  {
    jobFinishedExtInternal(id, errors, lastRun, outputfiles);
  }

  void RunManagerWatcher::outputFileChanged(const openstudio::UUID &id, const openstudio::runmanager::FileInfo& file)
  {
    jobOutputFileChanged(id, file);
  }

  void RunManagerWatcher::stateChanged(const openstudio::UUID &id)
  {
    jobStateChanged(id);
  }

  void RunManagerWatcher::outputDataAdded(const openstudio::UUID &id, const std::string &data)
  {
    jobOutputDataAdded(id, data);
  }

  void RunManagerWatcher::statusChanged(const openstudio::runmanager::AdvancedStatus &status)
  {
    jobStatusChanged(status);
  }

  void RunManagerWatcher::childrenChanged(const openstudio::UUID &id)
  {
    jobChildrenChanged(id);
  }

  void RunManagerWatcher::parentChanged(const openstudio::UUID &id)
  {
    jobParentChanged(id);
  }

  void RunManagerWatcher::treeChanged(const openstudio::UUID &id)
  {
    jobTreeChanged(id);
  }

  void RunManagerWatcher::uuidChanged(const openstudio::UUID &t_oldUUID, const openstudio::UUID &t_newUUID)
  {
    jobUuidChanged(t_oldUUID, t_newUUID);
  }

  void RunManagerWatcher::pausedChanged(bool t_paused)
  {
    runManagerPausedChanged(t_paused);
  }

  void RunManagerWatcher::statsChanged()
  {
    runManagerStatsChanged();
  }

  void RunManagerWatcher::jobTreeAdded(const openstudio::UUID &id)
  {
    runManagerJobTreeAddedInternal(m_runManager.getJob(id));
  }


}
}
