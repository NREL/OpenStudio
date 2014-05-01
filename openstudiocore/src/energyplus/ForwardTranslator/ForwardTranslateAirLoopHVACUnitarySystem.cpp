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

#include <energyplus/ForwardTranslator.hpp>
#include <model/Model.hpp>
#include <model/AirLoopHVACUnitarySystem.hpp>
#include <model/AirLoopHVACUnitarySystem_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <utilities/idd/AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
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
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACUnitarySystem( AirLoopHVACUnitarySystem & modelObject )
{
  boost::optional<std::string> s;
  boost::optional<double> d;

  // Name
  IdfObject unitarySystem = createRegisterAndNameIdfObject(openstudio::IddObjectType::AirLoopHVAC_UnitarySystem, modelObject);

  // Control Type
  unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::ControlType,"Load");
  // s = modelObject.controlType();
  // if (s) {
  //   unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::ControlType,s.get());
  // }

  // Controlling Zone or Thermostat Location
  if( boost::optional<ThermalZone> controllingZoneorThermostatLocation = modelObject.controllingZoneorThermostatLocation() )
  {
    boost::optional<IdfObject> _controllingZoneorThermostatLocation = translateAndMapModelObject(controllingZoneorThermostatLocation.get());

    if( _controllingZoneorThermostatLocation && _controllingZoneorThermostatLocation->name() )
    {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::ControllingZoneorThermostatLocation,_controllingZoneorThermostatLocation->name().get());
    }
  }

  // Dehumidification Control Type
  s = modelObject.dehumidificationControlType();
  if (s) {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::DehumidificationControlType,s.get());
  }

  // Availability Schedule Name
  if( boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule() )
  {
    boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

    if( _availabilitySchedule && _availabilitySchedule->name() )
    {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::AvailabilityScheduleName,_availabilitySchedule->name().get());
    }
  }

  boost::optional<std::string> airInletNodeName;
  boost::optional<std::string> airOutletNodeName;

  // Air Inlet Node Name
  if( boost::optional<ModelObject> mo = modelObject.airInletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      airInletNodeName = node->name().get();
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::AirInletNodeName,node->name().get());
    }
  }

  // Air Outlet Node Name
  if( boost::optional<ModelObject> mo = modelObject.airOutletModelObject() )
  {
    if( boost::optional<Node> node = mo->optionalCast<Node>() )
    {
      airOutletNodeName = node->name().get();
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::AirOutletNodeName,node->name().get());
    }
  }

  // Supply Fan Object Type
  // Supply Fan Name
  boost::optional<IdfObject> _fan;

  if( boost::optional<HVACComponent> supplyFan = modelObject.supplyFan() )
  {
    _fan = translateAndMapModelObject(supplyFan.get());

    if( _fan && _fan->name() )
    {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyFanObjectType,_fan->iddObject().name());
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyFanName,_fan->name().get());
    }
  }

  // Fan Placement
  s = modelObject.fanPlacement();
  bool blowThroughFan = false;
  if (s) {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::FanPlacement,s.get());
    if( istringEqual(*s, "BlowThrough") ) {
      blowThroughFan = true;
    }
  }

  // Supply Air Fan Operating Mode Schedule Name
  if( boost::optional<Schedule> supplyAirFanOperatingModeSchedule = modelObject.supplyAirFanOperatingModeSchedule() )
  {
    boost::optional<IdfObject> _supplyAirFanOperatingModeSchedule = translateAndMapModelObject(supplyAirFanOperatingModeSchedule.get());

    if( _supplyAirFanOperatingModeSchedule && _supplyAirFanOperatingModeSchedule->name() )
    {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyAirFanOperatingModeScheduleName,_supplyAirFanOperatingModeSchedule->name().get());
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
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::HeatingCoilObjectType,_heatingCoil->iddObject().name());
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::HeatingCoilName,_heatingCoil->name().get());
    }
  }

  // DX Heating Coil Sizing Ratio
  d = modelObject.dXHeatingCoilSizingRatio();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::DXHeatingCoilSizingRatio,d.get());
  }

  // Cooling Coil Object Type
  // Cooling Coil Name
  boost::optional<IdfObject> _coolingCoil;

  if( boost::optional<HVACComponent> coolingCoil = modelObject.coolingCoil() )
  {
    _coolingCoil = translateAndMapModelObject(coolingCoil.get());

    if( _coolingCoil && _coolingCoil->name() )
    {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::CoolingCoilObjectType,_coolingCoil->iddObject().name());
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::CoolingCoilName,_coolingCoil->name().get());
    }
  }

  // Use DOAS DX Cooling Coil
  if( modelObject.useDOASDXCoolingCoil() )
  {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil,"Yes");
  }
  else
  {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil,"No");
  }

  // DOAS DX Cooling Coil Leaving Minimum Air Temperature
  d = modelObject.dOASDXCoolingCoilLeavingMinimumAirTemperature();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::DOASDXCoolingCoilLeavingMinimumAirTemperature,d.get());
  }

  // Latent Load Control
  s = modelObject.latentLoadControl();
  if (s) {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::LatentLoadControl,s.get());
  }

  // Supplemental Heating Coil Object Type
  // Supplemental Heating Coil Name
  boost::optional<IdfObject> _supplementalHeatingCoil;

  if( boost::optional<HVACComponent> supplementalHeatingCoil = modelObject.supplementalHeatingCoil() )
  {
    _supplementalHeatingCoil = translateAndMapModelObject(supplementalHeatingCoil.get());

    if( _supplementalHeatingCoil && _supplementalHeatingCoil->name() )
    {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilObjectType,_supplementalHeatingCoil->iddObject().name());
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilName,_supplementalHeatingCoil->name().get());
    }
  }

  // Supply Air Flow Rate Method During Cooling Operation
  s = modelObject.supplyAirFlowRateMethodDuringCoolingOperation();
  if (s) {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringCoolingOperation,s.get());
  }

  // Supply Air Flow Rate During Cooling Operation
  if( modelObject.isSupplyAirFlowRateDuringCoolingOperationAutosized() ) {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation,"Autosize");
  } 
  else if ( (d = modelObject.supplyAirFlowRateDuringCoolingOperation()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringCoolingOperation,d.get());
  }

  // Supply Air Flow Rate Per Floor Area During Cooling Operation
  d = modelObject.supplyAirFlowRatePerFloorAreaDuringCoolingOperation();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaDuringCoolingOperation,d.get());
  }

  // Fraction of Autosized Design Cooling Supply Air Flow Rate
  d = modelObject.fractionofAutosizedDesignCoolingSupplyAirFlowRate();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRate,d.get());
  }

  // Design Supply Air Flow Rate Per Unit of Capacity During Cooling Operation
  d = modelObject.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation,d.get());
  }

  // Supply Air Flow Rate Method During Heating Operation
  s = modelObject.supplyAirFlowRateMethodDuringHeatingOperation();
  if (s) {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodDuringHeatingOperation,s.get());
  }

  // Supply Air Flow Rate During Heating Operation
  if( modelObject.isSupplyAirFlowRateDuringHeatingOperationAutosized() ) {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation,"Autosize");
  } 
  else if ( (d = modelObject.supplyAirFlowRateDuringHeatingOperation()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateDuringHeatingOperation,d.get());
  }

  // Supply Air Flow Rate Per Floor Area during Heating Operation
  d = modelObject.supplyAirFlowRatePerFloorAreaduringHeatingOperation();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaduringHeatingOperation,d.get());
  }

  // Fraction of Autosized Design Heating Supply Air Flow Rate
  d = modelObject.fractionofAutosizedDesignHeatingSupplyAirFlowRate();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRate,d.get());
  }

  // Design Supply Air Flow Rate Per Unit of Capacity During Heating Operation
  d = modelObject.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation,d.get());
  }

  // Supply Air Flow Rate Method When No Cooling or Heating is Required
  s = modelObject.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired();
  if (s) {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired,s.get());
  }

  // Supply Air Flow Rate When No Cooling or Heating is Required
  if( modelObject.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized() ) {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateWhenNoCoolingorHeatingisRequired,"Autosize");
  } 
  else if ( (d = modelObject.supplyAirFlowRateWhenNoCoolingorHeatingisRequired()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRateWhenNoCoolingorHeatingisRequired,d.get());
  }

  // Supply Air Flow Rate Per Floor Area When No Cooling or Heating is Required
  d = modelObject.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::SupplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired,d.get());
  }

  // Fraction of Autosized Design Cooling Supply Air Flow Rate When No Cooling or Heating is Required
  d = modelObject.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired,d.get());
  }

  // Fraction of Autosized Design Heating Supply Air Flow Rate When No Cooling or Heating is Required
  d = modelObject.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::FractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired,d.get());
  }

  // Design Supply Air Flow Rate Per Unit of Capacity During Cooling Operation When No Cooling or Heating is Required
  d = modelObject.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired,d.get());
  }

  // Design Supply Air Flow Rate Per Unit of Capacity During Heating Operation When No Cooling or Heating is Required
  d = modelObject.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::DesignSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired,d.get());
  }

  // Maximum Supply Air Temperature
  if( modelObject.isMaximumSupplyAirTemperatureAutosized() ) {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature,"Autosize");
  } 
  else if ( (d = modelObject.maximumSupplyAirTemperature()) ) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature,d.get());
  }

  // Maximum Outdoor Dry-Bulb Temperature for Supplemental Heater Operation
  d = modelObject.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,d.get());
  }

  // Outdoor Dry-Bulb Temperature Sensor Node Name
  s = modelObject.outdoorDryBulbTemperatureSensorNodeName();
  if (s) {
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::OutdoorDryBulbTemperatureSensorNodeName,s.get());
  }

  // Maximum Cycling Rate
  d = modelObject.maximumCyclingRate();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::MaximumCyclingRate,d.get());
  }

  // Heat Pump Time Constant
  d = modelObject.heatPumpTimeConstant();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::HeatPumpTimeConstant,d.get());
  }

  // Fraction of On-Cycle Power Use
  d = modelObject.fractionofOnCyclePowerUse();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::FractionofOnCyclePowerUse,d.get());
  }

  // Heat Pump Fan Delay Time
  d = modelObject.heatPumpFanDelayTime();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::HeatPumpFanDelayTime,d.get());
  }

  // Ancilliary On-Cycle Electric Power
  d = modelObject.ancilliaryOnCycleElectricPower();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::AncilliaryOnCycleElectricPower,d.get());
  }

  // Ancilliary Off-Cycle Electric Power
  d = modelObject.ancilliaryOffCycleElectricPower();
  if (d) {
    unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::AncilliaryOffCycleElectricPower,d.get());
  }

  // Not Implemented
  // Design Heat Recovery Water Flow Rate
  // d = modelObject.designHeatRecoveryWaterFlowRate();
  // if (d) {
  //   unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::DesignHeatRecoveryWaterFlowRate,d.get());
  // }

  // Not Implemented
  // Maximum Temperature for Heat Recovery
  // d = modelObject.maximumTemperatureforHeatRecovery();
  // if (d) {
  //   unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::MaximumTemperatureforHeatRecovery,d.get());
  // }

  // Not Implemented
  // Heat Recovery Water Inlet Node Name
  // if( boost::optional<ModelObject> mo = modelObject.waterInletModelObject() )
  // {
  //   if( boost::optional<Node> node = mo->optionalCast<Node>() )
  //   {
  //     unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::HeatRecoveryWaterInletNodeName,node->name().get());
  //   }
  // }

  // Not Implemented
  // Heat Recovery Water Outlet Node Name
  // if( boost::optional<ModelObject> mo = modelObject.waterOutletModelObject() )
  // {
  //   if( boost::optional<Node> node = mo->optionalCast<Node>() )
  //   {
  //     unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::HeatRecoveryWaterOutletNodeName,node->name().get());
  //   }
  // }

  // Not Implemented
  // Design Specification Multispeed Heat Pump Object Type
  // Design Specification Multispeed Heat Pump Object Name
  // if( boost::optional<HVACComponent> designSpecificationMultispeedHeatPumpObject = modelObject.designSpecificationMultispeedHeatPumpObject() )
  // {
  //   boost::optional<IdfObject> _designSpecificationMultispeedHeatPumpObject = translateAndMapModelObject(designSpecificationMultispeedHeatPumpObject.get());

  //   if( _designSpecificationMultispeedHeatPumpObject && _designSpecificationMultispeedHeatPumpObject->name() )
  //   {
  //     unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedHeatPumpObjectType,_designSpecificationMultispeedHeatPumpObject->iddObject().name());
  //     unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedHeatPumpObjectName,_designSpecificationMultispeedHeatPumpObject->name().get());
  //   }
  // }

  // Fill in node names for inner components
  if( !airInletNodeName || !airOutletNodeName ) {
    LOG(Error,modelObject.briefDescription() << ": Object not attached to AirLoopHVAC");
    return boost::none;
  }

  std::string baseName = modelObject.name().get();

  if( _fan )
  {
    std::string outletNodeName;
    std::string inletNodeName = airInletNodeName.get();
    if( blowThroughFan ) {
      if( _coolingCoil ) {
        outletNodeName = baseName + " Fan - Cooling Coil Node";
      } else if( _heatingCoil ) {
        outletNodeName = baseName + " Fan - Heating Coil Node";
      } else if( _supplementalHeatingCoil ) {
        outletNodeName = baseName + " Fan - Supplemental Coil Node";
      } else {
        outletNodeName = airOutletNodeName.get();
      }
    } else {
      if( _heatingCoil ) {
        inletNodeName = baseName + " Heating Coil - Fan Node";
      } else if( _coolingCoil ) {
        inletNodeName = baseName + " Cooling Coil - Fan Node";
      }
      if( _supplementalHeatingCoil ) {
        outletNodeName = baseName + " Fan - Supplemental Coil Node";
      } else {
        outletNodeName = airOutletNodeName.get();
      }
    }

    if( _fan->iddObject().type() == IddObjectType::Fan_ConstantVolume )
    {
      _fan->setString(Fan_ConstantVolumeFields::AirInletNodeName,inletNodeName);
      _fan->setString(Fan_ConstantVolumeFields::AirOutletNodeName,outletNodeName);
    }
    else if( _fan->iddObject().type() == IddObjectType::Fan_VariableVolume )
    {
      _fan->setString(Fan_VariableVolumeFields::AirInletNodeName,inletNodeName);
      _fan->setString(Fan_VariableVolumeFields::AirOutletNodeName,outletNodeName);
    }
    else if( _fan->iddObject().type() == IddObjectType::Fan_OnOff )
    {
      _fan->setString(Fan_OnOffFields::AirInletNodeName,inletNodeName);
      _fan->setString(Fan_OnOffFields::AirOutletNodeName,outletNodeName);
    }
  }

  if( _coolingCoil )
  {
    std::string outletNodeName;
    std::string inletNodeName;
    if( blowThroughFan && _fan ) {
      inletNodeName = baseName + " Fan - Cooling Coil Node";
    } else {
      inletNodeName = airInletNodeName.get();
    }
    if( _heatingCoil ) {
      outletNodeName = baseName + " Cooling Coil - Heating Coil Node";
    } else if( blowThroughFan && _supplementalHeatingCoil ) {
      outletNodeName = baseName + " Cooling Coil - Supplemental Coil Node";
    } else if( !blowThroughFan && _fan ) {
      outletNodeName = baseName + " Cooling Coil - Fan Node";
    } else {
      outletNodeName = airOutletNodeName.get();
    }

    if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed )
    {
      _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName,inletNodeName);
      _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName,outletNodeName);
    }
    else if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_TwoSpeed )
    {
      _coolingCoil->setString(Coil_Cooling_DX_TwoSpeedFields::AirInletNodeName,inletNodeName);
      _coolingCoil->setString(Coil_Cooling_DX_TwoSpeedFields::AirOutletNodeName,outletNodeName);
    }
    else if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_Water )
    {
      _coolingCoil->setString(Coil_Cooling_WaterFields::AirInletNodeName,inletNodeName);
      _coolingCoil->setString(Coil_Cooling_WaterFields::AirOutletNodeName,outletNodeName);
    }
    else if( _coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit )
    {
      _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName,inletNodeName);
      _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName,outletNodeName);
    }
  }

  if( _heatingCoil )
  {
    std::string outletNodeName;
    std::string inletNodeName;
    if( _coolingCoil ) {
      inletNodeName = baseName + " Cooling Coil - Heating Coil Node";
    } else if( blowThroughFan && _fan ) {
      inletNodeName = baseName + " Fan - Heating Coil Node";
    } else {
      inletNodeName = airInletNodeName.get();
    }
    if( blowThroughFan && _supplementalHeatingCoil ) {
      outletNodeName = baseName + " Heating Coil - Supplemental Coil Node";
    } else if( !blowThroughFan && _fan ) {
      outletNodeName = baseName + " Heating Coil - Fan Node";
    } else {
      outletNodeName = airOutletNodeName.get();
    }

    if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_SingleSpeed )
    {
      _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName,inletNodeName);
      _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName,outletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
    {
      _heatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,inletNodeName);
      _heatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,outletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas )
    {
      _heatingCoil->setString(Coil_Heating_GasFields::AirInletNodeName,inletNodeName);
      _heatingCoil->setString(Coil_Heating_GasFields::AirOutletNodeName,outletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
    {
      _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,inletNodeName);
      _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,outletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Desuperheater )
    {
      _heatingCoil->setString(Coil_Heating_DesuperheaterFields::AirInletNodeName,inletNodeName);
      _heatingCoil->setString(Coil_Heating_DesuperheaterFields::AirOutletNodeName,outletNodeName);
    }
    else if( _heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit )
    {
      _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName,inletNodeName);
      _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName,outletNodeName);
    }
  }

  if( _supplementalHeatingCoil )
  {
    std::string outletNodeName;
    std::string inletNodeName;
    if( !blowThroughFan && _fan ) {
      inletNodeName = baseName + " Fan - Supplemental Coil Node";
    } else if( _heatingCoil ) {
      inletNodeName = baseName + " Heating Coil - Supplemental Coil Node";
    } else if( _coolingCoil ) {
      inletNodeName = baseName + " Cooling Coil - Supplemental Coil Node";
    } else if( _fan ) {
      inletNodeName = baseName + " Fan - Supplemental Coil Node";
    } else {
      inletNodeName = airInletNodeName.get();
    }

    if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric )
    {
      _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName,inletNodeName);
      _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName,airOutletNodeName.get());
    }
    else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas )
    {
      _supplementalHeatingCoil->setString(Coil_Heating_GasFields::AirInletNodeName,inletNodeName);
      _supplementalHeatingCoil->setString(Coil_Heating_GasFields::AirOutletNodeName,airOutletNodeName.get());
    }
    else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water )
    {
      _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName,inletNodeName);
      _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName,airOutletNodeName.get());
    }
    else if( _supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Desuperheater )
    {
      _supplementalHeatingCoil->setString(Coil_Heating_DesuperheaterFields::AirInletNodeName,inletNodeName);
      _supplementalHeatingCoil->setString(Coil_Heating_DesuperheaterFields::AirOutletNodeName,airOutletNodeName.get());
    }
  }

  return unitarySystem;
}

} // energyplus

} // openstudio

