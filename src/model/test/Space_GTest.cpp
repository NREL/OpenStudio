/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../Model_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../SpaceType.hpp"
#include "../SpaceType_Impl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../BuildingStory.hpp"
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
#include "../LifeCycleCost_Impl.hpp"
#include "../SpaceInfiltrationDesignFlowRate.hpp"
#include "../AirLoopHVACSupplyPlenum.hpp"
#include "../AirLoopHVACReturnPlenum.hpp"

#include "../../utilities/core/UUID.hpp"

#include "../../utilities/geometry/BoundingBox.hpp"
#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Polyhedron.hpp"
#include "../../utilities/geometry/Transformation.hpp"
#include "../../utilities/geometry/Vector3d.hpp"

#include "../../utilities/idf/WorkspaceObjectWatcher.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../osversion/VersionTranslator.hpp"
#include "../../utilities/geometry/Intersection.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <fmt/ranges.h>

using namespace openstudio;
using namespace openstudio::model;

struct SurfaceAreaCounter
{
  SurfaceAreaCounter(const Model& m) {

    std::vector<Surface> surfaces = m.getConcreteModelObjects<Surface>();
    for (auto& surface : surfaces) {
      if (istringEqual(surface.surfaceType(), "RoofCeiling")) {
        if (istringEqual(surface.outsideBoundaryCondition(), "Outdoors")) {
          exteriorRoofArea += surface.grossArea();
          ++numRoofSurfaces;
        } else {
          interiorRoofArea += surface.grossArea();
        }
      } else if (istringEqual(surface.surfaceType(), "Floor")) {
        if (istringEqual(surface.outsideBoundaryCondition(), "Ground")) {
          exteriorFloorArea += surface.grossArea();
        } else {
          interiorFloorArea += surface.grossArea();
        }
      } else if (istringEqual(surface.surfaceType(), "Wall")) {
        if (istringEqual(surface.outsideBoundaryCondition(), "Outdoors")) {
          exteriorWallArea += surface.grossArea();
        } else {
          interiorWallArea += surface.grossArea();
        }
      }
    }
  }

  double exteriorFloorArea = 0;
  double interiorFloorArea = 0;
  double exteriorRoofArea = 0;
  double interiorRoofArea = 0;
  double exteriorWallArea = 0;
  double interiorWallArea = 0;
  int numRoofSurfaces = 0;
};

TEST_F(ModelFixture, Space) {
  Model model;

  Space space1(model);
  Space space2(model);

  ASSERT_TRUE(space1.name());
  ASSERT_TRUE(space2.name());
  EXPECT_NE("", space1.name().get());
  EXPECT_NE("", space2.name().get());
  EXPECT_NE(space1.name().get(), space2.name().get());
}

TEST_F(ModelFixture, Space_Clone) {
  Model model;
  Space space(model);
  ASSERT_TRUE(space.name());
  EXPECT_EQ("Space 1", space.name().get());

  Point3dVector points{
    {0, 4, 0},
    {0, 0, 0},
    {4, 0, 0},
    {4, 4, 0},
  };

  Surface surface(points, model);
  surface.setParent(space);

  points = {
    {1, 3, 0},
    {1, 1, 0},
    {3, 1, 0},
    {3, 3, 0},
  };

  SubSurface subSurface(points, model);
  subSurface.setParent(surface);

  EXPECT_EQ(static_cast<unsigned>(1), model.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(static_cast<unsigned>(1), model.getConcreteModelObjects<SubSurface>().size());

  EXPECT_FALSE(space.parent());
  ASSERT_TRUE(surface.parent());
  EXPECT_EQ(space.handle(), surface.parent()->handle());
  ASSERT_TRUE(subSurface.parent());
  EXPECT_EQ(surface.handle(), subSurface.parent()->handle());

  ModelObject clone = space.clone(model);

  ASSERT_TRUE(clone.optionalCast<Space>());

  auto spaceClone = clone.cast<Space>();
  ASSERT_TRUE(spaceClone.name());
  EXPECT_EQ("Space 2", spaceClone.name().get());

  EXPECT_EQ(static_cast<unsigned>(2), model.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(static_cast<unsigned>(2), model.getConcreteModelObjects<Surface>().size());
  EXPECT_EQ(static_cast<unsigned>(2), model.getConcreteModelObjects<SubSurface>().size());

  ASSERT_EQ(static_cast<unsigned>(1), spaceClone.surfaces().size());
  EXPECT_NE(surface.handle(), spaceClone.surfaces()[0].handle());
  ASSERT_EQ(static_cast<unsigned>(1), spaceClone.surfaces()[0].subSurfaces().size());
  EXPECT_NE(subSurface.handle(), spaceClone.surfaces()[0].subSurfaces()[0].handle());
}

TEST_F(ModelFixture, Space_FloorArea) {
  Model model;
  Space space(model);

  EXPECT_EQ(0, space.floorArea());

  Point3dVector points;

  // walls
  points = {
    {0, 0, 1},
    {0, 1, 1},
    {0, 1, 0},
    {0, 0, 0},
  };
  Surface westWall(points, model);
  westWall.setParent(space);
  EXPECT_EQ("Wall", westWall.surfaceType());

  EXPECT_EQ(0, space.floorArea());

  points = {
    {0, 1, 1},
    {1, 1, 1},
    {1, 1, 0},
    {0, 1, 0},
  };
  Surface northWall(points, model);
  northWall.setParent(space);
  EXPECT_EQ("Wall", northWall.surfaceType());

  EXPECT_EQ(0, space.floorArea());

  points = {
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 0},
    {1, 1, 0},
  };
  Surface eastWall(points, model);
  eastWall.setParent(space);
  EXPECT_EQ("Wall", eastWall.surfaceType());

  EXPECT_EQ(0, space.floorArea());

  points = {
    {1, 0, 1},
    {0, 0, 1},
    {0, 0, 0},
    {1, 0, 0},
  };
  Surface southWall(points, model);
  southWall.setParent(space);
  EXPECT_EQ("Wall", southWall.surfaceType());

  EXPECT_EQ(0, space.floorArea());

  // roof
  points = {
    {1, 1, 1},
    {0, 1, 1},
    {0, 0, 1},
    {1, 0, 1},
  };
  Surface roof(points, model);
  roof.setParent(space);
  EXPECT_EQ("RoofCeiling", roof.surfaceType());

  EXPECT_EQ(0, space.floorArea());

  // floor
  points = {
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
  };
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

  points = {
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
  };
  SubSurface floorWindow(points, model);
  floorWindow.setSurface(floor);
  EXPECT_EQ("Skylight", floorWindow.subSurfaceType());

  EXPECT_NEAR(6, space.floorArea(), 0.0001);
}

TEST_F(ModelFixture, Space_ThermalZone) {
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

TEST_F(ModelFixture, Space_Lights) {
  Model model;
  Space space(model);

  EXPECT_EQ(0, space.lightingPower());
  EXPECT_EQ(0, space.lightingPowerPerFloorArea());

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

TEST_F(ModelFixture, Space_Transformation) {
  Model model;
  Space space(model);
  Point3d origin(0, 0, 0);
  Point3d x1(1, 0, 0);

  Transformation transformation = space.transformation();

  Point3d test = transformation * origin;
  EXPECT_DOUBLE_EQ(0, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  test = transformation * x1;
  EXPECT_DOUBLE_EQ(1, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  EXPECT_TRUE(space.setTransformation(Transformation::translation(Vector3d(1, 0, 0))));
  EXPECT_DOUBLE_EQ(1, space.xOrigin());
  EXPECT_DOUBLE_EQ(0, space.yOrigin());
  EXPECT_DOUBLE_EQ(0, space.zOrigin());
  EXPECT_DOUBLE_EQ(0, space.directionofRelativeNorth());

  transformation = space.transformation();

  test = transformation * origin;
  EXPECT_DOUBLE_EQ(1, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  test = transformation * x1;
  EXPECT_DOUBLE_EQ(2, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  EXPECT_TRUE(
    space.setTransformation(Transformation::translation(Vector3d(1, 0, 0)) * Transformation::rotation(Vector3d(0, 0, 1), -openstudio::degToRad(90))));
  EXPECT_DOUBLE_EQ(1, space.xOrigin());
  EXPECT_DOUBLE_EQ(0, space.yOrigin());
  EXPECT_DOUBLE_EQ(0, space.zOrigin());
  EXPECT_DOUBLE_EQ(90, space.directionofRelativeNorth());

  transformation = space.transformation();
  test = transformation * origin;
  EXPECT_EQ(1, test.x());
  EXPECT_EQ(0, test.y());
  EXPECT_EQ(0, test.z());

  test = transformation * x1;
  EXPECT_DOUBLE_EQ(1, test.x());
  EXPECT_DOUBLE_EQ(-1, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  EXPECT_TRUE(space.setTransformation(Transformation()));

  transformation = space.transformation();
  test = transformation * origin;
  EXPECT_DOUBLE_EQ(0, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  test = transformation * x1;
  EXPECT_DOUBLE_EQ(1, test.x());
  EXPECT_DOUBLE_EQ(0, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());

  EXPECT_TRUE(space.changeTransformation(Transformation::translation(Vector3d(1, 0, 0))
                                         * Transformation::rotation(Vector3d(0, 0, 1), -openstudio::degToRad(90))));
  EXPECT_DOUBLE_EQ(1, space.xOrigin());
  EXPECT_DOUBLE_EQ(0, space.yOrigin());
  EXPECT_DOUBLE_EQ(0, space.zOrigin());
  EXPECT_DOUBLE_EQ(90, space.directionofRelativeNorth());

  transformation = space.transformation();
  test = transformation * origin;
  EXPECT_EQ(1, test.x());
  EXPECT_EQ(0, test.y());
  EXPECT_EQ(0, test.z());

  test = transformation * x1;
  EXPECT_DOUBLE_EQ(1, test.x());
  EXPECT_DOUBLE_EQ(-1, test.y());
  EXPECT_DOUBLE_EQ(0, test.z());
}

TEST_F(ModelFixture, Space_Transformation2) {
  Model model;
  Space space(model);
  Point3dVector points{
    {1, 0, 0},
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
  };
  Surface surface(points, model);
  surface.setSpace(space);

  EXPECT_TRUE(circularEqual(points, space.transformation() * points));
  EXPECT_TRUE(circularEqual(points, space.transformation() * surface.vertices()));

  EXPECT_TRUE(space.changeTransformation(Transformation::translation(Vector3d(1, 0, 0))
                                         * Transformation::rotation(Vector3d(0, 0, 1), -openstudio::degToRad(90))));

  EXPECT_FALSE(circularEqual(points, space.transformation() * points));
  EXPECT_TRUE(circularEqual(points, space.transformation() * surface.vertices()));
}

TEST_F(ModelFixture, Space_IddAssumptions) {
  Model model;
  Space space(model);

  EXPECT_TRUE(space.partofTotalFloorArea());
}

TEST_F(ModelFixture, Space_BoundingBox) {
  Model model;

  Point3dVector points{
    {1, 0, 0},
    {0, 0, 0},
    {0, 1, 0},
    {1, 1, 0},
  };

  Space space1(model);
  Surface surface1(points, model);
  surface1.setSpace(space1);

  Space space2(model);
  Surface surface2(points, model);
  surface2.setSpace(space2);

  BoundingBox box1 = space1.transformation() * space1.boundingBox();
  BoundingBox box2 = space2.transformation() * space2.boundingBox();

  EXPECT_TRUE(box1.intersects(box2));

  EXPECT_TRUE(space2.setTransformation(Transformation::translation(Vector3d(1, 0, 0))));

  box2 = space2.transformation() * space2.boundingBox();

  EXPECT_TRUE(box1.intersects(box2));

  EXPECT_TRUE(space2.setTransformation(Transformation::translation(Vector3d(1, 1, 0))));

  box2 = space2.transformation() * space2.boundingBox();

  EXPECT_TRUE(box1.intersects(box2));

  EXPECT_TRUE(space2.setTransformation(Transformation::translation(Vector3d(2, 2, 0))));

  box2 = space2.transformation() * space2.boundingBox();

  EXPECT_FALSE(box1.intersects(box2));
}

TEST_F(ModelFixture, Space_hardApplySpaceType_false) {
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
  EXPECT_EQ(2u, model.getConcreteModelObjects<Lights>().size());
  EXPECT_FALSE(lights.space());
  ASSERT_TRUE(lights.spaceType());
  EXPECT_EQ(spaceType.handle(), lights.spaceType()->handle());

  ASSERT_EQ(1u, space.lights().size());
  EXPECT_NE(lights.handle(), space.lights()[0].handle());
  EXPECT_EQ(1.0, space.lightingPowerPerFloorArea());
  EXPECT_EQ(1u, spaceType.lights().size());
}

TEST_F(ModelFixture, Space_SurfaceMatch_2WallsWindow) {
  Model model;
  Space space1(model);
  Space space2(model);

  // outward normal is in -y direction
  Point3dVector points{
    {0, -10, 10},
    {0, -10, 0},
    {0, 0, 0},
    {0, 0, 10},
  };
  Surface wall1(points, model);
  wall1.setSpace(space1);

  EXPECT_EQ("Wall", wall1.surfaceType());
  EXPECT_EQ("Outdoors", wall1.outsideBoundaryCondition());
  EXPECT_FALSE(wall1.adjacentSurface());

  points = {
    {0, -7, 7},
    {0, -7, 3},
    {0, -3, 3},
    {0, -3, 7},
  };
  SubSurface window1(points, model);
  window1.setSurface(wall1);

  EXPECT_EQ("FixedWindow", window1.subSurfaceType());
  EXPECT_FALSE(window1.adjacentSubSurface());

  // outward normal in +y
  points = {
    {0, 0, 10},
    {0, 0, 0},
    {0, -10, 0},
    {0, -10, 10},
  };
  Surface wall2(points, model);
  wall2.setSpace(space2);

  EXPECT_EQ("Wall", wall2.surfaceType());
  EXPECT_EQ("Outdoors", wall2.outsideBoundaryCondition());
  EXPECT_FALSE(wall2.adjacentSurface());

  points = {
    {0, -3, 7},
    {0, -3, 3},
    {0, -7, 3},
    {0, -7, 7},
  };
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

TEST_F(ModelFixture, Space_SurfaceMatch_LargeTest) {
  Model model;

  Point3dVector points{
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
  };

  int Nx = 2;
  int Ny = 2;
  int Nz = 2;

  for (int i = 0; i < Nx; ++i) {
    for (int j = 0; j < Ny; ++j) {
      for (int k = 0; k < Nz; ++k) {
        boost::optional<Space> space = Space::fromFloorPrint(points, 1, model);
        ASSERT_TRUE(space);
        space->setXOrigin(i);
        space->setYOrigin(j);
        space->setZOrigin(k);

        Point3dVector subSurfacePoints;
        std::vector<Surface> searchResults;

        // add window to north wall
        searchResults = space->findSurfaces(0.0, 0.0, 90.0, 90.0);
        ASSERT_EQ(1u, searchResults.size());

        subSurfacePoints = {
          {0.75, 1.0, 0.75},
          {0.75, 1.0, 0.25},
          {0.25, 1.0, 0.25},
          {0.25, 1.0, 0.75},
        };

        SubSurface northWindow(subSurfacePoints, model);
        northWindow.setSurface(searchResults[0]);

        // add window to south wall
        searchResults = space->findSurfaces(180.0, 180.0, 90.0, 90.0);
        ASSERT_EQ(1u, searchResults.size());

        subSurfacePoints = {
          {0.25, 0.0, 0.75},
          {0.25, 0.0, 0.25},
          {0.75, 0.0, 0.25},
          {0.75, 0.0, 0.75},
        };

        SubSurface southWindow(subSurfacePoints, model);
        southWindow.setSurface(searchResults[0]);

        // add door to east wall
        searchResults = space->findSurfaces(90.0, 90.0, 90.0, 90.0);
        ASSERT_EQ(1u, searchResults.size());

        subSurfacePoints = {
          {1.0, 0.25, 0.75},
          {1.0, 0.25, 0.25},
          {1.0, 0.75, 0.25},
          {1.0, 0.75, 0.75},
        };

        SubSurface eastDoor(subSurfacePoints, model);
        eastDoor.setSurface(searchResults[0]);

        // add door to west wall
        searchResults = space->findSurfaces(270.0, 270.0, 90.0, 90.0);
        ASSERT_EQ(1u, searchResults.size());

        subSurfacePoints = {
          {0.0, 0.75, 0.75},
          {0.0, 0.75, 0.25},
          {0.0, 0.25, 0.25},
          {0.0, 0.25, 0.75},
        };

        SubSurface westDoor(subSurfacePoints, model);
        westDoor.setSurface(searchResults[0]);
      }
    }
  }

  SpaceVector spaces = model.getConcreteModelObjects<Space>();
  matchSurfaces(spaces);

  // openstudio::path outpath = resourcesPath() / toPath("model/Space_SurfaceMatch_LargeTest.osm");
  // model.save(outpath, true);
}

TEST_F(ModelFixture, Space_FindSurfaces) {
  Model model;

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);

  std::vector<Surface> searchResults;

  // find north wall
  searchResults = space1->findSurfaces(0.0, 0.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, 1, 0).dot(searchResults[0].outwardNormal()));

  // find north wall
  searchResults = space1->findSurfaces(359.0, 1.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, 1, 0).dot(searchResults[0].outwardNormal()));

  // find east wall
  searchResults = space1->findSurfaces(90.0, 90.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(1, 0, 0).dot(searchResults[0].outwardNormal()));

  // find south wall
  searchResults = space1->findSurfaces(180.0, 180.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, -1, 0).dot(searchResults[0].outwardNormal()));

  // find west wall
  searchResults = space1->findSurfaces(270.0, 270.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(-1, 0, 0).dot(searchResults[0].outwardNormal()));

  // find floor
  searchResults = space1->findSurfaces(boost::none, boost::none, 180.0, 180.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, 0, -1).dot(searchResults[0].outwardNormal()));

  // find ceiling
  searchResults = space1->findSurfaces(boost::none, boost::none, 0.0, 0.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, 0, 1).dot(searchResults[0].outwardNormal()));
}

TEST_F(ModelFixture, Space_FindSurfaces_Translated) {
  Model model;

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setXOrigin(10);
  space1->setYOrigin(-10);

  std::vector<Surface> searchResults;

  // find north wall
  searchResults = space1->findSurfaces(0.0, 0.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, 1, 0).dot(searchResults[0].outwardNormal()));

  // find north wall
  searchResults = space1->findSurfaces(359.0, 1.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, 1, 0).dot(searchResults[0].outwardNormal()));

  // find east wall
  searchResults = space1->findSurfaces(90.0, 90.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(1, 0, 0).dot(searchResults[0].outwardNormal()));

  // find south wall
  searchResults = space1->findSurfaces(180.0, 180.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, -1, 0).dot(searchResults[0].outwardNormal()));

  // find west wall
  searchResults = space1->findSurfaces(270.0, 270.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(-1, 0, 0).dot(searchResults[0].outwardNormal()));

  // find floor
  searchResults = space1->findSurfaces(boost::none, boost::none, 180.0, 180.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, 0, -1).dot(searchResults[0].outwardNormal()));

  // find ceiling
  searchResults = space1->findSurfaces(boost::none, boost::none, 0.0, 0.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, 0, 1).dot(searchResults[0].outwardNormal()));
}

