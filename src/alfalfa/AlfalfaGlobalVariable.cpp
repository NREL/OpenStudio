/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "AlfalfaGlobalVariable.hpp"

#include "../utilities/idd/IddObject.hpp"
#include "../utilities/idd/IddEnums.hpp"

#include <utilities/idd/EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/OS_EnergyManagementSystem_GlobalVariable_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <fmt/format.h>

namespace openstudio {
namespace alfalfa {
  AlfalfaGlobalVariable::AlfalfaGlobalVariable(const std::string& variable_name) : m_variable_name(variable_name) {
    if (variable_name.empty()) {
      throw std::runtime_error("Error creating GlobalVariable: variable_name must be non-empty");
    }
  }

  AlfalfaGlobalVariable::AlfalfaGlobalVariable(const IdfObject& global_variable) {
    const IddObjectType idd_type = global_variable.iddObject().type();

    if (idd_type == IddObjectType::OS_EnergyManagementSystem_GlobalVariable) {
      m_variable_name = global_variable.getString(OS_EnergyManagementSystem_GlobalVariableFields::Name).value_or("");
    } else if (idd_type == IddObjectType::EnergyManagementSystem_GlobalVariable) {
      m_variable_name = global_variable.getString(EnergyManagementSystem_GlobalVariableExtensibleFields::ErlVariableName).value_or("");
    } else {
      throw std::runtime_error(fmt::format("Error creating GlobalVariable: {} is not a supported object type", idd_type.valueDescription()));
    }

    if (m_variable_name.empty()) {
      throw std::runtime_error("Error creating GlobalVariable: Object is missing a variable name");
    }
  }

  std::string AlfalfaGlobalVariable::deriveName() const {
    return "Global Variable for " + m_variable_name;
  }

  Json::Value AlfalfaGlobalVariable::toJSON() const {
    Json::Value parameters;
    parameters["variable_name"] = m_variable_name;

    return parameters;
  }

  std::string AlfalfaGlobalVariable::variableName() const {
    return m_variable_name;
  }

  bool AlfalfaGlobalVariable::acceptsObjectType(const IddObjectType& idd_type) {
    return idd_type.value() == IddObjectType::OS_EnergyManagementSystem_GlobalVariable
           || idd_type.value() == IddObjectType::EnergyManagementSystem_GlobalVariable;
  }
}  // namespace alfalfa
}  // namespace openstudio
