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

#include "EnergyPlusAPI.hpp"

#include <energyplus/ForwardTranslator.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/Construction.hpp>
#include <model/Construction_Impl.hpp>
#include <model/ConstructionWithInternalSource.hpp>
#include <model/ConstructionWithInternalSource_Impl.hpp>
#include <model/RunPeriod.hpp>
#include <model/RunPeriod_Impl.hpp>
#include <model/RunPeriodControlSpecialDays.hpp>
#include <model/RunPeriodControlSpecialDays_Impl.hpp>
#include <model/SimulationControl.hpp>
#include <model/SimulationControl_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/ConcreteModelObjects.hpp>

#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>
#include <utilities/idd/Output_Table_SummaryReports_FieldEnums.hxx>
#include <utilities/idd/OutputControl_Table_Style_FieldEnums.hxx>
#include <utilities/idd/Output_VariableDictionary_FieldEnums.hxx>
#include <utilities/idd/Output_SQLite_FieldEnums.hxx>
#include <utilities/idd/ProgramControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/plot/ProgressBar.hpp>

#include <QThread>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

ForwardTranslator::ForwardTranslator()
{
  m_logSink.setLogLevel(Warn);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.energyplus\\.ForwardTranslator"));
  m_logSink.setThreadId(QThread::currentThread());

  // temp code 
  m_keepRunControlSpecialDays = false;
}

Workspace ForwardTranslator::translateModel( const Model & model, ProgressBar* progressBar )
{
  Model modelCopy = model.clone().cast<Model>();

  m_progressBar = progressBar;
  if (m_progressBar){
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(model.numObjects());
  }

  return translateModelPrivate(modelCopy, true);
}

Workspace ForwardTranslator::translateModelObject( ModelObject & modelObject )
{
  Model modelCopy;
  modelObject.clone(modelCopy);

  m_progressBar = NULL;

  return translateModelPrivate(modelCopy, false);
}

std::vector<LogMessage> ForwardTranslator::warnings() const
{
  std::vector<LogMessage> result;

  BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
    if (logMessage.logLevel() == Warn){
      result.push_back(logMessage);
    }
  }

  return result;
}

std::vector<LogMessage> ForwardTranslator::errors() const
{
  std::vector<LogMessage> result;

  BOOST_FOREACH(LogMessage logMessage, m_logSink.logMessages()){
    if (logMessage.logLevel() > Warn){
      result.push_back(logMessage);
    }
  }

  return result;
}

// temp code
void ForwardTranslator::setKeepRunControlSpecialDays(bool keepRunControlSpecialDays)
{
  m_keepRunControlSpecialDays = keepRunControlSpecialDays;
}


Workspace ForwardTranslator::translateModelPrivate( model::Model & model, bool fullModelTranslation )
{
  m_idfObjects.clear();

  m_map.clear();

  m_anyNumberScheduleTypeLimits.reset();

  m_constructionHandleToReversedConstructions.clear();

  m_logSink.setThreadId(QThread::currentThread());

  m_logSink.resetStringStream();

  // translate Version first
  model::Version version = model.getUniqueModelObject<model::Version>();
  translateAndMapModelObject(version);

  // resolve surface marching conflicts before combining thermal zones or removing spaces
  // as those operations may change search distances
  resolveMatchedSurfaceConstructionConflicts(model);
  resolveMatchedSubSurfaceConstructionConflicts(model);

  // check for spaces not in a thermal zone
  BOOST_FOREACH(Space space, model.getModelObjects<Space>()){
    if (!space.thermalZone()){
      LOG(Warn, "Space " << space.name().get() << " is not associated with a ThermalZone, it will not be translated.");
      space.remove();
    }
  }

  // next thing to do is combine all spaces in each thermal zone
  // after this each zone will have 0 or 1 spaces and each space will have 0 or 1 zone
  BOOST_FOREACH(ThermalZone thermalZone, model.getModelObjects<ThermalZone>()){
    thermalZone.combineSpaces();
  }

  // remove unused space types
  std::vector<SpaceType> spaceTypes = model.getModelObjects<SpaceType>();
  BOOST_FOREACH(SpaceType spaceType, spaceTypes){
    std::vector<Space> spaces = spaceType.spaces();
    if (spaces.empty()){
      LOG(Info, "SpaceType " << spaceType.name().get() << " is not referenced by any space, it will not be translated.");
      spaceType.remove();
    }else if (spaces.size() == 1){
      //LOG(Info, "SpaceType " << spaceType.name().get() << " is referenced by one space, loads will be transfered to the space and the space type removed.");
      // hard apply space type adds a dummy space type to prevent inheriting building space type
      //spaces[0].hardApplySpaceType(false);
      //spaceType.remove();
    }
  }

  //Fix for Bug 717 - Take any OtherEquipment objects that still point to a spacetype and make
  //a new instance of them for every space that that spacetype points to then delete the one
  //that pointed to a spacetype
  std::vector<OtherEquipment> otherEquipments = model.getModelObjects<OtherEquipment>();
  BOOST_FOREACH(OtherEquipment otherEquipment, otherEquipments){
    boost::optional<SpaceType> spaceTypeOfOtherEquipment = otherEquipment.spaceType();
    if (spaceTypeOfOtherEquipment){
      //loop through the spaces in this space type and make a new instance for each one
      std::vector<Space> spaces = spaceTypeOfOtherEquipment.get().spaces();      
      BOOST_FOREACH(Space space, spaces){      
        OtherEquipment otherEquipmentForSpace = otherEquipment.clone().cast<OtherEquipment>();
        otherEquipmentForSpace.setSpace(space);
        //make a nice name for the thing
        
        //std::string otherEquipmentForSpaceName = otherEquipment.name()
        //otherEquipment.setName("newName")
      }
      //now, delete the one that points to a spacetype
      otherEquipment.remove();
    }
  }  

  // temp code
  if (!m_keepRunControlSpecialDays){
    // DLM: we will not translate these objects until we support holidays in the GUI
    // we will not warn users because these objects are not exposed in the GUI
    BOOST_FOREACH(model::RunPeriodControlSpecialDays holiday, model.getModelObjects<model::RunPeriodControlSpecialDays>()){ 
      holiday.remove();
    }
  }

  if (fullModelTranslation){

    // ensure that building exists
    if (!model.getOptionalUniqueModelObject<model::Building>()){
      model::Building building = model.getUniqueModelObject<model::Building>();
      translateAndMapModelObject(building);
    }

    // ensure that simulation control exists
    if (!model.getOptionalUniqueModelObject<model::SimulationControl>()){
      model::SimulationControl simulationControl = model.getUniqueModelObject<model::SimulationControl>();
      translateAndMapModelObject(simulationControl);
    }

    // Add a ProgramControl object to force a single threaded simulation
    IdfObject programControl(openstudio::IddObjectType::ProgramControl);
    programControl.setInt(openstudio::ProgramControlFields::NumberofThreadsAllowed,1);
    m_idfObjects.push_back(programControl);

    // ensure that sizing parameters control exists
    if (!model.getOptionalUniqueModelObject<model::SizingParameters>()){
      model::SizingParameters sizingParameters = model.getUniqueModelObject<model::SizingParameters>();
      translateAndMapModelObject(sizingParameters);
    }

    // ensure that at least one run period exists
    if (!model.getOptionalUniqueModelObject<model::RunPeriod>()){
      model::RunPeriod runPeriod = model.getUniqueModelObject<model::RunPeriod>();
      translateAndMapModelObject(runPeriod);
    }

    // add a global geometry rules object
    IdfObject globalGeometryRules(openstudio::IddObjectType::GlobalGeometryRules);
    globalGeometryRules.setString(openstudio::GlobalGeometryRulesFields::StartingVertexPosition, "UpperLeftCorner");
    globalGeometryRules.setString(openstudio::GlobalGeometryRulesFields::VertexEntryDirection, "Counterclockwise");
    globalGeometryRules.setString(openstudio::GlobalGeometryRulesFields::CoordinateSystem, "Relative");
    globalGeometryRules.setString(openstudio::GlobalGeometryRulesFields::DaylightingReferencePointCoordinateSystem, "Relative");
    globalGeometryRules.setString(openstudio::GlobalGeometryRulesFields::RectangularSurfaceCoordinateSystem, "Relative");
    m_idfObjects.push_back(globalGeometryRules);

    // translate life cycle cost parameters
    boost::optional<LifeCycleCostParameters> lifeCycleCostParameters = model.lifeCycleCostParameters();
    if (!lifeCycleCostParameters){
      // only warn if costs are present
      if (!model.getModelObjects<LifeCycleCost>().empty()){
        LOG(Warn, "No LifeCycleCostParameters but LifeCycleCosts are present, adding default LifeCycleCostParameters.");
      }
      
      // always add this object so E+ results section exists
      lifeCycleCostParameters = model.getUniqueModelObject<LifeCycleCostParameters>();
    }
  }

  translateConstructions(model);
  translateSchedules(model);

  // get air loops in sorted order
  std::vector<AirLoopHVAC> airLoops = model.getModelObjects<AirLoopHVAC>();
  std::sort(airLoops.begin(), airLoops.end(), WorkspaceObjectNameLess());
  BOOST_FOREACH(AirLoopHVAC airLoop, airLoops){
    translateAndMapModelObject(airLoop);
  }

  // get plant loops in sorted order
  std::vector<PlantLoop> plantLoops = model.getModelObjects<PlantLoop>();
  std::sort(plantLoops.begin(), plantLoops.end(), WorkspaceObjectNameLess());
  BOOST_FOREACH(PlantLoop plantLoop, plantLoops){
    translateAndMapModelObject(plantLoop);
  }

  // now loop over all objects
  BOOST_FOREACH(const IddObjectType& iddObjectType, iddObjectsToTranslate()){

    // get objects by type in sorted order
    std::vector<WorkspaceObject> objects = model.getObjectsByType(iddObjectType);
    std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());

    BOOST_FOREACH(const WorkspaceObject& workspaceObject, objects){
      model::ModelObject modelObject = workspaceObject.cast<ModelObject>();
      translateAndMapModelObject(modelObject);
    }
  }

  if (fullModelTranslation){
    // add output requests
    this->createStandardOutputRequests();
  }

  Workspace workspace(StrictnessLevel::None, IddFileType::EnergyPlus);
  OptionalWorkspaceObject vo = workspace.versionObject();
  OS_ASSERT(vo);
  workspace.removeObject(vo->handle());

  workspace.setFastNaming(true);
  workspace.addObjects(m_idfObjects);
  workspace.setFastNaming(false);
  OS_ASSERT(workspace.getObjectsByType(IddObjectType::Version).size() == 1u);

  return workspace;
}

