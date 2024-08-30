#include "AlfalfaGlobalVariable.hpp"

#include "utilities/idd/IddObject.hpp"
#include "utilities/idd/IddEnums.hpp"

#include <utilities/idd/EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace alfalfa {
  AlfalfaGlobalVariable::AlfalfaGlobalVariable(const std::string& variable_name)
    : AlfalfaComponent("GlobalVariable", Capability::Output | Capability::Input)
    {
      parameters["variable_name"] = variable_name;
    }

  AlfalfaGlobalVariable::AlfalfaGlobalVariable(const IdfObject& global_variable)
    : AlfalfaComponent("GlobalVariable", Capability::Output | Capability::Input)
    {
      IddObjectType idd_type = global_variable.iddObject().type();
      boost::optional<std::string> variable_name;

      if (idd_type == IddObjectType::OS_EnergyManagementSystem_GlobalVariable) {
        variable_name = global_variable.getString(OS_EnergyManagementSystem_GlobalVariableFields::Name);
      } else if (idd_type == IddObjectType::EnergyManagementSystem_GlobalVariable) {
        variable_name = global_variable.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName);
      } else {
        LOG(Error, "Unable to create Global Variable from Object of type " + idd_type.valueDescription());
        throw;
      }

      if (!variable_name.is_initialized()) {
        LOG(Error, "Unable to create Global Variable from EMS Output Variable without a Name");
        throw;
      }

      parameters["variable_name"] = variable_name.get();
    }
}
}
