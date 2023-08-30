/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SpaceInfiltrationDesignFlowRate.hpp"
#include "../../model/SpaceInfiltrationDesignFlowRate_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ZoneInfiltration_DesignFlowRate_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneInfiltrationDesignFlowRate(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneInfiltration_DesignFlowRate) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZoneInfiltration:DesignFlowRate");
      return boost::none;
    }

    openstudio::model::SpaceInfiltrationDesignFlowRate infiltration(m_model);

    OptionalString s = workspaceObject.name();
    if (s) {
      infiltration.setName(*s);
    }

    OptionalWorkspaceObject target =
      workspaceObject.getTarget(openstudio::ZoneInfiltration_DesignFlowRateFields::ZoneorZoneListorSpaceorSpaceListName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<Space>()) {
          infiltration.setSpace(modelObject->cast<Space>());
        } else if (modelObject->optionalCast<SpaceType>()) {
          infiltration.setSpaceType(modelObject->cast<SpaceType>());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneInfiltration_DesignFlowRateFields::ScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule(*intermediate);
          infiltration.setSchedule(schedule);
        }
      }
    }

    s = workspaceObject.getString(openstudio::ZoneInfiltration_DesignFlowRateFields::DesignFlowRateCalculationMethod, true);
    OS_ASSERT(s);

    OptionalDouble d;
    if (istringEqual("Flow/Zone", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::DesignFlowRate);
      if (d) {
        infiltration.setDesignFlowRate(*d);
      } else {
        LOG(Error, "Flow/Zone value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Flow/Area", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::FlowRateperFloorArea);
      if (d) {
        infiltration.setFlowperSpaceFloorArea(*d);
      } else {
        LOG(Error, "Flow/Area value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Flow/ExteriorArea", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::FlowRateperExteriorSurfaceArea);
      if (d) {
        infiltration.setFlowperExteriorSurfaceArea(*d);
      } else {
        LOG(Error, "Flow/ExteriorArea value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("Flow/ExteriorWallArea", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::FlowRateperExteriorSurfaceArea);
      if (d) {
        infiltration.setFlowperExteriorWallArea(*d);
      } else {
        LOG(Error, "Flow/ExteriorWallArea value not found for workspace object " << workspaceObject);
      }
    } else if (istringEqual("AirChanges/Hour", *s)) {
      d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::AirChangesperHour);
      if (d) {
        infiltration.setAirChangesperHour(*d);
      } else {
        LOG(Error, "AirChanges/Hour value not found for workspace object " << workspaceObject);
      }
    } else {
      LOG(Error, "Unknown DesignLevelCalculationMethod value for workspace object" << workspaceObject);
    }

    d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::ConstantTermCoefficient);
    if (d) {
      infiltration.setConstantTermCoefficient(*d);
    }

    d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::TemperatureTermCoefficient);
    if (d) {
      infiltration.setTemperatureTermCoefficient(*d);
    }

    d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::VelocityTermCoefficient);
    if (d) {
      infiltration.setVelocityTermCoefficient(*d);
    }

    d = workspaceObject.getDouble(openstudio::ZoneInfiltration_DesignFlowRateFields::VelocitySquaredTermCoefficient);
    if (d) {
      infiltration.setVelocitySquaredTermCoefficient(*d);
    }

    return infiltration;
  }

}  // namespace energyplus

}  // namespace openstudio
