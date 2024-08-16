#include "AlfalfaMeter.hpp"

namespace openstudio {
namespace alfalfa {
  AlfalfaMeter::AlfalfaMeter(std::string meter_name) : m_meter_name(meter_name) {}

  ComponentCapabilities AlfalfaMeter::capabilities() const {
    return ComponentCapabilities::Output;
  }

  Json::Value AlfalfaMeter::toJSON() const {
    Json::Value component;
    component["meter_name"] = m_meter_name;
    return component;
  }

  std::string AlfalfaMeter::type() const {
    return "Meter";
  }
}  // namespace alfalfa
}  // namespace openstudio
