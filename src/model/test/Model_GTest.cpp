/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../GenericModelObject.hpp"
#include "../GenericModelObject_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../SimulationControl.hpp"
#include "../SimulationControl_Impl.hpp"
#include "../OutputVariable.hpp"
#include "../OutputVariable_Impl.hpp"
#include "../ParentObject_Impl.hpp"
#include "../RunPeriod.hpp"
#include "../RunPeriod_Impl.hpp"

#include "../Site.hpp"
#include "../Site_Impl.hpp"
#include "../Facility.hpp"
#include "../Facility_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../DefaultConstructionSet.hpp"
#include "../DefaultConstructionSet_Impl.hpp"
#include "../DefaultScheduleSet.hpp"
#include "../DefaultScheduleSet_Impl.hpp"
#include "../Lights.hpp"
#include "../Lights_Impl.hpp"
#include "../LightsDefinition.hpp"
#include "../LightsDefinition_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"

#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"

#include "../FoundationKivaSettings.hpp"
#include "../FoundationKivaSettings_Impl.hpp"
#include "../OutputControlFiles.hpp"
#include "../OutputControlFiles_Impl.hpp"
#include "../OutputControlReportingTolerances.hpp"
#include "../OutputControlReportingTolerances_Impl.hpp"
#include "../OutputControlTableStyle.hpp"
#include "../OutputControlTableStyle_Impl.hpp"
#include "../OutputDiagnostics.hpp"
#include "../OutputDiagnostics_Impl.hpp"
#include "../OutputDebuggingData.hpp"
#include "../OutputDebuggingData_Impl.hpp"
#include "../OutputJSON.hpp"
#include "../OutputJSON_Impl.hpp"
#include "../OutputSQLite.hpp"
#include "../OutputSQLite_Impl.hpp"
#include "../OutputEnergyManagementSystem.hpp"
#include "../OutputEnergyManagementSystem_Impl.hpp"
#include "../OutputTableSummaryReports.hpp"
#include "../OutputTableSummaryReports_Impl.hpp"
#include "../PerformancePrecisionTradeoffs_Impl.hpp"
#include "../PerformancePrecisionTradeoffs_Impl.hpp"
#include "../LifeCycleCostParameters.hpp"
#include "../LifeCycleCostParameters_Impl.hpp"
#include "../SizingParameters.hpp"
#include "../SizingParameters_Impl.hpp"
#include "../RadianceParameters.hpp"
#include "../RadianceParameters_Impl.hpp"
#include "../RunPeriodControlDaylightSavingTime.hpp"
#include "../RunPeriodControlDaylightSavingTime_Impl.hpp"
#include "../YearDescription.hpp"
#include "../YearDescription_Impl.hpp"
#include "../SiteGroundReflectance.hpp"
#include "../SiteGroundReflectance_Impl.hpp"
#include "../SiteWaterMainsTemperature.hpp"
#include "../SiteWaterMainsTemperature_Impl.hpp"
#include "../SiteGroundTemperatureBuildingSurface.hpp"
#include "../SiteGroundTemperatureBuildingSurface_Impl.hpp"
#include "../SiteGroundTemperatureFCfactorMethod.hpp"
#include "../SiteGroundTemperatureFCfactorMethod_Impl.hpp"
#include "../SiteGroundTemperatureDeep.hpp"
#include "../SiteGroundTemperatureDeep_Impl.hpp"
#include "../SiteGroundTemperatureShallow.hpp"
#include "../SiteGroundTemperatureShallow_Impl.hpp"
#include "../WeatherFile.hpp"
#include "../WeatherFile_Impl.hpp"
#include "../Version.hpp"
#include "../Version_Impl.hpp"
#include "../LightingSimulationControl.hpp"
#include "../LightingSimulationControl_Impl.hpp"
#include "../AirflowNetworkSimulationControl.hpp"
#include "../AirflowNetworkSimulationControl_Impl.hpp"
#include "../InsideSurfaceConvectionAlgorithm.hpp"
#include "../InsideSurfaceConvectionAlgorithm_Impl.hpp"
#include "../OutsideSurfaceConvectionAlgorithm.hpp"
#include "../OutsideSurfaceConvectionAlgorithm_Impl.hpp"
#include "../HeatBalanceAlgorithm.hpp"
#include "../HeatBalanceAlgorithm_Impl.hpp"
#include "../ZoneAirHeatBalanceAlgorithm.hpp"
#include "../ZoneAirHeatBalanceAlgorithm_Impl.hpp"
#include "../ZoneAirMassFlowConservation.hpp"
#include "../ZoneAirMassFlowConservation_Impl.hpp"
#include "../ZoneCapacitanceMultiplierResearchSpecial.hpp"
#include "../ZoneCapacitanceMultiplierResearchSpecial_Impl.hpp"
#include "../ConvergenceLimits.hpp"
#include "../ConvergenceLimits_Impl.hpp"
#include "../ShadowCalculation.hpp"
#include "../ShadowCalculation_Impl.hpp"
#include "../Timestep.hpp"
#include "../Timestep_Impl.hpp"
#include "../ClimateZones.hpp"
#include "../ClimateZones_Impl.hpp"
#include "../EnvironmentalImpactFactors.hpp"
#include "../EnvironmentalImpactFactors_Impl.hpp"
#include "../ExternalInterface.hpp"
#include "../ExternalInterface_Impl.hpp"
#include "../PythonPluginSearchPaths.hpp"
#include "../PythonPluginSearchPaths_Impl.hpp"

#include "../../utilities/core/PathHelpers.hpp"
#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/ValidityReport.hpp"
#include "../../utilities/sql/SqlFile.hpp"

#include "../../osversion/VersionTranslator.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <OpenStudio.hxx>

#include <boost/algorithm/string/case_conv.hpp>

