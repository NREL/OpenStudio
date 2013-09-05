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

#include <QObject>
#include <QSharedPointer>
#include <utilities/core/Path.hpp>
#include <utilities/core/Url.hpp>
#include <vector>
#include <map>

namespace openstudio {

class CloudProvider;

namespace pat {

// CloudMonitor encapsulates CloudProvider, forwards signals from CloudProvider,
// and keeps the PatApp GUI/state current with CloudProvider.  CloudMonitor should issue dialogs
// as needed to inform user about what is going on with CloudProvider.
// Since CloudProvider instances might be created and destroyed while PatApp is running,
// connect to signals on CloudMonitor instead of connecting directly to CloudProvider.  
// CloudProvider connections will be broken anytime setCloudProvider is used.  
// Consider this class the glue between the GUI application and the CloudProvider backend.
class CloudMonitor : public QObject
{
  Q_OBJECT

  public:

  QSharedPointer<CloudProvider> cloudProvider() const;

  void setCloudProvider(QSharedPointer<CloudProvider> cloudProvider);

  signals:
    
  void serverStarting();

  void serverStarted(const Url& url);

  void workerStarting();

  void workerStarted(const Url& url);

  void allWorkersStarted();

  void terminating();

  void terminateComplete();

  private:

  QSharedPointer<CloudProvider> m_cloudProvider;
};

} // pat

} // openstudio

#endif // OPENSTUDIO_CLOUDMONITOR_HPP

