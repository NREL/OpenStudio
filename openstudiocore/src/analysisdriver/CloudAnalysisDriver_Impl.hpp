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

#ifndef ANALYSISDRIVER_CLOUDANALYSISDRIVER_IMPL_HPP
#define ANALYSISDRIVER_CLOUDANALYSISDRIVER_IMPL_HPP

#include "AnalysisDriverAPI.hpp"
#include "AnalysisDriverEnums.hpp"

#include "SimpleProject.hpp"

#include "../analysis/DataPoint.hpp"

#include "../utilities/cloud/CloudProvider.hpp"
#include "../utilities/cloud/OSServer.hpp"

#include <QObject>

#include <boost/smart_ptr.hpp>

#include <deque>

namespace openstudio {

class Tag;

namespace analysisdriver {

namespace detail {

  /** CloudAnalysisDriver_Impl is the implementation class for CloudAnalysisDriver.*/
  class ANALYSISDRIVER_API CloudAnalysisDriver_Impl : public QObject, public std::enable_shared_from_this<CloudAnalysisDriver_Impl> {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    CloudAnalysisDriver_Impl(const CloudSession& session,
                             const SimpleProject& project);

    virtual ~CloudAnalysisDriver_Impl() {}

    //@}
    /** @name Getters */
    //@{

    CloudSession session() const;

    SimpleProject project() const;

    AnalysisStatus status() const;

    /** Returns the number of data points the CloudAnalysisDriver has been asked to process
     *  since the last time all the queues were cleared. */
    unsigned numDataPointsInIteration() const;

    /** Returns the number of data points in all of the processing queues. */
    unsigned numIncompleteDataPoints() const;

    /** Returns the number of data points in this iteration that are no longer being processed. */
    unsigned numCompleteDataPoints() const;

    /** Returns the number of complete data points that are marked as .failed(). */
    unsigned numFailedDataPoints() const;
    
    /** Returns the DataPoints whose json files failed to download. Note that these are counted
     *  as 'complete' by CloudAnalysisDriver, but not by Analysis. */
    std::vector<analysis::DataPoint> failedJsonDownloads() const;

    /** Returns the DataPoints whose details failed to download. Note that these are counted as
     *  'complete' by CloudAnalysisDriver and by Analysis, but their .directory() is .empty(). */
    std::vector<analysis::DataPoint> failedDetailedDownloads() const;

    /** Returns true if dataPoint is associated with session(), that is, if its last run request
     *  was with session() (not local, and not another CloudSession). */
    bool inSession(const analysis::DataPoint& dataPoint) const;

    //@}
    /** @name Blocking Class Members */
    //@{

    bool run(int msec=-1);
    bool lastRunSuccess() const;

    bool stop(int msec=-1);
    bool lastStopSuccess() const;

    bool downloadDetailedResults(analysis::DataPoint& dataPoint,int msec=-1);
    bool lastDownloadDetailedResultsSuccess() const;

    bool isRunning() const;
    bool isStopping() const;
    bool isDownloading() const;

    std::vector<std::string> errors() const;
    std::vector<std::string> warnings() const;

    /** If no argument is specified, CloudAnalysisDriver will run until not isRunning() and
     *  not isDownloading(). If msec is specified, will wait for at most msec milliseconds.
     *  Returns true if not isRunning() and not isDownloading() upon exit; false otherwise. */
    bool waitForFinished(int msec=-1);

    //@}
    /** @name Non-blocking class members */
    //@{

    /** Request the project() to run on the session(). Returns false if isRunning().
     *  Otherwise returns true and emits runRequestComplete(bool success) when either the
     *  analysis has stopped running on the server or the process has failed. The ultimate
     *  value of success will also be available from lastRunSuccess(). This method will try
     *  to pick up where a previous run left off. */
    bool requestRun();

    /** Request the project() to stop running on the session(). Returns false if not
     *  (isRunning() || isDownloading()). Otherwise returns true and emits
     *  stopRequestComplete(bool success) when the analysis has stopped running and the
     *  download queue is empty, or the process has failed. The ultimate value of
     *  success will also be available from lastStopSuccess(). */
    bool requestStop(bool waitForAlreadyRunningDataPoints=false);

    /** Request for dataPoint's detailed results to be downloaded. Returns false if
     *  !dataPoint.complete() or if the detailed results have already been downloaded or
     *  if the dataPoint does not belong to project().analysis(). Otherwise returns true
     *  and emits dowloadRequestsComplete(bool success) when the detailed downloads queue
     *  is empty or the process has failed. Look for the dataPointDetailsComplete signal
     *  to see when this particular dataPoint's results have been incorporated. */
    bool requestDownloadDetailedResults(analysis::DataPoint& dataPoint);

