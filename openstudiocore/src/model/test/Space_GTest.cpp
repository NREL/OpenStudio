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

#include "../Model_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
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
#include "../Lights_Impl.hpp"
#include "../LightsDefinition.hpp"
#include "../People.hpp"
#include "../PeopleDefinition.hpp"
#include "../Schedule.hpp"
#include "../LifeCycleCost.hpp"
#include "../SpaceInfiltrationDesignFlowRate.hpp"
#include "../AirLoopHVACSupplyPlenum.hpp"
#include "../AirLoopHVACReturnPlenum.hpp"

#include "../../utilities/core/UUID.hpp"

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"
#include "../../utilities/geometry/Transformation.hpp"
#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/BoundingBox.hpp"
#include "../../utilities/idf/WorkspaceObjectWatcher.hpp"
#include "../../utilities/core/Compare.hpp"

#include <iostream>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Space)
{
  Model model;

  Space space1(model);
  Space space2(model);

  ASSERT_TRUE(space1.name());
  ASSERT_TRUE(space2.name());
  EXPECT_NE("",space1.name().get());
  EXPECT_NE("",space2.name().get());
  EXPECT_NE(space1.name().get(),space2.name().get());
}

TEST_F(ModelFixture, Space_Clone)
{
  Model model;
  Space space(model);
  ASSERT_TRUE(space.name());
  EXPECT_EQ("Space 1", space.name().get());

  Point3dVector points;
  points.push_back(Point3d(0, 4, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(4, 0, 0));
  points.push_back(Point3d(4, 4, 0));

  Surface surface(points, model);
  surface.setParent(space);

  points.clear();
  points.push_back(Point3d(1, 3, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(3, 1, 0));
  points.push_back(Point3d(3, 3, 0));

  SubSurface subSurface(points, model);
  subSurface.setParent(surface);

  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<Space>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<Surface>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getModelObjects<SubSurface>().size());

  EXPECT_FALSE(space.parent());
  ASSERT_TRUE(surface.parent());
  EXPECT_EQ(space.handle(), surface.parent()->handle());
  ASSERT_TRUE(subSurface.parent());
  EXPECT_EQ(surface.handle(), subSurface.parent()->handle());

  ModelObject clone = space.clone(model);

  ASSERT_TRUE(clone.optionalCast<Space>());

  Space spaceClone = clone.cast<Space>();
  ASSERT_TRUE(spaceClone.name());
  EXPECT_EQ("Space 2", spaceClone.name().get());

  EXPECT_EQ(static_cast<unsigned>(2), model.getModelObjects<Space>().size());
  EXPECT_EQ(static_cast<unsigned>(2), model.getModelObjects<Surface>().size());
  EXPECT_EQ(static_cast<unsigned>(2), model.getModelObjects<SubSurface>().size());

  ASSERT_EQ(static_cast<unsigned>(1), spaceClone.surfaces().size());
  EXPECT_NE(surface.handle(), spaceClone.surfaces()[0].handle());
  ASSERT_EQ(static_cast<unsigned>(1), spaceClone.surfaces()[0].subSurfaces().size());
  EXPECT_NE(subSurface.handle(), spaceClone.surfaces()[0].subSurfaces()[0].handle());

}

TEST_F(ModelFixture, Space_FloorArea)
{
  Model model;
  Space space(model);

  EXPECT_EQ(0, space.floorArea());

  Point3dVector points;

  // walls
  points.clear();
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 1, 1));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  Surface westWall(points, model);
  westWall.setParent(space);
  EXPECT_EQ("Wall", westWall.surfaceType());

  EXPECT_EQ(0, space.floorArea());

  points.clear();
  points.push_back(Point3d(0, 1, 1));
  points.push_back(Point3d(1, 1, 1));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(0, 1, 0));
  Surface northWall(points, model);
  northWall.setParent(space);
  EXPECT_EQ("Wall", northWall.surfaceType());

  EXPECT_EQ(0, space.floorArea());

  points.clear();
  points.push_back(Point3d(1, 1, 1));
  points.push_back(Point3d(1, 0, 1));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));
  Surface eastWall(points, model);
  eastWall.setParent(space);
  EXPECT_EQ("Wall", eastWall.surfaceType());

  EXPECT_EQ(0, space.floorArea());

  points.clear();
  points.push_back(Point3d(1, 0, 1));
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  Surface southWall(points, model);
  southWall.setParent(space);
  EXPECT_EQ("Wall", southWall.surfaceType());

  EXPECT_EQ(0, space.floorArea());

  // roof 
  points.clear();
  points.push_back(Point3d(1, 1, 1));
  points.push_back(Point3d(0, 1, 1));
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(1, 0, 1));
  Surface roof(points, model);
  roof.setParent(space);
  EXPECT_EQ("RoofCeiling", roof.surfaceType());

  EXPECT_EQ(0, space.floorArea());

  // floor 
  points.clear();
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  Surface floor(points, model);
  floor.setParent(space);
  EXPECT_EQ("Floor", floor.surfaceType());

  EXPECT_NEAR(1, space.floorArea(), 0.0001);

  space.setPartofTotalFloorArea(false);

  EXPECT_NEAR(1, space.floorArea(), 0.0001);

  space.setPartofTotalFloorArea(true);

  EXPECT_NEAR(1, space.floorArea(), 0.0001);

  floor.setSurfaceType("Wall");

  EXPECT_EQ(0, space.floorArea());

  floor.setSurfaceType("Floor");
  westWall.setSurfaceType("Floor");
  northWall.setSurfaceType("Floor");
  eastWall.setSurfaceType("Floor");
  southWall.setSurfaceType("Floor");
  roof.setSurfaceType("Floor");

  EXPECT_NEAR(6, space.floorArea(), 0.0001);

  points.clear();
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  SubSurface floorWindow(points, model);
  floorWindow.setSurface(floor);
  EXPECT_EQ("Skylight", floorWindow.subSurfaceType());

  EXPECT_NEAR(6, space.floorArea(), 0.0001);
}

