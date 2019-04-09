#ifndef MODEL_STRAIGHTCOMPONENT_I
#define MODEL_STRAIGHTCOMPONENT_I


#ifdef SWIGPYTHON
  %module openstudiomodelstraightcomponent
#endif

%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>
%import <model/ModelSimulation.i>
%import <model/ModelResources.i>
%import <model/ModelGeometry.i>
%import <model/ModelHVAC.i>
%import <model/ModelZoneHVAC.i>

#if defined SWIGCSHARP
  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelStraightComponent
#endif

%ignore std::vector<openstudio::model::GFunction>::vector(size_type);
%ignore std::vector<openstudio::model::GFunction>::resize(size_type);
%template(GFunctionVector) std::vector<openstudio::model::GFunction>;

MODELOBJECT_TEMPLATES(AirConditionerVariableRefrigerantFlow);
MODELOBJECT_TEMPLATES(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass);
MODELOBJECT_TEMPLATES(AirLoopHVACUnitaryHeatPumpAirToAir);
MODELOBJECT_TEMPLATES(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctConstantVolumeCooledBeam);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctConstantVolumeFourPipeBeam);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctConstantVolumeFourPipeInduction);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctConstantVolumeReheat);
// For backward compat, we also provide the old name (AirTerminalSingleDuctUncontrolled) below
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctInletSideMixer);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctParallelPIUReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctSeriesPIUReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctConstantVolumeNoReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctVAVHeatAndCoolNoReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctVAVHeatAndCoolReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctVAVNoReheat);
MODELOBJECT_TEMPLATES(AirTerminalSingleDuctVAVReheat);
MODELOBJECT_TEMPLATES(BoilerHotWater);
MODELOBJECT_TEMPLATES(BoilerSteam);
MODELOBJECT_TEMPLATES(CoilCoolingCooledBeam);
MODELOBJECT_TEMPLATES(CoilCoolingDXMultiSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingDXSingleSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingDXTwoSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingDXTwoStageWithHumidityControlMode);
MODELOBJECT_TEMPLATES(CoilCoolingDXVariableSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingFourPipeBeam);
MODELOBJECT_TEMPLATES(CoilCoolingLowTempRadiantConstFlow);
MODELOBJECT_TEMPLATES(CoilCoolingLowTempRadiantVarFlow);
MODELOBJECT_TEMPLATES(CoilHeatingDesuperheater);
MODELOBJECT_TEMPLATES(CoilHeatingDXMultiSpeed);
MODELOBJECT_TEMPLATES(CoilHeatingDXSingleSpeed);
MODELOBJECT_TEMPLATES(CoilHeatingDXVariableSpeed);
MODELOBJECT_TEMPLATES(CoilHeatingElectric);
MODELOBJECT_TEMPLATES(CoilHeatingFourPipeBeam);
MODELOBJECT_TEMPLATES(CoilHeatingGas);
MODELOBJECT_TEMPLATES(CoilHeatingGasMultiStage);
MODELOBJECT_TEMPLATES(CoilHeatingLowTempRadiantConstFlow);
MODELOBJECT_TEMPLATES(CoilHeatingLowTempRadiantVarFlow);
MODELOBJECT_TEMPLATES(CoilHeatingWaterBaseboard);
MODELOBJECT_TEMPLATES(CoilHeatingWaterBaseboardRadiant);
MODELOBJECT_TEMPLATES(CoilSystemCoolingDXHeatExchangerAssisted);
MODELOBJECT_TEMPLATES(CoilSystemCoolingWaterHeatExchangerAssisted);
MODELOBJECT_TEMPLATES(CoilWaterHeatingDesuperheater);
MODELOBJECT_TEMPLATES(CoolingTowerSingleSpeed);
MODELOBJECT_TEMPLATES(CoolingTowerTwoSpeed);
MODELOBJECT_TEMPLATES(CoolingTowerVariableSpeed);
MODELOBJECT_TEMPLATES(DistrictCooling);
MODELOBJECT_TEMPLATES(DistrictHeating);
MODELOBJECT_TEMPLATES(Duct);
MODELOBJECT_TEMPLATES(EvaporativeCoolerDirectResearchSpecial);
MODELOBJECT_TEMPLATES(EvaporativeCoolerIndirectResearchSpecial);
MODELOBJECT_TEMPLATES(EvaporativeFluidCoolerSingleSpeed);
MODELOBJECT_TEMPLATES(EvaporativeFluidCoolerTwoSpeed);
MODELOBJECT_TEMPLATES(FanConstantVolume);
MODELOBJECT_TEMPLATES(FanOnOff);
MODELOBJECT_TEMPLATES(FanVariableVolume);
MODELOBJECT_TEMPLATES(FluidCoolerSingleSpeed);
MODELOBJECT_TEMPLATES(FluidCoolerTwoSpeed);
MODELOBJECT_TEMPLATES(GroundHeatExchangerHorizontalTrench);
MODELOBJECT_TEMPLATES(GroundHeatExchangerVertical);
MODELOBJECT_TEMPLATES(HeaderedPumpsConstantSpeed);
MODELOBJECT_TEMPLATES(HeaderedPumpsVariableSpeed);
MODELOBJECT_TEMPLATES(HumidifierSteamElectric);
MODELOBJECT_TEMPLATES(LoadProfilePlant);
MODELOBJECT_TEMPLATES(PipeAdiabatic);
MODELOBJECT_TEMPLATES(PipeIndoor);
MODELOBJECT_TEMPLATES(PipeOutdoor);
MODELOBJECT_TEMPLATES(PlantComponentTemperatureSource);
MODELOBJECT_TEMPLATES(PumpConstantSpeed);
MODELOBJECT_TEMPLATES(PumpVariableSpeed);
MODELOBJECT_TEMPLATES(SolarCollectorFlatPlatePhotovoltaicThermal);
MODELOBJECT_TEMPLATES(SolarCollectorFlatPlateWater);
MODELOBJECT_TEMPLATES(SolarCollectorIntegralCollectorStorage);
MODELOBJECT_TEMPLATES(TemperingValve);
MODELOBJECT_TEMPLATES(ThermalStorageIceDetailed);
MODELOBJECT_TEMPLATES(WaterUseConnections);

