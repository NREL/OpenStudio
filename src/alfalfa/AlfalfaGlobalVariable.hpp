/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ALFALFA_COMPONENT_GLOBALVARIABLE_HPP
#define ALFALFA_COMPONENT_GLOBALVARIABLE_HPP

#include "AlfalfaAPI.hpp"

#include "AlfalfaComponentBase.hpp"

#include "../utilities/idf/IdfObject.hpp"

namespace openstudio {
namespace alfalfa {
  class ALFALFA_API AlfalfaGlobalVariable : public AlfalfaComponentBase
  {
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

    AlfalfaComponentCapability capability() const override {
      return AlfalfaComponentCapability::Bidirectional;
    }

    AlfalfaComponentType type() const override {
      return AlfalfaComponentType::GlobalVariable;
    }

    std::unique_ptr<AlfalfaComponentBase> clone() const override {
      return std::make_unique<AlfalfaGlobalVariable>(*this);
    }

    std::string variableName() const;

    static bool acceptsObjectType(const IddObjectType& idd_type);

   private:
    std::string m_variable_name;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
