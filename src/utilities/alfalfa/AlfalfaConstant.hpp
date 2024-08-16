#ifndef ALFALFA_CONSTANT_HPP
#define ALFALFA_CONSTANT_HPP

#include "AlfalfaComponent.hpp"
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  class AlfalfaConstant : public AlfalfaComponent
  {
   public:
    AlfalfaConstant(float value);

    ComponentCapabilities capabilities() const;

    Json::Value toJSON() const;

    std::string type() const;

   private:
    float m_value;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
