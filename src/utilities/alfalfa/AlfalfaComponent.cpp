#include "AlfalfaComponent.hpp"

namespace openstudio {
namespace alfalfa {

  AlfalfaComponent::AlfalfaComponent(const std::string& type, Capability capabilities) : type(type), capabilities(capabilities) {}

  bool AlfalfaComponent::canInput() const {
    return capabilities & Capability::Input;
  }

  bool AlfalfaComponent::canOutput() const {
    return capabilities & Capability::Output;
  }

  bool AlfalfaComponent::operator==(const AlfalfaComponent& rhs) const {
    return (type == rhs.type) && (capabilities == rhs.capabilities) && (parameters == rhs.parameters);
  }

}  // namespace alfalfa
}  // namespace openstudio
