#include "AlfalfaComponentBase.hpp"

namespace openstudio {
namespace alfalfa {

  bool AlfalfaComponentBase::canInput() const {
    return capability() == AlfalfaComponentCapability::Bidirectional || capability() == AlfalfaComponentCapability::Input;
  }

  bool AlfalfaComponentBase::canOutput() const {
    return capability() == AlfalfaComponentCapability::Bidirectional || capability() == AlfalfaComponentCapability::Output;
  }

}  // namespace alfalfa
}  // namespace openstudio
