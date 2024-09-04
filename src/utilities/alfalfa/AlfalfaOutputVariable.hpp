#ifndef ALFALFA_COMPONENT_OUTPUTVARIABLE_HPP
#define ALFALFA_COMPONENT_OUTPUTVARIABLE_HPP

#include "AlfalfaComponent.hpp"
#include "utilities/idf/IdfObject.hpp"
#include "utilities/core/Logger.hpp"

namespace openstudio {
namespace alfalfa {
  class AlfalfaOutputVariable : public AlfalfaComponent
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
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
