#ifndef ALFALFA_ACTUATOR_HPP
#define ALFALFA_ACTUATOR_HPP

#include "AlfalfaComponent.hpp"
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  class AlfalfaActuator : public AlfalfaComponent {
    public:
      AlfalfaActuator(std::string component_type, std::string control_type, std::string actuator_key);

      ComponentCapabilities capabilities() const;

      Json::Value toJSON() const;

      std::string type() const;

    private:
      std::string m_component_type, m_control_type, m_actuator_key;
  };
}
}

#endif
