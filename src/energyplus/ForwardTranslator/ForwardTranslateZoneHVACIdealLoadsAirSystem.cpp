/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/PortList.hpp"
#include "../../model/PortList_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../../model/AirLoopHVACReturnPlenum.hpp"
#include "../../model/AirLoopHVACReturnPlenum_Impl.hpp"

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

  std::vector<model::ZoneHVACIdealLoadsAirSystem> allIdealSystems;
  auto returnPlenum = modelObject.returnPlenum();
  if ( returnPlenum ) {
    allIdealSystems = returnPlenum->getImpl<model::detail::AirLoopHVACReturnPlenum_Impl>()->zoneHVACIdealLoadsAirSystems();
  }

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
  if ( allIdealSystems.empty() ) {
    boost::optional<std::string> zoneExhaustAirNodeName;

    auto plenum = modelObject.returnPlenum();
    if ( plenum ) {
      auto zone = modelObject.thermalZone();
      OS_ASSERT( zone );
      auto h = zone->handle();

      boost::optional<Node> plenumInletNode;

      auto plenumInlets = plenum->inletModelObjects();
      for ( auto & plenumInlet : plenumInlets ) {
        auto node = plenumInlet.optionalCast<Node>();
        if ( node ) {
          auto pl = node->inletModelObject();
          if ( pl && pl->optionalCast<PortList>() ) {
            auto mo = pl->cast<PortList>().getImpl<model::detail::PortList_Impl>()->hvacComponent();
            if ( mo.handle() == h ) {
              zoneExhaustAirNodeName = node->nameString();
              break;
            }
          }
        }
      }
    }

    if ( zoneExhaustAirNodeName ) {
      zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::ZoneExhaustAirNodeName,zoneExhaustAirNodeName.get());
    }
  }

  if ( allIdealSystems.empty() ) {
    // System Inlet Air Node Name
    if(boost::optional<Node> node = modelObject.inletNode()){
      if(boost::optional<std::string> s = node->name()){
        zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::ZoneExhaustAirNodeName,s.get());
      }
    }
  } else {
    auto front = allIdealSystems.front();
    if ( front.handle() == modelObject.handle() ) {
      auto node = front.inletNode();
      OS_ASSERT( node );
      zoneHVACIdealLoadsAirSystem.setString(ZoneHVAC_IdealLoadsAirSystemFields::SystemInletAirNodeName,node->nameString());
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

