/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef ENERGYPLUS_FORWARDTRANSLATOR_HPP
#define ENERGYPLUS_FORWARDTRANSLATOR_HPP

#include "EnergyPlusAPI.hpp"
#include "../model/Model.hpp"
#include "../model/ConstructionBase.hpp"
#include "../model/HVACComponent.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/StringStreamLogSink.hpp"
#include "../utilities/time/Time.hpp"

namespace openstudio {

class ProgressBar;
class Transformation;

namespace model{

class AirConditionerVariableRefrigerantFlow;
class AirGap;
class AirLoopHVAC;
class AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass;
class AirLoopHVACUnitaryHeatPumpAirToAir;
class AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed;
class AirLoopHVACUnitarySystem;
class AirLoopHVACReturnPlenum;
class AirLoopHVACSupplyPlenum;
class AirLoopHVACZoneSplitter;
class AirTerminalDualDuctVAV;
class AirTerminalSingleDuctConstantVolumeCooledBeam;
class AirTerminalSingleDuctConstantVolumeFourPipeInduction;
class AirTerminalSingleDuctConstantVolumeReheat;
class AirTerminalSingleDuctParallelPIUReheat;
class AirTerminalSingleDuctSeriesPIUReheat;
class AirTerminalSingleDuctUncontrolled;
class AirTerminalSingleDuctVAVNoReheat;
class AirTerminalSingleDuctVAVReheat;
class AirTerminalSingleDuctInletSideMixer;
class AirTerminalSingleDuctVAVHeatAndCoolNoReheat;
class AirTerminalSingleDuctVAVHeatAndCoolReheat;
class AirLoopHVACZoneMixer;
class AirLoopHVACOutdoorAirSystem;
class AirWallMaterial;
class AvailabilityManagerHybridVentilation;
class AvailabilityManagerOptimumStart;
class AvailabilityManagerDifferentialThermostat;
class AvailabilityManagerNightVentilation;
class AvailabilityManagerNightCycle;
class Blind;
class Building;
class BoilerHotWater;
class BoilerSteam;
class CentralHeatPumpSystem;
class CFactorUndergroundWallConstruction;
class ChillerAbsorption;
class ChillerAbsorptionIndirect;
class ChillerElectricEIR;
class ChillerHeaterPerformanceElectricEIR;
class CoilCoolingDXMultiSpeed;
class CoilCoolingDXSingleSpeed;
class CoilCoolingDXTwoSpeed;
class CoilCoolingDXTwoStageWithHumidityControlMode;
class CoilCoolingDXVariableRefrigerantFlow;
class CoilCoolingDXVariableSpeed;
class CoilCoolingWater;
class CoilCoolingWaterToAirHeatPumpEquationFit;
class CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit;
class CoilHeatingDesuperheater;
class CoilHeatingDXMultiSpeed;
class CoilHeatingDXSingleSpeed;
class CoilHeatingDXVariableRefrigerantFlow;
class CoilHeatingDXVariableSpeed;
class CoilHeatingElectric;
class CoilHeatingGas;
class CoilHeatingGasMultiStage;
class CoilHeatingWater;
class CoilHeatingWaterToAirHeatPumpEquationFit;
class CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit;
class CoilPerformanceDXCooling;
class CoilSystemCoolingDXHeatExchangerAssisted;
class CoilSystemCoolingWaterHeatExchangerAssisted;
class CoilWaterHeatingDesuperheater;
class CoilWaterHeatingAirToWaterHeatPump;
class CoilWaterHeatingAirToWaterHeatPumpWrapped;
class Construction;
class ConstructionWithInternalSource;
class ControllerOutdoorAir;
class ControllerMechanicalVentilation;
class ControllerWaterCoil;
class ConvergenceLimits;
class CoolingTowerPerformanceCoolTools;
class CoolingTowerPerformanceYorkCalc;
class CoolingTowerSingleSpeed;
class CoolingTowerTwoSpeed;
class CoolingTowerVariableSpeed;
class CurrencyType;
class CurveBicubic;
class CurveBiquadratic;
class CurveCubic;
class CurveDoubleExponentialDecay;
class CurveExponent;
class CurveExponentialDecay;
class CurveExponentialSkewNormal;
class CurveFanPressureRise;
class CurveFunctionalPressureDrop;
class CurveLinear;
class CurveQuadratic;
class CurveQuadraticLinear;
class CurveQuartic;
class CurveRectangularHyperbola1;
class CurveRectangularHyperbola2;
class CurveSigmoid;
class CurveTriquadratic;
class DaylightingControl;
class DaylightingDeviceShelf;
class DaylightRedirectionDevice;
class DesignDay;
class DesignSpecificationOutdoorAir;
class DistrictCooling;
class DistrictHeating;
class Duct;
class ElectricEquipment;
class ElectricLoadCenterDistribution;
class ElectricLoadCenterInverterLookUpTable;
class ElectricLoadCenterInverterSimple;
class ElectricLoadCenterStorageSimple;
class ElectricLoadCenterStorageConverter;
class EnergyManagementSystemActuator;
class EnergyManagementSystemConstructionIndexVariable;
class EnergyManagementSystemCurveOrTableIndexVariable;
class EnergyManagementSystemGlobalVariable;
class EnergyManagementSystemInternalVariable;
class EnergyManagementSystemMeteredOutputVariable;
class EnergyManagementSystemOutputVariable;
class EnergyManagementSystemProgram;
class EnergyManagementSystemProgramCallingManager;
class EnergyManagementSystemSensor;
class EnergyManagementSystemSubroutine;
class EnergyManagementSystemTrendVariable;
class EvaporativeCoolerDirectResearchSpecial;
class EvaporativeCoolerIndirectResearchSpecial;
class EvaporativeFluidCoolerSingleSpeed;
class EvaporativeFluidCoolerTwoSpeed;
class ExteriorLights;
class FanConstantVolume;
class FanOnOff;
class FanVariableVolume;
class FanZoneExhaust;
class FFactorGroundFloorConstruction;
class FluidCoolerSingleSpeed;
class FluidCoolerTwoSpeed;
class Gas;
class GasEquipment;
class GasMixture;
class GeneratorMicroTurbine;
class GeneratorFuelCell;
class GeneratorFuelCellAirSupply;
class GeneratorFuelCellAuxiliaryHeater;
class GeneratorFuelCellElectricalStorage;
class GeneratorFuelCellExhaustGasToWaterHeatExchanger;
class GeneratorFuelCellInverter;
class GeneratorFuelCellPowerModule;
class GeneratorFuelCellStackCooler;
class GeneratorFuelCellWaterSupply;
class GeneratorFuelSupply;
class GeneratorPhotovoltaic;
class GroundHeatExchangerHorizontalTrench;
class GroundHeatExchangerVertical;
class HeaderedPumpsConstantSpeed;
class HeaderedPumpsVariableSpeed;
class HeatBalanceAlgorithm;
class HeatExchangerAirToAirSensibleAndLatent;
class HeatExchangerFluidToFluid;
class HeatPumpWaterToWaterEquationFitCooling;
class HeatPumpWaterToWaterEquationFitHeating;
class HotWaterEquipment;
class HumidifierSteamElectric;
class IlluminanceMap;
class InsideSurfaceConvectionAlgorithm;
class InfraredTransparentMaterial;
class InteriorPartitionSurface;
class InteriorPartitionSurfaceGroup;
class InternalMass;
class LoadProfilePlant;
class LifeCycleCost;
class LifeCycleCostParameters;
class Lights;
class Luminaire;
class MaterialPropertyGlazingSpectralData;
class MaterialPropertyMoisturePenetrationDepthSettings;
class MasslessOpaqueMaterial;
class MeterCustom;
class MeterCustomDecrement;
class Node;
class OtherEquipment;
class OutsideSurfaceConvectionAlgorithm;
class OutputControlReportingTolerances;
class OutputMeter;
class OutputVariable;
class OutputEnergyManagementSystem;
class People;
class PhotovoltaicPerformanceEquivalentOneDiode;
class PhotovoltaicPerformanceSimple;
class PipeAdiabatic;
class PipeIndoor;
class PipeOutdoor;
class PlantLoop;
class PlantComponentTemperatureSource;
class PlantEquipmentOperationCoolingLoad;
class PlantEquipmentOperationHeatingLoad;
class PlantEquipmentOperationOutdoorDryBulb;
class PlantEquipmentOperationOutdoorWetBulb;
class PlantEquipmentOperationOutdoorDewpoint;
class PlantEquipmentOperationOutdoorRelativeHumidity;
class PlantEquipmentOperationOutdoorDryBulbDifference;
class PlantEquipmentOperationOutdoorWetBulbDifference;
class PlantEquipmentOperationOutdoorDewpointDifference;
class PortList;
class PumpConstantSpeed;
class PumpVariableSpeed;
class RefractionExtinctionGlazing;
class RefrigerationAirChiller;
class RefrigerationCase;
class RefrigerationCompressor;
class RefrigerationCondenserAirCooled;
class RefrigerationCondenserCascade;
class RefrigerationCondenserEvaporativeCooled;
class RefrigerationCondenserWaterCooled;
class RefrigerationGasCoolerAirCooled;
class RefrigerationSubcoolerLiquidSuction;
class RefrigerationSubcoolerMechanical;
class RefrigerationSecondarySystem;
class RefrigerationSystem;
class RefrigerationTranscriticalSystem;
class RefrigerationWalkIn;
class RoofVegetation;
class RunPeriod;
class RunPeriodControlDaylightSavingTime;
class RunPeriodControlSpecialDays;
class Screen;
class SetpointManagerColdest;
class SetpointManagerFollowGroundTemperature;
class SetpointManagerFollowOutdoorAirTemperature;
class SetpointManagerFollowSystemNodeTemperature;
class SetpointManagerMixedAir;
class SetpointManagerMultiZoneCoolingAverage;
class SetpointManagerMultiZoneHeatingAverage;
class SetpointManagerMultiZoneHumidityMaximum;
class SetpointManagerMultiZoneHumidityMinimum;
class SetpointManagerMultiZoneMaximumHumidityAverage;
class SetpointManagerMultiZoneMinimumHumidityAverage;
class SetpointManagerOutdoorAirPretreat;
class SetpointManagerOutdoorAirReset;
class SetpointManagerScheduled;
class SetpointManagerScheduledDualSetpoint;
class SetpointManagerSingleZoneCooling;
class SetpointManagerSingleZoneHeating;
class SetpointManagerSingleZoneHumidityMaximum;
class SetpointManagerSingleZoneHumidityMinimum;
class SetpointManagerSingleZoneOneStageCooling;
class SetpointManagerSingleZoneOneStageHeating;
class SetpointManagerSingleZoneReheat;
class SetpointManagerWarmest;
class SetpointManagerWarmestTemperatureFlow;
class ShadowCalculation;
class Shade;
class SimulationControl;
class SiteGroundReflectance;
class SiteGroundTemperatureBuildingSurface;
class SiteGroundTemperatureDeep;
class SiteGroundTemperatureFCfactorMethod;
class SiteGroundTemperatureShallow;
class SiteWaterMainsTemperature;
class SizingParameters;
class SizingPlant;
class SizingSystem;
class SizingZone;
class StandardGlazing;
class StandardOpaqueMaterial;
class SimpleGlazing;
class ScheduleCompact;
class ScheduleConstant;
class ScheduleDay;
class ScheduleFixedInterval;
class ScheduleRuleset;
class ScheduleTypeLimits;
class ScheduleVariableInterval;
class ScheduleWeek;
class ScheduleYear;
class ShadingControl;
class ShadingSurface;
class ShadingSurfaceGroup;
class Site;
class SkyTemperature;
class SolarCollectorFlatPlatePhotovoltaicThermal;
class SolarCollectorFlatPlateWater;
class SolarCollectorIntegralCollectorStorage;
class SolarCollectorPerformanceFlatPlate;
class SolarCollectorPerformanceIntegralCollectorStorage;
class SolarCollectorPerformancePhotovoltaicThermalSimple;
class Space;
class SpaceInfiltrationDesignFlowRate;
class SpaceInfiltrationEffectiveLeakageArea;
class SpaceVentilationDesignFlowRate;
class SpaceType;
class SteamEquipment;
class SubSurface;
class Surface;
class SurfacePropertyConvectionCoefficients;
class SurfacePropertyConvectionCoefficientsMultipleSurface;
class SurfacePropertyOtherSideCoefficients;
class SurfacePropertyOtherSideConditionsModel;
class TableMultiVariableLookup;
class TemperingValve;
class ThermalZone;
class ThermalStorageIceDetailed;
class ThermalStorageChilledWaterStratified;
class ThermostatSetpointDualSetpoint;
class Timestep;
class UnitarySystemPerformanceMultispeed;
class Version;
class WaterHeaterMixed;
class WaterHeaterHeatPump;
class WaterHeaterHeatPumpWrappedCondenser;
class WaterHeaterStratified;
class WaterUseConnections;
class WaterUseEquipment;
class WindowPropertyFrameAndDivider;
class ZoneAirContaminantBalance;
class ZoneAirHeatBalanceAlgorithm;
class ZoneAirMassFlowConservation;
class ZoneCapacitanceMultiplierResearchSpecial;
class ZoneControlContaminantController;
class ZoneControlHumidistat;
class ZoneControlThermostatStagedDualSetpoint;
class ZoneHVACBaseboardConvectiveElectric;
class ZoneHVACBaseboardConvectiveWater;
class ZoneHVACBaseboardRadiantConvectiveElectric;
class ZoneHVACBaseboardRadiantConvectiveWater;
class ZoneHVACDehumidifierDX;
class ZoneHVACEnergyRecoveryVentilator;
class ZoneHVACEnergyRecoveryVentilatorController;
class ZoneHVACEquipmentList;
class ZoneHVACFourPipeFanCoil;
class ZoneHVACHighTemperatureRadiant;
class ZoneHVACIdealLoadsAirSystem;
class ZoneHVACLowTemperatureRadiantElectric;
class ZoneHVACLowTempRadiantConstFlow;
class ZoneHVACLowTempRadiantVarFlow;
class ZoneHVACPackagedTerminalHeatPump;
class ZoneHVACPackagedTerminalAirConditioner;
class ZoneHVACTerminalUnitVariableRefrigerantFlow;
class ZoneHVACUnitHeater;
class ZoneHVACUnitVentilator;
class ZoneHVACWaterToAirHeatPump;
class ZoneMixing;
class ZoneVentilationDesignFlowRate;
}

namespace energyplus {

namespace detail
{
  struct ForwardTranslatorInitializer;
};

#define ENERGYPLUS_VERSION "8.8"

class ENERGYPLUS_API ForwardTranslator {
 public:

