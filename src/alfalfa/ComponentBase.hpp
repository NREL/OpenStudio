#ifndef ALFALFA_COMPONENTBASE_HPP
#define ALFALFA_COMPONENTBASE_HPP

#include "AlfalfaAPI.hpp"

#include "../utilities/core/Enum.hpp"

#include <json/json.h>

namespace openstudio {
namespace alfalfa {

  OPENSTUDIO_ENUM(ComponentCapability, ((Input))((Output))((Bidirectional)))

  OPENSTUDIO_ENUM(ComponentType, ((Actuator))((Constant))((Meter))((OutputVariable))((GlobalVariable)))

  class ALFALFA_API ComponentBase
  {
   public:
    virtual ~ComponentBase() = default;

    virtual Json::Value toJSON() const = 0;

    virtual ComponentCapability capability() const = 0;

    virtual ComponentType type() const = 0;

    virtual std::string typeName() const {
      return type().valueName();
    }

    virtual std::string deriveName() const = 0;

    virtual std::unique_ptr<ComponentBase> clone() const = 0;

    virtual bool canInput() const;

    virtual bool canOutput() const;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
