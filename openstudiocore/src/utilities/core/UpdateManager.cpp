/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "UpdateManager.hpp"
#include "Assert.hpp"
#include "Application.hpp"
#include <OpenStudio.hxx>

#include <QNetworkReply>
#include <QDomDocument>

#include <boost/lexical_cast.hpp>


namespace openstudio{

  UpdateManager::UpdateManager(const std::string& appName)
    : m_appName(appName), m_finished(false), m_error(false),
      m_newMajorRelease(false), m_newMinorRelease(false), m_newPatchRelease(false),
      m_mostRecentVersion(openStudioVersion()), m_manager(new QNetworkAccessManager(this))
  {
    Application::instance().processEvents(); // a kludge to make sure that updatemanager works correctly in a non-application environment on unix

    connect(m_manager, &QNetworkAccessManager::finished, this, &UpdateManager::replyFinished);

    connect(this, &UpdateManager::processed, this, &UpdateManager::replyProcessed);

    QUrl url(QString::fromStdString(updateUrl()));

    m_request = new QNetworkRequest(url);
    OS_ASSERT(m_request);

    m_reply = m_manager->get(*m_request);
    OS_ASSERT(m_reply);
  }

  UpdateManager::UpdateManager(const std::string& appName, const std::string& url)
    : m_appName(appName), m_finished(false), m_error(false),
      m_newMajorRelease(false), m_newMinorRelease(false), m_newPatchRelease(false),
      m_mostRecentVersion(openStudioVersion()), m_manager(new QNetworkAccessManager(this))
  {
    Application::instance().processEvents(); // a kludge to make sure that updatemanager works correctly in a non-application environment on unix



    connect(m_manager, &QNetworkAccessManager::finished, this, &UpdateManager::replyFinished);

    connect(this, &UpdateManager::processed, this, &UpdateManager::replyProcessed);

    m_request = new QNetworkRequest(QUrl(QString::fromStdString(url)));

    m_reply = m_manager->get(*m_request);
  }

  std::string UpdateManager::appName() const
  {
    return m_appName;
  }

  bool UpdateManager::finished() const
  {
    return m_finished;
  }

  bool UpdateManager::error() const
  {
    return m_error;
  }

  bool UpdateManager::newMajorRelease() const
  {
    return m_newMajorRelease;
  }

  bool UpdateManager::newMinorRelease() const
  {
    return m_newMinorRelease;
  }

  bool UpdateManager::newPatchRelease() const
  {
    return m_newPatchRelease;
  }

  std::string UpdateManager::mostRecentVersion() const
  {
    return m_mostRecentVersion;
  }

  std::string UpdateManager::mostRecentDownloadUrl() const
  {
    return m_mostRecentDownloadUrl;
  }

  std::vector<std::string> UpdateManager::updateMessages() const
  {
    return m_updateMessages;
  }


  std::string UpdateManager::updateUrl() const
  {
    return std::string("https://www.openstudio.net/update.html?app=") + appName() + std::string("&version=") + openStudioVersion();
  }

  void UpdateManager::replyFinished(QNetworkReply* reply)
  {
    // finished after this
    m_finished = true;

    if (reply){
      // don't delete here
      reply->deleteLater();

      m_error = (reply->error() != QNetworkReply::NoError);
      if (!m_error){

        // create xml document to read the response
        QDomDocument document;
        document.setContent(reply->readAll());
        QDomNodeList openstudioelements = document.elementsByTagName("openstudio");

        if (openstudioelements.size() > 0)
        {
          // Only process the first one for now
          QDomNodeList nodes = openstudioelements.at(0).childNodes();

          // all child nodes will be releases
          for(int i = 0; i < nodes.size(); ++i){
            QDomElement release = nodes.at(i).toElement();
            if (!release.isNull()){
              if (!checkRelease(release)){
                // break if not newer than current
                break;
              }
            }
          }
        }
      }else{
        LOG(Error, "QNetworkReply " << reply->error());
      }
    }

    emit processed();
  }

  void UpdateManager::replyProcessed()
  {
  }

  bool UpdateManager::checkRelease(const QDomElement& release)
  {
    bool updateAvailable = false;

    try{

      std::string version = release.attribute("version").toStdString();
      std::string currentVersion = openStudioVersion();
      boost::regex versionRegex("^([0-9]+)\\.([0-9]+)\\.([0-9]+).*?");

      boost::smatch versionMatch;
      boost::smatch currentVersionMatch;
      if( boost::regex_search(version, versionMatch, versionRegex) &&
          boost::regex_search(currentVersion, currentVersionMatch, versionRegex)){
        std::string versionMajorString = std::string(versionMatch[1].first,versionMatch[1].second); boost::trim(versionMajorString);
        std::string versionMinorString = std::string(versionMatch[2].first,versionMatch[2].second); boost::trim(versionMinorString);
        std::string versionPatchString = std::string(versionMatch[3].first,versionMatch[3].second); boost::trim(versionPatchString);

        unsigned versionMajor = boost::lexical_cast<unsigned>(versionMajorString);
        unsigned versionMinor = boost::lexical_cast<unsigned>(versionMinorString);
        unsigned versionPatch = boost::lexical_cast<unsigned>(versionPatchString);

        std::string currentVersionMajorString = std::string(currentVersionMatch[1].first,currentVersionMatch[1].second); boost::trim(currentVersionMajorString);
        std::string currentVersionMinorString = std::string(currentVersionMatch[2].first,currentVersionMatch[2].second); boost::trim(currentVersionMinorString);
        std::string currentVersionPatchString = std::string(currentVersionMatch[3].first,currentVersionMatch[3].second); boost::trim(currentVersionPatchString);

        unsigned currentVersionMajor = boost::lexical_cast<unsigned>(currentVersionMajorString);
        unsigned currentVersionMinor = boost::lexical_cast<unsigned>(currentVersionMinorString);
        unsigned currentVersionPatch = boost::lexical_cast<unsigned>(currentVersionPatchString);

        if (versionMajor > currentVersionMajor){
          m_newMajorRelease = true;
          updateAvailable = true;
        }else if(versionMajor == currentVersionMajor){
          if (versionMinor > currentVersionMinor){
            m_newMinorRelease = true;
            updateAvailable = true;
          }else if(versionMinor == currentVersionMinor){
            if (versionPatch > currentVersionPatch){
              m_newPatchRelease = true;
              updateAvailable = true;
            }
          }
        }

        if (updateAvailable){
          // only set download url to most recent (e.g. first) release
          if (m_updateMessages.empty()){
            m_mostRecentVersion = version;
            m_mostRecentDownloadUrl = release.attribute("download").toStdString();
          }
          // add messages from all releases newer than current
          m_updateMessages.push_back(release.firstChild().toCDATASection().data().toStdString());
        }
      }
    }catch(const std::exception& e){
      LOG(Error, e.what());
    }

    return updateAvailable;
  }


} // openstudio