  ForwardTranslator();

  /** Translates the given Model to a Workspace.
   */
  Workspace translateModel( const model::Model & model, ProgressBar* progressBar=nullptr );

  /** Translates a ModelObject into a Workspace
   */
  Workspace translateModelObject( model::ModelObject & modelObject );

  /** Get warning messages generated by the last translation.
   */
  std::vector<LogMessage> warnings() const;

  /** Get error messages generated by the last translation.
   */
  std::vector<LogMessage> errors() const;

  /** Temporary code, use to preserve holidays in the model.
   */
  void setKeepRunControlSpecialDays(bool keepRunControlSpecialDays);

  /** If isIP, translate to a Workspace that requests Inch Pound tabular (HTML) reports from EnergyPlus
    * Use this interface with extreme caution and understanding of the impacts of reporting IP units.
    * Some assumptions in OpenStudio results processing may fail.
   */
  void setIPTabularOutput(bool isIP);

  /** If excludeLCCObjects, skip translation of life cycle cost objects.
    * Use this interface with extreme caution and understanding of the impacts.
    * Some assumptions in OpenStudio will fail if you exclude life cycle costs
    */
  void setExcludeLCCObjects(bool excludeLCCObjects);

 private:

  REGISTER_LOGGER("openstudio.energyplus.ForwardTranslator");

  /** Translates the given Model to a workspace.  If fullModelTranslation is true
   *  various "front matter" objects (such as global geometry rules and others) are added to the workspace so that it is fully
   *  prepared for simulation.
   *
   *  The method translateModelPrivate() carries out its work by explicitly translating the highest level objects in
   *  the model by calling translateAndMapModelObject().  The translateAndMapModelObject method in turn calls a type
   *  specific function to translate the given object.  Each type specific function is responsible for translating
   *  not only the direct model object passed into it, but also related model objects under its purview.  Related objects
   *  are often identified by an object list reference pointing to them, but this rule is not absolute.  For example,
   *  AirLoopHVAC is connected to other components via node connections and the translate function for AirLoopHVAC
   *  is responsible for translating the components associated with the loop.  The method translateAndMapModelObject()
   *  uses an internal map to determine if a model object has already been translated, therefore there is no
   *  concern of translating a model object twice, provided that model objects are always translated using the
   *  translateAndMapModelObject() interface as opposed to the type specific translators.
   */
  Workspace translateModelPrivate( model::Model& model, bool fullModelTranslation );

