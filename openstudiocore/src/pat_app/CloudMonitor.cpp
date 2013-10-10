/**********************************************************************
 *  Copyright (c) 2008-2012, Alliance for Sustainable Energy.
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

#include "CloudMonitor.hpp"

#include "PatApp.hpp"
#include "PatMainWindow.hpp"
#include "RunTabController.hpp"
#include "RunView.hpp"

#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/cloud/CloudProvider_Impl.hpp>
#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>
#include <utilities/cloud/AWSProvider.hpp>
#include <utilities/cloud/AWSProvider_Impl.hpp>
#include <utilities/cloud/OSServer.hpp>
#include <utilities/cloud/OSServer_Impl.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/System.hpp>

#include "../shared_gui_components/LostCloudConnectionDialog.hpp"

#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QRegExp>
#include <QStringList>
#include <QThread>
#include <QTimer>

namespace openstudio {

namespace pat {

namespace detail {

void stopCloud()
{
  boost::optional<CloudProvider> provider;

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession();
  boost::optional<CloudSettings> settings = CloudMonitor::currentProjectSettings();

  // If there is already a session, try to connect to that
  if( session && settings ) 
  {
    provider = CloudMonitor::newCloudProvider(settings.get(),session);
  }

  if( provider->requestTerminate() )
  {
    provider->waitForTerminated();
  }
}

// Return true if the current project session is running
// ie all workers and server reporting running.
bool reconnectToCloudSession()
{
  bool result = false;

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession(); 
  boost::optional<CloudSettings> settings = CloudMonitor::currentProjectSettings();

  // If there is already a session, try to connect to that
  if( session && settings ) 
  {
    boost::optional<CloudProvider> provider = CloudMonitor::newCloudProvider(settings.get(),session);

    OS_ASSERT(provider);

    if( provider->serverRunning() && provider->workersRunning() )
    {
      result = true;
    }
  }

  return result;
}

} // namespace detail

CloudMonitor::CloudMonitor()
  : QObject(),
    m_serverStarted(false),
    m_allWorkersStarted(false)
{

  setStatus(CLOUD_STOPPED);

  // CloudMonitorWorker, used to report if something goes wrong
  m_workerThread = QSharedPointer<QThread>(new QThread());
  m_worker = QSharedPointer<CloudMonitorWorker>(new CloudMonitorWorker(this));
  m_worker->moveToThread(m_workerThread.data());

  bool bingo;
  bingo = connect(m_worker.data(),SIGNAL(cloudConnectionError()),
                  this,SLOT(onCloudConnectionError()));
  OS_ASSERT(bingo);

  // StartCloudWorker
  m_startCloudWorker = QSharedPointer<StartCloudWorker>(new StartCloudWorker(this));
  bingo = connect(m_startCloudWorker.data(),SIGNAL(doneWorking()),this,SLOT(onStartCloudWorkerComplete()));
  OS_ASSERT(bingo);

  // StopCloudWorker
  m_stopCloudWorker = QSharedPointer<StopCloudWorker>(new StopCloudWorker(this));
  bingo = connect(m_stopCloudWorker.data(),SIGNAL(doneWorking()),this,SLOT(onStopCloudWorkerComplete()));
  OS_ASSERT(bingo);

  // ReconnectCloudWorker
  m_reconnectCloudWorker = QSharedPointer<ReconnectCloudWorker>(new ReconnectCloudWorker(this));
  bingo = connect(m_reconnectCloudWorker.data(),SIGNAL(doneWorking()),this,SLOT(onReconnectCloudWorkerComplete()));
  OS_ASSERT(bingo);

  // RecoverCloudWorker
  m_recoverCloudWorker = QSharedPointer<RecoverCloudWorker>(new RecoverCloudWorker(this));
  bingo = connect(m_recoverCloudWorker.data(),SIGNAL(doneWorking()),this,SLOT(onRecoverCloudWorkerComplete()));
  OS_ASSERT(bingo);

  // Start the CloudMonitorWorker thread
  m_workerThread->start();
}      

CloudMonitor::~CloudMonitor()
{
  m_workerThread->quit();
  m_workerThread->wait();
}

void CloudMonitor::toggleCloud()
{
  if( m_status == CLOUD_STOPPED )
  {
    startCloud();
  }
  else if( m_status == CLOUD_RUNNING )
  {
    stopCloud();
  }
  else if( m_status == CLOUD_ERROR )
  {
    recoverCloud();
  }
}

void CloudMonitor::startCloud()
{
  if( m_status == CLOUD_STOPPED )
  {
    bool preCheckPassed = true;

    CloudSettings settings = PatApp::instance()->cloudSettings();

    if( boost::optional<AWSSettings> awsSettings = settings.optionalCast<AWSSettings>() )
    {
      if( ! awsSettings->validAccessKey() || 
          ! awsSettings->validSecretKey() || 
          ! awsSettings->userAgreementSigned() )
      {
        QString error("Missing or invalid cloud settings.  Verify settings from Cloud menu.");
        
        QMessageBox::critical(PatApp::instance()->mainWindow, "Cloud Settings", error);

        preCheckPassed = false;
      }
    }

    if( preCheckPassed )
    {
      setStatus(CLOUD_STARTING);
      
      m_startCloudThread = QSharedPointer<QThread>(new QThread());

      m_startCloudWorker->moveToThread(m_startCloudThread.data());

      m_startCloudThread->start();

      m_startCloudWorker->startWorking();
    }
  }
}

void CloudMonitor::onStartCloudWorkerComplete()
{
  if( ! m_startCloudWorker->internetAvailable() )
  {
    setStatus(CLOUD_ERROR);
    
    QString error("Unable to connect to internet.  Please verify your network settings and try again.");

    QMessageBox::critical(PatApp::instance()->mainWindow, "Cloud Error", error);

    setStatus(CLOUD_STOPPED);
  }
  else if( ! m_startCloudWorker->validCredentials() )
  {
    setStatus(CLOUD_ERROR);
    
    QString error("Invalid cloud credentials.  Verify account settings from Cloud menu.");

    QMessageBox::critical(PatApp::instance()->mainWindow, "Cloud Settings", error);

    setStatus(CLOUD_STOPPED);
  }
  else if( ! m_startCloudWorker->resourcesAvailableToStart() )
  {
    setStatus(CLOUD_ERROR);
    
    QString error("Insufficient cloud resources.  Reduce the number of workers requested or allow current cloud resources to complete.");

    QMessageBox::critical(PatApp::instance()->mainWindow, "Cloud Settings", error);

    setStatus(CLOUD_STOPPED);
  }
  else if( m_startCloudWorker->error() )
  {
    setStatus(CLOUD_ERROR);

    QString error("Unknown error starting cloud.");
    
    QMessageBox::critical(PatApp::instance()->mainWindow, "Cloud Error", error);

    setStatus(CLOUD_STOPPED);
  }
  else
  {
    setCurrentProjectSession(m_startCloudWorker->session());

    setCurrentProjectSettings(m_startCloudWorker->settings());

    m_startCloudThread->quit();

    m_startCloudThread->wait();

    m_startCloudThread.clear();

    setStatus(CLOUD_RUNNING);
  }
}

void CloudMonitor::stopCloud()
{
  if( m_status == CLOUD_RUNNING ||
      m_status == CLOUD_ERROR )
  {
    setStatus(CLOUD_STOPPING);

    m_stopCloudThread = QSharedPointer<QThread>(new QThread());

    m_stopCloudWorker->moveToThread(m_stopCloudThread.data());

    m_stopCloudThread->start();

    m_stopCloudWorker->startWorking();
  }
}

void CloudMonitor::onStopCloudWorkerComplete()
{
  setCurrentProjectSession(boost::none);
  setCurrentProjectSettings(boost::none);

  m_stopCloudThread->quit();

  m_stopCloudThread->wait();

  m_stopCloudThread.clear();

  setStatus(CLOUD_STOPPED);
}

void CloudMonitor::reconnectCloud()
{
  boost::optional<CloudSettings> settings = currentProjectSettings();

  boost::optional<CloudSession> session = currentProjectSession();

  if( session && settings && (m_status == CLOUD_STOPPED) )
  {
    setStatus(CLOUD_STARTING); 

    m_reconnectCloudThread = QSharedPointer<QThread>(new QThread());

    m_reconnectCloudWorker->moveToThread(m_reconnectCloudThread.data());

    m_reconnectCloudThread->start();

    m_reconnectCloudWorker->startWorking();
  }
}

void CloudMonitor::onReconnectCloudWorkerComplete()
{
  m_reconnectCloudThread->quit();

  m_reconnectCloudThread->wait();

  m_reconnectCloudThread.clear();

  if( m_reconnectCloudWorker->status() == CLOUD_RUNNING )
  {
    setStatus(CLOUD_RUNNING);
  }
  else
  {
    setStatus(CLOUD_STOPPED);
  }

  // TODO Handle other states
}

void CloudMonitor::onCloudConnectionError()
{
  setStatus(CLOUD_ERROR);

  LostCloudConnectionDialog dialog(m_worker->internetAvailable(),
                                   m_worker->authenticated(),
                                   m_worker->cloudRunning());

  dialog.exec();

  if( dialog.clearCloudSession() )
  {
    stopCloud();
  }
}

void CloudMonitor::recoverCloud()
{
  setStatus(CLOUD_STARTING);

  m_recoverCloudThread = QSharedPointer<QThread>(new QThread());

  m_recoverCloudWorker->moveToThread(m_recoverCloudThread.data());

  m_recoverCloudThread->start();

  m_recoverCloudWorker->startWorking();
}

void CloudMonitor::onRecoverCloudWorkerComplete()
{
  m_recoverCloudThread->quit();

  m_recoverCloudThread->wait();

  m_recoverCloudThread.clear();

  if( m_recoverCloudWorker->status() == CLOUD_RUNNING )
  {
    setStatus(CLOUD_RUNNING);
  }
  else
  {
    setCurrentProjectSettings(boost::none);
    setCurrentProjectSession(boost::none);

    setStatus(CLOUD_STOPPED);
  }
}

boost::optional<CloudSession> CloudMonitor::currentProjectSession()
{
  boost::optional<CloudSession> session;

  if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() )
  {
    session = project->cloudSession();
  }

  return session;
}

void CloudMonitor::setCurrentProjectSession(const boost::optional<CloudSession> & session)
{
  if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() )
  {
    if( ! session )
    {
      project->clearCloudAnalysisDriver();
      project->clearCloudSession();
    }
    else
    {
      project->clearCloudAnalysisDriver();
      project->setCloudSession(session.get());
    }

    project->save();
  }
}

boost::optional<CloudSettings> CloudMonitor::currentProjectSettings()
{
  boost::optional<CloudSettings> settings;

  if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() )
  {
    settings = project->cloudSettings();
  }

  return settings;
}

void CloudMonitor::setCurrentProjectSettings(const boost::optional<CloudSettings> & settings)
{
  if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() )
  {
    if( ! settings )
    {
      project->clearCloudSettings();
    }
    else
    {
      project->setCloudSettings(settings.get());
    }

    project->save();
  }
}

CloudProvider CloudMonitor::newCloudProvider(const CloudSettings & settings, 
                                             const boost::optional<CloudSession> & session)
{
  boost::optional<CloudProvider> cloudProvider;

  if( settings.optionalCast<AWSSettings>() )
  {
    AWSProvider awsProvider;

    cloudProvider = awsProvider;

    cloudProvider->setSettings(settings);

    if( session )
    {
      OS_ASSERT(session->optionalCast<AWSSession>());

      cloudProvider->setSession(session.get());
    }
  }
  else if( settings.optionalCast<VagrantSettings>() )
  {
    VagrantProvider vagrantProvider;

    cloudProvider = vagrantProvider;

    cloudProvider->setSettings(settings);

    if( session )
    {
      OS_ASSERT(session->optionalCast<VagrantSession>());

      cloudProvider->setSession(session.get());
    }
  } 

  OS_ASSERT(cloudProvider);

  return cloudProvider.get();
}

void CloudMonitor::setStatus(CloudStatus status)
{
  if( status != m_status )
  {
    m_status = status;

    if( m_status == CLOUD_RUNNING )
    {
      m_worker->monitorCloudRunning();
    }

    emit cloudStatusChanged(status);
  }
}

CloudStatus CloudMonitor::status() const
{
  return m_status;
}

StartCloudWorker::StartCloudWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor),
    m_internetAvailable(false),
    m_validCredentials(false),
    m_resourcesAvailableToStart(false)

{
}

StartCloudWorker::~StartCloudWorker()
{
}

boost::optional<CloudSettings> StartCloudWorker::settings() const
{
  return m_settings;
}

boost::optional<CloudSession> StartCloudWorker::session() const
{
  return m_session;
}

void StartCloudWorker::startWorking()
{
  m_error = false;
  m_internetAvailable = false;
  m_validCredentials = false;
  m_resourcesAvailableToStart = false;

  boost::optional<CloudProvider> provider;

  CloudSettings settings = PatApp::instance()->cloudSettings();

  provider = CloudMonitor::newCloudProvider(settings);

  OS_ASSERT(provider);

  VagrantProvider vagrantProvider;

  // AWS provider does not seem to be providing accurate information
  m_internetAvailable = vagrantProvider.internetAvailable();

  if( ! m_internetAvailable )
  {
    m_error = true;
  }

  m_validCredentials = provider->validateCredentials();

  if( ! m_validCredentials )
  {
    m_error = true;
  }

  m_resourcesAvailableToStart = provider->resourcesAvailableToStart();

  if( ! m_resourcesAvailableToStart )
  {
    m_error = true;
  }

  if( ! m_error )
  {
    m_error = true;

    if( provider->requestStartServer() )
    {
      m_error = ! provider->waitForServer();
    }
  }

  if( ! m_error )
  {
    m_error = true;

    if( provider->requestStartWorkers() )
    {
      m_error = ! provider->waitForWorkers(600000);
    }
  }

  if( ! m_error )
  {
    m_error = true;

    boost::optional<Url> serverUrl = provider->session().serverUrl();
 
    if(serverUrl){

      for(int i = 0; i < 15; i++)
      {
        OSServer server(serverUrl.get());

        if( server.available() )
        {
          m_error = false;

          break;
        }
        else
        {
          System::msleep(3000);
        }
      } 
    }
  }

  if( ! m_error )
  {
    m_session = provider->session();
    m_settings = provider->settings();
  }
  else
  {
    if( provider->requestTerminate() )
    {
      provider->waitForTerminated();
    }
  }

  emit doneWorking();
}

bool StartCloudWorker::internetAvailable() const
{
  return m_internetAvailable;
}

bool StartCloudWorker::validCredentials() const
{
  return m_validCredentials;
}

bool StartCloudWorker::resourcesAvailableToStart() const
{
  return m_resourcesAvailableToStart;
}

bool StartCloudWorker::error() const
{
  return m_error;
}

StopCloudWorker::StopCloudWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor)
{
}

StopCloudWorker::~StopCloudWorker()
{
}

void StopCloudWorker::startWorking()
{
  detail::stopCloud();

  emit doneWorking();
}

ReconnectCloudWorker::ReconnectCloudWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor) ,
    m_status(CLOUD_STOPPED)
{
}

ReconnectCloudWorker::~ReconnectCloudWorker()
{
}

void ReconnectCloudWorker::startWorking()
{
  m_status = CLOUD_STOPPED;

  if( detail::reconnectToCloudSession() )
  {
    m_status = CLOUD_RUNNING;
  }

  emit doneWorking();
}

CloudStatus ReconnectCloudWorker::status() const
{
  return m_status;
}

RecoverCloudWorker::RecoverCloudWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor)
{
}

RecoverCloudWorker::~RecoverCloudWorker()
{
}

CloudStatus RecoverCloudWorker::status() const
{
  return m_status;
}

void RecoverCloudWorker::startWorking()
{
  m_status = CLOUD_ERROR;  

  if( detail::reconnectToCloudSession() )
  {
    m_status = CLOUD_RUNNING;
  }

  if( m_status == CLOUD_ERROR )
  {
    detail::stopCloud();

    m_status = CLOUD_STOPPED;
  }

  emit doneWorking();
}

CloudMonitorWorker::CloudMonitorWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor),
    m_count(0)
{
}

CloudMonitorWorker::~CloudMonitorWorker()
{
}

void CloudMonitorWorker::monitorCloudRunning()
{
  if( m_monitor->status() == CLOUD_RUNNING )
  {
    m_cloudServiceRunning = checkCloudServiceRunning();

    if( ! m_cloudServiceRunning )
    {
      m_cloudRunning = checkCloudRunning();
      m_authenticated = checkAuthenticated();
      m_internetAvailable = checkInternetAvailable();
      m_count++;
    }

    if( ! m_cloudServiceRunning && (m_count == 3) )
    {
      m_count = 0;

      emit cloudConnectionError();
    }
    else
    {
      QTimer::singleShot(2000,this,SLOT(monitorCloudRunning()));
    }
  }
}

bool CloudMonitorWorker::checkInternetAvailable() const
{
  VagrantProvider cloudProvider;

  return cloudProvider.internetAvailable();
}

bool CloudMonitorWorker::checkCloudRunning() const
{
  bool cloudRunning = true;

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession();
  boost::optional<CloudSettings> settings = CloudMonitor::currentProjectSettings();

  // TODO Consider emitting and display an error dialog in CloudMonitor
  OS_ASSERT(session);    
  OS_ASSERT(settings);

  CloudProvider newProvider = CloudMonitor::newCloudProvider(settings.get(),session.get());

  bool serverRunning = newProvider.serverRunning(); 
  bool workersRunning = newProvider.workersRunning();

  if( (! serverRunning) || (! workersRunning) )
  {
    cloudRunning = false;
  }
  
  return cloudRunning;
}

bool CloudMonitorWorker::checkCloudServiceRunning() const
{
  bool cloudServiceRunning = false;

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession();

  OS_ASSERT(session);    
  if( boost::optional<Url> serverUrl = session->serverUrl() )
  {
    OSServer server(serverUrl.get());
    cloudServiceRunning = server.available();
  }
  
  return cloudServiceRunning;
}

bool CloudMonitorWorker::checkAuthenticated() const
{
  bool authenticated = true;

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession();
  boost::optional<CloudSettings> settings = CloudMonitor::currentProjectSettings();

  OS_ASSERT(session);    
  OS_ASSERT(settings);

  CloudProvider newProvider = CloudMonitor::newCloudProvider(settings.get(),session.get());

  authenticated = newProvider.validateCredentials();

  return authenticated;
}

bool CloudMonitorWorker::internetAvailable() const
{
  return m_internetAvailable;
}

bool CloudMonitorWorker::cloudRunning() const
{
  return m_cloudRunning;
}

bool CloudMonitorWorker::cloudServiceRunning() const
{
  return m_cloudServiceRunning;
}

bool CloudMonitorWorker::authenticated() const
{
  return m_authenticated;
}

} // pat

} // openstudio
