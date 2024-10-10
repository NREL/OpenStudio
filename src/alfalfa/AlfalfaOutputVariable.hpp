#ifndef ALFALFA_COMPONENT_OUTPUTVARIABLE_HPP
#define ALFALFA_COMPONENT_OUTPUTVARIABLE_HPP

#include "ComponentBase.hpp"
#include "../utilities/idf/IdfObject.hpp"

#include "AlfalfaAPI.hpp"

namespace openstudio {
namespace alfalfa {
  class ALFALFA_API AlfalfaOutputVariable : public ComponentBase
  {
   public:
    /**
       * Create an AlfalfaOutputVariable Component from a variable key and variable name which reflect the Output:Variable EnergyPlus IDD
       */
    AlfalfaOutputVariable(const std::string& variable_key, const std::string& variable_name);

    /**
       * Create an AlfalfaOutputVariable Component from an IdfObject.
       * Valid idf_object types are OutputVariable, EnergyManagementSystemOutputVariable, IdfObject(IddObjectType::Output_Variable) and IdfObject(IddObjectType::EnergyManagementSystem_OutputVariable)
       */
    AlfalfaOutputVariable(const openstudio::IdfObject& output_variable);

    std::string deriveName() const override;

    Json::Value toJSON() const override;

    ComponentCapability capability() const override {
      return ComponentCapability::Output;
    }

    ComponentType type() const override {
      return ComponentType::OutputVariable;
    }

    std::unique_ptr<ComponentBase> clone() const override {
      return std::make_unique<AlfalfaOutputVariable>(*this);
    }

    std::string variableKey() const;
    std::string variableName() const;

   private:
    std::string m_variable_key;
    std::string m_variable_name;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
