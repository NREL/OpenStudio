/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "System.hpp"
#include <OpenStudio.hxx>

#include <boost/lexical_cast.hpp>

#include <regex>

using namespace utility::conversions;

namespace openstudio {

UpdateManager::UpdateManager(const std::string& appName) : UpdateManager(appName, updateUrl(appName)) {}

UpdateManager::UpdateManager(const std::string& appName, const std::string& url)
  : m_appName(appName),
    m_finished(false),
    m_error(false),
    m_newMajorRelease(false),
    m_newMinorRelease(false),
    m_newPatchRelease(false),
    m_mostRecentVersion(openStudioVersion()) {

  // TODO: I don't see it being used anywhere, so not sure if it's trying to use SSL or not
  web::http::client::http_client_config config;
  config.set_validate_certificates(false);

  web::http::client::http_client client(to_string_t(url), config);
  m_httpResponse = client.request(web::http::methods::GET)
                     .then([](web::http::http_response resp) { return resp.extract_utf8string(); })
                     .then([this](const std::string& xml) { processReply(xml); });
}

std::string UpdateManager::appName() const {
  return m_appName;
}

bool UpdateManager::waitForFinished(int msec) const {
  int msecPerLoop = 20;
  int numTries = msec / msecPerLoop;
  int current = 0;
  while (true) {
    // if no request was made and the optional is empty return
    if (!m_httpResponse) {
      return false;
    }

    if (m_httpResponse->is_done()) {
      return true;
    }

    System::msleep(msecPerLoop);

    if (current > numTries) {
      LOG(Error, "waitForFinished timeout");
      break;
    }

    ++current;
  }

  return false;
}

bool UpdateManager::finished() const {
  return m_finished;
}

bool UpdateManager::error() const {
  return m_error;
}

bool UpdateManager::newMajorRelease() const {
  return m_newMajorRelease;
}

bool UpdateManager::newMinorRelease() const {
  return m_newMinorRelease;
}

bool UpdateManager::newPatchRelease() const {
  return m_newPatchRelease;
}

std::string UpdateManager::mostRecentVersion() const {
  return m_mostRecentVersion;
}

std::string UpdateManager::mostRecentDownloadUrl() const {
  return m_mostRecentDownloadUrl;
}

std::vector<std::string> UpdateManager::updateMessages() const {
  return m_updateMessages;
}

std::string UpdateManager::updateUrl(const std::string& appName) {
  const std::string url("https://www.openstudio.net");

  web::uri_builder builder(U("/update.html"));

  builder.append_query(U("app"), to_string_t(appName));
  builder.append_query(U("version"), to_string_t(openStudioVersion()));

  return url + toString(builder.to_string());
}

void UpdateManager::processReply(const std::string& reply) {
  m_error = false;

  if (!m_error) {
    // create xml document to read the response
    pugi::xml_document document;
    auto result = document.load_string(reply.c_str());
    if (!result) {
      m_error = true;
      LOG(Error, "Bad XML Response: " << result.description());
    } else {
      auto openstudio = document.document_element().child("openstudio");
      for (const pugi::xml_node& release : openstudio.children("release")) {
        if (!checkRelease(release)) {
          // break if not newer than current
          break;
        }
      }
    }
  }

  m_finished = true;
}

bool UpdateManager::checkRelease(const pugi::xml_node& release) {
  bool updateAvailable = false;

  try {
    std::string version = release.attribute("version").value();
    std::string currentVersion = openStudioVersion();
    std::regex versionRegex("^([0-9]+)\\.([0-9]+)\\.([0-9]+).*?");

    std::smatch versionMatch;
    std::smatch currentVersionMatch;
    if (std::regex_search(version, versionMatch, versionRegex) && std::regex_search(currentVersion, currentVersionMatch, versionRegex)) {
      auto versionMajorString = std::string(versionMatch[1].first, versionMatch[1].second);
      auto versionMinorString = std::string(versionMatch[2].first, versionMatch[2].second);
      auto versionPatchString = std::string(versionMatch[3].first, versionMatch[3].second);

      auto versionMajor = boost::lexical_cast<unsigned>(versionMajorString);
      auto versionMinor = boost::lexical_cast<unsigned>(versionMinorString);
      auto versionPatch = boost::lexical_cast<unsigned>(versionPatchString);

      auto currentVersionMajorString = std::string(currentVersionMatch[1].first, currentVersionMatch[1].second);
      auto currentVersionMinorString = std::string(currentVersionMatch[2].first, currentVersionMatch[2].second);
      auto currentVersionPatchString = std::string(currentVersionMatch[3].first, currentVersionMatch[3].second);

      auto currentVersionMajor = boost::lexical_cast<unsigned>(currentVersionMajorString);
      auto currentVersionMinor = boost::lexical_cast<unsigned>(currentVersionMinorString);
      auto currentVersionPatch = boost::lexical_cast<unsigned>(currentVersionPatchString);

      if (versionMajor > currentVersionMajor) {
        m_newMajorRelease = true;
        updateAvailable = true;
      } else if (versionMajor == currentVersionMajor) {
        if (versionMinor > currentVersionMinor) {
          m_newMinorRelease = true;
          updateAvailable = true;
        } else if (versionMinor == currentVersionMinor) {
          if (versionPatch > currentVersionPatch) {
            m_newPatchRelease = true;
            updateAvailable = true;
          }
        }
      }

      if (updateAvailable) {
        // only set download url to most recent (e.g. first) release
        if (m_updateMessages.empty()) {
          m_mostRecentVersion = version;
          m_mostRecentDownloadUrl = release.attribute("download").value();
        }
        // add messages from all releases newer than current
        m_updateMessages.push_back(release.first_child().value());
      }
    }
  } catch (const std::exception& e) {
    LOG(Error, e.what());
  }

  return updateAvailable;
}

}  // namespace openstudio
