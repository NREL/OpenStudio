#include "ComponentBase.hpp"

namespace openstudio {
namespace alfalfa {

  bool ComponentBase::canInput() const {
    return capability() == ComponentCapability::Bidirectional || capability() == ComponentCapability::Input;
  }

  bool ComponentBase::canOutput() const {
    return capability() == ComponentCapability::Bidirectional || capability() == ComponentCapability::Output;
  }

}  // namespace alfalfa
}  // namespace openstudio
