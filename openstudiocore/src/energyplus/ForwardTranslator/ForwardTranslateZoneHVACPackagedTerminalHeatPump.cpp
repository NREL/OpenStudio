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
#include <model/ZoneHVACPackagedTerminalHeatPump.hpp>
#include <model/ZoneHVACPackagedTerminalHeatPump_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/CoilCoolingDXSingleSpeed.hpp>
#include <model/CoilCoolingDXSingleSpeed_Impl.hpp>
#include <model/CoilHeatingDXSingleSpeed.hpp>
#include <model/CoilHeatingDXSingleSpeed_Impl.hpp>
#include <utilities/idd/ZoneHVAC_PackagedTerminalHeatPump_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACPackagedTerminalHeatPump( 
    ZoneHVACPackagedTerminalHeatPump & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::ZoneHVAC_PackagedTerminalHeatPump);

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
      idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // AirInletNodeName

  boost::optional<std::string> airInletNodeName;

  if( boost::optional<Node> node = modelObject.inletNode() )
  {
    if( (s = node->name()) )
    {
      airInletNodeName = s;

      idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::AirInletNodeName,s.get());
    }
  }

  // AirOutletNodeName

  boost::optional<std::string> airOutletNodeName;

  if( boost::optional<Node> node = modelObject.outletNode() )
  {
    if( (s = node->name()) )
    {
      airOutletNodeName = s;

      idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::AirOutletNodeName,s.get());
    }
  }

  // OutdoorAirMixerObjectType
  idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirMixerObjectType,
                      modelObject.outdoorAirMixerObjectType());

  // OutdoorAirMixerName
  
  std::string oaMixerName = modelObject.name().get() + " OA Mixer";  

  idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirMixerName,oaMixerName);

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

  // SupplyAirFlowRateDuringCoolingOperation

  if( modelObject.isSupplyAirFlowRateDuringCoolingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringCoolingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringCoolingOperation,value.get());
  }

  // SupplyAirFlowRateDuringHeatingOperation

  if( modelObject.isSupplyAirFlowRateDuringHeatingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringHeatingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateDuringHeatingOperation,value.get());
  }

  // SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded

  if( modelObject.isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,"Autosize");
  } 
  else if( (value = modelObject.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,value.get());
  }

  // OutdoorAirFlowRateDuringCoolingOperation
  
  if( modelObject.isOutdoorAirFlowRateDuringCoolingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateDuringCoolingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringCoolingOperation,value.get());
  }

  // OutdoorAirFlowRateDuringHeatingOperation
  
  if( modelObject.isOutdoorAirFlowRateDuringHeatingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateDuringHeatingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateDuringHeatingOperation,value.get());
  }

  // OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded
  
  if( modelObject.isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,value.get());
  }

  // SupplyAirFanObjectType

  HVACComponent supplyAirFan = modelObject.supplyAirFan();

  if( boost::optional<IdfObject> _supplyAirFan = translateAndMapModelObject(supplyAirFan) )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanObjectType,_supplyAirFan->iddObject().name() );

    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanName,_supplyAirFan->name().get());

    if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_ConstantVolume )
    {
      _supplyAirFan->setString(Fan_ConstantVolumeFields::AirInletNodeName,heatingCoilOutletNodeName);

      _supplyAirFan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,fanOutletNodeName);
    }
  }

  // HeatingCoilObjectType
  
  HVACComponent heatingCoil = modelObject.heatingCoil();

  if( boost::optional<CoilHeatingDXSingleSpeed> coilHeatingDXSingleSpeed = heatingCoil.optionalCast<CoilHeatingDXSingleSpeed>() )
  {
    if( boost::optional<IdfObject> _heatingCoil = translateCoilHeatingDXSingleSpeedWithoutUnitary(coilHeatingDXSingleSpeed.get()) )
    {
      idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilObjectType,_heatingCoil->iddObject().name() );

      idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingCoilName,_heatingCoil->name().get() ); 

      if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_SingleSpeed )
      {
        _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName,coolingCoilOutletNodeName);

        _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName,heatingCoilOutletNodeName);
      }
    }
  }

  // HeatingConvergenceTolerance

  if( (value = modelObject.heatingConvergenceTolerance()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::HeatingConvergenceTolerance,value.get());
  }

  // MinimumOutdoorDryBulbTemperatureforCompressorOperation

  if( (value = modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,value.get());
  }

  // CoolingCoilObjectType

  HVACComponent coolingCoil = modelObject.coolingCoil();

  boost::optional<IdfObject> _coolingCoil;

  if( boost::optional<CoilCoolingDXSingleSpeed> dxCoil = coolingCoil.optionalCast<CoilCoolingDXSingleSpeed>() )
  {
    _coolingCoil = translateCoilCoolingDXSingleSpeedWithoutUnitary(dxCoil.get());

    m_map.insert(std::make_pair(coolingCoil.handle(),_coolingCoil.get()));
  }
  else
  {
    _coolingCoil = translateAndMapModelObject(coolingCoil);
  }

  if( _coolingCoil )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilObjectType,_coolingCoil->iddObject().name() );

    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingCoilName,_coolingCoil->name().get() );

    if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed )
    {
      _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName,mixedAirNodeName);

      _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName,coolingCoilOutletNodeName);
    }
  }

  // CoolingConvergenceTolerance
  
  if( (value = modelObject.coolingConvergenceTolerance()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::CoolingConvergenceTolerance,value.get());
  }

  // SupplementalHeatingCoilObjectType

  HVACComponent supplementalHeatingCoil = modelObject.supplementalHeatingCoil();

  if( boost::optional<IdfObject> _supplementalHeatingCoil = translateAndMapModelObject(supplementalHeatingCoil) )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplementalHeatingCoilObjectType,_supplementalHeatingCoil->iddObject().name() );

    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplementalHeatingCoilName,_supplementalHeatingCoil->name().get() );

    if( airOutletNodeName )
    {
      if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_GasFields::AirInletNodeName,fanOutletNodeName);

        _supplementalHeatingCoil->setString(Coil_Heating_GasFields::AirOutletNodeName,airOutletNodeName.get());
      }
      else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
      {
        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,fanOutletNodeName);

        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,airOutletNodeName.get());
      }
    }
  }

  // MaximumSupplyAirTemperaturefromSupplementalHeater
  
  if( modelObject.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater,"Autosize"); 
  }
  else if( (value = modelObject.maximumSupplyAirTemperaturefromSupplementalHeater()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumSupplyAirTemperaturefromSupplementalHeater,value.get());
  }

  // MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation

  if( (value = modelObject.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalHeatPumpFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,value.get());
  }

  // FanPlacement

  idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::FanPlacement,modelObject.fanPlacement());

  // SupplyAirFanOperatingModeScheduleName
  
  if( boost::optional<Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_PackagedTerminalHeatPumpFields::SupplyAirFanOperatingModeScheduleName,_schedule->name().get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

