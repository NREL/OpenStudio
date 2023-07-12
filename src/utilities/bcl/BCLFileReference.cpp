/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "BCLFileReference.hpp"
#include "../core/Checksum.hpp"
#include "../core/StringHelpers.hpp"

#include <pugixml.hpp>
#include <json/json.h>

namespace openstudio {

BCLFileReference::BCLFileReference(const openstudio::path& measureRootDir, const openstudio::path& relativePath, const bool setMembers)
  : m_measureRootDir(openstudio::filesystem::system_complete(measureRootDir)),
    m_path(openstudio::filesystem::system_complete(measureRootDir / relativePath)) {
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
    } else if ((fileType == "rb") || (fileType == "py")) {
      m_softwareProgram = "OpenStudio";
      //m_softwareProgramVersion = "";
    }
  }
}

openstudio::path BCLFileReference::path() const {
  return m_path;
}

openstudio::path BCLFileReference::relativePath() const {
  return openstudio::filesystem::relative(m_path, m_measureRootDir);
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

  std::string usageType = this->usageType();
  openstudio::path baseDir = m_measureRootDir;
  if (usageType == "doc") {
    baseDir /= "docs";
  } else if (usageType == "resource") {
    baseDir /= "resources";
  } else if (usageType == "test") {
    baseDir /= "tests";
  }

  return toString(openstudio::filesystem::relative(m_path, baseDir));
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

Json::Value BCLFileReference::toJSON() const {
  Json::Value root;
  if (m_usageType == "script" && !m_softwareProgram.empty() && !m_softwareProgramVersion.empty()) {
    auto& versionElement = root["version"];

    versionElement["software_program"] = m_softwareProgram;
    versionElement["identifier"] = m_softwareProgramVersion;

    if (m_minCompatibleVersion) {
      versionElement["min_compatible"] = m_minCompatibleVersion->str();
    }

    if (m_maxCompatibleVersion) {
      versionElement["max_compatible"] = m_maxCompatibleVersion->str();
    }
  }

  root["filename"] = fileName();
  root["filetype"] = fileType();
  root["usage_type"] = m_usageType;
  root["checksum"] = m_checksum;

  return root;
}

std::string BCLFileReference::toJSONString() const {
  return toJSON().toStyledString();
}

bool BCLFileReference::checkForUpdate() {
  std::string newChecksum = openstudio::checksum(this->path());
  if (m_checksum != newChecksum) {
    m_checksum = newChecksum;
    return true;
  }
  return false;
}

// bool operator==(const BCLFileReference& lhs, const BCLFileReference& rhs) {
//   return lhs.m_path == rhs.m_path;
// }
//
// std::strong_ordering operator<=>(const BCLFileReference& lhs, const BCLFileReference& rhs) {
//   if (lhs.m_path < rhs.m_path) {
//     return std::strong_ordering::less;
//   } else if (lhs.m_path == rhs.m_path) {
//     return std::strong_ordering::equal;
//   }
//   return std::strong_ordering::greater;
// }

std::ostream& operator<<(std::ostream& os, const BCLFileReference& file) {
  pugi::xml_document doc;
  auto element = doc.append_child("File");
  file.writeValues(element);

  doc.save(os, "  ");
  return os;
}

}  // namespace openstudio
