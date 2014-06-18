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

#include "OnDemandGenerator.hpp"

#include "../core/Assert.hpp"
#include "../core/Compare.hpp"

#include <QDomElement>
#include <QDomDocument>
#include <QFile>

namespace openstudio{

  OnDemandGeneratorValueRestriction::OnDemandGeneratorValueRestriction(const QDomElement& valueElement)
  {
    QDomElement nameElement = valueElement.firstChildElement("name");
    QDomElement nestedArgumentsElement = valueElement.firstChildElement("nested_arguments");

    OS_ASSERT(!nameElement.isNull());

    m_name = nameElement.firstChild().nodeValue().toStdString();
   
    if (!nestedArgumentsElement.isNull()){
      QDomNodeList nestedArgumentElements = nestedArgumentsElement.childNodes();
      for (int i = 0; i < nestedArgumentElements.count(); i++){
        QDomElement nestedArgumentElement = nestedArgumentElements.at(i).toElement();
        m_nestedArguments.push_back(OnDemandGeneratorArgument(nestedArgumentElement));
      }
    }
  }

  OnDemandGeneratorValueRestriction::~OnDemandGeneratorValueRestriction()
  {}
  
  std::string OnDemandGeneratorValueRestriction::name() const
  {
    return m_name;
  }
  
  std::vector<OnDemandGeneratorArgument> OnDemandGeneratorValueRestriction::nestedArguments() const
  {
    return m_nestedArguments;
  }
    
  bool OnDemandGeneratorValueRestriction::setArgumentValue(const std::string& name, const std::string& value)
  {
    for (OnDemandGeneratorArgument& nestedArgument : m_nestedArguments){
      boost::optional<OnDemandGeneratorArgument> result = nestedArgument.getActiveArgument(name);
      if (result){
        return nestedArgument.setArgumentValue(name, value);
      }
    }
    return false;
  }

  bool OnDemandGeneratorValueRestriction::setArgumentValue(const std::string& name, double value)
  {
    for (OnDemandGeneratorArgument& nestedArgument : m_nestedArguments){
      boost::optional<OnDemandGeneratorArgument> result = nestedArgument.getActiveArgument(name);
      if (result){
        return nestedArgument.setArgumentValue(name, value);
      }
    }
    return false;
  }

  bool OnDemandGeneratorValueRestriction::setArgumentValue(const std::string& name, int value)
  {
    for (OnDemandGeneratorArgument& nestedArgument : m_nestedArguments){
      boost::optional<OnDemandGeneratorArgument> result = nestedArgument.getActiveArgument(name);
      if (result){
        return nestedArgument.setArgumentValue(name, value);
      }
    }
    return false;
  }

  OnDemandGeneratorArgument::OnDemandGeneratorArgument(const QDomElement& argumentElement)
  {
    QDomElement nameElement = argumentElement.firstChildElement("name");
    QDomElement displayNameElement = argumentElement.firstChildElement("displayname");
    QDomElement dataTypeElement = argumentElement.firstChildElement("datatype");
    QDomElement requiredElement = argumentElement.firstChildElement("required");
    QDomElement inputTypeElement = argumentElement.firstChildElement("input_type");
    QDomElement valuesElement = argumentElement.firstChildElement("values");

    OS_ASSERT(!nameElement.isNull());
    OS_ASSERT(!displayNameElement.isNull());
    OS_ASSERT(!dataTypeElement.isNull());
    OS_ASSERT(!requiredElement.isNull());
    OS_ASSERT(!inputTypeElement.isNull());

    m_name = nameElement.firstChild().nodeValue().toStdString();

    m_displayName = displayNameElement.firstChild().nodeValue().toStdString();

    std::string dataTypeString = dataTypeElement.firstChild().nodeValue().toStdString();
    if (istringEqual("string", dataTypeString)){
      m_dataType = OnDemandGeneratorArgumentType::String;
    }else if (istringEqual("float", dataTypeString)){
      m_dataType = OnDemandGeneratorArgumentType::Float;
    }else if (istringEqual("integer", dataTypeString)){
      m_dataType = OnDemandGeneratorArgumentType::Integer;
    }else{
      OS_ASSERT(false);
    }

    if (requiredElement.firstChild().nodeValue().toInt() == 1){
      m_required = true;
    }else{
      m_required = false;
    }

    std::string inputTypeString = inputTypeElement.firstChild().nodeValue().toStdString();
    if (istringEqual("select", inputTypeString)){
      m_inputType = OnDemandGeneratorInputType::Select;
    }else if (istringEqual("text", inputTypeString)){
      m_inputType = OnDemandGeneratorInputType::Text;
    }else if (istringEqual("component", inputTypeString)){
      m_inputType = OnDemandGeneratorInputType::Component;
    }else{
      OS_ASSERT(false);
    }

    if (!valuesElement.isNull()){
      QDomNodeList valueNodes = valuesElement.childNodes();
      for (int i = 0; i < valueNodes.count(); i++){
        QDomElement valueNode = valueNodes.at(i).toElement();
        m_valueRestrictions.push_back(OnDemandGeneratorValueRestriction(valueNode));
      }
    }

  }
    
