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

#include "CurrentAnalysis.hpp"
#include "CurrentAnalysis_Impl.hpp"

#include "../analysis/DataPoint_Impl.hpp"

#include "../runmanager/lib/RunManager.hpp"
#include "../runmanager/lib/Job.hpp"
#include "../runmanager/lib/JobErrors.hpp"

using namespace openstudio::analysis;

namespace openstudio {
namespace analysisdriver {

namespace detail {

  bool jobUUIDsEqual(const analysis::DataPoint& dataPoint, const openstudio::UUID& job) {
    boost::optional<runmanager::Job> topLevelJob = dataPoint.topLevelJob();
    if (topLevelJob){
      return (topLevelJob->uuid() == job);
    }
    return false;
  }

  bool dataPointsEqual(const analysis::DataPoint& dataPoint1, const analysis::DataPoint& dataPoint2) {
    return dataPoint1 == dataPoint2;
  }

  CurrentAnalysis_Impl::CurrentAnalysis_Impl(const analysis::Analysis& analysis,
                                             const AnalysisRunOptions& runOptions)
    : m_analysis(analysis),
      m_runOptions(runOptions),
      m_numOSJobsInIteration(0),
      m_numOSJobsComplete(0),
      m_numOSJobsFailed(0),
      m_dakotaStarted(false)
  {}

  analysis::Analysis CurrentAnalysis_Impl::analysis() const {
    return m_analysis;
  }

  AnalysisRunOptions CurrentAnalysis_Impl::runOptions() const {
    return m_runOptions;
  }

  CurrentAnalysis CurrentAnalysis_Impl::currentAnalysis() const {
    CurrentAnalysis result(std::const_pointer_cast<CurrentAnalysis_Impl>(shared_from_this()));
    return result;
  }

  int CurrentAnalysis_Impl::numQueuedJobs() const {
    return numQueuedOSJobs() + numQueuedDakotaJobs();
  }

  int CurrentAnalysis_Impl::numQueuedOSJobs() const {
    return m_queuedOSDataPoints.size();
  }

  int CurrentAnalysis_Impl::numQueuedDakotaJobs() const {
    return m_queuedDakotaDataPoints.size();
  }

  int CurrentAnalysis_Impl::numCompletedJobsInOSIteration() const {
    return m_numOSJobsComplete;
  }

  int CurrentAnalysis_Impl::numFailedJobsInOSIteration() const {
    return m_numOSJobsFailed;
  }

  int CurrentAnalysis_Impl::totalNumJobsInOSIteration() const {
    return m_numOSJobsInIteration;
  }

  boost::optional<runmanager::JobErrors> CurrentAnalysis_Impl::dakotaJobErrors() const {
    return m_dakotaJobErrors;
  }

  bool CurrentAnalysis_Impl::connect(const std::string& signal,
                                     const QObject* qObject,
                                     const std::string& slot,
                                     Qt::ConnectionType type) const
  {
    return QObject::connect(this, signal.c_str(), qObject, slot.c_str(), type);
  }

  void CurrentAnalysis_Impl::setNumOSJobsInIteration(int numJobs) {
    OS_ASSERT(m_numOSJobsInIteration == m_numOSJobsComplete);
    OS_ASSERT(m_queuedOSDataPoints.size() == 0u);

    m_numOSJobsInIteration = numJobs;
    m_numOSJobsComplete = 0;
    m_numOSJobsFailed = 0;
    emit iterationProgress(numCompletedJobsInOSIteration(),totalNumJobsInOSIteration());
  }

  bool CurrentAnalysis_Impl::dakotaStarted() const {
    return m_dakotaStarted;
  }

  boost::optional<openstudio::UUID> CurrentAnalysis_Impl::dakotaJob() const {
    return m_dakotaJob;
  }

  void CurrentAnalysis_Impl::setDakotaJob(const openstudio::UUID& dakotaJob) {
    m_dakotaJob = dakotaJob;
    m_dakotaStarted = true;
  }

