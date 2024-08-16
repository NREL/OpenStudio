#include "AlfalfaGlobalVariable.hpp"

namespace openstudio {
namespace alfalfa {
  AlfalfaGlobalVariable::AlfalfaGlobalVariable(std::string variable_name) : m_variable_name(variable_name) {}

  ComponentCapabilities AlfalfaGlobalVariable::capabilities() const {
    return ComponentCapabilities::Output | ComponentCapabilities::Input;
  }

  Json::Value AlfalfaGlobalVariable::toJSON() const {
    Json::Value component;
    component["variable_name"] = m_variable_name;
    return component;
  }

  std::string AlfalfaGlobalVariable::type() const {
    return "GlobalVariable";
  }
}  // namespace alfalfa
}  // namespace openstudio
