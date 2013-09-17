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
#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>
#include <utilities/cloud/OSServer.hpp>

#include <utilities/core/Application.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/System.hpp>

#include <QSettings>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMutex>

namespace openstudio{
  namespace detail{
        
    VagrantSettings_Impl::VagrantSettings_Impl()
      : CloudSettings_Impl()
    {
      this->loadSettings(true);
    }

    VagrantSettings_Impl::VagrantSettings_Impl(const openstudio::path& serverPath, const openstudio::Url& serverUrl,
                                               const openstudio::path& workerPath, const openstudio::Url& workerUrl,
                                               bool haltOnStop, const std::string& username, const std::string& password)
      : CloudSettings_Impl(), m_userAgreementSigned(false),
        m_serverPath(serverPath), m_serverUrl(serverUrl), 
        m_workerPath(workerPath), m_workerUrl(workerUrl), m_haltOnStop(haltOnStop),
        m_username(username), m_password(password)
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
                                               const std::string& username)
      : CloudSettings_Impl(uuid,versionUUID),
        m_userAgreementSigned(userAgreementSigned),
        m_serverPath(serverPath),
        m_serverUrl(serverUrl),
        m_workerPath(workerPath),
        m_workerUrl(workerUrl),
        m_haltOnStop(haltOnStop),
        m_username(username)
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

      // TODO: read password from a file in ~/.ssh
      if (overwriteExisting || m_password.empty()){
        m_password = settings.value("password").toString().toStdString();
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

      // TODO: write password to a file in ~/.ssh
      if (overwriteExisting || settings.value("password").isNull()){
        settings.setValue("password", toQString(m_password));
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
        m_networkReply(0),
        m_checkServiceProcess(0),
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
        boost::function1<bool, VagrantProvider_Impl*> f = &VagrantProvider_Impl::requestInternetAvailableRequestFinished;
        if (waitForFinished(msec, f)){
          return lastInternetAvailable();
        }
      }
      return false;
    }

    bool VagrantProvider_Impl::serviceAvailable(int msec)
    {
      if (requestServiceAvailable()){
        boost::function1<bool, VagrantProvider_Impl*> f = &VagrantProvider_Impl::requestServiceAvailableFinished;
        if (waitForFinished(msec, f)){
          return lastServiceAvailable();
        }
      }
      return false;
    }

    bool VagrantProvider_Impl::validateCredentials(int msec)
    {
      if (requestValidateCredentials()){
        boost::function1<bool, VagrantProvider_Impl*> f = &VagrantProvider_Impl::requestValidateCredentialsFinished;
        if (waitForFinished(msec, f)){
          return lastValidateCredentials();
        }
      }
      return false;
    }

    bool VagrantProvider_Impl::resourcesAvailableToStart(int msec)
    {
      if (requestResourcesAvailableToStart()){
        boost::function1<bool, VagrantProvider_Impl*> f = &VagrantProvider_Impl::requestResourcesAvailableToStartFinished;
        if (waitForFinished(msec, f)){
          return lastResourcesAvailableToStart();
        }
      }
      return false;
    }

    bool VagrantProvider_Impl::waitForServer(int msec)
    {
      boost::function1<bool, VagrantProvider_Impl*> f = &VagrantProvider_Impl::serverStarted;
      return waitForFinished(msec, f);
    }

    bool VagrantProvider_Impl::waitForWorkers(int msec) 
    {
      boost::function1<bool, VagrantProvider_Impl*> f = &VagrantProvider_Impl::workersStarted;
      return waitForFinished(msec, f);
    }

    bool VagrantProvider_Impl::serverRunning(int msec)
    {
      if (requestServerRunning()){
        boost::function1<bool, VagrantProvider_Impl*> f = &VagrantProvider_Impl::requestServerRunningFinished;
        if (waitForFinished(msec, f)){
          return lastServerRunning();
        }
      }
      return false;
    }

    bool VagrantProvider_Impl::workersRunning(int msec)
    {
      if (requestWorkersRunning()){
        boost::function1<bool, VagrantProvider_Impl*> f = &VagrantProvider_Impl::requestWorkersRunningFinished;
        if (waitForFinished(msec, f)){
          return lastWorkersRunning();
        }
      }
      return false;
    }

    bool VagrantProvider_Impl::waitForTerminated(int msec)
    {
      boost::function1<bool, VagrantProvider_Impl*> f = &VagrantProvider_Impl::requestTerminateFinished;
      return waitForFinished(msec, f);
    }

