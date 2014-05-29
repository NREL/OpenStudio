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

#include "BCLXML.hpp"

#include "../data/Attribute.hpp"

#include "../units/Unit.hpp"
#include "../units/Quantity.hpp"

#include "../core/String.hpp"
#include "../core/System.hpp"

#include <QDomDocument>
#include <QFile>

namespace openstudio{

  BCLXML::BCLXML(const BCLXMLType& bclXMLType)
    : m_bclXMLType(bclXMLType)
  {
    m_uid = removeBraces(UUID::createUuid());
    m_versionId = removeBraces(UUID::createUuid());
  }

  BCLXML::BCLXML(const openstudio::path& xmlPath):
    m_path(boost::filesystem::system_complete(xmlPath))
  {
    if (!boost::filesystem::exists(xmlPath) || !boost::filesystem::is_regular_file(xmlPath)){
      LOG_AND_THROW("'" << toString(xmlPath) << "' does not exist");
    }

    QDomDocument bclXML("bclXML");
    QFile file(toQString(m_path));
    bool opened = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (opened){
      bclXML.setContent(&file);
      file.close();
    }else{
      file.close();
      LOG_AND_THROW("'" << toString(xmlPath) << "' could not be opened");
    }
    
    QDomElement element = bclXML.firstChildElement("component");
    if (!element.isNull()){
      m_bclXMLType = BCLXMLType::ComponentXML;
    }else{
      element = bclXML.firstChildElement("measure");
      if (!element.isNull()){
        m_bclXMLType = BCLXMLType::MeasureXML;
      }else{
        LOG_AND_THROW("'" << toString(xmlPath) << "' is not a correct BCL XML");
      }
    }

    m_name = decodeString(element.firstChildElement("name").firstChild().nodeValue().replace("_", " ").toStdString());

    m_uid = element.firstChildElement("uid").firstChild().nodeValue().toStdString();
    m_versionId = element.firstChildElement("version_id").firstChild().nodeValue().toStdString();
    
    m_description = decodeString(element.firstChildElement("description").firstChild().nodeValue().toStdString());

    if (m_bclXMLType == BCLXMLType::MeasureXML){
      m_modelerDescription = decodeString(element.firstChildElement("modeler_description").firstChild().nodeValue().toStdString());
    }

    if (m_name.empty() || m_uid.empty() || m_versionId.empty()){
      LOG_AND_THROW("'" << toString(xmlPath) << "' is not a correct BCL XML");
    }

    QDomElement fileElement = element.firstChildElement("files").firstChildElement("file");
    while (!fileElement.isNull()){
      if (fileElement.hasChildNodes()){

        std::string softwareProgram;
        std::string softwareProgramVersion;
        QDomElement versionElement = fileElement.firstChildElement("version");
        if (!versionElement.isNull()){
          softwareProgram = versionElement.firstChildElement("software_program").firstChild().nodeValue().toStdString();
          softwareProgramVersion = versionElement.firstChildElement("identifier").firstChild().nodeValue().toStdString();
        }
        std::string fileName = fileElement.firstChildElement("filename").firstChild().nodeValue().toStdString();
        //std::string fileType = fileElement.firstChildElement("filetype").firstChild().nodeValue().toStdString();
        std::string usageType = fileElement.firstChildElement("usage_type").firstChild().nodeValue().toStdString();
        std::string checksum = fileElement.firstChildElement("checksum").firstChild().nodeValue().toStdString();

        openstudio::path path; ;
        if (usageType == "script"){
          path = m_path.parent_path() / toPath(fileName);
        }else if (usageType == "test"){
          path = m_path.parent_path() / toPath("tests") / toPath(fileName);
        }else if (usageType == "resource"){
          path = m_path.parent_path() / toPath("resources") / toPath(fileName);
        }else{
          path = m_path.parent_path() / toPath(fileName);
        }
        
        BCLFileReference file(path);
        file.setSoftwareProgram(softwareProgram);
        file.setSoftwareProgramVersion(softwareProgramVersion);
        file.setUsageType(usageType);
        file.setChecksum(checksum);

        m_files.push_back(file);

      }else{
        break;
      }
      fileElement = fileElement.nextSiblingElement("file");
    }

    QDomElement attributeElement = element.firstChildElement("attributes").firstChildElement("attribute");
    while (!attributeElement.isNull()){
      if (attributeElement.hasChildNodes()){
        std::string name = attributeElement.firstChildElement("name").firstChild()
          .nodeValue().toStdString();
        std::string value = attributeElement.firstChildElement("value").firstChild()
          .nodeValue().toStdString();
        std::string datatype = attributeElement.firstChildElement("datatype").firstChild()
          .nodeValue().toStdString();

        // Units are optional
        std::string units = attributeElement.firstChildElement("units").firstChild()
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
      }else{
        break;
      }
      attributeElement = attributeElement.nextSiblingElement("attribute");
    }

    QDomElement tagElement = element.firstChildElement("tags").firstChildElement("tag");
    while (!tagElement.isNull()){
      m_tags.push_back(tagElement.firstChild().nodeValue().toStdString());
      tagElement = tagElement.nextSiblingElement("tag");
    }
  }

