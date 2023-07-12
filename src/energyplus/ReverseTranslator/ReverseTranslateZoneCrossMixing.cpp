/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ZoneMixing.hpp"
#include "../../model/ZoneMixing_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneCrossMixing_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneCrossMixing(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneCrossMixing) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZoneCrossMixing");
      return boost::none;
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::ZoneorSpaceName);
    OptionalThermalZone zone;
    if (target) {
      if (OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target)) {
        if (target->iddObject().type() == IddObjectType::Zone) {
          // Zone maps to Space in RT
          if (auto s_ = modelObject->optionalCast<Space>()) {
            zone = s_->thermalZone();
          }
        } else {
          // It's a space, but we don't allow mapping to a space in model SDK for now, and we don't have a Space RT... so we'll never get here
        }
      }
    }

    if (!zone) {
      return boost::none;
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::SourceZoneorSpaceName);
    OptionalThermalZone sourceZone;
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (target->iddObject().type() == IddObjectType::Zone) {
        // Zone maps to Space in RT
        if (auto s_ = modelObject->optionalCast<Space>()) {
          sourceZone = s_->thermalZone();
        }
      } else {
        // It's a space, but we don't allow mapping to a space in model SDK for now, and we don't have a Space RT... so we'll never get here
      }
    }

    if (!sourceZone) {
      return boost::none;
    }

    // Create the mixing object
    openstudio::model::ZoneMixing mixing(*zone);
    mixing.setSourceZone(*sourceZone);

    // Create the reverse mixing object
    openstudio::model::ZoneMixing reverseMixing(*sourceZone);
    reverseMixing.setSourceZone(*zone);

    OptionalString s = workspaceObject.name();
    if (s) {
      mixing.setName(*s);
      reverseMixing.setName(*s + " Reverse");
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::ScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Schedule>()) {
          auto s = modelObject->cast<Schedule>();
          mixing.setSchedule(s);
          reverseMixing.setSchedule(s);
        }
      }
    }

    s = workspaceObject.getString(openstudio::ZoneCrossMixingFields::DesignFlowRateCalculationMethod, true);
    OS_ASSERT(s);

    OptionalDouble d;
    if (istringEqual("Flow/Zone", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneCrossMixingFields::DesignFlowRate);
      if (d) {
        mixing.setDesignFlowRate(*d);
        reverseMixing.setDesignFlowRate(*d);
      } else {
        LOG(Error, "Flow/Zone value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Flow/Area", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneCrossMixingFields::FlowRateperFloor);
      if (d) {
        mixing.setFlowRateperFloorArea(*d);
        reverseMixing.setFlowRateperFloorArea(*d);
      } else {
        LOG(Error, "Flow/Area value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Flow/Person", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneCrossMixingFields::FlowRateperPerson);
      if (d) {
        mixing.setFlowRateperPerson(*d);
        reverseMixing.setFlowRateperPerson(*d);
      } else {
        LOG(Error, "Flow/Person value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("AirChanges/Hour", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneCrossMixingFields::AirChangesperHour);
      if (d) {
        mixing.setAirChangesperHour(*d);
        reverseMixing.setAirChangesperHour(*d);
      } else {
        LOG(Error, "AirChanges/Hour value not found for workspace object " << workspaceObject);
      }
    } else {
      LOG(Error, "Unknown DesignFlowRateCalculationMethod value for workspace object" << workspaceObject);
    }

    d = workspaceObject.getDouble(openstudio::ZoneCrossMixingFields::DeltaTemperature);
    if (d) {
      mixing.setDeltaTemperature(*d);
      reverseMixing.setDeltaTemperature(*d);
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::DeltaTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setDeltaTemperatureSchedule(s.get());
          reverseMixing.setDeltaTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MinimumReceivingTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMinimumReceivingTemperatureSchedule(s.get());
          reverseMixing.setMinimumReceivingTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MaximumReceivingTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMaximumReceivingTemperatureSchedule(s.get());
          reverseMixing.setMaximumReceivingTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MinimumSourceTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMinimumSourceTemperatureSchedule(s.get());
          reverseMixing.setMinimumSourceTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MaximumSourceTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMaximumSourceTemperatureSchedule(s.get());
          reverseMixing.setMaximumSourceTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MinimumOutdoorTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMinimumOutdoorTemperatureSchedule(s.get());
          reverseMixing.setMinimumOutdoorTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MaximumOutdoorTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMaximumOutdoorTemperatureSchedule(s.get());
          reverseMixing.setMaximumOutdoorTemperatureSchedule(s.get());
        }
      }
    }

    return mixing;
  }

}  // namespace energyplus

}  // namespace openstudio
