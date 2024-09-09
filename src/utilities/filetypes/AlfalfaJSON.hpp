/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ALFALFA_ALFALFA_HPP
#define ALFALFA_ALFALFA_HPP

#include <vector>
#include <json/json.h>

#include "../UtilitiesAPI.hpp"
#include "../alfalfa/AlfalfaPoint.hpp"
#include "../core/Path.hpp"
#include "../core/Logger.hpp"
#include "../core/Optional.hpp"

#include "model/ModelObject.hpp"

namespace openstudio {
namespace alfalfa {
  namespace detail {
    class AlfalfaJSON_Impl;
  }

  class UTILITIES_API AlfalfaJSON
  {
   public:
    /** @name Constructors and Destructors */

    AlfalfaJSON();

    AlfalfaJSON(const std::string& s);

    AlfalfaJSON(const openstudio::path& p);

    bool save() const;

    bool saveAs(const openstudio::path& p);

    Json::Value toJSON() const;

    /**
     * Expose a point with a constant output value.
     * display_name is required
     */
    boost::optional<AlfalfaPoint> exposeConstant(float value, const std::string& display_name);

    /**
     * Expose a point with a meter as an output.
     * display_name is optional and by default will reflect the name of the meter.
     */
    boost::optional<AlfalfaPoint> exposeMeter(const std::string& meter_name, const std::string& display_name = std::string());

    /**
     * Expose a point with a meter as an output.
     * display_name is optional and by default will be infered from the displayName for model objects or meter name for idf objects.
     * Valid idf_object types are OutputMeter and IdfObject(IddObjectType::Output_Meter)
     */
    boost::optional<AlfalfaPoint> exposeMeter(const openstudio::IdfObject& output_meter, const std::string& display_name = std::string());

    /**
     * Expose a point with an actuator as input and ouput value.
     * display_name is optional and by default will reflect the parameters of the actuator.
     */
    boost::optional<AlfalfaPoint> exposeActuator(const std::string& component_name, const std::string& component_type,
                                                 const std::string& control_type, const std::string& display_name = std::string());

    /**
     * Expose a point with an actuator as input and ouput value.
     * display_name is optional and by default will be infered from the displayName for model objects or parameters for idf objects.
     * Valid idf_object types are EnergyManagementSystemActuator and IdfObject(IddObjectType::EnergyManagementSystem_Actuator)
     */
    boost::optional<AlfalfaPoint> exposeActuator(const openstudio::IdfObject& actuator, const std::string& display_name = std::string());

    /**
     * Expose point with Output Variable as output value.
     * display_name is optional and by default will reflect the parameters of the output variable.
     */
    boost::optional<AlfalfaPoint> exposeOutputVariable(const std::string& variable_key, const std::string& variable_name,
                                                       const std::string& display_name = std::string());

    /**
     * Expose point with Output Variable as output value.
     * display_name is optional and by default will be infered from the displayName for model objects or parameters for idf objects.
     * Valid idf_object types are OutputVariable, EnergyManagementSystemOutputVariable, IdfObject(IddObjectType::Output_Variable) and IdfObject(IddObjectType::EnergyManagementSystem_OutputVariable)
     */
    boost::optional<AlfalfaPoint> exposeOutputVariable(const openstudio::IdfObject& output_variable, const std::string& display_name = std::string());

    /**
     * Expose point with Global Variable as output and input value.
     * display_name is optional and by default will reflect the parameters of the global variable.
     */
    boost::optional<AlfalfaPoint> exposeGlobalVariable(const std::string& variable_name, const std::string& display_name = std::string());

    /**
     * Expose point with Global Variable as output and input value.
     * display_name is optional and by default will be infered from the displayName for model objects or parameters for idf objects.
     * Valid idf_object types are EnergyManagementSystemGlobalVariable and IdfObject(IddObjectType::EnergyManagementSystem_GlobalVariable)
     */
    boost::optional<AlfalfaPoint> exposeGlobalVariable(const openstudio::IdfObject& global_variable, const std::string& display_name = std::string());

    /**
     * Expose a point in the Alfalfa API
     */
    void exposePoint(const AlfalfaPoint& point);

    /**
     * Expose a point in the Alfalfa API from a ModelObject or IdfObject
     * display_name is optional and by default will be infered from the displayName for model objects or parameters for idf objects.
     * Valid idf_object types are EnergyManagementSystemActuator and IdfObject(IddObjectType::EnergyManagementSystem_Actuator) for actuators,
     * OutputVariable, EnergyManagementSystemOutputVariable, IdfObject(IddObjectType::Output_Variable) and IdfObject(IddObjectType::EnergyManagementSystem_OutputVariable) for output variables,
     * OutputMeter and IdfObject(IddObjectType::Output_Meter) for meters,
     * and EnergyManagementSystemGlobalVariable and IdfObject(IddObjectType::EnergyManagementSystem_GlobalVariable) for global variables.
     */
    boost::optional<AlfalfaPoint> exposePoint(const openstudio::IdfObject& idf_object, const std::string& display_name = std::string());

    /**
     * Expose a point in the alfalfa API from an AlfalfaComponent.
     * display_name is optional and by default will be infered from the contents of the component.
     * Input and Ouput of the point will reflect the capabilities of the component.
     */
    boost::optional<AlfalfaPoint> exposePoint(const AlfalfaComponent& component, const std::string& display_name = std::string());

    /**
     * Get a vector of all points currently exported to the Alfalfa API
     */
    std::vector<AlfalfaPoint> getPoints();

   private:
    boost::optional<std::string> getName(const openstudio::IdfObject& idf_object);

    std::shared_ptr<detail::AlfalfaJSON_Impl> m_impl;

    // configure logging
    REGISTER_LOGGER("openstudio.AlfalfaJSON");
  };

}  // namespace alfalfa
}  // namespace openstudio

#endif
