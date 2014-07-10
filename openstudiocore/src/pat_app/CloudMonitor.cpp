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

#include "CloudMonitor.hpp"

#include "PatApp.hpp"
#include "PatMainWindow.hpp"
#include "RunTabController.hpp"
#include "RunView.hpp"

#include "../project/ProjectDatabase.hpp"
#include "../analysisdriver/CloudAnalysisDriver.hpp"
#include "../analysisdriver/SimpleProject.hpp"
#include "../analysis/DataPoint.hpp"
#include "../utilities/cloud/CloudProvider.hpp"
#include "../utilities/cloud/CloudProvider_Impl.hpp"
#include "../utilities/cloud/VagrantProvider.hpp"
#include "../utilities/cloud/VagrantProvider_Impl.hpp"
#include "../utilities/cloud/AWSProvider.hpp"
#include "../utilities/cloud/AWSProvider_Impl.hpp"
#include "../utilities/cloud/OSServer.hpp"
#include "../utilities/cloud/OSServer_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/System.hpp"

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

bool stopCloud()
{
  bool result = false;

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
    result = provider->waitForTerminated();
  }

  return result;
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

// Return true if there is an internet connection
bool checkInternetAvailable() 
{
  VagrantProvider cloudProvider;

  return cloudProvider.internetAvailable();
}

// Return true if we can authenticate to cloud service
bool checkAuthenticated() 
{
  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession();
  boost::optional<CloudSettings> settings = CloudMonitor::currentProjectSettings();

  OS_ASSERT(session);    
  OS_ASSERT(settings);

  CloudProvider newProvider = CloudMonitor::newCloudProvider(settings.get(),session.get());

  bool authenticated = newProvider.validateCredentials();

  return authenticated;
}

