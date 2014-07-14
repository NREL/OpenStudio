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

#ifndef UTILITIES_CORE_UPDATEMANAGER_HPP
#define UTILITIES_CORE_UPDATEMANAGER_HPP

#include "../UtilitiesAPI.hpp"
#include "Logger.hpp"

#include <string>
#include <vector>
#include <QObject>

class QNetworkRequest;
class QNetworkReply;
class QNetworkAccessManager;
class QDomElement;

namespace openstudio {

  /** Class for checking whether a new version of OpenStudio is available
      for download.
  **/
  class UTILITIES_API UpdateManager : public QObject{
    
    Q_OBJECT;
  
  public:

    /// Constructor with application name
    UpdateManager(const std::string& appName);

    /// Constructor with application name and alternate test Url
    UpdateManager(const std::string& appName, const std::string& url);

    // virtual destructor
    virtual ~UpdateManager() {}

    /// returns the application name
    std::string appName() const;

    /// returns true when the manager is finished checking for updates
    bool finished() const;

    /// returns true if an error occurred while checking for updates,
    /// must call after update manager is finished
    bool error() const;

    /// returns true if a new major release is available, manager must have
    /// finished checking for updates with no errors
    bool newMajorRelease() const;

    /// returns true if a new minor release is available, manager must have
    /// finished checking for updates with no errors
    bool newMinorRelease() const;

    /// returns true if a new patch release is available, manager must have
    /// finished checking for updates with no errors
    bool newPatchRelease() const;

    /// returns most recent version, manager must have
    /// finished checking for updates with no errors
    std::string mostRecentVersion() const;

    /// returns url for the most recent download, manager must have
    /// finished checking for updates with no errors
    std::string mostRecentDownloadUrl() const;

    /// returns the description of each update since the current release with the most recent first, 
    /// manager must have finished checking for updates with no errors
    std::vector<std::string> updateMessages() const;
    
  public slots:

    void replyFinished(QNetworkReply* reply);

    // override in ruby classes
    virtual void replyProcessed();

  signals:
  
    void processed();
      
  private:

    REGISTER_LOGGER("openstudio.utilities.UpdateManager");

    std::string m_appName;
    bool m_finished;
    bool m_error;
    bool m_newMajorRelease;
    bool m_newMinorRelease;
    bool m_newPatchRelease;
    std::string m_mostRecentVersion;
    std::string m_mostRecentDownloadUrl;
    std::vector<std::string> m_updateMessages;

    // returns true if release being checked is newer than current release
    bool checkRelease(const QDomElement& release);

    // url used for checking updates
    std::string updateUrl() const;

    QNetworkAccessManager* m_manager;
    QNetworkRequest* m_request;
    QNetworkReply* m_reply;

  };

} // openstudio

#endif // UTILITIES_CORE_UPDATEMANAGER_HPP
