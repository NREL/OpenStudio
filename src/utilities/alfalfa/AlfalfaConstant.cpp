
#include "AlfalfaConstant.hpp"

namespace openstudio {
namespace alfalfa {
  AlfalfaConstant::AlfalfaConstant(float value)
    : m_value(value) {}

  ComponentCapabilities AlfalfaConstant::capabilities() const {
    return ComponentCapabilities::Input | ComponentCapabilities::Output;
  }

  Json::Value AlfalfaConstant::toJSON() const {
    Json::Value component;
    component["value"] = m_value;
    return component;
  }

  std::string AlfalfaConstant::type() const {
    return "Constant";
  }
}
}