    bool VagrantProvider_Impl::terminateCompleted(int msec)
    {
      if (requestTerminateCompleted()){
        boost::function1<bool, VagrantProvider_Impl*> f = &VagrantProvider_Impl::requestTerminateCompletedFinished;
        if (waitForFinished(msec, f)){
          return lastTerminateCompleted();
        }
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

    bool VagrantProvider_Impl::requestServiceAvailable()
    {
      if (m_checkServiceProcess){
        // already checking
        return false;
      }

      clearErrorsAndWarnings();

      m_lastServiceAvailable = false;

      m_checkServiceProcess = new QProcess();
      bool test = connect(m_checkServiceProcess, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckServiceComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "-v";

      m_checkServiceProcess->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      m_checkServiceProcess->start(processName(), args);

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

      m_startServerProcess = new QProcess();
      bool test = connect(m_startServerProcess, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onServerStarted(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "up";

      m_startServerProcess->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      m_startServerProcess->start(processName(), args);

      emit CloudProvider_Impl::serverStarting();

      return true;
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

      m_startWorkerProcess = new QProcess();
      bool test = connect(m_startWorkerProcess, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onWorkerStarted(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "up";

      m_startWorkerProcess->setWorkingDirectory(toQString(m_vagrantSettings.workerPath()));
      m_startWorkerProcess->start(processName(), args);

      emit CloudProvider_Impl::workerStarting();

      return true;
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

      m_checkServerRunningProcess = new QProcess();
      bool test = connect(m_checkServerRunningProcess, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckServerRunningComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "status";
      args << "default";

      m_checkServerRunningProcess->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      m_checkServerRunningProcess->start(processName(), args);

      return true;
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

      m_checkWorkerRunningProcess = new QProcess();
      bool test = connect(m_checkWorkerRunningProcess, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckWorkerRunningComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "status";
      args << "default";

      m_checkWorkerRunningProcess->setWorkingDirectory(toQString(m_vagrantSettings.workerPath()));
      m_checkWorkerRunningProcess->start(processName(), args);

      return true;
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

        QStringList args;
        addProcessArguments(args);
        args << "halt";

        m_stopServerProcess = new QProcess();
        m_stopServerProcess->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
        bool test = connect(m_stopServerProcess, SIGNAL(finished(int, QProcess::ExitStatus)), 
                            this, SLOT(onServerStopped(int, QProcess::ExitStatus)));
        OS_ASSERT(test);

        m_stopWorkerProcess = new QProcess();
        m_stopWorkerProcess->setWorkingDirectory(toQString(m_vagrantSettings.workerPath()));
        test = connect(m_stopWorkerProcess, SIGNAL(finished(int, QProcess::ExitStatus)), 
                       this, SLOT(onWorkerStopped(int, QProcess::ExitStatus)));
        OS_ASSERT(test);

        m_stopServerProcess->start(processName(), args);
        m_stopWorkerProcess->start(processName(), args);

      }else{
        m_serverStopped = true;
        m_workerStopped = true;
      }

      if (m_serverStopped && m_workerStopped){
        emit CloudProvider_Impl::terminated();
      }

      return true;
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

      m_checkTerminatedProcess = new QProcess();
      bool test = connect(m_checkTerminatedProcess, SIGNAL(finished(int, QProcess::ExitStatus)), 
                          this, SLOT(onCheckTerminatedComplete(int, QProcess::ExitStatus)));
      OS_ASSERT(test);

      QStringList args;
      addProcessArguments(args);
      args << "status";
      args << "default";

      m_checkTerminatedProcess->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      m_checkTerminatedProcess->start(processName(), args);

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
      m_networkReply = 0;
    }

    void VagrantProvider_Impl::onCheckServiceComplete(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_checkServiceProcess);

      int result = m_checkServiceProcess->exitCode();
      if (result == 0){
        m_lastServiceAvailable = true;
      }

      m_checkServiceProcess->deleteLater();
      m_checkServiceProcess = 0;
    }

    void VagrantProvider_Impl::onServerStarted(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_startServerProcess);

      QString output = m_startServerProcess->readAllStandardOutput();
      QString errors = m_startServerProcess->readAllStandardError();
      
      m_vagrantSession.setServerUrl(m_vagrantSettings.serverUrl());

      m_serverStarted = true;

      emit CloudProvider_Impl::serverStarted(m_vagrantSettings.serverUrl());

      m_startServerProcess->deleteLater();
      m_startServerProcess = 0;
    }

    void VagrantProvider_Impl::onWorkerStarted(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_startWorkerProcess);

      QString output = m_startWorkerProcess->readAllStandardOutput();
      QString errors = m_startWorkerProcess->readAllStandardError();
      
      m_vagrantSession.clearWorkerUrls();
      m_vagrantSession.addWorkerUrl(m_vagrantSettings.workerUrl());

      m_workerStarted = true;

      emit CloudProvider_Impl::workerStarted(m_vagrantSettings.workerUrl());

      emit CloudProvider_Impl::allWorkersStarted();

      m_startWorkerProcess->deleteLater();
      m_startWorkerProcess = 0;
    }

    void VagrantProvider_Impl::onCheckServerRunningComplete(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_checkServerRunningProcess);

      QString output = m_checkServerRunningProcess->readAllStandardOutput();
      QString errors = m_checkServerRunningProcess->readAllStandardError();

      if (m_vagrantSettings.haltOnStop()){
        // if running this is expected
        //default                   running (virtualbox)
        if (output.contains("running (virtualbox)")){
          m_lastServerRunning = true;
        }
      }else{
        // depend on local state variable in this case
        m_lastServerRunning = !m_serverStopped;
      }

      m_checkServerRunningProcess->deleteLater();
      m_checkServerRunningProcess = 0;
    }

    void VagrantProvider_Impl::onCheckWorkerRunningComplete(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_checkWorkerRunningProcess);

      QString output = m_checkWorkerRunningProcess->readAllStandardOutput();
      QString errors = m_checkWorkerRunningProcess->readAllStandardError();

      if (m_vagrantSettings.haltOnStop()){
        // if running this is expected
        // default                   running (virtualbox)
        if (output.contains("running (virtualbox)")){
          m_lastWorkerRunning = true;
        }
      }else{
        // depend on local state variable in this case
        m_lastWorkerRunning = !m_workerStopped;
      }

      m_checkWorkerRunningProcess->deleteLater();
      m_checkWorkerRunningProcess = 0;
    }

    void VagrantProvider_Impl::onServerStopped(int, QProcess::ExitStatus)
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

    void VagrantProvider_Impl::onWorkerStopped(int, QProcess::ExitStatus)
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

    void VagrantProvider_Impl::onCheckTerminatedComplete(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_checkTerminatedProcess);

      QString output = m_checkTerminatedProcess->readAllStandardOutput();
      QString errors = m_checkTerminatedProcess->readAllStandardError();

      // if halt on stop this is expected:
      // default                   poweroff (virtualbox)
      if (m_vagrantSettings.haltOnStop()){
        if (output.contains("poweroff (virtualbox)")){
          // depend on local state variable for the worker status
          if (m_workerStopped){
            m_lastTerminateCompleted = true;
          }
        }
      }else{
        // depend on local state variable in this case
        m_lastTerminateCompleted = m_terminateStarted;
      }

      m_checkTerminatedProcess->deleteLater();
      m_checkTerminatedProcess = 0;
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

    bool VagrantProvider_Impl::waitForFinished(int msec, const boost::function1<bool, VagrantProvider_Impl*>& f)
    {
      int msecPerLoop = 20;
      int numTries = msec / msecPerLoop;
      int current = 0;
      while (true)
      {
        if (f(this)){
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
      return (m_networkReply == 0);
    }

    bool VagrantProvider_Impl::requestServiceAvailableFinished() const
    {
      return (m_checkServiceProcess == 0);
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
      return (m_checkServerRunningProcess == 0);
    }

    bool VagrantProvider_Impl::requestWorkersRunningFinished() const
    {
      return (m_checkWorkerRunningProcess == 0);
    }

    bool VagrantProvider_Impl::requestTerminateFinished() const
    {
      return (m_serverStopped && m_workerStopped);
    }

    bool VagrantProvider_Impl::requestTerminateCompletedFinished() const
    {
      return (m_checkTerminatedProcess == 0);
    }

  }// detail

  VagrantSettings::VagrantSettings()
    : CloudSettings(boost::shared_ptr<detail::VagrantSettings_Impl>(new detail::VagrantSettings_Impl()))
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
                                   const std::string& username)
    : CloudSettings(boost::shared_ptr<detail::VagrantSettings_Impl>(
                        new detail::VagrantSettings_Impl(uuid,
                                                         versionUUID,
                                                         userAgreementSigned,
                                                         serverPath,
                                                         serverUrl,
                                                         workerPath,
                                                         workerUrl,
                                                         haltOnStop,
                                                         username)))
  {
    OS_ASSERT(getImpl<detail::VagrantSettings_Impl>());
  }

  VagrantSettings::VagrantSettings(const boost::shared_ptr<detail::VagrantSettings_Impl>& impl)
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

  VagrantSession::VagrantSession(const std::string& sessionId, 
                                 const boost::optional<Url>& serverUrl, 
                                 const std::vector<Url>& workerUrls)
    : CloudSession(boost::shared_ptr<detail::VagrantSession_Impl>(
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
    : CloudSession(boost::shared_ptr<detail::VagrantSession_Impl>(
                       new detail::VagrantSession_Impl(uuid,
                                                       versionUUID,
                                                       sessionId, 
                                                       serverUrl, 
                                                       workerUrls)))
  {
    OS_ASSERT(getImpl<detail::VagrantSession_Impl>());
  }

  VagrantSession::VagrantSession(const boost::shared_ptr<detail::VagrantSession_Impl>& impl)
    : CloudSession(impl)
  {
    OS_ASSERT(getImpl<detail::VagrantSession_Impl>());
  }

  VagrantSession::~VagrantSession()
  {
  }

  VagrantProvider::VagrantProvider()
    : CloudProvider(boost::shared_ptr<detail::VagrantProvider_Impl>(new detail::VagrantProvider_Impl()))
  {
    OS_ASSERT(getImpl<detail::VagrantProvider_Impl>());
  }

  VagrantProvider::VagrantProvider(const boost::shared_ptr<detail::VagrantProvider_Impl>& impl)
    : CloudProvider(impl)
  {
    OS_ASSERT(getImpl<detail::VagrantProvider_Impl>());
  }

  VagrantProvider::~VagrantProvider()
  {
  }


} // openstudio
