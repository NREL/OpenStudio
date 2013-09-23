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

#include <pat_app/CloudMonitor.hpp>
#include <pat_app/RunTabController.hpp>
#include <pat_app/RunView.hpp>
#include <pat_app/PatApp.hpp>
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

namespace openstudio {

namespace pat {

CloudMonitor::CloudMonitor()
  : QObject(),
    m_serverStarted(false),
    m_allWorkersStarted(false)
{    
  setStatus(CLOUD_STOPPED);

  m_workerThread = QSharedPointer<QThread>(new QThread());
  m_worker = QSharedPointer<CloudMonitorWorker>(new CloudMonitorWorker(this));
  m_worker->moveToThread(m_workerThread.data());
  qRegisterMetaType<CloudStatus>("CloudStatus");
  bool bingo;
  bingo = connect(m_worker.data(),SIGNAL(internetAvailable(bool)),
                  this,SIGNAL(internetAvailable(bool)));
  OS_ASSERT(bingo);
  //bingo = connect(m_worker.data(),SIGNAL(cloudConfigurationChanged()),
  //                this,SLOT(initializeCloudSession()));
  //OS_ASSERT(bingo);
  m_workerThread->start();

  //initializeCloudSession();

  m_worker->startWorking();
}      

CloudMonitor::~CloudMonitor()
{
  m_workerThread->quit();
  m_workerThread->wait();
}

void CloudMonitor::setCloudProvider(boost::optional<CloudProvider> cloudProvider)
{
  m_cloudProvider = cloudProvider;

  bool bingo;

  if( m_cloudProvider )
  {
    bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(serverStarted(const Url&)),
                    this,SLOT(onServerStarted()));
    OS_ASSERT(bingo);

    bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(allWorkersStarted()),
                    this,SLOT(onAllWorkersStarted()));
    OS_ASSERT(bingo);
  
    bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(terminated()),
                   this,SLOT(onCloudTerminateComplete()));
    OS_ASSERT(bingo);
  }
}

boost::optional<CloudProvider> CloudMonitor::cloudProvider() const
{
  return m_cloudProvider;
}

void CloudMonitor::toggleCloud()
{
  if( m_cloudProvider )
  {
    if( m_status == CLOUD_RUNNING )
    {
      m_cloudProvider->requestTerminate();

      setStatus(CLOUD_STOPPING);
    }
    else if( m_status == CLOUD_STOPPED )
    {
      startCloud();

      setStatus(CLOUD_STARTING);
    }
  }
  else if( ! currentProjectSession() )
  {
    CloudSettings settings = createTestSettings();

    CloudProvider provider = newCloudProvider(settings);

    setCloudProvider(provider);

    startCloud();

    setStatus(CLOUD_STARTING);
  }
}

void CloudMonitor::startCloud()
{
  if( m_cloudProvider && m_status == CLOUD_STOPPED )
  {
    m_serverStarted = false;

    m_allWorkersStarted = false;

    m_cloudProvider->requestStartServer();

    //m_cloudProvider->requestStartWorkers();
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

boost::optional<CloudSettings> CloudMonitor::currentProjectSettings()
{
  boost::optional<CloudSettings> settings;

  if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() )
  {
    settings = project->cloudSettings();
  }

  return settings;
}

void CloudMonitor::setCurrentProjectSession(const CloudSession & session)
{
  if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() )
  {
    project->setCloudSession(session);
  }
}

