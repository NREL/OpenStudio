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

  // ignore airflow objects for now, add back in with partial classes in ModelAirflow.i (swigged after us)
  // TODO: haven't added them to ModelAirflow.i but I don't see any other that are indeed implemented...
  %ignore openstudio::model::AirTerminalSingleDuctConstantVolumeReheat::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::AirTerminalSingleDuctConstantVolumeReheat::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::AirTerminalSingleDuctVAVReheat::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::AirTerminalSingleDuctVAVReheat::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilCoolingDXMultiSpeed::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilCoolingDXMultiSpeed::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilCoolingDXSingleSpeed::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilCoolingDXSingleSpeed::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilCoolingDXTwoStageWithHumidityControlMode::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilCoolingDXTwoStageWithHumidityControlMode::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingDesuperheater::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingDesuperheater::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingDXMultiSpeed::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingDXMultiSpeed::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingDXSingleSpeed::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingDXSingleSpeed::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingElectric::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingElectric::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingGas::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingGas::airflowNetworkEquivalentDuct;

  %ignore openstudio::model::FanComponentModel::getAirflowNetworkFan;
  %ignore openstudio::model::FanComponentModel::airflowNetworkFan;
  %ignore openstudio::model::FanConstantVolume::getAirflowNetworkFan;
  %ignore openstudio::model::FanConstantVolume::airflowNetworkFan;
  %ignore openstudio::model::FanOnOff::getAirflowNetworkFan;
  %ignore openstudio::model::FanOnOff::airflowNetworkFan;
  %ignore openstudio::model::FanVariableVolume::getAirflowNetworkFan;
  %ignore openstudio::model::FanVariableVolume::airflowNetworkFan;
  %ignore openstudio::model::FanSystemModel::getAirflowNetworkFan;
  %ignore openstudio::model::FanSystemModel::airflowNetworkFan;

  // ignore generator objects for now, add back in with partial classes in ModelGenerators.i (swigged after us)
  %ignore openstudio::model::SolarCollectorFlatPlatePhotovoltaicThermal::generatorPhotovoltaic;
  %ignore openstudio::model::SolarCollectorFlatPlatePhotovoltaicThermal::setGeneratorPhotovoltaic;

#endif

#if defined SWIGPYTHON
  %pythoncode %{
    Model = openstudiomodelcore.Model
  %}
#endif

namespace openstudio {
  namespace model {

    // forward declarations
    // For ATUs
    %feature("valuewrapper") AirflowNetworkEquivalentDuct;
    // For Fans
    %feature("valuewrapper") AirflowNetworkFan;
    // For SolarCollectorFlatPlatePhotovoltaicThermal
    %feature("valuewrapper") GeneratorPhotovoltaic;

    class AirflowNetworkEquivalentDuct;
    class AirflowNetworkFan;
    class GeneratorPhotovoltaic;

  }
}

