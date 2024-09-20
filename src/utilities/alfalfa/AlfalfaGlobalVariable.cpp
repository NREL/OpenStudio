#include "AlfalfaGlobalVariable.hpp"

#include "../idd/IddObject.hpp"
#include "../idd/IddEnums.hpp"

#include <utilities/idd/EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <fmt/format.h>

namespace openstudio {
namespace alfalfa {
  AlfalfaGlobalVariable::AlfalfaGlobalVariable(const std::string& variable_name)
    : AlfalfaComponent("GlobalVariable", Capability::Output | Capability::Input) {
    if (variable_name.empty()) {
      throw std::runtime_error("Error creating AlfalfaGlobalVariable: variable_name must be non-empty");
    }
    parameters["variable_name"] = variable_name;
  }

  AlfalfaGlobalVariable::AlfalfaGlobalVariable(const IdfObject& global_variable)
    : AlfalfaComponent("GlobalVariable", Capability::Output | Capability::Input) {
    IddObjectType idd_type = global_variable.iddObject().type();
    boost::optional<std::string> variable_name;

    if (idd_type == IddObjectType::OS_EnergyManagementSystem_GlobalVariable) {
      variable_name = global_variable.getString(OS_EnergyManagementSystem_GlobalVariableFields::Name);
    } else if (idd_type == IddObjectType::EnergyManagementSystem_GlobalVariable) {
      variable_name = global_variable.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName);
    } else {
      throw std::runtime_error(fmt::format("Error creating AlfalfaGlobalVariable: {} is not a supported object type", idd_type.valueDescription()));
    }

    if (!variable_name.is_initialized() || variable_name.get().empty()) {
      throw std::runtime_error("Error creating AlfalfaGlobalVariable: Object is missing a variable name");
    }

    parameters["variable_name"] = variable_name.get();
  }
}  // namespace alfalfa
}  // namespace openstudio