  boost::optional<IdfObject> translateAndMapModelObject( model::ModelObject & modelObject );

  boost::optional<IdfObject> translateAirConditionerVariableRefrigerantFlow( model::AirConditionerVariableRefrigerantFlow & modelObject );

  boost::optional<IdfObject> translateAirGap( model::AirGap & modelObject );

  boost::optional<IdfObject> translateAirLoopHVAC( model::AirLoopHVAC & modelObject );

  boost::optional<IdfObject> translateAirLoopHVACOutdoorAirSystem( model::AirLoopHVACOutdoorAirSystem & modelObject );

  boost::optional<IdfObject> translateAirLoopHVACUnitaryHeatCoolVAVChangeoverBypass( model::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass & modelObject );

  boost::optional<IdfObject> translateAirLoopHVACUnitaryHeatPumpAirToAir( model::AirLoopHVACUnitaryHeatPumpAirToAir & modelObject );

  boost::optional<IdfObject> translateAirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed( model::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed & modelObject );

  boost::optional<IdfObject> translateAirLoopHVACUnitarySystem( model::AirLoopHVACUnitarySystem & modelObject );

  boost::optional<IdfObject> translateAirLoopHVACReturnPlenum( model::AirLoopHVACReturnPlenum & airLoopHVACReturnPlenum );

  boost::optional<IdfObject> translateAirLoopHVACSupplyPlenum( model::AirLoopHVACSupplyPlenum & airLoopHVACSupplyPlenum );

  boost::optional<IdfObject> translateAirLoopHVACZoneMixer( model::AirLoopHVACZoneMixer & modelObject  );

  boost::optional<IdfObject> translateAirLoopHVACZoneSplitter( model::AirLoopHVACZoneSplitter & airLoopHVACZoneSplitter );

