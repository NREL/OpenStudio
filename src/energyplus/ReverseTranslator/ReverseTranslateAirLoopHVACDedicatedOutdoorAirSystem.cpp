/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
