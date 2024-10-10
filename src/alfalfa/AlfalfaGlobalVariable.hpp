#ifndef ALFALFA_COMPONENT_GLOBALVARIABLE_HPP
#define ALFALFA_COMPONENT_GLOBALVARIABLE_HPP

#include "ComponentBase.hpp"
#include "../utilities/idf/IdfObject.hpp"

#include "AlfalfaAPI.hpp"

namespace openstudio {
namespace alfalfa {
  class ALFALFA_API AlfalfaGlobalVariable : public ComponentBase {
    public:
    /**
       * Create an AlfalfaGlobalVariable Component from a variable name which reflect the EneregyManagementSystem:GlobalVariable EnergyPlus IDD
       */
    AlfalfaGlobalVariable(const std::string& variable_name);

    /**
       * Create an AlfalfaGlobalVariable Component from an IdfObject.
       * Valid idf_object types are EnergyManagementSystemGlobalVariable and IdfObject(IddObjectType::EnergyManagementSystem_GlobalVariable)
       */
    AlfalfaGlobalVariable(const IdfObject& global_variable);

    std::string deriveName() const override;

    Json::Value toJSON() const override;

    ComponentCapability capability() const override {
      return ComponentCapability::Bidirectional;
    }

    ComponentType type() const override {
      return ComponentType::GlobalVariable;
    }

    std::unique_ptr<ComponentBase> clone() const override {
        return std::make_unique<AlfalfaGlobalVariable>(*this);
    }

    std::string variableName() const;

    private:
    std::string m_variable_name;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
