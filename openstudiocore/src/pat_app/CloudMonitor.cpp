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
#include <QMutex>

namespace openstudio {

namespace pat {

CloudMonitor::CloudMonitor()
  : QObject()  
{    
  m_mutex = QSharedPointer<QMutex>(new QMutex());

  m_workerThread = QSharedPointer<QThread>(new QThread());
  m_worker = QSharedPointer<CloudMonitorWorker>(new CloudMonitorWorker(this));
  m_worker->moveToThread(m_workerThread.data());
  qRegisterMetaType<ToggleCloudButton::Status>("ToggleCloudButton::Status");
  bool bingo;
  bingo = connect(m_worker.data(),SIGNAL(cloudStatus(ToggleCloudButton::Status)),
                  this,SLOT(setCloudButtonStatus(ToggleCloudButton::Status)));
  OS_ASSERT(bingo);
  bingo = connect(m_worker.data(),SIGNAL(internetAvailable(bool)),
                  this,SIGNAL(internetAvailable(bool)));
  OS_ASSERT(bingo);
  m_workerThread->start();

  createTestSettings();

  m_worker->clockIn();
}      

CloudMonitor::~CloudMonitor()
{
  m_workerThread->quit();
  m_workerThread->wait();
}

void CloudMonitor::setCloudSettings(const CloudSettings & settings)
{
  if( m_cloudProvider )
  {
    m_cloudProvider.clear();
  }

  if( settings.optionalCast<AWSSettings>() )
  {
    setCloudProvider(QSharedPointer<AWSProvider>(new AWSProvider()));

    m_cloudProvider->setSettings(settings);
  }
  else if( settings.optionalCast<VagrantSettings>() )
  {
    setCloudProvider(QSharedPointer<VagrantProvider>(new VagrantProvider()));

    m_cloudProvider->setSettings(settings);
  }

  m_worker->setCloudSettings(settings);
}

CloudSettings CloudMonitor::cloudSettings() const
{
  if( m_cloudProvider )
  {
    return m_cloudProvider->settings();
  }
  else
  {
    return AWSSettings();
  }
}

//QSharedPointer<CloudProvider> CloudMonitor::cloudProvider() const
//{
//  return m_cloudProvider;
//}

bool CloudMonitor::starting()
{
  QMutexLocker locker(m_mutex.data());

  bool result(false);

  if( m_cloudProvider )
  {
    if( m_cloudProvider->serverStarted() && ! m_cloudProvider->serverRunning() ) 
    { 
      result = true; 
    }
  }

  return result;
}

bool CloudMonitor::stopping()
{
  QMutexLocker locker(m_mutex.data());

  bool result(false);

  if( m_cloudProvider )
  {
    if( m_cloudProvider->terminateStarted() && serverRunning() ) 
    { 
      result = true; 
    }
  }

  return result;
}

bool CloudMonitor::serverRunning()
{
  m_cloudProvider->requestServerRunning();

  return false;
}

void CloudMonitor::setCloudProvider(QSharedPointer<CloudProvider> cloudProvider)
{
  m_cloudProvider = cloudProvider;

  bool bingo;

  if( m_cloudProvider )
  {
    bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(serverStarted(const Url&)),
                    this,SLOT(onCloudStartupComplete()));
    OS_ASSERT(bingo);
  
    bingo = connect(m_cloudProvider->getImpl<detail::CloudProvider_Impl>().get(),SIGNAL(terminated()),
                   this,SLOT(onCloudTerminateComplete()));
    OS_ASSERT(bingo);
  }
}

void CloudMonitor::toggleCloud()
{
  if( m_cloudProvider )
  {
    if( m_cloudProvider->serverStarted() )
    {
      m_mutex->lock();

      m_cloudProvider->requestTerminate();

      setCloudButtonStatus(ToggleCloudButton::STOPPING);
    }
    else
    {
      m_mutex->lock();

      m_cloudProvider->requestStartServer();

      //m_cloudProvider->requestStartWorkers();

      setCloudButtonStatus(ToggleCloudButton::STARTING);
    }
  }
}

void CloudMonitor::setCloudButtonStatus(ToggleCloudButton::Status status)
{
  if( QSharedPointer<RunTabController> runTabController = PatApp::instance()->runTabController() )
  {
    RunView * runView = runTabController->runView; 
  
    OS_ASSERT(runView);
    
    runView->runStatusView->toggleCloudButton->setStatus(status);
  }
}

void CloudMonitor::onCloudStartupComplete()
{
  setCloudButtonStatus(ToggleCloudButton::RUNNING);

  m_mutex->unlock();
}

void CloudMonitor::onCloudTerminateComplete()
{
  createTestSettings();

  setCloudButtonStatus(ToggleCloudButton::STOPPED);

  m_mutex->unlock();
}

void CloudMonitor::createTestSettings()
{
  // create the vagrant provider
  path serverPath = vagrantServerPath();
  Url serverUrl("http://localhost:8080");
  path workerPath = vagrantWorkerPath();
  Url workerUrl("http://localhost:8081");

  QSharedPointer<VagrantProvider> provider;
  provider = QSharedPointer<VagrantProvider>(new VagrantProvider());

  VagrantSettings vagrantSettings;
  vagrantSettings.setServerUrl(serverUrl);
  vagrantSettings.setWorkerUrl(workerUrl);
  vagrantSettings.setServerPath(serverPath);
  vagrantSettings.setWorkerPath(workerPath);
  vagrantSettings.signUserAgreement(true);
  vagrantSettings.setHaltOnStop(true);
  
  setCloudSettings(vagrantSettings);
}

CloudMonitorWorker::CloudMonitorWorker(CloudMonitor * monitor)
  : m_monitor(monitor)
{
}

CloudMonitorWorker::~CloudMonitorWorker()
{
}

void CloudMonitorWorker::setCloudSettings(const CloudSettings & settings)
{
  m_cloudSettings = settings;
}

void CloudMonitorWorker::clockIn()
{
  nowGetToWork();
}

void CloudMonitorWorker::nowGetToWork()
{
  // TODO we should use the actual current provider(i.e. Vagrant, AwsProvider, etc)

  if( m_cloudSettings )
  {
    if( ! m_monitor->starting() && ! m_monitor->stopping() )
    {
      if( boost::optional<VagrantSettings> settings = m_cloudSettings->optionalCast<VagrantSettings>() )
      {
        VagrantProvider provider;
        provider.setSettings(settings.get());

        if( provider.serverRunning() )
        {
          emit cloudStatus(ToggleCloudButton::RUNNING);
        }
        else
        {
          emit cloudStatus(ToggleCloudButton::STOPPED);
        }
      }
      else if( boost::optional<AWSSettings> settings =  m_cloudSettings->optionalCast<AWSSettings>() )
      {
      }
    }

    // Check Internet Availability

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

    QTimer::singleShot(5000,this,SLOT(nowGetToWork()));
  }
}

} // pat

} // openstudio