  OnDemandGeneratorArgument::~OnDemandGeneratorArgument()
  {
  }

  std::string OnDemandGeneratorArgument::name() const
  {
    return m_name;
  }
    
  std::string OnDemandGeneratorArgument::displayName() const
  {
    return m_displayName;
  }

  OnDemandGeneratorArgumentType OnDemandGeneratorArgument::dataType() const
  {
    return m_dataType;
  }
  
  bool OnDemandGeneratorArgument::required() const
  {
    return m_required;
  }
  
  OnDemandGeneratorInputType OnDemandGeneratorArgument::inputType() const
  {
    return m_inputType;
  }
  
  std::string OnDemandGeneratorArgument::units() const
  {
    return m_units;
  }
  
  std::string OnDemandGeneratorArgument::componentType() const
  {
    return m_componentType;
  }
  
  std::vector<OnDemandGeneratorValueRestriction> OnDemandGeneratorArgument::valueRestrictions() const
  {
    return m_valueRestrictions;
  }

  std::vector<OnDemandGeneratorArgument> OnDemandGeneratorArgument::activeArguments() const
  {
    std::vector<OnDemandGeneratorArgument> result;
    result.push_back(*this);

    if (m_inputType == OnDemandGeneratorInputType::Select){
      boost::optional<std::string> value = this->valueAsString();
      if (value){
        // find value restrictions with this value
        for (const OnDemandGeneratorValueRestriction& valueRestriction : m_valueRestrictions){
          if (*value == valueRestriction.name()){
            // add nested arguments from value restriction
            for (const OnDemandGeneratorArgument& nestedArgument : valueRestriction.nestedArguments()){
              std::vector<OnDemandGeneratorArgument> temp = nestedArgument.activeArguments();
              result.insert(result.end(), temp.begin(), temp.end());
            }
            break;
          }
        }
      }
    }

    return result;

  }

  std::vector<std::string> OnDemandGeneratorArgument::activeArgumentNames() const
  {
    std::vector<std::string> result;
    result.push_back(m_name);

    if (m_inputType == OnDemandGeneratorInputType::Select){
      boost::optional<std::string> value = this->valueAsString();
      if (value){
        // find value restrictions with this value
        for (const OnDemandGeneratorValueRestriction& valueRestriction : m_valueRestrictions){
          if (*value == valueRestriction.name()){
            // add nested arguments from value restriction
            for (const OnDemandGeneratorArgument& nestedArgument : valueRestriction.nestedArguments()){
              std::vector<std::string> temp = nestedArgument.activeArgumentNames();
              result.insert(result.end(), temp.begin(), temp.end());
            }
            break;
          }
        }
      }
    }

    return result;
  }

  boost::optional<OnDemandGeneratorArgument> OnDemandGeneratorArgument::getActiveArgument(const std::string& name) const
  {
    if (name == m_name){
      return *this;
    }

    if (m_inputType == OnDemandGeneratorInputType::Select){
      boost::optional<std::string> value = this->valueAsString();
      if (value){
        // find value restrictions with this value
        for (const OnDemandGeneratorValueRestriction& valueRestriction : m_valueRestrictions){
          if (*value == valueRestriction.name()){
            // add nested arguments from value restriction
            for (const OnDemandGeneratorArgument& nestedArgument : valueRestriction.nestedArguments()){
              boost::optional<OnDemandGeneratorArgument> result = nestedArgument.getActiveArgument(name);
              if (result){
                return result;
              }
            }
          }
        }
      }
    }

    return boost::none;
  }

  bool OnDemandGeneratorArgument::hasValue() const
  {
    return (!m_value.isNull() && m_value.isValid());
  }

