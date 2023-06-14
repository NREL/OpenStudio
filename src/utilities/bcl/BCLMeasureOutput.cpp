/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "BCLMeasureOutput.hpp"

#include "../core/Assert.hpp"

#include <pugixml.hpp>
#include <json/json.h>

namespace openstudio {

BCLMeasureOutput::BCLMeasureOutput(const pugi::xml_node& element) {
  // TODO: escape name
  // cppcheck-suppress useInitializationList
  m_name = element.child("name").text().as_string();

  m_displayName = element.child("display_name").text().as_string();

  m_shortName = element.child("short_name").text().as_string();
  ;

  m_description = element.child("description").text().as_string();

  m_type = element.child("type").text().as_string();

  m_units = element.child("units").text().as_string();

  const std::string test = element.child("model_dependent").text().as_string();
  m_modelDependent = false;
  if (test == "true") {
    m_modelDependent = true;
  }
}

BCLMeasureOutput::BCLMeasureOutput(const std::string& name, const std::string& displayName, const boost::optional<std::string>& shortName,
                                   const boost::optional<std::string>& description, const std::string& type,
                                   const boost::optional<std::string>& units, bool modelDependent)
  : m_name(name),
    m_displayName(displayName),
    m_shortName(shortName),
    m_description(description),
    m_type(type),
    m_units(units),
    m_modelDependent(modelDependent) {}

std::string BCLMeasureOutput::name() const {
  return m_name;
}

std::string BCLMeasureOutput::displayName() const {
  return m_displayName;
}

boost::optional<std::string> BCLMeasureOutput::shortName() const {
  return m_shortName;
}

boost::optional<std::string> BCLMeasureOutput::description() const {
  return m_description;
}

std::string BCLMeasureOutput::type() const {
  return m_type;
}

boost::optional<std::string> BCLMeasureOutput::units() const {
  return m_units;
}

bool BCLMeasureOutput::modelDependent() const {
  return m_modelDependent;
}

void BCLMeasureOutput::writeValues(pugi::xml_node& element) const {
  auto subelement = element.append_child("name");
  auto text = subelement.text();
  text.set(m_name.c_str());

  subelement = element.append_child("display_name");
  text = subelement.text();
  text.set(m_displayName.c_str());

  if (m_shortName) {
    subelement = element.append_child("short_name");
    text = subelement.text();
    text.set((*m_shortName).c_str());
  }

  if (m_description) {
    subelement = element.append_child("description");
    text = subelement.text();
    text.set((*m_description).c_str());
  }

  subelement = element.append_child("type");
  text = subelement.text();
  text.set(m_type.c_str());

  if (m_units) {
    subelement = element.append_child("units");
    text = subelement.text();
    text.set((*m_units).c_str());
  }

  subelement = element.append_child("model_dependent");
  text = subelement.text();
  text.set(m_modelDependent ? "true" : "false");
}

Json::Value BCLMeasureOutput::toJSON() const {
  Json::Value root;

  root["name"] = m_name;

  root["display_name"] = m_displayName;

  if (m_shortName) {
    root["short_name"] = *m_shortName;
  }

  if (m_description) {
    root["description"] = *m_description;
  }

  root["type"] = m_type;

  if (m_units) {
    root["units"] = *m_units;
  }

  root["model_dependent"] = m_modelDependent;

  return root;
}

std::string BCLMeasureOutput::toJSONString() const {
  return toJSON().toStyledString();
}

bool BCLMeasureOutput::operator==(const BCLMeasureOutput& other) const {
  if (m_name != other.name()) {
    return false;
  }

  if (m_displayName != other.displayName()) {
    return false;
  }

  if (m_shortName && !other.shortName()) {
    return false;
  } else if (!m_shortName && other.shortName()) {
    return false;
  } else if (m_shortName && other.shortName()) {
    if (m_shortName.get() != other.shortName().get()) {
      return false;
    }
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

  if (m_modelDependent != other.modelDependent()) {
    return false;
  }

  return true;
}

std::ostream& operator<<(std::ostream& os, const BCLMeasureOutput& output) {
  pugi::xml_document doc;
  auto element = doc.append_child("Output");
  output.writeValues(element);

  doc.save(os, "  ");
  return os;
}

}  // namespace openstudio
