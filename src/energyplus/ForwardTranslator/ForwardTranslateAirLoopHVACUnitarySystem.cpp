/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/AirLoopHVACUnitarySystem_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/SetpointManagerMixedAir.hpp"
#include "../../model/SetpointManagerMixedAir_Impl.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem.hpp"
#include "../../model/AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "../../model/CoilCoolingDXMultiSpeedStageData.hpp"
#include "../../model/CoilCoolingDXMultiSpeedStageData_Impl.hpp"
#include "../../model/CoilCoolingDXVariableSpeedSpeedData.hpp"
#include "../../model/CoilCoolingDXVariableSpeedSpeedData_Impl.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"
#include "../../model/CoilHeatingDXMultiSpeedStageData.hpp"
#include "../../model/CoilHeatingDXMultiSpeedStageData_Impl.hpp"
#include "../../model/CoilHeatingDXVariableSpeedSpeedData.hpp"
#include "../../model/CoilHeatingDXVariableSpeedSpeedData_Impl.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit_Impl.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData_Impl.hpp"
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "../../model/CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "../../model/CoilSystemCoolingWaterHeatExchangerAssisted.hpp"
#include "../../model/CoilSystemCoolingWaterHeatExchangerAssisted_Impl.hpp"
#include "../../model/AirToAirComponent.hpp"
#include "../../model/AirToAirComponent_Impl.hpp"
#include "../../model/CoilHeatingDXMultiSpeed.hpp"
#include "../../model/CoilHeatingDXMultiSpeed_Impl.hpp"
#include "../../model/CoilHeatingDXVariableSpeed.hpp"
#include "../../model/CoilHeatingDXVariableSpeed_Impl.hpp"
#include "../../model/CoilHeatingGasMultiStage.hpp"
#include "../../model/CoilHeatingGasMultiStage_Impl.hpp"
#include "../../model/CoilHeatingGasMultiStageStageData.hpp"
#include "../../model/CoilHeatingGasMultiStageStageData_Impl.hpp"
#include "../../model/CoilCoolingDX.hpp"
#include "../../model/CoilCoolingDX_Impl.hpp"
#include "../../model/CoilCoolingDXMultiSpeed.hpp"
#include "../../model/CoilCoolingDXMultiSpeed_Impl.hpp"
#include "../../model/CoilCoolingDXVariableSpeed.hpp"
#include "../../model/CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../../model/UnitarySystemPerformanceMultispeed.hpp"
#include "../../model/UnitarySystemPerformanceMultispeed_Impl.hpp"
#include <utilities/idd/AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_ThermalStorage_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_TwoSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_TwoStageWithHumidityControlMode_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_MultiSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_MultiStage_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_MultiStage_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_AirToAir_SensibleAndLatent_FieldEnums.hxx>
#include <utilities/idd/HeatExchanger_Desiccant_BalancedFlow_FieldEnums.hxx>
#include <utilities/idd/Fan_ComponentModel_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_VariableVolume_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_MixedAir_FieldEnums.hxx>
#include <utilities/idd/UnitarySystemPerformance_Multispeed_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACUnitarySystem(AirLoopHVACUnitarySystem& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    // Name
    IdfObject unitarySystem = createRegisterAndNameIdfObject(openstudio::IddObjectType::AirLoopHVAC_UnitarySystem, modelObject);

    // Control Type
    s = modelObject.controlType();
    if (s) {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::ControlType, s.get());
    }

    // Controlling Zone or Thermostat Location
    if (boost::optional<ThermalZone> controllingZoneorThermostatLocation = modelObject.controllingZoneorThermostatLocation()) {
      boost::optional<IdfObject> _controllingZoneorThermostatLocation = translateAndMapModelObject(controllingZoneorThermostatLocation.get());

      if (_controllingZoneorThermostatLocation && _controllingZoneorThermostatLocation->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::ControllingZoneorThermostatLocation,
                                _controllingZoneorThermostatLocation->name().get());
      }
    }

    // Dehumidification Control Type
    s = modelObject.dehumidificationControlType();
    if (s) {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::DehumidificationControlType, s.get());
    }

    // Availability Schedule Name
    if (boost::optional<Schedule> availabilitySchedule = modelObject.availabilitySchedule()) {
      boost::optional<IdfObject> _availabilitySchedule = translateAndMapModelObject(availabilitySchedule.get());

      if (_availabilitySchedule && _availabilitySchedule->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::AvailabilityScheduleName, _availabilitySchedule->name().get());
      }
    }

    boost::optional<std::string> airInletNodeName;
    boost::optional<std::string> airOutletNodeName;

    // Air Inlet Node Name
    if (boost::optional<Node> node = modelObject.inletNode()) {
      airInletNodeName = node->name().get();
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::AirInletNodeName, node->name().get());
    }

    // Air Outlet Node Name
    if (boost::optional<Node> node = modelObject.outletNode()) {
      airOutletNodeName = node->name().get();
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::AirOutletNodeName, node->name().get());
    }

    // Supply Fan Object Type
    // Supply Fan Name
    boost::optional<IdfObject> _fan;
    if (boost::optional<HVACComponent> supplyFan = modelObject.supplyFan()) {
      _fan = translateAndMapModelObject(supplyFan.get());

      if (_fan && _fan->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyFanObjectType, _fan->iddObject().name());
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyFanName, _fan->name().get());
      }
    }

    // Fan Placement
    s = modelObject.fanPlacement();
    bool blowThroughFan = false;
    if (s) {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::FanPlacement, s.get());
      if (istringEqual(*s, "BlowThrough")) {
        blowThroughFan = true;
      }
    } else if (_fan) {
      // You must set one, or E+ will crash.
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::FanPlacement, "DrawThrough");
    }

    // Supply Air Fan Operating Mode Schedule Name
    if (boost::optional<Schedule> supplyAirFanOperatingModeSchedule = modelObject.supplyAirFanOperatingModeSchedule()) {
      boost::optional<IdfObject> _supplyAirFanOperatingModeSchedule = translateAndMapModelObject(supplyAirFanOperatingModeSchedule.get());

      if (_supplyAirFanOperatingModeSchedule && _supplyAirFanOperatingModeSchedule->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplyAirFanOperatingModeScheduleName,
                                _supplyAirFanOperatingModeSchedule->name().get());
      }
    }

    // Heating Coil Object Type
    // Heating Coil Name
    boost::optional<IdfObject> _heatingCoil;
    auto heatingCoil = modelObject.heatingCoil();
    if (heatingCoil) {
      _heatingCoil = translateAndMapModelObject(heatingCoil.get());

      if (_heatingCoil && _heatingCoil->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::HeatingCoilObjectType, _heatingCoil->iddObject().name());
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::HeatingCoilName, _heatingCoil->name().get());
      }

      // Supply Air Flow Rate Method During Heating Operation
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::HeatingSupplyAirFlowRateMethod,
                              modelObject.supplyAirFlowRateMethodDuringHeatingOperation());

      // Supply Air Flow Rate During Heating Operation
      if (modelObject.isSupplyAirFlowRateDuringHeatingOperationAutosized()) {
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::HeatingSupplyAirFlowRate, "Autosize");
      } else if (auto val_ = modelObject.supplyAirFlowRateDuringHeatingOperation()) {
        unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::HeatingSupplyAirFlowRate, val_.get());
      }

      // Supply Air Flow Rate Per Floor Area during Heating Operation
      if (auto val_ = modelObject.supplyAirFlowRatePerFloorAreaduringHeatingOperation()) {
        unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::HeatingSupplyAirFlowRatePerFloorArea, val_.get());
      }

      // Fraction of Autosized Design Heating Supply Air Flow Rate
      if (auto val_ = modelObject.fractionofAutosizedDesignHeatingSupplyAirFlowRate()) {
        unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::HeatingFractionofAutosizedHeatingSupplyAirFlowRate, val_.get());
      }

      // Design Supply Air Flow Rate Per Unit of Capacity During Heating Operation
      if (auto val_ = modelObject.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation()) {
        unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::HeatingSupplyAirFlowRatePerUnitofCapacity, val_.get());
      }
    } else {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::HeatingSupplyAirFlowRateMethod, "None");
    }

    // DX Heating Coil Sizing Ratio
    d = modelObject.dXHeatingCoilSizingRatio();
    if (d) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::DXHeatingCoilSizingRatio, d.get());
    }

    // Cooling Coil Object Type
    // Cooling Coil Name
    boost::optional<IdfObject> _coolingCoil;
    auto coolingCoil = modelObject.coolingCoil();

    if (coolingCoil) {
      _coolingCoil = translateAndMapModelObject(coolingCoil.get());

      if (_coolingCoil && _coolingCoil->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::CoolingCoilObjectType, _coolingCoil->iddObject().name());
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::CoolingCoilName, _coolingCoil->name().get());
      }

      // Supply Air Flow Rate Method During Cooling Operation
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::CoolingSupplyAirFlowRateMethod,
                              modelObject.supplyAirFlowRateMethodDuringCoolingOperation());

      // Supply Air Flow Rate During Cooling Operation
      if (modelObject.isSupplyAirFlowRateDuringCoolingOperationAutosized()) {
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::CoolingSupplyAirFlowRate, "Autosize");
      } else if (auto val_ = modelObject.supplyAirFlowRateDuringCoolingOperation()) {
        unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::CoolingSupplyAirFlowRate, val_.get());
      }

      // Supply Air Flow Rate Per Floor Area During Cooling Operation
      if (auto val_ = modelObject.supplyAirFlowRatePerFloorAreaDuringCoolingOperation()) {
        unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::CoolingSupplyAirFlowRatePerFloorArea, val_.get());
      }

      // Fraction of Autosized Design Cooling Supply Air Flow Rate
      if (auto val_ = modelObject.fractionofAutosizedDesignCoolingSupplyAirFlowRate()) {
        unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::CoolingFractionofAutosizedCoolingSupplyAirFlowRate, val_.get());
      }

      // Design Supply Air Flow Rate Per Unit of Capacity During Cooling Operation
      if (auto val_ = modelObject.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation()) {
        unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::CoolingSupplyAirFlowRatePerUnitofCapacity, val_.get());
      }
    } else {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::CoolingSupplyAirFlowRateMethod, "None");
    }

    // Use DOAS DX Cooling Coil
    if (modelObject.useDOASDXCoolingCoil()) {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil, "Yes");
    } else {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::UseDOASDXCoolingCoil, "No");
    }

    // DOAS DX Cooling Coil Leaving Minimum Air Temperature
    if (modelObject.isDOASDXCoolingCoilLeavingMinimumAirTemperatureAutosized()) {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::MinimumSupplyAirTemperature, "Autosize");
    } else if ((d = modelObject.dOASDXCoolingCoilLeavingMinimumAirTemperature())) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::MinimumSupplyAirTemperature, d.get());
    }

    // Latent Load Control
    s = modelObject.latentLoadControl();
    if (s) {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::LatentLoadControl, s.get());
    }

    // Supplemental Heating Coil Object Type
    // Supplemental Heating Coil Name
    boost::optional<IdfObject> _supplementalHeatingCoil;

    if (boost::optional<HVACComponent> supplementalHeatingCoil = modelObject.supplementalHeatingCoil()) {
      _supplementalHeatingCoil = translateAndMapModelObject(supplementalHeatingCoil.get());

      if (_supplementalHeatingCoil && _supplementalHeatingCoil->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilObjectType, _supplementalHeatingCoil->iddObject().name());
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::SupplementalHeatingCoilName, _supplementalHeatingCoil->name().get());
      }
    }

    // Supply Air Flow Rate Method When No Cooling or Heating is Required
    unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::NoLoadSupplyAirFlowRateMethod,
                            modelObject.supplyAirFlowRateMethodWhenNoCoolingorHeatingisRequired());

    // Supply Air Flow Rate When No Cooling or Heating is Required
    if (modelObject.isSupplyAirFlowRateWhenNoCoolingorHeatingisRequiredAutosized()) {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::NoLoadSupplyAirFlowRate, "Autosize");
    } else if ((d = modelObject.supplyAirFlowRateWhenNoCoolingorHeatingisRequired())) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::NoLoadSupplyAirFlowRate, d.get());
    }

    // Supply Air Flow Rate Per Floor Area When No Cooling or Heating is Required
    d = modelObject.supplyAirFlowRatePerFloorAreaWhenNoCoolingorHeatingisRequired();
    if (d) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::NoLoadSupplyAirFlowRatePerFloorArea, d.get());
    }

    // Fraction of Autosized Design Cooling Supply Air Flow Rate When No Cooling or Heating is Required
    d = modelObject.fractionofAutosizedDesignCoolingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
    if (d) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::NoLoadFractionofAutosizedCoolingSupplyAirFlowRate, d.get());
    }

    // Fraction of Autosized Design Heating Supply Air Flow Rate When No Cooling or Heating is Required
    d = modelObject.fractionofAutosizedDesignHeatingSupplyAirFlowRateWhenNoCoolingorHeatingisRequired();
    if (d) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::NoLoadFractionofAutosizedHeatingSupplyAirFlowRate, d.get());
    }

    // Design Supply Air Flow Rate Per Unit of Capacity During Cooling Operation When No Cooling or Heating is Required
    d = modelObject.designSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperationWhenNoCoolingorHeatingisRequired();
    if (d) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::NoLoadSupplyAirFlowRatePerUnitofCapacityDuringCoolingOperation, d.get());
    }

    // Design Supply Air Flow Rate Per Unit of Capacity During Heating Operation When No Cooling or Heating is Required
    d = modelObject.designSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperationWhenNoCoolingorHeatingisRequired();
    if (d) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::NoLoadSupplyAirFlowRatePerUnitofCapacityDuringHeatingOperation, d.get());
    }

    // No Load Supply Air Flow Rate Control Set To Low Speed
    if (modelObject.noLoadSupplyAirFlowRateControlSetToLowSpeed()) {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::NoLoadSupplyAirFlowRateControlSetToLowSpeed, "Yes");
    } else {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::NoLoadSupplyAirFlowRateControlSetToLowSpeed, "No");
    }

    // Maximum Supply Air Temperature
    if (modelObject.isMaximumSupplyAirTemperatureAutosized()) {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature, "Autosize");
    } else if ((d = modelObject.maximumSupplyAirTemperature())) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::MaximumSupplyAirTemperature, d.get());
    }

    // Maximum Outdoor Dry-Bulb Temperature for Supplemental Heater Operation
    d = modelObject.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation();
    if (d) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation, d.get());
    }

    // Outdoor Dry-Bulb Temperature Sensor Node Name
    s = modelObject.outdoorDryBulbTemperatureSensorNodeName();
    if (s) {
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::OutdoorDryBulbTemperatureSensorNodeName, s.get());
    }

    // Ancilliary On-Cycle Electric Power
    d = modelObject.ancilliaryOnCycleElectricPower();
    if (d) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::AncillaryOnCycleElectricPower, d.get());
    }

    // Ancilliary Off-Cycle Electric Power
    d = modelObject.ancilliaryOffCycleElectricPower();
    if (d) {
      unitarySystem.setDouble(AirLoopHVAC_UnitarySystemFields::AncillaryOffCycleElectricPower, d.get());
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

    // Design Specification Multispeed Object Name
    if (boost::optional<UnitarySystemPerformanceMultispeed> designSpecificationMultispeedObject = modelObject.designSpecificationMultispeedObject()) {
      boost::optional<IdfObject> _unitarySystemPerformance = translateAndMapModelObject(designSpecificationMultispeedObject.get());

      if (_unitarySystemPerformance && _unitarySystemPerformance->name()) {
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectType,
                                _unitarySystemPerformance->iddObject().name());
        unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName, _unitarySystemPerformance->name().get());
      }
      // If it doesn't have one hard set, we check if there's at least one coil that should have speeds
    } else if ((coolingCoil
                && ((coolingCoil->iddObjectType() == model::CoilCoolingDXMultiSpeed::iddObjectType())
                    || (coolingCoil->iddObjectType() == model::CoilCoolingDXVariableSpeed::iddObjectType())
                    || (coolingCoil->iddObjectType() == model::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType())))
               || (heatingCoil
                   && ((heatingCoil->iddObjectType() == model::CoilHeatingDXMultiSpeed::iddObjectType())
                       || (heatingCoil->iddObjectType() == model::CoilHeatingDXVariableSpeed::iddObjectType())
                       || (heatingCoil->iddObjectType() == model::CoilHeatingGasMultiStage::iddObjectType())
                       || (heatingCoil->iddObjectType() == model::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit::iddObjectType())))) {

      // If not user specified, then generate the UnitarySystemPerformance:Multispeed used for multi speed coils

      IdfObject _unitarySystemPerformance(openstudio::IddObjectType::UnitarySystemPerformance_Multispeed);
      m_idfObjects.push_back(_unitarySystemPerformance);
      _unitarySystemPerformance.setName(unitarySystem.nameString() + " Unitary System Performance");

      boost::optional<model::CoilHeatingDXMultiSpeed> multispeedDXHeating;
      boost::optional<model::CoilHeatingDXVariableSpeed> varSpeedDXHeating;
      boost::optional<model::CoilHeatingGasMultiStage> multistageGasHeating;
      boost::optional<model::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit> varSpeedWaterToAirHeating;
      boost::optional<model::CoilCoolingDXMultiSpeed> multispeedDXCooling;
      boost::optional<model::CoilCoolingDXVariableSpeed> varSpeedDXCooling;
      boost::optional<model::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit> varSpeedWaterToAirCooling;

      int maxHeatingStages = 0;
      int maxCoolingStages = 0;

      if (heatingCoil) {
        multispeedDXHeating = heatingCoil->optionalCast<model::CoilHeatingDXMultiSpeed>();
        multistageGasHeating = heatingCoil->optionalCast<model::CoilHeatingGasMultiStage>();
        varSpeedDXHeating = heatingCoil->optionalCast<model::CoilHeatingDXVariableSpeed>();
        varSpeedWaterToAirHeating = heatingCoil->optionalCast<model::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit>();
      }

      if (coolingCoil) {
        multispeedDXCooling = coolingCoil->optionalCast<model::CoilCoolingDXMultiSpeed>();
        varSpeedDXCooling = coolingCoil->optionalCast<model::CoilCoolingDXVariableSpeed>();
        varSpeedWaterToAirCooling = coolingCoil->optionalCast<model::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>();
      }

      std::vector<model::CoilHeatingDXMultiSpeedStageData> heatingStages;
      std::vector<model::CoilHeatingDXVariableSpeedSpeedData> varHeatingStages;
      std::vector<model::CoilHeatingGasMultiStageStageData> gasHeatingStages;
      std::vector<model::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> waterToAirHeatingStages;

      std::vector<model::CoilCoolingDXMultiSpeedStageData> coolingStages;
      std::vector<model::CoilCoolingDXVariableSpeedSpeedData> varCoolingStages;
      std::vector<model::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData> waterToAirCoolingStages;

      if (multispeedDXHeating) {
        heatingStages = multispeedDXHeating->stages();
        maxHeatingStages = heatingStages.size();
        _unitarySystemPerformance.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, maxHeatingStages);
      } else if (varSpeedDXHeating) {
        varHeatingStages = varSpeedDXHeating->speeds();
        maxHeatingStages = varHeatingStages.size();
        _unitarySystemPerformance.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, maxHeatingStages);
      } else if (multistageGasHeating) {
        gasHeatingStages = multistageGasHeating->stages();
        maxHeatingStages = gasHeatingStages.size();
        _unitarySystemPerformance.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, maxHeatingStages);
      } else if (varSpeedWaterToAirHeating) {
        waterToAirHeatingStages = varSpeedWaterToAirHeating->speeds();
        maxHeatingStages = waterToAirHeatingStages.size();
        _unitarySystemPerformance.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, maxHeatingStages);
      } else {
        _unitarySystemPerformance.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating, 1);
      }

      if (multispeedDXCooling) {
        coolingStages = multispeedDXCooling->stages();
        maxCoolingStages = coolingStages.size();
        _unitarySystemPerformance.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, maxCoolingStages);
      } else if (varSpeedDXCooling) {
        varCoolingStages = varSpeedDXCooling->speeds();
        maxCoolingStages = varCoolingStages.size();
        _unitarySystemPerformance.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, maxCoolingStages);
      } else if (varSpeedWaterToAirCooling) {
        waterToAirCoolingStages = varSpeedWaterToAirCooling->speeds();
        maxCoolingStages = waterToAirCoolingStages.size();
        _unitarySystemPerformance.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, maxCoolingStages);
      } else {
        _unitarySystemPerformance.setInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling, 1);
      }

      _unitarySystemPerformance.setString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation, "No");

      auto heatingFlow = modelObject.supplyAirFlowRateDuringHeatingOperation();
      auto coolingFlow = modelObject.supplyAirFlowRateDuringCoolingOperation();

      double totalGasHeatingCap = 0;
      for (const auto& gasStage : gasHeatingStages) {
        auto cap = gasStage.nominalCapacity();
        if (cap) {
          totalGasHeatingCap += cap.get();
        }
      };

      for (int i = 0; i < std::max(maxHeatingStages, maxCoolingStages); ++i) {
        auto extensible = _unitarySystemPerformance.pushExtensibleGroup();

        if (static_cast<unsigned>(i) < heatingStages.size()) {
          auto heatingStage = heatingStages[i];
          auto stageFlow = heatingStage.ratedAirFlowRate();
          if (stageFlow && heatingFlow) {
            extensible.setDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio,
                                 stageFlow.get() / heatingFlow.get());
          } else {
            extensible.setString(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio, "Autosize");
          }
        } else if (static_cast<unsigned>(i) < varHeatingStages.size()) {
          auto varHeatingStage = varHeatingStages[i];
          double stageFlow = varHeatingStage.referenceUnitRatedAirFlowRate();
          if (heatingFlow) {
            extensible.setDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio, stageFlow / heatingFlow.get());
          } else {
            extensible.setString(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio, "Autosize");
          }
        } else if (static_cast<unsigned>(i) < gasHeatingStages.size()) {
          auto gasHeatingStage = gasHeatingStages[i];
          auto stageCap = gasHeatingStage.nominalCapacity();
          if (stageCap) {
            extensible.setDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio,
                                 stageCap.get() / totalGasHeatingCap);
          } else {
            extensible.setString(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio, "Autosize");
          }
        } else if (static_cast<unsigned>(i) < waterToAirHeatingStages.size()) {
          auto waterToAirHeatingStage = waterToAirHeatingStages[i];
          double stageFlow = waterToAirHeatingStage.referenceUnitRatedAirFlow();
          if (heatingFlow) {
            extensible.setDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio, stageFlow / heatingFlow.get());
          } else {
            extensible.setString(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio, "Autosize");
          }
        } else {
          extensible.setDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio, 1.0);
        }

        if (static_cast<unsigned>(i) < coolingStages.size()) {
          auto coolingStage = coolingStages[i];
          auto stageFlow = coolingStage.ratedAirFlowRate();
          if (stageFlow && coolingFlow) {
            extensible.setDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio,
                                 stageFlow.get() / coolingFlow.get());
          } else {
            extensible.setString(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio, "Autosize");
          }
        } else if (static_cast<unsigned>(i) < varCoolingStages.size()) {
          auto varCoolingStage = varCoolingStages[i];
          double stageFlow = varCoolingStage.referenceUnitRatedAirFlowRate();
          if (coolingFlow) {
            extensible.setDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio, stageFlow / coolingFlow.get());
          } else {
            extensible.setString(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio, "Autosize");
          }
        } else if (static_cast<unsigned>(i) < waterToAirCoolingStages.size()) {
          auto waterToAirCoolingStage = waterToAirCoolingStages[i];
          double stageFlow = waterToAirCoolingStage.referenceUnitRatedAirFlowRate();
          if (coolingFlow) {
            extensible.setDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio, stageFlow / coolingFlow.get());
          } else {
            extensible.setString(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio, "Autosize");
          }
        } else {
          extensible.setDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio, 1.0);
        }
      }

      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectType, _unitarySystemPerformance.iddObject().name());
      unitarySystem.setString(AirLoopHVAC_UnitarySystemFields::DesignSpecificationMultispeedObjectName, _unitarySystemPerformance.nameString());
    }

    // Fill in node names for inner components
    if (!airInletNodeName || !airOutletNodeName) {
      LOG(Error, modelObject.briefDescription() << ": Object not attached to AirLoopHVAC");
      return boost::none;
    }

    std::string baseName = modelObject.name().get();

    // Figure out node connections:
    // If BlowThrough:   o---- Fan --- CC ----  HC ---- SupplHC ----o
    // If DrawThrough:   o----  CC --- HC ---- Fan ---- SupplHC ----o
    // (All above components are optional, but at least one must be present)

    if (_fan) {
      std::string outletNodeName;
      std::string inletNodeName = airInletNodeName.get();
      if (blowThroughFan) {
        if (_coolingCoil) {
          outletNodeName = baseName + " Fan - Cooling Coil Node";
        } else if (_heatingCoil) {
          outletNodeName = baseName + " Fan - Heating Coil Node";
        } else if (_supplementalHeatingCoil) {
          outletNodeName = baseName + " Fan - Supplemental Coil Node";
        } else {
          outletNodeName = airOutletNodeName.get();
        }
      } else {
        if (_heatingCoil) {
          inletNodeName = baseName + " Heating Coil - Fan Node";
        } else if (_coolingCoil) {
          inletNodeName = baseName + " Cooling Coil - Fan Node";
        }
        if (_supplementalHeatingCoil) {
          outletNodeName = baseName + " Fan - Supplemental Coil Node";
        } else {
          outletNodeName = airOutletNodeName.get();
        }
      }

      if (_fan->iddObject().type() == IddObjectType::Fan_ConstantVolume) {
        _fan->setString(Fan_ConstantVolumeFields::AirInletNodeName, inletNodeName);
        _fan->setString(Fan_ConstantVolumeFields::AirOutletNodeName, outletNodeName);
      } else if (_fan->iddObject().type() == IddObjectType::Fan_VariableVolume) {
        _fan->setString(Fan_VariableVolumeFields::AirInletNodeName, inletNodeName);
        _fan->setString(Fan_VariableVolumeFields::AirOutletNodeName, outletNodeName);
      } else if (_fan->iddObject().type() == IddObjectType::Fan_OnOff) {
        _fan->setString(Fan_OnOffFields::AirInletNodeName, inletNodeName);
        _fan->setString(Fan_OnOffFields::AirOutletNodeName, outletNodeName);
      } else if (_fan->iddObject().type() == IddObjectType::Fan_SystemModel) {
        _fan->setString(Fan_SystemModelFields::AirInletNodeName, inletNodeName);
        _fan->setString(Fan_SystemModelFields::AirOutletNodeName, outletNodeName);
      } else if (_fan->iddObject().type() == IddObjectType::Fan_ComponentModel) {
        _fan->setString(Fan_ComponentModelFields::AirInletNodeName, inletNodeName);
        _fan->setString(Fan_ComponentModelFields::AirOutletNodeName, outletNodeName);
      }

      fixSPMsForUnitarySystem(modelObject, inletNodeName, outletNodeName);
    }

    if (_coolingCoil) {
      std::string outletNodeName;
      std::string inletNodeName;
      if (blowThroughFan && _fan) {
        inletNodeName = baseName + " Fan - Cooling Coil Node";
      } else {
        inletNodeName = airInletNodeName.get();
      }
      if (_heatingCoil) {
        outletNodeName = baseName + " Cooling Coil - Heating Coil Node";
      } else if (!blowThroughFan && _fan) {
        outletNodeName = baseName + " Cooling Coil - Fan Node";
      } else if (_supplementalHeatingCoil) {
        outletNodeName = baseName + " Cooling Coil - Supplemental Coil Node";
      } else {
        outletNodeName = airOutletNodeName.get();
      }

      if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed) {
        _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName, inletNodeName);
        _coolingCoil->setString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName, outletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_SingleSpeed_ThermalStorage) {
        _coolingCoil->setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirInletNodeName, inletNodeName);
        _coolingCoil->setString(Coil_Cooling_DX_SingleSpeed_ThermalStorageFields::EvaporatorAirOutletNodeName, outletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_TwoSpeed) {
        _coolingCoil->setString(Coil_Cooling_DX_TwoSpeedFields::AirInletNodeName, inletNodeName);
        _coolingCoil->setString(Coil_Cooling_DX_TwoSpeedFields::AirOutletNodeName, outletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_MultiSpeed) {
        _coolingCoil->setString(Coil_Cooling_DX_MultiSpeedFields::AirInletNodeName, inletNodeName);
        _coolingCoil->setString(Coil_Cooling_DX_MultiSpeedFields::AirOutletNodeName, outletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_VariableSpeed) {
        _coolingCoil->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirInletNodeName, inletNodeName);
        _coolingCoil->setString(Coil_Cooling_DX_VariableSpeedFields::IndoorAirOutletNodeName, outletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_Water) {
        _coolingCoil->setString(Coil_Cooling_WaterFields::AirInletNodeName, inletNodeName);
        _coolingCoil->setString(Coil_Cooling_WaterFields::AirOutletNodeName, outletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX_TwoStageWithHumidityControlMode) {
        _coolingCoil->setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirInletNodeName, inletNodeName);
        _coolingCoil->setString(Coil_Cooling_DX_TwoStageWithHumidityControlModeFields::AirOutletNodeName, outletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit) {
        _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName, inletNodeName);
        _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName, outletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit) {
        _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirInletNodeName, inletNodeName);
        _coolingCoil->setString(Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirOutletNodeName, outletNodeName);
      } else if (_coolingCoil->iddObject().type() == IddObjectType::CoilSystem_Cooling_DX_HeatExchangerAssisted) {
        OS_ASSERT(coolingCoil);
        auto coilSystem = coolingCoil->optionalCast<model::CoilSystemCoolingDXHeatExchangerAssisted>();
        OS_ASSERT(coilSystem);
        auto hx = coilSystem->heatExchanger();
        auto _hx = translateAndMapModelObject(hx);
        OS_ASSERT(_hx);
        if (_hx->iddObject().type() == IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent) {
          _hx->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNodeName, inletNodeName);
          _hx->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNodeName, outletNodeName);
        } else if (_hx->iddObject().type() == IddObjectType::HeatExchanger_Desiccant_BalancedFlow) {
          _hx->setString(HeatExchanger_Desiccant_BalancedFlowFields::RegenerationAirInletNodeName, inletNodeName);
          _hx->setString(HeatExchanger_Desiccant_BalancedFlowFields::ProcessAirOutletNodeName, outletNodeName);
        } else {
          LOG(Warn, modelObject.briefDescription() << ": Contains an unsupported type " << _hx->iddObject().type() << ".");
        }
      } else if (_coolingCoil->iddObject().type() == IddObjectType::CoilSystem_Cooling_Water_HeatExchangerAssisted) {
        OS_ASSERT(coolingCoil);
        auto coilSystem = coolingCoil->optionalCast<model::CoilSystemCoolingWaterHeatExchangerAssisted>();
        OS_ASSERT(coilSystem);
        auto hx = coilSystem->heatExchanger();
        auto _hx = translateAndMapModelObject(hx);
        OS_ASSERT(_hx);
        if (_hx->iddObject().type() == IddObjectType::HeatExchanger_AirToAir_SensibleAndLatent) {
          _hx->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::SupplyAirInletNodeName, inletNodeName);
          _hx->setString(HeatExchanger_AirToAir_SensibleAndLatentFields::ExhaustAirOutletNodeName, outletNodeName);
          // Note: HeatExchangerDesiccantBalancedFlow is not accepted at this time
        } else {
          LOG(Warn, modelObject.briefDescription() << ": Contains an unsupported type " << _hx->iddObject().type() << ".");
        }
      } else if (_coolingCoil->iddObject().type() == IddObjectType::Coil_Cooling_DX) {
        _coolingCoil->setString(Coil_Cooling_DXFields::EvaporatorInletNodeName, inletNodeName);
        _coolingCoil->setString(Coil_Cooling_DXFields::EvaporatorOutletNodeName, outletNodeName);
      } else {
        LOG(Warn, modelObject.briefDescription() << ": Contains an unsupported type " << _coolingCoil->iddObject().type() << ".");
      }
    }

    if (_heatingCoil) {
      std::string outletNodeName;
      std::string inletNodeName;
      if (_coolingCoil) {
        inletNodeName = baseName + " Cooling Coil - Heating Coil Node";
      } else if (blowThroughFan && _fan) {
        inletNodeName = baseName + " Fan - Heating Coil Node";
      } else {
        inletNodeName = airInletNodeName.get();
      }
      if (!blowThroughFan && _fan) {
        outletNodeName = baseName + " Heating Coil - Fan Node";
      } else if (_supplementalHeatingCoil) {
        outletNodeName = baseName + " Heating Coil - Supplemental Coil Node";
      } else {
        outletNodeName = airOutletNodeName.get();
      }

      if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_SingleSpeed) {
        _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName, inletNodeName);
        _heatingCoil->setString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName, outletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric) {
        _heatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName, inletNodeName);
        _heatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName, outletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric_MultiStage) {
        _heatingCoil->setString(Coil_Heating_Electric_MultiStageFields::AirInletNodeName, inletNodeName);
        _heatingCoil->setString(Coil_Heating_Electric_MultiStageFields::AirOutletNodeName, outletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel) {
        _heatingCoil->setString(Coil_Heating_FuelFields::AirInletNodeName, inletNodeName);
        _heatingCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName, outletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water) {
        _heatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName, inletNodeName);
        _heatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName, outletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_MultiSpeed) {
        _heatingCoil->setString(Coil_Heating_DX_MultiSpeedFields::AirInletNodeName, inletNodeName);
        _heatingCoil->setString(Coil_Heating_DX_MultiSpeedFields::AirOutletNodeName, outletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_DX_VariableSpeed) {
        _heatingCoil->setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirInletNodeName, inletNodeName);
        _heatingCoil->setString(Coil_Heating_DX_VariableSpeedFields::IndoorAirOutletNodeName, outletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Gas_MultiStage) {
        _heatingCoil->setString(Coil_Heating_Gas_MultiStageFields::AirInletNodeName, inletNodeName);
        _heatingCoil->setString(Coil_Heating_Gas_MultiStageFields::AirOutletNodeName, outletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Desuperheater) {
        _heatingCoil->setString(Coil_Heating_DesuperheaterFields::AirInletNodeName, inletNodeName);
        _heatingCoil->setString(Coil_Heating_DesuperheaterFields::AirOutletNodeName, outletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_WaterToAirHeatPump_EquationFit) {
        _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirInletNodeName, inletNodeName);
        _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName, outletNodeName);
      } else if (_heatingCoil->iddObject().type() == IddObjectType::Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit) {
        _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirInletNodeName, inletNodeName);
        _heatingCoil->setString(Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFitFields::IndoorAirOutletNodeName, outletNodeName);
      } else {
        LOG(Warn, modelObject.briefDescription() << ": Contains an unsupported type " << _heatingCoil->iddObject().type() << ".");
      }
    }

    if (_supplementalHeatingCoil) {
      std::string outletNodeName;
      std::string inletNodeName;
      if (!blowThroughFan && _fan) {
        inletNodeName = baseName + " Fan - Supplemental Coil Node";
      } else if (_heatingCoil) {
        inletNodeName = baseName + " Heating Coil - Supplemental Coil Node";
      } else if (_coolingCoil) {
        inletNodeName = baseName + " Cooling Coil - Supplemental Coil Node";
      } else if (_fan) {
        inletNodeName = baseName + " Fan - Supplemental Coil Node";
      } else {
        inletNodeName = airInletNodeName.get();
      }

      if (_supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric) {
        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName, inletNodeName);
        _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName, airOutletNodeName.get());
      } else if (_supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric_MultiStage) {
        _supplementalHeatingCoil->setString(Coil_Heating_Electric_MultiStageFields::AirInletNodeName, inletNodeName);
        _supplementalHeatingCoil->setString(Coil_Heating_Electric_MultiStageFields::AirOutletNodeName, airOutletNodeName.get());
      } else if (_supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel) {
        _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirInletNodeName, inletNodeName);
        _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName, airOutletNodeName.get());
      } else if (_supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water) {
        _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName, inletNodeName);
        _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName, airOutletNodeName.get());
      } else if (_supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Desuperheater) {
        _supplementalHeatingCoil->setString(Coil_Heating_DesuperheaterFields::AirInletNodeName, inletNodeName);
        _supplementalHeatingCoil->setString(Coil_Heating_DesuperheaterFields::AirOutletNodeName, airOutletNodeName.get());
      } else {
        LOG(Warn, modelObject.briefDescription() << ": Contains an unsupported type " << _supplementalHeatingCoil->iddObject().type() << ".");
      }
    }

    return unitarySystem;
  }

}  // namespace energyplus

}  // namespace openstudio
