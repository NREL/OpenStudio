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
      : CloudSettings_Impl(),
        m_validAccessKey(false),
        m_validSecretKey(false)
    {}

    AWSSettings_Impl::AWSSettings_Impl(const UUID& uuid,
                                       const UUID& versionUUID,
                                       bool userAgreementSigned,
                                       const Url& serverUrl,
                                       const std::vector<Url>& workerUrls,
                                       const std::string& accessKey)
      : CloudSettings_Impl(uuid,versionUUID),
        m_validAccessKey(false),
        m_validSecretKey(false)
    {
      //todo
    }

    std::string AWSSettings_Impl::cloudProviderType() const {
      return AWSProvider_Impl::cloudProviderType();
    }

    std::string AWSSettings_Impl::userAgreementText() const {
      return "OpenStudio is provided by the National Renewable Energy Laboratory (\"NREL\"), which is operated by Alliance for Sustainable Energy, LLC, (\"Alliance\") for the U.S. Department of Energy (\"DOE\").  NREL, Alliance and DOE are not responsible for any charges incurred as a result of using OpenStudio in connection with web services, cloud services or any other third party computing services.  You are solely responsible for establishing, maintaining, terminating and paying for any third party computing services and are solely responsible for complying with any terms and conditions required by third party service providers.  Terminating OpenStudio may not terminate third party computing services used in connection with OpenStudio, and you are solely responsible for verifying that computing services and associated charges are terminated.\n\nTHE SOFTWARE IS PROVIDED BY DOE/NREL/ALLIANCE \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY DISCLAIMED.  IN NO EVENT SHALL DOE/NREL/ALLIANCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER, INCLUDING BUT NOT LIMITED TO CLAIMS ASSOCIATED WITH THE LOSS OF DATA OR PROFITS, WHICH MAY RESULT FROM ANY ACTION IN CONTRACT, NEGLIGENCE OR OTHER TORTIOUS CLAIM THAT ARISES OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE SOFTWARE. YOU AGREE TO INDEMNIFY DOE/NREL/ALLIANCE, AND ITS AFFILIATES, OFFICERS, AGENTS, AND EMPLOYEES AGAINST ANY CLAIM OR DEMAND, INCLUDING REASONABLE ATTORNEYS' FEES, RELATED TO YOUR USE, RELIANCE, OR ADOPTION OF THE SOFTWARE FOR ANY PURPOSE WHATSOEVER.";
    }

    bool AWSSettings_Impl::userAgreementSigned() const {
      return m_userAgreementSigned;
    }

    void AWSSettings_Impl::signUserAgreement(bool agree) {
      m_userAgreementSigned = agree;
      onChange();
    }

    bool AWSSettings_Impl::loadSettings(bool overwriteExisting) {
      QSettings settings("OpenStudio", toQString(cloudProviderType()));

      // only set this if overwriteExisting is true
      if (overwriteExisting){
        if (settings.value("userAgreementSigned").toString() == "Yes"){
          m_userAgreementSigned = true;
        }else{
          m_userAgreementSigned = false;
        }
      }

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

    bool AWSSettings_Impl::saveToSettings(bool overwriteExisting) const {
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

    Url AWSSettings_Impl::serverUrl() const {
      //todo
      return Url();
    }

    void AWSSettings_Impl::setServerUrl(const Url& serverUrl) {
      m_serverUrl = serverUrl;
    }

    std::vector<Url> AWSSettings_Impl::workerUrls() const {
      //todo
      return std::vector<Url>();
    }

    void AWSSettings_Impl::setWorkerUrls(const std::vector<Url>& workerUrls) {
      m_workerUrls = workerUrls;
    }

    std::string AWSSettings_Impl::accessKey() const {
      return m_accessKey;
    }

    bool AWSSettings_Impl::setAccessKey(std::string accessKey) {
      if (validAccessKey(accessKey)) {
        m_accessKey = accessKey;
        return true;
      }
      return false;
    }
    
    std::string AWSSettings_Impl::secretKey() const {
      return m_secretKey;
    }

    bool AWSSettings_Impl::setSecretKey(std::string secretKey) {
      if (validSecretKey(secretKey)) {
        m_secretKey = secretKey;
        return true;
      }
      return false;
    }

    bool AWSSettings_Impl::validAccessKey(std::string accessKey) const {
      return QRegExp("[A-Z0-9]{20}").exactMatch(toQString(accessKey));
    }

    bool AWSSettings_Impl::validSecretKey(std::string secretKey) const {
      return QRegExp("[a-zA-Z0-9/+]{40}").exactMatch(toQString(secretKey));
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

    std::string AWSSession_Impl::cloudProviderType() const {
      return AWSProvider_Impl::cloudProviderType();
    }

    AWSProvider_Impl::AWSProvider_Impl()
      : CloudProvider_Impl(),
        m_awsSettings(),
        m_awsSession(toString(createUUID()),boost::none,std::vector<Url>()),
        m_numWorkers(0),
        m_startServerProcess(NULL), m_startWorkersProcess(NULL),
        m_serverStarted(false), m_workersStarted(false), m_serverStopped(false), m_workersStopped(false), m_terminateStarted(false)
    {
      //Make sure a QApplication exists
      openstudio::Application::instance().application();

      // load credentials
    }

    std::string AWSProvider_Impl::type() const
    {
      return "AWSProvider";
    }

    unsigned AWSProvider_Impl::numWorkers() const
    {
      return m_numWorkers;
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

    bool AWSProvider_Impl::setSession(const CloudSession& session)
    {
      // todo
      return false;
    }

    bool AWSProvider_Impl::lastInternetAvailable() const
    {
      return m_lastInternetAvailable;
    }

    bool AWSProvider_Impl::lastServiceAvailable() const
    {
      return m_lastServiceAvailable;
    }

    bool AWSProvider_Impl::lastValidateCredentials() const
    {
      return m_lastValidateCredentials;
    }

    bool AWSProvider_Impl::serverStarted() const
    {
      return m_serverStarted;
    }

    bool AWSProvider_Impl::workersStarted() const
    {
      return m_workersStarted;
    }

    bool AWSProvider_Impl::running() const
    {
      // todo
      return false;
    }

    bool AWSProvider_Impl::terminateStarted() const
    {
      return m_terminateStarted;
    }

    bool AWSProvider_Impl::terminateCompleted() const
    {
      return (m_serverStopped && m_workersStopped);
    }

    std::vector<std::string> AWSProvider_Impl::errors() const
    {
      return m_errors;
    }

    std::vector<std::string> AWSProvider_Impl::warnings() const
    {
      return m_warnings;
    }

    bool AWSProvider_Impl::internetAvailable(int msec)
    {
      // todo: make use non-blocking requestInternetAvailable
      return serviceAvailable(msec);
    }

    bool AWSProvider_Impl::serviceAvailable(int msec)
    {
      // todo: make use non-blocking requestServiceAvailable
      QVariantMap map = awsRequest("describe_availability_zones");
      if (!map.keys().contains("error")) {
        return true;
      }
      int code = map["error"].toMap()["code"].toInt();
      return (code != 503);
    }

    bool AWSProvider_Impl::validateCredentials(int msec)
    {
      // todo: make use non-blocking requestValidateCredentials
      //if (m_validAccessKey && m_validSecretKey) {
      //  return true;
      //}

      QVariantMap map = awsRequest("describe_availability_zones");
      if (!map.keys().contains("error")) {
        //m_validAccessKey = true;
        //m_validSecretKey = true;
        return true;
      }

      int code = map["error"].toMap()["code"].toInt();
      if (code == 401) {
        //m_validAccessKey = false;
      } else if (code == 403) {
        //m_validAccessKey = true;
        //m_validSecretKey = false;
      }
      return false;
    }

    bool AWSProvider_Impl::waitForServer(int msec)
    {
      // todo
      return false;
    }

    bool AWSProvider_Impl::waitForWorkers(int msec)
    {
      // todo
      return false;
    }

    bool AWSProvider_Impl::waitForTerminated(int msec)
    {
      // todo
      return false;
    }

    bool AWSProvider_Impl::requestInternetAvailable()
    {
      // todo
      return false;
    }

    bool AWSProvider_Impl::requestServiceAvailable()
    {
      // todo
      return false;
    }

    bool AWSProvider_Impl::requestValidateCredentials()
    {
      clearErrorsAndWarnings();

      m_lastValidateCredentials = false;

      if ((m_awsSettings.accessKey() == "vagrant") &&
          (m_awsSettings.secretKey() == "vagrant")){
        m_lastValidateCredentials = true;
      }

      return true;
    }

    bool AWSProvider_Impl::requestStartServer()
    {
      // todo: make non-blocking
      //QVariantMap map = awsRequest("launch_instance");
      //return !map.keys().contains("error");
      return false;
    }

    bool AWSProvider_Impl::requestStartWorkers()
    {
      // todo: make non-blocking
      //QVariantMap map = awsRequest("launch_slaves");
      //return !map.keys().contains("error");
      return false;
    }

    bool AWSProvider_Impl::requestTerminate()
    {
      // todo
      return false;
    }


/*  TODO: move this to AWSSettings

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
*/

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

    QVariantMap AWSProvider_Impl::awsRequest(std::string request, std::string service) const {
      QString script = toQString(getOpenStudioRubyScriptsPath() / toPath("cloud/aws.rb"));
      QProcess *ruby2 = new QProcess();
      ruby2->start("\"C:\\Ruby200\\bin\\ruby.exe\"", QStringList() << script << toQString(m_awsSettings.accessKey()) << toQString(m_awsSettings.secretKey()) << toQString(service) << toQString(request));
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

    void AWSProvider_Impl::onWorkersStarted(int, QProcess::ExitStatus)
    {
    }

    void AWSProvider_Impl::onServerStopped(int, QProcess::ExitStatus)
    {
    }

    void AWSProvider_Impl::onWorkersStopped(int, QProcess::ExitStatus)
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
                           const UUID& versionUUID,
                           bool userAgreementSigned,
                           const Url& serverUrl,
                           const std::vector<Url>& workerUrls,
                           const std::string& accessKey)
    : CloudSettings(boost::shared_ptr<detail::AWSSettings_Impl>(
                      new detail::AWSSettings_Impl(uuid,
                                                   versionUUID,
                                                   userAgreementSigned,
                                                   serverUrl,
                                                   workerUrls,
                                                   accessKey)))
  {
    OS_ASSERT(getImpl<detail::AWSSettings_Impl>());
  }

  AWSSettings::AWSSettings(const boost::shared_ptr<detail::AWSSettings_Impl>& impl)
    : CloudSettings(impl)
  {
    OS_ASSERT(getImpl<detail::AWSSettings_Impl>());
  }

  Url AWSSettings::serverUrl() const {
    return getImpl<detail::AWSSettings_Impl>()->serverUrl(); 
  }

  void AWSSettings::setServerUrl(const Url& serverUrl) {
    getImpl<detail::AWSSettings_Impl>()->setServerUrl(serverUrl);
  }

  std::vector<Url> AWSSettings::workerUrls() const {
    return getImpl<detail::AWSSettings_Impl>()->workerUrls(); 
  }

  void AWSSettings::setWorkerUrls(const std::vector<Url>& workerUrls) {
    getImpl<detail::AWSSettings_Impl>()->setWorkerUrls(workerUrls);
  }

  std::string AWSSettings::accessKey() const {
    return getImpl<detail::AWSSettings_Impl>()->accessKey();
  }

  bool AWSSettings::setAccessKey(std::string accessKey) {
    return getImpl<detail::AWSSettings_Impl>()->setAccessKey(accessKey);
  }

  std::string AWSSettings::secretKey() const {
    return getImpl<detail::AWSSettings_Impl>()->secretKey();
  }

  bool AWSSettings::setSecretKey(std::string secretKey) {
    return getImpl<detail::AWSSettings_Impl>()->setSecretKey(secretKey);
  }

  bool AWSSettings::validAccessKey(std::string accessKey) const {
    return getImpl<detail::AWSSettings_Impl>()->validAccessKey(accessKey);
  }

  bool AWSSettings::validSecretKey(std::string secretKey) const {
    return getImpl<detail::AWSSettings_Impl>()->validSecretKey(secretKey);
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

  AWSProvider::AWSProvider(const boost::shared_ptr<detail::AWSProvider_Impl>& impl)
    : CloudProvider(impl)
  {
    OS_ASSERT(getImpl<detail::AWSProvider_Impl>());
  }

} // openstudio
