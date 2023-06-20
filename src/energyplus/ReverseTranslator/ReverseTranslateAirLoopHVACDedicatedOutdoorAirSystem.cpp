/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/AirLoopHVACDedicatedOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACDedicatedOutdoorAirSystem_Impl.hpp"

#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/AirLoopHVAC_DedicatedOutdoorAirSystem_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateAirLoopHVACDedicatedOutdoorAirSystem(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::AirLoopHVAC_DedicatedOutdoorAirSystem) {
      LOG(Error, "WorkspaceObject is not IddObjectType: AirLoopHVACDedicatedOutdoorAirSystem");
      return boost::none;
    }

    OptionalString s;
    OptionalDouble d;
    OptionalWorkspaceObject target;

    boost::optional<AirLoopHVACOutdoorAirSystem> outdoorAirSystem;
    if ((target = workspaceObject.getTarget(openstudio::AirLoopHVAC_DedicatedOutdoorAirSystemFields::AirLoopHVAC_OutdoorAirSystemName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<AirLoopHVACOutdoorAirSystem>()) {
          outdoorAirSystem = modelObject->cast<AirLoopHVACOutdoorAirSystem>();
        }
      }
    } else {
      LOG(Error, "Could not find outdoor air system attached to AirLoopHVACDedicatedOutdoorAirSystem object");
      return boost::none;
    }

    openstudio::model::AirLoopHVACDedicatedOutdoorAirSystem doas(*outdoorAirSystem);

    s = workspaceObject.name();
    if (s) {
      doas.setName(*s);
    }

    if ((target = workspaceObject.getTarget(AirLoopHVAC_DedicatedOutdoorAirSystemFields::AvailabilityScheduleName))) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto optSch = modelObject->optionalCast<Schedule>()) {
          doas.setAvailabilitySchedule(optSch.get());
        }
      }
    }

    d = workspaceObject.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignTemperature);
    if (d) {
      doas.setPreheatDesignTemperature(*d);
    }

    d = workspaceObject.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignHumidityRatio);
    if (d) {
      doas.setPreheatDesignTemperature(*d);
    }

    d = workspaceObject.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PrecoolDesignTemperature);
    if (d) {
      doas.setPrecoolDesignTemperature(*d);
    }

    d = workspaceObject.getDouble(AirLoopHVAC_DedicatedOutdoorAirSystemFields::PreheatDesignHumidityRatio);
    if (d) {
      doas.setPrecoolDesignHumidityRatio(*d);
    }

    for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()) {
      auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
      OptionalWorkspaceObject target = workspaceGroup.getTarget(AirLoopHVAC_DedicatedOutdoorAirSystemExtensibleFields::AirLoopHVACName);
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);

      // add the air loop
      doas.addAirLoop(modelObject->cast<AirLoopHVAC>());
    }

    return doas;
  }

}  // namespace energyplus

}  // namespace openstudio
