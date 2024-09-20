#include "AlfalfaActuator.hpp"
#include "../idd/IddObject.hpp"
#include "../idd/IddEnums.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <fmt/format.h>

namespace openstudio {
namespace alfalfa {
  AlfalfaActuator::AlfalfaActuator(const std::string& component_name, const std::string& component_type, const std::string& control_type)
    : AlfalfaComponent("Actuator", Capability::Input | Capability::Output) {
    if (component_name.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: component_name must be non-empty");
    } else if (component_type.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: component_type must be non-empty");
    } else if (control_type.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: control_type must be non-empty");
    }
    parameters["component_name"] = component_name;
    parameters["component_type"] = component_type;
    parameters["control_type"] = control_type;
  }

  AlfalfaActuator::AlfalfaActuator(const IdfObject& actuator) : AlfalfaComponent("Actuator", Capability::Input | Capability::Output) {
    IddObjectType idd_type = actuator.iddObject().type();

    std::string component_name;
    std::string component_type;
    std::string control_type;

    if (idd_type == IddObjectType::OS_EnergyManagementSystem_Actuator) {
      component_name = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentName).value_or("");
      component_type = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentType).value_or("");
      control_type = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType).value_or("");
    } else if (idd_type == IddObjectType::EnergyManagementSystem_Actuator) {
      component_name = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName).value_or("");
      component_type = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType).value_or("");
      control_type = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType).value_or("");
    } else {
      throw std::runtime_error(fmt::format("Error creating AlfalfaActuator: {} is not a supported object type", idd_type.valueDescription()));
    }

    if (component_name.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: Object is missing a component name");
    }
    if (component_type.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: Object is missing a component type");
    }
    if (control_type.empty()) {
      throw std::runtime_error("Error creating AlfalfaActuator: Object is missing a control type");
    }

    parameters["component_name"] = component_name;
    parameters["component_type"] = component_type;
    parameters["control_type"] = control_type;
  }
}  // namespace alfalfa
}  // namespace openstudio
