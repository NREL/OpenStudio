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

#include "UpdateManager.hpp"
#include "Assert.hpp"
#include "Application.hpp"
#include <OpenStudio.hxx>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QThread>
#include <QAbstractEventDispatcher>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>

namespace openstudio{

  UpdateManager::UpdateManager(const std::string& appName)
    : m_appName(appName), m_finished(false), m_error(false), 
      m_newMajorRelease(false), m_newMinorRelease(false), m_newPatchRelease(false), 
      m_mostRecentVersion(openStudioVersion()), m_manager(new QNetworkAccessManager(this))
  {
    Application::instance().processEvents(); // a kludge to make sure that updatemanager works correctly in a non-application environment on unix

    bool result = connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    OS_ASSERT(result);

    result = connect(this, SIGNAL(processed()), this, SLOT(replyProcessed()));
    OS_ASSERT(result);

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



    bool result = connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    OS_ASSERT(result);
    
    result = connect(this, SIGNAL(processed()), this, SLOT(replyProcessed()));
    OS_ASSERT(result);
  
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
    return std::string("http://openstudio.nrel.gov/update.html?app=") + appName() + std::string("&version=") + openStudioVersion();
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
