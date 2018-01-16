/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
