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

#include "CloudAnalysisDriver.hpp"
#include "CloudAnalysisDriver_Impl.hpp"

#include "../project/ProjectDatabase.hpp"
#include "../project/DataPointRecord.hpp"
#include "../project/DataPointRecord_Impl.hpp"

#include "../analysis/Analysis.hpp"
#include "../analysis/Problem.hpp"

#include "../runmanager/lib/RunManager.hpp"
#include "../runmanager/lib/Job.hpp"
#include "../runmanager/lib/Workflow.hpp"
#include "../runmanager/lib/AdvancedStatus.hpp"
#include "../runmanager/lib/JSON.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/System.hpp"
#include "../utilities/data/Tag.hpp"
#include "../utilities/idf/URLSearchPath.hpp"

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
      m_batchSize(0),
      m_analysisNotRunningCount(0),
      m_maxAnalysisNotRunningCount(0),
      m_dataPointsNotRunningCount(0),
      m_maxDataPointsNotRunningCount(0),
      m_numJsonTries(0),
      m_onlyProcessingDownloadRequests(true),
      m_noNewReadyDataPointsCount(0),
      m_numDetailsTries(0),
      m_waitForAlreadyRunningDataPoints(false),
      m_numDeleteDataPointTries(0),
      m_needToDeleteProject(false),
      m_numDeleteProjectTries(0)
  {
    bool connected;
    connected = m_project.analysis().connect(SIGNAL(removedDataPoint(const openstudio::UUID&)),
                                             this,
                                             SLOT(dataPointOrItsResultsRemovedFromAnalysis(const openstudio::UUID&)));
    OS_ASSERT(connected);

    connected = m_project.analysis().connect(SIGNAL(clearedDataPointResults(const openstudio::UUID&)),
                                             this,
                                             SLOT(dataPointOrItsResultsRemovedFromAnalysis(const openstudio::UUID&)));
    OS_ASSERT(connected);

    connected = m_project.analysis().connect(SIGNAL(removedAllDataPoints()),
                                             this,
                                             SLOT(allDataPointsOrResultsRemovedFromAnalysis()));
    OS_ASSERT(connected);

    connected = m_project.analysis().connect(SIGNAL(clearedAllResults()),
                                             this,
                                             SLOT(allDataPointsOrResultsRemovedFromAnalysis()));
    OS_ASSERT(connected);
  }

  CloudSession CloudAnalysisDriver_Impl::session() const {
    return m_session;
  }

  SimpleProject CloudAnalysisDriver_Impl::project() const {
    return m_project;
  }

  AnalysisStatus CloudAnalysisDriver_Impl::status() const
  {
    return m_status;
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
               m_runningQueue.size() +
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

  unsigned CloudAnalysisDriver_Impl::numFailedDataPoints() const {
    unsigned result=0;
    for (const DataPoint& dataPoint : m_iteration) {
      if (dataPoint.complete() && dataPoint.failed()) {
        ++result;
      }
    }
    return result;
  }

  std::vector<analysis::DataPoint> CloudAnalysisDriver_Impl::failedJsonDownloads() const {
    return m_jsonFailures;
  }

  std::vector<analysis::DataPoint> CloudAnalysisDriver_Impl::failedDetailedDownloads() const {
    return m_detailsFailures;
  }

  bool CloudAnalysisDriver_Impl::inSession(const DataPoint &dataPoint) const {
    return dataPoint.isTag(sessionTag());
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
    return (m_requestStop || m_requestDeleteProject);
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
    m_onlyProcessingDownloadRequests = false;
    resetState();

    // see if trivially complete
    m_iteration = project().analysis().dataPointsToQueue();
    DataPointVector missingDetails = project().analysis().dataPointsNeedingDetails();
    m_iteration.insert(m_iteration.end(),missingDetails.begin(),missingDetails.end());
    if (m_iteration.empty()) {
      LOG(Info,"Nothing to run or download. Run request trivially successful.");
      m_lastRunSuccess = true;
      return false; // false because no signal to wait for
    }

    // make sure run type is a cloud run type
    for (DataPoint& dataPoint : m_iteration) {
      if (dataPoint.runType() == DataPointRunType::Local) {
        dataPoint.setRunType(DataPointRunType::CloudSlim);
      }
      clearSessionTags(dataPoint);
      dataPoint.addTag(sessionTag());
    }

    if (OptionalUrl url = session().serverUrl()) {

      m_requestRun = OSServer(*url);

      setStatus(AnalysisStatus::Starting);

      // make sure the server is available
      LOG(Debug,"Checking that server is available.");
      bool test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                        this,
                                        SLOT(availableForRun(bool)),
                                        Qt::QueuedConnection);
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
    m_waitForAlreadyRunningDataPoints = waitForAlreadyRunningDataPoints;

    if (OptionalUrl url = session().serverUrl()) {

      m_requestStop = OSServer(*url);

      setStatus(AnalysisStatus::Stopping);

      // request analysis to stop
      bool test = m_requestStop->connect(SIGNAL(requestProcessed(bool)),
                                         this,
                                         SLOT(analysisStopped(bool)),
                                         Qt::QueuedConnection);
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
    if (!actualDataPoint || !actualDataPoint->directory().empty() || !inSession(*actualDataPoint)) {
      return false;
    }

    if (!actualDataPoint->complete()) {
      // then needs to be actively running on the cloud to continue
      bool keepGoing(false);
      if ((!m_onlyProcessingDownloadRequests) && (isRunning() || isDownloading())) {
        keepGoing = inIteration(*actualDataPoint);
      }
      if (!keepGoing) {
        return false;
      }
    }

    // this is probably already set, but just to be sure
    actualDataPoint->setRunType(DataPointRunType::CloudDetailed);

    if (!(isRunning() || isDownloading())) {
      // if not running or downloading rignt now, can be sure that only downloading is
      // going to be going on
      m_onlyProcessingDownloadRequests = true;
    }

    if (m_onlyProcessingDownloadRequests) {
      m_lastDownloadDetailedResultsSuccess = false;
    }

    bool found(false);
    if (m_onlyProcessingDownloadRequests && !isDownloading()) {
      resetState();
      setStatus(AnalysisStatus::Running);
    }
    else {
      // see if already in process, in which case, the right thing should happen automatically
      found = inIteration(*actualDataPoint) &&
              (!m_processingQueuesInitialized || inProcessingQueues(*actualDataPoint));
    }
  
    // if not, see if there are results on the server
    bool success(true);
    if (!found) {
      LOG(Debug,"Adding DataPoint '" << actualDataPoint->name() << "' to the pre-download "
          << "details queue.");
      m_preDetailsQueue.push_back(*actualDataPoint);
      if (!inIteration(*actualDataPoint)) {
        m_iteration.push_back(*actualDataPoint);
      }
      else {
        // if failed before, trying again now, so un-register the failure
        auto it = std::find(m_detailsFailures.begin(),m_detailsFailures.end(),actualDataPoint);
        if (it != m_detailsFailures.end()) {
          m_detailsFailures.erase(it);
        }
      }

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
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                   this,
                                   SLOT(projectOnServer(bool)),
                                   Qt::QueuedConnection);
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
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                     this,
                                     SLOT(projectCreated(bool)),
                                     Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->requestCreateProject(project().projectDatabase().handle());
      }
      else {
        LOG(Debug,"Project is on server, see if analysis is on server.");
        // see if the analysis needs to be posted
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                     this,
                                     SLOT(analysisOnServer(bool)),
                                     Qt::QueuedConnection);
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
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                   this,
                                   SLOT(analysisPosted(bool)),
                                   Qt::QueuedConnection);
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
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                     this,
                                     SLOT(analysisPosted(bool)),
                                     Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->startPostAnalysisJSON(
              project().projectDatabase().handle(),
              project().analysis().toJSON(AnalysisSerializationOptions(project().projectDir())));
      }
      else {
        LOG(Debug,"The analysis is on the server, see if there are data points there.");
        // analysis found -- see if there are any data points already on the server
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                     this,
                                     SLOT(allDataPointUUIDsReturned(bool)),
                                     Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->requestDataPointUUIDs(project().analysis().uuid());
      }
    }

    if (!success) {
      registerRunRequestFailure();
    }
  }

  void CloudAnalysisDriver_Impl::analysisPosted(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),
                                         this,
                                         SLOT(analysisPosted(bool)));
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
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                   this,
                                   SLOT(analysisUploaded(bool)),
                                   Qt::QueuedConnection);
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
        LOG(Debug,"There are no data points, go ahead and upload the analysis files.");
        // no data points posted yet, upload the analysis files
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                     this,
                                     SLOT(analysisUploaded(bool)),
                                     Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->startUploadAnalysisFiles(project().analysis().uuid(),
                                                         project().zipFileForCloud());
      }
      else {
        LOG(Debug,"There are data points. Get some more information so the queues can be sorted out.");
        // there are data points, sort out all the queues--have list of all data points
        // server knows about. need list of complete data points to determine waiting versus
        // downloading queues
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                     this,
                                     SLOT(readyToSortOutQueues(bool)),
                                     Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->requestCompleteDataPointUUIDs(project().analysis().uuid());
      }
    }

    if (!success) {
      registerRunRequestFailure();
    }
  }

  void CloudAnalysisDriver_Impl::analysisUploaded(bool success) {
    bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),
                                         this,
                                         SLOT(analysisUploaded(bool)));
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
      test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                   this,
                                   SLOT(dataPointsQueued(bool)),
                                   Qt::QueuedConnection);
      OS_ASSERT(test);

      // initialize queue
      OS_ASSERT(!m_processingQueuesInitialized);
      OS_ASSERT(numCompleteDataPoints() == 0u);
      m_postQueue = std::deque<DataPoint>(m_iteration.begin(),m_iteration.end());
      m_processingQueuesInitialized = true;
      OS_ASSERT(!m_postQueue.empty());
      OS_ASSERT(numCompleteDataPoints() == 0u);

      success = postNextDataPointBatch();
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

      for (const DataPoint& missingPoint : m_iteration) {
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

      for (const DataPoint& missingDetails : project().analysis().dataPointsNeedingDetails()) {
        if (std::find(completeUUIDs.begin(),completeUUIDs.end(),missingDetails.uuid()) != completeUUIDs.end()) {
          // details queue -- are complete, but details were requested and have not yet been downloaded
          if (!inIteration(missingDetails)) {
            m_iteration.push_back(missingDetails);
          }
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
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                     this,
                                     SLOT(dataPointsQueued(bool)),
                                     Qt::QueuedConnection);
        OS_ASSERT(test);

        success = postNextDataPointBatch();
      }
      else if (m_waitingQueue.size() > 0) {
        LOG(Debug,"No data points need to be posted, so see if the analysis is already running.");
        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),
                                     this,
                                     SLOT(analysisRunningOnServer(bool)),
                                     Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->requestIsAnalysisRunning(project().analysis().uuid());
      }
      else {
        // no running to do, just get results
        appendErrorsAndWarnings(*m_requestRun);
        m_requestRun.reset();
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

      checkForRunCompleteOrStopped();
    }

    if (!success) {
      registerRunRequestFailure();
    }

  }

  void CloudAnalysisDriver_Impl::dataPointsQueued(bool success) {

    if (!success) {
      logError("Run request failed on trying to post a data point.");
    }

    if (success) {
      success = m_requestRun->lastPostDataPointJSONSuccess();
      if (success) {

        OS_ASSERT(m_waitingQueue.size() >= m_batchSize);
        auto batchStart = m_waitingQueue.end();
        for (unsigned i = 0; i < m_batchSize; ++i) {
          --batchStart;
        }
        DataPointVector batch(batchStart,m_waitingQueue.end());
        OS_ASSERT(batch.size() == m_batchSize);

        for (DataPoint& queued : batch) {
          boost::optional<Job> topLevelJob = queued.topLevelJob();
          OS_ASSERT(topLevelJob);
          topLevelJob->setStatus(AdvancedStatusEnum(AdvancedStatusEnum::WaitingInQueue));
          // no need to call updateJob because we have the same job that is in the database
        }
        project().save();

      }else{
        logError("Run request failed because a DataPoint JSON did not post successfully.");
      }
    }

    if (success) {
      LOG(Debug,"Data points successfully posted.");
      // see if you make another post or go on to starting the analysis
      if (m_postQueue.empty()) {
        LOG(Debug,"All done posting data points, see if this analysis is already running on the server.");
        // done posting --move on
        bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(dataPointsQueued(bool)));
        OS_ASSERT(test);

        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisRunningOnServer(bool)),Qt::QueuedConnection);
        OS_ASSERT(test);

        // see if the analysis is already running
        success = m_requestRun->requestIsAnalysisRunning(project().analysis().uuid());
      }
      else {
        success = postNextDataPointBatch();
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
      OS_ASSERT(test);

      success = m_requestRun->requestIsAnalysisRunning(project().analysis().uuid());

      m_analysisNotRunningCount = 0;
      // DLM: let's be really conservative to start with
      m_maxAnalysisNotRunningCount = 300 + 2 * m_waitingQueue.size();
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
        LOG(Debug,"The analysis was started in " << m_analysisNotRunningCount
            << " tries. Now make sure some DataPoints start running.");

        m_analysisNotRunningCount = 0;
        m_maxAnalysisNotRunningCount = 1;
        
        bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(waitingForAnalysisToStart(bool)));
        OS_ASSERT(test);

        test = m_requestRun->connect(SIGNAL(requestProcessed(bool)),this,SLOT(waitingForADataPointToStart(bool)),Qt::QueuedConnection);
        OS_ASSERT(test);

        success = m_requestRun->requestRunningDataPointUUIDs(project().analysis().uuid());

        m_dataPointsNotRunningCount = 0;
        // DLM: let's be really conservative here
        m_maxDataPointsNotRunningCount = 300 + 2 * m_waitingQueue.size();
      }
      else {
        if (m_analysisNotRunningCount >= m_maxAnalysisNotRunningCount) {
          std::stringstream ss;
          ss << "Analysis failed to start running within " << m_maxAnalysisNotRunningCount << " tries.";
          logError(ss.str());
          success = false;
        }
        else {
          // wait one second
          LOG(Info, "Checking for running analysis, " << m_analysisNotRunningCount << "/" << m_maxAnalysisNotRunningCount << " tries.");
          QTimer::singleShot(1000, this, SLOT(askAgainIfAnalysisRunning()));
        }
      }
    }

    if (!success) {
      registerRunRequestFailure();
    }
  }

  void CloudAnalysisDriver_Impl::askAgainIfAnalysisRunning() {
    bool success = m_requestRun->requestIsAnalysisRunning(project().analysis().uuid());

    if (!success) {
      registerRunRequestFailure();
    }
  }

  void CloudAnalysisDriver_Impl::waitingForADataPointToStart(bool success) {
    if (!success) {
      logError("Run request failed on waiting for the server to report that a data point is running.");
    }

    if (success) {
      bool isRunning = !m_requestRun->lastRunningDataPointUUIDs().empty();
      ++m_dataPointsNotRunningCount;

      if (isRunning) {
        LOG(Debug,"DataPoints were reported to be running after checking the server "
            << m_dataPointsNotRunningCount << " times. On to monitor mode.");

        bool test = m_requestRun->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(waitingForADataPointToStart(bool)));
        OS_ASSERT(test);

        m_dataPointsNotRunningCount = 0;
        // DLM: let's be really conservative here
        m_maxDataPointsNotRunningCount = 300;

        startMonitoring();
      }
      else {
        if (m_dataPointsNotRunningCount >= m_maxDataPointsNotRunningCount) {
          std::stringstream ss;
          ss << "Server failed to report running data points within " << m_maxDataPointsNotRunningCount << " tries.";
          logError(ss.str());
          success = false;
        }
        else {
          // wait one second
          LOG(Info, "Checking for running data points, " << m_dataPointsNotRunningCount << "/" << m_maxDataPointsNotRunningCount << " tries.");
          QTimer::singleShot(1000, this, SLOT(askAgainForRunningDataPoints()));
        }
      }
    }

    if (!success) {
      registerRunRequestFailure();
    }
  }

  void CloudAnalysisDriver_Impl::askAgainForRunningDataPoints() {
    bool success = m_requestRun->requestRunningDataPointUUIDs(project().analysis().uuid());

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
      LOG(Debug,"There are " << completeUUIDs.size() << " complete DataPoints on the server.");
      // complete points could be running or waiting
      auto it = m_waitingQueue.begin();
      while (it != m_waitingQueue.end()) {
        if (completeUUIDs.find(it->uuid()) != completeUUIDs.end()) {
          DataPoint completePoint = *it;
          m_jsonQueue.push_back(completePoint);
          it = m_waitingQueue.erase(it);
          OS_ASSERT(completePoint.topLevelJob());
          completePoint.topLevelJob()->setStatus(AdvancedStatusEnum(AdvancedStatusEnum::CopyingResultFiles));
          // if go straight from waiting to complete, go ahead and show the data point as
          // running rather than queued (or complete--which will happen once json downloaded and
          // incorporated)
          emit dataPointRunning(project().analysis().uuid(),completePoint.uuid());
        }
        else {
          ++it;
        }
      }
      it = m_runningQueue.begin();
      while (it != m_runningQueue.end()) {
        if (completeUUIDs.find(it->uuid()) != completeUUIDs.end()) {
          DataPoint completePoint = *it;
          m_jsonQueue.push_back(completePoint);
          it = m_runningQueue.erase(it);
          OS_ASSERT(completePoint.topLevelJob());
          completePoint.topLevelJob()->setStatus(AdvancedStatusEnum(AdvancedStatusEnum::CopyingResultFiles));
        }
        else {
          ++it;
        }
      }

      if (!m_jsonQueue.empty() && !m_requestJson) {
        LOG(Debug,"Starting Json downloading.");
        startDownloadingJson();
      }

      if (m_waitingQueue.empty() && m_runningQueue.empty() && m_jsonQueue.empty()) {
        // got what we wanted, move into downloading only phase
        LOG(Debug,"All DataPoints complete. Stop monitoring.");
        appendErrorsAndWarnings(*m_monitorDataPoints); // keep warnings
        m_monitorDataPoints.reset();
        checkForRunCompleteOrStopped();
      }
      else if (isStopping() && m_lastStopSuccess &&
               (!m_waitForAlreadyRunningDataPoints || m_runningQueue.empty()))
      {
        LOG(Debug,"Stop monitoring as part of stopping the analysis.");
        appendErrorsAndWarnings(*m_monitorDataPoints); // keep warnings
        m_monitorDataPoints.reset();
        checkForRunCompleteOrStopped();
      }
      else {
        LOG(Info,"Waiting on " << m_waitingQueue.size() + m_runningQueue.size() << " DataPoints.");
        // ask if analysis is running
        QTimer::singleShot(1000,this,SLOT(askIfAnalysisIsRunning()));
      }
    }

    if (!success) {
      registerMonitoringFailure();
    }
  }

  void CloudAnalysisDriver_Impl::askIfAnalysisIsRunning() {
    bool test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisRunningReturned(bool)),Qt::QueuedConnection);
    OS_ASSERT(test);

    System::msleep(1000); // wait 1 second
    bool success = m_monitorDataPoints->requestIsAnalysisRunning(project().analysis().uuid());

    if (!success) {
      registerMonitoringFailure();
    }
  }

  void CloudAnalysisDriver_Impl::analysisRunningReturned(bool success) {
    bool test = m_monitorDataPoints->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisRunningReturned(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run failed during monitoring. Request asking if server is still running this analysis failed.");
    }

    if (success) {
      if (m_monitorDataPoints->lastIsAnalysisRunning()) {
        m_analysisNotRunningCount = 0;
      }
      else {
        ++m_analysisNotRunningCount;
      }

      if (m_analysisNotRunningCount > m_maxAnalysisNotRunningCount) {
        if (isStopping() && m_lastStopSuccess && m_waitForAlreadyRunningDataPoints) {
          LOG(Debug,"Was trying to wait for DataPoints to finish running as part of the stopping process, but have timed out. Stop monitoring.");
          appendErrorsAndWarnings(*m_monitorDataPoints); // keep warnings
          m_monitorDataPoints.reset();
          checkForRunCompleteOrStopped();
        }
        else {
          logError("Server reports that the analysis is complete, but we are still waiting on DataPoints.");
          success = false;
        }
      }
    }

    if (success) {
      LOG(Debug,"The analysis is still running. Ask for running data point uuids.");
      QTimer::singleShot(1000,this,SLOT(askForRunningDataPointUUIDs()));
    }

    if (!success) {
      registerMonitoringFailure();
    }
  }

  void CloudAnalysisDriver_Impl::askForRunningDataPointUUIDs() {
    bool test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(runningDataPointUUIDsReturned(bool)),Qt::QueuedConnection);
    OS_ASSERT(test);

    bool success = m_monitorDataPoints->requestRunningDataPointUUIDs(project().analysis().uuid());

    if (!success) {
      registerMonitoringFailure();
    }
  }

  void CloudAnalysisDriver_Impl::runningDataPointUUIDsReturned(bool success) {
    bool test = m_monitorDataPoints->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(runningDataPointUUIDsReturned(bool)));
    OS_ASSERT(test);

    if (!success) {
      logError("Run failed during monitoring. Request asking server for the UUIDs of any running data points failed.");
    }

    UUIDVector temp;
    if (success) {
      temp = m_monitorDataPoints->lastRunningDataPointUUIDs();
      LOG(Debug,"There are " << temp.size() << " DataPoints running on the server.");
      if (temp.empty()) {
        ++m_dataPointsNotRunningCount;
      }
      else {
        m_dataPointsNotRunningCount = 0;
      }

      if (m_dataPointsNotRunningCount > m_maxDataPointsNotRunningCount) {
        std::stringstream ss;
        ss << "Server reported no DataPoints running " << m_dataPointsNotRunningCount
           << " times in a row. Assuming that server has stopped working as expected.";
        logError(ss.str());
        ss.str("");
        success = false;
      }
    }

    if (success) {
      // move DataPoints from waiting to running
      std::set<UUID> runningUUIDs(temp.begin(),temp.end());
      auto it = m_waitingQueue.begin();
      while (it != m_waitingQueue.end()) {
        if (runningUUIDs.find(it->uuid()) != runningUUIDs.end()) {
          DataPoint nowRunning = *it;
          m_runningQueue.push_back(nowRunning);
          it = m_waitingQueue.erase(it);
          OS_ASSERT(nowRunning.topLevelJob());
          nowRunning.topLevelJob()->setStatus(AdvancedStatusEnum(AdvancedStatusEnum::Processing));
          emit dataPointRunning(project().analysis().uuid(),nowRunning.uuid());
        }
        else {
          ++it;
        }
      }

      // ask for complete data points
      QTimer::singleShot(1000,this,SLOT(askForCompleteDataPointUUIDs()));
    }

    if (!success) {
      registerMonitoringFailure();
    }
  }

  void CloudAnalysisDriver_Impl::askForCompleteDataPointUUIDs() {
    bool test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(completeDataPointUUIDsReturned(bool)),Qt::QueuedConnection);
    OS_ASSERT(test);

    bool success = m_monitorDataPoints->requestCompleteDataPointUUIDs(project().analysis().uuid());

    if (!success) {
      registerMonitoringFailure();
    }
  }

  void CloudAnalysisDriver_Impl::jsonDownloadComplete(bool success) {

    if (!success) {
      ++m_numJsonTries;
      if (m_numJsonTries >= 3) {
        logError("Unable to retrieve high level results for DataPoint '" +
                 m_jsonQueue.front().name() + "', " + removeBraces(m_jsonQueue.front().uuid()) +
                 " from server.");
        m_jsonFailures.push_back(m_jsonQueue.front());
        m_jsonQueue.pop_front();
      }
      else {
        QTimer::singleShot(1000,this,SLOT(requestJsonRetry()));
        return;
      }
    }

    if (success) {
      std::string json = m_requestJson->lastDataPointJSON();
      DataPoint toUpdate = m_jsonQueue.front();
      LOG(Debug,"Downloaded Json file for DataPoint '" << toUpdate.name() << "'.");
      m_jsonQueue.pop_front();
      boost::optional<RunManager> rm = project().runManager();
      bool test = toUpdate.updateFromJSON(json,rm);
      project().save();

      // DLM: Elaine, it seems that if this point is CloudDetailed we should not emit these signals until the download is done?
      // we are having issues where datapoint shows green arrow before results are available
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
        checkForRunCompleteOrStopped();
      }
      else {
        LOG(Info,"Have " << m_jsonQueue.size() << " DataPoints' slim results to download.");
        m_numJsonTries = 0;
        success = requestNextJsonDownload();
      }
    }

    if (!success) {
      registerDownloadingJsonFailure();
    }
  }

  void CloudAnalysisDriver_Impl::requestJsonRetry() {
    LOG(Info,"Have " << m_jsonQueue.size() << " DataPoints' slim results to download (retrying a point).");
    bool success = requestNextJsonDownload();

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
      unsigned initialSize = m_preDetailsQueue.size();
      auto it = m_preDetailsQueue.begin();
      while (it != m_preDetailsQueue.end()) {
        if (readyUUIDs.find(it->uuid()) != readyUUIDs.end()) {
          m_detailsQueue.push_back(*it);
          it = m_preDetailsQueue.erase(it);
        }
        else {
          ++it;
        }
      }

      if (m_preDetailsQueue.size() < initialSize) {
        m_noNewReadyDataPointsCount = 0;
      }
      else {
        ++m_noNewReadyDataPointsCount;
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
        LOG(Info,"Waiting on detailed results for " << m_preDetailsQueue.size() << " DataPoints.");
        if (m_noNewReadyDataPointsCount > 60) {
          logError("No points became ready for download within 60 tries.");
          success = false;
        }
        else {
          QTimer::singleShot(1000,this,SLOT(askForReadyForDownloadDataPointUUIDs()));
        }
      }
    }

    if (!success) {
      registerDownloadingDetailsFailure();
    }
  }

  void CloudAnalysisDriver_Impl::askForReadyForDownloadDataPointUUIDs() {
    bool success = m_checkForResultsToDownload->requestDownloadReadyDataPointUUIDs(project().analysis().uuid());
    if (!success) {
      registerDownloadingDetailsFailure();
    }
  }

  void CloudAnalysisDriver_Impl::detailsDownloadComplete(bool success) {

    success = success && m_requestDetails->lastDownloadDataPointSuccess();

    if (!success) {
      ++m_numDetailsTries;
      if (m_numDetailsTries >= 3) {
        logError("Unable to retrieve detailed results for DataPoint '" +
                 m_detailsQueue.front().name() + "', " + removeBraces(m_detailsQueue.front().uuid()) +
                 " from server.");
        m_detailsFailures.push_back(m_detailsQueue.front());
        m_detailsQueue.pop_front();
      }
      else {
        QTimer::singleShot(1000,this,SLOT(requestDetailsRetry()));
        return;
      }
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

  void CloudAnalysisDriver_Impl::requestDetailsRetry() {
    bool success = requestNextDetailsDownload();
    if (!success) {
      registerDownloadingDetailsFailure();
    }
  }

  void CloudAnalysisDriver_Impl::analysisStopped(bool success) {
    bool test = m_requestStop->disconnect(SIGNAL(requestProcessed(bool)),this,SLOT(analysisStopped(bool)));
    OS_ASSERT(test);

    if (success) {
      success = m_requestStop->lastStopSuccess();
      m_lastStopSuccess = success;
    }

    if (!success) {
      logError("Request to stop Analysis failed.");
    }

    if (success) {
      // if nothing else going on, emit signal stating that analysis has stopped
      if (m_waitForAlreadyRunningDataPoints) {
        // DLM: are these 1 second checks?  this is only 5 minutes.  Should we wait longer here?
        m_maxAnalysisNotRunningCount = 300; // wait up to 15 minutes for DataPoints to stop running
      }
      if (m_needToDeleteProject) {
        requestProjectDeletion();
        return;
      }
      checkForRunCompleteOrStopped();
      // otherwise, will keep spinning until all processes cease
    }

    if (!success) {
      if (m_needToDeleteProject) {
        registerProjectDeletionFailure();
      }
      else {
        registerStopRequestFailure();
      }
    }
  }

  void CloudAnalysisDriver_Impl::dataPointOrItsResultsRemovedFromAnalysis(const openstudio::UUID& dataPointUUID)
  {
    if (analysis::OptionalDataPoint dataPoint = m_project.analysis().getDataPointByUUID(dataPointUUID)) {
      m_deleteDataPointsQueue.push_back(*dataPoint);
    }
    if (!m_requestDeleteDataPoint && !m_deleteDataPointsQueue.empty()) {
      requestNextDataPointDeletion();
    }
  }

  void CloudAnalysisDriver_Impl::dataPointDeletedFromServer(bool success) {
    success = success && m_requestDeleteDataPoint->lastDeleteDataPointSuccess();

    if (!success) {
      ++m_numDeleteDataPointTries;
      if (m_numDeleteDataPointTries >= 3) {
        logError("Unable to delete DataPoint '" +
                 m_deleteDataPointsQueue.front().name() + "', " + removeBraces(m_deleteDataPointsQueue.front().uuid()) +
                 " from server.");
        m_deleteDataPointFailures.push_back(m_deleteDataPointsQueue.front());
        m_deleteDataPointsQueue.pop_front();
      }
      else {
        QTimer::singleShot(1000,this,SLOT(requestDeleteDataPointRetry()));
        return;
      }
    }

    if (success) {
      DataPoint deletedPoint = m_deleteDataPointsQueue.front();
      removeFromIteration(deletedPoint);
      m_deleteDataPointsQueue.pop_front();
      emit resultsChanged();
      emit iterationProgress(numCompleteDataPoints(),numDataPointsInIteration());

      if (m_deleteDataPointsQueue.empty()) {
        bool test = m_requestDeleteDataPoint->disconnect(SIGNAL(requestProcessed(bool)),
                                                         this,
                                                         SLOT(dataPointDeletedFromServer(bool)));
        OS_ASSERT(test);
        appendErrorsAndWarnings(*m_requestDeleteDataPoint);
        m_requestDeleteDataPoint.reset();
      }
      else {
        LOG(Info,"Have " << m_deleteDataPointsQueue.size() << " DataPoints to delete from the server.");
        success = requestNextDataPointDeletion();
      }
    }

    if (!success) {
      registerDataPointDeletionFailure();
    }
  }

  void CloudAnalysisDriver_Impl::requestDeleteDataPointRetry() {
    LOG(Info,"Have " << m_deleteDataPointsQueue.size() << " DataPoints to delete from the server (retrying).");
    bool success = requestNextDataPointDeletion();

    if (!success) {
      registerDataPointDeletionFailure();
    }
  }

  void CloudAnalysisDriver_Impl::allDataPointsOrResultsRemovedFromAnalysis() {
    LOG(Info,"All data points or results deleted locally. Will try to delete the project from the server.");
    m_needToDeleteProject = true;

    if (isRunning() || isDownloading()) {
      requestStop(false);
    }
    else {
      requestProjectDeletion();
    }
  }

  void CloudAnalysisDriver_Impl::projectDeletedFromServer(bool success) {
    success = success && m_requestDeleteProject->lastDeleteProjectSuccess();

    if (!success) {
      ++m_numDeleteProjectTries;
      if (m_numDeleteProjectTries >= 30) {
        logError("Unable to delete project from server.");
      }
      else {
        QTimer::singleShot(1000,this,SLOT(requestDeleteProjectRetry()));
        return;
      }
    }

    if (success) {
      bool test = m_requestDeleteProject->disconnect(SIGNAL(requestProcessed(bool)),
                                                     this,
                                                     SLOT(projectDeletedFromServer(bool)));
      OS_ASSERT(test);
      appendErrorsAndWarnings(*m_requestDeleteProject);
      m_requestDeleteProject.reset();
      OS_ASSERT(!isRunning() && !isDownloading() && !isStopping());
      resetState();
      setStatus(AnalysisStatus::Idle);
      emit resultsChanged();
      emit iterationProgress(numCompleteDataPoints(),numDataPointsInIteration());
    }

    if (!success) {
      registerProjectDeletionFailure();
    }
  }

  void CloudAnalysisDriver_Impl::requestDeleteProjectRetry() {
    LOG(Debug,"Trying to delete project from the server for the " << (m_numDeleteProjectTries + 1)
        << "th time.");

    bool success = requestProjectDeletion();

    if (!success) {
      registerProjectDeletionFailure();
    }
  }

  void CloudAnalysisDriver_Impl::resetState() {
    m_errors.clear();
    m_warnings.clear();

    m_iteration.clear();
    m_postQueue.clear();
    m_waitingQueue.clear();
    m_runningQueue.clear();
    m_jsonQueue.clear();
    m_jsonFailures.clear();
    m_preDetailsQueue.clear();
    m_detailsQueue.clear();
    m_detailsFailures.clear();
    m_deleteDataPointsQueue.clear();
    m_deleteDataPointFailures.clear();

    m_processingQueuesInitialized = false;
    m_batchSize = 0;
    m_analysisNotRunningCount = 0;
    m_maxAnalysisNotRunningCount = 0;
    m_dataPointsNotRunningCount = 0;
    m_maxDataPointsNotRunningCount = 0;
    m_numJsonTries = 0;
    m_noNewReadyDataPointsCount = 0;
    m_numDetailsTries = 0;
    m_numDeleteDataPointTries = 0;
    m_numDeleteProjectTries = 0;

    m_needToDeleteProject = false;
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
    setStatus(AnalysisStatus::Error);
    emit runRequestComplete(false);
    m_onlyProcessingDownloadRequests = true;
  }

  bool CloudAnalysisDriver_Impl::postNextDataPointBatch() {
    DataPointVector batch;
    while (!m_postQueue.empty() && batch.size() < 10u) {
      DataPoint toQueue = m_postQueue.front();
      if (toQueue.runType() == DataPointRunType::Local) {
        toQueue.setRunType(DataPointRunType::CloudSlim);
      }
      m_postQueue.pop_front();
      batch.push_back(toQueue);
      m_waitingQueue.push_back(toQueue);
    }

    // get json before we add the dummy job
    std::string batchJson = toJSON(batch);
    bool result = m_requestRun->startPostDataPointJSON(project().analysis().uuid(), batchJson);
    m_batchSize = batch.size();

    if (result) {

      for (DataPoint& toQueue : batch) {

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
        // ETH: Added enqueueOrReturnExisting because failure to enqueue resulted in
        // landing in the catch clause below. Clearing results for a DataPoint does remove the
        // topLevelJob, but maybe the initial fake job threw us off, that is, maybe the original
        // fake job  (with a different UUID than the final job) needs to get thrown away and is
        // not?
        boost::optional<runmanager::Job> substitute = project().runManager().enqueueOrReturnExisting(job, true);
        if (substitute) {
          job = *substitute;
        }

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
          for (runmanager::Job j : project().runManager().getJobs()){
            std::cout << "  " << toString(j.uuid()) << std::endl;
          }
          OS_ASSERT(false);
        }

        emit dataPointQueued(project().analysis().uuid(),toQueue.uuid());
      }

      project().save();
    }

    // caller will register failure if !result
    return result;
  }

  bool CloudAnalysisDriver_Impl::startMonitoring() {
    OS_ASSERT(!m_monitorDataPoints);

    bool success(false);

    if (OptionalUrl url = session().serverUrl()) {
      m_monitorDataPoints = OSServer(*url);

      bool test = m_monitorDataPoints->connect(SIGNAL(requestProcessed(bool)),this,SLOT(runningDataPointUUIDsReturned(bool)),Qt::QueuedConnection);
      OS_ASSERT(test);

      success = m_monitorDataPoints->requestRunningDataPointUUIDs(project().analysis().uuid());
    }
    else {
      logError("Cannot start monitoring this run because the CloudSession has been terminated.");
      setStatus(AnalysisStatus::Error);
      emit runRequestComplete(false);
      return success;
    }

    OS_ASSERT(m_monitorDataPoints);
    if (!success) {
      registerMonitoringFailure();
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

    setStatus(AnalysisStatus::Running);

    return success;
  }

  void CloudAnalysisDriver_Impl::registerMonitoringFailure() {
    appendErrorsAndWarnings(*m_monitorDataPoints);
    m_monitorDataPoints.reset();
    OS_ASSERT(m_postQueue.empty());
    setStatus(AnalysisStatus::Error);
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
      return success;
    }

    if (!success) {
      registerDownloadingJsonFailure();
    }

    setStatus(AnalysisStatus::Running);

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

    setStatus(AnalysisStatus::Running);

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

    setStatus(AnalysisStatus::Running);

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

    setStatus(AnalysisStatus::Running);

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
    if (m_onlyProcessingDownloadRequests) {
      OS_ASSERT(!m_lastDownloadDetailedResultsSuccess);
      emit detailedDownloadRequestsComplete(m_lastDownloadDetailedResultsSuccess);
    }
  }

  void CloudAnalysisDriver_Impl::registerStopRequestFailure() {
    appendErrorsAndWarnings(*m_requestStop);
    m_requestStop.reset();
    setStatus(AnalysisStatus::Error);
    OS_ASSERT(!m_lastStopSuccess);
    emit stopRequestComplete(m_lastStopSuccess);
  }

  bool CloudAnalysisDriver_Impl::requestNextDataPointDeletion() {
    if (!m_requestDeleteDataPoint) {
      if (OptionalUrl url = session().serverUrl()) {
        m_requestDeleteDataPoint = OSServer(*url);

        bool test = m_requestDeleteDataPoint->connect(SIGNAL(requestProcessed(bool)),
                                                      this,
                                                      SLOT(dataPointDeletedFromServer(bool)),
                                                      Qt::QueuedConnection);
        OS_ASSERT(test);
      }
      else {
        return false;
      }
    }
    return m_requestDeleteDataPoint->requestDeleteDataPoint(project().analysis().uuid(),
                                                            m_deleteDataPointsQueue.front().uuid());
  }

  void CloudAnalysisDriver_Impl::registerDataPointDeletionFailure() {
    appendErrorsAndWarnings(*m_requestDeleteDataPoint);
    m_requestDeleteDataPoint.reset();
    setStatus(AnalysisStatus::Error);
  }

  bool CloudAnalysisDriver_Impl::requestProjectDeletion() {
    if (!m_requestDeleteProject) {
      // if not already trying to delete the project ... go for it
      // otherwise, assume that the original request is going through
      if (OptionalUrl url = session().serverUrl()) {
        setStatus(AnalysisStatus::Stopping);

        m_requestDeleteProject = OSServer(*url);

        bool test = m_requestDeleteProject->connect(SIGNAL(requestProcessed(bool)),
                                                    this,
                                                    SLOT(projectDeletedFromServer(bool)),
                                                    Qt::QueuedConnection);
        OS_ASSERT(test);

        return m_requestDeleteProject->requestDeleteProject(project().projectDatabase().handle());
      }
    }
    return false;
  }

  void CloudAnalysisDriver_Impl::registerProjectDeletionFailure() {
    if (m_requestStop) {
      appendErrorsAndWarnings(*m_requestStop);
      m_requestStop.reset();
    }
    if (m_requestDeleteProject) {
      appendErrorsAndWarnings(*m_requestDeleteProject);
      m_requestDeleteProject.reset();
    }
    setStatus(AnalysisStatus::Error);
  }

  void CloudAnalysisDriver_Impl::checkForRunCompleteOrStopped() {
    if (isStopping()) {
      bool stopped = !isDownloading() && !isRunning();
      if (stopped) {
        appendErrorsAndWarnings(*m_requestStop);
        m_requestStop.reset();
        m_lastStopSuccess = true;

        setStatus(AnalysisStatus::Idle);
        emit stopRequestComplete(true);
        emit analysisStopped(project().analysis().uuid());
      }
    }
    else {
      if (!(isRunning() || isDownloading())) {
        // not trying to stop and not doing anything
        if (numIncompleteDataPoints() == 0) {
          if (m_onlyProcessingDownloadRequests) {
            m_lastDownloadDetailedResultsSuccess = true;
          }
          else {
            m_lastRunSuccess = true;
          }
        }
        if (m_onlyProcessingDownloadRequests) {
          emit detailedDownloadRequestsComplete(m_lastDownloadDetailedResultsSuccess);
        }
        else {
          emit runRequestComplete(m_lastRunSuccess);
          emit analysisComplete(project().analysis().uuid());
          m_onlyProcessingDownloadRequests = true; // no longer running
        }
        setStatus(AnalysisStatus::Idle);
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
    if (std::find(m_runningQueue.begin(),m_runningQueue.end(),dataPoint) != m_runningQueue.end()) {
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

  void CloudAnalysisDriver_Impl::removeFromIteration(const analysis::DataPoint& dataPoint) {
    std::vector<analysis::DataPoint>::iterator vit;
    std::deque<analysis::DataPoint>::iterator dit;

    vit = std::find(m_iteration.begin(),m_iteration.end(),dataPoint);
    if (vit != m_iteration.end()) {
      m_iteration.erase(vit);
    }

    dit = std::find(m_postQueue.begin(),m_postQueue.end(),dataPoint);
    if (dit != m_postQueue.end()) {
      m_postQueue.erase(dit);
    }

    vit = std::find(m_waitingQueue.begin(),m_waitingQueue.end(),dataPoint);
    if (vit != m_waitingQueue.end()) {
      m_waitingQueue.erase(vit);
    }

    vit = std::find(m_runningQueue.begin(),m_runningQueue.end(),dataPoint);
    if (vit != m_runningQueue.end()) {
      m_runningQueue.erase(vit);
    }

    dit = std::find(m_jsonQueue.begin(),m_jsonQueue.end(),dataPoint);
    if (dit != m_jsonQueue.end()) {
      m_jsonQueue.erase(dit);
    }

    vit = std::find(m_preDetailsQueue.begin(),m_preDetailsQueue.end(),dataPoint);
    if (vit != m_preDetailsQueue.end()) {
      m_preDetailsQueue.erase(vit);
    }

    dit = std::find(m_detailsQueue.begin(),m_detailsQueue.end(),dataPoint);
    if (dit != m_detailsQueue.end()) {
      m_detailsQueue.erase(dit);
    }
  }

  std::string CloudAnalysisDriver_Impl::sessionTag() const {
    return std::string("CloudSession_") + session().sessionId();
  }

  void CloudAnalysisDriver_Impl::clearSessionTags(DataPoint& dataPoint) const {
    TagVector tags = dataPoint.tags();
    boost::regex re("CloudSession_.*");
    for (const Tag& tag : tags) {
      if (boost::regex_match(tag.name(),re)) {
        dataPoint.deleteTag(tag.name());
      }
    }
  }

} // detail

CloudAnalysisDriver::CloudAnalysisDriver(const CloudSession& session,
                                         const SimpleProject& project)
  : m_impl(std::shared_ptr<detail::CloudAnalysisDriver_Impl>(
             new detail::CloudAnalysisDriver_Impl(session,project)))
{}

CloudSession CloudAnalysisDriver::session() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->session();
}

SimpleProject CloudAnalysisDriver::project() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->project();
}

AnalysisStatus CloudAnalysisDriver::status() const
{
  return getImpl<detail::CloudAnalysisDriver_Impl>()->status();
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

unsigned CloudAnalysisDriver::numFailedDataPoints() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->numFailedDataPoints();
}

std::vector<analysis::DataPoint> CloudAnalysisDriver::failedJsonDownloads() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->failedJsonDownloads();
}

std::vector<analysis::DataPoint> CloudAnalysisDriver::failedDetailedDownloads() const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->failedDetailedDownloads();
}

bool CloudAnalysisDriver::inSession(const DataPoint& dataPoint) const {
  return getImpl<detail::CloudAnalysisDriver_Impl>()->inSession(dataPoint);
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
CloudAnalysisDriver::CloudAnalysisDriver(std::shared_ptr<detail::CloudAnalysisDriver_Impl> impl)
  : m_impl(impl)
{}
/// @endcond

} // analysisdriver
} // openstudio

