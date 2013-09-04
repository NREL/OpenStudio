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

#include <QSettings>
#include <QProcess>

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
      : CloudSettings_Impl(), m_serverPath(serverPath), m_serverUrl(serverUrl), 
        m_workerPath(workerPath), m_workerUrl(workerUrl), m_haltOnStop(haltOnStop),
        m_username(username), m_password(password)
    {
    }

    VagrantSettings_Impl::~VagrantSettings_Impl()
    {
    }

    std::string VagrantSettings_Impl::cloudProviderType() const
    {
      return VagrantProvider_Impl::cloudProviderType();
    }

    bool VagrantSettings_Impl::loadSettings(bool overwriteExisting)
    {
      QSettings settings("OpenStudio", toQString(cloudProviderType()));

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

      if (overwriteExisting){
        if (settings.value("haltOnStop").toString() == "Yes"){
          m_haltOnStop = true;
        }else{
          m_haltOnStop = false;
        }
      }

      // DLM: i thought we wanted to write these to a file in ~/.ssh?
      if (overwriteExisting || m_username.empty()){
        m_username = settings.value("username").toString().toStdString();
      }

      if (overwriteExisting || m_password.empty()){
        m_password = settings.value("password").toString().toStdString();
      }

      // if succeeded we will have values for everything
      bool result = !(m_serverPath.empty() || m_serverUrl.isEmpty() || 
                      m_workerPath.empty() || m_workerUrl.isEmpty() ||
                      m_username.empty() || m_password.empty());
      return result;
    }

    bool VagrantSettings_Impl::saveToSettings(bool overwriteExisting) const
    {
      QSettings settings("OpenStudio", toQString(cloudProviderType()));

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

      // DLM: i thought we wanted to write these to a file in ~/.ssh?
      if (overwriteExisting || settings.value("username").isNull()){
        settings.setValue("username", toQString(m_username));
      }

      if (overwriteExisting || settings.value("password").isNull()){
        settings.setValue("password", toQString(m_password));
      }

      return true;
    }

    openstudio::path VagrantSettings_Impl::serverPath()const
    {
      return m_serverPath;
    }

    void VagrantSettings_Impl::setServerPath(const openstudio::path& serverPath)
    {
      m_serverPath = serverPath;
    }
 
    openstudio::Url VagrantSettings_Impl::serverUrl() const
    {
      return m_serverUrl;
    }

    void VagrantSettings_Impl::setServerUrl(const openstudio::Url& serverUrl)
    {
      m_serverUrl = serverUrl;
    }

    openstudio::path VagrantSettings_Impl::workerPath() const
    {
      return m_workerPath;
    }

    void VagrantSettings_Impl::setWorkerPath(const openstudio::path& workerPath)
    {
      m_workerPath = workerPath;
    }

    openstudio::Url VagrantSettings_Impl::workerUrl() const
    {
      return m_workerUrl;
    }

    void VagrantSettings_Impl::setWorkerUrl(const openstudio::Url& workerUrl)
    {
      m_workerUrl = workerUrl;
    }

    bool VagrantSettings_Impl::haltOnStop() const
    {
      return m_haltOnStop;
    }

    void VagrantSettings_Impl::setHaltOnStop(bool haltOnStop)
    {
      m_haltOnStop = haltOnStop;
    }

    std::string VagrantSettings_Impl::username() const
    {
      return m_username;
    }

    void VagrantSettings_Impl::setUsername(const std::string& username)
    {
      m_username = username;
    }

    std::string VagrantSettings_Impl::password() const
    {
      return m_password;
    }

    void VagrantSettings_Impl::setPassword(const std::string& password)
    {
      m_password = password;
    }


    VagrantSession_Impl::VagrantSession_Impl(const std::string& sessionId, const boost::optional<Url>& serverUrl, const std::vector<Url>& workerUrls)
      : CloudSession_Impl(sessionId, serverUrl, workerUrls)
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
        m_startServerProcess(NULL),
        m_startWorkerProcess(NULL),
        m_serverStarted(false),
        m_workerStarted(false),
        m_serverStopped(false),
        m_workerStopped(false),
        m_terminated(false)
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

    std::string VagrantProvider_Impl::userAgreementText() const
    {
      return "Do you agree?";
    }

    bool VagrantProvider_Impl::userAgreementSigned() const
    {
      QSettings settings("OpenStudio", toQString(cloudProviderType()));
      QString value = settings.value("userAgreementSigned", "No").toString();

      bool result = false;
      if (value == "Yes"){
        result = true;
      }

      return result;
    }

    void VagrantProvider_Impl::signUserAgreement(bool agree)
    {
      QString value;
      if (agree){
        value = "Yes";
      }else{
        value = "No";
      }
      QSettings settings("OpenStudio", toQString(cloudProviderType()));
      settings.setValue("userAgreementSigned", value);
    }

    bool VagrantProvider_Impl::internetAvailable() const
    {
      clearErrorsAndWarnings();
      return true;
    }

    bool VagrantProvider_Impl::serviceAvailable() const
    {
      clearErrorsAndWarnings();

      QProcess* vagrantProcess = new QProcess();

      QStringList args;
      addProcessArguments(args);
      args << "-v";

      vagrantProcess->setWorkingDirectory(toQString(m_vagrantSettings.serverPath()));
      vagrantProcess->start(processName(), args);

      if (!vagrantProcess->waitForStarted()){
        vagrantProcess->deleteLater();
        return false;
      }

      vagrantProcess->waitForFinished();

      int result = vagrantProcess->exitCode();

      vagrantProcess->deleteLater();

      if (result == 0){
        return true;
      }

      return false;
    }

    bool VagrantProvider_Impl::validateCredentials() const
    {
      clearErrorsAndWarnings();
      return true;
    }

    CloudSettings VagrantProvider_Impl::settings() const
    {
      return m_vagrantSettings;
    }

    bool VagrantProvider_Impl::setSettings(const CloudSettings& settings)
    {
      boost::optional<VagrantSettings> vagrantSettings = settings.optionalCast<VagrantSettings>();
      if (!vagrantSettings){
        return false;
      }
      if (m_serverStarted){
        return false;
      }
      if (m_workerStarted){
        return false;
      }
      if (m_terminated){
        return false;
      }
      
      m_vagrantSettings = *vagrantSettings;

      return true;
    }

    CloudSession VagrantProvider_Impl::session() const
    {
      return m_vagrantSession;
    }

    bool VagrantProvider_Impl::reconnect(const CloudSession& session)
    {
      boost::optional<VagrantSession> vagrantSession = session.optionalCast<VagrantSession>();
      if (!vagrantSession){
        return false;
      }
      if (m_serverStarted){
        return false;
      }
      if (m_workerStarted){
        return false;
      }
      if (m_terminated){
        return false;
      }

      clearErrorsAndWarnings();
      
      boost::optional<Url> serverUrl = session.serverUrl();
      if (serverUrl){
        OSServer server(*serverUrl);
        if (server.available()){
          m_vagrantSession = *vagrantSession;
          return true;
        }
      }

      return false;
    }

    /// returns the ip address of the cloud server if it is started and running
    boost::optional<Url> VagrantProvider_Impl::serverUrl() const
    {
      if (m_terminated){
        return boost::none;
      }
      return m_vagrantSession.serverUrl();
    }

    /// returns true if the cloud server successfully begins to start the server node
    /// returns false if terminated
    /// non-blocking call, clears errors and warnings
    bool VagrantProvider_Impl::startServer()
    {
      if (!userAgreementSigned()){
        return false;
      }
      if (m_serverStarted){
        return false;
      }
      if (m_terminated){
        return false;
      }
      if (m_startServerProcess){
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

      if (!m_startServerProcess->waitForStarted()){
        m_startServerProcess->deleteLater();
        m_startServerProcess = 0;
        return false;
      }

      emit serverStarting();

      return true;
    }

    /// returns the ip address of all cloud workers that are started and running
    std::vector<Url> VagrantProvider_Impl::workerUrls() const
    {
      if (m_terminated){
        return std::vector<Url>();
      }
      return m_vagrantSession.workerUrls();
    }

    unsigned VagrantProvider_Impl::numWorkers() const
    {
      return 1;
    }

    bool VagrantProvider_Impl::startWorkers()
    {
      if (!userAgreementSigned()){
        return false;
      }
      if (m_workerStarted){
        return false;
      }
      if (m_terminated){
        return false;
      }
      if (m_startWorkerProcess){
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

      if (!m_startWorkerProcess->waitForStarted()){
        m_startWorkerProcess->deleteLater();
        m_startWorkerProcess = 0;
        return false;
      }

      emit workerStarting();

      return true;
    }

    bool VagrantProvider_Impl::running() const
    {
      return ((m_serverStarted || m_workerStarted) && !m_terminated);
    }

    /// returns true if the cloud server successfully begins to stop all nodes
    /// returns false if not running
    /// non-blocking call, clears errors and warnings
    bool VagrantProvider_Impl::terminate()
    {
      if (!this->running()){
        return false;
      }

      m_terminated = true;

      clearErrorsAndWarnings();

      emit terminating();

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

        if (!m_stopServerProcess->waitForStarted()){
          m_stopServerProcess->deleteLater();
          m_stopServerProcess = 0;
          m_serverStopped = true;
          logError("Stop server process failed to start");
        }

        if (!m_stopWorkerProcess->waitForStarted()){
          m_stopWorkerProcess->deleteLater();
          m_stopWorkerProcess = 0;
          m_workerStopped = true;
          logError("Stop worker process failed to start");
        }

      }else{
        m_serverStopped = true;
        m_workerStopped = true;
      }

      if (this->is_terminateComplete()){
        emit terminateComplete();
      }

      return true;
    }

    bool VagrantProvider_Impl::terminated() const
    {
      return m_terminated;
    }

    std::vector<std::string> VagrantProvider_Impl::errors() const
    {
      return m_errors;
    }
    
    std::vector<std::string> VagrantProvider_Impl::warnings() const
    {
      return m_warnings;
    }

    std::string VagrantProvider_Impl::cloudProviderType()
    {
      return "VagrantProvider";
    }

    bool VagrantProvider_Impl::is_terminateComplete() const
    {
       return (m_serverStopped && m_workerStopped);
    }

    void VagrantProvider_Impl::onServerStarted(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_startServerProcess);

      QString output = m_startServerProcess->readAllStandardOutput();
      QString errors = m_startServerProcess->readAllStandardError();
      
      m_vagrantSession.setServerUrl(m_vagrantSettings.serverUrl());

      m_serverStarted = true;

      emit serverStarted(m_vagrantSettings.serverUrl());

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

      emit workerStarted(m_vagrantSettings.workerUrl());

      emit allWorkersStarted();

      m_startWorkerProcess->deleteLater();
      m_startWorkerProcess = 0;
    }

    void VagrantProvider_Impl::onServerStopped(int, QProcess::ExitStatus)
    {
      OS_ASSERT(m_stopServerProcess);

      QString output = m_stopServerProcess->readAllStandardOutput();
      QString errors = m_stopServerProcess->readAllStandardError();

      m_serverStopped = true;

      if (this->is_terminateComplete()){
        emit terminateComplete();
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

      if (this->is_terminateComplete()){
        emit terminateComplete();
      }

      m_stopWorkerProcess->deleteLater();
      m_stopWorkerProcess = 0;
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
      args << "vagrant.sh";
#endif
      return;
    }

  }// detail

  VagrantSettings::VagrantSettings()
    : CloudSettings(boost::shared_ptr<detail::VagrantSettings_Impl>(new detail::VagrantSettings_Impl()))
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

  VagrantSession::VagrantSession(const std::string& sessionId, const boost::optional<Url>& serverUrl, const std::vector<Url>& workerUrls)
    : CloudSession(boost::shared_ptr<detail::VagrantSession_Impl>(new detail::VagrantSession_Impl(sessionId, serverUrl, workerUrls)))
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

  bool VagrantProvider::terminateComplete() const
  {
     return getImpl<detail::VagrantProvider_Impl>()->is_terminateComplete();
  }


} // openstudio