// extend classes
%extend openstudio::model::FanSystemModelSpeed {
  // Use the overloaded operator<< for string representation
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
};

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
MODELOBJECT_TEMPLATES(CoilCoolingDX);
MODELOBJECT_TEMPLATES(CoilCoolingDXMultiSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingDXSingleSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingDXTwoSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingDXTwoStageWithHumidityControlMode);
MODELOBJECT_TEMPLATES(CoilCoolingDXVariableSpeed);
MODELOBJECT_TEMPLATES(CoilCoolingFourPipeBeam);
MODELOBJECT_TEMPLATES(CoilCoolingLowTempRadiantConstFlow);
MODELOBJECT_TEMPLATES(CoilCoolingLowTempRadiantVarFlow);
MODELOBJECT_TEMPLATES(CoilCoolingWaterPanelRadiant);
MODELOBJECT_TEMPLATES(CoilHeatingDesuperheater);
MODELOBJECT_TEMPLATES(CoilHeatingDXMultiSpeed);
MODELOBJECT_TEMPLATES(CoilHeatingDXSingleSpeed);
MODELOBJECT_TEMPLATES(CoilHeatingDXVariableSpeed);
MODELOBJECT_TEMPLATES(CoilHeatingElectric);
MODELOBJECT_TEMPLATES(CoilHeatingElectricMultiStage);
MODELOBJECT_TEMPLATES(CoilHeatingFourPipeBeam);
MODELOBJECT_TEMPLATES(CoilHeatingGas);
MODELOBJECT_TEMPLATES(CoilHeatingGasMultiStage);
MODELOBJECT_TEMPLATES(CoilHeatingLowTempRadiantConstFlow);
MODELOBJECT_TEMPLATES(CoilHeatingLowTempRadiantVarFlow);
MODELOBJECT_TEMPLATES(CoilHeatingWaterBaseboard);
MODELOBJECT_TEMPLATES(CoilHeatingWaterBaseboardRadiant);
MODELOBJECT_TEMPLATES(CoilSystemCoolingDXHeatExchangerAssisted);
MODELOBJECT_TEMPLATES(CoilSystemCoolingWaterHeatExchangerAssisted);
MODELOBJECT_TEMPLATES(CoilSystemIntegratedHeatPumpAirSource);
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
MODELOBJECT_TEMPLATES(FanComponentModel);
MODELOBJECT_TEMPLATES(FanConstantVolume);
MODELOBJECT_TEMPLATES(FanOnOff);
MODELOBJECT_TEMPLATES(FanSystemModelSpeed); // SWIG the helper class I implemented to add speeds
MODELOBJECT_TEMPLATES(FanSystemModel);
MODELOBJECT_TEMPLATES(FanVariableVolume);
MODELOBJECT_TEMPLATES(FluidCoolerSingleSpeed);
MODELOBJECT_TEMPLATES(FluidCoolerTwoSpeed);
MODELOBJECT_TEMPLATES(GroundHeatExchangerHorizontalTrench);
MODELOBJECT_TEMPLATES(GroundHeatExchangerVertical);
MODELOBJECT_TEMPLATES(HeaderedPumpsConstantSpeed);
MODELOBJECT_TEMPLATES(HeaderedPumpsVariableSpeed);
MODELOBJECT_TEMPLATES(HeatPumpAirToWaterFuelFiredHeating);
MODELOBJECT_TEMPLATES(HeatPumpAirToWaterFuelFiredCooling);
MODELOBJECT_TEMPLATES(HumidifierSteamElectric);
MODELOBJECT_TEMPLATES(HumidifierSteamGas);
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
SWIG_MODELOBJECT(CoilCoolingDX,1);
SWIG_MODELOBJECT(CoilCoolingDXMultiSpeed,1);
SWIG_MODELOBJECT(CoilCoolingDXSingleSpeed,1);
SWIG_MODELOBJECT(CoilCoolingDXTwoSpeed,1);
SWIG_MODELOBJECT(CoilCoolingDXTwoStageWithHumidityControlMode,1);
SWIG_MODELOBJECT(CoilCoolingDXVariableSpeed,1);
SWIG_MODELOBJECT(CoilCoolingFourPipeBeam, 1);
SWIG_MODELOBJECT(CoilCoolingLowTempRadiantConstFlow,1);
SWIG_MODELOBJECT(CoilCoolingLowTempRadiantVarFlow,1);
SWIG_MODELOBJECT(CoilCoolingWaterPanelRadiant,1);
SWIG_MODELOBJECT(CoilHeatingDesuperheater,1);
SWIG_MODELOBJECT(CoilHeatingDXMultiSpeed,1);
SWIG_MODELOBJECT(CoilHeatingDXSingleSpeed,1);
SWIG_MODELOBJECT(CoilHeatingDXVariableSpeed,1);
SWIG_MODELOBJECT(CoilHeatingElectric,1);
SWIG_MODELOBJECT(CoilHeatingElectricMultiStage,1);
SWIG_MODELOBJECT(CoilHeatingFourPipeBeam, 1);
SWIG_MODELOBJECT(CoilHeatingGas,1);
SWIG_MODELOBJECT(CoilHeatingGasMultiStage,1);
SWIG_MODELOBJECT(CoilHeatingLowTempRadiantConstFlow,1);
SWIG_MODELOBJECT(CoilHeatingLowTempRadiantVarFlow,1);
SWIG_MODELOBJECT(CoilHeatingWaterBaseboard,1);
SWIG_MODELOBJECT(CoilHeatingWaterBaseboardRadiant,1);
SWIG_MODELOBJECT(CoilSystemCoolingDXHeatExchangerAssisted,1);
SWIG_MODELOBJECT(CoilSystemCoolingWaterHeatExchangerAssisted,1);
SWIG_MODELOBJECT(CoilSystemIntegratedHeatPumpAirSource,1);
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
SWIG_MODELOBJECT(FanComponentModel,1);
SWIG_MODELOBJECT(FanConstantVolume,1);
SWIG_MODELOBJECT(FanOnOff,1);
SWIG_MODELOBJECT(FanSystemModel,1);
SWIG_MODELOBJECT(FanVariableVolume,1);
SWIG_MODELOBJECT(FluidCoolerSingleSpeed,1);
SWIG_MODELOBJECT(FluidCoolerTwoSpeed,1);
SWIG_MODELOBJECT(GroundHeatExchangerHorizontalTrench,1);
SWIG_MODELOBJECT(GroundHeatExchangerVertical,1);
SWIG_MODELOBJECT(HeaderedPumpsConstantSpeed,1);
SWIG_MODELOBJECT(HeaderedPumpsVariableSpeed,1);
SWIG_MODELOBJECT(HeatPumpAirToWaterFuelFiredHeating,1);
SWIG_MODELOBJECT(HeatPumpAirToWaterFuelFiredCooling,1);
SWIG_MODELOBJECT(HumidifierSteamElectric,1);
SWIG_MODELOBJECT(HumidifierSteamGas,1);
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

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
  %inline {
    namespace openstudio {
      namespace model {
        OptionalWaterUseConnections waterUseConnections(const openstudio::model::WaterUseEquipment& weq){
          return weq.waterUseConnections();
        }

        OptionalCoilCoolingDXMultiSpeed parentCoil(const openstudio::model::CoilCoolingDXMultiSpeedStageData& stage){
          return stage.parentCoil();
        }

        CoilCoolingDXVector coilCoolingDXs(const openstudio::model::CoilCoolingDXCurveFitPerformance& perf) {
          return perf.coilCoolingDXs();
        }

        OptionalCoilHeatingDXMultiSpeed parentCoil(const openstudio::model::CoilHeatingDXMultiSpeedStageData& stage){
          return stage.parentCoil();
        }

        OptionalCoilHeatingGasMultiStage parentCoil(const openstudio::model::CoilHeatingGasMultiStageStageData& stage){
          return stage.parentCoil();
        }

        OptionalCoilHeatingElectricMultiStage parentCoil(const openstudio::model::CoilHeatingElectricMultiStageStageData& stage){
          return stage.parentCoil();
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

    public partial class WaterUseEquipment : SpaceLoadInstance
    {
      public OptionalWaterUseConnections waterUseConnections() {
        return OpenStudio.OpenStudioModelStraightComponent.waterUseConnections(this);
      }
    }

    public partial class CoilCoolingDXMultiSpeedStageData : ParentObject
    {
      public OptionalCoilCoolingDXMultiSpeed parentCoil() {
        return OpenStudio.OpenStudioModelStraightComponent.parentCoil(this);
      }
    }

    public partial class CoilCoolingDXCurveFitPerformance : ResourceObject
    {
      public CoilCoolingDXVector coilCoolingDXs() {
        return OpenStudio.OpenStudioModelStraightComponent.coilCoolingDXs(this);
      }
    }

    public partial class CoilHeatingDXMultiSpeedStageData : ParentObject
    {
      public OptionalCoilHeatingDXMultiSpeed parentCoil() {
        return OpenStudio.OpenStudioModelStraightComponent.parentCoil(this);
      }
    }

    public partial class CoilHeatingGasMultiStageStageData : ModelObject
    {
      public OptionalCoilHeatingGasMultiStage parentCoil() {
        return OpenStudio.OpenStudioModelStraightComponent.parentCoil(this);
      }
    }

    public partial class CoilHeatingElectricMultiStageStageData : ModelObject
    {
      public OptionalCoilHeatingElectricMultiStage parentCoil() {
        return OpenStudio.OpenStudioModelStraightComponent.parentCoil(this);
      }
    }

  %}
#endif
#endif
