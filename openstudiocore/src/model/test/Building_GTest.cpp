/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/geometry/Geometry.hpp"

#include <math.h>

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

TEST_F(ModelFixture, Building_Rotations)
{
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
  EXPECT_EQ(7, m.getModelObjects<Node>().size());
  // Zone Inlet Port List, Zone Return Air Port List, Zone Air Exhaust Port List
  EXPECT_EQ(3, m.getModelObjects<PortList>().size());
  EXPECT_EQ(1, m.getModelObjects<ThermalZone>().size());

  m.getUniqueModelObject<Building>().remove();
  // 4 basic airLoopHVAC Nodes plus the Drop node between demand splitter and mixer
  EXPECT_EQ(5, m.getModelObjects<Node>().size());
  // Zone Inlet Port List, Zone Return Air Port List, Zone Air Exhaust Port List
  EXPECT_EQ(0, m.getModelObjects<PortList>().size());
  EXPECT_EQ(0, m.getModelObjects<ThermalZone>().size());

  // TODO: JM 2019-05-13 Once HVAC is handled (#2449), we should adjust this portion.
  // For now this tests that at least we don't end up with bad connections
  ASSERT_EQ(1, m.getModelObjects<AirLoopHVAC>().size());
  EXPECT_NO_THROW(m.getModelObjects<AirLoopHVAC>()[0].components());

}

TEST_F(ModelFixture, Building_remove_exampleModel) {

  Model m = exampleModel();

  EXPECT_NE(0, m.getModelObjects<Space>().size());
  EXPECT_NE(0, m.getModelObjects<ThermalZone>().size());
  EXPECT_NE(0, m.getModelObjects<ShadingSurfaceGroup>().size());
  EXPECT_NE(0, m.getModelObjects<BuildingStory>().size());

  // Creates three meters, but facility-level ones only
  unsigned ori_meters = m.getModelObjects<OutputMeter>().size();
  // Add one Building level one
  OutputMeter meter(m);
  meter.setName("Electricity:Building");
  EXPECT_EQ(InstallLocationType::Building, meter.installLocationType().get().value());

  EXPECT_NO_THROW(m.getUniqueModelObject<Building>().remove());

  EXPECT_EQ(0, m.getModelObjects<Space>().size());
  EXPECT_EQ(0, m.getModelObjects<ThermalZone>().size());

  // There is one Site Shading group that should be left untouched
  // The other two are Space and Building level, so should be removed
  EXPECT_EQ(1, m.getModelObjects<ShadingSurfaceGroup>().size());
  EXPECT_EQ(0, m.getModelObjects<BuildingStory>().size());
  EXPECT_EQ(ori_meters, m.getModelObjects<OutputMeter>().size());

  // TODO: JM 2019-05-13 Once HVAC is handled (#2449), we should adjust this portion.
  // For now this tests that at least we don't end up with bad connections
  ASSERT_EQ(1, m.getModelObjects<AirLoopHVAC>().size());
  EXPECT_NO_THROW(m.getModelObjects<AirLoopHVAC>()[0].components());
}
