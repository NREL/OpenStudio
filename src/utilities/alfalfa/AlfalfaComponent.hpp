#ifndef ALFALFA_COMPONENT_HPP
#define ALFALFA_COMPONENT_HPP

#include <stdint.h>
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  enum ComponentCapabilities
  {
    Input = 1,
    Output = 2
  };

  class AlfalfaComponent
  {
   public:
    AlfalfaComponent(std::string type, ComponentCapabilities capabilities);

    /** Returns capabilities of component */
    ComponentCapabilities capabilities() const;

    Json::Value toJSON() const;

    std::string type() const;

   protected:
    std::string m_type;

    Json::Value m_root;

    ComponentCapabilities m_capabilities;
  };

  inline ComponentCapabilities operator|(const ComponentCapabilities& a, const ComponentCapabilities& b) {
    return static_cast<ComponentCapabilities>(static_cast<int>(a) | static_cast<int>(b));
  }
}  // namespace alfalfa
}  // namespace openstudio

#endif