  void CurrentAnalysis_Impl::clearDakotaJob(const runmanager::JobErrors& dakotaJobErrors) {
    m_dakotaJob.reset();
    m_dakotaJobErrors = dakotaJobErrors;
  }

  bool CurrentAnalysis_Impl::isQueuedDataPoint(const openstudio::UUID& job) const {
    return isQueuedOSDataPoint(job) || isQueuedDakotaDataPoint(job);
  }

  bool CurrentAnalysis_Impl::isQueuedOSDataPoint(const openstudio::UUID& job) const {
    auto it = std::find_if(m_queuedOSDataPoints.begin(),
                           m_queuedOSDataPoints.end(),
                           std::bind(jobUUIDsEqual,std::placeholders::_1,job));
    return (it != m_queuedOSDataPoints.end());
  }

  bool CurrentAnalysis_Impl::isQueuedDakotaDataPoint(const openstudio::UUID& job) const {
    auto it = std::find_if(m_queuedDakotaDataPoints.begin(),
                           m_queuedDakotaDataPoints.end(),
                           std::bind(jobUUIDsEqual,std::placeholders::_1,job));
    return (it != m_queuedDakotaDataPoints.end());
  }

  boost::optional<analysis::DataPoint> CurrentAnalysis_Impl::isQueuedDataPoint(
      const analysis::DataPoint& dataPoint) const
  {
    analysis::OptionalDataPoint result = isQueuedOSDataPoint(dataPoint);
    if (!result) {
      result = isQueuedDakotaDataPoint(dataPoint);
    }
    return result;
  }

  boost::optional<analysis::DataPoint> CurrentAnalysis_Impl::isQueuedOSDataPoint(
      const analysis::DataPoint& dataPoint) const
  {
    auto it = std::find_if(m_queuedOSDataPoints.begin(),
                           m_queuedOSDataPoints.end(),
                           std::bind(dataPointsEqual,std::placeholders::_1,dataPoint));
    if (it != m_queuedOSDataPoints.end()) {
      return *it;
    }
    return boost::none;
  }

  boost::optional<analysis::DataPoint> CurrentAnalysis_Impl::isQueuedDakotaDataPoint(
      const analysis::DataPoint& dataPoint) const
  {
    auto it = std::find_if(m_queuedDakotaDataPoints.begin(),
                           m_queuedDakotaDataPoints.end(),
                           std::bind(dataPointsEqual,std::placeholders::_1,dataPoint));
    if (it != m_queuedDakotaDataPoints.end()) {
      return *it;
    }
    return boost::none;
  }

  void CurrentAnalysis_Impl::addNextBatchOSDataPoints(const std::vector<analysis::DataPoint>& nextBatchJobs) {
    for (const analysis::DataPoint& nextJob : nextBatchJobs) {
      m_queuedOSDataPoints.push_back(nextJob);
    }
  }

  void CurrentAnalysis_Impl::addDakotaDataPoint(const analysis::DataPoint& newJob) {
    m_queuedDakotaDataPoints.push_back(newJob);
  }

  void CurrentAnalysis_Impl::augmentQueuedDakotaDataPoint(const analysis::DataPoint& dataPoint,
                                                          const openstudio::path& dakotaParametersFile)
  {
    auto it = std::find_if(m_queuedDakotaDataPoints.begin(),
                           m_queuedDakotaDataPoints.end(),
                           std::bind(dataPointsEqual,std::placeholders::_1,dataPoint));
    if (it != m_queuedDakotaDataPoints.end()){
      std::vector<openstudio::path> dakotaParametersFiles = it->dakotaParametersFiles();
      dakotaParametersFiles.push_back(dakotaParametersFile);
      it->getImpl<analysis::detail::DataPoint_Impl>()->setDakotaParametersFiles(dakotaParametersFiles);
    }
  }

