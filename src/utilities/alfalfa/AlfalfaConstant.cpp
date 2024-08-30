#include "AlfalfaConstant.hpp"

namespace openstudio {
namespace alfalfa {
  AlfalfaConstant::AlfalfaConstant(float value) : AlfalfaComponent("Constant", Capability::Output) {
    parameters["value"] = value;
  }
}  // namespace alfalfa
}  // namespace openstudio
