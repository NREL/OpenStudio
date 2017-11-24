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

#include "EnergyPlusAPI.hpp"
#include <src/energyplus/embedded_files.hxx>

#include "ForwardTranslator.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/Surface.hpp"
#include "../model/Surface_Impl.hpp"
#include "../model/Construction.hpp"
#include "../model/Construction_Impl.hpp"
#include "../model/ConstructionWithInternalSource.hpp"
#include "../model/ConstructionWithInternalSource_Impl.hpp"
#include "../model/WaterUseEquipment.hpp"
#include "../model/WaterUseEquipment_Impl.hpp"
#include "../model/RunPeriod.hpp"
#include "../model/RunPeriod_Impl.hpp"
#include "../model/RunPeriodControlSpecialDays.hpp"
#include "../model/RunPeriodControlSpecialDays_Impl.hpp"
#include "../model/SimulationControl.hpp"
#include "../model/SimulationControl_Impl.hpp"
#include "../model/Building.hpp"
#include "../model/Building_Impl.hpp"
#include "../model/UtilityBill.hpp"
#include "../model/UtilityBill_Impl.hpp"
#include "../model/ElectricLoadCenterDistribution.hpp"
#include "../model/ElectricLoadCenterDistribution_Impl.hpp"
#include "../model/ConcreteModelObjects.hpp"

#include "../utilities/idf/Workspace.hpp"
#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "../utilities/idf/IdfFile.hpp"
#include "../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/FilesystemHelpers.hpp"
#include "../utilities/geometry/BoundingBox.hpp"
#include "../utilities/time/Time.hpp"
#include "../utilities/plot/ProgressBar.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/FluidProperties_Name_FieldEnums.hxx>
#include <utilities/idd/FluidProperties_GlycolConcentration_FieldEnums.hxx>
#include <utilities/idd/GlobalGeometryRules_FieldEnums.hxx>
#include <utilities/idd/Output_Table_SummaryReports_FieldEnums.hxx>
#include <utilities/idd/OutputControl_Table_Style_FieldEnums.hxx>
#include <utilities/idd/Output_VariableDictionary_FieldEnums.hxx>
#include <utilities/idd/Output_SQLite_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_NonrecurringCost_FieldEnums.hxx>
#include <utilities/idd/SetpointManager_MixedAir_FieldEnums.hxx>

#include "../utilities/idd/IddEnums.hpp"

#include <QFile>
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
  m_ipTabularOutput = false;
  m_excludeLCCObjects = false;
}

Workspace ForwardTranslator::translateModel( const Model & model, ProgressBar* progressBar )
{
  Model modelCopy = model.clone(true).cast<Model>();

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

  m_progressBar = nullptr;

  return translateModelPrivate(modelCopy, false);
}

std::vector<LogMessage> ForwardTranslator::warnings() const
{
  std::vector<LogMessage> result;

  for (LogMessage logMessage : m_logSink.logMessages()){
    if (logMessage.logLevel() == Warn){
      result.push_back(logMessage);
    }
  }

  return result;
}

