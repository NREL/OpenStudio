/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include <utilities/idd/DesignSpecification_OutdoorAir_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateDesignSpecificationOutdoorAir(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::DesignSpecification_OutdoorAir) {
      LOG(Error, "WorkspaceObject is not IddObjectType: DesignSpecification:OutdoorAir");
      return boost::none;
    }

    OptionalString outdoorAirMethod = workspaceObject.getString(DesignSpecification_OutdoorAirFields::OutdoorAirMethod, true);
    if (!outdoorAirMethod) {
      LOG(Error, "No OutdoorAirMethod specified for DesignSpecification:OutdoorAir named '" << workspaceObject.name().get() << "'");
      return boost::none;
    }

    DesignSpecificationOutdoorAir result(m_model);

    OptionalString name = workspaceObject.name();
    if (name) {
      result.setName(*name);
    }

    result.setOutdoorAirMethod(*outdoorAirMethod);

    boost::optional<double> flowPerPerson = workspaceObject.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperPerson);
    boost::optional<double> flowPerArea = workspaceObject.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZoneFloorArea);
    boost::optional<double> flowPerZone = workspaceObject.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowperZone);
    boost::optional<double> ach = workspaceObject.getDouble(DesignSpecification_OutdoorAirFields::OutdoorAirFlowAirChangesperHour);

    if (istringEqual(*outdoorAirMethod, "Flow/Person")) {
      if (flowPerPerson) {
        result.setOutdoorAirFlowperPerson(*flowPerPerson);
      }
    } else if (istringEqual(*outdoorAirMethod, "Flow/Area")) {
      if (flowPerArea) {
        result.setOutdoorAirFlowperFloorArea(*flowPerArea);
      }
    } else if (istringEqual(*outdoorAirMethod, "Flow/Zone")) {
      if (flowPerZone) {
        result.setOutdoorAirFlowRate(*flowPerZone);
      }
    } else if (istringEqual(*outdoorAirMethod, "AirChanges/Hour")) {
      if (ach) {
        result.setOutdoorAirFlowRate(*ach);
      }
    } else if (istringEqual(*outdoorAirMethod, "Sum") || istringEqual(*outdoorAirMethod, "Maximum")) {

      if (flowPerPerson) {
        result.setOutdoorAirFlowperPerson(*flowPerPerson);
      }
      if (flowPerArea) {
        result.setOutdoorAirFlowperFloorArea(*flowPerArea);
      }
      if (flowPerZone) {
        result.setOutdoorAirFlowRate(*flowPerZone);
      }
      if (ach) {
        result.setOutdoorAirFlowRate(*ach);
      }

    } else {
      LOG(Error, "Unknown OutdoorAirMethod '" << *outdoorAirMethod << "' specified for DesignSpecification:OutdoorAir named '"
                                              << workspaceObject.name().get() << "'");
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(DesignSpecification_OutdoorAirFields::OutdoorAirScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (OptionalSchedule intermediate = modelObject->optionalCast<Schedule>()) {
          Schedule schedule = *intermediate;
          result.setOutdoorAirFlowRateFractionSchedule(schedule);
        }
      }
    }

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
