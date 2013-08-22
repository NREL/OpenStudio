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
#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/cloud/CloudProvider_Impl.hpp>
#include <utilities/core/Application.hpp>

namespace openstudio{
  namespace detail {

    CloudProvider_Impl::CloudProvider_Impl()
      : QObject()
    {
      //Make sure a QApplication exists
      openstudio::Application::instance().application();
    }

    CloudProvider_Impl::~CloudProvider_Impl()
    {
    }

  } // detail

  CloudSession::CloudSession(const std::string& cloudProviderType, const std::string& sessionId, const boost::optional<Url>& serverUrl, const std::vector<Url>& workerUrls)
    : m_cloudProviderType(cloudProviderType), m_sessionId(sessionId), m_serverUrl(serverUrl), m_workerUrls(workerUrls)
  {}

  std::string CloudSession::cloudProviderType() const
  {
    return m_cloudProviderType;
  }

  std::string CloudSession::sessionId() const
  {
    return m_sessionId;
  }

  boost::optional<Url> CloudSession::serverUrl() const
  {
    return m_serverUrl;
  }
    
  void CloudSession::setServerUrl(const Url& serverUrl)
  {
    m_serverUrl = serverUrl;
  }

  void CloudSession::resetServerUrl()
  {
    m_serverUrl.reset();
  }

  std::vector<Url> CloudSession::workerUrls() const
  {
    return m_workerUrls;
  }

  void CloudSession::addWorkerUrl(const Url& workerUrl)
  {
    m_workerUrls.push_back(workerUrl);
  }

  void CloudSession::clearWorkerUrls()
  {
    m_workerUrls.clear();
  }


  CloudProvider::CloudProvider(const boost::shared_ptr<detail::CloudProvider_Impl>& impl)
    : m_impl(impl)
  {
  }

  CloudProvider::~CloudProvider()
  {
  }

  std::string CloudProvider::type() const
  {
    return getImpl<detail::CloudProvider_Impl>()->type();
  }
  
  bool CloudProvider::internetAvailable() const
  {
    return getImpl<detail::CloudProvider_Impl>()->internetAvailable();
  }

  bool CloudProvider::serviceAvailable() const
  {
    return getImpl<detail::CloudProvider_Impl>()->serviceAvailable();
  }

  bool CloudProvider::validateCredentials() const
  {
    return getImpl<detail::CloudProvider_Impl>()->validateCredentials();
  }

  CloudSession CloudProvider::session() const
  {
    return getImpl<detail::CloudProvider_Impl>()->session();
  }

  bool CloudProvider::reconnect(const CloudSession& session)
  {
    return getImpl<detail::CloudProvider_Impl>()->reconnect(session);
  }

  boost::optional<Url> CloudProvider::serverUrl() const
  {
    return getImpl<detail::CloudProvider_Impl>()->serverUrl();
  }

  bool CloudProvider::startServer()
  {
    return getImpl<detail::CloudProvider_Impl>()->startServer();
  }

  std::vector<Url> CloudProvider::workerUrls() const
  {
    return getImpl<detail::CloudProvider_Impl>()->workerUrls();
  }

  unsigned CloudProvider::numWorkers() const
  {
    return getImpl<detail::CloudProvider_Impl>()->numWorkers();
  }

  bool CloudProvider::startWorkers()
  {
    return getImpl<detail::CloudProvider_Impl>()->startWorkers();
  }

  bool CloudProvider::running() const
  {
    return getImpl<detail::CloudProvider_Impl>()->running();
  }

  bool CloudProvider::terminate()
  {
    return getImpl<detail::CloudProvider_Impl>()->terminate();
  }

  bool CloudProvider::terminated() const
  {
    return getImpl<detail::CloudProvider_Impl>()->terminated();
  }

  std::vector<std::string> CloudProvider::errors() const
  {
    return getImpl<detail::CloudProvider_Impl>()->errors();
  }
  
  std::vector<std::string> CloudProvider::warnings() const
  {
    return getImpl<detail::CloudProvider_Impl>()->warnings();
  }

} // openstudio