  boost::optional<std::string> OnDemandGeneratorArgument::valueAsString() const
  {
    if (m_dataType == OnDemandGeneratorArgumentType::String){
      if (!m_value.isNull() && m_value.isValid()){
        return m_value.value<std::string>();
      }
    }
    return boost::none;
  }
  
  boost::optional<double> OnDemandGeneratorArgument::valueAsDouble() const
  {
    if (m_dataType == OnDemandGeneratorArgumentType::Float){
      if (!m_value.isNull() && m_value.isValid()){
        return m_value.value<double>();
      }
    }
    return boost::none;
  }
  
  boost::optional<int> OnDemandGeneratorArgument::valueAsInteger() const
  {
    if (m_dataType == OnDemandGeneratorArgumentType::Integer){
      if (!m_value.isNull() && m_value.isValid()){
        return m_value.value<int>();
      }
    }
    return boost::none;
  }
  
  bool OnDemandGeneratorArgument::setValue(const std::string& value)
  {
    if (m_dataType == OnDemandGeneratorArgumentType::String){
      if (m_inputType == OnDemandGeneratorInputType::Select){
        // find value restrictions with this value
        for (const OnDemandGeneratorValueRestriction& valueRestriction : m_valueRestrictions){
          if (value == valueRestriction.name()){
            m_value.setValue(value);
            return true;
          }
        }
      }else{
        // just set it 
        m_value.setValue(value);
        return true;
      }
    }
    return false;
  }
  
  bool OnDemandGeneratorArgument::setValue(double value)
  {
    if (m_dataType == OnDemandGeneratorArgumentType::Float){
      m_value.setValue(value);
      return true;
    }
    return false;
  }
  
  bool OnDemandGeneratorArgument::setValue(int value)
  {
    if (m_dataType == OnDemandGeneratorArgumentType::Integer){
      m_value.setValue(value);
      return true;
    }
    return false;
  }

  bool OnDemandGeneratorArgument::setArgumentValue(const std::string& name, const std::string& value)
  {
    if (name == m_name){
      return setValue(value);
    }

    if (m_inputType == OnDemandGeneratorInputType::Select){
      boost::optional<std::string> thisValue = this->valueAsString();
      if (thisValue){
        // find value restrictions with this value
        for (OnDemandGeneratorValueRestriction& valueRestriction : m_valueRestrictions){
          if (*thisValue == valueRestriction.name()){
            return valueRestriction.setArgumentValue(name, value);
          }
        }
      }
    }

    return false;
  }

  bool OnDemandGeneratorArgument::setArgumentValue(const std::string& name, double value)
  {
    if (name == m_name){
      return setValue(value);
    }

    if (m_inputType == OnDemandGeneratorInputType::Select){
      boost::optional<std::string> thisValue = this->valueAsString();
      if (thisValue){
        // find value restrictions with this value
        for (OnDemandGeneratorValueRestriction& valueRestriction : m_valueRestrictions){
          if (*thisValue == valueRestriction.name()){
            return valueRestriction.setArgumentValue(name, value);
          }
        }
      }
    }

    return false;
  }

  bool OnDemandGeneratorArgument::setArgumentValue(const std::string& name, int value)
  {
    if (name == m_name){
      return setValue(value);
    }

    if (m_inputType == OnDemandGeneratorInputType::Select){
      boost::optional<std::string> thisValue = this->valueAsString();
      if (thisValue){
        // find value restrictions with this value
        for (OnDemandGeneratorValueRestriction& valueRestriction : m_valueRestrictions){
          if (*thisValue == valueRestriction.name()){
            return valueRestriction.setArgumentValue(name, value);
          }
        }
      }
    }

    return false;
  }

  boost::optional<OnDemandGenerator> OnDemandGenerator::load(const openstudio::path& path)
  {
    boost::optional<OnDemandGenerator> result;

    if (boost::filesystem::exists(path)){
      try{

        QFile file(toQString(path));
        file.open(QFile::ReadOnly);
        QDomDocument qDomDocument;
        qDomDocument.setContent(&file);
        file.close();

        result = OnDemandGenerator(qDomDocument.documentElement());
      }catch(...){
      }
    }
    return result;
  }

