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
#include <utilities/cloud/AWSProvider.hpp>
#include <utilities/cloud/AWSProvider_Impl.hpp>

#include <utilities/core/Application.hpp>
#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/String.hpp>

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QTemporaryFile>
#include <QTextStream>
#include <QUrl>

namespace openstudio{
  namespace detail{

    AWSSettings_Impl::AWSSettings_Impl()
      : CloudSettings_Impl()
    {}

    AWSSettings_Impl::AWSSettings_Impl(const UUID& uuid,
                                       const UUID& versionUUID)
      : CloudSettings_Impl(uuid,versionUUID)
    {}

    AWSSettings_Impl::~AWSSettings_Impl()
    {}

    std::string AWSSettings_Impl::cloudProviderType() const {
      return AWSProvider_Impl::cloudProviderType();
    }

    std::string AWSSettings_Impl::userAgreementText() const {
      return std::string();
    }

    bool AWSSettings_Impl::userAgreementSigned() const {
      return false;
    }

    void AWSSettings_Impl::signUserAgreement(bool agree) {

    }

    bool AWSSettings_Impl::loadSettings(bool overwriteExisting) {
      return false;
    }

    bool AWSSettings_Impl::saveToSettings(bool overwriteExisting) const {
      return false;
    }

    AWSSession_Impl::AWSSession_Impl(const std::string& sessionId,
                                     const boost::optional<Url>& serverUrl,
                                     const std::vector<Url>& workerUrls)
      : CloudSession_Impl(sessionId,serverUrl,workerUrls)
    {}

    AWSSession_Impl::AWSSession_Impl(const UUID& uuid,
                                     const UUID& versionUUID,
                                     const std::string& sessionId,
                                     const boost::optional<Url>& serverUrl,
                                     const std::vector<Url>& workerUrls)
      : CloudSession_Impl(uuid,versionUUID,sessionId,serverUrl,workerUrls)
    {}

    AWSSession_Impl::~AWSSession_Impl()
    {}

    std::string AWSSession_Impl::cloudProviderType() const {
      return AWSProvider_Impl::cloudProviderType();
    }

    AWSProvider_Impl::AWSProvider_Impl()
      : CloudProvider_Impl(),
        m_awsSettings(),
        m_awsSession(toString(createUUID()),boost::none,std::vector<Url>()),
        m_validAccessKey(false),
        m_validSecretKey(false),
        m_numWorkers(0),
        m_startServerProcess(NULL), m_startWorkerProcess(NULL),
        m_serverStarted(false), m_workerStarted(false), m_serverStopped(false), m_workerStopped(false), m_terminated(false)
    {
      //Make sure a QApplication exists
      openstudio::Application::instance().application();

      if (loadCredentials()) {
        validateCredentials();
      }
    }

    std::string AWSProvider_Impl::type() const
    {
      return "AWSProvider";
    }

    std::string AWSProvider_Impl::userAgreementText() const
    {
      return "NREL is not responsible for any charges you may incur as a result of using the OpenStudio suite with Amazon Web Services.";
    }

    bool AWSProvider_Impl::userAgreementSigned() const
    {
      QSettings settings("OpenStudio", "AWSProvider");
      QString value = settings.value("userAgreementSigned", "No").toString();

      bool result = false;
      if (value == "Yes"){
        result = true;
      }

      return result;
    }

    void AWSProvider_Impl::signUserAgreement(bool agree)
    {
      QString value = agree ? "Yes" : "No";
      QSettings settings("OpenStudio", "AWSProvider");
      settings.setValue("userAgreementSigned", value);
    }

    bool AWSProvider_Impl::internetAvailable() const
    {
      return serviceAvailable();
    }

    bool AWSProvider_Impl::serviceAvailable() const
    {
      QVariantMap map = awsRequest("describe_availability_zones");
      if (!map.keys().contains("error")) {
        return true;
      }
      int code = map["error"].toMap()["code"].toInt();
      return (code != 503);
    }

    bool AWSProvider_Impl::validateCredentials() const
    {
      if (m_validAccessKey && m_validSecretKey) {
        return true;
      }

      QVariantMap map = awsRequest("describe_availability_zones");
      if (!map.keys().contains("error")) {
        m_validAccessKey = true;
        m_validSecretKey = true;
        return true;
      }

      int code = map["error"].toMap()["code"].toInt();
      if (code == 401) {
        m_validAccessKey = false;
      } else if (code == 403) {
        m_validAccessKey = true;
        m_validSecretKey = false;
      }
      return false;
    }

