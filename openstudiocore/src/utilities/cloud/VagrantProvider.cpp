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
#include "VagrantProvider.hpp"
#include "VagrantProvider_Impl.hpp"
#include "OSServer.hpp"

#include "../core/Application.hpp"
#include "../core/Assert.hpp"
#include "../core/System.hpp"

#include <QSettings>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMutex>
#include <QFile>
#include <QDir>

namespace openstudio{
  namespace detail{
        
    VagrantSettings_Impl::VagrantSettings_Impl()
      : CloudSettings_Impl(),
        m_haltOnStop(true),
        m_terminationDelayEnabled(false),
        m_terminationDelay(0)
    {
      this->loadSettings(true);
    }

    VagrantSettings_Impl::VagrantSettings_Impl(const openstudio::path& serverPath, const openstudio::Url& serverUrl,
                                               const openstudio::path& workerPath, const openstudio::Url& workerUrl,
                                               bool haltOnStop, const std::string& username, const std::string& password,
                                               bool terminationDelayEnabled, unsigned terminationDelay)
      : CloudSettings_Impl(), m_userAgreementSigned(false),
        m_serverPath(serverPath), m_serverUrl(serverUrl), 
        m_workerPath(workerPath), m_workerUrl(workerUrl), m_haltOnStop(haltOnStop),
        m_username(username), m_password(password),
        m_terminationDelayEnabled(terminationDelayEnabled),
        m_terminationDelay(terminationDelay)
    {
      QSettings settings("OpenStudio", toQString(cloudProviderType()));
      if (settings.value("userAgreementSigned").toString() == "Yes"){
        m_userAgreementSigned = true;
      }
    }

    VagrantSettings_Impl::VagrantSettings_Impl(const UUID& uuid,
                                               const UUID& versionUUID,
                                               bool userAgreementSigned,
                                               const openstudio::path& serverPath,
                                               const openstudio::Url& serverUrl,
                                               const openstudio::path& workerPath,
                                               const openstudio::Url& workerUrl,
                                               bool haltOnStop,
                                               const std::string& username,
                                               bool terminationDelayEnabled, 
                                               unsigned terminationDelay)
      : CloudSettings_Impl(uuid,versionUUID),
        m_userAgreementSigned(userAgreementSigned),
        m_serverPath(serverPath),
        m_serverUrl(serverUrl),
        m_workerPath(workerPath),
        m_workerUrl(workerUrl),
        m_haltOnStop(haltOnStop),
        m_username(username),
        m_terminationDelayEnabled(terminationDelayEnabled),
        m_terminationDelay(terminationDelay)
    {}

    VagrantSettings_Impl::~VagrantSettings_Impl()
    {
    }

    std::string VagrantSettings_Impl::cloudProviderType() const
    {
      return VagrantProvider_Impl::cloudProviderType();
    }

    std::string VagrantSettings_Impl::userAgreementText() const
    {
      return "OpenStudio is provided by the National Renewable Energy Laboratory (\"NREL\"), which is operated by Alliance for Sustainable Energy, LLC, (\"Alliance\") for the U.S. Department of Energy (\"DOE\").  NREL, Alliance and DOE are not responsible for any charges incurred as a result of using OpenStudio in connection with web services, cloud services or any other third party computing services.  You are solely responsible for establishing, maintaining, terminating and paying for any third party computing services and are solely responsible for complying with any terms and conditions required by third party service providers.  Terminating OpenStudio may not terminate third party computing services used in connection with OpenStudio, and you are solely responsible for verifying that computing services and associated charges are terminated.\n\nTHE SOFTWARE IS PROVIDED BY DOE/NREL/ALLIANCE \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY DISCLAIMED.  IN NO EVENT SHALL DOE/NREL/ALLIANCE BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER, INCLUDING BUT NOT LIMITED TO CLAIMS ASSOCIATED WITH THE LOSS OF DATA OR PROFITS, WHICH MAY RESULT FROM ANY ACTION IN CONTRACT, NEGLIGENCE OR OTHER TORTIOUS CLAIM THAT ARISES OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE SOFTWARE. YOU AGREE TO INDEMNIFY DOE/NREL/ALLIANCE, AND ITS AFFILIATES, OFFICERS, AGENTS, AND EMPLOYEES AGAINST ANY CLAIM OR DEMAND, INCLUDING REASONABLE ATTORNEYS' FEES, RELATED TO YOUR USE, RELIANCE, OR ADOPTION OF THE SOFTWARE FOR ANY PURPOSE WHATSOEVER.";
    }

    bool VagrantSettings_Impl::userAgreementSigned() const
    {
      return m_userAgreementSigned;
    }

    void VagrantSettings_Impl::signUserAgreement(bool agree)
    {
      m_userAgreementSigned = agree;
      onChange();
    }

