#ifndef MODEL_HVAC_I
#define MODEL_HVAC_I

#ifdef SWIGPYTHON
  %module openstudiomodelhvac
#endif


%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>
%import <model/ModelSimulation.i>
%import <model/ModelResources.i>
%import <model/ModelGeometry.i>

%{
  #include <model/HVACTemplates.hpp>
%}

#if defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelHVAC
  
#endif

MODELOBJECT_TEMPLATES(PortList);
MODELOBJECT_TEMPLATES(Loop);
MODELOBJECT_TEMPLATES(HVACComponent);
MODELOBJECT_TEMPLATES(ZoneHVACComponent);
MODELOBJECT_TEMPLATES(ZoneHVACEquipmentList);
MODELOBJECT_TEMPLATES(Thermostat);
MODELOBJECT_TEMPLATES(SetpointManager);
MODELOBJECT_TEMPLATES(StraightComponent);
MODELOBJECT_TEMPLATES(WaterToAirComponent);
MODELOBJECT_TEMPLATES(WaterToWaterComponent);
MODELOBJECT_TEMPLATES(AirToAirComponent);
MODELOBJECT_TEMPLATES(Node);
MODELOBJECT_TEMPLATES(SizingZone);
MODELOBJECT_TEMPLATES(SizingSystem);
MODELOBJECT_TEMPLATES(ThermalZone);
MODELOBJECT_TEMPLATES(ThermostatSetpointDualSetpoint);
MODELOBJECT_TEMPLATES(ZoneControlHumidistat);
MODELOBJECT_TEMPLATES(ZoneControlThermostatStagedDualSetpoint);
MODELOBJECT_TEMPLATES(AirConditionerVariableRefrigerantFlow);
MODELOBJECT_TEMPLATES(AirLoopHVAC);
MODELOBJECT_TEMPLATES(SizingPlant);
MODELOBJECT_TEMPLATES(PlantLoop);
MODELOBJECT_TEMPLATES(AirLoopHVACOutdoorAirSystem);
MODELOBJECT_TEMPLATES(ControllerMechanicalVentilation);
MODELOBJECT_TEMPLATES(ControllerOutdoorAir);
MODELOBJECT_TEMPLATES(ControllerWaterCoil);
MODELOBJECT_TEMPLATES(Mixer);
MODELOBJECT_TEMPLATES(AirLoopHVACReturnPlenum);
MODELOBJECT_TEMPLATES(AirLoopHVACZoneMixer);
MODELOBJECT_TEMPLATES(ConnectorMixer);
MODELOBJECT_TEMPLATES(AvailabilityManagerNightCycle);
MODELOBJECT_TEMPLATES(AvailabilityManagerScheduled);
MODELOBJECT_TEMPLATES(SetpointManagerFollowOutdoorAirTemperature);
MODELOBJECT_TEMPLATES(SetpointManagerMixedAir);
MODELOBJECT_TEMPLATES(SetpointManagerMultiZoneHumidityMinimum);
MODELOBJECT_TEMPLATES(SetpointManagerMultiZoneMinimumHumidityAverage);
MODELOBJECT_TEMPLATES(SetpointManagerOutdoorAirPretreat);
MODELOBJECT_TEMPLATES(SetpointManagerOutdoorAirReset);
MODELOBJECT_TEMPLATES(SetpointManagerScheduled);
MODELOBJECT_TEMPLATES(SetpointManagerScheduledDualSetpoint);
MODELOBJECT_TEMPLATES(SetpointManagerSingleZoneHumidityMinimum);
MODELOBJECT_TEMPLATES(SetpointManagerSingleZoneReheat);
MODELOBJECT_TEMPLATES(SetpointManagerWarmest);
MODELOBJECT_TEMPLATES(SetpointManagerWarmestTemperatureFlow);
MODELOBJECT_TEMPLATES(Splitter);
MODELOBJECT_TEMPLATES(AirLoopHVACSupplyPlenum);
MODELOBJECT_TEMPLATES(AirLoopHVACZoneSplitter);
MODELOBJECT_TEMPLATES(ConnectorSplitter);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctConstantVolumeCooledBeam);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctConstantVolumeFourPipeInduction);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctConstantVolumeReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctParallelPIUReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctSeriesPIUReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctUncontrolled);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctVAVReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctVAVNoReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctInletSideMixer);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctVAVHeatAndCoolNoReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctVAVHeatAndCoolReheat);
MODELOBJECT_TEMPLATES(BoilerHotWater);
MODELOBJECT_TEMPLATES(BoilerSteam);
MODELOBJECT_TEMPLATES(ChillerElectricEIR);
MODELOBJECT_TEMPLATES(CoilCoolingCooledBeam);
MODELOBJECT_TEMPLATES(CoilCoolingDXSingleSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingDXMultiSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingDXMultiSpeedStageData);
MODELOBJECT_TEMPLATES(CoilCoolingDXTwoSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingDXVariableRefrigerantFlow);
MODELOBJECT_TEMPLATES(CoilCoolingLowTempRadiantConstFlow);
MODELOBJECT_TEMPLATES(CoilCoolingLowTempRadiantVarFlow);
MODELOBJECT_TEMPLATES(CoilCoolingWaterToAirHeatPumpEquationFit);
MODELOBJECT_TEMPLATES(CoilHeatingDesuperheater);
MODELOBJECT_TEMPLATES(CoilHeatingDXSingleSpeed);
MODELOBJECT_TEMPLATES(CoilHeatingDXVariableRefrigerantFlow);
MODELOBJECT_TEMPLATES(CoilHeatingElectric);
MODELOBJECT_TEMPLATES(CoilHeatingGas);
MODELOBJECT_TEMPLATES(CoilHeatingGasMultiStage);
MODELOBJECT_TEMPLATES(CoilHeatingGasMultiStageStageData);
MODELOBJECT_TEMPLATES(CoilHeatingLowTempRadiantConstFlow);
MODELOBJECT_TEMPLATES(CoilHeatingLowTempRadiantVarFlow);
MODELOBJECT_TEMPLATES(CoilWaterHeatingDesuperheater);
MODELOBJECT_TEMPLATES(CoolingTowerSingleSpeed);
MODELOBJECT_TEMPLATES(CoolingTowerTwoSpeed);
MODELOBJECT_TEMPLATES(CoolingTowerVariableSpeed);
MODELOBJECT_TEMPLATES(CoolingTowerPerformanceCoolTools);
MODELOBJECT_TEMPLATES(CoolingTowerPerformanceYorkCalc);
MODELOBJECT_TEMPLATES(EvaporativeCoolerDirectResearchSpecial);
MODELOBJECT_TEMPLATES(EvaporativeCoolerIndirectResearchSpecial);
MODELOBJECT_TEMPLATES(EvaporativeFluidCoolerSingleSpeed);
MODELOBJECT_TEMPLATES(FanConstantVolume);
MODELOBJECT_TEMPLATES(FanOnOff);
MODELOBJECT_TEMPLATES(FanVariableVolume);
MODELOBJECT_TEMPLATES(FanZoneExhaust);
MODELOBJECT_TEMPLATES(PumpConstantSpeed);
MODELOBJECT_TEMPLATES(PumpVariableSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingWater);
MODELOBJECT_TEMPLATES(CoilHeatingWater);
MODELOBJECT_TEMPLATES(CoilHeatingWaterToAirHeatPumpEquationFit);
MODELOBJECT_TEMPLATES(CoilHeatingWaterBaseboard);
MODELOBJECT_TEMPLATES(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass);
MODELOBJECT_TEMPLATES(AirLoopHVACUnitaryHeatPumpAirToAir);
MODELOBJECT_TEMPLATES(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed);
MODELOBJECT_TEMPLATES(AirLoopHVACUnitarySystem);
MODELOBJECT_TEMPLATES(ZoneHVACBaseboardConvectiveElectric);
MODELOBJECT_TEMPLATES(ZoneHVACBaseboardConvectiveWater);
MODELOBJECT_TEMPLATES(ZoneHVACIdealLoadsAirSystem);
MODELOBJECT_TEMPLATES(ZoneHVACFourPipeFanCoil);
MODELOBJECT_TEMPLATES(ZoneHVACHighTemperatureRadiant);
MODELOBJECT_TEMPLATES(ZoneHVACLowTemperatureRadiantElectric);
MODELOBJECT_TEMPLATES(ZoneHVACLowTempRadiantConstFlow);
MODELOBJECT_TEMPLATES(ZoneHVACLowTempRadiantVarFlow);
MODELOBJECT_TEMPLATES(ZoneHVACPackagedTerminalAirConditioner);
MODELOBJECT_TEMPLATES(ZoneHVACPackagedTerminalHeatPump);
MODELOBJECT_TEMPLATES(ZoneHVACTerminalUnitVariableRefrigerantFlow);
MODELOBJECT_TEMPLATES(ZoneHVACWaterToAirHeatPump);
MODELOBJECT_TEMPLATES(ZoneHVACUnitHeater);
MODELOBJECT_TEMPLATES(PipeAdiabatic);
MODELOBJECT_TEMPLATES(DistrictHeating);
MODELOBJECT_TEMPLATES(DistrictCooling);
MODELOBJECT_TEMPLATES(WaterHeaterMixed);
MODELOBJECT_TEMPLATES(WaterUseEquipmentDefinition);
MODELOBJECT_TEMPLATES(WaterUseEquipment);
MODELOBJECT_TEMPLATES(WaterUseConnections);
MODELOBJECT_TEMPLATES(HeatExchangerAirToAirSensibleAndLatent);
MODELOBJECT_TEMPLATES(HeatExchangerFluidToFluid);
MODELOBJECT_TEMPLATES(HumidifierSteamElectric);
MODELOBJECT_TEMPLATES(GroundHeatExchangerVertical);

