/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/ZoneHVACWaterToAirHeatPump.hpp>
#include <model/ZoneHVACWaterToAirHeatPump_Impl.hpp>
#include <model/FanOnOff.hpp>
#include <model/FanOnOff_Impl.hpp>
#include <model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp>
#include <model/CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp>
#include <model/CoilHeatingWaterToAirHeatPumpEquationFit.hpp>
#include <model/CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp>
#include <model/CoilHeatingElectric.hpp>
#include <model/CoilHeatingElectric_Impl.hpp>
#include <model/CoilHeatingGas.hpp>
#include <model/CoilHeatingGas_Impl.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/CoilHeatingWater_Impl.hpp>
#include <utilities/idd/ZoneHVAC_WaterToAirHeatPump_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACWaterToAirHeatPump( 
    ZoneHVACWaterToAirHeatPump& modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::ZoneHVAC_WaterToAirHeatPump);

  m_idfObjects.push_back(idfObject);

  // Name
  std::string baseName = modelObject.name().get();

  idfObject.setName(baseName);

  std::string mixedAirNodeName = baseName + " Mixed Air Node";

  std::string coolingCoilOutletNodeName = baseName + " Cooling Coil Outlet Node";

  std::string heatingCoilOutletNodeName = baseName + " Heating Coil Outlet Node";

  std::string fanOutletNodeName = baseName + " Fan Outlet Node";

  std::string reliefAirNodeName = baseName + " Relief Air Node";

  std::string oaNodeName = baseName + " OA Node";

  // AvailabilityScheduleName
  if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // AirInletNodeName
  boost::optional<std::string> airInletNodeName;

  if( boost::optional<Node> node = modelObject.inletNode() )
  {
    if( (s = node->name()) )
    {
      airInletNodeName = s;

      idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::AirInletNodeName,s.get());
    }
  }

  // AirOutletNodeName
  boost::optional<std::string> airOutletNodeName;

  if( boost::optional<Node> node = modelObject.outletNode() )
  {
    if( (s = node->name()) )
    {
      airOutletNodeName = s;

      idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::AirOutletNodeName,s.get());
    }
  }

  // Outdoor Air Mixer

  std::string oaMixerName = baseName + " OA Mixer";  

  IdfObject _outdoorAirMixer(IddObjectType::OutdoorAir_Mixer);
  _outdoorAirMixer.setName(oaMixerName);
  m_idfObjects.push_back(_outdoorAirMixer);

  _outdoorAirMixer.setString(OutdoorAir_MixerFields::MixedAirNodeName,mixedAirNodeName);

  _outdoorAirMixer.setString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName,oaNodeName);

  IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
  _oaNodeList.setString(0,oaNodeName);
  m_idfObjects.push_back(_oaNodeList);

  _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReliefAirStreamNodeName,reliefAirNodeName);

  if(airInletNodeName)
  {
    _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReturnAirStreamNodeName,airInletNodeName.get());
  }

  // OutdoorAirMixerObjectType
  idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirMixerObjectType,
                      _outdoorAirMixer.iddObject().name());

  // OutdoorAirMixerName
  idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirMixerName,oaMixerName);

  // SupplyAirFlowRateDuringCoolingOperation
  if( modelObject.isSupplyAirFlowRateDuringCoolingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringCoolingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation,value.get());
  }

  // SupplyAirFlowRateDuringHeatingOperation
  if( modelObject.isSupplyAirFlowRateDuringHeatingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringHeatingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation,value.get());
  }

  // SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded
  if( modelObject.isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() )
  {
    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,"Autosize");
  } 
  else if( (value = modelObject.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,value.get());
  }

  // OutdoorAirFlowRateDuringCoolingOperation
  if( modelObject.isOutdoorAirFlowRateDuringCoolingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateDuringCoolingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation,value.get());
  }

  // OutdoorAirFlowRateDuringHeatingOperation
  if( modelObject.isOutdoorAirFlowRateDuringHeatingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateDuringHeatingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation,value.get());
  }

  // OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded  
  if( modelObject.isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() )
  {
    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,value.get());
  }

  // SupplyAirFanObjectType and Name
  HVACComponent supplyAirFan = modelObject.supplyAirFan();

  if( boost::optional<IdfObject> _supplyAirFan = translateAndMapModelObject(supplyAirFan) )
  {
    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanObjectType,_supplyAirFan->iddObject().name() );

    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanName,_supplyAirFan->name().get());

    if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_OnOff )
    {
      _supplyAirFan->setString(Fan_OnOffFields::AirInletNodeName,mixedAirNodeName);

      _supplyAirFan->setString(Fan_OnOffFields::AirOutletNodeName,fanOutletNodeName);
    }
  }

  // HeatingCoilObjectType and Name  
  HVACComponent heatingCoil = modelObject.heatingCoil();

  if( boost::optional<IdfObject> _heatingCoil = translateAndMapModelObject(heatingCoil) )
  {
    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilObjectType,_heatingCoil->iddObject().name() );

    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::HeatingCoilName,_heatingCoil->name().get() ); 

    if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit )
    {
      _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName,coolingCoilOutletNodeName);

      _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName,heatingCoilOutletNodeName);
    }
  }

  // CoolingCoilObjectType and Name
  HVACComponent coolingCoil = modelObject.coolingCoil();

  if( boost::optional<IdfObject> _coolingCoil = translateAndMapModelObject(coolingCoil) )
  {
    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::CoolingCoilObjectType,_coolingCoil->iddObject().name() );

    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::CoolingCoilName,_coolingCoil->name().get() );

    if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit)
    {
      _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName,fanOutletNodeName);

      _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName,coolingCoilOutletNodeName);
    }
  }

  //Maximum Cycling Rate
  if( (value = modelObject.maximumCyclingRate()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::MaximumCyclingRate,value.get());
  }

  //Heat Pump Time Constant
  if( (value = modelObject.heatPumpTimeConstant()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpTimeConstant,value.get());
  }

  //Fraction of On-Cycle Power Use
  if( (value = modelObject.fractionofOnCyclePowerUse()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::FractionofOnCyclePowerUse,value.get());
  }

  //Heat Pump Fan Delay Time
  if( (value = modelObject.heatPumpFanDelayTime()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::HeatPumpFanDelayTime,value.get());
  }

  // SupplementalHeatingCoilObjectType and SupplementalHeatingCoilName
  HVACComponent supplementalHeatingCoil = modelObject.supplementalHeatingCoil();

  if( boost::optional<IdfObject> _supplementalHeatingCoil = translateAndMapModelObject(supplementalHeatingCoil) )
  {
    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::SupplementalHeatingCoilObjectType,_supplementalHeatingCoil->iddObject().name() );

    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::SupplementalHeatingCoilName,_supplementalHeatingCoil->name().get() );

    if( airOutletNodeName )
    {
      if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_GasFields::AirInletNodeName,heatingCoilOutletNodeName);

        _supplementalHeatingCoil->setString(Coil_Heating_GasFields::AirOutletNodeName,airOutletNodeName.get());
      }
      else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,heatingCoilOutletNodeName);

        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,airOutletNodeName.get());
      }
      else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,heatingCoilOutletNodeName);

        _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,airOutletNodeName.get());
      }
    }
  }

  // MaximumSupplyAirTemperaturefromSupplementalHeater
  if( modelObject.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() )
  {
    idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater,"Autosize"); 
  }
  else if( (value = modelObject.maximumSupplyAirTemperaturefromSupplementalHeater()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater,value.get());
  }

  // MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation
  if( (value = modelObject.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_WaterToAirHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,value.get());
  }

  //Outdoor Dry-bulb Temperature Sensor Node Name
  idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::OutdoorDryBulbTemperatureSensorNodeName,"");

  // FanPlacement
  idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::FanPlacement,modelObject.fanPlacement());

  // SupplyAirFanOperatingModeScheduleName
  if( boost::optional<Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_WaterToAirHeatPumpFields::SupplyAirFanOperatingModeScheduleName,_schedule->name().get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