  BCLXML::~BCLXML()
  {
  }

  boost::optional<BCLXML> BCLXML::load(const openstudio::path& xmlPath)
  {
    boost::optional<BCLXML> result;
    try{
      result = BCLXML(xmlPath);
    }catch(const std::exception&){
    }
    return result;
  }

  std::string BCLXML::escapeString(const std::string& txt)
  {
    // seems that Qt's built in parser does this already
    return txt;

    // http://stackoverflow.com/questions/2083754/why-shouldnt-apos-be-used-to-escape-single-quotes
    QString result = toQString(txt);//.replace("'", "#x27;");
    return result.toStdString();
  }

  std::string BCLXML::decodeString(const std::string& txt)
  {
    // seems that Qt's built in parser does this already
    // only thing that can't be in the text node on disk are '<' (should be '&lt;') and '&' (should be '&amp;')
    return txt;

    QString result = toQString(txt);//.replace("#x27;", "'");
    return result.toStdString();
  }

  std::string BCLXML::uid() const
  {
    return m_uid;
  }

  std::string BCLXML::versionId() const
  {
    return m_versionId;
  }

  std::string BCLXML::name() const
  {
    return m_name;
  }

  std::string BCLXML::description() const
  {
    return m_description;
  }

  std::string BCLXML::modelerDescription() const
  {
    return m_modelerDescription;
  }

  std::vector<BCLFileReference> BCLXML::files() const
  {
    return m_files;
  }

  std::vector<BCLFileReference> BCLXML::files(const std::string& filetype) const
  {
    std::vector<BCLFileReference> matches;
    for (const BCLFileReference& file : m_files) {
      if (file.fileType() == filetype){
        matches.push_back(file);
      }
    } 
    return matches;
  }

  std::vector<Attribute> BCLXML::attributes() const
  {
    return m_attributes;
  }

  boost::optional<Attribute> BCLXML::getAttribute(const std::string& name) const
  {
    for (const Attribute& attribute : m_attributes) {
      if (attribute.name() == name){
        return attribute;
      }
    }
    return boost::none;
  }

  std::vector<std::string> BCLXML::tags() const
  {
    return m_tags;
  }

  openstudio::path BCLXML::path() const
  {
    return m_path;
  }

  openstudio::path BCLXML::directory() const
  {
    return m_path.parent_path();
  }

  void BCLXML::setName(const std::string& name)
  {
    incrementVersionId();
    m_name = escapeString(name);
  }

  void BCLXML::setDescription(const std::string& description)
  {
    incrementVersionId();
    m_description = escapeString(description);
  }

  void BCLXML::setModelerDescription(const std::string& description)
  {
    incrementVersionId();
    m_modelerDescription = escapeString(description);
  }

  void BCLXML::addFile(const BCLFileReference& file)
  {
    removeFile(file.path());

    incrementVersionId();
    m_files.push_back(file);
  }