using namespace openstudio::model;
using namespace openstudio;
/*
void checkObject(ModelObject object){

  // should be able to get ModelObject from this object's handle
  Handle handle = object.handle();
  EXPECT_FALSE(handle.isNull());
  OptionalModelObject optionalObject = object.model().getModelObject<ModelObject>(handle);
  EXPECT_TRUE(optionalObject);
  if (!optionalObject){
    std::cout << "Need to add wrapper for '" << object.iddObject().name() << "' to Model::getModelObject" << '\n';
  }

  // object might have a name
  OptionalString name = object.name();

  // get the idd object type
  IddObject iddObject = object.iddObject();
  std::string className = iddObject.name();
  // we might want a virtual method on ModelObject_Impl "className()" or something like that
  // stringstream to help report debugging information
  std::stringstream ss;
  ss << "ModelObject of type " << className;
  if (name) { ss << " and named '" << *name << "'"; }

  // loop over all possible output variables the user could request
  for (const std::string& variableName : object.outputVariableNames()){
    EXPECT_FALSE(variableName.empty());
  }

  // loop over all output variables the user has already requested
  for (const OutputVariable& variable : object.outputVariables()){
    OptionalString variableName = variable.getString(Output_VariableFields::VariableName);
    ASSERT_TRUE(variableName);

    ASSERT_TRUE(object.model().sqlFile());
    ASSERT_TRUE(object.model().sqlFile()->connectionOpen());

    // get environment periods
    std::vector<std::string> envPeriods = object.model().sqlFile()->availableEnvPeriods();
    ASSERT_TRUE(envPeriods.size() > 0);

    // see if there is data for this variable we could plot
    OptionalTimeSeries data = object.getData(variable, envPeriods[0]);

    // TODO: DLM@20101102: in general this should be true but this is failing in case of asking for
    // electric lighting power from a zone that does not have lights, should the model object
    // be capable of providing all the potential output variables and the currently available variables
    // separately????
    //EXPECT_TRUE(data);
    if (!data) {
      LOG_FREE(Debug,"ModelFixture",ss.str() << " has an inaccessible output variable, '"
          << *variableName << "'. Attempted to access it using environment period '"
          << envPeriods[0] << "', which is 1 of " << envPeriods.size() << " available periods.");
    }
  }

  OptionalParentObject parentObject = object.optionalCast<ParentObject>();

  if (parentObject){
    // loop through each child
    for (const ModelObject& child : parentObject->children()){
      OptionalParentObject parent = child.parent();
      if (!parent){
        std::cout << "Child " << child << " does not have a parent" << '\n';
      }
      ASSERT_TRUE(parent);
      EXPECT_TRUE(*parentObject == *parent);
      if (*parentObject != *parent){
        std::cout << "pcObject = " << *parentObject << " does not equal parent = " << *parent << '\n';
      }
      checkObject(child);
    }
  }
}
*/

TEST_F(ModelFixture, iddObjectType) {

  Model model;
  model.order().setDirectOrder(HandleVector());
  ScheduleCompact s(model);
  FanConstantVolume fan(model, s);  // sets schedule's ScheduleTypeLimits

  ModelObjectVector mos = model.getModelObjects<ModelObject>(true);  // sort objects
  ASSERT_EQ(3u, mos.size());
  EXPECT_TRUE(ScheduleCompact::iddObjectType() == mos[0].iddObjectType());
  EXPECT_TRUE(FanConstantVolume::iddObjectType() == mos[1].iddObjectType());
  EXPECT_TRUE(ScheduleTypeLimits::iddObjectType() == mos[2].iddObjectType());
}

TEST_F(ModelFixture, IddFile) {
  boost::optional<IddFile> iddFile = IddFile::load(resourcesPath() / toPath("model/OpenStudio.idd"));
  ASSERT_TRUE(iddFile);
}

/*
TEST_F(ModelFixture, Model)
{
  Workspace workspace(idfFile);

  // translate energyplus simulation to building model
  energyplus::ReverseTranslator reverseTranslator(workspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  model.setSqlFile(sqlFile);
  ASSERT_TRUE(model.sqlFile());

  OptionalSimulationControl simulation = model.getOptionalUniqueModelObject<SimulationControl>();
  ASSERT_TRUE(simulation);
  EXPECT_FALSE(simulation->parent());
  checkObject(*simulation);

  OptionalSite site = model.getOptionalUniqueModelObject<Site>();
  ASSERT_TRUE(site);
  EXPECT_FALSE(site->parent());
  checkObject(*site);

  // translator no longer creates Facility object
  OptionalFacility facility = model.getOptionalUniqueModelObject<Facility>();
  EXPECT_FALSE(facility);
  facility = model.getUniqueModelObject<Facility>();
  ASSERT_TRUE(facility);
  EXPECT_FALSE(facility->parent());
  EXPECT_FALSE(facility->children().empty());
  checkObject(*facility);
}
*/

