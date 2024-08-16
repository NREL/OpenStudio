#include "AlfalfaActuator.hpp"

namespace openstudio {
namespace alfalfa {
  AlfalfaActuator::AlfalfaActuator(std::string component_type, std::string control_type, std::string actuator_key)
    : m_component_type(component_type),
      m_control_type(control_type),
      m_actuator_key(actuator_key) {}

  ComponentCapabilities AlfalfaActuator::capabilities() const {
    return ComponentCapabilities::Output | ComponentCapabilities::Input;
  }

  Json::Value AlfalfaActuator::toJSON() const {
    Json::Value component;
    component["component_type"] = m_component_type;
    component["control_type"] = m_control_type;
    component["actuator_key"] = m_actuator_key;
    return component;
  }

  std::string AlfalfaActuator::type() const {
    return "Actuator";
  }
}
}
