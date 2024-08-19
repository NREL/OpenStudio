#include "AlfalfaActuator.hpp"

namespace openstudio {
namespace alfalfa {
  AlfalfaActuator::AlfalfaActuator(const std::string& component_name, const std::string& component_type, const std::string& control_type)
    : m_component_name(component_name), m_component_type(component_type), m_control_type(control_type) {}

  ComponentCapabilities AlfalfaActuator::capabilities() const {
    return ComponentCapabilities::Output | ComponentCapabilities::Input;
  }

  Json::Value AlfalfaActuator::toJSON() const {
    Json::Value component;
    component["component_name"] = m_component_name;
    component["component_type"] = m_component_type;
    component["control_type"] = m_control_type;
    return component;
  }

  std::string AlfalfaActuator::type() const {
    return "Actuator";
  }
}  // namespace alfalfa
}  // namespace openstudio