    bool VagrantSettings_Impl::loadSettings(bool overwriteExisting)
    {
      QSettings settings("OpenStudio", toQString(cloudProviderType()));

      // only set this if overwriteExisting is true
      if (overwriteExisting){
        if (settings.value("userAgreementSigned").toString() == "Yes"){
          m_userAgreementSigned = true;
        }else{
          m_userAgreementSigned = false;
        }
      }

      if (overwriteExisting || m_serverPath.empty()){
        m_serverPath = toPath(settings.value("serverPath").toString());
      }
            
      if (overwriteExisting || m_serverUrl.isEmpty()){
        m_serverUrl = settings.value("serverUrl").toString();
      }

      if (overwriteExisting || m_workerPath.empty()){
        m_workerPath = toPath(settings.value("workerPath").toString());
      }
            
      if (overwriteExisting || m_workerUrl.isEmpty()){
        m_workerUrl = settings.value("workerUrl").toString();
      }

      // only set this if overwriteExisting is true
      if (overwriteExisting){
        if (settings.value("haltOnStop").toString() == "Yes"){
          m_haltOnStop = true;
        }else{
          m_haltOnStop = false;
        }
      }

      if (overwriteExisting || m_username.empty()){
        m_username = settings.value("username").toString().toStdString();
      }

      if (overwriteExisting || m_password.empty()){
        QFile file(QDir::homePath() + "/.ssh/vagrant");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
          m_password = QString(file.readAll()).toStdString();
        }
      }

      // only set this if overwriteExisting is true
      if (overwriteExisting){
        if (settings.value("terminationDelayEnabled").toString() == "Yes"){
          m_terminationDelayEnabled = true;
        }else{
          m_terminationDelayEnabled = false;
        }
      }

      // only set this if overwriteExisting is true
      if (overwriteExisting){
        if (!settings.value("terminationDelay").isNull()){
          m_terminationDelay = settings.value("terminationDelay").toUInt();
        }
      }

      onChange();

      // if succeeded we will have values for everything
      bool result = !(m_serverPath.empty() || m_serverUrl.isEmpty() || 
                      m_workerPath.empty() || m_workerUrl.isEmpty() ||
                      m_username.empty() || m_password.empty());
      return result;
    }

    bool VagrantSettings_Impl::saveToSettings(bool overwriteExisting) const
    {
      QSettings settings("OpenStudio", toQString(cloudProviderType()));

      if (overwriteExisting || settings.value("userAgreementSigned").isNull()){
        if (m_userAgreementSigned){
          settings.setValue("userAgreementSigned", "Yes");
        }else{
          settings.setValue("userAgreementSigned", "No");
        }
      }

      if (overwriteExisting || settings.value("serverPath").isNull()){
        settings.setValue("serverPath", toQString(m_serverPath));
      }

      if (overwriteExisting || settings.value("serverUrl").isNull()){
        settings.setValue("serverUrl", m_serverUrl);
      }

      if (overwriteExisting || settings.value("workerPath").isNull()){
        settings.setValue("workerPath", toQString(m_workerPath));
      }

      if (overwriteExisting || settings.value("workerUrl").isNull()){
        settings.setValue("workerUrl", m_workerUrl);
      }

      if (overwriteExisting || settings.value("haltOnStop").isNull()){
        if (m_haltOnStop){
          settings.setValue("haltOnStop", "Yes");
        }else{
          settings.setValue("haltOnStop", "No");
        }
      }

      if (overwriteExisting || settings.value("username").isNull()){
        settings.setValue("username", toQString(m_username));
      }

      QFile file(QDir::homePath() + "/.ssh/vagrant");
      if (overwriteExisting || !file.exists()){
        if (QDir::home().exists(".ssh") || QDir::home().mkdir(".ssh")) {
          if (file.open(QIODevice::WriteOnly)) {
            file.write(m_password.c_str());
            file.close();
          }
        }
      }
      
      if (overwriteExisting || settings.value("terminationDelayEnabled").isNull()){
        if (m_terminationDelayEnabled){
          settings.setValue("terminationDelayEnabled", "Yes");
        }else{
          settings.setValue("terminationDelayEnabled", "No");
        }
      }

      if (overwriteExisting || settings.value("terminationDelay").isNull()){
        settings.setValue("terminationDelay", QString::number(m_terminationDelay));
      }
      
      return true;
    }

    openstudio::path VagrantSettings_Impl::serverPath() const
    {
      return m_serverPath;
    }

    void VagrantSettings_Impl::setServerPath(const openstudio::path& serverPath)
    {
      m_serverPath = serverPath;
      onChange();
    }
 
    openstudio::Url VagrantSettings_Impl::serverUrl() const
    {
      return m_serverUrl;
    }

    void VagrantSettings_Impl::setServerUrl(const openstudio::Url& serverUrl)
    {
      m_serverUrl = serverUrl;
      onChange();
    }

    openstudio::path VagrantSettings_Impl::workerPath() const
    {
      return m_workerPath;
    }

    void VagrantSettings_Impl::setWorkerPath(const openstudio::path& workerPath)
    {
      m_workerPath = workerPath;
      onChange();
    }

    openstudio::Url VagrantSettings_Impl::workerUrl() const
    {
      return m_workerUrl;
    }

    void VagrantSettings_Impl::setWorkerUrl(const openstudio::Url& workerUrl)
    {
      m_workerUrl = workerUrl;
      onChange();
    }

    bool VagrantSettings_Impl::haltOnStop() const
    {
      return m_haltOnStop;
    }

