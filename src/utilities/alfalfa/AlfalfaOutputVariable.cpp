#include "AlfalfaOutputVariable.hpp"

#include "utilities/idd/IddObject.hpp"
#include "utilities/idd/IddEnums.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include <utilities/idd/OS_Output_Variable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include <utilities/idd/Output_Variable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <fmt/format.h>

namespace openstudio {
namespace alfalfa {
  AlfalfaOutputVariable::AlfalfaOutputVariable(const std::string& variable_key, const std::string& variable_name)
    : AlfalfaComponent("OutputVariable", Capability::Output) {
    if (variable_key.size() == 0) {
      throw std::runtime_error("Error creating AlfalfaOutputVariable: variable_key must be non-empty");
    } else if (variable_name.size() == 0) {
      throw std::runtime_error("Error creating AlfalfaOutputVariable: variable_name must be non-empty");
    } else if (variable_key == "*") {
      throw std::runtime_error("Error creating AlfalfaOutputVariable: Key must not be wildcard '*'");
    }

    parameters["variable_key"] = variable_key;
    parameters["variable_name"] = variable_name;
  }

  AlfalfaOutputVariable::AlfalfaOutputVariable(const openstudio::IdfObject& output_variable)
    : AlfalfaComponent("OutputVariable", Capability::Output) {
    openstudio::IddObjectType idd_type = output_variable.iddObject().type();

    boost::optional<std::string> variable_key;
    boost::optional<std::string> variable_name;

    if (idd_type == IddObjectType::OS_Output_Variable) {
      variable_key = output_variable.getString(OS_Output_VariableFields::KeyValue);
      variable_name = output_variable.getString(OS_Output_VariableFields::VariableName);
    } else if (idd_type == IddObjectType::Output_Variable) {
      variable_key = output_variable.getString(Output_VariableFields::KeyValue);
      variable_name = output_variable.getString(Output_VariableFields::VariableName);
    } else if (idd_type == IddObjectType::OS_EnergyManagementSystem_OutputVariable) {
      variable_key = "EMS";
      variable_name = output_variable.getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName);
    } else if (idd_type == IddObjectType::EnergyManagementSystem_OutputVariable) {
      variable_key = "EMS";
      variable_name = output_variable.getString(EnergyManagementSystem_OutputVariableFields::EMSVariableName);
    } else {
      throw std::runtime_error(fmt::format("Error creating AlfalfaOutputVariable: {} is not a supported object type", idd_type.valueDescription()));
    }

    if (!variable_key.is_initialized() || variable_key.get().size() == 0) {
      throw std::runtime_error("Error creating AlfalfaOutputVariable: Object is missing a variable key");
    }
    if (!variable_name.is_initialized() || variable_name.get().size() == 0) {
      throw std::runtime_error("Error creating AlfalfaOutputVariable: Object is missing a variable name");
    }
    if (variable_key.get() == "*") {
      throw std::runtime_error("Error creating AlfalfaOutputVariable: Key must not be wildcard '*'");
    }

    parameters["variable_key"] = variable_key.get();
    parameters["variable_name"] = variable_name.get();
  }

}  // namespace alfalfa
}  // namespace openstudio
