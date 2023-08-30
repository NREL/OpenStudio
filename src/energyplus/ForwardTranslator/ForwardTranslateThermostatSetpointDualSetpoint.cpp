/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ThermostatSetpointDualSetpoint.hpp"
#include <utilities/idd/ThermostatSetpoint_DualSetpoint_FieldEnums.hxx>
#include <utilities/idd/ThermostatSetpoint_SingleHeating_FieldEnums.hxx>
#include <utilities/idd/ThermostatSetpoint_SingleCooling_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateThermostatSetpointDualSetpoint(ThermostatSetpointDualSetpoint& modelObject) {
    OptionalSchedule heat_sch = modelObject.getHeatingSchedule();
    OptionalSchedule cool_sch = modelObject.getCoolingSchedule();
    boost::optional<IdfObject> result;

    // Two schedules = DualSetpoint
    if (heat_sch.is_initialized() && cool_sch.is_initialized()) {
      IdfObject thermostat(openstudio::IddObjectType::ThermostatSetpoint_DualSetpoint);
      m_idfObjects.push_back(thermostat);

      // Name
      OptionalString s = modelObject.name();
      if (s) {
        thermostat.setName(*s);
      }

      // Heating Setpoint Temperature Schedule Name
      translateAndMapModelObject(*heat_sch);
      thermostat.setString(ThermostatSetpoint_DualSetpointFields::HeatingSetpointTemperatureScheduleName, heat_sch->name().get());

      // Cooling Setpoint Temperature Schedule Name
      translateAndMapModelObject(*cool_sch);
      thermostat.setString(ThermostatSetpoint_DualSetpointFields::CoolingSetpointTemperatureScheduleName, cool_sch->name().get());

      result = thermostat;

      // Heating only
    } else if (heat_sch.is_initialized() && !cool_sch.is_initialized()) {
      IdfObject thermostat(openstudio::IddObjectType::ThermostatSetpoint_SingleHeating);
      m_idfObjects.push_back(thermostat);

      // Name
      OptionalString s = modelObject.name();
      if (s) {
        thermostat.setName(*s);
      }

      // (Heating) Setpoint Temperature Schedule Name
      translateAndMapModelObject(*heat_sch);
      thermostat.setString(ThermostatSetpoint_SingleHeatingFields::SetpointTemperatureScheduleName, heat_sch->name().get());

      result = thermostat;

      // Cooling only
    } else if (!heat_sch.is_initialized() && cool_sch.is_initialized()) {
      IdfObject thermostat(openstudio::IddObjectType::ThermostatSetpoint_SingleCooling);
      m_idfObjects.push_back(thermostat);

      // Name
      OptionalString s = modelObject.name();
      if (s) {
        thermostat.setName(*s);
      }

      // (Cooling) Setpoint Temperature Schedule Name
      translateAndMapModelObject(*cool_sch);
      thermostat.setString(ThermostatSetpoint_SingleCoolingFields::SetpointTemperatureScheduleName, cool_sch->name().get());

      result = thermostat;
    }
    // No other cases, in ForwardTranslateThermalZone, we have checked that there is at least one schedule

    // Temperature Difference Between Cutout And Setpoint => Handle in ForwardTranslateThermalZone to place on ZoneControlThermostat object

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