void CloudMonitor::setCurrentProjectSettings(const CloudSettings & settings)
{
  if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() )
  {
    project->setCloudSettings(settings);
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

void CloudMonitor::onCloudStartupComplete()
{
  OS_ASSERT(m_cloudProvider);

  // TODO figure out why this causes problems when we try to restart
  // Maybe bug in project, maybe bug in CloudMonitor

  //setCurrentProjectSession(m_cloudProvider->session());

  //setCurrentProjectSettings(m_cloudProvider->settings());

  setStatus(CLOUD_RUNNING);
}

void CloudMonitor::onServerStarted()
{
  m_serverStarted = true;

  if( m_cloudProvider && m_status == CLOUD_STARTING )
  {
    m_cloudProvider->requestStartWorkers();
  }

  //if( m_serverStarted && m_allWorkersStarted )
  //{
  //  onCloudStartupComplete();
  //}
}

void CloudMonitor::onAllWorkersStarted()
{
  m_allWorkersStarted = true;

  if( m_serverStarted && m_allWorkersStarted )
  {
    onCloudStartupComplete();
  }
}

void CloudMonitor::onCloudTerminateComplete()
{
  m_serverStarted = false;

  m_allWorkersStarted = false;

  m_cloudProvider = boost::none;

  setStatus(CLOUD_STOPPED);
}

CloudSettings CloudMonitor::createTestSettings() const
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

void CloudMonitor::initializeCloudSession()
{
  if( QSharedPointer<RunTabController> runTabController = PatApp::instance()->runTabController() )
  {
    RunView * runView = runTabController->runView; 
  
    OS_ASSERT(runView);
    
    runView->runStatusView->toggleCloudButton->setEnabled(false);
  }

  m_initializeCloudThread = QSharedPointer<QThread>(new QThread());

  m_initializeCloudWorker = QSharedPointer<InitializeCloudWorker>(new InitializeCloudWorker(this));

  bool bingo;

  bingo = connect(m_initializeCloudWorker.data(),SIGNAL(doneWorking()),this,SLOT(onCloudSessionInitializeComplete()));

  OS_ASSERT(bingo);

  m_initializeCloudWorker->moveToThread(m_initializeCloudThread.data());

  m_initializeCloudThread->start();

  m_initializeCloudWorker->startWorking();
}

void CloudMonitor::onCloudSessionInitializeComplete()
{
  if( QSharedPointer<RunTabController> runTabController = PatApp::instance()->runTabController() )
  {
    RunView * runView = runTabController->runView; 
  
    OS_ASSERT(runView);
    
    runView->runStatusView->toggleCloudButton->setEnabled(true);
  }

  m_initializeCloudThread->quit();
  m_initializeCloudThread.clear();
  m_initializeCloudWorker.clear();
}

void CloudMonitor::onCloudConnectionLost()
{
}

void CloudMonitor::onCloudUnexpectedlyStarted()
{
}

InitializeCloudWorker::InitializeCloudWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor)
{
}

InitializeCloudWorker::~InitializeCloudWorker()
{
}

void InitializeCloudWorker::startWorking()
{
  if( m_monitor->status() != CLOUD_STOPPED )
  {
  }
  else
  {
    boost::optional<CloudSession> session = CloudMonitor::currentProjectSession();

    boost::optional<CloudSettings> settings = CloudMonitor::currentProjectSettings();
    
    if( settings )
    {
      boost::optional<CloudProvider> cloudProvider = CloudMonitor::newCloudProvider(settings.get(),session);

      if( cloudProvider )
      {
        if( cloudProvider->serverRunning() )
        {
          m_monitor->setStatus(CLOUD_RUNNING);
        }
        else
        {
          m_monitor->setStatus(CLOUD_STOPPED);
        }

        m_monitor->setCloudProvider(cloudProvider);
      }
    }
  }

  emit doneWorking();
}

CloudMonitorWorker::CloudMonitorWorker(CloudMonitor * monitor)
  : m_monitor(monitor)
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
}

void CloudMonitorWorker::checkForNewSession()
{
  if( m_monitor->status() != CLOUD_STARTING &&
      m_monitor->status() != CLOUD_STOPPING )
  {
    boost::optional<CloudSession> session;
    boost::optional<CloudSettings> settings;

    if( boost::optional<CloudProvider> provider = m_monitor->cloudProvider() )
    {
      session = provider->session();
      settings = provider->settings();
    }

    boost::optional<CloudSettings> currentProjectSettings = m_monitor->currentProjectSettings();
    boost::optional<CloudSession> currentProjectSession = m_monitor->currentProjectSession();

    if( session && currentProjectSession && 
        settings && currentProjectSettings ) 
    {
      if( ( currentProjectSession->getImpl<detail::CloudSession_Impl>() != 
            session->getImpl<detail::CloudSession_Impl>() ) ||
          ( currentProjectSettings->getImpl<detail::CloudSettings_Impl>() != 
            settings->getImpl<detail::CloudSettings_Impl>() )
        )
      {
        emit cloudConfigurationChanged();
      } 
    } 
    else if( (session && ! currentProjectSession) ||
             (currentProjectSession && ! session ) )
    {
      emit cloudConfigurationChanged();
    }
    else if( (settings && ! currentProjectSettings) ||
             (currentProjectSettings && ! settings ) )
    {
      emit cloudConfigurationChanged();
    }
  }
}

} // pat

} // openstudio
