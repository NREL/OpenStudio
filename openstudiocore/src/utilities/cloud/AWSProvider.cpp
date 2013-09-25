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
      onChange();
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

    double AWSSession_Impl::estimatedCharges() const {
      //todo
      return 0.0;
    }

    unsigned AWSSession_Impl::totalSessionUptime() const {
      //todo
      return 0;
    }

    unsigned AWSSession_Impl::totalSessionInstances() const {
      //todo
      return 0;
    }

    unsigned AWSSession_Impl::totalInstances() const {
      //todo
      return 0;
    }


    AWSProvider_Impl::AWSProvider_Impl()
      : CloudProvider_Impl(),
        m_awsSettings(),
        m_awsSession(toString(createUUID()),boost::none,std::vector<Url>()),
        m_script(getOpenStudioRubyScriptsPath() / toPath("cloud/aws.rb")),
        m_checkServiceProcess(0),
        m_checkValidateProcess(0),
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

      m_regions.push_back("us-east-1");
      m_awsSession.setRegion(defaultRegion());

      m_serverInstanceTypes.push_back("t1.micro");
      m_serverInstanceTypes.push_back("m1.medium");
      m_serverInstanceTypes.push_back("m1.large");
      m_serverInstanceTypes.push_back("m1.xlarge");

      m_workerInstanceTypes.push_back("t1.micro");
      m_workerInstanceTypes.push_back("c1.xlarge");
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
      if (awsSession->serverUrl().isEmpty()){
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
        if (waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestInternetAvailableRequestFinished, this))){
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
      return waitForFinished(msec, boost::bind(&AWSProvider_Impl::serverStarted, this));
    }

    bool AWSProvider_Impl::waitForWorkers(int msec)
    {
      return waitForFinished(msec, boost::bind(&AWSProvider_Impl::workersStarted, this));
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
      // todo
      return false;
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
      // todo
      return false;
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

    bool AWSProvider_Impl::requestServerRunning()
    {
      // todo
      return false;
    }

    bool AWSProvider_Impl::requestWorkersRunning()
    {
      // todo
      return false;
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
      return m_awsSession.estimatedCharges();
    }

    unsigned AWSProvider_Impl::totalSessionUptime() const {
      return m_awsSession.totalSessionUptime();
    }

    unsigned AWSProvider_Impl::totalSessionInstances() const {
      return m_awsSession.totalSessionInstances();
    }

    unsigned AWSProvider_Impl::totalInstances() const {
      return m_awsSession.totalInstances();
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

    bool AWSProvider_Impl::requestInternetAvailableRequestFinished() const
    {
      //todo
      return false;
    }

    bool AWSProvider_Impl::requestServiceAvailableFinished() const
    {
      return (m_checkServiceProcess == 0);
    }

    bool AWSProvider_Impl::requestValidateCredentialsFinished() const
    {
      return (m_checkValidateProcess == 0);
    }

    bool AWSProvider_Impl::requestResourcesAvailableToStartFinished() const
    {
      //todo
      return false;
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
      return (m_serverStopped && m_workerStopped);
    }

    bool AWSProvider_Impl::requestTerminateCompletedFinished() const
    {
      return (m_checkTerminatedProcess == 0);
    }

    ProcessResults AWSProvider_Impl::handleProcessCompleted(QProcess *& t_qp)
    {
      OS_ASSERT(t_qp);

      ProcessResults pr(t_qp->exitCode(), t_qp->exitStatus(), t_qp->readAllStandardOutput(),
          t_qp->readAllStandardError());

      t_qp->deleteLater();
      t_qp = 0;

      return pr;
    }

    QProcess *AWSProvider_Impl::makeCheckServiceProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckServiceComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << toQString("EC2");
      args << toQString("describe_availability_zones");
      
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
      args << toQString("EC2");
      args << toQString("describe_availability_zones");
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeStartServerProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onServerStarted(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      //addProcessArguments(args);
      args << "up";

      //p->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      //p->start(processName(), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeStartWorkerProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onWorkerStarted(int, QProcess::ExitStatus)));
      OS_ASSERT(test);


      QStringList args;
      //addProcessArguments(args);
      args << "up";

      //p->setWorkingDirectory(toQString(m_vagrantSettings.workerPath()));
      //p->start(processName(), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckServerRunningProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckServerRunningComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      //addProcessArguments(args);
      args << "status";
      args << "default";

      //p->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      //p->start(processName(), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckWorkerRunningProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckWorkerRunningComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      //addProcessArguments(args);
      args << "status";
      args << "default";

      //p->setWorkingDirectory(toQString(m_vagrantSettings.workerPath()));
      //p->start(processName(), args);
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

    bool AWSProvider_Impl::parseServerStartedResults(const ProcessResults &t_results)
    {
      /*if (m_awsSession.serverUrl())
      {
        m_awsSession.setServerUrl(*m_awsSession.serverUrl());
      }*/
      return true;
    }

    bool AWSProvider_Impl::parseWorkerStartedResults(const ProcessResults &t_results)
    {
      m_awsSession.clearWorkerUrls();
      //m_awsSession.addWorkerUrl(m_awsSettings.workerUrl());
      return true;
    }

    bool AWSProvider_Impl::parseCheckServerRunningResults(const ProcessResults &t_results)
    {
      // if running this is expected
      //default                   running (virtualbox)
      return t_results.output.contains("running (virtualbox)");
    }

    bool AWSProvider_Impl::parseCheckWorkerRunningResults(const ProcessResults &t_results)
    {
      // if running this is expected
      //default                   running (virtualbox)
      return t_results.output.contains("running (virtualbox)");
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


    void AWSProvider_Impl::processInternetAvailable()
    {
      /*if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastInternetAvailable = true;
      }

      m_networkReply->deleteLater();
      m_networkReply = 0;*/
    }

    void AWSProvider_Impl::onCheckServiceComplete(int, QProcess::ExitStatus)
    {
      m_lastServiceAvailable = parseServiceAvailableResults(handleProcessCompleted(m_checkServiceProcess));
    }

    void AWSProvider_Impl::onCheckValidateComplete(int, QProcess::ExitStatus)
    {
      m_lastValidateCredentials = parseValidateCredentialsResults(handleProcessCompleted(m_checkValidateProcess));
    }
    
    void AWSProvider_Impl::onServerStarted(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_startServerProcess);

      QString output = m_startServerProcess->readAllStandardOutput();
      QString errors = m_startServerProcess->readAllStandardError();
      
      //m_awsSession.setServerUrl();

      m_serverStarted = true;

      emit CloudProvider_Impl::serverStarted(m_awsSession.serverUrl());

      m_startServerProcess->deleteLater();
      m_startServerProcess = 0;
    }

    void AWSProvider_Impl::onWorkerStarted(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_startWorkerProcess);

      QString output = m_startWorkerProcess->readAllStandardOutput();
      QString errors = m_startWorkerProcess->readAllStandardError();
      
      m_awsSession.clearWorkerUrls();
      //m_awsSession.addWorkerUrl();

      m_workerStarted = true;

      //emit CloudProvider_Impl::workerStarted(m_awsSession.workerUrls());

      //if all, emit CloudProvider_Impl::allWorkersStarted();

      m_startWorkerProcess->deleteLater();
      m_startWorkerProcess = 0;
    }

    void AWSProvider_Impl::onCheckServerRunningComplete(int, QProcess::ExitStatus)
    {
      bool running = parseCheckServerRunningResults(handleProcessCompleted(m_checkServerRunningProcess));
      //if (m_awsSettings.haltOnStop()){
        if (running) {
          m_lastServerRunning = true;
        }
      //}else{
        // depend on local state variable in this case
        m_lastServerRunning = !m_serverStopped;
      //}
    }
    
    void AWSProvider_Impl::onCheckWorkerRunningComplete(int, QProcess::ExitStatus)
    {
      bool running = parseCheckWorkerRunningResults(handleProcessCompleted(m_checkWorkerRunningProcess));
      //if (m_awsSettings.haltOnStop()){
        if (running){
          m_lastWorkerRunning = true;
        }
      //}else{
        // depend on local state variable in this case
        m_lastWorkerRunning = !m_workerStopped;
      //}
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

      m_startServerProcess->deleteLater();
      m_startServerProcess = 0;
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
  
  unsigned AWSSession::numWorkers() const {
    return getImpl<detail::AWSSession_Impl>()->workerUrls().size();
  }

  double AWSSession::estimatedCharges() const {
    return getImpl<detail::AWSSession_Impl>()->estimatedCharges();
  }

  unsigned AWSSession::totalSessionUptime() const {
    return getImpl<detail::AWSSession_Impl>()->totalSessionUptime();
  }

  unsigned AWSSession::totalSessionInstances() const {
    return getImpl<detail::AWSSession_Impl>()->totalSessionInstances();
  }

  unsigned AWSSession::totalInstances() const {
    return getImpl<detail::AWSSession_Impl>()->totalInstances();
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
