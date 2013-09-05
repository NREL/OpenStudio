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

#include <project/ProjectDatabase.hpp>

#include <analysis/Analysis.hpp>

#include <runmanager/lib/RunManager.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/System.hpp>

#include <boost/foreach.hpp>

using namespace openstudio::analysis;
using namespace openstudio::runmanager;

namespace openstudio {
namespace analysisdriver {

namespace detail {

  CloudAnalysisDriver_Impl::CloudAnalysisDriver_Impl(const CloudProvider& provider,
                                                     const SimpleProject& project)
    : m_provider(provider),
      m_project(project),
      m_lastRunSuccess(false),
      m_lastStopSuccess(false),
      m_lastDownloadDetailedResultsSuccess(false),
      m_checkDataPointsRunningInsteadOfAnalysis(false),
      m_lastGetRunningDataPointsSuccess(false)
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

  bool CloudAnalysisDriver_Impl::isStopping() const {
    return m_requestStop;
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
    if (isRunning() || isStopping() || isDownloading()) {
      LOG(Info,"Cannot request a run when the CloudAnalysisDriver is already running, stopping or downloading.");
      return false;
    }

    // try to start/restart run
    m_lastRunSuccess = false;
    // make sure all the queues are empty. will repopulate.
    m_postQueue.clear();
    m_waitingQueue.clear();
    m_jsonQueue.clear();
    m_detailsQueue.clear();
    clearErrorsAndWarnings();
    // default is for monitoring process to check if analysis is running
    // if stop, may switch to looking for running data points instead
    m_checkDataPointsRunningInsteadOfAnalysis = false;
    m_lastGetRunningDataPointsSuccess = false;

    if (OptionalUrl url = provider().serverUrl()) {

      m_requestRun = OSServer(*url);

      // make sure the server is available
      bool test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(availableForRun(bool)));
      OS_ASSERT(test);

      test = m_requestRun->requestAvailable();
      if (!test) {
        registerRunRequestFailure();
      }

      return true;
    }

    logError("Cannot start a run because the CloudProvider has not been started or has been terminated.");
    emit runRequestComplete(false);
    return true;
  }

  bool CloudAnalysisDriver_Impl::requestStop(bool waitForAlreadyRunningDataPoints) {
    if (! (isRunning() || isDownloading())) {
      LOG(Info,"Nothing to stop.")
      return false;
    }

    m_lastStopSuccess = false;
    // running or downloading, so let errors and warnings accumulate

    // if waitForAlreadyRunningDataPoints, switch the monitoring process to look for
    // data points running, not analysis running
    if (waitForAlreadyRunningDataPoints) {
      m_checkDataPointsRunningInsteadOfAnalysis = true;
      m_lastGetRunningDataPointsSuccess = false;
    }

    if (OptionalUrl url = provider().serverUrl()) {

      m_requestStop = OSServer(*url);

      // request analysis to stop
      bool test = m_requestStop->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStopped(bool)));
      OS_ASSERT(test);

      test = m_requestStop->requestStop(project().analysis().uuid());
      if (!test) {
        registerStopRequestFailure();
      }

      return true;
    }

