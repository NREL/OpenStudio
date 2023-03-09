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

// We ignore that, should instead call ZoneHVACEquipment::addToThermalZone to avoid issues
%ignore openstudio::model::ThermalZone::addEquipment;

%{
  #include <model/HVACTemplates.hpp>
%}

#if defined SWIGCSHARP

  #undef _csharp_module_name
  #define _csharp_module_name OpenStudioModelHVAC


  // ignore airflow objects for now, add back in with partial classes in ModelAirflow.i
  %ignore openstudio::model::AirLoopHVACOutdoorAirSystem::getAirflowNetworkDistributionNode;
  %ignore openstudio::model::AirLoopHVACOutdoorAirSystem::airflowNetworkDistributionNode;
  %ignore openstudio::model::AirLoopHVACZoneMixer::getAirflowNetworkDistributionNode;
  %ignore openstudio::model::AirLoopHVACZoneMixer::airflowNetworkDistributionNode;
  %ignore openstudio::model::AirLoopHVACZoneSplitter::getAirflowNetworkDistributionNode;
  %ignore openstudio::model::AirLoopHVACZoneSplitter::airflowNetworkDistributionNode;
  %ignore openstudio::model::CoilCoolingWater::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilCoolingWater::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingWater::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::CoilHeatingWater::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::ControllerOutdoorAir::getAirflowNetworkOutdoorAirflow;
  %ignore openstudio::model::ControllerOutdoorAir::airflowNetworkOutdoorAirflow;
  %ignore openstudio::model::HeatExchangerAirToAirSensibleAndLatent::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::HeatExchangerAirToAirSensibleAndLatent::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::HeatExchangerDesiccantBalancedFlow::getAirflowNetworkEquivalentDuct;
  %ignore openstudio::model::HeatExchangerDesiccantBalancedFlow::airflowNetworkEquivalentDuct;
  %ignore openstudio::model::Node::getAirflowNetworkDistributionNode;
  %ignore openstudio::model::Node::airflowNetworkDistributionNode;
  %ignore openstudio::model::ThermalZone::getAirflowNetworkZone;
  %ignore openstudio::model::ThermalZone::airflowNetworkZone;

  // PlantLoop has some methods that actually live in ModelPlantEquipmentOperationScheme.i (which depends on ModelHVAC.i)
  // So we ignore for now, and reimplment later in the PlantEq.i file with partial classes
  %ignore openstudio::model::PlantLoop::plantEquipmentOperationHeatingLoad;
  %ignore openstudio::model::PlantLoop::setPlantEquipmentOperationHeatingLoad;
  %ignore openstudio::model::PlantLoop::plantEquipmentOperationCoolingLoad;
  %ignore openstudio::model::PlantLoop::setPlantEquipmentOperationCoolingLoad;

  // WaterUseConnections is defined in ModelStraightComponent.i (which depends on this file)
  %ignore openstudio::model::WaterUseEquipment::waterUseConnections;

  // Defined in ModelStraightComponent.i
  %ignore openstudio::model::CoilCoolingDXMultiSpeedStageData::parentCoil;
  %ignore openstudio::model::CoilHeatingDXMultiSpeedStageData::parentCoil;
  %ignore openstudio::model::CoilHeatingGasMultiStageStageData::parentCoil;
  %ignore openstudio::model::CoilHeatingElectricMultiStageStageData::parentCoil;

#endif

#if defined SWIGPYTHON
  %pythoncode %{
    Model = openstudiomodelcore.Model
  %}
#endif

