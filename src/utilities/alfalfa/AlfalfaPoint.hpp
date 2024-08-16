/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ALFALFA_POINT_HPP
#define ALFALFA_POINT_HPP

#include "AlfalfaComponent.hpp"
#include <string>
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  class AlfalfaPoint
  {
    public:
      AlfalfaPoint();

      AlfalfaPoint(std::string display_name);

      AlfalfaPoint(std::string display_name, std::string id);

      void setInput(AlfalfaComponent &component);

      void setOutput(AlfalfaComponent &component);

      Json::Value toJSON() const;

      std::string id() const;

    private:
      AlfalfaComponent *m_input, *m_output;
      std::string m_display_name, m_id;
  };
}
}

#endif
