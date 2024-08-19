#ifndef ALFALFA_ACTUATOR_HPP
#define ALFALFA_ACTUATOR_HPP

#include "AlfalfaComponent.hpp"
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  class AlfalfaActuator : public AlfalfaComponent
  {
   public:
    AlfalfaActuator(const std::string& component_name, const std::string& component_type, const std::string& control_type);

    ComponentCapabilities capabilities() const;

    Json::Value toJSON() const;

    std::string type() const;

   private:
    std::string m_component_name, m_component_type, m_control_type;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
