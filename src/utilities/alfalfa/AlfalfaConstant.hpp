#ifndef ALFALFA_COMPONENT_CONSTANT_HPP
#define ALFALFA_COMPONENT_CONSTANT_HPP

#include "AlfalfaComponent.hpp"
#include "utilities/core/Logger.hpp"

namespace openstudio {
namespace alfalfa {
  class AlfalfaConstant : public AlfalfaComponent {
    public:
      AlfalfaConstant(float value);

    private:
      REGISTER_LOGGER("openstudio.AlfalfaConstant");
  };
}
}

#endif
