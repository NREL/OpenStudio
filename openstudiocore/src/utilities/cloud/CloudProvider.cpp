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
      : m_uuid(createUUID()),
        m_versionUUID(createUUID())
    {
    }

    CloudSettings_Impl::CloudSettings_Impl(const UUID& uuid,
                                           const UUID& versionUUID)
      : m_uuid(uuid),
        m_versionUUID(versionUUID)
    {}

    CloudSettings_Impl::~CloudSettings_Impl()
    {
    }

    UUID CloudSettings_Impl::uuid() const {
      return m_uuid;
    }

    UUID CloudSettings_Impl::versionUUID() const {
      return m_versionUUID;
    }

    void CloudSettings_Impl::onChange() {
      m_versionUUID = createUUID();
    }

    CloudSession_Impl::CloudSession_Impl(const std::string& sessionId, 
                                         const boost::optional<Url>& serverUrl, 
                                         const std::vector<Url>& workerUrls)
      : m_uuid(createUUID()),
        m_versionUUID(createUUID()),
        m_sessionId(sessionId), 
        m_serverUrl(serverUrl), 
        m_workerUrls(workerUrls)
    {
    }

    CloudSession_Impl::CloudSession_Impl(const UUID& uuid,
                                         const UUID& versionUUID,
                                         const std::string& sessionId,
                                         const boost::optional<Url>& serverUrl,
                                         const std::vector<Url>& workerUrls)
      : m_uuid(uuid),
        m_versionUUID(versionUUID),
        m_sessionId(sessionId),
        m_serverUrl(serverUrl),
        m_workerUrls(workerUrls)
    {}

    CloudSession_Impl::~CloudSession_Impl()
    {
    }

    UUID CloudSession_Impl::uuid() const {
      return m_uuid;
    }

    UUID CloudSession_Impl::versionUUID() const {
      return m_versionUUID;
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
      onChange();
    }

    void CloudSession_Impl::resetServerUrl()
    {
      m_serverUrl.reset();
      onChange();
    }

    std::vector<Url> CloudSession_Impl::workerUrls() const
    {
      return m_workerUrls;
    }

    void CloudSession_Impl::addWorkerUrl(const Url& workerUrl)
    {
      m_workerUrls.push_back(workerUrl);
      onChange();
    }

    void CloudSession_Impl::clearWorkerUrls()
    {
      m_workerUrls.clear();
      onChange();
    }

    void CloudSession_Impl::onChange() {
      m_versionUUID = createUUID();
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

  UUID CloudSettings::uuid() const {
    return getImpl<detail::CloudSettings_Impl>()->uuid();
  }

  UUID CloudSettings::versionUUID() const {
    return getImpl<detail::CloudSettings_Impl>()->versionUUID();
  }

  std::string CloudSettings::cloudProviderType() const
  {
    return getImpl<detail::CloudSettings_Impl>()->cloudProviderType();
  }

  std::string CloudSettings::userAgreementText() const
  {
    return getImpl<detail::CloudSettings_Impl>()->userAgreementText();
  }

  bool CloudSettings::userAgreementSigned() const
  {
    return getImpl<detail::CloudSettings_Impl>()->userAgreementSigned();
  }

  void CloudSettings::signUserAgreement(bool agree)
  {
    getImpl<detail::CloudSettings_Impl>()->signUserAgreement(agree);
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

  UUID CloudSession::uuid() const {
    return getImpl<detail::CloudSession_Impl>()->uuid();
  }

  UUID CloudSession::versionUUID() const {
    return getImpl<detail::CloudSession_Impl>()->versionUUID();
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