// Create several model objects and check their uniqueness
TEST_F(ModelFixture, UniqueModelObjects) {
  Model model;

  AirLoopHVAC airLoopHVAC1(model);
  AirLoopHVAC airLoopHVAC2(model);
  AirLoopHVAC airLoopHVAC3(model);
  AirLoopHVAC airLoopHVAC4(model);
  AirLoopHVAC airLoopHVAC5(model);

  std::vector<std::string> modelObjectHandles;
  modelObjectHandles.push_back(openstudio::toString(airLoopHVAC1.handle()));
  modelObjectHandles.push_back(openstudio::toString(airLoopHVAC2.handle()));
  modelObjectHandles.push_back(openstudio::toString(airLoopHVAC3.handle()));
  modelObjectHandles.push_back(openstudio::toString(airLoopHVAC4.handle()));
  modelObjectHandles.push_back(openstudio::toString(airLoopHVAC5.handle()));

  // DLM: have to sort before calling unique, unique only works on consecutive elements
  std::sort(modelObjectHandles.begin(), modelObjectHandles.end());
  auto end = std::unique(modelObjectHandles.begin(), modelObjectHandles.end());
  std::vector<std::string> uniqueModelObjectHandles(modelObjectHandles.begin(), end);

  EXPECT_EQ(modelObjectHandles.size(), uniqueModelObjectHandles.size());

  std::vector<AirLoopHVAC> modelObjects = model.getConcreteModelObjects<AirLoopHVAC>();

  EXPECT_EQ(modelObjectHandles.size(), modelObjects.size());

  std::vector<std::string> workspaceObjectNames;
  workspaceObjectNames.push_back(airLoopHVAC1.name().get());
  workspaceObjectNames.push_back(airLoopHVAC2.name().get());
  workspaceObjectNames.push_back(airLoopHVAC3.name().get());
  workspaceObjectNames.push_back(airLoopHVAC4.name().get());
  workspaceObjectNames.push_back(airLoopHVAC5.name().get());

  // DLM: have to sort before calling unique, unique only works on consecutive elements
  std::sort(workspaceObjectNames.begin(), workspaceObjectNames.end());
  end = std::unique(workspaceObjectNames.begin(), workspaceObjectNames.end());
  std::vector<std::string> uniqueWorkspaceObjectNames(workspaceObjectNames.begin(), end);

  EXPECT_EQ(modelObjects.size(), uniqueWorkspaceObjectNames.size());

  for (const std::string& name : uniqueWorkspaceObjectNames) {
    EXPECT_NE(name, "");
  }

  airLoopHVAC1.setName("one");
  airLoopHVAC2.setName("two");
  airLoopHVAC3.setName("three");
  airLoopHVAC4.setName("four");
  airLoopHVAC5.setName("five");

  EXPECT_EQ(airLoopHVAC1.name().get(), "one");
  EXPECT_EQ(airLoopHVAC2.name().get(), "two");
  EXPECT_EQ(airLoopHVAC3.name().get(), "three");
  EXPECT_EQ(airLoopHVAC4.name().get(), "four");
  EXPECT_EQ(airLoopHVAC5.name().get(), "five");
}
/*
// Tests undo of a remove operation
TEST_F(ModelFixture, UndoRemove)
{

  Workspace workspace(idfFile);

  // translate energyplus simulation to building model
  energyplus::ReverseTranslator reverseTranslator(workspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  // find plenum zone
  OptionalWorkspaceObject zoneObject = model.getObjectByTypeAndName(IddObjectType::Zone, "PLENUM-1");
  ASSERT_TRUE(zoneObject);
  OptionalZone plenumZone = zoneObject->optionalCast<Zone>();
  ASSERT_TRUE(plenumZone);

  // find surface in zone
  OptionalWorkspaceObject surfaceObject = model.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, "WALL-1PF");
  ASSERT_TRUE(surfaceObject);
  OptionalSurface plenumSurface = surfaceObject->optionalCast<Surface>();
  ASSERT_TRUE(plenumSurface);

  // delete plenum zone and check we can restore it
  IdfObjectVector removedObjects = plenumZone->remove();
  EXPECT_TRUE(removedObjects.size() > 2);

  // check that zone and surface are now gone
  zoneObject = model.getObjectByTypeAndName(IddObjectType::Zone, "PLENUM-1");
  EXPECT_FALSE(zoneObject);
  surfaceObject = model.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, "WALL-1PF");
  EXPECT_FALSE(surfaceObject);

  // add objects back to model
  model.addObjects(removedObjects);

  // check that zone and surface are now found
  zoneObject = model.getObjectByTypeAndName(IddObjectType::Zone, "PLENUM-1");
  EXPECT_TRUE(zoneObject);
  surfaceObject = model.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, "WALL-1PF");
  EXPECT_TRUE(surfaceObject);
}
*/
/*
// Tests undo of an add
TEST_F(ModelFixture, UndoAdd)
{
  Workspace workspace(idfFile);

  // translate energyplus simulation to building model
  energyplus::ReverseTranslator reverseTranslator(workspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  // check that zone and surface are not in original model
  EXPECT_EQ(static_cast<unsigned>(6), model.getModelObjects<Zone>().size());
  OptionalWorkspaceObject zoneObject = model.getObjectByTypeAndName(IddObjectType::Zone, "Classroom");
  EXPECT_FALSE(zoneObject);
  OptionalWorkspaceObject surfaceObject = model.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, "Classroom Floor");
  EXPECT_FALSE(surfaceObject);
  WorkspaceObjectVector buildingObjects = model.getObjectsByType(IddObjectType::Building);
  EXPECT_EQ(static_cast<unsigned>(1), buildingObjects.size());

  // load new model
  openstudio::path newPath = resourcesPath() / toPath("energyplus/Daylighting_School/in.idf");
  OptionalIdfFile newIdf = openstudio::IdfFile::load(newPath, openstudio::IddFileType::EnergyPlus);
  ASSERT_TRUE(newIdf);

  // translate energyplus simulation to building model
  reverseTranslator = energyplus::ReverseTranslator(Workspace(*newIdf));
  OptionalModel optionalNewModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalNewModel);
  Model newModel(*optionalNewModel);

  // check that zone and surface are in new model
  EXPECT_EQ(static_cast<unsigned>(1), newModel.getModelObjects<Zone>().size());
  zoneObject = newModel.getObjectByTypeAndName(IddObjectType::Zone, "Classroom");
  ASSERT_TRUE(zoneObject);
  OptionalZone classroomZone = zoneObject->optionalCast<Zone>();
  ASSERT_TRUE(classroomZone);
  surfaceObject = newModel.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, "Classroom Floor");
  EXPECT_TRUE(surfaceObject);
  buildingObjects = newModel.getObjectsByType(IddObjectType::Building);
  EXPECT_EQ(static_cast<unsigned>(1), buildingObjects.size());

  // get the zone and children
  HandleVector newHandles = getHandles<ModelObject>(getRecursiveChildren(*classroomZone));
  EXPECT_TRUE(newHandles.size() > 2);
  Workspace newWorkspace = newModel.cloneSubset(newHandles).cast<Model>();
  EXPECT_EQ(static_cast<unsigned>(1),newWorkspace.getObjectsByType(IddObjectType::Zone).size());
  zoneObject = newWorkspace.getObjectByTypeAndName(IddObjectType::Zone, "Classroom");
  EXPECT_TRUE(zoneObject);
  surfaceObject = newWorkspace.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, "Classroom Floor");
  EXPECT_TRUE(surfaceObject);
  buildingObjects = newWorkspace.getObjectsByType(IddObjectType::Building);
  EXPECT_EQ(static_cast<unsigned>(0), buildingObjects.size());

  // add new model to old model
  WorkspaceObjectVector added = model.addObjects(newWorkspace.objects());
  EXPECT_EQ(static_cast<unsigned>(7), model.getModelObjects<Zone>().size());
  zoneObject = model.getObjectByTypeAndName(IddObjectType::Zone, "Classroom");
  EXPECT_TRUE(zoneObject);
  surfaceObject = model.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, "Classroom Floor");
  EXPECT_TRUE(surfaceObject);
  buildingObjects = newModel.getObjectsByType(IddObjectType::Building);
  EXPECT_EQ(static_cast<unsigned>(1), buildingObjects.size());

  // remove added objects
  model.removeObjects(getHandles<WorkspaceObject>(added));
  EXPECT_EQ(static_cast<unsigned>(6), model.getModelObjects<Zone>().size());
  zoneObject = model.getObjectByTypeAndName(IddObjectType::Zone, "Classroom");
  EXPECT_FALSE(zoneObject);
  surfaceObject = model.getObjectByTypeAndName(IddObjectType::BuildingSurface_Detailed, "Classroom Floor");
  EXPECT_FALSE(surfaceObject);
  buildingObjects = newModel.getObjectsByType(IddObjectType::Building);
  EXPECT_EQ(static_cast<unsigned>(1), buildingObjects.size());
}
*/

