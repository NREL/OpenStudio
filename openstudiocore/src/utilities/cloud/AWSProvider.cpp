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
        m_userAgreementSigned(false),
        m_validAccessKey(false),
        m_validSecretKey(false)
    {
      loadSettings();
    }

    AWSSettings_Impl::AWSSettings_Impl(const UUID& uuid,
                                       const UUID& versionUUID,
                                       bool userAgreementSigned)
      : CloudSettings_Impl(uuid,versionUUID),
        m_validAccessKey(false),
        m_validSecretKey(false)
    {
      loadSettings();
      m_userAgreementSigned = userAgreementSigned;
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
      if (overwriteExisting) {
        if (settings.value("userAgreementSigned").toString() == "Yes") {
          m_userAgreementSigned = true;
        } else {
          m_userAgreementSigned = false;
        }
      }

      if (overwriteExisting || m_accessKey.empty()) {
        std::string accessKey = settings.value("accessKey").toString().toStdString();
        setAccessKey(accessKey);
      }

      if (overwriteExisting || m_secretKey.empty()) {
        QFile file(QDir::homePath() + "/.ssh/aws");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
          std::string secretKey = QString(file.readAll()).toStdString();
          setSecretKey(secretKey);
        }
      }
      return true;
    }

    bool AWSSettings_Impl::saveToSettings(bool overwriteExisting) const {
      QSettings settings("OpenStudio", toQString(cloudProviderType()));

      if (overwriteExisting || settings.value("accessKey").isNull()){
        settings.setValue("accessKey", toQString(m_accessKey));
      }
      
      QFile file(QDir::homePath() + "/.ssh/aws");
      if (overwriteExisting || !file.exists()) {
        if (QDir::home().exists(".ssh") || QDir::home().mkdir(".ssh")) {
          QFile file(QDir::homePath() + "/.ssh/aws");
          if (file.open(QIODevice::WriteOnly)) {
            file.write(m_secretKey.c_str());
            file.close();
          }
        } else {
          return false;
        }
      }
      return true;
    }

    std::string AWSSettings_Impl::accessKey() const {
      return m_accessKey;
    }

    bool AWSSettings_Impl::setAccessKey(const std::string& accessKey) {
      if (validAccessKey(accessKey)) {
        m_accessKey = accessKey;
        m_validAccessKey = true;
        return true;
      }
      return false;
    }
    
    std::string AWSSettings_Impl::secretKey() const {
      return m_secretKey;
    }

    bool AWSSettings_Impl::setSecretKey(const std::string& secretKey) {
      if (validSecretKey(secretKey)) {
        m_secretKey = secretKey;
        return true;
      }
      return false;
    }

    bool AWSSettings_Impl::validAccessKey(const std::string& accessKey) const {
      return QRegExp("[A-Z0-9]{20}").exactMatch(toQString(accessKey));
    }

    bool AWSSettings_Impl::validSecretKey(const std::string& secretKey) const {
      return QRegExp("[a-zA-Z0-9/+]{40}").exactMatch(toQString(secretKey));
    }

    bool AWSSettings_Impl::terminationDelayEnabled() {
      return m_terminationDelayEnabled;
    }

    void AWSSettings_Impl::setTerminationDelayEnabled(bool enabled) {
      m_terminationDelayEnabled = enabled;
    }

    unsigned AWSSettings_Impl::terminationDelay() {
      return m_terminationDelay;
    }

    void AWSSettings_Impl::setTerminationDelay(const unsigned delay) {
      m_terminationDelay = delay;
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

    Url AWSSession_Impl::serverUrl() const {
      return m_serverUrl;
    }

    void AWSSession_Impl::setServerUrl(const Url& serverUrl) {
      m_serverUrl = serverUrl;
      onChange();
    }

    std::vector<Url> AWSSession_Impl::workerUrls() const {
      return m_workerUrls;
    }

    void AWSSession_Impl::setWorkerUrls(const std::vector<Url>& workerUrls) {
      m_workerUrls = workerUrls;
      onChange();
    }

    std::string AWSSession_Impl::timestamp() const {
      return m_timestamp;
    }

    void AWSSession_Impl::setTimestamp(const std::string& timestamp) {
      m_timestamp = timestamp;
    }

    std::string AWSSession_Impl::region() const {
      return m_region;
    }

    void AWSSession_Impl::setRegion(const std::string& region) {
      m_region = region;
    }

    std::string AWSSession_Impl::serverInstanceType() const {
      return m_serverInstanceType;
    }

    void AWSSession_Impl::setServerInstanceType(const std::string& instanceType) {
      m_serverInstanceType = instanceType;
    }

    std::string AWSSession_Impl::workerInstanceType() const {
      return m_workerInstanceType;
    }

    void AWSSession_Impl::setWorkerInstanceType(const std::string& instanceType) {
      m_workerInstanceType = instanceType;
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

      m_regions.push_back("us-east-1");
      m_serverInstanceTypes.push_back("m1.medium");
      m_serverInstanceTypes.push_back("m1.large");
      m_serverInstanceTypes.push_back("m1.xlarge");
      m_workerInstanceTypes.push_back("c1.xlarge");

      // load credentials
    }

    std::string AWSProvider_Impl::type() const
    {
      return cloudProviderType();
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

    std::vector<std::string> AWSProvider_Impl::availableRegions() const {
      return m_regions;
    }

    std::string AWSProvider_Impl::region() const {
      return m_awsSession.region();
    }

    void AWSProvider_Impl::setRegion(const std::string& region) {
      m_awsSession.setRegion(region);
    }

    std::vector<std::string> AWSProvider_Impl::serverInstanceTypes() const {
      return m_serverInstanceTypes;
    }

    std::string AWSProvider_Impl::defaultServerInstanceType() const {
      return "m1.large";
    }

    std::string AWSProvider_Impl::serverInstanceType() const {
      return m_awsSession.serverInstanceType();
    }

    void AWSProvider_Impl::setServerInstanceType(const std::string& instanceType) {
      return m_awsSession.setServerInstanceType(instanceType);
    }

    std::vector<std::string> AWSProvider_Impl::workerInstanceTypes() const {
      return m_workerInstanceTypes;
    }

    std::string AWSProvider_Impl::defaultWorkerInstanceType() const {
      return "c1.xlarge";
    }

    std::string AWSProvider_Impl::workerInstanceType() const {
      return m_awsSession.workerInstanceType();
    }

    void AWSProvider_Impl::setWorkerInstanceType(const std::string& instanceType) {
      return m_awsSession.setWorkerInstanceType(instanceType);
    }

    bool AWSProvider_Impl::terminationDelayEnabled() {
      return m_awsSettings.terminationDelay();
    }

    void AWSProvider_Impl::setTerminationDelayEnabled(bool enabled) {
      m_awsSettings.terminationDelayEnabled();
    }

    unsigned AWSProvider_Impl::terminationDelay() {
      return m_awsSettings.terminationDelay();
    }

    void AWSProvider_Impl::setTerminationDelay(const unsigned delay) {
      m_awsSettings.setTerminationDelay(delay);
    }

    std::string AWSProvider_Impl::userAgreementText() const {
      return m_awsSettings.userAgreementText();
    }

    bool AWSProvider_Impl::userAgreementSigned() const {
      return m_awsSettings.userAgreementSigned();
    }

    void AWSProvider_Impl::signUserAgreement(bool agree) {
      m_awsSettings.signUserAgreement(agree);
    }

    std::string AWSProvider_Impl::accessKey() const {
      return m_awsSettings.accessKey();
    }

    bool AWSProvider_Impl::setAccessKey(const std::string& accessKey) {
      return m_awsSettings.setAccessKey(accessKey);
    }

    std::string AWSProvider_Impl::secretKey() const {
      return m_awsSettings.secretKey();
    }

    bool AWSProvider_Impl::setSecretKey(const std::string& secretKey) {
      return m_awsSettings.setSecretKey(secretKey);
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
                           bool userAgreementSigned)
    : CloudSettings(boost::shared_ptr<detail::AWSSettings_Impl>(
                      new detail::AWSSettings_Impl(uuid,
                                                   versionUUID,
                                                   userAgreementSigned)))
  {
    OS_ASSERT(getImpl<detail::AWSSettings_Impl>());
  }

  AWSSettings::AWSSettings(const boost::shared_ptr<detail::AWSSettings_Impl>& impl)
    : CloudSettings(impl)
  {
    OS_ASSERT(getImpl<detail::AWSSettings_Impl>());
  }

  std::string AWSSettings::accessKey() const {
    return getImpl<detail::AWSSettings_Impl>()->accessKey();
  }

  bool AWSSettings::setAccessKey(const std::string& accessKey) {
    return getImpl<detail::AWSSettings_Impl>()->setAccessKey(accessKey);
  }

  std::string AWSSettings::secretKey() const {
    return getImpl<detail::AWSSettings_Impl>()->secretKey();
  }

  bool AWSSettings::setSecretKey(const std::string& secretKey) {
    return getImpl<detail::AWSSettings_Impl>()->setSecretKey(secretKey);
  }

  bool AWSSettings::validAccessKey(const std::string& accessKey) const {
    return getImpl<detail::AWSSettings_Impl>()->validAccessKey(accessKey);
  }

  bool AWSSettings::validSecretKey(const std::string& secretKey) const {
    return getImpl<detail::AWSSettings_Impl>()->validSecretKey(secretKey);
  }

  bool AWSSettings::terminationDelayEnabled() {
    return getImpl<detail::AWSSettings_Impl>()->terminationDelayEnabled();
  }

   void AWSSettings::setTerminationDelayEnabled(bool enabled) {
    getImpl<detail::AWSSettings_Impl>()->setTerminationDelayEnabled(enabled);
  }

  unsigned AWSSettings::terminationDelay() {
    return getImpl<detail::AWSSettings_Impl>()->terminationDelay();
  }

  void AWSSettings::setTerminationDelay(const unsigned delay) {
    getImpl<detail::AWSSettings_Impl>()->setTerminationDelay(delay);
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

  AWSSession::AWSSession(const boost::shared_ptr<detail::AWSSession_Impl>& impl)
    : CloudSession(impl)
  {
    OS_ASSERT(getImpl<detail::AWSSession_Impl>());
  }

  Url AWSSession::serverUrl() const {
    return getImpl<detail::AWSSession_Impl>()->serverUrl(); 
  }

  void AWSSession::setServerUrl(const Url& serverUrl) {
    getImpl<detail::AWSSession_Impl>()->setServerUrl(serverUrl);
  }

  std::vector<Url> AWSSession::workerUrls() const {
    return getImpl<detail::AWSSession_Impl>()->workerUrls(); 
  }

  void AWSSession::setWorkerUrls(const std::vector<Url>& workerUrls) {
    getImpl<detail::AWSSession_Impl>()->setWorkerUrls(workerUrls);
  }

  std::string AWSSession::timestamp() const {
    return getImpl<detail::AWSSession_Impl>()->timestamp();
  }

  void AWSSession::setTimestamp(const std::string& timestamp) {
    getImpl<detail::AWSSession_Impl>()->setTimestamp(timestamp);
  }
  
  std::string AWSSession::region() const {
    return getImpl<detail::AWSSession_Impl>()->region();
  }

  void AWSSession::setRegion(const std::string& region) {
    getImpl<detail::AWSSession_Impl>()->setRegion(region);
  }

  std::string AWSSession::serverInstanceType() const {
    return getImpl<detail::AWSSession_Impl>()->serverInstanceType();
  }

  void AWSSession::setServerInstanceType(const std::string& instanceType) {
    getImpl<detail::AWSSession_Impl>()->setServerInstanceType(instanceType);
  }

  std::string AWSSession::workerInstanceType() const {
    return getImpl<detail::AWSSession_Impl>()->workerInstanceType();
  }

  void AWSSession::setWorkerInstanceType(const std::string& instanceType) {
    getImpl<detail::AWSSession_Impl>()->setWorkerInstanceType(instanceType);
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

  std::string AWSProvider::userAgreementText() const {
    return getImpl<detail::AWSProvider_Impl>()->userAgreementText();
  }

  bool AWSProvider::userAgreementSigned() const {
    return getImpl<detail::AWSProvider_Impl>()->userAgreementSigned();
  }

  void AWSProvider::signUserAgreement(bool agree) {
    getImpl<detail::AWSProvider_Impl>()->signUserAgreement(agree);
  }

  std::string AWSProvider::accessKey() const {
    return getImpl<detail::AWSProvider_Impl>()->accessKey();
  }

  bool AWSProvider::setAccessKey(const std::string& accessKey) {
    return getImpl<detail::AWSProvider_Impl>()->setAccessKey(accessKey);
  }

  std::string AWSProvider::secretKey() const {
    return getImpl<detail::AWSProvider_Impl>()->secretKey();
  }

  bool AWSProvider::setSecretKey(const std::string& secretKey) {
    return getImpl<detail::AWSProvider_Impl>()->setSecretKey(secretKey);
  }

  unsigned AWSProvider::numWorkers() const {
    return getImpl<detail::AWSProvider_Impl>()->numWorkers();
  }
  
  void AWSProvider::setNumWorkers(const unsigned numWorkers) {
    return getImpl<detail::AWSProvider_Impl>()->setNumWorkers(numWorkers);
  }

  std::vector<std::string> AWSProvider::availableRegions() const {
    return getImpl<detail::AWSProvider_Impl>()->availableRegions();
  }

  std::string AWSProvider::region() const {
    return getImpl<detail::AWSProvider_Impl>()->region();
  }

  void AWSProvider::setRegion(const std::string& region) {
    getImpl<detail::AWSProvider_Impl>()->setRegion(region);
  }

  std::vector<std::string> AWSProvider::serverInstanceTypes() const {
    return getImpl<detail::AWSProvider_Impl>()->serverInstanceTypes();
  }

  std::string AWSProvider::defaultServerInstanceType() const {
    return getImpl<detail::AWSProvider_Impl>()->defaultServerInstanceType();
  }

  std::string AWSProvider::serverInstanceType() const {
    return getImpl<detail::AWSProvider_Impl>()->serverInstanceType();
  }

  void AWSProvider::setServerInstanceType(const std::string& instanceType) {
    getImpl<detail::AWSProvider_Impl>()->setServerInstanceType(instanceType);
  }

  std::vector<std::string> AWSProvider::workerInstanceTypes() const {
    return getImpl<detail::AWSProvider_Impl>()->workerInstanceTypes();
  }

  std::string AWSProvider::defaultWorkerInstanceType() const {
    return getImpl<detail::AWSProvider_Impl>()->defaultWorkerInstanceType();
  }

  std::string AWSProvider::workerInstanceType() const {
    return getImpl<detail::AWSProvider_Impl>()->workerInstanceType();
  }

  void AWSProvider::setWorkerInstanceType(const std::string& instanceType) {
    getImpl<detail::AWSProvider_Impl>()->setWorkerInstanceType(instanceType);
  }

  bool AWSProvider::terminationDelayEnabled() {
    return getImpl<detail::AWSProvider_Impl>()->terminationDelayEnabled();
  }

  void AWSProvider::setTerminationDelayEnabled(bool enabled) {
    getImpl<detail::AWSProvider_Impl>()->setTerminationDelayEnabled(enabled);
  }

  unsigned AWSProvider::terminationDelay() {
    return getImpl<detail::AWSProvider_Impl>()->terminationDelay();
  }

  void AWSProvider::setTerminationDelay(const unsigned delay) {
    getImpl<detail::AWSProvider_Impl>()->setTerminationDelay(delay);
  }


} // openstudio
