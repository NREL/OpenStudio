#ifndef ALFALFA_COMPONENT_HPP
#define ALFALFA_COMPONENT_HPP

#include <json/json.h>

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

  class AlfalfaComponent
  {
   public:
    AlfalfaComponent(const std::string& type, const Capability capabilities);

    bool canInput() const;

    bool canOutput() const;

    std::string type;
    Capability capabilities;
    Json::Value parameters;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
