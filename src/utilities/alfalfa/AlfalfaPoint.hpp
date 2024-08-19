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
  class AlfalfaPoint
  {
   public:
    AlfalfaPoint();

    AlfalfaPoint(const std::string& display_name);

    AlfalfaPoint(const std::string& display_name, const std::string& id);

    void setInput(AlfalfaComponent& component);

    boost::optional<AlfalfaComponent*> getInput();

    void setOutput(AlfalfaComponent& component);

    boost::optional<AlfalfaComponent*> getOutput();

    void setUnits(const std::string& units);

    boost::optional<std::string> getUnits();

    Json::Value toJSON() const;

    std::string id() const;

   private:
    AlfalfaComponent *m_input, *m_output;
    std::string m_display_name, m_id, m_units;
  };
}  // namespace alfalfa
}  // namespace openstudio

#endif
