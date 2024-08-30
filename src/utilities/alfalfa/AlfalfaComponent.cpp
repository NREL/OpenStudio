#include "AlfalfaComponent.hpp"
#include <iostream>

namespace openstudio {
namespace alfalfa {

  AlfalfaComponent::AlfalfaComponent(const std::string& type, const Capability capabilities)
    : type(type), capabilities(capabilities) {}

  bool AlfalfaComponent::canInput() const {
    return capabilities & Capability::Input;

  }

  bool AlfalfaComponent::canOutput() const {
    return capabilities & Capability::Output;
  }

}  // namespace alfalfa
}  // namespace openstudio
