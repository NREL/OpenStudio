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

#include <utilities/core/Assert.hpp>

namespace openstudio{
  namespace detail {

    CloudSettings_Impl::CloudSettings_Impl()
    {
    }

    CloudSettings_Impl::~CloudSettings_Impl()
    {
    }

    CloudSession_Impl::CloudSession_Impl(const std::string& sessionId, const boost::optional<Url>& serverUrl, const std::vector<Url>& workerUrls)
      : m_sessionId(sessionId), m_serverUrl(serverUrl), m_workerUrls(workerUrls)
    {
    }

    CloudSession_Impl::~CloudSession_Impl()
    {
    }

    std::string CloudSession_Impl::sessionId() const
    {
      return m_sessionId;
    }

    boost::optional<Url> CloudSession_Impl::serverUrl() const
    {
      return m_serverUrl;
    }
      
    void CloudSession_Impl::setServerUrl(const Url& serverUrl)
    {
      m_serverUrl = serverUrl;
    }

    void CloudSession_Impl::resetServerUrl()
    {
      m_serverUrl.reset();
    }

    std::vector<Url> CloudSession_Impl::workerUrls() const
    {
      return m_workerUrls;
    }

    void CloudSession_Impl::addWorkerUrl(const Url& workerUrl)
    {
      m_workerUrls.push_back(workerUrl);
    }

    void CloudSession_Impl::clearWorkerUrls()
    {
      m_workerUrls.clear();
    }

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

  CloudSettings::CloudSettings(const boost::shared_ptr<detail::CloudSettings_Impl>& impl)
    : m_impl(impl)
  {
    OS_ASSERT(getImpl<detail::CloudSettings_Impl>());
  }

  CloudSettings::~CloudSettings()
  {
  }

  std::string CloudSettings::cloudProviderType() const
  {
    return getImpl<detail::CloudSettings_Impl>()->cloudProviderType();
  }
  
  bool CloudSettings::loadSettings(bool overwriteExisting)
  {
    return getImpl<detail::CloudSettings_Impl>()->loadSettings(overwriteExisting);
  }

  bool CloudSettings::saveToSettings(bool overwriteExisting) const
  {
    return getImpl<detail::CloudSettings_Impl>()->saveToSettings(overwriteExisting);
  }

  CloudSession::CloudSession(const boost::shared_ptr<detail::CloudSession_Impl>& impl)
    : m_impl(impl)
  {
    OS_ASSERT(getImpl<detail::CloudSession_Impl>());
  }

  CloudSession::~CloudSession()
  {
  }

  std::string CloudSession::cloudProviderType() const
  {
    return getImpl<detail::CloudSession_Impl>()->cloudProviderType();
  }

  std::string CloudSession::sessionId() const
  {
    return getImpl<detail::CloudSession_Impl>()->sessionId();
  }

  boost::optional<Url> CloudSession::serverUrl() const
  {
    return getImpl<detail::CloudSession_Impl>()->serverUrl();
  }
    
  void CloudSession::setServerUrl(const Url& serverUrl)
  {
    getImpl<detail::CloudSession_Impl>()->setServerUrl(serverUrl);
  }

  void CloudSession::resetServerUrl()
  {
    getImpl<detail::CloudSession_Impl>()->resetServerUrl();
  }

  std::vector<Url> CloudSession::workerUrls() const
  {
    return getImpl<detail::CloudSession_Impl>()->workerUrls();
  }

  void CloudSession::addWorkerUrl(const Url& workerUrl)
  {
    getImpl<detail::CloudSession_Impl>()->addWorkerUrl(workerUrl);
  }

  void CloudSession::clearWorkerUrls()
  {
    getImpl<detail::CloudSession_Impl>()->clearWorkerUrls();
  }


  CloudProvider::CloudProvider(const boost::shared_ptr<detail::CloudProvider_Impl>& impl)
    : m_impl(impl)
  {
    OS_ASSERT(getImpl<detail::CloudProvider_Impl>());
  }

  CloudProvider::~CloudProvider()
  {
  }

  std::string CloudProvider::type() const
  {
    return getImpl<detail::CloudProvider_Impl>()->type();
  }

  std::string CloudProvider::userAgreementText() const
  {
    return getImpl<detail::CloudProvider_Impl>()->userAgreementText();
  }

  bool CloudProvider::userAgreementSigned() const
  {
    return getImpl<detail::CloudProvider_Impl>()->userAgreementSigned();
  }

  void CloudProvider::signUserAgreement(bool agree)
  {
    getImpl<detail::CloudProvider_Impl>()->signUserAgreement(agree);
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

  CloudSettings CloudProvider::settings() const
  {
    return getImpl<detail::CloudProvider_Impl>()->settings();
  }

  bool CloudProvider::setSettings(const CloudSettings& settings)
  {
    return getImpl<detail::CloudProvider_Impl>()->setSettings(settings);
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