class ExampleModelFixture : public ModelFixture
{
 protected:
  // By default, SetUp() inherits the behavior of ModelFixture::SetUp(). In both cases, nothing to be done

  void addPathToCleanUp(const openstudio::path& p) {
    m_cleanUpPaths.push_back(p);
  }

  // We override the teardown to make sure we clean up any files we output to the root directory
  void TearDown() override {

    for (const openstudio::path& p : m_cleanUpPaths) {
      if (openstudio::filesystem::exists(p)) {
        openstudio::filesystem::remove(p);
      }
    }
    // Call ModelFixture::TearDown, which does nothing (right now)
    ModelFixture::TearDown();
  }

 private:
  std::vector<openstudio::path> m_cleanUpPaths;
};

TEST_F(ExampleModelFixture, ExampleModel) {
  Model model = exampleModel();

  boost::optional<Building> building = model.getOptionalUniqueModelObject<Building>();
  ASSERT_TRUE(building);

  std::vector<ThermalZone> thermalZones = model.getConcreteModelObjects<ThermalZone>();
  std::vector<BuildingStory> buildingStories = model.getConcreteModelObjects<BuildingStory>();
  std::vector<SpaceType> spaceTypes = model.getConcreteModelObjects<SpaceType>();
  std::vector<DefaultConstructionSet> defaultConstructionSets = model.getConcreteModelObjects<DefaultConstructionSet>();
  std::vector<DefaultScheduleSet> defaultScheduleSets = model.getConcreteModelObjects<DefaultScheduleSet>();
  std::vector<Space> spaces = model.getConcreteModelObjects<Space>();

  EXPECT_EQ(400, building->floorArea());
  EXPECT_FALSE(building->conditionedFloorArea());

  ASSERT_EQ(1u, thermalZones.size());
  ASSERT_EQ(1u, buildingStories.size());
  ASSERT_EQ(1u, spaceTypes.size());
  ASSERT_EQ(1u, defaultConstructionSets.size());
  ASSERT_EQ(1u, defaultScheduleSets.size());
  EXPECT_EQ(4u, spaces.size());

  for (const Space& space : spaces) {
    boost::optional<SpaceType> testSpaceType = space.spaceType();
    ASSERT_TRUE(testSpaceType);
    EXPECT_EQ(spaceTypes[0].handle(), testSpaceType->handle());
    EXPECT_EQ(10.0, space.lightingPowerPerFloorArea());
  }
}

TEST_F(ExampleModelFixture, ExampleModel_Save) {
  Model model = exampleModel();

  openstudio::path path = toPath("./ExampleModel_Save.osm");
  addPathToCleanUp(path);
  EXPECT_TRUE(model.save(path, true));

  boost::optional<Model> model2 = Model::load(path);
  ASSERT_TRUE(model2);

  EXPECT_EQ(model.numObjects(), model2->numObjects());
}

TEST_F(ExampleModelFixture, ExampleModel_StagedLoad) {
  Model model = exampleModel();
  openstudio::path path = toPath("./ExampleModel_StagedLoad.osm");
  addPathToCleanUp(path);
  model.save(path, true);

  IdfFile idf = IdfFile::load(path, IddFileType::OpenStudio).get();
  model = Model();
  WorkspaceObjectVector added = model.addObjects(idf.objects());
  // check that links between objects were kept
  ThermalZoneVector zones = model.getConcreteModelObjects<ThermalZone>();
  ASSERT_FALSE(zones.empty());
  EXPECT_FALSE(zones[0].spaces().empty());
}

TEST_F(ExampleModelFixture, ExampleModel_ReloadTwoTimes) {
  Model model = exampleModel();

  openstudio::path path1 = toPath("./ExampleModel_ReloadTwoTimes1.osm");
  openstudio::path path2 = toPath("./ExampleModel_ReloadTwoTimes2.osm");
  addPathToCleanUp(path1);
  addPathToCleanUp(path2);

  EXPECT_TRUE(model.save(path1, true));
  EXPECT_TRUE(model.save(path2, true));

  boost::optional<Model> model1 = Model::load(path1);
  ASSERT_TRUE(model1);

  boost::optional<Model> model2 = Model::load(path2);
  ASSERT_TRUE(model2);

  unsigned N = model.numObjects();
  ASSERT_EQ(N, model1->numObjects());
  ASSERT_EQ(N, model2->numObjects());

  // order of reloaded models should be the same as in memory model
  std::vector<WorkspaceObject> objects = model.objects();
  std::vector<WorkspaceObject> objects1 = model1->objects();
  std::vector<WorkspaceObject> objects2 = model2->objects();

  for (unsigned i = 0; i < N; ++i) {
    EXPECT_EQ(objects[i].handle(), objects1[i].handle());
    if (objects[i].name()) {
      // not every object has a name
      EXPECT_EQ(objects[i].name().get(), objects1[i].name().get());
    }
    EXPECT_EQ(objects[i].handle(), objects2[i].handle());
    if (objects[i].name()) {
      // not every object has a name
      EXPECT_EQ(objects[i].name().get(), objects2[i].name().get());
    }
  }
}

