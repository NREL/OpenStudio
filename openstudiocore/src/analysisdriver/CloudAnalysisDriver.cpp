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

#include <analysisdriver/CloudAnalysisDriver.hpp>
#include <analysisdriver/CloudAnalysisDriver_Impl.hpp>

#include <analysis/Analysis.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/System.hpp>

using namespace openstudio::analysis;

namespace openstudio {
namespace analysisdriver {

namespace detail {

  CloudAnalysisDriver_Impl::CloudAnalysisDriver_Impl(const CloudProvider& provider,
                                                     const SimpleProject& project)
    : m_provider(provider),
      m_project(project),
      m_lastRunSuccess(false),
      m_lastStopSuccess(false),
      m_lastDownloadDetailedResultsSuccess(false)
  {}

  CloudProvider CloudAnalysisDriver_Impl::provider() const {
    return m_provider;
  }

  SimpleProject CloudAnalysisDriver_Impl::project() const {
    return m_project;
  }

  bool CloudAnalysisDriver_Impl::run(int msec) {
    if (requestRun()) {
      waitForFinished(msec);
    }
    return lastRunSuccess();
  }

  bool CloudAnalysisDriver_Impl::lastRunSuccess() const {
    return m_lastRunSuccess;
  }

  bool CloudAnalysisDriver_Impl::stop(int msec) {
    if (requestStop()) {
      waitForFinished(msec);
    }
    return lastStopSuccess();
  }

  bool CloudAnalysisDriver_Impl::lastStopSuccess() const {
    return m_lastStopSuccess;
  }

  bool CloudAnalysisDriver_Impl::downloadDetailedResults(analysis::DataPoint& dataPoint,
                                                         int msec)
  {
    if (requestDownloadDetailedResults(dataPoint)) {
      waitForFinished(msec);
    }
    return lastDownloadDetailedResultsSuccess();
  }

  bool CloudAnalysisDriver_Impl::lastDownloadDetailedResultsSuccess() const {
    return m_lastDownloadDetailedResultsSuccess;
  }

  bool CloudAnalysisDriver_Impl::isRunning() const {
    return (m_requestRun || m_monitorDataPoints);
  }

  bool CloudAnalysisDriver_Impl::isDownloading() const {
    return (m_requestJson || m_requestDetails);
  }

  std::vector<std::string> CloudAnalysisDriver_Impl::errors() const {
    return m_errors;
  }

  std::vector<std::string> CloudAnalysisDriver_Impl::warnings() const {
    return m_warnings;
  }

  bool CloudAnalysisDriver_Impl::waitForFinished(int msec) {

    int msecPerLoop = 20;
    int currentMsecs = 0;

    while (isRunning() || isDownloading()) {
      System::msleep(msecPerLoop);

      currentMsecs += msecPerLoop;

      if ((msec > 0) && (currentMsecs > msec)) {
        return false;
      }
    }

    return true;
  }

  bool CloudAnalysisDriver_Impl::requestRun() {
    if (isRunning()) {
      LOG(Info,"Cannot request a run when the CloudAnalysisDriver is already running.");
      return false;
    }

    // try to start/restart run
    m_lastRunSuccess = false;
    clearErrorsAndWarnings();

    if (OptionalUrl url = provider().serverUrl()) {

      m_requestRun = OSServer(*url);

      // make sure the server is available
      bool test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(availableForRun(bool)));
      OS_ASSERT(test);

      test = m_requestRun->requestAvailable();
      if (!test) {
        registerRunFailure();
      }

      return true;
    }

    logError("Cannot start a run because the CloudProvider has not been started or has been terminated.");
    emit runRequestComplete(false);
    return true;
  }

  bool CloudAnalysisDriver_Impl::connect(const std::string& signal,
                                         const QObject* qObject,
                                         const std::string& slot,
                                         Qt::ConnectionType type) const
  {
    return QObject::connect(this, signal.c_str(), qObject, slot.c_str(), type);
  }

