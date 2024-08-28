#include "AlfalfaComponent.hpp"
#include <iostream>

namespace openstudio {
namespace alfalfa {

  AlfalfaComponent::AlfalfaComponent(std::string type, ComponentCapabilities capabilities) : m_type(type), m_capabilities(capabilities) {}

  std::string AlfalfaComponent::type() const {
    return m_type;
  }

  ComponentCapabilities AlfalfaComponent::capabilities() const {
    return m_capabilities;
  }

  Json::Value AlfalfaComponent::toJSON() const {
    return m_root;
  }
}  // namespace alfalfa
}  // namespace openstudio
