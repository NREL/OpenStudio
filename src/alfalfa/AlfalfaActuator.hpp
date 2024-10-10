#ifndef ALFALFA_COMPONENT_ACTUATOR_HPP
#define ALFALFA_COMPONENT_ACTUATOR_HPP

#include "ComponentBase.hpp"
#include "../utilities/idf/IdfObject.hpp"
#include "AlfalfaAPI.hpp"

namespace openstudio {
namespace alfalfa {
  class ALFALFA_API AlfalfaActuator : public ComponentBase
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

    std::string deriveName() const override;

    Json::Value toJSON() const override;

    ComponentCapability capability() const override {
      return ComponentCapability::Bidirectional;
    }

    ComponentType type() const override {
      return ComponentType::Actuator;
    }

    std::unique_ptr<ComponentBase> clone() const override {
      return std::make_unique<AlfalfaActuator>(*this);
    }

    std::string componentName() const;
    std::string componentType() const;
    std::string controlType() const;

    static bool acceptsObjectType(const IddObjectType& idd_type);

   private:
    std::string m_component_name;
    std::string m_component_type;
    std::string m_control_type;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