TEST_F(ModelFixture, Space_FindSurfaces_Translated_Rotated) {
  Model model;

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

  boost::optional<Space> space1 = Space::fromFloorPrint(floorPrint, 3, model);
  ASSERT_TRUE(space1);
  space1->setXOrigin(10.0);
  space1->setYOrigin(-10.0);
  space1->setDirectionofRelativeNorth(20.0);

  std::vector<Surface> searchResults;

  // find north wall
  searchResults = space1->findSurfaces(20.0, 20.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, 1, 0).dot(searchResults[0].outwardNormal()));

  // find east wall
  searchResults = space1->findSurfaces(110.0, 110.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(1, 0, 0).dot(searchResults[0].outwardNormal()));

  // find south wall
  searchResults = space1->findSurfaces(200.0, 200.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, -1, 0).dot(searchResults[0].outwardNormal()));

  // find west wall
  searchResults = space1->findSurfaces(290.0, 290.0, 90.0, 90.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(-1, 0, 0).dot(searchResults[0].outwardNormal()));

  // find floor
  searchResults = space1->findSurfaces(boost::none, boost::none, 180.0, 180.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, 0, -1).dot(searchResults[0].outwardNormal()));

  // find ceiling
  searchResults = space1->findSurfaces(boost::none, boost::none, 0.0, 0.0);
  ASSERT_TRUE(searchResults.size() == 1);
  EXPECT_DOUBLE_EQ(1, Vector3d(0, 0, 1).dot(searchResults[0].outwardNormal()));
}

TEST_F(ModelFixture, Space_Remove) {
  Model model;

  Space space(model);
  WorkspaceObjectWatcher spaceWatcher(space);
  EXPECT_FALSE(spaceWatcher.dirty());
  EXPECT_FALSE(spaceWatcher.relationshipChanged());
  EXPECT_FALSE(spaceWatcher.removedFromWorkspace());

  Point3dVector points{
    {0, 4, 0},
    {0, 0, 0},
    {4, 0, 0},
    {4, 4, 0},
  };

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

TEST_F(ModelFixture, Space_Remove2) {
  Model model;

  Space space(model);
  WorkspaceObjectWatcher spaceWatcher(space);
  EXPECT_FALSE(spaceWatcher.dirty());
  EXPECT_FALSE(spaceWatcher.relationshipChanged());
  EXPECT_FALSE(spaceWatcher.removedFromWorkspace());

  Point3dVector points{
    {0, 4, 0},
    {0, 0, 0},
    {4, 0, 0},
    {4, 4, 0},
  };

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

TEST_F(ModelFixture, Space_FloorPrint1) {
  Model model;

  Space space(model);

  std::vector<Point3d> floorPrint = space.floorPrint();
  EXPECT_EQ(0, floorPrint.size());

  Point3dVector points{
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
    {0, 1, 0},
  };

  Surface surface1(points, model);
  surface1.setParent(space);

  floorPrint = space.floorPrint();

  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(1, 1, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(1, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 1, 0))) << floorPrint;

  points = {
    {1, 2, 0},
    {1, 1, 0},
    {0, 1, 0},
    {0, 2, 0},
  };

  Surface surface2(points, model);
  surface2.setParent(space);

  floorPrint = space.floorPrint();
  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(1, 2, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(1, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 2, 0))) << floorPrint;

  points = {
    {2, 2, 0},
    {2, 1, 0},
    {1, 1, 0},
    {1, 2, 0},
  };

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

  points = {
    {1, 1, 0},
    {2, 1, 0},
    {2, 0, 0},
    {1, 0, 0},
  };

  Surface surface4(points, model);
  surface4.setParent(space);

  floorPrint = space.floorPrint();
  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(2, 2, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(2, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 2, 0))) << floorPrint;

  // now we will make a 3x3 square, then delete the center (surface3)

  points = {
    {1, 3, 0},
    {1, 2, 0},
    {0, 2, 0},
    {0, 3, 0},
  };

  Surface surface5(points, model);
  surface5.setParent(space);

  points = {
    {2, 3, 0},
    {2, 2, 0},
    {1, 2, 0},
    {1, 3, 0},
  };

  Surface surface6(points, model);
  surface6.setParent(space);

  points = {
    {3, 3, 0},
    {3, 2, 0},
    {2, 2, 0},
    {2, 3, 0},
  };

  Surface surface7(points, model);
  surface7.setParent(space);

  points = {
    {3, 2, 0},
    {3, 1, 0},
    {2, 1, 0},
    {2, 2, 0},
  };

  Surface surface8(points, model);
  surface8.setParent(space);

  points = {
    {3, 1, 0},
    {3, 0, 0},
    {2, 0, 0},
    {2, 1, 0},
  };

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

TEST_F(ModelFixture, Space_FloorPrint2) {
  Model model;

  Space space(model);

  std::vector<Point3d> floorPrint = space.floorPrint();
  EXPECT_EQ(0, floorPrint.size());

  Point3dVector points{
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
    {0, 1, 0},
  };

  Surface surface1(points, model);
  surface1.setParent(space);

  floorPrint = space.floorPrint();

  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(1, 1, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(1, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 1, 0))) << floorPrint;

  points = {
    {2, 1, 0},
    {2, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

  Surface surface2(points, model);
  surface2.setParent(space);

  floorPrint = space.floorPrint();

  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(2, 1, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(2, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 1, 0))) << floorPrint;

  points = {
    {2, 2, 0},
    {2, 1, 0},
    {0, 1, 0},
    {0, 2, 0},
  };

  Surface surface3(points, model);
  surface3.setParent(space);

  floorPrint = space.floorPrint();

  ASSERT_EQ(4u, floorPrint.size()) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[0], Point3d(2, 2, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[1], Point3d(2, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[2], Point3d(0, 0, 0))) << floorPrint;
  EXPECT_DOUBLE_EQ(0, getDistance(floorPrint[3], Point3d(0, 2, 0))) << floorPrint;
}

