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

#ifndef OPENSTUDIO_CLOUDMONITOR_HPP
#define OPENSTUDIO_CLOUDMONITOR_HPP

#include "RunView.hpp"
#include "PatConstants.hpp"
#include <QObject>
#include <QSharedPointer>
#include <utilities/core/Path.hpp>
#include <utilities/core/Url.hpp>
#include <utilities/cloud/CloudProvider.hpp>
#include <vector>
#include <map>

class QThread;

namespace openstudio {

namespace pat {

class CloudMonitorWorker;

class InitializeCloudWorker;

class CloudMonitor : public QObject
{
  Q_OBJECT

  public:

  CloudMonitor();

  virtual ~CloudMonitor();

  // A reliable method to retrieve the cloud status
  CloudStatus status() const;

  // Return a new CloudProvider with from settings and session.
  static CloudProvider newCloudProvider(const CloudSettings & settings, 
                                        const boost::optional<CloudSession> & session = boost::none);

  // Return the session stored in the App's current project
  static boost::optional<CloudSession> currentProjectSession();

  // Return the settings stored in the App's current project
  static boost::optional<CloudSettings> currentProjectSettings();

  // Set the session in the App's current project
  static void setCurrentProjectSession(const CloudSession & session);

  // Set the settings in the App's current project
  static void setCurrentProjectSettings(const CloudSettings & settings);

  signals:

  void internetAvailable(bool isAvailable);

  public slots:

  // If cloud is on then turn off, and vice versa
  void toggleCloud();
  
  protected:

  // We don't want to expose this pulically.  CloudMonitor should insulate m_cloudProvider.
  boost::optional<CloudProvider> cloudProvider() const;

  // Dont assign to m_cloudProvider directly
  // This method makes the requred connections
  void setCloudProvider(boost::optional<CloudProvider> cloudProvider);

  private slots:

  // Initialize the monitor using the current project's cloud session.
  // If no session then make sure the UI indicates the cloud is off.
  // This will only work if the current status is stopped.

  // The CloudMonitorWorker should continuously check that the
  // m_cloudProvider session matches what is in the current project.
  // If there is a descrepency then this slot should be called.
  void initializeCloudSession();

  // We don't want the initialization to block so we 
  // handle the initialization in two parts
  void onCloudSessionInitializeComplete();

  // The CloudMonitorWorker should call this when m_status says running,
  // but the cloud is not available.  This is an indication that something went wrong.
  // Inform with a dialog and try to reinitialize.
  void onCloudConnectionLost();

  // The CloudMonitorWorker should call this when m_status says stopped,
  // but the cloud referred to by the current session is actually available.
  // The UI should reflect that the cloud is running.  This condition might be
  // encountered if the cloud is started up externally, like from AWS interface.
  void onCloudUnexpectedlyStarted();

  // When startup is completely done, set status to CLOUD_RUNNING
  // and enable UI as required.
  void onCloudStartupComplete();

  // For now start server and workers in series, 
  // once the server has started, then start the workers
  void onServerStarted();

  // When all workers have started, and assuming server has already started,
  // call onCloudStartupComplete for final initialization
  void onAllWorkersStarted();

  // Reset the cloud provider, renable UI, etc.
  void onCloudTerminateComplete();

  private:

  Q_DISABLE_COPY(CloudMonitor);

  // Temporary settings for development
  CloudSettings createTestSettings() const;

  // Set m_status and make sure the button visually shows that status
  // CloudMonitorWorker will make sure m_status is accurate
  void setStatus(CloudStatus status);

  void startCloud();

  bool m_serverStarted;

  bool m_allWorkersStarted;

  boost::optional<CloudProvider> m_cloudProvider;

  QSharedPointer<QThread> m_workerThread;

  QSharedPointer<CloudMonitorWorker> m_worker;

  QSharedPointer<QThread> m_initializeCloudThread;

  QSharedPointer<InitializeCloudWorker> m_initializeCloudWorker;

  CloudStatus m_status;

  friend class CloudMonitorWorker;

  friend class InitializeCloudWorker;
};

// The purpose of this class is to initialize the m_cloudProvider member of
// CloudMonitor and make sure that the UI reflects the proper state.  
// The CloudProvider instance should be initialized based on the project settings, and 
// session.  If there is no session then reset m_cloudProvider
// If the cloud is running, first turn it off.
class InitializeCloudWorker : public QObject
{
  Q_OBJECT

  public:

  InitializeCloudWorker(CloudMonitor * monitor);

  virtual ~InitializeCloudWorker();

  signals:

  void doneWorking();

  public slots:

  void startWorking();

  private:

  Q_DISABLE_COPY(InitializeCloudWorker);

  QPointer<CloudMonitor> m_monitor;  
};

// This class is assigned to its own thread,
// where it will continuously check that the UI is accurately reporting the condition of the cloud.
class CloudMonitorWorker : public QObject
{
  Q_OBJECT

  public:

  CloudMonitorWorker(CloudMonitor * monitor);

  virtual ~CloudMonitorWorker();

  public slots:

  void startWorking();

  signals:

  void cloudConnectionLost();

  void cloudConnectionEstablished();

  void internetAvailable(bool isAvailable);

  void cloudConfigurationChanged();

  private:

  Q_DISABLE_COPY(CloudMonitorWorker);

  // Check CloudMonitor status,
  // if status is CLOUD_RUNNING then make sure it is still running
  // if status is CLOUD_RUNNING but test does not agree then 
  // emit cloudConnectionLost
  // Similarly, if status is CLOUD_STOPPED, but the cloud was in fact found to be running,
  // then emit cloudConnectionEstablished
  void checkForCloudConnection();

  // Check the status of the internet connection and emit internetAvailable
  void checkForInternetConnection();

  // See if there is a current session and that it is the same
  // session that we are monitoring.  
  // If it is not then emit cloudSessionChanged
  void checkForNewSession();

  QPointer<CloudMonitor> m_monitor;  
};

} // pat

} // openstudio

#endif // OPENSTUDIO_CLOUDMONITOR_HPP

