#ifndef ALFALFA_COMPONENT_OUTPUTVARIABLE_HPP
#define ALFALFA_COMPONENT_OUTPUTVARIABLE_HPP

#include "AlfalfaAPI.hpp"

#include "AlfalfaComponentBase.hpp"

#include "../utilities/idf/IdfObject.hpp"

namespace openstudio {
namespace alfalfa {
  class ALFALFA_API AlfalfaOutputVariable : public AlfalfaComponentBase
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

    AlfalfaComponentCapability capability() const override {
      return AlfalfaComponentCapability::Output;
    }

    AlfalfaComponentType type() const override {
      return AlfalfaComponentType::OutputVariable;
    }

    std::unique_ptr<AlfalfaComponentBase> clone() const override {
      return std::make_unique<AlfalfaOutputVariable>(*this);
    }

    std::string variableKey() const;
    std::string variableName() const;

    static bool acceptsObjectType(const IddObjectType& idd_type);

   private:
    std::string m_variable_key;
    std::string m_variable_name;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