TEST_F(ModelFixture, Space_Attributes) 
{
  Model model;
  Space space(model);
  boost::optional<openstudio::Attribute> attribute;
  std::vector<Attribute> attributes = space.attributes();

  ASSERT_TRUE(space.name());
  std::string spaceName = space.name().get();

  EXPECT_TRUE(space.isSettableAttribute("name"));
  EXPECT_TRUE(space.isOptionalAttribute("name"));
  attribute = space.getAttribute("name");
  ASSERT_TRUE(attribute);
  EXPECT_EQ("name", attribute->name()); // from ModelObject
  EXPECT_EQ(spaceName, attribute->valueAsString());
  EXPECT_TRUE(space.setAttribute("name", "Office Space"));
  EXPECT_EQ("Office Space", space.name().get());

  EXPECT_FALSE(space.isSettableAttribute("floorArea"));
  EXPECT_FALSE(space.isOptionalAttribute("floorArea"));
  attribute = space.getAttribute("floorArea");
  ASSERT_TRUE(attribute);
  ASSERT_EQ("floorArea", attribute->name());
  EXPECT_NO_THROW(attribute->valueAsDouble());
  EXPECT_THROW(attribute->valueAsBoolean(),std::exception);
  EXPECT_FALSE(space.setAttribute("floorArea", 0.0));
}

TEST_F(ModelFixture, Space_ThermalZone)
{
  Model model;
  Space space(model);
  ThermalZone thermalZone1(model);
  ThermalZone thermalZone2(model);

  EXPECT_FALSE(space.thermalZone());
  EXPECT_TRUE(space.setThermalZone(thermalZone1));
  ASSERT_TRUE(space.thermalZone());
  EXPECT_EQ(thermalZone1.handle(), space.thermalZone()->handle());
  EXPECT_TRUE(space.setThermalZone(thermalZone2));
  ASSERT_TRUE(space.thermalZone());
  EXPECT_EQ(thermalZone2.handle(), space.thermalZone()->handle());

  EXPECT_EQ(space.multiplier(), thermalZone2.multiplier());
  EXPECT_FALSE(thermalZone2.setMultiplier(-1));
  EXPECT_TRUE(thermalZone2.setMultiplier(3));
  EXPECT_EQ(3, thermalZone2.multiplier());
  EXPECT_EQ(space.multiplier(), thermalZone2.multiplier());
}

TEST_F(ModelFixture, Space_Lights)
{
  Model model;
  Space space(model);

  EXPECT_EQ(0, space.lightingPower());
  EXPECT_EQ(0, space.lightingPowerPerFloorArea());

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

  EXPECT_EQ(0, space.lightingPower());
  EXPECT_EQ(0, space.lightingPowerPerFloorArea());

  // add lights
  LightsDefinition definition(model);
  Lights light(definition);
  EXPECT_TRUE(light.setSpace(space));

  EXPECT_EQ(0, space.lightingPower());
  EXPECT_EQ(0, space.lightingPowerPerFloorArea());

  EXPECT_TRUE(definition.setLightingLevel(100));
  EXPECT_EQ(100, space.lightingPower());
  EXPECT_EQ(1, space.lightingPowerPerFloorArea());

  EXPECT_TRUE(definition.setWattsperSpaceFloorArea(1));
  EXPECT_EQ(100, space.lightingPower());
  EXPECT_EQ(1, space.lightingPowerPerFloorArea());

  EXPECT_TRUE(definition.setWattsperPerson(100));
  EXPECT_EQ(0, space.lightingPower());
  EXPECT_EQ(0, space.lightingPowerPerFloorArea());

  PeopleDefinition peopleDefinition(model);
  People people(peopleDefinition);
  EXPECT_TRUE(people.setSpace(space));
  EXPECT_TRUE(peopleDefinition.setNumberofPeople(1));
  EXPECT_EQ(100, space.lightingPower());
  EXPECT_EQ(1, space.lightingPowerPerFloorArea());

  EXPECT_TRUE(definition.setLightingLevel(100));
  EXPECT_EQ(100, space.lightingPower());
  EXPECT_EQ(1, space.lightingPowerPerFloorArea());

  EXPECT_FALSE(light.setMultiplier(-1));
  EXPECT_TRUE(light.isMultiplierDefaulted());

  EXPECT_TRUE(light.setMultiplier(0));
  EXPECT_FALSE(light.isMultiplierDefaulted());
  EXPECT_EQ(0, space.lightingPower());
  EXPECT_EQ(0, space.lightingPowerPerFloorArea());

  EXPECT_TRUE(light.setMultiplier(1));
  EXPECT_FALSE(light.isMultiplierDefaulted());
  EXPECT_EQ(100, space.lightingPower());
  EXPECT_EQ(1, space.lightingPowerPerFloorArea());

  EXPECT_TRUE(light.setMultiplier(2));
  EXPECT_FALSE(light.isMultiplierDefaulted());
  EXPECT_EQ(200, space.lightingPower());
  EXPECT_EQ(2, space.lightingPowerPerFloorArea());

  definition.remove();
  EXPECT_EQ(0, space.lightingPower());
  EXPECT_EQ(0, space.lightingPowerPerFloorArea());
}


