#include "AlfalfaComponent.hpp"

namespace openstudio {
namespace alfalfa {
  Json::Value AlfalfaComponent::toJSON() const {
    return m_component->toJSON();
  }

  AlfalfaComponentCapability AlfalfaComponent::capability() const {
    return m_component->capability();
  }

  AlfalfaComponentType AlfalfaComponent::type() const {
    return m_component->type();
  }

  std::string AlfalfaComponent::typeName() const {
    return m_component->typeName();
  }

  std::string AlfalfaComponent::deriveName() const {
    return m_component->deriveName();
  }

  bool AlfalfaComponent::canInput() const {
    return m_component->canInput();
  }

  bool AlfalfaComponent::canOutput() const {
    return m_component->canOutput();
  }

}  // namespace alfalfa
}  // namespace openstudio