  void CloudAnalysisDriver_Impl::availableForRun(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(availableForRun(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run request failed on checking the server availability.");
    }

    if (success) {
      success = m_requestRun->lastAvailable();
      if (!success) {
        logError("Run request failed because the server is not available.");
      }
    }

    if (success) {
      // see if the analysis needs to be posted
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisOnServer(bool)));
      OS_ASSERT(test);

      success = m_requestRun->requestAnalysisUUIDs(project().analysis().uuid());
    }

    if (!success) {
      registerRunFailure();
    }
  }

  void CloudAnalysisDriver_Impl::analysisOnServer(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisOnServer(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run request failed on retrieving analysis UUIDs.");
    }

    if (success) {
      UUIDVector analysisUUIDs = m_requestRun->lastAnalysisUUIDs();
      if (std::find(analysisUUIDs.begin(),analysisUUIDs.end(),project().analysis().uuid()) == analysisUUIDs.end()) {
        // analysis not found -- post it
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisPosted(bool)));
        OS_ASSERT(test);

        success = m_requestRun->startPostAnalysisJSON(
              project().analysis().uuid(),
              project().analysis().toJSON(AnalysisSerializationOptions(project().projectDir())));
      }
      else {
        // analysis found -- see if there are any data points already on the server
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(allDataPointUUIDsReturned(bool)));
        OS_ASSERT(test);

