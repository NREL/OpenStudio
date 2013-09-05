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
MODELOBJECT_TEMPLATES(ZoneHVACComponent);
MODELOBJECT_TEMPLATES(ZoneHVACEquipmentList);
MODELOBJECT_TEMPLATES(HVACComponent);
MODELOBJECT_TEMPLATES(StraightComponent);
MODELOBJECT_TEMPLATES(WaterToAirComponent);
MODELOBJECT_TEMPLATES(WaterToWaterComponent);
MODELOBJECT_TEMPLATES(AirToAirComponent);
MODELOBJECT_TEMPLATES(Node);
MODELOBJECT_TEMPLATES(SizingZone);
MODELOBJECT_TEMPLATES(SizingSystem);
MODELOBJECT_TEMPLATES(ThermalZone);
MODELOBJECT_TEMPLATES(ThermostatSetpointDualSetpoint);
MODELOBJECT_TEMPLATES(AirLoopHVAC);
MODELOBJECT_TEMPLATES(SizingPlant);
MODELOBJECT_TEMPLATES(PlantLoop);
MODELOBJECT_TEMPLATES(AirLoopHVACOutdoorAirSystem);
MODELOBJECT_TEMPLATES(ControllerMechanicalVentilation);
MODELOBJECT_TEMPLATES(ControllerOutdoorAir);
MODELOBJECT_TEMPLATES(ControllerWaterCoil);
MODELOBJECT_TEMPLATES(Mixer);
MODELOBJECT_TEMPLATES(AirLoopHVACZoneMixer);
MODELOBJECT_TEMPLATES(ConnectorMixer);
MODELOBJECT_TEMPLATES(AvailabilityManagerNightCycle);
MODELOBJECT_TEMPLATES(AvailabilityManagerScheduled);
MODELOBJECT_TEMPLATES(SetpointManagerFollowOutdoorAirTemperature);
MODELOBJECT_TEMPLATES(SetpointManagerMixedAir);
MODELOBJECT_TEMPLATES(SetpointManagerOutdoorAirReset);
MODELOBJECT_TEMPLATES(SetpointManagerScheduled);
MODELOBJECT_TEMPLATES(SetpointManagerSingleZoneReheat);
MODELOBJECT_TEMPLATES(SetpointManagerWarmest);
MODELOBJECT_TEMPLATES(Splitter);
MODELOBJECT_TEMPLATES(AirLoopHVACZoneSplitter);
MODELOBJECT_TEMPLATES(ConnectorSplitter);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctConstantVolumeCooledBeam);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctParallelPIUReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctUncontrolled);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctVAVReheat);
MODELOBJECT_TEMPLATES(BoilerHotWater);
MODELOBJECT_TEMPLATES(BoilerSteam);
MODELOBJECT_TEMPLATES(ChillerElectricEIR);
MODELOBJECT_TEMPLATES(CoilCoolingCooledBeam);
MODELOBJECT_TEMPLATES(CoilCoolingDXSingleSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingDXTwoSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingLowTempRadiantConstFlow);
MODELOBJECT_TEMPLATES(CoilCoolingLowTempRadiantVarFlow);
MODELOBJECT_TEMPLATES(CoilCoolingWaterToAirHeatPumpEquationFit);
MODELOBJECT_TEMPLATES(CoilHeatingDXSingleSpeed);
MODELOBJECT_TEMPLATES(CoilHeatingElectric);
MODELOBJECT_TEMPLATES(CoilHeatingGas);
MODELOBJECT_TEMPLATES(CoilHeatingLowTempRadiantConstFlow);
MODELOBJECT_TEMPLATES(CoilHeatingLowTempRadiantVarFlow);
MODELOBJECT_TEMPLATES(CoolingTowerSingleSpeed);
MODELOBJECT_TEMPLATES(EvaporativeCoolerDirectResearchSpecial);
MODELOBJECT_TEMPLATES(EvaporativeFluidCoolerSingleSpeed);
MODELOBJECT_TEMPLATES(FanConstantVolume);
MODELOBJECT_TEMPLATES(FanOnOff);
MODELOBJECT_TEMPLATES(FanVariableVolume);
MODELOBJECT_TEMPLATES(PumpConstantSpeed);
MODELOBJECT_TEMPLATES(PumpVariableSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingWater);
MODELOBJECT_TEMPLATES(CoilHeatingWater);
MODELOBJECT_TEMPLATES(CoilHeatingWaterToAirHeatPumpEquationFit);
MODELOBJECT_TEMPLATES(CoilHeatingWaterBaseboard);
MODELOBJECT_TEMPLATES(AirLoopHVACUnitaryHeatPumpAirToAir);
MODELOBJECT_TEMPLATES(ZoneHVACBaseboardConvectiveElectric);
MODELOBJECT_TEMPLATES(ZoneHVACBaseboardConvectiveWater);
MODELOBJECT_TEMPLATES(ZoneHVACIdealLoadsAirSystem);
MODELOBJECT_TEMPLATES(ZoneHVACFourPipeFanCoil);
MODELOBJECT_TEMPLATES(ZoneHVACLowTemperatureRadiantElectric);
MODELOBJECT_TEMPLATES(ZoneHVACLowTempRadiantConstFlow);
MODELOBJECT_TEMPLATES(ZoneHVACLowTempRadiantVarFlow);
MODELOBJECT_TEMPLATES(ZoneHVACPackagedTerminalAirConditioner);
MODELOBJECT_TEMPLATES(ZoneHVACPackagedTerminalHeatPump);
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
MODELOBJECT_TEMPLATES(GroundHeatExchangerVertical);

