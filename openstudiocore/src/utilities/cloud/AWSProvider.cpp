/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
#include "AWSProvider.hpp"
#include "AWSProvider_Impl.hpp"

#include "../core/Application.hpp"
#include "../core/ApplicationPathHelpers.hpp"
#include "../core/Assert.hpp"
#include "../core/Path.hpp"
#include "../core/String.hpp"
#include "../core/System.hpp"

#include <OpenStudio.hxx>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QProcess>
#include <QSettings>
#include <QString>
#include <QTemporaryFile>
#include <QTextStream>
#include <QUrl>

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
        m_region(AWSProvider_Impl::defaultRegion()),
        m_serverInstanceType(AWSProvider_Impl::defaultServerInstanceType()),
        m_workerInstanceType(AWSProvider_Impl::defaultWorkerInstanceType())
    {
      loadSettings(true);
    }

    AWSSettings_Impl::AWSSettings_Impl(const UUID& uuid,
                                       const UUID& versionUUID,
                                       bool userAgreementSigned,
                                       unsigned numWorkers,
                                       bool terminationDelayEnabled,
                                       unsigned terminationDelay,
                                       std::string region,
                                       std::string serverInstanceType,
                                       std::string workerInstanceType)
      : CloudSettings_Impl(uuid,versionUUID),
        m_userAgreementSigned(false),
        m_validAccessKey(false),
        m_validSecretKey(false),
        m_numWorkers(2),
        m_terminationDelayEnabled(false),
        m_terminationDelay(0),
        m_region(AWSProvider_Impl::defaultRegion()),
        m_serverInstanceType(AWSProvider_Impl::defaultServerInstanceType()),
        m_workerInstanceType(AWSProvider_Impl::defaultWorkerInstanceType())
    {
      loadSettings(true);
      m_userAgreementSigned = userAgreementSigned;
      setNumWorkers(numWorkers);
      setTerminationDelayEnabled(terminationDelayEnabled);
      setTerminationDelay(terminationDelay);
      setRegion(region);
      setServerInstanceType(serverInstanceType);
      setWorkerInstanceType(workerInstanceType);
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
        m_accessKey = key;
        m_validAccessKey = true;
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
        m_secretKey = key;
        m_validSecretKey = true;
        return true;
      }
      return false;
    }

    bool AWSSettings_Impl::validAccessKey(const std::string& accessKey) const {
      return QRegExp("[A-Z0-9]{20}").exactMatch(toQString(accessKey));
    }

    bool AWSSettings_Impl::validAccessKey() const {
      return m_validAccessKey;
    }

    bool AWSSettings_Impl::validSecretKey(const std::string& secretKey) const {
      return QRegExp("[a-zA-Z0-9/+]{40}").exactMatch(toQString(secretKey));
    }

    bool AWSSettings_Impl::validSecretKey() const {
      return m_validSecretKey;
    }

    void AWSSettings_Impl::clearKeys() {
      m_accessKey.clear();
      m_secretKey.clear();
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
      std::vector<std::string> regions = AWSProvider_Impl::availableRegions();
      if (std::find(regions.begin(), regions.end(), region) != regions.end() && m_region != region) {
        m_region = region;
        onChange();
      }
    }

    std::string AWSSettings_Impl::serverInstanceType() const {
      return m_serverInstanceType;
    }

    void AWSSettings_Impl::setServerInstanceType(const std::string& instanceType) {
      std::vector<std::string> instanceTypes = AWSProvider_Impl::serverInstanceTypes();
      if (std::find(instanceTypes.begin(), instanceTypes.end(), instanceType) != instanceTypes.end() && m_serverInstanceType != instanceType) {
        m_serverInstanceType = instanceType;
        onChange();
      }
    }

    std::string AWSSettings_Impl::workerInstanceType() const {
      return m_workerInstanceType;
    }

    void AWSSettings_Impl::setWorkerInstanceType(const std::string& instanceType) {
      std::vector<std::string> instanceTypes = AWSProvider_Impl::workerInstanceTypes();
      if (std::find(instanceTypes.begin(), instanceTypes.end(), instanceType) != instanceTypes.end() && m_workerInstanceType != instanceType) {
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
        m_checkInternetProcess(nullptr),
        m_checkServiceProcess(nullptr),
        m_checkValidateProcess(nullptr),
        m_checkResourcesProcess(nullptr),
        m_startServerProcess(nullptr),
        m_startWorkerProcess(nullptr),
        m_checkServerRunningProcess(nullptr),
        m_checkWorkerRunningProcess(nullptr),
        m_stopInstancesProcess(nullptr),
        m_checkTerminatedProcess(nullptr),
        m_checkEstimatedChargesProcess(nullptr),
        m_checkTotalInstancesProcess(nullptr),
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
        m_script = getApplicationInstallDirectory() / openstudio::toPath("share/openstudio-" + openStudioVersion() + "/Ruby/cloud/aws.rb");
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

    // DLM: we should deal with running processes in the AWSProvider_Impl destructor

    std::vector<std::string> AWSProvider_Impl::availableRegions() {
      static std::vector<std::string> regions;
      if (!regions.size()) {
        regions.push_back("us-east-1");
      }
      return regions;
    }

    std::string AWSProvider_Impl::defaultRegion() {
      return "us-east-1";
    }

    std::vector<std::string> AWSProvider_Impl::serverInstanceTypes() {
      static std::vector<std::string> instanceTypes;
      if (!instanceTypes.size()) {
        std::vector<AWSComputerInformation> info = serverInformation();
        for (const AWSComputerInformation& awsComputerInformation : info) {
          instanceTypes.push_back(awsComputerInformation.instanceType);
        }
      }
      return instanceTypes;
    }

    std::string AWSProvider_Impl::defaultServerInstanceType() {
      return "m2.xlarge";
    }

    std::vector<std::string> AWSProvider_Impl::workerInstanceTypes() {
      static std::vector<std::string> instanceTypes;
      if (!instanceTypes.size()) {
        std::vector<AWSComputerInformation> info = workerInformation();
        for (const AWSComputerInformation& awsComputerInformation : info) {
          instanceTypes.push_back(awsComputerInformation.instanceType);
        }
      }
      return instanceTypes;
    }

    std::string AWSProvider_Impl::defaultWorkerInstanceType() {
      return "cc2.8xlarge";
    }

    std::vector<unsigned> AWSProvider_Impl::serverProcessorCounts() {
      static std::vector<unsigned> processorCounts;
      if (!processorCounts.size()) {
        std::vector<AWSComputerInformation> info = serverInformation();
        for (const AWSComputerInformation& awsComputerInformation : info) {
          processorCounts.push_back(awsComputerInformation.processorCount);
        }
      }
      return processorCounts;
    }

    std::vector<unsigned> AWSProvider_Impl::workerProcessorCounts() {
      static std::vector<unsigned> processorCounts;
      if (!processorCounts.size()) {
        std::vector<AWSComputerInformation> info = workerInformation();
        for (const AWSComputerInformation& awsComputerInformation : info) {
          processorCounts.push_back(awsComputerInformation.processorCount);
        }
      }
      return processorCounts;
    }

    std::vector<std::string> AWSProvider_Impl::serverPrettyNames() {
      static std::vector<std::string> prettyNames;
      if (!prettyNames.size()) {
        std::vector<AWSComputerInformation> info = serverInformation();
        for (const AWSComputerInformation& awsComputerInformation : info) {
          prettyNames.push_back(awsComputerInformation.prettyName);
        }
      }
      return prettyNames;
    }

    std::vector<std::string> AWSProvider_Impl::workerPrettyNames() {
      static std::vector<std::string> prettyNames;
      if (!prettyNames.size()) {
        std::vector<AWSComputerInformation> info = workerInformation();
        for (const AWSComputerInformation& awsComputerInformation : info) {
          prettyNames.push_back(awsComputerInformation.prettyName);
        }
      }
      return prettyNames;
    }

    std::string AWSProvider_Impl::getServerPrettyName(const std::string & instanceType) {
      static std::vector<std::string> instanceTypes = serverInstanceTypes();
      std::vector<std::string>::iterator it;
      it = std::find(instanceTypes.begin(), instanceTypes.end(), instanceType);
      unsigned index = it - instanceTypes.begin();
      OS_ASSERT(index < instanceTypes.size());

      static std::vector<std::string> prettyNames = serverPrettyNames();
      return prettyNames.at(index);
    }

    std::string AWSProvider_Impl::getWorkerPrettyName(const std::string & instanceType) {
      static std::vector<std::string> instanceTypes = workerInstanceTypes();
      std::vector<std::string>::iterator it;
      it = std::find(instanceTypes.begin(), instanceTypes.end(), instanceType);
      unsigned index = it - instanceTypes.begin();
      OS_ASSERT(index < instanceTypes.size());

      static std::vector<std::string> prettyNames = workerPrettyNames();
      return prettyNames.at(index);
    }

    unsigned AWSProvider_Impl::getServerProcessorCount(const std::string & instanceType) {
      static std::vector<std::string> instanceTypes = serverInstanceTypes();
      std::vector<std::string>::iterator it;
      it = std::find(instanceTypes.begin(), instanceTypes.end(), instanceType);
      unsigned index = it - instanceTypes.begin();
      OS_ASSERT(index < instanceTypes.size());

      static std::vector<unsigned> processorCounts = serverProcessorCounts();
      return processorCounts.at(index);
    }

    unsigned AWSProvider_Impl::getWorkerProcessorCount(const std::string & instanceType) {
      static std::vector<std::string> instanceTypes = workerInstanceTypes();
      std::vector<std::string>::iterator it;
      it = std::find(instanceTypes.begin(), instanceTypes.end(), instanceType);
      unsigned index = it - instanceTypes.begin();
      OS_ASSERT(index < instanceTypes.size());

      static std::vector<unsigned> processorCounts = workerProcessorCounts();
      return processorCounts.at(index);
    }

    std::vector<AWSComputerInformation> AWSProvider_Impl::serverInformation(){
      static std::vector<AWSComputerInformation> info;
      if (!info.size()) {
        AWSComputerInformation awsComputerInformation;

        // micro is currently insufficient to run server
        //awsComputerInformation.instanceType = "t1.micro";
        //awsComputerInformation.prettyName = "Micro";
        //awsComputerInformation.processorCount = 1;
        //info.push_back(awsComputerInformation);

        //awsComputerInformation.instanceType = "m1.large";
        //awsComputerInformation.prettyName = "M1 Large";
        //awsComputerInformation.processorCount = 2;
        //info.push_back(awsComputerInformation);

        //awsComputerInformation.instanceType = "m1.xlarge";
        //awsComputerInformation.prettyName = "M1 Extra Large";
        //awsComputerInformation.processorCount = 4;
        //info.push_back(awsComputerInformation);

        awsComputerInformation.instanceType = "m2.xlarge";
        awsComputerInformation.prettyName = "M2 Extra Large";
        awsComputerInformation.processorCount = 2;
        info.push_back(awsComputerInformation);

        awsComputerInformation.instanceType = "m2.2xlarge";
        awsComputerInformation.prettyName = "High-Memory Double Extra Large";
        awsComputerInformation.processorCount = 4;
        info.push_back(awsComputerInformation);

        awsComputerInformation.instanceType = "m2.4xlarge";
        awsComputerInformation.prettyName = "High-Memory Quadruple Extra Large";
        awsComputerInformation.processorCount = 8;
        info.push_back(awsComputerInformation);

        //awsComputerInformation.instanceType = "m3.xlarge";
        //awsComputerInformation.prettyName = "M3 Extra Large";
        //awsComputerInformation.processorCount = 2; // Hyperthreading disabled
        //info.push_back(awsComputerInformation);

        //awsComputerInformation.instanceType = "m3.2xlarge";
        //awsComputerInformation.prettyName = "M3 Double Extra Large";
        //awsComputerInformation.processorCount = 4; // Hyperthreading disabled
        //info.push_back(awsComputerInformation);
      }
      return info;
    }

    std::vector<AWSComputerInformation> AWSProvider_Impl::workerInformation(){
      static std::vector<AWSComputerInformation> info;
      if (!info.size()) {
        AWSComputerInformation awsComputerInformation;

        // micro is currently insufficient to run worker
        //awsComputerInformation.instanceType = "t1.micro";
        //awsComputerInformation.prettyName = "Micro";
        //awsComputerInformation.processorCount = 1;
        //info.push_back(awsComputerInformation);

        //awsComputerInformation.instanceType = "c1.xlarge";
        //awsComputerInformation.prettyName = "High-CPU Extra Large";
        //awsComputerInformation.processorCount = 8;
        //info.push_back(awsComputerInformation);

        awsComputerInformation.instanceType = "cc2.8xlarge";
        awsComputerInformation.prettyName = "Cluster Compute Eight Extra Large";
        awsComputerInformation.processorCount = 16; // Hyperthreading disabled
        info.push_back(awsComputerInformation);
      }
      return info;
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
        if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestInternetAvailableFinished, this))){
          return lastInternetAvailable();
        }
      }
      if (m_checkInternetProcess){
        m_checkInternetProcess->disconnect(this, nullptr);
        m_checkInternetProcess->kill();
        m_checkInternetProcess = nullptr;
      }
      return false;
    }

    bool AWSProvider_Impl::serviceAvailable(int msec)
    {
      if (requestServiceAvailable()){
        if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestServiceAvailableFinished, this))){
          return lastServiceAvailable();
        }
      }
      if (m_checkServiceProcess){
        m_checkServiceProcess->disconnect(this, nullptr);
        m_checkServiceProcess->kill();
        m_checkServiceProcess = nullptr;
      }
      return false;
    }

    bool AWSProvider_Impl::validateCredentials(int msec)
    {
      if (requestValidateCredentials()){
        if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestValidateCredentialsFinished, this))){
          return lastValidateCredentials();
        }
      }
      if (m_checkValidateProcess){
        m_checkValidateProcess->disconnect(this, nullptr);
        m_checkValidateProcess->kill();
        m_checkValidateProcess = nullptr;
      }
      return false;
    }

    bool AWSProvider_Impl::resourcesAvailableToStart(int msec)
    {
      if (requestResourcesAvailableToStart()){
        if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestResourcesAvailableToStartFinished, this))){
          return lastResourcesAvailableToStart();
        }
      }
      if (m_checkResourcesProcess){
        m_checkResourcesProcess->disconnect(this, nullptr);
        m_checkResourcesProcess->kill();
        m_checkResourcesProcess = nullptr;
      }
      return false;
    }

    bool AWSProvider_Impl::waitForServer(int msec)
    {
      if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestServerStartedFinished, this))){
        return serverStarted();
      }
      if (m_startServerProcess){
        m_startServerProcess->disconnect(this, nullptr);
        m_startServerProcess->kill();
        m_startServerProcess = nullptr;
      }
      return false;
    }

    bool AWSProvider_Impl::waitForWorkers(int msec)
    {
      if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestWorkerStartedFinished, this))){
        return workersStarted();
      }
      if (m_startWorkerProcess){
        m_startWorkerProcess->disconnect(this, nullptr);
        m_startWorkerProcess->kill();
        m_startWorkerProcess = nullptr;
      }
      return false;
    }

    bool AWSProvider_Impl::serverRunning(int msec)
    {
      if (requestServerRunning()){
        if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestServerRunningFinished, this))){
          return lastServerRunning();
        }
      }
      if (m_checkServerRunningProcess){
        m_checkServerRunningProcess->disconnect(this, nullptr);
        m_checkServerRunningProcess->kill();
        m_checkServerRunningProcess = nullptr;
      }
      return false;
    }

    bool AWSProvider_Impl::workersRunning(int msec)
    {
      if (requestWorkersRunning()){
        if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestWorkersRunningFinished, this))){
          return lastWorkersRunning();
        }
      }
      if (m_checkWorkerRunningProcess){
        m_checkWorkerRunningProcess->disconnect(this, nullptr);
        m_checkWorkerRunningProcess->kill();
        m_checkWorkerRunningProcess = nullptr;
      }
      return false;
    }

    bool AWSProvider_Impl::waitForTerminated(int msec)
    {
      if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestTerminateFinished, this))){
        return m_instancesStopped;
      }
      if (m_stopInstancesProcess){
        m_stopInstancesProcess->disconnect(this, nullptr);
        m_stopInstancesProcess->kill();
        m_stopInstancesProcess = nullptr;
      }
      return false;
    }

    bool AWSProvider_Impl::terminateCompleted(int msec)
    {
      if (requestTerminateCompleted()){
        if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestTerminateCompletedFinished, this))){
          return lastTerminateCompleted();
        }
      }
      if (m_checkTerminatedProcess){
        m_checkTerminatedProcess->disconnect(this, nullptr);
        m_checkTerminatedProcess->kill();
        m_checkTerminatedProcess = nullptr;
      }
      return false;
    }

    double AWSProvider_Impl::estimatedCharges(int msec)
    {
      if (requestEstimatedCharges()){
        if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestEstimatedChargesFinished, this))){
          return lastEstimatedCharges();
        }
      }
      if (m_checkEstimatedChargesProcess){
        m_checkEstimatedChargesProcess->disconnect(this, nullptr);
        m_checkEstimatedChargesProcess->kill();
        m_checkEstimatedChargesProcess = nullptr;
      }
      return 0.0;
    }

    unsigned AWSProvider_Impl::totalInstances(int msec)
    {
      if (requestTotalInstances()){
        if (waitForFinished(msec, std::bind(&AWSProvider_Impl::requestTotalInstancesFinished, this))){
          return lastTotalInstances();
        }
      }
      if (m_checkTotalInstancesProcess){
        m_checkTotalInstancesProcess->disconnect(this, nullptr);
        m_checkTotalInstancesProcess->kill();
        m_checkTotalInstancesProcess = nullptr;
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

      if (m_awsSettings.accessKey().empty()) {
        logError("The Access Key cannot be empty");
        return false;
      } else if (m_awsSettings.secretKey().empty()) {
        logError("The Secret Key cannot be empty");
        return false;
      } else if (!m_awsSettings.validAccessKey()) {
        logError("Invalid Access Key");
        return false;
      } else if (!m_awsSettings.validSecretKey()) {
        logError("Invalid Secret Key");
        return false;
      }

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

      if (!userAgreementSigned()) return false;

      if (!authenticated()) return false;

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

      if (!userAgreementSigned()) return false;

      if (!authenticated()) return false;

      m_serverStarted = false;

      emit CloudProvider_Impl::serverStarting();

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

      if (!userAgreementSigned()) return false;

      if (!authenticated()) return false;

      if (!m_serverStarted){
        logError("A server must be started before starting workers");
        return false;
      }

      m_workerStarted = false;

      for (unsigned i=0; i<m_awsSettings.numWorkers(); ++i) {
        emit CloudProvider_Impl::workerStarting();
      }

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

      if (!userAgreementSigned()) return false;

      if (!authenticated()) return false;

      if (!m_awsSession.serverUrl()) {
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

      if (!userAgreementSigned()) return false;

      if (!authenticated()) return false;

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

      if (!userAgreementSigned()) return false;

      if (!authenticated()) return false;

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

      if (!userAgreementSigned()) return false;

      if (!authenticated()) return false;

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

      if (!authenticated()) return false;

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

      if (!authenticated()) return false;

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

    bool AWSProvider_Impl::waitForFinished(int msec, const std::function<bool ()>& f) {
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
      return (m_checkInternetProcess == nullptr);
    }

    bool AWSProvider_Impl::requestServiceAvailableFinished() const
    {
      return (m_checkServiceProcess == nullptr);
    }

    bool AWSProvider_Impl::requestValidateCredentialsFinished() const
    {
      return (m_checkValidateProcess == nullptr);
    }

    bool AWSProvider_Impl::requestServerStartedFinished() const
    {
      return (m_startServerProcess == nullptr);
    }

    bool AWSProvider_Impl::requestWorkerStartedFinished() const
    {
      return (m_startWorkerProcess == nullptr);
    }

    bool AWSProvider_Impl::requestResourcesAvailableToStartFinished() const
    {
      return (m_checkResourcesProcess == nullptr);
    }

    bool AWSProvider_Impl::requestServerRunningFinished() const
    {
      return (m_checkServerRunningProcess == nullptr);
    }

    bool AWSProvider_Impl::requestWorkersRunningFinished() const
    {
      return (m_checkWorkerRunningProcess == nullptr);
    }

    bool AWSProvider_Impl::requestTerminateFinished() const
    {
      return (m_stopInstancesProcess == nullptr);
    }

    bool AWSProvider_Impl::requestTerminateCompletedFinished() const
    {
      return (m_checkTerminatedProcess == nullptr);
    }

    bool AWSProvider_Impl::requestEstimatedChargesFinished() const
    {
      return (m_checkEstimatedChargesProcess == nullptr);
    }

    bool AWSProvider_Impl::requestTotalInstancesFinished() const
    {
      return (m_checkTotalInstancesProcess == nullptr);
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
      auto p = new QProcess();
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
      auto p = new QProcess();
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
      auto p = new QProcess();
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
      auto p = new QProcess();
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
      auto p = new QProcess();
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
      
      QJsonObject options;
      options["instance_type"] = QJsonValue(toQString(m_awsSettings.serverInstanceType()));
      args << QString(QJsonDocument(options).toJson(QJsonDocument::Compact));
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeStartWorkerProcess() const
    {
      auto p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onWorkerStarted(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("launch_workers");
      
      QJsonObject options;
      options["instance_type"] = QJsonValue(toQString(m_awsSettings.workerInstanceType()));
      options["num"] = QJsonValue(static_cast<int>(m_awsSettings.numWorkers()));
      options["server_id"] = QJsonValue(toQString(m_awsSession.serverId()));
      options["server_procs"] = QJsonValue(static_cast<int>(m_awsSession.numServerProcessors()));
      options["timestamp"] = QJsonValue(toQString(m_awsSession.timestamp()));

      if (m_privateKey.open()) {
        m_privateKey.write(m_awsSession.privateKey().c_str());
        options["private_key"] = QJsonValue(m_privateKey.fileName());
        m_privateKey.close();
      }
      args << QString(QJsonDocument(options).toJson(QJsonDocument::Compact));
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckServerRunningProcess() const
    {
      auto p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckServerRunningComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("instance_status");
      
      QJsonObject options;
      options["instance_id"] = QJsonValue(toQString(m_awsSession.serverId()));
      args << QString(QJsonDocument(options).toJson(QJsonDocument::Compact));
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckWorkerRunningProcess() const
    {
      auto p = new QProcess();
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
      auto p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onInstancesStopped(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("terminate_session");
      
      QJsonObject options;
      QStringList workerIds;
      for (const std::string& workerId : m_awsSession.workerIds()) {
        workerIds.push_back(toQString(workerId));
      }
      options["server_id"] = QJsonValue(toQString(m_awsSession.serverId()));
      options["worker_ids"] = QJsonValue::fromVariant(workerIds);
      
      args << QString(QJsonDocument(options).toJson(QJsonDocument::Compact));
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckTerminateProcess() const
    {
      auto p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckTerminatedComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << QString("EC2");
      args << QString("termination_status");
      
      QJsonObject options;
      QStringList workerIds;
      for (const std::string& workerId : m_awsSession.workerIds()) {
        workerIds.push_back(toQString(workerId));
      }
      options["server_id"] = QJsonValue(toQString(m_awsSession.serverId()));
      options["worker_ids"] = QJsonValue::fromVariant(workerIds);
      
      args << QString(QJsonDocument(options).toJson(QJsonDocument::Compact));
      
      p->start(toQString(m_ruby), args);

      return p;
    }

    QProcess *AWSProvider_Impl::makeCheckEstimatedChargesProcess() const
    {
      auto p = new QProcess();
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
      auto p = new QProcess();
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
      if (t_results.output.isEmpty()) {
        logError("ServiceAvailable process failed to return output");
        return false;
      }
      
      QJsonParseError err;
      QJsonDocument json = QJsonDocument::fromJson(t_results.output.toUtf8(), &err);
      if (!err.error) {
        if (!json.object().contains("error")) return true;

        int code = json.object()["error"].toObject()["code"].toInt();
        return (code != 503);
      } else {
        logError("Error parsing serviceAvailable JSON: " + toString(err.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseValidateCredentialsResults(const ProcessResults &t_results)
    {
      if (t_results.output.isEmpty()) {
        logError("ValidateCredentials process failed to return output");
        return false;
      }
      
      QJsonParseError err;
      QJsonDocument json = QJsonDocument::fromJson(t_results.output.toUtf8(), &err);
      if (!err.error) {
        if (!json.object().contains("error")) return true;

        int code = json.object()["error"].toObject()["code"].toInt();
        if (code == 401) {
          logError("Invalid Access Key");
        } else if (code == 403) {
          logError("Invalid Secret Key");
        }
      } else {
        logError("Error parsing validateCredentials JSON: " + toString(err.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseResourcesAvailableToStartResults(const ProcessResults &t_results)
    {
      if (t_results.output.isEmpty()) {
        logError("ResourcesAvailableToStart process failed to return output");
        return false;
      }
      
      QJsonParseError err;
      QJsonDocument json = QJsonDocument::fromJson(t_results.output.toUtf8(), &err);
      if (!err.error) {
        if (!json.object().contains("error")) {
          m_lastTotalInstances = json.object()["instances"].toInt();
          if (m_awsSettings.numWorkers() + 1 + m_lastTotalInstances <= 20) return true;
        } else {
          logError(json.object()["error"].toObject()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing resourcesAvailableToStart JSON: " + toString(err.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseServerStartedResults(const ProcessResults &t_results)
    {
      if (t_results.output.isEmpty()) {
        logError("ServerStarted process failed to return output");
        return false;
      }
      
      QJsonParseError err;
      QJsonDocument json = QJsonDocument::fromJson(t_results.output.toUtf8(), &err);
      if (!err.error) {
        if (!json.object().contains("error")) {
          m_awsSession.setTimestamp(json.object()["timestamp"].toString().toStdString());
          m_awsSession.setPrivateKey(json.object()["private_key"].toString().toStdString());
          m_awsSession.setServerUrl(Url(json.object()["server"].toObject()["ip"].toString()));
          m_awsSession.setServerId(json.object()["server"].toObject()["id"].toString().toStdString());
          m_awsSession.setNumServerProcessors(json.object()["server"].toObject()["procs"].toInt());

          emit CloudProvider_Impl::serverStarted(Url(json.object()["server"].toObject()["ip"].toString()));

          return true;
        } else {
          logError(json.object()["error"].toObject()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing serverStarted JSON: " + toString(err.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseWorkerStartedResults(const ProcessResults &t_results)
    {
      m_privateKey.remove();
      if (t_results.output.isEmpty()) {
        logError("WorkerStarted process failed to return output");
        return false;
      }
      
      QJsonParseError err;
      QJsonDocument json = QJsonDocument::fromJson(t_results.output.toUtf8(), &err);
      if (!err.error) {
        if (!json.object().contains("error")) {
          unsigned numWorkerProcessors = 0;
          for (const QJsonValue& worker : json.object()["workers"].toArray()) {
            m_awsSession.addWorkerUrl(Url(worker.toObject()["ip"].toString()));
            m_awsSession.addWorkerId(worker.toObject()["id"].toString().toStdString());
            if (numWorkerProcessors == 0) numWorkerProcessors = worker.toObject()["procs"].toInt();

            emit CloudProvider_Impl::workerStarted(Url(worker.toObject()["ip"].toString()));
          }
          m_awsSession.setNumWorkerProcessors(numWorkerProcessors);

          emit CloudProvider_Impl::allWorkersStarted();
          
          return true;
        } else {
          logError(json.object()["error"].toObject()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing workerStarted JSON: " + toString(err.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseCheckServerRunningResults(const ProcessResults &t_results)
    {
      if (t_results.output.isEmpty()) {
        logError("CheckServerRunning process failed to return output");
        return false;
      }
      
      QJsonParseError err;
      QJsonDocument json = QJsonDocument::fromJson(t_results.output.toUtf8(), &err);
      if (!err.error) {
        if (!json.object().contains("error")) {
          std::string status = json.object()[toQString(m_awsSession.serverId())].toString().toStdString();
          if (status == "running") return true;
          logError("Server is not running (" + m_awsSession.serverId() + ": " + status + ")");
        } else {
          logError(json.object()["error"].toObject()["message"].toString().toStdString());
        }
      } else {
        logError("Error parsing checkServerRunning JSON: " + toString(err.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseCheckWorkerRunningResults(const ProcessResults &t_results)
    {
      if (t_results.output.isEmpty()) {
        logError("CheckWorkerRunning process failed to return output");
        return false;
      }
      
      QJsonParseError err;
      QJsonDocument json = QJsonDocument::fromJson(t_results.output.toUtf8(), &err);
      if (!err.error) {
        if (!json.object().contains("error")) {
          std::vector<std::string> running;
          std::vector<std::string> notRunning;
          for (const std::string& workerId : m_awsSession.workerIds()) {
            std::string status = json.object()[toQString(workerId)].toString().toStdString();
            if (status == "running") {
              running.push_back(workerId);
            } else {
              notRunning.push_back(workerId);
            }
          }
          if (running.size() == m_awsSession.workerIds().size()) return true;
          QString output = QString::number(notRunning.size()) + "/" + QString::number(m_awsSession.workerIds().size()) + " workers are not running (" + toQString(boost::algorithm::join(notRunning, ", ")) + ")";
          logError(output.toStdString());
        } else {
          logError(json.object()["error"].toObject()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing checkWorkerRunning JSON: " + toString(err.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseInstancesStoppedResults(const ProcessResults &t_results)
    {
      if (t_results.output.isEmpty()) {
        logError("InstancesStopped process failed to return output");
        return false;
      }
      
      QJsonParseError err;
      QJsonDocument json = QJsonDocument::fromJson(t_results.output.toUtf8(), &err);
      if (!err.error) {
        if (!json.object().contains("error")) {
          m_lastTerminateCompleted = true;

          emit CloudProvider_Impl::terminated();
          
          return true;
        } else {
          logError(json.object()["error"].toObject()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing instancesStopped JSON: " + toString(err.errorString()));
      }
      
      return false;
    }

    bool AWSProvider_Impl::parseCheckTerminatedResults(const ProcessResults &t_results)
    {
      if (t_results.output.isEmpty()) {
        logError("CheckTerminated process failed to return output");
        return false;
      }
      
      QJsonParseError err;
      QJsonDocument json = QJsonDocument::fromJson(t_results.output.toUtf8(), &err);
      if (!err.error) {
        if (!json.object().contains("error")) {
          if (json.object()["all_instances_terminated"].toBool()) {
            emit CloudProvider_Impl::terminated();
            
            return true;
          }
        } else {
          logError(json.object()["error"].toObject()["message"].toString().toStdString());
        } 
      } else {
        logError("Error parsing checkTerminated JSON: " + toString(err.errorString()));
      }
      
      return false;
    }

    double AWSProvider_Impl::parseCheckEstimatedChargesResults(const ProcessResults &t_results)
    {
      if (t_results.output.isEmpty()) {
        logError("CheckEstimatedCharges process failed to return output");
        return 0.0;
      }
      
      QJsonParseError err;
      QJsonDocument json = QJsonDocument::fromJson(t_results.output.toUtf8(), &err);
      if (!err.error) {
        if (!json.object().contains("error")) {
          return json.object()["estimated_charges"].toDouble();
        } else {
          QString message = json.object()["error"].toObject()["message"].toString();
          if (message == "InvalidClientTokenId") {
            logError("Invalid Access Key");
          } else if (message == "SignatureDoesNotMatch") {
            logError("Invalid Secret Key");
          } else {
            logError(message.toStdString());
          }
        } 
      } else {
        logError("Error parsing checkEstimatedCharges JSON: " + toString(err.errorString()));
      }
      
      return 0.0;
    }

    unsigned AWSProvider_Impl::parseCheckTotalInstancesResults(const ProcessResults &t_results)
    {
      if (t_results.output.isEmpty()) {
        logError("CheckTotalInstances process failed to return output");
        return 0;
      }
      
      QJsonParseError err;
      QJsonDocument json = QJsonDocument::fromJson(t_results.output.toUtf8(), &err);
      if (!err.error) {
        if (!json.object().contains("error")) {
          return json.object()["total_instances"].toInt();
        } else {
          int code = json.object()["error"].toObject()["code"].toInt();
          if (code == 401) {
            logError("Invalid Access Key");
          } else if (code == 403) {
            logError("Invalid Secret Key");
          } else {
            logError(json.object()["error"].toObject()["message"].toString().toStdString());
          }
        } 
      } else {
        logError("Error parsing checkTotalInstances JSON: " + toString(err.errorString()));
      }
      
      return 0;
    }

    void AWSProvider_Impl::onCheckInternetComplete(int, QProcess::ExitStatus)
    {
      m_lastInternetAvailable = parseServiceAvailableResults(handleProcessCompleted(m_checkInternetProcess));
      m_checkInternetProcess = nullptr;
    }

    void AWSProvider_Impl::onCheckServiceComplete(int, QProcess::ExitStatus)
    {
      m_lastServiceAvailable = parseServiceAvailableResults(handleProcessCompleted(m_checkServiceProcess));
      m_checkServiceProcess = nullptr;
    }

    void AWSProvider_Impl::onCheckValidateComplete(int, QProcess::ExitStatus)
    {
      m_lastValidateCredentials = parseValidateCredentialsResults(handleProcessCompleted(m_checkValidateProcess));
      m_checkValidateProcess = nullptr;
    }

    void AWSProvider_Impl::onCheckResourcesComplete(int, QProcess::ExitStatus)
    {
      m_lastResourcesAvailableToStart = parseResourcesAvailableToStartResults(handleProcessCompleted(m_checkResourcesProcess));
      m_checkResourcesProcess = nullptr;
    }
    
    void AWSProvider_Impl::onServerStarted(int, QProcess::ExitStatus)
    {
      m_serverStarted = parseServerStartedResults(handleProcessCompleted(m_startServerProcess));
      m_startServerProcess = nullptr;
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
      }else if (error == QProcess::UnknownError){
        LOG(Error, "An unknown error occurred. This is the default return value of error().");
      }

      m_serverStarted = false;
      m_startServerProcess->deleteLater();
      m_startServerProcess = nullptr;
    }

    void AWSProvider_Impl::onWorkerStarted(int, QProcess::ExitStatus)
    {
      m_workerStarted = parseWorkerStartedResults(handleProcessCompleted(m_startWorkerProcess));
      m_startWorkerProcess = nullptr;
    }

    void AWSProvider_Impl::onCheckServerRunningComplete(int, QProcess::ExitStatus)
    {
      m_lastServerRunning = parseCheckServerRunningResults(handleProcessCompleted(m_checkServerRunningProcess));
      m_checkServerRunningProcess = nullptr;
    }
    
    void AWSProvider_Impl::onCheckWorkerRunningComplete(int, QProcess::ExitStatus)
    {
      m_lastWorkerRunning = parseCheckWorkerRunningResults(handleProcessCompleted(m_checkWorkerRunningProcess));
      m_checkWorkerRunningProcess = nullptr;
    }
    
    void AWSProvider_Impl::onInstancesStopped(int, QProcess::ExitStatus)
    {
      m_instancesStopped = parseInstancesStoppedResults(handleProcessCompleted(m_stopInstancesProcess));
      m_stopInstancesProcess = nullptr;
    }

    void AWSProvider_Impl::onCheckTerminatedComplete(int, QProcess::ExitStatus)
    {
      m_lastTerminateCompleted = parseCheckTerminatedResults(handleProcessCompleted(m_checkTerminatedProcess));
      m_checkTerminatedProcess = nullptr;
    }

    void AWSProvider_Impl::onCheckEstimatedChargesComplete(int, QProcess::ExitStatus)
    {
      m_lastEstimatedCharges = parseCheckEstimatedChargesResults(handleProcessCompleted(m_checkEstimatedChargesProcess));
      emit estimatedChargesAvailable();
      m_checkEstimatedChargesProcess = nullptr;
    }

    void AWSProvider_Impl::onCheckTotalInstancesComplete(int, QProcess::ExitStatus)
    {
      m_lastTotalInstances = parseCheckTotalInstancesResults(handleProcessCompleted(m_checkTotalInstancesProcess));
      emit totalInstancesAvailable();
      m_checkTotalInstancesProcess = nullptr;
    }

    bool AWSProvider_Impl::userAgreementSigned() const {
      if (!m_awsSettings.userAgreementSigned()) {
        logError("The user agreement must be reviewed and signed before continuing");
        return false;
      }
      return true;
    }
    
    bool AWSProvider_Impl::authenticated() const {
      if (!m_awsSettings.validAccessKey()) {
        logError("Invalid Access Key");
        return false;
      } else if (!m_awsSettings.validSecretKey()) {
        logError("Invalid Secret Key");
        return false;
      } else if (!m_lastValidateCredentials) {
        logWarning("Credentials have not yet been determined valid");
      }
      return true;
    }

  } // detail

  AWSSettings::AWSSettings()
    : CloudSettings(std::shared_ptr<detail::AWSSettings_Impl>(
                      new detail::AWSSettings_Impl()))
  {
    OS_ASSERT(getImpl<detail::AWSSettings_Impl>());
  }

  AWSSettings::AWSSettings(const UUID& uuid,
                           const UUID& versionUUID,
                           bool userAgreementSigned,
                           unsigned numWorkers,
                           bool terminationDelayEnabled,
                           unsigned terminationDelay,
                           std::string region,
                           std::string serverInstanceType,
                           std::string workerInstanceType)
    : CloudSettings(std::shared_ptr<detail::AWSSettings_Impl>(
                      new detail::AWSSettings_Impl(uuid,
                                                   versionUUID,
                                                   userAgreementSigned,
                                                   numWorkers,
                                                   terminationDelayEnabled,
                                                   terminationDelay,
                                                   region,
                                                   serverInstanceType,
                                                   workerInstanceType)))
  {
    OS_ASSERT(getImpl<detail::AWSSettings_Impl>());
  }

  AWSSettings::AWSSettings(const std::shared_ptr<detail::AWSSettings_Impl>& impl)
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

  bool AWSSettings::validAccessKey() const {
    return getImpl<detail::AWSSettings_Impl>()->validAccessKey();
  }

  bool AWSSettings::validSecretKey(const std::string& secretKey) const {
    return getImpl<detail::AWSSettings_Impl>()->validSecretKey(secretKey);
  }

  bool AWSSettings::validSecretKey() const {
    return getImpl<detail::AWSSettings_Impl>()->validSecretKey();
  }

  void AWSSettings::clearKeys() {
    getImpl<detail::AWSSettings_Impl>()->clearKeys();
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
    : CloudSession(std::shared_ptr<detail::AWSSession_Impl>(
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
    : CloudSession(std::shared_ptr<detail::AWSSession_Impl>(
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

  AWSSession::AWSSession(const std::shared_ptr<detail::AWSSession_Impl>& impl)
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
    : CloudProvider(std::shared_ptr<detail::AWSProvider_Impl>(new detail::AWSProvider_Impl()))
  {
  }

  AWSProvider::AWSProvider(const std::shared_ptr<detail::AWSProvider_Impl>& impl)
    : CloudProvider(impl)
  {
    OS_ASSERT(getImpl<detail::AWSProvider_Impl>());
  }
  
  std::vector<std::string> AWSProvider::availableRegions() {
    return detail::AWSProvider_Impl::availableRegions();
  }

  std::string AWSProvider::defaultRegion() {
    return detail::AWSProvider_Impl::defaultRegion();
  }

  std::vector<std::string> AWSProvider::serverInstanceTypes() {
    return detail::AWSProvider_Impl::serverInstanceTypes();
  }

  std::string AWSProvider::defaultServerInstanceType() {
    return detail::AWSProvider_Impl::defaultServerInstanceType();
  }

  std::vector<std::string> AWSProvider::workerInstanceTypes() {
    return detail::AWSProvider_Impl::workerInstanceTypes();
  }

  std::string AWSProvider::defaultWorkerInstanceType() {
    return detail::AWSProvider_Impl::defaultWorkerInstanceType();
  }

  std::vector<unsigned> AWSProvider::serverProcessorCounts() {
    return detail::AWSProvider_Impl::serverProcessorCounts();
  }

  std::vector<unsigned> AWSProvider::workerProcessorCounts() {
    return detail::AWSProvider_Impl::workerProcessorCounts();
  }

  std::vector<std::string> AWSProvider::serverPrettyNames() {
    return detail::AWSProvider_Impl::serverPrettyNames();
  }

  std::vector<std::string> AWSProvider::workerPrettyNames() {
    return detail::AWSProvider_Impl::workerPrettyNames();
  }

  std::string AWSProvider::getServerPrettyName(const std::string & instanceType) {
    return detail::AWSProvider_Impl::getServerPrettyName(instanceType);
  }

  std::string AWSProvider::getWorkerPrettyName(const std::string & instanceType) {
    return detail::AWSProvider_Impl::getWorkerPrettyName(instanceType);
  }

  unsigned AWSProvider::getServerProcessorCount(const std::string & instanceType) {
    return detail::AWSProvider_Impl::getServerProcessorCount(instanceType);
  }

  unsigned AWSProvider::getWorkerProcessorCount(const std::string & instanceType) {
    return detail::AWSProvider_Impl::getWorkerProcessorCount(instanceType);
  }

  std::vector<AWSComputerInformation> AWSProvider::serverInformation() {
    return detail::AWSProvider_Impl::serverInformation();
  }

  std::vector<AWSComputerInformation> AWSProvider::workerInformation() {
    return detail::AWSProvider_Impl::workerInformation();
  }

  bool AWSProvider::requestEstimatedCharges() {
    return getImpl<detail::AWSProvider_Impl>()->requestEstimatedCharges();
  }

  bool AWSProvider::requestTotalInstances() {
    return getImpl<detail::AWSProvider_Impl>()->requestTotalInstances();
  }

  double AWSProvider::estimatedCharges(int msec) {
    return getImpl<detail::AWSProvider_Impl>()->estimatedCharges(msec);
  }

  unsigned AWSProvider::totalInstances(int msec) {
    return getImpl<detail::AWSProvider_Impl>()->totalInstances(msec);
  }

  double AWSProvider::lastEstimatedCharges() const {
    return getImpl<detail::AWSProvider_Impl>()->lastEstimatedCharges();
  }

  unsigned AWSProvider::lastTotalInstances() const {
    return getImpl<detail::AWSProvider_Impl>()->lastTotalInstances();
  }


} // openstudio
