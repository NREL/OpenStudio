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

#include <utilities/idd/ZoneMixing_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneMixing(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneMixing) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZoneMixing");
      return boost::none;
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ZoneMixingFields::ZoneorSpaceName);
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

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::SourceZoneorSpaceName);
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

    openstudio::model::ZoneMixing mixing(*zone);
    mixing.setSourceZone(sourceZone.get());

    OptionalString s = workspaceObject.name();
    if (s) {
      mixing.setName(*s);
    }

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::ScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setSchedule(s.get());
        }
      }
    }

    s = workspaceObject.getString(openstudio::ZoneMixingFields::DesignFlowRateCalculationMethod, true);
    OS_ASSERT(s);

    OptionalDouble d;
    if (istringEqual("Flow/Zone", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneMixingFields::DesignFlowRate);
      if (d) {
        mixing.setDesignFlowRate(*d);
      } else {
        LOG(Error, "Flow/Zone value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Flow/Area", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneMixingFields::FlowRateperFloorArea);
      if (d) {
        mixing.setFlowRateperFloorArea(*d);
      } else {
        LOG(Error, "Flow/Area value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Flow/Person", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneMixingFields::FlowRateperPerson);
      if (d) {
        mixing.setFlowRateperPerson(*d);
      } else {
        LOG(Error, "Flow/Person value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("AirChanges/Hour", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneMixingFields::AirChangesperHour);
      if (d) {
        mixing.setAirChangesperHour(*d);
      } else {
        LOG(Error, "AirChanges/Hour value not found for workspace object " << workspaceObject);
      }
    } else {
      LOG(Error, "Unknown DesignFlowRateCalculationMethod value for workspace object" << workspaceObject);
    }

    d = workspaceObject.getDouble(openstudio::ZoneMixingFields::DeltaTemperature);
    if (d) {
      mixing.setDeltaTemperature(*d);
    }

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::DeltaTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setDeltaTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MinimumReceivingTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMinimumReceivingTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MaximumReceivingTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMaximumReceivingTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MinimumSourceTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMinimumSourceTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MaximumSourceTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMaximumSourceTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MinimumOutdoorTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMinimumOutdoorTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MaximumOutdoorTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMaximumOutdoorTemperatureSchedule(s.get());
        }
      }
    }

    return mixing;
  }

}  // namespace energyplus

}  // namespace openstudio
