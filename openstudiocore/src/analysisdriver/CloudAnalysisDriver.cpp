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
      m_project(project)
  {}

  CloudProvider CloudAnalysisDriver_Impl::provider() const {
    return m_provider;
  }

  SimpleProject CloudAnalysisDriver_Impl::project() const {
    return m_project;
  }

  bool CloudAnalysisDriver_Impl::hasServer() const {
    return m_server;
  }

  OSServer CloudAnalysisDriver_Impl::server() const {
    OS_ASSERT(m_server);
    return m_server.get();
  }

  bool CloudAnalysisDriver_Impl::run() {
    // for now, don't run if already running
    if (isRunning() || !queue().empty()) {
      LOG(Error,"Cannot run because already running.");
      return false;
    }

    // provider must be running
    if (!provider().running()) {
      LOG(Error,"Cannot run because the server and workers are not all running.");
      return false;
    }

    // need a server
    OptionalUrl serverUrl = provider.serverUrl();
    if (!serverUrl) {
      LOG(Error,"Cannot run because the provider cannot provide the server url.");
      return false;
    }
    m_server = OSServer(*serverUrl);
    if (!server().available()) {
      LOG(Error,"Cannot run because the server is unavailable.");
      return false;
    }

    // start the run
    bool ok(false);
    // register the analysis
    ok = server().postAnalysisJSON(
          project().analysis().uuid(),
          project().analysis().toJSON(AnalysisSerializationOptions(project().projectDir())));
    UUIDVector serverAnalyses = server().serverAnalyses();
    UUIDVector::const_iterator uuidIt;
    uuidIt = std::find(serverAnalyses.begin(),serverAnalyses.end(),project().analysis().uuid());
    ok = ok && (uuidIt != serverAnalyses.end());
    if (!ok) {
      LOG(Error,"Could not post the analysis JSON or confirm that the post succeeded.");
      return false;
    }
    // upload the analysis files
    ok = server().uploadAnalysisFiles(project().analysis().uuid(),
                                      project().zipFileForCloud());
    if (!ok) {
      LOG(Error,"Could not upload this project's files to the cloud provider.");
      return false;
    }
    // post the data points to run
    BOOST_FOREACH(const DataPoint& dataPoint,project().analysis().dataPointsToQueue()) {
      // make sure dataPoint is marked as being run on the cloud
      if (dataPoint.runType() == DataPointRunType::Local) {
        dataPoint.setRunType(DataPointRunType::Cloud-Slim);
      }
      // post to server
      ok = server().postDataPointJSON(
            project().analysis().uuid(),
            dataPoint.toJSON(DataPointSerializationOptions(project().projectDir())));
      if (ok) {
        // keep data point
        m_queue.push_back(dataPoint);
      }
      else {
        LOG(Warn,"Unable to post DataPoint '" << dataPoint.name() <<"'.");
      }
    }

    // tell the server to start
    ok = server().start(project().analysis().uuid());
    if (!ok) {
      LOG(Error,"Unable to start analysis.");
      return false;
    }

    return true;
  }

  bool CloudAnalysisDriver_Impl::isRunning() const {
    if (hasServer()) {
      // HERE -- May also need to see if downloads are complete.
      return server().isAnalysisRunning(project().analysis().uuid());
    }
    return false;
  }

  bool CloudAnalysisDriver_Impl::waitForFinished(int m_secs) {

  }

  void CloudAnalysisDriver_Impl::stop() {
    server().stop(project().analysis().uuid());
    // HERE -- Get last available results?
    m_queue.clear();
  }

  bool CloudAnalysisDriver_Impl::connect(const std::string& signal,
                                         const QObject* qObject,
                                         const std::string& slot,
                                         Qt::ConnectionType type) const
  {
  }

  void CloudAnalysisDriver_Impl::moveToThread(QThread* targetThread) {

  }

} // detail

/// @cond
CloudAnalysisDriver::CloudAnalysisDriver(boost::shared_ptr<detail::CloudAnalysisDriver_Impl> impl)
  : m_impl(impl)
{}
/// @endcond

} // analysisdriver
} // openstudio

