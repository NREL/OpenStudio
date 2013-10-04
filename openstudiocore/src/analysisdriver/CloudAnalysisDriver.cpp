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
#include <project/DataPointRecord.hpp>
#include <project/DataPointRecord_Impl.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Problem.hpp>

#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/Job.hpp>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/AdvancedStatus.hpp>
#include <runmanager/lib/JSON.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/System.hpp>
#include <utilities/idf/URLSearchPath.hpp>

#include <boost/foreach.hpp>

#include <QTimer>

using namespace openstudio::analysis;
using namespace openstudio::project;
using namespace openstudio::runmanager;

namespace openstudio {
namespace analysisdriver {

namespace detail {

  CloudAnalysisDriver_Impl::CloudAnalysisDriver_Impl(const CloudSession& session,
                                                     const SimpleProject& project)
    : m_session(session),
      m_project(project),
      m_lastRunSuccess(false),
      m_lastStopSuccess(false),
      m_lastDownloadDetailedResultsSuccess(false),
      m_status(AnalysisStatus::Idle),
      m_processingQueuesInitialized(false),
      m_analysisNotRunningCount(0),
      m_maxAnalysisNotRunningCount(0),
      m_checkDataPointsRunningInsteadOfAnalysis(false),
      m_lastGetRunningDataPointsSuccess(false)
  {}

  CloudSession CloudAnalysisDriver_Impl::session() const {
    return m_session;
  }

  SimpleProject CloudAnalysisDriver_Impl::project() const {
    return m_project;
  }

  unsigned CloudAnalysisDriver_Impl::numDataPointsInIteration() const {
    return m_iteration.size();
  }

  unsigned CloudAnalysisDriver_Impl::numIncompleteDataPoints() const {
    unsigned result(0);
    if (!m_processingQueuesInitialized) {
      result = numDataPointsInIteration();
    }
    else {
      result = m_postQueue.size() +
               m_waitingQueue.size() +
               m_jsonQueue.size() +
               m_preDetailsQueue.size() +
               m_detailsQueue.size();
    }

    return result;
  }

  unsigned CloudAnalysisDriver_Impl::numCompleteDataPoints() const {
    OS_ASSERT(numIncompleteDataPoints() <= numDataPointsInIteration());
    return (numDataPointsInIteration() - numIncompleteDataPoints());
  }