TEST_F(ModelFixture, Model_building) {
  Model model;

  boost::optional<Building> building = model.building();
  EXPECT_FALSE(building);

  model.getUniqueModelObject<Building>();

  building = model.building();
  EXPECT_TRUE(building);

  building = model.building();
  EXPECT_TRUE(building);

  building->remove();

  building = model.building();
  EXPECT_FALSE(building);
}

TEST_F(ModelFixture, MatchSurfaces) {
  std::stringstream testOSMString;
  testOSMString << "OS:Version,  \n\
    {7c2c8c1a-908e-44c0-82c1-ba2dba9143c9},  ! Handle  \n\
    0.8.2;                    ! Version Identifier  \n\
    \n\
  OS:BuildingStory,  \n\
    {1c3874dd-9792-4481-aa3e-77765386357b},  ! Handle  \n\
    Story 1,                  ! Name  \n\
    0,                        ! Nominal Z Coordinate {m}  \n\
    4.572,                    ! Nominal Floor to Floor Height {m}  \n\
    ,                         ! Default Construction Set Name  \n\
    ,                         ! Default Schedule Set Name  \n\
    {dae209d3-6045-4450-b8c0-d975460b635a};  ! Group Rendering Name  \n\
    \n\
  OS:Space,  \n\
    {abfced1d-d204-4b9d-9f7b-fcd3012c83c2},  ! Handle  \n\
    Story 1 East Perimeter Space,  ! Name  \n\
    ,                         ! Space Type Name  \n\
    ,                         ! Default Construction Set Name  \n\
    ,                         ! Default Schedule Set Name  \n\
    -0,                       ! Direction of Relative North {deg}  \n\
    24.5364,                  ! X Origin {m}  \n\
    3.6576,                   ! Y Origin {m}  \n\
    0,                        ! Z Origin {m}  \n\
    {1c3874dd-9792-4481-aa3e-77765386357b},  ! Building Story Name  \n\
    {9c659e9d-4b93-4caa-94ef-991bc0ab6e13},  ! Thermal Zone Name  \n\
    ,                         ! Part of Total Floor Area  \n\
    ;                         ! Design Specification Outdoor Air Object Name  \n\
    \n\
  OS:Surface,  \n\
    {bfa04394-167d-4497-b0bf-338b9127c8b4},  ! Handle  \n\
    OS:Surface 16,            ! Name  \n\
    Wall,                     ! Surface Type  \n\
    ,                         ! Construction Name  \n\
    {abfced1d-d204-4b9d-9f7b-fcd3012c83c2},  ! Space Name  \n\
    Outdoors,                 ! Outside Boundary Condition  \n\
    ,                         ! Outside Boundary Condition Object  \n\
    SunExposed,               ! Sun Exposure  \n\
    WindExposed,              ! Wind Exposure  \n\
    ,                         ! View Factor to Ground  \n\
    ,                         ! Number of Vertices  \n\
    0, 42.291, 4.572,                    ! X,Y,Z Vertex 1 {m}  \n\
    0, 42.291, 0,                        ! X,Y,Z Vertex 2 {m}  \n\
    0, 0, 0,                        ! X,Y,Z Vertex 3 {m}  \n\
    0, 0, 4.572;                    ! X,Y,Z Vertex 4 {m}  \n\
    \n\
  OS:Space,  \n\
    {05bfa3a7-dcce-49c6-9e7e-08dad784ca11},  ! Handle  \n\
    Story 1 Core Space,       ! Name  \n\
    ,                         ! Space Type Name  \n\
    ,                         ! Default Construction Set Name  \n\
    ,                         ! Default Schedule Set Name  \n\
    -0,                       ! Direction of Relative North {deg}  \n\
    3.6576,                   ! X Origin {m}  \n\
    3.6576,                   ! Y Origin {m}  \n\
    0,                        ! Z Origin {m}  \n\
    {1c3874dd-9792-4481-aa3e-77765386357b},  ! Building Story Name  \n\
    {1efd18ee-3d5a-4d92-956e-0149c44eab1e},  ! Thermal Zone Name  \n\
    ,                         ! Part of Total Floor Area  \n\
    ;                         ! Design Specification Outdoor Air Object Name  \n\
    \n\
  OS:Surface,  \n\
    {51604eb7-4e13-42a0-94d3-c59dd3604432},  ! Handle  \n\
    OS:Surface 28,            ! Name  \n\
    Wall,                     ! Surface Type  \n\
    ,                         ! Construction Name  \n\
    {05bfa3a7-dcce-49c6-9e7e-08dad784ca11},  ! Space Name  \n\
    Outdoors,                 ! Outside Boundary Condition  \n\
    ,                         ! Outside Boundary Condition Object  \n\
    SunExposed,               ! Sun Exposure  \n\
    WindExposed,              ! Wind Exposure  \n\
    ,                         ! View Factor to Ground  \n\
    ,                         ! Number of Vertices  \n\
    20.8788, 0, 4.572,                    ! X,Y,Z Vertex 1 {m}  \n\
    20.8788, 0, 0,                        ! X,Y,Z Vertex 2 {m}  \n\
    20.8788, 42.291, 0,                        ! X,Y,Z Vertex 3 {m}  \n\
    20.8788, 42.291, 4.572;                    ! X,Y,Z Vertex 4 {m}  \n\
    \n\
  OS:ThermalZone,  \n\
    {9c659e9d-4b93-4caa-94ef-991bc0ab6e13},  ! Handle  \n\
    OS:ThermalZone 1,         ! Name  \n\
    ,                         ! Multiplier  \n\
    ,                         ! Ceiling Height {m}  \n\
    ,                         ! Volume {m3}  \n\
    ,                         ! Floor Area {m2}  \n\
    ,                         ! Zone Inside Convection Algorithm  \n\
    ,                         ! Zone Outside Convection Algorithm  \n\
    ,                         ! Zone Conditioning Equipment List Name  \n\
    ,                         ! Zone Air Inlet Node or NodeList Name  \n\
    {fa9f1259-32d5-40ab-8703-ec3b93bc329a},  ! Zone Air Exhaust Node or NodeList Name  \n\
    {1410c573-d478-4940-99cd-b0a228ec9315},  ! Zone Air Node Name  \n\
    ,                         ! Zone Return Air Node Name  \n\
    ,                         ! Primary Daylighting Control Name  \n\
    ,                         ! Fraction of Zone Controlled by Primary Daylighting Control  \n\
    ,                         ! Secondary Daylighting Control Name  \n\
    ,                         ! Fraction of Zone Controlled by Secondary Daylighting Control  \n\
    ,                         ! Illuminance Map Name  \n\
    {31e448e8-f3e4-4ef8-af9e-5346ede4b7bc},  ! Group Rendering Name  \n\
    ,                         ! Thermostat Name  \n\
    No;                       ! Use Ideal Air Loads  \n\
    \n\
  OS:ThermalZone,  \n\
    {1efd18ee-3d5a-4d92-956e-0149c44eab1e},  ! Handle  \n\
    OS:ThermalZone 2,         ! Name  \n\
    ,                         ! Multiplier  \n\
    ,                         ! Ceiling Height {m}  \n\
    ,                         ! Volume {m3}  \n\
    ,                         ! Floor Area {m2}  \n\
    ,                         ! Zone Inside Convection Algorithm  \n\
    ,                         ! Zone Outside Convection Algorithm  \n\
    ,                         ! Zone Conditioning Equipment List Name  \n\
    ,                         ! Zone Air Inlet Node or NodeList Name  \n\
    {f270b029-1b22-43ec-8737-164e6179b194},  ! Zone Air Exhaust Node or NodeList Name  \n\
    {728b762b-b497-404a-a961-7c03b6df9a9c},  ! Zone Air Node Name  \n\
    ,                         ! Zone Return Air Node Name  \n\
    ,                         ! Primary Daylighting Control Name  \n\
    ,                         ! Fraction of Zone Controlled by Primary Daylighting Control  \n\
    ,                         ! Secondary Daylighting Control Name  \n\
    ,                         ! Fraction of Zone Controlled by Secondary Daylighting Control  \n\
    ,                         ! Illuminance Map Name  \n\
    {10a4c9f9-b642-4a19-a859-bfe1c3a99108},  ! Group Rendering Name  \n\
    ,                         ! Thermostat Name  \n\
    No;                       ! Use Ideal Air Loads  \n\
    \n\
  OS:Facility,  \n\
    {7d83a5a4-1ead-4f8a-a3a5-a41618e9e279};  ! Handle  \n\
    \n\
  OS:Building,  \n\
    {f089bb6e-9e07-482e-9a6a-1d342138bf33},  ! Handle  \n\
    OS:Building 1,            ! Name  \n\
    ,                         ! Building Type  \n\
    ,                         ! North Axis {deg}  \n\
    ,                         ! Nominal Floor to Floor Height {m}  \n\
    ,                         ! Space Type Name  \n\
    ,                         ! Default Construction Set Name  \n\
    ;                         ! Default Schedule Set Name  \n\
    \n\
  OS:Rendering:Color,  \n\
    {31e448e8-f3e4-4ef8-af9e-5346ede4b7bc},  ! Handle  \n\
    OS:Rendering:Color 1,     ! Name  \n\
    255,                      ! Rendering Red Value  \n\
    250,                      ! Rendering Green Value  \n\
    250;                      ! Rendering Blue Value  \n\
    \n\
  OS:Rendering:Color,  \n\
    {10a4c9f9-b642-4a19-a859-bfe1c3a99108},  ! Handle  \n\
    OS:Rendering:Color 2,     ! Name  \n\
    100,                      ! Rendering Red Value  \n\
    149,                      ! Rendering Green Value  \n\
    237;                      ! Rendering Blue Value  \n\
    \n\
  OS:Rendering:Color,  \n\
    {dae209d3-6045-4450-b8c0-d975460b635a},  ! Handle  \n\
    OS:Rendering:Color 6,     ! Name  \n\
    240,                      ! Rendering Red Value  \n\
    248,                      ! Rendering Green Value  \n\
    255;                      ! Rendering Blue Value";

  IdfFile idfFile = IdfFile::load(testOSMString, IddFileType(IddFileType::OpenStudio)).get();
  Model model(idfFile);

  //get ahold of the surfaces we want to match
  boost::optional<ModelObject> surface16OptModelObj = model.getModelObjectByName<ModelObject>("OS:Surface 16");
  boost::optional<ModelObject> surface28OptModelObj = model.getModelObjectByName<ModelObject>("OS:Surface 28");

  //make sure it read in the .osm correctly
  ASSERT_TRUE(surface16OptModelObj);
  ASSERT_TRUE(surface28OptModelObj);

  //change the surfaces to Surface class
  auto surface16 = surface16OptModelObj.get().cast<Surface>();
  auto surface28 = surface28OptModelObj.get().cast<Surface>();

  //before surface matching, both walls should have exterior boundary condition
  EXPECT_EQ("Outdoors", surface16.outsideBoundaryCondition());
  EXPECT_EQ("Outdoors", surface28.outsideBoundaryCondition());

  //match surfaces
  std::vector<Space> spaces = model.getConcreteModelObjects<Space>();
  matchSurfaces(spaces);

  //both walls should now be matched
  //before surface matching, both walls should have exterior boundary condition
  EXPECT_EQ("Surface", surface16.outsideBoundaryCondition());
  EXPECT_EQ("Surface", surface28.outsideBoundaryCondition());

  //ASSERT_TRUE(zone->name());
  //EXPECT_EQ("Zone", zone->name().get());
  //ASSERT_EQ(static_cast<unsigned>(1), zone->sources().size());
  //EXPECT_EQ(wall->handle(), zone->sources()[0].handle());
  //
  //ASSERT_TRUE(wall->name());
  //EXPECT_EQ("Wall", wall->name().get());
  //ASSERT_EQ(static_cast<unsigned>(1), wall->sources().size());
  //EXPECT_EQ(window->handle(), wall->sources()[0].handle());
  //ASSERT_TRUE(wall->getTarget(Wall_ExteriorFields::ZoneName));
  //EXPECT_EQ(zone->handle(), wall->getTarget(Wall_ExteriorFields::ZoneName)->handle());
  //
  //ASSERT_TRUE(window->getString(WindowFields::BuildingSurfaceName));
  //EXPECT_EQ("Wall", window->getString(WindowFields::BuildingSurfaceName).get());
  //ASSERT_TRUE(window->getTarget(WindowFields::BuildingSurfaceName));
  //ASSERT_TRUE(window->getTarget(WindowFields::BuildingSurfaceName)->name());
  //EXPECT_EQ("Wall", window->getTarget(WindowFields::BuildingSurfaceName)->name().get());
}

