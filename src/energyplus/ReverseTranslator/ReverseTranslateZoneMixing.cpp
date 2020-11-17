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

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::ZoneMixingFields::ZoneName);
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

    openstudio::model::ZoneMixing mixing(*zone);

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
      d = workspaceObject.getDouble(openstudio::ZoneMixingFields::FlowRateperZoneFloorArea);
      if (d) {
        mixing.setFlowRateperZoneFloorArea(*d);
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

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::SourceZoneName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<ThermalZone>()) {
          mixing.setSourceZone(modelObject->cast<ThermalZone>());
        }
      }
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

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MinimumZoneTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMinimumZoneTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MaximumZoneTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMaximumZoneTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MinimumSourceZoneTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMinimumSourceZoneTemperatureSchedule(s.get());
        }
      }
    }

    target = workspaceObject.getTarget(openstudio::ZoneMixingFields::MaximumSourceZoneTemperatureScheduleName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (auto s = modelObject->optionalCast<Schedule>()) {
          mixing.setMaximumSourceZoneTemperatureSchedule(s.get());
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
