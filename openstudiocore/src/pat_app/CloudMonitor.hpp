/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef PATAPP_CLOUDMONITOR_HPP
#define PATAPP_CLOUDMONITOR_HPP

#include "PatConstants.hpp"
#include "RunView.hpp"

#include <QObject>
#include <QSharedPointer>

#include "../utilities/cloud/CloudProvider.hpp"
#include "../utilities/core/Path.hpp"
#include "../utilities/core/Url.hpp"

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

  // Return the settings stored in the App's current project
  static boost::optional<CloudSettings> currentProjectSettings();

  // Set the settings in the App's current project
  static void setCurrentProjectSettings(const boost::optional<CloudSettings> & settings);

  signals:

  // This will be removed.  Use ::status() to get information about the cloud.
  void internetAvailable(bool isAvailable);

  void cloudStatusChanged(const CloudStatus & newStatus);

  public slots:

  // If cloud is on then turn off, and vice versa
  void toggleCloud();

  // Start new cloud session
  void startCloud();

  // Stop current project cloud session
  void stopCloud();

  // Reestablish connection to current project session.
  // Use this when you open a new project that has a running session.
  // This only supports clouds that are stopped or running.
  // Behavior is undefined for clouds that are starting or stopping.
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

  // Reset the cloud provider, enable UI, etc.
  void onStopCloudWorkerComplete();

  void onReconnectCloudWorkerComplete();

  private:

  Q_DISABLE_COPY(CloudMonitor);

  // Set m_status and make sure the button visually shows that status
  // CloudMonitorWorker will make sure m_status is accurate
  void setStatus(CloudStatus status);

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

  bool internetAvailable() const;

  bool validCredentials() const;

  bool resourcesAvailableToStart() const;

  bool error() const;

  std::vector<std::string> errors() const;
    
  std::vector<std::string> warnings() const;

  signals:

  void doneWorking();

  public slots:

  void startWorking();

  private:

  Q_DISABLE_COPY(StartCloudWorker);

  QPointer<CloudMonitor> m_monitor;  

  boost::optional<CloudSettings> m_settings;

  boost::optional<CloudSession> m_session; 

  bool m_internetAvailable;

  bool m_validCredentials;

  bool m_resourcesAvailableToStart;

  bool m_error;

  std::vector<std::string> m_errors;

  std::vector<std::string> m_warnings;
};

class StopCloudWorker : public QObject
{
  Q_OBJECT

  public:

  StopCloudWorker(CloudMonitor * monitor);

  virtual ~StopCloudWorker();

  bool error() const; 

  signals:

  void doneWorking();

  public slots:

  void startWorking();

  private:

  Q_DISABLE_COPY(StopCloudWorker);

  QPointer<CloudMonitor> m_monitor;  

  bool m_error;
};

class ReconnectCloudWorker : public QObject
{
  Q_OBJECT

  public:

  ReconnectCloudWorker(CloudMonitor * monitor);

  virtual ~ReconnectCloudWorker();

  bool internetAvailable() const;

  bool authenticated() const;

  bool cloudRunning() const;

  bool cloudServiceRunning() const;

  bool projectIsOnCloud() const;

  signals:

  void doneWorking();

  public slots:

  void startWorking();

  private:

  Q_DISABLE_COPY(ReconnectCloudWorker);

  QPointer<CloudMonitor> m_monitor;

  bool m_internetAvailable;

  bool m_authenticated;

  bool m_cloudRunning;

  bool m_cloudServiceRunning;

  bool m_projectIsOnCloud;

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

  bool internetAvailable() const;

  bool authenticated() const;

  bool cloudRunning() const;

  bool cloudServiceRunning() const;

  signals:

  void doneWorking();

  public slots:

  void startWorking();

  private:

  Q_DISABLE_COPY(RecoverCloudWorker);

  QPointer<CloudMonitor> m_monitor;

  bool m_internetAvailable;

  bool m_authenticated;

  bool m_cloudRunning;

  bool m_cloudServiceRunning;
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

  QPointer<CloudMonitor> m_monitor;  

  bool m_internetAvailable;

  bool m_authenticated;

  bool m_cloudRunning;

  bool m_cloudServiceRunning;

  int m_count;
};

} // pat

} // openstudio

#endif // PATAPP_CLOUDMONITOR_HPP