    void VagrantSettings_Impl::setHaltOnStop(bool haltOnStop)
    {
      m_haltOnStop = haltOnStop;
      onChange();
    }

    std::string VagrantSettings_Impl::username() const
    {
      return m_username;
    }

    void VagrantSettings_Impl::setUsername(const std::string& username)
    {
      m_username = username;
      onChange();
    }

    std::string VagrantSettings_Impl::password() const
    {
      return m_password;
    }

    void VagrantSettings_Impl::setPassword(const std::string& password)
    {
      m_password = password;
      // not stored in OSP, so no onChange()
    }

    bool VagrantSettings_Impl::terminationDelayEnabled()
    {
      return m_terminationDelayEnabled;
    }

    void VagrantSettings_Impl::setTerminationDelayEnabled(bool enabled)
    {
      m_terminationDelayEnabled = enabled;
      onChange();
    }

    unsigned VagrantSettings_Impl::terminationDelay()
    {
      return m_terminationDelay;
    }

    void VagrantSettings_Impl::setTerminationDelay(const unsigned delay)
    {
      m_terminationDelay = delay;
      onChange();
    }

    VagrantSession_Impl::VagrantSession_Impl(const std::string& sessionId, 
                                             const boost::optional<Url>& serverUrl, 
                                             const std::vector<Url>& workerUrls)
      : CloudSession_Impl(sessionId, serverUrl, workerUrls)
    {}

    VagrantSession_Impl::VagrantSession_Impl(const UUID& uuid,
                                             const UUID& versionUUID,
                                             const std::string& sessionId,
                                             const boost::optional<Url>& serverUrl,
                                             const std::vector<Url>& workerUrls)
      : CloudSession_Impl(uuid,versionUUID,sessionId,serverUrl,workerUrls)
    {}

    VagrantSession_Impl::~VagrantSession_Impl()
    {
    }

    std::string VagrantSession_Impl::cloudProviderType() const
    {
      return VagrantProvider_Impl::cloudProviderType();
    }