// Return true if AWS servers and workers are running
bool checkCloudRunning() 
{
  bool cloudRunning = true;

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession();
  boost::optional<CloudSettings> settings = CloudMonitor::currentProjectSettings();

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

// Return true if OSServer is running
bool checkCloudServiceRunning() 
{
  bool cloudServiceRunning = false;

  boost::optional<CloudSession> session = CloudMonitor::currentProjectSession();
  if( session)
  {
    boost::optional<Url> serverUrl = session->serverUrl();
    if (serverUrl)
    {
      OSServer server(serverUrl.get());
      cloudServiceRunning = server.available();
    }
  }
  
  return cloudServiceRunning;
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

    boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();
    if( preCheckPassed && project )
    {
      // check for baseline
      analysis::DataPoint baseline = project->baselineDataPoint();
      if ( !baseline.complete() ||  baseline.failed() ){
        QString error("The baseline model must be run before starting the cloud.");
        QMessageBox::critical(PatApp::instance()->mainWindow, "Baseline Not Run", error);
        preCheckPassed = false;
      }
    }

    CloudSettings settings = PatApp::instance()->cloudSettings();
    boost::optional<AWSSettings> awsSettings = settings.optionalCast<AWSSettings>();
    if( preCheckPassed && awsSettings )
    {
      if( !awsSettings->validAccessKey() ) {
        QString error("Invalid Access Key.  Verify settings from Cloud menu.");
        QMessageBox::critical(PatApp::instance()->mainWindow, "Cloud Settings", error);
        preCheckPassed = false;

      } else if( !awsSettings->validSecretKey() ) {
        QString error("Invalid Secret Key.  Verify settings from Cloud menu.");
        QMessageBox::critical(PatApp::instance()->mainWindow, "Cloud Settings", error);
        preCheckPassed = false;

      } else if( !awsSettings->userAgreementSigned() ) {
        QString error("The user agreement must be reviewed and signed before continuing.  Verify settings from Cloud menu.");
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

    QString error;
    if( m_startCloudWorker->errors().size() )
    {
      error = toQString(m_startCloudWorker->errors()[0]) + ".  Verify settings from Cloud menu.";
    }
    else
    {
      error = "Invalid cloud credentials.  Verify settings from Cloud menu.";
    }

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

    QString error;
    if( m_startCloudWorker->errors().size() )
    {
      QString errorMsg = toQString(m_startCloudWorker->errors()[0]);
      if (errorMsg == "InvalidAMIID.NotFound")
      {
        errorMsg = "AMI not found";
      }
      error = "Error starting cloud: " + errorMsg + ".  Check for any unintentionally running instances using AWS console and terminate them to avoid charges.";
    } else {
      error = "Unknown error starting cloud.  Check for any unintentionally running instances using AWS console and terminate them to avoid charges.";
    }
    
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

  if( m_stopCloudWorker->error() )
  {
    QMessageBox::warning(PatApp::instance()->mainWindow, 
      "Cloud Shutdown Error", 
      "The OpenStudio Cloud encountered an error while shutting down.  Please use the AWS Management Console to confirm that services have been stopped.");
  }

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

  boost::optional<CloudSettings> settings = currentProjectSettings();

  boost::optional<CloudSession> session = currentProjectSession();

  if( m_reconnectCloudWorker->cloudServiceRunning() )
  {
    setStatus(CLOUD_RUNNING);

    if( m_reconnectCloudWorker->projectIsOnCloud() )
    {
      boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project();

      if( project )
      {
        boost::optional<analysisdriver::CloudAnalysisDriver> cloudAnalysisDriver = project->cloudAnalysisDriver();

        if( cloudAnalysisDriver )
        {
          cloudAnalysisDriver->requestRun();
        }
      }
    }
  }
  else if( settings && session )
  {
    setStatus(CLOUD_ERROR);

    LostCloudConnectionDialog dialog(m_reconnectCloudWorker->internetAvailable(),
                                     m_reconnectCloudWorker->authenticated(),
                                     m_reconnectCloudWorker->cloudRunning());

    dialog.exec();

    if( dialog.clearCloudSession() )
    {
      stopCloud();
    }
  }
  else
  {
    setStatus(CLOUD_STOPPED);
  }
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

  if( m_recoverCloudWorker->cloudServiceRunning() )
  {
    setStatus(CLOUD_RUNNING);
  }
  else
  {
    setStatus(CLOUD_ERROR);

    LostCloudConnectionDialog dialog(m_recoverCloudWorker->internetAvailable(),
                                     m_recoverCloudWorker->authenticated(),
                                     m_recoverCloudWorker->cloudRunning());

    dialog.exec();

    if( dialog.clearCloudSession() )
    {
      stopCloud();
    }
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
    m_resourcesAvailableToStart(false),
    m_error(false)

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

  if( ! m_error )
  {
    m_validCredentials = provider->validateCredentials();
    if( ! m_validCredentials )
    {
      m_error = true;
    }
  }

  if( ! m_error)
  {
    m_resourcesAvailableToStart = provider->resourcesAvailableToStart();
    if( ! m_resourcesAvailableToStart )
    {
      m_error = true;
    }
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

  std::vector<std::string> serverErrors;
  if( ! m_error )
  {
    m_error = true;

    boost::optional<Url> serverUrl = provider->session().serverUrl();
 
    if(serverUrl){

      unsigned waitMS = 3000; // 3000
      int increment = 15; // 15
      int maxTries = increment;
      for(int i = 0; i < maxTries; i++)
      {
        OSServer server(serverUrl.get());

        if( server.available() )
        {
          m_error = false;

          break;
        }
        else
        {
          if (i < maxTries - 1) 
          {
            System::msleep(waitMS);
          } 
          else 
          {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(PatApp::instance()->mainWidget(), QString("Continue Waiting For Cloud?"), QString("Cloud has not yet started, continue waiting for cloud to launch?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
            if (reply == QMessageBox::Yes) 
            {
              maxTries += increment;
            }
            else
            {
              serverErrors = server.errors();
            }
          }
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
    m_errors = provider->errors();
    m_warnings = provider->warnings();

    if (!m_errors.size() && serverErrors.size()) {
      m_errors = serverErrors;
    }

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

std::vector<std::string> StartCloudWorker::errors() const
{
  return m_errors;
}

std::vector<std::string> StartCloudWorker::warnings() const
{
  return m_warnings;
}

StopCloudWorker::StopCloudWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor),
    m_error(false)
{
}

StopCloudWorker::~StopCloudWorker()
{
}

bool StopCloudWorker::error() const
{
  return m_error;
}

void StopCloudWorker::startWorking()
{
  m_error = ( ! detail::stopCloud() );

  emit doneWorking();
}

ReconnectCloudWorker::ReconnectCloudWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor),
    m_internetAvailable(false),
    m_authenticated(false),
    m_cloudRunning(false),
    m_cloudServiceRunning(false),
    m_projectIsOnCloud(false),
    m_status(CLOUD_STOPPED)
{
}

ReconnectCloudWorker::~ReconnectCloudWorker()
{
}

void ReconnectCloudWorker::startWorking()
{
  m_cloudServiceRunning = detail::checkCloudServiceRunning();

  m_projectIsOnCloud = false;

  if( m_cloudServiceRunning )
  {
    // It would be better to put this code related to running analysis somewhere outside of CloudMonitor
    // or bring all of the "run" code into cloud monitor.  This is a convenience to save time.
    // Consider a RunMonitor counterpart to CloudMonitor.
    boost::optional<CloudSession> session = CloudMonitor::currentProjectSession();
    if( session)
    {
      boost::optional<Url> serverUrl = session->serverUrl();
      if (serverUrl)
      {
        OSServer server(serverUrl.get());

        std::vector<UUID> projectIDs = server.projectUUIDs(10000);

        if( boost::optional<analysisdriver::SimpleProject> project = PatApp::instance()->project() )
        {
          std::vector<UUID>::iterator it;
          it = std::find(projectIDs.begin(),projectIDs.end(),project->projectDatabase().handle());
          if( it != projectIDs.end() )
          {
            m_projectIsOnCloud = true;
          }
        }
      }
    }
  }
  else
  {
    m_cloudRunning = detail::checkCloudRunning();
    m_authenticated = detail::checkAuthenticated();
    m_internetAvailable = detail::checkInternetAvailable();
  }

  emit doneWorking();
}

bool ReconnectCloudWorker::projectIsOnCloud() const
{
  return m_projectIsOnCloud;
}

bool ReconnectCloudWorker::internetAvailable() const
{
  return m_internetAvailable;
}

bool ReconnectCloudWorker::cloudRunning() const
{
  return m_cloudRunning;
}

bool ReconnectCloudWorker::cloudServiceRunning() const
{
  return m_cloudServiceRunning;
}

bool ReconnectCloudWorker::authenticated() const
{
  return m_authenticated;
}

RecoverCloudWorker::RecoverCloudWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor),
    m_internetAvailable(false),
    m_authenticated(false),
    m_cloudRunning(false),
    m_cloudServiceRunning(false)
{
}

RecoverCloudWorker::~RecoverCloudWorker()
{
}

void RecoverCloudWorker::startWorking()
{
  m_cloudServiceRunning = detail::checkCloudServiceRunning();

  if( ! m_cloudServiceRunning )
  {
    m_cloudRunning = detail::checkCloudRunning();
    m_authenticated = detail::checkAuthenticated();
    m_internetAvailable = detail::checkInternetAvailable();
  }

  emit doneWorking();
}

bool RecoverCloudWorker::internetAvailable() const
{
  return m_internetAvailable;
}

bool RecoverCloudWorker::cloudRunning() const
{
  return m_cloudRunning;
}

bool RecoverCloudWorker::cloudServiceRunning() const
{
  return m_cloudServiceRunning;
}

bool RecoverCloudWorker::authenticated() const
{
  return m_authenticated;
}

CloudMonitorWorker::CloudMonitorWorker(CloudMonitor * monitor)
  : QObject(),
    m_monitor(monitor),
    m_internetAvailable(false),
    m_authenticated(false),
    m_cloudRunning(false),
    m_cloudServiceRunning(false),
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
    m_cloudServiceRunning = detail::checkCloudServiceRunning();

    if( ! m_cloudServiceRunning )
    {
      // Fixing OS_ASSERT crash on exiting PAT with Cloud running, 
      // but definitely not fixing it in the best way. TODO: Do it right.
      try {
        m_cloudRunning = detail::checkCloudRunning();
      }
      catch (...) {
        m_cloudRunning = false;
      }
      m_authenticated = detail::checkAuthenticated();
      m_internetAvailable = detail::checkInternetAvailable();
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
