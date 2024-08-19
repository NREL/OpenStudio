#include "AlfalfaOutputVariable.hpp"

namespace openstudio {
namespace alfalfa {
  AlfalfaOutputVariable::AlfalfaOutputVariable(const std::string& variable_key, const std::string& variable_name)
    : m_variable_key(variable_key), m_variable_name(variable_name) {}

  ComponentCapabilities AlfalfaOutputVariable::capabilities() const {
    return ComponentCapabilities::Output;
  }

  Json::Value AlfalfaOutputVariable::toJSON() const {
    Json::Value component;
    component["variable_key"] = m_variable_key;
    component["variable_name"] = m_variable_name;
    return component;
  }

  std::string AlfalfaOutputVariable::type() const {
    return "OutputVariable";
  }
}  // namespace alfalfa
}  // namespace openstudio