TEST_F(ModelFixture, Model_BadSwaps) {
  Workspace workspace;
  Model model;

  EXPECT_ANY_THROW(workspace.swap(model));
  EXPECT_ANY_THROW(model.swap(workspace));
}

TEST_F(ModelFixture, Ensure_Name_Unicity_SpaceAndSpaceGroupNames) {
  // Starting in 9.6.0, Space and SpaceList are supported.
  // Zone, ZoneList, Space, SpaceList all need to be unique names
  Model m;

  std::vector<ModelObject> mos{Space{m}, BuildingStory{m}, SpaceType{m}, ThermalZone{m}};
  EXPECT_EQ(4, m.getObjectsByReference("SpaceAndSpaceGroupNames").size());

  std::string name = "A Name";

  std::vector<std::pair<size_t, size_t>> combinations{
    {0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3},
  };

  auto resetNames = [&mos]() {
    for (auto& mo : mos) {
      mo.setName(mo.iddObject().name());
    }
  };

  for (auto& [i1, i2] : combinations) {
    resetNames();  // Starting point: all names are unique
    // We set two names: first one should work
    auto s1_ = mos[i1].setName(name);
    ASSERT_TRUE(s1_);
    EXPECT_EQ(name, s1_.get());
    // Second should be modified to keep unicity of names
    auto s2_ = mos[i2].setName(name);
    ASSERT_TRUE(s2_);
    EXPECT_NE(name, s2_.get());
    EXPECT_NE(s1_.get(), s2_.get());
    EXPECT_NE(mos[i1].nameString(), mos[i2].nameString());
  }
}

