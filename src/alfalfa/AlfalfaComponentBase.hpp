/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ALFALFA_COMPONENTBASE_HPP
#define ALFALFA_COMPONENTBASE_HPP

#include "AlfalfaAPI.hpp"

#include "../utilities/data/DataEnums.hpp"

#include <json/json.h>

namespace openstudio {
namespace alfalfa {

  class ALFALFA_API AlfalfaComponentBase
  {
   public:
    virtual ~AlfalfaComponentBase() = default;

    virtual Json::Value toJSON() const = 0;

    virtual AlfalfaComponentCapability capability() const = 0;

    virtual AlfalfaComponentType type() const = 0;

    virtual std::string typeName() const {
      return type().valueName();
    }

    virtual std::string deriveName() const = 0;

    virtual std::unique_ptr<AlfalfaComponentBase> clone() const = 0;

    virtual bool canInput() const;

    virtual bool canOutput() const;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
