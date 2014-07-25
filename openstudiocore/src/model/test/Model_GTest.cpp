/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "../../utilities/sql/SqlFile.hpp"
#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/ValidityReport.hpp"

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
    std::cout << "Need to add wrapper for '" << object.iddObject().name() << "' to Model::getModelObject" << std::endl;
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
    // be capable of providing all the potentional output variables and the currently available variables
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
    for (ModelObject child : parentObject->children()){
      OptionalParentObject parent = child.parent();
      if (!parent){
        std::cout << "Child " << child << " does not have a parent" << std::endl;
      }
      ASSERT_TRUE(parent);
      EXPECT_TRUE(*parentObject == *parent);
      if (*parentObject != *parent){
        std::cout << "pcObject = " << *parentObject << " does not equal parent = " << *parent << std::endl;
      }
      checkObject(child);
    }
  }
}
*/

TEST_F(ModelFixture, iddObjectType)
{

  Model model;
  model.order().setDirectOrder(HandleVector());
  ScheduleCompact s(model);
  FanConstantVolume fan(model, s); // sets schedule's ScheduleTypeLimits

  ModelObjectVector mos = model.getModelObjects<ModelObject>(true); // sort objects
  ASSERT_EQ(3u,mos.size());
  EXPECT_TRUE(ScheduleCompact::iddObjectType() == mos[0].iddObjectType());
  EXPECT_TRUE(FanConstantVolume::iddObjectType() == mos[1].iddObjectType());
  EXPECT_TRUE(ScheduleTypeLimits::iddObjectType() == mos[2].iddObjectType());
}

