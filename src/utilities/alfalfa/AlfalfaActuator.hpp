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
    /**
       * Create an AlfalfaActuator Component from a component_name, component_type, and control_type which reflect the EneregyManagementSystem:Actuator EnergyPlus IDD
       */
    AlfalfaActuator(const std::string& component_name, const std::string& component_type, const std::string& control_type);

    /**
       * Create an AlfalfaActuator Component form an IdfObject.
       * Valid idf_object types are EnergyManagementSystemActuator and IdfObject(IddObjectType::EnergyManagementSystem_Actuator)
       */
    AlfalfaActuator(const IdfObject& actuator);
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