  bool BCLXML::hasFile(const openstudio::path& path)
  {
    bool result = false;

    openstudio::path test = boost::filesystem::system_complete(path);

    for (const BCLFileReference& file : m_files) {
      if (file.path() == test){
        result = true;
        break;
      }
    } 

    return result;
  }

  bool BCLXML::removeFile(const openstudio::path& path)
  {
    bool result = false;

    openstudio::path test = boost::filesystem::system_complete(path);

    std::vector<BCLFileReference> newFiles;
    for (const BCLFileReference& file : m_files) {
      if (file.path() == test){
        result = true;
      }else{
        newFiles.push_back(file);
      }
    } 

    if (result == true){
      incrementVersionId();
      m_files = newFiles;
    }

    return result;
  }

  void BCLXML::clearFiles()
  {
    incrementVersionId();
    m_files.clear();
  }

  void BCLXML::addAttribute(const Attribute& attribute)
  {
    removeAttribute(attribute.name());

    incrementVersionId();
    m_attributes.push_back(attribute);
  }

  bool BCLXML::removeAttribute(const std::string& name)
  {
    bool result = false;

    std::vector<Attribute> newAttributes;
    for (const Attribute& attribute : m_attributes) {
      if (attribute.name() == name){
        result = true;
      }else{
        newAttributes.push_back(attribute);
      }
    } 

    if (result == true){
      incrementVersionId();
      m_attributes = newAttributes;
    }

    return result;
  }

  void BCLXML::clearAttributes()
  {
    incrementVersionId();
    m_attributes.clear();
  }

  void BCLXML::addTag(const std::string& tagName)
  {
    removeTag(tagName);

    incrementVersionId();
    m_tags.push_back(tagName);
  }

  bool BCLXML::removeTag(const std::string& tagName)
  {
    auto it = std::find(m_tags.begin(), m_tags.end(), tagName);
    if (it != m_tags.end()){
      incrementVersionId();
      m_tags.erase(it);
      return true;
    }

    return false;
  }

  void BCLXML::clearTags()
  {
    incrementVersionId();
    m_tags.clear();
  }

  bool BCLXML::save() const
  {
    if (m_path.empty()){
      return false;
    }

    QDomDocument doc;
    doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\"");

    QDomElement docElement;
    if (m_bclXMLType == BCLXMLType::ComponentXML){
      docElement = doc.createElement("component");
      doc.appendChild(docElement);
    }else if (m_bclXMLType == BCLXMLType::MeasureXML){
      docElement = doc.createElement("measure");
      doc.appendChild(docElement);
    }else{
      return false;
    }

    QDomElement element = doc.createElement("name");
    docElement.appendChild(element);
    element.appendChild(doc.createTextNode(toQString(escapeString(m_name))));

    element = doc.createElement("uid");
    docElement.appendChild(element);
    element.appendChild(doc.createTextNode(toQString(m_uid)));

    element = doc.createElement("version_id");
    docElement.appendChild(element);
    element.appendChild(doc.createTextNode(toQString(m_versionId)));

    element = doc.createElement("description");
    docElement.appendChild(element);
    element.appendChild(doc.createTextNode(toQString(escapeString(m_description))));

    if (m_bclXMLType == BCLXMLType::MeasureXML){
      element = doc.createElement("modeler_description");
      docElement.appendChild(element);
      element.appendChild(doc.createTextNode(toQString(escapeString(m_modelerDescription))));
    }

    // TODO: write provenances
    element = doc.createElement("provenances");
    docElement.appendChild(element);

    // write tags
    element = doc.createElement("tags");
    for (const std::string& tag : m_tags) {
      QDomElement tagElement = doc.createElement("tag");
      element.appendChild(tagElement);
      tagElement.appendChild(doc.createTextNode(toQString(tag)));
    }
    docElement.appendChild(element);