        success = m_requestRun->requestDataPointUUIDs(project().analysis().uuid());
      }
    }

    if (!success) {
      registerRunFailure();
    }
  }

  void CloudAnalysisDriver_Impl::analysisPosted(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisPosted(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run request failed on posting the Analysis JSON.");
    }

    if (success) {
      success = m_requestRun->lastPostAnalysisJSONSuccess();
      if (!success) {
        logError("Run request failed because posting the analysis JSON failed.");
      }
    }

    if (success) {
      // upload the analysis
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisUploaded(bool)));
      OS_ASSERT(test);

      success = m_requestRun->startUploadAnalysisFiles(project().analysis().uuid(),
                                                       project().zipFileForCloud());
    }

    if (!success) {
      registerRunFailure();
    }
  }

  void CloudAnalysisDriver_Impl::allDataPointUUIDsReturned(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(allDataPointUUIDsReturned(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run request failed on request for list of DataPoint UUIDs.");
    }

    if (success) {
      if (m_requestRun->lastDataPointUUIDs().empty()) {
        // no data points posted yet, upload the analysis files
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisUploaded(bool)));
        OS_ASSERT(test);

        success = m_requestRun->startUploadAnalysisFiles(project().analysis().uuid(),
                                                         project().zipFileForCloud());
      }
      else {
        // there are data points, sort out all the queues--have list of all data points
        // server knows about. need list of complete data points to determine waiting versus
        // downloading queues
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(readyToSortOutQueues(bool)));
        OS_ASSERT(test);

        success = m_requestRun->requestCompleteDataPointUUIDs(project().analysis().uuid());
      }
    }

    if (!success) {
      registerRunFailure();
    }
  }

  void CloudAnalysisDriver_Impl::analysisUploaded(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisUploaded(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run request failed on trying to upload the analysis files.");
    }

    if (success) {
      success = m_requestRun->lastUploadAnalysisFilesSuccess();
      if (!success) {
        logError("Run request failed because uploading the analysis files failed.");
      }
    }

    if (success) {
      // start posting DataPoints
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointQueued(bool)));
      OS_ASSERT(test);

      // initialize queue
      DataPointVector dataPoints = project().analysis().dataPointsToQueue();
      m_postQueue = std::deque<DataPoint>(dataPoints.begin(),dataPoints.end());

      DataPoint toQueue = m_postQueue.front();
      m_postQueue.pop_front();
      success = m_requestRun->startPostDataPointJSON(
                    project().analysis().uuid(),
                    toQueue.toJSON(DataPointSerializationOptions(project().projectDir())));
      m_waitingQueue.push_back(toQueue);
      emit dataPointQueued(project().analysis().uuid(),toQueue.uuid());
    }

    if (!success) {
      registerRunFailure();
    }
  }

  void CloudAnalysisDriver_Impl::readyToSortOutQueues(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(readyToSortOutQueues(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run request failed on trying to set up the queues based on the server state.");
    }

    if (success) {
      UUIDVector allUUIDs = m_requestRun->lastDataPointUUIDs();
      UUIDVector completeUUIDs = m_requestRun->lastCompleteDataPointUUIDs();

      BOOST_FOREACH(const DataPoint& missingPoint, project().analysis().dataPointsToQueue()) {
        if (std::find(allUUIDs.begin(),allUUIDs.end(),missingPoint.uuid()) == allUUIDs.end()) {
          // post queue -- need to be run and are not in allUUIDs
          m_postQueue.push_back(missingPoint);
          continue;
        }
        if (std::find(completeUUIDs.begin(),completeUUIDs.end(),missingPoint.uuid()) == completeUUIDs.end()) {
          // waiting queue -- need to be run and are not in completeUUIDs
          m_waitingQueue.push_back(missingPoint);
        }
        else {
          // json queue -- need to be run and are in completeUUIDs
          m_jsonQueue.push_back(missingPoint);
        }
      }

      BOOST_FOREACH(const DataPoint& missingDetails, project().analysis().dataPointsNeedingDetails()) {
        // details queue -- are complete, but details were reqeusted and are not available yet

        // out of luck -- not complete on this server, issue warning. user will need to clear
        // results and re-run.

      }

    }

  }

  /* void CloudAnalysisDriver_Impl::dataPointQueued(bool success) {
    if (success) {
      // see if you make another post or go on to starting the analysis
      if (m_uploadQueue.empty()) {
        // done uploading --move on
        bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointQueued(bool)));
        OS_ASSERT(test);

        test = m_request->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStarted(bool)));
        OS_ASSERT(test);

        // start analysis
        success = m_requestRun->requestStart(project().analysis().uuid());
      }
      else {
        // post another data point
        DataPoint toQueue = m_uploadQueue.front();
        m_uploadQueue.pop_front();
        success = m_requestRun->startPostDataPointJSON(
              project().analysis().uuid(),
              toQueue.toJSON(DataPointSerializationOptions(project().projectDir())));
        m_waitingQueue.push_back(toQueue);
        emit dataPointQueued(project().analysis().uuid(),toQueue.uuid());
      }
    }

    if (!success) {
      m_requestRun.reset();
      emit runRequestComplete(false);
    }
  } */

  /* void CloudAnalysisDriver_Impl::analysisStarted(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStarted(bool)));
    OS_ASSERT(test);

    // start monitoring server for completed data points
    if (OptionalUrl url = provider().serverUrl()) {
      m_monitorDataPoints = OSServer(*url);

      test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(completeDataPointUUIDsReturned(bool)));
      OS_ASSERT(test);

      test = m_monitorDataPoints->requestCompleteDataPointUUIDs(project().analysis().uuid());
      if (!test) {
        // not able to monitor, for now just stop everything in this case
        LOG(Error,"Request for list of complete DataPoint UUIDs failed.");
        requestStop();
      }
    }

    m_lastRunSuccess = success;
    m_requestRun.reset();
    emit runRequestComplete(success);
  } */

  /* void CloudAnalysisDriver_Impl::completeDataPointUUIDsReturned(bool success) {
    if (success) {
      OS_ASSERT(m_monitorDataPoints);

      UUIDVector temp = m_monitorDataPoints->lastCompleteDataPointUUIDs();
      std::set<UUID> completeUUIDs(temp.begin(),temp.end());
      DataPointVector::iterator it = m_waitingQueue.begin();
      while (it != m_waitingQueue.end()) {
        if (completeUUIDs.find(it->uuid()) != completeUUIDs.end()) {
          m_downloadQueue.push_back(*it);
          it = m_waitingQueue.erase(it);
        }
        else {
          ++it;
        }
      }

      if (!m_downloadQueue.empty() && !m_requestDownload) {
        success = startDownloading();
      }

      if (m_waitingQueue.empty()) {
        m_monitorDataPoints.reset();
      }
      else {
        LOG(Info,"Waiting on " << m_waitingQueue.size() << " DataPoints.");
        success = success && m_monitorDataPoints->requestCompleteDataPointUUIDs(project().analysis().uuid());
      }
    }

    if (!success) {
      LOG(Error,"Server monitoring failed mid-run.");
      requestStop();
    }
  } */

  void CloudAnalysisDriver_Impl::clearErrorsAndWarnings() {
    m_errors.clear();
    m_warnings.clear();
  }

  void CloudAnalysisDriver_Impl::logError(const std::string& error) {
    m_errors.push_back(error);
    LOG(Error,error);
  }

  void CloudAnalysisDriver_Impl::logWarning(const std::string& warning) {
    m_warnings.push_back(warning);
    LOG(Warn,warning);
  }

  void CloudAnalysisDriver_Impl::appendErrorsAndWarnings(const OSServer& server) {
    StringVector temp = server.errors();
    m_errors.insert(m_errors.end(),temp.begin(),temp.end());
    temp = server.warnings();
    m_warnings.insert(m_warnings.end(),temp.begin(),temp.end());
  }

  void CloudAnalysisDriver_Impl::registerRunFailure() {
    appendErrorsAndWarnings(*m_requestRun);
    m_requestRun.reset();
    m_postQueue.clear();
    m_waitingQueue.clear();
    emit runRequestComplete(false);
  }

} // detail