    VagrantProvider_Impl::VagrantProvider_Impl()
      : CloudProvider_Impl(),
        m_vagrantSettings(),
        m_vagrantSession(toString(createUUID()), boost::none, std::vector<Url>()),
        m_networkAccessManager(new QNetworkAccessManager()),
        m_networkReply(nullptr),
        m_checkServiceProcess(nullptr),
        m_startServerProcess(nullptr),
        m_startWorkerProcess(nullptr),
        m_checkServerRunningProcess(nullptr),
        m_checkWorkerRunningProcess(nullptr),
        m_stopServerProcess(nullptr),
        m_stopWorkerProcess(nullptr),
        m_checkTerminatedProcess(nullptr),
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
    }

    VagrantProvider_Impl::~VagrantProvider_Impl()
    {
    }

    std::string VagrantProvider_Impl::type() const
    {
      return VagrantProvider_Impl::cloudProviderType();
    }

    unsigned VagrantProvider_Impl::numWorkers() const
    {
      return 1;
    }

    CloudSettings VagrantProvider_Impl::settings() const
    {
      return m_vagrantSettings;
    }

    bool VagrantProvider_Impl::setSettings(const CloudSettings& settings)
    {
      clearErrorsAndWarnings();

      boost::optional<VagrantSettings> vagrantSettings = settings.optionalCast<VagrantSettings>();
      if (!vagrantSettings){
        // wrong type of settings
        return false;
      }
      if (m_serverStarted || m_startServerProcess){
        // can't change settings once server has been started or is starting
        return false;
      }
      if (m_workerStarted || m_startWorkerProcess){
        // can't change settings once worker has been started or is starting
        return false;
      }
      if (m_terminateStarted){
        // can't change settings once terminate is called
        return false;
      }

      m_vagrantSettings = *vagrantSettings;

      return true;
    }

    CloudSession VagrantProvider_Impl::session() const
    {
      return m_vagrantSession;
    }

    bool VagrantProvider_Impl::setSession(const CloudSession& session)
    {
      clearErrorsAndWarnings();

      boost::optional<VagrantSession> vagrantSession = session.optionalCast<VagrantSession>();
      if (!vagrantSession){
        // wrong type of session
        return false;
      }
      if (!vagrantSession->serverUrl()){
        // session to set should be a non-empty one
        return false;
      }
      if (vagrantSession->workerUrls().size() != 1){
        // session to set should be a non-empty one
        return false;
      }
      if (m_serverStarted || m_startServerProcess){
        // can't change session once server has been started or is starting
        return false;
      }
      if (m_workerStarted || m_startWorkerProcess){
        // can't change session once worker has been started or is starting
        return false;
      }
      if (m_terminateStarted){
        // can't change session once terminate has been called
        return false;
      }

      m_vagrantSession = *vagrantSession;

      // assumes that the other session is already started
      m_serverStarted = true;
      m_workerStarted = true;

      // should we emit a signal here?

      return true;
    }

    bool VagrantProvider_Impl::lastInternetAvailable() const
    {
      return m_lastInternetAvailable;
    }

    bool VagrantProvider_Impl::lastServiceAvailable() const
    {
      return m_lastServiceAvailable;
    }

    bool VagrantProvider_Impl::lastValidateCredentials() const
    {
      return m_lastValidateCredentials;
    }

    bool VagrantProvider_Impl::lastResourcesAvailableToStart() const
    {
      return m_lastResourcesAvailableToStart;
    }

    bool VagrantProvider_Impl::serverStarted() const
    {
      return m_serverStarted;
    }

    bool VagrantProvider_Impl::workersStarted() const
    {
      return m_workerStarted;
    }

    bool VagrantProvider_Impl::lastServerRunning() const
    {
      return m_lastServerRunning;
    }

    bool VagrantProvider_Impl::lastWorkersRunning() const
    {
      return m_lastWorkerRunning;
    }

    bool VagrantProvider_Impl::terminateStarted() const
    {
      return m_terminateStarted;
    }

    bool VagrantProvider_Impl::lastTerminateCompleted() const
    {
      return m_lastTerminateCompleted;
    }

    std::vector<std::string> VagrantProvider_Impl::errors() const
    {
      return m_errors;
    }
    
    std::vector<std::string> VagrantProvider_Impl::warnings() const
    {
      return m_warnings;
    }

    bool VagrantProvider_Impl::internetAvailable(int msec)
    {
      if (requestInternetAvailable()){
        if (waitForFinished(msec, std::bind(&VagrantProvider_Impl::requestInternetAvailableRequestFinished, this))){
          return lastInternetAvailable();
        }
      }
      if (m_networkReply){
        m_networkReply->blockSignals(true);
        m_networkReply->deleteLater();
        m_networkReply = nullptr;
      }
      return false;
    }

    bool VagrantProvider_Impl::serviceAvailable(int msec)
    {
      if (requestServiceAvailable()){
        if (waitForFinished(msec, std::bind(&VagrantProvider_Impl::requestServiceAvailableFinished, this))){
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

    bool VagrantProvider_Impl::validateCredentials(int msec)
    {
      if (requestValidateCredentials()){
        if (waitForFinished(msec, std::bind(&VagrantProvider_Impl::requestValidateCredentialsFinished, this))){
          return lastValidateCredentials();
        }
      }
      // nothing to clean up
      return false;
    }

    bool VagrantProvider_Impl::resourcesAvailableToStart(int msec)
    {
      if (requestResourcesAvailableToStart()){
        if (waitForFinished(msec, std::bind(&VagrantProvider_Impl::requestResourcesAvailableToStartFinished, this))){
          return lastResourcesAvailableToStart();
        }
      }
      // nothing to clean up
      return false;
    }

    bool VagrantProvider_Impl::waitForServer(int msec)
    {
      if (waitForFinished(msec, std::bind(&VagrantProvider_Impl::serverStarted, this))){
        return m_serverStarted;
      }
      if (m_startServerProcess){
        m_startServerProcess->disconnect(this, nullptr);
        m_startServerProcess->kill();
        m_startServerProcess = nullptr;
      }
      return false;
    }

    bool VagrantProvider_Impl::waitForWorkers(int msec) 
    {
      if (waitForFinished(msec, std::bind(&VagrantProvider_Impl::workersStarted, this))){
        return m_workerStarted;
      }
      if (m_startWorkerProcess){
        m_startWorkerProcess->disconnect(this, nullptr);
        m_startWorkerProcess->kill();
        m_startWorkerProcess = nullptr;
      }
      return false;
    }

    bool VagrantProvider_Impl::serverRunning(int msec)
    {
      if (requestServerRunning()){
        if (waitForFinished(msec, std::bind(&VagrantProvider_Impl::requestServerRunningFinished, this))){
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

    bool VagrantProvider_Impl::workersRunning(int msec)
    {
      if (requestWorkersRunning()){
        if (waitForFinished(msec, std::bind(&VagrantProvider_Impl::requestWorkersRunningFinished, this))){
          return lastWorkersRunning();
        }
      }
      if (m_checkWorkerRunningProcess){
        m_checkServerRunningProcess->disconnect(this, nullptr);
        m_checkWorkerRunningProcess->kill();
        m_checkWorkerRunningProcess = nullptr;
      }
      return false;
    }

    bool VagrantProvider_Impl::waitForTerminated(int msec)
    {
      if (waitForFinished(msec, std::bind(&VagrantProvider_Impl::requestTerminateFinished, this))){
        return (m_serverStopped && m_workerStopped);
      }
      if (m_stopServerProcess){
        m_stopServerProcess->disconnect(this, nullptr);
        m_stopServerProcess->kill();
        m_stopServerProcess = nullptr;
      }
      if (m_stopWorkerProcess){
        m_stopWorkerProcess->disconnect(this, nullptr);
        m_stopWorkerProcess->kill();
        m_stopWorkerProcess = nullptr;
      }
      return false;
    }

    bool VagrantProvider_Impl::terminateCompleted(int msec)
    {
      if (requestTerminateCompleted()){
        if (waitForFinished(msec, std::bind(&VagrantProvider_Impl::requestTerminateCompletedFinished, this))){
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

    bool VagrantProvider_Impl::requestInternetAvailable()
    {
      if (m_networkReply){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_lastInternetAvailable = false;

      QUrl url("http://openstudio.nrel.gov"); // todo: url?
      QNetworkRequest request(url);
      m_networkReply = m_networkAccessManager->get(request);

      bool test = QObject::connect(m_networkReply, SIGNAL(finished()), this, SLOT(processInternetAvailable()));
      OS_ASSERT(test);

      return true;
    }

    QProcess *VagrantProvider_Impl::makeCheckServiceProcess() const
    {
      auto p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckServiceComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);
      QStringList args;
      addProcessArguments(args);
      args << "-v";

      p->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      p->start(processName(), args);

      return p;
    }

    bool VagrantProvider_Impl::requestServiceAvailable()
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

    bool VagrantProvider_Impl::requestValidateCredentials() 
    {
      // DLM: this would be a non-blocking network call, taking shortcut here
      // could use a QTimer to simulate if we really want to

      clearErrorsAndWarnings();

      m_lastValidateCredentials = false;

      if ((m_vagrantSettings.username() == "vagrant") &&
          (m_vagrantSettings.password() == "vagrant")){
        m_lastValidateCredentials = true;
      }

      return true;
    }

    bool VagrantProvider_Impl::requestResourcesAvailableToStart() 
    {
      // DLM: this would be a non-blocking network call, taking shortcut here
      // could use a QTimer to simulate if we really want to

      clearErrorsAndWarnings();

      m_lastResourcesAvailableToStart = true;

      return true;
    }

    QProcess *VagrantProvider_Impl::makeStartServerProcess() const
    {
      auto p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onServerStarted(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "up";

      p->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      p->start(processName(), args);

      return p;
    }

    bool VagrantProvider_Impl::requestStartServer()
    {
      if (!m_vagrantSettings.userAgreementSigned()){
        // haven't signed agreement
        return false;
      }
      if (m_serverStarted || m_startServerProcess){
        // already started or starting
        return false;
      }
      if (m_terminateStarted){
        // already terminated or terminating
        return false;
      }

      clearErrorsAndWarnings();

      m_startServerProcess = makeStartServerProcess();
      emit CloudProvider_Impl::serverStarting();

      return true;
    }

    QProcess *VagrantProvider_Impl::makeStartWorkerProcess() const
    {
      auto p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onWorkerStarted(int, QProcess::ExitStatus)));
      OS_ASSERT(test);


      QStringList args;
      addProcessArguments(args);
      args << "up";

      p->setWorkingDirectory(toQString(m_vagrantSettings.workerPath()));
      p->start(processName(), args);

      return p;
    }

    bool VagrantProvider_Impl::requestStartWorkers()
    {
      if (!m_vagrantSettings.userAgreementSigned()){
        // haven't signed agreement
        return false;
      }
      if (m_workerStarted || m_startWorkerProcess){
        // already started or starting
        return false;
      }
      if (m_terminateStarted){
        // already terminated or terminating
        return false;
      }

      clearErrorsAndWarnings();

      m_startWorkerProcess = makeStartWorkerProcess();
      emit CloudProvider_Impl::workerStarting();

      return true;
    }

    QProcess *VagrantProvider_Impl::makeCheckServerRunningProcess() const
    {
      auto p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckServerRunningComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "status";
      args << "default";

      p->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      p->start(processName(), args);

      return p;
    }

    bool VagrantProvider_Impl::requestServerRunning() 
    {
      if (m_checkServerRunningProcess){
        // already checking
        return false;
      }
      if (!m_vagrantSession.serverUrl()){
        // need this to check in real life
        return false;
      }

      clearErrorsAndWarnings();

      m_lastServerRunning = false;

      m_checkServerRunningProcess = makeCheckServerRunningProcess();
      return true;
    }

    QProcess *VagrantProvider_Impl::makeCheckWorkerRunningProcess() const
    {
      auto p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckWorkerRunningComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "status";
      args << "default";

      p->setWorkingDirectory(toQString(m_vagrantSettings.workerPath()));
      p->start(processName(), args);
      return p;
    }

    bool VagrantProvider_Impl::requestWorkersRunning() 
    {
      if (m_checkWorkerRunningProcess){
        // already checking
        return false;
      }
      if (m_vagrantSession.workerUrls().empty()){
        // need this to check in real life
        return false;
      }

      clearErrorsAndWarnings();

      m_lastWorkerRunning = false;

      m_checkWorkerRunningProcess = makeCheckWorkerRunningProcess();

      return true;
    }

    QProcess *VagrantProvider_Impl::makeStopServerProcess() const
    {
      auto p = new QProcess();
      p->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
          this, SLOT(onServerStopped(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "halt";
      p->start(processName(), args);
      return p;
    }

    QProcess *VagrantProvider_Impl::makeStopWorkerProcess() const
    {
      auto p = new QProcess();
      p->setWorkingDirectory(toQString(m_vagrantSettings.workerPath()));
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
          this, SLOT(onWorkerStopped(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "halt";
      p->start(processName(), args);
      return p;
    }

    bool VagrantProvider_Impl::requestTerminate()
    {
      if (!(m_serverStarted || m_workerStarted)){
        // haven't started, nothing to terminate
        return false;
      }
      if (m_terminateStarted || m_stopServerProcess || m_stopWorkerProcess){
        // already terminating
        return false;
      }

      m_terminateStarted = true;

      clearErrorsAndWarnings();

      emit CloudProvider_Impl::terminating();

      if (m_vagrantSettings.haltOnStop()){

        m_stopServerProcess = makeStopServerProcess();

      }else{
      
        m_serverStopped = true;
        m_workerStopped = true;
        
        emit CloudProvider_Impl::terminated();
      }

      return true;
    }

    QProcess *VagrantProvider_Impl::makeCheckTerminateProcess() const
    {
      auto p = new QProcess();
      bool test = connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckTerminatedComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "status";
      args << "default";

      p->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      p->start(processName(), args);
      return p;
    }

    bool VagrantProvider_Impl::requestTerminateCompleted() 
    {
      if (m_checkTerminatedProcess){
        // already checking
        return false;
      }

      // DLM: this should really check both server and worker, taking a shortcut here

      clearErrorsAndWarnings();

      m_lastTerminateCompleted = false;

      m_checkTerminatedProcess = makeCheckTerminateProcess();

      return true;
    }

    std::string VagrantProvider_Impl::cloudProviderType()
    {
      return "VagrantProvider";
    }

    void VagrantProvider_Impl::processInternetAvailable()
    {
      if (m_networkReply->error() == QNetworkReply::NoError){
        m_lastInternetAvailable = true;
      }

      m_networkReply->deleteLater();
      m_networkReply = nullptr;
    }


    ProcessResults VagrantProvider_Impl::handleProcessCompleted(QProcess * t_qp)
    {
      OS_ASSERT(t_qp);

      ProcessResults pr(t_qp->exitCode(), t_qp->exitStatus(), t_qp->readAllStandardOutput(),
          t_qp->readAllStandardError());

      t_qp->deleteLater();

      return pr;
    }

    bool VagrantProvider_Impl::parseServiceAvailableResults(const ProcessResults &t_results)
    {
      return t_results.exitCode == 0;
    }

    void VagrantProvider_Impl::onCheckServiceComplete(int, QProcess::ExitStatus)
    {
      m_lastServiceAvailable = parseServiceAvailableResults(handleProcessCompleted(m_checkServiceProcess));
      m_checkServiceProcess = nullptr;
    }

    bool VagrantProvider_Impl::parseServerStartedResults(const ProcessResults &t_results)
    {
      m_vagrantSession.setServerUrl(m_vagrantSettings.serverUrl());
      return true;
    }

    void VagrantProvider_Impl::onServerStarted(int, QProcess::ExitStatus)
    {
      m_serverStarted = parseServerStartedResults(handleProcessCompleted(m_startServerProcess));
      m_startServerProcess = nullptr;

      if (m_serverStarted)
      {
        emit CloudProvider_Impl::serverStarted(m_vagrantSettings.serverUrl());
      }
    }

    bool VagrantProvider_Impl::parseWorkerStartedResults(const ProcessResults &t_results)
    {
      m_vagrantSession.clearWorkerUrls();
      m_vagrantSession.addWorkerUrl(m_vagrantSettings.workerUrl());
      return true;
    }

    void VagrantProvider_Impl::onWorkerStarted(int, QProcess::ExitStatus)
    {
      m_workerStarted = parseWorkerStartedResults(handleProcessCompleted(m_startWorkerProcess));
      m_startWorkerProcess = nullptr;

      if (m_workerStarted)
      {
        emit CloudProvider_Impl::workerStarted(m_vagrantSettings.workerUrl());
        emit CloudProvider_Impl::allWorkersStarted();
      }
    }

    bool VagrantProvider_Impl::parseCheckServerRunningResults(const ProcessResults &t_results)
    {
      // if running this is expected
      //default                   running (virtualbox)
      return t_results.output.contains("running (virtualbox)");
    }

    void VagrantProvider_Impl::onCheckServerRunningComplete(int, QProcess::ExitStatus)
    {
      bool running = parseCheckServerRunningResults(handleProcessCompleted(m_checkServerRunningProcess));
      m_checkServerRunningProcess = nullptr;

      if (m_vagrantSettings.haltOnStop()){
        if (running) {
          m_lastServerRunning = true;
        }
      }else{
        // depend on local state variable in this case
        m_lastServerRunning = !m_serverStopped;
      }
    }

    bool VagrantProvider_Impl::parseCheckWorkerRunningResults(const ProcessResults &t_results)
    {
      // if running this is expected
      //default                   running (virtualbox)
      return t_results.output.contains("running (virtualbox)");
    }

    void VagrantProvider_Impl::onCheckWorkerRunningComplete(int, QProcess::ExitStatus)
    {
      bool running = parseCheckWorkerRunningResults(handleProcessCompleted(m_checkWorkerRunningProcess));
      m_checkWorkerRunningProcess = nullptr;

      if (m_vagrantSettings.haltOnStop()){
        if (running){
          m_lastWorkerRunning = true;
        }
      }else{
        // depend on local state variable in this case
        m_lastWorkerRunning = !m_workerStopped;
      }
    }

    bool VagrantProvider_Impl::parseServerStoppedResults(const ProcessResults &t_results)
    {
      return true;
    }

    void VagrantProvider_Impl::onServerStopped(int, QProcess::ExitStatus)
    {
      m_serverStopped = parseServerStoppedResults(handleProcessCompleted(m_stopServerProcess));
      m_stopServerProcess = nullptr;
      
      m_stopWorkerProcess = makeStopWorkerProcess();
    }

    bool VagrantProvider_Impl::parseWorkerStoppedResults(const ProcessResults &t_results)
    {
      return true;
    }

    void VagrantProvider_Impl::onWorkerStopped(int, QProcess::ExitStatus)
    {
      m_workerStopped = parseWorkerStoppedResults(handleProcessCompleted(m_stopWorkerProcess));
      m_stopWorkerProcess = nullptr;

      if (m_serverStopped && m_workerStopped){
        emit CloudProvider_Impl::terminated();
      }
    }

    bool VagrantProvider_Impl::parseCheckTerminatedResults(const ProcessResults &t_results)
    {
      // if halt on stop this is expected:
      // default                   poweroff (virtualbox)
      return t_results.output.contains("poweroff (virtualbox)");
    }

    void VagrantProvider_Impl::onCheckTerminatedComplete(int, QProcess::ExitStatus)
    {
      // note, it's important that this function is always called, to clean up the QProcess object
      bool terminated = parseCheckTerminatedResults(handleProcessCompleted(m_checkTerminatedProcess));
      m_checkTerminatedProcess = nullptr;

      if (m_vagrantSettings.haltOnStop()){
        if (terminated) {
          // depend on local state variable for the worker status
          if (m_workerStopped){
            m_lastTerminateCompleted = true;
          }
        }
      }else{
        // depend on local state variable in this case
        m_lastTerminateCompleted = m_terminateStarted;
      }

    }

    void VagrantProvider_Impl::clearErrorsAndWarnings() const
    {
      m_errors.clear();
      m_warnings.clear();
    }

    void VagrantProvider_Impl::logError(const std::string& error) const
    {
      LOG(Error, error);
      m_errors.push_back(error);
    }

    void VagrantProvider_Impl::logWarning(const std::string& warning) const
    {
      LOG(Warn, warning);
      m_warnings.push_back(warning);
    }

    QString VagrantProvider_Impl::processName() const
    {
      QString result;
#ifdef _WINDOWS
      result = "cmd.exe";
#else
      result = "sh";
#endif
      return result;
    }

    void VagrantProvider_Impl::addProcessArguments(QStringList& args) const
    {
#ifdef _WINDOWS
      args << "/C";
      args << "vagrant.bat";
#else
      args << "vagrant";
#endif
      return;
    }

    bool VagrantProvider_Impl::waitForFinished(int msec, const std::function<bool ()>& f)
    {
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

    bool VagrantProvider_Impl::requestInternetAvailableRequestFinished() const
    {
      return (m_networkReply == nullptr);
    }

    bool VagrantProvider_Impl::requestServiceAvailableFinished() const
    {
      return (m_checkServiceProcess == nullptr);
    }

    bool VagrantProvider_Impl::requestValidateCredentialsFinished() const
    {
      return true;
    }

    bool VagrantProvider_Impl::requestResourcesAvailableToStartFinished() const
    {
      return true;
    }

    bool VagrantProvider_Impl::requestServerRunningFinished() const
    {
      return (m_checkServerRunningProcess == nullptr);
    }

    bool VagrantProvider_Impl::requestWorkersRunningFinished() const
    {
      return (m_checkWorkerRunningProcess == nullptr);
    }

    bool VagrantProvider_Impl::requestTerminateFinished() const
    {
      return (m_serverStopped && m_workerStopped);
    }

    bool VagrantProvider_Impl::requestTerminateCompletedFinished() const
    {
      return (m_checkTerminatedProcess == nullptr);
    }

  }// detail

  VagrantSettings::VagrantSettings()
    : CloudSettings(std::shared_ptr<detail::VagrantSettings_Impl>(new detail::VagrantSettings_Impl()))
  {
    OS_ASSERT(getImpl<detail::VagrantSettings_Impl>());
  }

  VagrantSettings::VagrantSettings(const UUID& uuid,
                                   const UUID& versionUUID,
                                   bool userAgreementSigned,
                                   const openstudio::path& serverPath,
                                   const openstudio::Url& serverUrl,
                                   const openstudio::path& workerPath,
                                   const openstudio::Url& workerUrl,
                                   bool haltOnStop,
                                   const std::string& username, 
                                   bool terminationDelayEnabled, 
                                   unsigned terminationDelay)
    : CloudSettings(std::shared_ptr<detail::VagrantSettings_Impl>(
                        new detail::VagrantSettings_Impl(uuid,
                                                         versionUUID,
                                                         userAgreementSigned,
                                                         serverPath,
                                                         serverUrl,
                                                         workerPath,
                                                         workerUrl,
                                                         haltOnStop,
                                                         username, 
                                                         terminationDelayEnabled, 
                                                         terminationDelay)))
  {
    OS_ASSERT(getImpl<detail::VagrantSettings_Impl>());
  }

  VagrantSettings::VagrantSettings(const std::shared_ptr<detail::VagrantSettings_Impl>& impl)
    : CloudSettings(impl)
  {
    OS_ASSERT(getImpl<detail::VagrantSettings_Impl>());
  }

  VagrantSettings::~VagrantSettings()
  {
  }

  openstudio::path VagrantSettings::serverPath() const
  {
    return getImpl<detail::VagrantSettings_Impl>()->serverPath();
  }

  void VagrantSettings::setServerPath(const openstudio::path& serverPath)
  {
    getImpl<detail::VagrantSettings_Impl>()->setServerPath(serverPath);
  }

  openstudio::Url VagrantSettings::serverUrl() const
  {
    return getImpl<detail::VagrantSettings_Impl>()->serverUrl();
  }

  void VagrantSettings::setServerUrl(const openstudio::Url& serverUrl)
  {
    getImpl<detail::VagrantSettings_Impl>()->setServerUrl(serverUrl);
  }

  openstudio::path VagrantSettings::workerPath() const
  {
    return getImpl<detail::VagrantSettings_Impl>()->workerPath();
  }

  void VagrantSettings::setWorkerPath(const openstudio::path& workerPath)
  {
    getImpl<detail::VagrantSettings_Impl>()->setWorkerPath(workerPath);
  }

  openstudio::Url VagrantSettings::workerUrl() const
  {
    return getImpl<detail::VagrantSettings_Impl>()->workerUrl();
  }

  void VagrantSettings::setWorkerUrl(const openstudio::Url& workerUrl)
  {
    getImpl<detail::VagrantSettings_Impl>()->setWorkerUrl(workerUrl);
  }

  bool VagrantSettings::haltOnStop() const
  {
    return getImpl<detail::VagrantSettings_Impl>()->haltOnStop();
  }

  void VagrantSettings::setHaltOnStop(bool haltOnStop)
  {
    getImpl<detail::VagrantSettings_Impl>()->setHaltOnStop(haltOnStop);
  }

  std::string VagrantSettings::username() const
  {
    return getImpl<detail::VagrantSettings_Impl>()->username();
  }

  void VagrantSettings::setUsername(const std::string& username)
  {
    getImpl<detail::VagrantSettings_Impl>()->setUsername(username);
  }

  std::string VagrantSettings::password() const
  {
    return getImpl<detail::VagrantSettings_Impl>()->password();
  }

  void VagrantSettings::setPassword(const std::string& password)
  {
    getImpl<detail::VagrantSettings_Impl>()->setPassword(password);
  }

  bool VagrantSettings::terminationDelayEnabled()
  {
    return getImpl<detail::VagrantSettings_Impl>()->terminationDelayEnabled();
  }

  void VagrantSettings::setTerminationDelayEnabled(bool enabled)
  {
    getImpl<detail::VagrantSettings_Impl>()->setTerminationDelayEnabled(enabled);
  }

  unsigned VagrantSettings::terminationDelay()
  {
    return getImpl<detail::VagrantSettings_Impl>()->terminationDelay();
  }

  void VagrantSettings::setTerminationDelay(const unsigned delay)
  {
    getImpl<detail::VagrantSettings_Impl>()->setTerminationDelay(delay);
  }

  VagrantSession::VagrantSession(const std::string& sessionId, 
                                 const boost::optional<Url>& serverUrl, 
                                 const std::vector<Url>& workerUrls)
    : CloudSession(std::shared_ptr<detail::VagrantSession_Impl>(
                       new detail::VagrantSession_Impl(sessionId, 
                                                       serverUrl, 
                                                       workerUrls)))
  {
    OS_ASSERT(getImpl<detail::VagrantSession_Impl>());
  }

  VagrantSession::VagrantSession(const UUID& uuid,
                                 const UUID& versionUUID,
                                 const std::string& sessionId, 
                                 const boost::optional<Url>& serverUrl, 
                                 const std::vector<Url>& workerUrls)
    : CloudSession(std::shared_ptr<detail::VagrantSession_Impl>(
                       new detail::VagrantSession_Impl(uuid,
                                                       versionUUID,
                                                       sessionId, 
                                                       serverUrl, 
                                                       workerUrls)))
  {
    OS_ASSERT(getImpl<detail::VagrantSession_Impl>());
  }

  VagrantSession::VagrantSession(const std::shared_ptr<detail::VagrantSession_Impl>& impl)
    : CloudSession(impl)
  {
    OS_ASSERT(getImpl<detail::VagrantSession_Impl>());
  }

  VagrantSession::~VagrantSession()
  {
  }

  VagrantProvider::VagrantProvider()
    : CloudProvider(std::shared_ptr<detail::VagrantProvider_Impl>(new detail::VagrantProvider_Impl()))
  {
    OS_ASSERT(getImpl<detail::VagrantProvider_Impl>());
  }

  VagrantProvider::VagrantProvider(const std::shared_ptr<detail::VagrantProvider_Impl>& impl)
    : CloudProvider(impl)
  {
    OS_ASSERT(getImpl<detail::VagrantProvider_Impl>());
  }

  VagrantProvider::~VagrantProvider()
  {
  }


} // openstudio