TEST_F(ModelFixture,IddFile)
{
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
TEST_F(ModelFixture, UniqueModelObjects)
{
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
  std::sort(modelObjectHandles.begin(),modelObjectHandles.end());
  std::vector<std::string>::iterator end = std::unique(modelObjectHandles.begin(),modelObjectHandles.end());
  std::vector<std::string> uniqueModelObjectHandles(modelObjectHandles.begin(),end);

  EXPECT_EQ(modelObjectHandles.size(),uniqueModelObjectHandles.size());

  std::vector<AirLoopHVAC> modelObjects = model.getModelObjects<AirLoopHVAC>();

  EXPECT_EQ(modelObjectHandles.size(),modelObjects.size());

  std::vector<std::string> workspaceObjectNames;
  workspaceObjectNames.push_back(airLoopHVAC1.name().get());
  workspaceObjectNames.push_back(airLoopHVAC2.name().get());
  workspaceObjectNames.push_back(airLoopHVAC3.name().get());
  workspaceObjectNames.push_back(airLoopHVAC4.name().get());
  workspaceObjectNames.push_back(airLoopHVAC5.name().get());

  // DLM: have to sort before calling unique, unique only works on consecutive elements
  std::sort(workspaceObjectNames.begin(),workspaceObjectNames.end());
  end = std::unique(workspaceObjectNames.begin(),workspaceObjectNames.end());
  std::vector<std::string> uniqueWorkspaceObjectNames(workspaceObjectNames.begin(),end);

  EXPECT_EQ(modelObjects.size(),uniqueWorkspaceObjectNames.size());

  for (const std::string& name : uniqueWorkspaceObjectNames)
  {
    EXPECT_NE(name,"");
  }

  airLoopHVAC1.setName("one");
  airLoopHVAC2.setName("two");
  airLoopHVAC3.setName("three");
  airLoopHVAC4.setName("four");
  airLoopHVAC5.setName("five");

  EXPECT_EQ(airLoopHVAC1.name().get(),"one");
  EXPECT_EQ(airLoopHVAC2.name().get(),"two");
  EXPECT_EQ(airLoopHVAC3.name().get(),"three");
  EXPECT_EQ(airLoopHVAC4.name().get(),"four");
  EXPECT_EQ(airLoopHVAC5.name().get(),"five");

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

TEST_F(ModelFixture, ExampleModel)
{
  Model model = exampleModel();

  boost::optional<Building> building = model.getOptionalUniqueModelObject<Building>();
  ASSERT_TRUE(building);

  std::vector<ThermalZone> thermalZones = model.getModelObjects<ThermalZone>();
  std::vector<BuildingStory> buildingStories = model.getModelObjects<BuildingStory>();
  std::vector<SpaceType> spaceTypes = model.getModelObjects<SpaceType>();
  std::vector<DefaultConstructionSet> defaultConstructionSets = model.getModelObjects<DefaultConstructionSet>();
  std::vector<DefaultScheduleSet> defaultScheduleSets = model.getModelObjects<DefaultScheduleSet>();
  std::vector<Space> spaces = model.getModelObjects<Space>();

  EXPECT_EQ(400, building->floorArea());
  EXPECT_FALSE(building->conditionedFloorArea());

  ASSERT_EQ(1u, thermalZones.size());
  ASSERT_EQ(1u, buildingStories.size());
  ASSERT_EQ(1u, spaceTypes.size());
  ASSERT_EQ(1u, defaultConstructionSets.size());
  ASSERT_EQ(1u, defaultScheduleSets.size());
  EXPECT_EQ(4u, spaces.size());

  for (const Space& space : spaces){
    boost::optional<SpaceType> testSpaceType = space.spaceType();
    ASSERT_TRUE(testSpaceType);
    EXPECT_EQ(spaceTypes[0].handle(), testSpaceType->handle());
    EXPECT_EQ(10.0, space.lightingPowerPerFloorArea());
  }
}

TEST_F(ModelFixture, ExampleModel_Save)
{
  Model model = exampleModel();

  openstudio::path path = toPath("./ExampleModel_Save.osm");
  if(boost::filesystem::exists(path)){
    boost::filesystem::remove(path);
  }

  EXPECT_TRUE(model.save(path, true));

  boost::optional<Model> model2 = Model::load(path);
  ASSERT_TRUE(model2);

  EXPECT_EQ(model.numObjects(), model2->numObjects());
}

TEST_F(ModelFixture, ExampleModel_StagedLoad) {
  Model model = exampleModel();
  openstudio::path path = toPath("./example.osm");
  model.save(path,true);

  IdfFile idf = IdfFile::load(path,IddFileType::OpenStudio).get();
  model = Model();
  WorkspaceObjectVector added = model.addObjects(idf.objects());
  // check that links between objects were kept
  ThermalZoneVector zones = model.getModelObjects<ThermalZone>();
  ASSERT_FALSE(zones.empty());
  EXPECT_FALSE(zones[0].spaces().empty());
}

TEST_F(ModelFixture, ExampleModel_ReloadTwoTimes)
{
  Model model = exampleModel();

  openstudio::path path1 = toPath("./ExampleModel_ReloadTwoTimes1.osm");
  if(boost::filesystem::exists(path1)){
    boost::filesystem::remove(path1);
  }
  openstudio::path path2 = toPath("./ExampleModel_ReloadTwoTimes2.osm");
  if(boost::filesystem::exists(path2)){
    boost::filesystem::remove(path2);
  }

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

  for (unsigned i = 0; i < N; ++i){
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

TEST_F(ModelFixture, MatchSurfaces)
{
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

  IdfFile idfFile = IdfFile::load(testOSMString,IddFileType(IddFileType::OpenStudio)).get();
  Model model(idfFile);

  //get ahold of the surfaces we want to match
  boost::optional<ModelObject> surface16OptModelObj = model.getModelObjectByName<ModelObject>("OS:Surface 16");
  boost::optional<ModelObject> surface28OptModelObj = model.getModelObjectByName<ModelObject>("OS:Surface 28");

  //make sure it read in the .osm correctly
  ASSERT_TRUE(surface16OptModelObj);
  ASSERT_TRUE(surface28OptModelObj);

  //change the surfaces to Surface class
  Surface surface16 = surface16OptModelObj.get().cast<Surface>();
  Surface surface28 = surface28OptModelObj.get().cast<Surface>();

  //before surface matching, both walls should have exterior boundary condition
  EXPECT_EQ("Outdoors", surface16.outsideBoundaryCondition());
  EXPECT_EQ("Outdoors", surface28.outsideBoundaryCondition());

  //match surfaces
  std::vector<Space> spaces =  model.getModelObjects<Space>();
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

TEST_F(ModelFixture,Model_BadSwaps) {
  Workspace workspace;
  Model model;

  EXPECT_ANY_THROW(workspace.swap(model));
  EXPECT_ANY_THROW(model.swap(workspace));
}
