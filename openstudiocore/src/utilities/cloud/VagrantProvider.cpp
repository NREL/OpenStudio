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

    VagrantProvider_Impl::VagrantProvider_Impl(const openstudio::path& serverPath, const openstudio::Url& serverUrl,
                                               const openstudio::path& workerPath, const openstudio::Url& workerUrl)
      : CloudProvider_Impl(), m_cloudSession(this->type(), toString(createUUID()), boost::none, std::vector<Url>()),
        m_serverPath(serverPath), m_serverUrl(serverUrl), m_workerPath(workerPath), m_workerUrl(workerUrl),
        m_startServerProcess(NULL), m_startWorkerProcess(NULL), 
        m_serverStarted(false), m_workersStarted(false), m_terminated(false)
    {
      //Make sure a QApplication exists
      openstudio::Application::instance().application();
    }

    VagrantProvider_Impl::~VagrantProvider_Impl()
    {
    }

    std::string VagrantProvider_Impl::type() const
    {
      return "VagrantProvider";
    }

    std::string VagrantProvider_Impl::userAgreementText() const
    {
      return "Do you agree?";
    }

    bool VagrantProvider_Impl::userAgreementSigned() const
    {
      QSettings settings("OpenStudio", "VagrantProvider");
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
      QSettings settings("OpenStudio", "VagrantProvider");
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
      args << "help";

      vagrantProcess->setWorkingDirectory(toQString(m_serverPath));
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

    CloudSession VagrantProvider_Impl::session() const
    {
      return m_cloudSession;
    }

    bool VagrantProvider_Impl::reconnect(const CloudSession& session)
    {
      if (m_serverStarted){
        return false;
      }
      if (m_workersStarted){
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
          m_cloudSession = session;
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
      return m_cloudSession.serverUrl();
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

      m_startServerProcess->setWorkingDirectory(toQString(m_serverPath));
      m_startServerProcess->start(processName(), args);

      if (!m_startServerProcess->waitForStarted()){
        m_startServerProcess->deleteLater();
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
      return m_cloudSession.workerUrls();
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
      if (m_workersStarted){
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

      m_startWorkerProcess->setWorkingDirectory(toQString(m_workerPath));
      m_startWorkerProcess->start(processName(), args);

      if (!m_startWorkerProcess->waitForStarted()){
        m_startWorkerProcess->deleteLater();
        return false;
      }

      emit workerStarting();

      return true;
    }

    bool VagrantProvider_Impl::running() const
    {
      return ((m_serverStarted || m_workersStarted) && !m_terminated);
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

      QStringList args;
      addProcessArguments(args);
      args << "halt";

      QProcess* stopServerProcess = new QProcess();
      stopServerProcess->setWorkingDirectory(toQString(m_serverPath));
      stopServerProcess->start(processName(), args);

      QProcess* stopWorkerProcess = new QProcess();
      stopWorkerProcess->setWorkingDirectory(toQString(m_workerPath));
      stopWorkerProcess->start(processName(), args);

      emit terminating();

      if (stopServerProcess->waitForStarted()){
        stopServerProcess->waitForFinished();
      }
      if (stopWorkerProcess->waitForStarted()){
        stopWorkerProcess->waitForFinished();
      }
      
      stopServerProcess->deleteLater();
      stopWorkerProcess->deleteLater();

      emit terminateComplete();

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

    void VagrantProvider_Impl::clearErrorsAndWarnings() const
    {
      m_errors.clear();
      m_warnings.clear();
    }

    void VagrantProvider_Impl::onServerStarted(int, QProcess::ExitStatus)
    {
      if (m_startServerProcess){

        QString output = m_startServerProcess->readAllStandardOutput();
        QString errors = m_startServerProcess->readAllStandardError();
        
        m_cloudSession.setServerUrl(m_serverUrl);

        m_serverStarted = true;

        emit serverStarted(m_serverUrl);

        m_startServerProcess->deleteLater();
      }
    }

    void VagrantProvider_Impl::onWorkerStarted(int, QProcess::ExitStatus)
    {
      if (m_startWorkerProcess){

        QString output = m_startWorkerProcess->readAllStandardOutput();
        QString errors = m_startWorkerProcess->readAllStandardError();
        
        m_cloudSession.clearWorkerUrls();
        m_cloudSession.addWorkerUrl(m_workerUrl);

        m_workersStarted = true;

        emit workerStarted(m_workerUrl);

        emit allWorkersStarted();

        m_startWorkerProcess->deleteLater();
      }
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

  VagrantProvider::VagrantProvider(const openstudio::path& serverPath, const openstudio::Url& serverUrl,
                                   const openstudio::path& workerPath, const openstudio::Url& workerUrl)
    : CloudProvider(boost::shared_ptr<detail::VagrantProvider_Impl>(new detail::VagrantProvider_Impl(serverPath, serverUrl, workerPath, workerUrl)))
  {
  }

  VagrantProvider::~VagrantProvider()
  {
  }


} // openstudio