    //@}
    /** @name Signals, Slots, Threads */
    //@{

    bool connect(const std::string& signal,
                 const QObject* qObject,
                 const std::string& slot,
                 Qt::ConnectionType type = Qt::AutoConnection) const;

    //@}
    /** @name Type Casting */
    //@{

    /** Get a public object that wraps this impl.*/
    template<typename T>
    T getPublicObject() const {
      T result(std::dynamic_pointer_cast<typename T::ImplType>(
                 std::const_pointer_cast<CloudAnalysisDriver_Impl>(shared_from_this())));
      return result;
    }

    //@}
   signals:
    /** @name Non-blocking Call Finished Signals */
    //@{

    void runRequestComplete(bool success);

    void stopRequestComplete(bool success);

    void detailedDownloadRequestsComplete(bool success);

    //@}
    /** @name AnalysisDriver/CurrentAnalysis Progress Signals */
    //@{

    void resultsChanged();

    void iterationProgress(int numCompletedJobs,int numJobsInIteration);

    // emitted when data point posted to server
    void dataPointQueued(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

    // emitted when data point reported as running
    void dataPointRunning(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

    // emitted when data point slim results downloaded and incorporated into project
    void dataPointComplete(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

    // emitted when data point detailed results downloaded and incorporated into project
    void dataPointDetailsComplete(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

    // emitted when last data point downloaded and incorporated into project
    void analysisComplete(const openstudio::UUID& analysis);

    void analysisStopped(const openstudio::UUID& analysis);
    
    void analysisStatusChanged(analysisdriver::AnalysisStatus newStatus);

    //@}
   protected slots:
     // RUNNING ================================================================

     // 1. Is the server available?
     void availableForRun(bool success);
     // 2. If so, does the server know about out project?
     void projectOnServer(bool success);
     // 3. If not, create it
     void projectCreated(bool success);
     // 4. Does the server know about our analysis?
     void analysisOnServer(bool success);
     // 5a. If not, post it.
     void analysisPosted(bool success);
     // 5b. If so, are there any data points on the server?
     void allDataPointUUIDsReturned(bool success);

     // 6. If posted analysis (3a) or there are no data points (3b and result is empty),
     //    upload the analysis files.
     void analysisUploaded(bool success);

     // 7a. If there were data points (3b with non-empty result), figure out all the queues.
     //     Gets complete data points.
     void readyToSortOutQueues(bool success);

     // 7b. If the analysis was uploaded (4), populate the postQueue and start posting
     //     DataPoints.

     // 8. Keep posting DataPoints until the postQueue is empty.
     void dataPointsQueued(bool success);

     // 9. Is the analysis already running on the server?
     //    (Could skip this step if uploaded analysis, but doesn't seem worth the extra
     //    state to do so.)
     void analysisRunningOnServer(bool success);

     // 10. If not, kick it off.
     void analysisStarted(bool success);

     // 11. Wait for the server to
     //     report that the analysis is running.
     void waitingForAnalysisToStart(bool success);

     void askAgainIfAnalysisRunning();

     // 12. Wait for the server to report that
     //     at least one DataPoint is running.
     void waitingForADataPointToStart(bool success);

     void askAgainForRunningDataPoints();

     // 13. Start the monitoring process (if already running or just kicked off).

     // MONITORING =============================================================

     // watch for complete data points
     void completeDataPointUUIDsReturned(bool success);

     void askIfAnalysisIsRunning();

     // make sure analysis is still running
     // (try to avoid spinning when nothing is happening)
     void analysisRunningReturned(bool success);

     void askForRunningDataPointUUIDs();

     // see if data points are still running
     // (mark DataPoints that are running, and also make sure something is happening.)
     void runningDataPointUUIDsReturned(bool success);

     void askForCompleteDataPointUUIDs();

     // DOWNLOADING ============================================================

     // slim results received
     void jsonDownloadComplete(bool success);

     void requestJsonRetry();

     // pause between slim and detailed results
     void readyForDownloadDataPointUUIDsReturned(bool success);

     void askForReadyForDownloadDataPointUUIDs();

     // detailed results received
     void detailsDownloadComplete(bool success);

     void requestDetailsRetry();

     // STOPPING ===============================================================

     // 0. If are to wait for already running DataPoints, switch the monitoring process to
     //    look for running data points, not for the analysis running.

     // 1. Stop the analysis.
     void analysisStopped(bool success);

     // 2. Wait (almost-as) usual. If isStopping() and all processes have stopped, emit
     //    signal.

     // MAINTENANCE ============================================================

     // DataPoint or its results removed locally. Have server delete it too.
     void dataPointOrItsResultsRemovedFromAnalysis(const openstudio::UUID& dataPointUUID);

     // Process result of request put to server.
     void dataPointDeletedFromServer(bool success);

     void requestDeleteDataPointRetry();

     // All data points or all results removed locally. Stop analysis and remove project.
     void allDataPointsOrResultsRemovedFromAnalysis();

     // Process result of request to delete project from server.
     void projectDeletedFromServer(bool success);

     void requestDeleteProjectRetry();

   private:
    REGISTER_LOGGER("openstudio.analysisdriver.CloudAnalysisDriver");

    CloudSession m_session;
    SimpleProject m_project;

    bool m_lastRunSuccess;
    bool m_lastStopSuccess;
    bool m_lastDownloadDetailedResultsSuccess;
    AnalysisStatus m_status;

    std::vector<std::string> m_errors;
    std::vector<std::string> m_warnings;

    std::vector<analysis::DataPoint> m_iteration; // DataPoints in this iteration
    bool m_processingQueuesInitialized; // if false, processing queues empty just because
                                        // still spinning up process

    // request run process
    boost::optional<OSServer> m_requestRun;
    std::deque<analysis::DataPoint> m_postQueue;
    unsigned m_batchSize;
    unsigned m_analysisNotRunningCount;
    unsigned m_maxAnalysisNotRunningCount;
    // the following are used in starting the run and in monitoring
    unsigned m_dataPointsNotRunningCount;
    unsigned m_maxDataPointsNotRunningCount;

    // watch for complete data points
    boost::optional<OSServer> m_monitorDataPoints;
    std::vector<analysis::DataPoint> m_waitingQueue;
    std::vector<analysis::DataPoint> m_runningQueue;

    // download slim data points
    boost::optional<OSServer> m_requestJson;
    std::deque<analysis::DataPoint> m_jsonQueue;
    unsigned m_numJsonTries;
    std::vector<analysis::DataPoint> m_jsonFailures;

    // check to see if details can be downloaded
    boost::optional<OSServer> m_checkForResultsToDownload;
    std::vector<analysis::DataPoint> m_preDetailsQueue;
    bool m_onlyProcessingDownloadRequests; // to distinguish between main request being
                                           // run or download of details
    unsigned m_noNewReadyDataPointsCount;

    // download detailed results
    boost::optional<OSServer> m_requestDetails;
    std::deque<analysis::DataPoint> m_detailsQueue;
    unsigned m_numDetailsTries;
    std::vector<analysis::DataPoint> m_detailsFailures;

    // stop analysis
    boost::optional<OSServer> m_requestStop;
    bool m_waitForAlreadyRunningDataPoints;

    // delete data point
    boost::optional<OSServer> m_requestDeleteDataPoint;
    std::deque<analysis::DataPoint> m_deleteDataPointsQueue;
    unsigned m_numDeleteDataPointTries;
    std::vector<analysis::DataPoint> m_deleteDataPointFailures;

    // stop analysis and delete project
    boost::optional<OSServer> m_requestDeleteProject;
    bool m_needToDeleteProject;
    unsigned m_numDeleteProjectTries;

    void resetState();

    void logError(const std::string& error);
    void logWarning(const std::string& warning);
    void appendErrorsAndWarnings(const OSServer& server);
    void setStatus(AnalysisStatus status);

    void registerRunRequestFailure();
    bool postNextDataPointBatch(); // groups of 50
    bool startMonitoring();
    void registerMonitoringFailure();

    bool startDownloadingJson();
    bool requestNextJsonDownload();
    void registerDownloadingJsonFailure();

    bool startDownloadingDetails();
    bool startDetailsReadyMonitoring();
    bool startActualDownloads();
    bool requestNextDetailsDownload();
    void registerDownloadingDetailsFailure();

    void registerStopRequestFailure();

    bool requestNextDataPointDeletion();
    void registerDataPointDeletionFailure();

    bool requestProjectDeletion();
    void registerProjectDeletionFailure();

    void checkForRunCompleteOrStopped();

    bool inIteration(const analysis::DataPoint& dataPoint) const;
    bool inProcessingQueues(const analysis::DataPoint& dataPoint) const;
    void removeFromIteration(const analysis::DataPoint& dataPoint);

    std::string sessionTag() const;
    void clearSessionTags(analysis::DataPoint& dataPoint) const;

  };

} // detail

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_CLOUDANALYSISDRIVER_IMPL_HPP