namespace openstudio {
namespace model {

// forward declarations
%feature("valuewrapper") AirflowNetworkDistributionNode;
%feature("valuewrapper") AirflowNetworkZone;
%feature("valuewrapper") AirflowNetworkEquivalentDuct;
%feature("valuewrapper") AirflowNetworkFan;
%feature("valuewrapper") AirflowNetworkOutdoorAirflow;
%feature("valuewrapper") AirflowNetworkZoneExhaustFan;
%feature("valuewrapper") PlantEquipmentOperationHeatingLoad;
%feature("valuewrapper") PlantEquipmentOperationCoolingLoad;
%feature("valuewrapper") WaterUseConnections;
%feature("valuewrapper") CoilCoolingDXMultiSpeed;
%feature("valuewrapper") CoilHeatingDXMultiSpeed;
%feature("valuewrapper") CoilHeatingGasMultiStage;
%feature("valuewrapper") CoilHeatingElectricMultiStage;

class AirflowNetworkDistributionNode;
class AirflowNetworkZone;
class AirflowNetworkEquivalentDuct;
class AirflowNetworkFan;
class AirflowNetworkOutdoorAirflow;
class AirflowNetworkZoneExhaustFan;
class PlantEquipmentOperationHeatingLoad;
class PlantEquipmentOperationCoolingLoad;
class WaterUseConnections;
class CoilCoolingDXMultiSpeed;
class CoilHeatingDXMultiSpeed;
class CoilHeatingGasMultiStage;
class CoilHeatingElectricMultiStage;

}
}

%extend openstudio::model::TransitionZone {
  // Use the overloaded operator<< for string representation
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
};

%ignore std::vector<openstudio::model::SupplyAirflowRatioField>::vector(size_type);
%ignore std::vector<openstudio::model::SupplyAirflowRatioField>::resize(size_type);
%template(SupplyAirflowRatioFieldVector) std::vector<openstudio::model::SupplyAirflowRatioField>;

