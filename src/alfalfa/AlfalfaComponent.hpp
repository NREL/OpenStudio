/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ALFALFA_COMPONENT_HPP
#define ALFALFA_COMPONENT_HPP

#include <json/json.h>
#include <type_traits>

#include "AlfalfaAPI.hpp"
#include "AlfalfaComponentBase.hpp"

namespace openstudio {
namespace alfalfa {
  class ALFALFA_API AlfalfaComponent
  {
   public:
    AlfalfaComponent(const AlfalfaComponentBase& component) : m_component(component.clone()) {}

    AlfalfaComponent(const AlfalfaComponent& other) : m_component(other.m_component->clone()) {}

    AlfalfaComponent(AlfalfaComponent&&) noexcept = default;

    ~AlfalfaComponent() = default;

    AlfalfaComponent& operator=(const AlfalfaComponent& other) {
      if (this != &other) {
        m_component = other.m_component->clone();
      }
      return *this;
    }

    AlfalfaComponent& operator=(AlfalfaComponent&&) noexcept = default;

    Json::Value toJSON() const;

    AlfalfaComponentCapability capability() const;

    AlfalfaComponentType type() const;

    std::string typeName() const;

    std::string deriveName() const;

    bool canInput() const;

    bool canOutput() const;

   private:
    AlfalfaComponent() = default;
    std::unique_ptr<AlfalfaComponentBase> m_component;
  };

  inline bool operator==(const AlfalfaComponent& lhs, const AlfalfaComponent& rhs) {
    return (lhs.type() == rhs.type() && lhs.toJSON() == rhs.toJSON());
  }
}  // namespace alfalfa
}  // namespace openstudio

#endif
