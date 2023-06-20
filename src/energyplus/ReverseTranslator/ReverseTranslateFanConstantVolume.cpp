/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateFanConstantVolume(const WorkspaceObject& workspaceObject) {
    OptionalModelObject result;
    OptionalModelObject temp;
    OptionalSchedule schedule;

    OptionalWorkspaceObject owo = workspaceObject.getTarget(Fan_ConstantVolumeFields::AvailabilityScheduleName);
    if (!owo) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find Schedule: ");
      return result;
    }
    temp = translateAndMapWorkspaceObject(*owo);
    if (temp) {
      schedule = temp->optionalCast<Schedule>();
    }

    if (!schedule) {
      LOG(Error, "Error importing object: " << workspaceObject.name().get()
                                            << "Failed to convert iddObjects into model Objects. Maybe they do not exist in model yet");

      return result;
    }

    openstudio::model::FanConstantVolume fan(m_model, *schedule);
    OptionalString optS = workspaceObject.name();
    if (optS) {
      fan.setName(*optS);
    }
    //inlet and outlet nodes are set my the HVACAirLoop
    OptionalDouble d;
    d = workspaceObject.getDouble(openstudio::Fan_ConstantVolumeFields::FanTotalEfficiency);
    if (d) {
      fan.setFanEfficiency(*d);
    }
    d = workspaceObject.getDouble(openstudio::Fan_ConstantVolumeFields::PressureRise);
    if (d) {
      fan.setPressureRise(*d);
    }
    d = workspaceObject.getDouble(openstudio::Fan_ConstantVolumeFields::MaximumFlowRate);
    if (d) {
      fan.setMaximumFlowRate(*d);
    }
    d = workspaceObject.getDouble(openstudio::Fan_ConstantVolumeFields::MotorEfficiency);
    if (d) {
      fan.setMotorEfficiency(*d);
    }
    d = workspaceObject.getDouble(openstudio::Fan_ConstantVolumeFields::MotorInAirstreamFraction);
    if (d) {
      fan.setMotorInAirstreamFraction(*d);
    }

    optS = workspaceObject.getString(openstudio::Fan_ConstantVolumeFields::EndUseSubcategory);
    if (optS) {
      fan.setEndUseSubcategory(*optS);
    }

    result = fan;
    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
