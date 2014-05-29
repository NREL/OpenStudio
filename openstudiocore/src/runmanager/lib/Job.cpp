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

#include "../../utilities/core/UUID.hpp"
#include "../../utilities/core/System.hpp"
#include "Job.hpp"
#include "Job_Impl.hpp"
#include "JSON.hpp"
#include "MergedJobResults.hpp"

namespace openstudio {
namespace runmanager {


  Job::Job(const std::shared_ptr<detail::Job_Impl> &t_impl)
    : m_impl(t_impl)
  {

  }

  Job::Job(const Job &temp)
    : m_impl(temp.m_impl)
  {
  }

  int Job::index() const
  {
    return m_impl->index();
  }

  void Job::setIndex(int index)
  {
    m_impl->setIndex(index);
  }

  JobParams Job::allParams() const
  {
    return m_impl->allParams();
  }

  Files Job::relativeOutputFiles() const
  {
    return m_impl->relativeOutputFiles();
  }

  UUID Job::uuid() const 
  {
    return m_impl->uuid();
  }

  JobType Job::jobType() const
  {
    return m_impl->jobType();
  }

  void Job::waitForFinished(int t_msecs)
  {
    if (t_msecs < 0)
    {
      t_msecs = INT_MAX;
    }

    boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();

    while ( m_impl->isRunning() 
            && (boost::posix_time::microsec_clock::universal_time() - start).total_milliseconds() < t_msecs)
    {
      openstudio::System::msleep(1);
    }
  }

  bool Job::running() const
  {
    return m_impl->isRunning();
  }

  bool Job::runningRemotely() const
  {
    return m_impl->runningRemotely();
  }

  bool Job::outOfDate() const
  {
    return m_impl->outOfDate();
  }

  bool Job::canceled() const
  {
    return m_impl->canceled();
  }

  void Job::setRunnable(bool force)
  {
    m_impl->setRunnable(force);
  }

  bool Job::remoteRunnable() const
  {
    return m_impl->remoteRunnable();
  }

  bool Job::runnable() const
  {
    return m_impl->runnable();
  }

  void Job::setCanceled(bool t_canceled) 
  {
    m_impl->setCanceled(t_canceled);
  }

  boost::optional<openstudio::DateTime> Job::startTime() const
  {
    return m_impl->osStartTime();
  }

  boost::optional<openstudio::DateTime> Job::endTime() const
  {
    return m_impl->osEndTime();
  }


  JobErrors Job::errors() const
  {
    return m_impl->errors();
  }

  AdvancedStatus Job::status() const
  {
    return m_impl->status();
  }


  void Job::showErrors() const
  {
    m_impl->showErrors();
  }

  void Job::showTreeErrors() const
  {
    m_impl->showTreeErrors();
  }

  void Job::setTreeRunnable(bool force)
  {
    m_impl->setTreeRunnable(force);
  }

  void Job::setTreeCanceled(bool t_canceled)
  {
    m_impl->setTreeCanceled(t_canceled);
  }

  void Job::cleanup()
  {
    m_impl->cleanup();
  }

  std::string Job::description() const
  {
    return m_impl->description();
  }

  boost::optional<openstudio::DateTime> Job::lastRun() const
  {
    boost::optional<QDateTime> qdt(m_impl->lastRun());
    if (qdt)
    {
      return boost::optional<openstudio::DateTime>(openstudio::toDateTime(*qdt));
    } else {
      return boost::optional<openstudio::DateTime>();
    }
  }

  boost::optional<openstudio::DateTime> Job::childrenLastRun() const
  {
    boost::optional<QDateTime> qdt(m_impl->childrenLastRun());
    if (qdt)
    {
      return boost::optional<openstudio::DateTime>(openstudio::toDateTime(*qdt));
    } else {
      return boost::optional<openstudio::DateTime>();
    }
  }

  bool Job::childrenRunning() const
  {
    return m_impl->childrenRunning();
  }

  bool Job::childrenTreesRunning() const
  {
    return m_impl->childrenTreesRunning();
  }

  std::vector<FileInfo> Job::rawInputFiles() const
  {
    return m_impl->rawInputFiles().files();
  }

  std::vector<FileInfo> Job::inputFiles() const
  {
    return m_impl->inputFiles().files();
  }

  std::vector<ToolInfo> Job::tools() const
  {
    return m_impl->tools().tools();
  }

  std::vector<JobParam> Job::params() const
  {
    return m_impl->params().params();
  }

  JobParams Job::jobParams() const
  {
    return m_impl->params();
  }

  std::string Job::detailedDescription() const
  {
    return m_impl->detailedDescription();
  }

  Files Job::allInputFiles() const
  {
    return m_impl->allInputFiles();
  }

  Files Job::allFiles() const
  {
    return m_impl->allFiles();
  }

  std::vector<FileInfo> Job::outputFiles() const
  {
    return m_impl->outputFiles().files();
  }

  std::string Job::getOutput() const
  {
    return m_impl->getOutput();
  }

  void Job::start(const std::shared_ptr<ProcessCreator> &t_pc)
  {
    m_impl->setProcessCreator(t_pc);
    m_impl->start();
  }

  void Job::start(const std::shared_ptr<ProcessCreator> &t_pc, int t_remoteid, int t_remoteTaskId)
  {
    m_impl->setProcessCreator(t_pc);
    m_impl->setRemoteId(t_remoteid, t_remoteTaskId);
    m_impl->start();
  }

  bool Job::connect(const char *signal, 
      const QObject *receiver, const char *method, Qt::ConnectionType type) const
  {
    return QObject::connect(m_impl.get(), signal, receiver, method, type);
  }