//%ignore std::vector<openstudio::model::GFunction>::vector(size_type);
//%ignore std::vector<openstudio::model::GFunction>::resize(size_type);
//%template(GFunctionVector) std::vector<openstudio::model::GFunction>;

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
MODELOBJECT_TEMPLATES(TransitionZone);
MODELOBJECT_TEMPLATES(ThermostatSetpointDualSetpoint);
MODELOBJECT_TEMPLATES(ZoneControlContaminantController);
MODELOBJECT_TEMPLATES(ZoneControlHumidistat);
MODELOBJECT_TEMPLATES(ZoneControlThermostatStagedDualSetpoint);
MODELOBJECT_TEMPLATES(AirLoopHVAC);
MODELOBJECT_TEMPLATES(SizingPlant);
MODELOBJECT_TEMPLATES(PlantLoop);
MODELOBJECT_TEMPLATES(PlantEquipmentOperationScheme);
MODELOBJECT_TEMPLATES(PlantEquipmentOperationRangeBasedScheme);
MODELOBJECT_TEMPLATES(PlantComponentUserDefined);
MODELOBJECT_TEMPLATES(AirLoopHVACOutdoorAirSystem);
MODELOBJECT_TEMPLATES(AirLoopHVACDedicatedOutdoorAirSystem);
MODELOBJECT_TEMPLATES(ControllerMechanicalVentilation);
MODELOBJECT_TEMPLATES(ControllerOutdoorAir);
MODELOBJECT_TEMPLATES(ControllerWaterCoil);
MODELOBJECT_TEMPLATES(Mixer);
MODELOBJECT_TEMPLATES(AirLoopHVACReturnPlenum);
MODELOBJECT_TEMPLATES(AirLoopHVACZoneMixer);
MODELOBJECT_TEMPLATES(ConnectorMixer);
MODELOBJECT_TEMPLATES(AvailabilityManagerAssignmentList);
MODELOBJECT_TEMPLATES(AvailabilityManager);
MODELOBJECT_TEMPLATES(Splitter);
MODELOBJECT_TEMPLATES(AirLoopHVACSupplyPlenum);
MODELOBJECT_TEMPLATES(AirLoopHVACZoneSplitter);
MODELOBJECT_TEMPLATES(ConnectorSplitter);
MODELOBJECT_TEMPLATES(AirConditionerVariableRefrigerantFlowFluidTemperatureControl);
MODELOBJECT_TEMPLATES(AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR);
MODELOBJECT_TEMPLATES(AirTerminalDualDuctConstantVolume);
MODELOBJECT_TEMPLATES(AirTerminalDualDuctVAV);
MODELOBJECT_TEMPLATES(AirTerminalDualDuctVAVOutdoorAir);
MODELOBJECT_TEMPLATES(CentralHeatPumpSystem);
MODELOBJECT_TEMPLATES(CentralHeatPumpSystemModule);
MODELOBJECT_TEMPLATES(ChillerHeaterPerformanceElectricEIR);
MODELOBJECT_TEMPLATES(ChillerElectricASHRAE205);
MODELOBJECT_TEMPLATES(ChillerElectricEIR);
MODELOBJECT_TEMPLATES(ChillerElectricReformulatedEIR);
MODELOBJECT_TEMPLATES(CoilCoolingDXMultiSpeedStageData);
MODELOBJECT_TEMPLATES(CoilCoolingDXVariableRefrigerantFlow);
MODELOBJECT_TEMPLATES(CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl);
MODELOBJECT_TEMPLATES(CoilCoolingDXVariableSpeedSpeedData);
MODELOBJECT_TEMPLATES(CoilCoolingWater);
MODELOBJECT_TEMPLATES(CoilCoolingWaterToAirHeatPumpEquationFit);
MODELOBJECT_TEMPLATES(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit);
MODELOBJECT_TEMPLATES(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData);
MODELOBJECT_TEMPLATES(CoilHeatingDXMultiSpeedStageData);
MODELOBJECT_TEMPLATES(CoilHeatingDXVariableRefrigerantFlow);
MODELOBJECT_TEMPLATES(CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl);
MODELOBJECT_TEMPLATES(CoilHeatingGasMultiStageStageData);
MODELOBJECT_TEMPLATES(CoilHeatingElectricMultiStageStageData);
MODELOBJECT_TEMPLATES(CoilHeatingDXVariableSpeedSpeedData);
MODELOBJECT_TEMPLATES(CoilHeatingWater);
MODELOBJECT_TEMPLATES(CoilHeatingWaterToAirHeatPumpEquationFit);
MODELOBJECT_TEMPLATES(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit);
MODELOBJECT_TEMPLATES(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData);
MODELOBJECT_TEMPLATES(CoilPerformanceDXCooling);
MODELOBJECT_TEMPLATES(CoilUserDefined);
MODELOBJECT_TEMPLATES(CoilWaterHeatingAirToWaterHeatPump);
MODELOBJECT_TEMPLATES(CoilWaterHeatingAirToWaterHeatPumpVariableSpeed);
MODELOBJECT_TEMPLATES(CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData);
MODELOBJECT_TEMPLATES(CoilWaterHeatingAirToWaterHeatPumpWrapped);
MODELOBJECT_TEMPLATES(CoolingTowerPerformanceCoolTools);
MODELOBJECT_TEMPLATES(CoolingTowerPerformanceYorkCalc);
MODELOBJECT_TEMPLATES(UnitarySystemPerformanceMultispeed);
MODELOBJECT_TEMPLATES(ZoneMixing);
MODELOBJECT_TEMPLATES(ZoneVentilationDesignFlowRate);
MODELOBJECT_TEMPLATES(ZoneVentilationWindandStackOpenArea);
MODELOBJECT_TEMPLATES(WaterHeaterMixed);
MODELOBJECT_TEMPLATES(WaterHeaterStratified);
MODELOBJECT_TEMPLATES(WaterHeaterSizing);
MODELOBJECT_TEMPLATES(WaterUseEquipmentDefinition);
MODELOBJECT_TEMPLATES(WaterUseEquipment);
MODELOBJECT_TEMPLATES(HeatExchangerAirToAirSensibleAndLatent);
MODELOBJECT_TEMPLATES(HeatExchangerDesiccantBalancedFlow);
MODELOBJECT_TEMPLATES(HeatExchangerFluidToFluid);
MODELOBJECT_TEMPLATES(HeatPumpWaterToWaterEquationFitCooling);
MODELOBJECT_TEMPLATES(HeatPumpWaterToWaterEquationFitHeating);
MODELOBJECT_TEMPLATES(HeatPumpPlantLoopEIRCooling);
MODELOBJECT_TEMPLATES(HeatPumpPlantLoopEIRHeating);
MODELOBJECT_TEMPLATES(ThermalStorageChilledWaterStratified);
MODELOBJECT_TEMPLATES(ChillerAbsorptionIndirect);
MODELOBJECT_TEMPLATES(ChillerAbsorption);
MODELOBJECT_TEMPLATES(SolarCollectorPerformanceFlatPlate);
MODELOBJECT_TEMPLATES(SolarCollectorPerformanceIntegralCollectorStorage);
MODELOBJECT_TEMPLATES(SolarCollectorPerformancePhotovoltaicThermalBIPVT);
MODELOBJECT_TEMPLATES(SolarCollectorPerformancePhotovoltaicThermalSimple);
MODELOBJECT_TEMPLATES(SwimmingPoolIndoor);

