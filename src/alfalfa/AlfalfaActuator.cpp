/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AlfalfaActuator.hpp"

#include "../utilities/idd/IddObject.hpp"
#include "../utilities/idd/IddEnums.hpp"
#include "../utilities/idf/IdfObject.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <fmt/format.h>
#include <json/value.h>

#include <string>

namespace openstudio {
namespace alfalfa {
  AlfalfaActuator::AlfalfaActuator(const std::string& component_name, const std::string& component_type, const std::string& control_type)
    : m_component_name(component_name), m_component_type(component_type), m_control_type(control_type) {
    if (component_name.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: component_name must be non-empty");
    } else if (component_type.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: component_type must be non-empty");
    } else if (control_type.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: control_type must be non-empty");
    }
  }

  AlfalfaActuator::AlfalfaActuator(const IdfObject& actuator) {
    const IddObjectType idd_type = actuator.iddObject().type();

    if (idd_type == IddObjectType::OS_EnergyManagementSystem_Actuator) {
      m_component_name = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentName).value_or("");
      m_component_type = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentType).value_or("");
      m_control_type = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType).value_or("");
    } else if (idd_type == IddObjectType::EnergyManagementSystem_Actuator) {
      m_component_name = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName).value_or("");
      m_component_type = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType).value_or("");
      m_control_type = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType).value_or("");
    } else {
      throw std::runtime_error(fmt::format("Error creating AlfalfaActuator: {} is not a supported object type", idd_type.valueDescription()));
    }

    if (m_component_name.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: Object is missing a component name");
    }
    if (m_component_type.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: Object is missing a component type");
    }
    if (m_control_type.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: Object is missing a control type");
    }
  }

  std::string AlfalfaActuator::deriveName() const {
    return fmt::format("Actuator for {}:{}:{}", m_component_name, m_component_type, m_control_type);
  }

  Json::Value AlfalfaActuator::toJSON() const {
    Json::Value parameters;
    parameters["component_name"] = m_component_name;
    parameters["component_type"] = m_component_type;
    parameters["control_type"] = m_control_type;

    return parameters;
  }

  std::string AlfalfaActuator::componentName() const {
    return m_component_name;
  }

  std::string AlfalfaActuator::componentType() const {
    return m_component_type;
  }

  std::string AlfalfaActuator::controlType() const {
    return m_control_type;
  }

  bool AlfalfaActuator::acceptsObjectType(const IddObjectType& idd_type) {
    return idd_type.value() == IddObjectType::OS_EnergyManagementSystem_Actuator
           || idd_type.value() == IddObjectType::EnergyManagementSystem_Actuator;
  }
}  // namespace alfalfa
}  // namespace openstudio
