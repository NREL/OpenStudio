/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OSOutput.hpp"

#include "../utilities/core/Assert.hpp"

#include <json/value.h>

#include <sstream>

namespace openstudio {
namespace measure {

  OSOutput OSOutput::makeBoolOutput(const std::string& name, bool modelDependent) {
    OSOutput result(name, OSOutputType::Boolean, modelDependent);
    return result;
  }

  OSOutput OSOutput::makeDoubleOutput(const std::string& name, bool modelDependent) {
    OSOutput result(name, OSOutputType::Double, modelDependent);
    return result;
  }

  OSOutput OSOutput::makeIntegerOutput(const std::string& name, bool modelDependent) {
    OSOutput result(name, OSOutputType::Integer, modelDependent);
    return result;
  }

  OSOutput OSOutput::makeStringOutput(const std::string& name, bool modelDependent) {
    OSOutput result(name, OSOutputType::String, modelDependent);
    return result;
  }

  std::string OSOutput::name() const {
    return m_name;
  }

  std::string OSOutput::displayName() const {
    return m_displayName;
  }

  std::string OSOutput::shortName() const {
    return m_shortName;
  }

  boost::optional<std::string> OSOutput::description() const {
    return m_description;
  }

  OSOutputType OSOutput::type() const {
    return m_type;
  }

  boost::optional<std::string> OSOutput::units() const {
    return m_units;
  }

  bool OSOutput::modelDependent() const {
    return m_modelDependent;
  }

  void OSOutput::setDisplayName(const std::string& displayName) {
    m_displayName = displayName;
  }

  void OSOutput::setShortName(const std::string& shortName) {
    m_shortName = shortName;
  }

  void OSOutput::setDescription(const std::string& description) {
    m_description = description;
  }

  void OSOutput::setUnits(const std::string& units) {
    m_units = units;
  }

  std::string OSOutput::print() const {
    std::stringstream ss;

    // name
    ss << name();
    if (!displayName().empty()) {
      ss << " (" << displayName() << ")";
    }
    ss << '\n';

    // type and required
    ss << type().valueName() << '\n';

    return ss.str();
  }

  OSOutput::OSOutput() = default;

  OSOutput::OSOutput(const std::string& name, const OSOutputType& type, bool modelDependent)
    : m_name(name), m_displayName(name), m_shortName(name), m_type(type), m_modelDependent(modelDependent) {}

  std::ostream& operator<<(std::ostream& os, const OSOutput& output) {
    os << output.print();
    return os;
  }

  Json::Value OSOutput::toJSON() const {
    Json::Value root;
    root["name"] = m_name;
    root["display_name"] = m_displayName;
    root["m_shortName"] = m_shortName;
    if (m_description) {
      root["description"] = *m_description;
    }
    root["type"] = m_type.valueName();
    if (m_units) {
      root["units"] = *m_units;
    }
    root["model_dependent"] = m_modelDependent;

    return root;
  }

  std::string OSOutput::toJSONString() const {
    return toJSON().toStyledString();
  }

}  // namespace measure
}  // namespace openstudio