MODELOBJECT_TEMPLATES(SetpointManagerFollowOutdoorAirTemperature);
MODELOBJECT_TEMPLATES(SetpointManagerFollowSystemNodeTemperature);
MODELOBJECT_TEMPLATES(SetpointManagerMixedAir);
MODELOBJECT_TEMPLATES(SetpointManagerMultiZoneCoolingAverage);
MODELOBJECT_TEMPLATES(SetpointManagerMultiZoneHeatingAverage);
MODELOBJECT_TEMPLATES(SetpointManagerMultiZoneHumidityMaximum);
MODELOBJECT_TEMPLATES(SetpointManagerMultiZoneHumidityMinimum);
MODELOBJECT_TEMPLATES(SetpointManagerMultiZoneMaximumHumidityAverage);
MODELOBJECT_TEMPLATES(SetpointManagerMultiZoneMinimumHumidityAverage);
MODELOBJECT_TEMPLATES(SetpointManagerOutdoorAirPretreat);
MODELOBJECT_TEMPLATES(SetpointManagerOutdoorAirReset);
MODELOBJECT_TEMPLATES(SetpointManagerScheduled);
MODELOBJECT_TEMPLATES(SetpointManagerScheduledDualSetpoint);
MODELOBJECT_TEMPLATES(SetpointManagerSingleZoneHumidityMaximum);
MODELOBJECT_TEMPLATES(SetpointManagerSingleZoneHumidityMinimum);
MODELOBJECT_TEMPLATES(SetpointManagerSingleZoneOneStageCooling);
MODELOBJECT_TEMPLATES(SetpointManagerSingleZoneOneStageHeating);
MODELOBJECT_TEMPLATES(SetpointManagerSingleZoneCooling);
MODELOBJECT_TEMPLATES(SetpointManagerSingleZoneHeating);
MODELOBJECT_TEMPLATES(SetpointManagerSingleZoneReheat);
MODELOBJECT_TEMPLATES(SetpointManagerWarmest);
MODELOBJECT_TEMPLATES(SetpointManagerWarmestTemperatureFlow);
MODELOBJECT_TEMPLATES(SetpointManagerColdest);
MODELOBJECT_TEMPLATES(SetpointManagerFollowGroundTemperature);
MODELOBJECT_TEMPLATES(SetpointManagerSystemNodeResetHumidity);
MODELOBJECT_TEMPLATES(SetpointManagerSystemNodeResetTemperature);
MODELOBJECT_TEMPLATES(LoadingIndex);

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
SWIG_MODELOBJECT(ZoneControlContaminantController,1);
SWIG_MODELOBJECT(ZoneControlHumidistat,1);
SWIG_MODELOBJECT(ZoneControlThermostatStagedDualSetpoint,1);
SWIG_MODELOBJECT(AirLoopHVAC, 1);
SWIG_MODELOBJECT(SizingPlant, 1);
SWIG_MODELOBJECT(PlantLoop, 1);
SWIG_MODELOBJECT(PlantEquipmentOperationScheme, 0);
SWIG_MODELOBJECT(PlantEquipmentOperationRangeBasedScheme, 0);
SWIG_MODELOBJECT(PlantComponentUserDefined, 1);
SWIG_MODELOBJECT(AirLoopHVACOutdoorAirSystem, 1);
SWIG_MODELOBJECT(AirLoopHVACDedicatedOutdoorAirSystem, 1);
SWIG_MODELOBJECT(ControllerMechanicalVentilation, 1);
SWIG_MODELOBJECT(ControllerOutdoorAir, 1);
SWIG_MODELOBJECT(ControllerWaterCoil, 1);
SWIG_MODELOBJECT(Mixer, 0);
SWIG_MODELOBJECT(AirLoopHVACReturnPlenum, 1);
SWIG_MODELOBJECT(AirLoopHVACZoneMixer, 1);
SWIG_MODELOBJECT(ConnectorMixer, 1);
SWIG_MODELOBJECT(AvailabilityManagerAssignmentList, 1);
SWIG_MODELOBJECT(AvailabilityManager, 0);
SWIG_MODELOBJECT(Splitter, 0);
SWIG_MODELOBJECT(AirLoopHVACSupplyPlenum, 1);
SWIG_MODELOBJECT(AirLoopHVACZoneSplitter, 1);
SWIG_MODELOBJECT(ConnectorSplitter, 1);
SWIG_MODELOBJECT(AirConditionerVariableRefrigerantFlowFluidTemperatureControl,1);
SWIG_MODELOBJECT(AirConditionerVariableRefrigerantFlowFluidTemperatureControlHR,1);
SWIG_MODELOBJECT(AirTerminalDualDuctConstantVolume, 1);
SWIG_MODELOBJECT(AirTerminalDualDuctVAV, 1);
SWIG_MODELOBJECT(AirTerminalDualDuctVAVOutdoorAir, 1);
SWIG_MODELOBJECT(CentralHeatPumpSystem, 1);
SWIG_MODELOBJECT(CentralHeatPumpSystemModule, 1);
SWIG_MODELOBJECT(ChillerHeaterPerformanceElectricEIR, 1);
SWIG_MODELOBJECT(ChillerElectricASHRAE205, 1);
SWIG_MODELOBJECT(ChillerElectricEIR, 1);
SWIG_MODELOBJECT(ChillerElectricReformulatedEIR, 1);
SWIG_MODELOBJECT(CoilCoolingDXMultiSpeedStageData, 1);
SWIG_MODELOBJECT(CoilCoolingDXVariableRefrigerantFlow, 1);
SWIG_MODELOBJECT(CoilCoolingDXVariableRefrigerantFlowFluidTemperatureControl, 1);
SWIG_MODELOBJECT(CoilCoolingDXVariableSpeedSpeedData, 1);
SWIG_MODELOBJECT(CoilCoolingWater, 1);
SWIG_MODELOBJECT(CoilCoolingWaterToAirHeatPumpEquationFit, 1);
SWIG_MODELOBJECT(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit, 1);
SWIG_MODELOBJECT(CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData, 1);
SWIG_MODELOBJECT(CoilHeatingDXMultiSpeedStageData, 1);
SWIG_MODELOBJECT(CoilHeatingDXVariableRefrigerantFlow, 1);
SWIG_MODELOBJECT(CoilHeatingDXVariableRefrigerantFlowFluidTemperatureControl, 1);
SWIG_MODELOBJECT(CoilHeatingGasMultiStageStageData, 1);
SWIG_MODELOBJECT(CoilHeatingElectricMultiStageStageData, 1);
SWIG_MODELOBJECT(CoilHeatingDXVariableSpeedSpeedData, 1);
SWIG_MODELOBJECT(CoilHeatingWater, 1);
SWIG_MODELOBJECT(CoilHeatingWaterToAirHeatPumpEquationFit, 1);
SWIG_MODELOBJECT(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit, 1);
SWIG_MODELOBJECT(CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData, 1);
SWIG_MODELOBJECT(CoilPerformanceDXCooling, 1);
SWIG_MODELOBJECT(CoilUserDefined, 1);
SWIG_MODELOBJECT(CoilWaterHeatingAirToWaterHeatPump, 1);
SWIG_MODELOBJECT(CoilWaterHeatingAirToWaterHeatPumpVariableSpeed, 1);
SWIG_MODELOBJECT(CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData, 1);
SWIG_MODELOBJECT(CoilWaterHeatingAirToWaterHeatPumpWrapped, 1);
SWIG_MODELOBJECT(CoolingTowerPerformanceYorkCalc, 1);
SWIG_MODELOBJECT(CoolingTowerPerformanceCoolTools, 1);
SWIG_MODELOBJECT(UnitarySystemPerformanceMultispeed, 1);
SWIG_MODELOBJECT(ZoneMixing, 1);
SWIG_MODELOBJECT(ZoneVentilationDesignFlowRate, 1);
SWIG_MODELOBJECT(ZoneVentilationWindandStackOpenArea, 1);
SWIG_MODELOBJECT(WaterHeaterMixed, 1);
SWIG_MODELOBJECT(WaterHeaterStratified, 1);
SWIG_MODELOBJECT(WaterHeaterSizing, 1);
SWIG_MODELOBJECT(WaterUseEquipmentDefinition, 1);
SWIG_MODELOBJECT(WaterUseEquipment, 1);
SWIG_MODELOBJECT(HeatExchangerAirToAirSensibleAndLatent, 1);
SWIG_MODELOBJECT(HeatExchangerDesiccantBalancedFlow, 1);
SWIG_MODELOBJECT(HeatExchangerFluidToFluid,1);
SWIG_MODELOBJECT(HeatPumpWaterToWaterEquationFitCooling,1);
SWIG_MODELOBJECT(HeatPumpWaterToWaterEquationFitHeating,1);
SWIG_MODELOBJECT(HeatPumpPlantLoopEIRCooling,1);
SWIG_MODELOBJECT(HeatPumpPlantLoopEIRHeating,1);
SWIG_MODELOBJECT(ThermalStorageChilledWaterStratified, 1);
SWIG_MODELOBJECT(ChillerAbsorptionIndirect, 1);
SWIG_MODELOBJECT(ChillerAbsorption, 1);
SWIG_MODELOBJECT(SolarCollectorPerformanceFlatPlate, 1);
SWIG_MODELOBJECT(SolarCollectorPerformanceIntegralCollectorStorage, 1);
SWIG_MODELOBJECT(SolarCollectorPerformancePhotovoltaicThermalBIPVT, 1);
SWIG_MODELOBJECT(SolarCollectorPerformancePhotovoltaicThermalSimple, 1);
SWIG_MODELOBJECT(SwimmingPoolIndoor,1);

