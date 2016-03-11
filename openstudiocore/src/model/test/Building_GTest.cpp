/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "../Building.hpp"
#include "../Building_Impl.hpp"

#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../BuildingStory.hpp"
#include "../BuildingStory_Impl.hpp"
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

#include "../../utilities/data/Attribute.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, Building)
{
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

TEST_F(ModelFixture, Building_SpaceAttributes)
{
  Model model;

  Building building = model.getUniqueModelObject<Building>();

  EXPECT_EQ(0, building.floorArea());
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_EQ(0, building.lightingPower());
  EXPECT_EQ(0, building.lightingPowerPerFloorArea());
  EXPECT_EQ(0, building.peoplePerFloorArea());

  Space space(model);

  // floor 
  Point3dVector points;
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(0, 0, 0));
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
  EXPECT_NEAR(1.0/100.0, building.peoplePerFloorArea(), 0.0001);

  EXPECT_TRUE(light.setMultiplier(2));
  EXPECT_TRUE(person.setMultiplier(2));

  EXPECT_NEAR(100, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_NEAR(200, building.lightingPower(), 0.0001);
  EXPECT_NEAR(2, building.lightingPowerPerFloorArea(), 0.0001);
  EXPECT_NEAR(2.0/100.0, building.peoplePerFloorArea(), 0.0001);

  ThermalZone thermalZone(model);
  EXPECT_TRUE(thermalZone.setMultiplier(2));
  EXPECT_TRUE(space.setThermalZone(thermalZone));
  EXPECT_EQ(2, space.multiplier());

  EXPECT_NEAR(200, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_NEAR(400, building.lightingPower(), 0.0001);
  EXPECT_NEAR(2, building.lightingPowerPerFloorArea(), 0.0001);
  EXPECT_NEAR(2.0/100.0, building.peoplePerFloorArea(), 0.0001);

}

TEST_F(ModelFixture, Building_SpaceTypeAttributes)
{
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
  Point3dVector points;
  points.push_back(Point3d(0, 10, 0));
  points.push_back(Point3d(10, 10, 0));
  points.push_back(Point3d(10, 0, 0));
  points.push_back(Point3d(0, 0, 0));
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
  EXPECT_NEAR(1.0/100.0, building.peoplePerFloorArea(), 0.0001);

  EXPECT_TRUE(light.setMultiplier(2));
  EXPECT_TRUE(person.setMultiplier(2));

  EXPECT_NEAR(100, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_NEAR(200, building.lightingPower(), 0.0001);
  EXPECT_NEAR(2, building.lightingPowerPerFloorArea(), 0.0001);
  EXPECT_NEAR(2.0/100.0, building.peoplePerFloorArea(), 0.0001);

  ThermalZone thermalZone(model);
  EXPECT_TRUE(thermalZone.setMultiplier(2));
  EXPECT_TRUE(space.setThermalZone(thermalZone));
  EXPECT_EQ(2, space.multiplier());

  EXPECT_NEAR(200, building.floorArea(), 0.0001);
  EXPECT_FALSE(building.conditionedFloorArea());
  EXPECT_NEAR(400, building.lightingPower(), 0.0001);
  EXPECT_NEAR(2, building.lightingPowerPerFloorArea(), 0.0001);
  EXPECT_NEAR(2.0/100.0, building.peoplePerFloorArea(), 0.0001);

}

TEST_F(ModelFixture, Building_SpaceType)
{
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

TEST_F(ModelFixture,Building_StandardsInformation) {
  Model model;
  Building building = model.getUniqueModelObject<Building>();
  EXPECT_FALSE(building.standardsNumberOfStories());
  building.setStandardsNumberOfStories(3);
  ASSERT_TRUE(building.standardsNumberOfStories());
  EXPECT_EQ(3,building.standardsNumberOfStories().get());
}


TEST_F(ModelFixture, Building_Cost)
{
  Model model;
  Building building = model.getUniqueModelObject<Building>();

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

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

TEST_F(ModelFixture, Building_Clone)
{
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
    EXPECT_EQ(building,buildingClone);
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

    EXPECT_NE(modelBuilding,libraryBuilding);

    // Expect that the original modelBuilding will be removed and replaced by a clone of the libraryBuilding
    auto clone = libraryBuilding.clone(model);

    EXPECT_TRUE(modelBuilding.handle().isNull());
    EXPECT_NE(modelBuilding,clone);

    EXPECT_EQ(1u,model.getModelObjects<Building>().size());
    EXPECT_EQ(2u,model.getModelObjects<Space>().size());

    auto zones = model.getModelObjects<ThermalZone>();

    ASSERT_EQ(1u,zones.size());
    EXPECT_EQ(2u,zones.front().spaces().size());

    auto stories = model.getModelObjects<BuildingStory>();
    ASSERT_EQ(1u,stories.size());
    ASSERT_EQ(2u,stories.front().spaces().size());
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

    EXPECT_EQ(1u,library.getModelObjects<Building>().size());
    EXPECT_EQ(2u,library.getModelObjects<Space>().size());
    EXPECT_EQ(1u,library.getModelObjects<ThermalZone>().size());
    EXPECT_EQ(1u,library.getModelObjects<BuildingStory>().size());
    EXPECT_EQ(2u,zone.spaces().size());

    auto modelBuilding = libraryBuilding.clone(model).cast<Building>();

    EXPECT_NE(libraryBuilding,modelBuilding);

    EXPECT_EQ(1u,model.getModelObjects<Building>().size());
    EXPECT_EQ(2u,model.getModelObjects<Space>().size());

    auto zones = model.getModelObjects<ThermalZone>();

    ASSERT_EQ(1u,zones.size());
    EXPECT_EQ(2u,zones.front().spaces().size());

    auto stories = model.getModelObjects<BuildingStory>();
    ASSERT_EQ(1u,stories.size());
    ASSERT_EQ(2u,stories.front().spaces().size());
  }
}

