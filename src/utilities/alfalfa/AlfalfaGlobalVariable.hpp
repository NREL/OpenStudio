#ifndef ALFALFA_COMPONENT_GLOBALVARIABLE_HPP
#define ALFALFA_COMPONENT_GLOBALVARIABLE_HPP

#include "AlfalfaComponent.hpp"
#include "utilities/idf/IdfObject.hpp"
#include "utilities/core/Logger.hpp"

#include "../UtilitiesAPI.hpp"

namespace openstudio {
namespace alfalfa {
  class UTILITIES_API AlfalfaGlobalVariable : public AlfalfaComponent
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
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
