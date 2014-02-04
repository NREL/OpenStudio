#include "RunManagerWatcher.hpp"
#include <utilities/time/DateTime.hpp>
#include <utilities/core/Assert.hpp>

#include "JobErrors.hpp"
#include "FileInfo.hpp"
#include "Job.hpp"
#include "AdvancedStatus.hpp"

namespace openstudio {
namespace runmanager {

  RunManagerWatcher::RunManagerWatcher(const RunManager &t_runManager)
    : m_runManager(t_runManager)
  {
    bool connected = m_runManager.connect(SIGNAL(pausedChanged(bool)), this, SLOT(pausedChanged(bool)));
    OS_ASSERT(connected);

    connected = m_runManager.connect(SIGNAL(jobTreeAdded(const openstudio::runmanager::Job &t_job)), 
        this, SLOT(jobTreeAdded(const openstudio::runmanager::Job &t_job)));
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
    bool connected = t_job.connect(SIGNAL(started(const openstudio::UUID &)), this, SLOT(started(openstudio::UUID &)));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(finished(const openstudio::UUID &, const openstudio::runmanager::JobErrors&)), 
        this, SLOT(finished(const started(openstudio::UUID &, const openstudio::runmanager::JobErrors&))));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(finishedExt(const openstudio::UUID &, const openstudio::runmanager::JobErrors&, const openstudio::DateTime &, const std::vector<openstudio::runmanager::FileInfo> &)), 
        this, SLOT(finishedExt(const started(openstudio::UUID &, const openstudio::runmanager::JobErrors&, const openstudio::DateTime &, const std::vector<openstudio::runmanager::FileInfo> &))));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(outputFileChanged(const openstudio::UUID &id, const openstudio::runmanager::FileInfo& file)), 
        this, SLOT(outputFileChanged(const openstudio::UUID &id, const openstudio::runmanager::FileInfo& file)));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(stateChanged(const openstudio::UUID &)), this, SLOT(stateChanged(openstudio::UUID &)));
    OS_ASSERT(connected);

    connected = t_job.connect(SIGNAL(outputDataAdded(const openstudio::UUID &, const std::string &)), 
        this, SLOT(outputDataAdded(openstudio::UUID &, const std::string &)));
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
    jobFinishedExt(id, errors, lastRun, outputfiles);
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

  void RunManagerWatcher::jobTreeAdded(const openstudio::runmanager::Job &t_job)
  {
    runManagerJobTreeAddedInternal(t_job);
  }

}
}