TEST_F(ModelFixture, Space_Transformation)
{
  Model model;
  Space space(model);
  Point3d origin(0,0,0);
  Point3d x1(1,0,0);

  Transformation transformation = space.transformation();

  Point3d test = transformation*origin;
  EXPECT_DOUBLE_EQ(0, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  test = transformation*x1;
  EXPECT_DOUBLE_EQ(1, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  EXPECT_TRUE(space.setTransformation(Transformation::translation(Vector3d(1,0,0))));
  EXPECT_DOUBLE_EQ(1, space.xOrigin());
  EXPECT_DOUBLE_EQ(0, space.yOrigin());
  EXPECT_DOUBLE_EQ(0, space.zOrigin());
  EXPECT_DOUBLE_EQ(0, space.directionofRelativeNorth());

  transformation = space.transformation();

  test = transformation*origin;
  EXPECT_DOUBLE_EQ(1, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  test = transformation*x1;
  EXPECT_DOUBLE_EQ(2, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  EXPECT_TRUE(space.setTransformation(Transformation::translation(Vector3d(1,0,0))*Transformation::rotation(Vector3d(0,0,1),-openstudio::degToRad(90))));
  EXPECT_DOUBLE_EQ(1, space.xOrigin());
  EXPECT_DOUBLE_EQ(0, space.yOrigin());
  EXPECT_DOUBLE_EQ(0, space.zOrigin());
  EXPECT_DOUBLE_EQ(90, space.directionofRelativeNorth());

  transformation = space.transformation();
  test = transformation*origin;
  EXPECT_EQ(1, test.x());
  EXPECT_EQ(0, test.y());
  EXPECT_EQ(0, test.z());

  test = transformation*x1;
  EXPECT_DOUBLE_EQ(1, test.x());
  EXPECT_DOUBLE_EQ(-1, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  EXPECT_TRUE(space.setTransformation(Transformation()));

  transformation = space.transformation();
  test = transformation*origin;
  EXPECT_DOUBLE_EQ(0, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  test = transformation*x1;
  EXPECT_DOUBLE_EQ(1, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  EXPECT_TRUE(space.changeTransformation(Transformation::translation(Vector3d(1,0,0))*Transformation::rotation(Vector3d(0,0,1),-openstudio::degToRad(90))));
  EXPECT_DOUBLE_EQ(1, space.xOrigin());
  EXPECT_DOUBLE_EQ(0, space.yOrigin());
  EXPECT_DOUBLE_EQ(0, space.zOrigin());
  EXPECT_DOUBLE_EQ(90, space.directionofRelativeNorth());

  transformation = space.transformation();
  test = transformation*origin;
  EXPECT_EQ(1, test.x());
  EXPECT_EQ(0, test.y());
  EXPECT_EQ(0, test.z());

  test = transformation*x1;
  EXPECT_DOUBLE_EQ(1, test.x());
  EXPECT_DOUBLE_EQ(-1, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());
}

TEST_F(ModelFixture, Space_Transformation2)
{
  Model model;
  Space space(model);
  Point3dVector points;
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(1,1,0));
  Surface surface(points, model);
  surface.setSpace(space);

  EXPECT_TRUE(circularEqual(points, space.transformation()*points));
  EXPECT_TRUE(circularEqual(points, space.transformation()*surface.vertices()));

  EXPECT_TRUE(space.changeTransformation(Transformation::translation(Vector3d(1,0,0))*Transformation::rotation(Vector3d(0,0,1),-openstudio::degToRad(90))));

  EXPECT_FALSE(circularEqual(points, space.transformation()*points));
  EXPECT_TRUE(circularEqual(points, space.transformation()*surface.vertices()));
}

TEST_F(ModelFixture, Space_IddAssumptions) {
  Model model;
  Space space(model);

  EXPECT_TRUE(space.partofTotalFloorArea());
}

TEST_F(ModelFixture, Space_BoundingBox)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(1,1,0));

  Space space1(model);
  Surface surface1(points, model);
  surface1.setSpace(space1);

  Space space2(model);
  Surface surface2(points, model);
  surface2.setSpace(space2);

  BoundingBox box1 = space1.transformation() * space1.boundingBox();
  BoundingBox box2 = space2.transformation() * space2.boundingBox();

  EXPECT_TRUE(box1.intersects(box2));

  EXPECT_TRUE(space2.setTransformation(Transformation::translation(Vector3d(1,0,0))));

  box2 = space2.transformation() * space2.boundingBox();

  EXPECT_TRUE(box1.intersects(box2));

  EXPECT_TRUE(space2.setTransformation(Transformation::translation(Vector3d(1,1,0))));

  box2 = space2.transformation() * space2.boundingBox();

  EXPECT_TRUE(box1.intersects(box2));

  EXPECT_TRUE(space2.setTransformation(Transformation::translation(Vector3d(2,2,0))));

  box2 = space2.transformation() * space2.boundingBox();

  EXPECT_FALSE(box1.intersects(box2));
}

TEST_F(ModelFixture, Space_hardApplySpaceType_false)
{
  Model model;

  SpaceType spaceType(model);

  Building building = model.getUniqueModelObject<Building>();
  building.setSpaceType(spaceType);

  Space space(model);
  EXPECT_TRUE(space.isSpaceTypeDefaulted());
  ASSERT_TRUE(space.spaceType());
  EXPECT_EQ(spaceType.handle(), space.spaceType()->handle());

  EXPECT_EQ(0u, space.lights().size());
  EXPECT_EQ(0, space.lightingPowerPerFloorArea());
  EXPECT_EQ(0u, spaceType.lights().size());

  LightsDefinition definition(model);
  definition.setWattsperSpaceFloorArea(1.0);

  Lights lights(definition);
  lights.setSpaceType(spaceType);

  EXPECT_FALSE(lights.space());
  ASSERT_TRUE(lights.spaceType());
  EXPECT_EQ(spaceType.handle(), lights.spaceType()->handle());

  EXPECT_EQ(0u, space.lights().size());
  EXPECT_EQ(1.0, space.lightingPowerPerFloorArea());
  ASSERT_EQ(1u, spaceType.lights().size());
  EXPECT_EQ(lights.handle(), spaceType.lights()[0].handle());

  space.hardApplySpaceType(false);

  EXPECT_FALSE(space.isSpaceTypeDefaulted());
  ASSERT_TRUE(space.spaceType());
  EXPECT_NE(spaceType.handle(), space.spaceType()->handle());
  EXPECT_EQ(1u, spaceType.lights().size());
  EXPECT_EQ(0u, space.spaceType()->lights().size());

  // existing light is still attached to space type, new clone is attached to space
  EXPECT_EQ(2u, model.getModelObjects<Lights>().size());
  EXPECT_FALSE(lights.space());
  ASSERT_TRUE(lights.spaceType());
  EXPECT_EQ(spaceType.handle(), lights.spaceType()->handle());

  ASSERT_EQ(1u, space.lights().size());
  EXPECT_NE(lights.handle(), space.lights()[0].handle());
  EXPECT_EQ(1.0, space.lightingPowerPerFloorArea());
  EXPECT_EQ(1u, spaceType.lights().size());
}

TEST_F(ModelFixture, Space_SurfaceMatch_2WallsWindow)
{
  Model model;
  Space space1(model);
  Space space2(model);

  // outward normal is in -y direction
  Point3dVector points;
  points.push_back(Point3d(0, -10, 10));
  points.push_back(Point3d(0, -10, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 0, 10));
  Surface wall1(points, model);
  wall1.setSpace(space1);

  EXPECT_EQ("Wall", wall1.surfaceType());
  EXPECT_EQ("Outdoors", wall1.outsideBoundaryCondition());
  EXPECT_FALSE(wall1.adjacentSurface());

  points.clear();
  points.push_back(Point3d(0, -7, 7));
  points.push_back(Point3d(0, -7, 3));
  points.push_back(Point3d(0, -3, 3));
  points.push_back(Point3d(0, -3, 7));
  SubSurface window1(points, model);
  window1.setSurface(wall1);

  EXPECT_EQ("FixedWindow", window1.subSurfaceType());
  EXPECT_FALSE(window1.adjacentSubSurface());

  // outward normal in +y
  points.clear();
  points.push_back(Point3d(0, 0, 10));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, -10, 0));
  points.push_back(Point3d(0, -10, 10));
  Surface wall2(points, model);
  wall2.setSpace(space2);

  EXPECT_EQ("Wall", wall2.surfaceType());
  EXPECT_EQ("Outdoors", wall2.outsideBoundaryCondition());
  EXPECT_FALSE(wall2.adjacentSurface());

  points.clear();
  points.push_back(Point3d(0, -3, 7));
  points.push_back(Point3d(0, -3, 3));
  points.push_back(Point3d(0, -7, 3));
  points.push_back(Point3d(0, -7, 7));
  SubSurface window2(points, model);
  window2.setSurface(wall2);

  EXPECT_EQ("FixedWindow", window2.subSurfaceType());
  EXPECT_FALSE(window2.adjacentSubSurface());

  space1.matchSurfaces(space2);

  EXPECT_EQ("Wall", wall1.surfaceType());
  EXPECT_EQ("Surface", wall1.outsideBoundaryCondition());
  ASSERT_TRUE(wall1.adjacentSurface());
  EXPECT_EQ(wall2.handle(), wall1.adjacentSurface()->handle());

  EXPECT_EQ("FixedWindow", window1.subSurfaceType());
  ASSERT_TRUE(window1.adjacentSubSurface());
  EXPECT_EQ(window2.handle(), window1.adjacentSubSurface()->handle());

  EXPECT_EQ("Wall", wall2.surfaceType());
  EXPECT_EQ("Surface", wall2.outsideBoundaryCondition());
  ASSERT_TRUE(wall2.adjacentSurface());
  EXPECT_EQ(wall1.handle(), wall2.adjacentSurface()->handle());

  EXPECT_EQ("FixedWindow", window2.subSurfaceType());
  ASSERT_TRUE(window2.adjacentSubSurface());
  EXPECT_EQ(window1.handle(), window2.adjacentSubSurface()->handle());

  space1.unmatchSurfaces();

  EXPECT_EQ("Wall", wall1.surfaceType());
  EXPECT_EQ("Outdoors", wall1.outsideBoundaryCondition());
  EXPECT_FALSE(wall1.adjacentSurface());

  EXPECT_EQ("FixedWindow", window1.subSurfaceType());
  EXPECT_FALSE(window1.adjacentSubSurface());

  EXPECT_EQ("Wall", wall2.surfaceType());
  EXPECT_EQ("Outdoors", wall2.outsideBoundaryCondition());
  EXPECT_FALSE(wall2.adjacentSurface());

  EXPECT_EQ("FixedWindow", window2.subSurfaceType());
  EXPECT_FALSE(window2.adjacentSubSurface());
}

TEST_F(ModelFixture, Space_SurfaceMatch_LargeTest)
{
  Model model;

  Point3dVector points;
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(0, 0, 0));

  int Nx = 2;
  int Ny = 2;
  int Nz = 2;

  for(int i = 0; i < Nx; ++i){
    for(int j = 0; j < Ny; ++j){
      for(int k = 0; k < Nz; ++k){
        boost::optional<Space> space = Space::fromFloorPrint(points, 1, model);
        ASSERT_TRUE(space);
        space->setXOrigin(i);
        space->setYOrigin(j);
        space->setZOrigin(k);

        Point3dVector subSurfacePoints;
        std::vector<Surface> searchResults;

        // add window to north wall
        searchResults = space->findSurfaces(0.0,0.0,90.0,90.0);
        ASSERT_EQ(1u, searchResults.size());

        subSurfacePoints.clear();
        subSurfacePoints.push_back(Point3d(0.75, 1.0, 0.75));
        subSurfacePoints.push_back(Point3d(0.75, 1.0, 0.25));
        subSurfacePoints.push_back(Point3d(0.25, 1.0, 0.25));
        subSurfacePoints.push_back(Point3d(0.25, 1.0, 0.75));

        SubSurface northWindow(subSurfacePoints, model);
        northWindow.setSurface(searchResults[0]);

        // add window to south wall
        searchResults = space->findSurfaces(180.0,180.0,90.0,90.0);
        ASSERT_EQ(1u, searchResults.size());

        subSurfacePoints.clear();
        subSurfacePoints.push_back(Point3d(0.25, 0.0, 0.75));
        subSurfacePoints.push_back(Point3d(0.25, 0.0, 0.25));
        subSurfacePoints.push_back(Point3d(0.75, 0.0, 0.25));
        subSurfacePoints.push_back(Point3d(0.75, 0.0, 0.75));

        SubSurface southWindow(subSurfacePoints, model);
        southWindow.setSurface(searchResults[0]);

        // add door to east wall
        searchResults = space->findSurfaces(90.0,90.0,90.0,90.0);
        ASSERT_EQ(1u, searchResults.size());

        subSurfacePoints.clear();
        subSurfacePoints.push_back(Point3d(1.0, 0.25, 0.75));
        subSurfacePoints.push_back(Point3d(1.0, 0.25, 0.25));
        subSurfacePoints.push_back(Point3d(1.0, 0.75, 0.25));
        subSurfacePoints.push_back(Point3d(1.0, 0.75, 0.75));

        SubSurface eastDoor(subSurfacePoints, model);
        eastDoor.setSurface(searchResults[0]);

        // add door to west wall
        searchResults = space->findSurfaces(270.0,270.0,90.0,90.0);
        ASSERT_EQ(1u, searchResults.size());

        subSurfacePoints.clear();
        subSurfacePoints.push_back(Point3d(0.0, 0.75, 0.75));
        subSurfacePoints.push_back(Point3d(0.0, 0.75, 0.25));
        subSurfacePoints.push_back(Point3d(0.0, 0.25, 0.25));
        subSurfacePoints.push_back(Point3d(0.0, 0.25, 0.75));

        SubSurface westDoor(subSurfacePoints, model);
        westDoor.setSurface(searchResults[0]);
      }
    }
  }

  SpaceVector spaces = model.getModelObjects<Space>();
  matchSurfaces(spaces);

  model.save(toPath("./Space_SurfaceMatch_LargeTest.osm"), true);
}

TEST_F(ModelFixture, Space_FindSurfaces)
{
  Model model;

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);

  std::vector<Surface> searchResults;

  // find north wall
  searchResults = space1->findSurfaces(0.0,0.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,1,0).dot(searchResults[0].outwardNormal()));

  // find north wall
  searchResults = space1->findSurfaces(359.0,1.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,1,0).dot(searchResults[0].outwardNormal()));

  // find east wall
  searchResults = space1->findSurfaces(90.0,90.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(1,0,0).dot(searchResults[0].outwardNormal()));

  // find south wall
  searchResults = space1->findSurfaces(180.0,180.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,-1,0).dot(searchResults[0].outwardNormal()));

  // find west wall
  searchResults = space1->findSurfaces(270.0,270.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(-1,0,0).dot(searchResults[0].outwardNormal()));

  // find floor
  searchResults = space1->findSurfaces(boost::none,boost::none,180.0,180.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,0,-1).dot(searchResults[0].outwardNormal()));

  // find ceiling
  searchResults = space1->findSurfaces(boost::none,boost::none,0.0,0.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,0,1).dot(searchResults[0].outwardNormal()));
}

