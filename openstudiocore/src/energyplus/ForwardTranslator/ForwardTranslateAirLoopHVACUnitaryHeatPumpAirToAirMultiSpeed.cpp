/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed.hpp"
#include "../../model/AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeed_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_MultiStage_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed( AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> value;
  boost::optional<int> num;

  IdfObject idfObject(IddObjectType::AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed);

  m_idfObjects.push_back(idfObject);

  // Name
  std::string baseName = modelObject.name().get();
  idfObject.setName(baseName);
 

  // AvailabilityScheduleName
  if( boost::optional<Schedule> schedule = modelObject.availabilitySchedule() )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get()) )
    {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // AirInletNodeName
  std::string airInletNodeName;
  if( boost::optional<ModelObject> mo = modelObject.inletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      if( (s = node->name()) )
      {
        airInletNodeName = s.get();
        idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AirInletNodeName,s.get());
      }
    }
  }

  // AirOutletNodeName
  std::string airOutletNodeName;
  if( boost::optional<ModelObject> mo = modelObject.outletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      if( (s = node->name()) )
      {
        airOutletNodeName = s.get();
        idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AirOutletNodeName,s.get());
      }
    }
  }

  // MinimumOutdoorDryBulbTemperatureforCompressorOperation
  if( value = modelObject.minimumOutdoorDryBulbTemperatureforCompressorOperation() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MinimumOutdoorDryBulbTemperatureforCompressorOperation,value.get());
  }

  HVACComponent coolingCoil = modelObject.coolingCoil();
  boost::optional<IdfObject> _coolingCoil = translateAndMapModelObject(coolingCoil);
  if( _coolingCoil ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::CoolingCoilObjectType,_coolingCoil->iddObject().name() );
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::CoolingCoilName,_coolingCoil->name().get());
  }

  HVACComponent heatingCoil = modelObject.heatingCoil();
  boost::optional<IdfObject> _heatingCoil = translateAndMapModelObject(heatingCoil);
  if( _heatingCoil ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::HeatingCoilObjectType,_heatingCoil->iddObject().name() );
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::HeatingCoilName,_heatingCoil->name().get());
  }

  // ControllingZoneorThermostatLocation
  if( boost::optional<ThermalZone> tz = modelObject.controllingZoneorThermostatLocation() )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::ControllingZoneorThermostatLocation,tz->name().get() );
  } else if( auto airLoop = modelObject.airLoopHVAC() ) {
    auto zones = airLoop->thermalZones();
    if( zones.size() == 1u ) {
      if( auto zone = translateAndMapModelObject(zones.front()) ) {
        idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::ControllingZoneorThermostatLocation,zone->name().get() );
      }
    }
  }

  // SupplyAirFanName
  HVACComponent fan = modelObject.supplyAirFan();
  boost::optional<IdfObject> _fan = translateAndMapModelObject(fan);
  if( _fan )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanObjectType,_fan->iddObject().name() );
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanName,_fan->name().get());
  }

  // SupplementalHeatingCoilName
  boost::optional<IdfObject> _supplementalHeatingCoil;
  boost::optional<HVACComponent> supplementalHeatingCoil = modelObject.supplementalHeatingCoil();
  if( supplementalHeatingCoil )
  {
    _supplementalHeatingCoil = translateAndMapModelObject(supplementalHeatingCoil.get());
  }
  if( _supplementalHeatingCoil )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplementalHeatingCoilObjectType,
      _supplementalHeatingCoil->iddObject().name());

    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplementalHeatingCoilName,_supplementalHeatingCoil->name().get()); 
  }

  // FanPlacement
  bool t_blowThrough = true;
  if( (s = modelObject.supplyAirFanPlacement()) )
  {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanPlacement,s.get());
    if( istringEqual(s.get(),"DrawThrough") ) {
      t_blowThrough = false;
    }
  }

  // SupplyAirFanOperatingModeScheduleName
  if( boost::optional<Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule() )
  {
    boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get());
    if( _schedule )
    {
      idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFanOperatingModeScheduleName,_schedule->name().get());
    }
  }

  // MaximumSupplyAirTemperaturefromSupplementalHeater
  if( modelObject.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized() ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumSupplyAirTemperaturefromSupplementalHeater,"AutoSize");
  } else if( value = modelObject.maximumSupplyAirTemperaturefromSupplementalHeater() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumSupplyAirTemperaturefromSupplementalHeater,value.get());
  }
  
  // MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation
  if( value = modelObject.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,value.get());
  } 

  // AuxiliaryOnCycleElectricPower
  if( value = modelObject.auxiliaryOnCycleElectricPower() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AuxiliaryOnCycleElectricPower,value.get());
  }

  // AuxiliaryOffCycleElectricPower
  if( value = modelObject.auxiliaryOffCycleElectricPower() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::AuxiliaryOffCycleElectricPower,value.get());
  }

  // DesignHeatRecoveryWaterFlowRate
  if( value = modelObject.designHeatRecoveryWaterFlowRate() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::DesignHeatRecoveryWaterFlowRate,value.get());
  }

  // MaximumTemperatureforHeatRecovery
  if( value = modelObject.maximumTemperatureforHeatRecovery() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::MaximumTemperatureforHeatRecovery,value.get());
  }

  // HeatRecoveryWaterInletNodeName

  // HeatRecoveryWaterOutletNodeName

  // SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded
  if( modelObject.isSupplyAirFlowRateWhenNoCoolingorHeatingisNeededAutosized() ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,"AutoSize");
  } else if( value = modelObject.supplyAirFlowRateWhenNoCoolingorHeatingisNeeded() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::SupplyAirFlowRateWhenNoCoolingorHeatingisNeeded,value.get());
  }

  // NumberofSpeedsforHeating
  if( num = modelObject.numberofSpeedsforHeating() ) {
    idfObject.setInt(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::NumberofSpeedsforHeating,num.get());
  }

  // NumberofSpeedsforCooling
  if( num = modelObject.numberofSpeedsforCooling() ) {
    idfObject.setInt(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::NumberofSpeedsforCooling,num.get());
  }

  // Speed1SupplyAirFlowRateDuringHeatingOperation
  if( modelObject.isSpeed1SupplyAirFlowRateDuringHeatingOperationAutosized() ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringHeatingOperation,"AutoSize");
  } else if( value = modelObject.speed1SupplyAirFlowRateDuringHeatingOperation() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringHeatingOperation,value.get());
  }

  // Speed2SupplyAirFlowRateDuringHeatingOperation
  if( modelObject.isSpeed2SupplyAirFlowRateDuringHeatingOperationAutosized() ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringHeatingOperation,"AutoSize");
  } else if( value = modelObject.speed2SupplyAirFlowRateDuringHeatingOperation() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringHeatingOperation,value.get());
  }

  // Speed3SupplyAirFlowRateDuringHeatingOperation
  if( modelObject.isSpeed3SupplyAirFlowRateDuringHeatingOperationAutosized() ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringHeatingOperation,"AutoSize");
  } else if( value = modelObject.speed3SupplyAirFlowRateDuringHeatingOperation() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringHeatingOperation,value.get());
  }

  // Speed4SupplyAirFlowRateDuringHeatingOperation
  if( modelObject.isSpeed4SupplyAirFlowRateDuringHeatingOperationAutosized() ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringHeatingOperation,"AutoSize");
  } else if( value = modelObject.speed4SupplyAirFlowRateDuringHeatingOperation() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringHeatingOperation,value.get());
  }

  // Speed1SupplyAirFlowRateDuringCoolingOperation
  if( modelObject.isSpeed1SupplyAirFlowRateDuringCoolingOperationAutosized() ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringCoolingOperation,"AutoSize");
  } else if( value = modelObject.speed1SupplyAirFlowRateDuringCoolingOperation() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed1SupplyAirFlowRateDuringCoolingOperation,value.get());
  }

  // Speed2SupplyAirFlowRateDuringCoolingOperation
  if( modelObject.isSpeed2SupplyAirFlowRateDuringCoolingOperationAutosized() ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringCoolingOperation,"AutoSize");
  } else if( value = modelObject.speed2SupplyAirFlowRateDuringCoolingOperation() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed2SupplyAirFlowRateDuringCoolingOperation,value.get());
  }

  // Speed3SupplyAirFlowRateDuringCoolingOperation
  if( modelObject.isSpeed3SupplyAirFlowRateDuringCoolingOperationAutosized() ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringCoolingOperation,"AutoSize");
  } else if( value = modelObject.speed3SupplyAirFlowRateDuringCoolingOperation() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed3SupplyAirFlowRateDuringCoolingOperation,value.get());
  }

  // Speed4SupplyAirFlowRateDuringCoolingOperation
  if( modelObject.isSpeed4SupplyAirFlowRateDuringCoolingOperationAutosized() ) {
    idfObject.setString(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringCoolingOperation,"AutoSize");
  } else if( value = modelObject.speed4SupplyAirFlowRateDuringCoolingOperation() ) {
    idfObject.setDouble(AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeedFields::Speed4SupplyAirFlowRateDuringCoolingOperation,value.get());
  }

  std::string fanOutletNodeName;
  std::string fanInletNodeName;
  std::string coolingCoilOutletNodeName;
  std::string coolingCoilInletNodeName;
  std::string heatingCoilOutletNodeName;
  std::string heatingCoilInletNodeName;
  std::string suppCoilOutletNodeName;
  std::string suppCoilInletNodeName;

  if( t_blowThrough ) {
    fanInletNodeName = airInletNodeName;
    fanOutletNodeName = baseName + " Fan Outlet";

    coolingCoilInletNodeName = fanOutletNodeName;
    coolingCoilOutletNodeName = baseName + " Cool Coil Outlet";

    heatingCoilInletNodeName = coolingCoilOutletNodeName;
    heatingCoilOutletNodeName = baseName + " Heat Coil Outlet";

    suppCoilInletNodeName = heatingCoilOutletNodeName;
    suppCoilOutletNodeName = airOutletNodeName;
  } else {
    coolingCoilInletNodeName = airInletNodeName;
    coolingCoilOutletNodeName = baseName + " Cool Coil Outlet";

    heatingCoilInletNodeName = coolingCoilOutletNodeName; 
    heatingCoilOutletNodeName = baseName + " Heat Coil Outlet";

    fanInletNodeName = heatingCoilOutletNodeName;
    fanOutletNodeName = baseName + " Fan Outlet";

    suppCoilInletNodeName = fanOutletNodeName;
    suppCoilOutletNodeName = airOutletNodeName;
  }

  if(  _fan ) {
    if( _fan->iddObject().type() == IddObjectType::Fan_ConstantVolume ) {
      _fan->setString(Fan_ConstantVolumeFields::AirInletNodeName,fanInletNodeName);
      _fan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,fanOutletNodeName);
    } else if( _fan->iddObject().type() == IddObjectType::Fan_OnOff ) {
      _fan->setString(Fan_OnOffFields::AirInletNodeName,fanInletNodeName);
      _fan->setString(Fan_OnOffFields::AirOutletNodeName,fanOutletNodeName);
    }
  }

  if( _coolingCoil ) {
    if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_MultiSpeed ) {
      _coolingCoil->setString(Coil_Cooling_DX_MultiSpeedFields::AirInletNodeName,coolingCoilInletNodeName);
      _coolingCoil->setString(Coil_Cooling_DX_MultiSpeedFields::AirOutletNodeName,coolingCoilOutletNodeName);
    }
  }

  if( _heatingCoil ) {
    if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas_MultiStage ) {
      _heatingCoil->setString(Coil_Heating_Gas_MultiStageFields::AirInletNodeName,heatingCoilInletNodeName);
      _heatingCoil->setString(Coil_Heating_Gas_MultiStageFields::AirOutletNodeName,heatingCoilOutletNodeName);
    } else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water ) {
      _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,heatingCoilInletNodeName);
      _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,heatingCoilOutletNodeName);
    }
  }

  if( _supplementalHeatingCoil ) {
    if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water ) {
      _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,suppCoilInletNodeName);
      _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,suppCoilOutletNodeName);
    } else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas ) {
      _supplementalHeatingCoil->setString(Coil_Heating_GasFields::AirInletNodeName,suppCoilInletNodeName);
      _supplementalHeatingCoil->setString(Coil_Heating_GasFields::AirOutletNodeName,suppCoilOutletNodeName);
    } else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric ) {
      _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,suppCoilInletNodeName);
      _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,suppCoilOutletNodeName);
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

