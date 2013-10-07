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

#ifndef ANALYSISDRIVER_CLOUDANALYSISDRIVER_IMPL_HPP
#define ANALYSISDRIVER_CLOUDANALYSISDRIVER_IMPL_HPP

#include <analysisdriver/AnalysisDriverAPI.hpp>
#include <analysisdriver/AnalysisDriverEnums.hpp>

#include <analysisdriver/SimpleProject.hpp>

#include <analysis/DataPoint.hpp>

#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/cloud/OSServer.hpp>

#include <QObject>

#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <deque>

namespace openstudio {
namespace analysisdriver {

namespace detail {

  /** CloudAnalysisDriver_Impl is the implementation class for CloudAnalysisDriver.*/
  class ANALYSISDRIVER_API CloudAnalysisDriver_Impl : public QObject, public boost::enable_shared_from_this<CloudAnalysisDriver_Impl> {
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

    /** Returns the number of data points the CloudAnalysisDriver has been asked to process
     *  since the last time all the queues were cleared. */
    unsigned numDataPointsInIteration() const;

    /** Returns the number of data points in all of the processing queues. */
    unsigned numIncompleteDataPoints() const;

    /** Returns the number of data points in this iteration that are no longer being processed. */
    unsigned numCompleteDataPoints() const;
    
    AnalysisStatus status() const;

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
      T result(boost::dynamic_pointer_cast<typename T::ImplType>(
                 boost::const_pointer_cast<CloudAnalysisDriver_Impl>(shared_from_this())));
      return result;
    }

    //@}
   signals:
    /** @name Non-blocking Call Finished Signals */
    //@{

    void runRequestComplete(bool success);

    void stopRequestComplete(bool success);

    void jsonDownloadRequestsComplete(bool success);

    void detailedDownloadRequestsComplete(bool success);

    //@}
    /** @name AnalysisDriver/CurrentAnalysis Progress Signals */
    //@{

    void resultsChanged();

    void iterationProgress(int numCompletedJobs,int numJobsInIteration);

    // emitted when data point posted to server
    void dataPointQueued(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

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
     void dataPointQueued(bool success);

     // 9. Is the analysis already running on the server?
     //    (Could skip this step if uploaded analysis, but doesn't seem worth the extra
     //    state to do so.)
     void analysisRunningOnServer(bool success);

     // 10. If not, kick it off.
     void analysisStarted(bool success);

     // 11. Wait up to 10 + 2 per data point tries (w/ 1s of sleep) for the server to 
     //     report that the analysis is running.
     void waitingForAnalysisToStart(bool success);

     // 12. Start the monitoring process (if already running or just kicked off).

     // MONITORING =============================================================

     // watch for complete data points
     void completeDataPointUUIDsReturned(bool success);

     // make sure analysis is still running (try to avoid spinning when nothing is happening)
     void analysisStillRunning(bool success);

     // see if data points are still running (optional part of the stopping process)
     void dataPointsStillRunning(bool success);

     // DOWNLOADING ============================================================

     // slim results received
     void jsonDownloadComplete(bool success);

     // pause between slim and detailed results
     void readyForDownloadDataPointUUIDsReturned(bool success);

     // detailed results received
     void detailsDownloadComplete(bool success);

     // STOPPING ===============================================================

     // 0. If are to wait for already running DataPoints, switch the monitoring process to
     //    look for running data points, not for the analysis running.

     // 1. Stop the analysis.
     void analysisStopped(bool success);

     // 2. Wait (almost-as) usual. If isStopping() and all processes have stopped, emit
     //    signal.

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

    // request run process
    boost::optional<OSServer> m_requestRun;
    std::vector<analysis::DataPoint> m_iteration; // DataPoints in this iteration
    bool m_processingQueuesInitialized; // if false, processing queues empty just because
                                        // still spinning up process
    std::deque<analysis::DataPoint> m_postQueue;
    unsigned m_analysisNotRunningCount;
    unsigned m_maxAnalysisNotRunningCount;

    // watch for complete data points
    boost::optional<OSServer> m_monitorDataPoints;
    std::vector<analysis::DataPoint> m_waitingQueue;
    bool m_checkDataPointsRunningInsteadOfAnalysis;
    bool m_lastGetRunningDataPointsSuccess;

    // download slim data points
    boost::optional<OSServer> m_requestJson;
    std::deque<analysis::DataPoint> m_jsonQueue;

    // check to see if details can be downloaded
    boost::optional<OSServer> m_checkForResultsToDownload;
    std::vector<analysis::DataPoint> m_preDetailsQueue;

    // download detailed results
    boost::optional<OSServer> m_requestDetails;
    std::deque<analysis::DataPoint> m_detailsQueue;

    // stop analysis
    boost::optional<OSServer> m_requestStop;

    void clearErrorsAndWarnings();
    void logError(const std::string& error);
    void logWarning(const std::string& warning);
    void appendErrorsAndWarnings(const OSServer& server);
    void setStatus(AnalysisStatus status);

    void registerRunRequestFailure();
    bool postNextDataPoint();
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

    void registerDownloadDetailsRequestFailure();

    void checkForRunCompleteOrStopped();

    bool inIteration(const analysis::DataPoint& dataPoint) const;

    bool inProcessingQueues(const analysis::DataPoint& dataPoint) const;
  };

} // detail

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_CLOUDANALYSISDRIVER_IMPL_HPP

