/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ALFALFA_ALFALFA_HPP
#define ALFALFA_ALFALFA_HPP

#include <vector>
#include <json/json.h>

#include "../alfalfa/AlfalfaPoint.hpp"
#include "../core/Path.hpp"
#include "../core/Logger.hpp"

namespace openstudio {
namespace alfalfa {

  class AlfalfaJSON
  {
    public:
      /** @name Constructors and Destructors */

      AlfalfaJSON();

      AlfalfaJSON(const std::string& s);

      AlfalfaJSON(const openstudio::path& p);

      bool save() const;

      bool saveAs(const openstudio::path& p);

      AlfalfaPoint addConstant(const std::string display_name, const float value);

      AlfalfaPoint addMeter(const std::string display_name, const std::string meter_name);

      AlfalfaPoint addActuator(const std::string display_name, const std::string component_type, const std::string control_type, const std::string actuator_key);

      AlfalfaPoint addOutputVariable(const std::string display_name, const std::string variable_name, const std::string variable_key);

      AlfalfaPoint addGlobalVariable(const std::string display_name, const std::string variable_name);

      void addPoint(const AlfalfaPoint &point);

    private:

      openstudio::path m_JSONPath;

      std::vector<AlfalfaPoint> m_points;

      // configure logging
      REGISTER_LOGGER("openstudio.AlfalfaJSON");

  };

}
}

#endif
