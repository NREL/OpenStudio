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

#include "BCLComponent.hpp"
#include "../core/System.hpp"
#include "../core/FilesystemHelpers.hpp"

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
      if (componentElement.first_child()) {
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
      if (componentElement.first_child()) {
        std::string name = componentElement.child("name").text().as_string();
        std::string value = componentElement.child("value").text().as_string();
        std::string datatype = componentElement.child("datatype").text().as_string();

        // Units are optional
        std::string units = componentElement.child("units").text().as_string();

        if (datatype == "float") {
          if (units.empty()) {
            Attribute attr(name, boost::lexical_cast<double>(value));
            m_attributes.push_back(attr);
          } else {
            Attribute attr(name, boost::lexical_cast<double>(value), units);
            m_attributes.push_back(attr);
          }
        } else if (datatype == "int") {
          if (units.empty()) {
            Attribute attr(name, boost::lexical_cast<int>(value));
            m_attributes.push_back(attr);
          } else {
            Attribute attr(name, boost::lexical_cast<int>(value), units);
            m_attributes.push_back(attr);
          }
        } else if (datatype == "boolean") {
          bool temp;
          if (value == "true") {
            temp = true;
          } else {
            temp = false;
          }
          if (units.empty()) {
            Attribute attr(name, temp);
            m_attributes.push_back(attr);
          } else {
            Attribute attr(name, temp, units);
            m_attributes.push_back(attr);
          }
        } else {
          // Assume string
          if (units.empty()) {
            Attribute attr(name, value);
            m_attributes.push_back(attr);
          } else {
            Attribute attr(name, value, units);
            m_attributes.push_back(attr);
          }
        }
      } else {
        break;
      }
    }
  }
}

BCLComponent::~BCLComponent() {}

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

}  // namespace openstudio
