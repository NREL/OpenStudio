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

#include "BCLComponent.hpp"
#include "../core/System.hpp"
#include "../data/Attribute.hpp"

#include <QDomDocument>
#include <QFile>

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
    QFile file(toQString(m_directory+"/component.xml"));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    component.setContent(&file);
    file.close();

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
