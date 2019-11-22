/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "BCLMeasureArgument.hpp"

#include "../core/Assert.hpp"

#include <QDomDocument>

namespace openstudio{

  BCLMeasureArgument::BCLMeasureArgument(const QDomElement& element)
  {
    // todo: escape name
    m_name = element.firstChildElement("name").firstChild().nodeValue().toStdString();

    m_displayName = element.firstChildElement("display_name").firstChild().nodeValue().toStdString();

    if (!element.firstChildElement("description").isNull()){
      m_description = element.firstChildElement("description").firstChild().nodeValue().toStdString();
    }

    m_type = element.firstChildElement("type").firstChild().nodeValue().toStdString();

    if (!element.firstChildElement("units").isNull()){
      m_units = element.firstChildElement("units").firstChild().nodeValue().toStdString();
    }

    QString test = element.firstChildElement("required").firstChild().nodeValue();
    if (test == "true"){
      m_required = true;
    } else {
      m_required = false;
    }

    test = element.firstChildElement("model_dependent").firstChild().nodeValue();
    if (test == "true"){
      m_modelDependent = true;
    } else {
      m_modelDependent = false;
    }

    if (!element.firstChildElement("default_value").isNull()){
      m_defaultValue = element.firstChildElement("default_value").firstChild().nodeValue().toStdString();
    }

    QDomElement choiceElement = element.firstChildElement("choices").firstChildElement("choice");
    while (!choiceElement.isNull()){
      if (choiceElement.hasChildNodes()){
        std::string choiceValue = choiceElement.firstChildElement("value").firstChild().nodeValue().toStdString();
        m_choiceValues.push_back(choiceValue);

        if (choiceElement.firstChildElement("display_name").isNull()){
          // DLM: this is technically an invalid file, attempt to fix it here
          m_choiceDisplayNames.push_back(choiceValue);
        } else {
          std::string displayValue = choiceElement.firstChildElement("display_name").firstChild().nodeValue().toStdString();
          m_choiceDisplayNames.push_back(displayValue);
        }
      }
      choiceElement = choiceElement.nextSiblingElement("choice");
    }

    if (!element.firstChildElement("min_value").isNull()){
      m_minValue = element.firstChildElement("min_value").firstChild().nodeValue().toStdString();
    }

    if (!element.firstChildElement("max_value").isNull()){
      m_maxValue = element.firstChildElement("max_value").firstChild().nodeValue().toStdString();
    }

  }

  BCLMeasureArgument::BCLMeasureArgument(const std::string& name, const std::string& displayName,
                                         const boost::optional<std::string>& description,
                                         const std::string& type, const boost::optional<std::string>& units,
                                         bool required, bool modelDependent,
                                         const boost::optional<std::string>& defaultValue,
                                         const std::vector<std::string>& choiceValues,
                                         const std::vector<std::string>& choiceDisplayNames,
                                         const boost::optional<std::string>& minValue,
                                         const boost::optional<std::string>& maxValue)
                                         : m_name(name), m_displayName(displayName), m_description(description), m_type(type),
                                         m_units(units), m_required(required), m_modelDependent(modelDependent), m_defaultValue(defaultValue),
                                         m_choiceValues(choiceValues), m_choiceDisplayNames(choiceDisplayNames), m_minValue(minValue), m_maxValue(maxValue)
  {
    if (!m_choiceValues.empty() && m_choiceDisplayNames.empty()){
      m_choiceDisplayNames = m_choiceValues;
    }else if (m_choiceValues.size() != m_choiceDisplayNames.size()){
      throw openstudio::Exception("Sizes of choiceValues and choiceDisplayNames do not match");
    }
  }


  std::string BCLMeasureArgument::name() const
  {
    return m_name;
  }

  std::string BCLMeasureArgument::displayName() const
  {
    return m_displayName;
  }

  boost::optional<std::string> BCLMeasureArgument::description() const
  {
    return m_description;
  }

  std::string BCLMeasureArgument::type() const
  {
    return m_type;
  }

  boost::optional<std::string> BCLMeasureArgument::units() const
  {
    return m_units;
  }

  bool BCLMeasureArgument::required() const
  {
    return m_required;
  }

  bool BCLMeasureArgument::modelDependent() const
  {
    return m_modelDependent;
  }

  boost::optional<std::string> BCLMeasureArgument::defaultValue() const
  {
    return m_defaultValue;
  }

  std::vector<std::string> BCLMeasureArgument::choiceValues() const
  {
    return m_choiceValues;
  }

  std::vector<std::string> BCLMeasureArgument::choiceDisplayNames() const
  {
    return m_choiceDisplayNames;
  }

  boost::optional<std::string> BCLMeasureArgument::minValue() const
  {
    return m_minValue;
  }

  boost::optional<std::string> BCLMeasureArgument::maxValue() const
  {
    return m_maxValue;
  }