  bool Job::disconnect(const char *signal, const QObject *receiver, const char *method)
  {
    return m_impl->disconnect(signal, receiver, method);
  }

  void Job::addChild(const Job &t_job)
  {
    detail::Job_Impl::addChild(m_impl, t_job.m_impl);
  }

  bool Job::treeRunning() const
  {
    return m_impl->treeRunning();
  }

  bool Job::treeOutOfDate() const
  {
    return m_impl->treeOutOfDate();
  }

  JobErrors Job::treeErrors() const
  {
    return m_impl->treeErrors();
  }

  std::string Job::treeDescription() const
  {
    return m_impl->treeDescription();
  }

  TreeStatusEnum Job::treeStatus() const
  {
    return m_impl->treeStatus();
  }

  std::vector<std::string> Job::treeDetailedDescription() const
  {
    return m_impl->treeDetailedDescription();
  }

  boost::optional<openstudio::DateTime> Job::treeLastRun() const
  {
    boost::optional<QDateTime> qdt(m_impl->treeLastRun());
    if (qdt)
    {
      return boost::optional<openstudio::DateTime>(openstudio::toDateTime(*qdt));
    } else {
      return boost::optional<openstudio::DateTime>();
    }
  }

  bool Job::ranBefore(const openstudio::runmanager::Job &t_rhs) const
  {
    boost::optional<QDateTime> lhs(m_impl->lastRun());
    boost::optional<QDateTime> rhs(t_rhs.m_impl->lastRun());

    if (lhs && rhs)
    {
      return lhs < rhs;
    } else if (lhs) {
      return true;
    } else {
      return false;
    }
  }

  Files Job::treeAllFiles() const
  {
    return m_impl->treeAllFiles();
  }

  Files Job::treeOutputFiles() const
  {
    return m_impl->treeOutputFiles();
  }

  bool Job::removeChild(const Job &t_job)
  {
    return m_impl->removeChild(t_job.m_impl);
  }

  std::vector<Job> Job::children() const
  {
    std::vector<Job> ret;

    std::vector<std::shared_ptr<detail::Job_Impl> > childvec = 
      m_impl->children();

    for (std::vector<std::shared_ptr<detail::Job_Impl> >::const_iterator itr = childvec.begin();
         itr != childvec.end();
         ++itr)
    {
      ret.push_back(Job(*itr));
    }

    return ret;
  }

  bool Job::childrenOutOfDate() const
  {
    return m_impl->childrenOutOfDate();
  }

  bool Job::childrenTreesOutOfDate() const
  {
    return m_impl->childrenTreesOutOfDate();
  }

  boost::optional<Job> Job::finishedJob() const
  {
    std::shared_ptr<detail::Job_Impl> fj = 
      m_impl->finishedJob();

    if (fj)
    {
      return Job(fj);
    } else {
      return boost::optional<Job>();
    }
  }

  void Job::setFinishedJob(const Job &t_job)
  {
    detail::Job_Impl::setFinishedJob(m_impl, t_job.m_impl);
  }

  boost::optional<Job> Job::parent() const
  {
    std::shared_ptr<detail::Job_Impl> p = 
      m_impl->parent();

    if (p)
    {
      return Job(p); 
    } else {
      return boost::optional<Job>();
    }
  }

  openstudio::path Job::outdir() const
  {
    return m_impl->outdir();
  }

  openstudio::path Job::getBasePath() const
  {
    return m_impl->getBasePath();
  }

  void Job::setBasePath(const openstudio::path &t_basePath)
  {
    m_impl->setBasePath(t_basePath);
  }

  void Job::setBasePathRecursive(const openstudio::path &t_basePath)
  {
    m_impl->setBasePathRecursive(t_basePath);
  }

  bool Job::operator==(const Job &rhs) const
  {
    return m_impl->uuid() == rhs.m_impl->uuid();
  }

  bool Job::operator<(const Job &rhs) const
  {
    return m_impl->uuid() < rhs.m_impl->uuid();
  }

  Job &Job::operator=(Job temp) 
  {
    swap(temp);
    return *this;
  }
    
  void Job::mergeJob(const openstudio::runmanager::Job &t_job)
  {
    m_impl->mergeJob(m_impl, t_job.m_impl);
  }

  void Job::swap(Job &rhs)
  {
    m_impl.swap(rhs.m_impl);
  }
      
  void Job::moveToThread(QThread* targetThread)
  {
    m_impl->moveToThread(targetThread);
  }

  void Job::requestStop()
  {
    m_impl->requestStop();
  }


  std::vector<std::pair<boost::posix_time::ptime, AdvancedStatus> > Job::history() const
  {
    return m_impl->history();
  }


  Job Job::fromJSON(const std::string &t_json)
  {
    return detail::JSON::toJob(t_json, false);
  }

  std::string Job::toJSON() const
  {
    return detail::JSON::toJSON(*this);
  }

  void Job::updateJob(const Job &t_other, bool t_allowUUIDUpdate)
  {
    m_impl->updateJob(t_other.m_impl, t_allowUUIDUpdate);
  }

  bool Job::externallyManaged() const
  {
    return m_impl->externallyManaged();
  }

  void Job::sendSignals()
  {
    return m_impl->sendSignals();
  }


  void Job::makeExternallyManaged()
  {
    m_impl->makeExternallyManaged();
  }

  void Job::setStatus(const AdvancedStatus &t_status)
  {
    m_impl->setStatus(t_status);
  }

  bool Job::hasMergedJobs() const
  {
    return m_impl->hasMergedJobs();
  }

  std::vector<MergedJobResults> Job::mergedJobResults() const
  {
    return m_impl->mergedJobResults();
  }


}
}

