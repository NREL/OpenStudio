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
#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "../../model/AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"
#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilCoolingDXSingleSpeed_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_TwoSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACUnitaryHeatCoolVAVChangeoverBypass( AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> d;

  // Name
  IdfObject unitarySystem = createRegisterAndNameIdfObject(openstudio::IddObjectType::AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass, modelObject);

  // Availability Schedule Name
  if( boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule() )
  {
    boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

    if( _availabilitySchedule && _availabilitySchedule->name() )
    {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
    }
  }

  //  SystemAirFlowRateDuringCoolingOperation
  if( modelObject.isSystemAirFlowRateDuringCoolingOperationAutosized() ) {
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingSupplyAirFlowRate,"AutoSize");
  } else if( (d = modelObject.systemAirFlowRateDuringCoolingOperation()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingSupplyAirFlowRate,d.get());
  }

  //  SystemAirFlowRateDuringHeatingOperation
  if( modelObject.isSystemAirFlowRateDuringHeatingOperationAutosized() ) {
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingSupplyAirFlowRate,"AutoSize");
  } else if( (d = modelObject.systemAirFlowRateDuringHeatingOperation()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingSupplyAirFlowRate,d.get());
  }
  
  //  SystemAirFlowRateWhenNoCoolingorHeatingisNeeded
  if( modelObject.isSystemAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() ) {
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::NoLoadSupplyAirFlowRate,"AutoSize");
  } else if( (d = modelObject.systemAirFlowRateWhenNoCoolingorHeatingisNeeded()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::NoLoadSupplyAirFlowRate,d.get());
  } 
  
  //  OutdoorAirFlowRateDuringCoolingOperation
  if( modelObject.isOutdoorAirFlowRateDuringCoolingOperationAutosized() ) {
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingOutdoorAirFlowRate,"AutoSize");
  } else if( (d = modelObject.outdoorAirFlowRateDuringCoolingOperation()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingOutdoorAirFlowRate,d.get());
  }
  
  //  OutdoorAirFlowRateDuringHeatingOperation
  if( modelObject.isOutdoorAirFlowRateDuringHeatingOperationAutosized() ) {
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingOutdoorAirFlowRate,"AutoSize");
  } else if( (d = modelObject.outdoorAirFlowRateDuringHeatingOperation()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingOutdoorAirFlowRate,d.get());
  }
  
  //  OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded
  if( modelObject.isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() ) {
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::NoLoadOutdoorAirFlowRate,"AutoSize");
  } else if( (d = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::NoLoadOutdoorAirFlowRate,d.get());
  } 

  //  OutdoorAirFlowRateMultiplierScheduleName
  if( boost::optional<Schedule> schedule = modelObject.outdoorAirFlowRateMultiplierSchedule() ) {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) ) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirFlowRateMultiplierScheduleName,_schedule->name().get());
    }
  }

  boost::optional<std::string> airInletNodeName;
  boost::optional<std::string> airOutletNodeName;

  // Air Inlet Node Name
  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      airInletNodeName = node->name().get();
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AirInletNodeName,node->name().get());
    }
  }

  // Air Outlet Node Name
  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      airOutletNodeName = node->name().get();
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::AirOutletNodeName,node->name().get());
    }
  }

  std::string baseName = modelObject.name().get();

  //  BypassDuctMixerNodeName
  std::string bypassDuctMixerNodeName = baseName + " Bypass Duct Mixer Node";
  unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::BypassDuctMixerNodeName,bypassDuctMixerNodeName);
  
  //  BypassDuctSplitterNodeName
  std::string bypassDuctSplitterNodeName = baseName + " Bypass Duct Splitter Node";
  unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::BypassDuctSplitterNodeName,bypassDuctSplitterNodeName);
  
  //  OutdoorAirMixerObjectType
  unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirMixerObjectType,"OutdoorAir:Mixer");
  
  //  OutdoorAirMixerName
  std::string outdoorAirMixerName = baseName + " Outdoor Air Mixer";
  unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::OutdoorAirMixerName,outdoorAirMixerName);

  //  SupplyAirFanName
  boost::optional<IdfObject> _fan;
  if( boost::optional<HVACComponent> fan = modelObject.supplyAirFan() ) {
    if( (_fan = translateAndMapModelObject(fan.get())) ) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanObjectType,_fan->iddObject().name());
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanName,_fan->name().get());
    }
  }
  
  bool blowThroughFan = false;
  //  SupplyAirFanPlacement
  if( (s = modelObject.supplyAirFanPlacement()) ) {
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanPlacement,s.get());
    if( istringEqual(s.get(),"BlowThrough") ) {
      blowThroughFan = true;
    }
  }
  
  //  SupplyAirFanOperatingModeScheduleName
  if( boost::optional<Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule() ) {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) ) {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::SupplyAirFanOperatingModeScheduleName,_schedule->name().get()); 
    }
  }

  // Heating Coil Object Type
  // Heating Coil Name
  boost::optional<IdfObject> _heatingCoil;
  if( boost::optional<HVACComponent> heatingCoil = modelObject.heatingCoil() )
  {
    _heatingCoil = translateAndMapModelObject(heatingCoil.get());

    if( _heatingCoil && _heatingCoil->name() )
    {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingCoilObjectType,_heatingCoil->iddObject().name());
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::HeatingCoilName,_heatingCoil->name().get());
    }
  }

  // Cooling Coil Object Type
  // Cooling Coil Name
  boost::optional<IdfObject> _coolingCoil;
  if( boost::optional<HVACComponent> coolingCoil = modelObject.coolingCoil() )
  {
    if( boost::optional<CoilCoolingDXSingleSpeed> dxCoil = coolingCoil->optionalCast<CoilCoolingDXSingleSpeed>() ) {
      _coolingCoil = translateCoilCoolingDXSingleSpeedWithoutUnitary(dxCoil.get());
      m_map.insert(std::make_pair(coolingCoil->handle(),_coolingCoil.get()));
    } else {
      _coolingCoil = translateAndMapModelObject(coolingCoil.get());
    }

    if( _coolingCoil && _coolingCoil->name() )
    {
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingCoilObjectType,_coolingCoil->iddObject().name());
      unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::CoolingCoilName,_coolingCoil->name().get());
    }
  }

  //  PriorityControlMode
  if( (s = modelObject.priorityControlMode()) ) {
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::PriorityControlMode,s.get());
  }
  
  //  MinimumOutletAirTemperatureDuringCoolingOperation
  if( (d = modelObject.minimumOutletAirTemperatureDuringCoolingOperation()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MinimumOutletAirTemperatureDuringCoolingOperation,d.get());
  }
  
  //  MaximumOutletAirTemperatureDuringHeatingOperation)(Maximum Outlet Air Temperature During Heating Operation))
  if( (d = modelObject.maximumOutletAirTemperatureDuringHeatingOperation()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::MaximumOutletAirTemperatureDuringHeatingOperation,d.get());
  } 
  
  // DehumidificationControlType
  if( (s = modelObject.dehumidificationControlType()) ) {
    unitarySystem.setString(AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypassFields::DehumidificationControlType,s.get());
  }

  if( _fan && _coolingCoil && _heatingCoil )
  {
    std::string fanOutletNodeName;
    std::string fanInletNodeName;
    std::string heatOutletNodeName;
    std::string heatInletNodeName;
    std::string coolOutletNodeName;
    std::string coolInletNodeName;
    std::string mixedAirNodeName;

    if( blowThroughFan ) {
      mixedAirNodeName = baseName + " Mixed Air Node";
      fanInletNodeName = mixedAirNodeName;
      fanOutletNodeName = baseName + " Fan Outlet Node";
      coolInletNodeName = fanOutletNodeName;
      coolOutletNodeName = baseName + " Cooling Coil Outlet Node";
      heatInletNodeName = coolOutletNodeName;
      heatOutletNodeName = bypassDuctSplitterNodeName;
    } else {
      mixedAirNodeName = baseName + " Mixed Air Node";
      coolInletNodeName = mixedAirNodeName;
      coolOutletNodeName = baseName + " Cooling Coil Outlet Node";
      heatInletNodeName = coolOutletNodeName;
      heatOutletNodeName = baseName + " Heating Coil Outlet Node";
      fanInletNodeName = heatOutletNodeName;
      fanOutletNodeName = bypassDuctSplitterNodeName;
    }

    if( _fan->iddObject().type() == IddObjectType::Fan_ConstantVolume )
    {
      _fan->setString(Fan_ConstantVolumeFields::AirInletNodeName,fanInletNodeName);
      _fan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,fanOutletNodeName);
    }
    else if( _fan->iddObject().type() == IddObjectType::Fan_VariableVolume )
    {
      _fan->setString(Fan_VariableVolumeFields::AirInletNodeName,fanInletNodeName);
      _fan->setString(Fan_VariableVolumeFields::AirOutletNodeName,fanOutletNodeName);
    }
    else if( _fan->iddObject().type() == IddObjectType::Fan_OnOff )
    {
      _fan->setString(Fan_OnOffFields::AirInletNodeName,fanInletNodeName);
      _fan->setString(Fan_OnOffFields::AirOutletNodeName,fanOutletNodeName);
    }

    fixSPMsForUnitarySystem(modelObject,fanInletNodeName,fanOutletNodeName);

    if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed )
    {
      _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName,coolInletNodeName);
      _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName,coolOutletNodeName);
    }
    else if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_TwoSpeed )
    {
      _coolingCoil->setString(Coil_Cooling_DX_TwoSpeedFields::AirInletNodeName,coolInletNodeName);
      _coolingCoil->setString(Coil_Cooling_DX_TwoSpeedFields::AirOutletNodeName,coolOutletNodeName);
    }
    else if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_Water )
    {
      _coolingCoil->setString(Coil_Cooling_WaterFields::AirInletNodeName,coolInletNodeName);
      _coolingCoil->setString(Coil_Cooling_WaterFields::AirOutletNodeName,coolOutletNodeName);
    }
    else if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit )
    {
      _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName,coolInletNodeName);
      _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName,coolOutletNodeName);
    }

    if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_SingleSpeed )
    {
      _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName,heatInletNodeName);
      _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName,heatOutletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
    {
      _heatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,heatInletNodeName);
      _heatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,heatOutletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas )
    {
      _heatingCoil->setString(Coil_Heating_GasFields::AirInletNodeName,heatInletNodeName);
      _heatingCoil->setString(Coil_Heating_GasFields::AirOutletNodeName,heatOutletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
    {
      _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,heatInletNodeName);
      _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,heatOutletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Desuperheater )
    {
      _heatingCoil->setString(Coil_Heating_DesuperheaterFields::AirInletNodeName,heatInletNodeName);
      _heatingCoil->setString(Coil_Heating_DesuperheaterFields::AirOutletNodeName,heatOutletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit )
    {
      _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName,heatInletNodeName);
      _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName,heatOutletNodeName);
    }

    IdfObject _outdoorAirMixer(IddObjectType::OutdoorAir_Mixer);
    _outdoorAirMixer.setName(outdoorAirMixerName);
    m_idfObjects.push_back(_outdoorAirMixer);

    std::string oaNodeName = baseName + " OA Node";
    _outdoorAirMixer.setString(OutdoorAir_MixerFields::MixedAirNodeName,mixedAirNodeName);
    _outdoorAirMixer.setString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName,oaNodeName);

    IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
    _oaNodeList.setString(0,oaNodeName);
    m_idfObjects.push_back(_oaNodeList);
    _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReliefAirStreamNodeName,baseName + " Relief Air Node");
    _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReturnAirStreamNodeName,bypassDuctMixerNodeName);
  }

  return unitarySystem;
}

} // energyplus

} // openstudio