TEST_F(ModelFixture, Space_Cost) {
  Model model;

  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };

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
  Point3dVector floorPrint{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };
  boost::optional<Space> ospace = Space::fromFloorPrint(floorPrint, 3.6, model);
  ASSERT_TRUE(ospace);
  Space space = *ospace;

  // check dimensions
  EXPECT_DOUBLE_EQ(100.0, space.floorArea());
  EXPECT_DOUBLE_EQ(360.0, space.volume());
  EXPECT_DOUBLE_EQ(144.0, space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0, space.exteriorArea());  // ground does not count
  EXPECT_DOUBLE_EQ(0.0, space.infiltrationDesignFlowRate());

  // add an infiltration object
  SpaceInfiltrationDesignFlowRate spaceInfiltration(model);
  spaceInfiltration.setSpace(space);
  spaceInfiltration.setAirChangesperHour(1.0);

  // check infiltration getters
  EXPECT_DOUBLE_EQ(0.1, space.infiltrationDesignFlowRate());
  EXPECT_DOUBLE_EQ(0.001, space.infiltrationDesignFlowPerSpaceFloorArea());
  EXPECT_DOUBLE_EQ(4.0983606557377049E-4, space.infiltrationDesignFlowPerExteriorSurfaceArea());
  EXPECT_DOUBLE_EQ(6.9444444444444447E-4, space.infiltrationDesignFlowPerExteriorWallArea());
  EXPECT_DOUBLE_EQ(1.0, space.infiltrationDesignAirChangesPerHour());

  // create a space type with infiltration
  SpaceType spaceType(model);
  SpaceInfiltrationDesignFlowRate spaceTypeInfiltration(model);
  spaceTypeInfiltration.setSpaceType(spaceType);
  spaceTypeInfiltration.setFlowperExteriorWallArea(5.0E-4);
  space.setSpaceType(spaceType);

  // check infiltration getters again
  EXPECT_DOUBLE_EQ(0.172, space.infiltrationDesignFlowRate());
  EXPECT_DOUBLE_EQ(0.00172, space.infiltrationDesignFlowPerSpaceFloorArea());
  EXPECT_DOUBLE_EQ(7.0491803278688531E-4, space.infiltrationDesignFlowPerExteriorSurfaceArea());
  EXPECT_DOUBLE_EQ(11.944444444444446E-4, space.infiltrationDesignFlowPerExteriorWallArea());
  EXPECT_DOUBLE_EQ(1.72, space.infiltrationDesignAirChangesPerHour());

  // go ahead and check building now
  Building building = model.getUniqueModelObject<Building>();
  EXPECT_DOUBLE_EQ(0.172, building.infiltrationDesignFlowRate());
  EXPECT_DOUBLE_EQ(0.00172, building.infiltrationDesignFlowPerSpaceFloorArea());
  EXPECT_DOUBLE_EQ(7.0491803278688531E-4, building.infiltrationDesignFlowPerExteriorSurfaceArea());
  EXPECT_DOUBLE_EQ(11.944444444444446E-4, building.infiltrationDesignFlowPerExteriorWallArea());
  EXPECT_DOUBLE_EQ(1.72, building.infiltrationDesignAirChangesPerHour());
}

TEST_F(ModelFixture, Space_Plenum) {
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

  std::vector<Space> plenumSpaces = plenumSpaceType.spaces();
  ASSERT_EQ(2u, plenumSpaces.size());

  EXPECT_EQ("Plenum Space Type", plenumSpaceType.nameString());
  plenumSpaceType.setNameProtected("Some Other Name");
  EXPECT_EQ("Plenum Space Type", plenumSpaceType.nameString());

  EXPECT_EQ("Plenum Space Type", plenumSpaceType.nameString());
  plenumSpaceType.setName("Some Other Name");
  EXPECT_EQ("Some Other Name", plenumSpaceType.nameString());

  plenumSpaceType = model.plenumSpaceType();
  EXPECT_EQ(3u, model.getConcreteModelObjects<model::SpaceType>().size());

  ASSERT_TRUE(supplySpace.spaceType());
  EXPECT_EQ(plenumSpaceType.handle(), supplySpace.spaceType()->handle());
  EXPECT_FALSE(supplySpace.partofTotalFloorArea());
  ASSERT_TRUE(space.spaceType());
  EXPECT_EQ(spaceType.handle(), space.spaceType()->handle());
  EXPECT_TRUE(space.partofTotalFloorArea());
  ASSERT_TRUE(returnSpace.spaceType());
  EXPECT_EQ(plenumSpaceType.handle(), returnSpace.spaceType()->handle());
  EXPECT_FALSE(returnSpace.partofTotalFloorArea());

  plenumSpaces = plenumSpaceType.spaces();
  ASSERT_EQ(2u, plenumSpaces.size());
}

TEST_F(ModelFixture, Space_Intersect_OneToFour) {

  double areaTol = 0.000001;
  double xOrigin = 20.0;

  // space 1 has one large surface, space 2 has 4 rectangles, test that intersection is correct independent of rotation and intersect order
  for (double rotation = 0; rotation < 360.0; rotation += 10.0) {
    for (unsigned iStart = 0; iStart < 4; ++iStart) {

      Transformation t = Transformation::rotation(Vector3d(0, 0, 1), degToRad(rotation));

      Model model;
      Space space1(model);
      Space space2(model);

      Point3dVector points{
        {xOrigin, 0, 20},
        {xOrigin, 0, 0},
        {xOrigin, 10, 0},
        {xOrigin, 10, 20},
      };
      Surface surface(t * points, model);
      surface.setSpace(space1);
      EXPECT_NEAR(200.0, surface.grossArea(), areaTol);

      std::vector<Surface> surfaces;
      for (unsigned i = 0; i < 4; ++i) {
        points = {
          {xOrigin, 10, (i + 1) * 5.0},
          {xOrigin, 10, i * 5.0},
          {xOrigin, 0, i * 5.0},
          {xOrigin, 0, (i + 1) * 5.0},
        };
        Surface tempSurface(t * points, model);
        tempSurface.setSpace(space2);
        EXPECT_NEAR(50.0, tempSurface.grossArea(), areaTol);
        surfaces.push_back(tempSurface);
      }

      space1.intersectSurfaces(space2);
      space1.matchSurfaces(space2);

      EXPECT_EQ(4u, space1.surfaces().size());
      for (const Surface& s : space1.surfaces()) {
        EXPECT_EQ(4u, s.vertices().size());
        EXPECT_NEAR(50.0, s.grossArea(), areaTol);
        EXPECT_TRUE(s.adjacentSurface());
      }

      EXPECT_EQ(4u, space2.surfaces().size());
      for (const Surface& s : space2.surfaces()) {
        EXPECT_EQ(4u, s.vertices().size());
        EXPECT_NEAR(50.0, s.grossArea(), areaTol);
        EXPECT_TRUE(s.adjacentSurface());
      }
    }
  }
}

TEST_F(ModelFixture, Space_Intersect_FourToOne) {

  double areaTol = 0.000001;
  double xOrigin = 20.0;

  // space 1 has one large surface, space 2 has 4 rectangles, test that intersection is correct independent of rotation and intersect order
  for (double rotation = 0; rotation < 360.0; rotation += 10.0) {
    for (unsigned iStart = 0; iStart < 4; ++iStart) {

      Transformation t = Transformation::rotation(Vector3d(0, 0, 1), degToRad(rotation));

      Model model;
      Space space1(model);
      Space space2(model);

      Point3dVector points{
        {xOrigin, 0, 20},
        {xOrigin, 0, 0},
        {xOrigin, 10, 0},
        {xOrigin, 10, 20},
      };
      Surface surface(t * points, model);
      surface.setSpace(space1);
      EXPECT_NEAR(200.0, surface.grossArea(), areaTol);

      std::vector<Surface> surfaces;
      for (unsigned i = 0; i < 4; ++i) {
        points = {
          {xOrigin, 10, (i + 1) * 5.0},
          {xOrigin, 10, i * 5.0},
          {xOrigin, 0, i * 5.0},
          {xOrigin, 0, (i + 1) * 5.0},
        };
        Surface tempSurface(t * points, model);
        tempSurface.setSpace(space2);
        EXPECT_NEAR(50.0, tempSurface.grossArea(), areaTol);
        surfaces.push_back(tempSurface);
      }

      space2.intersectSurfaces(space1);
      space2.matchSurfaces(space1);

      EXPECT_EQ(4u, space1.surfaces().size());
      for (const Surface& s : space1.surfaces()) {
        EXPECT_EQ(4u, s.vertices().size());
        EXPECT_NEAR(50.0, s.grossArea(), areaTol);
        EXPECT_TRUE(s.adjacentSurface());
      }

      EXPECT_EQ(4u, space2.surfaces().size());
      for (const Surface& s : space2.surfaces()) {
        EXPECT_EQ(4u, s.vertices().size());
        EXPECT_NEAR(50.0, s.grossArea(), areaTol);
        EXPECT_TRUE(s.adjacentSurface());
      }
    }
  }
}

TEST_F(ModelFixture, Space_LifeCycleCost) {
  Model model;
  Space space(model);
  EXPECT_EQ(0, space.lifeCycleCosts().size());
  EXPECT_EQ(0, model.getConcreteModelObjects<LifeCycleCost>().size());
  boost::optional<LifeCycleCost> lifeCycleCost = LifeCycleCost::createLifeCycleCost("New Cost", space, 1, "CostPerEach", "Construction");
  ASSERT_TRUE(lifeCycleCost);
  EXPECT_EQ(1u, space.lifeCycleCosts().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<LifeCycleCost>().size());
  space.remove();
  EXPECT_TRUE(lifeCycleCost->handle().isNull());
  EXPECT_EQ(0, model.getConcreteModelObjects<LifeCycleCost>().size());
}

TEST_F(ModelFixture, Space_hardApplySpaceType_Plenum) {
  Model m;
  Space s(m);
  ThermalZone z(m);
  s.setThermalZone(z);

  AirLoopHVACSupplyPlenum a(m);
  a.setThermalZone(z);

  ASSERT_EQ(m.plenumSpaceType().handle(), s.spaceType().get().handle());

  // This should not do anything, space type should change
  s.hardApplySpaceType(true);

  ASSERT_NE(m.plenumSpaceType().handle(), s.spaceType().get().handle());
}

TEST_F(ModelFixture, Space_hardApplySpaceType_Plenum2) {
  Model m;
  Space s1(m);
  Space s2(m);
  ThermalZone z(m);
  s1.setThermalZone(z);
  s2.setThermalZone(z);

  AirLoopHVACSupplyPlenum a(m);
  a.setThermalZone(z);

  ASSERT_EQ(m.plenumSpaceType().handle(), s1.spaceType().get().handle());
  ASSERT_EQ(m.plenumSpaceType().handle(), s2.spaceType().get().handle());

  // This should not do anything, space type should change
  s1.hardApplySpaceType(true);
  s2.hardApplySpaceType(true);

  ASSERT_NE(m.plenumSpaceType().handle(), s1.spaceType().get().handle());
  ASSERT_NE(m.plenumSpaceType().handle(), s2.spaceType().get().handle());
  ASSERT_NE(s1.spaceType().get().handle(), s2.spaceType().get().handle());
}