TEST_F(ModelFixture, Space_FindSurfaces_Translated)
{
  Model model;

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setXOrigin(10);
  space1->setYOrigin(-10);

  std::vector<Surface> searchResults;

  // find north wall
  searchResults = space1->findSurfaces(0.0,0.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,1,0).dot(searchResults[0].outwardNormal()));

  // find north wall
  searchResults = space1->findSurfaces(359.0,1.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,1,0).dot(searchResults[0].outwardNormal()));

  // find east wall
  searchResults = space1->findSurfaces(90.0,90.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(1,0,0).dot(searchResults[0].outwardNormal()));

  // find south wall
  searchResults = space1->findSurfaces(180.0,180.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,-1,0).dot(searchResults[0].outwardNormal()));

  // find west wall
  searchResults = space1->findSurfaces(270.0,270.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(-1,0,0).dot(searchResults[0].outwardNormal()));

  // find floor
  searchResults = space1->findSurfaces(boost::none,boost::none,180.0,180.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,0,-1).dot(searchResults[0].outwardNormal()));

  // find ceiling
  searchResults = space1->findSurfaces(boost::none,boost::none,0.0,0.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,0,1).dot(searchResults[0].outwardNormal()));
}


TEST_F(ModelFixture, Space_FindSurfaces_Translated_Rotated)
{
  Model model;

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setXOrigin(10.0);
  space1->setYOrigin(-10.0);
  space1->setDirectionofRelativeNorth(20.0);

  std::vector<Surface> searchResults;

  // find north wall
  searchResults = space1->findSurfaces(20.0,20.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,1,0).dot(searchResults[0].outwardNormal()));

  // find east wall
  searchResults = space1->findSurfaces(110.0,110.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(1,0,0).dot(searchResults[0].outwardNormal()));

  // find south wall
  searchResults = space1->findSurfaces(200.0,200.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,-1,0).dot(searchResults[0].outwardNormal()));

  // find west wall
  searchResults = space1->findSurfaces(290.0,290.0,90.0,90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(-1,0,0).dot(searchResults[0].outwardNormal()));

  // find floor
  searchResults = space1->findSurfaces(boost::none,boost::none,180.0,180.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,0,-1).dot(searchResults[0].outwardNormal()));

  // find ceiling
  searchResults = space1->findSurfaces(boost::none,boost::none,0.0,0.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0,0,1).dot(searchResults[0].outwardNormal()));
}

