/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef OPENSTUDIO_ENERGYPLUS_FORWARDTRANSLATOR_HPP
#define OPENSTUDIO_ENERGYPLUS_FORWARDTRANSLATOR_HPP

#include <energyplus/EnergyPlusAPI.hpp>
#include <model/Model.hpp>
#include <model/ConstructionBase.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/StringStreamLogSink.hpp>

namespace openstudio {

class ProgressBar;
class Transformation;

namespace model{

class AirGap;
class AirLoopHVAC;
class AirLoopHVACUnitaryHeatPumpAirToAir;
class AirLoopHVACZoneSplitter;
class AirTerminalSingleDuctParallelPIUReheat;
class AirTerminalSingleDuctUncontrolled;
class AirTerminalSingleDuctVAVReheat;
class AirLoopHVACZoneMixer;
class AirLoopHVACOutdoorAirSystem;
class AirWallMaterial;
class Blind;
class Building;
class BoilerHotWater;
class BoilerSteam;
class CFactorUndergroundWallConstruction;
class ChillerElectricEIR;
class CoilCoolingDXSingleSpeed;
class CoilCoolingDXTwoSpeed;
class CoilCoolingWater;
class CoilCoolingWaterToAirHeatPumpEquationFit;
class CoilHeatingDXSingleSpeed;
class CoilHeatingElectric;
class CoilHeatingGas;
class CoilHeatingWater;
class CoilHeatingWaterToAirHeatPumpEquationFit;
class Construction;
class ControllerOutdoorAir;
class ControllerMechanicalVentilation;
class ControllerWaterCoil;
class ConvergenceLimits;
class CoolingTowerSingleSpeed;
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
class DesignDay;
class DesignSpecificationOutdoorAir;
class DistrictCooling;
class DistrictHeating;
class ElectricEquipment;
class EvaporativeCoolerDirectResearchSpecial;
class EvaporativeFluidCoolerSingleSpeed;
class ExteriorLights;
class FanConstantVolume;
class FanOnOff;
class FanVariableVolume;
class FFactorGroundFloorConstruction;
class Gas;
class GasEquipment;
class GasMixture;
class HeatBalanceAlgorithm;
class HeatExchangerAirToAirSensibleAndLatent;
class HotWaterEquipment;
class IlluminanceMap;
class InsideSurfaceConvectionAlgorithm;
class InfraredTransparentMaterial;
class InteriorPartitionSurface;
class InteriorPartitionSurfaceGroup;
class InternalMass;
class LifeCycleCost;
class LifeCycleCostParameters;
class Lights;
class Luminaire;
class MasslessOpaqueMaterial;
class Meter;
class Node;
class OtherEquipment;
class OutsideSurfaceConvectionAlgorithm;
class OutputVariable;
class People;
class PipeAdiabatic;
class PlantLoop;
class PortList;
class PumpConstantSpeed;
class PumpVariableSpeed;
class RefractionExtinctionGlazing;
class RoofVegetation;
class RunPeriod;
class RunPeriodControlDaylightSavingTime;
class RunPeriodControlSpecialDays;
class Screen;
class SetpointManagerFollowOutdoorAirTemperature;
class SetpointManagerMixedAir;
class SetpointManagerOutdoorAirReset;
class SetpointManagerScheduled;
class SetpointManagerSingleZoneReheat;
class ShadowCalculation;
class Shade;
class SimulationControl;
class SiteGroundReflectance;
class SiteGroundTemperatureBuildingSurface;
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
class ShadingSurface;
class ShadingSurfaceGroup;
class Site;
class SkyTemperature;
class Space;
class SpaceInfiltrationDesignFlowRate;
class SpaceInfiltrationEffectiveLeakageArea;
class SpaceVentilationDesignFlowRate;
class SpaceType;
class SteamEquipment;
class SubSurface;
class Surface;
class ThermalZone;
class ThermostatSetpointDualSetpoint;
class Timestep;
class Version;
class WaterHeaterMixed;
class WaterUseConnections;
class WaterUseEquipment;
class ZoneAirHeatBalanceAlgorithm;
class ZoneHVACBaseboardConvectiveElectric;
class ZoneHVACBaseboardConvectiveWater;
class ZoneHVACFourPipeFanCoil;
class ZoneHVACIdealLoadsAirSystem;
class ZoneHVACPackagedTerminalHeatPump;
class ZoneHVACPackagedTerminalAirConditioner;
class ZoneHVACWaterToAirHeatPump;
class ZoneHVACEquipmentList;
class ZoneHVACUnitHeater;

}

namespace energyplus {

namespace detail
{
  struct ForwardTranslatorInitializer;
};

#define ENERGYPLUS_VERSION "8.0"

class ENERGYPLUS_API ForwardTranslator {
 public:

  ForwardTranslator();

  /** Translates the given Model to a Workspace. 
   */
  Workspace translateModel( const model::Model & model, ProgressBar* progressBar=NULL );

  /** Translates a ModelObject into a Workspace
   */
  Workspace translateModelObject( model::ModelObject & modelObject );

  /** Get warning messages generated by the last translation.
   */
  std::vector<LogMessage> warnings() const;

  /** Get error messages generated by the last translation.
   */
  std::vector<LogMessage> errors() const;

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
   *  is responsbile for translating the components associated with the loop.  The method translateAndMapModelObject()
   *  uses an internal map to determine if a model object has already been translated, therefore there is no
   *  concern of translating a model object twice, provided that model objects are always translated using the 
   *  translateAndMapModelObject() interface as opposed to the type specific translators.
   */
  Workspace translateModelPrivate( model::Model& model, bool fullModelTranslation );

  boost::optional<IdfObject> translateAndMapModelObject( model::ModelObject & modelObject );

  boost::optional<IdfObject> translateAirGap( model::AirGap & modelObject );

  boost::optional<IdfObject> translateAirLoopHVAC( model::AirLoopHVAC & modelObject );

  boost::optional<IdfObject> translateAirLoopHVACOutdoorAirSystem( model::AirLoopHVACOutdoorAirSystem & modelObject );

  boost::optional<IdfObject> translateAirLoopHVACUnitaryHeatPumpAirToAir( model::AirLoopHVACUnitaryHeatPumpAirToAir & modelObject );

  boost::optional<IdfObject> translateAirLoopHVACZoneMixer( model::AirLoopHVACZoneMixer & modelObject  );

  boost::optional<IdfObject> translateAirLoopHVACZoneSplitter( model::AirLoopHVACZoneSplitter & airLoopHVACZoneSplitter );