SWIG_MODELOBJECT(PortList);
SWIG_MODELOBJECT(Loop);
SWIG_MODELOBJECT(ZoneHVACComponent);
SWIG_MODELOBJECT(ZoneHVACEquipmentList);
SWIG_MODELOBJECT(HVACComponent);
SWIG_MODELOBJECT(StraightComponent);
SWIG_MODELOBJECT(WaterToAirComponent);
SWIG_MODELOBJECT(WaterToWaterComponent);
SWIG_MODELOBJECT(AirToAirComponent);
SWIG_MODELOBJECT(Node);
SWIG_MODELOBJECT(SizingZone);
SWIG_MODELOBJECT(SizingSystem);
SWIG_MODELOBJECT(ThermalZone);
SWIG_MODELOBJECT(ThermostatSetpointDualSetpoint);
SWIG_MODELOBJECT(AirLoopHVAC);
SWIG_MODELOBJECT(SizingPlant);
SWIG_MODELOBJECT(PlantLoop);
SWIG_MODELOBJECT(AirLoopHVACOutdoorAirSystem);
SWIG_MODELOBJECT(ControllerMechanicalVentilation);
SWIG_MODELOBJECT(ControllerOutdoorAir);
SWIG_MODELOBJECT(ControllerWaterCoil);
SWIG_MODELOBJECT(Mixer);
SWIG_MODELOBJECT(AirLoopHVACZoneMixer);
SWIG_MODELOBJECT(ConnectorMixer);
SWIG_MODELOBJECT(AvailabilityManagerNightCycle);
SWIG_MODELOBJECT(AvailabilityManagerScheduled);
SWIG_MODELOBJECT(SetpointManagerFollowOutdoorAirTemperature);
SWIG_MODELOBJECT(SetpointManagerMixedAir);
SWIG_MODELOBJECT(SetpointManagerOutdoorAirReset);
SWIG_MODELOBJECT(SetpointManagerScheduled);
SWIG_MODELOBJECT(SetpointManagerSingleZoneReheat);
SWIG_MODELOBJECT(SetpointManagerWarmest);
SWIG_MODELOBJECT(Splitter);
SWIG_MODELOBJECT(AirLoopHVACZoneSplitter);
SWIG_MODELOBJECT(ConnectorSplitter);
SWIG_MODELOBJECT(AirTerminalSingleDuctConstantVolumeCooledBeam);
SWIG_MODELOBJECT(AirTerminalSingleDuctParallelPIUReheat);
SWIG_MODELOBJECT(AirTerminalSingleDuctUncontrolled);
SWIG_MODELOBJECT(AirTerminalSingleDuctVAVReheat);
SWIG_MODELOBJECT(BoilerHotWater);
SWIG_MODELOBJECT(BoilerSteam);
SWIG_MODELOBJECT(ChillerElectricEIR);
SWIG_MODELOBJECT(CoilCoolingCooledBeam);
SWIG_MODELOBJECT(CoilCoolingDXSingleSpeed);
SWIG_MODELOBJECT(CoilCoolingDXTwoSpeed);
SWIG_MODELOBJECT(CoilCoolingLowTempRadiantConstFlow);
SWIG_MODELOBJECT(CoilCoolingLowTempRadiantVarFlow);
SWIG_MODELOBJECT(CoilCoolingWaterToAirHeatPumpEquationFit);
SWIG_MODELOBJECT(CoilHeatingDXSingleSpeed);
SWIG_MODELOBJECT(CoilHeatingElectric);
SWIG_MODELOBJECT(CoilHeatingGas);
SWIG_MODELOBJECT(CoilHeatingLowTempRadiantConstFlow);
SWIG_MODELOBJECT(CoilHeatingLowTempRadiantVarFlow);
SWIG_MODELOBJECT(CoolingTowerSingleSpeed);
SWIG_MODELOBJECT(EvaporativeCoolerDirectResearchSpecial);
SWIG_MODELOBJECT(EvaporativeFluidCoolerSingleSpeed);
SWIG_MODELOBJECT(FanConstantVolume);
SWIG_MODELOBJECT(FanOnOff);
SWIG_MODELOBJECT(FanVariableVolume);
SWIG_MODELOBJECT(PumpConstantSpeed);
SWIG_MODELOBJECT(PumpVariableSpeed);
SWIG_MODELOBJECT(CoilCoolingWater);
SWIG_MODELOBJECT(CoilHeatingWater);
SWIG_MODELOBJECT(CoilHeatingWaterToAirHeatPumpEquationFit);
SWIG_MODELOBJECT(CoilHeatingWaterBaseboard);
SWIG_MODELOBJECT(AirLoopHVACUnitaryHeatPumpAirToAir);
SWIG_MODELOBJECT(ZoneHVACBaseboardConvectiveElectric);
SWIG_MODELOBJECT(ZoneHVACBaseboardConvectiveWater);
SWIG_MODELOBJECT(ZoneHVACIdealLoadsAirSystem);
SWIG_MODELOBJECT(ZoneHVACFourPipeFanCoil);
SWIG_MODELOBJECT(ZoneHVACLowTemperatureRadiantElectric);
SWIG_MODELOBJECT(ZoneHVACLowTempRadiantConstFlow);
SWIG_MODELOBJECT(ZoneHVACLowTempRadiantVarFlow);
SWIG_MODELOBJECT(ZoneHVACPackagedTerminalAirConditioner);
SWIG_MODELOBJECT(ZoneHVACPackagedTerminalHeatPump);
SWIG_MODELOBJECT(ZoneHVACWaterToAirHeatPump);
SWIG_MODELOBJECT(ZoneHVACUnitHeater);
SWIG_MODELOBJECT(PipeAdiabatic);
SWIG_MODELOBJECT(DistrictHeating);
SWIG_MODELOBJECT(DistrictCooling);
SWIG_MODELOBJECT(WaterHeaterMixed);
SWIG_MODELOBJECT(WaterUseEquipmentDefinition);
SWIG_MODELOBJECT(WaterUseEquipment);
SWIG_MODELOBJECT(WaterUseConnections);
SWIG_MODELOBJECT(HeatExchangerAirToAirSensibleAndLatent);
SWIG_MODELOBJECT(GroundHeatExchangerVertical);

#if defined SWIGCSHARP

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

%include <model/HVACTemplates.hpp>

#endif 