TEST_F(ModelFixture, Space_intersectSurfaces_degenerate1) {
  Model m;
  std::vector<Point3d> vertices;

  // bottom floor
  BuildingStory bottomStory(m);

  // bottom core
  vertices = {
    {-13.0256, 7.1598, 0},
    {13.0256, 7.1598, 0},
    {13.0256, -7.1598, 0},
    {-13.0256, -7.1598, 0},
  };
  boost::optional<Space> bottomCore = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomCore);
  bottomCore->setZOrigin(0);
  bottomCore->setBuildingStory(bottomStory);
  bottomCore->setName("bottomCore");

  // bottom top
  vertices = {
    {-17.5976, 11.7318, 0},
    {17.5976, 11.7318, 0},
    {13.0256, 7.1598, 0},
    {-13.0256, 7.1598, 0},
  };
  boost::optional<Space> bottomTop = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomTop);
  bottomTop->setZOrigin(0);
  bottomTop->setBuildingStory(bottomStory);
  bottomTop->setName("bottomTop");

  // bottom right
  vertices = {
    {17.5976, 11.7318, 0},
    {17.5976, -11.7318, 0},
    {13.0256, -7.1598, 0},
    {13.0256, 7.1598, 0},
  };
  boost::optional<Space> bottomRight = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomRight);
  bottomRight->setZOrigin(0);
  bottomRight->setBuildingStory(bottomStory);
  bottomRight->setName("bottomRight");

  // bottom bottom
  vertices = {
    {17.5976, -11.7318, 0},
    {-17.5976, -11.7318, 0},
    {-13.0256, -7.1598, 0},
    {13.0256, -7.1598, 0},
  };
  boost::optional<Space> bottomBottom = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomBottom);
  bottomBottom->setZOrigin(0);
  bottomBottom->setBuildingStory(bottomStory);
  bottomBottom->setName("bottomBottom");

  // bottom left
  vertices = {
    {-17.5976, 11.7318, 0},
    {-13.0256, 7.1598, 0},
    {-13.0256, -7.1598, 0},
    {-17.5976, -11.7318, 0},
  };
  boost::optional<Space> bottomLeft = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomLeft);
  bottomLeft->setZOrigin(0);
  bottomLeft->setBuildingStory(bottomStory);
  bottomLeft->setName("bottomLeft");

  // top floor
  BuildingStory topStory(m);

  // top core
  vertices = {
    {-7.8714, 3.7236, 0},
    {7.8714, 3.7236, 0},
    {7.8714, -3.7236, 0},
    {-7.8714, -3.7236, 0},
  };
  boost::optional<Space> topCore = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topCore);
  topCore->setZOrigin(3);
  topCore->setBuildingStory(topStory);
  topCore->setName("topCore");

  // top top
  vertices = {
    {-12.4434, 8.2956, 0},
    {12.4434, 8.2956, 0},
    {7.8714, 3.7236, 0},
    {-7.8714, 3.7236, 0},
  };
  boost::optional<Space> topTop = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topTop);
  topTop->setZOrigin(3);
  topTop->setBuildingStory(topStory);
  topTop->setName("topTop");

  // top right
  vertices = {
    {12.4434, 8.2956, 0},
    {12.4434, -8.2956, 0},
    {7.8714, -3.7236, 0},
    {7.8714, 3.7236, 0},
  };
  boost::optional<Space> topRight = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topRight);
  topRight->setZOrigin(3);
  topRight->setBuildingStory(topStory);
  topRight->setName("topRight");

  // top bottom
  vertices = {
    {12.4434, -8.2956, 0},
    {-12.4434, -8.2956, 0},
    {-7.8714, -3.7236, 0},
    {7.8714, -3.7236, 0},
  };
  boost::optional<Space> topBottom = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topBottom);
  topBottom->setZOrigin(3);
  topBottom->setBuildingStory(topStory);
  topBottom->setName("topBottom");

  // top left
  vertices = {
    {-12.4434, 8.2956, 0},
    {-7.8714, 3.7236, 0},
    {-7.8714, -3.7236, 0},
    {-12.4434, -8.2956, 0},
  };
  boost::optional<Space> topLeft = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topLeft);
  topLeft->setZOrigin(3);
  topLeft->setBuildingStory(topStory);
  topLeft->setName("topLeft");

  // create thermal zones
  std::vector<Space> spaces = m.getConcreteModelObjects<Space>();
  for (auto& space : spaces) {
    ThermalZone z(m);
    space.setThermalZone(z);
  }

  // Rename surfaces to be able to debug what's going on
  for (auto& space : spaces) {
    int n_floors = 0;
    int n_walls = 0;
    int n_roofs = 0;
    for (auto& surface : space.surfaces()) {
      if (istringEqual("Floor", surface.surfaceType())) {
        surface.setName(space.nameString() + " Floor " + std::to_string(n_floors++));
      } else if (istringEqual("RoofCeiling", surface.surfaceType())) {
        surface.setName(space.nameString() + " RoofCeiling " + std::to_string(n_roofs++));
      } else if (istringEqual("Wall", surface.surfaceType())) {
        surface.setName(space.nameString() + " Wall " + std::to_string(n_walls++));
      }
    }
  }

  // Debug
  // openstudio::path outpath = resourcesPath() / toPath("model/Space_intersectSurfaces_degenerate1_before_intersect.osm");
  // m.save(outpath, true);

  intersectSurfaces(spaces);
  matchSurfaces(spaces);

  SurfaceAreaCounter sfCounter(m);

  EXPECT_NEAR(sfCounter.exteriorFloorArea, 825.8048, 0.01);
  EXPECT_NEAR(sfCounter.interiorFloorArea, 412.9019, 0.01);
  EXPECT_NEAR(sfCounter.exteriorRoofArea, 825.8048, 0.01);
  EXPECT_NEAR(sfCounter.interiorRoofArea, 412.9019, 0.01);
  EXPECT_EQ(sfCounter.numRoofSurfaces, 11);

  // Debug
  // outpath = resourcesPath() / toPath("model/Space_intersectSurfaces_degenerate1_after_intersect.osm");
  // m.save(outpath, true);
}

TEST_F(ModelFixture, Space_intersectSurfaces_degenerate2) {
  Model m;
  std::vector<Point3d> vertices;

  // bottom floor
  BuildingStory bottomStory(m);

  // bottom core
  vertices = {
    {-13.0256, 7.1598, 0},
    {13.0256, 7.1598, 0},
    {13.0256, -7.1598, 0},
    {-13.0256, -7.1598, 0},
  };
  boost::optional<Space> bottomCore = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomCore);
  bottomCore->setZOrigin(0);
  bottomCore->setBuildingStory(bottomStory);

  // bottom top
  vertices = {
    {-17.5976, 11.7318, 0},
    {17.5976, 11.7318, 0},
    {13.0256, 7.1598, 0},
    {-13.0256, 7.1598, 0},
  };
  boost::optional<Space> bottomTop = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomTop);
  bottomTop->setZOrigin(0);
  bottomTop->setBuildingStory(bottomStory);

  // bottom right
  vertices = {
    {17.5976, 11.7318, 0},
    {17.5976, -11.7318, 0},
    {13.0256, -7.1598, 0},
    {13.0256, 7.1598, 0},
  };
  boost::optional<Space> bottomRight = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomRight);
  bottomRight->setZOrigin(0);
  bottomRight->setBuildingStory(bottomStory);

  // bottom bottom
  vertices = {
    {17.5976, -11.7318, 0},
    {-17.5976, -11.7318, 0},
    {-13.0256, -7.1598, 0},
    {13.0256, -7.1598, 0},
  };
  boost::optional<Space> bottomBottom = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomBottom);
  bottomBottom->setZOrigin(0);
  bottomBottom->setBuildingStory(bottomStory);

  // bottom left
  vertices = {
    {-17.5976, 11.7318, 0},
    {-13.0256, 7.1598, 0},
    {-13.0256, -7.1598, 0},
    {-17.5976, -11.7318, 0},
  };
  boost::optional<Space> bottomLeft = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomLeft);
  bottomLeft->setZOrigin(0);
  bottomLeft->setBuildingStory(bottomStory);

  // top floor
  BuildingStory topStory(m);

  // top core
  vertices = {
    {-7.8714, 3.7236, 0},
    {7.8714, 3.7236, 0},
    {7.8714, -3.7236, 0},
    {-7.8714, -3.7236, 0},
  };
  boost::optional<Space> topCore = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topCore);
  topCore->setZOrigin(3);
  topCore->setBuildingStory(topStory);

  // top top
  vertices = {
    {-14.1614, 8.2956, 0},
    {14.1614, 8.2956, 0},
    {7.8714, 3.7236, 0},
    {-7.8714, 3.7236, 0},
  };
  boost::optional<Space> topTop = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topTop);
  topTop->setZOrigin(3);
  topTop->setBuildingStory(topStory);

  // top right
  vertices = {
    {14.1614, 8.2956, 0},
    {14.1614, -8.2956, 0},
    {7.8714, -3.7236, 0},
    {7.8714, 3.7236, 0},
  };
  boost::optional<Space> topRight = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topRight);
  topRight->setZOrigin(3);
  topRight->setBuildingStory(topStory);

  // top bottom
  vertices = {
    {14.1614, -8.2956, 0},
    {-14.1614, -8.2956, 0},
    {-7.8714, -3.7236, 0},
    {7.8714, -3.7236, 0},
  };
  boost::optional<Space> topBottom = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topBottom);
  topBottom->setZOrigin(3);
  topBottom->setBuildingStory(topStory);

  // top left
  vertices = {
    {-14.1614, 8.2956, 0},
    {-7.8714, 3.7236, 0},
    {-7.8714, -3.7236, 0},
    {-14.1614, -8.2956, 0},
  };
  boost::optional<Space> topLeft = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topLeft);
  topLeft->setZOrigin(3);
  topLeft->setBuildingStory(topStory);

  // create thermal zones
  std::vector<Space> spaces = m.getConcreteModelObjects<Space>();
  //std::sort(spaces.begin(), spaces.end(), WorkspaceObjectNameLess());

  for (auto& space : spaces) {
    ThermalZone z(m);
    space.setThermalZone(z);
  }

  //openstudio::path outpath1 = resourcesPath() / toPath("model/Space_intersectSurfaces_degenerate2_before_intersect.osm");
  //m.save(outpath1, true);

  intersectSurfaces(spaces);
  matchSurfaces(spaces);

  SurfaceAreaCounter sfCounter(m);

  // We know there are two small surfaces that are being generated that don't have opposite
  // surfaces and therefore are being set as Outdoors, the small surfaces are approx 0.008m2 and triangular.
  // Small surfaces but enougb to throw off the area check and the surface count (obviously)
  EXPECT_NEAR(sfCounter.exteriorFloorArea, 825.8048, 0.01);
  //EXPECT_NEAR(sfCounter.interiorFloorArea, 412.9019, 0.01);
  EXPECT_NEAR(sfCounter.exteriorRoofArea, 825.8048, 0.01);
  //EXPECT_NEAR(sfCounter.interiorRoofArea, 412.9019, 0.01);
  EXPECT_EQ(sfCounter.numRoofSurfaces, 9);

  //openstudio::path outpath = resourcesPath() / toPath("model/Space_intersectSurfaces_degenerate2_after_intersect.osm");
  //m.save(outpath, true);
}

TEST_F(ModelFixture, Space_intersectSurfaces_degenerate3) {
  Model m;
  std::vector<Point3d> vertices;

  // bottom floor

  // bottom core
  vertices = {
    {-13.0256, 7.1598, 0},
    {13.0256, 7.1598, 0},
    {13.0256, -7.1598, 0},
    {-13.0256, -7.1598, 0},
  };
  boost::optional<Space> bottomCore = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomCore);
  bottomCore->setZOrigin(0);

  // bottom top
  vertices = {
    {-17.5976, 11.7318, 0},
    {17.5976, 11.7318, 0},
    {13.0256, 7.1598, 0},
    {-13.0256, 7.1598, 0},
  };
  boost::optional<Space> bottomTop = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomTop);
  bottomTop->setZOrigin(0);

  // bottom right
  vertices = {
    {17.5976, 11.7318, 0},
    {17.5976, -11.7318, 0},
    {13.0256, -7.1598, 0},
    {13.0256, 7.1598, 0},
  };
  boost::optional<Space> bottomRight = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomRight);
  bottomRight->setZOrigin(0);

  // bottom bottom
  vertices = {
    {17.5976, -11.7318, 0},
    {-17.5976, -11.7318, 0},
    {-13.0256, -7.1598, 0},
    {13.0256, -7.1598, 0},
  };
  boost::optional<Space> bottomBottom = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomBottom);
  bottomBottom->setZOrigin(0);

  // bottom left
  vertices = {
    {-17.5976, 11.7318, 0},
    {-13.0256, 7.1598, 0},
    {-13.0256, -7.1598, 0},
    {-17.5976, -11.7318, 0},
  };
  boost::optional<Space> bottomLeft = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(bottomLeft);
  bottomLeft->setZOrigin(0);

  // top floor

  // top core
  vertices = {
    {-7.8714, 3.7236, 0},
    {7.8714, 3.7236, 0},
    {7.8714, -3.7236, 0},
    {-7.8714, -3.7236, 0},
  };
  boost::optional<Space> topCore = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topCore);
  topCore->setZOrigin(3);

  // top top
  vertices = {
    {-15.4434, 8.2956, 0},
    {15.4434, 8.2956, 0},
    {7.8714, 3.7236, 0},
    {-7.8714, 3.7236, 0},
  };
  boost::optional<Space> topTop = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topTop);
  topTop->setZOrigin(3);

  // top right
  vertices = {
    {15.4434, 8.2956, 0},
    {15.4434, -8.2956, 0},
    {7.8714, -3.7236, 0},
    {7.8714, 3.7236, 0},
  };
  boost::optional<Space> topRight = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topRight);
  topRight->setZOrigin(3);

  // top bottom
  vertices = {
    {15.4434, -8.2956, 0},
    {-15.4434, -8.2956, 0},
    {-7.8714, -3.7236, 0},
    {7.8714, -3.7236, 0},
  };
  boost::optional<Space> topBottom = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topBottom);
  topBottom->setZOrigin(3);

  // top left
  vertices = {
    {-15.4434, 8.2956, 0},
    {-7.8714, 3.7236, 0},
    {-7.8714, -3.7236, 0},
    {-15.4434, -8.2956, 0},
  };
  boost::optional<Space> topLeft = Space::fromFloorPrint(vertices, 3, m);
  ASSERT_TRUE(topLeft);
  topLeft->setZOrigin(3);

  // create thermal zones
  std::vector<Space> spaces = m.getConcreteModelObjects<Space>();
  for (auto& space : spaces) {
    ThermalZone z(m);
    space.setThermalZone(z);
  }

  intersectSurfaces(spaces);
  matchSurfaces(spaces);

  SurfaceAreaCounter sfCounter(m);

  EXPECT_NEAR(sfCounter.exteriorFloorArea, 825.8048, 0.01);
  //EXPECT_NEAR(sfCounter.interiorFloorArea, 412.9019, 0.01);
  EXPECT_NEAR(sfCounter.exteriorRoofArea, 825.8048, 0.01);
  //EXPECT_NEAR(sfCounter.interiorRoofArea, 412.9019, 0.01);
  EXPECT_EQ(sfCounter.numRoofSurfaces, 9);

  //openstudio::path outpath = resourcesPath() / toPath("model/Space_intersectSurfaces_degenerate3_after_intersect.osm");
  //m.save(outpath, true);
}