  AnalysisStatus CloudAnalysisDriver_Impl::status() const
  {
    return m_status;
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
    return (m_requestJson || m_checkForResultsToDownload || m_requestDetails);
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
    m_iteration.clear();
    m_postQueue.clear();
    m_waitingQueue.clear();
    m_jsonQueue.clear();
    m_preDetailsQueue.clear();
    m_detailsQueue.clear();
    m_processingQueuesInitialized = false;
    clearErrorsAndWarnings();
    // default is for monitoring process to check if analysis is running
    // if stop, may switch to looking for running data points instead
    m_checkDataPointsRunningInsteadOfAnalysis = false;
    m_lastGetRunningDataPointsSuccess = false;

    // see if trivially complete
    m_iteration = project().analysis().dataPointsToQueue();
    if (m_iteration.empty()) {
      LOG(Info,"Nothing to run. Run request trivially successful.");
      m_lastRunSuccess = true;
      return false; // false because no signal to wait for
    }

    // make sure run type is a cloud run type
    BOOST_FOREACH(DataPoint& dataPoint,m_iteration) {
      if (dataPoint.runType() == DataPointRunType::Local) {
        dataPoint.setRunType(DataPointRunType::CloudSlim);
      }
    }

    if (OptionalUrl url = session().serverUrl()) {

      m_requestRun = OSServer(*url);

      // make sure the server is available
      LOG(Debug,"Checking that server is available.");
      bool test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(availableForRun(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      test = m_requestRun->requestAvailable();
      if (!test) {
        registerRunRequestFailure();
      }

      emit iterationProgress(numCompleteDataPoints(),numDataPointsInIteration());

      return true;
    }

    logError("Cannot start a run because the CloudSession has not been started or has been terminated.");
    return false;
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

    if (OptionalUrl url = session().serverUrl()) {

      m_requestStop = OSServer(*url);

      // request analysis to stop
      bool test = m_requestStop->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStopped(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      test = m_requestStop->requestStop(project().analysis().uuid());
      if (!test) {
        registerStopRequestFailure();
      }

      return true;
    }

    logError("Cannot stop the run because the CloudSession has not been started or has been terminated.");
    return false;
  }

  bool CloudAnalysisDriver_Impl::requestDownloadDetailedResults(analysis::DataPoint& dataPoint) {

    if (isStopping()) {
      logWarning("A download cannot be requested while the analysis is stopping.");
      return false;
    }

    OptionalDataPoint actualDataPoint = project().analysis().getDataPointByUUID(dataPoint.uuid());
    // data point must exist and must not have details yet
    if (!actualDataPoint || !actualDataPoint->directory().empty()) {
      return false;
    }

    if (!actualDataPoint->complete()) {
      // then needs to be actively running on the cloud to continue
      bool keepGoing(false);
      if (isRunning() || isDownloading()) {
        keepGoing = inIteration(*actualDataPoint);
      }
      if (!keepGoing) {
        return false;
      }
    }

    // this is probably already set, but just to be sure
    actualDataPoint->setRunType(DataPointRunType::CloudDetailed);

    m_lastDownloadDetailedResultsSuccess = false;
    if (!(isRunning() || isDownloading())) {
      clearErrorsAndWarnings();
    }

    // see if already in process, in which case, the right thing should happen automatically
    bool found = inProcessingQueues(*actualDataPoint);
  
    // if not, see if there are results on the server
    bool success = true;
    if (!found) {
      LOG(Debug,"Adding DataPoint '" << actualDataPoint->name() << "' to the pre-download "
          << "details queue.");
      m_preDetailsQueue.push_back(*actualDataPoint);

      if (!m_checkForResultsToDownload) {
        success = success && startDetailsReadyMonitoring();
      }
    }

    if (!success) {
      logError("Was unable to request which data points are ready for download.");
      registerDownloadingDetailsFailure();
    }

    return success;
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
      LOG(Debug,"Server is available.");

      // see if the project needs to be created
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(projectOnServer(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      LOG(Debug,"Requesting project UUIDs.");
      success = m_requestRun->requestProjectUUIDs();
    }

    if (!success) {
      registerRunRequestFailure();
    }
  }

  void CloudAnalysisDriver_Impl::projectOnServer(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(projectOnServer(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run request failed on checking if the project is already on the server.");
    }

    if (success) {
      UUIDVector projects = m_requestRun->lastProjectUUIDs();

      if (std::find(projects.begin(),
                    projects.end(),
                    project().projectDatabase().handle()) == projects.end()) 
      {
        LOG(Debug,"Project is not yet on server, create it.");
        // project is not yet on server, create it
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(projectCreated(bool)),Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->requestCreateProject(project().projectDatabase().handle());
      }
      else {
        LOG(Debug,"Project is on server, see if analysis is on server.");
        // see if the analysis needs to be posted
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisOnServer(bool)),Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->requestAnalysisUUIDs(project().projectDatabase().handle());
      }
    }

    if (!success) {
      registerRunRequestFailure();
    }
  }
  
  void CloudAnalysisDriver_Impl::projectCreated(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(projectCreated(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run request failed on trying to create project.");
    }

    if (success) {
      success = m_requestRun->lastCreateProjectSuccess();
      if (!success) {
        logError("Run request failed because creating the project failed.");
      }
    }

    if (success) {
      LOG(Debug,"Successfully created the project. Now post the analysis.");
      // project was not on the server, so analysis can't be either, post it.
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisPosted(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      success = m_requestRun->startPostAnalysisJSON(
            project().projectDatabase().handle(),
            project().analysis().toJSON(AnalysisSerializationOptions(project().projectDir())));
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
        LOG(Debug,"The analysis is not on the server, so post it.");
        // analysis not found -- post it
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisPosted(bool)),Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->startPostAnalysisJSON(
              project().projectDatabase().handle(),
              project().analysis().toJSON(AnalysisSerializationOptions(project().projectDir())));
      }
      else {
        LOG(Debug,"The analysis is on the server, see if there are data points there.");
        // analysis found -- see if there are any data points already on the server
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(allDataPointUUIDsReturned(bool)),Qt::QueuedConnection);
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
      LOG(Debug,"The analysis was posted, now upload its files.");
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisUploaded(bool)),Qt::QueuedConnection);
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
        LOG(Debug,"There are not data points, go ahead and upload the analysis files.");
        // no data points posted yet, upload the analysis files
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisUploaded(bool)),Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->startUploadAnalysisFiles(project().analysis().uuid(),
                                                         project().zipFileForCloud());
      }
      else {
        LOG(Debug,"There are data points. Get some more information so the queues can be sorted out.");
        // there are data points, sort out all the queues--have list of all data points
        // server knows about. need list of complete data points to determine waiting versus
        // downloading queues
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(readyToSortOutQueues(bool)),Qt::QueuedConnection);
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
      LOG(Debug,"The analysis files were successfully uploaded. Start posting data points.");
      // start posting DataPoints
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointQueued(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      // initialize queue
      OS_ASSERT(!m_processingQueuesInitialized);
      OS_ASSERT(numCompleteDataPoints() == 0u);
      m_postQueue = std::deque<DataPoint>(m_iteration.begin(),m_iteration.end());
      m_processingQueuesInitialized = true;
      OS_ASSERT(!m_postQueue.empty());
      OS_ASSERT(numCompleteDataPoints() == 0u);

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
      LOG(Debug,"Have the server data point information we need. Now sort out the client-side queues.");
      UUIDVector allUUIDs = m_requestRun->lastDataPointUUIDs();
      UUIDVector completeUUIDs = m_requestRun->lastCompleteDataPointUUIDs();

      BOOST_FOREACH(const DataPoint& missingPoint, m_iteration) {
        if (std::find(allUUIDs.begin(),allUUIDs.end(),missingPoint.uuid()) == allUUIDs.end()) {
          // missingPoint is not on the server. add it to the ...
          // post queue -- need to be run and are not in allUUIDs
          m_postQueue.push_back(missingPoint);
        }
        else if (std::find(completeUUIDs.begin(),completeUUIDs.end(),missingPoint.uuid()) == completeUUIDs.end()) {
          // missingPoint is on the server, but is not complete, add it to the ...
          // waiting queue -- need to be run and are not in completeUUIDs
          m_waitingQueue.push_back(missingPoint);
        }
        else {
          // missingPoint is complete on the server. add it to the ...
          // json queue -- need to be run and are in completeUUIDs
          m_jsonQueue.push_back(missingPoint);
        }
      }

      BOOST_FOREACH(const DataPoint& missingDetails, project().analysis().dataPointsNeedingDetails()) {
        if (std::find(completeUUIDs.begin(),completeUUIDs.end(),missingDetails.uuid()) != completeUUIDs.end()) {
          // details queue -- are complete, but details were reqeusted and have not yet been downloaded
          OS_ASSERT(std::find(m_iteration.begin(),m_iteration.end(),missingDetails) == m_iteration.end());
          m_iteration.push_back(missingDetails);
          m_preDetailsQueue.push_back(missingDetails);
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

      m_processingQueuesInitialized = true;

      // restart run process
      if (m_postQueue.size() > 0) {
        LOG(Debug,"Have some data points to post, so do it.");
        // start posting DataPoints
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointQueued(bool)),Qt::QueuedConnection);
        OS_ASSERT(test);

        success = postNextDataPoint();
      }
      else if (m_waitingQueue.size() > 0) {
        LOG(Debug,"No data points need to be posted, so see if the analysis is already running.");
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisRunningOnServer(bool)),Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->requestIsAnalysisRunning(project().analysis().uuid());
      }
      
      // restart download process(es)
      if (!m_jsonQueue.empty()) {
        OS_ASSERT(!m_requestJson);
        startDownloadingJson();
      }
      if (!m_preDetailsQueue.empty()) {
        OS_ASSERT(!m_checkForResultsToDownload && !m_requestDetails);
        startDownloadingDetails();
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
      if (success) {

        OS_ASSERT(!m_waitingQueue.empty());
        DataPoint lastQueued = m_waitingQueue.back();
        boost::optional<Job> topLevelJob = lastQueued.topLevelJob();
        OS_ASSERT(topLevelJob);
        topLevelJob->setStatus(AdvancedStatusEnum(AdvancedStatusEnum::WaitingInQueue));
        // no need to call updateJob because we have the same job that is in the database
        project().save();

      }else{
        logError("Run request failed because a DataPoint JSON did not post successfully.");
      }
    }

    if (success) {
      LOG(Debug,"Data point successfully posted.");
      // see if you make another post or go on to starting the analysis
      if (m_postQueue.empty()) {
        LOG(Debug,"All done posting data points, see if this analysis is already running on the server.");
        // done posting --move on
        bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointQueued(bool)));
        OS_ASSERT(test);

        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisRunningOnServer(bool)),Qt::QueuedConnection);
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
        LOG(Debug,"The analysis is already running, so monitor progress.");
        // analysis is already running, start monitoring data points (this process will also
        // start downloading results if either of those queues are not empty).
        startMonitoring();
      }
      else {
        LOG(Debug,"The analysis is not running. Start it.");
        // start the analysis
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStarted(bool)),Qt::QueuedConnection);
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
      LOG(Debug,"Start waiting for the server to report that the analysis has started running.");
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(waitingForAnalysisToStart(bool)),Qt::QueuedConnection);

      success = m_requestRun->requestIsAnalysisRunning(project().analysis().uuid());

      m_analysisNotRunningCount = 0;
      m_maxAnalysisNotRunningCount = 20 + 2 * m_waitingQueue.size();
    }

    if (!success) {
      registerRunRequestFailure();
    }
  }

  void CloudAnalysisDriver_Impl::waitingForAnalysisToStart(bool success) {
    if (!success) {
      logError("Run request failed on waiting for the analysis to start running on the server.");
    }

    if (success) {
      bool isRunning = m_requestRun->lastIsAnalysisRunning();
      ++m_analysisNotRunningCount;
      
      if (isRunning) {
        LOG(Debug,"The analysis was started in " << m_analysisNotRunningCount << " tries. Monitor it.");
        
        bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(waitingForAnalysisToStart(bool)));
        OS_ASSERT(test);

        startMonitoring();
      }
      else {
        if (m_analysisNotRunningCount >= m_maxAnalysisNotRunningCount) {
          std::stringstream ss;
          ss << "Analysis failed to start running within " << m_maxAnalysisNotRunningCount << " tries.";
          logError(ss.str());
          success = false;
        }
        else {
          System::msleep(1000); // wait 1 second
          success = m_requestRun->requestIsAnalysisRunning(project().analysis().uuid());
        }
      }
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
      LOG(Debug,"Received reply to request for complete data point uuids. There are " 
          << completeUUIDs.size() << ".");
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
        LOG(Debug,"Starting Json downloading.");
        startDownloadingJson();
      }

      if (m_checkDataPointsRunningInsteadOfAnalysis &&
          m_lastGetRunningDataPointsSuccess &&
          m_monitorDataPoints->lastRunningDataPointUUIDs().empty())
      {
        // no more data points running. stop monitoring. only get here if trying to stop running.
        LOG(Debug,"No more data points are running. Stop monitoring.");
        appendErrorsAndWarnings(*m_monitorDataPoints); // keep warnings
        m_monitorDataPoints.reset();
        checkForRunCompleteOrStopped();
      }
      else if (m_waitingQueue.empty()) {
        LOG(Debug,"All DataPoints complete. Stop monitoring.");
        appendErrorsAndWarnings(*m_monitorDataPoints); // keep warnings
        m_monitorDataPoints.reset();
        checkForRunCompleteOrStopped();
      }
      else {
        LOG(Info,"Waiting on " << m_waitingQueue.size() << " DataPoints.");
        if (m_checkDataPointsRunningInsteadOfAnalysis) {
          test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointsStillRunning(bool)),Qt::QueuedConnection);
          OS_ASSERT(test);

          System::msleep(1000); // wait 1 second
          success = m_monitorDataPoints->requestRunningDataPointUUIDs(project().analysis().uuid());
        }
        else {
          test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStillRunning(bool)),Qt::QueuedConnection);
          OS_ASSERT(test);

          System::msleep(1000); // wait 1 second
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
      LOG(Debug,"The analysis is still running. Ask for complete data point uuids.");
      test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(completeDataPointUUIDsReturned(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      System::msleep(1000); // wait 1 second
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

      test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(completeDataPointUUIDsReturned(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      System::msleep(1000); // wait 1 second
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
      LOG(Debug,"Downloaded Json file for DataPoint '" << toUpdate.name() << "'.");
      m_jsonQueue.pop_front();
      boost::optional<RunManager> rm = project().runManager();
      bool test = toUpdate.updateFromJSON(json,rm);
      project().save();
      emit resultsChanged();
      emit dataPointComplete(project().analysis().uuid(),toUpdate.uuid());
      OS_ASSERT(toUpdate.runType() != DataPointRunType::Local);
      if (toUpdate.runType() == DataPointRunType::CloudSlim) {
        emit iterationProgress(numCompleteDataPoints(),numDataPointsInIteration());
      }

      if (test) {
        if (toUpdate.runType() == DataPointRunType::CloudDetailed) {
          m_preDetailsQueue.push_back(toUpdate);
          if (!(m_checkForResultsToDownload || m_requestDetails)) {
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

  void CloudAnalysisDriver_Impl::readyForDownloadDataPointUUIDsReturned(bool success) {
    if (!success) {
      logError("Download failed while asking server for which DataPoints can be downloaded.");
    }

    if (success) {
      UUIDVector temp = m_checkForResultsToDownload->lastDownloadReadyDataPointUUIDs();
      std::set<UUID> readyUUIDs(temp.begin(),temp.end());
      LOG(Debug,"Received reply to request for data points ready for download. There are "
          << readyUUIDs.size() << ".");
      DataPointVector::iterator it = m_preDetailsQueue.begin();
      while (it != m_preDetailsQueue.end()) {
        if (readyUUIDs.find(it->uuid()) != readyUUIDs.end()) {
          m_detailsQueue.push_back(*it);
          it = m_preDetailsQueue.erase(it);
        }
        else {
          ++it;
        }
      }

      if (!m_detailsQueue.empty() && !m_requestDetails) {
        LOG(Debug,"Start details downloading.");
        success = startActualDownloads();
      }

      if (m_preDetailsQueue.empty()) {
        LOG(Debug,"All DataPoints whose details we want have details available for download.");
        bool test = m_checkForResultsToDownload->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(readyForDownloadDataPointUUIDsReturned(bool)));
        OS_ASSERT(test);
        appendErrorsAndWarnings(*m_checkForResultsToDownload);
        m_checkForResultsToDownload.reset();
        checkForRunCompleteOrStopped();
      }
      else {
        // HERE -- Make sure it is reasonable to ask for this data point's details at all!
        // Ask for the complete DataPoint UUIDs and remove points from this queue that aren't there.
        LOG(Info,"Waiting on detailed results for " << m_preDetailsQueue.size() << " DataPoints.");
        System::msleep(1000); // wait 1 second
        success = success && m_checkForResultsToDownload->requestDownloadReadyDataPointUUIDs(project().analysis().uuid());
      }
    }

    if (!success) {
      registerDownloadingDetailsFailure();
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
      boost::optional<RunManager> rm = project().runManager();
      LOG(Debug,"Getting detailed results for DataPoint '" << toUpdate.name() << "'.");
      bool test = toUpdate.updateDetails(rm);
      project().save();
      m_detailsQueue.pop_front();
      emit resultsChanged();
      emit dataPointDetailsComplete(project().analysis().uuid(),toUpdate.uuid());
      emit iterationProgress(numCompleteDataPoints(),numDataPointsInIteration());
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

      if (!m_checkForResultsToDownload && !m_preDetailsQueue.empty()) {
        success = success && startDetailsReadyMonitoring();
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

  void CloudAnalysisDriver_Impl::setStatus(AnalysisStatus status) {
    if (m_status != status){
      m_status = status;
      emit analysisStatusChanged(m_status);
    }
  }

  void CloudAnalysisDriver_Impl::registerRunRequestFailure() {
    appendErrorsAndWarnings(*m_requestRun);
    m_requestRun.reset();
    emit runRequestComplete(false);
  }

  bool CloudAnalysisDriver_Impl::postNextDataPoint() {
    DataPoint toQueue = m_postQueue.front();
    if (toQueue.runType() == DataPointRunType::Local) {
      toQueue.setRunType(DataPointRunType::CloudSlim);
    }
    m_postQueue.pop_front();

    // get json before we add the dummy job
    std::string toQueueJson = toQueue.toJSON(DataPointSerializationOptions(project().projectDir()));
    bool result = m_requestRun->startPostDataPointJSON(project().analysis().uuid(), toQueueJson);

    // DLM: what to do if result is false?

    if (result){

      m_waitingQueue.push_back(toQueue);

      // here we are going to create a dummy job to attach to the datapoint for saving job state
      runmanager::Workflow workflow = project().analysis().problem().createWorkflow(toQueue,openstudio::path());

      // DLM: need to make this workflow's hash appear unique to run manager or it will not enqueue it
      JobParams dataPointUUID;
      dataPointUUID.append("dataPointUUID", openstudio::toString(toQueue.uuid()));
      workflow.add(dataPointUUID);

      runmanager::Job job = workflow.create(openstudio::path(),
                                            openstudio::path(),
                                            openstudio::path(),
                                            std::vector<URLSearchPath>());
      runmanager::JobFactory::optimizeJobTree(job);
      
      // can only set status on externally managed jobs
      job.makeExternallyManaged();
      job.setStatus(AdvancedStatusEnum(AdvancedStatusEnum::Queuing));

      // add job to database
      project().runManager().enqueue(job, true);

      // attach externalJob to datapoint
      bool test = project().analysis().setDataPointRunInformation(toQueue, job, std::vector<openstudio::path>());
      OS_ASSERT(test);

      // make sure things are good
      boost::optional<Job> topLevelJob = toQueue.topLevelJob();
      OS_ASSERT(topLevelJob);
      OS_ASSERT(topLevelJob->uuid() == job.uuid());
      OS_ASSERT(topLevelJob->externallyManaged());
      try{
        project().runManager().getJob(topLevelJob->uuid());
      }catch (const std::out_of_range &) {
        std::cout << "UUID is " << toString(job.uuid()) << std::endl;
        std::cout << "DB has " << std::endl;
        BOOST_FOREACH(runmanager::Job j, project().runManager().getJobs()){
          std::cout << "  " << toString(j.uuid()) << std::endl;
        }
        OS_ASSERT(false);
      }

      project().save();

      emit dataPointQueued(project().analysis().uuid(),toQueue.uuid());
    }

    return result;
  }

  bool CloudAnalysisDriver_Impl::startMonitoring() {
    OS_ASSERT(!m_monitorDataPoints);

    bool success(false);

    if (OptionalUrl url = session().serverUrl()) {
      m_monitorDataPoints = OSServer(*url);

      bool test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(completeDataPointUUIDsReturned(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      success = m_monitorDataPoints->requestCompleteDataPointUUIDs(project().analysis().uuid());
    }
    else {
      logError("Cannot start monitoring this run because the CloudSession has been terminated.");
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

    if (!(m_preDetailsQueue.empty() && m_detailsQueue.empty()) &&
        !m_checkForResultsToDownload && !m_requestDetails)
    {
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
    emit runRequestComplete(false);
  }

  bool CloudAnalysisDriver_Impl::startDownloadingJson() {
    OS_ASSERT(!m_requestJson);
    OS_ASSERT(!m_jsonQueue.empty());

    bool success(false);

    if (OptionalUrl url = session().serverUrl()) {
      m_requestJson = OSServer(*url);

      bool test = m_requestJson->connect(SIGNAL(requestProcessed(bool)),this,SLOT(jsonDownloadComplete(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      success = requestNextJsonDownload();
    }
    else {
      logError("Cannot start download of DataPoint because the CloudSession has been terminated.");
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
    OS_ASSERT(!m_checkForResultsToDownload);
    OS_ASSERT(!m_requestDetails);
    OS_ASSERT(!(m_preDetailsQueue.empty() && m_detailsQueue.empty()));

    bool success(true);

    if (!m_preDetailsQueue.empty()) {
      success = success && startDetailsReadyMonitoring();
    }

    if (!m_detailsQueue.empty()) {
      success = success && startActualDownloads();
    }

    if (!success) {
      registerDownloadingDetailsFailure();
    }

    return success;
  }

  bool CloudAnalysisDriver_Impl::startDetailsReadyMonitoring() {
    OS_ASSERT(!m_checkForResultsToDownload);
    OS_ASSERT(!m_preDetailsQueue.empty());

    bool success(false);

    if (OptionalUrl url = session().serverUrl()) {
      m_checkForResultsToDownload = OSServer(*url);
      LOG(Debug,"Spinning up process to check for data points that are ready to download.");

      bool test = m_checkForResultsToDownload->connect(SIGNAL(requestProcessed(bool)),this,SLOT(readyForDownloadDataPointUUIDsReturned(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      success = m_checkForResultsToDownload->requestDownloadReadyDataPointUUIDs(project().analysis().uuid());
    }
    else {
      logError("Cannot start monitoring for data points that are ready to download because the CloudSession has been terminated.");
      emit detailedDownloadRequestsComplete(false);
    }

    // don't register failure here. calling method will do so.

    return success;
  }

  bool CloudAnalysisDriver_Impl::startActualDownloads() {
    OS_ASSERT(!m_requestDetails);
    OS_ASSERT(!m_detailsQueue.empty());

    bool success(false);

    if (OptionalUrl url = session().serverUrl()) {
      m_requestDetails = OSServer(*url);

      bool test = m_requestDetails->connect(SIGNAL(requestProcessed(bool)),this,SLOT(detailsDownloadComplete(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      success = requestNextDetailsDownload();
    }
    else {
      logError("Cannot start downloading data point details because the CloudSession has been terminated.");
      emit detailedDownloadRequestsComplete(false);
    }

    // don't register failure here. calling method will do so.

    return success;
  }

  bool CloudAnalysisDriver_Impl::requestNextDetailsDownload() {
    OS_ASSERT(m_requestDetails);
    DataPoint needsDetails = m_detailsQueue.front();
    openstudio::path dataPointFolderName = toPath("dataPoint_" + removeBraces(needsDetails.uuid()));
    if (OptionalDataPointRecord dataPointRecord = project().projectDatabase().getObjectRecordByHandle<DataPointRecord>(needsDetails.uuid())) {
      std::stringstream ss;
      ss << "dataPoint" << dataPointRecord->id();
      dataPointFolderName = toPath(ss.str());
    }
    else {
      LOG(Debug,"DataPoint not in ProjectDatabase by UUID. This is highly unexpected.");
    }
    openstudio::path resultsDirectory = project().projectDir() / dataPointFolderName;
    LOG(Debug,"Trying to get DataPoint '" << needsDetails.name() << "' details and put them in '" 
        << toString(resultsDirectory) << "'.");
    if (boost::filesystem::exists(resultsDirectory)) {
      boost::filesystem::remove_all(resultsDirectory);
    }
    boost::filesystem::create_directory(resultsDirectory);
    needsDetails.setDirectory(resultsDirectory);
    project().save();
    emit resultsChanged();
    return m_requestDetails->startDownloadDataPoint(project().analysis().uuid(),
                                                    needsDetails.uuid(),
                                                    resultsDirectory / toPath("dataPoint.zip"));
  }

  void CloudAnalysisDriver_Impl::registerDownloadingDetailsFailure() {
    if (m_checkForResultsToDownload) {
      appendErrorsAndWarnings(*m_checkForResultsToDownload);
      m_checkForResultsToDownload.reset();
    }
    if (m_requestDetails) {
      appendErrorsAndWarnings(*m_requestDetails);
      m_requestDetails.reset();
    }
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
        m_lastStopSuccess = true;

        emit stopRequestComplete(true);
        emit analysisStopped(project().analysis().uuid());
      }
      else if (m_postQueue.empty() && m_waitingQueue.empty() && m_jsonQueue.empty() && m_preDetailsQueue.empty() && m_detailsQueue.empty()) {
        if (numIncompleteDataPoints() != 0u) {
          LOG(Debug,"Run appears to have ended cleanly, but number of complete data points is " 
              << numCompleteDataPoints() << " when expected " << numDataPointsInIteration()
              << " (total number of points in run).");
        }
        m_lastRunSuccess = true;
        emit runRequestComplete(true);
        emit analysisComplete(project().analysis().uuid());
      }
    }
  }

  bool CloudAnalysisDriver_Impl::inIteration(const DataPoint &dataPoint) const {
    return !(std::find(m_iteration.begin(),m_iteration.end(),dataPoint) == m_iteration.end());
  }

  bool CloudAnalysisDriver_Impl::inProcessingQueues(const DataPoint &dataPoint) const {
    if (std::find(m_postQueue.begin(),m_postQueue.end(),dataPoint) != m_postQueue.end()) {
      return true;
    }
    if (std::find(m_waitingQueue.begin(),m_waitingQueue.end(),dataPoint) != m_waitingQueue.end()) {
      return true;
    }
    if (std::find(m_jsonQueue.begin(),m_jsonQueue.end(),dataPoint) != m_jsonQueue.end()) {
      return true;
    }
    if (std::find(m_preDetailsQueue.begin(),m_preDetailsQueue.end(),dataPoint) != m_preDetailsQueue.end()) {
      return true;
    }
    if (std::find(m_detailsQueue.begin(),m_detailsQueue.end(),dataPoint) != m_detailsQueue.end()) {
      return true;
    }
    return false;
  }

} // detail

CloudAnalysisDriver::CloudAnalysisDriver(const CloudSession& session,
                                         const SimpleProject& project)
  : m_impl(boost::shared_ptr<detail::CloudAnalysisDriver_Impl>(
             new detail::CloudAnalysisDriver_Impl(session,project)))
{}

CloudSession CloudAnalysisDriver::session() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->session();
}

SimpleProject CloudAnalysisDriver::project() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->project();
}

unsigned CloudAnalysisDriver::numDataPointsInIteration() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->numDataPointsInIteration();
}

unsigned CloudAnalysisDriver::numIncompleteDataPoints() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->numIncompleteDataPoints();
}

unsigned CloudAnalysisDriver::numCompleteDataPoints() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->numCompleteDataPoints();
}

AnalysisStatus CloudAnalysisDriver::status() const
{
  return getImpl<detail::CloudAnalysisDriver_Impl>()->status();
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

bool CloudAnalysisDriver::isStopping() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->isStopping();
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

