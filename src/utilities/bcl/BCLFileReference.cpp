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

#include "BCLFileReference.hpp"
#include "../core/Checksum.hpp"
#include "../core/StringHelpers.hpp"

#include <pugixml.hpp>

namespace openstudio {

BCLFileReference::BCLFileReference(const openstudio::path& path, const bool setMembers) : m_path(openstudio::filesystem::system_complete(path)) {
  // DLM: why would you not want to set the members?
  if (setMembers) {
    m_checksum = openstudio::checksum(m_path);

    std::string fileType = this->fileType();
    if (fileType == "osm") {
      m_softwareProgram = "OpenStudio";
      //m_softwareProgramVersion = "";
    } else if (fileType == "osc") {
      m_softwareProgram = "OpenStudio";
      //m_softwareProgramVersion = "";
    } else if (fileType == "idf") {
      m_softwareProgram = "EnergyPlus";
      //m_softwareProgramVersion = "";
    } else if (fileType == "rb") {
      m_softwareProgram = "OpenStudio";
      //m_softwareProgramVersion = "";
    }
  }
}

BCLFileReference::~BCLFileReference() {}

openstudio::path BCLFileReference::path() const {
  return m_path;
}

std::string BCLFileReference::checksum() const {
  return m_checksum;
}

std::string BCLFileReference::softwareProgram() const {
  return m_softwareProgram;
}

std::string BCLFileReference::softwareProgramVersion() const {
  return m_softwareProgramVersion;
}

boost::optional<VersionString> BCLFileReference::minCompatibleVersion() const {
  return m_minCompatibleVersion;
}

boost::optional<VersionString> BCLFileReference::maxCompatibleVersion() const {
  return m_maxCompatibleVersion;
}

std::string BCLFileReference::fileName() const {
  return toString(m_path.filename());
}

std::string BCLFileReference::fileType() const {
  return openstudio::replace(openstudio::toString(m_path.extension()), ".", "");
}

std::string BCLFileReference::usageType() const {
  return m_usageType;
}

void BCLFileReference::setChecksum(const std::string& checksum) {
  m_checksum = checksum;
}

void BCLFileReference::setSoftwareProgram(const std::string& softwareProgram) {
  m_softwareProgram = softwareProgram;
}

void BCLFileReference::setSoftwareProgramVersion(const std::string& softwareProgramVersion) {
  m_softwareProgramVersion = softwareProgramVersion;
}

void BCLFileReference::setMinCompatibleVersion(const VersionString& minCompatibleVersion) {
  m_minCompatibleVersion = minCompatibleVersion;
}

void BCLFileReference::resetMinCompatibleVersion() {
  m_minCompatibleVersion.reset();
}

void BCLFileReference::setMaxCompatibleVersion(const VersionString& maxCompatibleVersion) {
  m_maxCompatibleVersion = maxCompatibleVersion;
}

void BCLFileReference::resetMaxCompatibleVersion() {
  m_maxCompatibleVersion.reset();
}

void BCLFileReference::setUsageType(const std::string& usageType) {
  m_usageType = usageType;
}

void BCLFileReference::writeValues(pugi::xml_node& element) const {
  if (m_usageType == "script" && !m_softwareProgram.empty() && !m_softwareProgramVersion.empty()) {
    auto versionElement = element.append_child("version");

    auto subelement = versionElement.append_child("software_program");
    auto text = subelement.text();
    text.set(m_softwareProgram.c_str());

    subelement = versionElement.append_child("identifier");
    text = subelement.text();
    text.set(m_softwareProgramVersion.c_str());

    if (m_minCompatibleVersion) {
      subelement = versionElement.append_child("min_compatible");
      text = subelement.text();
      text.set(m_minCompatibleVersion->str().c_str());
    }

    if (m_maxCompatibleVersion) {
      subelement = versionElement.append_child("max_compatible");
      text = subelement.text();
      text.set(m_maxCompatibleVersion->str().c_str());
    }
  }

  auto subelement = element.append_child("filename");
  auto text = subelement.text();
  text.set(fileName().c_str());  // careful to write out function result instead of member

  subelement = element.append_child("filetype");
  text = subelement.text();
  text.set(fileType().c_str());  // careful to write out function result instead of member

  subelement = element.append_child("usage_type");
  text = subelement.text();
  text.set(m_usageType.c_str());

  subelement = element.append_child("checksum");
  text = subelement.text();
  text.set(m_checksum.c_str());
}

bool BCLFileReference::checkForUpdate() {
  std::string newChecksum = openstudio::checksum(this->path());
  if (m_checksum != newChecksum) {
    m_checksum = newChecksum;
    return true;
  }
  return false;
}

std::ostream& operator<<(std::ostream& os, const BCLFileReference& file) {
  pugi::xml_document doc;
  auto element = doc.append_child("File");
  file.writeValues(element);

  doc.save(os, "  ");
  return os;
}

}  // namespace openstudio
