/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "BCLComponent.hpp"
#include "../core/System.hpp"
#include "../core/FilesystemHelpers.hpp"

#include <boost/lexical_cast.hpp>
#include <pugixml.hpp>

#include <algorithm>

namespace openstudio {

// New component
BCLComponent::BCLComponent() : m_uid(toString(openstudio::createUUID())) {}

// Downloaded component from component.xml
BCLComponent::BCLComponent(const openstudio::path& dir) : m_directory(dir) {
  pugi::xml_document component;
  component.load_file((m_directory / "component.xml").c_str());  // No checking?

  auto comp = component.child("component");
  m_name = comp.child("name").text().as_string();
  std::replace(m_name.begin(), m_name.end(), '_', ' ');
  // m_name = comp.firstChildElement("name").firstChild().nodeValue().replace("_", " ").toStdString();
  m_uid = comp.child("uid").text().as_string();
  m_versionId = comp.child("version_id").text().as_string();
  m_description = comp.child("description").text().as_string();

  auto files = comp.child("files");
  if (files) {
    for (auto& componentElement : files.children("file")) {
      if (componentElement.first_child() != nullptr) {
        m_files.push_back(componentElement.child("filename").text().as_string());
        m_filetypes.push_back(componentElement.child("filetype").text().as_string());
      } else {
        break;
      }
    }
  }

  auto attributes = comp.child("attributes");
  if (attributes) {
    for (auto& componentElement : attributes.children("attribute")) {
      if (componentElement.first_child() != nullptr) {
        std::string name = componentElement.child("name").text().as_string();
        std::string value = componentElement.child("value").text().as_string();
        std::string datatype = componentElement.child("datatype").text().as_string();

        // Units are optional
        std::string units = componentElement.child("units").text().as_string();

        if (datatype == "float") {
          double castVal;
          try {
            castVal = boost::lexical_cast<double>(value);
          } catch (const boost::bad_lexical_cast&) {
            LOG_AND_THROW("Please double check your XML, you have an attribute named '" << name << "' with a datatype of '" << datatype
                                                                                        << "' but the value isn't a double: '" << value << "'.");
          }
          if (units.empty()) {
            m_attributes.emplace_back(name, castVal);
          } else {
            m_attributes.emplace_back(name, castVal, units);
          }
        } else if (datatype == "int") {
          int castVal;
          try {
            castVal = boost::lexical_cast<int>(value);
          } catch (const boost::bad_lexical_cast&) {
            LOG_AND_THROW("Please double check your XML, you have an attribute named '" << name << "' with a datatype of '" << datatype
                                                                                        << "' but the value isn't an integer: '" << value << "'.");
          }
          if (units.empty()) {
            m_attributes.emplace_back(name, castVal);
          } else {
            m_attributes.emplace_back(name, castVal, units);
          }

        } else if (datatype == "boolean") {
          bool temp;
          if (value == "true") {
            temp = true;
          } else {
            temp = false;
          }
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
            Attribute attr(name, value, units);
            m_attributes.emplace_back(name, value, units);
          }
        }
      } else {
        break;
      }
    }
  }
}

std::string BCLComponent::uid() const {
  return m_uid;
}

std::string BCLComponent::versionId() const {
  return m_versionId;
}

std::string BCLComponent::name() const {
  return m_name;
}

std::string BCLComponent::description() const {
  return m_description;
}

std::vector<std::string> BCLComponent::files() const {
  return m_files;
}

std::vector<std::string> BCLComponent::files(const std::string& filetype) const {
  std::vector<std::string> matches;
  for (unsigned i = 0; i < m_filetypes.size(); i++) {
    if (m_filetypes[i] == filetype) {
      matches.push_back(toString(directory() / "files" / m_files[i]));
    }
  }
  return matches;
}

std::vector<std::string> BCLComponent::filetypes() const {
  return m_filetypes;
}

std::vector<Attribute> BCLComponent::attributes() const {
  return m_attributes;
}

openstudio::path BCLComponent::directory() const {
  return m_directory;
}

void BCLComponent::setName(const std::string& name) {
  m_name = name;
}

bool BCLComponent::operator==(const BCLComponent& other) const {
  return ((m_uid == other.m_uid) && (m_versionId == other.m_versionId));
}

bool BCLComponent::operator!=(const BCLComponent& other) const {
  return !(operator==(other));
}

bool BCLComponent::operator<(const BCLComponent& other) const {
  return (uid() + versionId()) < (other.uid() + other.versionId());
}

}  // namespace openstudio