  void BCLMeasureArgument::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    QDomElement nameElement = doc.createElement("name");
    element.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(toQString(m_name)));

    QDomElement displayNameElement = doc.createElement("display_name");
    element.appendChild(displayNameElement);
    displayNameElement.appendChild(doc.createTextNode(toQString(m_displayName)));

    if (m_description){
      QDomElement descriptionElement = doc.createElement("description");
      element.appendChild(descriptionElement);
      descriptionElement.appendChild(doc.createTextNode(toQString(*m_description)));
    }

    QDomElement typeElement = doc.createElement("type");
    element.appendChild(typeElement);
    typeElement.appendChild(doc.createTextNode(toQString(m_type)));

    if (m_units){
      QDomElement unitsElement = doc.createElement("units");
      element.appendChild(unitsElement);
      unitsElement.appendChild(doc.createTextNode(toQString(*m_units)));
    }

    QDomElement requiredElement = doc.createElement("required");
    element.appendChild(requiredElement);
    requiredElement.appendChild(doc.createTextNode(m_required ? "true" : "false"));

    QDomElement modelDependentElement = doc.createElement("model_dependent");
    element.appendChild(modelDependentElement);
    modelDependentElement.appendChild(doc.createTextNode(m_modelDependent ? "true" : "false"));

    if (m_defaultValue){
      QDomElement defaultValueElement = doc.createElement("default_value");
      element.appendChild(defaultValueElement);
      defaultValueElement.appendChild(doc.createTextNode(toQString(*m_defaultValue)));
    }

    unsigned n = m_choiceValues.size();
    OS_ASSERT(n == m_choiceDisplayNames.size());
    if (n > 0){
      QDomElement choicesElement = doc.createElement("choices");
      element.appendChild(choicesElement);
      for (unsigned i = 0; i < n; ++i){
        QDomElement choiceElement = doc.createElement("choice");
        choicesElement.appendChild(choiceElement);

        QDomElement valueElement = doc.createElement("value");
        choiceElement.appendChild(valueElement);
        valueElement.appendChild(doc.createTextNode(toQString(m_choiceValues[i])));

        QDomElement displayName2Element = doc.createElement("display_name");
        choiceElement.appendChild(displayName2Element);
        displayName2Element.appendChild(doc.createTextNode(toQString(m_choiceDisplayNames[i])));
      }
    }

    if (m_minValue){
      QDomElement minValueElement = doc.createElement("min_value");
      element.appendChild(minValueElement);
      minValueElement.appendChild(doc.createTextNode(toQString(*m_minValue)));
    }

    if (m_maxValue){
      QDomElement maxValueElement = doc.createElement("max_value");
      element.appendChild(maxValueElement);
      maxValueElement.appendChild(doc.createTextNode(toQString(*m_maxValue)));
    }
  }

  bool BCLMeasureArgument::operator == (const BCLMeasureArgument& other) const
  {
    if (m_name != other.name()){
      return false;
    }

    if (m_displayName != other.displayName()){
      return false;
    }

    if (m_description && !other.description()){
      return false;
    } else if (!m_description && other.description()){
      return false;
    } else if (m_description && other.description()){
      if (m_description.get() != other.description().get()){
        return false;
      }
    }

    if (m_type != other.type()){
      return false;
    }

    if (m_units && !other.units()){
      return false;
    } else if (!m_units && other.units()){
      return false;
    } else if (m_units && other.units()){
      if (m_units.get() != other.units().get()){
        return false;
      }
    }

    if (m_required != other.required()){
      return false;
    }

    if (m_modelDependent != other.modelDependent()){
      return false;
    }

    if (m_defaultValue && !other.defaultValue()){
      return false;
    } else if (!m_defaultValue && other.defaultValue()){
      return false;
    } else if (m_defaultValue && other.defaultValue()){
      if (m_defaultValue.get() != other.defaultValue().get()){
        return false;
      }
    }

    unsigned n = m_choiceValues.size();
    std::vector<std::string> otherChoiceValues = other.choiceValues();
    if (otherChoiceValues.size() != n){
      return false;
    }
    for (unsigned i = 0; i < n; ++i){
      if (m_choiceValues[i] != otherChoiceValues[i]){
        return false;
      }
    }

    n = m_choiceDisplayNames.size();
    std::vector<std::string> otherChoiceDisplayNames = other.choiceDisplayNames();
    if (otherChoiceDisplayNames.size() != n){
      return false;
    }
    for (unsigned i = 0; i < n; ++i){
      if (m_choiceDisplayNames[i] != otherChoiceDisplayNames[i]){
        return false;
      }
    }

    if (m_minValue && !other.minValue()){
      return false;
    } else if (!m_minValue && other.minValue()){
      return false;
    } else if (m_minValue && other.minValue()){
      if (m_minValue.get() != other.minValue().get()){
        return false;
      }
    }

    if (m_maxValue && !other.maxValue()){
      return false;
    } else if (!m_maxValue && other.maxValue()){
      return false;
    } else if (m_maxValue && other.maxValue()){
      if (m_maxValue.get() != other.maxValue().get()){
        return false;
      }
    }

    return true;
  }

  std::ostream& operator<<(std::ostream& os, const BCLMeasureArgument& argument)
  {
    QDomDocument doc;
    QDomElement element = doc.createElement(QString("Argument"));
    doc.appendChild(element);
    argument.writeValues(doc, element);

    QString str;
    QTextStream qts(&str);
    doc.save(qts, 2);
    os << str.toStdString();
    return os;
  }


} // openstudio
