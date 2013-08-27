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

#include <analysisdriver/AnalysisdriverAPI.hpp>

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

    CloudAnalysisDriver_Impl(const CloudProvider& provider,
                             const SimpleProject& project);

    virtual ~CloudAnalysisDriver_Impl() {}

    //@}
    /** @name Getters */
    //@{

    CloudProvider provider() const;

    SimpleProject project() const;

    //@}
    /** @name Blocking Class Members */
    //@{

    bool run(int msec=-1);
    bool lastRunSuccess() const;

    bool stop(int msec=-1);
    bool lastStopSuccess() const;

    bool downloadDetailedResults(analysis::DataPoint& dataPoint);
    bool lastDownloadDetailedResultsSuccess() const;

    bool isRunning() const;

    bool isDownloading() const;

    /** If no argument is specified, CloudAnalysisDriver will run until not isRunning() and
     *  not isDownloading(). If msec is specified, will wait for at most msec milliseconds.
     *  Returns true if not isRunning() and not isDownloading() upon exit; false otherwise. */
    bool waitForFinished(int msec=-1);

    //@}
    /** @name Non-blocking class members */
    //@{

    bool requestRun();

    bool requestStop();

    bool requestDownloadDetailedResults(analysis::DataPoint& dataPoint);

    //@}
    /** @name Signals, Slots, Threads */
    //@{

    bool connect(const std::string& signal,
                 const QObject* qObject,
                 const std::string& slot,
                 Qt::ConnectionType type = Qt::AutoConnection) const;

    void moveToThread(QThread* targetThread);

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

    //@}
    /** @name AnalysisDriver Progress Signals */
    //@{

    void resultsChanged();

    // emitted when data point posted to server
    void dataPointQueued(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

    // emitted when data point results downloaded and incorporated into project
    void dataPointComplete(const openstudio::UUID& analysis, const openstudio::UUID& dataPoint);

    // emitted when last data point downloaded and incorporated into project
    void analysisComplete(const openstudio::UUID& analysis);

    void analysisStopped(const openstudio::UUID& analysis);

    //@}
   protected slots:
     // request run process
     void availableForRun(bool success);
     void analysisPosted(bool success);
     void analysisUploaded(bool success);
     void dataPointQueued(bool success);
     void analysisStarted(bool success);

     // watch for complete data points
     void completeDataPointUUIDsReturned(bool success);

     // download process
     void dataPointDownloadComplete(bool success);

   protected:
   private:
    REGISTER_LOGGER("openstudio.analysisdriver.CloudAnalysisDriver");

    CloudProvider m_provider;
    SimpleProject m_project;

    bool m_lastRunSuccess;
    bool m_lastStopSuccess;
    bool m_lastDownloadDetaileResultsSuccess;

    // request run process
    boost::optional<OSServer> m_requestRun;
    std::deque<DataPoint> m_uploadQueue;

    // watch for complete data points
    boost::optional<OSServer> m_monitorDataPoints;
    std::vector<DataPoint> m_waitingQueue;

    // download data points
    boost::optional<OSServer> m_requestDownload;
    std::deque<DataPoint> m_downloadQueue;

    bool startDownloading();

    bool requestDataPointDownload(const DataPoint& dataPoint);
  };

} // detail

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_CLOUDANALYSISDRIVER_IMPL_HPP

