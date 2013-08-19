/**********************************************************************
* Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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
#include <utilities/cloud/OSServer.hpp>
#include <utilities/cloud/OSServer_Impl.hpp>
#include <utilities/core/Application.hpp>

namespace openstudio{
  namespace detail{

    OSServer_Impl::OSServer_Impl(const QUrl& url)
      : QObject()
    {
      //Make sure a QApplication exists
      openstudio::Application::instance().application();
    }

    OSServer_Impl::~OSServer_Impl()
    {
    }

    bool OSServer_Impl::available() const
    {
      return false;
    }

    std::vector<UUID> OSServer_Impl::projectUUIDs() const
    {
      return std::vector<UUID>();
    }

    std::vector<UUID> OSServer_Impl::analysisUUIDs() const
    {
      return std::vector<UUID>();
    }

    bool OSServer_Impl::postAnalysisJSON(const UUID& projectUUID, const std::string& analysisJSON) const
    {
      return false;
    }

    bool OSServer_Impl::postDataPointJSON(const UUID& analysisUUID, const std::string& dataPointJSON) const
    {
      return false;
    }

    // zip file created by SimpleProject_Impl::zipFileForRemoteSystem(), base 64 this file
    bool OSServer_Impl::uploadAnalysisFiles(const UUID& analysisUUID, const openstudio::path& analysisZipFile)
    {
      return false;
    }

    bool OSServer_Impl::start(const UUID& analysisUUID) const
    {
      return false;
    }

    bool OSServer_Impl::isAnalysisRunning(const UUID& analysisUUID) const
    {
      return false;
    }

    bool OSServer_Impl::stop(const UUID& analysisUUID) const
    {
      return false;
    }

    std::vector<UUID> OSServer_Impl::dataPointsJSON(const UUID& analysisUUID) const
    {
      return std::vector<UUID>();
    }

    std::vector<UUID> OSServer_Impl::runningDataPointsJSON(const UUID& analysisUUID) const
    {
      return std::vector<UUID>();
    }

    std::vector<UUID> OSServer_Impl::queuedDataPointsJSON(const UUID& analysisUUID) const
    {
      return std::vector<UUID>();
    }

    std::vector<UUID> OSServer_Impl::completeDataPointsJSON(const UUID& analysisUUID) const
    {
      return std::vector<UUID>();
    }

    std::string OSServer_Impl::getDataPointJSON(const UUID& analysisUUID, const UUID& dataPointUUID) const
    {
      return "";
    }

    bool OSServer_Impl::downloadDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, const openstudio::path& downloadPath) const
    {
      return false;
    }

    std::vector<std::string> OSServer_Impl::errors() const
    {
      return std::vector<std::string>();
    }

    std::vector<std::string> OSServer_Impl::warnings() const
    {
      return std::vector<std::string>();
    }
  }

  OSServer::OSServer(const QUrl& url)
    : m_impl(boost::shared_ptr<detail::OSServer_Impl>(new detail::OSServer_Impl(url)))
  {
  }

  OSServer::~OSServer()
  {
  }

  bool OSServer::available() const
  {
    return getImpl<detail::OSServer_Impl>()->available();
  }

  std::vector<UUID> OSServer::projectUUIDs() const
  {
    return getImpl<detail::OSServer_Impl>()->projectUUIDs();
  }

  std::vector<UUID> OSServer::analysisUUIDs() const
  {
    return getImpl<detail::OSServer_Impl>()->analysisUUIDs();
  }

  bool OSServer::postAnalysisJSON(const UUID& projectUUID, const std::string& analysisJSON) const
  {
    return getImpl<detail::OSServer_Impl>()->postAnalysisJSON(projectUUID, analysisJSON);
  }

  bool OSServer::postDataPointJSON(const UUID& analysisUUID, const std::string& dataPointJSON) const
  {
    return getImpl<detail::OSServer_Impl>()->postDataPointJSON(analysisUUID, dataPointJSON);
  }

  bool OSServer::uploadAnalysisFiles(const UUID& analysisUUID, const openstudio::path& analysisZipFile)
  {
    return getImpl<detail::OSServer_Impl>()->uploadAnalysisFiles(analysisUUID, analysisZipFile);
  }

  bool OSServer::start(const UUID& analysisUUID) const
  {
    return getImpl<detail::OSServer_Impl>()->start(analysisUUID);
  }

  bool OSServer::isAnalysisRunning(const UUID& analysisUUID) const
  {
    return getImpl<detail::OSServer_Impl>()->isAnalysisRunning(analysisUUID);
  }

  bool OSServer::stop(const UUID& analysisUUID) const
  {
    return getImpl<detail::OSServer_Impl>()->stop(analysisUUID);
  }

  std::vector<UUID> OSServer::dataPointsJSON(const UUID& analysisUUID) const
  {
    return getImpl<detail::OSServer_Impl>()->dataPointsJSON(analysisUUID);
  }

  std::vector<UUID> OSServer::runningDataPointsJSON(const UUID& analysisUUID) const
  {
    return getImpl<detail::OSServer_Impl>()->runningDataPointsJSON(analysisUUID);
  }

  std::vector<UUID> OSServer::queuedDataPointsJSON(const UUID& analysisUUID) const
  {
    return getImpl<detail::OSServer_Impl>()->queuedDataPointsJSON(analysisUUID);
  }

  std::vector<UUID> OSServer::completeDataPointsJSON(const UUID& analysisUUID) const
  {
    return getImpl<detail::OSServer_Impl>()->completeDataPointsJSON(analysisUUID);
  }

  std::string OSServer::getDataPointJSON(const UUID& analysisUUID, const UUID& dataPointUUID) const
  {
    return getImpl<detail::OSServer_Impl>()->getDataPointJSON(analysisUUID, dataPointUUID);
  }

  bool OSServer::downloadDataPoint(const UUID& analysisUUID, const UUID& dataPointUUID, const openstudio::path& downloadPath) const
  {
    return getImpl<detail::OSServer_Impl>()->downloadDataPoint(analysisUUID, dataPointUUID, downloadPath);
  }

  std::vector<std::string> OSServer::errors() const
  {
    return getImpl<detail::OSServer_Impl>()->errors();
  }

  std::vector<std::string> OSServer::warnings() const
  {
    return getImpl<detail::OSServer_Impl>()->warnings();
  }


} // openstudio
