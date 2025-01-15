/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AlfalfaOutputVariable.hpp"

#include "../utilities/idd/IddObject.hpp"
#include "../utilities/idd/IddEnums.hpp"

#include <utilities/idd/OS_EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include <utilities/idd/OS_Output_Variable_FieldEnums.hxx>
#include <utilities/idd/EnergyManagementSystem_OutputVariable_FieldEnums.hxx>
#include <utilities/idd/Output_Variable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <fmt/format.h>

namespace openstudio {
namespace alfalfa {
  AlfalfaOutputVariable::AlfalfaOutputVariable(const std::string& variable_key, const std::string& variable_name)
    : m_variable_key(variable_key), m_variable_name(variable_name) {
    if (variable_key.empty()) {
      throw std::runtime_error("Error creating OutputVariable: variable_key must be non-empty");
    } else if (variable_name.empty()) {
      throw std::runtime_error("Error creating OutputVariable: variable_name must be non-empty");
    } else if (variable_key == "*") {
      throw std::runtime_error("Error creating OutputVariable: Key must not be wildcard '*'");
    }
  }

  AlfalfaOutputVariable::AlfalfaOutputVariable(const openstudio::IdfObject& output_variable) {
    const openstudio::IddObjectType idd_type = output_variable.iddObject().type();

    if (idd_type == IddObjectType::OS_Output_Variable) {
      m_variable_key = output_variable.getString(OS_Output_VariableFields::KeyValue).value_or("");
      m_variable_name = output_variable.getString(OS_Output_VariableFields::VariableName).value_or("");
    } else if (idd_type == IddObjectType::Output_Variable) {
      m_variable_key = output_variable.getString(Output_VariableFields::KeyValue).value_or("");
      m_variable_name = output_variable.getString(Output_VariableFields::VariableName).value_or("");
    } else if (idd_type == IddObjectType::OS_EnergyManagementSystem_OutputVariable) {
      m_variable_key = "EMS";
      m_variable_name = output_variable.getString(OS_EnergyManagementSystem_OutputVariableFields::EMSVariableName).value_or("");
    } else if (idd_type == IddObjectType::EnergyManagementSystem_OutputVariable) {
      m_variable_key = "EMS";
      m_variable_name = output_variable.getString(EnergyManagementSystem_OutputVariableFields::EMSVariableName).value_or("");
    } else {
      throw std::runtime_error(fmt::format("Error creating OutputVariable: {} is not a supported object type", idd_type.valueDescription()));
    }

    if (m_variable_key.empty()) {
      throw std::runtime_error("Error creating OutputVariable: Object is missing a variable key");
    }
    if (m_variable_name.empty()) {
      throw std::runtime_error("Error creating OutputVariable: Object is missing a variable name");
    }
    if (m_variable_key == "*") {
      throw std::runtime_error("Error creating OutputVariable: Key must not be wildcard '*'");
    }
  }

  std::string AlfalfaOutputVariable::deriveName() const {
    return "Output Variable for " + m_variable_key + ":" + m_variable_name;
  }

  Json::Value AlfalfaOutputVariable::toJSON() const {
    Json::Value parameters;

    parameters["variable_key"] = m_variable_key;
    parameters["variable_name"] = m_variable_name;

    return parameters;
  }

  std::string AlfalfaOutputVariable::variableKey() const {
    return m_variable_key;
  }

  std::string AlfalfaOutputVariable::variableName() const {
    return m_variable_name;
  }

  bool AlfalfaOutputVariable::acceptsObjectType(const IddObjectType& idd_type) {
    return idd_type.value() == IddObjectType::OS_Output_Variable || idd_type.value() == IddObjectType::Output_Variable
           || idd_type.value() == IddObjectType::EnergyManagementSystem_OutputVariable
           || idd_type.value() == IddObjectType::OS_EnergyManagementSystem_OutputVariable;
  }

}  // namespace alfalfa
}  // namespace openstudio