// Digital Alchemy

boost::optional<Model> CreatePerimeterTestModel() {
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

// Illustrates a fix for surface intersection getting stuck in a loop
// First of all we need to remove surfaces that overlap within the same space
// Second of all we use a different removeSpikes method that shrinks and expands the polygon

TEST_F(ModelFixture, RemoveSpikesAndOverlaps_TZ46_TZ47) {
  Model model;
  openstudio::path path = resourcesPath() / toPath("/model/RemoveSpikesAndOverlaps_TZ46_TZ47");

#ifdef WIN32
#  pragma region SPACE 1(TZ46 - 81)
#endif

  Space space(model);
  ASSERT_TRUE(space.name());
  EXPECT_EQ("Space 1", space.name().get());  //TZ46-81

  Point3dVector points{
    {0.787401574803132, 3.14960629921254, 3.84078906495842e-15},
    {2.75590551181098, 1.18110236220469, 7.70979364755774e-15},
    {2.00599925009369, 0.356205474315723, 3.76899861514623e-15},
    {0, 2.36220472440939, 0},
  };

  Surface surface(points, model);
  surface.setParent(space);

  points = {
    {1.37795275590549, 0.984251968503903, 3.51739188744401},
    {5.31496062992121, 4.92125984251968, 3.51739188744401},
    {3.93700787401569, 6.2992125984252, 3.51739188744401},
    {0, 2.36220472440939, 3.51739188744401},
  };

  Surface surface2(points, model);
  surface2.setParent(space);

  points = {
    {5.11811023622049, 2.75590551181099, 3.51739188744401},
    {5.11811023622049, 2.75590551181099, 0},
    {6.2992125984252, 3.93700787401571, 0},
    {6.2992125984252, 3.93700787401571, 3.51739188744401},
  };

  Surface surface3(points, model);
  surface3.setParent(space);

  points = {
    {2.36220472440939, 0, 3.51739188744401},
    {2.36220472440939, 0, 0},
    {5.11811023622049, 2.75590551181099, 0},
    {5.11811023622049, 2.75590551181099, 3.51739188744401},
  };

  Surface surface4(points, model);
  surface4.setParent(space);

  points = {
    {0, 2.36220472440939, 3.51739188744401},
    {0, 2.36220472440939, 0},
    {2.36220472440939, 0, 0},
    {2.36220472440939, 0, 3.51739188744401},
  };

  Surface surface5(points, model);
  surface5.setParent(space);

  points = {
    {3.93700787401569, 6.2992125984252, 3.51739188744401},
    {3.93700787401569, 6.2992125984252, 0},
    {0, 2.36220472440939, 0},
    {0, 2.36220472440939, 3.51739188744401},
  };

  Surface surface6(points, model);
  surface6.setParent(space);

  points = {
    {6.2992125984252, 3.93700787401571, 3.51739188744401},
    {6.2992125984252, 3.93700787401571, 0},
    {3.93700787401569, 6.2992125984252, 0},
    {3.93700787401569, 6.2992125984252, 3.51739188744401},
  };

  Surface surface7(points, model);
  surface7.setParent(space);

  points = {
    {2.3622047244094, -1.44382283906452e-15, 3.51739188744401},
    {6.2992125984252, 3.93700787401571, 3.51739188744401},
    {5.31496062992121, 4.92125984251968, 3.51739188744401},
    {1.37795275590549, 0.984251968503901, 3.51739188744401},
  };

  Surface surface8(points, model);
  surface8.setParent(space);

  points = {
    {3.93700787401569, 6.2992125984252, 0},
    {6.2992125984252, 3.93700787401571, 0},
    {2.3622047244094, -2.88764567812905e-15, 0},
    {2.00599925009369, 0.356205474315719, 4.33146851719357e-15},
    {2.75590551181099, 1.18110236220469, 7.21911419532262e-15},
    {0.787401574803138, 3.14960629921254, 4.33146851719357e-15},
  };

  Surface surface9(points, model);
  surface9.setParent(space);

#ifdef WIN32
#  pragma endregion

#  pragma region SPACE 2(TZ47 - 91)
#endif
  Space space2(model);
  ASSERT_TRUE(space2.name());
  EXPECT_EQ("Space 2", space2.name().get());  //TZ47-91

  points = {
    {7.0866141732284, 9.44881889763781, 0},
    {8.16034359341449, 8.55404438081602, -1.44382283906452e-15},
    {5.90551181102359, 6.2992125984252, -1.44382283906452e-15},
    {4.92125984251972, 7.28346456692913, -7.21911419532262e-16},
  };

  Surface surface10(points, model);
  surface10.setParent(space2);

  points = {
    {1.96850393700789, -2.88764567812905e-15, 6.095999804928},
    {9.44881889763779, 7.4803149606299, 6.095999804928},
    {7.0866141732284, 9.44881889763781, 6.095999804928},
    {-5.77529135625809e-15, 2.3622047244094, 6.095999804928},
  };

  Surface surface11(points, model);
  surface11.setParent(space2);

  points = {
    {7.0866141732284, 9.44881889763781, 6.095999804928},
    {7.0866141732284, 9.44881889763781, 0},
    {-5.77529135625809e-15, 2.3622047244094, 0},
    {-5.77529135625809e-15, 2.3622047244094, 6.095999804928},
  };

  Surface surface12(points, model);
  surface12.setParent(space2);

  points = {
    {9.44881889763779, 7.4803149606299, 6.095999804928},
    {9.44881889763779, 7.4803149606299, 0},
    {7.0866141732284, 9.44881889763781, 0},
    {7.0866141732284, 9.44881889763781, 6.095999804928},
  };

  Surface surface13(points, model);
  surface13.setParent(space2);

  points = {
    {1.96850393700789, -2.88764567812905e-15, 6.095999804928},
    {1.96850393700789, -2.88764567812905e-15, 0},
    {9.44881889763779, 7.4803149606299, 0},
    {9.44881889763779, 7.4803149606299, 6.095999804928},
  };

  Surface surface14(points, model);
  surface14.setParent(space2);

  points = {
    {-5.77529135625809e-15, 2.3622047244094, 6.095999804928},
    {-5.77529135625809e-15, 2.3622047244094, 0},
    {1.96850393700789, -2.88764567812905e-15, 0},
    {1.96850393700789, -2.88764567812905e-15, 6.095999804928},
  };

  Surface surface15(points, model);
  surface15.setParent(space2);

  points = {
    {8.16034359341449, 8.55404438081602, -1.44382283906452e-15},
    {9.44881889763779, 7.4803149606299, 0},
    {1.96850393700789, -2.88764567812905e-15, 0},
    {-5.77529135625809e-15, 2.3622047244094, 0},
    {4.92125984251972, 7.28346456692913, -7.21911419532262e-16},
    {5.90551181102359, 6.2992125984252, -1.44382283906452e-15},
  };

  Surface surface16(points, model);
  surface16.setParent(space2);

#ifdef WIN32
#  pragma endregion
#endif

  EXPECT_EQ(static_cast<unsigned>(2), model.getConcreteModelObjects<Space>().size());
  EXPECT_EQ(static_cast<unsigned>(16), model.getConcreteModelObjects<Surface>().size());

  EXPECT_TRUE(space.setTransformation(Transformation::translation(Vector3d(-34.6329065993854, 2.80342559613037, -9.61339169237201))
                                      * Transformation::rotation(Vector3d(0, 0, 1), -openstudio::degToRad(24.5))));
  EXPECT_DOUBLE_EQ(-34.6329065993854, space.xOrigin());
  EXPECT_DOUBLE_EQ(2.80342559613037, space.yOrigin());
  EXPECT_DOUBLE_EQ(-9.61339169237201, space.zOrigin());
  EXPECT_DOUBLE_EQ(24.5, space.directionofRelativeNorth());

  EXPECT_TRUE(space2.setTransformation(Transformation::translation(Vector3d(-40.4694197176487, -1.45922839997927, -6.095999804928))
                                       * Transformation::rotation(Vector3d(0, 0, 1), -openstudio::degToRad(24.5))));
  EXPECT_DOUBLE_EQ(-40.4694197176487, space2.xOrigin());
  EXPECT_DOUBLE_EQ(-1.45922839997927, space2.yOrigin());
  EXPECT_DOUBLE_EQ(-6.095999804928, space2.zOrigin());
  EXPECT_DOUBLE_EQ(24.5, space2.directionofRelativeNorth());

  SpaceVector spaces = model.getConcreteModelObjects<Space>();
  SpaceVector blacklist;

  int nSurfaces = 0;
  for (const auto& space : spaces) {
    nSurfaces += space.surfaces().size();
  }

  EXPECT_EQ(nSurfaces, 16);

  intersectSurfaces(spaces);

  nSurfaces = 0;
  for (const auto& space : spaces) {
    nSurfaces += space.surfaces().size();
  }

  EXPECT_EQ(nSurfaces, 17);

  matchSurfaces(spaces);

  // openstudio::path outPath = path;
  // outPath.replace_extension(openstudio::toPath("_finished.osm"));
  // model.save(outPath);
}

// Sorts a list of surfaces by ascending vertex count
bool sortSurfacesByNumberVertices(const Surface& a, const Surface& b) {
  return a.vertices().size() < b.vertices().size();
}

// Tests how concave surfaces are handled
TEST_F(ModelFixture, Surface_Intersect_ConcaveSurfaces) {

  Model model;
  Space sp1(model);

  double z = 1;

  Point3dVector top{
    {3, 0, z},
    {3, 5, z},
    {7, 5, z},
    {7, 0, z},
  };
  auto normTop = getOutwardNormal(top);
  EXPECT_NEAR(normTop->z(), -1, 0.01);
  Surface s1(top, model);
  s1.setParent(sp1);

  Space sp2(model);
  Point3dVector bottom{
    {0, 0, z},
    {10, 0, z},
    {10, 10, z},
    {0, 10, z},
  };
  auto normBottom = getOutwardNormal(bottom);
  EXPECT_NEAR(normBottom->z(), 1, 0.01);
  Surface s2(bottom, model);
  s2.setParent(sp2);

  SpaceVector spaces;
  spaces.push_back(sp1);
  spaces.push_back(sp2);
  intersectSurfaces(spaces);
  matchSurfaces(spaces);

  EXPECT_EQ(1, sp1.surfaces().size());
  EXPECT_EQ(4, sp1.surfaces().front().vertices().size());

  auto space2Surfaces = sp2.surfaces();
  std::sort(space2Surfaces.begin(), space2Surfaces.end(), sortSurfacesByNumberVertices);
  EXPECT_EQ(2, sp2.surfaces().size());
  EXPECT_EQ(4, space2Surfaces.front().vertices().size());
  EXPECT_EQ(8, space2Surfaces.back().vertices().size());
}

TEST_F(ModelFixture, Issue_2560) {
  Model model;

  BuildingStory story2(model);
  Point3dVector perimeter{
    {4, 2, 3},
    {4, 6, 3},
    {18, 6, 3},
    {18, 2, 3},
  };
  auto sp1 = Space::fromFloorPrint(perimeter, 3, model);
  ASSERT_TRUE(sp1);
  ThermalZone tz1(model);
  sp1->setThermalZone(tz1);
  sp1->setBuildingStory(story2);

  BuildingStory story1(model);
  perimeter = {
    {0, 0, 0},
    {0, 3, 0},
    {22, 3, 0},
    {22, 0, 0},
  };
  auto sp2 = Space::fromFloorPrint(perimeter, 3, model);
  ASSERT_TRUE(sp2);
  ThermalZone tz2(model);
  sp2->setThermalZone(tz2);
  sp2->setBuildingStory(story1);

  perimeter = {
    {0, 3, 0},
    {0, 5, 0},
    {22, 5, 0},
    {22, 3, 0},
  };
  auto sp3 = Space::fromFloorPrint(perimeter, 3, model);
  ASSERT_TRUE(sp3);
  ThermalZone tz3(model);
  sp3->setThermalZone(tz3);
  sp3->setBuildingStory(story1);

  perimeter = {
    {0, 5, 0},
    {0, 8, 0},
    {22, 8, 0},
    {22, 5, 0},
  };
  auto sp4 = Space::fromFloorPrint(perimeter, 3, model);
  ASSERT_TRUE(sp4);
  ThermalZone tz4(model);
  sp4->setThermalZone(tz4);
  sp4->setBuildingStory(story1);

  // openstudio::path outPath = resourcesPath() / toPath("2560_before.osm");
  // model.save(outPath);

  SpaceVector spaces = model.getConcreteModelObjects<Space>();
  intersectSurfaces(spaces);
  matchSurfaces(spaces);

  // outPath = resourcesPath() / toPath("2560_after.osm");
  // model.save(outPath);

  // Verify that the floor surfaces on space 1 are matched
  auto space1Surfaces = sp1->surfaces();

  auto it = std::find_if(space1Surfaces.begin(), space1Surfaces.end(), [](const Surface& s) { return s.nameString() == "Surface 1"; });
  ASSERT_TRUE(it != space1Surfaces.end());
  ASSERT_TRUE(it->adjacentSurface());
  auto s = it->adjacentSurface();
  EXPECT_EQ(s->nameString(), "Surface 18");

  it = std::find_if(space1Surfaces.begin(), space1Surfaces.end(), [](const Surface& s) { return s.nameString() == "Surface 27"; });
  ASSERT_TRUE(it != space1Surfaces.end());
  ASSERT_TRUE(it->adjacentSurface());
  s = it->adjacentSurface();
  EXPECT_EQ(s->nameString(), "Surface 12");

  it = std::find_if(space1Surfaces.begin(), space1Surfaces.end(), [](const Surface& s) { return s.nameString() == "Surface 28"; });
  ASSERT_TRUE(it != space1Surfaces.end());
  ASSERT_TRUE(it->adjacentSurface());
  s = it->adjacentSurface();
  EXPECT_EQ(s->nameString(), "Surface 24");
}

TEST_F(ModelFixture, Issue_3982) {
  Model model;
  Space sp1(model);

  // Create a rectangular surface and an overlapping triangular surface and intersect them
  Point3dVector faceVertices{
    {0, 0, 0},
    {50, 0, 0},
    {50, 10, 0},
    {0, 10, 0},
  };
  Surface s1(faceVertices, model);
  s1.setParent(sp1);

  Space sp2(model);
  Point3dVector otherFaceVertices{
    {25, 0, 0},
    {37.50, 8, 0},
    {50, 0, 0},
  };
  Surface s2(otherFaceVertices, model);
  s2.setParent(sp2);

  SpaceVector spaces;
  spaces.push_back(sp1);
  spaces.push_back(sp2);
  intersectSurfaces(spaces);

  auto space1Surfaces = sp1.surfaces();
  std::sort(space1Surfaces.begin(), space1Surfaces.end(), sortSurfacesByNumberVertices);
  EXPECT_EQ(2, space1Surfaces.size());
  EXPECT_EQ(3, space1Surfaces[0].vertices().size());
  EXPECT_EQ(6, space1Surfaces[1].vertices().size());

  auto space2Surfaces = sp2.surfaces();
  EXPECT_EQ(1, space2Surfaces.size());
  EXPECT_EQ(3, space2Surfaces[0].vertices().size());
}

TEST_F(ModelFixture, Perimeter) {

  model::OptionalModel model = CreatePerimeterTestModel();

  EXPECT_TRUE(model);

  std::vector<Point3dVector> polygons;
  std::vector<Space> spaces;
  std::vector<std::string> spaceNames;
  std::vector<Surface> surfaces;

  for (const auto& space : model->getConcreteModelObjects<Space>()) {
    std::string spacename = space.name().value();
    Transformation spaceTransformation = space.transformation();
    for (const auto& surface : space.surfaces()) {
      std::string surfname = surface.name().value();
      Point3dVector points = spaceTransformation * surface.vertices();
      auto normal = openstudio::getOutwardNormal(points);
      if (normal) {
        if (normal->z() == -1) {
          if (points[0].z() == 0) {
            polygons.push_back(points);
            spaces.push_back(space);
            spaceNames.push_back(spacename);
          }
        }
      }
    }
  }

  // The traditional method
  auto result1 = openstudio::joinAll(polygons, 0.01);
  ASSERT_EQ(1, result1.size());
  EXPECT_EQ(12, result1[0].size());

  // Using polygons
  auto result2 = openstudio::joinAllPolygons(polygons, 0.01);
  ASSERT_EQ(1, result2.size());
  EXPECT_EQ(12, result2[0].getOuterPath().size());

  // Calculate perimeter
  Polygon3d footprint = result2[0];
  double perimeter = footprint.perimeter();
  EXPECT_NEAR(perimeter, 1428.0, 0.01);

  // Compare points list to polygon
  EXPECT_TRUE(circularEqual(result1.front(), footprint.getOuterPath()));
}

// Checks the exposed perimeter calculation for each down facing surface at ground level
TEST_F(ModelFixture, ExposedPerimeter) {

  model::OptionalModel model = CreatePerimeterTestModel();

  EXPECT_TRUE(model);

  std::vector<Point3dVector> polygons;
  std::vector<Space> spaces;
  std::vector<Surface> surfaces;

  // Iterate over spaces, get their surfaces and transform to model coordinates,
  // find the surfaces with downward facing normal at elevation of 0
  for (const auto& space : model->getConcreteModelObjects<Space>()) {
    Transformation spaceTransformation = space.transformation();
    std::string spacename = space.name().value();
    for (const auto& surface : space.surfaces()) {
      bool added = false;
      Point3dVector points = spaceTransformation * surface.vertices();
      auto normal = openstudio::getOutwardNormal(points);
      if (normal) {
        if (normal->z() < 0) {
          if (points[0].z() == 0) {
            polygons.push_back(points);
            spaces.push_back(space);
            if (!added) {
              added = true;
              surfaces.push_back(surface);
            }
          }
        }
      }
    }
  }

  ASSERT_EQ(spaces.size(), 20);

  // Join all those polygons into one
  auto result2 = openstudio::joinAllPolygons(polygons, 0.01);
  ASSERT_EQ(1, result2.size());
  EXPECT_EQ(12, result2[0].getOuterPath().size());
  Polygon3d footprint = result2.front();

  for (const auto& surface : surfaces) {
    std::string surfname = surface.name().value();
    double exposedPerimeter = 0;
    int nOverlaps = 0;
    boost::optional<Space> space = surface.space();
    if (!space) {
      continue;
    }
    std::string spacename = space->name().value();
    Transformation spaceTransformation = space->transformation();
    Point3dVector points = spaceTransformation * surface.vertices();

    // Find points/edges on the perimeter that points on the surface are coincident with
    for (size_t i = 0; i < points.size(); i++) {
      Point3d p1 = points[i];
      Point3d p2 = points[(i + 1) % points.size()];
      Point3dVector line;
      line.push_back(p1);
      line.push_back(p2);
      Point3dVectorVector overlaps = footprint.overlap(line);
      for (const auto& overlap : overlaps) {
        exposedPerimeter += openstudio::getDistance(overlap[0], overlap[1]);
        nOverlaps++;
      }
    }

    // Get the space perimeter from the space and compare
    auto spacePerimeter = space->exposedPerimeter(footprint);
    EXPECT_EQ(exposedPerimeter, spacePerimeter);

    if (spacename == "Artroom 103") {
      EXPECT_EQ(nOverlaps, 1);
      EXPECT_NEAR(exposedPerimeter, 60, 0.01);
    } else if (spacename == "Bathroom 115") {
      EXPECT_EQ(nOverlaps, 1);
      EXPECT_NEAR(exposedPerimeter, 32, 0.01);
    } else if (spacename == "Classroom 101" || spacename == "Classroom 105" || spacename == "Classroom 109" || spacename == "Classroom 111") {
      EXPECT_EQ(nOverlaps, 2);
      EXPECT_NEAR(exposedPerimeter, 66, 0.01);
    } else if (spacename == "Classroom 102" || spacename == "Lobby 113") {
      EXPECT_EQ(nOverlaps, 1);
      EXPECT_NEAR(exposedPerimeter, 62, 0.01);
    } else if (spacename == "Classroom 106" || spacename == "Classroom 108" || spacename == "Classroom 112") {
      EXPECT_EQ(nOverlaps, 1);
      EXPECT_NEAR(exposedPerimeter, 122, 0.01);
    } else if (spacename == "Gym 118") {
      EXPECT_EQ(nOverlaps, 1);
      EXPECT_NEAR(exposedPerimeter, 54, 0.01);
    } else if (spacename == "Kitchen 119") {
      EXPECT_EQ(nOverlaps, 1);
      EXPECT_NEAR(exposedPerimeter, 26, 0.01);
    } else if (spacename == "Mechanical 114") {
      EXPECT_EQ(nOverlaps, 0);
      EXPECT_NEAR(exposedPerimeter, 0, 0.01);
    } else if (spacename == "Media Center 116") {
      EXPECT_EQ(nOverlaps, 3);
      EXPECT_NEAR(exposedPerimeter, 198, 0.01);
    } else if (spacename == "Offices 117") {
      EXPECT_EQ(nOverlaps, 2);
      EXPECT_NEAR(exposedPerimeter, 138, 0.01);
    } else if (spacename == "Corridor 104" || spacename == "Corridor 110") {
      EXPECT_EQ(nOverlaps, 1);
      EXPECT_NEAR(exposedPerimeter, 10, 0.01);
    } else if (spacename == "Corridor 107") {
      EXPECT_EQ(nOverlaps, 1);
      EXPECT_NEAR(exposedPerimeter, 28, 0.01);
    } else if (spacename == "Cafeteria 120") {
      EXPECT_EQ(nOverlaps, 2);
      EXPECT_NEAR(exposedPerimeter, 118, 0.01);
    } else {
      // Makes sure we've checked all 20 spaces and got the names right
      ASSERT_FALSE(true);
    }
  }
}

TEST_F(ModelFixture, Space_setVolumeAndCeilingHeightAndFloorArea) {
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
  EXPECT_TRUE(space.isVolumeAutocalculated());
  EXPECT_TRUE(space.isVolumeDefaulted());
  EXPECT_TRUE(space.isCeilingHeightAutocalculated());
  EXPECT_TRUE(space.isCeilingHeightDefaulted());
  EXPECT_TRUE(space.isFloorAreaAutocalculated());
  EXPECT_TRUE(space.isFloorAreaDefaulted());
  EXPECT_DOUBLE_EQ(100.0, space.floorArea());
  EXPECT_DOUBLE_EQ(360.0, space.volume());
  EXPECT_DOUBLE_EQ(3.6, space.ceilingHeight());
  EXPECT_DOUBLE_EQ(144.0, space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0, space.exteriorArea());  // ground does not count

  // VOLUME

  // set
  EXPECT_TRUE(space.setVolume(365.0));
  EXPECT_FALSE(space.isVolumeAutocalculated());
  EXPECT_FALSE(space.isVolumeDefaulted());
  EXPECT_DOUBLE_EQ(100.0, space.floorArea());
  EXPECT_DOUBLE_EQ(365.0, space.volume());
  EXPECT_DOUBLE_EQ(3.6, space.ceilingHeight());
  EXPECT_DOUBLE_EQ(144.0, space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0, space.exteriorArea());  // ground does not count

  // reset
  space.resetVolume();
  EXPECT_TRUE(space.isVolumeAutocalculated());
  EXPECT_TRUE(space.isVolumeDefaulted());
  EXPECT_DOUBLE_EQ(100.0, space.floorArea());
  EXPECT_DOUBLE_EQ(360.0, space.volume());
  EXPECT_DOUBLE_EQ(3.6, space.ceilingHeight());
  EXPECT_DOUBLE_EQ(144.0, space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0, space.exteriorArea());  // ground does not count

  // autocalculate
  EXPECT_TRUE(space.setVolume(370.0));
  EXPECT_FALSE(space.isVolumeAutocalculated());
  EXPECT_FALSE(space.isVolumeDefaulted());
  space.autocalculateVolume();
  EXPECT_TRUE(space.isVolumeAutocalculated());
  EXPECT_FALSE(space.isVolumeDefaulted());
  EXPECT_DOUBLE_EQ(100.0, space.floorArea());
  EXPECT_DOUBLE_EQ(360.0, space.volume());
  EXPECT_DOUBLE_EQ(3.6, space.ceilingHeight());
  EXPECT_DOUBLE_EQ(144.0, space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0, space.exteriorArea());  // ground does not count

  // CEILING HEIGHT

  // set
  EXPECT_TRUE(space.setCeilingHeight(8.0));
  EXPECT_FALSE(space.isCeilingHeightAutocalculated());
  EXPECT_FALSE(space.isCeilingHeightDefaulted());
  EXPECT_DOUBLE_EQ(100.0, space.floorArea());
  EXPECT_DOUBLE_EQ(360.0, space.volume());
  EXPECT_DOUBLE_EQ(8.0, space.ceilingHeight());
  EXPECT_DOUBLE_EQ(144.0, space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0, space.exteriorArea());  // ground does not count

  // reset
  space.resetCeilingHeight();
  EXPECT_TRUE(space.isCeilingHeightAutocalculated());
  EXPECT_TRUE(space.isCeilingHeightDefaulted());
  EXPECT_DOUBLE_EQ(100.0, space.floorArea());
  EXPECT_DOUBLE_EQ(360.0, space.volume());
  EXPECT_DOUBLE_EQ(3.6, space.ceilingHeight());
  EXPECT_DOUBLE_EQ(144.0, space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0, space.exteriorArea());  // ground does not count

  // autocalculate
  EXPECT_TRUE(space.setCeilingHeight(9.0));
  EXPECT_FALSE(space.isCeilingHeightAutocalculated());
  EXPECT_FALSE(space.isCeilingHeightDefaulted());
  space.autocalculateCeilingHeight();
  EXPECT_TRUE(space.isCeilingHeightAutocalculated());
  EXPECT_FALSE(space.isCeilingHeightDefaulted());
  EXPECT_DOUBLE_EQ(100.0, space.floorArea());
  EXPECT_DOUBLE_EQ(360.0, space.volume());
  EXPECT_DOUBLE_EQ(3.6, space.ceilingHeight());
  EXPECT_DOUBLE_EQ(144.0, space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0, space.exteriorArea());  // ground does not count

  // FLOOR AREA

  // set
  EXPECT_TRUE(space.setFloorArea(400.0));
  EXPECT_FALSE(space.isFloorAreaAutocalculated());
  EXPECT_FALSE(space.isFloorAreaDefaulted());
  EXPECT_DOUBLE_EQ(400.0, space.floorArea());
  EXPECT_DOUBLE_EQ(360.0, space.volume());
  EXPECT_DOUBLE_EQ(3.6, space.ceilingHeight());
  EXPECT_DOUBLE_EQ(144.0, space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0, space.exteriorArea());  // ground does not count

  // reset
  space.resetFloorArea();
  EXPECT_TRUE(space.isFloorAreaAutocalculated());
  EXPECT_TRUE(space.isFloorAreaDefaulted());
  EXPECT_DOUBLE_EQ(100.0, space.floorArea());
  EXPECT_DOUBLE_EQ(360.0, space.volume());
  EXPECT_DOUBLE_EQ(3.6, space.ceilingHeight());
  EXPECT_DOUBLE_EQ(144.0, space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0, space.exteriorArea());  // ground does not count

  // autocalculate
  EXPECT_TRUE(space.setFloorArea(500.0));
  EXPECT_FALSE(space.isFloorAreaAutocalculated());
  EXPECT_FALSE(space.isFloorAreaDefaulted());
  space.autocalculateFloorArea();
  EXPECT_TRUE(space.isFloorAreaAutocalculated());
  EXPECT_FALSE(space.isFloorAreaDefaulted());
  EXPECT_DOUBLE_EQ(100.0, space.floorArea());
  EXPECT_DOUBLE_EQ(360.0, space.volume());
  EXPECT_DOUBLE_EQ(3.6, space.ceilingHeight());
  EXPECT_DOUBLE_EQ(144.0, space.exteriorWallArea());
  EXPECT_DOUBLE_EQ(244.0, space.exteriorArea());  // ground does not count
}

/*****************************************************************************************************************************************************
*                                                           D I S A B L E D    T E S T S                                                            *
*****************************************************************************************************************************************************/

// This takes 20secs but passes: TODO enable? (this is a bit too long)
TEST_F(ModelFixture, DISABLED_Issue_1322) {

  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/7-7_Windows_Complete.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  EXPECT_TRUE(model);

  SpaceVector spaces = model->getConcreteModelObjects<Space>();

  intersectSurfaces(spaces);
  matchSurfaces(spaces);
}

// This takes 5secs but passes: TODO: enable? (borderline too long to pass)
TEST_F(ModelFixture, DISABLED_Issue_1683) {

  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/15023_Model12.osm");
  model::OptionalModel model = translator.loadModel(modelPath);
  EXPECT_TRUE(model);

  SpaceVector spaces = model->getConcreteModelObjects<Space>();
  intersectSurfaces(spaces);
  matchSurfaces(spaces);

  // openstudio::path outPath = resourcesPath() / toPath("model/15023_Model12_after.osm");
  // model->save(outPath, true);
}

// Skipping this one because this is outside of the current scope.
// To correct this intersection and matching needs to allow holes and then
// decompose polygons with holes as the last step of the process. Often as
// is shown here the holes are removed by the intersection process anyway
TEST_F(ModelFixture, DISABLED_ShatteredModel_Existing_3424) {

  // Issue #3424
  //
  // Polygon decomposition is order dependent. IN this case if the first intersect makes a hole
  // we triangulate, if it doesn't then triangulation is not needed
  // Two space lists 1,3,2 and 4,5,6 where 1 and 4, 2 and 5, and 3 and 6 are the same (but shifted)
  // so intersecting 1,3,3 should give the same result as 4,5,6 if order doesn't matter.
  // In order to prove this we have to not order by area so an additional argument has been added to
  // intersect(spaces) to this affect

  Model model;
  BuildingStory bottom(model);
  BuildingStory top(model);

  Point3dVector points1{
    {25.908, 24.384, 0},
    {25.908, -22.86, 0},
    {-38.1, -22.86, 0},
    {-38.1, 24.384, 0},
  };
  boost::optional<Space> space1 = Space::fromFloorPrint(points1, 2.4384, model);
  ASSERT_TRUE(space1);
  space1->setBuildingStory(bottom);
  // double a1 = getArea(points1).value();

  Point3dVector points2{
    {9.144, 10.668, 2.4384},
    {9.144, -9.144, 2.4384},
    {-19.812, -9.144, 2.4384},
    {-19.812, 10.668, 2.4384},
  };
  boost::optional<Space> space2 = Space::fromFloorPrint(points2, 2.4384, model);
  ASSERT_TRUE(space2);
  space2->setBuildingStory(top);
  // double a2 = getArea(points2).value();

  Point3dVector points3{
    {9.144, 24.384, 2.4384},
    {9.144, 10.668, 2.4384},
    {-38.1, 10.668, 2.4384},
    {-38.1, 24.384, 2.4384},
  };
  boost::optional<Space> space3 = Space::fromFloorPrint(points3, 2.4384, model);
  ASSERT_TRUE(space3);
  space3->setBuildingStory(top);
  // double a3 = getArea(points3).value();

  Point3dVector points4{
    {25.908, 24.384, 0},
    {25.908, -22.86, 0},
    {-38.1, -22.86, 0},
    {-38.1, 24.384, 0},
  };
  boost::optional<Space> space4 = Space::fromFloorPrint(points4, 2.4384, model);
  ASSERT_TRUE(space4);
  space4->setBuildingStory(bottom);
  space4->setXOrigin(75);
  // double a4 = getArea(points4).value();

  Point3dVector points5{
    {9.144, 10.668, 2.4384},
    {9.144, -9.144, 2.4384},
    {-19.812, -9.144, 2.4384},
    {-19.812, 10.668, 2.4384},
  };
  boost::optional<Space> space5 = Space::fromFloorPrint(points5, 2.4384, model);
  ASSERT_TRUE(space5);
  space5->setBuildingStory(top);
  space5->setXOrigin(75);
  // double a5 = getArea(points5).value();

  Point3dVector points6{
    {9.144, 24.384, 2.4384},
    {9.144, 10.668, 2.4384},
    {-38.1, 10.668, 2.4384},
    {-38.1, 24.384, 2.4384},
  };
  boost::optional<Space> space6 = Space::fromFloorPrint(points6, 2.4384, model);
  ASSERT_TRUE(space6);
  space6->setBuildingStory(top);
  space6->setXOrigin(75);
  // double a6 = getArea(points6).value();

  // Make two lists of spaces (so we can call intersect with the spaces in different orders)
  std::vector<Space> spaces1;
  spaces1.push_back(*space1);
  spaces1.push_back(*space3);
  spaces1.push_back(*space2);

  std::vector<Space> spaces2;
  spaces2.push_back(*space4);
  spaces2.push_back(*space5);
  spaces2.push_back(*space6);

  //std::vector<PolygonGroup> polygonGroups1;
  //polygonGroups1.push_back(space1->ToPolygonGroup());
  //polygonGroups1.push_back(space2->ToPolygonGroup());
  //polygonGroups1.push_back(space3->ToPolygonGroup());
  //std::vector<PolygonGroup> polygonGroups2;
  //PolygonGroup& g4 = space4->ToPolygonGroup();
  //polygonGroups2.push_back(space4->ToPolygonGroup());
  //polygonGroups2.push_back(space5->ToPolygonGroup());
  //polygonGroups2.push_back(space6->ToPolygonGroup());

  // Model before intersection
  // openstudio::path outpath = resourcesPath() / toPath("model/ShatterTest00.osm");
  // model.save(outpath, true);
  // LOG(Info, "Saved model before intersection");

  // Run the existing code
  ASSERT_NO_THROW(intersectSurfaces(spaces1)) << "Failed during first surface intersections";

  ASSERT_NO_THROW(intersectSurfaces(spaces2)) << "Failed during second surface intersections";

  // Model after intersection
  // outpath = resourcesPath() / toPath("model/ShatterTest00.osm");
  // model.save(outpath, true);
  // LOG(Info, "Saved model after intersection");

  EXPECT_EQ(8, space1->surfaces().size());
  EXPECT_EQ(12, space4->surfaces().size());

  std::vector<Surface> ceilingSpace1;
  std::vector<Surface> ceilingSpace4;

  for (const auto& surface : space1->surfaces()) {
    if (getOutwardNormal(surface.vertices())->z() == 1) {
      ceilingSpace1.push_back(surface);
    }
  }
  for (const auto& surface : space4->surfaces()) {
    if (getOutwardNormal(surface.vertices())->z() == 1) {
      ceilingSpace4.push_back(surface);
    }
  }

  EXPECT_EQ(3, ceilingSpace1.size());
  EXPECT_EQ(7, ceilingSpace4.size());
}

TEST_F(ModelFixture, Space_Polyhedron_Volume) {

  Model m;
  Space s(m);

  // This is a 30x10x0.3 base, with a rectangle triangle on top of 30x10x10
  //                       ▲ z
  //                       │
  //                      x├─ 10.3
  //                    x  │
  //                  x    │
  //                x      │
  //              x        │
  //            x          │
  //          x            │
  //        x              ├─ 0.3
  //        │              │
  //   ◄────┼──────────────┼─
  //  y    10.0            0.0

  Surface south2({{+15.0, +0.0, +10.3}, {+15.0, +0.0, +0.0}, {+30.0, +0.0, +0.0}, {+30.0, +0.0, +10.3}}, m);
  south2.setName("1-SOUTH-2");
  south2.setSpace(s);

  // Putting extra vertices here on purpose to show that the Space::volume was miscalculating due to averaging foor and ceiling heights
  Surface roof({{+30.0, +0.0, +10.3}, {+30.0, +10.0, +0.3}, {+0.0, +10.0, +0.3}, {+0.0, +0.0, +10.3}, {+10.0, +0.0, +10.3}, {+20.0, +0.0, +10.3}}, m);
  roof.setName("ROOF");
  roof.setSpace(s);

  Surface east({{+30.0, +0.0, +10.3}, {+30.0, +0.0, +0.0}, {+30.0, +10.0, +0.0}, {+30.0, +10.0, +0.3}}, m);
  east.setName("3-EAST");
  east.setSpace(s);

  Surface north({{+30.0, +10.0, +0.3}, {+30.0, +10.0, +0.0}, {+0.0, +10.0, +0.0}, {+0.0, +10.0, +0.3}}, m);
  north.setName("4-NORTH");
  north.setSpace(s);

  Surface west({{+0.0, +10.0, +0.3}, {+0.0, +10.0, +0.0}, {+0.0, +0.0, +0.0}, {+0.0, +0.0, +10.3}}, m);
  west.setName("2-WEST");
  west.setSpace(s);

  Surface south1({{+0.0, +0.0, +10.3}, {+0.0, +0.0, +0.0}, {+15.0, +0.0, +0.0}, {+15.0, +0.0, +10.3}}, m);
  south1.setName("1-SOUTH-1");
  south1.setSpace(s);

  Surface floor({{+0.0, +0.0, +0.0}, {+0.0, +10.0, +0.0}, {+30.0, +10.0, +0.0}, {+30.0, +0.0, +0.0}}, m);
  floor.setName("FLOOR");
  floor.setSpace(s);

  auto wrongOrientations = s.findSurfacesWithIncorrectOrientation();
  EXPECT_EQ(0, wrongOrientations.size()) << [&wrongOrientations]() {
    std::vector<std::string> surfaceNames;
    surfaceNames.reserve(wrongOrientations.size());
    std::transform(wrongOrientations.cbegin(), wrongOrientations.cend(), std::back_inserter(surfaceNames),
                   [](const auto& sf) { return sf.nameString(); });
    return fmt::format("surfaceNames={}", surfaceNames);
  }();
  EXPECT_TRUE(s.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(s.isEnclosedVolume());

  constexpr double volume = (30.0 * 10.0 * 0.3) + (30.0 * 10.0 * 10.0) / 2.0;
  EXPECT_EQ(volume, s.volume());
}

TEST_F(ModelFixture, Issue_4837) {
  osversion::VersionTranslator translator;
  openstudio::path modelPath = resourcesPath() / toPath("model/4837_SpaceVolume.osm");
  model::OptionalModel model_ = translator.loadModel(modelPath);
  ASSERT_TRUE(model_);
  Model model = model_.get();

  {
    auto space = model.getConcreteModelObjectByName<Space>("Zone1 Office").get();
    EXPECT_TRUE(space.areAllSurfacesCorrectlyOriented());
    EXPECT_TRUE(space.isEnclosedVolume());
    EXPECT_NEAR(1010.76, space.volume(), 0.1);
  }

  {
    auto surface = model.getConcreteModelObjectByName<Surface>("Bulk Storage Front Wall Reversed").get();
    auto vertices = surface.vertices();
    auto outNormal = surface.outwardNormal();
    EXPECT_EQ(openstudio::Vector3d(0, -1, 0), outNormal);

    auto space = model.getConcreteModelObjectByName<Space>("Zone2 Fine Storage").get();
    auto wrongOrientations = space.findSurfacesWithIncorrectOrientation();
    EXPECT_EQ(1, wrongOrientations.size());
    EXPECT_EQ("Bulk Storage Front Wall Reversed", wrongOrientations.front().nameString());
    EXPECT_FALSE(space.areAllSurfacesCorrectlyOriented());
    EXPECT_TRUE(space.fixSurfacesWithIncorrectOrientation());
    EXPECT_TRUE(space.areAllSurfacesCorrectlyOriented());
    EXPECT_TRUE(space.isEnclosedVolume());
    EXPECT_NEAR(10880.57, space.volume(), 0.1);

    auto newVertices = surface.vertices();
    EXPECT_NE(vertices, newVertices);
    auto newOutNormal = surface.outwardNormal();
    EXPECT_NE(outNormal, newOutNormal);
    EXPECT_EQ(openstudio::Vector3d(0, 1, 0), newOutNormal);
  }

  {
    auto surface = model.getConcreteModelObjectByName<Surface>("Bulk Storage Front Wall").get();
    auto vertices = surface.vertices();
    auto outNormal = surface.outwardNormal();
    EXPECT_EQ(openstudio::Vector3d(0, 1, 0), outNormal);

    auto space = model.getConcreteModelObjectByName<Space>("Zone3 Bulk Storage").get();
    EXPECT_FALSE(space.areAllSurfacesCorrectlyOriented());
    auto wrongOrientations = space.findSurfacesWithIncorrectOrientation();
    EXPECT_EQ(1, wrongOrientations.size());
    EXPECT_EQ(surface.nameString(), wrongOrientations.front().nameString());
    EXPECT_TRUE(space.fixSurfacesWithIncorrectOrientation());
    EXPECT_TRUE(space.areAllSurfacesCorrectlyOriented());
    EXPECT_TRUE(space.isEnclosedVolume());
    EXPECT_NEAR(27350.0, space.volume(), 0.1);

    auto newVertices = surface.vertices();
    EXPECT_NE(vertices, newVertices);
    auto newOutNormal = surface.outwardNormal();
    EXPECT_NE(outNormal, newOutNormal);
    EXPECT_EQ(openstudio::Vector3d(0, -1, 0), newOutNormal);
  }
}

TEST_F(ModelFixture, Space_4837_SpaceVolume) {

  Model m;

  constexpr double width = 10.0;
  constexpr double height = 3.6;
  constexpr double spaceFloorArea = width * width;
  constexpr double spaceVolume = spaceFloorArea * height;

  //    y (=North)
  //   ▲
  //   │
  // 10o────────o
  //   │        │
  //   │        │
  //   │ Space 1│
  //   │        │
  //   o────────o───► x
  //  0        10

  // Counterclockwise points, Upper Left Corner convention
  std::vector<Point3d> floorPointsSpace1{
    {width, width, 0.0},
    {width, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, width, 0.0},
  };

  auto space1 = Space::fromFloorPrint(floorPointsSpace1, height, m).get();
  EXPECT_TRUE(space1.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(space1.isEnclosedVolume());
  EXPECT_DOUBLE_EQ(spaceVolume, space1.volume());

  // Grab a wall, flip it so it's outward normal points towards the space and not outside
  auto surfaces = space1.surfaces();
  auto it = std::find_if(surfaces.begin(), surfaces.end(), [](auto& sf) { return sf.surfaceType() == "Wall"; });
  ASSERT_TRUE(it != surfaces.end());
  auto sfName = it->nameString();
  auto vertices = it->vertices();
  std::reverse(vertices.begin(), vertices.end());
  it->setVertices(vertices);
  auto wrongOrientations = space1.findSurfacesWithIncorrectOrientation();
  EXPECT_EQ(1, wrongOrientations.size());
  EXPECT_EQ(sfName, wrongOrientations.front().nameString());
  EXPECT_FALSE(space1.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(space1.isEnclosedVolume());
  EXPECT_EQ(spaceVolume, space1.volume());  // It falls back to the floor * ceilingHeight and since this is a box, it works...
  EXPECT_TRUE(space1.fixSurfacesWithIncorrectOrientation());
  EXPECT_TRUE(space1.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(space1.isEnclosedVolume());
  EXPECT_DOUBLE_EQ(spaceVolume, space1.volume());

  // Flip everything
  for (auto& sf : surfaces) {
    auto vertices = sf.vertices();
    std::reverse(vertices.begin(), vertices.end());
    sf.setVertices(vertices);
  }

  wrongOrientations = space1.findSurfacesWithIncorrectOrientation();
  EXPECT_EQ(6, wrongOrientations.size());
  EXPECT_FALSE(space1.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(space1.isEnclosedVolume());
  EXPECT_EQ(spaceVolume, space1.volume());
  EXPECT_TRUE(space1.fixSurfacesWithIncorrectOrientation());
  EXPECT_TRUE(space1.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(space1.isEnclosedVolume());
  EXPECT_DOUBLE_EQ(spaceVolume, space1.volume());
}

TEST_F(ModelFixture, Space_4837_SpaceVolume_NonConvex) {

  Model m;

  //   y (=North)
  //   ▲
  //   │                  building height = 2m
  // 10├────────┼────────┼
  //   │                 │
  //   │        |        │
  //   │ z=0      z=1    │
  //   │        |        │
  //   └────────┴────────┴───► x
  //  0        10       20

  constexpr double width = 10.0;
  constexpr double heightPart1 = 2.0;
  constexpr double heightPart2 = 1.0;
  constexpr double spaceFloorAreaEach = width * width;
  constexpr double spaceVolumePart1 = (spaceFloorAreaEach * heightPart1);
  constexpr double spaceVolumePart2 = (spaceFloorAreaEach * heightPart2);
  constexpr double spaceVolume = spaceVolumePart1 + spaceVolumePart2;

  auto makeFloor = [](double min_x, double max_x, double min_y, double max_y, double z) {
    // Counterclockwise points, Upper Left Corner convention, except here we want to create a floor
    // so outward normal must be pointing DOWN, so clockwise order
    return std::vector<Point3d>{
      Point3d(max_x, max_y, z),
      Point3d(max_x, min_y, z),
      Point3d(min_x, min_y, z),
      Point3d(min_x, max_y, z),
    };
  };

  auto floorSurface1Points = makeFloor(0.0, width, 0.0, width, 0.0);
  auto space1 = Space::fromFloorPrint(floorSurface1Points, heightPart1, m).get();
  EXPECT_EQ(spaceFloorAreaEach, space1.floorArea());
  EXPECT_EQ(spaceVolumePart1, space1.volume());
  EXPECT_TRUE(space1.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(space1.isEnclosedVolume());

  auto floorSurface2Points = makeFloor(width, 2 * width, 0.0, width, 1.0);
  auto space2 = Space::fromFloorPrint(floorSurface2Points, heightPart2, m).get();
  EXPECT_EQ(spaceFloorAreaEach, space2.floorArea());
  EXPECT_EQ(spaceVolumePart2, space2.volume());
  EXPECT_TRUE(space2.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(space2.isEnclosedVolume());

  EXPECT_EQ(12, m.getConcreteModelObjects<Surface>().size());
  space1.intersectSurfaces(space2);
  EXPECT_EQ(13, m.getConcreteModelObjects<Surface>().size());
  space1.matchSurfaces(space2);
  EXPECT_EQ(13, m.getConcreteModelObjects<Surface>().size());

  ThermalZone z(m);
  space1.setThermalZone(z);
  space2.setThermalZone(z);
  auto mergedSpace = z.combineSpaces().get();
  EXPECT_EQ(11, m.getConcreteModelObjects<Surface>().size());

  EXPECT_TRUE(mergedSpace.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(mergedSpace.isEnclosedVolume());
  EXPECT_EQ(2 * spaceFloorAreaEach, mergedSpace.floorArea());
  EXPECT_EQ(spaceVolume, mergedSpace.volume());
}

TEST_F(ModelFixture, Space_4837_SpaceVolume_Hshaped) {

  Model m;

  //   ▲ y (=North)
  //   │    (5)   (8)         building height = 2m
  //15 o────o    o────o (1)
  //   │(4) │    │    │
  //   │    │<---│----│---- Space1 Wall 5
  //10 │    o────o    │<---- Space1 Wall 1
  //   │   (6)  (7)   │
  //   │              │
  //   │(3)           │ (2)
  //   o──────────────o───► x
  //  0     5    10   15

  constexpr double height = 2.0;

  constexpr double width = 15.0;
  constexpr double spaceFloorAreaOri = width * width;

  constexpr double widthRemoved = 5.0;
  constexpr double spaceFloorAreaRemoved = widthRemoved * widthRemoved;

  constexpr double spaceFloorArea = spaceFloorAreaOri - spaceFloorAreaRemoved;
  constexpr double spaceVolume = spaceFloorArea * height;

  EXPECT_EQ(200.0, spaceFloorArea);
  EXPECT_EQ(400.0, spaceVolume);

  std::vector<Point3d> floorPoints{
    {15.0, 15.0, 0.0},  //
    {15.0, 0.00, 0.0},  //
    {0.00, 0.00, 0.0},  //
    {0.00, 15.0, 0.0},  //
    {5.00, 15.0, 0.0},  //
    {5.00, 10.0, 0.0},  //
    {10.0, 10.0, 0.0},  //
    {10.0, 15.0, 0.0},  //
  };
  auto space = Space::fromFloorPrint(floorPoints, 2.0, m, "Space1").get();
  EXPECT_TRUE(space.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(space.isEnclosedVolume());
  EXPECT_DOUBLE_EQ(spaceVolume, space.volume());

  auto surfaces = space.surfaces();
  const std::string sfName = "Space1 Wall 5";

  auto it = std::find_if(surfaces.begin(), surfaces.end(), [&sfName](const Surface& s) { return s.nameString() == sfName; });
  ASSERT_TRUE(it != surfaces.end()) << [&surfaces]() {
    std::vector<std::string> surfaceNames;
    surfaceNames.reserve(surfaces.size());
    std::transform(surfaces.cbegin(), surfaces.cend(), std::back_inserter(surfaceNames), [](const auto& sf) { return sf.nameString(); });
    return fmt::format("surfaceNames={}", surfaceNames);
  }();
  auto vertices = it->vertices();
  std::reverse(vertices.begin(), vertices.end());
  it->setVertices(vertices);

  {
    auto wrongOrientations = space.findSurfacesWithIncorrectOrientation();
    EXPECT_EQ(1, wrongOrientations.size());
  }

  // Use the Polyhedron method of checking the surface edges (ensuring they are in REVERSE order)
  {
    std::vector<Surface3d> wrongOrientations = space.polyhedron().findSurfacesWithIncorrectOrientation();
    EXPECT_EQ(1, wrongOrientations.size());
    EXPECT_EQ(sfName, wrongOrientations.front().name);
  }

  EXPECT_FALSE(space.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(space.isEnclosedVolume());
  EXPECT_EQ(spaceVolume, space.volume());  // It falls back to the floor * ceilingHeight and since this is a box, it works...
  EXPECT_TRUE(space.fixSurfacesWithIncorrectOrientation());
  EXPECT_TRUE(space.areAllSurfacesCorrectlyOriented());
  EXPECT_TRUE(space.isEnclosedVolume());
  EXPECT_DOUBLE_EQ(spaceVolume, space.volume());
}
