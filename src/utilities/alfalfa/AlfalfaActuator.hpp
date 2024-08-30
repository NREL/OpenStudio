#ifndef ALFALFA_COMPONENT_ACTUATOR_HPP
#define ALFALFA_COMPONENT_ACTUATOR_HPP

#include "AlfalfaComponent.hpp"
#include "utilities/idf/IdfObject.hpp"
#include "utilities/core/Logger.hpp"

namespace openstudio {
namespace alfalfa {
  class AlfalfaActuator : public AlfalfaComponent
  {
   public:
    AlfalfaActuator(const std::string& component_name, const std::string& component_type, const std::string& control_type);

    AlfalfaActuator(const IdfObject& actuator);

   private:
    // configure logging
    REGISTER_LOGGER("openstudio.AlfalfaActuator");
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
