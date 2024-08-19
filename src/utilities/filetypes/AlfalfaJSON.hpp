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

  class AlfalfaJSON
  {
   public:
    /** @name Constructors and Destructors */

    AlfalfaJSON();

    AlfalfaJSON(const std::string& s);

    AlfalfaJSON(const openstudio::path& p);

    bool save() const;

    bool saveAs(const openstudio::path& p);

    AlfalfaPoint addConstant(const std::string& display_name, float value);

    AlfalfaPoint addMeter(const std::string& display_name, const std::string& meter_name);

    boost::optional<AlfalfaPoint> addMeter(const std::string& display_name, const openstudio::model::OutputMeter& output_meter);

    AlfalfaPoint addActuator(const std::string& display_name, const std::string& component_type, const std::string& control_type,
                             const std::string& actuator_key);

    boost::optional<AlfalfaPoint> addActuator(const std::string& display_name, const openstudio::model::EnergyManagementSystemActuator& actuator);

    AlfalfaPoint addOutputVariable(const std::string& display_name, const std::string& variable_key, const std::string& variable_name);

    boost::optional<AlfalfaPoint> addOutputVariable(const std::string& display_name, const openstudio::model::OutputVariable& output_variable);

    boost::optional<AlfalfaPoint> addOutputVariable(const openstudio::model::OutputVariable& output_variable);

    boost::optional<AlfalfaPoint> addOutputVariable(const std::string& display_name,
                                                    const openstudio::model::EnergyManagementSystemOutputVariable& output_variable);

    boost::optional<AlfalfaPoint> addOutputVariable(const openstudio::model::EnergyManagementSystemOutputVariable& output_variable);

    AlfalfaPoint addGlobalVariable(const std::string& display_name, const std::string& variable_name);

    boost::optional<AlfalfaPoint> addGlobalVariable(const std::string& display_name,
                                                    const openstudio::model::EnergyManagementSystemGlobalVariable& global_variable);

    boost::optional<AlfalfaPoint> addGlobalVariable(const openstudio::model::EnergyManagementSystemGlobalVariable& global_variable);

    void addPoint(const AlfalfaPoint& point);

   private:
    openstudio::path m_JSONPath;

    std::vector<AlfalfaPoint> m_points;

    // configure logging
    REGISTER_LOGGER("openstudio.AlfalfaJSON");
  };

}  // namespace alfalfa
}  // namespace openstudio

#endif
