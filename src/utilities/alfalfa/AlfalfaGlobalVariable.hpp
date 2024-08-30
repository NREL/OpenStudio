#ifndef ALFALFA_COMPONENT_GLOBALVARIABLE_HPP
#define ALFALFA_COMPONENT_GLOBALVARIABLE_HPP

#include "AlfalfaComponent.hpp"
#include "utilities/idf/IdfObject.hpp"
#include "utilities/core/Logger.hpp"

namespace openstudio {
namespace alfalfa {
  class AlfalfaGlobalVariable : public AlfalfaComponent
  {
   public:
    AlfalfaGlobalVariable(const std::string& variable_name);

    AlfalfaGlobalVariable(const IdfObject& global_variable);

   private:
    // configure logging
    REGISTER_LOGGER("openstudio.AlfalfaGlobalVariable");
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
