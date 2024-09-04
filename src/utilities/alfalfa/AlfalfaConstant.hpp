#ifndef ALFALFA_COMPONENT_CONSTANT_HPP
#define ALFALFA_COMPONENT_CONSTANT_HPP

#include "AlfalfaComponent.hpp"
#include "utilities/core/Logger.hpp"

#include "../UtilitiesAPI.hpp"

namespace openstudio {
namespace alfalfa {
  class UTILITIES_API AlfalfaConstant : public AlfalfaComponent
  {
   public:
    /**
     * Create an AlfalfaConstant Component with a set value.
     */
    AlfalfaConstant(float value);
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