TEST_F(ModelFixture, Space_Remove)
{
  Model model;

  Space space(model);
  WorkspaceObjectWatcher spaceWatcher(space);
  EXPECT_FALSE(spaceWatcher.dirty());
  EXPECT_FALSE(spaceWatcher.relationshipChanged());
  EXPECT_FALSE(spaceWatcher.removedFromWorkspace());

  Point3dVector points;
  points.push_back(Point3d(0, 4, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(4, 0, 0));
  points.push_back(Point3d(4, 4, 0));

  Surface surface(points, model);
  surface.setParent(space);

  WorkspaceObjectWatcher surfaceWatcher(surface);
  EXPECT_FALSE(surfaceWatcher.dirty());
  EXPECT_FALSE(surfaceWatcher.relationshipChanged());
  EXPECT_FALSE(surfaceWatcher.removedFromWorkspace());

  std::vector<Handle> handlesToRemove;
  handlesToRemove.push_back(space.handle());
  handlesToRemove.push_back(surface.handle());

  model.removeObjects(handlesToRemove);

  EXPECT_FALSE(spaceWatcher.dirty());
  EXPECT_FALSE(spaceWatcher.relationshipChanged());
  EXPECT_TRUE(spaceWatcher.removedFromWorkspace());
  EXPECT_FALSE(surfaceWatcher.dirty());
  EXPECT_FALSE(surfaceWatcher.relationshipChanged());
  EXPECT_TRUE(surfaceWatcher.removedFromWorkspace());
}

TEST_F(ModelFixture, Space_Remove2)
{
  Model model;

  Space space(model);
  WorkspaceObjectWatcher spaceWatcher(space);
  EXPECT_FALSE(spaceWatcher.dirty());
  EXPECT_FALSE(spaceWatcher.relationshipChanged());
  EXPECT_FALSE(spaceWatcher.removedFromWorkspace());

  Point3dVector points;
  points.push_back(Point3d(0, 4, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(4, 0, 0));
  points.push_back(Point3d(4, 4, 0));

  Surface surface(points, model);
  surface.setParent(space);

  WorkspaceObjectWatcher surfaceWatcher(surface);
  EXPECT_FALSE(surfaceWatcher.dirty());
  EXPECT_FALSE(surfaceWatcher.relationshipChanged());
  EXPECT_FALSE(surfaceWatcher.removedFromWorkspace());

  std::vector<Handle> handlesToRemove;
  handlesToRemove.push_back(space.handle());
  //handlesToRemove.push_back(surface.handle());

  // this does not remove children
  model.removeObjects(handlesToRemove);

  EXPECT_FALSE(spaceWatcher.dirty());
  EXPECT_FALSE(spaceWatcher.relationshipChanged());
  EXPECT_TRUE(spaceWatcher.removedFromWorkspace());
  EXPECT_TRUE(surfaceWatcher.dirty());
  EXPECT_TRUE(surfaceWatcher.relationshipChanged());
  EXPECT_FALSE(surfaceWatcher.removedFromWorkspace());
}

TEST_F(ModelFixture, Space_FloorPrint1)
{
  Model model;

  Space space(model);

  std::vector<Point3d> floorPrint = space.floorPrint();
  EXPECT_EQ(0, floorPrint.size());

  Point3dVector points;
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));

  Surface surface1(points, model);
  surface1.setParent(space);

  floorPrint = space.floorPrint();

  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(1, 1, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(1, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 1, 0))) << floorPrint;
  
  points.clear();
  points.push_back(Point3d(1, 2, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 2, 0));

  Surface surface2(points, model);
  surface2.setParent(space);

  floorPrint = space.floorPrint();
  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(1, 2, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(1, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 2, 0))) << floorPrint;
  
  points.clear();
  points.push_back(Point3d(2, 2, 0));
  points.push_back(Point3d(2, 1, 0));
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 2, 0));

  Surface surface3(points, model);
  surface3.setParent(space);

  floorPrint = space.floorPrint();
  ASSERT_EQ(6u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(2, 2, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(2, 1, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(1, 1, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(1, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[4], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[5], Point3d(0, 2, 0))) << floorPrint;

  points.clear();
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(2, 1, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(1, 0, 0));

  Surface surface4(points, model);
  surface4.setParent(space);

  floorPrint = space.floorPrint();
  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(2, 2, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(2, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 2, 0))) << floorPrint;

  // now we will make a 3x3 square, then delete the center (surface3)

  points.clear();
  points.push_back(Point3d(1, 3, 0));
  points.push_back(Point3d(1, 2, 0));
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 3, 0));

  Surface surface5(points, model);
  surface5.setParent(space);

  points.clear();
  points.push_back(Point3d(2, 3, 0));
  points.push_back(Point3d(2, 2, 0));
  points.push_back(Point3d(1, 2, 0));
  points.push_back(Point3d(1, 3, 0));

  Surface surface6(points, model);
  surface6.setParent(space);

  points.clear();
  points.push_back(Point3d(3, 3, 0));
  points.push_back(Point3d(3, 2, 0));
  points.push_back(Point3d(2, 2, 0));
  points.push_back(Point3d(2, 3, 0));

  Surface surface7(points, model);
  surface7.setParent(space);

  points.clear();
  points.push_back(Point3d(3, 2, 0));
  points.push_back(Point3d(3, 1, 0));
  points.push_back(Point3d(2, 1, 0));
  points.push_back(Point3d(2, 2, 0));

  Surface surface8(points, model);
  surface8.setParent(space);

  points.clear();
  points.push_back(Point3d(3, 1, 0));
  points.push_back(Point3d(3, 0, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(2, 1, 0));

  Surface surface9(points, model);
  surface9.setParent(space);

  floorPrint = space.floorPrint();
  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(3, 3, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(3, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 3, 0))) << floorPrint;

  surface3.remove();

  floorPrint = space.floorPrint();
  ASSERT_EQ(10u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(3, 3, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(3, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 3, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[4], Point3d(1, 2, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[5], Point3d(1, 1, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[6], Point3d(2, 1, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[7], Point3d(2, 2, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[8], Point3d(1, 2, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[9], Point3d(0, 3, 0))) << floorPrint;

}

TEST_F(ModelFixture, Space_FloorPrint2)
{
  Model model;

  Space space(model);

  std::vector<Point3d> floorPrint = space.floorPrint();
  EXPECT_EQ(0, floorPrint.size());

  Point3dVector points;
  points.push_back(Point3d(1, 1, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(0, 1, 0));

  Surface surface1(points, model);
  surface1.setParent(space);

  floorPrint = space.floorPrint();

  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(1, 1, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(1, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 1, 0))) << floorPrint;

  points.clear();
  points.push_back(Point3d(2, 1, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));

  Surface surface2(points, model);
  surface2.setParent(space);

  floorPrint = space.floorPrint();

  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(2, 1, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(2, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 1, 0))) << floorPrint;

  points.clear();
  points.push_back(Point3d(2, 2, 0));
  points.push_back(Point3d(2, 1, 0));
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 2, 0));

  Surface surface3(points, model);
  surface3.setParent(space);

  floorPrint = space.floorPrint();

  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(2, 2, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(2, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 2, 0))) << floorPrint;
}

TEST_F(ModelFixture, Space_Cost)
{
  Model model;

  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));

  boost::optional<Space> space = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space);

  boost::optional<LifeCycleCost> cost1 = LifeCycleCost::createLifeCycleCost("Cleaning", *space, 1, "CostPerArea", "Maintenance", 1, 0);
  ASSERT_TRUE(cost1);
  boost::optional<LifeCycleCost> cost2 = LifeCycleCost::createLifeCycleCost("Design", *space, 100, "CostPerEach", "Construction", 0, 0);
  ASSERT_TRUE(cost2);

  EXPECT_DOUBLE_EQ(100, cost1->totalCost());
  EXPECT_DOUBLE_EQ(100, cost2->totalCost());

  ThermalZone thermalZone(model);
  space->setThermalZone(thermalZone);
  thermalZone.setMultiplier(2);

  EXPECT_DOUBLE_EQ(200, cost1->totalCost());
  EXPECT_DOUBLE_EQ(200, cost2->totalCost());
}

