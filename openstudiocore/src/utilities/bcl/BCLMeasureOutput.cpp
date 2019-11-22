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

#include "BCLMeasureOutput.hpp"

#include "../core/Assert.hpp"

#include <QDomDocument>

namespace openstudio{

  BCLMeasureOutput::BCLMeasureOutput(const QDomElement& element)
  {
    // todo: escape name
    m_name = element.firstChildElement("name").firstChild().nodeValue().toStdString();

    m_displayName = element.firstChildElement("display_name").firstChild().nodeValue().toStdString();

    if (!element.firstChildElement("short_name").isNull()){
      m_shortName = element.firstChildElement("short_name").firstChild().nodeValue().toStdString();
    }

    if (!element.firstChildElement("description").isNull()){
      m_description = element.firstChildElement("description").firstChild().nodeValue().toStdString();
    }

    m_type = element.firstChildElement("type").firstChild().nodeValue().toStdString();

    if (!element.firstChildElement("units").isNull()){
      m_units = element.firstChildElement("units").firstChild().nodeValue().toStdString();
    }

    QString test = element.firstChildElement("model_dependent").firstChild().nodeValue();
    if (test == "true"){
      m_modelDependent = true;
    } else {
      m_modelDependent = false;
    }

  }

  BCLMeasureOutput::BCLMeasureOutput(const std::string& name, const std::string& displayName,
                                         const boost::optional<std::string>& shortName,
                                         const boost::optional<std::string>& description,
                                         const std::string& type, const boost::optional<std::string>& units,
                                         bool modelDependent)
                                         : m_name(name), m_displayName(displayName),
                                         m_shortName(shortName), m_description(description),
                                         m_type(type), m_units(units), m_modelDependent(modelDependent)
  {
  }

  std::string BCLMeasureOutput::name() const
  {
    return m_name;
  }

  std::string BCLMeasureOutput::displayName() const
  {
    return m_displayName;
  }

  boost::optional<std::string> BCLMeasureOutput::shortName() const
  {
    return m_shortName;
  }

  boost::optional<std::string> BCLMeasureOutput::description() const
  {
    return m_description;
  }

  std::string BCLMeasureOutput::type() const
  {
    return m_type;
  }

  boost::optional<std::string> BCLMeasureOutput::units() const
  {
    return m_units;
  }

  bool BCLMeasureOutput::modelDependent() const
  {
    return m_modelDependent;
  }

  void BCLMeasureOutput::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    QDomElement nameElement = doc.createElement("name");
    element.appendChild(nameElement);
    nameElement.appendChild(doc.createTextNode(toQString(m_name)));

    QDomElement displayNameElement = doc.createElement("display_name");
    element.appendChild(displayNameElement);
    displayNameElement.appendChild(doc.createTextNode(toQString(m_displayName)));

    if (m_shortName){
      QDomElement shortNameElement = doc.createElement("short_name");
      element.appendChild(shortNameElement);
      shortNameElement.appendChild(doc.createTextNode(toQString(*m_shortName)));
    }

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

    QDomElement modelDependentElement = doc.createElement("model_dependent");
    element.appendChild(modelDependentElement);
    modelDependentElement.appendChild(doc.createTextNode(m_modelDependent ? "true" : "false"));
  }

  bool BCLMeasureOutput::operator == (const BCLMeasureOutput& other) const
  {
    if (m_name != other.name()){
      return false;
    }

    if (m_displayName != other.displayName()){
      return false;
    }

    if (m_shortName && !other.shortName()){
      return false;
    } else if (!m_shortName && other.shortName()){
      return false;
    } else if (m_shortName && other.shortName()){
      if (m_shortName.get() != other.shortName().get()){
        return false;
      }
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

    if (m_modelDependent != other.modelDependent()){
      return false;
    }

    return true;
  }

  std::ostream& operator<<(std::ostream& os, const BCLMeasureOutput& argument)
  {
    QDomDocument doc;
    QDomElement element = doc.createElement(QString("Output"));
    doc.appendChild(element);
    argument.writeValues(doc, element);

    QString str;
    QTextStream qts(&str);
    doc.save(qts, 2);
    os << str.toStdString();
    return os;
  }


} // openstudio
