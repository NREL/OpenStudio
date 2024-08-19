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
    virtual ~AlfalfaComponent() = default;

    /** Returns capabilities of component */
    virtual ComponentCapabilities capabilities() const = 0;

    virtual Json::Value toJSON() const = 0;

    virtual std::string type() const = 0;
  };

  inline ComponentCapabilities operator|(const ComponentCapabilities& a, const ComponentCapabilities& b) {
    return static_cast<ComponentCapabilities>(static_cast<int>(a) | static_cast<int>(b));
  }
}  // namespace alfalfa
}  // namespace openstudio

#endif
