#include "AlfalfaOutputVariable.hpp"

namespace openstudio {
namespace alfalfa {
  AlfalfaOutputVariable::AlfalfaOutputVariable(std::string variable_name, std::string variable_key)
    : m_variable_name(variable_name),
      m_variable_key(variable_key) {}

  ComponentCapabilities AlfalfaOutputVariable::capabilities() const {
    return ComponentCapabilities::Output;
  }

  Json::Value AlfalfaOutputVariable::toJSON() const {
    Json::Value component;
    component["variable_name"] = m_variable_name;
    component["variable_key"] = m_variable_key;
    return component;
  }

  std::string AlfalfaOutputVariable::type() const {
    return "OutputVariable";
  }
}
}
