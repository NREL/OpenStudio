/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Building.hpp"
#include "../Building_Impl.hpp"

#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"
#include "../ShadingSurfaceGroup.hpp"
#include "../ShadingSurfaceGroup_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../ElectricEquipment.hpp"
#include "../ElectricEquipmentDefinition.hpp"
#include "../GasEquipment.hpp"
#include "../GasEquipmentDefinition.hpp"
#include "../Lights.hpp"
#include "../LightsDefinition.hpp"
#include "../People.hpp"
#include "../PeopleDefinition.hpp"
#include "../Schedule.hpp"
#include "../LifeCycleCost.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../PortList.hpp"
#include "../PortList_Impl.hpp"
#include "../OutputMeter.hpp"
#include "../OutputMeter_Impl.hpp"
#include "../DefaultScheduleSet.hpp"
#include "../ScheduleConstant.hpp"

#include "../../utilities/geometry/Geometry.hpp"
#include "../../osversion/VersionTranslator.hpp"

#include <math.h>
#include <algorithm>

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, Building) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  EXPECT_EQ(0, building.northAxis());
  EXPECT_FALSE(building.getDouble(2));
  EXPECT_TRUE(building.isNorthAxisDefaulted());
  EXPECT_FALSE(building.nominalFloortoFloorHeight());
  EXPECT_FALSE(building.getDouble(3));

  OptionalString buildingName = building.name();
  ASSERT_TRUE(buildingName);
  EXPECT_EQ("Building 1", *buildingName);

  building.setName("Edificio");

  buildingName = building.name();
  ASSERT_TRUE(buildingName);
  EXPECT_EQ("Edificio", *buildingName);
}

