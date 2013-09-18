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
#include <QObject>
#include <QSharedPointer>
#include <utilities/core/Path.hpp>
#include <utilities/core/Url.hpp>
#include <utilities/cloud/CloudProvider.hpp>
#include <vector>
#include <map>

class QThread;
class QMutex;

namespace openstudio {

namespace pat {

class CloudMonitorWorker;

class CloudMonitor : public QObject
{
  Q_OBJECT

  public:

  CloudMonitor();

  virtual ~CloudMonitor();

  void setCloudSettings(const CloudSettings & settings);

  CloudSettings cloudSettings() const;

  bool starting();

  bool stopping();

  bool serverRunning();

  signals:

  void internetAvailable(bool isAvailable);

  public slots:

  // If cloud is on then turn off, and vice versa
  void toggleCloud();

  private slots:

  void onCloudStartupComplete();

  void onCloudTerminateComplete();

  void setCloudButtonStatus(ToggleCloudButton::Status status);

  private:

  Q_DISABLE_COPY(CloudMonitor);

  void setCloudProvider(QSharedPointer<CloudProvider> cloudProvider);

  void createTestSettings();

  QSharedPointer<CloudProvider> m_cloudProvider;

  QSharedPointer<QThread> m_workerThread;

  QSharedPointer<CloudMonitorWorker> m_worker;

  QSharedPointer<QMutex> m_mutex;
};

// This class is assigned to its own thread,
// where it will continuously check that the UI is accurately reporting the condition of the cloud.
class CloudMonitorWorker : public QObject
{
  Q_OBJECT

  public:

  CloudMonitorWorker(CloudMonitor * monitor);

  virtual ~CloudMonitorWorker();

  void setCloudSettings(const CloudSettings & settings);

  public slots:

  // Start up a timer
  void clockIn();
   
  signals:

  void cloudStatus(ToggleCloudButton::Status status);

  void internetAvailable(bool isAvailable);

  private slots:

  // Does work when the timer expires
  void nowGetToWork();

  private:

  Q_DISABLE_COPY(CloudMonitorWorker);

  boost::optional<CloudSettings> m_cloudSettings;

  QPointer<CloudMonitor> m_monitor;  
};

} // pat

} // openstudio

#endif // OPENSTUDIO_CLOUDMONITOR_HPP

