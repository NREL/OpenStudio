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

#ifndef ANALYSISDRIVER_CLOUDANALYSISDRIVER_HPP
#define ANALYSISDRIVER_CLOUDANALYSISDRIVER_HPP

#include <analysisdriver/AnalysisDriverAPI.hpp>

#include <utilities/core/Logger.hpp>

#include <boost/smart_ptr.hpp>
#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

class CloudProvider;

namespace analysis {
  class DataPoint;
}

namespace analysisdriver {

class SimpleProject;

namespace detail {

  class CloudAnalysisDriver_Impl;

} // detail

/** CloudProvider launches and watches cloud runs of SimpleProjects. As results become
 *  available, they are downloaded and re-integrated into project. */
class ANALYSISDRIVER_API CloudAnalysisDriver {
 public:
  /** @name Constructors and Destructors */
  //@{

  CloudAnalysisDriver(const CloudProvider& provider,
                      const SimpleProject& project);

  virtual ~CloudAnalysisDriver() {}

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

  /** Request the project() to run on the provider(). Returns false if isRunning().
   *  Otherwise returns true and emits runRequestComplete(bool success) when either the
   *  analysis has stopped running on the server or the process has failed. The ultimate
   *  value of success will also be available from lastRunSuccess(). This method will try
   *  to pick up where a previous run left off. */
  bool requestRun();

  /** Request the project() to stop running on the provider(). Returns false if not
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
  boost::shared_ptr<T> getImpl() const {
    return boost::dynamic_pointer_cast<T>(m_impl);
  }

  /** Cast to type T. Throws std::bad_cast if object is not a T. */
  template<typename T>
  T cast() const {
    boost::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
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
    boost::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(impl);
    }
    return result;
  }

  //@}
 protected:
  /// @cond
  typedef detail::CloudAnalysisDriver_Impl ImplType;

  explicit CloudAnalysisDriver(boost::shared_ptr<detail::CloudAnalysisDriver_Impl> impl);

  friend class detail::CloudAnalysisDriver_Impl;
  /// @endcond
 private:
  boost::shared_ptr<detail::CloudAnalysisDriver_Impl> m_impl;

  REGISTER_LOGGER("openstudio.analysisdriver.CloudAnalysisDriver");
};

/** \relates CloudAnalysisDriver*/
typedef boost::optional<CloudAnalysisDriver> OptionalCloudAnalysisDriver;

/** \relates CloudAnalysisDriver*/
typedef std::vector<CloudAnalysisDriver> CloudAnalysisDriverVector;

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_CLOUDANALYSISDRIVER_HPP

