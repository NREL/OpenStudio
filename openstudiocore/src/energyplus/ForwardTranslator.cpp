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
#include <model/UtilityBill.hpp>
#include <model/UtilityBill_Impl.hpp>
#include <model/ConcreteModelObjects.hpp>

#include <utilities/idf/Workspace.hpp>
#include <utilities/idf/IdfExtensibleGroup.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/WorkspaceObjectOrder.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/idd/FluidProperties_Name_FieldEnums.hxx>
#include <utilities/idd/FluidProperties_GlycolConcentration_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>
#include <utilities/idd/Output_Table_SummaryReports_FieldEnums.hxx>
#include <utilities/idd/OutputControl_Table_Style_FieldEnums.hxx>
#include <utilities/idd/Output_VariableDictionary_FieldEnums.hxx>
#include <utilities/idd/Output_SQLite_FieldEnums.hxx>
#include <utilities/idd/ProgramControl_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/plot/ProgressBar.hpp>

#include <QFile>
#include <QTextStream>
#include <QThread>

#include <sstream>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

ForwardTranslator::ForwardTranslator()
{
  m_logSink.setLogLevel(Warn);
  m_logSink.setChannelRegex(boost::regex("openstudio\\.energyplus\\.ForwardTranslator"));
  m_logSink.setThreadId(QThread::currentThread());
  createFluidPropertiesMap();

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
    boost::optional<model::Building> building = model.building();
    if (!building){
      building = model.getUniqueModelObject<model::Building>();
    }
    translateAndMapModelObject(*building);

    // ensure that simulation control exists
    boost::optional<model::SimulationControl> simulationControl = model.getOptionalUniqueModelObject<model::SimulationControl>();
    if (!simulationControl){
      simulationControl = model.getUniqueModelObject<model::SimulationControl>();
    }
    translateAndMapModelObject(*simulationControl);

    // Add a ProgramControl object to force a single threaded simulation
    IdfObject programControl(openstudio::IddObjectType::ProgramControl);
    programControl.setInt(openstudio::ProgramControlFields::NumberofThreadsAllowed,1);
    m_idfObjects.push_back(programControl);

    // ensure that sizing parameters control exists
    boost::optional<model::SizingParameters> sizingParameters = model.getOptionalUniqueModelObject<model::SizingParameters>();
    if (!sizingParameters){
      sizingParameters = model.getUniqueModelObject<model::SizingParameters>();
    }
    translateAndMapModelObject(*sizingParameters);

    // ensure that run period exists
    // DLM: should this only be done if there is a WeatherFile object?
    boost::optional<model::RunPeriod> runPeriod = model.runPeriod();
    if (!runPeriod){
      runPeriod = model.getUniqueModelObject<model::RunPeriod>();
    }
    translateAndMapModelObject(*runPeriod);

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
    translateAndMapModelObject(*lifeCycleCostParameters);
  
    // create meters for utility bill objects
    std::vector<UtilityBill> utilityBills = model.getModelObjects<UtilityBill>();
    BOOST_FOREACH(UtilityBill utilityBill, utilityBills){
      // these meters and variables will be translated later
      Meter consumptionMeter = utilityBill.consumptionMeter();
      boost::optional<Meter> peakDemandMeter = utilityBill.peakDemandMeter();
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

  // get AirConditionerVariableRefrigerantFlow objects in sorted order
  std::vector<AirConditionerVariableRefrigerantFlow> vrfs = model.getModelObjects<AirConditionerVariableRefrigerantFlow>();
  std::sort(vrfs.begin(), vrfs.end(), WorkspaceObjectNameLess());
  BOOST_FOREACH(AirConditionerVariableRefrigerantFlow vrf, vrfs){
    translateAndMapModelObject(vrf);
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

  if( modelObject.iddObject().type() == AirConditionerVariableRefrigerantFlow::iddObjectType() )
  {
       model::AirConditionerVariableRefrigerantFlow vrf = modelObject.cast<AirConditionerVariableRefrigerantFlow>();
       retVal = translateAirConditionerVariableRefrigerantFlow(vrf);
  }
  else if( modelObject.iddObject().type() == AirLoopHVAC::iddObjectType() )
  {
       model::AirLoopHVAC airLoopHVAC = modelObject.cast<AirLoopHVAC>();
       retVal = translateAirLoopHVAC(airLoopHVAC);
  }
  else if( modelObject.iddObject().type() == AirTerminalSingleDuctConstantVolumeReheat::iddObjectType() )
  {
       model::AirTerminalSingleDuctConstantVolumeReheat airTerminal = modelObject.cast<AirTerminalSingleDuctConstantVolumeReheat>();
       retVal = translateAirTerminalSingleDuctConstantVolumeReheat(airTerminal);
  }
  else if( modelObject.iddObject().type() ==  AirTerminalSingleDuctConstantVolumeCooledBeam::iddObjectType() )
  {
       model::AirTerminalSingleDuctConstantVolumeCooledBeam airTerminal = modelObject.cast<AirTerminalSingleDuctConstantVolumeCooledBeam>();
       retVal = translateAirTerminalSingleDuctConstantVolumeCooledBeam(airTerminal);
  }
  else if( modelObject.iddObject().type() ==  AirTerminalSingleDuctParallelPIUReheat::iddObjectType() )
  {
       model::AirTerminalSingleDuctParallelPIUReheat airTerminal = modelObject.cast<AirTerminalSingleDuctParallelPIUReheat>();
       retVal = translateAirTerminalSingleDuctParallelPIUReheat(airTerminal);
  }
  else if( modelObject.iddObject().type() ==  AirTerminalSingleDuctUncontrolled::iddObjectType() )
  {
       model::AirTerminalSingleDuctUncontrolled airTerminal = modelObject.cast<AirTerminalSingleDuctUncontrolled>();
       retVal = translateAirTerminalSingleDuctUncontrolled(airTerminal);
  }
  else if( modelObject.iddObject().type() ==  AirTerminalSingleDuctVAVNoReheat::iddObjectType() )
  {
       model::AirTerminalSingleDuctVAVNoReheat airTerminal = modelObject.cast<AirTerminalSingleDuctVAVNoReheat>();
       retVal = translateAirTerminalSingleDuctVAVNoReheat(airTerminal);
  }
  else if( modelObject.iddObject().type() ==  AirTerminalSingleDuctVAVReheat::iddObjectType() )
  {
       model::AirTerminalSingleDuctVAVReheat airTerminal = modelObject.cast<AirTerminalSingleDuctVAVReheat>();
       retVal = translateAirTerminalSingleDuctVAVReheat(airTerminal);
  }
  else if( modelObject.iddObject().type() ==  AirLoopHVACZoneSplitter::iddObjectType() )
  {
       model::AirLoopHVACZoneSplitter splitter = modelObject.cast<AirLoopHVACZoneSplitter>();
       retVal = translateAirLoopHVACZoneSplitter(splitter);
  }
  else if( modelObject.iddObject().type() ==  AirLoopHVACZoneMixer::iddObjectType() )
  {
       model::AirLoopHVACZoneMixer mixer = modelObject.cast<AirLoopHVACZoneMixer>();
       retVal = translateAirLoopHVACZoneMixer(mixer);
  }
  else if( modelObject.iddObject().type() ==  AirLoopHVACOutdoorAirSystem::iddObjectType() )
  {
       model::AirLoopHVACOutdoorAirSystem oaSystem = modelObject.cast<AirLoopHVACOutdoorAirSystem>();
       retVal = translateAirLoopHVACOutdoorAirSystem(oaSystem);
  }
  else if( modelObject.iddObject().type() ==  AirLoopHVACUnitaryHeatPumpAirToAir::iddObjectType() )
  {
       model::AirLoopHVACUnitaryHeatPumpAirToAir unitary = modelObject.cast<AirLoopHVACUnitaryHeatPumpAirToAir>();
       retVal = translateAirLoopHVACUnitaryHeatPumpAirToAir(unitary);
  }
  else if( modelObject.iddObject().type() ==  AvailabilityManagerAssignmentList::iddObjectType() )
  {
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  AvailabilityManagerScheduled::iddObjectType() )
  {
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  AirWallMaterial::iddObjectType() )
  {
       model::AirWallMaterial airWallMaterial = modelObject.cast<AirWallMaterial>();
       retVal = translateAirWallMaterial(airWallMaterial);
  }
  else if( modelObject.iddObject().type() ==  Blind::iddObjectType() )
  {
       model::Blind blind = modelObject.cast<Blind>();
       retVal = translateBlind(blind);
  }
  else if( modelObject.iddObject().type() ==  BoilerHotWater::iddObjectType() )
  {
       model::BoilerHotWater boiler = modelObject.cast<BoilerHotWater>();
       retVal = translateBoilerHotWater(boiler);
  }
  else if( modelObject.iddObject().type() ==  BoilerSteam::iddObjectType() )
  {
       model::BoilerSteam boiler = modelObject.cast<BoilerSteam>();
       retVal = translateBoilerSteam(boiler);
  }
  else if( modelObject.iddObject().type() ==  Building::iddObjectType() )
  {
       model::Building building = modelObject.cast<Building>();
       retVal = translateBuilding(building);
  }
  else if( modelObject.iddObject().type() ==  BuildingStory::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  ChillerElectricEIR::iddObjectType() )
  {
       model::ChillerElectricEIR chiller = modelObject.cast<ChillerElectricEIR>();
       retVal = translateChillerElectricEIR(chiller);
  }
  else if( modelObject.iddObject().type() ==  CFactorUndergroundWallConstruction::iddObjectType() )
  {
       model::CFactorUndergroundWallConstruction construction = modelObject.cast<CFactorUndergroundWallConstruction>();
       retVal = translateCFactorUndergroundWallConstruction(construction);
  }
  else if( modelObject.iddObject().type() ==  ConvergenceLimits::iddObjectType() )
  {
       model::ConvergenceLimits limits = modelObject.cast<ConvergenceLimits>();
       retVal = translateConvergenceLimits(limits);
  }
  else if( modelObject.iddObject().type() ==  CoilCoolingDXSingleSpeed::iddObjectType() )
  {
       model::CoilCoolingDXSingleSpeed coil = modelObject.cast<CoilCoolingDXSingleSpeed>();
       retVal = translateCoilCoolingDXSingleSpeed(coil);
  }
  else if( modelObject.iddObject().type() ==  CoilCoolingDXTwoSpeed::iddObjectType() )
  {
       model::CoilCoolingDXTwoSpeed coil = modelObject.cast<CoilCoolingDXTwoSpeed>();
       retVal = translateCoilCoolingDXTwoSpeed(coil);
  }
  else if ( modelObject.iddObject().type() ==  CoilCoolingDXVariableRefrigerantFlow::iddObjectType() )
  {
       model::CoilCoolingDXVariableRefrigerantFlow coil = modelObject.cast<CoilCoolingDXVariableRefrigerantFlow>();
       retVal = translateCoilCoolingDXVariableRefrigerantFlow(coil);
  }
  else if( modelObject.iddObject().type() ==  CoilCoolingLowTempRadiantConstFlow::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  CoilCoolingLowTempRadiantVarFlow::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  CoilCoolingWater::iddObjectType() )
  {
       model::CoilCoolingWater coil = modelObject.cast<CoilCoolingWater>();
       retVal = translateCoilCoolingWater(coil);
  }
  else if( modelObject.iddObject().type() ==  CoilCoolingWaterToAirHeatPumpEquationFit::iddObjectType() )
  {
       model::CoilCoolingWaterToAirHeatPumpEquationFit coil = modelObject.cast<CoilCoolingWaterToAirHeatPumpEquationFit>();
       retVal = translateCoilCoolingWaterToAirHeatPumpEquationFit(coil);
  }
  else if( modelObject.iddObject().type() ==  CoilHeatingDXSingleSpeed::iddObjectType() )
  {
       model::CoilHeatingDXSingleSpeed coil = modelObject.cast<CoilHeatingDXSingleSpeed>();
       retVal = translateCoilHeatingDXSingleSpeed(coil);
  }
  else if( modelObject.iddObject().type() ==  CoilHeatingElectric::iddObjectType() )
  {
       model::CoilHeatingElectric coil = modelObject.cast<CoilHeatingElectric>();
       retVal = translateCoilHeatingElectric(coil);
  }
  else if( modelObject.iddObject().type() ==  CoilHeatingGas::iddObjectType() )
  {
       model::CoilHeatingGas coil = modelObject.cast<CoilHeatingGas>();
       retVal = translateCoilHeatingGas(coil);
  }
  else if( modelObject.iddObject().type() ==  CoilHeatingLowTempRadiantConstFlow::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  CoilHeatingLowTempRadiantVarFlow::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  CoilHeatingDXVariableRefrigerantFlow::iddObjectType() )
  {
       model::CoilHeatingDXVariableRefrigerantFlow coil = modelObject.cast<CoilHeatingDXVariableRefrigerantFlow>();
       retVal = translateCoilHeatingDXVariableRefrigerantFlow(coil);
  }
  else if( modelObject.iddObject().type() ==  CoilHeatingWater::iddObjectType() )
  {
       model::CoilHeatingWater coil = modelObject.cast<CoilHeatingWater>();
       retVal = translateCoilHeatingWater(coil);
  }
  else if( modelObject.iddObject().type() ==  CoilHeatingWaterToAirHeatPumpEquationFit::iddObjectType() )
  {
       model::CoilHeatingWaterToAirHeatPumpEquationFit coil = modelObject.cast<CoilHeatingWaterToAirHeatPumpEquationFit>();
       retVal = translateCoilHeatingWaterToAirHeatPumpEquationFit(coil);
  }
  else if( modelObject.iddObject().type() ==  ComponentData::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  ComponentCostAdjustments::iddObjectType() )
  {
       LOG(Warn, "OS:ComponentCost:Adjustments '" << modelObject.name().get() << "' not translated to EnergyPlus.");
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  Connection::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  ConnectorMixer::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  ConnectorSplitter::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  Construction::iddObjectType() )
  {
       model::Construction construction = modelObject.cast<Construction>();
       retVal = translateConstruction(construction);
  }
  else if( modelObject.iddObject().type() ==  ConstructionWithInternalSource::iddObjectType() )
  {
       model::ConstructionWithInternalSource constructionIntSource = modelObject.cast<ConstructionWithInternalSource>();
       retVal = translateConstructionWithInternalSource(constructionIntSource);
  }
  else if( modelObject.iddObject().type() ==  FFactorGroundFloorConstruction::iddObjectType() )
  {
       model::FFactorGroundFloorConstruction mo = modelObject.cast<FFactorGroundFloorConstruction>();
       retVal = translateFFactorGroundFloorConstruction(mo);
  }
  else if( modelObject.iddObject().type() ==  ControllerMechanicalVentilation::iddObjectType() )
  {
       model::ControllerMechanicalVentilation controller = modelObject.cast<ControllerMechanicalVentilation>();
       retVal = translateControllerMechanicalVentilation(controller);
  }
  else if( modelObject.iddObject().type() ==  ControllerOutdoorAir::iddObjectType() )
  {
       model::ControllerOutdoorAir controller = modelObject.cast<ControllerOutdoorAir>();
       retVal = translateControllerOutdoorAir(controller);
  }
  else if( modelObject.iddObject().type() ==  ControllerWaterCoil::iddObjectType() )
  {
       model::ControllerWaterCoil controller = modelObject.cast<ControllerWaterCoil>();
       retVal = translateControllerWaterCoil(controller);
  }
  else if( modelObject.iddObject().type() ==  CoolingTowerPerformanceCoolTools::iddObjectType() )
  {
       model::CoolingTowerPerformanceCoolTools mo = modelObject.cast<CoolingTowerPerformanceCoolTools>();
       retVal = translateCoolingTowerPerformanceCoolTools(mo);
  }
  else if( modelObject.iddObject().type() ==  CoolingTowerPerformanceYorkCalc::iddObjectType() )
  {
       model::CoolingTowerPerformanceYorkCalc mo = modelObject.cast<CoolingTowerPerformanceYorkCalc>();
       retVal = translateCoolingTowerPerformanceYorkCalc(mo);
  }
  else if( modelObject.iddObject().type() ==  CoolingTowerSingleSpeed::iddObjectType() )
  {
       model::CoolingTowerSingleSpeed tower = modelObject.cast<CoolingTowerSingleSpeed>();
       retVal = translateCoolingTowerSingleSpeed(tower);
  }
  else if( modelObject.iddObject().type() ==  CoolingTowerVariableSpeed::iddObjectType() )
  {
       model::CoolingTowerVariableSpeed tower = modelObject.cast<CoolingTowerVariableSpeed>();
       retVal = translateCoolingTowerVariableSpeed(tower);
  }
  else if( modelObject.iddObject().type() ==  CurrencyType::iddObjectType() )
  {
       model::CurrencyType mo = modelObject.cast<CurrencyType>();
       retVal = translateCurrencyType(mo);
  }
  else if( modelObject.iddObject().type() ==  CurveBicubic::iddObjectType() )
  {
       model::CurveBicubic curve = modelObject.cast<CurveBicubic>();
       retVal = translateCurveBicubic(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveBiquadratic::iddObjectType() )
  {
       model::CurveBiquadratic curve = modelObject.cast<CurveBiquadratic>();
       retVal = translateCurveBiquadratic(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveCubic::iddObjectType() )
  {
       model::CurveCubic curve = modelObject.cast<CurveCubic>();
       retVal = translateCurveCubic(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveDoubleExponentialDecay::iddObjectType() )
  {
       model::CurveDoubleExponentialDecay curve = modelObject.cast<CurveDoubleExponentialDecay>();
       retVal = translateCurveDoubleExponentialDecay(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveExponent::iddObjectType() )
  {
       model::CurveExponent curve = modelObject.cast<CurveExponent>();
       retVal = translateCurveExponent(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveExponentialDecay::iddObjectType() )
  {
       model::CurveExponentialDecay curve = modelObject.cast<CurveExponentialDecay>();
       retVal = translateCurveExponentialDecay(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveExponentialSkewNormal::iddObjectType() )
  {
       model::CurveExponentialSkewNormal curve = modelObject.cast<CurveExponentialSkewNormal>();
       retVal = translateCurveExponentialSkewNormal(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveFanPressureRise::iddObjectType() )
  {
       model::CurveFanPressureRise curve = modelObject.cast<CurveFanPressureRise>();
       retVal = translateCurveFanPressureRise(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveFunctionalPressureDrop::iddObjectType() )
  {
       model::CurveFunctionalPressureDrop curve = modelObject.cast<CurveFunctionalPressureDrop>();
       retVal = translateCurveFunctionalPressureDrop(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveLinear::iddObjectType() )
  {
       model::CurveLinear curve = modelObject.cast<CurveLinear>();
       retVal = translateCurveLinear(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveQuadratic::iddObjectType() )
  {
       model::CurveQuadratic curve = modelObject.cast<CurveQuadratic>();
       retVal = translateCurveQuadratic(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveQuadraticLinear::iddObjectType() )
  {
       model::CurveQuadraticLinear curve = modelObject.cast<CurveQuadraticLinear>();
       retVal = translateCurveQuadraticLinear(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveQuartic::iddObjectType() )
  {
       model::CurveQuartic curve = modelObject.cast<CurveQuartic>();
       retVal = translateCurveQuartic(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveRectangularHyperbola1::iddObjectType() )
  {
       model::CurveRectangularHyperbola1 curve = modelObject.cast<CurveRectangularHyperbola1>();
       retVal = translateCurveRectangularHyperbola1(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveRectangularHyperbola2::iddObjectType() )
  {
       model::CurveRectangularHyperbola2 curve = modelObject.cast<CurveRectangularHyperbola2>();
       retVal = translateCurveRectangularHyperbola2(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveSigmoid::iddObjectType() )
  {
       model::CurveSigmoid curve = modelObject.cast<CurveSigmoid>();
       retVal = translateCurveSigmoid(curve);
  }
  else if( modelObject.iddObject().type() ==  CurveTriquadratic::iddObjectType() )
  {
       model::CurveTriquadratic curve = modelObject.cast<CurveTriquadratic>();
       retVal = translateCurveTriquadratic(curve);
  }
  else if( modelObject.iddObject().type() ==  DesignDay::iddObjectType() )
  {
       model::DesignDay designDay = modelObject.cast<DesignDay>();
       retVal = translateDesignDay(designDay);
  }
  else if( modelObject.iddObject().type() ==  SizingPlant::iddObjectType() )
  {
       model::SizingPlant sizingPlant = modelObject.cast<SizingPlant>();
       retVal = translateSizingPlant(sizingPlant);
  }
  else if( modelObject.iddObject().type() ==  SizingSystem::iddObjectType() )
  {
       model::SizingSystem mo = modelObject.cast<SizingSystem>();
       retVal = translateSizingSystem(mo);
  }
  else if( modelObject.iddObject().type() ==  SizingZone::iddObjectType() )
  {
       model::SizingZone mo = modelObject.cast<SizingZone>();
       retVal = translateSizingZone(mo);
  }
  else if( modelObject.iddObject().type() ==  DaylightingControl::iddObjectType() )
  {
       model::DaylightingControl daylightingControl = modelObject.cast<DaylightingControl>();
       retVal = translateDaylightingControl(daylightingControl);
  }

  else if( modelObject.iddObject().type() ==  DaylightingDeviceShelf::iddObjectType() )
  {
       model::DaylightingDeviceShelf daylightingDeviceShelf = modelObject.cast<DaylightingDeviceShelf>();
       retVal = translateDaylightingDeviceShelf(daylightingDeviceShelf);
  }
  else if( modelObject.iddObject().type() ==  DefaultConstructionSet::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  DefaultScheduleSet::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  DefaultSurfaceConstructions::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  DefaultSubSurfaceConstructions::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  DesignSpecificationOutdoorAir::iddObjectType() )
  {
       model::DesignSpecificationOutdoorAir designSpecificationOutdoorAir = modelObject.cast<DesignSpecificationOutdoorAir>();
       retVal = translateDesignSpecificationOutdoorAir(designSpecificationOutdoorAir);
  }
  else if( modelObject.iddObject().type() ==  DistrictCooling::iddObjectType() )
  {
       model::DistrictCooling districtCooling = modelObject.cast<DistrictCooling>();
       retVal = translateDistrictCooling(districtCooling);
  }
  else if( modelObject.iddObject().type() ==  DistrictHeating::iddObjectType() )
  {
       model::DistrictHeating districtHeating = modelObject.cast<DistrictHeating>();
       retVal = translateDistrictHeating(districtHeating);
  }
  else if( modelObject.iddObject().type() ==  ElectricEquipment::iddObjectType() )
  {
       model::ElectricEquipment equipment = modelObject.cast<ElectricEquipment>();
       retVal = translateElectricEquipment(equipment);
  }     
  else if( modelObject.iddObject().type() ==  EvaporativeCoolerDirectResearchSpecial::iddObjectType() )
  {
       model::EvaporativeCoolerDirectResearchSpecial evap = modelObject.cast<EvaporativeCoolerDirectResearchSpecial>();
       retVal = translateEvaporativeCoolerDirectResearchSpecial(evap);
  }
  else if( modelObject.iddObject().type() ==  EvaporativeFluidCoolerSingleSpeed::iddObjectType() )
  {
       model::EvaporativeFluidCoolerSingleSpeed evap = modelObject.cast<EvaporativeFluidCoolerSingleSpeed>();
       retVal = translateEvaporativeFluidCoolerSingleSpeed(evap);
  }
  else if( modelObject.iddObject().type() ==  ExteriorLights::iddObjectType() )
  {
       model::ExteriorLights lights = modelObject.cast<ExteriorLights>();
       retVal = translateExteriorLights(lights);
  }
  else if( modelObject.iddObject().type() ==  Facility::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  FanConstantVolume::iddObjectType() )
  {
       model::FanConstantVolume fan = modelObject.cast<FanConstantVolume>();
       retVal = translateFanConstantVolume(fan);
  }
  else if( modelObject.iddObject().type() ==  FanOnOff::iddObjectType() )
  {
       model::FanOnOff fan = modelObject.cast<FanOnOff>();
       retVal = translateFanOnOff(fan);
  }
  else if( modelObject.iddObject().type() ==  FanVariableVolume::iddObjectType() )
  {
       model::FanVariableVolume fan = modelObject.cast<FanVariableVolume>();
       retVal = translateFanVariableVolume(fan);
  }
  else if( modelObject.iddObject().type() ==  GroundHeatExchangerVertical::iddObjectType() )
  {
       model::GroundHeatExchangerVertical groundHeatExchangerVertical = modelObject.cast<GroundHeatExchangerVertical>();
       retVal = translateGroundHeatExchangerVertical(groundHeatExchangerVertical);
  }
  else if( modelObject.iddObject().type() ==  HeatBalanceAlgorithm::iddObjectType() )
  {
       model::HeatBalanceAlgorithm mo = modelObject.cast<HeatBalanceAlgorithm>();
       retVal = translateHeatBalanceAlgorithm(mo);
  }
  else if( modelObject.iddObject().type() ==  HeatExchangerAirToAirSensibleAndLatent::iddObjectType() )
  {
       model::HeatExchangerAirToAirSensibleAndLatent mo = modelObject.cast<HeatExchangerAirToAirSensibleAndLatent>();
       retVal = translateHeatExchangerAirToAirSensibleAndLatent(mo);
  }
  else if( modelObject.iddObject().type() ==  HotWaterEquipment::iddObjectType() )
  {
       model::HotWaterEquipment hotWaterEquipment = modelObject.cast<HotWaterEquipment>();
       retVal = translateHotWaterEquipment(hotWaterEquipment);
  }
  else if( modelObject.iddObject().type() ==  Gas::iddObjectType() )
  {
       model::Gas gas = modelObject.cast<Gas>();
       retVal = translateGas(gas);
  }
  else if( modelObject.iddObject().type() ==  GasEquipment::iddObjectType() )
  {
       model::GasEquipment equipment = modelObject.cast<GasEquipment>();
       retVal = translateGasEquipment(equipment);
  }
  else if( modelObject.iddObject().type() ==  IlluminanceMap::iddObjectType() )
  {
       model::IlluminanceMap illuminanceMap = modelObject.cast<IlluminanceMap>();
       retVal = translateIlluminanceMap(illuminanceMap);
  }
  else if( modelObject.iddObject().type() ==  InteriorPartitionSurface::iddObjectType() )
  {
       model::InteriorPartitionSurface interiorPartitionSurface = modelObject.cast<InteriorPartitionSurface>();
       retVal = translateInteriorPartitionSurface(interiorPartitionSurface);
  }
  else if( modelObject.iddObject().type() ==  InteriorPartitionSurfaceGroup::iddObjectType() )
  {
       model::InteriorPartitionSurfaceGroup interiorPartitionSurfaceGroup = modelObject.cast<InteriorPartitionSurfaceGroup>();
       retVal = translateInteriorPartitionSurfaceGroup(interiorPartitionSurfaceGroup);
  }
  else if( modelObject.iddObject().type() ==  InternalMass::iddObjectType() )
  {
       model::InternalMass internalMass = modelObject.cast<InternalMass>();
       retVal = translateInternalMass(internalMass);
  }
  else if( modelObject.iddObject().type() ==  WaterHeaterMixed::iddObjectType() )
  {
       model::WaterHeaterMixed waterHeaterMixed = modelObject.cast<WaterHeaterMixed>();
       retVal = translateWaterHeaterMixed(waterHeaterMixed);
  }
  else if( modelObject.iddObject().type() ==  WaterUseConnections::iddObjectType() )
  {
       model::WaterUseConnections waterUseConnections = modelObject.cast<WaterUseConnections>();
       retVal = translateWaterUseConnections(waterUseConnections);
  }
  else if( modelObject.iddObject().type() ==  WaterUseEquipment::iddObjectType() )
  {
       model::WaterUseEquipment waterUseEquipment = modelObject.cast<WaterUseEquipment>();
       retVal = translateWaterUseEquipment(waterUseEquipment);
  }
  else if( modelObject.iddObject().type() ==  GasMixture::iddObjectType() )
  {
       model::GasMixture gasMixture = modelObject.cast<GasMixture>();
       retVal = translateGasMixture(gasMixture);
  }
  else if( modelObject.iddObject().type() ==  LifeCycleCost::iddObjectType() )
  {
       model::LifeCycleCost lifeCycleCost = modelObject.cast<LifeCycleCost>();
       retVal = translateLifeCycleCost(lifeCycleCost);
  }
  else if( modelObject.iddObject().type() ==  LifeCycleCostParameters::iddObjectType() )
  {
       model::LifeCycleCostParameters lifeCycleCostParameters = modelObject.cast<LifeCycleCostParameters>();
       retVal = translateLifeCycleCostParameters(lifeCycleCostParameters);
  }
  else if( modelObject.iddObject().type() ==  LifeCycleCostUsePriceEscalation::iddObjectType() )
  {
       // DLM: these objects can be created from LifeCycleCostParameters
       LOG(Warn, "OS:LifeCycleCost:UsePriceEscalation '" << modelObject.name().get() << "' not translated to EnergyPlus.");
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  Lights::iddObjectType() )
  {
       model::Lights lights = modelObject.cast<Lights>();
       retVal = translateLights(lights);
  }
  else if( modelObject.iddObject().type() ==  Luminaire::iddObjectType() )
  {
       model::Luminaire luminaire = modelObject.cast<Luminaire>();
       retVal = translateLuminaire(luminaire);
  }
  else if( modelObject.iddObject().type() ==  StandardOpaqueMaterial::iddObjectType() )
  {
       model::StandardOpaqueMaterial material = modelObject.cast<StandardOpaqueMaterial>();
       retVal = translateStandardOpaqueMaterial(material);
  }
  else if( modelObject.iddObject().type() ==  AirGap::iddObjectType() )
  {
       model::AirGap material = modelObject.cast<AirGap>();
       retVal = translateAirGap(material);
  }
  else if( modelObject.iddObject().type() ==  InfraredTransparentMaterial::iddObjectType() )
  {
       model::InfraredTransparentMaterial material = modelObject.cast<InfraredTransparentMaterial>();
       retVal = translateInfraredTransparentMaterial(material);
  }
  else if( modelObject.iddObject().type() ==  MasslessOpaqueMaterial::iddObjectType() )
  {
       model::MasslessOpaqueMaterial material = modelObject.cast<MasslessOpaqueMaterial>();
       retVal = translateMasslessOpaqueMaterial(material);
  }
  else if( modelObject.iddObject().type() ==  RoofVegetation::iddObjectType() )
  {
       model::RoofVegetation material = modelObject.cast<RoofVegetation>();
       retVal = translateRoofVegetation(material);
  }
  else if( modelObject.iddObject().type() ==  Meter::iddObjectType() )
  {
       model::Meter meter = modelObject.cast<Meter>();
       retVal = translateMeter(meter);
  }
  else if( modelObject.iddObject().type() ==  Node::iddObjectType() )
  {
       model::Node node = modelObject.cast<Node>();
       retVal = translateNode(node);
  }
  else if( modelObject.iddObject().type() ==  OtherEquipment::iddObjectType() )
  {
       model::OtherEquipment otherEquipment = modelObject.cast<OtherEquipment>();
       retVal = translateOtherEquipment(otherEquipment);
  }
  else if( modelObject.iddObject().type() ==  PlantLoop::iddObjectType() )
  {
       PlantLoop plantLoop = modelObject.cast<PlantLoop>();
       retVal = translatePlantLoop(plantLoop);
  }
  else if( modelObject.iddObject().type() ==  PumpConstantSpeed::iddObjectType() )
  {
       PumpConstantSpeed pump = modelObject.cast<PumpConstantSpeed>();
       retVal = translatePumpConstantSpeed(pump);
  }
  else if( modelObject.iddObject().type() ==  PumpVariableSpeed::iddObjectType() )
  {
       PumpVariableSpeed pump = modelObject.cast<PumpVariableSpeed>();
       retVal = translatePumpVariableSpeed(pump);
  }
  else if( modelObject.iddObject().type() ==  OutputControlReportingTolerances::iddObjectType() )
  {
       model::OutputControlReportingTolerances outputControl = modelObject.cast<OutputControlReportingTolerances>();
       retVal = translateOutputControlReportingTolerances(outputControl);
  }
  else if( modelObject.iddObject().type() ==  OutputVariable::iddObjectType() )
  {
       model::OutputVariable outputVariable = modelObject.cast<OutputVariable>();
       retVal = translateOutputVariable(outputVariable);
  }
  else if( modelObject.iddObject().type() ==  People::iddObjectType() )
  {
       model::People people = modelObject.cast<People>();
       retVal = translatePeople(people);
  }
  else if( modelObject.iddObject().type() ==  PipeAdiabatic::iddObjectType() )
  {
       model::PipeAdiabatic pipe = modelObject.cast<PipeAdiabatic>();
       retVal = translatePipeAdiabatic(pipe);
  }
  else if( modelObject.iddObject().type() ==  PortList::iddObjectType() )
  {
       model::PortList portList = modelObject.cast<PortList>();
       retVal = translatePortList(portList);
  }
  else if ( modelObject.iddObject().type() ==  RefrigerationCase::iddObjectType() )
  {
       model::RefrigerationCase refrigerationCase = modelObject.cast<RefrigerationCase>();
       retVal = translateRefrigerationCase(refrigerationCase);
  }
  else if ( modelObject.iddObject().type() ==  RefrigerationCompressor::iddObjectType() )
  {
       model::RefrigerationCompressor refrigerationCompressor = modelObject.cast<RefrigerationCompressor>();
       retVal = translateRefrigerationCompressor(refrigerationCompressor);
  }
  else if ( modelObject.iddObject().type() ==  RefrigerationCondenserAirCooled::iddObjectType() )
  {
       model::RefrigerationCondenserAirCooled refrigerationCondenserAirCooled = modelObject.cast<RefrigerationCondenserAirCooled>();
       retVal = translateRefrigerationCondenserAirCooled(refrigerationCondenserAirCooled);
  }
  else if ( modelObject.iddObject().type() ==  RefrigerationCondenserCascade::iddObjectType() )
  {
       model::RefrigerationCondenserCascade refrigerationCondenserCascade = modelObject.cast<RefrigerationCondenserCascade>();
       retVal = translateRefrigerationCondenserCascade(refrigerationCondenserCascade);
  }
  else if ( modelObject.iddObject().type() ==  RefrigerationCondenserEvaporativeCooled::iddObjectType() )
  {
       model::RefrigerationCondenserEvaporativeCooled refrigerationCondenserEvaporativeCooled = modelObject.cast<RefrigerationCondenserEvaporativeCooled>();
       retVal = translateRefrigerationCondenserEvaporativeCooled(refrigerationCondenserEvaporativeCooled);
  }
  else if ( modelObject.iddObject().type() ==  RefrigerationCondenserWaterCooled::iddObjectType() )
  {
       model::RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = modelObject.cast<RefrigerationCondenserWaterCooled>();
       retVal = translateRefrigerationCondenserWaterCooled(refrigerationCondenserWaterCooled);
  }
  else if ( modelObject.iddObject().type() ==  RefrigerationSubcoolerLiquidSuction::iddObjectType() )
  {
       model::RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = modelObject.cast<RefrigerationSubcoolerLiquidSuction>();
       retVal = translateRefrigerationSubcoolerLiquidSuction(refrigerationSubcoolerLiquidSuction);
  }
  else if ( modelObject.iddObject().type() ==  RefrigerationSubcoolerMechanical::iddObjectType() )
  {
       model::RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = modelObject.cast<RefrigerationSubcoolerMechanical>();
       retVal = translateRefrigerationSubcoolerMechanical(refrigerationSubcoolerMechanical);
  }
  else if ( modelObject.iddObject().type() ==  RefrigerationSecondarySystem::iddObjectType() )
  {
       model::RefrigerationSecondarySystem refrigerationSecondarySystem = modelObject.cast<RefrigerationSecondarySystem>();
       retVal = translateRefrigerationSecondarySystem(refrigerationSecondarySystem);
  }
  else if ( modelObject.iddObject().type() ==  RefrigerationSystem::iddObjectType() )
  {
       model::RefrigerationSystem refrigerationSystem = modelObject.cast<RefrigerationSystem>();
       retVal = translateRefrigerationSystem(refrigerationSystem);
  }
  else if ( modelObject.iddObject().type() ==  RefrigerationWalkIn::iddObjectType() )
  {
       model::RefrigerationWalkIn refrigerationWalkIn = modelObject.cast<RefrigerationWalkIn>();
       retVal = translateRefrigerationWalkIn(refrigerationWalkIn);
  }
  else if( modelObject.iddObject().type() ==  RenderingColor::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  RunPeriod::iddObjectType() )
  {
       model::RunPeriod runPeriod = modelObject.cast<RunPeriod>();
       retVal = translateRunPeriod(runPeriod);
  }
  else if( modelObject.iddObject().type() ==  RunPeriodControlDaylightSavingTime::iddObjectType() )
  {
       model::RunPeriodControlDaylightSavingTime mo = modelObject.cast<RunPeriodControlDaylightSavingTime>();
       retVal = translateRunPeriodControlDaylightSavingTime(mo);
  }
  else if( modelObject.iddObject().type() ==  RunPeriodControlSpecialDays::iddObjectType() )
  {
       model::RunPeriodControlSpecialDays mo = modelObject.cast<RunPeriodControlSpecialDays>();
       retVal = translateRunPeriodControlSpecialDays(mo);
  }
  else if( modelObject.iddObject().type() ==  ScheduleCompact::iddObjectType() )
  {
       model::ScheduleCompact schedule = modelObject.cast<ScheduleCompact>();
       retVal = translateScheduleCompact(schedule);
  }
  else if( modelObject.iddObject().type() ==  ScheduleConstant::iddObjectType() )
  {
       model::ScheduleConstant schedule = modelObject.cast<ScheduleConstant>();
       retVal = translateScheduleConstant(schedule);
  }
  else if( modelObject.iddObject().type() ==  ScheduleDay::iddObjectType() )
  {
       model::ScheduleDay schedule = modelObject.cast<ScheduleDay>();
       retVal = translateScheduleDay(schedule);
  }
  else if( modelObject.iddObject().type() ==   ScheduleFixedInterval::iddObjectType() )
  {
       model::ScheduleFixedInterval schedule = modelObject.cast<ScheduleFixedInterval>();
       retVal = translateScheduleFixedInterval(schedule);
  }
  else if( modelObject.iddObject().type() ==   ScheduleRule::iddObjectType() )
  {
       // no-op
  }
  else if( modelObject.iddObject().type() ==   ScheduleRuleset::iddObjectType() )
  {
       model::ScheduleRuleset mo = modelObject.cast<ScheduleRuleset>();
       retVal = translateScheduleRuleset(mo);
  }
  else if( modelObject.iddObject().type() ==   ScheduleTypeLimits::iddObjectType() )
  {
       model::ScheduleTypeLimits mo = modelObject.cast<ScheduleTypeLimits>();
       retVal = translateScheduleTypeLimits(mo);
  }
  else if( modelObject.iddObject().type() ==   ScheduleWeek::iddObjectType() )
  {
       model::ScheduleWeek schedule = modelObject.cast<ScheduleWeek>();
       retVal = translateScheduleWeek(schedule);
  }
  else if( modelObject.iddObject().type() ==   ScheduleYear::iddObjectType() )
  {
       model::ScheduleYear schedule = modelObject.cast<ScheduleYear>();
       retVal = translateScheduleYear(schedule);
  }
  else if( modelObject.iddObject().type() ==   ScheduleVariableInterval::iddObjectType() )
  {
       model::ScheduleVariableInterval schedule = modelObject.cast<ScheduleVariableInterval>();
       retVal = translateScheduleVariableInterval(schedule);
  }
  else if( modelObject.iddObject().type() ==   SetpointManagerFollowOutdoorAirTemperature::iddObjectType() )
  {
       model::SetpointManagerFollowOutdoorAirTemperature spm = modelObject.cast<SetpointManagerFollowOutdoorAirTemperature>();
       retVal = translateSetpointManagerFollowOutdoorAirTemperature(spm);
  }
  else if( modelObject.iddObject().type() ==   SetpointManagerSingleZoneReheat::iddObjectType() )
  {
       model::SetpointManagerSingleZoneReheat spm = modelObject.cast<SetpointManagerSingleZoneReheat>();
       retVal = translateSetpointManagerSingleZoneReheat(spm);
  }
  else if( modelObject.iddObject().type() ==   SetpointManagerScheduled::iddObjectType() )
  {
       model::SetpointManagerScheduled spm = modelObject.cast<SetpointManagerScheduled>();
       retVal = translateSetpointManagerScheduled(spm);
  }
  else if( modelObject.iddObject().type() ==   SetpointManagerMixedAir::iddObjectType() )
  {
       model::SetpointManagerMixedAir spm = modelObject.cast<SetpointManagerMixedAir>();
       retVal = translateSetpointManagerMixedAir(spm);
  }
  else if( modelObject.iddObject().type() ==   SetpointManagerOutdoorAirReset::iddObjectType() )
  {
       model::SetpointManagerOutdoorAirReset spm = modelObject.cast<SetpointManagerOutdoorAirReset>();
       retVal = translateSetpointManagerOutdoorAirReset(spm);
  }
  else if( modelObject.iddObject().type() ==   SetpointManagerWarmest::iddObjectType() )
  {
       model::SetpointManagerWarmest spm = modelObject.cast<SetpointManagerWarmest>();
       retVal = translateSetpointManagerWarmest(spm);
  }
  else if( modelObject.iddObject().type() ==   ShadingControl::iddObjectType() )
  {
       model::ShadingControl shadingControl = modelObject.cast<ShadingControl>();
       retVal = translateShadingControl(shadingControl);
  }    
  else if( modelObject.iddObject().type() ==   ShadingSurface::iddObjectType() )
  {
       model::ShadingSurface shadingSurface = modelObject.cast<ShadingSurface>();
       retVal = translateShadingSurface(shadingSurface);
  }
  else if( modelObject.iddObject().type() ==   ShadingSurfaceGroup::iddObjectType() )
  {
       model::ShadingSurfaceGroup shadingSurfaceGroup = modelObject.cast<ShadingSurfaceGroup>();
       retVal = translateShadingSurfaceGroup(shadingSurfaceGroup);
  }
  else if( modelObject.iddObject().type() ==   ShadowCalculation::iddObjectType() )
  {
       model::ShadowCalculation mo = modelObject.cast<ShadowCalculation>();
       retVal = translateShadowCalculation(mo);
  }
  else if( modelObject.iddObject().type() ==   SimulationControl::iddObjectType() )
  {
       model::SimulationControl simulationControl = modelObject.cast<SimulationControl>();
       retVal = translateSimulationControl(simulationControl);
  }
  else if( modelObject.iddObject().type() ==  Space::iddObjectType() )
  {
       model::Space space = modelObject.cast<Space>();
       retVal = translateSpace(space);
  }
  else if( modelObject.iddObject().type() ==  SpaceInfiltrationDesignFlowRate::iddObjectType() )
  {
       model::SpaceInfiltrationDesignFlowRate spaceInfiltrationDesignFlowRate = modelObject.cast<SpaceInfiltrationDesignFlowRate>();
       retVal = translateSpaceInfiltrationDesignFlowRate(spaceInfiltrationDesignFlowRate);
  }
  else if( modelObject.iddObject().type() ==  SpaceInfiltrationEffectiveLeakageArea::iddObjectType() )
  {
       model::SpaceInfiltrationEffectiveLeakageArea spaceInfiltrationEffectiveLeakageArea = modelObject.cast<SpaceInfiltrationEffectiveLeakageArea>();
       retVal = translateSpaceInfiltrationEffectiveLeakageArea(spaceInfiltrationEffectiveLeakageArea);
  }
  else if( modelObject.iddObject().type() ==  SpaceType::iddObjectType() )
  {
       model::SpaceType spaceType = modelObject.cast<SpaceType>();
       retVal = translateSpaceType(spaceType);
  }
  else if( modelObject.iddObject().type() ==  Site::iddObjectType() )
  {
       model::Site site = modelObject.cast<Site>();
       retVal = translateSite(site);
  }
  else if( modelObject.iddObject().type() ==  SiteGroundReflectance::iddObjectType() )
  {
       model::SiteGroundReflectance mo = modelObject.cast<SiteGroundReflectance>();
       retVal = translateSiteGroundReflectance(mo);
  }
  else if( modelObject.iddObject().type() ==  SiteGroundTemperatureBuildingSurface::iddObjectType() )
  {
       model::SiteGroundTemperatureBuildingSurface mo = modelObject.cast<SiteGroundTemperatureBuildingSurface>();
       retVal = translateSiteGroundTemperatureBuildingSurface(mo);
  }
  else if( modelObject.iddObject().type() ==  SiteWaterMainsTemperature::iddObjectType() )
  {
       model::SiteWaterMainsTemperature mo = modelObject.cast<SiteWaterMainsTemperature>();
       retVal = translateSiteWaterMainsTemperature(mo);
  }
  else if( modelObject.iddObject().type() ==  SizingParameters::iddObjectType() )
  {
       model::SizingParameters mo = modelObject.cast<SizingParameters>();
       retVal = translateSizingParameters(mo);
  }
  else if( modelObject.iddObject().type() ==  ConstructionBaseStandardsInformation::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  SteamEquipment::iddObjectType() )
  {
       model::SteamEquipment steamEquipment = modelObject.cast<SteamEquipment>();
       retVal = translateSteamEquipment(steamEquipment);
  }
  else if( modelObject.iddObject().type() ==  Surface::iddObjectType() )
  {
       model::Surface surface = modelObject.cast<Surface>();
       retVal = translateSurface(surface);
  }
  else if( modelObject.iddObject().type() ==  InsideSurfaceConvectionAlgorithm::iddObjectType() )
  {
       model::InsideSurfaceConvectionAlgorithm mo = modelObject.cast<InsideSurfaceConvectionAlgorithm>();
       retVal = translateInsideSurfaceConvectionAlgorithm(mo);
  }
  else if( modelObject.iddObject().type() ==  OutsideSurfaceConvectionAlgorithm::iddObjectType() )
  {
       model::OutsideSurfaceConvectionAlgorithm mo = modelObject.cast<OutsideSurfaceConvectionAlgorithm>();
       retVal = translateOutsideSurfaceConvectionAlgorithm(mo);
  }
  else if( modelObject.iddObject().type() ==  SubSurface::iddObjectType() )
  {
       model::SubSurface subSurface = modelObject.cast<SubSurface>();
       retVal = translateSubSurface(subSurface);
  }
  else if( modelObject.iddObject().type() ==  ThermalZone::iddObjectType() )
  {
       model::ThermalZone zone = modelObject.cast<ThermalZone>();
       retVal = translateThermalZone(zone);
  }
  else if( modelObject.iddObject().type() ==  ThermostatSetpointDualSetpoint::iddObjectType() )
  {
       model::ThermostatSetpointDualSetpoint thermostat = modelObject.cast<ThermostatSetpointDualSetpoint>();
       retVal = translateThermostatSetpointDualSetpoint(thermostat);
  }
  else if( modelObject.iddObject().type() ==  Timestep::iddObjectType() )
  {
       model::Timestep timestep = modelObject.cast<Timestep>();
       retVal = translateTimestep(timestep);
  }
  else if( modelObject.iddObject().type() ==  Version::iddObjectType() )
  {
       model::Version version = modelObject.cast<Version>();
       retVal = translateVersion(version);
  }
  else if( modelObject.iddObject().type() ==  WeatherFile::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  SkyTemperature::iddObjectType() )
  {
       model::SkyTemperature mo = modelObject.cast<SkyTemperature>();
       retVal = translateSkyTemperature(mo);
  }
  else if( modelObject.iddObject().type() ==  RefractionExtinctionGlazing::iddObjectType() )
  {
       model::RefractionExtinctionGlazing mo = modelObject.cast<RefractionExtinctionGlazing>();
       retVal = translateRefractionExtinctionGlazing(mo);
  }
  else if( modelObject.iddObject().type() ==  Screen::iddObjectType() )
  {
       model::Screen mo = modelObject.cast<Screen>();
       retVal = translateScreen(mo);
  }
  else if( modelObject.iddObject().type() ==  Shade::iddObjectType() )
  {
       model::Shade shade = modelObject.cast<Shade>();
       retVal = translateShade(shade);
  }
  else if( modelObject.iddObject().type() ==  SimpleGlazing::iddObjectType() )
  {
       model::SimpleGlazing glazing = modelObject.cast<SimpleGlazing>();
       retVal = translateSimpleGlazing(glazing);
  }
  else if( modelObject.iddObject().type() ==  StandardGlazing::iddObjectType() )
  {
       model::StandardGlazing glazing = modelObject.cast<StandardGlazing>();
       retVal = translateStandardGlazing(glazing);
  }
  else if( modelObject.iddObject().type() ==  ZoneAirContaminantBalance::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  ZoneAirHeatBalanceAlgorithm::iddObjectType() )
  {
       model::ZoneAirHeatBalanceAlgorithm mo = modelObject.cast<ZoneAirHeatBalanceAlgorithm>();
       retVal = translateZoneAirHeatBalanceAlgorithm(mo);
  }
  else if( modelObject.iddObject().type() ==  ZoneCapacitanceMultiplierResearchSpecial::iddObjectType() )
  {
       // no-op
       return retVal;
  }
  else if( modelObject.iddObject().type() ==  ZoneHVACBaseboardConvectiveElectric::iddObjectType() )
  {
       model::ZoneHVACBaseboardConvectiveElectric mo = modelObject.cast<ZoneHVACBaseboardConvectiveElectric>();
       retVal = translateZoneHVACBaseboardConvectiveElectric(mo);
  }  
  else if( modelObject.iddObject().type() ==  ZoneHVACBaseboardConvectiveWater::iddObjectType() )
  {
       model::ZoneHVACBaseboardConvectiveWater mo = modelObject.cast<ZoneHVACBaseboardConvectiveWater>();
       retVal = translateZoneHVACBaseboardConvectiveWater(mo);
  }  
  else if( modelObject.iddObject().type() ==  ZoneHVACEquipmentList::iddObjectType() )
  {
       model::ZoneHVACEquipmentList mo = modelObject.cast<ZoneHVACEquipmentList>();
       retVal = translateZoneHVACEquipmentList(mo);
  }
  else if( modelObject.iddObject().type() ==  ZoneHVACFourPipeFanCoil::iddObjectType() )
  {
       model::ZoneHVACFourPipeFanCoil mo = modelObject.cast<ZoneHVACFourPipeFanCoil>();
       retVal = translateZoneHVACFourPipeFanCoil(mo);
  }  
  else if( modelObject.iddObject().type() ==  ZoneHVACIdealLoadsAirSystem::iddObjectType() )
  {
       model::ZoneHVACIdealLoadsAirSystem mo = modelObject.cast<ZoneHVACIdealLoadsAirSystem>();
       retVal = translateZoneHVACIdealLoadsAirSystem(mo);
  }
  else if( modelObject.iddObject().type() ==  ZoneHVACLowTempRadiantConstFlow::iddObjectType() )
  { 
       model::ZoneHVACLowTempRadiantConstFlow mo = modelObject.cast<ZoneHVACLowTempRadiantConstFlow>();
       retVal = translateZoneHVACLowTempRadiantConstFlow(mo);
  }
  else if( modelObject.iddObject().type() ==  ZoneHVACLowTempRadiantVarFlow::iddObjectType() )
  { 
       model::ZoneHVACLowTempRadiantVarFlow mo = modelObject.cast<ZoneHVACLowTempRadiantVarFlow>();
       retVal = translateZoneHVACLowTempRadiantVarFlow(mo);
  }
  else if( modelObject.iddObject().type() ==  ZoneHVACLowTemperatureRadiantElectric::iddObjectType() )
  {
       model::ZoneHVACLowTemperatureRadiantElectric mo = modelObject.cast<ZoneHVACLowTemperatureRadiantElectric>();
       retVal = translateZoneHVACLowTemperatureRadiantElectric(mo);
  }  
  else if( modelObject.iddObject().type() ==  ZoneHVACPackagedTerminalHeatPump::iddObjectType() )
  {
       model::ZoneHVACPackagedTerminalHeatPump mo = modelObject.cast<ZoneHVACPackagedTerminalHeatPump>();
       retVal = translateZoneHVACPackagedTerminalHeatPump(mo);
  }
  else if( modelObject.iddObject().type() ==  ZoneHVACPackagedTerminalAirConditioner::iddObjectType() )
  {
       model::ZoneHVACPackagedTerminalAirConditioner mo = modelObject.cast<ZoneHVACPackagedTerminalAirConditioner>();
       retVal = translateZoneHVACPackagedTerminalAirConditioner(mo);
  }
  else if( modelObject.iddObject().type() ==  ZoneHVACTerminalUnitVariableRefrigerantFlow::iddObjectType() )
  {
       model::ZoneHVACTerminalUnitVariableRefrigerantFlow mo = modelObject.cast<ZoneHVACTerminalUnitVariableRefrigerantFlow>();
       retVal = translateZoneHVACTerminalUnitVariableRefrigerantFlow(mo);
  }
  else if( modelObject.iddObject().type() ==  ZoneHVACWaterToAirHeatPump::iddObjectType() )
  {
       model::ZoneHVACWaterToAirHeatPump mo = modelObject.cast<ZoneHVACWaterToAirHeatPump>();
       retVal = translateZoneHVACWaterToAirHeatPump(mo);
  }
  else if( modelObject.iddObject().type() ==  ZoneHVACUnitHeater::iddObjectType() )
  {
       model::ZoneHVACUnitHeater mo = modelObject.cast<ZoneHVACUnitHeater>();
       retVal = translateZoneHVACUnitHeater(mo);
  }
  //If no case statement log a warning
  else
  {
      LOG(Warn, "Unknown IddObjectType: '" << modelObject.iddObject().name() << "'");
      return retVal;
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
  result.push_back(IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam);
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

  result.push_back(IddObjectType::OS_Refrigeration_System);

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

boost::optional<IdfFile> ForwardTranslator::findIdfFile(const std::string& path) {
  QFile file(QString().fromStdString(path));
  bool opened = file.open(QIODevice::ReadOnly | QIODevice::Text);
  OS_ASSERT(opened);

  QTextStream in(&file);
  std::stringstream ss;
  ss << in.readAll().toStdString();

  return IdfFile::load(ss, IddFileType::EnergyPlus);
}

void ForwardTranslator::createFluidPropertiesMap()
{
  m_fluidPropertiesMap.insert(make_pair("R11", ":/Resources/R11_FluidPropertiesDataSet.idf"));
  m_fluidPropertiesMap.insert(make_pair("R12", ":/Resources/R12_FluidPropertiesDataSet.idf"));
  m_fluidPropertiesMap.insert(make_pair("R22", ":/Resources/R22_FluidPropertiesDataSet.idf"));
  m_fluidPropertiesMap.insert(make_pair("R123", ":/Resources/R123_FluidPropertiesDataSet.idf"));
  m_fluidPropertiesMap.insert(make_pair("R134a", ":/Resources/R134a_FluidPropertiesDataSet.idf"));
  m_fluidPropertiesMap.insert(make_pair("R404a", ":/Resources/R404a_FluidPropertiesDataSet.idf"));
  m_fluidPropertiesMap.insert(make_pair("R407a", ":/Resources/R407a_FluidPropertiesDataSet.idf"));
  m_fluidPropertiesMap.insert(make_pair("R410a", ":/Resources/R410a_FluidPropertiesDataSet.idf"));
  m_fluidPropertiesMap.insert(make_pair("NH3", ":/Resources/NH3_FluidPropertiesDataSet.idf"));
  m_fluidPropertiesMap.insert(make_pair("R507a", ":/Resources/R507a_FluidPropertiesDataSet.idf"));
  m_fluidPropertiesMap.insert(make_pair("R744", ":/Resources/R744_FluidPropertiesDataSet.idf"));
}

boost::optional<IdfObject> ForwardTranslator::createFluidProperties(const std::string& glycolType, int glycolConcentration) {

  std::stringstream sstm;
  sstm << glycolType << "_" << glycolConcentration;
  std::string glycolName = sstm.str();

  for( std::vector<IdfObject>::iterator it = m_idfObjects.begin();
     it != m_idfObjects.end();
     it++ )
  {
    if(it->iddObject().type() == openstudio::IddObjectType::FluidProperties_Name) {
      if(istringEqual(it->getString(FluidProperties_NameFields::FluidName,true).get(), glycolName)) {
        return *it;
      }
    }
  }

  IdfObject fluidPropName(openstudio::IddObjectType::FluidProperties_Name);
  fluidPropName.setString(FluidProperties_NameFields::FluidName, glycolName);
  fluidPropName.setString(FluidProperties_NameFields::FluidType, "Glycol");

  IdfObject fluidPropGlyConcentration(openstudio::IddObjectType::FluidProperties_GlycolConcentration);
  fluidPropGlyConcentration.setName(glycolName);
  if(istringEqual(glycolType, "PropyleneGlycol")){
    fluidPropGlyConcentration.setString(FluidProperties_GlycolConcentrationFields::GlycolType, "PropyleneGlycol");
  }
  else if(istringEqual(glycolType, "EthyleneGlycol")){
    fluidPropGlyConcentration.setString(FluidProperties_GlycolConcentrationFields::GlycolType, "EthyleneGlycol");
  }
  else {
    return boost::none;
  }
  fluidPropGlyConcentration.setDouble(FluidProperties_GlycolConcentrationFields::GlycolConcentration, glycolConcentration * 0.01);

  m_idfObjects.push_back(fluidPropName);
  m_idfObjects.push_back(fluidPropGlyConcentration);

  return fluidPropName;
}

boost::optional<IdfObject> ForwardTranslator::createFluidProperties(const std::string& fluidType) {

  boost::optional<IdfObject> idfObject;
  boost::optional<IdfFile> idfFile;

  for( std::vector<IdfObject>::iterator it = m_idfObjects.begin();
     it != m_idfObjects.end();
     it++ )
  {
    if(it->iddObject().type() == openstudio::IddObjectType::FluidProperties_Name) {
      if(istringEqual(it->getString(FluidProperties_NameFields::FluidName,true).get(), fluidType)) {
        return *it;
      }
    }
  }

  FluidPropertiesMap::const_iterator objInMap = m_fluidPropertiesMap.find( fluidType );
  if( objInMap != m_fluidPropertiesMap.end() )
  {
    idfFile = findIdfFile(objInMap->second);
  }
  else
  {
    LOG(Warn, "Fluid Type not valid choice: '" << fluidType << "'");
    return boost::none;
  }

  if(idfFile){
    std::vector<IdfObject> fluidObjects = idfFile->objects();

    for( std::vector<IdfObject>::iterator it = fluidObjects.begin();
       it != fluidObjects.end();
       it++ )
    {
      if(it->iddObject().type() == openstudio::IddObjectType::FluidProperties_Name) {
        idfObject = *it;
      }
      m_idfObjects.push_back(*it);
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio
