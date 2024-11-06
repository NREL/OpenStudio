#include "ComponentBase.hpp"

namespace openstudio {
namespace alfalfa {

  bool ComponentBase::canInput() const {
    return capability() == AlfalfaComponentCapability::Bidirectional || capability() == AlfalfaComponentCapability::Input;
  }

  bool ComponentBase::canOutput() const {
    return capability() == AlfalfaComponentCapability::Bidirectional || capability() == AlfalfaComponentCapability::Output;
  }

}  // namespace alfalfa
}  // namespace openstudio