    // write attributes
    element = doc.createElement("attributes");
    for (const Attribute& attribute : m_attributes) {

      std::string value;
      std::string dataType;
      switch (attribute.valueType().value()){
        case AttributeValueType::Boolean :
          if (attribute.valueAsBoolean()){
            value = "true";
          }else{
            value = "false";
          }
          dataType = "boolean";
          break;
        case AttributeValueType::Double :
          value = attribute.toString();
          dataType = "float";
          break;
        case AttributeValueType::Quantity :
          value = toString(attribute.valueAsQuantity().value());
          dataType = "float";
          break;
        case AttributeValueType::Unit :
          value = attribute.valueAsUnit().standardString();
          dataType = "string";
          break;
        case AttributeValueType::Integer :
          value = attribute.toString();
          dataType = "integer";
          break;
        case AttributeValueType::Unsigned :
          value = attribute.toString();
          dataType = "unsigned";
          break;
        case AttributeValueType::String :
          value = attribute.toString();
          dataType = "string";
          break;
        case AttributeValueType::AttributeVector :
          // can't handle this yet
          continue;
          break;
        default:
          // can't handle this yet
          continue;
      }

      QDomElement attributeElement = doc.createElement("attribute");
      element.appendChild(attributeElement);

      QDomElement nameElement = doc.createElement("name");
      attributeElement.appendChild(nameElement);
      nameElement.appendChild(doc.createTextNode(toQString(attribute.name())));

      QDomElement valueElement = doc.createElement("value");
      attributeElement.appendChild(valueElement);
      valueElement.appendChild(doc.createTextNode(toQString(value)));

      QDomElement dataTypeElement = doc.createElement("datatype");
      attributeElement.appendChild(dataTypeElement);
      dataTypeElement.appendChild(doc.createTextNode(toQString(dataType)));

      boost::optional<std::string> units = attribute.units();
      if (units){
        QDomElement unitsElement = doc.createElement("units");
        attributeElement.appendChild(unitsElement);
        unitsElement.appendChild(doc.createTextNode(toQString(*units)));
      }
    }
    docElement.appendChild(element);

    // write files
    element = doc.createElement("files");
    for (const BCLFileReference& file : m_files) {

      QDomElement fileElement = doc.createElement("file");
      element.appendChild(fileElement);

      if (file.usageType() == "script" && !file.softwareProgram().empty() && !file.softwareProgramVersion().empty()){
        QDomElement versionElement = doc.createElement("version");
        fileElement.appendChild(versionElement);

        QDomElement softwareProgramElement = doc.createElement("software_program");
        versionElement.appendChild(softwareProgramElement);
        softwareProgramElement.appendChild(doc.createTextNode(toQString(file.softwareProgram())));

        QDomElement softwareProgramVersionElement = doc.createElement("identifier");
        versionElement.appendChild(softwareProgramVersionElement);
        softwareProgramVersionElement.appendChild(doc.createTextNode(toQString(file.softwareProgramVersion())));
      }

      QDomElement fileNameElement = doc.createElement("filename");
      fileElement.appendChild(fileNameElement);
      fileNameElement.appendChild(doc.createTextNode(toQString(file.fileName())));

      QDomElement fileTypeElement = doc.createElement("filetype");
      fileElement.appendChild(fileTypeElement);
      fileTypeElement.appendChild(doc.createTextNode(toQString(file.fileType())));

      QDomElement usageTypeElement = doc.createElement("usage_type");
      fileElement.appendChild(usageTypeElement);
      usageTypeElement.appendChild(doc.createTextNode(toQString(file.usageType())));

      QDomElement checksumElement = doc.createElement("checksum");
      fileElement.appendChild(checksumElement);
      checksumElement.appendChild(doc.createTextNode(toQString(file.checksum())));
    }
    docElement.appendChild(element);

    // write to disk
    QFile file(toQString(m_path));
    bool opened = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!opened){
      file.close();
      return false;
    }

    QTextStream textStream(&file);
    textStream << doc.toString(2);
    file.close();
    return true;
  }

  bool BCLXML::saveAs(const openstudio::path& xmlPath)
  {
    incrementVersionId();
    m_path = boost::filesystem::system_complete(xmlPath);
    return save();
  }

  void BCLXML::changeUID()
  {
    m_uid = removeBraces(UUID::createUuid());
  }

  void BCLXML::incrementVersionId()
  {
    m_versionId = removeBraces(UUID::createUuid());
  }


} // openstudio