TEST_F(ModelFixture, Space_InfiltrationDesignFlowRate) {
  // create from floor print
  Model model;
  Point3dVector floorPrint;
  floorPrint.push_back(Point3d(0, 10, 0));
  floorPrint.push_back(Point3d(10, 10, 0));
  floorPrint.push_back(Point3d(10, 0, 0));
  floorPrint.push_back(Point3d(0, 0, 0));
  boost::optional<Space> ospace = Space::fromFloorPrint(floorPrint, 3.6, model);
  ASSERT_TRUE(ospace);
  Space space = *ospace;

  // check dimensions
  EXPECT_DOUBLE_EQ(100.0,space.floorArea());
  EXPECT_DOUBLE_EQ(360.0,space.volume());
  EXPECT_DOUBLE_EQ(144.0,space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0,space.exteriorArea()); // ground does not count
  EXPECT_DOUBLE_EQ(0.0,space.infiltrationDesignFlowRate());

  // add an infiltration object
  SpaceInfiltrationDesignFlowRate spaceInfiltration(model);
  spaceInfiltration.setSpace(space);
  spaceInfiltration.setAirChangesperHour(1.0);

  // check infiltration getters
  EXPECT_DOUBLE_EQ(0.1,space.infiltrationDesignFlowRate());
  EXPECT_DOUBLE_EQ(0.001,space.infiltrationDesignFlowPerSpaceFloorArea());
  EXPECT_DOUBLE_EQ(4.0983606557377049E-4,space.infiltrationDesignFlowPerExteriorSurfaceArea());
  EXPECT_DOUBLE_EQ(6.9444444444444447E-4,space.infiltrationDesignFlowPerExteriorWallArea());
  EXPECT_DOUBLE_EQ(1.0,space.infiltrationDesignAirChangesPerHour());

  // create a space type with infiltration
  SpaceType spaceType(model);
  SpaceInfiltrationDesignFlowRate spaceTypeInfiltration(model);
  spaceTypeInfiltration.setSpaceType(spaceType);
  spaceTypeInfiltration.setFlowperExteriorWallArea(5.0E-4);
  space.setSpaceType(spaceType);

  // check infiltration getters again
  EXPECT_DOUBLE_EQ(0.172,space.infiltrationDesignFlowRate());
  EXPECT_DOUBLE_EQ(0.00172,space.infiltrationDesignFlowPerSpaceFloorArea());
  EXPECT_DOUBLE_EQ(7.0491803278688531E-4,space.infiltrationDesignFlowPerExteriorSurfaceArea());
  EXPECT_DOUBLE_EQ(11.944444444444446E-4,space.infiltrationDesignFlowPerExteriorWallArea());
  EXPECT_DOUBLE_EQ(1.72,space.infiltrationDesignAirChangesPerHour());

  // go ahead and check building now
  Building building = model.getUniqueModelObject<Building>();
  EXPECT_DOUBLE_EQ(0.172,building.infiltrationDesignFlowRate());
  EXPECT_DOUBLE_EQ(0.00172,building.infiltrationDesignFlowPerSpaceFloorArea());
  EXPECT_DOUBLE_EQ(7.0491803278688531E-4,building.infiltrationDesignFlowPerExteriorSurfaceArea());
  EXPECT_DOUBLE_EQ(11.944444444444446E-4,building.infiltrationDesignFlowPerExteriorWallArea());
  EXPECT_DOUBLE_EQ(1.72,building.infiltrationDesignAirChangesPerHour());
}