    logError("Cannot stop the run because the CloudProvider has not been started or has been terminated.");
    emit stopRequestComplete(false);
    return true;
  }

  bool CloudAnalysisDriver_Impl::requestDownloadDetailedResults(analysis::DataPoint& dataPoint) {
    OptionalDataPoint actualDataPoint = project().analysis().getDataPointByUUID(dataPoint.uuid());
    if (!actualDataPoint || !actualDataPoint->complete() || !actualDataPoint->directory().empty()) {
      return false;
    }
    if (isStopping()) {
      logWarning("A download cannot be requested while the analysis is stopping.");
      return false;
    }

    // this is probably already set, but just to be sure
    actualDataPoint->setRunType(DataPointRunType::CloudDetailed);

    m_lastDownloadDetailedResultsSuccess = false;
    if (!(isRunning() || isDownloading())) {
      clearErrorsAndWarnings();
    }

    // see if already in download queue (nothing to do for the moment)
    bool found = !(std::find(m_detailsQueue.begin(),
                             m_detailsQueue.end(),
                             actualDataPoint.get()) == m_detailsQueue.end());

    // if not, see if there are results on the server
    if (!found) {
      found = !(std::find(m_preDetailsQueue.begin(),
                          m_preDetailsQueue.end(),
                          actualDataPoint.get()) == m_preDetailsQueue.end());
      if (!found) {
        m_preDetailsQueue.push_back(*actualDataPoint);
      }
      if (!m_checkForResultsToDownload) {

        if (OptionalUrl url = provider().serverUrl()) {
          m_checkForResultsToDownload = OSServer(*url);

          // request completed data point uuids
          bool test = m_checkForResultsToDownload->connect(SIGNAL(requestProcessed(bool)),this,SLOT(areResultsAvailableForDownload(bool)));
          OS_ASSERT(test);

          test = m_checkForResultsToDownload->requestCompleteDataPointUUIDs(project().analysis().uuid());
          if (!test) {
            registerDownloadDetailsRequestFailure();
          }

          return true;
        }

        logError("Cannot request a detailed results download because the CloudProvider has not been started or has been terminated.");
        emit detailedDownloadRequestsComplete(false);
        return true;
      }
    }

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

      success = m_requestRun->requestAnalysisUUIDs(project().projectDatabase().handle());
    }

    if (!success) {
      registerRunRequestFailure();
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
              project().projectDatabase().handle(),
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
      registerRunRequestFailure();
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
      registerRunRequestFailure();
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
      registerRunRequestFailure();
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

      success = postNextDataPoint();
    }

    if (!success) {
      registerRunRequestFailure();
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
        // ETH@20130830 -- Is missingPoint waiting on already running results, or does it
        //     need to be re-run? Maybe need --force boolean. Then download complete results
        //     if reconnected to CloudProvider, but force re-run if had previously completed
        //     successfully.
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
        if (std::find(completeUUIDs.begin(),completeUUIDs.end(),missingDetails.uuid()) != completeUUIDs.end()) {
          // details queue -- are complete, but details were reqeusted and are not available yet
          m_detailsQueue.push_back(missingDetails);
        }
        else {
          // out of luck -- not complete on this server, issue warning. user will need to clear
          // results and re-run.
          std::stringstream ss;
          ss << "Unable to load details for DataPoint '" << missingDetails.name() << "', "
             << removeBraces(missingDetails.uuid()) << ". The data is no longer available on the "
             << "OpenStudio Server. Please clear that DataPoint's results and re-run to get the details.";
          logWarning(ss.str());
        }

      }

      if (m_postQueue.size() > 0) {
        // start posting DataPoints
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointQueued(bool)));
        OS_ASSERT(test);

        // initialize queue
        DataPointVector dataPoints = project().analysis().dataPointsToQueue();
        m_postQueue = std::deque<DataPoint>(dataPoints.begin(),dataPoints.end());

        success = postNextDataPoint();
      }
      else {
        // all data points are already posted, go ahead and see if the analysis is running on
        // the server
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisRunningOnServer(bool)));
        OS_ASSERT(test);

        success = m_requestRun->requestIsAnalysisRunning(project().analysis().uuid());
      }

    }

    if (!success) {
      registerRunRequestFailure();
    }

  }

  void CloudAnalysisDriver_Impl::dataPointQueued(bool success) {

    if (!success) {
      logError("Run request failed on trying to post a data point.");
    }

    if (success) {
      success = m_requestRun->lastPostDataPointJSONSuccess();
      if (!success) {
        logError("Run request failed because a DataPoint JSON did not post successfully.");
      }
    }

    if (success) {
      // see if you make another post or go on to starting the analysis
      if (m_postQueue.empty()) {
        // done posting --move on
        bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointQueued(bool)));
        OS_ASSERT(test);

        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisRunningOnServer(bool)));
        OS_ASSERT(test);

        // see if the analysis is already running
        success = m_requestRun->requestIsAnalysisRunning(project().analysis().uuid());
      }
      else {
        success = postNextDataPoint();
      }
    }

    if (!success) {
      registerRunRequestFailure();
    }
  }

  void CloudAnalysisDriver_Impl::analysisRunningOnServer(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisRunningOnServer(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run request failed on trying to check whether the analysis is already running on the server.");
    }

    if (success) {
      if (m_requestRun->lastIsAnalysisRunning()) {
        // analysis is already running, start monitoring data points (this process will also
        // start downloading results if either of those queues are not empty).
        startMonitoring();
      }
      else {
        // start the analysis
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStarted(bool)));
        OS_ASSERT(test);

        success = m_requestRun->requestStart(project().analysis().uuid());
      }
    }

    if (!success) {
      registerRunRequestFailure();
    }
  }

  void CloudAnalysisDriver_Impl::analysisStarted(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStarted(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run request failed on trying to start the analysis.");
    }

    if (success) {
      success = m_requestRun->lastStartSuccess();
      if (!success) {
        logError("The server was unable to start running the analysis.");
      }
    }

    if (success) {
      startMonitoring();
    }

    if (!success) {
      registerRunRequestFailure();
    }
  }

  void CloudAnalysisDriver_Impl::completeDataPointUUIDsReturned(bool success) {
    bool test = m_monitorDataPoints->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(completeDataPointUUIDsReturned(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run failed during monitoring. Request for complete DataPoint UUIDs failed.");
    }

    if (success) {
      UUIDVector temp = m_monitorDataPoints->lastCompleteDataPointUUIDs();
      std::set<UUID> completeUUIDs(temp.begin(),temp.end());
      DataPointVector::iterator it = m_waitingQueue.begin();
      while (it != m_waitingQueue.end()) {
        if (completeUUIDs.find(it->uuid()) != completeUUIDs.end()) {
          m_jsonQueue.push_back(*it);
          it = m_waitingQueue.erase(it);
        }
        else {
          ++it;
        }
      }

      if (!m_jsonQueue.empty() && !m_requestJson) {
        startDownloadingJson();
      }

      if (m_checkDataPointsRunningInsteadOfAnalysis &&
          m_lastGetRunningDataPointsSuccess &&
          m_monitorDataPoints->lastRunningDataPointUUIDs().empty())
      {
        // no more data points running. stop monitoring. only get here if trying to stop running.
        appendErrorsAndWarnings(*m_monitorDataPoints); // keep warnings
        m_monitorDataPoints.reset();
        checkForRunCompleteOrStopped();
      }
      else if (m_waitingQueue.empty()) {
        appendErrorsAndWarnings(*m_monitorDataPoints); // keep warnings
        m_monitorDataPoints.reset();
        checkForRunCompleteOrStopped();
      }
      else {
        LOG(Info,"Waiting on " << m_waitingQueue.size() << " DataPoints.");
        if (m_checkDataPointsRunningInsteadOfAnalysis) {
          test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointsStillRunning(bool)));
          OS_ASSERT(test);

          success = m_monitorDataPoints->requestRunningDataPointUUIDs(project().analysis().uuid());
        }
        else {
          test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStillRunning(bool)));
          OS_ASSERT(test);

          success = m_monitorDataPoints->requestIsAnalysisRunning(project().analysis().uuid());
        }
      }
    }

    if (!success) {
      registerMonitoringFailure();
    }
  }

  void CloudAnalysisDriver_Impl::analysisStillRunning(bool success) {
    bool test = m_monitorDataPoints->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStillRunning(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run failed during monitoring. Request asking if server is still running this analysis failed.");
    }

    if (success) {
      success = m_monitorDataPoints->lastIsAnalysisRunning();
      if (!success) {
        logError("Analysis stopped running on the server unexpectedly.");
      }
    }

    if (success) {
      test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(completeDataPointUUIDsReturned(bool)));
      OS_ASSERT(test);

      success = m_monitorDataPoints->requestCompleteDataPointUUIDs(project().analysis().uuid());
    }

    if (!success) {
      registerMonitoringFailure();
    }
  }

  void CloudAnalysisDriver_Impl::dataPointsStillRunning(bool success) {
    bool test = m_monitorDataPoints->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointsStillRunning(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run failed during monitoring. Request asking server for the UUIDs of any running data points failed.");
    }

    if (success) {
      // keep monitoring - will use running data point uuids after check to see if got more results
      m_lastGetRunningDataPointsSuccess = true;

      test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(completeDataPointUUIDsReturned(bool)));
      OS_ASSERT(test);

      success = m_monitorDataPoints->requestCompleteDataPointUUIDs(project().analysis().uuid());
    }

    if (!success) {
      registerMonitoringFailure();
    }
  }

  void CloudAnalysisDriver_Impl::jsonDownloadComplete(bool success) {

    if (!success) {
      logError("Unable to retrieve high level results for DataPoint '" +
               m_jsonQueue.front().name() + "', " + removeBraces(m_jsonQueue.front().uuid()) +
               " from server.");
    }

    if (success) {
      std::string json = m_requestJson->lastDataPointJSON();
      DataPoint toUpdate = m_jsonQueue.front();
      m_jsonQueue.pop_front();
      boost::optional<RunManager> rm = project().runManager();
      bool test = toUpdate.updateFromJSON(json,rm);
      project().save();
      emit dataPointComplete(project().analysis().uuid(),toUpdate.uuid());

      if (test) {
        if (toUpdate.runType() == DataPointRunType::CloudDetailed) {
          m_detailsQueue.push_back(toUpdate);
          if (!m_requestDetails) {
            startDownloadingDetails();
          }
        }
      }
      else {
        logWarning("Update of DataPoint '" + toUpdate.name() + "', " + removeBraces(toUpdate.uuid()) + " from JSON string failed.");
      }

      if (m_jsonQueue.empty()) {
        test = m_requestJson->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(jsonDownloadComplete(bool)));
        OS_ASSERT(test);
        appendErrorsAndWarnings(*m_requestJson);
        m_requestJson.reset();
        emit jsonDownloadRequestsComplete(true);
        checkForRunCompleteOrStopped();
      }
      else {
        LOG(Info,"Have " << m_jsonQueue.size() << " DataPoints' slim results to download.");
        success = requestNextJsonDownload();
      }
    }

    if (!success) {
      registerDownloadingJsonFailure();
    }
  }

  void CloudAnalysisDriver_Impl::detailsDownloadComplete(bool success) {

    if (success) {
      success = m_requestDetails->lastDownloadDataPointSuccess();
    }

    if (!success) {
      logError("Unable to download DataPoint '" + m_detailsQueue.front().name() + "', " +
               removeBraces(m_detailsQueue.front().uuid()) + " from server.");
    }

    if (success) {
      DataPoint toUpdate = m_detailsQueue.front();
      m_detailsQueue.pop_front();
      boost::optional<RunManager> rm = project().runManager();
      bool test = toUpdate.updateDetails(rm);
      project().save();
      emit dataPointDetailsComplete(project().analysis().uuid(),toUpdate.uuid());
      if (!test) {
        logWarning("Incorporation of DataPoint '" + toUpdate.name() + "', " + removeBraces(toUpdate.uuid()) + " files and details failed.");
      }

      if (m_detailsQueue.empty()) {
        test = m_requestDetails->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(detailsDownloadComplete(bool)));
        OS_ASSERT(test);
        appendErrorsAndWarnings(*m_requestDetails);
        m_requestDetails.reset();
        m_lastDownloadDetailedResultsSuccess = true;
        emit detailedDownloadRequestsComplete(true);
        checkForRunCompleteOrStopped();
      }
      else {
        LOG(Info,"Have " << m_detailsQueue.size() << " DataPoints' detailed results to download.");
        success = requestNextDetailsDownload();
      }
    }

    if (!success) {
      registerDownloadingDetailsFailure();
    }
  }

  void CloudAnalysisDriver_Impl::analysisStopped(bool success) {
    bool test = m_requestStop->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStopped(bool)));
    OS_ASSERT(test);

    if (success) {
      success = m_requestStop->lastStopSuccess();
    }

    if (!success) {
      logError("Request to stop Analysis failed.");
    }

    if (success) {
      // if nothing else going on, emit signal stating that analysis has stopped
      checkForRunCompleteOrStopped();
      // otherwise, will keep spinning until all processes cease
    }

    if (!success) {
      registerStopRequestFailure();
    }
  }

  void CloudAnalysisDriver_Impl::areResultsAvailableForDownload(bool success) {
    bool test = m_checkForResultsToDownload->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(areResultsAvailableForDownload(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Could not complete request for detailed results download because asking server for the completed DataPoint UUIDs failed.");
    }

    if (success) {
      UUIDVector temp = m_checkForResultsToDownload->lastCompleteDataPointUUIDs();
      std::set<UUID> completeUUIDs(temp.begin(),temp.end());
      DataPointVector::iterator it = m_preDetailsQueue.begin();
      while (it != m_preDetailsQueue.end()) {
        if (completeUUIDs.find(it->uuid()) != completeUUIDs.end()) {
          m_detailsQueue.push_back(*it);
          it = m_preDetailsQueue.erase(it);
        }
        else {
          ++it;
        }
      }

      if (!m_detailsQueue.empty() && !m_requestDetails) {
        startDownloadingDetails();
      }

      BOOST_FOREACH(const DataPoint& dataPoint,m_preDetailsQueue) {
        logWarning("Cannot download detailed results for DataPoint '" + dataPoint.name() +
                   "', " + removeBraces(dataPoint.uuid()) +
                   ", because it is not registered as complete on the server.");
      }

      if (!m_preDetailsQueue.empty() && !isDownloading()) {
        emit detailedDownloadRequestsComplete(false);
      }
    }

    if (!success) {
      registerDownloadDetailsRequestFailure();
    }

    m_checkForResultsToDownload.reset();
  }

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

  void CloudAnalysisDriver_Impl::registerRunRequestFailure() {
    appendErrorsAndWarnings(*m_requestRun);
    m_requestRun.reset();
    m_postQueue.clear();
    m_waitingQueue.clear();
    if (!isDownloading()) {
      m_jsonQueue.clear();
      m_detailsQueue.clear();
    }
    emit runRequestComplete(false);
  }

  bool CloudAnalysisDriver_Impl::postNextDataPoint() {
    DataPoint toQueue = m_postQueue.front();
    m_postQueue.pop_front();
    bool result = m_requestRun->startPostDataPointJSON(
          project().analysis().uuid(),
          toQueue.toJSON(DataPointSerializationOptions(project().projectDir())));
    m_waitingQueue.push_back(toQueue);
    emit dataPointQueued(project().analysis().uuid(),toQueue.uuid());
    return result;
  }

  bool CloudAnalysisDriver_Impl::startMonitoring() {
    OS_ASSERT(!m_monitorDataPoints);

    bool success(false);

    if (OptionalUrl url = provider().serverUrl()) {
      m_monitorDataPoints = OSServer(*url);

      bool test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(completeDataPointUUIDsReturned(bool)));
      OS_ASSERT(test);

      success = m_monitorDataPoints->requestCompleteDataPointUUIDs(project().analysis().uuid());
    }
    else {
      logError("Cannot start monitoring this run because the CloudProvider has been terminated.");
      emit runRequestComplete(false);
      return success;
    }

    OS_ASSERT(m_monitorDataPoints);
    if (!success) {
      registerMonitoringFailure(); // will clear download queues if not yet downloading
    }

    if (!m_jsonQueue.empty() && !m_requestJson) {
      OS_ASSERT(success);
      startDownloadingJson();
    }

    if (!m_detailsQueue.empty() && !m_requestDetails) {
      OS_ASSERT(success);
      startDownloadingDetails();
    }

    OS_ASSERT(m_requestRun);
    appendErrorsAndWarnings(*m_requestRun); // keep any warnings registered with this server
    m_requestRun.reset();

    return success;
  }

  void CloudAnalysisDriver_Impl::registerMonitoringFailure() {
    appendErrorsAndWarnings(*m_monitorDataPoints);
    m_monitorDataPoints.reset();
    OS_ASSERT(m_postQueue.empty());
    m_waitingQueue.clear();
    if (!isDownloading()) {
      m_jsonQueue.clear();
      m_detailsQueue.clear();
    }
    emit runRequestComplete(false);
  }

  bool CloudAnalysisDriver_Impl::startDownloadingJson() {
    OS_ASSERT(!m_requestJson);
    OS_ASSERT(!m_jsonQueue.empty());

    bool success(false);

    if (OptionalUrl url = provider().serverUrl()) {
      m_requestJson = OSServer(*url);

      bool test = m_requestJson->connect(SIGNAL(requestProcessed(bool)),this,SLOT(jsonDownloadComplete(bool)));
      OS_ASSERT(test);

      success = requestNextJsonDownload();
    }
    else {
      logError("Cannot start download of DataPoint because the CloudProvider has been terminated.");
      emit jsonDownloadRequestsComplete(false);
      return success;
    }

    if (!success) {
      registerDownloadingJsonFailure();
    }

    return success;
  }

  bool CloudAnalysisDriver_Impl::requestNextJsonDownload() {
    OS_ASSERT(m_requestJson);
    return m_requestJson->requestDataPointJSON(project().analysis().uuid(),
                                               m_jsonQueue.front().uuid());
  }

  void CloudAnalysisDriver_Impl::registerDownloadingJsonFailure() {
    appendErrorsAndWarnings(*m_requestJson);
    m_requestJson.reset();
    // ETH@20130830 - Not sure what to do with the queues. Am at least clearing them upon
    //     requestRun, and also requiring this driver not to be downloading when requestRun.
    emit jsonDownloadRequestsComplete(false);
  }

  bool CloudAnalysisDriver_Impl::startDownloadingDetails() {
    OS_ASSERT(!m_requestDetails);
    OS_ASSERT(!m_detailsQueue.empty());

    bool success(false);

    if (OptionalUrl url = provider().serverUrl()) {
      m_requestDetails = OSServer(*url);

      bool test = m_requestDetails->connect(SIGNAL(requestProcessed(bool)),this,SLOT(detailsDownloadComplete(bool)));
      OS_ASSERT(test);

      success = requestNextDetailsDownload();
    }
    else {
      logError("Cannot start download of DataPoint details because the CloudProvider has been terminated.");
      emit detailedDownloadRequestsComplete(false);
      return success;
    }

    if (!success) {
      registerDownloadingDetailsFailure();
    }

    return success;
  }

  bool CloudAnalysisDriver_Impl::requestNextDetailsDownload() {
    OS_ASSERT(m_requestDetails);
    DataPoint needsDetails = m_detailsQueue.front();
    openstudio::path resultsDirectory = project().projectDir() / toPath("dataPoint_" + removeBraces(needsDetails.uuid()));
    if (boost::filesystem::exists(resultsDirectory)) {
      boost::filesystem::remove_all(resultsDirectory);
    }
    boost::filesystem::create_directory(resultsDirectory);
    needsDetails.setDirectory(resultsDirectory);
    return m_requestDetails->startDownloadDataPoint(project().analysis().uuid(),
                                                    needsDetails.uuid(),
                                                    resultsDirectory / toPath("dataPoint.zip"));
  }

  void CloudAnalysisDriver_Impl::registerDownloadingDetailsFailure() {
    appendErrorsAndWarnings(*m_requestDetails);
    m_requestDetails.reset();
    // ETH@20130830 - Not sure what to do with the queues. Am at least clearing them upon
    //     requestRun, and also requiring this driver not to be downloading when requestRun.
    emit detailedDownloadRequestsComplete(false);
  }

  void CloudAnalysisDriver_Impl::registerStopRequestFailure() {
    appendErrorsAndWarnings(*m_requestStop);
    m_requestStop.reset();
    emit stopRequestComplete(false);
  }

  void CloudAnalysisDriver_Impl::registerDownloadDetailsRequestFailure() {
    appendErrorsAndWarnings(*m_checkForResultsToDownload);
    m_checkForResultsToDownload.reset();
    emit detailedDownloadRequestsComplete(false);
  }

  void CloudAnalysisDriver_Impl::checkForRunCompleteOrStopped() {
    if (!isRunning() && !isDownloading()) {
      if (isStopping()) {
        appendErrorsAndWarnings(*m_requestStop);
        m_requestStop.reset();

        m_postQueue.clear();
        m_waitingQueue.clear();
        m_jsonQueue.clear();
        m_detailsQueue.clear();
        m_lastStopSuccess = true;

        emit stopRequestComplete(true);
        emit analysisStopped(project().analysis().uuid());
      }
      else if (m_postQueue.empty() && m_waitingQueue.empty() && m_jsonQueue.empty() && m_detailsQueue.empty()) {
        m_lastRunSuccess = true;
        emit runRequestComplete(true);
        emit analysisComplete(project().analysis().uuid());
      }
    }
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