// struct for sorting children in forward translator
struct ChildSorter {
  ChildSorter(std::vector<IddObjectType>& iddObjectTypes)
    : m_iddObjectTypes(iddObjectTypes)
  {}
  
  // sort first by position in iddObjectTypes and then by name
  bool operator()(const model::ModelObject& a, const model::ModelObject& b) const
  {
    std::vector<IddObjectType>::const_iterator ita = std::find(m_iddObjectTypes.begin(), m_iddObjectTypes.end(), a.iddObject().type());
    std::vector<IddObjectType>::const_iterator itb = std::find(m_iddObjectTypes.begin(), m_iddObjectTypes.end(), b.iddObject().type());
  
    if (ita < itb){
      return true;
    }else if (ita > itb){
      return false;
    }

    std::string aname;
    boost::optional<std::string> oaname = a.name();
    if (oaname){
      aname = *oaname;
    }
    std::string bname;
    boost::optional<std::string> obname = b.name();
    if (obname){
      bname = *obname;
    }
    return istringLess(aname, bname);
  }

  std::vector<IddObjectType> m_iddObjectTypes;
};

boost::optional<IdfObject> ForwardTranslator::translateAndMapModelObject(ModelObject & modelObject)
{
  boost::optional<IdfObject> retVal;

  // if already translated then exit
  ModelObjectMap::const_iterator objInMap = m_map.find( modelObject.handle() );
  if( objInMap != m_map.end() )
  {
    return boost::optional<IdfObject>(objInMap->second);
  }

  LOG(Trace,"Translating " << modelObject.briefDescription() << ".");

  switch(modelObject.iddObject().type().value())
  {
  case openstudio::IddObjectType::OS_AirLoopHVAC :
    {
      model::AirLoopHVAC airLoopHVAC = modelObject.cast<AirLoopHVAC>();
      retVal = translateAirLoopHVAC(airLoopHVAC);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ParallelPIU_Reheat :
    {
      model::AirTerminalSingleDuctParallelPIUReheat airTerminal = modelObject.cast<AirTerminalSingleDuctParallelPIUReheat>();
      retVal = translateAirTerminalSingleDuctParallelPIUReheat(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled :
    {
      model::AirTerminalSingleDuctUncontrolled airTerminal = modelObject.cast<AirTerminalSingleDuctUncontrolled>();
      retVal = translateAirTerminalSingleDuctUncontrolled(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat :
    {
      model::AirTerminalSingleDuctVAVReheat airTerminal = modelObject.cast<AirTerminalSingleDuctVAVReheat>();
      retVal = translateAirTerminalSingleDuctVAVReheat(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirLoopHVAC_ZoneSplitter :
    {
      model::AirLoopHVACZoneSplitter splitter = modelObject.cast<AirLoopHVACZoneSplitter>();
      retVal = translateAirLoopHVACZoneSplitter(splitter);
      break;
    }
  case openstudio::IddObjectType::OS_AirLoopHVAC_ZoneMixer :
    {
      model::AirLoopHVACZoneMixer mixer = modelObject.cast<AirLoopHVACZoneMixer>();
      retVal = translateAirLoopHVACZoneMixer(mixer);
      break;
    }
  case openstudio::IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem :
    {
      model::AirLoopHVACOutdoorAirSystem oaSystem = modelObject.cast<AirLoopHVACOutdoorAirSystem>();
      retVal = translateAirLoopHVACOutdoorAirSystem(oaSystem);
      break;
    }
  case openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir :
    {
      model::AirLoopHVACUnitaryHeatPumpAirToAir unitary = modelObject.cast<AirLoopHVACUnitaryHeatPumpAirToAir>();
      retVal = translateAirLoopHVACUnitaryHeatPumpAirToAir(unitary);
      break;
    }
  case openstudio::IddObjectType::OS_AvailabilityManagerAssignmentList :
    {
      return retVal;
    }
  case openstudio::IddObjectType::OS_AvailabilityManager_Scheduled :
    {
      return retVal;
    }
  case openstudio::IddObjectType::OS_Material_AirWall :
    {
      model::AirWallMaterial airWallMaterial = modelObject.cast<AirWallMaterial>();
      retVal = translateAirWallMaterial(airWallMaterial);
      break;
    }
  case openstudio::IddObjectType::OS_WindowMaterial_Blind :
    {
      model::Blind blind = modelObject.cast<Blind>();
      retVal = translateBlind(blind);
      break;
    }
  case openstudio::IddObjectType::OS_Boiler_HotWater :
    {
      model::BoilerHotWater boiler = modelObject.cast<BoilerHotWater>();
      retVal = translateBoilerHotWater(boiler);
      break;
    }
  case openstudio::IddObjectType::OS_Boiler_Steam :
    {
      model::BoilerSteam boiler = modelObject.cast<BoilerSteam>();
      retVal = translateBoilerSteam(boiler);
      break;
    }
  case openstudio::IddObjectType::OS_Building :
    {
      model::Building building = modelObject.cast<Building>();
      retVal = translateBuilding(building);
      break;
    }
  case openstudio::IddObjectType::OS_BuildingStory :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_Chiller_Electric_EIR :
    {
      model::ChillerElectricEIR chiller = modelObject.cast<ChillerElectricEIR>();
      retVal = translateChillerElectricEIR(chiller);
      break;
    }
  case openstudio::IddObjectType::OS_Construction_CfactorUndergroundWall :
    {
      model::CFactorUndergroundWallConstruction construction = modelObject.cast<CFactorUndergroundWallConstruction>();
      retVal = translateCFactorUndergroundWallConstruction(construction);
      break;
    }
  case openstudio::IddObjectType::OS_ConvergenceLimits :
    {
      model::ConvergenceLimits limits = modelObject.cast<ConvergenceLimits>();
      retVal = translateConvergenceLimits(limits);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed :
    {
      model::CoilCoolingDXSingleSpeed coil = modelObject.cast<CoilCoolingDXSingleSpeed>();
      retVal = translateCoilCoolingDXSingleSpeed(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoSpeed :
    {
      model::CoilCoolingDXTwoSpeed coil = modelObject.cast<CoilCoolingDXTwoSpeed>();
      retVal = translateCoilCoolingDXTwoSpeed(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_ConstantFlow :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_LowTemperatureRadiant_VariableFlow :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_Water :
    {
      model::CoilCoolingWater coil = modelObject.cast<CoilCoolingWater>();
      retVal = translateCoilCoolingWater(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit :
    {
      model::CoilCoolingWaterToAirHeatPumpEquationFit coil = modelObject.cast<CoilCoolingWaterToAirHeatPumpEquationFit>();
      retVal = translateCoilCoolingWaterToAirHeatPumpEquationFit(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_DX_SingleSpeed :
    {
      model::CoilHeatingDXSingleSpeed coil = modelObject.cast<CoilHeatingDXSingleSpeed>();
      retVal = translateCoilHeatingDXSingleSpeed(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_Electric :
    {
      model::CoilHeatingElectric coil = modelObject.cast<CoilHeatingElectric>();
      retVal = translateCoilHeatingElectric(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_Gas :
    {
      model::CoilHeatingGas coil = modelObject.cast<CoilHeatingGas>();
      retVal = translateCoilHeatingGas(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_ConstantFlow :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_LowTemperatureRadiant_VariableFlow :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_Water :
    {
      model::CoilHeatingWater coil = modelObject.cast<CoilHeatingWater>();
      retVal = translateCoilHeatingWater(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit :
    {
      model::CoilHeatingWaterToAirHeatPumpEquationFit coil = modelObject.cast<CoilHeatingWaterToAirHeatPumpEquationFit>();
      retVal = translateCoilHeatingWaterToAirHeatPumpEquationFit(coil);
      break;
    }
  case openstudio::IddObjectType::OS_ComponentData :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_ComponentCost_Adjustments :
    {
      LOG(Warn, "OS:ComponentCost:Adjustments '" << modelObject.name().get() << "' not translated to EnergyPlus.");
      return retVal;
    }
  case openstudio::IddObjectType::OS_Connection :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_Connector_Mixer :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_Connector_Splitter :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_Construction :
    {
      model::Construction construction = modelObject.cast<Construction>();
      retVal = translateConstruction(construction);
      break;
    }
  case openstudio::IddObjectType::OS_Construction_InternalSource :
    {
      model::ConstructionWithInternalSource constructionIntSource = modelObject.cast<ConstructionWithInternalSource>();
      retVal = translateConstructionWithInternalSource(constructionIntSource);
      break;
    }
  case openstudio::IddObjectType::OS_Construction_FfactorGroundFloor :
    {
      model::FFactorGroundFloorConstruction mo = modelObject.cast<FFactorGroundFloorConstruction>();
      retVal = translateFFactorGroundFloorConstruction(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Controller_MechanicalVentilation :
    {
      model::ControllerMechanicalVentilation controller = modelObject.cast<ControllerMechanicalVentilation>();
      retVal = translateControllerMechanicalVentilation(controller);
      break;
    }
  case openstudio::IddObjectType::OS_Controller_OutdoorAir :
    {
      model::ControllerOutdoorAir controller = modelObject.cast<ControllerOutdoorAir>();
      retVal = translateControllerOutdoorAir(controller);
      break;
    }
  case openstudio::IddObjectType::OS_Controller_WaterCoil :
    {
      model::ControllerWaterCoil controller = modelObject.cast<ControllerWaterCoil>();
      retVal = translateControllerWaterCoil(controller);
      break;
    }
  case openstudio::IddObjectType::OS_CoolingTower_SingleSpeed :
    {
      model::CoolingTowerSingleSpeed tower = modelObject.cast<CoolingTowerSingleSpeed>();
      retVal = translateCoolingTowerSingleSpeed(tower);
      break;
    }
  case openstudio::IddObjectType::OS_CurrencyType :
    {
      model::CurrencyType mo = modelObject.cast<CurrencyType>();
      retVal = translateCurrencyType(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_Bicubic :
    {
      model::CurveBicubic curve = modelObject.cast<CurveBicubic>();
      retVal = translateCurveBicubic(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_Biquadratic :
    {
      model::CurveBiquadratic curve = modelObject.cast<CurveBiquadratic>();
      retVal = translateCurveBiquadratic(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_Cubic :
    {
      model::CurveCubic curve = modelObject.cast<CurveCubic>();
      retVal = translateCurveCubic(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_DoubleExponentialDecay :
    {
      model::CurveDoubleExponentialDecay curve = modelObject.cast<CurveDoubleExponentialDecay>();
      retVal = translateCurveDoubleExponentialDecay(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_Exponent :
    {
      model::CurveExponent curve = modelObject.cast<CurveExponent>();
      retVal = translateCurveExponent(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_ExponentialDecay :
    {
      model::CurveExponentialDecay curve = modelObject.cast<CurveExponentialDecay>();
      retVal = translateCurveExponentialDecay(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_ExponentialSkewNormal :
    {
      model::CurveExponentialSkewNormal curve = modelObject.cast<CurveExponentialSkewNormal>();
      retVal = translateCurveExponentialSkewNormal(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_FanPressureRise :
    {
      model::CurveFanPressureRise curve = modelObject.cast<CurveFanPressureRise>();
      retVal = translateCurveFanPressureRise(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_Functional_PressureDrop :
    {
      model::CurveFunctionalPressureDrop curve = modelObject.cast<CurveFunctionalPressureDrop>();
      retVal = translateCurveFunctionalPressureDrop(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_Linear :
    {
      model::CurveLinear curve = modelObject.cast<CurveLinear>();
      retVal = translateCurveLinear(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_Quadratic :
    {
      model::CurveQuadratic curve = modelObject.cast<CurveQuadratic>();
      retVal = translateCurveQuadratic(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_QuadraticLinear :
    {
      model::CurveQuadraticLinear curve = modelObject.cast<CurveQuadraticLinear>();
      retVal = translateCurveQuadraticLinear(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_Quartic :
    {
      model::CurveQuartic curve = modelObject.cast<CurveQuartic>();
      retVal = translateCurveQuartic(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_RectangularHyperbola1 :
    {
      model::CurveRectangularHyperbola1 curve = modelObject.cast<CurveRectangularHyperbola1>();
      retVal = translateCurveRectangularHyperbola1(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_RectangularHyperbola2 :
    {
      model::CurveRectangularHyperbola2 curve = modelObject.cast<CurveRectangularHyperbola2>();
      retVal = translateCurveRectangularHyperbola2(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_Sigmoid :
    {
      model::CurveSigmoid curve = modelObject.cast<CurveSigmoid>();
      retVal = translateCurveSigmoid(curve);
      break;
    }
  case openstudio::IddObjectType::OS_Curve_Triquadratic :
    {
      model::CurveTriquadratic curve = modelObject.cast<CurveTriquadratic>();
      retVal = translateCurveTriquadratic(curve);
      break;
    }
  case openstudio::IddObjectType::OS_SizingPeriod_DesignDay :
    {
      model::DesignDay designDay = modelObject.cast<DesignDay>();
      retVal = translateDesignDay(designDay);
      break;
    }
  case openstudio::IddObjectType::OS_Sizing_Plant :
    {
      model::SizingPlant sizingPlant = modelObject.cast<SizingPlant>();
      retVal = translateSizingPlant(sizingPlant);
      break;
    }
  case openstudio::IddObjectType::OS_Sizing_System :
    {
      model::SizingSystem mo = modelObject.cast<SizingSystem>();
      retVal = translateSizingSystem(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Sizing_Zone :
    {
      model::SizingZone mo = modelObject.cast<SizingZone>();
      retVal = translateSizingZone(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Daylighting_Control :
    {
      model::DaylightingControl daylightingControl = modelObject.cast<DaylightingControl>();
      retVal = translateDaylightingControl(daylightingControl);
      break;
    }

  case openstudio::IddObjectType::OS_DaylightingDevice_Shelf :
    {
      model::DaylightingDeviceShelf daylightingDeviceShelf = modelObject.cast<DaylightingDeviceShelf>();
      retVal = translateDaylightingDeviceShelf(daylightingDeviceShelf);
      break;
    }
  case openstudio::IddObjectType::OS_DefaultConstructionSet :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_DefaultScheduleSet :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_DefaultSurfaceConstructions :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_DefaultSubSurfaceConstructions :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_DesignSpecification_OutdoorAir :
    {
      model::DesignSpecificationOutdoorAir designSpecificationOutdoorAir = modelObject.cast<DesignSpecificationOutdoorAir>();
      retVal = translateDesignSpecificationOutdoorAir(designSpecificationOutdoorAir);
      break;
    }
  case openstudio::IddObjectType::OS_DistrictCooling :
    {
      model::DistrictCooling districtCooling = modelObject.cast<DistrictCooling>();
      retVal = translateDistrictCooling(districtCooling);
      break;
    }
  case openstudio::IddObjectType::OS_DistrictHeating :
    {
      model::DistrictHeating districtHeating = modelObject.cast<DistrictHeating>();
      retVal = translateDistrictHeating(districtHeating);
      break;
    }
  case openstudio::IddObjectType::OS_ElectricEquipment :
    {
      model::ElectricEquipment equipment = modelObject.cast<ElectricEquipment>();
      retVal = translateElectricEquipment(equipment);
      break;
    }     
  case openstudio::IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial :
    {
      model::EvaporativeCoolerDirectResearchSpecial evap = modelObject.cast<EvaporativeCoolerDirectResearchSpecial>();
      retVal = translateEvaporativeCoolerDirectResearchSpecial(evap);
      break;
    }
  case openstudio::IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed :
    {
      model::EvaporativeFluidCoolerSingleSpeed evap = modelObject.cast<EvaporativeFluidCoolerSingleSpeed>();
      retVal = translateEvaporativeFluidCoolerSingleSpeed(evap);
      break;
    }
  case openstudio::IddObjectType::OS_Exterior_Lights :
    {
      model::ExteriorLights lights = modelObject.cast<ExteriorLights>();
      retVal = translateExteriorLights(lights);
      break;
    }
  case openstudio::IddObjectType::OS_Facility :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_Fan_ConstantVolume :
    {
      model::FanConstantVolume fan = modelObject.cast<FanConstantVolume>();
      retVal = translateFanConstantVolume(fan);
      break;
    }
  case openstudio::IddObjectType::OS_Fan_OnOff :
    {
      model::FanOnOff fan = modelObject.cast<FanOnOff>();
      retVal = translateFanOnOff(fan);
      break;
    }
  case openstudio::IddObjectType::OS_Fan_VariableVolume :
    {
      model::FanVariableVolume fan = modelObject.cast<FanVariableVolume>();
      retVal = translateFanVariableVolume(fan);
      break;
    }
  case openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical :
    {
      model::GroundHeatExchangerVertical groundHeatExchangerVertical = modelObject.cast<GroundHeatExchangerVertical>();
      retVal = translateGroundHeatExchangerVertical(groundHeatExchangerVertical);
      break;
    }
  case openstudio::IddObjectType::OS_HeatBalanceAlgorithm :
    {
      model::HeatBalanceAlgorithm mo = modelObject.cast<HeatBalanceAlgorithm>();
      retVal = translateHeatBalanceAlgorithm(mo);
      break;
    }
  case openstudio::IddObjectType::OS_HeatExchanger_AirToAir_SensibleAndLatent :
    {
      model::HeatExchangerAirToAirSensibleAndLatent mo = modelObject.cast<HeatExchangerAirToAirSensibleAndLatent>();
      retVal = translateHeatExchangerAirToAirSensibleAndLatent(mo);
      break;
    }
  case openstudio::IddObjectType::OS_HotWaterEquipment :
    {
      model::HotWaterEquipment hotWaterEquipment = modelObject.cast<HotWaterEquipment>();
      retVal = translateHotWaterEquipment(hotWaterEquipment);
      break;
    }
  case openstudio::IddObjectType::OS_WindowMaterial_Gas :
    {
      model::Gas gas = modelObject.cast<Gas>();
      retVal = translateGas(gas);
      break;
    }
  case openstudio::IddObjectType::OS_GasEquipment :
    {
      model::GasEquipment equipment = modelObject.cast<GasEquipment>();
      retVal = translateGasEquipment(equipment);
      break;
    }
  case openstudio::IddObjectType::OS_IlluminanceMap :
    {
      model::IlluminanceMap illuminanceMap = modelObject.cast<IlluminanceMap>();
      retVal = translateIlluminanceMap(illuminanceMap);
      break;
    }
  case openstudio::IddObjectType::OS_InteriorPartitionSurface :
    {
      model::InteriorPartitionSurface interiorPartitionSurface = modelObject.cast<InteriorPartitionSurface>();
      retVal = translateInteriorPartitionSurface(interiorPartitionSurface);
      break;
    }
  case openstudio::IddObjectType::OS_InteriorPartitionSurfaceGroup :
    {
      model::InteriorPartitionSurfaceGroup interiorPartitionSurfaceGroup = modelObject.cast<InteriorPartitionSurfaceGroup>();
      retVal = translateInteriorPartitionSurfaceGroup(interiorPartitionSurfaceGroup);
      break;
    }
  case openstudio::IddObjectType::OS_InternalMass :
    {
      model::InternalMass internalMass = modelObject.cast<InternalMass>();
      retVal = translateInternalMass(internalMass);
      break;
    }
  case openstudio::IddObjectType::OS_WaterHeater_Mixed :
    {
      model::WaterHeaterMixed waterHeaterMixed = modelObject.cast<WaterHeaterMixed>();
      retVal = translateWaterHeaterMixed(waterHeaterMixed);
      break;
    }
  case openstudio::IddObjectType::OS_WaterUse_Connections :
    {
      model::WaterUseConnections waterUseConnections = modelObject.cast<WaterUseConnections>();
      retVal = translateWaterUseConnections(waterUseConnections);
      break;
    }
  case openstudio::IddObjectType::OS_WaterUse_Equipment :
    {
      model::WaterUseEquipment waterUseEquipment = modelObject.cast<WaterUseEquipment>();
      retVal = translateWaterUseEquipment(waterUseEquipment);
      break;
    }
  case openstudio::IddObjectType::OS_WindowMaterial_GasMixture :
    {
      model::GasMixture gasMixture = modelObject.cast<GasMixture>();
      retVal = translateGasMixture(gasMixture);
      break;
    }
  case openstudio::IddObjectType::OS_LifeCycleCost :
    {
      model::LifeCycleCost lifeCycleCost = modelObject.cast<LifeCycleCost>();
      retVal = translateLifeCycleCost(lifeCycleCost);
      break;
    }
  case openstudio::IddObjectType::OS_LifeCycleCost_Parameters :
    {
      model::LifeCycleCostParameters lifeCycleCostParameters = modelObject.cast<LifeCycleCostParameters>();
      retVal = translateLifeCycleCostParameters(lifeCycleCostParameters);
      break;
    }
  case openstudio::IddObjectType::OS_LifeCycleCost_UsePriceEscalation :
    {
      // DLM: these objects can be created from LifeCycleCostParameters
      LOG(Warn, "OS:LifeCycleCost:UsePriceEscalation '" << modelObject.name().get() << "' not translated to EnergyPlus.");
      return retVal;
    }
  case openstudio::IddObjectType::OS_Lights :
    {
      model::Lights lights = modelObject.cast<Lights>();
      retVal = translateLights(lights);
      break;
    }
  case openstudio::IddObjectType::OS_Luminaire :
    {
      model::Luminaire luminaire = modelObject.cast<Luminaire>();
      retVal = translateLuminaire(luminaire);
      break;
    }
  case openstudio::IddObjectType::OS_Material :
    {
      model::StandardOpaqueMaterial material = modelObject.cast<StandardOpaqueMaterial>();
      retVal = translateStandardOpaqueMaterial(material);
      break;
    }
  case openstudio::IddObjectType::OS_Material_AirGap :
    {
      model::AirGap material = modelObject.cast<AirGap>();
      retVal = translateAirGap(material);
      break;
    }
  case openstudio::IddObjectType::OS_Material_InfraredTransparent :
    {
      model::InfraredTransparentMaterial material = modelObject.cast<InfraredTransparentMaterial>();
      retVal = translateInfraredTransparentMaterial(material);
      break;
    }
  case openstudio::IddObjectType::OS_Material_NoMass :
    {
      model::MasslessOpaqueMaterial material = modelObject.cast<MasslessOpaqueMaterial>();
      retVal = translateMasslessOpaqueMaterial(material);
      break;
    }
  case openstudio::IddObjectType::OS_Material_RoofVegetation :
    {
      model::RoofVegetation material = modelObject.cast<RoofVegetation>();
      retVal = translateRoofVegetation(material);
      break;
    }
  case openstudio::IddObjectType::OS_Meter :
    {
      model::Meter meter = modelObject.cast<Meter>();
      retVal = translateMeter(meter);
      break;
    }
  case openstudio::IddObjectType::OS_Node :
    {
      model::Node node = modelObject.cast<Node>();
      retVal = translateNode(node);
      break;
    }
  case openstudio::IddObjectType::OS_OtherEquipment :
    {
      model::OtherEquipment otherEquipment = modelObject.cast<OtherEquipment>();
      retVal = translateOtherEquipment(otherEquipment);
      break;
    }
  case openstudio::IddObjectType::OS_PlantLoop :
    {
      PlantLoop plantLoop = modelObject.cast<PlantLoop>();
      retVal = translatePlantLoop(plantLoop);
      break;
    }
  case openstudio::IddObjectType::OS_Pump_ConstantSpeed :
    {
      PumpConstantSpeed pump = modelObject.cast<PumpConstantSpeed>();
      retVal = translatePumpConstantSpeed(pump);
      break;
    }
  case openstudio::IddObjectType::OS_Pump_VariableSpeed :
    {
      PumpVariableSpeed pump = modelObject.cast<PumpVariableSpeed>();
      retVal = translatePumpVariableSpeed(pump);
      break;
    }
  case openstudio::IddObjectType::OS_OutputControl_ReportingTolerances :
    {
      model::OutputControlReportingTolerances outputControl = modelObject.cast<OutputControlReportingTolerances>();
      retVal = translateOutputControlReportingTolerances(outputControl);
      break;
    }
  case openstudio::IddObjectType::OS_Output_Variable :
    {
      model::OutputVariable outputVariable = modelObject.cast<OutputVariable>();
      retVal = translateOutputVariable(outputVariable);
      break;
    }
  case openstudio::IddObjectType::OS_People :
    {
      model::People people = modelObject.cast<People>();
      retVal = translatePeople(people);
      break;
    }
  case openstudio::IddObjectType::OS_Pipe_Adiabatic :
    {
      model::PipeAdiabatic pipe = modelObject.cast<PipeAdiabatic>();
      retVal = translatePipeAdiabatic(pipe);
      break;
    }
  case openstudio::IddObjectType::OS_PortList :
    {
      model::PortList portList = modelObject.cast<PortList>();
      retVal = translatePortList(portList);
      break;
    }
  case openstudio::IddObjectType::OS_Rendering_Color :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_RunPeriod :
    {
      model::RunPeriod runPeriod = modelObject.cast<RunPeriod>();
      retVal = translateRunPeriod(runPeriod);
      break;
    }
  case openstudio::IddObjectType::OS_RunPeriodControl_DaylightSavingTime :
    {
      model::RunPeriodControlDaylightSavingTime mo = modelObject.cast<RunPeriodControlDaylightSavingTime>();
      retVal = translateRunPeriodControlDaylightSavingTime(mo);
      break;
    }
  case openstudio::IddObjectType::OS_RunPeriodControl_SpecialDays :
    {
      model::RunPeriodControlSpecialDays mo = modelObject.cast<RunPeriodControlSpecialDays>();
      retVal = translateRunPeriodControlSpecialDays(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Schedule_Compact :
    {
      model::ScheduleCompact schedule = modelObject.cast<ScheduleCompact>();
      retVal = translateScheduleCompact(schedule);
      break;
    }
  case openstudio::IddObjectType::OS_Schedule_Constant :
    {
      model::ScheduleConstant schedule = modelObject.cast<ScheduleConstant>();
      retVal = translateScheduleConstant(schedule);
      break;
    }
  case openstudio::IddObjectType::OS_Schedule_Day :
    {
      model::ScheduleDay schedule = modelObject.cast<ScheduleDay>();
      retVal = translateScheduleDay(schedule);
      break;
    }
  case  openstudio::IddObjectType::OS_Schedule_FixedInterval :
    {
      model::ScheduleFixedInterval schedule = modelObject.cast<ScheduleFixedInterval>();
      retVal = translateScheduleFixedInterval(schedule);
      break;
    }
  case  openstudio::IddObjectType::OS_Schedule_Rule :
    {
      // no-op
      break;
    }
  case  openstudio::IddObjectType::OS_Schedule_Ruleset :
    {
      model::ScheduleRuleset mo = modelObject.cast<ScheduleRuleset>();
      retVal = translateScheduleRuleset(mo);
      break;
    }
  case  openstudio::IddObjectType::OS_ScheduleTypeLimits :
    {
      model::ScheduleTypeLimits mo = modelObject.cast<ScheduleTypeLimits>();
      retVal = translateScheduleTypeLimits(mo);
      break;
    }
  case  openstudio::IddObjectType::OS_Schedule_Week :
    {
      model::ScheduleWeek schedule = modelObject.cast<ScheduleWeek>();
      retVal = translateScheduleWeek(schedule);
      break;
    }
  case  openstudio::IddObjectType::OS_Schedule_Year :
    {
      model::ScheduleYear schedule = modelObject.cast<ScheduleYear>();
      retVal = translateScheduleYear(schedule);
      break;
    }
  case  openstudio::IddObjectType::OS_Schedule_VariableInterval :
    {
      model::ScheduleVariableInterval schedule = modelObject.cast<ScheduleVariableInterval>();
      retVal = translateScheduleVariableInterval(schedule);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_FollowOutdoorAirTemperature :
    {
      model::SetpointManagerFollowOutdoorAirTemperature spm = modelObject.cast<SetpointManagerFollowOutdoorAirTemperature>();
      retVal = translateSetpointManagerFollowOutdoorAirTemperature(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_SingleZone_Reheat :
    {
      model::SetpointManagerSingleZoneReheat spm = modelObject.cast<SetpointManagerSingleZoneReheat>();
      retVal = translateSetpointManagerSingleZoneReheat(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_Scheduled :
    {
      model::SetpointManagerScheduled spm = modelObject.cast<SetpointManagerScheduled>();
      retVal = translateSetpointManagerScheduled(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_MixedAir :
    {
      model::SetpointManagerMixedAir spm = modelObject.cast<SetpointManagerMixedAir>();
      retVal = translateSetpointManagerMixedAir(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_OutdoorAirReset :
    {
      model::SetpointManagerOutdoorAirReset spm = modelObject.cast<SetpointManagerOutdoorAirReset>();
      retVal = translateSetpointManagerOutdoorAirReset(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_Warmest :
    {
      model::SetpointManagerWarmest spm = modelObject.cast<SetpointManagerWarmest>();
      retVal = translateSetpointManagerWarmest(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_ShadingControl :
    {
      model::ShadingControl shadingControl = modelObject.cast<ShadingControl>();
      retVal = translateShadingControl(shadingControl);
      break;
    }    
  case  openstudio::IddObjectType::OS_ShadingSurface :
    {
      model::ShadingSurface shadingSurface = modelObject.cast<ShadingSurface>();
      retVal = translateShadingSurface(shadingSurface);
      break;
    }
  case  openstudio::IddObjectType::OS_ShadingSurfaceGroup :
    {
      model::ShadingSurfaceGroup shadingSurfaceGroup = modelObject.cast<ShadingSurfaceGroup>();
      retVal = translateShadingSurfaceGroup(shadingSurfaceGroup);
      break;
    }
  case  openstudio::IddObjectType::OS_ShadowCalculation :
    {
      model::ShadowCalculation mo = modelObject.cast<ShadowCalculation>();
      retVal = translateShadowCalculation(mo);
      break;
    }
  case  openstudio::IddObjectType::OS_SimulationControl :
    {
      model::SimulationControl simulationControl = modelObject.cast<SimulationControl>();
      retVal = translateSimulationControl(simulationControl);
      break;
    }
  case openstudio::IddObjectType::OS_Space :
    {
      model::Space space = modelObject.cast<Space>();
      retVal = translateSpace(space);
      break;
    }
  case openstudio::IddObjectType::OS_SpaceInfiltration_DesignFlowRate :
    {
      model::SpaceInfiltrationDesignFlowRate spaceInfiltrationDesignFlowRate = modelObject.cast<SpaceInfiltrationDesignFlowRate>();
      retVal = translateSpaceInfiltrationDesignFlowRate(spaceInfiltrationDesignFlowRate);
      break;
    }
  case openstudio::IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea :
    {
      model::SpaceInfiltrationEffectiveLeakageArea spaceInfiltrationEffectiveLeakageArea = modelObject.cast<SpaceInfiltrationEffectiveLeakageArea>();
      retVal = translateSpaceInfiltrationEffectiveLeakageArea(spaceInfiltrationEffectiveLeakageArea);
      break;
    }
  case openstudio::IddObjectType::OS_SpaceType :
    {
      model::SpaceType spaceType = modelObject.cast<SpaceType>();
      retVal = translateSpaceType(spaceType);
      break;
    }
  case openstudio::IddObjectType::OS_Site :
    {
      model::Site site = modelObject.cast<Site>();
      retVal = translateSite(site);
      break;
    }
  case openstudio::IddObjectType::OS_Site_GroundReflectance :
    {
      model::SiteGroundReflectance mo = modelObject.cast<SiteGroundReflectance>();
      retVal = translateSiteGroundReflectance(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Site_GroundTemperature_BuildingSurface :
    {
      model::SiteGroundTemperatureBuildingSurface mo = modelObject.cast<SiteGroundTemperatureBuildingSurface>();
      retVal = translateSiteGroundTemperatureBuildingSurface(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Site_WaterMainsTemperature :
    {
      model::SiteWaterMainsTemperature mo = modelObject.cast<SiteWaterMainsTemperature>();
      retVal = translateSiteWaterMainsTemperature(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Sizing_Parameters :
    {
      model::SizingParameters mo = modelObject.cast<SizingParameters>();
      retVal = translateSizingParameters(mo);
      break;
    }
  case openstudio::IddObjectType::OS_StandardsInformation_Construction :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_SteamEquipment :
    {
      model::SteamEquipment steamEquipment = modelObject.cast<SteamEquipment>();
      retVal = translateSteamEquipment(steamEquipment);
      break;
    }
  case openstudio::IddObjectType::OS_Surface :
    {
      model::Surface surface = modelObject.cast<Surface>();
      retVal = translateSurface(surface);
      break;
    }
  case openstudio::IddObjectType::OS_SurfaceConvectionAlgorithm_Inside :
    {
      model::InsideSurfaceConvectionAlgorithm mo = modelObject.cast<InsideSurfaceConvectionAlgorithm>();
      retVal = translateInsideSurfaceConvectionAlgorithm(mo);
      break;
    }
  case openstudio::IddObjectType::OS_SurfaceConvectionAlgorithm_Outside :
    {
      model::OutsideSurfaceConvectionAlgorithm mo = modelObject.cast<OutsideSurfaceConvectionAlgorithm>();
      retVal = translateOutsideSurfaceConvectionAlgorithm(mo);
      break;
    }
  case openstudio::IddObjectType::OS_SubSurface :
    {
      model::SubSurface subSurface = modelObject.cast<SubSurface>();
      retVal = translateSubSurface(subSurface);
      break;
    }
  case openstudio::IddObjectType::OS_ThermalZone :
    {
      model::ThermalZone zone = modelObject.cast<ThermalZone>();
      retVal = translateThermalZone(zone);
      break;
    }
  case openstudio::IddObjectType::OS_ThermostatSetpoint_DualSetpoint :
    {
      model::ThermostatSetpointDualSetpoint thermostat = modelObject.cast<ThermostatSetpointDualSetpoint>();
      retVal = translateThermostatSetpointDualSetpoint(thermostat);
      break;
    }
  case openstudio::IddObjectType::OS_Timestep :
    {
      model::Timestep timestep = modelObject.cast<Timestep>();
      retVal = translateTimestep(timestep);
      break;
    }
  case openstudio::IddObjectType::OS_Version :
    {
      model::Version version = modelObject.cast<Version>();
      retVal = translateVersion(version);
      break;
    }
  case openstudio::IddObjectType::OS_WeatherFile :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_WeatherProperty_SkyTemperature :
    {
      model::SkyTemperature mo = modelObject.cast<SkyTemperature>();
      retVal = translateSkyTemperature(mo);
      break;
    }
  case openstudio::IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod :
    {
      model::RefractionExtinctionGlazing mo = modelObject.cast<RefractionExtinctionGlazing>();
      retVal = translateRefractionExtinctionGlazing(mo);
      break;
    }
  case openstudio::IddObjectType::OS_WindowMaterial_Screen :
    {
      model::Screen mo = modelObject.cast<Screen>();
      retVal = translateScreen(mo);
      break;
    }
  case openstudio::IddObjectType::OS_WindowMaterial_Shade :
    {
      model::Shade shade = modelObject.cast<Shade>();
      retVal = translateShade(shade);
      break;
    }
  case openstudio::IddObjectType::OS_WindowMaterial_SimpleGlazingSystem :
    {
      model::SimpleGlazing glazing = modelObject.cast<SimpleGlazing>();
      retVal = translateSimpleGlazing(glazing);
      break;
    }
  case openstudio::IddObjectType::OS_WindowMaterial_Glazing :
    {
      model::StandardGlazing glazing = modelObject.cast<StandardGlazing>();
      retVal = translateStandardGlazing(glazing);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneAirContaminantBalance :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_ZoneAirHeatBalanceAlgorithm :
    {
      model::ZoneAirHeatBalanceAlgorithm mo = modelObject.cast<ZoneAirHeatBalanceAlgorithm>();
      retVal = translateZoneAirHeatBalanceAlgorithm(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneCapacitanceMultiplier_ResearchSpecial :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric :
    {
      model::ZoneHVACBaseboardConvectiveElectric mo = modelObject.cast<ZoneHVACBaseboardConvectiveElectric>();
      retVal = translateZoneHVACBaseboardConvectiveElectric(mo);
      break;
    }  
  case openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water :
    {
      model::ZoneHVACBaseboardConvectiveWater mo = modelObject.cast<ZoneHVACBaseboardConvectiveWater>();
      retVal = translateZoneHVACBaseboardConvectiveWater(mo);
      break;
    }  
  case openstudio::IddObjectType::OS_ZoneHVAC_EquipmentList :
    {
      model::ZoneHVACEquipmentList mo = modelObject.cast<ZoneHVACEquipmentList>();
      retVal = translateZoneHVACEquipmentList(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_FourPipeFanCoil :
    {
      model::ZoneHVACFourPipeFanCoil mo = modelObject.cast<ZoneHVACFourPipeFanCoil>();
      retVal = translateZoneHVACFourPipeFanCoil(mo);
      break;
    }  
  case openstudio::IddObjectType::OS_ZoneHVAC_IdealLoadsAirSystem :
    {
      model::ZoneHVACIdealLoadsAirSystem mo = modelObject.cast<ZoneHVACIdealLoadsAirSystem>();
      retVal = translateZoneHVACIdealLoadsAirSystem(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow :
    { 
      model::ZoneHVACLowTempRadiantConstFlow mo = modelObject.cast<ZoneHVACLowTempRadiantConstFlow>();
      retVal = translateZoneHVACLowTempRadiantConstFlow(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow :
    { 
      model::ZoneHVACLowTempRadiantVarFlow mo = modelObject.cast<ZoneHVACLowTempRadiantVarFlow>();
      retVal = translateZoneHVACLowTempRadiantVarFlow(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric :
    {
      model::ZoneHVACLowTemperatureRadiantElectric mo = modelObject.cast<ZoneHVACLowTemperatureRadiantElectric>();
      retVal = translateZoneHVACLowTemperatureRadiantElectric(mo);
      break;
    }  
  case openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump :
    {
      model::ZoneHVACPackagedTerminalHeatPump mo = modelObject.cast<ZoneHVACPackagedTerminalHeatPump>();
      retVal = translateZoneHVACPackagedTerminalHeatPump(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner :
    {
      model::ZoneHVACPackagedTerminalAirConditioner mo = modelObject.cast<ZoneHVACPackagedTerminalAirConditioner>();
      retVal = translateZoneHVACPackagedTerminalAirConditioner(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump :
    {
      model::ZoneHVACWaterToAirHeatPump mo = modelObject.cast<ZoneHVACWaterToAirHeatPump>();
      retVal = translateZoneHVACWaterToAirHeatPump(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_UnitHeater :
    {
      model::ZoneHVACUnitHeater mo = modelObject.cast<ZoneHVACUnitHeater>();
      retVal = translateZoneHVACUnitHeater(mo);
      break;
    }
  //If no case statement log a warning
  default:
    {
      LOG(Warn, "Unknown IddObjectType: '" << modelObject.iddObject().name() << "'");
      return retVal;
    }
  }

  if(retVal)
  {
    m_map.insert(make_pair(modelObject.handle(),retVal.get()));

    if (m_progressBar){
      m_progressBar->setValue(m_map.size());
    }
  }

  // is this redundant?
  // ETH@20120112 Yes.
  OptionalParentObject opo = modelObject.optionalCast<ParentObject>();
  if(opo)
  {
    ModelObjectVector children = opo->children();
    IddObjectTypeVector types = iddObjectsToTranslate();

    // sort these objects as well
    std::sort(children.begin(), children.end(), ChildSorter(types));
    
    for(ModelObjectVector::iterator c=children.begin(),cend=children.end();c!=cend;++c)
    {
      if (std::find(types.begin(),types.end(),c->iddObject().type()) != types.end()) {
        translateAndMapModelObject(*c);
      }
    }
  }

  return retVal;
}

std::string ForwardTranslator::stripOS2(const string& s)
{
  std::string result;
  if( s.substr(0,3) == "OS:" )
  {
    result = s.substr(3);
  }
  else
  {
    result = s;
  }
  return result;
}

std::vector<IddObjectType> ForwardTranslator::iddObjectsToTranslate()
{
  static std::vector<IddObjectType> result = iddObjectsToTranslateInitializer();
  return result;
}

std::vector<IddObjectType> ForwardTranslator::iddObjectsToTranslateInitializer()
{
  std::vector<IddObjectType> result;

  result.push_back(IddObjectType::OS_Version);

  result.push_back(IddObjectType::OS_ComponentData);

  result.push_back(IddObjectType::OS_LifeCycleCost_Parameters);
  result.push_back(IddObjectType::OS_CurrencyType);
  result.push_back(IddObjectType::OS_ComponentCost_Adjustments);
  result.push_back(IddObjectType::OS_LifeCycleCost_UsePriceEscalation);

  result.push_back(IddObjectType::OS_SimulationControl);
  result.push_back(IddObjectType::OS_ConvergenceLimits);
  result.push_back(IddObjectType::OS_HeatBalanceAlgorithm);
  result.push_back(IddObjectType::OS_RunPeriod);
  result.push_back(IddObjectType::OS_RunPeriodControl_DaylightSavingTime);
  result.push_back(IddObjectType::OS_RunPeriodControl_SpecialDays);
  result.push_back(IddObjectType::OS_ShadowCalculation);
  result.push_back(IddObjectType::OS_SurfaceConvectionAlgorithm_Inside);
  result.push_back(IddObjectType::OS_SurfaceConvectionAlgorithm_Outside);
  result.push_back(IddObjectType::OS_Timestep);
  result.push_back(IddObjectType::OS_Sizing_Parameters);
  result.push_back(IddObjectType::OS_ZoneAirContaminantBalance);
  result.push_back(IddObjectType::OS_ZoneAirHeatBalanceAlgorithm);
  result.push_back(IddObjectType::OS_ZoneCapacitanceMultiplier_ResearchSpecial);
  result.push_back(IddObjectType::OS_OutputControl_ReportingTolerances);

  result.push_back(IddObjectType::OS_Site);
  result.push_back(IddObjectType::OS_Site_GroundReflectance);
  result.push_back(IddObjectType::OS_Site_GroundTemperature_BuildingSurface);
  result.push_back(IddObjectType::OS_Site_WaterMainsTemperature);
  result.push_back(IddObjectType::OS_ClimateZones);
  result.push_back(IddObjectType::OS_SizingPeriod_DesignDay);
  result.push_back(IddObjectType::OS_SizingPeriod_WeatherFileConditionType);
  result.push_back(IddObjectType::OS_SizingPeriod_WeatherFileDays);

  result.push_back(IddObjectType::OS_UtilityCost_Charge_Block);
  result.push_back(IddObjectType::OS_UtilityCost_Charge_Simple);
  result.push_back(IddObjectType::OS_UtilityCost_Computation);
  result.push_back(IddObjectType::OS_UtilityCost_Qualify);
  result.push_back(IddObjectType::OS_UtilityCost_Ratchet);
  result.push_back(IddObjectType::OS_UtilityCost_Tariff);
  result.push_back(IddObjectType::OS_UtilityCost_Variable);

  result.push_back(IddObjectType::OS_WeatherFile);
  result.push_back(IddObjectType::OS_WeatherProperty_SkyTemperature);

  result.push_back(IddObjectType::OS_Rendering_Color);
  result.push_back(IddObjectType::OS_SpaceType);

  result.push_back(IddObjectType::OS_Facility);
  result.push_back(IddObjectType::OS_Building);
  result.push_back(IddObjectType::OS_StandardsInformation_Building);
  result.push_back(IddObjectType::OS_BuildingStory);
  result.push_back(IddObjectType::OS_LightingSimulationZone);

  result.push_back(IddObjectType::OS_ThermalZone);
  result.push_back(IddObjectType::OS_Space);
  result.push_back(IddObjectType::OS_Surface);
  result.push_back(IddObjectType::OS_SubSurface);
  result.push_back(IddObjectType::OS_InteriorPartitionSurfaceGroup);
  result.push_back(IddObjectType::OS_InteriorPartitionSurface);
  result.push_back(IddObjectType::OS_ShadingSurfaceGroup);
  result.push_back(IddObjectType::OS_ShadingSurface);

  result.push_back(IddObjectType::OS_Daylighting_Control);
  result.push_back(IddObjectType::OS_DaylightingDevice_Shelf);
  result.push_back(IddObjectType::OS_IlluminanceMap);

  // Definition objects will be translated as needed by instance objects.
  result.push_back(IddObjectType::OS_InternalMass);
  result.push_back(IddObjectType::OS_People);
  result.push_back(IddObjectType::OS_Lights);
  result.push_back(IddObjectType::OS_Luminaire);
  result.push_back(IddObjectType::OS_ElectricEquipment);
  result.push_back(IddObjectType::OS_GasEquipment);
  result.push_back(IddObjectType::OS_HotWaterEquipment);
  result.push_back(IddObjectType::OS_SteamEquipment);
  result.push_back(IddObjectType::OS_OtherEquipment);
  result.push_back(IddObjectType::OS_SpaceInfiltration_DesignFlowRate);
  result.push_back(IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea);
  result.push_back(IddObjectType::OS_Exterior_Lights);

  result.push_back(IddObjectType::OS_AirLoopHVAC);
  result.push_back(IddObjectType::OS_AirLoopHVAC_ControllerList);
  result.push_back(IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem);
  result.push_back(IddObjectType::OS_AirLoopHVAC_UnitaryCoolOnly);
  result.push_back(IddObjectType::OS_AirLoopHVAC_ZoneMixer);
  result.push_back(IddObjectType::OS_AirLoopHVAC_ZoneSplitter);
  result.push_back(IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled);
  result.push_back(IddObjectType::OS_AvailabilityManagerAssignmentList);
  result.push_back(IddObjectType::OS_AvailabilityManager_Scheduled);
  result.push_back(IddObjectType::OS_Chiller_Electric_EIR);
  result.push_back(IddObjectType::OS_Coil_Cooling_DX_SingleSpeed);
  result.push_back(IddObjectType::OS_Coil_Cooling_DX_TwoSpeed);
  result.push_back(IddObjectType::OS_Coil_Cooling_Water);
	result.push_back(IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_EquationFit);
  result.push_back(IddObjectType::OS_Coil_Heating_Gas);
  result.push_back(IddObjectType::OS_Coil_Heating_Water);
	result.push_back(IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit);
  result.push_back(IddObjectType::OS_Connection);
  result.push_back(IddObjectType::OS_Connector_Mixer);
  result.push_back(IddObjectType::OS_Connector_Splitter);
  result.push_back(IddObjectType::OS_Controller_OutdoorAir);
  result.push_back(IddObjectType::OS_CoolingTower_SingleSpeed);
  result.push_back(IddObjectType::OS_Curve_Bicubic);
  result.push_back(IddObjectType::OS_Curve_Biquadratic);
  result.push_back(IddObjectType::OS_Curve_Cubic);
  result.push_back(IddObjectType::OS_Curve_DoubleExponentialDecay);
  result.push_back(IddObjectType::OS_Curve_Exponent);
  result.push_back(IddObjectType::OS_Curve_ExponentialDecay);
  result.push_back(IddObjectType::OS_Curve_ExponentialSkewNormal);
  result.push_back(IddObjectType::OS_Curve_FanPressureRise);
  result.push_back(IddObjectType::OS_Curve_Functional_PressureDrop);
  result.push_back(IddObjectType::OS_Curve_Linear);
  result.push_back(IddObjectType::OS_Curve_Quadratic);
  result.push_back(IddObjectType::OS_Curve_QuadraticLinear);
  result.push_back(IddObjectType::OS_Curve_Quartic);
  result.push_back(IddObjectType::OS_Curve_RectangularHyperbola1);
  result.push_back(IddObjectType::OS_Curve_RectangularHyperbola2);
  result.push_back(IddObjectType::OS_Curve_Sigmoid);
  result.push_back(IddObjectType::OS_Curve_Triquadratic);
  result.push_back(IddObjectType::OS_DistrictCooling);
  result.push_back(IddObjectType::OS_DistrictHeating); 
  result.push_back(IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial);
  result.push_back(IddObjectType::OS_Fan_ConstantVolume);
  result.push_back(IddObjectType::OS_Fan_OnOff);
  result.push_back(IddObjectType::OS_Node);
  result.push_back(IddObjectType::OS_PlantLoop);
  result.push_back(IddObjectType::OS_Splitter);
  result.push_back(IddObjectType::OS_ThermostatSetpoint_DualSetpoint);
  result.push_back(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water);
  result.push_back(IddObjectType::OS_ZoneHVAC_IdealLoadsAirSystem);
  result.push_back(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow);
  result.push_back(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow);
  result.push_back(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric);
  // put these down here so they have a chance to be translated with their "parent"
  result.push_back(IddObjectType::OS_LifeCycleCost);

  result.push_back(IddObjectType::OS_Meter);
  result.push_back(IddObjectType::OS_Output_Variable);

  return result;
}

void ForwardTranslator::translateConstructions(const model::Model & model)
{
  std::vector<IddObjectType> iddObjectTypes;
  iddObjectTypes.push_back(IddObjectType::OS_Material);
  iddObjectTypes.push_back(IddObjectType::OS_Material_AirGap);
  iddObjectTypes.push_back(IddObjectType::OS_Material_AirWall);
  iddObjectTypes.push_back(IddObjectType::OS_Material_InfraredTransparent);
  iddObjectTypes.push_back(IddObjectType::OS_Material_NoMass);
  iddObjectTypes.push_back(IddObjectType::OS_Material_RoofVegetation);

  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Blind);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Gas);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_GasMixture);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Glazing);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Screen);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Shade);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_SimpleGlazingSystem);
  iddObjectTypes.push_back(IddObjectType::OS_ShadingControl);

  iddObjectTypes.push_back(IddObjectType::OS_Construction);
  iddObjectTypes.push_back(IddObjectType::OS_Construction_CfactorUndergroundWall);
  iddObjectTypes.push_back(IddObjectType::OS_Construction_FfactorGroundFloor);
  iddObjectTypes.push_back(IddObjectType::OS_Construction_InternalSource);
  iddObjectTypes.push_back(IddObjectType::OS_Construction_WindowDataFile);
  iddObjectTypes.push_back(IddObjectType::OS_StandardsInformation_Construction);

  iddObjectTypes.push_back(IddObjectType::OS_DefaultSubSurfaceConstructions);
  iddObjectTypes.push_back(IddObjectType::OS_DefaultSurfaceConstructions);
  iddObjectTypes.push_back(IddObjectType::OS_DefaultConstructionSet);
  iddObjectTypes.push_back(IddObjectType::OS_DefaultScheduleSet);

  BOOST_FOREACH(const IddObjectType& iddObjectType, iddObjectTypes){
    
    // get objects by type in sorted order
    std::vector<WorkspaceObject> objects = model.getObjectsByType(iddObjectType);
    std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());

    BOOST_FOREACH(const WorkspaceObject& workspaceObject, objects){
      model::ModelObject modelObject = workspaceObject.cast<ModelObject>();
      boost::optional<IdfObject> result = translateAndMapModelObject(modelObject);
    }
  }
}

void ForwardTranslator::translateSchedules(const model::Model & model)
{

  // loop over schedule type limits
  std::vector<WorkspaceObject> objects = model.getObjectsByType(IddObjectType::OS_ScheduleTypeLimits);
  std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());
  BOOST_FOREACH(const WorkspaceObject& workspaceObject, objects){
    model::ModelObject modelObject = workspaceObject.cast<ModelObject>();
    translateAndMapModelObject(modelObject);
  }

  // now loop over all schedule types
  std::vector<IddObjectType> iddObjectTypes;
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_Compact);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_Constant);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_Day);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_Week);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_Year);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_Ruleset);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_FixedInterval);
  iddObjectTypes.push_back(IddObjectType::OS_Schedule_VariableInterval);

  BOOST_FOREACH(const IddObjectType& iddObjectType, iddObjectTypes){
    
    // get objects by type in sorted order
    objects = model.getObjectsByType(iddObjectType);
    std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());

    BOOST_FOREACH(const WorkspaceObject& workspaceObject, objects){
      model::ModelObject modelObject = workspaceObject.cast<ModelObject>();
      boost::optional<IdfObject> result = translateAndMapModelObject(modelObject);

      if (istringEqual("Always_On", workspaceObject.name().get())){
        m_alwaysOnSchedule = result;
      }

      if (istringEqual("Always_Off", workspaceObject.name().get())){
        m_alwaysOffSchedule = result;
      }
    }
  }
}

IdfObject ForwardTranslator::alwaysOnSchedule()
{
  if (m_alwaysOnSchedule){
    return *m_alwaysOnSchedule;
  }

  m_alwaysOnSchedule = IdfObject(IddObjectType::Schedule_Constant);
  m_alwaysOnSchedule->setName("Always_On");
  m_alwaysOnSchedule->setDouble(2, 1.0);

  m_idfObjects.push_back(*m_alwaysOnSchedule);

  return *m_alwaysOnSchedule;
}

IdfObject ForwardTranslator::alwaysOffSchedule()
{
  if (m_alwaysOffSchedule){
    return *m_alwaysOffSchedule;
  }

  m_alwaysOffSchedule = IdfObject(IddObjectType::Schedule_Constant);
  m_alwaysOffSchedule->setName("Always_Off");
  m_alwaysOffSchedule->setDouble(2, 0.0);

  m_idfObjects.push_back(*m_alwaysOffSchedule);

  return *m_alwaysOffSchedule;
}

model::ConstructionBase ForwardTranslator::reverseConstruction(const model::ConstructionBase& construction)
{
  std::map<Handle, ConstructionBase>::iterator it = m_constructionHandleToReversedConstructions.find(construction.handle());
  if (it != m_constructionHandleToReversedConstructions.end()){
    return it->second;
  }

  if (!construction.optionalCast<model::LayeredConstruction>()){
    m_constructionHandleToReversedConstructions.insert(std::make_pair<Handle, model::ConstructionBase>(construction.handle(), construction));
    return construction;
  }

  model::LayeredConstruction layeredConstruction = construction.cast<model::LayeredConstruction>();
  
  if (layeredConstruction.isSymmetric()){
    m_constructionHandleToReversedConstructions.insert(std::make_pair<Handle, model::ConstructionBase>(construction.handle(), construction));
    return construction;
  }

  if (construction.optionalCast<model::Construction>()){
    model::Construction reversed = construction.cast<model::Construction>().reverseConstruction();
    m_constructionHandleToReversedConstructions.insert(std::make_pair<Handle, model::ConstructionBase>(construction.handle(), reversed)); 
    return reversed;
  }

  if (construction.optionalCast<model::ConstructionWithInternalSource>()){
    model::ConstructionWithInternalSource reversed = construction.cast<model::ConstructionWithInternalSource>().reverseConstructionWithInternalSource();
    m_constructionHandleToReversedConstructions.insert(std::make_pair<Handle, model::ConstructionBase>(construction.handle(), reversed)); 
    return reversed;
  }

  // DLM: this is not right, should make reverseConstruction a virtual method on LayeredConstruction so it will work for ConstructionWithInternalSource, etc
  LOG(Error, "Do not know how to reverse construction " << construction);

  model::MaterialVector layers = layeredConstruction.layers();
  std::reverse(layers.begin(), layers.end());

  model::Construction reversed = model::Construction(construction.model());
  reversed.setName(construction.name().get() + " Reversed");
  reversed.setLayers(layers);
  m_constructionHandleToReversedConstructions.insert(std::make_pair<Handle, model::ConstructionBase>(construction.handle(), reversed)); 

  return reversed;
}

void ForwardTranslator::resolveMatchedSurfaceConstructionConflicts(model::Model& model)
{
  // DLM: there is duplicate code in Surface_Impl::construction
  // if you update this code you must update that code

  std::set<Handle> processedSurfaces;

  model::SurfaceVector surfaces = model.getModelObjects<model::Surface>();
  BOOST_FOREACH(model::Surface surface, surfaces){

    if (processedSurfaces.find(surface.handle()) != processedSurfaces.end()){
      continue;
    }
    
    model::OptionalSurface adjacentSurface = surface.adjacentSurface();
    if (!adjacentSurface){
      processedSurfaces.insert(surface.handle());
      continue;
    }

    boost::optional<std::pair<model::ConstructionBase, int> > constructionWithSearchDistance = surface.constructionWithSearchDistance();
    boost::optional<std::pair<model::ConstructionBase, int> > adjacentConstructionWithSearchDistance = adjacentSurface->constructionWithSearchDistance();

    if (constructionWithSearchDistance && !adjacentConstructionWithSearchDistance){

      model::ConstructionBase c1 = constructionWithSearchDistance->first;
      model::ConstructionBase c2 = reverseConstruction(c1);

      LOG(Info, "Surface '" << surface.name() << "' has a construction and '" << adjacentSurface->name() 
             << "' does not, using '" << surface.name() << "'\'s construction." );

      surface.setConstruction(c1);
      adjacentSurface->setConstruction(c2);
      processedSurfaces.insert(surface.handle());
      processedSurfaces.insert(adjacentSurface->handle());
      continue;
    }

    if (!constructionWithSearchDistance && adjacentConstructionWithSearchDistance){

      model::ConstructionBase c2 = adjacentConstructionWithSearchDistance->first;
      model::ConstructionBase c1 = reverseConstruction(c2);

      LOG(Info, "Surface '" << adjacentSurface->name() << "' has a construction and '" << surface.name() 
             << "' does not, using '" << adjacentSurface->name() << "'\'s construction." );

      surface.setConstruction(c1);
      adjacentSurface->setConstruction(c2);
      processedSurfaces.insert(surface.handle());
      processedSurfaces.insert(adjacentSurface->handle());
      continue;
    }

    if (!constructionWithSearchDistance && !adjacentConstructionWithSearchDistance){
      // no constructions, nothing to be done
      LOG(Error, "No construction for either surface '" << surface.name()  
        << "', and '" << adjacentSurface->name() << "'");

      processedSurfaces.insert(surface.handle());
      processedSurfaces.insert(adjacentSurface->handle());
      continue;
    }

    // both surfaces return a construction

    if (constructionWithSearchDistance->first.handle() == adjacentConstructionWithSearchDistance->first.handle()){

      // both surfaces have same construction

      model::ConstructionBase c1 = constructionWithSearchDistance->first;
      model::ConstructionBase c2 = reverseConstruction(c1);

      if (c1.handle() != c2.handle()){
        LOG(Warn, "Both surfaces '" << surface.name() << "', and '" << adjacentSurface->name() 
               << "' reference the same construction '" << c1.name() << "' but it is not symmetric, creating a reversed copy." );

        surface.setConstruction(c1);
        adjacentSurface->setConstruction(c2);
      }

      processedSurfaces.insert(surface.handle());
      processedSurfaces.insert(adjacentSurface->handle());
      continue;
    }

    // both surfaces return a construction and they are not the same

    if (constructionWithSearchDistance->second < adjacentConstructionWithSearchDistance->second){
      // lower search distance to construction
      model::ConstructionBase c1 = constructionWithSearchDistance->first;
      model::ConstructionBase c2 = reverseConstruction(c1);

      LOG(Info, "Surfaces '" << surface.name() << "', and '" << adjacentSurface->name() 
             << "' reference different constructions, choosing '" << surface.name().get() << "'\'s construction based on search distance." );

      surface.setConstruction(c1);
      adjacentSurface->setConstruction(c2);
      processedSurfaces.insert(surface.handle());
      processedSurfaces.insert(adjacentSurface->handle());
      continue;
    }

    if (constructionWithSearchDistance->second > adjacentConstructionWithSearchDistance->second){
      // lower search distance to adjacent construction
      model::ConstructionBase c2 = adjacentConstructionWithSearchDistance->first;
      model::ConstructionBase c1 = reverseConstruction(c2);

      LOG(Info, "Surfaces '" << surface.name() << "', and '" << adjacentSurface->name() 
             << "' reference different constructions, choosing '" << adjacentSurface->name().get() << "'\'s construction based on search distance." );

      surface.setConstruction(c1);
      adjacentSurface->setConstruction(c2);
      processedSurfaces.insert(surface.handle());
      processedSurfaces.insert(adjacentSurface->handle());
      continue;
    }

    // both surfaces return a construction, they are not the same, and both have same search distance

    if (constructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>() && 
        adjacentConstructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>()){
      if (constructionWithSearchDistance->first.cast<model::LayeredConstruction>().reverseEqualLayers(adjacentConstructionWithSearchDistance->first.cast<model::LayeredConstruction>())){
        // these constructions are reverse equal
        surface.setConstruction(constructionWithSearchDistance->first);
        adjacentSurface->setConstruction(adjacentConstructionWithSearchDistance->first);
        processedSurfaces.insert(surface.handle());
        processedSurfaces.insert(adjacentSurface->handle());
        continue;
      }
    }

    // give up for now, we can add more later
    LOG(Error, "Could not resolve matched surface construction conflicts between surfaces '" << surface.name()  
            << "', and '" << adjacentSurface->name() << "'");
    surface.setConstruction(constructionWithSearchDistance->first);
    adjacentSurface->setConstruction(adjacentConstructionWithSearchDistance->first);
    processedSurfaces.insert(surface.handle());
    processedSurfaces.insert(adjacentSurface->handle());
  }
}

void ForwardTranslator::resolveMatchedSubSurfaceConstructionConflicts(model::Model& model)
{
  // DLM: there is duplicate code in SubSurface_Impl::construction
  // if you update this code you must update that code

  std::set<Handle> processedSubSurfaces;

  model::SubSurfaceVector subSurfaces = model.getModelObjects<model::SubSurface>();
  BOOST_FOREACH(model::SubSurface subSurface, subSurfaces){

    if (processedSubSurfaces.find(subSurface.handle()) != processedSubSurfaces.end()){
      continue;
    }
    
    model::OptionalSubSurface adjacentSubSurface = subSurface.adjacentSubSurface();
    if (!adjacentSubSurface){
      processedSubSurfaces.insert(subSurface.handle());
      continue;
    }

    boost::optional<std::pair<model::ConstructionBase, int> > constructionWithSearchDistance = subSurface.constructionWithSearchDistance();
    boost::optional<std::pair<model::ConstructionBase, int> > adjacentConstructionWithSearchDistance = adjacentSubSurface->constructionWithSearchDistance();

    if (constructionWithSearchDistance && !adjacentConstructionWithSearchDistance){

      model::ConstructionBase c1 = constructionWithSearchDistance->first;
      model::ConstructionBase c2 = reverseConstruction(c1);

      LOG(Info, "SubSurface '" << subSurface.name() << "' has a construction and '" << adjacentSubSurface->name() 
             << "' does not, using '" << subSurface.name() << "'\'s construction." );

      subSurface.setConstruction(c1);
      adjacentSubSurface->setConstruction(c2);
      processedSubSurfaces.insert(subSurface.handle());
      processedSubSurfaces.insert(adjacentSubSurface->handle());
      continue;
    }

    if (!constructionWithSearchDistance && adjacentConstructionWithSearchDistance){

      model::ConstructionBase c2 = adjacentConstructionWithSearchDistance->first;
      model::ConstructionBase c1 = reverseConstruction(c2);

      LOG(Info, "SubSurface '" << adjacentSubSurface->name() << "' has a construction and '" << subSurface.name() 
             << "' does not, using '" << adjacentSubSurface->name() << "'\'s construction." );

      subSurface.setConstruction(c1);
      adjacentSubSurface->setConstruction(c2);
      processedSubSurfaces.insert(subSurface.handle());
      processedSubSurfaces.insert(adjacentSubSurface->handle());
      continue;
    }

    if (!constructionWithSearchDistance && !adjacentConstructionWithSearchDistance){
      // no constructions, nothing to be done
      LOG(Error, "No construction for either sub surface '" << subSurface.name()  
        << "', and '" << adjacentSubSurface->name() << "'");

      processedSubSurfaces.insert(subSurface.handle());
      processedSubSurfaces.insert(adjacentSubSurface->handle());
      continue;
    }

    // both surfaces return a construction

    if (constructionWithSearchDistance->first.handle() == adjacentConstructionWithSearchDistance->first.handle()){

      // both surfaces have same construction

      model::ConstructionBase c1 = constructionWithSearchDistance->first;
      model::ConstructionBase c2 = reverseConstruction(c1);

      if (c1.handle() != c2.handle()){
        LOG(Warn, "Both sub surfaces '" << subSurface.name() << "', and '" << adjacentSubSurface->name() 
               << "' reference the same construction '" << c1.name() << "' but it is not symmetric, creating a reversed copy." );

        subSurface.setConstruction(c1);
        adjacentSubSurface->setConstruction(c2);
      }

      processedSubSurfaces.insert(subSurface.handle());
      processedSubSurfaces.insert(adjacentSubSurface->handle());
      continue;
    }

    // both surfaces return a construction and they are not the same

    if (constructionWithSearchDistance->second < adjacentConstructionWithSearchDistance->second){
      // lower search distance to construction
      model::ConstructionBase c1 = constructionWithSearchDistance->first;
      model::ConstructionBase c2 = reverseConstruction(c1);

      LOG(Info, "SubSurfaces '" << subSurface.name() << "', and '" << adjacentSubSurface->name() 
             << "' reference different constructions, choosing '" << subSurface.name().get() << "'\'s construction based on search distance." );

      subSurface.setConstruction(c1);
      adjacentSubSurface->setConstruction(c2);
      processedSubSurfaces.insert(subSurface.handle());
      processedSubSurfaces.insert(adjacentSubSurface->handle());
      continue;
    }

    if (constructionWithSearchDistance->second > adjacentConstructionWithSearchDistance->second){
      // lower search distance to adjacent construction
      model::ConstructionBase c2 = adjacentConstructionWithSearchDistance->first;
      model::ConstructionBase c1 = reverseConstruction(c2);

      LOG(Info, "SubSurfaces '" << subSurface.name() << "', and '" << adjacentSubSurface->name() 
             << "' reference different constructions, choosing '" << adjacentSubSurface->name().get() << "'\'s construction based on search distance." );

      subSurface.setConstruction(c1);
      adjacentSubSurface->setConstruction(c2);
      processedSubSurfaces.insert(subSurface.handle());
      processedSubSurfaces.insert(adjacentSubSurface->handle());
      continue;
    }

    // both surfaces return a construction, they are not the same, and both have same search distance

    if (constructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>() && 
        adjacentConstructionWithSearchDistance->first.optionalCast<model::LayeredConstruction>()){
      if (constructionWithSearchDistance->first.cast<model::LayeredConstruction>().reverseEqualLayers(adjacentConstructionWithSearchDistance->first.cast<model::LayeredConstruction>())){
        // these constructions are reverse equal
        subSurface.setConstruction(constructionWithSearchDistance->first);
        adjacentSubSurface->setConstruction(adjacentConstructionWithSearchDistance->first);
        processedSubSurfaces.insert(subSurface.handle());
        processedSubSurfaces.insert(adjacentSubSurface->handle());
        continue;
      }
    }

    // give up for now, we can add more later
    LOG(Error, "Could not resolve matched construction conflicts between sub surfaces '" << subSurface.name()  
            << "', and '" << adjacentSubSurface->name() << "'");
    subSurface.setConstruction(constructionWithSearchDistance->first);
    adjacentSubSurface->setConstruction(adjacentConstructionWithSearchDistance->first);
    processedSubSurfaces.insert(subSurface.handle());
    processedSubSurfaces.insert(adjacentSubSurface->handle());
  }
}

void ForwardTranslator::createStandardOutputRequests()
{
  IdfObject tableStyle(IddObjectType::OutputControl_Table_Style);
  m_idfObjects.push_back(tableStyle);
  tableStyle.setString(OutputControl_Table_StyleFields::ColumnSeparator,"HTML");

  IdfObject outputTableSummaryReport(IddObjectType::Output_Table_SummaryReports);
  IdfExtensibleGroup eg = outputTableSummaryReport.pushExtensibleGroup();
  eg.setString(Output_Table_SummaryReportsExtensibleFields::ReportName,"AllSummary");
  m_idfObjects.push_back(outputTableSummaryReport);

  IdfObject rddRequest(IddObjectType::Output_VariableDictionary);
  rddRequest.setString(Output_VariableDictionaryFields::KeyField, "IDF");
  rddRequest.setString(Output_VariableDictionaryFields::SortOption, "Unsorted");
  m_idfObjects.push_back(rddRequest);

  IdfObject sqliteOutput(IddObjectType::Output_SQLite);
  sqliteOutput.setString(Output_SQLiteFields::OptionType,"SimpleAndTabular");
  m_idfObjects.push_back(sqliteOutput);
}

IdfObject ForwardTranslator::createAndRegisterIdfObject(const IddObjectType& idfObjectType,
                                                        const model::ModelObject& modelObject)
{
  IdfObject idfObject(idfObjectType);
  m_idfObjects.push_back(idfObject);
  m_map.insert(std::make_pair(modelObject.handle(),idfObject));
  return idfObject;
}

IdfObject ForwardTranslator::createRegisterAndNameIdfObject(const IddObjectType& idfObjectType,
                                                            const model::ModelObject& modelObject)
{
  IdfObject idfObject = createAndRegisterIdfObject(idfObjectType,modelObject);
  if (OptionalString moName = modelObject.name()) {
    idfObject.setName(*moName);
  }
  return idfObject;
}

} // energyplus

} // openstudio

