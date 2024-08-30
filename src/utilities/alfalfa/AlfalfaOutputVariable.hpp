#ifndef ALFALFA_COMPONENT_OUTPUTVARIABLE_HPP
#define ALFALFA_COMPONENT_OUTPUTVARIABLE_HPP

#include "AlfalfaComponent.hpp"
#include "utilities/idf/IdfObject.hpp"
#include "utilities/core/Logger.hpp"

namespace openstudio {
namespace alfalfa {
  class AlfalfaOutputVariable : public AlfalfaComponent {
    public:
      AlfalfaOutputVariable(const std::string& variable_key, const std::string& variable_name);

      AlfalfaOutputVariable(const openstudio::IdfObject& output_variable);

    private:
      // configure logging
      REGISTER_LOGGER("openstudio.AlfalfaOutputVariable");
  };
}
}

#endif
