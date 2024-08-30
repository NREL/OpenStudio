#include "AlfalfaActuator.hpp"
#include "utilities/idd/IddObject.hpp"
#include "utilities/idd/IddEnums.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_Actuator_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace alfalfa {
  AlfalfaActuator::AlfalfaActuator(const std::string& component_name, const std::string& component_type, const std::string& control_type)
    : AlfalfaComponent("Actuator", Capability::Input | Capability::Output) {
    parameters["component_name"] = component_name;
    parameters["component_type"] = component_type;
    parameters["control_type"] = control_type;
  }

  AlfalfaActuator::AlfalfaActuator(const IdfObject& actuator) : AlfalfaComponent("Actuator", Capability::Input | Capability::Output) {
    IddObjectType idd_type = actuator.iddObject().type();

    boost::optional<std::string> component_name;
    boost::optional<std::string> component_type;
    boost::optional<std::string> control_type;

    if (idd_type == IddObjectType::OS_EnergyManagementSystem_Actuator) {
      component_name = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentName);
      component_type = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentType);
      control_type = actuator.getString(OS_EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType);
    } else if (idd_type == IddObjectType::EnergyManagementSystem_Actuator) {
      component_name = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentUniqueName);
      component_type = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentType);
      control_type = actuator.getString(EnergyManagementSystem_ActuatorFields::ActuatedComponentControlType);
    } else {
      LOG(Error, "Unable to create Actuator from Object of type " + idd_type.valueDescription());
      throw;
    }

    if (!component_name.is_initialized()) {
      LOG(Error, "Unable to create Actuator from EMS Actuator without a Component Name");
      throw;
    }
    if (!component_type.is_initialized()) {
      LOG(Error, "Unable to create Actuator from EMS Actuator without a Component Type");
      throw;
    }
    if (!control_type.is_initialized()) {
      LOG(Error, "Unable to create Actuator from EMS Actuator without a Control Type");
      throw;
    }
  }
}  // namespace alfalfa
}  // namespace openstudio
