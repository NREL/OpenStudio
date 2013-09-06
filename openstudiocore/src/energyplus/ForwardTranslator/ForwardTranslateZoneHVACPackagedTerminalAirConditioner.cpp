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
#include <model/ZoneHVACPackagedTerminalAirConditioner.hpp>
#include <model/ZoneHVACPackagedTerminalAirConditioner_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/CoilCoolingDXSingleSpeed.hpp>
#include <model/CoilCoolingDXSingleSpeed_Impl.hpp>
#include <utilities/idd/ZoneHVAC_PackagedTerminalAirConditioner_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneHVACPackagedTerminalAirConditioner( 
    ZoneHVACPackagedTerminalAirConditioner & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;

  IdfObject idfObject(IddObjectType::ZoneHVAC_PackagedTerminalAirConditioner);

  m_idfObjects.push_back(idfObject);

  // Name

  std::string baseName = modelObject.name().get();

  idfObject.setName(baseName);

  std::string mixedAirNodeName = baseName + " Mixed Air Node";

  std::string coolingCoilOutletNodeName = baseName + " Cooling Coil Outlet Node";

  std::string heatingCoilOutletNodeName = baseName + " Heating Coil Outlet Node";

  std::string reliefAirNodeName = baseName + " Relief Air Node";

  std::string oaNodeName = baseName + " OA Node";

  // AvailabilityScheduleName

  if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // AirInletNodeName

  boost::optional<std::string> airInletNodeName;

  if( boost::optional<Node> node = modelObject.inletNode() )
  {
    if( (s = node->name()) )
    {
      airInletNodeName = s;

      idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::AirInletNodeName,s.get());
    }
  }

  // AirOutletNodeName

  boost::optional<std::string> airOutletNodeName;

  if( boost::optional<Node> node = modelObject.outletNode() )
  {
    if( (s = node->name()) )
    {
      airOutletNodeName = s;

      idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::AirOutletNodeName,s.get());
    }
  }

  // OutdoorAirMixerObjectType
  idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerObjectType,
                      modelObject.outdoorAirMixerObjectType());

  // OutdoorAirMixerName
  
  std::string oaMixerName = modelObject.name().get() + " OA Mixer";  

  idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirMixerName,oaMixerName);

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
    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringCoolingOperation,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringCoolingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringCoolingOperation,value.get());
  }

  // SupplyAirFlowRateDuringHeatingOperation

  if( modelObject.isSupplyAirFlowRateDuringHeatingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringHeatingOperation,"Autosize");
  }
  else if( (value = modelObject.supplyAirFlowRateDuringHeatingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateDuringHeatingOperation,value.get());
  }

  // SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded

  if( modelObject.isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,"Autosize");
  } 
  else if( (value = modelObject.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,value.get());
  }

  // OutdoorAirFlowRateDuringCoolingOperation
  
  if( modelObject.isOutdoorAirFlowRateDuringCoolingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateDuringCoolingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringCoolingOperation,value.get());
  }

  // OutdoorAirFlowRateDuringHeatingOperation
  
  if( modelObject.isOutdoorAirFlowRateDuringHeatingOperationAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateDuringHeatingOperation()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateDuringHeatingOperation,value.get());
  }

  // OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded
  
  if( modelObject.isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,"Autosize");
  }
  else if( (value = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded()) )
  {
    idfObject.setDouble(ZoneHVAC_PackagedTerminalAirConditionerFields::OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded,value.get());
  }

  // SupplyAirFanObjectType

  HVACComponent supplyAirFan = modelObject.supplyAirFan();

  if( boost::optional<IdfObject> _supplyAirFan = translateAndMapModelObject(supplyAirFan) )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanObjectType,_supplyAirFan->iddObject().name() );

    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanName,_supplyAirFan->name().get());

    if( airOutletNodeName )
    {
      if( _supplyAirFan->iddObject().type() == IddObjectType::Fan_ConstantVolume )
      {
        _supplyAirFan->setString(Fan_ConstantVolumeFields::AirInletNodeName,heatingCoilOutletNodeName);

        _supplyAirFan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,airOutletNodeName.get());
      }
    }
  }

  // HeatingCoilObjectType
  
  HVACComponent heatingCoil = modelObject.heatingCoil();

  if( boost::optional<IdfObject> _heatingCoil = translateAndMapModelObject(heatingCoil) )
  {
    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::HeatingCoilObjectType,_heatingCoil->iddObject().name() );

    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::HeatingCoilName,_heatingCoil->name().get() ); 

    if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
    {
      _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,coolingCoilOutletNodeName);

      _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,heatingCoilOutletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas )
    {
      _heatingCoil->setString(Coil_Heating_GasFields::AirInletNodeName,coolingCoilOutletNodeName);

      _heatingCoil->setString(Coil_Heating_GasFields::AirOutletNodeName,heatingCoilOutletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
    {
      _heatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,coolingCoilOutletNodeName);

      _heatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,heatingCoilOutletNodeName);
    }
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
    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilObjectType,_coolingCoil->iddObject().name() );

    idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::CoolingCoilName,_coolingCoil->name().get() );

    if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed )
    {
      _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName,mixedAirNodeName);

      _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName,coolingCoilOutletNodeName);
    }
  }

  // FanPlacement

  idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::FanPlacement,modelObject.fanPlacement());

  // SupplyAirFanOperatingModeScheduleName
  
  if( boost::optional<Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(ZoneHVAC_PackagedTerminalAirConditionerFields::SupplyAirFanOperatingModeScheduleName,_schedule->name().get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

