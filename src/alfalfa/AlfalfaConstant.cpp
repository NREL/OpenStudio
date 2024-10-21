#include "AlfalfaConstant.hpp"

namespace openstudio {
namespace alfalfa {
  AlfalfaConstant::AlfalfaConstant(double value) : m_value(value) {}

  Json::Value AlfalfaConstant::toJSON() const {
    Json::Value parameters;
    parameters["value"] = m_value;

    return parameters;
  }

  std::string AlfalfaConstant::deriveName() const {
    return "";
  }

  double AlfalfaConstant::value() const {
    return m_value;
  }
}  // namespace alfalfa
}  // namespace openstudio
