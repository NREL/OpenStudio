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

#include "BCLComponent.hpp"
#include "../core/System.hpp"
#include "../data/Attribute.hpp"
#include "../core/FilesystemHelpers.hpp"

#include <QDomDocument>

namespace openstudio{

  // New component
  BCLComponent::BCLComponent():
    m_uid(toString(openstudio::createUUID()))
  {
  }

  // Downloaded component from component.xml
  BCLComponent::BCLComponent(const std::string& dir):
    m_directory(dir)
  {
    QDomDocument component("component.xml");
    component.setContent(openstudio::filesystem::read_as_QByteArray(openstudio::toPath(m_directory) / "component.xml"));

    QDomElement comp = component.firstChildElement("component");
    m_name = comp.firstChildElement("name").firstChild().nodeValue().replace("_", " ")
      .toStdString();
    m_uid = comp.firstChildElement("uid").firstChild().nodeValue().toStdString();
    m_versionId = comp.firstChildElement("version_id").firstChild().nodeValue().toStdString();
    m_description = comp.firstChildElement("description").firstChild().nodeValue().toStdString();
    QDomElement componentElement = comp.firstChildElement("files").firstChildElement("file");

    while (!componentElement.isNull())
    {
      if (componentElement.hasChildNodes())
      {
        m_files.push_back(componentElement.firstChildElement("filename").firstChild()
          .nodeValue().toStdString());
        m_filetypes.push_back(componentElement.firstChildElement("filetype").firstChild()
          .nodeValue().toStdString());
      }
      else
      {
        break;
      }
      componentElement = componentElement.nextSiblingElement("file");
    }
    componentElement = comp.firstChildElement("attributes").firstChildElement("attribute");
    while (!componentElement.isNull())
    {
      if (componentElement.hasChildNodes())
      {
        std::string name = componentElement.firstChildElement("name").firstChild()
          .nodeValue().toStdString();
        std::string value = componentElement.firstChildElement("value").firstChild()
          .nodeValue().toStdString();
        std::string datatype = componentElement.firstChildElement("datatype").firstChild()
          .nodeValue().toStdString();

        // Units are optional
        std::string units = componentElement.firstChildElement("units").firstChild()
          .nodeValue().toStdString();

        if (datatype == "float"){
          if (units.empty()){
            Attribute attr(name, boost::lexical_cast<double>(value));
            m_attributes.push_back(attr);
          }else{
            Attribute attr(name, boost::lexical_cast<double>(value), units);
            m_attributes.push_back(attr);
          }
        }else if (datatype == "int"){
          if (units.empty()){
            Attribute attr(name, boost::lexical_cast<int>(value));
            m_attributes.push_back(attr);
          }else{
            Attribute attr(name, boost::lexical_cast<int>(value), units);
            m_attributes.push_back(attr);
          }
        }else if (datatype == "boolean"){
          bool temp;
          if (value == "true"){
            temp = true;
          }else{
            temp = false;
          }
          if (units.empty()){
            Attribute attr(name, temp);
            m_attributes.push_back(attr);
          }else{
            Attribute attr(name, temp, units);
            m_attributes.push_back(attr);
          }
        }else{
          // Assume string
          if (units.empty()){
            Attribute attr(name, value);
            m_attributes.push_back(attr);
          }else{
            Attribute attr(name, value, units);
            m_attributes.push_back(attr);
          }
        }
      }
      else
      {
        break;
      }
      componentElement = componentElement.nextSiblingElement("attribute");
    }
  }

  BCLComponent::~BCLComponent()
  {
  }

  std::string BCLComponent::uid() const
  {
    return m_uid;
  }

  std::string BCLComponent::versionId() const
  {
    return m_versionId;
  }

  std::string BCLComponent::name() const
  {
    return m_name;
  }

  std::string BCLComponent::description() const
  {
    return m_description;
  }

  std::vector<std::string> BCLComponent::files() const
  {
    return m_files;
  }

  std::vector<std::string> BCLComponent::files(std::string filetype) const
  {
    std::vector<std::string> matches;
    for (unsigned i=0; i< m_filetypes.size(); i++)
    {
      if (m_filetypes[i]==filetype)
      {
        matches.push_back(directory()+"/files/"
          +m_files[i]);
      }
    }
    return matches;
  }

  std::vector<std::string> BCLComponent::filetypes() const
  {
    return m_filetypes;
  }

  std::vector<Attribute> BCLComponent::attributes() const
  {
    return m_attributes;
  }

  std::string BCLComponent::directory() const
  {
    return m_directory;
  }

  void BCLComponent::setName(const std::string& name)
  {
    m_name = name;
  }

  bool BCLComponent::operator==(const BCLComponent& other) const {
    return ((m_uid == other.m_uid) && (m_versionId == other.m_versionId));
  }

  bool BCLComponent::operator!=(const BCLComponent& other) const {
    return !(operator==(other));
  }

} // openstudio