  analysis::DataPoint CurrentAnalysis_Impl::removeCompletedOSDataPoint(const openstudio::UUID& completedJob) {
    auto it = std::find_if(m_queuedOSDataPoints.begin(),
                           m_queuedOSDataPoints.end(),
                           std::bind(jobUUIDsEqual,std::placeholders::_1,completedJob));
    OS_ASSERT(it != m_queuedOSDataPoints.end());
    analysis::DataPoint result = *it;
    
    ++m_numOSJobsComplete;

    if (it->failed()){
      ++m_numOSJobsFailed;
    }

    m_queuedOSDataPoints.erase(it);

    emit iterationProgress(numCompletedJobsInOSIteration(),totalNumJobsInOSIteration());
    return result;
  }

  analysis::DataPoint CurrentAnalysis_Impl::removeCompletedDakotaDataPoint(const openstudio::UUID& completedJob) {
    auto it = std::find_if(m_queuedDakotaDataPoints.begin(),
                           m_queuedDakotaDataPoints.end(),
                           std::bind(jobUUIDsEqual,std::placeholders::_1,completedJob));
    OS_ASSERT(it != m_queuedDakotaDataPoints.end());
    analysis::DataPoint result = *it;
    m_queuedDakotaDataPoints.erase(it);
    return result;
  }

  // DLM: Jason it seems the following code should be in run manager somewhere?
  // maybe a treeChildren() or allChildren() method?
  void recursivelyAddJobAndChildren(std::vector<runmanager::Job>& jobs, const runmanager::Job& job){
    jobs.push_back(job);
    for (const runmanager::Job& child : job.children()) {
      recursivelyAddJobAndChildren(jobs, child);
    }
  }

  // DLM: Jason maybe this is what setTreeCanceled is for?  
  // But there is no treeRequestStop or treeWaitForFinished?
  void cancelAllJobs(std::vector<runmanager::Job>& jobs){

    // cancel jobs asynchronously
    for (runmanager::Job& job : jobs) {
      job.setCanceled(true);
    }
    // wait for all jobs to stop
    for (runmanager::Job& job : jobs) {
      job.requestStop();
    }
    // wait for all jobs to stop
    for (runmanager::Job& job : jobs) {
      job.waitForFinished();
    }
  }

  void CurrentAnalysis_Impl::stop(runmanager::RunManager& runManager) {

    // get all jobs in the queued analysis, including child jobs
    analysis::DataPointVector queuedOSDataPoints = m_queuedOSDataPoints;
    std::vector<runmanager::Job> jobs;
    for (const analysis::DataPoint& queuedOSDataPoint : queuedOSDataPoints) {
      boost::optional<runmanager::Job> job = queuedOSDataPoint.topLevelJob();
      if (job){
        recursivelyAddJobAndChildren(jobs, *job);
      }
    }
    m_queuedOSDataPoints.clear();

    analysis::DataPointVector queuedDakotaDataPoints = m_queuedDakotaDataPoints;
    for (const analysis::DataPoint& queuedDakotaDataPoint : queuedDakotaDataPoints) {
      boost::optional<runmanager::Job> job = queuedDakotaDataPoint.topLevelJob();
      if (job){
        recursivelyAddJobAndChildren(jobs, *job);
      }
    }
    m_queuedDakotaDataPoints.clear();

    cancelAllJobs(jobs);

    if (m_dakotaJob) {
      // DLM: does this job have children that need to be canceled too?
      runmanager::Job dakotaJob = runManager.getJob(*m_dakotaJob);
      dakotaJob.setCanceled(true);
      dakotaJob.requestStop();
      dakotaJob.waitForFinished();
      m_dakotaJobErrors = dakotaJob.errors();
      m_dakotaJob.reset();
    }
  }

  void CurrentAnalysis_Impl::stopQueuedDakotaJobs(runmanager::RunManager& runManager) {
    // get all jobs in the queued analysis, including child jobs
    std::vector<runmanager::Job> jobs;
    auto it = m_queuedDakotaDataPoints.begin();
    while (it != m_queuedDakotaDataPoints.end()) {
      boost::optional<runmanager::Job> job = it->topLevelJob();
      if (job){
        recursivelyAddJobAndChildren(jobs, *job);
      }
      it = m_queuedDakotaDataPoints.erase(it);
    }

    cancelAllJobs(jobs);
  }