SWIG_MODELOBJECT(AirConditionerVariableRefrigerantFlow,1);
SWIG_MODELOBJECT(AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass,1);
SWIG_MODELOBJECT(AirLoopHVACUnitaryHeatPumpAirToAir,1);
SWIG_MODELOBJECT(AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed,1);
SWIG_MODELOBJECT(AirTerminalSingleDuctConstantVolumeCooledBeam,1);
SWIG_MODELOBJECT(AirTerminalSingleDuctConstantVolumeFourPipeBeam, 1);
SWIG_MODELOBJECT(AirTerminalSingleDuctConstantVolumeFourPipeInduction,1);
SWIG_MODELOBJECT(AirTerminalSingleDuctConstantVolumeReheat,1);
SWIG_MODELOBJECT(AirTerminalSingleDuctInletSideMixer,1);
SWIG_MODELOBJECT(AirTerminalSingleDuctParallelPIUReheat,1);
SWIG_MODELOBJECT(AirTerminalSingleDuctSeriesPIUReheat,1);
SWIG_MODELOBJECT(AirTerminalSingleDuctConstantVolumeNoReheat,1);
SWIG_MODELOBJECT(AirTerminalSingleDuctVAVHeatAndCoolNoReheat,1);
SWIG_MODELOBJECT(AirTerminalSingleDuctVAVHeatAndCoolReheat,1);
SWIG_MODELOBJECT(AirTerminalSingleDuctVAVNoReheat,1);
SWIG_MODELOBJECT(AirTerminalSingleDuctVAVReheat,1);
SWIG_MODELOBJECT(BoilerHotWater,1);
SWIG_MODELOBJECT(BoilerSteam,1);
SWIG_MODELOBJECT(CoilCoolingCooledBeam,1);
SWIG_MODELOBJECT(CoilCoolingDXMultiSpeed,1);
SWIG_MODELOBJECT(CoilCoolingDXSingleSpeed,1);
SWIG_MODELOBJECT(CoilCoolingDXTwoSpeed,1);
SWIG_MODELOBJECT(CoilCoolingDXTwoStageWithHumidityControlMode,1);
SWIG_MODELOBJECT(CoilCoolingDXVariableSpeed,1);
SWIG_MODELOBJECT(CoilCoolingFourPipeBeam, 1);
SWIG_MODELOBJECT(CoilCoolingLowTempRadiantConstFlow,1);
SWIG_MODELOBJECT(CoilCoolingLowTempRadiantVarFlow,1);
SWIG_MODELOBJECT(CoilHeatingDesuperheater,1);
SWIG_MODELOBJECT(CoilHeatingDXMultiSpeed,1);
SWIG_MODELOBJECT(CoilHeatingDXSingleSpeed,1);
SWIG_MODELOBJECT(CoilHeatingDXVariableSpeed,1);
SWIG_MODELOBJECT(CoilHeatingElectric,1);
SWIG_MODELOBJECT(CoilHeatingFourPipeBeam, 1);
SWIG_MODELOBJECT(CoilHeatingGas,1);
SWIG_MODELOBJECT(CoilHeatingGasMultiStage,1);
SWIG_MODELOBJECT(CoilHeatingLowTempRadiantConstFlow,1);
SWIG_MODELOBJECT(CoilHeatingLowTempRadiantVarFlow,1);
SWIG_MODELOBJECT(CoilHeatingWaterBaseboard,1);
SWIG_MODELOBJECT(CoilHeatingWaterBaseboardRadiant,1);
SWIG_MODELOBJECT(CoilSystemCoolingDXHeatExchangerAssisted,1);
SWIG_MODELOBJECT(CoilSystemCoolingWaterHeatExchangerAssisted,1);
SWIG_MODELOBJECT(CoilWaterHeatingDesuperheater,1);
SWIG_MODELOBJECT(CoolingTowerSingleSpeed,1);
SWIG_MODELOBJECT(CoolingTowerTwoSpeed,1);
SWIG_MODELOBJECT(CoolingTowerVariableSpeed,1);
SWIG_MODELOBJECT(DistrictCooling,1);
SWIG_MODELOBJECT(DistrictHeating,1);
SWIG_MODELOBJECT(Duct,1);
SWIG_MODELOBJECT(EvaporativeCoolerDirectResearchSpecial,1);
SWIG_MODELOBJECT(EvaporativeCoolerIndirectResearchSpecial,1);
SWIG_MODELOBJECT(EvaporativeFluidCoolerSingleSpeed,1);
SWIG_MODELOBJECT(EvaporativeFluidCoolerTwoSpeed,1);
SWIG_MODELOBJECT(FanConstantVolume,1);
SWIG_MODELOBJECT(FanOnOff,1);
SWIG_MODELOBJECT(FanVariableVolume,1);
SWIG_MODELOBJECT(FluidCoolerSingleSpeed,1);
SWIG_MODELOBJECT(FluidCoolerTwoSpeed,1);
SWIG_MODELOBJECT(GroundHeatExchangerHorizontalTrench,1);
SWIG_MODELOBJECT(GroundHeatExchangerVertical,1);
SWIG_MODELOBJECT(HeaderedPumpsConstantSpeed,1);
SWIG_MODELOBJECT(HeaderedPumpsVariableSpeed,1);
SWIG_MODELOBJECT(HumidifierSteamElectric,1);
SWIG_MODELOBJECT(LoadProfilePlant,1);
SWIG_MODELOBJECT(PipeAdiabatic,1);
SWIG_MODELOBJECT(PipeIndoor,1);
SWIG_MODELOBJECT(PipeOutdoor,1);
SWIG_MODELOBJECT(PlantComponentTemperatureSource,1);
SWIG_MODELOBJECT(PumpConstantSpeed,1);
SWIG_MODELOBJECT(PumpVariableSpeed,1);
SWIG_MODELOBJECT(SolarCollectorFlatPlatePhotovoltaicThermal,1);
SWIG_MODELOBJECT(SolarCollectorFlatPlateWater,1);
SWIG_MODELOBJECT(SolarCollectorIntegralCollectorStorage,1);
SWIG_MODELOBJECT(TemperingValve,1);
SWIG_MODELOBJECT(ThermalStorageIceDetailed,1);
SWIG_MODELOBJECT(WaterUseConnections,1);

