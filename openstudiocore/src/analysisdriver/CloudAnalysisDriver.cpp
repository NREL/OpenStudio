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
      m_lastDownloadDetaileResultsSuccess(false)
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

  bool CloudAnalysisDriver_Impl::downloadDetailedResults(analysis::DataPoint& dataPoint) {

  }

  bool CloudAnalysisDriver_Impl::lastDownloadDetailedResultsSuccess() const {

  }

  bool CloudAnalysisDriver_Impl::isRunning() const {
    return (m_requestRun || m_monitorDataPoints);
  }

  bool CloudAnalysisDriver_Impl::isDownloading() const {
    return m_requestDownload;
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

    if (OptionalUrl url = provider().serverUrl()) {

      m_lastRunSuccess = false;
      m_requestRun = OSServer(*url);

      // make sure the server is available
      bool test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(availableForRun(bool)));
      OS_ASSERT(test);

      test = m_requestRun->requestAvailable();
      if (!test) {
        m_requestRun.reset();
        emit runRequestComplete(false);
      }

      return test;
    }

    LOG(Error,"Cannot start a run because the CloudProvider has not been started or has been terminated.")
    return false;
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

    if (success) {
      // post the analysis
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisPosted(bool)));
      OS_ASSERT(test);

      success = m_requestRun->startPostAnalysisJSON(
                    project().analysis().uuid(),
                    project.analysis().toJSON(AnalysisSerializationOptions(project().projectDir())));
    }

    if (!success) {
      m_requestRun.reset();
      emit runRequestComplete(false);
    }
  }

  void CloudAnalysisDriver_Impl::analysisPosted(bool success) {

    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisPosted(bool)));
    OS_ASSERT(test);

    if (success) {
      // upload the analysis
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisUploaded(bool)));
      OS_ASSERT(test);

      success = m_requestRun->startUploadAnalysisFiles(project().analysis().uuid(),
                                                       project().zipFileForCloud());
    }

    if (!success) {
      m_requestRun.reset();
      emit runRequestComplete(false);
    }
  }

  void CloudAnalysisDriver_Impl::analysisUploaded(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisUploaded(bool)));
    OS_ASSERT(test);

    if (success) {
      // start posting DataPoints
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointQueued(bool)));
      OS_ASSERT(test);

      // initialize queue
      DataPointVector dataPoints = project().analysis().dataPointsToQueue();
      m_uploadQueue = std::deque<DataPoint>(dataPoints.begin(),dataPoints.end());

      DataPoint toQueue = m_uploadQueue.front();
      m_uploadQueue.pop_front();
      success = m_requestRun->startPostDataPointJSON(
                    project().analysis().uuid(),
                    toQueue.toJSON(DataPointSerializationOptions(project().projectDir())));
      m_waitingQueue.push_back(toQueue);
      emit dataPointQueued(project().analysis().uuid(),toQueue.uuid());
    }

    if (!success) {
      m_requestRun.reset();
      emit runRequestComplete(false);
    }
  }

  void CloudAnalysisDriver_Impl::dataPointQueued(bool success) {
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
  }

  void CloudAnalysisDriver_Impl::analysisStarted(bool success) {
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
  }

  void CloudAnalysisDriver_Impl::completeDataPointUUIDsReturned(bool success) {
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
  }

  bool CloudAnalysisDriver_Impl::startDownloading() {
    if (isDownloading()) {
      LOG(Info,"Already started the download process.");
      return false;
    }

    if (OptionalUrl url = provider().serverUrl()) {
      m_requestDownload = OSServer(*url);

      bool test = m_requestDownload->connect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointDownloadComplete(bool)));
      OS_ASSERT(test);

      DataPoint toDownload = m_downloadQueue.front();
      test = requestDataPointDownload(toDownload);
      return test;
    }

    LOG(Error,"Cannot start the download process because the CloudProvider has not been started or has been terminated.")
    return false;
  }

  bool CloudAnalysisDriver_Impl::requestDataPointDownload(const DataPoint& dataPoint) {
    OS_ASSERT(m_requestDownload);

    bool result;
    if (dataPoint.runType() == DataPointRunType::CloudSlim) {
      result = m_requestDownload->requestDataPointJSON(project().analysis().uuid(),
                                                       dataPoint.uuid());
    }
    else {
      OS_ASSERT(dataPoint.runType() == DataPointRunType::CloudDetailed);
      openstudio::path dataPointDir = project().projectDir() / toPath("dataPoint_" + removeBraces(dataPoint.uuid()));
      dataPoint.setDirectory(dataPointDir);
      if (boost::filesystem::exists(dataPointDir)) {
        try {
          boost::filesystem::remove_all(dataPointDir);
        }
        catch (...) {}
      }
      boost::filesystem::create_directory(dataPointDir);
      result = m_requestDownload->startDownloadDataPoint(project().analysis().uuid(),
                                                         dataPoint.uuid(),
                                                         dataPointDir / toPath("data_point.zip"));
    }

    return result;
  }

} // detail

/// @cond
CloudAnalysisDriver::CloudAnalysisDriver(boost::shared_ptr<detail::CloudAnalysisDriver_Impl> impl)
  : m_impl(impl)
{}
/// @endcond

} // analysisdriver
} // openstudio

