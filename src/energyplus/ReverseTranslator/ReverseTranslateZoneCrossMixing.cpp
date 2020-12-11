/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ZoneMixing.hpp"
#include "../../model/ZoneMixing_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
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

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::ZoneName);
    OptionalThermalZone zone;
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        zone = modelObject->optionalCast<ThermalZone>();
      }
    }

    if (!zone) {
      return boost::none;
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::SourceZoneName);
    OptionalThermalZone sourceZone;
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        sourceZone = modelObject->optionalCast<ThermalZone>();
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
      d = workspaceObject.getDouble(openstudio::ZoneCrossMixingFields::FlowRateperZoneFloorArea);
      if (d) {
        mixing.setFlowRateperZoneFloorArea(*d);
        reverseMixing.setFlowRateperZoneFloorArea(*d);
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

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MinimumZoneTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMinimumZoneTemperatureSchedule(s.get());
          reverseMixing.setMinimumZoneTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MaximumZoneTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMaximumZoneTemperatureSchedule(s.get());
          reverseMixing.setMaximumZoneTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MinimumSourceZoneTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMinimumSourceZoneTemperatureSchedule(s.get());
          reverseMixing.setMinimumSourceZoneTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneCrossMixingFields::MaximumSourceZoneTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMaximumSourceZoneTemperatureSchedule(s.get());
          reverseMixing.setMaximumSourceZoneTemperatureSchedule(s.get());
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