std::vector<LogMessage> ForwardTranslator::errors() const
{
  std::vector<LogMessage> result;

  for (LogMessage logMessage : m_logSink.logMessages()){
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

void ForwardTranslator::setIPTabularOutput(bool isIP)
{
  m_ipTabularOutput = isIP;
}

void ForwardTranslator::setExcludeLCCObjects(bool excludeLCCObjects)
{
  m_excludeLCCObjects = excludeLCCObjects;
}

Workspace ForwardTranslator::translateModelPrivate( model::Model & model, bool fullModelTranslation )
{
  reset();

  // translate Version first
  model::Version version = model.getUniqueModelObject<model::Version>();
  translateAndMapModelObject(version);

  // resolve surface marching conflicts before combining thermal zones or removing spaces
  // as those operations may change search distances
  resolveMatchedSurfaceConstructionConflicts(model);
  resolveMatchedSubSurfaceConstructionConflicts(model);

  // check for spaces not in a thermal zone
  for (Space space : model.getConcreteModelObjects<Space>()){
    if (!space.thermalZone()){
      LOG(Warn, "Space " << space.name().get() << " is not associated with a ThermalZone, it will not be translated.");
      space.remove();
    }
  }

  // remove orphan surfaces
  for (PlanarSurface planarSurface : model.getModelObjects<PlanarSurface>()){
    if (!planarSurface.planarSurfaceGroup()){
      LOG(Warn, planarSurface.briefDescription() << " is not associated with a PlanarSurfaceGroup, it will not be translated.");
      planarSurface.remove();
    }
  }

  // remove orphan loads
  for (SpaceLoad spaceLoad : model.getModelObjects<SpaceLoad>()){
    if (spaceLoad.optionalCast<model::WaterUseEquipment>()){
      // WaterUseEquipment is not required to be attached to a space
      continue;
    }
    if ((!spaceLoad.space()) && (!spaceLoad.spaceType())){
      LOG(Warn, spaceLoad.briefDescription() << " is not associated with a Space or SpaceType, it will not be translated.");
      spaceLoad.remove();
    }
  }

  // next thing to do is combine all spaces in each thermal zone
  // after this each zone will have 0 or 1 spaces and each space will have 0 or 1 zone
  for (ThermalZone thermalZone : model.getConcreteModelObjects<ThermalZone>()){
    thermalZone.combineSpaces();
  }

  // remove unused space types
  std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
  for (SpaceType spaceType : spaceTypes){
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
  std::vector<OtherEquipment> otherEquipments = model.getConcreteModelObjects<OtherEquipment>();
  for (OtherEquipment otherEquipment : otherEquipments){
    boost::optional<SpaceType> spaceTypeOfOtherEquipment = otherEquipment.spaceType();
    if (spaceTypeOfOtherEquipment){
      //loop through the spaces in this space type and make a new instance for each one
      std::vector<Space> spaces = spaceTypeOfOtherEquipment.get().spaces();
      for (Space space : spaces){
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

  // Temporary workaround for EnergyPlusTeam #4451
  // requested by http://code.google.com/p/cbecc/issues/detail?id=736
  // do this after combining spaces to avoid suprises about relative coordinate changes
  for (const auto& thermalZone : model.getConcreteModelObjects<ThermalZone>()){
    boost::optional<DaylightingControl> dc = thermalZone.secondaryDaylightingControl();
    if (dc){
      double z = dc->positionZCoordinate();
      if (z < 0){
        // find lowest point in thermalZone and move space origin down to that point
        // lowest point will have z = 0 in relative coordinates
        std::vector<Space> spaces = thermalZone.spaces();
        OS_ASSERT(spaces.size() == 1);

        double minZ = z;
        BoundingBox bb = spaces[0].boundingBox();
        if (bb.minZ()){
          minZ = std::min(minZ, bb.minZ().get());
        }
        OS_ASSERT(minZ < 0);

        Transformation currentT = spaces[0].transformation();
        Transformation newT = Transformation::translation(Vector3d(0, 0, minZ))*currentT;
        bool test = spaces[0].changeTransformation(newT);
        OS_ASSERT(test);
      }
    }
  }

  // remove orphan Generator:MicroTurbine
  for (auto& chp : model.getConcreteModelObjects<GeneratorMicroTurbine>()){
    if (!chp.electricLoadCenterDistribution()){
      LOG(Warn, "GeneratorMicroTurbine " << chp.name().get() << " is not referenced by any ElectricLoadCenterDistribution, it will not be translated.");
      chp.remove();
      continue;
    }
  }

  // remove orphan photovoltaics
  for (auto& pv : model.getConcreteModelObjects<GeneratorPhotovoltaic>()){
    if (!pv.electricLoadCenterDistribution()){
      LOG(Warn, "GeneratorPhotovoltaic " << pv.name().get() << " is not referenced by any ElectricLoadCenterDistribution, it will not be translated.");
      pv.remove();
      continue;
    }
    if (!pv.surface()){
      LOG(Warn, "GeneratorPhotovoltaic " << pv.name().get() << " is not referenced by any surface, it will not be translated.");
      pv.remove();
    }
  }

  // Remove orphan Storage
  for (auto& storage : model.getModelObjects<ElectricalStorage>()) {
    if (!storage.electricLoadCenterDistribution()){
      LOG(Warn, "Electrical Storage " << storage.name().get() << " is not referenced by any ElectricLoadCenterDistribution, it will not be translated.");
      storage.remove();
    }
  }

  // Remove orphan Converters
  for (auto& converter : model.getConcreteModelObjects<ElectricLoadCenterStorageConverter>()){
    if (!converter.electricLoadCenterDistribution()){
      LOG(Warn, "Converter " << converter.name().get() << " is not referenced by any ElectricLoadCenterDistribution, it will not be translated.");
      converter.remove();
    }
  }

  // Remove empty electric load center distribution objects (e.g. with no generators)
  // requested by jmarrec, https://github.com/NREL/OpenStudio/pull/1927
  for (auto& elcd : model.getConcreteModelObjects<ElectricLoadCenterDistribution>()){
    if (elcd.generators().empty()){
      LOG(Warn, "ElectricLoadCenterDistribution " << elcd.name().get() << " is not referenced by any generators, it will not be translated.");
      if (auto inverter = elcd.inverter()){
        inverter->remove();
      }
      elcd.remove();
    }
  }

  for (auto& inverter : model.getModelObjects<Inverter>()){
    if (!inverter.electricLoadCenterDistribution()){
      LOG(Warn, "Inverter " << inverter.name().get() << " is not referenced by any ElectricLoadCenterDistribution, it will not be translated.");
      inverter.remove();
    }
  }



  // temp code
  if (!m_keepRunControlSpecialDays){
    // DLM: we will not translate these objects until we support holidays in the GUI
    // we will not warn users because these objects are not exposed in the GUI
    for (model::RunPeriodControlSpecialDays holiday : model.getConcreteModelObjects<model::RunPeriodControlSpecialDays>()){
      holiday.remove();
    }
  }

  if (fullModelTranslation){

    // translate life cycle cost parameters
    if( ! m_excludeLCCObjects ){
      boost::optional<LifeCycleCostParameters> lifeCycleCostParameters = model.lifeCycleCostParameters();
      if (!lifeCycleCostParameters){
        // only warn if costs are present
        if (!model.getConcreteModelObjects<LifeCycleCost>().empty()){
          LOG(Warn, "No LifeCycleCostParameters but LifeCycleCosts are present, adding default LifeCycleCostParameters.");
        }

        // always add this object so E+ results section exists
        lifeCycleCostParameters = model.getUniqueModelObject<LifeCycleCostParameters>();
      }
      translateAndMapModelObject(*lifeCycleCostParameters);
    }

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

    // create meters for utility bill objects
    std::vector<UtilityBill> utilityBills = model.getConcreteModelObjects<UtilityBill>();
    for (UtilityBill utilityBill : utilityBills){
      // these meters and variables will be translated later
      OutputMeter consumptionMeter = utilityBill.consumptionMeter();
      boost::optional<OutputMeter> peakDemandMeter = utilityBill.peakDemandMeter();
    }
  }

  translateConstructions(model);
  translateSchedules(model);

  // get air loops in sorted order
  std::vector<AirLoopHVAC> airLoops = model.getConcreteModelObjects<AirLoopHVAC>();
  std::sort(airLoops.begin(), airLoops.end(), WorkspaceObjectNameLess());
  for (AirLoopHVAC airLoop : airLoops){
    translateAndMapModelObject(airLoop);
  }

  // get AirConditionerVariableRefrigerantFlow objects in sorted order
  std::vector<AirConditionerVariableRefrigerantFlow> vrfs = model.getConcreteModelObjects<AirConditionerVariableRefrigerantFlow>();
  std::sort(vrfs.begin(), vrfs.end(), WorkspaceObjectNameLess());
  for (AirConditionerVariableRefrigerantFlow vrf : vrfs){
    translateAndMapModelObject(vrf);
  }

  // get plant loops in sorted order
  std::vector<PlantLoop> plantLoops = model.getConcreteModelObjects<PlantLoop>();
  std::sort(plantLoops.begin(), plantLoops.end(), WorkspaceObjectNameLess());
  for (PlantLoop plantLoop : plantLoops){
    translateAndMapModelObject(plantLoop);
  }

  // now loop over all objects
  for (const IddObjectType& iddObjectType : iddObjectsToTranslate()){

    // get objects by type in sorted order
    std::vector<WorkspaceObject> objects = model.getObjectsByType(iddObjectType);
    std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());

    for (const WorkspaceObject& workspaceObject : objects){
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
    auto ita = std::find(m_iddObjectTypes.begin(), m_iddObjectTypes.end(), a.iddObject().type());
    auto itb = std::find(m_iddObjectTypes.begin(), m_iddObjectTypes.end(), b.iddObject().type());

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
  case openstudio::IddObjectType::OS_AirConditioner_VariableRefrigerantFlow :
    {
      model::AirConditionerVariableRefrigerantFlow vrf = modelObject.cast<AirConditionerVariableRefrigerantFlow>();
      retVal = translateAirConditionerVariableRefrigerantFlow(vrf);
      break;
    }
  case openstudio::IddObjectType::OS_AirLoopHVAC :
    {
      model::AirLoopHVAC airLoopHVAC = modelObject.cast<AirLoopHVAC>();
      retVal = translateAirLoopHVAC(airLoopHVAC);
      break;
    }
  case openstudio::IddObjectType::OS_AirLoopHVAC_ReturnPlenum :
    {
      model::AirLoopHVACReturnPlenum airLoopHVACReturnPlenum = modelObject.cast<AirLoopHVACReturnPlenum>();
      retVal = translateAirLoopHVACReturnPlenum(airLoopHVACReturnPlenum);
      break;
    }
  case openstudio::IddObjectType::OS_AirLoopHVAC_SupplyPlenum :
    {
      model::AirLoopHVACSupplyPlenum airLoopHVACSupplyPlenum = modelObject.cast<AirLoopHVACSupplyPlenum>();
      retVal = translateAirLoopHVACSupplyPlenum(airLoopHVACSupplyPlenum);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_DualDuct_VAV :
    {
      auto mo = modelObject.cast<AirTerminalDualDuctVAV>();
      retVal = translateAirTerminalDualDuctVAV(mo);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction :
    {
      model::AirTerminalSingleDuctConstantVolumeFourPipeInduction airTerminal = modelObject.cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
      retVal = translateAirTerminalSingleDuctConstantVolumeFourPipeInduction(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_Reheat :
    {
      model::AirTerminalSingleDuctConstantVolumeReheat airTerminal = modelObject.cast<AirTerminalSingleDuctConstantVolumeReheat>();
      retVal = translateAirTerminalSingleDuctConstantVolumeReheat(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam :
    {
      model::AirTerminalSingleDuctConstantVolumeCooledBeam airTerminal = modelObject.cast<AirTerminalSingleDuctConstantVolumeCooledBeam>();
      retVal = translateAirTerminalSingleDuctConstantVolumeCooledBeam(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_ParallelPIU_Reheat :
    {
      model::AirTerminalSingleDuctParallelPIUReheat airTerminal = modelObject.cast<AirTerminalSingleDuctParallelPIUReheat>();
      retVal = translateAirTerminalSingleDuctParallelPIUReheat(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_SeriesPIU_Reheat :
    {
      model::AirTerminalSingleDuctSeriesPIUReheat airTerminal = modelObject.cast<AirTerminalSingleDuctSeriesPIUReheat>();
      retVal = translateAirTerminalSingleDuctSeriesPIUReheat(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled :
    {
      model::AirTerminalSingleDuctUncontrolled airTerminal = modelObject.cast<AirTerminalSingleDuctUncontrolled>();
      retVal = translateAirTerminalSingleDuctUncontrolled(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_NoReheat :
    {
      model::AirTerminalSingleDuctVAVNoReheat airTerminal = modelObject.cast<AirTerminalSingleDuctVAVNoReheat>();
      retVal = translateAirTerminalSingleDuctVAVNoReheat(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat :
    {
      model::AirTerminalSingleDuctVAVReheat airTerminal = modelObject.cast<AirTerminalSingleDuctVAVReheat>();
      retVal = translateAirTerminalSingleDuctVAVReheat(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_InletSideMixer :
    {
      model::AirTerminalSingleDuctInletSideMixer airTerminal = modelObject.cast<AirTerminalSingleDuctInletSideMixer>();
      retVal = translateAirTerminalSingleDuctInletSideMixer(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat :
    {
      model::AirTerminalSingleDuctVAVHeatAndCoolNoReheat airTerminal = modelObject.cast<AirTerminalSingleDuctVAVHeatAndCoolNoReheat>();
      retVal = translateAirTerminalSingleDuctVAVHeatAndCoolNoReheat(airTerminal);
      break;
    }
  case openstudio::IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat :
    {
      model::AirTerminalSingleDuctVAVHeatAndCoolReheat airTerminal = modelObject.cast<AirTerminalSingleDuctVAVHeatAndCoolReheat>();
      retVal = translateAirTerminalSingleDuctVAVHeatAndCoolReheat(airTerminal);
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
  case openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass :
    {
      model::AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass unitary = modelObject.cast<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();
      retVal = translateAirLoopHVACUnitaryHeatCoolVAVChangeoverBypass(unitary);
      break;
    }
  case openstudio::IddObjectType::OS_AirLoopHVAC_UnitaryHeatPump_AirToAir_MultiSpeed :
    {
      model::AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed unitary = modelObject.cast<AirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed>();
      retVal = translateAirLoopHVACUnitaryHeatPumpAirToAirMultiSpeed(unitary);
      break;
    }
  case openstudio::IddObjectType::OS_AirLoopHVAC_UnitarySystem :
    {
      model::AirLoopHVACUnitarySystem unitary = modelObject.cast<AirLoopHVACUnitarySystem>();
      retVal = translateAirLoopHVACUnitarySystem(unitary);
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
  case openstudio::IddObjectType::OS_AvailabilityManager_HybridVentilation :
    {
      auto mo = modelObject.cast<AvailabilityManagerHybridVentilation>();
      retVal = translateAvailabilityManagerHybridVentilation(mo);
      break;
    }
  case openstudio::IddObjectType::OS_AvailabilityManager_OptimumStart :
    {
      auto mo = modelObject.cast<AvailabilityManagerOptimumStart>();
      retVal = translateAvailabilityManagerOptimumStart(mo);
      break;
    }
  case openstudio::IddObjectType::OS_AvailabilityManager_DifferentialThermostat :
    {
      auto mo = modelObject.cast<AvailabilityManagerDifferentialThermostat>();
      retVal = translateAvailabilityManagerDifferentialThermostat(mo);
      break;
    }
  case openstudio::IddObjectType::OS_AvailabilityManager_NightVentilation :
    {
      auto mo = modelObject.cast<AvailabilityManagerNightVentilation>();
      retVal = translateAvailabilityManagerNightVentilation(mo);
      break;
    }
  case openstudio::IddObjectType::OS_AvailabilityManager_NightCycle :
    {
      auto mo = modelObject.cast<AvailabilityManagerNightCycle>();
      retVal = translateAvailabilityManagerNightCycle(mo);
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
  case openstudio::IddObjectType::OS_CentralHeatPumpSystem :
    {
      model::CentralHeatPumpSystem mo = modelObject.cast<CentralHeatPumpSystem>();
      retVal = translateCentralHeatPumpSystem(mo);
      break;
    }
  case openstudio::IddObjectType::OS_CentralHeatPumpSystem_Module :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_Chiller_Absorption :
    {
      auto mo = modelObject.cast<ChillerAbsorption>();
      retVal = translateChillerAbsorption(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Chiller_Absorption_Indirect :
    {
      auto mo = modelObject.cast<ChillerAbsorptionIndirect>();
      retVal = translateChillerAbsorptionIndirect(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Chiller_Electric_EIR :
    {
      model::ChillerElectricEIR chiller = modelObject.cast<ChillerElectricEIR>();
      retVal = translateChillerElectricEIR(chiller);
      break;
    }
  case openstudio::IddObjectType::OS_ChillerHeaterPerformance_Electric_EIR :
    {
      model::ChillerHeaterPerformanceElectricEIR mo = modelObject.cast<ChillerHeaterPerformanceElectricEIR>();
      retVal = translateChillerHeaterPerformanceElectricEIR(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ClimateZones:
  {
    // no-op
    return retVal;
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
  case openstudio::IddObjectType::OS_Coil_Cooling_CooledBeam:
    {
      // DLM: is this a no-op?
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_DX_SingleSpeed :
    {
      model::CoilCoolingDXSingleSpeed coil = modelObject.cast<CoilCoolingDXSingleSpeed>();
      if( this->isHVACComponentWithinUnitary(coil) ) {
        retVal = translateCoilCoolingDXSingleSpeedWithoutUnitary(coil);
      } else {
        retVal = translateCoilCoolingDXSingleSpeed(coil);
      }
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_DX_MultiSpeed :
    {
      model::CoilCoolingDXMultiSpeed coil = modelObject.cast<CoilCoolingDXMultiSpeed>();
      retVal = translateCoilCoolingDXMultiSpeed(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_DX_MultiSpeed_StageData :
    {
      return retVal;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoSpeed :
    {
      model::CoilCoolingDXTwoSpeed coil = modelObject.cast<CoilCoolingDXTwoSpeed>();
      if( this->isHVACComponentWithinUnitary(coil) ) {
        retVal = translateCoilCoolingDXTwoSpeedWithoutUnitary(coil);
      } else {
        retVal = translateCoilCoolingDXTwoSpeed(coil);
      }
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_DX_TwoStageWithHumidityControlMode :
    {
      auto coil = modelObject.cast<CoilCoolingDXTwoStageWithHumidityControlMode>();
      if( this->isHVACComponentWithinUnitary(coil) ) {
        retVal = translateCoilCoolingDXTwoStageWithHumidityControlModeWithoutUnitary(coil);
      } else {
        retVal = translateCoilCoolingDXTwoStageWithHumidityControlMode(coil);
      }
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_DX_VariableRefrigerantFlow :
    {
      model::CoilCoolingDXVariableRefrigerantFlow coil = modelObject.cast<CoilCoolingDXVariableRefrigerantFlow>();
      retVal = translateCoilCoolingDXVariableRefrigerantFlow(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_DX_VariableSpeed :
    {
      model::CoilCoolingDXVariableSpeed coil = modelObject.cast<CoilCoolingDXVariableSpeed>();
      if( this->isHVACComponentWithinUnitary(coil) ) {
        retVal = translateCoilCoolingDXVariableSpeedWithoutUnitary(coil);
      } else {
        retVal = translateCoilCoolingDXVariableSpeed(coil);
      }
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_DX_VariableSpeed_SpeedData :
    {
      // DLM: is this a no-op?
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
  case openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit :
    {
      model::CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit coil = modelObject.cast<CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit>();
      retVal = translateCoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Cooling_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData:
    {
      // DLM: is this a no-op?
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_Desuperheater :
    {
      model::CoilHeatingDesuperheater coil = modelObject.cast<CoilHeatingDesuperheater>();
      retVal = translateCoilHeatingDesuperheater(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_DX_SingleSpeed :
    {
      model::CoilHeatingDXSingleSpeed coil = modelObject.cast<CoilHeatingDXSingleSpeed>();
      if( this->isHVACComponentWithinUnitary(coil) ) {
        retVal = translateCoilHeatingDXSingleSpeedWithoutUnitary(coil);
      } else {
        retVal = translateCoilHeatingDXSingleSpeed(coil);
      }
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_DX_MultiSpeed :
    {
      model::CoilHeatingDXMultiSpeed coil = modelObject.cast<CoilHeatingDXMultiSpeed>();
      retVal = translateCoilHeatingDXMultiSpeed(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_DX_MultiSpeed_StageData:
    {
      //DLM: is this a no-op?
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_DX_VariableSpeed :
    {
      model::CoilHeatingDXVariableSpeed coil = modelObject.cast<CoilHeatingDXVariableSpeed>();
      if( this->isHVACComponentWithinUnitary(coil) ) {
        retVal = translateCoilHeatingDXVariableSpeedWithoutUnitary(coil);
      } else {
        retVal = translateCoilHeatingDXVariableSpeed(coil);
      }
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_DX_VariableSpeed_SpeedData:
    {
      //DLM: is this a no-op?
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
  case openstudio::IddObjectType::OS_Coil_Heating_Gas_MultiStage :
    {
      model::CoilHeatingGasMultiStage coil = modelObject.cast<CoilHeatingGasMultiStage>();
      retVal = translateCoilHeatingGasMultiStage(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_Gas_MultiStage_StageData :
    {
      // no-op
      return retVal;
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
  case openstudio::IddObjectType::OS_Coil_Heating_DX_VariableRefrigerantFlow :
    {
      model::CoilHeatingDXVariableRefrigerantFlow coil = modelObject.cast<CoilHeatingDXVariableRefrigerantFlow>();
      retVal = translateCoilHeatingDXVariableRefrigerantFlow(coil);
      break;
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
  case openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit :
    {
      model::CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit coil = modelObject.cast<CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit>();
      retVal = translateCoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_VariableSpeedEquationFit_SpeedData:
    {
      // DLM: is this a no-op?
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_Water_Baseboard:
    {
      // DLM: is this a no-op?
      break;
    }
  case openstudio::IddObjectType::OS_Coil_Heating_Water_Baseboard_Radiant:
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_CoilPerformance_DX_Cooling :
    {
      auto mo = modelObject.cast<CoilPerformanceDXCooling>();
      retVal = translateCoilPerformanceDXCooling(mo);
      break;
    }
  case openstudio::IddObjectType::OS_CoilSystem_Cooling_Water_HeatExchangerAssisted :
    {
      auto mo = modelObject.cast<CoilSystemCoolingWaterHeatExchangerAssisted>();
      retVal = translateCoilSystemCoolingWaterHeatExchangerAssisted(mo);
      break;
    }
  case openstudio::IddObjectType::OS_CoilSystem_Cooling_DX_HeatExchangerAssisted :
    {
      auto mo = modelObject.cast<CoilSystemCoolingDXHeatExchangerAssisted>();
      retVal = translateCoilSystemCoolingDXHeatExchangerAssisted(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_WaterHeating_Desuperheater :
    {
      model::CoilWaterHeatingDesuperheater coil = modelObject.cast<CoilWaterHeatingDesuperheater>();
      retVal = translateCoilWaterHeatingDesuperheater(coil);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_WaterHeating_AirToWaterHeatPump :
    {
      auto mo = modelObject.cast<CoilWaterHeatingAirToWaterHeatPump>();
      retVal = translateCoilWaterHeatingAirToWaterHeatPump(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Coil_WaterHeating_AirToWaterHeatPump_Wrapped :
    {
      auto mo = modelObject.cast<CoilWaterHeatingAirToWaterHeatPumpWrapped>();
      retVal = translateCoilWaterHeatingAirToWaterHeatPumpWrapped(mo);
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
  case openstudio::IddObjectType::OS_Construction_WindowDataFile:
    {
      LOG(Warn, "OS_WindowDataFile is not currently translated");
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
  case openstudio::IddObjectType::OS_CoolingTowerPerformance_CoolTools :
    {
      model::CoolingTowerPerformanceCoolTools mo = modelObject.cast<CoolingTowerPerformanceCoolTools>();
      retVal = translateCoolingTowerPerformanceCoolTools(mo);
      break;
    }
  case openstudio::IddObjectType::OS_CoolingTowerPerformance_YorkCalc :
    {
      model::CoolingTowerPerformanceYorkCalc mo = modelObject.cast<CoolingTowerPerformanceYorkCalc>();
      retVal = translateCoolingTowerPerformanceYorkCalc(mo);
      break;
    }
  case openstudio::IddObjectType::OS_CoolingTower_SingleSpeed :
    {
      model::CoolingTowerSingleSpeed tower = modelObject.cast<CoolingTowerSingleSpeed>();
      retVal = translateCoolingTowerSingleSpeed(tower);
      break;
    }
  case openstudio::IddObjectType::OS_CoolingTower_TwoSpeed :
    {
      model::CoolingTowerTwoSpeed tower = modelObject.cast<CoolingTowerTwoSpeed>();
      retVal = translateCoolingTowerTwoSpeed(tower);
      break;
    }
  case openstudio::IddObjectType::OS_CoolingTower_VariableSpeed :
    {
      model::CoolingTowerVariableSpeed tower = modelObject.cast<CoolingTowerVariableSpeed>();
      retVal = translateCoolingTowerVariableSpeed(tower);
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
  case openstudio::IddObjectType::OS_SizingPeriod_WeatherFileConditionType:
    {
      LOG(Warn, "OS_SizingPeriod_WeatherFileConditionType is not currently translated");
      return retVal;
    }
  case openstudio::IddObjectType::OS_SizingPeriod_WeatherFileDays:
    {
      LOG(Warn, "OS_SizingPeriod_WeatherFileDays is not currently translated");
      return retVal;
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
  case openstudio::IddObjectType::OS_DesignSpecification_ZoneAirDistribution:
    {
      // DLM: appears to be translated in SizingZone
      break;
    }
  case openstudio::IddObjectType::OS_DistrictCooling :
    {
      model::DistrictCooling districtCooling = modelObject.cast<DistrictCooling>();
      retVal = translateDistrictCooling(districtCooling);
      break;
    }
  case openstudio::IddObjectType::OS_DistrictHeating:
  {
    model::DistrictHeating districtHeating = modelObject.cast<DistrictHeating>();
    retVal = translateDistrictHeating(districtHeating);
    break;
  }
  case openstudio::IddObjectType::OS_Duct:
  {
    model::Duct duct = modelObject.cast<Duct>();
    retVal = translateDuct(duct);
    break;
  }
  case openstudio::IddObjectType::OS_ElectricEquipment :
    {
      model::ElectricEquipment equipment = modelObject.cast<ElectricEquipment>();
      retVal = translateElectricEquipment(equipment);
      break;
    }
  case openstudio::IddObjectType::OS_ElectricEquipment_Definition:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_ElectricLoadCenter_Distribution:
  {
    model::ElectricLoadCenterDistribution temp = modelObject.cast<ElectricLoadCenterDistribution>();
    retVal = translateElectricLoadCenterDistribution(temp);
    break;
  }
  case openstudio::IddObjectType::OS_ElectricLoadCenter_Inverter_LookUpTable:
  {
    model::ElectricLoadCenterInverterLookUpTable temp = modelObject.cast<ElectricLoadCenterInverterLookUpTable>();
    retVal = translateElectricLoadCenterInverterLookUpTable(temp);
    break;
  }
  case openstudio::IddObjectType::OS_ElectricLoadCenter_Inverter_Simple:
  {
    model::ElectricLoadCenterInverterSimple temp = modelObject.cast<ElectricLoadCenterInverterSimple>();
    retVal = translateElectricLoadCenterInverterSimple(temp);
    break;
  }
  case openstudio::IddObjectType::OS_ElectricLoadCenter_Storage_Simple:
  {
    model::ElectricLoadCenterStorageSimple temp = modelObject.cast<ElectricLoadCenterStorageSimple>();
    retVal = translateElectricLoadCenterStorageSimple(temp);
    break;
  }
  case openstudio::IddObjectType::OS_ElectricLoadCenter_Storage_Converter:
  {
    model::ElectricLoadCenterStorageConverter temp = modelObject.cast<ElectricLoadCenterStorageConverter>();
    retVal = translateElectricLoadCenterStorageConverter(temp);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_Actuator:
  {
    model::EnergyManagementSystemActuator actuator = modelObject.cast<EnergyManagementSystemActuator>();
    retVal = translateEnergyManagementSystemActuator(actuator);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_ConstructionIndexVariable:
  {
    model::EnergyManagementSystemConstructionIndexVariable civ = modelObject.cast<EnergyManagementSystemConstructionIndexVariable>();
    retVal = translateEnergyManagementSystemConstructionIndexVariable(civ);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_CurveOrTableIndexVariable:
  {
    model::EnergyManagementSystemCurveOrTableIndexVariable cotiv = modelObject.cast<EnergyManagementSystemCurveOrTableIndexVariable>();
    retVal = translateEnergyManagementSystemCurveOrTableIndexVariable(cotiv);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_GlobalVariable:
  {
    model::EnergyManagementSystemGlobalVariable globalVariable = modelObject.cast<EnergyManagementSystemGlobalVariable>();
    retVal = translateEnergyManagementSystemGlobalVariable(globalVariable);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_InternalVariable:
  {
    model::EnergyManagementSystemInternalVariable internalVariable = modelObject.cast<EnergyManagementSystemInternalVariable>();
    retVal = translateEnergyManagementSystemInternalVariable(internalVariable);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_MeteredOutputVariable:
  {
    model::EnergyManagementSystemMeteredOutputVariable mov = modelObject.cast<EnergyManagementSystemMeteredOutputVariable>();
    retVal = translateEnergyManagementSystemMeteredOutputVariable(mov);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_OutputVariable:
  {
    model::EnergyManagementSystemOutputVariable outputVariable = modelObject.cast<EnergyManagementSystemOutputVariable>();
    retVal = translateEnergyManagementSystemOutputVariable(outputVariable);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_Program:
  {
    model::EnergyManagementSystemProgram program = modelObject.cast<EnergyManagementSystemProgram>();
    retVal = translateEnergyManagementSystemProgram(program);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_ProgramCallingManager:
  {
    model::EnergyManagementSystemProgramCallingManager programCallingManager = modelObject.cast<EnergyManagementSystemProgramCallingManager>();
    retVal = translateEnergyManagementSystemProgramCallingManager(programCallingManager);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_Sensor:
  {
    model::EnergyManagementSystemSensor sensor = modelObject.cast<EnergyManagementSystemSensor>();
    retVal = translateEnergyManagementSystemSensor(sensor);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_Subroutine:
  {
    model::EnergyManagementSystemSubroutine subroutine = modelObject.cast<EnergyManagementSystemSubroutine>();
    retVal = translateEnergyManagementSystemSubroutine(subroutine);
    break;
  }
  case openstudio::IddObjectType::OS_EnergyManagementSystem_TrendVariable:
  {
    model::EnergyManagementSystemTrendVariable trendVariable = modelObject.cast<EnergyManagementSystemTrendVariable>();
    retVal = translateEnergyManagementSystemTrendVariable(trendVariable);
    break;
  }
  case openstudio::IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial :
    {
      model::EvaporativeCoolerDirectResearchSpecial evap = modelObject.cast<EvaporativeCoolerDirectResearchSpecial>();
      retVal = translateEvaporativeCoolerDirectResearchSpecial(evap);
      break;
    }
  case openstudio::IddObjectType::OS_EvaporativeCooler_Indirect_ResearchSpecial :
    {
      model::EvaporativeCoolerIndirectResearchSpecial evap = modelObject.cast<EvaporativeCoolerIndirectResearchSpecial>();
      retVal = translateEvaporativeCoolerIndirectResearchSpecial(evap);
      break;
    }
  case openstudio::IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed :
    {
      model::EvaporativeFluidCoolerSingleSpeed evap = modelObject.cast<EvaporativeFluidCoolerSingleSpeed>();
      retVal = translateEvaporativeFluidCoolerSingleSpeed(evap);
      break;
    }
  case openstudio::IddObjectType::OS_EvaporativeFluidCooler_TwoSpeed :
    {
      model::EvaporativeFluidCoolerTwoSpeed mo = modelObject.cast<EvaporativeFluidCoolerTwoSpeed>();
      retVal = translateEvaporativeFluidCoolerTwoSpeed(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Exterior_Lights :
    {
      model::ExteriorLights lights = modelObject.cast<ExteriorLights>();
      retVal = translateExteriorLights(lights);
      break;
    }
  case openstudio::IddObjectType::OS_Exterior_Lights_Definition:
    {
      // no-op
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

  case openstudio::IddObjectType::OS_Fan_ZoneExhaust :
    {
      model::FanZoneExhaust fan = modelObject.cast<FanZoneExhaust>();
      retVal = translateFanZoneExhaust(fan);
      break;
    }
  case openstudio::IddObjectType::OS_FluidCooler_SingleSpeed:
  {
    model::FluidCoolerSingleSpeed fluidCoolerSingleSpeed = modelObject.cast<FluidCoolerSingleSpeed>();
    retVal = translateFluidCoolerSingleSpeed(fluidCoolerSingleSpeed);
    break;
  }
  case openstudio::IddObjectType::OS_FluidCooler_TwoSpeed:
  {
    model::FluidCoolerTwoSpeed fluidCoolerTwoSpeed = modelObject.cast<FluidCoolerTwoSpeed>();
    retVal = translateFluidCoolerTwoSpeed(fluidCoolerTwoSpeed);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_MicroTurbine:
  {
    // Will also translate the Generator:MicroTurbine:HeatRecovery if there is one attached to the Generator:MicroTurbine
    model::GeneratorMicroTurbine mchp = modelObject.cast<GeneratorMicroTurbine>();
    retVal = translateGeneratorMicroTurbine(mchp);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_MicroTurbine_HeatRecovery:
  {
    // no-op, just Log a Trace message
    LOG(Trace, "OS_Generator_MicroTurbine_HeatRecovery is not translated by itself but in the parent GeneratorMicroTurbine");
    break;
  }
  case openstudio::IddObjectType::OS_Generator_FuelCell:
  {
    // Will also translate the Generator:FuelCell if there is one
    model::GeneratorFuelCell temp = modelObject.cast<GeneratorFuelCell>();
    retVal = translateGeneratorFuelCell(temp);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_FuelCell_AirSupply:
  {
    // Will also translate the Generator:FuelCell:AirSupply if there is one
    model::GeneratorFuelCellAirSupply temp = modelObject.cast<GeneratorFuelCellAirSupply>();
    retVal = translateGeneratorFuelCellAirSupply(temp);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_FuelCell_AuxiliaryHeater:
  {
    // Will also translate the Generator:FuelCell:AuxiliaryHeater if there is one
    model::GeneratorFuelCellAuxiliaryHeater temp = modelObject.cast<GeneratorFuelCellAuxiliaryHeater>();
    retVal = translateGeneratorFuelCellAuxiliaryHeater(temp);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_FuelCell_ElectricalStorage:
  {
    // Will also translate the Generator:FuelCell:ElectricalStorage if there is one
    model::GeneratorFuelCellElectricalStorage temp = modelObject.cast<GeneratorFuelCellElectricalStorage>();
    retVal = translateGeneratorFuelCellElectricalStorage(temp);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_FuelCell_ExhaustGasToWaterHeatExchanger:
  {
    // Will also translate the Generator:FuelCell:ExhaustGasToWaterHeatExchanger if there is one
    model::GeneratorFuelCellExhaustGasToWaterHeatExchanger temp = modelObject.cast<GeneratorFuelCellExhaustGasToWaterHeatExchanger>();
    retVal = translateGeneratorFuelCellExhaustGasToWaterHeatExchanger(temp);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_FuelCell_Inverter:
  {
    // Will also translate the Generator:FuelCell:Inverter if there is one
    model::GeneratorFuelCellInverter temp = modelObject.cast<GeneratorFuelCellInverter>();
    retVal = translateGeneratorFuelCellInverter(temp);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_FuelCell_PowerModule:
  {
    // Will also translate the Generator:FuelCell:PowerModule if there is one
    model::GeneratorFuelCellPowerModule temp = modelObject.cast<GeneratorFuelCellPowerModule>();
    retVal = translateGeneratorFuelCellPowerModule(temp);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_FuelCell_StackCooler:
  {
    // Will also translate the Generator:FuelCell:StackCooler if there is one
    model::GeneratorFuelCellStackCooler temp = modelObject.cast<GeneratorFuelCellStackCooler>();
    retVal = translateGeneratorFuelCellStackCooler(temp);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_FuelCell_WaterSupply:
  {
    // Will also translate the Generator:FuelCell:WaterSupply if there is one
    model::GeneratorFuelCellWaterSupply temp = modelObject.cast<GeneratorFuelCellWaterSupply>();
    retVal = translateGeneratorFuelCellWaterSupply(temp);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_FuelSupply:
  {
    // Will also translate the Generator:FuelSupply if there is one
    model::GeneratorFuelSupply temp = modelObject.cast<GeneratorFuelSupply>();
    retVal = translateGeneratorFuelSupply(temp);
    break;
  }
  case openstudio::IddObjectType::OS_Generator_Photovoltaic:
  {
    model::GeneratorPhotovoltaic temp = modelObject.cast<GeneratorPhotovoltaic>();
    retVal = translateGeneratorPhotovoltaic(temp);
    break;
  }
  case openstudio::IddObjectType::OS_Glare_Sensor:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_GroundHeatExchanger_HorizontalTrench :
    {
      auto mo = modelObject.cast<GroundHeatExchangerHorizontalTrench>();
      retVal = translateGroundHeatExchangerHorizontalTrench(mo);
      break;
    }
  case openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical :
    {
      model::GroundHeatExchangerVertical groundHeatExchangerVertical = modelObject.cast<GroundHeatExchangerVertical>();
      retVal = translateGroundHeatExchangerVertical(groundHeatExchangerVertical);
      break;
    }
  case openstudio::IddObjectType::OS_HeaderedPumps_ConstantSpeed :
    {
      auto mo = modelObject.cast<HeaderedPumpsConstantSpeed>();
      retVal = translateHeaderedPumpsConstantSpeed(mo);
      break;
    }
  case openstudio::IddObjectType::OS_HeaderedPumps_VariableSpeed :
    {
      auto mo = modelObject.cast<HeaderedPumpsVariableSpeed>();
      retVal = translateHeaderedPumpsVariableSpeed(mo);
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
  case openstudio::IddObjectType::OS_HeatExchanger_FluidToFluid :
    {
      model::HeatExchangerFluidToFluid mo = modelObject.cast<HeatExchangerFluidToFluid>();
      retVal = translateHeatExchangerFluidToFluid(mo);
      break;
    }
  case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling :
    {
      auto mo = modelObject.cast<HeatPumpWaterToWaterEquationFitCooling>();
      retVal = translateHeatPumpWaterToWaterEquationFitCooling(mo);
      break;
    }
  case openstudio::IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating :
    {
      auto mo = modelObject.cast<HeatPumpWaterToWaterEquationFitHeating>();
      retVal = translateHeatPumpWaterToWaterEquationFitHeating(mo);
      break;
    }
  case openstudio::IddObjectType::OS_HotWaterEquipment :
    {
      model::HotWaterEquipment hotWaterEquipment = modelObject.cast<HotWaterEquipment>();
      retVal = translateHotWaterEquipment(hotWaterEquipment);
      break;
    }
  case openstudio::IddObjectType::OS_HotWaterEquipment_Definition:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_Humidifier_Steam_Electric :
    {
      model::HumidifierSteamElectric humidifierSteamElectric = modelObject.cast<HumidifierSteamElectric>();
      retVal = translateHumidifierSteamElectric(humidifierSteamElectric);
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
  case openstudio::IddObjectType::OS_GasEquipment_Definition:
    {
      // no-op
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
  case openstudio::IddObjectType::OS_InternalMass_Definition:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_WaterHeater_Mixed :
    {
      model::WaterHeaterMixed waterHeaterMixed = modelObject.cast<WaterHeaterMixed>();
      retVal = translateWaterHeaterMixed(waterHeaterMixed);
      break;
    }
  case openstudio::IddObjectType::OS_WaterHeater_HeatPump :
    {
      auto mo = modelObject.cast<WaterHeaterHeatPump>();
      retVal = translateWaterHeaterHeatPump(mo);
      break;
    }
  case openstudio::IddObjectType::OS_WaterHeater_HeatPump_WrappedCondenser :
    {
      auto mo = modelObject.cast<WaterHeaterHeatPumpWrappedCondenser>();
      retVal = translateWaterHeaterHeatPumpWrappedCondenser(mo);
      break;
    }
  case openstudio::IddObjectType::OS_WaterHeater_Stratified :
    {
      model::WaterHeaterStratified waterHeaterStratified = modelObject.cast<WaterHeaterStratified>();
      retVal = translateWaterHeaterStratified(waterHeaterStratified);
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
  case openstudio::IddObjectType::OS_WaterUse_Equipment_Definition:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_WindowMaterial_GasMixture :
    {
      model::GasMixture gasMixture = modelObject.cast<GasMixture>();
      retVal = translateGasMixture(gasMixture);
      break;
    }
  case openstudio::IddObjectType::OS_LoadProfile_Plant :
    {
      model::LoadProfilePlant mo = modelObject.cast<LoadProfilePlant>();
      retVal = translateLoadProfilePlant(mo);
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
  case openstudio::IddObjectType::OS_LightingDesignDay:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_LightingSimulationControl:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_LightingSimulationZone:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_Lights :
    {
      model::Lights lights = modelObject.cast<Lights>();
      retVal = translateLights(lights);
      break;
    }
  case openstudio::IddObjectType::OS_Lights_Definition:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_Luminaire :
    {
      model::Luminaire luminaire = modelObject.cast<Luminaire>();
      retVal = translateLuminaire(luminaire);
      break;
    }
  case openstudio::IddObjectType::OS_Luminaire_Definition:
    {
      // no-op
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
  case openstudio::IddObjectType::OS_MaterialProperty_GlazingSpectralData :
    {
      model::MaterialPropertyGlazingSpectralData spectralData = modelObject.cast<MaterialPropertyGlazingSpectralData>();
      retVal = translateMaterialPropertyGlazingSpectralData(spectralData);
      break;
    }
  case openstudio::IddObjectType::OS_MaterialProperty_MoisturePenetrationDepth_Settings :
    {
      model::MaterialPropertyMoisturePenetrationDepthSettings empd = modelObject.cast<MaterialPropertyMoisturePenetrationDepthSettings>();
      retVal = translateMaterialPropertyMoisturePenetrationDepthSettings(empd);
      break;
    }    
  case openstudio::IddObjectType::OS_Material_RoofVegetation :
    {
      model::RoofVegetation material = modelObject.cast<RoofVegetation>();
      retVal = translateRoofVegetation(material);
      break;
    }
  case openstudio::IddObjectType::OS_Meter_Custom :
    {
      model::MeterCustom meterCustom = modelObject.cast<MeterCustom>();
      retVal = translateMeterCustom(meterCustom);
      break;
    }
  case openstudio::IddObjectType::OS_Meter_CustomDecrement :
    {
      model::MeterCustomDecrement meterCustomDecrement = modelObject.cast<MeterCustomDecrement>();
      retVal = translateMeterCustomDecrement(meterCustomDecrement);
      break;
    }
  case openstudio::IddObjectType::OS_ModelObjectList :
    {
      // no-op
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
  case openstudio::IddObjectType::OS_PhotovoltaicPerformance_EquivalentOneDiode:
  {
    PhotovoltaicPerformanceEquivalentOneDiode temp = modelObject.cast<PhotovoltaicPerformanceEquivalentOneDiode>();
    retVal = translatePhotovoltaicPerformanceEquivalentOneDiode(temp);
    break;
  }
  case openstudio::IddObjectType::OS_PhotovoltaicPerformance_Simple:
  {
    PhotovoltaicPerformanceSimple temp = modelObject.cast<PhotovoltaicPerformanceSimple>();
    retVal = translatePhotovoltaicPerformanceSimple(temp);
    break;
  }
  case openstudio::IddObjectType::OS_OtherEquipment_Definition:
    {
      //no-op
      break;
    }
  case openstudio::IddObjectType::OS_PlantLoop :
    {
      PlantLoop plantLoop = modelObject.cast<PlantLoop>();
      retVal = translatePlantLoop(plantLoop);
      break;
    }
  case openstudio::IddObjectType::OS_PlantComponent_TemperatureSource :
    {
      model::PlantComponentTemperatureSource mo = modelObject.cast<PlantComponentTemperatureSource>();
      retVal = translatePlantComponentTemperatureSource(mo);
      break;
    }
  case openstudio::IddObjectType::OS_PlantEquipmentOperation_CoolingLoad :
    {
      auto mo = modelObject.cast<PlantEquipmentOperationCoolingLoad>();
      retVal = translatePlantEquipmentOperationCoolingLoad(mo);
      break;
    }
  case openstudio::IddObjectType::OS_PlantEquipmentOperation_HeatingLoad :
    {
      auto mo = modelObject.cast<PlantEquipmentOperationHeatingLoad>();
      retVal = translatePlantEquipmentOperationHeatingLoad(mo);
      break;
    }
  case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorDryBulb :
    {
      auto mo = modelObject.cast<PlantEquipmentOperationOutdoorDryBulb>();
      retVal = translatePlantEquipmentOperationOutdoorDryBulb(mo);
      break;
    }
  case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorWetBulb :
    {
      auto mo = modelObject.cast<PlantEquipmentOperationOutdoorWetBulb>();
      retVal = translatePlantEquipmentOperationOutdoorWetBulb(mo);
      break;
    }
  case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorDewpoint :
    {
      auto mo = modelObject.cast<PlantEquipmentOperationOutdoorDewpoint>();
      retVal = translatePlantEquipmentOperationOutdoorDewpoint(mo);
      break;
    }
  case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorDryBulbDifference :
    {
      auto mo = modelObject.cast<PlantEquipmentOperationOutdoorDryBulbDifference>();
      retVal = translatePlantEquipmentOperationOutdoorDryBulbDifference(mo);
      break;
    }
  case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorWetBulbDifference :
    {
      auto mo = modelObject.cast<PlantEquipmentOperationOutdoorWetBulbDifference>();
      retVal = translatePlantEquipmentOperationOutdoorWetBulbDifference(mo);
      break;
    }
  case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorDewpointDifference :
    {
      auto mo = modelObject.cast<PlantEquipmentOperationOutdoorDewpointDifference>();
      retVal = translatePlantEquipmentOperationOutdoorDewpointDifference(mo);
      break;
    }
  case openstudio::IddObjectType::OS_PlantEquipmentOperation_OutdoorRelativeHumidity :
    {
      auto mo = modelObject.cast<PlantEquipmentOperationOutdoorRelativeHumidity>();
      retVal = translatePlantEquipmentOperationOutdoorRelativeHumidity(mo);
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
  case openstudio::IddObjectType::OS_Output_Meter :
    {
      model::OutputMeter meter = modelObject.cast<OutputMeter>();
      retVal = translateOutputMeter(meter);
      break;
    }
  case openstudio::IddObjectType::OS_Output_Variable :
    {
      model::OutputVariable outputVariable = modelObject.cast<OutputVariable>();
      retVal = translateOutputVariable(outputVariable);
      break;
    }
  case openstudio::IddObjectType::OS_Output_EnergyManagementSystem:
  {
    model::OutputEnergyManagementSystem outputEnergyManagementSystem = modelObject.cast<OutputEnergyManagementSystem>();
    retVal = translateOutputEnergyManagementSystem(outputEnergyManagementSystem);
    break;
  }
  case openstudio::IddObjectType::OS_People :
    {
      model::People people = modelObject.cast<People>();
      retVal = translatePeople(people);
      break;
    }
  case openstudio::IddObjectType::OS_People_Definition:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_Pipe_Adiabatic:
  {
    model::PipeAdiabatic pipe = modelObject.cast<PipeAdiabatic>();
    retVal = translatePipeAdiabatic(pipe);
    break;
  }
  case openstudio::IddObjectType::OS_Pipe_Indoor:
  {
    model::PipeIndoor pipe = modelObject.cast<PipeIndoor>();
    retVal = translatePipeIndoor(pipe);
    break;
  }
  case openstudio::IddObjectType::OS_Pipe_Outdoor:
  {
    model::PipeOutdoor pipe = modelObject.cast<PipeOutdoor>();
    retVal = translatePipeOutdoor(pipe);
    break;
  }
  case openstudio::IddObjectType::OS_PortList :
    {
      model::PortList portList = modelObject.cast<PortList>();
      retVal = translatePortList(portList);
      break;
    }
  case openstudio::IddObjectType::OS_ProgramControl:
    {
      LOG(Warn, "OS_ProgramControl is not currently translated");
      break;
    }
  case openstudio::IddObjectType::OS_RadianceParameters:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_AirChiller :
    {
      model::RefrigerationAirChiller refrigerationAirChiller = modelObject.cast<RefrigerationAirChiller>();
      retVal = translateRefrigerationAirChiller(refrigerationAirChiller);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_Case :
    {
      model::RefrigerationCase refrigerationCase = modelObject.cast<RefrigerationCase>();
      retVal = translateRefrigerationCase(refrigerationCase);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_Compressor :
    {
      model::RefrigerationCompressor refrigerationCompressor = modelObject.cast<RefrigerationCompressor>();
      retVal = translateRefrigerationCompressor(refrigerationCompressor);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_Condenser_AirCooled :
    {
      model::RefrigerationCondenserAirCooled refrigerationCondenserAirCooled = modelObject.cast<RefrigerationCondenserAirCooled>();
      retVal = translateRefrigerationCondenserAirCooled(refrigerationCondenserAirCooled);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_Condenser_Cascade:
    {
      model::RefrigerationCondenserCascade refrigerationCondenserCascade = modelObject.cast<RefrigerationCondenserCascade>();
      retVal = translateRefrigerationCondenserCascade(refrigerationCondenserCascade);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_Condenser_EvaporativeCooled :
    {
      model::RefrigerationCondenserEvaporativeCooled refrigerationCondenserEvaporativeCooled = modelObject.cast<RefrigerationCondenserEvaporativeCooled>();
      retVal = translateRefrigerationCondenserEvaporativeCooled(refrigerationCondenserEvaporativeCooled);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_Condenser_WaterCooled:
    {
      model::RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = modelObject.cast<RefrigerationCondenserWaterCooled>();
      retVal = translateRefrigerationCondenserWaterCooled(refrigerationCondenserWaterCooled);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_DefrostCycleParameters:
    {
      // DLM: is this a no-op?
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_GasCooler_AirCooled:
    {
      model::RefrigerationGasCoolerAirCooled refrigerationGasCoolerAirCooled = modelObject.cast<RefrigerationGasCoolerAirCooled>();
      retVal = translateRefrigerationGasCoolerAirCooled(refrigerationGasCoolerAirCooled);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_Subcooler_LiquidSuction:
    {
      model::RefrigerationSubcoolerLiquidSuction refrigerationSubcoolerLiquidSuction = modelObject.cast<RefrigerationSubcoolerLiquidSuction>();
      retVal = translateRefrigerationSubcoolerLiquidSuction(refrigerationSubcoolerLiquidSuction);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_Subcooler_Mechanical:
    {
      model::RefrigerationSubcoolerMechanical refrigerationSubcoolerMechanical = modelObject.cast<RefrigerationSubcoolerMechanical>();
      retVal = translateRefrigerationSubcoolerMechanical(refrigerationSubcoolerMechanical);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_SecondarySystem :
    {
      model::RefrigerationSecondarySystem refrigerationSecondarySystem = modelObject.cast<RefrigerationSecondarySystem>();
      retVal = translateRefrigerationSecondarySystem(refrigerationSecondarySystem);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_System :
    {
      model::RefrigerationSystem refrigerationSystem = modelObject.cast<RefrigerationSystem>();
      retVal = translateRefrigerationSystem(refrigerationSystem);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_TranscriticalSystem :
    {
      model::RefrigerationTranscriticalSystem refrigerationTranscriticalSystem = modelObject.cast<RefrigerationTranscriticalSystem>();
      retVal = translateRefrigerationTranscriticalSystem(refrigerationTranscriticalSystem);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_WalkIn :
    {
      model::RefrigerationWalkIn refrigerationWalkIn = modelObject.cast<RefrigerationWalkIn>();
      retVal = translateRefrigerationWalkIn(refrigerationWalkIn);
      break;
    }
  case openstudio::IddObjectType::OS_Refrigeration_WalkIn_ZoneBoundary:
    {
      // DLM: is this a no-op
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
  case  openstudio::IddObjectType::OS_SetpointManager_Coldest :
    {
      model::SetpointManagerColdest spm = modelObject.cast<SetpointManagerColdest>();
      retVal = translateSetpointManagerColdest(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_FollowGroundTemperature :
    {
      model::SetpointManagerFollowGroundTemperature spm = modelObject.cast<SetpointManagerFollowGroundTemperature>();
      retVal = translateSetpointManagerFollowGroundTemperature(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_FollowOutdoorAirTemperature :
    {
      model::SetpointManagerFollowOutdoorAirTemperature spm = modelObject.cast<SetpointManagerFollowOutdoorAirTemperature>();
      retVal = translateSetpointManagerFollowOutdoorAirTemperature(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_FollowSystemNodeTemperature :
    {
      model::SetpointManagerFollowSystemNodeTemperature spm = modelObject.cast<SetpointManagerFollowSystemNodeTemperature>();
      retVal = translateSetpointManagerFollowSystemNodeTemperature(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_MixedAir :
    {
      model::SetpointManagerMixedAir spm = modelObject.cast<SetpointManagerMixedAir>();
      retVal = translateSetpointManagerMixedAir(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_MultiZone_Cooling_Average :
    {
      model::SetpointManagerMultiZoneCoolingAverage spm = modelObject.cast<SetpointManagerMultiZoneCoolingAverage>();
      retVal = translateSetpointManagerMultiZoneCoolingAverage(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_MultiZone_Heating_Average :
    {
      model::SetpointManagerMultiZoneHeatingAverage spm = modelObject.cast<SetpointManagerMultiZoneHeatingAverage>();
      retVal = translateSetpointManagerMultiZoneHeatingAverage(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_MultiZone_Humidity_Maximum :
    {
      model::SetpointManagerMultiZoneHumidityMaximum spm = modelObject.cast<SetpointManagerMultiZoneHumidityMaximum>();
      retVal = translateSetpointManagerMultiZoneHumidityMaximum(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_MultiZone_Humidity_Minimum :
    {
      model::SetpointManagerMultiZoneHumidityMinimum spm = modelObject.cast<SetpointManagerMultiZoneHumidityMinimum>();
      retVal = translateSetpointManagerMultiZoneHumidityMinimum(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_MultiZone_MaximumHumidity_Average :
    {
      model::SetpointManagerMultiZoneMaximumHumidityAverage spm = modelObject.cast<SetpointManagerMultiZoneMaximumHumidityAverage>();
      retVal = translateSetpointManagerMultiZoneMaximumHumidityAverage(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_MultiZone_MinimumHumidity_Average :
    {
      model::SetpointManagerMultiZoneMinimumHumidityAverage spm = modelObject.cast<SetpointManagerMultiZoneMinimumHumidityAverage>();
      retVal = translateSetpointManagerMultiZoneMinimumHumidityAverage(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_OutdoorAirPretreat :
    {
      model::SetpointManagerOutdoorAirPretreat spm = modelObject.cast<SetpointManagerOutdoorAirPretreat>();
      retVal = translateSetpointManagerOutdoorAirPretreat(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_OutdoorAirReset :
    {
      model::SetpointManagerOutdoorAirReset spm = modelObject.cast<SetpointManagerOutdoorAirReset>();
      retVal = translateSetpointManagerOutdoorAirReset(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_Scheduled :
    {
      model::SetpointManagerScheduled spm = modelObject.cast<SetpointManagerScheduled>();
      retVal = translateSetpointManagerScheduled(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_Scheduled_DualSetpoint :
    {
      model::SetpointManagerScheduledDualSetpoint spm = modelObject.cast<SetpointManagerScheduledDualSetpoint>();
      retVal = translateSetpointManagerScheduledDualSetpoint(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_SingleZone_Cooling :
    {
      model::SetpointManagerSingleZoneCooling spm = modelObject.cast<SetpointManagerSingleZoneCooling>();
      retVal = translateSetpointManagerSingleZoneCooling(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_SingleZone_Heating :
    {
      model::SetpointManagerSingleZoneHeating spm = modelObject.cast<SetpointManagerSingleZoneHeating>();
      retVal = translateSetpointManagerSingleZoneHeating(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_SingleZone_Humidity_Maximum :
    {
      model::SetpointManagerSingleZoneHumidityMaximum spm = modelObject.cast<SetpointManagerSingleZoneHumidityMaximum>();
      retVal = translateSetpointManagerSingleZoneHumidityMaximum(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_SingleZone_Humidity_Minimum :
    {
      model::SetpointManagerSingleZoneHumidityMinimum spm = modelObject.cast<SetpointManagerSingleZoneHumidityMinimum>();
      retVal = translateSetpointManagerSingleZoneHumidityMinimum(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_SingleZone_OneStageCooling :
    {
      model::SetpointManagerSingleZoneOneStageCooling spm = modelObject.cast<SetpointManagerSingleZoneOneStageCooling>();
      retVal = translateSetpointManagerSingleZoneOneStageCooling(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_SingleZone_OneStageHeating :
    {
      model::SetpointManagerSingleZoneOneStageHeating spm = modelObject.cast<SetpointManagerSingleZoneOneStageHeating>();
      retVal = translateSetpointManagerSingleZoneOneStageHeating(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_SingleZone_Reheat :
    {
      model::SetpointManagerSingleZoneReheat spm = modelObject.cast<SetpointManagerSingleZoneReheat>();
      retVal = translateSetpointManagerSingleZoneReheat(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_Warmest :
    {
      model::SetpointManagerWarmest spm = modelObject.cast<SetpointManagerWarmest>();
      retVal = translateSetpointManagerWarmest(spm);
      break;
    }
  case  openstudio::IddObjectType::OS_SetpointManager_WarmestTemperatureFlow :
    {
      model::SetpointManagerWarmestTemperatureFlow spm = modelObject.cast<SetpointManagerWarmestTemperatureFlow>();
      retVal = translateSetpointManagerWarmestTemperatureFlow(spm);
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
  case openstudio::IddObjectType::OS_Site_GroundTemperature_Deep :
    {
      model::SiteGroundTemperatureDeep mo = modelObject.cast<SiteGroundTemperatureDeep>();
      retVal = translateSiteGroundTemperatureDeep(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Site_GroundTemperature_FCfactorMethod :
    {
      model::SiteGroundTemperatureFCfactorMethod mo = modelObject.cast<SiteGroundTemperatureFCfactorMethod>();
      retVal = translateSiteGroundTemperatureFCfactorMethod(mo);
      break;
    }
  case openstudio::IddObjectType::OS_Site_GroundTemperature_Shallow :
    {
      model::SiteGroundTemperatureShallow mo = modelObject.cast<SiteGroundTemperatureShallow>();
      retVal = translateSiteGroundTemperatureShallow(mo);
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
  case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_PhotovoltaicThermal:
  {
    model::SolarCollectorFlatPlatePhotovoltaicThermal mo = modelObject.cast<SolarCollectorFlatPlatePhotovoltaicThermal>();
    retVal = translateSolarCollectorFlatPlatePhotovoltaicThermal(mo);
    break;
  }
  case openstudio::IddObjectType::OS_SolarCollector_FlatPlate_Water:
  {
    model::SolarCollectorFlatPlateWater mo = modelObject.cast<SolarCollectorFlatPlateWater>();
    retVal = translateSolarCollectorFlatPlateWater(mo);
    break;
  }

  case openstudio::IddObjectType::OS_SolarCollector_IntegralCollectorStorage:
  {
    model::SolarCollectorIntegralCollectorStorage mo = modelObject.cast<SolarCollectorIntegralCollectorStorage>();
    retVal = translateSolarCollectorIntegralCollectorStorage(mo);
    break;
  }
  case openstudio::IddObjectType::OS_SolarCollectorPerformance_FlatPlate :
  {
    model::SolarCollectorPerformanceFlatPlate mo = modelObject.cast<SolarCollectorPerformanceFlatPlate>();
    retVal = translateSolarCollectorPerformanceFlatPlate(mo);
    break;
  }
  case openstudio::IddObjectType::OS_SolarCollectorPerformance_IntegralCollectorStorage:
  {
    model::SolarCollectorPerformanceIntegralCollectorStorage mo = modelObject.cast<SolarCollectorPerformanceIntegralCollectorStorage>();
    retVal = translateSolarCollectorPerformanceIntegralCollectorStorage(mo);
    break;
  }
  case openstudio::IddObjectType::OS_SolarCollectorPerformance_PhotovoltaicThermal_Simple:
  {
    model::SolarCollectorPerformancePhotovoltaicThermalSimple mo = modelObject.cast<SolarCollectorPerformancePhotovoltaicThermalSimple>();
    retVal = translateSolarCollectorPerformancePhotovoltaicThermalSimple(mo);
    break;
  }
  case openstudio::IddObjectType::OS_StandardsInformation_Construction :
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_StandardsInformation_Material:
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
  case openstudio::IddObjectType::OS_SteamEquipment_Definition:
    {
      // no-op
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
  case openstudio::IddObjectType::OS_SurfaceProperty_ConvectionCoefficients:
  {
    model::SurfacePropertyConvectionCoefficients obj = modelObject.cast<SurfacePropertyConvectionCoefficients>();
    retVal = translateSurfacePropertyConvectionCoefficients(obj);
    break;
  }
  case openstudio::IddObjectType::OS_SurfaceProperty_ConvectionCoefficients_MultipleSurface:
  {
    model::SurfacePropertyConvectionCoefficientsMultipleSurface obj = modelObject.cast<SurfacePropertyConvectionCoefficientsMultipleSurface>();
    retVal = translateSurfacePropertyConvectionCoefficientsMultipleSurface(obj);
    break;
  }
  case openstudio::IddObjectType::OS_SurfaceProperty_OtherSideCoefficients:
  {
    model::SurfacePropertyOtherSideCoefficients obj = modelObject.cast<SurfacePropertyOtherSideCoefficients>();
    retVal = translateSurfacePropertyOtherSideCoefficients(obj);
    break;
  }
  case openstudio::IddObjectType::OS_SurfaceProperty_OtherSideConditionsModel:
  {
    model::SurfacePropertyOtherSideConditionsModel obj = modelObject.cast<SurfacePropertyOtherSideConditionsModel>();
    retVal = translateSurfacePropertyOtherSideConditionsModel(obj);
    break;
  }
  case openstudio::IddObjectType::OS_SubSurface :
    {
      model::SubSurface subSurface = modelObject.cast<SubSurface>();
      retVal = translateSubSurface(subSurface);
      break;
    }
  case openstudio::IddObjectType::OS_Table_MultiVariableLookup :
    {
      model::TableMultiVariableLookup table = modelObject.cast<TableMultiVariableLookup>();
      retVal = translateTableMultiVariableLookup(table);
      break;
    }
  case openstudio::IddObjectType::OS_TemperingValve :
    {
      auto mo = modelObject.cast<TemperingValve>();
      retVal = translateTemperingValve(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ThermalZone :
    {
      model::ThermalZone zone = modelObject.cast<ThermalZone>();
      retVal = translateThermalZone(zone);
      break;
    }
  case openstudio::IddObjectType::OS_ThermalStorage_Ice_Detailed :
    {
      auto mo = modelObject.cast<ThermalStorageIceDetailed>();
      retVal = translateThermalStorageIceDetailed(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ThermalStorage_ChilledWater_Stratified :
    {
      model::ThermalStorageChilledWaterStratified mo = modelObject.cast<ThermalStorageChilledWaterStratified>();
      retVal = translateThermalStorageChilledWaterStratified(mo);
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
  case openstudio::IddObjectType::OS_UnitarySystemPerformance_Multispeed :
    {
      model::UnitarySystemPerformanceMultispeed sysPerfMultispeed = modelObject.cast<UnitarySystemPerformanceMultispeed>();
      retVal = translateUnitarySystemPerformanceMultispeed(sysPerfMultispeed);
      break;
    }
  case openstudio::IddObjectType::OS_UtilityBill:
    {
      // no-op
      break;
    }
  case openstudio::IddObjectType::OS_UtilityCost_Charge_Block:
    {
      LOG(Warn, "OS_UtilityCost_Charge_Block is not currently translated");
      break;
    }
  case openstudio::IddObjectType::OS_UtilityCost_Charge_Simple:
    {
      LOG(Warn, "OS_UtilityCost_Charge_Simple is not currently translated");
      break;
    }
  case openstudio::IddObjectType::OS_UtilityCost_Computation:
    {
      LOG(Warn, "OS_UtilityCost_Computation is not currently translated");
      break;
    }
  case openstudio::IddObjectType::OS_UtilityCost_Qualify:
    {
      LOG(Warn, "OS_UtilityCost_Qualify is not currently translated");
      break;
    }
  case openstudio::IddObjectType::OS_UtilityCost_Ratchet:
    {
      LOG(Warn, "OS_UtilityCost_Ratchet is not currently translated");
      break;
    }
  case openstudio::IddObjectType::OS_UtilityCost_Tariff:
    {
      LOG(Warn, "OS_UtilityCost_Tariff is not currently translated");
      break;
    }
  case openstudio::IddObjectType::OS_UtilityCost_Variable:
    {
      LOG(Warn, "OS_UtilityCost_Variable is not currently translated");
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
  case openstudio::IddObjectType::OS_WindowMaterial_Blind:
  {
    model::Blind blind = modelObject.cast<Blind>();
    retVal = translateBlind(blind);
    break;
  }
  case openstudio::IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice:
  {
    model::DaylightRedirectionDevice drd = modelObject.cast<DaylightRedirectionDevice>();
    retVal = translateDaylightRedirectionDevice(drd);
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
  case openstudio::IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic:
    {
      LOG(Warn, "OS_WindowMaterial_GlazingGroup_Thermochromic is not currently translated");
      break;
    }
  case  openstudio::IddObjectType::OS_WindowProperty_FrameAndDivider:
  {
    model::WindowPropertyFrameAndDivider frameAndDivider = modelObject.cast<WindowPropertyFrameAndDivider>();
    retVal = translateWindowPropertyFrameAndDivider(frameAndDivider);
    break;
  }
  case openstudio::IddObjectType::OS_YearDescription:
    {
      // no-op
      return retVal;
    }
  case openstudio::IddObjectType::OS_ZoneAirContaminantBalance :
    {
      auto mo = modelObject.cast<ZoneAirContaminantBalance>();
      retVal = translateZoneAirContaminantBalance(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneAirHeatBalanceAlgorithm :
    {
      model::ZoneAirHeatBalanceAlgorithm mo = modelObject.cast<ZoneAirHeatBalanceAlgorithm>();
      retVal = translateZoneAirHeatBalanceAlgorithm(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneAirMassFlowConservation:
    {
      model::ZoneAirMassFlowConservation mo = modelObject.cast<ZoneAirMassFlowConservation>();
      retVal = translateZoneAirMassFlowConservation(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneCapacitanceMultiplier_ResearchSpecial :
    {
      model::ZoneCapacitanceMultiplierResearchSpecial mo = modelObject.cast<ZoneCapacitanceMultiplierResearchSpecial>();
      retVal = translateZoneCapacitanceMultiplierResearchSpecial(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneControl_ContaminantController :
    {
      auto mo = modelObject.cast<ZoneControlContaminantController>();
      retVal = translateZoneControlContaminantController(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneControl_Humidistat :
    {
      model::ZoneControlHumidistat mo = modelObject.cast<ZoneControlHumidistat>();
      retVal = translateZoneControlHumidistat(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneControl_Thermostat_StagedDualSetpoint :
    {
      model::ZoneControlThermostatStagedDualSetpoint mo = modelObject.cast<ZoneControlThermostatStagedDualSetpoint>();
      retVal = translateZoneControlThermostatStagedDualSetpoint(mo);
      break;
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
  case openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Electric :
    {
      model::ZoneHVACBaseboardRadiantConvectiveElectric mo = modelObject.cast<ZoneHVACBaseboardRadiantConvectiveElectric>();
      retVal = translateZoneHVACBaseboardRadiantConvectiveElectric(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Water :
    {
      model::ZoneHVACBaseboardRadiantConvectiveWater mo = modelObject.cast<ZoneHVACBaseboardRadiantConvectiveWater>();
      retVal = translateZoneHVACBaseboardRadiantConvectiveWater(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_Dehumidifier_DX :
    {
      model::ZoneHVACDehumidifierDX mo = modelObject.cast<ZoneHVACDehumidifierDX>();
      retVal = translateZoneHVACDehumidifierDX(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_EnergyRecoveryVentilator :
    {
      model::ZoneHVACEnergyRecoveryVentilator mo = modelObject.cast<ZoneHVACEnergyRecoveryVentilator>();
      retVal = translateZoneHVACEnergyRecoveryVentilator(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_EnergyRecoveryVentilator_Controller :
    {
      model::ZoneHVACEnergyRecoveryVentilatorController mo = modelObject.cast<ZoneHVACEnergyRecoveryVentilatorController>();
      retVal = translateZoneHVACEnergyRecoveryVentilatorController(mo);
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
  case openstudio::IddObjectType::OS_ZoneHVAC_HighTemperatureRadiant :
    {
      model::ZoneHVACHighTemperatureRadiant mo = modelObject.cast<ZoneHVACHighTemperatureRadiant>();
      retVal = translateZoneHVACHighTemperatureRadiant(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_IdealLoadsAirSystem :
    {
      model::ZoneHVACIdealLoadsAirSystem mo = modelObject.cast<ZoneHVACIdealLoadsAirSystem>();
      retVal = translateZoneHVACIdealLoadsAirSystem(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric :
    {
      model::ZoneHVACLowTemperatureRadiantElectric mo = modelObject.cast<ZoneHVACLowTemperatureRadiantElectric>();
      retVal = translateZoneHVACLowTemperatureRadiantElectric(mo);
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
  case openstudio::IddObjectType::OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow :
    {
      model::ZoneHVACTerminalUnitVariableRefrigerantFlow mo = modelObject.cast<ZoneHVACTerminalUnitVariableRefrigerantFlow>();
      retVal = translateZoneHVACTerminalUnitVariableRefrigerantFlow(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_UnitHeater :
    {
      model::ZoneHVACUnitHeater mo = modelObject.cast<ZoneHVACUnitHeater>();
      retVal = translateZoneHVACUnitHeater(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_UnitVentilator :
    {
      model::ZoneHVACUnitVentilator mo = modelObject.cast<ZoneHVACUnitVentilator>();
      retVal = translateZoneHVACUnitVentilator(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump :
    {
      model::ZoneHVACWaterToAirHeatPump mo = modelObject.cast<ZoneHVACWaterToAirHeatPump>();
      retVal = translateZoneHVACWaterToAirHeatPump(mo);
      break;
    }
  case openstudio::IddObjectType::OS_ZoneMixing:
  {
    model::ZoneMixing mo = modelObject.cast<ZoneMixing>();
    retVal = translateZoneMixing(mo);
    break;
  }
  case openstudio::IddObjectType::OS_ZoneVentilation_DesignFlowRate :
    {
      auto mo = modelObject.cast<ZoneVentilationDesignFlowRate>();
      retVal = translateZoneVentilationDesignFlowRate(mo);
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
      m_progressBar->setValue((int)m_map.size());
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

    for(auto & elem : children)
    {
      if (std::find(types.begin(),types.end(),elem.iddObject().type()) != types.end()) {
        translateAndMapModelObject(elem);
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
  result.push_back(IddObjectType::OS_ProgramControl);
  result.push_back(IddObjectType::OS_ConvergenceLimits);
  result.push_back(IddObjectType::OS_HeatBalanceAlgorithm);
  result.push_back(IddObjectType::OS_RunPeriod);
  result.push_back(IddObjectType::OS_RunPeriodControl_DaylightSavingTime);
  result.push_back(IddObjectType::OS_RunPeriodControl_SpecialDays);
  result.push_back(IddObjectType::OS_ShadowCalculation);
  result.push_back(IddObjectType::OS_SurfaceConvectionAlgorithm_Inside);
  result.push_back(IddObjectType::OS_SurfaceConvectionAlgorithm_Outside);
  result.push_back(IddObjectType::OS_SurfaceProperty_ConvectionCoefficients_MultipleSurface);
  result.push_back(IddObjectType::OS_Timestep);
  result.push_back(IddObjectType::OS_Sizing_Parameters);
  result.push_back(IddObjectType::OS_ZoneAirContaminantBalance);
  result.push_back(IddObjectType::OS_ZoneAirHeatBalanceAlgorithm);
  result.push_back(IddObjectType::OS_ZoneAirMassFlowConservation);
  result.push_back(IddObjectType::OS_ZoneCapacitanceMultiplier_ResearchSpecial);
  result.push_back(IddObjectType::OS_OutputControl_ReportingTolerances);

  result.push_back(IddObjectType::OS_Site);
  result.push_back(IddObjectType::OS_Site_GroundReflectance);
  result.push_back(IddObjectType::OS_Site_GroundTemperature_BuildingSurface);
  result.push_back(IddObjectType::OS_Site_GroundTemperature_Deep);
  result.push_back(IddObjectType::OS_Site_GroundTemperature_FCfactorMethod);
  result.push_back(IddObjectType::OS_Site_GroundTemperature_Shallow);
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

  result.push_back(IddObjectType::OS_SurfaceProperty_ConvectionCoefficients);

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
  result.push_back(IddObjectType::OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass);
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
  result.push_back(IddObjectType::OS_Coil_Heating_Desuperheater);
  result.push_back(IddObjectType::OS_Coil_Heating_Gas);
  result.push_back(IddObjectType::OS_Coil_Heating_Water);
  result.push_back(IddObjectType::OS_Coil_Heating_WaterToAirHeatPump_EquationFit);
  result.push_back(IddObjectType::OS_Coil_WaterHeating_Desuperheater);
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
  result.push_back(IddObjectType::OS_Table_MultiVariableLookup);
  result.push_back(IddObjectType::OS_DistrictCooling);
  result.push_back(IddObjectType::OS_DistrictHeating);
  result.push_back(IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial);
  result.push_back(IddObjectType::OS_Fan_ConstantVolume);
  result.push_back(IddObjectType::OS_Fan_OnOff);
  result.push_back(IddObjectType::OS_Fan_ZoneExhaust);
  result.push_back(IddObjectType::OS_Node);
  result.push_back(IddObjectType::OS_PlantLoop);
  result.push_back(IddObjectType::OS_Splitter);
  result.push_back(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water);
  result.push_back(IddObjectType::OS_ZoneHVAC_IdealLoadsAirSystem);
  result.push_back(IddObjectType::OS_ZoneHVAC_HighTemperatureRadiant);
  result.push_back(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow);
  result.push_back(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow);
  result.push_back(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric);
  result.push_back(IddObjectType::OS_ZoneMixing);

  result.push_back(IddObjectType::OS_Refrigeration_System);
  result.push_back(IddObjectType::OS_Refrigeration_TranscriticalSystem);

  result.push_back(IddObjectType::OS_ElectricLoadCenter_Distribution);
  result.push_back(IddObjectType::OS_Generator_MicroTurbine);
  result.push_back(IddObjectType::OS_Generator_FuelCell);
  result.push_back(IddObjectType::OS_Generator_FuelCell_AirSupply);
  result.push_back(IddObjectType::OS_Generator_FuelCell_AuxiliaryHeater);
  result.push_back(IddObjectType::OS_Generator_FuelCell_ElectricalStorage);
  result.push_back(IddObjectType::OS_Generator_FuelCell_ExhaustGasToWaterHeatExchanger);
  result.push_back(IddObjectType::OS_Generator_FuelCell_Inverter);
  result.push_back(IddObjectType::OS_Generator_FuelCell_PowerModule);
  result.push_back(IddObjectType::OS_Generator_FuelCell_StackCooler);
  result.push_back(IddObjectType::OS_Generator_FuelCell_WaterSupply);
  result.push_back(IddObjectType::OS_Generator_FuelSupply);
  result.push_back(IddObjectType::OS_Generator_Photovoltaic);
  result.push_back(IddObjectType::OS_PhotovoltaicPerformance_EquivalentOneDiode);
  result.push_back(IddObjectType::OS_PhotovoltaicPerformance_Simple);
  result.push_back(IddObjectType::OS_ElectricLoadCenter_Inverter_LookUpTable);
  result.push_back(IddObjectType::OS_ElectricLoadCenter_Inverter_Simple);
  result.push_back(IddObjectType::OS_ElectricLoadCenter_Storage_Simple);
  result.push_back(IddObjectType::OS_ElectricLoadCenter_Storage_Converter);

  // put these down here so they have a chance to be translated with their "parent"
  result.push_back(IddObjectType::OS_LifeCycleCost);

  result.push_back(IddObjectType::OS_Output_Meter);
  result.push_back(IddObjectType::OS_Meter_Custom);
  result.push_back(IddObjectType::OS_Meter_CustomDecrement);
  result.push_back(IddObjectType::OS_Output_Variable);

  result.push_back(IddObjectType::OS_EnergyManagementSystem_GlobalVariable);
  result.push_back(IddObjectType::OS_EnergyManagementSystem_InternalVariable);
  result.push_back(IddObjectType::OS_EnergyManagementSystem_Sensor);
  result.push_back(IddObjectType::OS_EnergyManagementSystem_Actuator);
  result.push_back(IddObjectType::OS_EnergyManagementSystem_ConstructionIndexVariable);
  result.push_back(IddObjectType::OS_EnergyManagementSystem_CurveOrTableIndexVariable);
  result.push_back(IddObjectType::OS_EnergyManagementSystem_MeteredOutputVariable);
  result.push_back(IddObjectType::OS_EnergyManagementSystem_Program);
  result.push_back(IddObjectType::OS_EnergyManagementSystem_Subroutine);
  result.push_back(IddObjectType::OS_EnergyManagementSystem_ProgramCallingManager);
  result.push_back(IddObjectType::OS_EnergyManagementSystem_OutputVariable);
  result.push_back(IddObjectType::OS_EnergyManagementSystem_TrendVariable);
  result.push_back(IddObjectType::OS_Output_EnergyManagementSystem);

  return result;
}

void ForwardTranslator::translateConstructions(const model::Model & model)
{
  std::vector<IddObjectType> iddObjectTypes;
  iddObjectTypes.push_back(IddObjectType::OS_MaterialProperty_GlazingSpectralData);
  iddObjectTypes.push_back(IddObjectType::OS_MaterialProperty_MoisturePenetrationDepth_Settings);
  iddObjectTypes.push_back(IddObjectType::OS_Material);
  iddObjectTypes.push_back(IddObjectType::OS_Material_AirGap);
  iddObjectTypes.push_back(IddObjectType::OS_Material_AirWall);
  iddObjectTypes.push_back(IddObjectType::OS_Material_InfraredTransparent);
  iddObjectTypes.push_back(IddObjectType::OS_Material_NoMass);
  iddObjectTypes.push_back(IddObjectType::OS_Material_RoofVegetation);

  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Blind);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Gas);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_GasMixture);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Glazing);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Screen);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_Shade);
  iddObjectTypes.push_back(IddObjectType::OS_WindowMaterial_SimpleGlazingSystem);
  iddObjectTypes.push_back(IddObjectType::OS_WindowProperty_FrameAndDivider);
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

  iddObjectTypes.push_back(IddObjectType::OS_SurfaceProperty_OtherSideCoefficients);
  iddObjectTypes.push_back(IddObjectType::OS_SurfaceProperty_OtherSideConditionsModel);
  iddObjectTypes.push_back(IddObjectType::OS_SurfaceProperty_ConvectionCoefficients);

  for (const IddObjectType& iddObjectType : iddObjectTypes){

    // get objects by type in sorted order
    std::vector<WorkspaceObject> objects = model.getObjectsByType(iddObjectType);
    std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());

    for (const WorkspaceObject& workspaceObject : objects){
      model::ModelObject modelObject = workspaceObject.cast<ModelObject>();
      boost::optional<IdfObject> result = translateAndMapModelObject(modelObject);

      if (modelObject.optionalCast<ConstructionBase>()){
        if (istringEqual("Interior Partition Surface Construction", workspaceObject.name().get())){
          m_interiorPartitionSurfaceConstruction = modelObject.cast<ConstructionBase>();
        }

        if (istringEqual("Shading Surface Construction", workspaceObject.name().get())){
          m_exteriorSurfaceConstruction = modelObject.cast<ConstructionBase>();
        }
      }
    }
  }
}

void ForwardTranslator::translateSchedules(const model::Model & model)
{

  // loop over schedule type limits
  std::vector<WorkspaceObject> objects = model.getObjectsByType(IddObjectType::OS_ScheduleTypeLimits);
  std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());
  for (const WorkspaceObject& workspaceObject : objects){
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

  for (const IddObjectType& iddObjectType : iddObjectTypes){

    // get objects by type in sorted order
    objects = model.getObjectsByType(iddObjectType);
    std::sort(objects.begin(), objects.end(), WorkspaceObjectNameLess());

    for (const WorkspaceObject& workspaceObject : objects){
      model::ModelObject modelObject = workspaceObject.cast<ModelObject>();
      boost::optional<IdfObject> result = translateAndMapModelObject(modelObject);

      if ((iddObjectType == IddObjectType::OS_Schedule_Compact) ||
          (iddObjectType == IddObjectType::OS_Schedule_Constant) ||
          (iddObjectType == IddObjectType::OS_Schedule_Ruleset) ||
          (iddObjectType == IddObjectType::OS_Schedule_FixedInterval) ||
          (iddObjectType == IddObjectType::OS_Schedule_VariableInterval)){
        // This predates Model::alwaysOnDiscreteSchedule, but leaving it in place for now
        if (istringEqual("Always_On", workspaceObject.name().get())){
          m_alwaysOnSchedule = result;
        }

        if (istringEqual("Always_Off", workspaceObject.name().get())){
          m_alwaysOffSchedule = result;
        }
      }
    }
  }

  // Make sure these get in the idf file
  {
    auto schedule = model.alwaysOnDiscreteSchedule();
    translateAndMapModelObject(schedule);
    schedule = model.alwaysOffDiscreteSchedule();
    translateAndMapModelObject(schedule);
    schedule = model.alwaysOnContinuousSchedule();
    translateAndMapModelObject(schedule);
  }
}

void ForwardTranslator::reset()
{
  m_idfObjects.clear();

  m_map.clear();

  m_anyNumberScheduleTypeLimits.reset();

  m_alwaysOnSchedule.reset();

  m_alwaysOffSchedule.reset();

  m_interiorPartitionSurfaceConstruction.reset();

  m_exteriorSurfaceConstruction.reset();

  m_constructionHandleToReversedConstructions.clear();

  m_logSink.setThreadId(QThread::currentThread());

  m_logSink.resetStringStream();

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

model::ConstructionBase ForwardTranslator::interiorPartitionSurfaceConstruction(model::Model & model)
{
  if (m_interiorPartitionSurfaceConstruction){
    return *m_interiorPartitionSurfaceConstruction;
  }

  StandardOpaqueMaterial material(model, "MediumSmooth", 0.0254, 0.16, 800, 1090);
  material.setThermalAbsorptance(0.4);
  material.setSolarAbsorptance(0.4);
  material.setVisibleAbsorptance(0.3);

  model::Construction construction(model);
  construction.setName("Interior Partition Surface Construction");
  construction.insertLayer(0, material);
  m_interiorPartitionSurfaceConstruction = construction;

  translateAndMapModelObject(material);
  translateAndMapModelObject(construction);

  return *m_interiorPartitionSurfaceConstruction;
}

model::ConstructionBase ForwardTranslator::exteriorSurfaceConstruction(model::Model & model)
{
  if (m_exteriorSurfaceConstruction){
    return *m_exteriorSurfaceConstruction;
  }

  StandardOpaqueMaterial material(model, "MediumSmooth", 0.1524, 0.49, 512, 880);
  material.setThermalAbsorptance(0.6);
  material.setSolarAbsorptance(0.6);
  material.setVisibleAbsorptance(0.5);

  model::Construction construction(model);
  construction.setName("Interior Partition Surface Construction");
  construction.insertLayer(0, material);
  m_exteriorSurfaceConstruction = construction;

  translateAndMapModelObject(material);
  translateAndMapModelObject(construction);

  return *m_exteriorSurfaceConstruction;
}

model::ConstructionBase ForwardTranslator::reverseConstruction(const model::ConstructionBase& construction)
{
  auto it = m_constructionHandleToReversedConstructions.find(construction.handle());
  if (it != m_constructionHandleToReversedConstructions.end()){
    return it->second;
  }

  if (!construction.optionalCast<model::LayeredConstruction>()){
    m_constructionHandleToReversedConstructions.insert(std::make_pair(construction.handle(), construction));
    return construction;
  }

  model::LayeredConstruction layeredConstruction = construction.cast<model::LayeredConstruction>();

  if (layeredConstruction.isSymmetric()){
    m_constructionHandleToReversedConstructions.insert(std::make_pair(construction.handle(), construction));
    return construction;
  }

  if (construction.optionalCast<model::Construction>()){
    model::Construction reversed = construction.cast<model::Construction>().reverseConstruction();
    m_constructionHandleToReversedConstructions.insert(std::make_pair(construction.handle(), reversed));
    return reversed;
  }

  if (construction.optionalCast<model::ConstructionWithInternalSource>()){
    model::ConstructionWithInternalSource reversed = construction.cast<model::ConstructionWithInternalSource>().reverseConstructionWithInternalSource();
    m_constructionHandleToReversedConstructions.insert(std::make_pair(construction.handle(), reversed));
    return reversed;
  }

  // DLM: this is not right, should make reverseConstruction a virtual method on LayeredConstruction so it will work for ConstructionWithInternalSource, etc
  LOG(Error, "Do not know how to reverse construction " << construction);

  model::MaterialVector layers = layeredConstruction.layers();
  std::reverse(layers.begin(), layers.end());

  model::Construction reversed = model::Construction(construction.model());
  reversed.setName(construction.name().get() + " Reversed");
  reversed.setLayers(layers);
  m_constructionHandleToReversedConstructions.insert(std::make_pair(construction.handle(), reversed));

  return reversed;
}

void ForwardTranslator::resolveMatchedSurfaceConstructionConflicts(model::Model& model)
{
  // DLM: there is duplicate code in Surface_Impl::construction
  // if you update this code you must update that code

  std::set<Handle> processedSurfaces;

  model::SurfaceVector surfaces = model.getConcreteModelObjects<model::Surface>();
  for (model::Surface surface : surfaces){

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

      LOG(Info, "Surface '" << surface.nameString() << "' has a construction and '" << adjacentSurface->nameString()
          << "' does not, using '" << surface.nameString() << "'\'s construction.");

      surface.setConstruction(c1);
      adjacentSurface->setConstruction(c2);
      processedSurfaces.insert(surface.handle());
      processedSurfaces.insert(adjacentSurface->handle());
      continue;
    }

    if (!constructionWithSearchDistance && adjacentConstructionWithSearchDistance){

      model::ConstructionBase c2 = adjacentConstructionWithSearchDistance->first;
      model::ConstructionBase c1 = reverseConstruction(c2);

      LOG(Info, "Surface '" << adjacentSurface->nameString() << "' has a construction and '" << surface.nameString()
          << "' does not, using '" << adjacentSurface->nameString() << "'\'s construction.");

      surface.setConstruction(c1);
      adjacentSurface->setConstruction(c2);
      processedSurfaces.insert(surface.handle());
      processedSurfaces.insert(adjacentSurface->handle());
      continue;
    }

    if (!constructionWithSearchDistance && !adjacentConstructionWithSearchDistance){
      // no constructions, nothing to be done
      LOG(Error, "No construction for either surface '" << surface.nameString()
          << "', and '" << adjacentSurface->nameString() << "'");

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
        LOG(Warn, "Both surfaces '" << surface.nameString() << "', and '" << adjacentSurface->nameString()
            << "' reference the same construction '" << c1.nameString() << "' but it is not symmetric, creating a reversed copy.");

        // DLM: use surface name to choose which surface gets the original copy, not a good way but at least repeatable
        if (surface.nameString() < adjacentSurface->nameString()){
          surface.setConstruction(c1);
          adjacentSurface->setConstruction(c2);
        }else{
          surface.setConstruction(c2);
          adjacentSurface->setConstruction(c1);
        }
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

      LOG(Info, "Surfaces '" << surface.nameString() << "', and '" << adjacentSurface->nameString()
          << "' reference different constructions, choosing '" << surface.nameString() << "'\'s construction based on search distance.");

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

      LOG(Info, "Surfaces '" << surface.nameString() << "', and '" << adjacentSurface->nameString()
          << "' reference different constructions, choosing '" << adjacentSurface->nameString() << "'\'s construction based on search distance.");

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
    LOG(Error, "Could not resolve matched surface construction conflicts between surfaces '" << surface.nameString()
        << "', and '" << adjacentSurface->nameString() << "'");
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

  model::SubSurfaceVector subSurfaces = model.getConcreteModelObjects<model::SubSurface>();
  for (model::SubSurface subSurface : subSurfaces){

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

      LOG(Info, "SubSurface '" << subSurface.nameString() << "' has a construction and '" << adjacentSubSurface->nameString()
          << "' does not, using '" << subSurface.nameString() << "'\'s construction.");

      subSurface.setConstruction(c1);
      adjacentSubSurface->setConstruction(c2);
      processedSubSurfaces.insert(subSurface.handle());
      processedSubSurfaces.insert(adjacentSubSurface->handle());
      continue;
    }

    if (!constructionWithSearchDistance && adjacentConstructionWithSearchDistance){

      model::ConstructionBase c2 = adjacentConstructionWithSearchDistance->first;
      model::ConstructionBase c1 = reverseConstruction(c2);

      LOG(Info, "SubSurface '" << adjacentSubSurface->nameString() << "' has a construction and '" << subSurface.nameString()
          << "' does not, using '" << adjacentSubSurface->nameString() << "'\'s construction.");

      subSurface.setConstruction(c1);
      adjacentSubSurface->setConstruction(c2);
      processedSubSurfaces.insert(subSurface.handle());
      processedSubSurfaces.insert(adjacentSubSurface->handle());
      continue;
    }

    if (!constructionWithSearchDistance && !adjacentConstructionWithSearchDistance){
      // no constructions, nothing to be done
      LOG(Error, "No construction for either sub surface '" << subSurface.nameString()
          << "', and '" << adjacentSubSurface->nameString() << "'");

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
        LOG(Warn, "Both sub surfaces '" << subSurface.nameString() << "', and '" << adjacentSubSurface->nameString()
            << "' reference the same construction '" << c1.nameString() << "' but it is not symmetric, creating a reversed copy.");

        // DLM: use subSurface name to choose which surface gets the original copy, not a good way but at least repeatable
        if (subSurface.nameString() < adjacentSubSurface->nameString()){
          subSurface.setConstruction(c1);
          adjacentSubSurface->setConstruction(c2);
        }else{
          subSurface.setConstruction(c2);
          adjacentSubSurface->setConstruction(c1);
        }
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

      LOG(Info, "SubSurfaces '" << subSurface.nameString() << "', and '" << adjacentSubSurface->nameString()
          << "' reference different constructions, choosing '" << subSurface.nameString() << "'\'s construction based on search distance.");

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

      LOG(Info, "SubSurfaces '" << subSurface.nameString() << "', and '" << adjacentSubSurface->nameString()
          << "' reference different constructions, choosing '" << adjacentSubSurface->nameString() << "'\'s construction based on search distance.");

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
    LOG(Error, "Could not resolve matched construction conflicts between sub surfaces '" << subSurface.nameString()
        << "', and '" << adjacentSubSurface->nameString() << "'");
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
  if( m_ipTabularOutput )
  {
    tableStyle.setString(OutputControl_Table_StyleFields::UnitConversion,"InchPound");
  }

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

  // ensure at least one life cycle cost exists to prevent crash in E+ 8
  unsigned numCosts = 0;
  for (const IdfObject& object : m_idfObjects){
    if (object.iddObject().type() == openstudio::IddObjectType::LifeCycleCost_NonrecurringCost){
      numCosts += 1;
    }else if (object.iddObject().type() == openstudio::IddObjectType::LifeCycleCost_RecurringCosts){
      numCosts += 1;
    }
  }
  if (numCosts == 0){
    // add default cost
    IdfObject idfObject(openstudio::IddObjectType::LifeCycleCost_NonrecurringCost);
    m_idfObjects.push_back(idfObject);

    idfObject.setString(LifeCycleCost_NonrecurringCostFields::Name, "Default Cost");
    idfObject.setString(LifeCycleCost_NonrecurringCostFields::Category, "Construction");
    idfObject.setDouble(LifeCycleCost_NonrecurringCostFields::Cost, 0.0);
    idfObject.setString(LifeCycleCost_NonrecurringCostFields::StartofCosts, "ServicePeriod");
  }

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
  std::stringstream ss;
  ss << ::energyplus::embedded_files::getFileAsString(path);
  return IdfFile::load(ss, IddFileType::EnergyPlus);
}

bool ForwardTranslator::isHVACComponentWithinUnitary(const model::HVACComponent& hvacComponent) const
{
  if( hvacComponent.containingHVACComponent() )
  {
    return true;
  }
  else if( hvacComponent.containingZoneHVACComponent() )
  {
    return true;
  }
  else if( hvacComponent.containingStraightComponent() )
  {
    return true;
  }
  else
  {
    return false;
  }
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

  for( auto it = m_idfObjects.begin();
     it != m_idfObjects.end();
     ++it )
  {
    if(it->iddObject().type().value() == openstudio::IddObjectType::FluidProperties_Name) {
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

  for( auto it = m_idfObjects.begin();
     it != m_idfObjects.end();
     ++it )
  {
    if(it->iddObject().type().value() == openstudio::IddObjectType::FluidProperties_Name) {
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

    for( auto it = fluidObjects.begin();
       it != fluidObjects.end();
       ++it )
    {
      if(it->iddObject().type().value() == openstudio::IddObjectType::FluidProperties_Name) {
        idfObject = *it;
      }
      m_idfObjects.push_back(*it);
    }
  }

  return idfObject;
}

boost::optional<IdfObject> ForwardTranslator::createSimpleSchedule(const std::string & name,
                                                                   const std::vector< std::pair<openstudio::Time, double> > & defaultDay,
                                                                   const std::vector< std::pair<openstudio::Time, double> > & summerDesignDay,
                                                                   const std::vector< std::pair<openstudio::Time, double> > & winterDesignDay) {

  IdfObject idfObject(openstudio::IddObjectType::Schedule_Compact);

  idfObject.setName(name);

  StringVector values;
  values.push_back("Through: 12/31");
  IdfExtensibleGroup eg = idfObject.pushExtensibleGroup(values);
  OS_ASSERT(!eg.empty());

  if(!summerDesignDay.empty()) {
    bool hasEndTime = false;
    double endTimeValue;
    values[0] = "For: SummerDesignDay";
    eg = idfObject.pushExtensibleGroup(values);
    OS_ASSERT(!eg.empty());

    for( auto it = summerDesignDay.begin();
         it != summerDesignDay.end();
         ++it )
    {
      int minutes = it->first.minutes();
      int hours = it->first.hours();
      if(0 == minutes && 0 == hours) {
        hasEndTime = true;
        endTimeValue = it->second;
        continue;
      }
      values[0] = "Until: " + std::string(hours < 10 ? "0" : "") + boost::lexical_cast<std::string>(hours) + std::string(minutes < 10 ? ":0" : ":") + boost::lexical_cast<std::string>(minutes);
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      values[0] = "";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      bool ok = eg.setDouble(0,it->second);
      OS_ASSERT(ok);
    }
    if(hasEndTime) {
      values[0] = "Until: 24:00";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      values[0] = "";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      bool ok = eg.setDouble(0,endTimeValue);
      OS_ASSERT(ok);
    } else {
      LOG(Error, "Summer Design Day must have a value for all 24 hours");
    }
  }

  if(!winterDesignDay.empty()) {
    bool hasEndTime = false;
    double endTimeValue;
    values[0] = "For: WinterDesignDay";
    eg = idfObject.pushExtensibleGroup(values);
    OS_ASSERT(!eg.empty());

    for( auto it = winterDesignDay.begin();
         it != winterDesignDay.end();
         ++it )
    {
      int minutes = it->first.minutes();
      int hours = it->first.hours();
      if(0 == minutes && 0 == hours) {
        hasEndTime = true;
        endTimeValue = it->second;
        continue;
      }
      values[0] = "Until: " + std::string(hours < 10 ? "0" : "") + boost::lexical_cast<std::string>(hours) + std::string(minutes < 10 ? ":0" : ":") + boost::lexical_cast<std::string>(minutes);
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      values[0] = "";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      bool ok = eg.setDouble(0,it->second);
      OS_ASSERT(ok);
    }
    if(hasEndTime) {
      values[0] = "Until: 24:00";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      values[0] = "";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      bool ok = eg.setDouble(0,endTimeValue);
      OS_ASSERT(ok);
    } else {
      LOG(Error, "Winter Design Day must have a value for all 24 hours");
    }
  }

  if(!summerDesignDay.empty() || !winterDesignDay.empty()) {
    values[0] = "For: AllOtherDays";
    eg = idfObject.pushExtensibleGroup(values);
    OS_ASSERT(!eg.empty());
  } else {
    values[0] = "For: AllDays";
    eg = idfObject.pushExtensibleGroup(values);
    OS_ASSERT(!eg.empty());
  }

  bool hasEndTime = false;
  double endTimeValue;
  for( auto it = defaultDay.begin();
         it != defaultDay.end();
         ++it )
    {
      int minutes = it->first.minutes();
      int hours = it->first.hours();
      if(0 == minutes && 0 == hours) {
        hasEndTime = true;
        endTimeValue = it->second;
        continue;
      }
      values[0] = "Until: " + std::string(hours < 10 ? "0" : "") + boost::lexical_cast<std::string>(hours) + std::string(minutes < 10 ? ":0" : ":") + boost::lexical_cast<std::string>(minutes);
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      values[0] = "";
      eg = idfObject.pushExtensibleGroup(values);
      OS_ASSERT(!eg.empty());
      bool ok = eg.setDouble(0,it->second);
      OS_ASSERT(ok);
    }
  if(hasEndTime) {
    values[0] = "Until: 24:00";
    eg = idfObject.pushExtensibleGroup(values);
    OS_ASSERT(!eg.empty());
    values[0] = "";
    eg = idfObject.pushExtensibleGroup(values);
    OS_ASSERT(!eg.empty());
    bool ok = eg.setDouble(0,endTimeValue);
    OS_ASSERT(ok);
  } else {
      LOG(Error, "Default Day must have a value for all 24 hours");
    }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}


void ForwardTranslator::fixSPMsForUnitarySystem(const model::HVACComponent & unitary,const std::string & fanInletNodeName, const std::string & fanOutletNodeName)
{
  if( auto airSystem = unitary.airLoopHVAC() ) {
    auto supplyComponents = airSystem->supplyComponents(airSystem->supplyInletNode(),unitary);
    auto oaSystems = subsetCastVector<model::AirLoopHVACOutdoorAirSystem>(supplyComponents);
    if( ! oaSystems.empty() ) {
      auto reliefComponents = oaSystems.back().oaComponents();
      supplyComponents.insert(supplyComponents.end(),reliefComponents.begin(),reliefComponents.end());
    }
    auto upstreamNodes = subsetCastVector<model::Node>(supplyComponents);

    for( const auto & node : upstreamNodes ) {
      auto spms = subsetCastVector<model::SetpointManagerMixedAir>(node.setpointManagers());
      for( auto & spm : spms ) {
        auto pred = [&spm](IdfObject & idfObject) {
          if( idfObject.iddObject().type() == IddObjectType::SetpointManager_MixedAir ) {
            auto idfName = idfObject.name();
            auto osName = spm.name();
            if( idfName && osName && (osName.get() == idfName.get()) ) {
              return true;
            }
          }
          return false;
        };
        auto spm_idf = std::find_if(m_idfObjects.begin(),m_idfObjects.end(),pred);
        if( spm_idf != m_idfObjects.end() ) {
          auto result = spm_idf->getString(SetpointManager_MixedAirFields::FanInletNodeName);
          if( ! result || result->empty() ) {
            spm_idf->setString(SetpointManager_MixedAirFields::FanInletNodeName,fanInletNodeName);
          }
          result = spm_idf->getString(SetpointManager_MixedAirFields::FanOutletNodeName);
          if( ! result || result->empty() ) {
            spm_idf->setString(SetpointManager_MixedAirFields::FanOutletNodeName,fanOutletNodeName);
          }
        }
      }
    }
  }
}

} // energyplus

} // openstudio
