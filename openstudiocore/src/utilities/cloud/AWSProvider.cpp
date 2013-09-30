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
#include <utilities/core/System.hpp>

#include <qjson/parser.h>
#include <qjson/serializer.h>

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QTemporaryFile>
#include <QTextStream>
#include <QUrl>

#include <boost/bind.hpp>

#include <algorithm>

namespace openstudio{
  namespace detail{

    AWSSettings_Impl::AWSSettings_Impl()
      : CloudSettings_Impl(),
        m_userAgreementSigned(false),
        m_validAccessKey(false),
        m_validSecretKey(false),
        m_numWorkers(2),
        m_terminationDelayEnabled(false),
        m_terminationDelay(0)
    {
      loadSettings(true);
    }

    AWSSettings_Impl::AWSSettings_Impl(const UUID& uuid,
                                       const UUID& versionUUID,
                                       bool userAgreementSigned,
                                       unsigned numWorkers,
                                       bool terminationDelayEnabled,
                                       unsigned terminationDelay)
      : CloudSettings_Impl(uuid,versionUUID),
        m_validAccessKey(false),
        m_validSecretKey(false)
    {
      loadSettings(true);
      m_userAgreementSigned = userAgreementSigned;
      setNumWorkers(numWorkers);
      setTerminationDelayEnabled(terminationDelayEnabled);
      setTerminationDelay(terminationDelay);
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
      if (m_userAgreementSigned != agree) {
        m_userAgreementSigned = agree;
        onChange();
      }
    }