    CloudSettings AWSProvider_Impl::settings() const {
      return m_awsSettings;
    }

    bool AWSProvider_Impl::setSettings(const CloudSettings& settings) {
      if (OptionalAWSSettings candidate = settings.optionalCast<AWSSettings>()) {
        m_awsSettings = *candidate;
        return true;
      }
      return false;
    }

    CloudSession AWSProvider_Impl::session() const
    {
      return m_awsSession;
    }

    bool AWSProvider_Impl::reconnect(const CloudSession& session)
    {
      return false;
    }

    boost::optional<Url> AWSProvider_Impl::serverUrl() const
    {
      return OptionalUrl();
    }

    bool AWSProvider_Impl::startServer()
    {
      QVariantMap map = awsRequest("launch_instance");
      return !map.keys().contains("error");
    }

    std::vector<Url> AWSProvider_Impl::workerUrls() const
    {
      return UrlVector();
    }

    unsigned AWSProvider_Impl::numWorkers() const
    {
      return m_numWorkers;
    }

    bool AWSProvider_Impl::startWorkers()
    {
      QVariantMap map = awsRequest("launch_slaves");
      return !map.keys().contains("error");
    }

    bool AWSProvider_Impl::running() const
    {
      return false;
    }

    bool AWSProvider_Impl::terminate()
    {
      return false;
    }

    bool AWSProvider_Impl::terminated() const
    {
      return false;
    }

    std::vector<std::string> AWSProvider_Impl::errors() const
    {
      return std::vector<std::string>();
    }

    std::vector<std::string> AWSProvider_Impl::warnings() const
    {
      return std::vector<std::string>();
    }

    std::string AWSProvider_Impl::cloudProviderType() {
      return "AWSProvider";
    }
     
    void AWSProvider_Impl::clearErrorsAndWarnings() const
    {
      m_errors.clear();
      m_warnings.clear();
    }

    void AWSProvider_Impl::logError(const std::string& error) const
    {
      m_errors.push_back(error);
      LOG(Error, error);
    }

    void AWSProvider_Impl::logWarning(const std::string& warning) const
    {
      m_warnings.push_back(warning);
      LOG(Warn, warning);
    }



    bool AWSProvider_Impl::loadCredentials() const {
      QFile file(QDir::homePath() + "/.ssh/aws");
      if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJson::Parser parser;
        bool ok = false;
        QVariantMap map = parser.parse(QString(file.readAll()).toUtf8(), &ok).toMap();
        
        if (ok) {
          if (validAccessKey(toString(map["accessKey"].toString())) && validSecretKey(toString(map["secretKey"].toString()))) {
            m_accessKey = toString(map["accessKey"].toString());
            m_secretKey = toString(map["secretKey"].toString());
            return true;
          }
        }
      }
      return false;
    }

    bool AWSProvider_Impl::saveCredentials() const {
      QVariantMap json;
      json.insert("accessKey", toQString(m_accessKey));
      json.insert("secretKey", toQString(m_secretKey));
      
      QJson::Serializer serializer;
      if (QDir::home().exists(".ssh") || QDir::home().mkdir(".ssh")) {
        QFile file(QDir::homePath() + "/.ssh/aws");
        if (file.open(QIODevice::WriteOnly)) {
          file.write(serializer.serialize(json));
          file.close();
          return true;
        }
      }
      return false;
    }

    bool AWSProvider_Impl::validAccessKey(std::string accessKey) const {
      return QRegExp("[A-Z0-9]{20}").exactMatch(toQString(accessKey));
    }

    bool AWSProvider_Impl::validSecretKey(std::string secretKey) const {
      return QRegExp("[a-zA-Z0-9/+]{40}").exactMatch(toQString(secretKey));
    }

    std::string AWSProvider_Impl::accessKey() const {
      return m_accessKey;
    }
    
    std::string AWSProvider_Impl::secretKey() const {
      return m_secretKey;
    }
    
    bool AWSProvider_Impl::setKeys(std::string accessKey, std::string secretKey) const {
      if (validAccessKey(accessKey) && validSecretKey(secretKey)) {
        m_accessKey = accessKey;
        m_secretKey = secretKey;

        return true;
      }
      return false;
    }