  boost::optional<IdfObject> translateAirTerminalDualDuctVAV ( model::AirTerminalDualDuctVAV & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctConstantVolumeCooledBeam ( model::AirTerminalSingleDuctConstantVolumeCooledBeam & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctConstantVolumeFourPipeInduction( model::AirTerminalSingleDuctConstantVolumeFourPipeInduction & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctConstantVolumeReheat( model::AirTerminalSingleDuctConstantVolumeReheat & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctParallelPIUReheat( model::AirTerminalSingleDuctParallelPIUReheat & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctSeriesPIUReheat( model::AirTerminalSingleDuctSeriesPIUReheat & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctUncontrolled( model::AirTerminalSingleDuctUncontrolled & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctVAVNoReheat( model::AirTerminalSingleDuctVAVNoReheat & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctVAVReheat( model::AirTerminalSingleDuctVAVReheat & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctInletSideMixer( model::AirTerminalSingleDuctInletSideMixer & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctVAVHeatAndCoolNoReheat( model::AirTerminalSingleDuctVAVHeatAndCoolNoReheat & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctVAVHeatAndCoolReheat( model::AirTerminalSingleDuctVAVHeatAndCoolReheat & modelObject );

  boost::optional<IdfObject> translateAirWallMaterial( model::AirWallMaterial & modelObject );

  boost::optional<IdfObject> translateAvailabilityManagerHybridVentilation( model::AvailabilityManagerHybridVentilation & modelObject );

  boost::optional<IdfObject> translateAvailabilityManagerOptimumStart( model::AvailabilityManagerOptimumStart & modelObject );

  boost::optional<IdfObject> translateAvailabilityManagerDifferentialThermostat( model::AvailabilityManagerDifferentialThermostat & modelObject );

  boost::optional<IdfObject> translateAvailabilityManagerNightVentilation( model::AvailabilityManagerNightVentilation & modelObject );

  boost::optional<IdfObject> translateAvailabilityManagerNightCycle( model::AvailabilityManagerNightCycle & modelObject );

  boost::optional<IdfObject> translateBlind( model::Blind & modelObject );

  boost::optional<IdfObject> translateBoilerHotWater( model::BoilerHotWater & modelObject );

  boost::optional<IdfObject> translateBoilerSteam( model::BoilerSteam & modelObject );

  boost::optional<IdfObject> translateBuilding( model::Building & modelObject );

  boost::optional<IdfObject> translateCentralHeatPumpSystem( model::CentralHeatPumpSystem & modelObject  );

  boost::optional<IdfObject> translateCFactorUndergroundWallConstruction( model::CFactorUndergroundWallConstruction & modelObject );

  boost::optional<IdfObject> translateChillerAbsorption( model::ChillerAbsorption & modelObject );

  boost::optional<IdfObject> translateChillerAbsorptionIndirect( model::ChillerAbsorptionIndirect & modelObject );

  boost::optional<IdfObject> translateChillerElectricEIR( model::ChillerElectricEIR & modelObject );

  boost::optional<IdfObject> translateChillerHeaterPerformanceElectricEIR( model::ChillerHeaterPerformanceElectricEIR & modelObject  );

  boost::optional<IdfObject> translateCoilCoolingDXSingleSpeed( model::CoilCoolingDXSingleSpeed & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXMultiSpeed( model::CoilCoolingDXMultiSpeed & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXSingleSpeedWithoutUnitary( model::CoilCoolingDXSingleSpeed & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXTwoSpeed( model::CoilCoolingDXTwoSpeed & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXTwoSpeedWithoutUnitary( model::CoilCoolingDXTwoSpeed & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXTwoStageWithHumidityControlMode( model::CoilCoolingDXTwoStageWithHumidityControlMode & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXTwoStageWithHumidityControlModeWithoutUnitary( model::CoilCoolingDXTwoStageWithHumidityControlMode & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXVariableRefrigerantFlow( model::CoilCoolingDXVariableRefrigerantFlow & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXVariableSpeed( model::CoilCoolingDXVariableSpeed & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXVariableSpeedWithoutUnitary( model::CoilCoolingDXVariableSpeed & modelObject );

  boost::optional<IdfObject> translateCoilCoolingWater( model::CoilCoolingWater & modelObject );

  boost::optional<IdfObject> translateCoilCoolingWaterToAirHeatPumpEquationFit( model::CoilCoolingWaterToAirHeatPumpEquationFit & modelObject );

  boost::optional<IdfObject> translateCoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit( model::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit & modelObject );

  boost::optional<IdfObject> translateCoilHeatingDesuperheater( model::CoilHeatingDesuperheater & modelObject );

  boost::optional<IdfObject> translateCoilHeatingDXMultiSpeed( model::CoilHeatingDXMultiSpeed & modelObject );

  boost::optional<IdfObject> translateCoilHeatingDXSingleSpeed( model::CoilHeatingDXSingleSpeed & modelObject );

  boost::optional<IdfObject> translateCoilHeatingDXSingleSpeedWithoutUnitary( model::CoilHeatingDXSingleSpeed & modelObject );

  boost::optional<IdfObject> translateCoilHeatingDXVariableRefrigerantFlow( model::CoilHeatingDXVariableRefrigerantFlow & modelObject );

  boost::optional<IdfObject> translateCoilHeatingDXVariableSpeed( model::CoilHeatingDXVariableSpeed & modelObject );

  boost::optional<IdfObject> translateCoilHeatingDXVariableSpeedWithoutUnitary( model::CoilHeatingDXVariableSpeed & modelObject );

  boost::optional<IdfObject> translateCoilHeatingElectric( model::CoilHeatingElectric & modelObject );

  boost::optional<IdfObject> translateCoilHeatingGas( model::CoilHeatingGas & modelObject );

  boost::optional<IdfObject> translateCoilHeatingGasMultiStage( model::CoilHeatingGasMultiStage & modelObject );

  boost::optional<IdfObject> translateCoilHeatingWater( model::CoilHeatingWater & modelObject );

  boost::optional<IdfObject> translateCoilHeatingWaterToAirHeatPumpEquationFit( model::CoilHeatingWaterToAirHeatPumpEquationFit & modelObject );

  boost::optional<IdfObject> translateCoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit( model::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit & modelObject );

  boost::optional<IdfObject> translateCoilPerformanceDXCooling( model::CoilPerformanceDXCooling & modelObject );

  boost::optional<IdfObject> translateCoilSystemCoolingDXHeatExchangerAssisted( model::CoilSystemCoolingDXHeatExchangerAssisted & modelObject );

  boost::optional<IdfObject> translateCoilSystemCoolingWaterHeatExchangerAssisted( model::CoilSystemCoolingWaterHeatExchangerAssisted & modelObject );

  boost::optional<IdfObject> translateCoilWaterHeatingDesuperheater( model::CoilWaterHeatingDesuperheater & modelObject );

  boost::optional<IdfObject> translateCoilWaterHeatingAirToWaterHeatPump( model::CoilWaterHeatingAirToWaterHeatPump & modelObject );

  boost::optional<IdfObject> translateCoilWaterHeatingAirToWaterHeatPumpWrapped( model::CoilWaterHeatingAirToWaterHeatPumpWrapped & modelObject );

  boost::optional<IdfObject> translateConstruction( model::Construction & modelObject );

  boost::optional<IdfObject> translateConstructionWithInternalSource( model::ConstructionWithInternalSource & modelObject );

  boost::optional<IdfObject> translateControllerMechanicalVentilation( model::ControllerMechanicalVentilation & modelObject );

  boost::optional<IdfObject> translateControllerOutdoorAir( model::ControllerOutdoorAir & modelObject );

  boost::optional<IdfObject> translateControllerWaterCoil( model::ControllerWaterCoil & modelObject );

  boost::optional<IdfObject> translateConvergenceLimits( model::ConvergenceLimits & modelObject );

  boost::optional<IdfObject> translateCoolingTowerPerformanceCoolTools( model::CoolingTowerPerformanceCoolTools & modelObject );

  boost::optional<IdfObject> translateCoolingTowerPerformanceYorkCalc( model::CoolingTowerPerformanceYorkCalc & modelObject );

  boost::optional<IdfObject> translateCoolingTowerTwoSpeed( model::CoolingTowerTwoSpeed & modelObject );

  boost::optional<IdfObject> translateCoolingTowerSingleSpeed( model::CoolingTowerSingleSpeed & modelObject );

  boost::optional<IdfObject> translateCoolingTowerVariableSpeed( model::CoolingTowerVariableSpeed & modelObject );

  boost::optional<IdfObject> translateCurrencyType( model::CurrencyType & modelObject );

  boost::optional<IdfObject> translateCurveBicubic(model::CurveBicubic& modelObject);

  boost::optional<IdfObject> translateCurveBiquadratic(model::CurveBiquadratic& modelObject);

  boost::optional<IdfObject> translateCurveCubic(model::CurveCubic& modelObject);

  boost::optional<IdfObject> translateCurveDoubleExponentialDecay(model::CurveDoubleExponentialDecay& modelObject);

  boost::optional<IdfObject> translateCurveExponent(model::CurveExponent& modelObject);

  boost::optional<IdfObject> translateCurveExponentialDecay(model::CurveExponentialDecay& modelObject);

  boost::optional<IdfObject> translateCurveExponentialSkewNormal(model::CurveExponentialSkewNormal& modelObject);

  boost::optional<IdfObject> translateCurveFanPressureRise(model::CurveFanPressureRise& modelObject);

  boost::optional<IdfObject> translateCurveFunctionalPressureDrop(model::CurveFunctionalPressureDrop& modelObject);

  boost::optional<IdfObject> translateCurveLinear(model::CurveLinear& modelObject);

  boost::optional<IdfObject> translateCurveQuadratic(model::CurveQuadratic& modelObject);

  boost::optional<IdfObject> translateCurveQuadraticLinear(model::CurveQuadraticLinear& modelObject);

  boost::optional<IdfObject> translateCurveQuartic(model::CurveQuartic& modelObject);

  boost::optional<IdfObject> translateCurveRectangularHyperbola1(model::CurveRectangularHyperbola1& modelObject);

  boost::optional<IdfObject> translateCurveRectangularHyperbola2(model::CurveRectangularHyperbola2& modelObject);

  boost::optional<IdfObject> translateCurveSigmoid(model::CurveSigmoid& modelObject);

  boost::optional<IdfObject> translateCurveTriquadratic(model::CurveTriquadratic& modelObject);

  boost::optional<IdfObject> translateDaylightingControl( model::DaylightingControl & modelObject );

  boost::optional<IdfObject> translateDaylightingDeviceShelf( model::DaylightingDeviceShelf & modelObject );

  boost::optional<IdfObject> translateDaylightRedirectionDevice(model::DaylightRedirectionDevice & modelObject);

  boost::optional<IdfObject> translateDesignDay( model::DesignDay & modelObject );

  boost::optional<IdfObject> translateDesignSpecificationOutdoorAir( model::DesignSpecificationOutdoorAir & modelObject );

  boost::optional<IdfObject> translateDistrictCooling( model::DistrictCooling & modelObject );

  boost::optional<IdfObject> translateDistrictHeating(model::DistrictHeating & modelObject);

  boost::optional<IdfObject> translateDuct(model::Duct & modelObject);

  boost::optional<IdfObject> translateElectricEquipment( model::ElectricEquipment & modelObject );

  boost::optional<IdfObject> translateElectricLoadCenterDistribution( model::ElectricLoadCenterDistribution & modelObject );

  boost::optional<IdfObject> translateElectricLoadCenterInverterLookUpTable( model::ElectricLoadCenterInverterLookUpTable & modelObject );

  boost::optional<IdfObject> translateElectricLoadCenterInverterSimple( model::ElectricLoadCenterInverterSimple & modelObject );

  boost::optional<IdfObject> translateElectricLoadCenterStorageSimple( model::ElectricLoadCenterStorageSimple & modelObject );

  boost::optional<IdfObject> translateElectricLoadCenterStorageConverter( model::ElectricLoadCenterStorageConverter & modelObject );

  boost::optional<IdfObject> translateEnergyManagementSystemActuator(model::EnergyManagementSystemActuator & modelObject);

  boost::optional<IdfObject> translateEnergyManagementSystemConstructionIndexVariable(model::EnergyManagementSystemConstructionIndexVariable & modelObject);

  boost::optional<IdfObject> translateEnergyManagementSystemCurveOrTableIndexVariable(model::EnergyManagementSystemCurveOrTableIndexVariable & modelObject);

  boost::optional<IdfObject> translateEnergyManagementSystemGlobalVariable(model::EnergyManagementSystemGlobalVariable & modelObject);

  boost::optional<IdfObject> translateEnergyManagementSystemInternalVariable(model::EnergyManagementSystemInternalVariable & modelObject);

  boost::optional<IdfObject> translateEnergyManagementSystemMeteredOutputVariable(model::EnergyManagementSystemMeteredOutputVariable & modelObject);

  boost::optional<IdfObject> translateEnergyManagementSystemOutputVariable(model::EnergyManagementSystemOutputVariable & modelObject);

  boost::optional<IdfObject> translateEnergyManagementSystemProgram(model::EnergyManagementSystemProgram & modelObject);

  boost::optional<IdfObject> translateEnergyManagementSystemProgramCallingManager(model::EnergyManagementSystemProgramCallingManager & modelObject);

  boost::optional<IdfObject> translateEnergyManagementSystemSensor(model::EnergyManagementSystemSensor & modelObject);

  boost::optional<IdfObject> translateEnergyManagementSystemSubroutine(model::EnergyManagementSystemSubroutine & modelObject);

  boost::optional<IdfObject> translateEnergyManagementSystemTrendVariable(model::EnergyManagementSystemTrendVariable & modelObject);

  boost::optional<IdfObject> translateEvaporativeFluidCoolerSingleSpeed( model::EvaporativeFluidCoolerSingleSpeed & modelObject );

  boost::optional<IdfObject> translateEvaporativeFluidCoolerTwoSpeed( model::EvaporativeFluidCoolerTwoSpeed & modelObject  );

  boost::optional<IdfObject> translateExteriorLights(model::ExteriorLights& modelObject );

  boost::optional<IdfObject> translateFanConstantVolume( model::FanConstantVolume & modelObject );

  boost::optional<IdfObject> translateFanOnOff( model::FanOnOff & modelObject );

  boost::optional<IdfObject> translateFanVariableVolume( model::FanVariableVolume & modelObject );

  boost::optional<IdfObject> translateFanZoneExhaust( model::FanZoneExhaust & modelObject );

  boost::optional<IdfObject> translateEvaporativeCoolerDirectResearchSpecial( model::EvaporativeCoolerDirectResearchSpecial & modelObject );

  boost::optional<IdfObject> translateEvaporativeCoolerIndirectResearchSpecial( model::EvaporativeCoolerIndirectResearchSpecial & modelObject );

  boost::optional<IdfObject> translateFFactorGroundFloorConstruction(model::FFactorGroundFloorConstruction & modelObject);

  boost::optional<IdfObject> translateFluidCoolerSingleSpeed(model::FluidCoolerSingleSpeed & modelObject);

  boost::optional<IdfObject> translateFluidCoolerTwoSpeed(model::FluidCoolerTwoSpeed & modelObject);

  boost::optional<IdfObject> translateGas( model::Gas & modelObject );

  boost::optional<IdfObject> translateGasEquipment( model::GasEquipment & modelObject );

  boost::optional<IdfObject> translateGasMixture( model::GasMixture & modelObject );

  boost::optional<IdfObject> translateGeneratorMicroTurbine( model::GeneratorMicroTurbine & modelObject );

  boost::optional<IdfObject> translateGeneratorFuelCell(model::GeneratorFuelCell & modelObject);
  boost::optional<IdfObject> translateGeneratorFuelCellAirSupply(model::GeneratorFuelCellAirSupply & modelObject);
  boost::optional<IdfObject> translateGeneratorFuelCellAuxiliaryHeater(model::GeneratorFuelCellAuxiliaryHeater & modelObject);
  boost::optional<IdfObject> translateGeneratorFuelCellElectricalStorage(model::GeneratorFuelCellElectricalStorage & modelObject);
  boost::optional<IdfObject> translateGeneratorFuelCellExhaustGasToWaterHeatExchanger(model::GeneratorFuelCellExhaustGasToWaterHeatExchanger & modelObject);
  boost::optional<IdfObject> translateGeneratorFuelCellInverter(model::GeneratorFuelCellInverter & modelObject);
  boost::optional<IdfObject> translateGeneratorFuelCellPowerModule(model::GeneratorFuelCellPowerModule & modelObject);
  boost::optional<IdfObject> translateGeneratorFuelCellStackCooler(model::GeneratorFuelCellStackCooler & modelObject);
  boost::optional<IdfObject> translateGeneratorFuelCellWaterSupply(model::GeneratorFuelCellWaterSupply & modelObject);
  boost::optional<IdfObject> translateGeneratorFuelSupply(model::GeneratorFuelSupply & modelObject);

  boost::optional<IdfObject> translateGeneratorPhotovoltaic( model::GeneratorPhotovoltaic & modelObject );

  boost::optional<IdfObject> translateGroundHeatExchangerHorizontalTrench( model::GroundHeatExchangerHorizontalTrench & modelObject );

  boost::optional<IdfObject> translateGroundHeatExchangerVertical( model::GroundHeatExchangerVertical & modelObject );

  boost::optional<IdfObject> translateHeaderedPumpsConstantSpeed( model::HeaderedPumpsConstantSpeed & modelObject );

  boost::optional<IdfObject> translateHeaderedPumpsVariableSpeed( model::HeaderedPumpsVariableSpeed & modelObject );

  boost::optional<IdfObject> translateHeatBalanceAlgorithm( model::HeatBalanceAlgorithm & modelObject );

  boost::optional<IdfObject> translateHeatExchangerAirToAirSensibleAndLatent( model::HeatExchangerAirToAirSensibleAndLatent & modelObject );

  boost::optional<IdfObject> translateHeatExchangerFluidToFluid( model::HeatExchangerFluidToFluid & modelObject );

  boost::optional<IdfObject> translateHeatPumpWaterToWaterEquationFitCooling( model::HeatPumpWaterToWaterEquationFitCooling & modelObject );

  boost::optional<IdfObject> translateHeatPumpWaterToWaterEquationFitHeating( model::HeatPumpWaterToWaterEquationFitHeating & modelObject );

  boost::optional<IdfObject> translateHumidifierSteamElectric( model::HumidifierSteamElectric & modelObject );

  boost::optional<IdfObject> translateHotWaterEquipment( model::HotWaterEquipment & modelObject );

  boost::optional<IdfObject> translateIlluminanceMap( model::IlluminanceMap & modelObject );

  boost::optional<IdfObject> translateInfraredTransparentMaterial( model::InfraredTransparentMaterial & modelObject );

  boost::optional<IdfObject> translateInsideSurfaceConvectionAlgorithm( model::InsideSurfaceConvectionAlgorithm & modelObject );

  boost::optional<IdfObject> translateInteriorPartitionSurface( model::InteriorPartitionSurface & modelObject );

  boost::optional<IdfObject> translateInteriorPartitionSurfaceGroup( model::InteriorPartitionSurfaceGroup & modelObject );

  boost::optional<IdfObject> translateInternalMass( model::InternalMass & modelObject );

  boost::optional<IdfObject> translateLoadProfilePlant( model::LoadProfilePlant & modelObject  );

  boost::optional<IdfObject> translateLifeCycleCost( model::LifeCycleCost & modelObject );

  boost::optional<IdfObject> translateLifeCycleCostParameters( model::LifeCycleCostParameters & modelObject );

  boost::optional<IdfObject> translateLights( model::Lights & modelObject );

  boost::optional<IdfObject> translateLuminaire( model::Luminaire & modelObject );

  boost::optional<IdfObject> translateMaterialPropertyGlazingSpectralData( model::MaterialPropertyGlazingSpectralData & modelObject );
  
  boost::optional<IdfObject> translateMaterialPropertyMoisturePenetrationDepthSettings( model::MaterialPropertyMoisturePenetrationDepthSettings & modelObject );

  boost::optional<IdfObject> translateMasslessOpaqueMaterial( model::MasslessOpaqueMaterial & modelObject );

  boost::optional<IdfObject> translateMeterCustom( model::MeterCustom & modelObject );

  boost::optional<IdfObject> translateMeterCustomDecrement( model::MeterCustomDecrement & modelObject );

  boost::optional<IdfObject> translateNode( model::Node & modelObject );

  boost::optional<IdfObject> translateOtherEquipment(model::OtherEquipment& modelObject);

  boost::optional<IdfObject> translateOutsideSurfaceConvectionAlgorithm( model::OutsideSurfaceConvectionAlgorithm & modelObject );

  boost::optional<IdfObject> translateOutputControlReportingTolerances( model::OutputControlReportingTolerances & modelObject );

  boost::optional<IdfObject> translateOutputMeter( model::OutputMeter & modelObject );

  boost::optional<IdfObject> translateOutputVariable( model::OutputVariable & modelObject );

  boost::optional<IdfObject> translateOutputEnergyManagementSystem(model::OutputEnergyManagementSystem & modelObject );

  boost::optional<IdfObject> translatePeople( model::People & modelObject );

  boost::optional<IdfObject> translatePhotovoltaicPerformanceEquivalentOneDiode(model::PhotovoltaicPerformanceEquivalentOneDiode & modelObject);

  boost::optional<IdfObject> translatePhotovoltaicPerformanceSimple(model::PhotovoltaicPerformanceSimple & modelObject);

  boost::optional<IdfObject> translatePipeAdiabatic(model::PipeAdiabatic & modelObject);

  boost::optional<IdfObject> translatePipeIndoor(model::PipeIndoor & modelObject);

  boost::optional<IdfObject> translatePipeOutdoor(model::PipeOutdoor & modelObject);

  boost::optional<IdfObject> translatePlantLoop( model::PlantLoop & modelObject );

  boost::optional<IdfObject> translatePlantComponentTemperatureSource( model::PlantComponentTemperatureSource & modelObject  );

  boost::optional<IdfObject> translatePlantEquipmentOperationSchemes( model::PlantLoop & plantLoop );

  boost::optional<IdfObject> translatePlantEquipmentOperationCoolingLoad( model::PlantEquipmentOperationCoolingLoad & modelObject );

  boost::optional<IdfObject> translatePlantEquipmentOperationHeatingLoad( model::PlantEquipmentOperationHeatingLoad & modelObject );

  boost::optional<IdfObject> translatePlantEquipmentOperationOutdoorDryBulb( model::PlantEquipmentOperationOutdoorDryBulb & modelObject );

  boost::optional<IdfObject> translatePlantEquipmentOperationOutdoorWetBulb( model::PlantEquipmentOperationOutdoorWetBulb & modelObject );

  boost::optional<IdfObject> translatePlantEquipmentOperationOutdoorDewpoint( model::PlantEquipmentOperationOutdoorDewpoint & modelObject );

  boost::optional<IdfObject> translatePlantEquipmentOperationOutdoorRelativeHumidity( model::PlantEquipmentOperationOutdoorRelativeHumidity & modelObject );

  boost::optional<IdfObject> translatePlantEquipmentOperationOutdoorDryBulbDifference( model::PlantEquipmentOperationOutdoorDryBulbDifference & modelObject );

  boost::optional<IdfObject> translatePlantEquipmentOperationOutdoorWetBulbDifference( model::PlantEquipmentOperationOutdoorWetBulbDifference & modelObject );

  boost::optional<IdfObject> translatePlantEquipmentOperationOutdoorDewpointDifference( model::PlantEquipmentOperationOutdoorDewpointDifference & modelObject );

  boost::optional<IdfObject> translatePortList( model::PortList & modelObject );

  boost::optional<IdfObject> translatePumpConstantSpeed( model::PumpConstantSpeed & modelObject );

  boost::optional<IdfObject> translatePumpVariableSpeed( model::PumpVariableSpeed & modelObject );

  boost::optional<IdfObject> translateRefractionExtinctionGlazing( model::RefractionExtinctionGlazing & modelObject );

  boost::optional<IdfObject> translateRefrigerationAirChiller( model::RefrigerationAirChiller & modelObject );

  boost::optional<IdfObject> translateRefrigerationCase( model::RefrigerationCase & modelObject );

  boost::optional<IdfObject> translateRefrigerationCompressor( model::RefrigerationCompressor & modelObject );

  boost::optional<IdfObject> translateRefrigerationCondenserAirCooled( model::RefrigerationCondenserAirCooled & modelObject );

  boost::optional<IdfObject> translateRefrigerationCondenserCascade( model::RefrigerationCondenserCascade & modelObject );

  boost::optional<IdfObject> translateRefrigerationCondenserEvaporativeCooled( model::RefrigerationCondenserEvaporativeCooled & modelObject );

  boost::optional<IdfObject> translateRefrigerationCondenserWaterCooled( model::RefrigerationCondenserWaterCooled & modelObject );

  boost::optional<IdfObject> translateRefrigerationGasCoolerAirCooled( model::RefrigerationGasCoolerAirCooled & modelObject );

  boost::optional<IdfObject> translateRefrigerationSubcoolerLiquidSuction( model::RefrigerationSubcoolerLiquidSuction & modelObject );

  boost::optional<IdfObject> translateRefrigerationSubcoolerMechanical( model::RefrigerationSubcoolerMechanical & modelObject );

  boost::optional<IdfObject> translateRefrigerationSecondarySystem( model::RefrigerationSecondarySystem & modelObject );

  boost::optional<IdfObject> translateRefrigerationSystem( model::RefrigerationSystem & modelObject );

  boost::optional<IdfObject> translateRefrigerationTranscriticalSystem( model::RefrigerationTranscriticalSystem & modelObject );

  boost::optional<IdfObject> translateRefrigerationWalkIn( model::RefrigerationWalkIn & modelObject );

  boost::optional<IdfObject> translateRoofVegetation( model::RoofVegetation & modelObject );

  boost::optional<IdfObject> translateRunPeriod( model::RunPeriod & modelObject );

  boost::optional<IdfObject> translateRunPeriodControlDaylightSavingTime( model::RunPeriodControlDaylightSavingTime & modelObject );

  boost::optional<IdfObject> translateRunPeriodControlSpecialDays( model::RunPeriodControlSpecialDays & modelObject );

  boost::optional<IdfObject> translateScheduleCompact( model::ScheduleCompact & modelObject );

  boost::optional<IdfObject> translateScheduleConstant( model::ScheduleConstant & modelObject );//blb

  boost::optional<IdfObject> translateScheduleDay( model::ScheduleDay & modelObject );

  boost::optional<IdfObject> translateScheduleFixedInterval( model::ScheduleFixedInterval & modelObject );

  boost::optional<IdfObject> translateScheduleRuleset( model::ScheduleRuleset & modelObject );

  boost::optional<IdfObject> translateScheduleTypeLimits( model::ScheduleTypeLimits & modelObject );

  boost::optional<IdfObject> translateScheduleVariableInterval( model::ScheduleVariableInterval & modelObject );

  boost::optional<IdfObject> translateScheduleWeek( model::ScheduleWeek & modelObject );

  boost::optional<IdfObject> translateScheduleYear( model::ScheduleYear & modelObject );

  boost::optional<IdfObject> translateScreen( model::Screen & modelObject );

  boost::optional<IdfObject> translateSetpointManagerColdest( model::SetpointManagerColdest & modelObject );

  boost::optional<IdfObject> translateSetpointManagerFollowGroundTemperature( model::SetpointManagerFollowGroundTemperature & modelObject );

  boost::optional<IdfObject> translateSetpointManagerFollowOutdoorAirTemperature( model::SetpointManagerFollowOutdoorAirTemperature & modelObject );

  boost::optional<IdfObject> translateSetpointManagerFollowSystemNodeTemperature( model::SetpointManagerFollowSystemNodeTemperature & modelObject );

  boost::optional<IdfObject> translateSetpointManagerMixedAir( model::SetpointManagerMixedAir & modelObject );

  boost::optional<IdfObject> translateSetpointManagerMultiZoneCoolingAverage( model::SetpointManagerMultiZoneCoolingAverage & modelObject );

  boost::optional<IdfObject> translateSetpointManagerMultiZoneHeatingAverage( model::SetpointManagerMultiZoneHeatingAverage & modelObject );

  boost::optional<IdfObject> translateSetpointManagerMultiZoneHumidityMaximum( model::SetpointManagerMultiZoneHumidityMaximum & modelObject );

  boost::optional<IdfObject> translateSetpointManagerMultiZoneHumidityMinimum( model::SetpointManagerMultiZoneHumidityMinimum & modelObject );

  boost::optional<IdfObject> translateSetpointManagerMultiZoneMaximumHumidityAverage( model::SetpointManagerMultiZoneMaximumHumidityAverage & modelObject );

  boost::optional<IdfObject> translateSetpointManagerMultiZoneMinimumHumidityAverage( model::SetpointManagerMultiZoneMinimumHumidityAverage & modelObject );

  boost::optional<IdfObject> translateSetpointManagerOutdoorAirPretreat( model::SetpointManagerOutdoorAirPretreat & modelObject );

  boost::optional<IdfObject> translateSetpointManagerOutdoorAirReset( model::SetpointManagerOutdoorAirReset & modelObject );

  boost::optional<IdfObject> translateSetpointManagerScheduled( model::SetpointManagerScheduled & modelObject );

  boost::optional<IdfObject> translateSetpointManagerScheduledDualSetpoint( model::SetpointManagerScheduledDualSetpoint & modelObject );

  boost::optional<IdfObject> translateSetpointManagerSingleZoneCooling( model::SetpointManagerSingleZoneCooling & modelObject );

  boost::optional<IdfObject> translateSetpointManagerSingleZoneHeating( model::SetpointManagerSingleZoneHeating & modelObject );

  boost::optional<IdfObject> translateSetpointManagerSingleZoneHumidityMaximum( model::SetpointManagerSingleZoneHumidityMaximum & modelObject );

  boost::optional<IdfObject> translateSetpointManagerSingleZoneHumidityMinimum( model::SetpointManagerSingleZoneHumidityMinimum & modelObject );

  boost::optional<IdfObject> translateSetpointManagerSingleZoneOneStageCooling( model::SetpointManagerSingleZoneOneStageCooling & modelObject );

  boost::optional<IdfObject> translateSetpointManagerSingleZoneOneStageHeating( model::SetpointManagerSingleZoneOneStageHeating & modelObject );

  boost::optional<IdfObject> translateSetpointManagerSingleZoneReheat( model::SetpointManagerSingleZoneReheat & modelObject );

  boost::optional<IdfObject> translateSetpointManagerWarmest( model::SetpointManagerWarmest & modelObject );

  boost::optional<IdfObject> translateSetpointManagerWarmestTemperatureFlow( model::SetpointManagerWarmestTemperatureFlow & modelObject );

  boost::optional<IdfObject> translateShade( model::Shade & modelObject );

  boost::optional<IdfObject> translateShadingControl( model::ShadingControl & modelObject );

  boost::optional<IdfObject> translateShadingSurface( model::ShadingSurface & modelObject );

  boost::optional<IdfObject> translateShadingSurfaceGroup( model::ShadingSurfaceGroup & modelObject );

  boost::optional<IdfObject> translateShadowCalculation( model::ShadowCalculation & modelObject );

  boost::optional<IdfObject> translateSimpleGlazing( model::SimpleGlazing & modelObject );

  boost::optional<IdfObject> translateSimulationControl( model::SimulationControl & modelObject );

  boost::optional<IdfObject> translateSite( model::Site & modelObject );

  boost::optional<IdfObject> translateSiteGroundReflectance( model::SiteGroundReflectance & modelObject );

  boost::optional<IdfObject> translateSiteGroundTemperatureBuildingSurface( model::SiteGroundTemperatureBuildingSurface & modelObject );

  boost::optional<IdfObject> translateSiteGroundTemperatureDeep( model::SiteGroundTemperatureDeep & modelObject );

  boost::optional<IdfObject> translateSiteGroundTemperatureFCfactorMethod( model::SiteGroundTemperatureFCfactorMethod & modelObject );

  boost::optional<IdfObject> translateSiteGroundTemperatureShallow( model::SiteGroundTemperatureShallow & modelObject );

  boost::optional<IdfObject> translateSiteWaterMainsTemperature( model::SiteWaterMainsTemperature & modelObject );

  boost::optional<IdfObject> translateSizingParameters( model::SizingParameters & modelObject );

  boost::optional<IdfObject> translateSizingPlant( model::SizingPlant & modelObject );

  boost::optional<IdfObject> translateSizingSystem( model::SizingSystem & modelObject );

  boost::optional<IdfObject> translateSizingZone( model::SizingZone & modelObject );

  boost::optional<IdfObject> translateSkyTemperature( model::SkyTemperature & modelObject );

  boost::optional<IdfObject> translateSolarCollectorFlatPlatePhotovoltaicThermal(model::SolarCollectorFlatPlatePhotovoltaicThermal & modelObject);

  boost::optional<IdfObject> translateSolarCollectorFlatPlateWater(model::SolarCollectorFlatPlateWater & modelObject);

  boost::optional<IdfObject> translateSolarCollectorIntegralCollectorStorage(model::SolarCollectorIntegralCollectorStorage & modelObject);

  boost::optional<IdfObject> translateSolarCollectorPerformanceFlatPlate(model::SolarCollectorPerformanceFlatPlate & modelObject);

  boost::optional<IdfObject> translateSolarCollectorPerformanceIntegralCollectorStorage(model::SolarCollectorPerformanceIntegralCollectorStorage & modelObject);

  boost::optional<IdfObject> translateSolarCollectorPerformancePhotovoltaicThermalSimple(model::SolarCollectorPerformancePhotovoltaicThermalSimple & modelObject);

  boost::optional<IdfObject> translateSpace( model::Space & modelObject );

  boost::optional<IdfObject> translateSpaceInfiltrationDesignFlowRate( model::SpaceInfiltrationDesignFlowRate & modelObject );

  boost::optional<IdfObject> translateSpaceInfiltrationEffectiveLeakageArea( model::SpaceInfiltrationEffectiveLeakageArea & modelObject );

  boost::optional<IdfObject> translateSpaceType( model::SpaceType & modelObject );

  boost::optional<IdfObject> translateStandardGlazing( model::StandardGlazing & modelObject );

  boost::optional<IdfObject> translateStandardOpaqueMaterial( model::StandardOpaqueMaterial & modelObject );

  boost::optional<IdfObject> translateSteamEquipment(model::SteamEquipment& modelObject);

  boost::optional<IdfObject> translateSubSurface( model::SubSurface & modelObject );

  boost::optional<IdfObject> translateSurface( model::Surface & modelObject );

  boost::optional<IdfObject> translateSurfacePropertyConvectionCoefficients(model::SurfacePropertyConvectionCoefficients & modelObject);

  boost::optional<IdfObject> translateSurfacePropertyConvectionCoefficientsMultipleSurface(model::SurfacePropertyConvectionCoefficientsMultipleSurface & modelObject);

  boost::optional<IdfObject> translateSurfacePropertyOtherSideCoefficients(model::SurfacePropertyOtherSideCoefficients & modelObject);

  boost::optional<IdfObject> translateSurfacePropertyOtherSideConditionsModel(model::SurfacePropertyOtherSideConditionsModel & modelObject);

  boost::optional<IdfObject> translateTableMultiVariableLookup( model::TableMultiVariableLookup & modelObject );

  boost::optional<IdfObject> translateTemperingValve( model::TemperingValve & modelObject );

  boost::optional<IdfObject> translateThermalStorageIceDetailed( model::ThermalStorageIceDetailed & modelObject );

  boost::optional<IdfObject> translateThermalStorageChilledWaterStratified( model::ThermalStorageChilledWaterStratified & modelObject  );

  boost::optional<IdfObject> translateThermalZone( model::ThermalZone & modelObject );

  boost::optional<IdfObject> translateThermostatSetpointDualSetpoint( model::ThermostatSetpointDualSetpoint& tsds );

  boost::optional<IdfObject> translateTimestep( model::Timestep & modelObject );

  boost::optional<IdfObject> translateUnitarySystemPerformanceMultispeed( model::UnitarySystemPerformanceMultispeed & modelObject );

  boost::optional<IdfObject> translateVersion( model::Version & modelObject );

  boost::optional<IdfObject> translateWaterHeaterMixed( model::WaterHeaterMixed & modelObject );

  boost::optional<IdfObject> translateWaterHeaterHeatPump( model::WaterHeaterHeatPump & modelObject );

  boost::optional<IdfObject> translateWaterHeaterHeatPumpWrappedCondenser( model::WaterHeaterHeatPumpWrappedCondenser & modelObject );

  boost::optional<IdfObject> translateWaterHeaterStratified( model::WaterHeaterStratified & modelObject );

  boost::optional<IdfObject> translateWaterUseConnections( model::WaterUseConnections & modelObject );

  boost::optional<IdfObject> translateWaterUseEquipment( model::WaterUseEquipment & modelObject );

  boost::optional<IdfObject> translateWindowPropertyFrameAndDivider(model::WindowPropertyFrameAndDivider & modelObject);

  boost::optional<IdfObject> translateZoneAirHeatBalanceAlgorithm( model::ZoneAirHeatBalanceAlgorithm & modelObject );

  boost::optional<IdfObject> translateZoneAirContaminantBalance( model::ZoneAirContaminantBalance & modelObject );

  boost::optional<IdfObject> translateZoneAirMassFlowConservation(model::ZoneAirMassFlowConservation & modelObject);

  boost::optional<IdfObject> translateZoneCapacitanceMultiplierResearchSpecial( model::ZoneCapacitanceMultiplierResearchSpecial & modelObject );

  boost::optional<IdfObject> translateZoneControlContaminantController( model::ZoneControlContaminantController& modelObject );

  boost::optional<IdfObject> translateZoneControlHumidistat( model::ZoneControlHumidistat& modelObject );

  boost::optional<IdfObject> translateZoneControlThermostatStagedDualSetpoint( model::ZoneControlThermostatStagedDualSetpoint& modelObject );

  boost::optional<IdfObject> translateZoneHVACBaseboardConvectiveElectric( model::ZoneHVACBaseboardConvectiveElectric & modelObject );

  boost::optional<IdfObject> translateZoneHVACBaseboardConvectiveWater( model::ZoneHVACBaseboardConvectiveWater & modelObject );

  boost::optional<IdfObject> translateZoneHVACBaseboardRadiantConvectiveElectric( model::ZoneHVACBaseboardRadiantConvectiveElectric & modelObject );

  boost::optional<IdfObject> translateZoneHVACBaseboardRadiantConvectiveWater( model::ZoneHVACBaseboardRadiantConvectiveWater & modelObject );

  boost::optional<IdfObject> translateZoneHVACDehumidifierDX( model::ZoneHVACDehumidifierDX & modelObject );

  boost::optional<IdfObject> translateZoneHVACEnergyRecoveryVentilator( model::ZoneHVACEnergyRecoveryVentilator & modelObject );

  boost::optional<IdfObject> translateZoneHVACEnergyRecoveryVentilatorController( model::ZoneHVACEnergyRecoveryVentilatorController & modelObject );

  boost::optional<IdfObject> translateZoneHVACEquipmentList( model::ZoneHVACEquipmentList & modelObject );

  boost::optional<IdfObject> translateZoneHVACFourPipeFanCoil( model::ZoneHVACFourPipeFanCoil & modelObject );

  boost::optional<IdfObject> translateZoneHVACHighTemperatureRadiant( model::ZoneHVACHighTemperatureRadiant & modelObject );

  boost::optional<IdfObject> translateZoneHVACIdealLoadsAirSystem( model::ZoneHVACIdealLoadsAirSystem & modelObject );

  boost::optional<IdfObject> translateZoneHVACLowTemperatureRadiantElectric( model::ZoneHVACLowTemperatureRadiantElectric & modelObject );

  boost::optional<IdfObject> translateZoneHVACLowTempRadiantConstFlow( model::ZoneHVACLowTempRadiantConstFlow & modelObject );

  boost::optional<IdfObject> translateZoneHVACLowTempRadiantVarFlow( model::ZoneHVACLowTempRadiantVarFlow & modelObject );

  boost::optional<IdfObject> translateZoneHVACPackagedTerminalHeatPump( model::ZoneHVACPackagedTerminalHeatPump & modelObject );

  boost::optional<IdfObject> translateZoneHVACPackagedTerminalAirConditioner( model::ZoneHVACPackagedTerminalAirConditioner & modelObject );

  boost::optional<IdfObject> translateZoneHVACTerminalUnitVariableRefrigerantFlow( model::ZoneHVACTerminalUnitVariableRefrigerantFlow & modelObject );

  boost::optional<IdfObject> translateZoneHVACUnitHeater( model::ZoneHVACUnitHeater & modelObject );

  boost::optional<IdfObject> translateZoneHVACUnitVentilator( model::ZoneHVACUnitVentilator & modelObject );

  boost::optional<IdfObject> translateZoneHVACWaterToAirHeatPump( model::ZoneHVACWaterToAirHeatPump & modelObject );

  boost::optional<IdfObject> translateZoneMixing( model::ZoneMixing & modelObject);

  boost::optional<IdfObject> createAirLoopHVACSupplyPath( const model::Node & demandInletNode );

  boost::optional<IdfObject> translateZoneVentilationDesignFlowRate( model::ZoneVentilationDesignFlowRate & modelObject );

  boost::optional<IdfObject> createAirLoopHVACSupplyPath( model::AirLoopHVAC & airLoopHVAC );

  boost::optional<IdfObject> createAirLoopHVACReturnPath( model::AirLoopHVAC & airLoopHVAC );

  // reset the state of the translator between translations
  void reset();

  // helper method used by ForwardTranslatePlantLoop
  IdfObject populateBranch( IdfObject & branchIdfObject, std::vector<model::ModelObject> & modelObjects, model::Loop & loop, bool isSupplyBranch);

  // translate all constructions
  void translateConstructions(const model::Model & model);

  // translate all schedules and find always on and always off schedules if they exist
  void translateSchedules(const model::Model & model);

  // returns the always on schedule if found, otherwise creates one and saves for later
  IdfObject alwaysOnSchedule();
  boost::optional<IdfObject> m_alwaysOnSchedule;

  // returns the always off schedule if found, otherwise creates one and saves for later
  IdfObject alwaysOffSchedule();
  boost::optional<IdfObject> m_alwaysOffSchedule;

  // returns the default interior partition surface construction, otherwise creates one and saves for later
  model::ConstructionBase interiorPartitionSurfaceConstruction(model::Model & model);
  boost::optional<model::ConstructionBase> m_interiorPartitionSurfaceConstruction;

  // returns the default exterior surface construction, otherwise creates one and saves for later
  model::ConstructionBase exteriorSurfaceConstruction(model::Model & model);
  boost::optional<model::ConstructionBase> m_exteriorSurfaceConstruction;

  // reverse a construction if needed
  model::ConstructionBase reverseConstruction(const model::ConstructionBase& construction);
  std::map<Handle, model::ConstructionBase> m_constructionHandleToReversedConstructions;

  // resolve conflicts about constructions in matched surfaces
  void resolveMatchedSurfaceConstructionConflicts(model::Model& model);
  void resolveMatchedSubSurfaceConstructionConflicts(model::Model& model);

  // ugly hack to fix upstream mixed air setpoint managers that have internal fans
  // This should be used by the various translateUnitaryFoo methods.
  void fixSPMsForUnitarySystem(const model::HVACComponent & unitary,const std::string & fanInletNodeName, const std::string & FanOutletNodeName);

  void createStandardOutputRequests();

  std::string stripOS2(const std::string& s);

  IdfObject createAndRegisterIdfObject(const IddObjectType& idfObjectType,
                                       const model::ModelObject& modelObject);

  IdfObject createRegisterAndNameIdfObject(const IddObjectType& idfObjectType,
                                           const model::ModelObject& modelObject);

  static std::vector<IddObjectType> iddObjectsToTranslate();
  static std::vector<IddObjectType> iddObjectsToTranslateInitializer();

  /** Determines whether or not the HVACComponent is part of a unitary system or on an
   *  AirLoopHVAC */
  bool isHVACComponentWithinUnitary(const model::HVACComponent& hvacComponent) const;

  /** Takes the path to a Qt resource file, loads the IdfFile from the qrc, and returns the
   *  IdfFile if successful. */
  boost::optional<IdfFile> findIdfFile(const std::string& path);

  /** Create a simple Schedule:Compact based on input vectors. The function will consume the vectors in
   *  order, so the times must be in chronological order otherwise E+ will output an error. Summer and
   *  winter design days are not required entries, only defaultDay and name are required. At the moment,
   *  there is no ScheduleTypeLimit so there is no validation and E+ outputs a warning. It is up to the
   *  developer to make sure all E+ rules and validation for Schedule:Compact are upheld. This converts
   *  openstudio::Time of 00:00 to 24:00 and makes sure it is the last value.
   */
  boost::optional<IdfObject> createSimpleSchedule(const std::string & name,
                                                  const std::vector< std::pair<openstudio::Time, double> > & defaultDay,
                                                  const std::vector< std::pair<openstudio::Time, double> > & summerDesignDay = std::vector< std::pair<openstudio::Time, double> > (),
                                                  const std::vector< std::pair<openstudio::Time, double> > & winterDesignDay = std::vector< std::pair<openstudio::Time, double> > ());

  /** Creates the FluidProperties IdfObjects and adds them to m_idfObjects based on the input
   *  fluidType. Returns an uninitialized object if unsuccessful for any reason. If successful, returns
   *  the FluidProperties:Name IdfObject. If the fluidType already exists in m_idfObjects, it will not
   *  add new IdfObjects and will return the existing FluidProperties:Name IdfObject. Valid choices for
   *  fluidType are: R11, R12, R22, R123, R134a, R404a, R407a, R410a, NH3, R507a, R744 */
  boost::optional<IdfObject> createFluidProperties(const std::string& fluidType);

  /** Creates the FluidProperties IdfObjects and adds them to m_idfObjects based on the input
   *  glycolType and glycolConcentration. Returns an uninitialized object if unsuccessful for any reason.
   *  If successful, returns the FluidProperties:Name IdfObject with a FluidName of
   *  glycolType + "_" + glycolConcentration ie. PropyleneGlycol_30. If the fluidType already
   *  exists in m_idfObjects, it will not add new IdfObjects and will return the existing
   *  FluidProperties:Name IdfObject. Valid choices for glycolType are: PropyleneGlycol, EthyleneGlycol
   *   */
  boost::optional<IdfObject> createFluidProperties(const std::string& glycolType, int glycolConcentration);

  /** Initializes m_fluidPropertiesMap with refrigerant names and path to refrigerant resource files.
   *  Valid refrigerants are: R11, R12, R22, R123, R134a, R404a, R407a, R410a, NH3, R507a, R744 */
  void createFluidPropertiesMap();

  typedef std::map<const openstudio::Handle, const IdfObject> ModelObjectMap;

  typedef std::map<const std::string, const std::string> FluidPropertiesMap;

  FluidPropertiesMap m_fluidPropertiesMap;

  ModelObjectMap m_map;

  std::vector<IdfObject> m_idfObjects;

  boost::optional<IdfObject> m_anyNumberScheduleTypeLimits;

  StringStreamLogSink m_logSink;

  ProgressBar* m_progressBar;

  friend struct detail::ForwardTranslatorInitializer;

  // temp code
  bool m_keepRunControlSpecialDays;

  bool m_ipTabularOutput;

  bool m_excludeLCCObjects;
};

namespace detail
{
  struct ForwardTranslatorInitializer : StaticInitializer<ForwardTranslatorInitializer>
  {
    static void initialize()
    {
      ForwardTranslator::iddObjectsToTranslate();
    }
  };

  struct MakeSureForwardTranslatorInitializerIsInitialized
  {
    MakeSureForwardTranslatorInitializerIsInitialized()
    {
    }

    ForwardTranslatorInitializer m_i;
  };
}

} // energyplus

} // openstudio

#endif // ENERGYPLUS_FORWARDTRANSLATOR_HPP
