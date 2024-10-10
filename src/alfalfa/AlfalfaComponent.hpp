#ifndef ALFALFA_COMPONENT_HPP
#define ALFALFA_COMPONENT_HPP

#include <json/json.h>
#include <type_traits>

#include "AlfalfaAPI.hpp"
#include "ComponentBase.hpp"

namespace openstudio {
namespace alfalfa {
  class ALFALFA_API AlfalfaComponent {
    public:

    template <typename T,
              std::enable_if_t<std::is_base_of<ComponentBase, T>::value, bool> = true>
    AlfalfaComponent(T component) : m_component(std::make_unique<T>(std::move(component))) {}

    AlfalfaComponent(const AlfalfaComponent& other) : m_component(other.m_component->clone()) {}

    AlfalfaComponent(AlfalfaComponent&&) noexcept = default;

    AlfalfaComponent& operator=(const AlfalfaComponent& other) {
      if (this != &other) {
        m_component = other.m_component->clone();
      }
      return *this;
    }

    AlfalfaComponent& operator=(AlfalfaComponent&&) noexcept = default;

    Json::Value toJSON() const;

    ComponentCapability capability() const;

    ComponentType type() const;

    std::string typeName() const;

    std::string deriveName() const;

    bool canInput() const;

    bool canOutput() const;



    private:
      AlfalfaComponent() = default;
      std::unique_ptr<ComponentBase> m_component;
  };
template <typename L, typename R,
    std::enable_if_t<(std::is_base_of_v<ComponentBase, L> ||
    std::is_same_v<AlfalfaComponent, L>) &&
    (std::is_base_of_v<ComponentBase, R> ||
    std::is_same_v<AlfalfaComponent, R>), bool> = true>
inline bool operator==(const L& lhs, const R& rhs) {
  return (lhs.type() == rhs.type() && lhs.toJSON() == rhs.toJSON());
}
}  // namespace alfalfa
}  // namespace openstudio

#endif
