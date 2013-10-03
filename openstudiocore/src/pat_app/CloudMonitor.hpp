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

#include "PatConstants.hpp"
#include "RunView.hpp"

#include "../shared_gui_components/LostCloudConnectionDialog.hpp"

#include <QObject>
#include <QSharedPointer>

#include <utilities/cloud/CloudProvider.hpp>
#include <utilities/core/Path.hpp>
#include <utilities/core/Url.hpp>

#include <map>
#include <vector>

class QThread;

namespace openstudio {

namespace pat {

class CloudMonitorWorker;

class StartCloudWorker;

class StopCloudWorker;

class ReconnectCloudWorker;

class RecoverCloudWorker;

class CloudMonitor : public QObject
{
  Q_OBJECT

  public:

  CloudMonitor();

  virtual ~CloudMonitor();

  // A reliable method to retrieve the cloud status
  CloudStatus status() const;

  // Return a new CloudProvider from settings and session.
  static CloudProvider newCloudProvider(const CloudSettings & settings, 
                                        const boost::optional<CloudSession> & session = boost::none);

  // Return the session stored in the App's current project
  static boost::optional<CloudSession> currentProjectSession();

  // Set the session in the App's current project
  static void setCurrentProjectSession(const boost::optional<CloudSession> & session);

  signals:

  // This will be removed.  Use ::status() to get information about the cloud.
  void internetAvailable(bool isAvailable);

  public slots:

  // If cloud is on then turn off, and vice versa
  void toggleCloud();

  // Start new cloud session
  void startCloud();

  // Stop current project cloud session
  void stopCloud();

  // Restablish connection to current project session.
  // Use this when you open a new project that has a running session.
  void reconnectCloud();
  
  private slots:

  // After a cloud connection is lost, this method will try to get things going again.
  // or at least make sure everything is shutdown, such as orphan nodes.
  void recoverCloud();

  void onRecoverCloudWorkerComplete();

  // The CloudMonitorWorker should call this when m_status says running,
  // but the cloud is not available.  This is an indication that something went wrong.
  // Inform with a dialog and try to reinitialize.
  void onCloudConnectionError();

  // When startup is completely done, set status to CLOUD_RUNNING
  // and enable UI as required.
  void onStartCloudWorkerComplete();

  // Reset the cloud provider, renable UI, etc.
  void onStopCloudWorkerComplete();

  void onReconnectCloudWorkerComplete();

  void on_closeLostCloudConnectionDlg();

  private:

  Q_DISABLE_COPY(CloudMonitor);

  // Set m_status and make sure the button visually shows that status
  // CloudMonitorWorker will make sure m_status is accurate
  void setStatus(CloudStatus status);

  void openLostCloudConnectionDlg(bool internetAvailable,
    bool authenticated,
    bool cloudRunning);

  bool lostCloudConnectionDlgClearSession();

  bool m_serverStarted;

  bool m_allWorkersStarted;

  boost::optional<CloudProvider> m_cloudProvider;

  QSharedPointer<QThread> m_workerThread;

  QSharedPointer<CloudMonitorWorker> m_worker;

  QSharedPointer<QThread> m_startCloudThread;

  QSharedPointer<StartCloudWorker> m_startCloudWorker;

  QSharedPointer<QThread> m_stopCloudThread;

  QSharedPointer<StopCloudWorker> m_stopCloudWorker;

  QSharedPointer<QThread> m_reconnectCloudThread;

  QSharedPointer<ReconnectCloudWorker> m_reconnectCloudWorker;

  QSharedPointer<QThread> m_recoverCloudThread;

  QSharedPointer<RecoverCloudWorker> m_recoverCloudWorker;

  QPointer<LostCloudConnectionDialog> m_lostCloudConnectiopnDialog;

  CloudStatus m_status;

  friend class CloudMonitorWorker;
};

class StartCloudWorker : public QObject
{
  Q_OBJECT

  public:

  StartCloudWorker(CloudMonitor * monitor);

  virtual ~StartCloudWorker();

  boost::optional<CloudSettings> settings() const;

  boost::optional<CloudSession> session() const;

  signals:

  void doneWorking();

  public slots:

  void startWorking();

  private:

  Q_DISABLE_COPY(StartCloudWorker);

  QPointer<CloudMonitor> m_monitor;  

  boost::optional<CloudSettings> m_settings;

  boost::optional<CloudSession> m_session; 
};

class StopCloudWorker : public QObject
{
  Q_OBJECT

  public:

  StopCloudWorker(CloudMonitor * monitor);

  virtual ~StopCloudWorker();

  signals:

  void doneWorking();

  public slots:

  void startWorking();

  private:

  Q_DISABLE_COPY(StopCloudWorker);

  QPointer<CloudMonitor> m_monitor;  
};

class ReconnectCloudWorker : public QObject
{
  Q_OBJECT

  public:

  ReconnectCloudWorker(CloudMonitor * monitor);

  virtual ~ReconnectCloudWorker();

  CloudStatus status() const;

  signals:

  void doneWorking();

  public slots:

  void startWorking();

  private:

  Q_DISABLE_COPY(ReconnectCloudWorker);

  QPointer<CloudMonitor> m_monitor;

  CloudStatus m_status;
};

// The purpose of RecoverCloudWorker is to recover a cloud session
// that was reportedly not responding.
class RecoverCloudWorker : public QObject
{
  Q_OBJECT

  public:

  RecoverCloudWorker(CloudMonitor * monitor);

  virtual ~RecoverCloudWorker();

  signals:

  void doneWorking();

  public slots:

  void startWorking();

  private:

  Q_DISABLE_COPY(RecoverCloudWorker);

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

  bool internetAvailable() const;

  bool authenticated() const;

  bool cloudRunning() const;

  bool cloudServiceRunning() const;

  public slots:

  void monitorCloudRunning();

  signals:

  void cloudConnectionError();

  private:

  Q_DISABLE_COPY(CloudMonitorWorker);

  bool checkInternetAvailable() const;

  bool checkAuthenticated() const;

  // Check that server and worker(s) are running
  bool checkCloudRunning() const;

  // Check that the openstudio service is running on the server
  bool checkCloudServiceRunning() const;

  QPointer<CloudMonitor> m_monitor;  

  bool m_internetAvailable;

  bool m_authenticated;

  bool m_cloudRunning;

  bool m_cloudServiceRunning;

  int m_count;
};

} // pat

} // openstudio

#endif // OPENSTUDIO_CLOUDMONITOR_HPP

