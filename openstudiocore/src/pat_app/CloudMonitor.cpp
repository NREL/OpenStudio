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
#include "RunTabController.hpp"
#include "RunView.hpp"
#include "PatApp.hpp"
#include "PatMainWindow.hpp"
#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/cloud/CloudProvider_Impl.hpp>
#include <utilities/cloud/VagrantProvider.hpp>
#include <utilities/cloud/VagrantProvider_Impl.hpp>
#include <utilities/cloud/AWSProvider.hpp>
#include <utilities/cloud/AWSProvider_Impl.hpp>
#include <utilities/core/Assert.hpp>
#include <pat_app/VagrantConfiguration.hxx>
#include <QThread>
#include <QTimer>
#include <QMessageBox>

namespace openstudio {

namespace pat {

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
  bingo = connect(m_worker.data(),SIGNAL(internetAvailable(bool)),
                  this,SIGNAL(internetAvailable(bool)));
  OS_ASSERT(bingo);
  bingo = connect(m_worker.data(),SIGNAL(cloudConnectionLost()),
                  this,SLOT(onCloudConnectionLost()));
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

  // Start the CloudMonitorWorker 
  m_workerThread->start();

  m_worker->startWorking();
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
}

void CloudMonitor::startCloud()
{
  if( m_status == CLOUD_STOPPED )
  {
    setStatus(CLOUD_STARTING);
    
    m_startCloudThread = QSharedPointer<QThread>(new QThread());

    m_startCloudWorker->moveToThread(m_startCloudThread.data());

    m_startCloudThread->start();

    m_startCloudWorker->startWorking();
  }
}

void CloudMonitor::onStartCloudWorkerComplete()
{
  m_startCloudThread->quit();

  m_startCloudThread->wait();

  m_startCloudThread.clear();

  setStatus(CLOUD_RUNNING);

  emit startCloudCompleted();
}

void CloudMonitor::stopCloud()
{
  if( m_status == CLOUD_RUNNING )
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
  m_stopCloudThread->quit();

  m_stopCloudThread->wait();

  m_stopCloudThread.clear();

  setStatus(CLOUD_STOPPED);

  emit stopCloudCompleted();
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

  emit reconnectCloudCompleted();

  // TODO Handle other states
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

boost::optional<CloudSettings> CloudMonitor::currentProjectSettings()
{
  boost::optional<CloudSettings> settings;

  if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() )
  {
    settings = project->cloudSettings();
  }

  return settings;
}

void CloudMonitor::setCurrentProjectSession(const boost::optional<CloudSession> & session)
{
  if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() )
  {
    if( ! session )
    {
      project->clearCloudSession();
    }
    else
    {
      project->setCloudSession(session.get());
    }
  }
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
  m_status = status;

  if( QSharedPointer<RunTabController> runTabController = PatApp::instance()->runTabController() )
  {
    RunView * runView = runTabController->runView; 
  
    OS_ASSERT(runView);
    
    runView->runStatusView->toggleCloudButton->setStatus(status);
  }
}

CloudStatus CloudMonitor::status() const
{
  return m_status;
}

CloudSettings CloudMonitor::createTestSettings()
{
  // create the vagrant provider
  path serverPath = vagrantServerPath();
  Url serverUrl("http://localhost:8080");
  path workerPath = vagrantWorkerPath();
  Url workerUrl("http://localhost:8081");

  VagrantSettings vagrantSettings;
  vagrantSettings.setServerUrl(serverUrl);
  vagrantSettings.setWorkerUrl(workerUrl);
  vagrantSettings.setServerPath(serverPath);
  vagrantSettings.setWorkerPath(workerPath);
  vagrantSettings.signUserAgreement(true);
  vagrantSettings.setHaltOnStop(true);

  return vagrantSettings;
}

void CloudMonitor::onCloudConnectionLost()
{
  QMessageBox message(PatApp::instance()->mainWindow);
  message.setText("OpenStudio Cloud connection was unexpectedly lost.");
  message.exec();
}

void CloudMonitor::onCloudUnexpectedlyStarted()
{
}

StartCloudWorker::StartCloudWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor)
{
}

StartCloudWorker::~StartCloudWorker()
{
}