TEST_F(ModelFixture, Issue_4372) {

  // Open OffsetTests
  // INtersect surfaces
  // Match Surfaces
  // Check the internal walls are paired
  // Surface 4 Space 101 with SPace 102 surface 12
  // Surface 2 Space 102 with

  openstudio::path modelPath = resourcesPath() / "model" / toPath("offset_tests.osm");
  ASSERT_TRUE(openstudio::filesystem::exists(modelPath));

  openstudio::osversion::VersionTranslator vt;
  boost::optional<openstudio::model::Model> model = vt.loadModel(modelPath);
  ASSERT_TRUE(model);

  std::vector<Space> spaces = model->getConcreteModelObjects<Space>();
  intersectSurfaces(spaces);

  matchSurfaces(spaces);
  std::vector<Surface> surfacesAfter = model->getConcreteModelObjects<Surface>();
  for (const auto& surface : surfacesAfter) {

    std::string name = surface.name().value();
    OptionalSurface otherSurface;
    if (name == "Surface 4") {
      otherSurface = surface.adjacentSurface();
      ASSERT_TRUE(otherSurface);
      EXPECT_EQ(otherSurface->nameString(), "Surface 8");
    } else if (name == "Surface 10") {
      otherSurface = surface.adjacentSurface();
      ASSERT_TRUE(otherSurface);
      ASSERT_EQ(otherSurface->name().value(), "Surface 14");
    } else if (name == "Surface 16") {
      otherSurface = surface.adjacentSurface();
      ASSERT_TRUE(otherSurface);
      ASSERT_EQ(otherSurface->name().value(), "Surface 20");
    } else if (name == "Surface 22") {
      otherSurface = surface.adjacentSurface();
      ASSERT_TRUE(otherSurface);
      ASSERT_EQ(otherSurface->name().value(), "Surface 26");
    } else if (name == "Surface 28") {
      otherSurface = surface.adjacentSurface();
      ASSERT_TRUE(otherSurface);
      ASSERT_EQ(otherSurface->name().value(), "Surface 32");
    }

    // if (otherSurface) {
    //   LOG(Info, "Surface " << surface.name().value() << " is paired with " << otherSurface->name().value());
    // }
  }

  // modelPath = resourcesPath() / "model" / toPath("offset_tests_matched.osm");
  // model->save(modelPath, true);
}

