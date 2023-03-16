/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "BCLXML.hpp"

#include "../units/Quantity.hpp"
#include "../core/Checksum.hpp"
#include "../core/FilesystemHelpers.hpp"
#include "../time/DateTime.hpp"

#include <sstream>
#include <pugixml.hpp>

namespace openstudio {

BCLXML::BCLXML(const BCLXMLType& bclXMLType)
  : m_bclXMLType(bclXMLType),
    m_uid{removeBraces(openstudio::createUUID())},
    m_versionId{removeBraces(openstudio::createUUID())},
    m_versionModified{DateTime::nowUTC().toISO8601()} {}

BCLXML::BCLXML(const openstudio::path& xmlPath) : m_path(openstudio::filesystem::system_complete(xmlPath)) {
  if (!openstudio::filesystem::exists(xmlPath)) {
    LOG_AND_THROW("'" << openstudio::toString(xmlPath) << "' does not exist");
  } else if (!openstudio::filesystem::is_regular_file(xmlPath)) {
    LOG_AND_THROW("'" << openstudio::toString(xmlPath) << "' cannot be opened for reading BCL XML data");
  }

  pugi::xml_document bclXML;

  openstudio::filesystem::ifstream file(m_path);
  if (file.is_open()) {
    auto result = bclXML.load(file);
    if (!result) {
      LOG_AND_THROW("'" << openstudio::toString(xmlPath) << "' could not be read as XML data");
    }

    file.close();
  } else {
    file.close();
    LOG_AND_THROW("'" << openstudio::toString(xmlPath) << "' could not be opened");
  }

  auto element = bclXML.child("component");
  if (element) {
    m_bclXMLType = BCLXMLType::ComponentXML;
  } else {
    element = bclXML.child("measure");
    if (element) {
      m_bclXMLType = BCLXMLType::MeasureXML;
    } else {
      LOG_AND_THROW("'" << openstudio::toString(xmlPath) << "' is not a correct BCL XML");
    }
  }

  // get schema version to see if we need to upgrade anything
  // added in schema version 3
  VersionString startingVersion("2.0");
  auto subelement = element.child("schema_version");
  if (subelement) {
    try {
      startingVersion = VersionString(subelement.text().as_string());
    } catch (const std::exception&) {
      // Yuck
    }
  }

  m_name = decodeString(element.child("name").text().as_string());

  m_uid = element.child("uid").text().as_string();

  m_versionId = element.child("version_id").text().as_string();

  if (m_name.empty() || m_uid.empty() || m_versionId.empty()) {
    LOG_AND_THROW("'" << openstudio::toString(xmlPath) << "' is not a correct BCL XML");
  }

  // added in schema version 3
  // error is only present if something went wrong, so we check if it exists first
  // to avoid always initializing m_error to a string (even if empty)
  // note: decodeString(element.child("error").text().as_string()) would always return a string even if 'error' key didn't exist
  const pugi::xml_node errorElement = element.child("error");
  if (errorElement) {
    m_error = errorElement.text().as_string();
  }

  subelement = element.child("version_modified");
  if (subelement) {
    m_versionModified = subelement.text().as_string();
    if (!DateTime::fromISO8601(m_versionModified)) {
      // not an allowable date time
      m_versionModified = "";
    }
  }

  if (m_bclXMLType == BCLXMLType::MeasureXML) {
    // added in schema version 3
    m_className = decodeString(element.child("class_name").text().as_string());
  }

  // added in schema version 3
  m_displayName = decodeString(element.child("display_name").text().as_string());
  if (m_displayName.empty()) {
    // use name
    m_displayName = m_name;
  }

  m_description = decodeString(element.child("description").text().as_string());

  if (m_bclXMLType == BCLXMLType::MeasureXML) {
    m_modelerDescription = decodeString(element.child("modeler_description").text().as_string());

    subelement = element.child("arguments");
    if (subelement) {
      for (auto& arg : subelement.children("argument")) {
        try {
          m_arguments.emplace_back(arg);
        } catch (const std::exception&) {
          LOG(Error, "Bad argument in BCL XML");
        }
      }
    }

    subelement = element.child("outputs");
    if (subelement) {
      for (auto& outputElement : subelement.children("output")) {
        if (outputElement.first_child() != nullptr) {
          try {
            m_outputs.emplace_back(outputElement);
          } catch (const std::exception&) {
            LOG(Error, "Bad output in BCL XML");
          }
        }
      }
    }
  }

  subelement = element.child("files");
  if (subelement) {
    for (auto& fileElement : subelement.children("file")) {
      if (fileElement.first_child() != nullptr) {

        std::string softwareProgram;
        std::string softwareProgramVersion;
        boost::optional<VersionString> minCompatibleVersion;
        boost::optional<VersionString> maxCompatibleVersion;
        auto versionElement = fileElement.child("version");
        if (versionElement) {
          softwareProgram = versionElement.child("software_program").text().as_string();
          softwareProgramVersion = versionElement.child("identifier").text().as_string();

          // added in schema version 3
          auto minCompatibleVersionElement = versionElement.child("min_compatible");
          if (!minCompatibleVersionElement) {
            try {
              // if minCompatibleVersion not explicitly set, assume softwareProgramVersion is min
              minCompatibleVersion = VersionString(softwareProgramVersion);
            } catch (const std::exception&) {
            }
          } else {
            try {
              minCompatibleVersion = VersionString(minCompatibleVersionElement.text().as_string());
            } catch (const std::exception&) {
            }
          }

          // added in schema version 3
          auto maxCompatibleVersionElement = versionElement.child("max_compatible");
          if (maxCompatibleVersionElement) {
            try {
              maxCompatibleVersion = VersionString(maxCompatibleVersionElement.text().as_string());
            } catch (const std::exception&) {
            }
          }
        }
        const std::string fileName = fileElement.child("filename").text().as_string();
        //std::string fileType = fileElement.firstChildElement("filetype").firstChild().nodeValue().toStdString();
        const std::string usageType = fileElement.child("usage_type").text().as_string();
        const std::string checkSum = fileElement.child("checksum").text().as_string();

        openstudio::path relativePath;

        if (usageType == "script") {
          relativePath = toPath(fileName);
        } else if (usageType == "doc") {
          relativePath = toPath("docs") / toPath(fileName);
        } else if (usageType == "test") {
          relativePath = toPath("tests") / toPath(fileName);
        } else if (usageType == "resource") {
          relativePath = toPath("resources") / toPath(fileName);
        } else {
          relativePath = toPath(fileName);
        }

        BCLFileReference fileref(m_path.parent_path(), relativePath);
        fileref.setSoftwareProgram(softwareProgram);
        fileref.setSoftwareProgramVersion(softwareProgramVersion);
        if (minCompatibleVersion) {
          fileref.setMinCompatibleVersion(*minCompatibleVersion);
        }
        if (maxCompatibleVersion) {
          fileref.setMaxCompatibleVersion(*maxCompatibleVersion);
        }
        fileref.setUsageType(usageType);
        fileref.setChecksum(checkSum);

        m_files.push_back(fileref);

      } else {
        break;
      }
    }
  }

  subelement = element.child("attributes");
  if (subelement) {
    for (auto& attributeElement : subelement.children("attribute")) {
      if (attributeElement.first_child() != nullptr) {
        std::string name = attributeElement.child("name").text().as_string();
        std::string value = attributeElement.child("value").text().as_string();
        std::string datatype = attributeElement.child("datatype").text().as_string();

        // Units are optional
        std::string units = attributeElement.child("units").text().as_string();

        if (datatype == "float") {
          if (units.empty()) {
            m_attributes.emplace_back(name, boost::lexical_cast<double>(value));
          } else {
            m_attributes.emplace_back(name, boost::lexical_cast<double>(value), units);
          }
        } else if (datatype == "int") {
          if (units.empty()) {
            m_attributes.emplace_back(name, boost::lexical_cast<int>(value));
          } else {
            m_attributes.emplace_back(name, boost::lexical_cast<int>(value), units);
          }
        } else if (datatype == "boolean") {
          const bool temp = (value == "true");
          if (units.empty()) {
            m_attributes.emplace_back(name, temp);
          } else {
            m_attributes.emplace_back(name, temp, units);
          }
        } else {
          // Assume string
          if (units.empty()) {
            m_attributes.emplace_back(name, value);
          } else {
            m_attributes.emplace_back(name, value, units);
          }
        }
      } else {
        break;
      }
    }
  }

  subelement = element.child("tags");
  if (subelement) {
    for (auto& tagElement : subelement.children("tag")) {
      auto text = tagElement.text();
      if (!text.empty()) {
        m_tags.emplace_back(text.as_string());
      }
    }
  }

  // added in schema version 3
  m_xmlChecksum = element.child("xml_checksum").text().as_string();
}

boost::optional<BCLXML> BCLXML::load(const openstudio::path& xmlPath) {
  boost::optional<BCLXML> result;
  try {
    result = BCLXML(xmlPath);
  } catch (const std::exception&) {
  }
  return result;
}

std::string BCLXML::escapeString(const std::string& txt) {
  // seems that we don't need to do this anymore
  return txt;

  // http://stackoverflow.com/questions/2083754/why-shouldnt-apos-be-used-to-escape-single-quotes
  // This code was dead already, with the return above.
  // Commented out, but should likely be removed
  // QString result = toQString(txt);//.replace("'", "#x27;");
  // return result.toStdString();
}

std::string BCLXML::decodeString(const std::string& txt) {
  // seems that we don't need to do this anymore
  // only thing that can't be in the text node on disk are '<' (should be '&lt;') and '&' (should be '&amp;')
  return txt;

  // This code was dead already, with the return above.
  // Commented out, but should likely be removed
  // QString result = toQString(txt);//.replace("#x27;", "'");
  // return result.toStdString();
}

std::string BCLXML::uid() const {
  return m_uid;
}

std::string BCLXML::versionId() const {
  return m_versionId;
}

boost::optional<DateTime> BCLXML::versionModified() const {
  boost::optional<DateTime> result;
  if (!m_versionModified.empty()) {
    result = DateTime::fromISO8601(m_versionModified);
  }
  return result;
}

std::string BCLXML::xmlChecksum() const {
  return m_xmlChecksum;
}

std::string BCLXML::name() const {
  return m_name;
}

std::string BCLXML::displayName() const {
  return m_displayName;
}

std::string BCLXML::className() const {
  return m_className;
}

std::string BCLXML::description() const {
  return m_description;
}

std::string BCLXML::modelerDescription() const {
  return m_modelerDescription;
}

std::vector<BCLMeasureArgument> BCLXML::arguments() const {
  return m_arguments;
}

std::vector<BCLMeasureOutput> BCLXML::outputs() const {
  return m_outputs;
}

std::vector<BCLFileReference> BCLXML::files() const {
  return m_files;
}

std::vector<BCLFileReference> BCLXML::files(const std::string& filetype) const {
  std::vector<BCLFileReference> matches;
  for (const BCLFileReference& file : m_files) {
    if (file.fileType() == filetype) {
      matches.push_back(file);
    }
  }
  return matches;
}

std::vector<Attribute> BCLXML::attributes() const {
  return m_attributes;
}

std::vector<Attribute> BCLXML::getAttributes(const std::string& name) const {
  std::vector<Attribute> result;
  for (const Attribute& attribute : m_attributes) {
    if (attribute.name() == name) {
      result.push_back(attribute);
    }
  }
  return result;
}

std::vector<std::string> BCLXML::tags() const {
  return m_tags;
}

openstudio::path BCLXML::path() const {
  return m_path;
}

openstudio::path BCLXML::directory() const {
  return m_path.parent_path();
}

boost::optional<std::string> BCLXML::error() const {
  return m_error;
}

void BCLXML::resetXMLChecksum() {
  incrementVersionId();
  m_xmlChecksum = "00000000";
}

void BCLXML::setError(const std::string& error) {
  incrementVersionId();
  m_error = escapeString(error);
}

void BCLXML::resetError() {
  incrementVersionId();
  m_error.reset();
}

void BCLXML::setName(const std::string& name) {
  incrementVersionId();
  m_name = escapeString(name);
}

void BCLXML::setDisplayName(const std::string& displayName) {
  incrementVersionId();
  m_displayName = escapeString(displayName);
}

void BCLXML::setClassName(const std::string& className) {
  incrementVersionId();
  m_className = escapeString(className);
}

void BCLXML::setDescription(const std::string& description) {
  incrementVersionId();
  m_description = escapeString(description);
}

void BCLXML::setModelerDescription(const std::string& modelerDescription) {
  incrementVersionId();
  m_modelerDescription = escapeString(modelerDescription);
}

void BCLXML::setArguments(const std::vector<BCLMeasureArgument>& arguments) {
  incrementVersionId();
  m_arguments = arguments;
}

void BCLXML::setOutputs(const std::vector<BCLMeasureOutput>& outputs) {
  incrementVersionId();
  m_outputs = outputs;
}

void BCLXML::addFile(const BCLFileReference& file) {
  removeFile(file.path());

  incrementVersionId();
  m_files.push_back(file);
}

bool BCLXML::hasFile(const openstudio::path& path) const {
  bool result = false;

  const openstudio::path test = openstudio::filesystem::system_complete(path);

  for (const BCLFileReference& file : m_files) {
    if (file.path() == test) {
      result = true;
      break;
    }
  }

  return result;
}

bool BCLXML::removeFile(const openstudio::path& path) {
  bool result = false;

  const openstudio::path test = openstudio::filesystem::system_complete(path);

  std::vector<BCLFileReference> newFiles;
  for (const BCLFileReference& file : m_files) {
    if (file.path() == test) {
      result = true;
    } else {
      newFiles.push_back(file);
    }
  }

  if (result) {
    incrementVersionId();
    m_files = newFiles;
  }

  return result;
}

void BCLXML::clearFiles() {
  incrementVersionId();
  m_files.clear();
}

void BCLXML::addAttribute(const Attribute& attribute) {
  incrementVersionId();
  m_attributes.push_back(attribute);
}

bool BCLXML::removeAttributes(const std::string& name) {
  bool result = false;

  std::vector<Attribute> newAttributes;
  for (const Attribute& attribute : m_attributes) {
    if (attribute.name() == name) {
      result = true;
    } else {
      newAttributes.push_back(attribute);
    }
  }

  if (result) {
    incrementVersionId();
    m_attributes = newAttributes;
  }

  return result;
}

void BCLXML::clearAttributes() {
  incrementVersionId();
  m_attributes.clear();
}

void BCLXML::addTag(const std::string& tagName) {
  removeTag(tagName);

  incrementVersionId();
  m_tags.push_back(tagName);
}

bool BCLXML::removeTag(const std::string& tagName) {
  auto it = std::find(m_tags.begin(), m_tags.end(), tagName);
  if (it != m_tags.end()) {
    incrementVersionId();
    m_tags.erase(it);
    return true;
  }

  return false;
}

void BCLXML::clearTags() {
  incrementVersionId();
  m_tags.clear();
}

pugi::xml_document BCLXML::toXML() const {

  pugi::xml_document doc;

  //doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");

  pugi::xml_node docElement;
  if (m_bclXMLType == BCLXMLType::ComponentXML) {
    docElement = doc.append_child("component");
  } else if (m_bclXMLType == BCLXMLType::MeasureXML) {
    docElement = doc.append_child("measure");
  } else {
    LOG_AND_THROW("Unknown BCLXMLType '" << m_bclXMLType.valueName() << "'.");
  }

  auto element = docElement.append_child("schema_version");
  auto text = element.text();
  text.set("3.0");

  if (m_error) {
    element = docElement.append_child("error");
    text = element.text();
    text.set(escapeString(*m_error).c_str());
  }

  element = docElement.append_child("name");
  text = element.text();
  text.set(escapeString(m_name).c_str());

  element = docElement.append_child("uid");
  text = element.text();
  text.set(m_uid.c_str());

  element = docElement.append_child("version_id");
  text = element.text();
  text.set(m_versionId.c_str());

  if (!m_versionModified.empty()) {
    element = docElement.append_child("version_modified");
    text = element.text();
    text.set(m_versionModified.c_str());
  }

  element = docElement.append_child("xml_checksum");
  text = element.text();
  text.set(m_xmlChecksum.c_str());

  if (m_bclXMLType == BCLXMLType::MeasureXML) {
    element = docElement.append_child("class_name");
    text = element.text();
    text.set(m_className.c_str());
  }

  element = docElement.append_child("display_name");
  text = element.text();
  text.set(m_displayName.c_str());

  element = docElement.append_child("description");
  text = element.text();
  text.set(escapeString(m_description).c_str());

  if (m_bclXMLType == BCLXMLType::MeasureXML) {
    element = docElement.append_child("modeler_description");
    text = element.text();
    text.set(escapeString(m_modelerDescription).c_str());

    element = docElement.append_child("arguments");
    for (const BCLMeasureArgument& argument : m_arguments) {
      auto argumentElement = element.append_child("argument");
      argument.writeValues(argumentElement);
    }

    element = docElement.append_child("outputs");
    for (const BCLMeasureOutput& output : m_outputs) {
      auto outputElement = element.append_child("output");
      output.writeValues(outputElement);
    }
  }

  // TODO: write provenances
  element = docElement.append_child("provenances");

  // write tags
  // provenances isn't written so element above is not used... so ignore it here
  // cppcheck-suppress redundantAssignment
  element = docElement.append_child("tags");
  for (const std::string& tag : m_tags) {
    auto tagElement = element.append_child("tag");
    text = tagElement.text();
    text.set(tag.c_str());
  }

  // write attributes
  element = docElement.append_child("attributes");
  for (const Attribute& attribute : m_attributes) {

    std::string value;
    std::string dataType;
    switch (attribute.valueType().value()) {
      case AttributeValueType::Boolean:
        if (attribute.valueAsBoolean()) {
          value = "true";
        } else {
          value = "false";
        }
        dataType = "boolean";
        break;
      case AttributeValueType::Double:
        value = attribute.toString();
        dataType = "float";
        break;
      case AttributeValueType::Integer:
        value = attribute.toString();
        dataType = "integer";
        break;
      case AttributeValueType::Unsigned:
        value = attribute.toString();
        dataType = "unsigned";
        break;
      case AttributeValueType::String:
        value = attribute.toString();
        dataType = "string";
        break;
      case AttributeValueType::AttributeVector:
        // can't handle this yet
        continue;
        break;
      default:
        // can't handle this yet
        continue;
    }

    auto attributeElement = element.append_child("attribute");

    auto subelement = attributeElement.append_child("name");
    text = subelement.text();
    text.set(attribute.name().c_str());

    subelement = attributeElement.append_child("value");
    text = subelement.text();
    text.set(value.c_str());

    subelement = attributeElement.append_child("datatype");
    text = subelement.text();
    text.set(dataType.c_str());

    boost::optional<std::string> units = attribute.units();
    if (units) {
      subelement = attributeElement.append_child("units");
      text = subelement.text();
      text.set((*units).c_str());
    }
  }

  // write files
  element = docElement.append_child("files");
  for (const BCLFileReference& file : m_files) {
    auto subelement = element.append_child("file");
    file.writeValues(subelement);
  }

  return doc;
}

bool BCLXML::save() const {

  if (m_path.empty()) {
    LOG(Warn, "Cannot save, since it has no known path. Use BCLXML::save(const openstudio::path& xmlPath) instead.");
    return false;
  }

  const pugi::xml_document doc = toXML();
  if (!doc) {
    return false;
  }

  // write to disk
  openstudio::filesystem::ofstream file(m_path);
  if (!file.is_open()) {
    return false;
  }

  doc.save(file, "  ");
  file.close();
  return true;
}

bool BCLXML::saveAs(const openstudio::path& xmlPath) {
  incrementVersionId();
  m_path = openstudio::filesystem::system_complete(xmlPath);
  return save();
}

std::string BCLXML::toString() const {

  std::string result;

  const pugi::xml_document doc = toXML();
  if (!doc) {
    return result;
  }

  std::stringstream ss;
  doc.save(ss, "  ");
  result = ss.str();

  return result;
}

std::ostream& operator<<(std::ostream& os, const BCLXML& bclXML) {
  const pugi::xml_document doc = bclXML.toXML();
  if (doc) {
    doc.save(os, "  ");
  }

  return os;
}

void BCLXML::changeUID() {
  m_uid = removeBraces(openstudio::createUUID());
  // DLM: should this call incrementVersionId() ?
}

void BCLXML::incrementVersionId() {
  m_versionId = removeBraces(openstudio::createUUID());
  m_versionModified = DateTime::nowUTC().toISO8601();
}

bool BCLXML::checkForUpdatesXML() {
  std::string newChecksum = computeXMLChecksum();

  if (m_xmlChecksum.empty()) {
    // we are unsure if this is a real change or update from version 2
    // set this here and return false
    m_xmlChecksum = newChecksum;
    return false;
  }

  if (m_xmlChecksum != newChecksum) {
    incrementVersionId();
    m_xmlChecksum = newChecksum;
    return true;
  }

  return false;
}

void printAttributeForChecksum(std::ostream& os, const Attribute& attribute, const std::string& tabs) {
  if (attribute.valueType() == AttributeValueType::AttributeVector) {
    for (const Attribute& child : attribute.valueAsAttributeVector()) {
      printAttributeForChecksum(os, child, tabs + "  ");
    }
  } else {
    os << tabs << "Name: " << attribute.name() << '\n';
    if (attribute.displayName()) {
      os << tabs << "Display Name: " << attribute.displayName().get() << '\n';
    }
    os << tabs << "Value Type: " << attribute.valueType().valueName() << '\n';
    os << tabs << "Value: " << attribute.toString() << '\n';
    if (attribute.units()) {
      os << tabs << "Units: " << attribute.units().get() << '\n';
    }
    os << '\n';
  }
}

std::string BCLXML::computeXMLChecksum() const {
  // DLM: CHANGING THE IMPLEMENTATION OF THIS FUNCTION WILL CAUSE
  // CHECKSUMS TO BE COMPUTED DIFFERENTLY
  // WE WANT TO AVOID FIGHTING WHERE DIFFERENT VERSIONS OF OPENSTUDIO
  // COMPUTE THE CHECKSUM IN DIFFERENT WAYS
  std::stringstream ss;

  // will be picked up when Ruby file changes
  //ss << "Name: " << m_name << '\n';

  // will be picked up when Ruby file changes
  //ss << "Display Name: " << m_displayName << '\n';

  // will be picked up when Ruby file changes
  //ss << "Class Name: " << m_className << '\n';

  // not managed manually
  //ss << m_uid;
  //ss << m_versionId;
  //ss << m_xmlChecksum;

  // will be picked up when Ruby file changes
  //ss << "Description: " << m_description << '\n';

  // will be picked up when Ruby file changes
  //ss << "Modeler Description: " << m_modelerDescription << '\n';

  // will be picked up when Ruby file changes
  //ss << "Arguments: " << '\n';
  //for (const BCLMeasureArgument& argument : m_arguments){
  //  ss << argument << '\n';
  //}

  // will be picked up when checkForUpdatesFiles
  //ss << "Files: " << '\n';
  //for (const BCLFileReference& file : m_files){
  //  ss << file << '\n';
  //}

  // attributes are edited in the xml
  ss << "Attributes: " << '\n';
  for (const Attribute& attribute : m_attributes) {
    //ss << attribute; // can't use this because attributes uuid are regenerated on each load

    // DLM: in the end just create a new method that won't change
    printAttributeForChecksum(ss, attribute, "  ");
  }

  // tags are edited in the xml
  ss << "Tags: " << '\n';
  for (const std::string& tag : m_tags) {
    ss << "  " << tag << '\n';
  }

  //std::cout << "Checksum computed on:" << '\n';
  //std::cout << ss.str() << '\n';

  return checksum(ss);
}

}  // namespace openstudio
