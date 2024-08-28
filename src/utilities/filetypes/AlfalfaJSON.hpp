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
#include "../core/Optional.hpp"
#include "../../model/EnergyManagementSystemGlobalVariable.hpp"
#include "../../model/EnergyManagementSystemOutputVariable.hpp"
#include "../../model/EnergyManagementSystemActuator.hpp"
#include "../../model/OutputMeter.hpp"
#include "../../model/OutputVariable.hpp"

namespace openstudio {
namespace alfalfa {
  namespace detail {
    class AlfalfaJSON_Impl;
  }

  class AlfalfaJSON
  {
   public:
    /** @name Constructors and Destructors */

    AlfalfaJSON();

    AlfalfaJSON(const std::string& s);

    AlfalfaJSON(const openstudio::path& p);

    bool save() const;

    bool saveAs(const openstudio::path& p);

    AlfalfaPoint exposeConstant(float value, const std::string& display_name);

    AlfalfaPoint exposeMeter(const std::string& meter_name, const std::string& display_name = std::string());

    boost::optional<AlfalfaPoint> exposeMeter(const openstudio::IdfObject& output_meter, const std::string& display_name = std::string());

    AlfalfaPoint exposeActuator(const std::string& component_type, const std::string& control_type, const std::string& actuator_key,
                                const std::string& display_name = std::string());

    boost::optional<AlfalfaPoint> exposeActuator(const openstudio::IdfObject& actuator, const std::string& display_name = std::string());

    AlfalfaPoint exposeOutputVariable(const std::string& variable_key, const std::string& variable_name,
                                      const std::string& display_name = std::string());

    boost::optional<AlfalfaPoint> exposeOutputVariable(const openstudio::IdfObject& output_variable, const std::string& display_name = std::string());

    AlfalfaPoint exposeGlobalVariable(const std::string& variable_name, const std::string& display_name = std::string());

    boost::optional<AlfalfaPoint> exposeGlobalVariable(const openstudio::IdfObject& global_variable, const std::string& display_name = std::string());

    void exposePoint(const AlfalfaPoint& point);

    boost::optional<AlfalfaPoint> exposePoint(const openstudio::IdfObject& idf_object, const std::string& display_name = std::string());

    std::vector<AlfalfaPoint> getPoints();

   private:
    std::shared_ptr<detail::AlfalfaJSON_Impl> m_impl;

    // configure logging
    REGISTER_LOGGER("openstudio.AlfalfaJSON");
  };

}  // namespace alfalfa
}  // namespace openstudio

#endif