void StartCloudWorker::startWorking()
{
  // Add event loop to wait for cloud to stop
  //QEventLoop loop;
  //bool bingo;
  //bingo = connect(m_monitor,SIGNAL(stopCloudCompleted()),&loop,SLOT(quit()));
  //OS_ASSERT(bingo);

  //m_monitor->stopCloud();

  //loop.exec();

  boost::optional<CloudProvider> provider;

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession(); 
  boost::optional<CloudSettings> settings = CloudMonitor::currentProjectSettings();

  // If there is already a session, try to connect to that
  if( session && settings ) 
  {
    provider = CloudMonitor::newCloudProvider(settings.get(),session);
  }
  else
  {
    // TODO: Grab these from PatApp
    settings = CloudMonitor::createTestSettings();

    provider = CloudMonitor::newCloudProvider(settings.get());
  }

  OS_ASSERT(provider);

  //m_monitor->setCloudProvider(provider.get());

  if( provider->requestStartServer() )
  {
    provider->waitForServer();
  }

  if( provider->requestStartWorkers() )
  {
    provider->waitForWorkers();
  }

  CloudMonitor::setCurrentProjectSession(provider->session());
  CloudMonitor::setCurrentProjectSettings(provider->settings());

  emit doneWorking();
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
  boost::optional<CloudProvider> provider;

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession(); 
  boost::optional<CloudSettings> settings = CloudMonitor::currentProjectSettings();

  // If there is already a session, try to connect to that
  if( session && settings ) 
  {
    provider = CloudMonitor::newCloudProvider(settings.get(),session);
  }
  else
  {
    // TODO: Grab these from PatApp
    settings = CloudMonitor::createTestSettings();

    provider = CloudMonitor::newCloudProvider(settings.get());
  }

  OS_ASSERT(provider);

  //m_monitor->setCloudProvider(provider.get());

  if( provider->requestTerminate() )
  {
    provider->waitForTerminated();
  }

  CloudMonitor::setCurrentProjectSession(boost::none);
  CloudMonitor::setCurrentProjectSettings(boost::none);

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

  boost::optional<CloudProvider> provider;

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession(); 
  boost::optional<CloudSettings> settings = CloudMonitor::currentProjectSettings();

  // If there is already a session, try to connect to that
  if( session && settings ) 
  {
    provider = CloudMonitor::newCloudProvider(settings.get(),session);

    OS_ASSERT(provider);

    if( provider->serverRunning() && provider->workersRunning() )
    {
      m_status = CLOUD_RUNNING;
    }
  }

  emit doneWorking();
}

CloudStatus ReconnectCloudWorker::status() const
{
  return m_status;
}

CloudMonitorWorker::CloudMonitorWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor)
{
}

CloudMonitorWorker::~CloudMonitorWorker()
{
}

void CloudMonitorWorker::startWorking()
{
  checkForNewSession();

  checkForInternetConnection();

  checkForCloudConnection();

  QTimer::singleShot(5000,this,SLOT(startWorking()));
}

void CloudMonitorWorker::checkForInternetConnection()
{
  // TODO we should use the actual current provider(i.e. Vagrant, AwsProvider, etc)

  static bool internetAvailableCurrently = false;
  static bool internetAvailableLastTime = false;
  static bool internetAvailableTimeBeforeLast = false;

  internetAvailableTimeBeforeLast = internetAvailableLastTime;
  internetAvailableLastTime = internetAvailableCurrently;

  VagrantProvider cloudProvider;
  internetAvailableCurrently = cloudProvider.internetAvailable();

  if(internetAvailableCurrently && internetAvailableLastTime && internetAvailableTimeBeforeLast){
    emit internetAvailable(true);
  } else if(!internetAvailableCurrently && !internetAvailableLastTime && !internetAvailableTimeBeforeLast){
    emit internetAvailable(false);
  }
}

void CloudMonitorWorker::checkForCloudConnection()
{
  //if( m_monitor->status() == CLOUD_RUNNING )
  //{
  //  boost::optional<CloudSession> session;
  //  boost::optional<CloudSettings> settings;

  //  boost::optional<CloudProvider> provider = m_monitor->cloudProvider();

  //  OS_ASSERT(provider);

  //  session = provider->session();
  //  settings = provider->settings();

  //  OS_ASSERT(session);    
  //  OS_ASSERT(settings);

  //  CloudProvider newProvider = CloudMonitor::newCloudProvider(settings.get(),session.get());

  //  bool serverRunning = newProvider.serverRunning(); 
  //  bool workersRunning = newProvider.workersRunning();

  //  if( (! serverRunning) || (! workersRunning) )
  //  {
  //    emit cloudConnectionLost();
  //  }
  //}
}

void CloudMonitorWorker::checkForNewSession()
{
  //if( m_monitor->status() != CLOUD_STARTING &&
  //    m_monitor->status() != CLOUD_STOPPING )
  //{
  //  boost::optional<CloudSession> session;
  //  boost::optional<CloudSettings> settings;

  //  if( boost::optional<CloudProvider> provider = m_monitor->cloudProvider() )
  //  {
  //    session = provider->session();
  //    settings = provider->settings();
  //  }

  //  boost::optional<CloudSettings> currentProjectSettings = m_monitor->currentProjectSettings();
  //  boost::optional<CloudSession> currentProjectSession = m_monitor->currentProjectSession();

  //  if( session && currentProjectSession && 
  //      settings && currentProjectSettings ) 
  //  {
  //    if( ( currentProjectSession->getImpl<detail::CloudSession_Impl>() != 
  //          session->getImpl<detail::CloudSession_Impl>() ) ||
  //        ( currentProjectSettings->getImpl<detail::CloudSettings_Impl>() != 
  //          settings->getImpl<detail::CloudSettings_Impl>() )
  //      )
  //    {
  //      emit cloudConfigurationChanged();
  //    } 
  //  } 
  //  else if( (session && ! currentProjectSession) ||
  //           (currentProjectSession && ! session ) )
  //  {
  //    emit cloudConfigurationChanged();
  //  }
  //  else if( (settings && ! currentProjectSettings) ||
  //           (currentProjectSettings && ! settings ) )
  //  {
  //    emit cloudConfigurationChanged();
  //  }
  //}
}

} // pat

} // openstudio