  void CurrentAnalysis_Impl::stop(analysis::DataPoint& dataPoint,
                                  runmanager::RunManager& runManager)
  {
    OptionalDataPoint exactDataPoint = analysis().getDataPointByUUID(dataPoint);
    if (exactDataPoint) {

      // get all jobs to cancel including child jobs
      std::vector<runmanager::Job> jobs;

      auto it = std::find_if(m_queuedOSDataPoints.begin(),
                             m_queuedOSDataPoints.end(),
                             std::bind(dataPointsEqual,std::placeholders::_1,*exactDataPoint));
      if (it != m_queuedOSDataPoints.end()) {
        boost::optional<runmanager::Job> job = it->topLevelJob();
        if (job){
          recursivelyAddJobAndChildren(jobs, *job);
        }
        m_queuedOSDataPoints.erase(it);
      }

      it = std::find_if(m_queuedDakotaDataPoints.begin(),
                        m_queuedDakotaDataPoints.end(),
                        std::bind(dataPointsEqual,std::placeholders::_1,*exactDataPoint));
      if (it != m_queuedDakotaDataPoints.end()) {
        boost::optional<runmanager::Job> job = it->topLevelJob();
        if (job){
          recursivelyAddJobAndChildren(jobs, *job);
        }
        m_queuedDakotaDataPoints.erase(it);
      }

      cancelAllJobs(jobs);
    }
  }

} // detail

CurrentAnalysis::CurrentAnalysis(const analysis::Analysis& analysis,
                                 const AnalysisRunOptions& runOptions)
  : m_impl(std::shared_ptr<detail::CurrentAnalysis_Impl>(
               new detail::CurrentAnalysis_Impl(analysis,runOptions)))
{}

analysis::Analysis CurrentAnalysis::analysis() const {
  return getImpl()->analysis();
}

AnalysisRunOptions CurrentAnalysis::runOptions() const {
  return getImpl()->runOptions();
}

std::shared_ptr<detail::CurrentAnalysis_Impl> CurrentAnalysis::getImpl() const {
  return m_impl;
}

int CurrentAnalysis::numQueuedJobs() const {
  return getImpl()->numQueuedJobs();
}

int CurrentAnalysis::numQueuedOSJobs() const {
  return getImpl()->numQueuedOSJobs();
}

int CurrentAnalysis::numQueuedDakotaJobs() const {
  return getImpl()->numQueuedDakotaJobs();
}

int CurrentAnalysis::numCompletedJobsInOSIteration() const {
  return getImpl()->numCompletedJobsInOSIteration();
}

int CurrentAnalysis::numFailedJobsInOSIteration() const {
  return getImpl()->numFailedJobsInOSIteration();
}


int CurrentAnalysis::totalNumJobsInOSIteration() const {
  return getImpl()->totalNumJobsInOSIteration();
}

boost::optional<runmanager::JobErrors> CurrentAnalysis::dakotaJobErrors() const {
  return getImpl()->dakotaJobErrors();
}

bool CurrentAnalysis::operator==(const CurrentAnalysis& other) const {
  return (m_impl == other.m_impl);
}

bool CurrentAnalysis::operator!=(const CurrentAnalysis& other) const {
  return !(operator==(other));
}

bool CurrentAnalysis::connect(const std::string& signal,
                              const QObject* qObject,
                              const std::string& slot,
                              Qt::ConnectionType type) const
{
  return getImpl()->connect(signal,qObject,slot,type);
}

void CurrentAnalysis::moveToThread(QThread* targetThread) {
  m_impl->moveToThread(targetThread);
}

/// @cond
CurrentAnalysis::CurrentAnalysis(std::shared_ptr<detail::CurrentAnalysis_Impl> impl)
  : m_impl(impl)
{}
/// @endcond

} // analysisdriver
} // openstudio

