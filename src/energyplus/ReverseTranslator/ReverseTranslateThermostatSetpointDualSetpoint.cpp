/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ThermostatSetpointDualSetpoint.hpp"
#include "../../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ThermostatSetpoint_DualSetpoint_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateThermostatSetpointDualSetpoint(const WorkspaceObject& workspaceObject) {
    OptionalModelObject result;
    OptionalModelObject temp;
    OptionalSchedule schedule;

    ThermostatSetpointDualSetpoint tsds(m_model);

    if (OptionalString os = workspaceObject.name()) {
      tsds.setName(*os);
    }

    OptionalWorkspaceObject owo = workspaceObject.getTarget(ThermostatSetpoint_DualSetpointFields::HeatingSetpointTemperatureScheduleName);
    if (!owo) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find Schedule: ");
      return result;
    }
    temp = translateAndMapWorkspaceObject(*owo);
    if (temp) {
      schedule = temp->optionalCast<Schedule>();
      if (schedule) {
        tsds.setHeatingSchedule(*schedule);
      }
    }

    owo = workspaceObject.getTarget(ThermostatSetpoint_DualSetpointFields::CoolingSetpointTemperatureScheduleName);
    if (!owo) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find Schedule: ");
      return result;
    }
    temp = translateAndMapWorkspaceObject(*owo);
    if (temp) {
      schedule = temp->optionalCast<Schedule>();
      if (schedule) {
        tsds.setCoolingSchedule(*schedule);
      }
    }

    result = tsds;
    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
