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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ZoneHVACIdealLoadsAirSystem.hpp"
#include "../../model/ZoneHVACIdealLoadsAirSystem_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"

#include <utilities/idd/ZoneHVAC_IdealLoadsAirSystem_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACIdealLoadsAirSystem(ZoneHVACIdealLoadsAirSystem & modelObject)
{
  
  IdfObject zoneHVACIdealLoadsAirSystem = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneHVAC_IdealLoadsAirSystem,modelObject);
  
  // availability schedule name
  boost::optional<Schedule> schedule = modelObject.availabilitySchedule();
  if(schedule){
    boost::optional<IdfObject> idfSchedule = translateAndMapModelObject(*schedule);
    if(idfSchedule){
      zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::AvailabilityScheduleName,idfSchedule->name().get());
    }
  }

  // zone supply air node name
  boost::optional<std::string> zoneSupplyAirNodeName;
  if(boost::optional<Node> node = modelObject.outletNode()){
    if(boost::optional<std::string> s = node->name()){
      zoneSupplyAirNodeName = s;
      zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::ZoneSupplyAirNodeName,s.get());
    }
  }

  // zone exhaust air node name
  boost::optional<std::string> zoneExhaustAirNodeName;
  if(boost::optional<Node> node = modelObject.inletNode()){
    if(boost::optional<std::string> s = node->name()){
      zoneExhaustAirNodeName = s;
      zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::ZoneExhaustAirNodeName,s.get());
    }
  }

  // maximum heating supply air temperature
  if(! modelObject.isMaximumHeatingSupplyAirTemperatureDefaulted()){
    zoneHVACIdealLoadsAirSystem.setDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirTemperature,modelObject.maximumHeatingSupplyAirTemperature());
  }

  // minimum cooling supply air temperature
  if (! modelObject.isMinimumCoolingSupplyAirTemperatureDefaulted()){
    zoneHVACIdealLoadsAirSystem.setDouble(ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirTemperature,modelObject.minimumCoolingSupplyAirTemperature());
  }

  // maximum heating supply air humidity ratio
  if (! modelObject.isMaximumHeatingSupplyAirHumidityRatioDefaulted()){
    zoneHVACIdealLoadsAirSystem.setDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingSupplyAirHumidityRatio,modelObject.maximumHeatingSupplyAirHumidityRatio());
  }

  // minimum cooling supply air humidity ratio
  if (! modelObject.isMinimumCoolingSupplyAirHumidityRatioDefaulted()){
    zoneHVACIdealLoadsAirSystem.setDouble(ZoneHVAC_IdealLoadsAirSystemFields::MinimumCoolingSupplyAirHumidityRatio,modelObject.minimumCoolingSupplyAirHumidityRatio());
  }

  // heating limit
  if (! modelObject.isHeatingLimitDefaulted()){
    zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::HeatingLimit,modelObject.heatingLimit());
  }

  // maximum heating air flow rate
  if(modelObject.isMaximumHeatingAirFlowRateAutosized()){
    zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate,"Autosize");
  }else if(boost::optional<double> d = modelObject.maximumHeatingAirFlowRate()){
    zoneHVACIdealLoadsAirSystem.setDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumHeatingAirFlowRate,d.get());
  }

  // maximum sensible heating capacity
  if(modelObject.isMaximumSensibleHeatingCapacityAutosized()){
    zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity,"Autosize");
  }else if(boost::optional<double> d = modelObject.maximumSensibleHeatingCapacity()){
    zoneHVACIdealLoadsAirSystem.setDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumSensibleHeatingCapacity,d.get());
  }

  // cooling limit
  if (! modelObject.isCoolingLimitDefaulted()){
    zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::CoolingLimit,modelObject.coolingLimit());
  }

  // maximum cooling air flow rate
  if(modelObject.isMaximumCoolingAirFlowRateAutosized()){
    zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate,"Autosize");
  }else if(boost::optional<double> d = modelObject.maximumCoolingAirFlowRate()){
    zoneHVACIdealLoadsAirSystem.setDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumCoolingAirFlowRate,d.get());
  }

  // maximum total cooling capacity
  if(modelObject.isMaximumTotalCoolingCapacityAutosized()){
    zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity,"Autosize");
  }else if(boost::optional<double> d = modelObject.maximumTotalCoolingCapacity()){
    zoneHVACIdealLoadsAirSystem.setDouble(ZoneHVAC_IdealLoadsAirSystemFields::MaximumTotalCoolingCapacity,d.get());
  }

  // heating availability schedule name
  if(boost::optional<Schedule> schedule = modelObject.heatingAvailabilitySchedule()){
    if(boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())){
      zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::HeatingAvailabilityScheduleName,_schedule->name().get());
    }
  }

 // cooling availability schedule name
  if(boost::optional<Schedule> schedule = modelObject.coolingAvailabilitySchedule()){
    if(boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())){
      zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::CoolingAvailabilityScheduleName,_schedule->name().get());
    }
  }

  // dehumidification control type
  if (! modelObject.isDehumidificationControlTypeDefaulted()){
    zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::DehumidificationControlType,modelObject.dehumidificationControlType());
  }

  // cooling sensible heat ratio
  if (! modelObject.isCoolingSensibleHeatRatioDefaulted()){
    zoneHVACIdealLoadsAirSystem.setDouble(ZoneHVAC_IdealLoadsAirSystemFields::CoolingSensibleHeatRatio,modelObject.coolingSensibleHeatRatio());
  }

  // humidification control type
  if (! modelObject.isHumidificationControlTypeDefaulted()){
    zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::HumidificationControlType,modelObject.humidificationControlType());
  }

  // forward translate design specification outdoor air object name
  // get the zone that this piece of equipment is connected to
  boost::optional<ThermalZone> zone = modelObject.thermalZone();
  if (zone) {
    // get this zone's space
    std::vector<Space> spaces = zone->spaces();
    // get the space's design specification outdoor air, if one exists
    if ( ! spaces.empty() ){
      boost::optional<DesignSpecificationOutdoorAir> designSpecificationOutdoorAir;
      designSpecificationOutdoorAir = spaces[0].designSpecificationOutdoorAir();
      if (designSpecificationOutdoorAir){
        // translate the design specification outdoor air to idf      
        boost::optional<IdfObject> designSpecificationOutdoorAirIdf;
        designSpecificationOutdoorAirIdf = translateAndMapModelObject(*designSpecificationOutdoorAir);
        // the translation should complete successfully
        OS_ASSERT(designSpecificationOutdoorAirIdf);      
        // set the field to reference the design specification outdoor air
        zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::DesignSpecificationOutdoorAirObjectName,designSpecificationOutdoorAirIdf->name().get()); 
      }
    }
  }

  // demand controlled ventilation type
  if (! modelObject.isDemandControlledVentilationTypeDefaulted()){
    zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::DemandControlledVentilationType,modelObject.demandControlledVentilationType());
  }

  // outdoor air economizer type
  if (! modelObject.isOutdoorAirEconomizerTypeDefaulted()){
    zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::OutdoorAirEconomizerType,modelObject.outdoorAirEconomizerType());
  }

  // heat recovery type
  if (! modelObject.isHeatRecoveryTypeDefaulted()){
    zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::HeatRecoveryType,modelObject.heatRecoveryType());
  }

  // sensible heat recovery effectiveness
  if (! modelObject.isSensibleHeatRecoveryEffectivenessDefaulted()){
    zoneHVACIdealLoadsAirSystem.setDouble(ZoneHVAC_IdealLoadsAirSystemFields::SensibleHeatRecoveryEffectiveness,modelObject.sensibleHeatRecoveryEffectiveness());
  }

  // latent heat recovery effectiveness
  if (! modelObject.isLatentHeatRecoveryEffectivenessDefaulted()){
    zoneHVACIdealLoadsAirSystem.setDouble(ZoneHVAC_IdealLoadsAirSystemFields::LatentHeatRecoveryEffectiveness,modelObject.latentHeatRecoveryEffectiveness());
  }

  return zoneHVACIdealLoadsAirSystem;
}

} // energyplus

} // openstudio

