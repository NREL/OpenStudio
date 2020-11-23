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

#include "BCLMeasureArgument.hpp"

#include "../core/Assert.hpp"

#include <pugixml.hpp>

namespace openstudio {

BCLMeasureArgument::BCLMeasureArgument(const pugi::xml_node& element) {
  // TODO: escape name
  // cppcheck-suppress useInitializationList
  m_name = element.child("name").text().as_string();

  m_displayName = element.child("display_name").text().as_string();

  m_description = element.child("description").text().as_string();

  m_type = element.child("type").text().as_string();

  m_units = element.child("units").text().as_string();

  m_required = false;
  std::string test = element.child("required").text().as_string();
  if (test == "true") {
    m_required = true;
  }

  m_modelDependent = false;
  test = element.child("model_dependent").text().as_string();
  if (test == "true") {
    m_modelDependent = true;
  }

  m_defaultValue = element.child("default_value").text().as_string();

  auto subelement = element.child("choices");
  if (subelement) {
    for (auto& choiceElement : subelement.children("choice")) {
      std::string choiceValue = choiceElement.child("value").text().as_string();
      if (!choiceValue.empty()) {  // Not exactly the same test as before, probably needs a better test and/or error reporting
        m_choiceValues.push_back(choiceValue);
        auto display_name = choiceElement.child("display_name");
        if (!display_name) {
          // DLM: this is technically an invalid file, attempt to fix it here
          m_choiceDisplayNames.push_back(choiceValue);
        } else {
          m_choiceDisplayNames.push_back(display_name.text().as_string());
        }
      }
    }
  }

  m_minValue = element.child("min_value").text().as_string();

  m_maxValue = element.child("max_value").text().as_string();
}

BCLMeasureArgument::BCLMeasureArgument(const std::string& name, const std::string& displayName, const boost::optional<std::string>& description,
                                       const std::string& type, const boost::optional<std::string>& units, bool required, bool modelDependent,
                                       const boost::optional<std::string>& defaultValue, const std::vector<std::string>& choiceValues,
                                       const std::vector<std::string>& choiceDisplayNames, const boost::optional<std::string>& minValue,
                                       const boost::optional<std::string>& maxValue)
  : m_name(name),
    m_displayName(displayName),
    m_description(description),
    m_type(type),
    m_units(units),
    m_required(required),
    m_modelDependent(modelDependent),
    m_defaultValue(defaultValue),
    m_choiceValues(choiceValues),
    m_choiceDisplayNames(choiceDisplayNames),
    m_minValue(minValue),
    m_maxValue(maxValue) {
  if (!m_choiceValues.empty() && m_choiceDisplayNames.empty()) {
    m_choiceDisplayNames = m_choiceValues;
  } else if (m_choiceValues.size() != m_choiceDisplayNames.size()) {
    throw openstudio::Exception("Sizes of choiceValues and choiceDisplayNames do not match");
  }
}

std::string BCLMeasureArgument::name() const {
  return m_name;
}

std::string BCLMeasureArgument::displayName() const {
  return m_displayName;
}

boost::optional<std::string> BCLMeasureArgument::description() const {
  return m_description;
}

std::string BCLMeasureArgument::type() const {
  return m_type;
}

boost::optional<std::string> BCLMeasureArgument::units() const {
  return m_units;
}

bool BCLMeasureArgument::required() const {
  return m_required;
}

bool BCLMeasureArgument::modelDependent() const {
  return m_modelDependent;
}

boost::optional<std::string> BCLMeasureArgument::defaultValue() const {
  return m_defaultValue;
}

std::vector<std::string> BCLMeasureArgument::choiceValues() const {
  return m_choiceValues;
}

std::vector<std::string> BCLMeasureArgument::choiceDisplayNames() const {
  return m_choiceDisplayNames;
}

boost::optional<std::string> BCLMeasureArgument::minValue() const {
  return m_minValue;
}

boost::optional<std::string> BCLMeasureArgument::maxValue() const {
  return m_maxValue;
}

void BCLMeasureArgument::writeValues(pugi::xml_node& element) const {
  auto subElement = element.append_child("name");
  auto text = subElement.text();
  text.set(m_name.c_str());

  subElement = element.append_child("display_name");
  text = subElement.text();
  text.set(m_displayName.c_str());

  if (m_description) {
    subElement = element.append_child("description");
    text = subElement.text();
    text.set((*m_description).c_str());
  }

  subElement = element.append_child("type");
  text = subElement.text();
  text.set(m_type.c_str());

  if (m_units) {
    subElement = element.append_child("units");
    text = subElement.text();
    text.set((*m_units).c_str());
  }

  subElement = element.append_child("required");
  text = subElement.text();
  text.set(m_required ? "true" : "false");

  subElement = element.append_child("model_dependent");
  text = subElement.text();
  text.set(m_modelDependent ? "true" : "false");

  if (m_defaultValue) {
    subElement = element.append_child("default_value");
    text = subElement.text();
    text.set((*m_defaultValue).c_str());
  }

  unsigned n = m_choiceValues.size();
  OS_ASSERT(n == m_choiceDisplayNames.size());
  if (n > 0) {
    auto choicesElement = element.append_child("choices");
    for (unsigned i = 0; i < n; ++i) {
      auto choiceElement = choicesElement.append_child("choice");

      auto valueElement = choiceElement.append_child("value");
      text = valueElement.text();
      text.set(m_choiceValues[i].c_str());

      auto displayNameElement = choiceElement.append_child("display_name");
      text = displayNameElement.text();
      text.set(m_choiceDisplayNames[i].c_str());
    }
  }

  if (m_minValue) {
    subElement = element.append_child("min_value");
    text = subElement.text();
    text.set((*m_minValue).c_str());
  }

  if (m_maxValue) {
    subElement = element.append_child("max_value");
    text = subElement.text();
    text.set((*m_maxValue).c_str());
  }
}

bool BCLMeasureArgument::operator==(const BCLMeasureArgument& other) const {
  if (m_name != other.name()) {
    return false;
  }

  if (m_displayName != other.displayName()) {
    return false;
  }

  if (m_description && !other.description()) {
    return false;
  } else if (!m_description && other.description()) {
    return false;
  } else if (m_description && other.description()) {
    if (m_description.get() != other.description().get()) {
      return false;
    }
  }

  if (m_type != other.type()) {
    return false;
  }

  if (m_units && !other.units()) {
    return false;
  } else if (!m_units && other.units()) {
    return false;
  } else if (m_units && other.units()) {
    if (m_units.get() != other.units().get()) {
      return false;
    }
  }

  if (m_required != other.required()) {
    return false;
  }

  if (m_modelDependent != other.modelDependent()) {
    return false;
  }

  if (m_defaultValue && !other.defaultValue()) {
    return false;
  } else if (!m_defaultValue && other.defaultValue()) {
    return false;
  } else if (m_defaultValue && other.defaultValue()) {
    if (m_defaultValue.get() != other.defaultValue().get()) {
      return false;
    }
  }

  unsigned n = m_choiceValues.size();
  std::vector<std::string> otherChoiceValues = other.choiceValues();
  if (otherChoiceValues.size() != n) {
    return false;
  }
  for (unsigned i = 0; i < n; ++i) {
    if (m_choiceValues[i] != otherChoiceValues[i]) {
      return false;
    }
  }

  n = m_choiceDisplayNames.size();
  std::vector<std::string> otherChoiceDisplayNames = other.choiceDisplayNames();
  if (otherChoiceDisplayNames.size() != n) {
    return false;
  }
  for (unsigned i = 0; i < n; ++i) {
    if (m_choiceDisplayNames[i] != otherChoiceDisplayNames[i]) {
      return false;
    }
  }

  if (m_minValue && !other.minValue()) {
    return false;
  } else if (!m_minValue && other.minValue()) {
    return false;
  } else if (m_minValue && other.minValue()) {
    if (m_minValue.get() != other.minValue().get()) {
      return false;
    }
  }

  if (m_maxValue && !other.maxValue()) {
    return false;
  } else if (!m_maxValue && other.maxValue()) {
    return false;
  } else if (m_maxValue && other.maxValue()) {
    if (m_maxValue.get() != other.maxValue().get()) {
      return false;
    }
  }

  return true;
}

std::ostream& operator<<(std::ostream& os, const BCLMeasureArgument& argument) {
  pugi::xml_document doc;
  auto element = doc.append_child("Argument");
  argument.writeValues(element);

  doc.save(os, "  ");
  return os;
}

}  // namespace openstudio