    bool AWSSettings_Impl::loadSettings(bool overwriteExisting) {
      QSettings settings("OpenStudio", toQString(cloudProviderType()));

      // only set this if overwriteExisting is true
      if (overwriteExisting) {
        m_userAgreementSigned = (settings.value("userAgreementSigned").toString() == "Yes");
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

      if (overwriteExisting && !settings.value("numWorkers").isNull()) {
        setNumWorkers(settings.value("numWorkers").toUInt());
      }

      if (overwriteExisting && !settings.value("terminationDelayEnabled").isNull()) {
        setTerminationDelayEnabled(settings.value("terminationDelayEnabled").toBool());
      }

      if (overwriteExisting && !settings.value("terminationDelay").isNull()) {
        setTerminationDelay(settings.value("terminationDelay").toUInt());
      }

      onChange();

      return !(m_accessKey.empty() || m_secretKey.empty());
    }

    bool AWSSettings_Impl::saveToSettings(bool overwriteExisting) const {
      QSettings settings("OpenStudio", toQString(cloudProviderType()));

      if (overwriteExisting || settings.value("userAgreementSigned").isNull()){
        settings.setValue("userAgreementSigned", m_userAgreementSigned ? "Yes" : "No");
      }

      if (overwriteExisting || settings.value("accessKey").isNull()){
        settings.setValue("accessKey", toQString(m_accessKey));
      }
      
      QFile file(QDir::homePath() + "/.ssh/aws");
      if (overwriteExisting || !file.exists()) {
        if (QDir::home().exists(".ssh") || QDir::home().mkdir(".ssh")) {
          if (file.open(QIODevice::WriteOnly)) {
            file.write(m_secretKey.c_str());
          }
        }
      }
      file.close();

      if (overwriteExisting || settings.value("numWorkers").isNull()) {
        settings.setValue("numWorkers", m_numWorkers);
      }

      if (overwriteExisting || settings.value("terminationDelayEnabled").isNull()) {
        settings.setValue("terminationDelayEnabled", m_terminationDelayEnabled);
      }

      if (overwriteExisting || settings.value("terminationDelay").isNull()) {
        settings.setValue("terminationDelay", m_terminationDelay);
      }

      return true;
    }

    std::string AWSSettings_Impl::accessKey() const {
      return m_accessKey;
    }

    bool AWSSettings_Impl::setAccessKey(const std::string& accessKey) {
      if (validAccessKey(accessKey)) {
        if (m_accessKey != accessKey) {
          m_accessKey = accessKey;
          m_validAccessKey = true;
          onChange();
        }
        return true;
      }
      return false;
    }
    
    std::string AWSSettings_Impl::secretKey() const {
      return m_secretKey;
    }

    bool AWSSettings_Impl::setSecretKey(const std::string& secretKey) {
      if (validSecretKey(secretKey)) {
        if (m_secretKey != secretKey) {
          m_secretKey = secretKey;
          m_validSecretKey = true;
          onChange();
        }
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

    unsigned AWSSettings_Impl::numWorkers() const {
      return m_numWorkers;
    }

    unsigned AWSSettings_Impl::setNumWorkers(const unsigned numWorkers) {
      if (numWorkers > 0 && numWorkers < 20 && m_numWorkers != numWorkers) {
        m_numWorkers = numWorkers;
        onChange();
      }
      return m_numWorkers;
    }

    bool AWSSettings_Impl::terminationDelayEnabled() const {
      return m_terminationDelayEnabled;
    }

    void AWSSettings_Impl::setTerminationDelayEnabled(bool enabled) {
      if (m_terminationDelayEnabled != enabled) {
        m_terminationDelayEnabled = enabled;
        onChange();
      }
    }

    unsigned AWSSettings_Impl::terminationDelay() const {
      return m_terminationDelay;
    }

    void AWSSettings_Impl::setTerminationDelay(const unsigned delay) {
      if (m_terminationDelay != delay) {
        m_terminationDelay = delay;
        onChange();
      }
    }


    AWSSession_Impl::AWSSession_Impl(const std::string& sessionId,
                                     const boost::optional<Url>& serverUrl,
                                     const std::vector<Url>& workerUrls)
      : CloudSession_Impl(sessionId,serverUrl,workerUrls),
        m_numServerProcessors(0),
        m_numWorkerProcessors(0)
    {}

    AWSSession_Impl::AWSSession_Impl(const UUID& uuid,
                                     const UUID& versionUUID,
                                     const std::string& sessionId,
                                     const boost::optional<Url>& serverUrl,
                                     const std::vector<Url>& workerUrls)
      : CloudSession_Impl(uuid,versionUUID,sessionId,serverUrl,workerUrls),
        m_numServerProcessors(0),
        m_numWorkerProcessors(0)
    {}

    AWSSession_Impl::AWSSession_Impl(const UUID& uuid,
                                     const UUID& versionUUID,
                                     const std::string& sessionId,
                                     const boost::optional<Url>& serverUrl,
                                     const std::string& serverId,
                                     const unsigned numServerProcessors,
                                     const std::vector<Url>& workerUrls,
                                     const std::vector<std::string>& workerIds,
                                     const unsigned numWorkerProcessors,
                                     const std::string& privateKey,
                                     const std::string& timestamp,
                                     const std::string& region,
                                     const std::string& serverInstanceType,
                                     const std::string& workerInstanceType)
      : CloudSession_Impl(uuid,versionUUID,sessionId,serverUrl,workerUrls),
        m_serverId(serverId),
        m_numServerProcessors(numServerProcessors),
        m_workerIds(workerIds),
        m_numWorkerProcessors(numWorkerProcessors),
        m_privateKey(privateKey),
        m_timestamp(timestamp),
        m_region(region),
        m_serverInstanceType(serverInstanceType),
        m_workerInstanceType(workerInstanceType)
    {}

    std::string AWSSession_Impl::cloudProviderType() const {
      return AWSProvider_Impl::cloudProviderType();
    }

//    Url AWSSession_Impl::serverUrl() const {
//      return m_serverUrl;
//    }

//    void AWSSession_Impl::setServerUrl(const Url& serverUrl) {
//      if (m_serverUrl != serverUrl) {
//        m_serverUrl = serverUrl;
//        onChange();
//      }
//   }

    std::string AWSSession_Impl::serverId() const {
      return m_serverId;
    }

    void AWSSession_Impl::setServerId(const std::string& serverId) {
      if (m_serverId != serverId) {
        m_serverId = serverId;
        onChange();
      }
    }

    unsigned AWSSession_Impl::numServerProcessors() const {
      return m_numServerProcessors;
    }

    void AWSSession_Impl::setNumServerProcessors(const unsigned numServerProcessors) {
      if (m_numServerProcessors != numServerProcessors) {
        m_numServerProcessors = numServerProcessors;
        onChange();
      }
    }

//    std::vector<Url> AWSSession_Impl::workerUrls() const {
//      return m_workerUrls;
//    }

    void AWSSession_Impl::setWorkerUrls(const std::vector<Url>& workerUrls) {
      if (m_workerUrls != workerUrls) {
        m_workerUrls = workerUrls;
        onChange();
      }
    }

    std::vector<std::string> AWSSession_Impl::workerIds() const {
      return m_workerIds;
    }

    void AWSSession_Impl::setWorkerIds(const std::vector<std::string>& workerIds) {
      if (m_workerIds != workerIds) {
        m_workerIds = workerIds;
        onChange();
      }
    }

    unsigned AWSSession_Impl::numWorkerProcessors() const {
      return m_numWorkerProcessors;
    }

    void AWSSession_Impl::setNumWorkerProcessors(const unsigned numWorkerProcessors) {
      if (m_numWorkerProcessors != numWorkerProcessors) {
        m_numWorkerProcessors = numWorkerProcessors;
        onChange();
      }
    }
    
    std::string AWSSession_Impl::privateKey() const {
      return m_privateKey;
    }

    void AWSSession_Impl::setPrivateKey(const std::string& privateKey) {
      if (m_privateKey != privateKey) {
        m_privateKey = privateKey;
        onChange();
      }
    }

    std::string AWSSession_Impl::timestamp() const {
      return m_timestamp;
    }

    void AWSSession_Impl::setTimestamp(const std::string& timestamp) {
      if (m_timestamp != timestamp) {
        m_timestamp = timestamp;
        onChange();
      }
    }

    std::string AWSSession_Impl::region() const {
      return m_region;
    }

    void AWSSession_Impl::setRegion(const std::string& region) {
      if (m_region != region) {
        m_region = region;
        onChange();
      }
    }

    std::string AWSSession_Impl::serverInstanceType() const {
      return m_serverInstanceType;
    }

    void AWSSession_Impl::setServerInstanceType(const std::string& instanceType) {
      if (m_serverInstanceType != instanceType) {
        m_serverInstanceType = instanceType;
        onChange();
      }
    }

    std::string AWSSession_Impl::workerInstanceType() const {
      return m_workerInstanceType;
    }

    void AWSSession_Impl::setWorkerInstanceType(const std::string& instanceType) {
      if (m_workerInstanceType != instanceType) {
        m_workerInstanceType = instanceType;
        onChange();
      }
    }

    unsigned AWSSession_Impl::totalSessionUptime() const {
      //todo
      return 0;
    }

    unsigned AWSSession_Impl::totalSessionInstances() const {
      //todo
      return 0;
    }


    AWSProvider_Impl::AWSProvider_Impl()
      : CloudProvider_Impl(),
        m_awsSettings(),
        m_awsSession(toString(createUUID()),boost::none,std::vector<Url>()),
        m_script(getOpenStudioRubyScriptsPath() / toPath("cloud/aws.rb")),
        m_checkInternetProcess(0),
        m_checkServiceProcess(0),
        m_checkValidateProcess(0),
        m_checkResourcesProcess(0),
        m_startServerProcess(0),
        m_startWorkerProcess(0),
        m_checkServerRunningProcess(0),
        m_checkWorkerRunningProcess(0),
        m_stopServerProcess(0),
        m_stopWorkerProcess(0),
        m_checkTerminatedProcess(0),
        m_lastInternetAvailable(false),
        m_lastServiceAvailable(false),
        m_lastValidateCredentials(false),
        m_lastResourcesAvailableToStart(false),
        m_serverStarted(false),
        m_workerStarted(false),
        m_lastServerRunning(false),
        m_lastWorkerRunning(false),
        m_serverStopped(false),
        m_workerStopped(false),
        m_terminateStarted(false),
        m_lastTerminateCompleted(false)
    {
      //Make sure a QApplication exists
      openstudio::Application::instance().application();

      m_ruby = getOpenStudioAWSRubyPath();
      
#if defined(Q_OS_WIN32)
      m_ruby /= toPath("bin/ruby.exe");
#else
      m_ruby /= toPath("bin/ruby");
#endif
      if (!boost::filesystem::exists(m_ruby)) {
        LOG_AND_THROW("Ruby 2.0 executable cannot be found.");
      }

      m_regions.push_back("us-east-1");
      m_awsSession.setRegion(defaultRegion());

      m_serverInstanceTypes.push_back("t1.micro");
      m_serverInstanceTypes.push_back("m1.medium");
      m_serverInstanceTypes.push_back("m1.large");
      m_serverInstanceTypes.push_back("m1.xlarge");

      m_workerInstanceTypes.push_back("t1.micro");
      m_workerInstanceTypes.push_back("c1.xlarge");

      m_privateKey.setAutoRemove(false);
    }

    std::string AWSProvider_Impl::type() const
    {
      return AWSProvider_Impl::cloudProviderType();
    }

    unsigned AWSProvider_Impl::numWorkers() const
    {
      return m_awsSettings.numWorkers();
    }

    CloudSettings AWSProvider_Impl::settings() const {
      return m_awsSettings;
    }

    bool AWSProvider_Impl::setSettings(const CloudSettings& settings) {
      clearErrorsAndWarnings();

      boost::optional<AWSSettings> awsSettings = settings.optionalCast<AWSSettings>();
      if (!awsSettings){
        // wrong type of settings
        return false;
      }
      if (m_serverStarted || m_startServerProcess){
        // can't change settings once server has been started or is starting
        return false;
      }
      if (m_workerStarted || m_startWorkerProcess){
        // can't change settings once workers have been started or are starting
        return false;
      }
      if (m_terminateStarted){
        // can't change settings once terminate is called
        return false;
      }

      m_awsSettings = *awsSettings;

      return true;
    }

    CloudSession AWSProvider_Impl::session() const
    {
      return m_awsSession;
    }

    bool AWSProvider_Impl::setSession(const CloudSession& session)
    {
      clearErrorsAndWarnings();

      boost::optional<AWSSession> awsSession = session.optionalCast<AWSSession>();
      if (!awsSession){
        // wrong type of session
        return false;
      }
      if (!awsSession->serverUrl()){
        // session to set should be a non-empty one
        return false;
      }
      if (awsSession->workerUrls().size() == 0){
        // session to set should be a non-empty one
        return false;
      }
      if (m_serverStarted || m_startServerProcess){
        // can't change session once server has been started or is starting
        return false;
      }
      if (m_workerStarted || m_startWorkerProcess){
        // can't change session once workers have been started or are starting
        return false;
      }
      if (m_terminateStarted){
        // can't change session once terminate has been called
        return false;
      }

      m_awsSession = *awsSession;

      // assumes that the other session is already started
      m_serverStarted = true;
      m_workerStarted = true;

      // should we emit a signal here?

      return true;
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

    bool AWSProvider_Impl::lastResourcesAvailableToStart() const
    {
      return m_lastResourcesAvailableToStart;
    }

    bool AWSProvider_Impl::serverStarted() const
    {
      return m_serverStarted;
    }

    bool AWSProvider_Impl::workersStarted() const
    {
      return m_workerStarted;
    }

    bool AWSProvider_Impl::lastServerRunning() const
    {
      return m_lastServerRunning;
    }

    bool AWSProvider_Impl::lastWorkersRunning() const
    {
      return m_lastWorkerRunning;
    }

    bool AWSProvider_Impl::terminateStarted() const
    {
      return m_terminateStarted;
    }

    bool AWSProvider_Impl::lastTerminateCompleted() const
    {
      return (m_serverStopped && m_workerStopped);
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
      if (requestInternetAvailable()){
        if (waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestInternetAvailableFinished, this))){
          return lastInternetAvailable();
        }
      }
      return false;
    }

    bool AWSProvider_Impl::serviceAvailable(int msec)
    {
      if (requestServiceAvailable()){
        if (waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestServiceAvailableFinished, this))){
          return lastServiceAvailable();
        }
      }
      return false;
    }