SWIG_MODELOBJECT(SetpointManagerFollowOutdoorAirTemperature,1);
SWIG_MODELOBJECT(SetpointManagerFollowSystemNodeTemperature,1);
SWIG_MODELOBJECT(SetpointManagerMixedAir,1);
SWIG_MODELOBJECT(SetpointManagerMultiZoneCoolingAverage,1);
SWIG_MODELOBJECT(SetpointManagerMultiZoneHeatingAverage,1);
SWIG_MODELOBJECT(SetpointManagerMultiZoneHumidityMaximum,1);
SWIG_MODELOBJECT(SetpointManagerMultiZoneHumidityMinimum,1);
SWIG_MODELOBJECT(SetpointManagerMultiZoneMaximumHumidityAverage,1);
SWIG_MODELOBJECT(SetpointManagerMultiZoneMinimumHumidityAverage,1);
SWIG_MODELOBJECT(SetpointManagerOutdoorAirPretreat,1);
SWIG_MODELOBJECT(SetpointManagerOutdoorAirReset,1);
SWIG_MODELOBJECT(SetpointManagerScheduled,1);
SWIG_MODELOBJECT(SetpointManagerScheduledDualSetpoint,1);
SWIG_MODELOBJECT(SetpointManagerSingleZoneHumidityMaximum,1);
SWIG_MODELOBJECT(SetpointManagerSingleZoneHumidityMinimum,1);
SWIG_MODELOBJECT(SetpointManagerSingleZoneOneStageCooling,1);
SWIG_MODELOBJECT(SetpointManagerSingleZoneOneStageHeating,1);
SWIG_MODELOBJECT(SetpointManagerSingleZoneCooling,1);
SWIG_MODELOBJECT(SetpointManagerSingleZoneHeating,1);
SWIG_MODELOBJECT(SetpointManagerSingleZoneReheat,1);
SWIG_MODELOBJECT(SetpointManagerWarmest,1);
SWIG_MODELOBJECT(SetpointManagerWarmestTemperatureFlow,1);
SWIG_MODELOBJECT(SetpointManagerColdest,1);
SWIG_MODELOBJECT(SetpointManagerFollowGroundTemperature,1);
SWIG_MODELOBJECT(SetpointManagerSystemNodeResetHumidity,1);
SWIG_MODELOBJECT(SetpointManagerSystemNodeResetTemperature,1);
SWIG_MODELOBJECT(LoadingIndex, 1);

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
  %inline {
    namespace openstudio {
      namespace model {
        openstudio::model::Node getOutdoorAirNode(openstudio::model::Model model){
          return model.outdoorAirNode();
        }
        std::vector<openstudio::model::ThermalZone> getThermalZones(const openstudio::model::Building& building){
          return building.thermalZones();
        }
        boost::optional<openstudio::model::ThermalZone> getThermalZone(const openstudio::model::Space& space){
          return space.thermalZone();
        }
        bool setThermalZone(openstudio::model::Space space, openstudio::model::ThermalZone thermalZone){
          return space.setThermalZone(thermalZone);
        }

        openstudio::model::ThermalZone getThermalZone(const openstudio::model::ZonePropertyUserViewFactorsBySurfaceName& zoneProp){
          return zoneProp.thermalZone();
        }

        // EMS Actuator setter for ThermalZone (reimplemented from ModelCore.i)
        bool setThermalZoneForEMSActuator(openstudio::model::EnergyManagementSystemActuator actuator, openstudio::model::ThermalZone thermalZone) {
          return actuator.setThermalZone(thermalZone);
        }

        // Reimplemented from ModelSimulation.i
        std::vector<openstudio::model::ThermalZone> getShadingZoneGroup(const openstudio::model::ShadowCalculation& sc, unsigned groupIndex) {
          return sc.getShadingZoneGroup(groupIndex);
        }
        bool addShadingZoneGroup(openstudio::model::ShadowCalculation sc, const std::vector<openstudio::model::ThermalZone>& thermalZones) {
          return sc.addShadingZoneGroup(thermalZones);
        }

        // Reimplemented from ModelResources.i
        std::vector<openstudio::model::HeatExchangerDesiccantBalancedFlow> getHeatExchangerDesiccantBalancedFlows(
            const openstudio::model::HeatExchangerDesiccantBalancedFlowPerformanceDataType1& hxData)
        {
          return hxData.heatExchangerDesiccantBalancedFlows();
        }

        // DaylightingDeviceTubular, reimplemented from ModelGeometry.i
        std::vector<TransitionZone> transitionZones(const openstudio::model::DaylightingDeviceTubular& tubular) {
          return tubular.transitionZones();
        }
        bool addTransitionZone(openstudio::model::DaylightingDeviceTubular tubular, const TransitionZone& transitionZone) {
          return tubular.addTransitionZone(transitionZone);
        }

        bool addTransitionZone(openstudio::model::DaylightingDeviceTubular tubular, const ThermalZone& zone, double length) {
          return tubular.addTransitionZone(zone, length);
        }
        bool addTransitionZones(openstudio::model::DaylightingDeviceTubular tubular, const std::vector<TransitionZone>& transitionZones) {
          return tubular.addTransitionZones(transitionZones);
        }

      } // namespace model
    } // namespace openstudio
  } // %inline