  OnDemandGenerator::OnDemandGenerator(const QDomElement& generatorElement)
  {
    QDomElement nameElement = generatorElement.firstChildElement("name");
    QDomElement uidElement = generatorElement.firstChildElement("uid");
    QDomElement versionIdElement = generatorElement.firstChildElement("version_id");
    QDomElement descriptionElement = generatorElement.firstChildElement("description");
    QDomElement argumentsElement = generatorElement.firstChildElement("arguments");

    OS_ASSERT(!nameElement.isNull());
    OS_ASSERT(!uidElement.isNull());
    OS_ASSERT(!versionIdElement.isNull());
    OS_ASSERT(!descriptionElement.isNull());
    OS_ASSERT(!argumentsElement.isNull());

    QString name = nameElement.firstChild().nodeValue().replace('_', ' ');
    while (name.indexOf("  ") != -1) {
      name = name.replace("  ", " ");
    }
    name[0] = name[0].toUpper();
    m_name = name.toStdString();

    if (!uidElement.isNull() && uidElement.firstChild().nodeValue().length() == 36)
    {
      m_uid = uidElement.firstChild().nodeValue().toStdString();
    }

    if (!versionIdElement.isNull() && versionIdElement.firstChild().nodeValue().length() == 36)
    {
      m_versionId = versionIdElement.firstChild().nodeValue().toStdString();
    }

    QString description = descriptionElement.firstChild().nodeValue();
    m_description = description.toStdString();

    QDomNodeList argumentNodes = argumentsElement.childNodes();
    for (int i = 0; i < argumentNodes.count(); i++){
      QDomElement argumentElement = argumentNodes.at(i).toElement();
      m_arguments.push_back(OnDemandGeneratorArgument(argumentElement));
    }
  }

  OnDemandGenerator::~OnDemandGenerator()
  {}
  
  std::string OnDemandGenerator::name() const
  {
    return m_name;
  }
  
  std::string OnDemandGenerator::uid() const
  {
    return m_uid;
  }
  
  std::string OnDemandGenerator::versionId() const
  {
    return m_versionId;
  }
  
  std::string OnDemandGenerator::description() const
  {
    return m_description;
  }
  
  std::vector<OnDemandGeneratorArgument> OnDemandGenerator::arguments() const
  {
    return m_arguments;
  }

  std::vector<OnDemandGeneratorArgument> OnDemandGenerator::activeArguments() const
  {
    std::vector<OnDemandGeneratorArgument> result;
    for (const OnDemandGeneratorArgument& argument : m_arguments){
      std::vector<OnDemandGeneratorArgument> temp = argument.activeArguments();
      result.insert(result.end(), temp.begin(), temp.end());
    }

    return result;
  }

  std::vector<std::string> OnDemandGenerator::activeArgumentNames() const
  {
    std::vector<std::string> result;
    for (const OnDemandGeneratorArgument& argument : m_arguments){
      std::vector<std::string> temp = argument.activeArgumentNames();
      result.insert(result.end(), temp.begin(), temp.end());
    }

    return result;
  }

  boost::optional<OnDemandGeneratorArgument> OnDemandGenerator::getActiveArgument(const std::string& name) const
  {
    for (const OnDemandGeneratorArgument& argument : m_arguments){
      boost::optional<OnDemandGeneratorArgument> result = argument.getActiveArgument(name);
      if (result){
        return result;
      }
    }
    return boost::none;
  }

  bool OnDemandGenerator::setArgumentValue(const std::string& name, const std::string& value)
  {
    for (OnDemandGeneratorArgument& argument : m_arguments){
      boost::optional<OnDemandGeneratorArgument> result = argument.getActiveArgument(name);
      if (result){
        return argument.setArgumentValue(name, value);
      }
    }
    return false;
  }

  bool OnDemandGenerator::setArgumentValue(const std::string& name, double value)
  {
    for (OnDemandGeneratorArgument& argument : m_arguments){
      boost::optional<OnDemandGeneratorArgument> result = argument.getActiveArgument(name);
      if (result){
        return argument.setArgumentValue(name, value);
      }
    }
    return false;
  }

  bool OnDemandGenerator::setArgumentValue(const std::string& name, int value)
  {
    for (OnDemandGeneratorArgument& argument : m_arguments){
      boost::optional<OnDemandGeneratorArgument> result = argument.getActiveArgument(name);
      if (result){
        return argument.setArgumentValue(name, value);
      }
    }
    return false;
  }

  bool OnDemandGenerator::checkArgumentValues() const
  {
    // check that each active argument has a value
    for (const std::string& activeArgumentName : this->activeArgumentNames()){
      boost::optional<OnDemandGeneratorArgument> activeArgument = this->getActiveArgument(activeArgumentName);
      OS_ASSERT(activeArgument);
      if (!activeArgument->hasValue()){
        return false;
      }
    }
    return true;
  }

} // openstudio
