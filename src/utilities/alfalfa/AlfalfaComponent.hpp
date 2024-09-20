#ifndef ALFALFA_COMPONENT_HPP
#define ALFALFA_COMPONENT_HPP

#include <json/json.h>

#include "../UtilitiesAPI.hpp"

namespace openstudio {
namespace alfalfa {
  enum Capability
  {
    Input = 1,
    Output = 2
  };

  inline Capability operator|(const Capability& a, const Capability& b) {
    return static_cast<Capability>(static_cast<int>(a) | static_cast<int>(b));
  }

  inline Capability operator&(const Capability& a, const Capability& b) {
    return static_cast<Capability>(static_cast<int>(a) & static_cast<int>(b));
  }

  class UTILITIES_API AlfalfaComponent
  {
   public:
    AlfalfaComponent(const std::string& type, Capability capabilities);

    /**
     * Get if the component can be used as an Input to the model
     */
    bool canInput() const;

    /**
     * Get if the component can be used as an Output from the model
     */
    bool canOutput() const;

    bool operator==(const AlfalfaComponent& rhs) const;

    /**
     * A string which reflects the type of the component
     */
    std::string type;

    /**
     * The raw input/output capabilities of the component
     */
    Capability capabilities;

    /**
     * JSON encoded parameters of the component
     */
    Json::Value parameters;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