TEST_F(ModelFixture,Space_Plenum)
{
  model::Model model = openstudio::model::Model();

  model::SpaceType spaceType(model);

  model::Building building = model.getUniqueModelObject<model::Building>();
  EXPECT_TRUE(building.setSpaceType(spaceType));

  EXPECT_EQ(1u, model.getConcreteModelObjects<model::SpaceType>().size());

  model::ThermalZone supplyZone(model);
  model::ThermalZone zone(model);
  model::ThermalZone returnZone(model);

  model::Space supplySpace(model);
  EXPECT_TRUE(supplySpace.setThermalZone(supplyZone));
  model::Space space(model);
  EXPECT_TRUE(space.setThermalZone(zone));
  model::Space returnSpace(model);
  EXPECT_TRUE(returnSpace.setThermalZone(returnZone));

  ASSERT_TRUE(supplySpace.spaceType());
  EXPECT_EQ(spaceType.handle(), supplySpace.spaceType()->handle());
  EXPECT_TRUE(supplySpace.partofTotalFloorArea());
  ASSERT_TRUE(space.spaceType());
  EXPECT_EQ(spaceType.handle(), space.spaceType()->handle());
  EXPECT_TRUE(space.partofTotalFloorArea());
  ASSERT_TRUE(returnSpace.spaceType());
  EXPECT_EQ(spaceType.handle(), returnSpace.spaceType()->handle());
  EXPECT_TRUE(returnSpace.partofTotalFloorArea());

  model::AirLoopHVAC airLoopHVAC = openstudio::model::AirLoopHVAC(model);

  bool result = airLoopHVAC.addBranchForZone(zone);
  EXPECT_TRUE(result);
  result = zone.setSupplyPlenum(supplyZone);
  EXPECT_TRUE(result);
  result = zone.setReturnPlenum(returnZone);
  EXPECT_TRUE(result);

  ASSERT_TRUE(supplySpace.spaceType());
  EXPECT_NE(spaceType.handle(), supplySpace.spaceType()->handle());
  EXPECT_FALSE(supplySpace.partofTotalFloorArea());
  ASSERT_TRUE(space.spaceType());
  EXPECT_EQ(spaceType.handle(), space.spaceType()->handle());
  EXPECT_TRUE(space.partofTotalFloorArea());
  ASSERT_TRUE(returnSpace.spaceType());
  EXPECT_NE(spaceType.handle(), returnSpace.spaceType()->handle());
  EXPECT_FALSE(returnSpace.partofTotalFloorArea());

  EXPECT_EQ(2u, model.getConcreteModelObjects<model::SpaceType>().size());

  SpaceType plenumSpaceType = model.plenumSpaceType();

  EXPECT_EQ(2u, model.getConcreteModelObjects<model::SpaceType>().size());

  ASSERT_TRUE(supplySpace.spaceType());
  EXPECT_EQ(plenumSpaceType.handle(), supplySpace.spaceType()->handle());
  EXPECT_FALSE(supplySpace.partofTotalFloorArea());
  ASSERT_TRUE(space.spaceType());
  EXPECT_EQ(spaceType.handle(), space.spaceType()->handle());
  EXPECT_TRUE(space.partofTotalFloorArea());
  ASSERT_TRUE(returnSpace.spaceType());
  EXPECT_EQ(plenumSpaceType.handle(), returnSpace.spaceType()->handle());
  EXPECT_FALSE(returnSpace.partofTotalFloorArea());

}