#endif

#if defined(SWIGCSHARP)
  //%pragma(csharp) imclassimports=%{
  %pragma(csharp) moduleimports=%{

    using System;
    using System.Runtime.InteropServices;

    public partial class Model : Workspace {
      public Node outdoorAirNode()
      {
        return OpenStudio.OpenStudioModelHVAC.getOutdoorAirNode(this);
      }
    }

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

    public partial class ZonePropertyUserViewFactorsBySurfaceName : ModelObject {
      public ThermalZone thermalZone()
      {
        return OpenStudio.OpenStudioModelHVAC.getThermalZone(this);
      }
    }

    public partial class EnergyManagementSystemActuator : ModelObject {
      public bool setThermalZone(OpenStudio.ThermalZone thermalZone) {
        return OpenStudio.OpenStudioModelHVAC.setThermalZoneForEMSActuator(this, thermalZone);
      }

      // Overloaded Ctor, calling Ctor that doesn't use ThermalZone
      public EnergyManagementSystemActuator(ModelObject modelObject, string actuatedComponentType, string actuatedComponentControlType, OpenStudio.ThermalZone thermalZone)
        : this(modelObject, actuatedComponentType, actuatedComponentControlType) {
        this.setThermalZone(thermalZone);
      }
    }

    public partial class ShadowCalculation : ModelObject {
      public ThermalZoneVector getShadingZoneGroup(uint groupIndex) {
        return OpenStudio.OpenStudioModelHVAC.getShadingZoneGroup(this, groupIndex);
      }
      public bool addShadingZoneGroup(ThermalZoneVector thermalZones) {
        return OpenStudio.OpenStudioModelHVAC.addShadingZoneGroup(this, thermalZones);
      }
    }

    public partial class HeatExchangerDesiccantBalancedFlowPerformanceDataType1 : ResourceObject {
      public HeatExchangerDesiccantBalancedFlowVector heatExchangerDesiccantBalancedFlows() {
        return OpenStudio.OpenStudioModelHVAC.getHeatExchangerDesiccantBalancedFlows(this);
      }
    }

    public partial class DaylightingDeviceTubular : ModelObject {

      public TransitionZoneVector transitionZones(){
        return OpenStudio.OpenStudioModelHVAC.transitionZones(this);
      }

      public bool addTransitionZone(OpenStudio.TransitionZone transitionZone) {
        return OpenStudio.OpenStudioModelHVAC.addTransitionZone(this, transitionZone);
      }

      public bool addTransitionZone(OpenStudio.ThermalZone zone, double length) {
        return OpenStudio.OpenStudioModelHVAC.addTransitionZone(this, zone, length);
      }

      public bool addTransitionZone(OpenStudio.TransitionZoneVector transitionZones) {
        return OpenStudio.OpenStudioModelHVAC.addTransitionZones(this, transitionZones);
      }
    }

  %}
#endif

%include <model/HVACTemplates.hpp>

#endif
