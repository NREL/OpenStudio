/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

OptionalModelObject ReverseTranslator::translateZoneHVACIdealLoadsAirSystem( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::ZoneHVAC_IdealLoadsAirSystem ) {
    LOG(Error, "WorkspaceObject is not IddObjectType: ZoneHVAC:IdealLoadsAirSystem");
     return boost::none;
  }

  ZoneHVACIdealLoadsAirSystem zoneHVACIdealLoadsAirSystem(m_model);
  
  // name
  boost::optional<std::string> s = workspaceObject.name();
  if(s){
    zoneHVACIdealLoadsAirSystem.setName(*s);
  }
  
  // availability schedule
  OptionalWorkspaceObject target = workspaceObject.getTarget(ZoneHVAC_IdealLoadsAirSystemFields::AvailabilityScheduleName);
  if (target){
    OptionalModelObject availabilitySchedule = translateAndMapWorkspaceObject(*target);
    if (availabilitySchedule){
      zoneHVACIdealLoadsAirSystem.setPointer(OS_ZoneHVAC_IdealLoadsAirSystemFields::AvailabilityScheduleName, availabilitySchedule->handle());
    }
  }

  // skip inlet and outlet node names - that should be done by the zone HVAC object translator

  // maximum heating supply air temperature
  OptionalDouble d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirTemperature);
  if (d){
    zoneHVACIdealLoadsAirSystem.setMaximumHeatingSupplyAirTemperature(*d);
  }

  // minimum cooling supply air temperature
  d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirTemperature);
  if (d){
    zoneHVACIdealLoadsAirSystem.setMinimumCoolingSupplyAirTemperature(*d);
  }

  // maximum heating supply air humidity ratio
  d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirHumidityRatio);
  if (d){
    zoneHVACIdealLoadsAirSystem.setMaximumHeatingSupplyAirHumidityRatio(*d);
  }

  // minimum cooling supply air humidity ratio
  d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirHumidityRatio);
  if (d){
    zoneHVACIdealLoadsAirSystem.setMinimumCoolingSupplyAirHumidityRatio(*d);
  }

  // heating limit
  s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::HeatingLimit);
  if (s){
    zoneHVACIdealLoadsAirSystem.setHeatingLimit(*s);
  }

  // maximum heating air flow rate
  d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate);
  if (d){
    zoneHVACIdealLoadsAirSystem.setMaximumHeatingAirFlowRate(*d);
  }

  // maximum sensible heating capacity
  d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity);
  if (d){
    zoneHVACIdealLoadsAirSystem.setMaximumSensibleHeatingCapacity(*d);
  }

  // cooling limit
  s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::CoolingLimit);
  if (s){
    zoneHVACIdealLoadsAirSystem.setCoolingLimit(*s);
  }

  // maximum cooling airflow rate
  d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate);
  if (d){
    zoneHVACIdealLoadsAirSystem.setMaximumCoolingAirFlowRate(*d);
  }

  // maximum total cooling capacity
  d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity);
  if (d){
    zoneHVACIdealLoadsAirSystem.setMaximumTotalCoolingCapacity(*d);
  }

  // heating availability schedule
  target = workspaceObject.getTarget(ZoneHVAC_IdealLoadsAirSystemFields::HeatingAvailabilityScheduleName);
  if (target){
    OptionalModelObject heatingAvailabilitySchedule = translateAndMapWorkspaceObject(*target);
    if (heatingAvailabilitySchedule){
      zoneHVACIdealLoadsAirSystem.setPointer(OS_ZoneHVAC_IdealLoadsAirSystemFields::HeatingAvailabilityScheduleName, heatingAvailabilitySchedule->handle());
    }
  }

  // cooling availability schedule
  target = workspaceObject.getTarget(ZoneHVAC_IdealLoadsAirSystemFields::CoolingAvailabilityScheduleName);
  if (target){
    OptionalModelObject coolingAvailabilitySchedule = translateAndMapWorkspaceObject(*target);
    if (coolingAvailabilitySchedule){
      zoneHVACIdealLoadsAirSystem.setPointer(OS_ZoneHVAC_IdealLoadsAirSystemFields::CoolingAvailabilityScheduleName, coolingAvailabilitySchedule->handle());
    }
  }

  // dehumidification control type
  s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::DehumidificationControlType);
  if (s){
    zoneHVACIdealLoadsAirSystem.setDehumidificationControlType(*s);
  }

  // cooling sensible heat ratio
  d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::CoolingSensibleHeatRatio);
  if (d){
    zoneHVACIdealLoadsAirSystem.setCoolingSensibleHeatRatio(*d);
  }

  // humidification control type
  s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::HumidificationControlType);
  if (s){
    zoneHVACIdealLoadsAirSystem.setHumidificationControlType(*s);
  }

  // design specification outdoor air object name
  target = workspaceObject.getTarget(ZoneHVAC_IdealLoadsAirSystemFields::DesignSpecificationOutdoorAirObjectName);
  if (target){
    OptionalModelObject designSpecificationOutdoorAir = translateAndMapWorkspaceObject(*target);
    if (designSpecificationOutdoorAir){
      zoneHVACIdealLoadsAirSystem.setPointer(OS_ZoneHVAC_IdealLoadsAirSystemFields::DesignSpecificationOutdoorAirObjectName, designSpecificationOutdoorAir->handle());
    }
  }

  // demand controlled ventilation type
  s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::DemandControlledVentilationType);
  if (s){
    zoneHVACIdealLoadsAirSystem.setDemandControlledVentilationType(*s);
  }

  // outdoor air economizer type
  s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::OutdoorAirEconomizerType);
  if (s){
    zoneHVACIdealLoadsAirSystem.setOutdoorAirEconomizerType(*s);
  }

  // heat recovery type
  s = workspaceObject.getString(ZoneHVAC_IdealLoadsAirSystemFields::HeatRecoveryType);
  if (s){
    zoneHVACIdealLoadsAirSystem.setHeatRecoveryType(*s);
  }

  // sensible heat recovery effectiveness
  d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::SensibleHeatRecoveryEffectiveness);
  if (d){
    zoneHVACIdealLoadsAirSystem.setSensibleHeatRecoveryEffectiveness(*d);
  }

  // latent heat recovery effectiveness
  d = workspaceObject.getDouble(ZoneHVAC_IdealLoadsAirSystemFields::LatentHeatRecoveryEffectiveness);
  if (d){
    zoneHVACIdealLoadsAirSystem.setLatentHeatRecoveryEffectiveness(*d);
  }

  return zoneHVACIdealLoadsAirSystem;
}

} // energyplus

} // openstudio

