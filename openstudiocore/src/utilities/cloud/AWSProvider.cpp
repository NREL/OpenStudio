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

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QList>
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
        m_terminationDelay(0),
        m_region(AWSProvider::defaultRegion()),
        m_serverInstanceType(AWSProvider::defaultServerInstanceType()),
        m_workerInstanceType(AWSProvider::defaultWorkerInstanceType())
    {
      loadSettings(true);
    }

    AWSSettings_Impl::AWSSettings_Impl(const UUID& uuid,
                                       const UUID& versionUUID,
                                       bool userAgreementSigned,
                                       unsigned numWorkers,
                                       bool terminationDelayEnabled,
                                       unsigned terminationDelay/*,
                                       std::string region,
                                       std::string serverInstanceType,
                                       std::string workerInstanceType*/)
      : CloudSettings_Impl(uuid,versionUUID),
        m_validAccessKey(false),
        m_validSecretKey(false)
    {
      loadSettings(true);
      m_userAgreementSigned = userAgreementSigned;
      setNumWorkers(numWorkers);
      setTerminationDelayEnabled(terminationDelayEnabled);
      setTerminationDelay(terminationDelay);
      // todo
      //setRegion(region);
      //setServerInstanceType(serverInstanceType);
      //setWorkerInstanceType(workerInstanceType);
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

      if (overwriteExisting && !settings.value("region").isNull()) {
        setRegion(settings.value("region").toString().toStdString());
      }

      if (overwriteExisting && !settings.value("serverInstanceType").isNull()) {
        setServerInstanceType(settings.value("serverInstanceType").toString().toStdString());
      }

      if (overwriteExisting && !settings.value("workerInstanceType").isNull()) {
        setWorkerInstanceType(settings.value("workerInstanceType").toString().toStdString());
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

      if (overwriteExisting || settings.value("region").isNull()) {
        settings.setValue("region", toQString(m_region));
      }

      if (overwriteExisting || settings.value("serverInstanceType").isNull()) {
        settings.setValue("serverInstanceType", toQString(m_serverInstanceType));
      }

      if (overwriteExisting || settings.value("workerInstanceType").isNull()) {
        settings.setValue("workerInstanceType", toQString(m_workerInstanceType));
      }

      return true;
    }

    std::string AWSSettings_Impl::accessKey() const {
      return m_accessKey;
    }

    bool AWSSettings_Impl::setAccessKey(const std::string& accessKey) {
      std::string key = toQString(accessKey).trimmed().toStdString();
      if (validAccessKey(key)) {
        if (m_accessKey != key) {
          m_accessKey = key;
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
      std::string key = toQString(secretKey).trimmed().toStdString();
      if (validSecretKey(key)) {
        if (m_secretKey != key) {
          m_secretKey = key;
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

    std::string AWSSettings_Impl::region() const {
      return m_region;
    }

    void AWSSettings_Impl::setRegion(const std::string& region) {
      if (m_region != region) {
        m_region = region;
        onChange();
      }
    }

    std::string AWSSettings_Impl::serverInstanceType() const {
      return m_serverInstanceType;
    }

    void AWSSettings_Impl::setServerInstanceType(const std::string& instanceType) {
      if (m_serverInstanceType != instanceType) {
        m_serverInstanceType = instanceType;
        onChange();
      }
    }

    std::string AWSSettings_Impl::workerInstanceType() const {
      return m_workerInstanceType;
    }

    void AWSSettings_Impl::setWorkerInstanceType(const std::string& instanceType) {
      if (m_workerInstanceType != instanceType) {
        m_workerInstanceType = instanceType;
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

    std::vector<std::string> AWSSession_Impl::workerIds() const {
      return m_workerIds;
    }

    void AWSSession_Impl::addWorkerId(const std::string& workerId) {
      m_workerIds.push_back(workerId);
      onChange();
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
      unsigned now = QDateTime().currentDateTime().toTime_t();
      unsigned startTime = toQString(m_timestamp).toLongLong();

      if (m_timestamp.empty() || startTime > now) return 0;
      return (now - startTime) / 60;
    }

    unsigned AWSSession_Impl::totalSessionInstances() const {
      unsigned instances = 0;
      if (serverUrl()) ++instances;
      instances += workerUrls().size();
      return instances;
    }


    AWSProvider_Impl::AWSProvider_Impl()
      : CloudProvider_Impl(),
        m_awsSettings(),
        m_awsSession(toString(createUUID()),boost::none,std::vector<Url>()),
        m_checkInternetProcess(0),
        m_checkServiceProcess(0),
        m_checkValidateProcess(0),
        m_checkResourcesProcess(0),
        m_startServerProcess(0),
        m_startWorkerProcess(0),
        m_checkServerRunningProcess(0),
        m_checkWorkerRunningProcess(0),
        m_stopInstancesProcess(0),
        m_checkTerminatedProcess(0),
        m_checkEstimatedChargesProcess(0),
        m_checkTotalInstancesProcess(0),
        m_lastInternetAvailable(false),
        m_lastServiceAvailable(false),
        m_lastValidateCredentials(false),
        m_lastResourcesAvailableToStart(false),
        m_serverStarted(false),
        m_workerStarted(false),
        m_lastServerRunning(false),
        m_lastWorkerRunning(false),
        m_instancesStopped(false),
        m_terminateStarted(false),
        m_lastTerminateCompleted(false),
        m_lastEstimatedCharges(0),
        m_lastTotalInstances(0)
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

      if (applicationIsRunningFromBuildDirectory())
      {
        m_script = getApplicationBuildDirectory() / openstudio::toPath("ruby/cloud/aws.rb");
      } else {
#ifdef Q_OS_LINUX
        m_script = getApplicationInstallDirectory() / openstudio::toPath("share/openstudio/Ruby/cloud/aws.rb");
#elif defined(Q_OS_MAC)
        m_script = getApplicationRunDirectory().parent_path().parent_path().parent_path() / openstudio::toPath("Ruby/cloud/aws.rb");
#else
        m_script = getApplicationRunDirectory().parent_path() / openstudio::toPath("Ruby/cloud/aws.rb");
#endif
      }
      if (!boost::filesystem::exists(m_script)) {
        LOG_AND_THROW("AWS script cannot be found.");
      }

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
      return m_lastEstimatedCharges;
    }

    double AWSProvider_Impl::lastEstimatedCharges() const
    {
      return m_lastEstimatedCharges;
    }

    unsigned AWSProvider_Impl::lastTotalInstances() const
    {
      return m_lastTotalInstances;
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

    double AWSProvider_Impl::estimatedCharges(int msec)
    {
      if (requestEstimatedCharges()){
        if (waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestEstimatedChargesFinished, this))){
          return lastEstimatedCharges();
        }
      }
      return 0;
    }

    unsigned AWSProvider_Impl::totalInstances(int msec)
    {
      if (requestTotalInstances()){
        if (waitForFinished(msec, boost::bind(&AWSProvider_Impl::requestTotalInstancesFinished, this))){
          return lastTotalInstances();
        }
      }
      return 0;
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

      if (!m_awsSession.workerUrls().size()) {
        logWarning("There is no server");
        return false;
      }

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

      if (!m_awsSession.workerUrls().size()) {
        logWarning("There are no workers");
        return false;
      }

      m_lastWorkerRunning = false;

      m_checkWorkerRunningProcess = makeCheckWorkerRunningProcess();

      return true;
    }

    bool AWSProvider_Impl::requestTerminate()
    {
      if (!(m_serverStarted || m_workerStarted)){
        // haven't started, nothing to terminate
        return false;
      }
      if (m_terminateStarted || m_stopInstancesProcess){
        // already terminating
        return false;
      }

      m_terminateStarted = true;

      clearErrorsAndWarnings();

      emit CloudProvider_Impl::terminating();

      m_stopInstancesProcess = makeStopInstancesProcess();
            
      return true;
    }

    bool AWSProvider_Impl::requestTerminateCompleted()
    {
      if (!m_terminateStarted || m_checkTerminatedProcess) {
        // haven't started terminating or already checking, nothing to verify
        return false;
      }
      if (m_lastTerminateCompleted) {
        // already verified termination
        return true;
      }

      clearErrorsAndWarnings();

      m_lastTerminateCompleted = false;

      m_checkTerminatedProcess = makeCheckTerminateProcess();

      return true;
    }

    bool AWSProvider_Impl::requestEstimatedCharges()
    {
      if (m_checkEstimatedChargesProcess){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_lastEstimatedCharges = 0;

      m_checkEstimatedChargesProcess = makeCheckEstimatedChargesProcess();

      return true;
    }

    bool AWSProvider_Impl::requestTotalInstances()
    {
      if (m_checkTotalInstancesProcess){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_lastTotalInstances = 0;

      m_checkTotalInstancesProcess = makeCheckTotalInstancesProcess();

      return true;
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
      args << toQString(m_awsSettings.region());
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
      return (m_stopInstancesProcess == 0);
    }

    bool AWSProvider_Impl::requestTerminateCompletedFinished() const
    {
      return (m_checkTerminatedProcess == 0);
    }

    bool AWSProvider_Impl::requestEstimatedChargesFinished() const
    {
      return (m_checkEstimatedChargesProcess == 0);
    }

    bool AWSProvider_Impl::requestTotalInstancesFinished() const
    {
      return (m_checkTotalInstancesProcess == 0);
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
      options.insert("instance_type", toQString(m_awsSettings.serverInstanceType()));
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
      options.insert("instance_type", toQString(m_awsSettings.workerInstanceType()));
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

    QProcess *AWSProvider_Impl::makeStopInstancesProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onInstancesStopped(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("terminate_session");
      
      QVariantMap options;
      QStringList workerIds;
      Q_FOREACH(std::string workerId, m_awsSession.workerIds()) {
        workerIds.push_back(toQString(workerId));
      }
      options.insert("server_id", toQString(m_awsSession.serverId()));
      options.insert("worker_ids", workerIds);
      
      QJson::Serializer serializer;
      serializer.setIndentMode(QJson::IndentCompact);
      args << QString(serializer.serialize(options));
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckTerminateProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckTerminatedComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("termination_status");
      
      QVariantMap options;
      QStringList workerIds;
      Q_FOREACH(std::string workerId, m_awsSession.workerIds()) {
        workerIds.push_back(toQString(workerId));
      }
      options.insert("server_id", toQString(m_awsSession.serverId()));
      options.insert("worker_ids", workerIds);
      
      QJson::Serializer serializer;
      serializer.setIndentMode(QJson::IndentCompact);
      args << QString(serializer.serialize(options));
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckEstimatedChargesProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckEstimatedChargesComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("CloudWatch");
      args << QString("estimated_charges");
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckTotalInstancesProcess() const
    {
      QProcess *p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckTotalInstancesComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("total_instances");
      
      p->start(toQString(m_ruby), args);

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
            m_awsSession.addWorkerUrl(Url(worker.toMap()["ip"].toString()));
            m_awsSession.addWorkerId(worker.toMap()["id"].toString().toStdString());
            if (numWorkerProcessors == 0) numWorkerProcessors = worker.toMap()["procs"].toUInt();
          }
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

    bool AWSProvider_Impl::parseInstancesStoppedResults(const ProcessResults &t_results)
    {
      QJson::Parser parser;
      bool ok = false;
      QVariantMap map = parser.parse(t_results.output.toUtf8(), &ok).toMap();
      if (ok) {
        if (!map.keys().contains("error")) {
          m_lastTerminateCompleted = true;

          emit CloudProvider_Impl::terminated();
          
          return true;
        } else {
          logError(map["error"].toMap()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing instancesStopped JSON: " + toString(parser.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseCheckTerminatedResults(const ProcessResults &t_results)
    {
      QJson::Parser parser;
      bool ok = false;
      QVariantMap map = parser.parse(t_results.output.toUtf8(), &ok).toMap();
      if (ok) {
        if (!map.keys().contains("error")) {
          if (map["all_instances_terminated"].toBool()) {
            emit CloudProvider_Impl::terminated();
          
            return true;
          }
        } else {
          logError(map["error"].toMap()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing checkTerminated JSON: " + toString(parser.errorString()));
      }
      
      return false;
    }

    double AWSProvider_Impl::parseCheckEstimatedChargesResults(const ProcessResults &t_results)
    {
      QJson::Parser parser;
      bool ok = false;
      QVariantMap map = parser.parse(t_results.output.toUtf8(), &ok).toMap();
      if (ok) {
        if (!map.keys().contains("error")) {
          return map["estimated_charges"].toDouble();
        } else {
          logError(map["error"].toMap()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing checkEstimatedCharges JSON: " + toString(parser.errorString()));
      }
      
      return false;
    }

    unsigned AWSProvider_Impl::parseCheckTotalInstancesResults(const ProcessResults &t_results)
    {
      QJson::Parser parser;
      bool ok = false;
      QVariantMap map = parser.parse(t_results.output.toUtf8(), &ok).toMap();
      if (ok) {
        if (!map.keys().contains("error")) {
          return map["total_instances"].toUInt();
        } else {
          logError(map["error"].toMap()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing checkTotalInstances JSON: " + toString(parser.errorString()));
      }
      
      return false;
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
    
    void AWSProvider_Impl::onInstancesStopped(int, QProcess::ExitStatus)
    {
      m_instancesStopped = parseInstancesStoppedResults(handleProcessCompleted(m_stopInstancesProcess));
      m_stopInstancesProcess = 0;
    }

    void AWSProvider_Impl::onCheckTerminatedComplete(int, QProcess::ExitStatus)
    {
      m_lastTerminateCompleted = parseCheckTerminatedResults(handleProcessCompleted(m_checkTerminatedProcess));
      m_checkTerminatedProcess = 0;
    }

    void AWSProvider_Impl::onCheckEstimatedChargesComplete(int, QProcess::ExitStatus)
    {
      m_lastEstimatedCharges = parseCheckEstimatedChargesResults(handleProcessCompleted(m_checkEstimatedChargesProcess));
      m_checkEstimatedChargesProcess = 0;
    }

    void AWSProvider_Impl::onCheckTotalInstancesComplete(int, QProcess::ExitStatus)
    {
      m_lastTotalInstances = parseCheckTotalInstancesResults(handleProcessCompleted(m_checkTotalInstancesProcess));
      m_checkTotalInstancesProcess = 0;
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

  std::string AWSSettings::region() const {
    return getImpl<detail::AWSSettings_Impl>()->region();
  }

  void AWSSettings::setRegion(const std::string& region) {
    getImpl<detail::AWSSettings_Impl>()->setRegion(region);
  }

  std::string AWSSettings::serverInstanceType() const {
    return getImpl<detail::AWSSettings_Impl>()->serverInstanceType();
  }

  void AWSSettings::setServerInstanceType(const std::string& instanceType) {
    getImpl<detail::AWSSettings_Impl>()->setServerInstanceType(instanceType);
  }

  std::string AWSSettings::workerInstanceType() const {
    return getImpl<detail::AWSSettings_Impl>()->workerInstanceType();
  }

  void AWSSettings::setWorkerInstanceType(const std::string& instanceType) {
    getImpl<detail::AWSSettings_Impl>()->setWorkerInstanceType(instanceType);
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

  std::vector<std::string> AWSSession::workerIds() const {
    return getImpl<detail::AWSSession_Impl>()->workerIds();
  }

  void AWSSession::addWorkerId(const std::string& workerId) {
    getImpl<detail::AWSSession_Impl>()->addWorkerId(workerId);
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
  
  std::vector<std::string> AWSProvider::availableRegions() {
    std::vector<std::string> regions;
    regions.push_back("us-east-1");
    return regions;
  }

  std::string AWSProvider::defaultRegion() {
    return "us-east-1";
  }

  std::vector<std::string> AWSProvider::serverInstanceTypes() {
    std::vector<std::string> serverInstanceTypes;
    serverInstanceTypes.push_back("t1.micro");
    serverInstanceTypes.push_back("m1.medium");
    serverInstanceTypes.push_back("m1.large");
    serverInstanceTypes.push_back("m1.xlarge");
    return serverInstanceTypes;
  }

  std::string AWSProvider::defaultServerInstanceType() {
    return "m1.medium";
  }

  std::vector<std::string> AWSProvider::workerInstanceTypes() {
    std::vector<std::string> workerInstanceTypes;
    workerInstanceTypes.push_back("t1.micro");
    workerInstanceTypes.push_back("c1.xlarge");
    return workerInstanceTypes;
  }

  std::string AWSProvider::defaultWorkerInstanceType() {
    return "c1.xlarge";
  }

  double AWSProvider::estimatedCharges(int msec) {
    return getImpl<detail::AWSProvider_Impl>()->estimatedCharges(msec);
  }

  unsigned AWSProvider::totalInstances(int msec) {
    return getImpl<detail::AWSProvider_Impl>()->totalInstances(msec);
  }


} // openstudio
