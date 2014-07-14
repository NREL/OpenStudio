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

#ifndef ANALYSISDRIVER_CLOUDANALYSISDRIVER_HPP
#define ANALYSISDRIVER_CLOUDANALYSISDRIVER_HPP

#include "AnalysisDriverAPI.hpp"
#include "AnalysisDriverEnums.hpp"

#include "../utilities/core/Logger.hpp"

#include <boost/smart_ptr.hpp>
#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

class CloudSession;

namespace analysis {
  class DataPoint;
}

namespace analysisdriver {

class SimpleProject;

namespace detail {

  class CloudAnalysisDriver_Impl;

} // detail

/** CloudAnalysisDriver launches and watches cloud runs of SimpleProjects. As results become
 *  available, they are downloaded and re-integrated into project. */
class ANALYSISDRIVER_API CloudAnalysisDriver {
 public:
  /** @name Constructors and Destructors */
  //@{

  CloudAnalysisDriver(const CloudSession& session,
                      const SimpleProject& project);

  virtual ~CloudAnalysisDriver() {}

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

  /** Request the project() to run on session(). Returns false if isRunning() or 
   *  isStopping() or isDownloading(). Also returns false if there are no data points to 
   *  queue. Otherwise returns true and emits runRequestComplete(bool success) when either 
   *  the analysis has stopped running on the server or the process has failed. The ultimate
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

  /** Connect signal from this CloudAnalysisDriver to slot on qObject. */
  bool connect(const std::string& signal,
               const QObject* qObject,
               const std::string& slot,
               Qt::ConnectionType type = Qt::AutoConnection) const;

  void moveToThread(QThread* targetThread);

  //@}
  /** @name Type Casting */
  //@{

  /** Get the impl pointer */
  template<typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  /** Cast to type T. Throws std::bad_cast if object is not a T. */
  template<typename T>
  T cast() const {
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl) {
      throw(std::bad_cast());
    }
    return T(impl);
  }

  /** Cast to boost::optional<T>. Return value is boost::none (evaluates to false) 
   *  if object is not a T. */
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(impl);
    }
    return result;
  }

  //@}
 protected:
  /// @cond
  typedef detail::CloudAnalysisDriver_Impl ImplType;

  explicit CloudAnalysisDriver(std::shared_ptr<detail::CloudAnalysisDriver_Impl> impl);

  friend class detail::CloudAnalysisDriver_Impl;
  /// @endcond
 private:
  std::shared_ptr<detail::CloudAnalysisDriver_Impl> m_impl;

  REGISTER_LOGGER("openstudio.analysisdriver.CloudAnalysisDriver");
};

/** \relates CloudAnalysisDriver*/
typedef boost::optional<CloudAnalysisDriver> OptionalCloudAnalysisDriver;

/** \relates CloudAnalysisDriver*/
typedef std::vector<CloudAnalysisDriver> CloudAnalysisDriverVector;

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_CLOUDANALYSISDRIVER_HPP