SWIG_MODELOBJECT(PortList, 1);
SWIG_MODELOBJECT(Loop, 0);
SWIG_MODELOBJECT(HVACComponent, 0);
SWIG_MODELOBJECT(ZoneHVACComponent, 0);
SWIG_MODELOBJECT(ZoneHVACEquipmentList, 1);
SWIG_MODELOBJECT(Thermostat, 0);
SWIG_MODELOBJECT(SetpointManager, 0);
SWIG_MODELOBJECT(StraightComponent, 0);
SWIG_MODELOBJECT(WaterToAirComponent, 0);
SWIG_MODELOBJECT(WaterToWaterComponent, 0);
SWIG_MODELOBJECT(AirToAirComponent, 0);
SWIG_MODELOBJECT(Node, 1);
SWIG_MODELOBJECT(SizingZone, 1);
SWIG_MODELOBJECT(SizingSystem, 1);
SWIG_MODELOBJECT(ThermalZone, 1);
SWIG_MODELOBJECT(ThermostatSetpointDualSetpoint, 1);
SWIG_MODELOBJECT(ZoneControlHumidistat,1);
SWIG_MODELOBJECT(ZoneControlThermostatStagedDualSetpoint,1);
SWIG_MODELOBJECT(AirConditionerVariableRefrigerantFlow, 1);
SWIG_MODELOBJECT(AirLoopHVAC, 1);
SWIG_MODELOBJECT(SizingPlant, 1);
SWIG_MODELOBJECT(PlantLoop, 1);
SWIG_MODELOBJECT(AirLoopHVACOutdoorAirSystem, 1);
SWIG_MODELOBJECT(ControllerMechanicalVentilation, 1);
SWIG_MODELOBJECT(ControllerOutdoorAir, 1);
SWIG_MODELOBJECT(ControllerWaterCoil, 1);
SWIG_MODELOBJECT(Mixer, 0);
SWIG_MODELOBJECT(AirLoopHVACReturnPlenum, 1);
SWIG_MODELOBJECT(AirLoopHVACZoneMixer, 1);
SWIG_MODELOBJECT(ConnectorMixer, 1);
SWIG_MODELOBJECT(AvailabilityManagerNightCycle, 1);
SWIG_MODELOBJECT(AvailabilityManagerScheduled, 1);
SWIG_MODELOBJECT(SetpointManagerFollowOutdoorAirTemperature, 1);
SWIG_MODELOBJECT(SetpointManagerMixedAir, 1);
SWIG_MODELOBJECT(SetpointManagerMultiZoneHumidityMinimum, 1);
SWIG_MODELOBJECT(SetpointManagerMultiZoneMinimumHumidityAverage, 1);
SWIG_MODELOBJECT(SetpointManagerOutdoorAirPretreat, 1);
SWIG_MODELOBJECT(SetpointManagerOutdoorAirReset, 1);
SWIG_MODELOBJECT(SetpointManagerScheduled, 1);
SWIG_MODELOBJECT(SetpointManagerScheduledDualSetpoint, 1);
SWIG_MODELOBJECT(SetpointManagerSingleZoneHumidityMinimum, 1);
SWIG_MODELOBJECT(SetpointManagerSingleZoneReheat, 1);
SWIG_MODELOBJECT(SetpointManagerWarmest, 1);
SWIG_MODELOBJECT(SetpointManagerWarmestTemperatureFlow, 1);
SWIG_MODELOBJECT(Splitter, 0);
SWIG_MODELOBJECT(AirLoopHVACSupplyPlenum, 1);
SWIG_MODELOBJECT(AirLoopHVACZoneSplitter, 1);
SWIG_MODELOBJECT(ConnectorSplitter, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctConstantVolumeCooledBeam, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctConstantVolumeFourPipeInduction, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctConstantVolumeReheat, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctParallelPIUReheat, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctSeriesPIUReheat, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctUncontrolled, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctVAVReheat, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctVAVNoReheat, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctInletSideMixer, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctVAVHeatAndCoolNoReheat, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctVAVHeatAndCoolReheat, 1);
SWIG_MODELOBJECT(BoilerHotWater, 1);
SWIG_MODELOBJECT(BoilerSteam, 1);
SWIG_MODELOBJECT(ChillerElectricEIR, 1);
SWIG_MODELOBJECT(CoilCoolingCooledBeam, 1);
SWIG_MODELOBJECT(CoilCoolingDXSingleSpeed, 1);
SWIG_MODELOBJECT(CoilCoolingDXMultiSpeed, 1);
SWIG_MODELOBJECT(CoilCoolingDXMultiSpeedStageData, 1);
SWIG_MODELOBJECT(CoilCoolingDXTwoSpeed, 1);
SWIG_MODELOBJECT(CoilCoolingDXVariableRefrigerantFlow, 1);
SWIG_MODELOBJECT(CoilCoolingLowTempRadiantConstFlow, 1);
SWIG_MODELOBJECT(CoilCoolingLowTempRadiantVarFlow, 1);
SWIG_MODELOBJECT(CoilCoolingWaterToAirHeatPumpEquationFit, 1);
SWIG_MODELOBJECT(CoilHeatingDesuperheater, 1);
SWIG_MODELOBJECT(CoilHeatingDXSingleSpeed, 1);
SWIG_MODELOBJECT(CoilHeatingDXVariableRefrigerantFlow, 1);
SWIG_MODELOBJECT(CoilHeatingElectric, 1);
SWIG_MODELOBJECT(CoilHeatingGas, 1);
SWIG_MODELOBJECT(CoilHeatingGasMultiStage, 1);
SWIG_MODELOBJECT(CoilHeatingGasMultiStageStageData, 1);
SWIG_MODELOBJECT(CoilHeatingLowTempRadiantConstFlow, 1);
SWIG_MODELOBJECT(CoilHeatingLowTempRadiantVarFlow, 1);
SWIG_MODELOBJECT(CoilWaterHeatingDesuperheater, 1);
SWIG_MODELOBJECT(CoolingTowerSingleSpeed, 1);
SWIG_MODELOBJECT(CoolingTowerTwoSpeed, 1);
SWIG_MODELOBJECT(CoolingTowerVariableSpeed, 1);
SWIG_MODELOBJECT(CoolingTowerPerformanceYorkCalc, 1);
SWIG_MODELOBJECT(CoolingTowerPerformanceCoolTools, 1);
SWIG_MODELOBJECT(EvaporativeCoolerDirectResearchSpecial, 1);
SWIG_MODELOBJECT(EvaporativeCoolerIndirectResearchSpecial, 1);
SWIG_MODELOBJECT(EvaporativeFluidCoolerSingleSpeed, 1);
SWIG_MODELOBJECT(FanConstantVolume, 1);
SWIG_MODELOBJECT(FanOnOff, 1);
SWIG_MODELOBJECT(FanVariableVolume, 1);
SWIG_MODELOBJECT(FanZoneExhaust, 1);
SWIG_MODELOBJECT(PumpConstantSpeed, 1);
SWIG_MODELOBJECT(PumpVariableSpeed, 1);
SWIG_MODELOBJECT(CoilCoolingWater, 1);
SWIG_MODELOBJECT(CoilHeatingWater, 1);
SWIG_MODELOBJECT(CoilHeatingWaterToAirHeatPumpEquationFit, 1);
SWIG_MODELOBJECT(CoilHeatingWaterBaseboard, 1);
SWIG_MODELOBJECT(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass, 1);
SWIG_MODELOBJECT(AirLoopHVACUnitaryHeatPumpAirToAir, 1);
SWIG_MODELOBJECT(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed, 1);
SWIG_MODELOBJECT(AirLoopHVACUnitarySystem, 1);
SWIG_MODELOBJECT(ZoneHVACBaseboardConvectiveElectric, 1);
SWIG_MODELOBJECT(ZoneHVACBaseboardConvectiveWater, 1);
SWIG_MODELOBJECT(ZoneHVACIdealLoadsAirSystem, 1);
SWIG_MODELOBJECT(ZoneHVACFourPipeFanCoil, 1);
SWIG_MODELOBJECT(ZoneHVACHighTemperatureRadiant, 1);
SWIG_MODELOBJECT(ZoneHVACLowTemperatureRadiantElectric, 1);
SWIG_MODELOBJECT(ZoneHVACLowTempRadiantConstFlow, 1);
SWIG_MODELOBJECT(ZoneHVACLowTempRadiantVarFlow, 1);
SWIG_MODELOBJECT(ZoneHVACPackagedTerminalAirConditioner, 1);
SWIG_MODELOBJECT(ZoneHVACPackagedTerminalHeatPump, 1);
SWIG_MODELOBJECT(ZoneHVACTerminalUnitVariableRefrigerantFlow, 1);
SWIG_MODELOBJECT(ZoneHVACWaterToAirHeatPump, 1);
SWIG_MODELOBJECT(ZoneHVACUnitHeater, 1);
SWIG_MODELOBJECT(PipeAdiabatic, 1);
SWIG_MODELOBJECT(DistrictHeating, 1);
SWIG_MODELOBJECT(DistrictCooling, 1);
SWIG_MODELOBJECT(WaterHeaterMixed, 1);
SWIG_MODELOBJECT(WaterUseEquipmentDefinition, 1);
SWIG_MODELOBJECT(WaterUseEquipment, 1);
SWIG_MODELOBJECT(WaterUseConnections, 1);
SWIG_MODELOBJECT(HeatExchangerAirToAirSensibleAndLatent, 1);
SWIG_MODELOBJECT(HeatExchangerFluidToFluid,1);
SWIG_MODELOBJECT(HumidifierSteamElectric,1);
SWIG_MODELOBJECT(GroundHeatExchangerVertical, 1);

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
  %inline {
    namespace openstudio {
      namespace model {
        std::vector<openstudio::model::ThermalZone> getThermalZones(const openstudio::model::Building& building){
          return building.thermalZones();
        }
        boost::optional<openstudio::model::ThermalZone> getThermalZone(const openstudio::model::Space& space){
          return space.thermalZone();
        }
        bool setThermalZone(openstudio::model::Space space, openstudio::model::ThermalZone thermalZone){
          return space.setThermalZone(thermalZone);
        }
      }
    }
  }
#endif

#if defined(SWIGCSHARP)
  //%pragma(csharp) imclassimports=%{
  %pragma(csharp) moduleimports=%{
  
    using System;
    using System.Runtime.InteropServices;
        
    public partial class Building : ParentObject {
      public ThermalZoneVector thermalZones()
      {
        return OpenStudio.OpenStudioModelHVAC.getThermalZones(this);
      }
    }  
    
    public partial class Space : PlanarSurfaceGroup {
      public OptionalThermalZone thermalZone()
      {
        return OpenStudio.OpenStudioModelHVAC.getThermalZone(this);
      }
      public bool setThermalZone(OpenStudio.ThermalZone thermalZone)
      {
        return OpenStudio.OpenStudioModelHVAC.setThermalZone(this, thermalZone);
      }
    }  
  %}
#endif

%include <model/HVACTemplates.hpp>

#endif 