TEST_F(ModelFixture, UniqueModelObjectCachedGetters) {
  Model m;
  unsigned i = 0;
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<Building>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto building = m.getUniqueModelObject<Building>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<Building>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<FoundationKivaSettings>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto foundationKivaSettings = m.getUniqueModelObject<FoundationKivaSettings>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<FoundationKivaSettings>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputControlFiles>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto outputControlFiles = m.getUniqueModelObject<OutputControlFiles>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<OutputControlFiles>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputControlReportingTolerances>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto outputControlReportingTolerances = m.getUniqueModelObject<OutputControlReportingTolerances>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<OutputControlReportingTolerances>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputControlTableStyle>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto outputControlTableStyle = m.getUniqueModelObject<OutputControlTableStyle>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<OutputControlTableStyle>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputDiagnostics>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto outputDiagnostics = m.getUniqueModelObject<OutputDiagnostics>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<OutputDiagnostics>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputDebuggingData>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto outputDebuggingData = m.getUniqueModelObject<OutputDebuggingData>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<OutputDebuggingData>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputJSON>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto outputJSON = m.getUniqueModelObject<OutputJSON>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<OutputJSON>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputSQLite>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto outputSQLite = m.getUniqueModelObject<OutputSQLite>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<OutputSQLite>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputEnergyManagementSystem>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto outputEnergyManagementSystem = m.getUniqueModelObject<OutputEnergyManagementSystem>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<OutputEnergyManagementSystem>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputTableSummaryReports>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto outputTableSummaryReports = m.getUniqueModelObject<OutputTableSummaryReports>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<OutputTableSummaryReports>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<PerformancePrecisionTradeoffs>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto performancePrecisionTradeoffs = m.getUniqueModelObject<PerformancePrecisionTradeoffs>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<PerformancePrecisionTradeoffs>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<LifeCycleCostParameters>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto lifeCycleCostParameters = m.getUniqueModelObject<LifeCycleCostParameters>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<LifeCycleCostParameters>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<SizingParameters>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto sizingParameters = m.getUniqueModelObject<SizingParameters>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<SizingParameters>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<RadianceParameters>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto radianceParameters = m.getUniqueModelObject<RadianceParameters>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<RadianceParameters>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<RunPeriod>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto runPeriod = m.getUniqueModelObject<RunPeriod>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<RunPeriod>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto runPeriodControlDaylightSavingTime = m.getUniqueModelObject<RunPeriodControlDaylightSavingTime>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<RunPeriodControlDaylightSavingTime>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<model::YearDescription>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto yearDescription = m.getUniqueModelObject<model::YearDescription>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<model::YearDescription>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<Site>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto site = m.getUniqueModelObject<Site>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<Site>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<SiteGroundReflectance>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto siteGroundReflectance = m.getUniqueModelObject<SiteGroundReflectance>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<SiteGroundReflectance>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<SiteWaterMainsTemperature>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto siteWaterMainsTemperature = m.getUniqueModelObject<SiteWaterMainsTemperature>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<SiteWaterMainsTemperature>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<SiteGroundTemperatureBuildingSurface>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto siteGroundTemperatureBuildingSurface = m.getUniqueModelObject<SiteGroundTemperatureBuildingSurface>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<SiteGroundTemperatureBuildingSurface>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<SiteGroundTemperatureFCfactorMethod>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto siteGroundTemperatureFCfactorMethod = m.getUniqueModelObject<SiteGroundTemperatureFCfactorMethod>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<SiteGroundTemperatureFCfactorMethod>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<SiteGroundTemperatureDeep>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto siteGroundTemperatureDeep = m.getUniqueModelObject<SiteGroundTemperatureDeep>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<SiteGroundTemperatureDeep>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<SiteGroundTemperatureShallow>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto siteGroundTemperatureShallow = m.getUniqueModelObject<SiteGroundTemperatureShallow>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<SiteGroundTemperatureShallow>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<Facility>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto facility = m.getUniqueModelObject<Facility>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<Facility>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<WeatherFile>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto weatherFile = m.getUniqueModelObject<WeatherFile>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<WeatherFile>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<SimulationControl>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto simulationControl = m.getUniqueModelObject<SimulationControl>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<SimulationControl>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<LightingSimulationControl>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto lightingSimulationControl = m.getUniqueModelObject<LightingSimulationControl>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<LightingSimulationControl>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<AirflowNetworkSimulationControl>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto airflowNetworkSimulationControl = m.getUniqueModelObject<AirflowNetworkSimulationControl>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<AirflowNetworkSimulationControl>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<InsideSurfaceConvectionAlgorithm>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto insideSurfaceConvectionAlgorithm = m.getUniqueModelObject<InsideSurfaceConvectionAlgorithm>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<InsideSurfaceConvectionAlgorithm>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutsideSurfaceConvectionAlgorithm>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto outsideSurfaceConvectionAlgorithm = m.getUniqueModelObject<OutsideSurfaceConvectionAlgorithm>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<OutsideSurfaceConvectionAlgorithm>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<HeatBalanceAlgorithm>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto heatBalanceAlgorithm = m.getUniqueModelObject<HeatBalanceAlgorithm>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<HeatBalanceAlgorithm>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<ZoneAirHeatBalanceAlgorithm>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto zoneAirHeatBalanceAlgorithm = m.getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<ZoneAirHeatBalanceAlgorithm>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<ZoneAirMassFlowConservation>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto zoneAirMassFlowConservation = m.getUniqueModelObject<ZoneAirMassFlowConservation>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<ZoneAirMassFlowConservation>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto zoneCapacitanceMultiplierResearchSpecial = m.getUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<ZoneCapacitanceMultiplierResearchSpecial>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<ConvergenceLimits>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto convergenceLimits = m.getUniqueModelObject<ConvergenceLimits>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<ConvergenceLimits>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<ShadowCalculation>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto shadowCalculation = m.getUniqueModelObject<ShadowCalculation>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<ShadowCalculation>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<Timestep>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto timestep = m.getUniqueModelObject<Timestep>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<Timestep>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<ClimateZones>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto climateZones = m.getUniqueModelObject<ClimateZones>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<ClimateZones>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<EnvironmentalImpactFactors>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto environmentImpactFactors = m.getUniqueModelObject<EnvironmentalImpactFactors>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<EnvironmentalImpactFactors>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<ExternalInterface>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto externalInterface = m.getUniqueModelObject<ExternalInterface>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<ExternalInterface>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());

  EXPECT_FALSE(m.getOptionalUniqueModelObject<PythonPluginSearchPaths>());
  EXPECT_EQ(i, m.getModelObjects<ModelObject>().size());
  auto pythonPluginSearchPaths = m.getUniqueModelObject<PythonPluginSearchPaths>();
  EXPECT_TRUE(m.getOptionalUniqueModelObject<PythonPluginSearchPaths>());
  EXPECT_EQ(++i, m.getModelObjects<ModelObject>().size());
}

TEST_F(ModelFixture, Model_load) {

  openstudio::path modelPath = resourcesPath() / "model" / toPath("empty361.osm");
  ASSERT_TRUE(openstudio::filesystem::exists(modelPath));

  const openstudio::path workflowJSONPath = getCompanionFolder(modelPath) / toPath("workflow.osw");
  ASSERT_TRUE(openstudio::filesystem::exists(workflowJSONPath));

  // Check that the versionObject is indeed translated to the current version
  boost::optional<Model> model_ = Model::load(modelPath);
  ASSERT_TRUE(model_);
  auto versionObject_ = model_->versionObject();
  ASSERT_TRUE(versionObject_);
  EXPECT_EQ(openStudioVersion(), versionObject_->getString(1).get());

  // Check that the workflowJSON in the companion folder is correctly loaded
  auto workflowJSON = model_->workflowJSON();
  auto p_ = workflowJSON.oswPath();
  ASSERT_TRUE(p_);
  EXPECT_EQ(workflowJSONPath, *p_);

  ASSERT_TRUE(workflowJSON.seedFile());
  EXPECT_EQ(workflowJSON.seedFile().get(), openstudio::toPath("../empty361.osm"));
}