    bool AWSProvider_Impl::validateCredentials(int msec)
    {
      if (requestValidateCredentials()){
        if (waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestValidateCredentialsFinished, this))){
          return lastValidateCredentials();
        }
      }
      return false;
    }

    bool AWSProvider_Impl::resourcesAvailableToStart(int msec)
    {
      if (requestResourcesAvailableToStart()){
        if (waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestResourcesAvailableToStartFinished, this))){
          return lastResourcesAvailableToStart();
        }
      }
      return false;
    }

    bool AWSProvider_Impl::waitForServer(int msec)
    {
      return waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestServerStartedFinished, this));
    }

    bool AWSProvider_Impl::waitForWorkers(int msec)
    {
      return waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestWorkerStartedFinished, this));
    }

    bool AWSProvider_Impl::serverRunning(int msec)
    {
      if (requestServerRunning()){
        if (waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestServerRunningFinished, this))){
          return lastServerRunning();
        }
      }
      return false;
    }

    bool AWSProvider_Impl::workersRunning(int msec)
    {
      if (requestWorkersRunning()){
        if (waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestWorkersRunningFinished, this))){
          return lastWorkersRunning();
        }
      }
      return false;
    }

    bool AWSProvider_Impl::waitForTerminated(int msec)
    {
      return waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestTerminateFinished, this));
    }

    bool AWSProvider_Impl::terminateCompleted(int msec)
    {
      if (requestTerminateCompleted()){
        if (waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestTerminateCompletedFinished, this))){
          return lastTerminateCompleted();
        }
      }
      return false;
    }

    bool AWSProvider_Impl::requestInternetAvailable()
    {
      if (m_checkInternetProcess){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_lastInternetAvailable = false;

      m_checkInternetProcess = makeCheckInternetProcess();

      return true;
    }

    bool AWSProvider_Impl::requestServiceAvailable()
    {
      if (m_checkServiceProcess){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_lastServiceAvailable = false;

      m_checkServiceProcess = makeCheckServiceProcess();

      return true;
    }

    bool AWSProvider_Impl::requestValidateCredentials()
    {
      if (m_checkValidateProcess){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_lastValidateCredentials = false;

      m_checkValidateProcess = makeCheckValidateProcess();

      return true;
    }

    bool AWSProvider_Impl::requestResourcesAvailableToStart()
    {
      if (m_checkResourcesProcess){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_lastResourcesAvailableToStart = false;

      m_checkResourcesProcess = makeCheckResourcesProcess();

      return true;
    }

    bool AWSProvider_Impl::requestStartServer()
    {
      if (m_startServerProcess){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_serverStarted = false;

      m_startServerProcess = makeStartServerProcess();

      return true;
    }

    bool AWSProvider_Impl::requestStartWorkers()
    {
      if (m_startWorkerProcess){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_workerStarted = false;

      m_startWorkerProcess = makeStartWorkerProcess();

      return true;
    }

    bool AWSProvider_Impl::requestServerRunning()
    {
      if (m_checkServerRunningProcess){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_lastServerRunning = false;

      m_checkServerRunningProcess = makeCheckServerRunningProcess();

      return true;
    }

    bool AWSProvider_Impl::requestWorkersRunning()
    {
      if (m_checkWorkerRunningProcess){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_lastWorkerRunning = false;

      m_checkWorkerRunningProcess = makeCheckWorkerRunningProcess();

      return true;
    }

    bool AWSProvider_Impl::requestTerminate()
    {
      // todo
      return false;
    }

    bool AWSProvider_Impl::requestTerminateCompleted()
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

    void AWSProvider_Impl::addProcessArguments(QStringList& args) const
    {
      args << toQString(m_script);
      args << toQString(m_awsSettings.accessKey());
      args << toQString(m_awsSettings.secretKey());
      args << toQString(m_awsSession.region());
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

    unsigned AWSProvider_Impl::setNumWorkers(const unsigned numWorkers)
    {
      return m_awsSettings.setNumWorkers(numWorkers);
    }

    std::vector<std::string> AWSProvider_Impl::availableRegions() const {
      return m_regions;
    }

    std::string AWSProvider_Impl::defaultRegion() const {
      return "us-east-1";
    }

    std::string AWSProvider_Impl::region() const {
      return m_awsSession.region();
    }

    void AWSProvider_Impl::setRegion(const std::string& region) {
      if (std::find(m_regions.begin(), m_regions.end(), region) != m_regions.end())
      {
        m_awsSession.setRegion(region);
      }
    }

    std::vector<std::string> AWSProvider_Impl::serverInstanceTypes() const {
      return m_serverInstanceTypes;
    }

    std::string AWSProvider_Impl::defaultServerInstanceType() const {
      return "m1.medium";
    }

    std::string AWSProvider_Impl::serverInstanceType() const {
      if (m_awsSession.serverInstanceType().empty()) return defaultServerInstanceType();
      return m_awsSession.serverInstanceType();
    }

    void AWSProvider_Impl::setServerInstanceType(const std::string& instanceType) {
      if (std::find(m_serverInstanceTypes.begin(), m_serverInstanceTypes.end(), instanceType) != m_serverInstanceTypes.end())
      {
        m_awsSession.setServerInstanceType(instanceType);
      }
    }

    std::vector<std::string> AWSProvider_Impl::workerInstanceTypes() const {
      return m_workerInstanceTypes;
    }

    std::string AWSProvider_Impl::defaultWorkerInstanceType() const {
      return "c1.xlarge";
    }

    std::string AWSProvider_Impl::workerInstanceType() const {
      if (m_awsSession.workerInstanceType().empty()) return defaultWorkerInstanceType();
      return m_awsSession.workerInstanceType();
    }

    void AWSProvider_Impl::setWorkerInstanceType(const std::string& instanceType) {
      if (std::find(m_workerInstanceTypes.begin(), m_workerInstanceTypes.end(), instanceType) != m_workerInstanceTypes.end())
      {
        m_awsSession.setWorkerInstanceType(instanceType);
      }
    }

    bool AWSProvider_Impl::terminationDelayEnabled() const {
      return m_awsSettings.terminationDelay();
    }

    void AWSProvider_Impl::setTerminationDelayEnabled(bool enabled) {
      m_awsSettings.terminationDelayEnabled();
    }

    unsigned AWSProvider_Impl::terminationDelay() const {
      return m_awsSettings.terminationDelay();
    }

    void AWSProvider_Impl::setTerminationDelay(const unsigned delay) {
      m_awsSettings.setTerminationDelay(delay);
    }

    unsigned AWSProvider_Impl::numSessionWorkers() const {
      return m_awsSession.numWorkers();
    }

    double AWSProvider_Impl::estimatedCharges() const {
      //todo
      return 0.0;
    }

    unsigned AWSProvider_Impl::totalSessionUptime() const {
      return m_awsSession.totalSessionUptime();
    }

    unsigned AWSProvider_Impl::totalSessionInstances() const {
      return m_awsSession.totalSessionInstances();
    }

    unsigned AWSProvider_Impl::totalInstances() const {
      //todo
      return 0;
    }

    bool AWSProvider_Impl::waitForFinished(int msec, const boost::function<bool ()>& f) {
      int msecPerLoop = 20;
      int numTries = msec / msecPerLoop;
      int current = 0;
      while (true)
      {
        if (f()){
          return true;
        }

        // this calls process events
        System::msleep(msecPerLoop);

        if (current > numTries){
          m_errors.push_back("Wait for finished timed out");
          break;
        }

        ++current;
      }

      return false;
    }

    bool AWSProvider_Impl::requestInternetAvailableFinished() const
    {
      return (m_checkInternetProcess == 0);
    }

    bool AWSProvider_Impl::requestServiceAvailableFinished() const
    {
      return (m_checkServiceProcess == 0);
    }

    bool AWSProvider_Impl::requestValidateCredentialsFinished() const
    {
      return (m_checkValidateProcess == 0);
    }

    bool AWSProvider_Impl::requestServerStartedFinished() const
    {
      return (m_startServerProcess == 0);
    }

    bool AWSProvider_Impl::requestWorkerStartedFinished() const
    {
      return (m_startWorkerProcess == 0);
    }

    bool AWSProvider_Impl::requestResourcesAvailableToStartFinished() const
    {
      return (m_checkResourcesProcess == 0);
    }

    bool AWSProvider_Impl::requestServerRunningFinished() const
    {
      return (m_checkServerRunningProcess == 0);
    }

    bool AWSProvider_Impl::requestWorkersRunningFinished() const
    {
      return (m_checkWorkerRunningProcess == 0);
    }

    bool AWSProvider_Impl::requestTerminateFinished() const
    {
      return (m_stopServerProcess == 0) && (m_stopWorkerProcess == 0);
    }

    bool AWSProvider_Impl::requestTerminateCompletedFinished() const
    {
      return (m_checkTerminatedProcess == 0);
    }

    ProcessResults AWSProvider_Impl::handleProcessCompleted(QProcess * t_qp)
    {
      OS_ASSERT(t_qp);

      ProcessResults pr(t_qp->exitCode(), t_qp->exitStatus(), t_qp->readAllStandardOutput(),
          t_qp->readAllStandardError());

      t_qp->deleteLater();

      return pr;
    }

    QProcess *AWSProvider_Impl::makeCheckInternetProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckInternetComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("describe_availability_zones");
      
      p->start(toQString(m_ruby), args);

      return p;
    }
    
    QProcess *AWSProvider_Impl::makeCheckServiceProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckServiceComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("describe_availability_zones");
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckValidateProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckValidateComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("describe_availability_zones");
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckResourcesProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckResourcesComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("total_instances");
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeStartServerProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onServerStarted(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      test = connect(p, SIGNAL(error(QProcess::ProcessError)), 
                     this, SLOT(onServerStartedError(QProcess::ProcessError)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("launch_server");
      
      QVariantMap options;
      options.insert("instance_type", toQString(serverInstanceType()));
      QJson::Serializer serializer;
      serializer.setIndentMode(QJson::IndentCompact);
      args << QString(serializer.serialize(options));
      
      //LOG(Info, "makeStartServerProcess: " << toString(m_ruby) << " " << toString(args.join(" ")));
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeStartWorkerProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onWorkerStarted(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("launch_workers");
      
      QVariantMap options;
      options.insert("instance_type", toQString(serverInstanceType()));
      options.insert("num", m_awsSettings.numWorkers());
      options.insert("server_id", toQString(m_awsSession.serverId()));
      options.insert("server_procs", m_awsSession.numServerProcessors());
      options.insert("timestamp", toQString(m_awsSession.timestamp()));

      if (m_privateKey.open()) {
        m_privateKey.write(m_awsSession.privateKey().c_str());
        options.insert("private_key", m_privateKey.fileName());
        m_privateKey.close();
      }
      QJson::Serializer serializer;
      serializer.setIndentMode(QJson::IndentCompact);
      args << QString(serializer.serialize(options));
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckServerRunningProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckServerRunningComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("instance_status");
      
      QVariantMap options;
      options.insert("instance_id", toQString(m_awsSession.serverId()));
      QJson::Serializer serializer;
      serializer.setIndentMode(QJson::IndentCompact);
      args << QString(serializer.serialize(options));
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckWorkerRunningProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckWorkerRunningComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("instance_status");
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeStopServerProcess() const
    {
      QProcess *p = new QProcess();
      //p->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
          this, SLOT(onServerStopped(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      //addProcessArguments(args);
      args << "halt";
      //p->start(processName(), args);
      return p;
    }

    QProcess *AWSProvider_Impl::makeStopWorkerProcess() const
    {
      QProcess *p = new QProcess();
      //p->setWorkingDirectory(toQString(m_vagrantSettings.workerPath()));
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
          this, SLOT(onWorkerStopped(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      //addProcessArguments(args);
      args << "halt";
      //p->start(processName(), args);
      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckTerminateProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckTerminatedComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      //addProcessArguments(args);
      args << "status";
      args << "default";

      //p->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      //p->start(processName(), args);
      return p;
    }

    bool AWSProvider_Impl::parseServiceAvailableResults(const ProcessResults &t_results)
    {
      QJson::Parser parser;
      bool ok = false;
      QVariantMap map = parser.parse(t_results.output.toUtf8(), &ok).toMap();
      if (ok) {
        if (!map.keys().contains("error")) return true;

        int code = map["error"].toMap()["code"].toInt();
        return (code != 503);
      } else {
        logError("Error parsing serviceAvailable JSON: " + toString(parser.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseValidateCredentialsResults(const ProcessResults &t_results)
    {
      QJson::Parser parser;
      bool ok = false;
      QVariantMap map = parser.parse(t_results.output.toUtf8(), &ok).toMap();
      if (ok) {
        if (!map.keys().contains("error")) return true;

        int code = map["error"].toMap()["code"].toInt();
        if (code == 401) {
          logWarning("Invalid Access Key");
        } else if (code == 403) {
          logWarning("Invalid Secret Key");
        }
      } else {
        logError("Error parsing validateCredentials JSON: " + toString(parser.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseResourcesAvailableToStartResults(const ProcessResults &t_results)
    {
      QJson::Parser parser;
      bool ok = false;
      QVariantMap map = parser.parse(t_results.output.toUtf8(), &ok).toMap();
      if (ok) {
        if (!map.keys().contains("error")) {
          m_lastTotalInstances = map["instances"].toUInt();
          if (m_awsSettings.numWorkers() + 1 + m_lastTotalInstances <= 20) return true;
        } else {
          logError(map["error"].toMap()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing resourcesAvailableToStart JSON: " + toString(parser.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseServerStartedResults(const ProcessResults &t_results)
    {
      QJson::Parser parser;
      bool ok = false;
      QVariantMap map = parser.parse(t_results.output.toUtf8(), &ok).toMap();
      if (ok) {
        if (!map.keys().contains("error")) {
          m_awsSession.setTimestamp(map["timestamp"].toString().toStdString());
          m_awsSession.setPrivateKey(map["private_key"].toString().toStdString());
          std::string url = map["server"].toMap()["ip"].toString().toStdString();
          m_awsSession.setServerUrl(Url(map["server"].toMap()["ip"].toString()));
          m_awsSession.setServerId(map["server"].toMap()["id"].toString().toStdString());
          m_awsSession.setNumServerProcessors(map["server"].toMap()["procs"].toUInt());
          return true;
        } else {
          logError(map["error"].toMap()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing serverStarted JSON: " + toString(parser.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseWorkerStartedResults(const ProcessResults &t_results)
    {
      m_privateKey.remove();
      QJson::Parser parser;
      bool ok = false;
      QVariantMap map = parser.parse(t_results.output.toUtf8(), &ok).toMap();
      if (ok) {
        if (!map.keys().contains("error")) {
          std::vector<Url> workerUrls;
          std::vector<std::string> workerIds;
          unsigned numWorkerProcessors = 0;
          Q_FOREACH(QVariant worker, map["workers"].toList()) {
            workerUrls.push_back(Url(worker.toMap()["ip"].toString()));
            workerIds.push_back(worker.toMap()["id"].toString().toStdString());
            if (numWorkerProcessors == 0) numWorkerProcessors = worker.toMap()["procs"].toUInt();
          }
          m_awsSession.setWorkerUrls(workerUrls);
          m_awsSession.setWorkerIds(workerIds);
          m_awsSession.setNumWorkerProcessors(numWorkerProcessors);
          
          return true;
        } else {
          logError(map["error"].toMap()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing workerStarted JSON: " + toString(parser.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseCheckServerRunningResults(const ProcessResults &t_results)
    {
      QJson::Parser parser;
      bool ok = false;
      QVariantMap map = parser.parse(t_results.output.toUtf8(), &ok).toMap();
      if (ok) {
        if (!map.keys().contains("error")) {
          std::string status = map[toQString(m_awsSession.serverId())].toString().toStdString();
          if (status == "running") return true;
          logError("Server " + m_awsSession.serverId() + " is not running");
        } else {
          logError(map["error"].toMap()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing checkServerRunning JSON: " + toString(parser.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseCheckWorkerRunningResults(const ProcessResults &t_results)
    {
      QJson::Parser parser;
      bool ok = false;
      QVariantMap map = parser.parse(t_results.output.toUtf8(), &ok).toMap();
      if (ok) {
        if (!map.keys().contains("error")) {
          std::vector<std::string> running;
          std::vector<std::string> notRunning;
          Q_FOREACH(std::string workerId, m_awsSession.workerIds()) {
            std::string status = map[toQString(workerId)].toString().toStdString();
            if (status == "running") {
              running.push_back(workerId);
            } else {
              notRunning.push_back(workerId);
            }
          }
          if (running.size() == m_awsSession.workerIds().size()) return true;
          std::string output = notRunning.size() + "/" + m_awsSession.workerIds().size();
          output += " workers are not running (" + boost::algorithm::join(notRunning, ", ") + ")";
          logError(output);
        } else {
          logError(map["error"].toMap()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing checkWorkerRunning JSON: " + toString(parser.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseServerStoppedResults(const ProcessResults &t_results)
    {
      return true;
    }

    bool AWSProvider_Impl::parseWorkerStoppedResults(const ProcessResults &t_results)
    {
      return true;
    }

    bool AWSProvider_Impl::parseCheckTerminatedResults(const ProcessResults &t_results)
    {
      // if halt on stop this is expected:
      // default                   poweroff (virtualbox)
      return t_results.output.contains("poweroff (virtualbox)");
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


    void AWSProvider_Impl::onCheckInternetComplete(int, QProcess::ExitStatus)
    {
      m_lastInternetAvailable = parseServiceAvailableResults(handleProcessCompleted(m_checkInternetProcess));
      m_checkInternetProcess = 0;
    }

    void AWSProvider_Impl::onCheckServiceComplete(int, QProcess::ExitStatus)
    {
      m_lastServiceAvailable = parseServiceAvailableResults(handleProcessCompleted(m_checkServiceProcess));
      m_checkServiceProcess = 0;
    }

    void AWSProvider_Impl::onCheckValidateComplete(int, QProcess::ExitStatus)
    {
      m_lastValidateCredentials = parseValidateCredentialsResults(handleProcessCompleted(m_checkValidateProcess));
      m_checkValidateProcess = 0;
    }

    void AWSProvider_Impl::onCheckResourcesComplete(int, QProcess::ExitStatus)
    {
      m_lastResourcesAvailableToStart = parseResourcesAvailableToStartResults(handleProcessCompleted(m_checkResourcesProcess));
      m_checkResourcesProcess = 0;
    }
    
    void AWSProvider_Impl::onServerStarted(int, QProcess::ExitStatus)
    {
      // DLM: requestServerStartedFinished was checking for m_startServerProcess == 0
      // however, handleProcessCompleted was setting that before parseServerStartedResults could run
      m_serverStarted = parseServerStartedResults(handleProcessCompleted(m_startServerProcess));
      m_startServerProcess = 0;
    }

    void AWSProvider_Impl::onServerStartedError(QProcess::ProcessError error)
    {
      if (error == QProcess::FailedToStart){
        LOG(Error, "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.");
      }else if (error == QProcess::Crashed){
        LOG(Error, "The process crashed some time after starting successfully.");
      }else if (error == QProcess::Timedout){
        LOG(Error, "The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.");
      }else if (error == QProcess::WriteError){
        LOG(Error, "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.");
      }else if (error == QProcess::ReadError){
        LOG(Error, "An error occurred when attempting to read from the process. For example, the process may not be running.");
      }else if (error == QProcess::ReadError){
        LOG(Error, "An unknown error occurred. This is the default return value of error().");
      }

      m_serverStarted = false;
      m_startServerProcess->deleteLater();
      m_startServerProcess = 0;
    }

    void AWSProvider_Impl::onWorkerStarted(int, QProcess::ExitStatus)
    {
      m_workerStarted = parseWorkerStartedResults(handleProcessCompleted(m_startWorkerProcess));
      m_startWorkerProcess = 0;
    }

    void AWSProvider_Impl::onCheckServerRunningComplete(int, QProcess::ExitStatus)
    {
      m_lastServerRunning = parseCheckServerRunningResults(handleProcessCompleted(m_checkServerRunningProcess));
      m_checkServerRunningProcess = 0;
    }
    
    void AWSProvider_Impl::onCheckWorkerRunningComplete(int, QProcess::ExitStatus)
    {
      m_lastWorkerRunning = parseCheckWorkerRunningResults(handleProcessCompleted(m_checkWorkerRunningProcess));
      m_checkWorkerRunningProcess = 0;
    }
    
    void AWSProvider_Impl::onServerStopped(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_stopServerProcess);

      QString output = m_stopServerProcess->readAllStandardOutput();
      QString errors = m_stopServerProcess->readAllStandardError();

      m_serverStopped = true;

      if (m_serverStopped && m_workerStopped){
        emit CloudProvider_Impl::terminated();
      }

      m_stopServerProcess->deleteLater();
      m_stopServerProcess = 0;
    }

    void AWSProvider_Impl::onWorkerStopped(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_stopWorkerProcess);

      QString output = m_stopWorkerProcess->readAllStandardOutput();
      QString errors = m_stopWorkerProcess->readAllStandardError();

      m_workerStopped = true;

      if (m_serverStopped && m_workerStopped){
        emit CloudProvider_Impl::terminated();
      }

      m_stopWorkerProcess->deleteLater();
      m_stopWorkerProcess = 0;
    }

    void AWSProvider_Impl::onCheckTerminatedComplete(int, QProcess::ExitStatus)
    {
      // note, it's important that this functon is always called, to clean up the QProcess object
      bool terminated = parseCheckTerminatedResults(handleProcessCompleted(m_checkTerminatedProcess));
      m_checkTerminatedProcess = 0;

      //if (m_awsSettings.haltOnStop()){
        if (terminated) {
          // depend on local state variable for the worker status
          if (m_workerStopped){
            m_lastTerminateCompleted = true;
          }
        }
      //}else{
        // depend on local state variable in this case
        m_lastTerminateCompleted = m_terminateStarted;
      //}

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
                           unsigned numWorkers,
                           bool terminationDelayEnabled,
                           unsigned terminationDelay)
    : CloudSettings(boost::shared_ptr<detail::AWSSettings_Impl>(
                      new detail::AWSSettings_Impl(uuid,
                                                   versionUUID,
                                                   userAgreementSigned,
                                                   numWorkers,
                                                   terminationDelayEnabled,
                                                   terminationDelay)))
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

  unsigned AWSSettings::numWorkers() const {
    return getImpl<detail::AWSSettings_Impl>()->numWorkers();
  }

  unsigned AWSSettings::setNumWorkers(const unsigned numWorkers) {
    return getImpl<detail::AWSSettings_Impl>()->setNumWorkers(numWorkers);
  }

  bool AWSSettings::terminationDelayEnabled() const {
    return getImpl<detail::AWSSettings_Impl>()->terminationDelayEnabled();
  }

   void AWSSettings::setTerminationDelayEnabled(bool enabled) {
    getImpl<detail::AWSSettings_Impl>()->setTerminationDelayEnabled(enabled);
  }

  unsigned AWSSettings::terminationDelay() const {
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

  AWSSession::AWSSession(const UUID& uuid,
                         const UUID& versionUUID,
                         const std::string& sessionId,
                         const boost::optional<Url>& serverUrl,
                         const std::string& serverId,
                         const unsigned numServerProcessors,
                         const std::vector<Url>& workerUrls,
                         const std::vector<std::string>& workerIds,
                         const unsigned numWorkerProcessors,
                         const std::string& privateKey,
                         const std::string& timestamp,
                         const std::string& region,
                         const std::string& serverInstanceType,
                         const std::string& workerInstanceType)
    : CloudSession(boost::shared_ptr<detail::AWSSession_Impl>(
                     new detail::AWSSession_Impl(uuid,
                                                 versionUUID,
                                                 sessionId,
                                                 serverUrl,
                                                 serverId,
                                                 numServerProcessors,
                                                 workerUrls,
                                                 workerIds,
                                                 numWorkerProcessors,
                                                 privateKey,
                                                 timestamp,
                                                 region,
                                                 serverInstanceType,
                                                 workerInstanceType)))
    {
      OS_ASSERT(getImpl<detail::AWSSession_Impl>());
    }

  AWSSession::AWSSession(const boost::shared_ptr<detail::AWSSession_Impl>& impl)
    : CloudSession(impl)
  {
    OS_ASSERT(getImpl<detail::AWSSession_Impl>());
  }

  //Url AWSSession::serverUrl() const {
  //  return getImpl<detail::AWSSession_Impl>()->serverUrl(); 
  //}

  //void AWSSession::setServerUrl(const Url& serverUrl) {
  //  getImpl<detail::AWSSession_Impl>()->setServerUrl(serverUrl);
  //}

  std::string AWSSession::serverId() const {
    return getImpl<detail::AWSSession_Impl>()->serverId(); 
  }

  void AWSSession::setServerId(const std::string& serverId) {
    getImpl<detail::AWSSession_Impl>()->setServerId(serverId);
  }

  unsigned AWSSession::numServerProcessors() const {
    return getImpl<detail::AWSSession_Impl>()->numServerProcessors();
  }

  void AWSSession::setNumServerProcessors(const unsigned numServerProcessors) {
    getImpl<detail::AWSSession_Impl>()->setNumServerProcessors(numServerProcessors);
  }

  //std::vector<Url> AWSSession::workerUrls() const {
  //  return getImpl<detail::AWSSession_Impl>()->workerUrls();
  //}

  void AWSSession::setWorkerUrls(const std::vector<Url>& workerUrls) {
    getImpl<detail::AWSSession_Impl>()->setWorkerUrls(workerUrls);
  }

  std::vector<std::string> AWSSession::workerIds() const {
    return getImpl<detail::AWSSession_Impl>()->workerIds();
  }

  void AWSSession::setWorkerIds(const std::vector<std::string>& workerIds) {
    getImpl<detail::AWSSession_Impl>()->setWorkerIds(workerIds);
  }

  unsigned AWSSession::numWorkerProcessors() const {
    return getImpl<detail::AWSSession_Impl>()->numWorkerProcessors();
  }

  void AWSSession::setNumWorkerProcessors(const unsigned numWorkerProcessors) {
    getImpl<detail::AWSSession_Impl>()->setNumWorkerProcessors(numWorkerProcessors);
  }

  std::string AWSSession::privateKey() const {
    return getImpl<detail::AWSSession_Impl>()->privateKey();
  }

  void AWSSession::setPrivateKey(const std::string& privateKey) {
    getImpl<detail::AWSSession_Impl>()->setPrivateKey(privateKey);
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
  
  unsigned AWSSession::numWorkers() const {
    return getImpl<detail::AWSSession_Impl>()->workerUrls().size();
  }

  unsigned AWSSession::totalSessionUptime() const {
    return getImpl<detail::AWSSession_Impl>()->totalSessionUptime();
  }

  unsigned AWSSession::totalSessionInstances() const {
    return getImpl<detail::AWSSession_Impl>()->totalSessionInstances();
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
  
  unsigned AWSProvider::setNumWorkers(const unsigned numWorkers) {
    return getImpl<detail::AWSProvider_Impl>()->setNumWorkers(numWorkers);
  }

  std::vector<std::string> AWSProvider::availableRegions() const {
    return getImpl<detail::AWSProvider_Impl>()->availableRegions();
  }

  std::string AWSProvider::defaultRegion() const {
    return getImpl<detail::AWSProvider_Impl>()->defaultRegion();
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

  bool AWSProvider::terminationDelayEnabled() const {
    return getImpl<detail::AWSProvider_Impl>()->terminationDelayEnabled();
  }

  void AWSProvider::setTerminationDelayEnabled(bool enabled) {
    getImpl<detail::AWSProvider_Impl>()->setTerminationDelayEnabled(enabled);
  }

  unsigned AWSProvider::terminationDelay() const {
    return getImpl<detail::AWSProvider_Impl>()->terminationDelay();
  }

  void AWSProvider::setTerminationDelay(const unsigned delay) {
    getImpl<detail::AWSProvider_Impl>()->setTerminationDelay(delay);
  }

  unsigned AWSProvider::numSessionWorkers() const {
    return getImpl<detail::AWSProvider_Impl>()->numSessionWorkers();
  }

  double AWSProvider::estimatedCharges() const {
    return getImpl<detail::AWSProvider_Impl>()->estimatedCharges();
  }

  unsigned AWSProvider::totalSessionUptime() const {
    return getImpl<detail::AWSProvider_Impl>()->totalSessionUptime();
  }

  unsigned AWSProvider::totalSessionInstances() const {
    return getImpl<detail::AWSProvider_Impl>()->totalSessionInstances();
  }

  unsigned AWSProvider::totalInstances() const {
    return getImpl<detail::AWSProvider_Impl>()->totalInstances();
  }


} // openstudio
