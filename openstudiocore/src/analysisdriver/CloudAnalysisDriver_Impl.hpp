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

    /// not in public object
    bool hasServer() const;

    /// not in public object
    OSServer server() const;

    /// not in public object
    std::vector<analysis::DataPoint> queue() const;

    //@}
    /** @name Run Management */
    //@{

    bool run();

    bool isRunning() const;

    bool waitForFinished(int m_secs=-1);

    void stop();

    bool connect(const std::string& signal,
                 const QObject* qObject,
                 const std::string& slot,
                 Qt::ConnectionType type = Qt::AutoConnection) const;

    void moveToThread(QThread* targetThread);

    //@}
    /** @name Actions */
    //@{

    /** If dataPoint was run on the cloud, but its detailed results have not yet been downloaded,
     *  download them. */
    bool downloadDetailedResults(analysis::DataPoint& dataPoint) const;

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
   protected:
   private:
    REGISTER_LOGGER("openstudio.analysisdriver.CloudAnalysisDriver");

    CloudProvider m_provider;
    SimpleProject m_project;
    boost::optional<OSServer> m_server;
    std::vector<analysis::DataPoint> m_queue;
  };

} // detail

} // analysisdriver
} // openstudio

#endif // ANALYSISDRIVER_CLOUDANALYSISDRIVER_IMPL_HPP