#if defined SWIGRUBY

  // Provide alternative name for backwards compatibility between old=AirTerminalSingleDuctUncontrolled and new=AirTerminalSingleDuctConstantVolumeNoReheat
  %init %{
    // Alias class name, which allows instantion of new object
    rb_eval_string("OpenStudio::Model::AirTerminalSingleDuctUncontrolled = OpenStudio::Model::AirTerminalSingleDuctConstantVolumeNoReheat");

    // Provide get methods
    rb_eval_string("OpenStudio::IdfObject.class_eval { define_method(:to_AirTerminalSingleDuctUncontrolled) { OpenStudio::Model::toAirTerminalSingleDuctConstantVolumeNoReheat(self); } }");
    rb_eval_string("OpenStudio::Model::Model.class_eval { define_method(:getAirTerminalSingleDuctUncontrolled) { |handle| OpenStudio::Model::getAirTerminalSingleDuctConstantVolumeNoReheat(self, handle); } }");
    rb_eval_string("OpenStudio::Model::Model.class_eval { define_method(:getAirTerminalSingleDuctUncontrolleds) { OpenStudio::Model::getAirTerminalSingleDuctConstantVolumeNoReheats(self); } }");
    rb_eval_string("OpenStudio::Model::Model.class_eval { define_method(:getAirTerminalSingleDuctUncontrolledByName) { |name| OpenStudio::Model::getAirTerminalSingleDuctConstantVolumeNoReheatByName(self, name); } }");
    rb_eval_string("OpenStudio::Model::Model.class_eval { define_method(:getAirTerminalSingleDuctUncontrolledsByName) { |name, exactMatch| OpenStudio::Model::getAirTerminalSingleDuctConstantVolumeNoReheatsByName(self, name, exactMatch); } }");
  %}
#endif

#endif