  boost::optional<IdfObject> translateAirTerminalSingleDuctParallelPIUReheat( model::AirTerminalSingleDuctParallelPIUReheat & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctUncontrolled( model::AirTerminalSingleDuctUncontrolled & modelObject );

  boost::optional<IdfObject> translateAirTerminalSingleDuctVAVReheat( model::AirTerminalSingleDuctVAVReheat & modelObject );

  boost::optional<IdfObject> translateAirWallMaterial( model::AirWallMaterial & modelObject );

  boost::optional<IdfObject> translateBlind( model::Blind & modelObject );

  boost::optional<IdfObject> translateBoilerHotWater( model::BoilerHotWater & modelObject );

  boost::optional<IdfObject> translateBoilerSteam( model::BoilerSteam & modelObject );

  boost::optional<IdfObject> translateBuilding( model::Building & modelObject );

  boost::optional<IdfObject> translateCFactorUndergroundWallConstruction( model::CFactorUndergroundWallConstruction & modelObject );

  boost::optional<IdfObject> translateChillerElectricEIR( model::ChillerElectricEIR & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXSingleSpeed( model::CoilCoolingDXSingleSpeed & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXSingleSpeedWithoutUnitary( model::CoilCoolingDXSingleSpeed & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXTwoSpeed( model::CoilCoolingDXTwoSpeed & modelObject );

  boost::optional<IdfObject> translateCoilCoolingDXTwoSpeedWithoutUnitary( model::CoilCoolingDXTwoSpeed & modelObject );

  boost::optional<IdfObject> translateCoilCoolingWater( model::CoilCoolingWater & modelObject );

  boost::optional<IdfObject> translateCoilCoolingWaterToAirHeatPumpEquationFit( model::CoilCoolingWaterToAirHeatPumpEquationFit & modelObject );

  boost::optional<IdfObject> translateCoilHeatingDXSingleSpeed( model::CoilHeatingDXSingleSpeed & modelObject );

  boost::optional<IdfObject> translateCoilHeatingDXSingleSpeedWithoutUnitary( model::CoilHeatingDXSingleSpeed & modelObject );

  boost::optional<IdfObject> translateCoilHeatingElectric( model::CoilHeatingElectric & modelObject );

  boost::optional<IdfObject> translateCoilHeatingGas( model::CoilHeatingGas & modelObject );

  boost::optional<IdfObject> translateCoilHeatingWater( model::CoilHeatingWater & modelObject );

  boost::optional<IdfObject> translateCoilHeatingWaterToAirHeatPumpEquationFit( model::CoilHeatingWaterToAirHeatPumpEquationFit & modelObject );

  boost::optional<IdfObject> translateConstruction( model::Construction & modelObject );

  boost::optional<IdfObject> translateControllerMechanicalVentilation( model::ControllerMechanicalVentilation & modelObject );

  boost::optional<IdfObject> translateControllerOutdoorAir( model::ControllerOutdoorAir & modelObject );

  boost::optional<IdfObject> translateControllerWaterCoil( model::ControllerWaterCoil & modelObject );

  boost::optional<IdfObject> translateConvergenceLimits( model::ConvergenceLimits & modelObject );

  boost::optional<IdfObject> translateCoolingTowerSingleSpeed( model::CoolingTowerSingleSpeed & modelObject );

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

  boost::optional<IdfObject> translateDesignDay( model::DesignDay & modelObject );

  boost::optional<IdfObject> translateDesignSpecificationOutdoorAir( model::DesignSpecificationOutdoorAir & modelObject );

  boost::optional<IdfObject> translateDistrictCooling( model::DistrictCooling & modelObject );   
  
  boost::optional<IdfObject> translateDistrictHeating( model::DistrictHeating & modelObject );  
  
  boost::optional<IdfObject> translateElectricEquipment( model::ElectricEquipment & modelObject );

  boost::optional<IdfObject> translateEvaporativeFluidCoolerSingleSpeed( model::EvaporativeFluidCoolerSingleSpeed & modelObject );
  
  boost::optional<IdfObject> translateExteriorLights(model::ExteriorLights& modelObject);

  boost::optional<IdfObject> translateFanConstantVolume( model::FanConstantVolume & modelObject );
  
  boost::optional<IdfObject> translateFanOnOff( model::FanOnOff & modelObject );

  boost::optional<IdfObject> translateFanVariableVolume( model::FanVariableVolume & modelObject );

  boost::optional<IdfObject> translateEvaporativeCoolerDirectResearchSpecial( model::EvaporativeCoolerDirectResearchSpecial & modelObject );

  boost::optional<IdfObject> translateFFactorGroundFloorConstruction( model::FFactorGroundFloorConstruction & modelObject );

  boost::optional<IdfObject> translateGas( model::Gas & modelObject );

  boost::optional<IdfObject> translateGasEquipment( model::GasEquipment & modelObject );

  boost::optional<IdfObject> translateGasMixture( model::GasMixture & modelObject );

  boost::optional<IdfObject> translateHeatBalanceAlgorithm( model::HeatBalanceAlgorithm & modelObject );

  boost::optional<IdfObject> translateHeatExchangerAirToAirSensibleAndLatent( model::HeatExchangerAirToAirSensibleAndLatent & modelObject );

  boost::optional<IdfObject> translateHotWaterEquipment( model::HotWaterEquipment & modelObject );

  boost::optional<IdfObject> translateIlluminanceMap( model::IlluminanceMap & modelObject );

  boost::optional<IdfObject> translateInfraredTransparentMaterial( model::InfraredTransparentMaterial & modelObject );

  boost::optional<IdfObject> translateInsideSurfaceConvectionAlgorithm( model::InsideSurfaceConvectionAlgorithm & modelObject );

  boost::optional<IdfObject> translateInteriorPartitionSurface( model::InteriorPartitionSurface & modelObject );

  boost::optional<IdfObject> translateInteriorPartitionSurfaceGroup( model::InteriorPartitionSurfaceGroup & modelObject );

  boost::optional<IdfObject> translateInternalMass( model::InternalMass & modelObject );

  boost::optional<IdfObject> translateLifeCycleCost( model::LifeCycleCost & modelObject );

  boost::optional<IdfObject> translateLifeCycleCostParameters( model::LifeCycleCostParameters & modelObject );

  boost::optional<IdfObject> translateLights( model::Lights & modelObject );

  boost::optional<IdfObject> translateLuminaire( model::Luminaire & modelObject );

  boost::optional<IdfObject> translateMasslessOpaqueMaterial( model::MasslessOpaqueMaterial & modelObject );

  boost::optional<IdfObject> translateMeter( model::Meter & modelObject );

  boost::optional<IdfObject> translateNode( model::Node & modelObject );

  boost::optional<IdfObject> translateOtherEquipment(model::OtherEquipment& modelObject);

  boost::optional<IdfObject> translateOutsideSurfaceConvectionAlgorithm( model::OutsideSurfaceConvectionAlgorithm & modelObject );

  boost::optional<IdfObject> translateOutputVariable( model::OutputVariable & modelObject );

  boost::optional<IdfObject> translatePeople( model::People & modelObject );

  boost::optional<IdfObject> translatePipeAdiabatic( model::PipeAdiabatic & modelObject );

  boost::optional<IdfObject> translatePlantLoop( model::PlantLoop & modelObject );

  boost::optional<IdfObject> translatePortList( model::PortList & modelObject );

  boost::optional<IdfObject> translatePumpConstantSpeed( model::PumpConstantSpeed & modelObject );

  boost::optional<IdfObject> translatePumpVariableSpeed( model::PumpVariableSpeed & modelObject );

  boost::optional<IdfObject> translateRefractionExtinctionGlazing( model::RefractionExtinctionGlazing & modelObject );

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

  boost::optional<IdfObject> translateSetpointManagerFollowOutdoorAirTemperature( model::SetpointManagerFollowOutdoorAirTemperature & modelObject );

  boost::optional<IdfObject> translateSetpointManagerMixedAir( model::SetpointManagerMixedAir & modelObject );

  boost::optional<IdfObject> translateSetpointManagerOutdoorAirReset( model::SetpointManagerOutdoorAirReset & modelObject );

  boost::optional<IdfObject> translateSetpointManagerScheduled( model::SetpointManagerScheduled & modelObject );

  boost::optional<IdfObject> translateSetpointManagerSingleZoneReheat( model::SetpointManagerSingleZoneReheat & modelObject);

  boost::optional<IdfObject> translateShade( model::Shade & modelObject );

  boost::optional<IdfObject> translateShadingSurface( model::ShadingSurface & modelObject );

  boost::optional<IdfObject> translateShadingSurfaceGroup( model::ShadingSurfaceGroup & modelObject );

  boost::optional<IdfObject> translateShadowCalculation( model::ShadowCalculation & modelObject );

  boost::optional<IdfObject> translateSimpleGlazing( model::SimpleGlazing & modelObject );

  boost::optional<IdfObject> translateSimulationControl( model::SimulationControl & modelObject );

  boost::optional<IdfObject> translateSite( model::Site & modelObject );

  boost::optional<IdfObject> translateSiteGroundReflectance( model::SiteGroundReflectance & modelObject );

  boost::optional<IdfObject> translateSiteGroundTemperatureBuildingSurface( model::SiteGroundTemperatureBuildingSurface & modelObject );

  boost::optional<IdfObject> translateSiteWaterMainsTemperature( model::SiteWaterMainsTemperature & modelObject );

  boost::optional<IdfObject> translateSizingParameters( model::SizingParameters & modelObject );

  boost::optional<IdfObject> translateSizingPlant( model::SizingPlant & modelObject );

  boost::optional<IdfObject> translateSizingSystem( model::SizingSystem & modelObject );

  boost::optional<IdfObject> translateSizingZone( model::SizingZone & modelObject );

  boost::optional<IdfObject> translateSkyTemperature( model::SkyTemperature & modelObject );

  boost::optional<IdfObject> translateSpace( model::Space & modelObject );

  boost::optional<IdfObject> translateSpaceInfiltrationDesignFlowRate( model::SpaceInfiltrationDesignFlowRate & modelObject );

  boost::optional<IdfObject> translateSpaceInfiltrationEffectiveLeakageArea( model::SpaceInfiltrationEffectiveLeakageArea & modelObject );

  boost::optional<IdfObject> translateSpaceType( model::SpaceType & modelObject );

  boost::optional<IdfObject> translateStandardGlazing( model::StandardGlazing & modelObject );

  boost::optional<IdfObject> translateStandardOpaqueMaterial( model::StandardOpaqueMaterial & modelObject );

  boost::optional<IdfObject> translateSteamEquipment(model::SteamEquipment& modelObject);

  boost::optional<IdfObject> translateSubSurface( model::SubSurface & modelObject );

  boost::optional<IdfObject> translateSurface( model::Surface & modelObject );

  boost::optional<IdfObject> translateThermalZone( model::ThermalZone & modelObject );

  boost::optional<IdfObject> translateThermostatSetpointDualSetpoint( model::ThermostatSetpointDualSetpoint& tsds );

  boost::optional<IdfObject> translateTimestep( model::Timestep & modelObject );

  boost::optional<IdfObject> translateVersion( model::Version & modelObject );

  boost::optional<IdfObject> translateWaterHeaterMixed( model::WaterHeaterMixed & modelObject );

  boost::optional<IdfObject> translateWaterUseConnections( model::WaterUseConnections & modelObject );

  boost::optional<IdfObject> translateWaterUseEquipment( model::WaterUseEquipment & modelObject );

  boost::optional<IdfObject> translateZoneAirHeatBalanceAlgorithm( model::ZoneAirHeatBalanceAlgorithm & modelObject );
  
  boost::optional<IdfObject> translateZoneHVACBaseboardConvectiveElectric( model::ZoneHVACBaseboardConvectiveElectric & modelObject );

  boost::optional<IdfObject> translateZoneHVACBaseboardConvectiveWater( model::ZoneHVACBaseboardConvectiveWater & modelObject );
  
  boost::optional<IdfObject> translateZoneHVACEquipmentList( model::ZoneHVACEquipmentList & modelObject );

  boost::optional<IdfObject> translateZoneHVACFourPipeFanCoil( model::ZoneHVACFourPipeFanCoil & modelObject );

  boost::optional<IdfObject> translateZoneHVACIdealLoadsAirSystem( model::ZoneHVACIdealLoadsAirSystem & modelObject );

  boost::optional<IdfObject> translateZoneHVACPackagedTerminalHeatPump( model::ZoneHVACPackagedTerminalHeatPump & modelObject );

  boost::optional<IdfObject> translateZoneHVACPackagedTerminalAirConditioner( model::ZoneHVACPackagedTerminalAirConditioner & modelObject );

  boost::optional<IdfObject> translateZoneHVACWaterToAirHeatPump( model::ZoneHVACWaterToAirHeatPump & modelObject );
  
  boost::optional<IdfObject> translateZoneHVACUnitHeater( model::ZoneHVACUnitHeater & modelObject );

  boost::optional<IdfObject> createAirLoopHVACSupplyPath( model::AirLoopHVAC & airLoopHVAC );

  boost::optional<IdfObject> createAirLoopHVACReturnPath( model::AirLoopHVAC & airLoopHVAC );

  // helper method used by ForwardTranslatePlantLoop
  IdfObject populateBranch( IdfObject & branchIdfObject, std::vector<model::ModelObject> & modelObjects,	model::PlantLoop & plantLoop);

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

  // reverse a construction if needed
  model::ConstructionBase reverseConstruction(const model::ConstructionBase& construction);
  std::map<Handle, model::ConstructionBase> m_constructionHandleToReversedConstructions;

  // resolve conflicts about constructions in matched surfaces
  void resolveMatchedSurfaceConstructionConflicts(model::Model& model);
  void resolveMatchedSubSurfaceConstructionConflicts(model::Model& model);

  void createStandardOutputRequests();

  std::string stripOS2(const std::string& s);

  IdfObject createAndRegisterIdfObject(const IddObjectType& idfObjectType,
                                       const model::ModelObject& modelObject);

  IdfObject createRegisterAndNameIdfObject(const IddObjectType& idfObjectType,
                                           const model::ModelObject& modelObject);

  static std::vector<IddObjectType> iddObjectsToTranslate();
  static std::vector<IddObjectType> iddObjectsToTranslateInitializer();

  typedef std::map<const openstudio::Handle, const IdfObject> ModelObjectMap;

  ModelObjectMap m_map;

  std::vector<IdfObject> m_idfObjects;

  boost::optional<IdfObject> m_anyNumberScheduleTypeLimits;

  StringStreamLogSink m_logSink;

  ProgressBar* m_progressBar;

  friend struct detail::ForwardTranslatorInitializer;
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

#endif // OPENSTUDIO_ENERGYPLUS_FORWARDTRANSLATOR_HPP