TEST_F(ModelFixture, Space_Intersect_OneToFour){

  double areaTol = 0.000001;
  double xOrigin = 20.0;

  // space 1 has one large surface, space 2 has 4 rectangles, test that intersection is correct independent of rotation and intersect order
  for (double rotation = 0; rotation < 360.0; rotation += 10.0){
    for (unsigned iStart = 0; iStart < 4; ++iStart){

      Transformation t = Transformation::rotation(Vector3d(0,0,1), degToRad(rotation));

      Model model;
      Space space1(model);
      Space space2(model);

      Point3dVector points;
      points.push_back(Point3d(xOrigin,  0, 20));
      points.push_back(Point3d(xOrigin,  0,  0));
      points.push_back(Point3d(xOrigin, 10,  0));
      points.push_back(Point3d(xOrigin, 10, 20));
      Surface surface(t*points, model);
      surface.setSpace(space1);
      EXPECT_NEAR(200.0, surface.grossArea(), areaTol);

      std::vector<Surface> surfaces;
      for (unsigned i = 0; i < 4; ++i){
        points.clear();
        points.push_back(Point3d(xOrigin, 10, (i+1)*5));
        points.push_back(Point3d(xOrigin, 10,  i*5));
        points.push_back(Point3d(xOrigin,  0,  i*5));
        points.push_back(Point3d(xOrigin,  0, (i+1)*5));
        Surface tempSurface(t*points, model);
        tempSurface.setSpace(space2);
        EXPECT_NEAR(50.0, tempSurface.grossArea(), areaTol);
        surfaces.push_back(tempSurface);
      }

      space1.intersectSurfaces(space2);
      space1.matchSurfaces(space2);

      EXPECT_EQ(4u, space1.surfaces().size());
      for (const Surface& s : space1.surfaces()){
        EXPECT_EQ(4u, s.vertices().size());
        EXPECT_NEAR(50.0, s.grossArea(), areaTol);
        EXPECT_TRUE(s.adjacentSurface());
      }

      EXPECT_EQ(4u, space2.surfaces().size());
      for (const Surface& s : space2.surfaces()){
        EXPECT_EQ(4u, s.vertices().size());
        EXPECT_NEAR(50.0, s.grossArea(), areaTol);
        EXPECT_TRUE(s.adjacentSurface());
      }
    }
  }
}

TEST_F(ModelFixture, Space_Intersect_FourToOne){

  double areaTol = 0.000001;
  double xOrigin = 20.0;

  // space 1 has one large surface, space 2 has 4 rectangles, test that intersection is correct independent of rotation and intersect order
  for (double rotation = 0; rotation < 360.0; rotation += 10.0){
    for (unsigned iStart = 0; iStart < 4; ++iStart){

      Transformation t = Transformation::rotation(Vector3d(0,0,1), degToRad(rotation));

      Model model;
      Space space1(model);
      Space space2(model);

      Point3dVector points;
      points.push_back(Point3d(xOrigin,  0, 20));
      points.push_back(Point3d(xOrigin,  0,  0));
      points.push_back(Point3d(xOrigin, 10,  0));
      points.push_back(Point3d(xOrigin, 10, 20));
      Surface surface(t*points, model);
      surface.setSpace(space1);
      EXPECT_NEAR(200.0, surface.grossArea(), areaTol);

      std::vector<Surface> surfaces;
      for (unsigned i = 0; i < 4; ++i){
        points.clear();
        points.push_back(Point3d(xOrigin, 10, (i+1)*5));
        points.push_back(Point3d(xOrigin, 10,  i*5));
        points.push_back(Point3d(xOrigin,  0,  i*5));
        points.push_back(Point3d(xOrigin,  0, (i+1)*5));
        Surface tempSurface(t*points, model);
        tempSurface.setSpace(space2);
        EXPECT_NEAR(50.0, tempSurface.grossArea(), areaTol);
        surfaces.push_back(tempSurface);
      }

      space2.intersectSurfaces(space1);
      space2.matchSurfaces(space1);

      EXPECT_EQ(4u, space1.surfaces().size());
      for (const Surface& s : space1.surfaces()){
        EXPECT_EQ(4u, s.vertices().size());
        EXPECT_NEAR(50.0, s.grossArea(), areaTol);
        EXPECT_TRUE(s.adjacentSurface());
      }

      EXPECT_EQ(4u, space2.surfaces().size());
      for (const Surface& s : space2.surfaces()){
        EXPECT_EQ(4u, s.vertices().size());
        EXPECT_NEAR(50.0, s.grossArea(), areaTol);
        EXPECT_TRUE(s.adjacentSurface());
      }
    }
  }
}