CloudAnalysisDriver::CloudAnalysisDriver(const CloudProvider& provider,
                                         const SimpleProject& project)
  : m_impl(boost::shared_ptr<detail::CloudAnalysisDriver_Impl>(
             new detail::CloudAnalysisDriver_Impl(provider,project)))
{}

CloudProvider CloudAnalysisDriver::provider() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->provider();
}

SimpleProject CloudAnalysisDriver::project() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->project();
}

bool CloudAnalysisDriver::run(int msec) {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->run(msec);
}

bool CloudAnalysisDriver::lastRunSuccess() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->lastRunSuccess();
}

bool CloudAnalysisDriver::stop(int msec) {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->stop(msec);
}

bool CloudAnalysisDriver::lastStopSuccess() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->lastStopSuccess();
}

bool CloudAnalysisDriver::downloadDetailedResults(analysis::DataPoint& dataPoint,int msec) {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->downloadDetailedResults(dataPoint,msec);
}

bool CloudAnalysisDriver::lastDownloadDetailedResultsSuccess() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->lastDownloadDetailedResultsSuccess();
}

bool CloudAnalysisDriver::isRunning() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->isRunning();
}

bool CloudAnalysisDriver::isDownloading() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->isDownloading();
}

std::vector<std::string> CloudAnalysisDriver::errors() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->errors();
}

std::vector<std::string> CloudAnalysisDriver::warnings() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->warnings();
}

bool CloudAnalysisDriver::waitForFinished(int msec) {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->waitForFinished(msec);
}

bool CloudAnalysisDriver::requestRun() {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->requestRun();
}

bool CloudAnalysisDriver::requestStop(bool waitForAlreadyRunningDataPoints) {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->requestStop(waitForAlreadyRunningDataPoints);
}

bool CloudAnalysisDriver::requestDownloadDetailedResults(analysis::DataPoint& dataPoint) {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->requestDownloadDetailedResults(dataPoint);
}

bool CloudAnalysisDriver::connect(const std::string& signal,
                                  const QObject* qObject,
                                  const std::string& slot,
                                  Qt::ConnectionType type) const
{
  return getImpl<detail::CloudAnalysisDriver_Impl>()->connect(signal,qObject,slot,type);
}

void CloudAnalysisDriver::moveToThread(QThread* targetThread) {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->moveToThread(targetThread);
}

/// @cond
CloudAnalysisDriver::CloudAnalysisDriver(boost::shared_ptr<detail::CloudAnalysisDriver_Impl> impl)
  : m_impl(impl)
{}
/// @endcond

} // analysisdriver
} // openstudio

