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

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACIdealLoadsAirSystem.hpp"
#include "../../model/ZoneHVACIdealLoadsAirSystem_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/ZoneHVAC_IdealLoadsAirSystem_FieldEnums.hxx>
#include <utilities/idd/OS_ZoneHVAC_IdealLoadsAirSystem_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneHVACIdealLoadsAirSystem(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneHVAC_IdealLoadsAirSystem) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZoneHVAC:IdealLoadsAirSystem");
      return boost::none;
    }

    ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(m_model);

    // name
    boost::optional<std::string> s = workspaceObject.name();
    if (s) {
      zoneHVACIdealLoadsAirSystem.setName(*s);
    }

    // availability schedule
    OptionalWorkspaceObject target = workspaceObject.getTarget(ZoneHVAC_IdealLoadsAirSystemFields::AvailabilityScheduleName);
    if (target) {
      OptionalModelObject availabilitySchedule = translateAndMapWorkspaceObject(*target);
      if (availabilitySchedule) {
        zoneHVACIdealLoadsAirSystem.setPointer(OS_ZoneHVAC_IdealLoadsAirSystemFields::AvailabilityScheduleName, availabilitySchedule->handle());
      }
    }

    // skip inlet and outlet node names - that should be done by the zone HVAC object translator

    // maximum heating supply air temperature
    OptionalDouble d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirTemperature);
    if (d) {
      zoneHVACIdealLoadsAirSystem.setMaximumHeatingSupplyAirTemperature(*d);
    }

    // minimum cooling supply air temperature
    d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirTemperature);
    if (d) {
      zoneHVACIdealLoadsAirSystem.setMinimumCoolingSupplyAirTemperature(*d);
    }

    // maximum heating supply air humidity ratio
    d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirHumidityRatio);
    if (d) {
      zoneHVACIdealLoadsAirSystem.setMaximumHeatingSupplyAirHumidityRatio(*d);
    }

    // minimum cooling supply air humidity ratio
    d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirHumidityRatio);
    if (d) {
      zoneHVACIdealLoadsAirSystem.setMinimumCoolingSupplyAirHumidityRatio(*d);
    }

    // heating limit
    s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::HeatingLimit);
    if (s) {
      zoneHVACIdealLoadsAirSystem.setHeatingLimit(*s);
    }

    // maximum heating air flow rate
    d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate);
    if (d) {
      zoneHVACIdealLoadsAirSystem.setMaximumHeatingAirFlowRate(*d);
    }

    // maximum sensible heating capacity
    d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity);
    if (d) {
      zoneHVACIdealLoadsAirSystem.setMaximumSensibleHeatingCapacity(*d);
    }

    // cooling limit
    s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::CoolingLimit);
    if (s) {
      zoneHVACIdealLoadsAirSystem.setCoolingLimit(*s);
    }

    // maximum cooling airflow rate
    d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate);
    if (d) {
      zoneHVACIdealLoadsAirSystem.setMaximumCoolingAirFlowRate(*d);
    }

    // maximum total cooling capacity
    d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity);
    if (d) {
      zoneHVACIdealLoadsAirSystem.setMaximumTotalCoolingCapacity(*d);
    }

    // heating availability schedule
    target = workspaceObject.getTarget(ZoneHVAC_IdealLoadsAirSystemFields::HeatingAvailabilityScheduleName);
    if (target) {
      OptionalModelObject heatingAvailabilitySchedule = translateAndMapWorkspaceObject(*target);
      if (heatingAvailabilitySchedule) {
        zoneHVACIdealLoadsAirSystem.setPointer(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatingAvailabilityScheduleName,
                                               heatingAvailabilitySchedule->handle());
      }
    }

    // cooling availability schedule
    target = workspaceObject.getTarget(ZoneHVAC_IdealLoadsAirSystemFields::CoolingAvailabilityScheduleName);
    if (target) {
      OptionalModelObject coolingAvailabilitySchedule = translateAndMapWorkspaceObject(*target);
      if (coolingAvailabilitySchedule) {
        zoneHVACIdealLoadsAirSystem.setPointer(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingAvailabilityScheduleName,
                                               coolingAvailabilitySchedule->handle());
      }
    }

    // dehumidification control type
    s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::DehumidificationControlType);
    if (s) {
      zoneHVACIdealLoadsAirSystem.setDehumidificationControlType(*s);
    }

    // cooling sensible heat ratio
    d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::CoolingSensibleHeatRatio);
    if (d) {
      zoneHVACIdealLoadsAirSystem.setCoolingSensibleHeatRatio(*d);
    }

    // humidification control type
    s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::HumidificationControlType);
    if (s) {
      zoneHVACIdealLoadsAirSystem.setHumidificationControlType(*s);
    }

    // design specification outdoor air object name
    target = workspaceObject.getTarget(ZoneHVAC_IdealLoadsAirSystemFields::DesignSpecificationOutdoorAirObjectName);
    if (target) {
      OptionalModelObject designSpecificationOutdoorAir = translateAndMapWorkspaceObject(*target);
      if (designSpecificationOutdoorAir) {
        zoneHVACIdealLoadsAirSystem.setPointer(OS_ZoneHVAC_IdealLoadsAirSystemFields::DesignSpecificationOutdoorAirObjectName,
                                               designSpecificationOutdoorAir->handle());
      }
    }

    // demand controlled ventilation type
    s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::DemandControlledVentilationType);
    if (s) {
      zoneHVACIdealLoadsAirSystem.setDemandControlledVentilationType(*s);
    }

    // outdoor air economizer type
    s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::OutdoorAirEconomizerType);
    if (s) {
      zoneHVACIdealLoadsAirSystem.setOutdoorAirEconomizerType(*s);
    }

    // heat recovery type
    s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::HeatRecoveryType);
    if (s) {
      zoneHVACIdealLoadsAirSystem.setHeatRecoveryType(*s);
    }

    // sensible heat recovery effectiveness
    d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::SensibleHeatRecoveryEffectiveness);
    if (d) {
      zoneHVACIdealLoadsAirSystem.setSensibleHeatRecoveryEffectiveness(*d);
    }

    // latent heat recovery effectiveness
    d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::LatentHeatRecoveryEffectiveness);
    if (d) {
      zoneHVACIdealLoadsAirSystem.setLatentHeatRecoveryEffectiveness(*d);
    }

    return zoneHVACIdealLoadsAirSystem;
  }

}  // namespace energyplus

}  // namespace openstudio