TEST_F(ModelFixture, Building_SpaceAttributes) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  EXPECT_EQ(0, building.floorArea());
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_EQ(0, building.lightingPower());
  EXPECT_EQ(0, building.lightingPowerPerFloorArea());
  EXPECT_EQ(0, building.peoplePerFloorArea());

  Space space(model);

  // floor
  Point3dVector points{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };
  Surface floor(points, model);
  floor.setParent(space);
  EXPECT_EQ("Floor", floor.surfaceType());
  EXPECT_NEAR(100, space.floorArea(), 0.0001);

  EXPECT_NEAR(100, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_EQ(0, building.lightingPower());
  EXPECT_EQ(0, building.lightingPowerPerFloorArea());
  EXPECT_EQ(0, building.peoplePerFloorArea());

  LightsDefinition lightsDefinition(model);
  Lights light(lightsDefinition);
  EXPECT_TRUE(light.setSpace(space));
  EXPECT_TRUE(lightsDefinition.setLightingLevel(100));

  PeopleDefinition peopleDefinition(model);
  People person(peopleDefinition);
  EXPECT_TRUE(person.setSpace(space));
  EXPECT_TRUE(peopleDefinition.setNumberofPeople(1));

  EXPECT_NEAR(100, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_NEAR(100, building.lightingPower(), 0.0001);
  EXPECT_NEAR(1, building.lightingPowerPerFloorArea(), 0.0001);
  EXPECT_NEAR(1.0 / 100.0, building.peoplePerFloorArea(), 0.0001);

  EXPECT_TRUE(light.setMultiplier(2));
  EXPECT_TRUE(person.setMultiplier(2));

  EXPECT_NEAR(100, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_NEAR(200, building.lightingPower(), 0.0001);
  EXPECT_NEAR(2, building.lightingPowerPerFloorArea(), 0.0001);
  EXPECT_NEAR(2.0 / 100.0, building.peoplePerFloorArea(), 0.0001);

  ThermalZone thermalZone(model);
  EXPECT_TRUE(thermalZone.setMultiplier(2));
  EXPECT_TRUE(space.setThermalZone(thermalZone));
  EXPECT_EQ(2, space.multiplier());

  EXPECT_NEAR(200, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_NEAR(400, building.lightingPower(), 0.0001);
  EXPECT_NEAR(2, building.lightingPowerPerFloorArea(), 0.0001);
  EXPECT_NEAR(2.0 / 100.0, building.peoplePerFloorArea(), 0.0001);
}

TEST_F(ModelFixture, Building_SpaceTypeAttributes) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType(model);
  EXPECT_TRUE(building.setSpaceType(spaceType));
  Space space(model);

  EXPECT_EQ(0, building.floorArea());
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_EQ(0, building.lightingPower());
  EXPECT_EQ(0, building.lightingPowerPerFloorArea());
  EXPECT_EQ(0, building.peoplePerFloorArea());

  // floor
  Point3dVector points{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };
  Surface floor(points, model);
  floor.setParent(space);
  EXPECT_EQ("Floor", floor.surfaceType());
  EXPECT_NEAR(100, space.floorArea(), 0.0001);

  EXPECT_NEAR(100, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_EQ(0, building.lightingPower());
  EXPECT_EQ(0, building.lightingPowerPerFloorArea());
  EXPECT_EQ(0, building.peoplePerFloorArea());

  LightsDefinition lightsDefinition(model);
  Lights light(lightsDefinition);
  EXPECT_TRUE(light.setSpaceType(spaceType));
  EXPECT_TRUE(lightsDefinition.setLightingLevel(100));

  PeopleDefinition peopleDefinition(model);
  People person(peopleDefinition);
  EXPECT_TRUE(person.setSpaceType(spaceType));
  EXPECT_TRUE(peopleDefinition.setNumberofPeople(1));

  EXPECT_NEAR(100, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_NEAR(100, building.lightingPower(), 0.0001);
  EXPECT_NEAR(1, building.lightingPowerPerFloorArea(), 0.0001);
  EXPECT_NEAR(1.0 / 100.0, building.peoplePerFloorArea(), 0.0001);

  EXPECT_TRUE(light.setMultiplier(2));
  EXPECT_TRUE(person.setMultiplier(2));

  EXPECT_NEAR(100, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_NEAR(200, building.lightingPower(), 0.0001);
  EXPECT_NEAR(2, building.lightingPowerPerFloorArea(), 0.0001);
  EXPECT_NEAR(2.0 / 100.0, building.peoplePerFloorArea(), 0.0001);

  ThermalZone thermalZone(model);
  EXPECT_TRUE(thermalZone.setMultiplier(2));
  EXPECT_TRUE(space.setThermalZone(thermalZone));
  EXPECT_EQ(2, space.multiplier());

  EXPECT_NEAR(200, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_NEAR(400, building.lightingPower(), 0.0001);
  EXPECT_NEAR(2, building.lightingPowerPerFloorArea(), 0.0001);
  EXPECT_NEAR(2.0 / 100.0, building.peoplePerFloorArea(), 0.0001);
}

TEST_F(ModelFixture, Building_SpaceType) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  SpaceType spaceType(model);
  BuildingStory buildingStory(model);
  Space space(model);
  space.setBuildingStory(buildingStory);

  EXPECT_FALSE(building.spaceType());
  EXPECT_FALSE(space.spaceType());

  EXPECT_TRUE(building.setSpaceType(spaceType));

  ASSERT_TRUE(building.spaceType());
  EXPECT_EQ(spaceType.handle(), building.spaceType()->handle());
  EXPECT_TRUE(space.spaceType());
  EXPECT_TRUE(space.isSpaceTypeDefaulted());
}

TEST_F(ModelFixture, Building_StandardsInformation) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  EXPECT_FALSE(building.standardsNumberOfStories());
  building.setStandardsNumberOfStories(3);
  ASSERT_TRUE(building.standardsNumberOfStories());
  EXPECT_EQ(3, building.standardsNumberOfStories().get());
}

TEST_F(ModelFixture, Building_Cost) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space);

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Cleaning", building, 1, "CostPerArea", "Maintenance", 1, 0);
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Design", building, 100, "CostPerEach", "Construction", 0, 0);
  ASSERT_TRUE(cost2);
  boost::optional<LifeCycleCost> cost3 = LifeCycleCost::createLifeCycleCost("Design", building, 60, "CostPerThermalZone", "Construction", 0, 0);
  ASSERT_TRUE(cost3);

  EXPECT_DOUBLE_EQ(100, cost1->totalCost());
  EXPECT_DOUBLE_EQ(100, cost2->totalCost());
  EXPECT_DOUBLE_EQ(0, cost3->totalCost());

  ThermalZone thermalZone(model);
  space->setThermalZone(thermalZone);

  EXPECT_DOUBLE_EQ(100, cost1->totalCost());
  EXPECT_DOUBLE_EQ(100, cost2->totalCost());
  EXPECT_DOUBLE_EQ(60, cost3->totalCost());

  thermalZone.setMultiplier(2);

  EXPECT_DOUBLE_EQ(200, cost1->totalCost());
  EXPECT_DOUBLE_EQ(100, cost2->totalCost());
  EXPECT_DOUBLE_EQ(120, cost3->totalCost());
}

TEST_F(ModelFixture, Building_Clone) {
  // Remember that Building is a unique object
  // There are basically three scenarios to consider,
  // cloning into the same model, cloning into a different model that already has a building object,
  // and cloning into a different model that doesn't have a building object.

  // Clone into the same model
  {
    Model model;

    auto building = model.getUniqueModelObject<Building>();
    auto buildingClone = building.clone(model);

    // Don't expect cloning into the same model to do anything.
    // Just return the Building instance
    EXPECT_EQ(building, buildingClone);
  }

  // Clone into a different model that already has a Building instance.
  {
    Model model;
    Model library;

    auto modelBuilding = model.getUniqueModelObject<Building>();
    auto libraryBuilding = library.getUniqueModelObject<Building>();

    Space space1(library);
    Space space2(library);

    BuildingStory story(library);
    space1.setBuildingStory(story);
    space2.setBuildingStory(story);

    ThermalZone zone(library);

    space1.setThermalZone(zone);
    space2.setThermalZone(zone);

    EXPECT_NE(modelBuilding, libraryBuilding);

    // Expect that the original modelBuilding will be removed and replaced by a clone of the libraryBuilding
    auto clone = libraryBuilding.clone(model);

    EXPECT_TRUE(modelBuilding.handle().isNull());
    EXPECT_NE(modelBuilding, clone);

    EXPECT_EQ(1u, model.getConcreteModelObjects<Building>().size());
    EXPECT_EQ(2u, model.getConcreteModelObjects<Space>().size());

    auto zones = model.getConcreteModelObjects<ThermalZone>();

    ASSERT_EQ(1u, zones.size());
    EXPECT_EQ(2u, zones.front().spaces().size());

    auto stories = model.getConcreteModelObjects<BuildingStory>();
    ASSERT_EQ(1u, stories.size());
    ASSERT_EQ(2u, stories.front().spaces().size());
  }

  // Clone into a different model that does not already have a Building instance.
  {
    Model model;
    Model library;

    auto libraryBuilding = library.getUniqueModelObject<Building>();

    Space space1(library);
    Space space2(library);

    BuildingStory story(library);
    space1.setBuildingStory(story);
    space2.setBuildingStory(story);

    ThermalZone zone(library);

    space1.setThermalZone(zone);
    space2.setThermalZone(zone);

    EXPECT_EQ(1u, library.getConcreteModelObjects<Building>().size());
    EXPECT_EQ(2u, library.getConcreteModelObjects<Space>().size());
    EXPECT_EQ(1u, library.getConcreteModelObjects<ThermalZone>().size());
    EXPECT_EQ(1u, library.getConcreteModelObjects<BuildingStory>().size());
    EXPECT_EQ(2u, zone.spaces().size());

    auto modelBuilding = libraryBuilding.clone(model).cast<Building>();

    EXPECT_NE(libraryBuilding, modelBuilding);

    EXPECT_EQ(1u, model.getConcreteModelObjects<Building>().size());
    EXPECT_EQ(2u, model.getConcreteModelObjects<Space>().size());

    auto zones = model.getConcreteModelObjects<ThermalZone>();

    ASSERT_EQ(1u, zones.size());
    EXPECT_EQ(2u, zones.front().spaces().size());

    auto stories = model.getConcreteModelObjects<BuildingStory>();
    ASSERT_EQ(1u, stories.size());
    ASSERT_EQ(2u, stories.front().spaces().size());
  }
}

TEST_F(ModelFixture, Building_Rotations) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  Space space(model);
  ShadingSurfaceGroup siteGroup(model);
  EXPECT_TRUE(siteGroup.setShadingSurfaceType("Site"));
  ShadingSurfaceGroup buildingGroup(model);
  EXPECT_TRUE(buildingGroup.setShadingSurfaceType("Building"));
  ShadingSurfaceGroup spaceGroup(model);
  EXPECT_TRUE(spaceGroup.setSpace(space));

  double degrees = 0;
  building.setNorthAxis(degrees);

  EXPECT_NEAR(cos(degToRad(degrees)), building.transformation().matrix()(0, 0), 0.0001);

  EXPECT_NEAR(1, space.transformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(1, space.buildingTransformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(cos(degToRad(degrees)), space.siteTransformation().matrix()(0, 0), 0.0001);

  EXPECT_NEAR(1, siteGroup.transformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(cos(degToRad(-degrees)), siteGroup.buildingTransformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(1, siteGroup.siteTransformation().matrix()(0, 0), 0.0001);

  EXPECT_NEAR(1, buildingGroup.transformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(1, buildingGroup.buildingTransformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(cos(degToRad(degrees)), buildingGroup.siteTransformation().matrix()(0, 0), 0.0001);

  EXPECT_NEAR(1, spaceGroup.transformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(1, spaceGroup.buildingTransformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(cos(degToRad(degrees)), spaceGroup.siteTransformation().matrix()(0, 0), 0.0001);

  degrees = 20;
  building.setNorthAxis(degrees);

  EXPECT_NEAR(cos(degToRad(degrees)), building.transformation().matrix()(0, 0), 0.0001);

  EXPECT_NEAR(1, space.transformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(1, space.buildingTransformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(cos(degToRad(degrees)), space.siteTransformation().matrix()(0, 0), 0.0001);

  EXPECT_NEAR(1, siteGroup.transformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(cos(degToRad(-degrees)), siteGroup.buildingTransformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(1, siteGroup.siteTransformation().matrix()(0, 0), 0.0001);

  EXPECT_NEAR(1, buildingGroup.transformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(1, buildingGroup.buildingTransformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(cos(degToRad(degrees)), buildingGroup.siteTransformation().matrix()(0, 0), 0.0001);

  EXPECT_NEAR(1, spaceGroup.transformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(1, spaceGroup.buildingTransformation().matrix()(0, 0), 0.0001);
  EXPECT_NEAR(cos(degToRad(degrees)), spaceGroup.siteTransformation().matrix()(0, 0), 0.0001);
}

TEST_F(ModelFixture, Building_remove) {

  Model m;
  ThermalZone z(m);
  AirLoopHVAC a(m);

  a.addBranchForZone(z);
  // 4 basic AirLoopHVAC nodes (supply/demand inlet/outlet Nodes)
  // One AirLoopHVAC branch node before and one after ThermalZones
  // 1 Zone Air Node
  EXPECT_EQ(7, m.getConcreteModelObjects<Node>().size());
  // Zone Inlet Port List, Zone Return Air Port List, Zone Air Exhaust Port List
  EXPECT_EQ(3, m.getConcreteModelObjects<PortList>().size());
  EXPECT_EQ(1, m.getConcreteModelObjects<ThermalZone>().size());

  m.getUniqueModelObject<Building>().remove();
  // 4 basic airLoopHVAC Nodes plus the Drop node between demand splitter and mixer
  EXPECT_EQ(5, m.getConcreteModelObjects<Node>().size());
  // Zone Inlet Port List, Zone Return Air Port List, Zone Air Exhaust Port List
  EXPECT_EQ(0, m.getConcreteModelObjects<PortList>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<ThermalZone>().size());

  // TODO: JM 2019-05-13 Once HVAC is handled (#2449), we should adjust this portion.
  // For now this tests that at least we don't end up with bad connections
  ASSERT_EQ(1, m.getConcreteModelObjects<AirLoopHVAC>().size());
  EXPECT_NO_THROW(m.getConcreteModelObjects<AirLoopHVAC>()[0].components());
}

TEST_F(ModelFixture, Building_remove_exampleModel) {

  Model m = exampleModel();

  EXPECT_NE(0, m.getConcreteModelObjects<Space>().size());
  EXPECT_NE(0, m.getConcreteModelObjects<ThermalZone>().size());
  EXPECT_NE(0, m.getConcreteModelObjects<ShadingSurfaceGroup>().size());
  EXPECT_NE(0, m.getConcreteModelObjects<BuildingStory>().size());

  // Creates three meters, but facility-level ones only
  unsigned ori_meters = m.getConcreteModelObjects<OutputMeter>().size();
  // Add one Building level one
  OutputMeter meter(m);
  meter.setName("Electricity:Building");
  EXPECT_EQ(InstallLocationType::Building, meter.installLocationType().get().value());

  EXPECT_NO_THROW(m.getUniqueModelObject<Building>().remove());

  EXPECT_EQ(0, m.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<ThermalZone>().size());

  // There is one Site Shading group that should be left untouched
  // The other two are Space and Building level, so should be removed
  EXPECT_EQ(1, m.getConcreteModelObjects<ShadingSurfaceGroup>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<BuildingStory>().size());
  EXPECT_EQ(ori_meters, m.getConcreteModelObjects<OutputMeter>().size());

  // TODO: JM 2019-05-13 Once HVAC is handled (#2449), we should adjust this portion.
  // For now this tests that at least we don't end up with bad connections
  ASSERT_EQ(1, m.getConcreteModelObjects<AirLoopHVAC>().size());
  EXPECT_NO_THROW(m.getConcreteModelObjects<AirLoopHVAC>()[0].components());
}

TEST_F(ModelFixture, Building_getDefaultSchedule) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  DefaultScheduleSet bldgDefaultScheduleSet(model);
  ScheduleConstant sch_bldg_people(model);
  EXPECT_TRUE(bldgDefaultScheduleSet.setNumberofPeopleSchedule(sch_bldg_people));
  EXPECT_TRUE(building.setDefaultScheduleSet(bldgDefaultScheduleSet));

  SpaceType spaceType(model);
  EXPECT_TRUE(building.setSpaceType(spaceType));

  DefaultScheduleSet spDefaultScheduleSet(model);
  ScheduleConstant sch_sp_people(model);
  EXPECT_TRUE(spDefaultScheduleSet.setNumberofPeopleSchedule(sch_sp_people));
  ScheduleConstant sch_sp_hours(model);
  EXPECT_TRUE(spDefaultScheduleSet.setHoursofOperationSchedule(sch_sp_hours));
  EXPECT_TRUE(spaceType.setDefaultScheduleSet(spDefaultScheduleSet));

  // Building and its SpaceType both have a people schedule. It should return the building's one in priority
  ASSERT_TRUE(building.getDefaultSchedule(DefaultScheduleType::NumberofPeopleSchedule));
  EXPECT_EQ(sch_bldg_people.handle(), building.getDefaultSchedule(DefaultScheduleType::NumberofPeopleSchedule)->handle());

  // Building doesn't have an hours of operation schedule, but its SpaceType does so it should return the SpaceType's one
  ASSERT_TRUE(building.getDefaultSchedule(DefaultScheduleType::HoursofOperationSchedule));
  EXPECT_EQ(sch_sp_hours.handle(), building.getDefaultSchedule(DefaultScheduleType::HoursofOperationSchedule)->handle());
}

// DA

boost::optional<Model> CreatePerimeterTestModel1() {
  Model model;

  ThermalZone tz1(model);

  BuildingStory story1(model);

  Point3dVector p1{
    {36, 168, 0},
    {36, 138, 0},
    {0, 138, 0},
    {0, 168, 0},
  };
  auto sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Classroom 101");

  p1 = {
    {220, 168, 0}, {220, 150, 0}, {220, 124, 0}, {220, 70, 0}, {220, 30, 0}, {200, 30, 0}, {200, 168, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Mechanical 114");

  p1 = {
    {288, 70, 0}, {288, 0, 0}, {220, 0, 0}, {220, 30, 0}, {220, 70, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Offices 117");

  p1 = {
    {288, 124, 0},
    {288, 70, 0},
    {220, 70, 0},
    {220, 124, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Gym 118");

  p1 = {
    {158, 168, 0},
    {158, 138, 0},
    {98, 138, 0},
    {98, 168, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Artroom 103");

  p1 = {
    {288, 150, 0},
    {288, 124, 0},
    {220, 124, 0},
    {220, 150, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Kitchen 119");

  p1 = {
    {288, 200, 0}, {288, 150, 0}, {220, 150, 0}, {220, 168, 0}, {220, 200, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Cafeteria 120");

  p1 = {
    {158, 138, 0}, {158, 128, 0}, {36, 128, 0}, {0, 128, 0}, {0, 138, 0}, {36, 138, 0}, {98, 138, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Corridor 104");

  p1 = {
    {36, 128, 0},
    {36, 98, 0},
    {0, 98, 0},
    {0, 128, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Classroom 105");

  p1 = {
    {158, 128, 0},
    {158, 98, 0},
    {36, 98, 0},
    {36, 128, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Classroom 106");

  p1 = {
    {158, 30, 0}, {158, 40, 0}, {158, 70, 0}, {158, 98, 0}, {158, 128, 0}, {158, 138, 0}, {158, 168, 0}, {200, 168, 0}, {200, 30, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Corridor 107");

  p1 = {
    {36, 40, 0},
    {36, 70, 0},
    {158, 70, 0},
    {158, 40, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Classroom 108");

  p1 = {
    {0, 40, 0},
    {0, 70, 0},
    {36, 70, 0},
    {36, 40, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Classroom 109");

  p1 = {
    {98, 168, 0},
    {98, 138, 0},
    {36, 138, 0},
    {36, 168, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Classroom 102");

  p1 = {
    {158, 40, 0}, {158, 30, 0}, {36, 30, 0}, {0, 30, 0}, {0, 40, 0}, {36, 40, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Corridor 110");

  p1 = {
    {36, 30, 0},
    {36, 0, 0},
    {0, 0, 0},
    {0, 30, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Classroom 111");

  p1 = {
    {158, 30, 0},
    {158, 0, 0},
    {36, 0, 0},
    {36, 30, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Classroom 112");

  p1 = {
    {158, 0, 0}, {158, 30, 0}, {200, 30, 0}, {220, 30, 0}, {220, 0, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Lobby 113");

  p1 = {
    {220, 200, 0}, {220, 168, 0}, {200, 168, 0}, {158, 168, 0}, {158, 200, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Bathroom 115");

  p1 = {
    {220, 268, 0},
    {220, 200, 0},
    {158, 200, 0},
    {158, 268, 0},
  };
  sp1 = Space::fromFloorPrint(p1, 13.125, model);
  sp1->setBuildingStory(story1);
  sp1->setThermalZone(tz1);
  sp1->setName("Media Center 116");

  return model;
}

TEST_F(ModelFixture, Building_exteriorPerimeter) {
  // Open a test model
  model::OptionalModel model = CreatePerimeterTestModel1();

  EXPECT_TRUE(model);

  auto buildings = model->getConcreteModelObjects<Building>();
  for (const auto& building : buildings) {
    double perimeter = building.exteriorPerimeter();
    ASSERT_NEAR(perimeter, 1428.0, 0.01);
  }
}

TEST_F(ModelFixture, Building_clone_SurfaceMatching) {
  Model m = exampleModel();
  Building b = m.getUniqueModelObject<Building>();

  auto surfaces = m.getConcreteModelObjects<Surface>();
  auto nMatched = std::count_if(surfaces.cbegin(), surfaces.cend(), [](const auto& s) { return s.adjacentSurface(); });
  EXPECT_EQ(8, nMatched);

  Model m2;
  auto bClone = b.clone(m2);
  auto surfaceClones = m2.getConcreteModelObjects<Surface>();
  EXPECT_EQ(surfaces.size(), surfaceClones.size());
  auto nMatchedClone = std::count_if(surfaceClones.cbegin(), surfaceClones.cend(), [](const auto& s) { return s.adjacentSurface(); });
  EXPECT_EQ(nMatched, nMatchedClone);
}
