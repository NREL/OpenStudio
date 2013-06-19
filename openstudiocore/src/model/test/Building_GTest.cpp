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

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>

#include <model/Building.hpp>
#include <model/Building_Impl.hpp>

#include <model/BuildingStandardsInformation.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/BuildingStory.hpp>
#include <model/BuildingStory_Impl.hpp>
#include <model/Space.hpp>
#include <model/Space_Impl.hpp>
#include <model/SpaceType.hpp>
#include <model/SpaceType_Impl.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/Surface.hpp>
#include <model/Surface_Impl.hpp>
#include <model/SubSurface.hpp>
#include <model/SubSurface_Impl.hpp>
#include <model/ElectricEquipment.hpp>
#include <model/ElectricEquipmentDefinition.hpp>
#include <model/GasEquipment.hpp>
#include <model/GasEquipmentDefinition.hpp>
#include <model/Lights.hpp>
#include <model/LightsDefinition.hpp>
#include <model/People.hpp>
#include <model/PeopleDefinition.hpp>
#include <model/Schedule.hpp>
#include <model/LifeCycleCost.hpp>

#include <utilities/data/Attribute.hpp>

#include <boost/foreach.hpp>

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, Building)
{
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  EXPECT_EQ("Commercial", building.buildingType());
  EXPECT_EQ(0, building.northAxis());
  EXPECT_FALSE(building.getDouble(2));
  EXPECT_TRUE(building.isNorthAxisDefaulted());
  EXPECT_EQ(3, building.nominalFloortoFloorHeight());
  EXPECT_FALSE(building.getDouble(3));
  EXPECT_TRUE(building.isNominalFloortoFloorHeightDefaulted());

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

  EXPECT_EQ(1u,model.numObjects());
  BuildingStandardsInformation standardsInformation = building.standardsInformation();
  EXPECT_EQ(2u,model.numObjects());
  EXPECT_FALSE(building.numberOfStories());
  standardsInformation.remove();
  EXPECT_EQ(1u,model.numObjects());

  building.setNumberOfStories(3);
  EXPECT_EQ(2u,model.numObjects());
  ASSERT_TRUE(building.numberOfStories());
  EXPECT_EQ(3,building.numberOfStories().get());

  ASSERT_TRUE(building.getAttribute("numberOfStories"));
  Attribute numberOfStories = building.getAttribute("numberOfStories").get();
  ASSERT_TRUE(numberOfStories.valueType() == AttributeValueType::Integer);
  EXPECT_EQ(3,numberOfStories.valueAsInteger());
  EXPECT_TRUE(building.setAttribute("numberOfStories", 1));
  ASSERT_TRUE(building.numberOfStories());
  EXPECT_EQ(1,building.numberOfStories().get());
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