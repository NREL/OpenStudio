#include "AlfalfaGlobalVariable.hpp"

#include "utilities/idd/IddObject.hpp"
#include "utilities/idd/IddEnums.hpp"

#include <utilities/idd/EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace alfalfa {
  AlfalfaGlobalVariable::AlfalfaGlobalVariable(const std::string& variable_name)
    : AlfalfaComponent("GlobalVariable", Capability::Output | Capability::Input) {
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
      throw std::runtime_error("Unable to create Global Variable from Object of type " + idd_type.valueDescription());
    }

    if (!variable_name.is_initialized() || variable_name.get().size() == 0) {
      throw std::runtime_error("Unable to create Global Variable from EMS Output Variable without a Name");
    }

    parameters["variable_name"] = variable_name.get();
  }
}  // namespace alfalfa
}  // namespace openstudio