    QVariantMap AWSProvider_Impl::awsRequest(std::string request, std::string service) const {
      QString script = toQString(getOpenStudioRubyScriptsPath() / toPath("cloud/aws.rb"));
      QProcess *ruby2 = new QProcess();
      ruby2->start("\"C:\\Ruby200\\bin\\ruby.exe\"", QStringList() << script << toQString(accessKey()) << toQString(secretKey()) << toQString(service) << toQString(request));
      ruby2->waitForFinished();
      QString output = ruby2->readAllStandardOutput();
      QString errors = ruby2->readAllStandardError();
      LOG(Debug, output.toStdString());

      if (errors.length()) {
        LOG(Error, errors.toStdString());
      } else {
        QJson::Parser parser;
        bool ok = false;
        QVariantMap map = parser.parse(output.toUtf8(), &ok).toMap();
        if (ok) {
          return map;
        } else {
          LOG(Error, "Error parsing JSON: " + toString(parser.errorString()));
        }
      }
      return QVariantMap();
    }

    void AWSProvider_Impl::setNumWorkers(const unsigned numWorkers)
    {
      m_numWorkers = numWorkers;
    }


    void AWSProvider_Impl::onServerStarted(int, QProcess::ExitStatus)
    {
    }

    void AWSProvider_Impl::onWorkerStarted(int, QProcess::ExitStatus)
    {
    }

    void AWSProvider_Impl::onServerStopped(int, QProcess::ExitStatus)
    {
    }

    void AWSProvider_Impl::onWorkerStopped(int, QProcess::ExitStatus)
    {
    }



  } // detail

  AWSSettings::AWSSettings()
    : CloudSettings(boost::shared_ptr<detail::AWSSettings_Impl>(
                      new detail::AWSSettings_Impl()))
  {
    OS_ASSERT(getImpl<detail::AWSSettings_Impl>());
  }

  AWSSettings::AWSSettings(const UUID& uuid,
                           const UUID& versionUUID)
    : CloudSettings(boost::shared_ptr<detail::AWSSettings_Impl>(
                      new detail::AWSSettings_Impl(uuid,versionUUID)))
  {
    OS_ASSERT(getImpl<detail::AWSSettings_Impl>());
  }

  AWSSettings::~AWSSettings()
  {}

  AWSSettings::AWSSettings(const boost::shared_ptr<detail::AWSSettings_Impl>& impl)
    : CloudSettings(impl)
  {
    OS_ASSERT(getImpl<detail::AWSSettings_Impl>());
  }

  AWSSession::AWSSession(const std::string& sessionId,
                         const boost::optional<Url>& serverUrl,
                         const std::vector<Url>& workerUrls)
    : CloudSession(boost::shared_ptr<detail::AWSSession_Impl>(
                     new detail::AWSSession_Impl(sessionId,
                                                 serverUrl,
                                                 workerUrls)))
  {
    OS_ASSERT(getImpl<detail::AWSSession_Impl>());
  }

  AWSSession::AWSSession(const UUID& uuid,
                         const UUID& versionUUID,
                         const std::string& sessionId,
                         const boost::optional<Url>& serverUrl,
                         const std::vector<Url>& workerUrls)
    : CloudSession(boost::shared_ptr<detail::AWSSession_Impl>(
                     new detail::AWSSession_Impl(uuid,
                                                 versionUUID,
                                                 sessionId,
                                                 serverUrl,
                                                 workerUrls)))
  {
    OS_ASSERT(getImpl<detail::AWSSession_Impl>());
  }

  AWSSession::~AWSSession()
  {}

  AWSSession::AWSSession(const boost::shared_ptr<detail::AWSSession_Impl>& impl)
    : CloudSession(impl)
  {
    OS_ASSERT(getImpl<detail::AWSSession_Impl>());
  }

  AWSProvider::AWSProvider()
    : CloudProvider(boost::shared_ptr<detail::AWSProvider_Impl>(new detail::AWSProvider_Impl()))
  {
  }

  unsigned AWSProvider::numWorkers() {
    return getImpl<detail::AWSProvider_Impl>()->numWorkers();
  }

  AWSProvider::AWSProvider(const boost::shared_ptr<detail::AWSProvider_Impl>& impl)
    : CloudProvider(impl)
  {
    OS_ASSERT(getImpl<detail::AWSProvider_Impl>());
  }

} // openstudio
