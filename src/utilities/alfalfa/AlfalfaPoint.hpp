/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ALFALFA_POINT_HPP
#define ALFALFA_POINT_HPP

#include "AlfalfaComponent.hpp"
#include "../core/Optional.hpp"

#include <string>
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  namespace detail {
    class AlfalfaPoint_Impl;
  }
  class AlfalfaPoint
  {
   public:
    AlfalfaPoint();

    AlfalfaPoint(const std::string& display_name);

    AlfalfaPoint(const std::string& display_name, const std::string& id);

    void setInput(const AlfalfaComponent& component);

    boost::optional<AlfalfaComponent> getInput();

    void setOutput(const AlfalfaComponent& component);

    boost::optional<AlfalfaComponent> getOutput();

    void setUnits(const std::string& units);

    boost::optional<std::string> getUnits();

    Json::Value toJSON() const;

    std::string id() const;

    bool operator==(const AlfalfaPoint& rhs) const;

   private:
    std::shared_ptr<detail::AlfalfaPoint_Impl> m_impl;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
