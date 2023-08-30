/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Blind.hpp"
#include "../ShadingControl.hpp"
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../WindowPropertyFrameAndDivider.hpp"
#include "../WindowPropertyFrameAndDivider_Impl.hpp"
#include "../SimpleGlazing.hpp"
#include "../Construction.hpp"
#include "../DefaultSubSurfaceConstructions.hpp"
#include "../DefaultConstructionSet.hpp"
#include "../SurfacePropertyConvectionCoefficients.hpp"
#include "../SurfacePropertyOtherSideCoefficients.hpp"
#include "../SurfacePropertyOtherSideConditionsModel.hpp"
#include "../Blind.hpp"
#include "../ShadingControl.hpp"
#include "../Model_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../PlanarSurface.hpp"

#include "../../energyplus/ReverseTranslator.hpp"
#include "../../utilities/sql/SqlFile.hpp"

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"

#include <boost/algorithm/string/case_conv.hpp>

using namespace openstudio;
using namespace openstudio::model;

void removeSubSurfaces(Surface& surface) {
  for (SubSurface& s : surface.subSurfaces()) {
    s.remove();
  }
  EXPECT_EQ(0, surface.subSurfaces().size());
}

void removeSubSurfaces(Model& model) {
  for (auto& s : model.getConcreteModelObjects<SubSurface>()) {
    s.remove();
  }
  EXPECT_EQ(0, model.getConcreteModelObjects<SubSurface>().size());
}

/* HAS TO WAIT UNTIL WE GET A GOOD OSM EXAMPLE
TEST_F(ModelFixture, SubSurface_In_File)
{
  Workspace workspace(idfFile);

  // translate energyplus simulation to building model
  energyplus::ReverseTranslator reverseTranslator(workspace);
  OptionalModel optionalModel = reverseTranslator.convert();
  ASSERT_TRUE(optionalModel);
  Model model(*optionalModel);

  model.setSqlFile(sqlFile);
  ASSERT_TRUE(model.sqlFile());

  SubSurfaceVector subSurfaces = model.getConcreteModelObjects<SubSurface>();
  EXPECT_TRUE(subSurfaces.size() > 0);

  for (const SubSurface& subSurface : subSurfaces){
    OptionalString name = subSurface.name();
    ASSERT_TRUE(name);

    OptionalString type = subSurface.getString(FenestrationSurface_DetailedFields::SurfaceType);
    ASSERT_TRUE(type);

    // compute metrics from input object
    double area = subSurface.grossArea();
    EXPECT_TRUE(area > 0);

    // TODO: check tilt and azimuth too

    // query to get area from sql file
    std::string query = "SELECT Area FROM surfaces WHERE SurfaceName='" + to_upper_copy(*name) + "'";
    OptionalDouble sqlArea = model.sqlFile()->execAndReturnFirstDouble(query);
    ASSERT_TRUE(sqlArea);
    EXPECT_TRUE(*sqlArea > 0);
    EXPECT_NEAR(*sqlArea, area, 0.000001);

    // query to get gross area from sql file
    query = "SELECT GrossArea FROM surfaces WHERE SurfaceName='" + to_upper_copy(*name) + "'";
    OptionalDouble sqlGrossArea = model.sqlFile()->execAndReturnFirstDouble(query);
    ASSERT_TRUE(sqlGrossArea);
    EXPECT_TRUE(*sqlGrossArea > 0);
    EXPECT_NEAR(*sqlGrossArea, area, 0.000001);

    // check visible transmittance
    if (istringEqual("Door", *type)){
      OptionalDouble tvis = subSurface.visibleTransmittance();
      ASSERT_TRUE(tvis);
      EXPECT_EQ(0.0, *tvis);
    }else{
      OptionalDouble tvis = subSurface.visibleTransmittance();
      ASSERT_TRUE(tvis);
      EXPECT_TRUE(*tvis > 0.0);
    }
  }
}
*/

TEST_F(ModelFixture, 0_Vertex_SubSurface) {
  Model model;

  std::vector<Point3d> vertices;

  EXPECT_THROW(SubSurface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 1_Vertex_SubSurface) {
  Model model;

  std::vector<Point3d> vertices{
    {0, 0, 1},
  };

  EXPECT_THROW(SubSurface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 2_Vertex_SubSurface) {
  Model model;

  std::vector<Point3d> vertices{
    {0, 0, 1},
    {0, 0, 0},
  };

  EXPECT_THROW(SubSurface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 3_Vertex_SubSurface) {
  Model model;

  std::vector<Point3d> vertices{
    {0, 0, 1},
    {0, 0, 0},
    {1, 0, 0},
  };

  EXPECT_NO_THROW(SubSurface(vertices, model));
}

TEST_F(ModelFixture, 4_Vertex_SubSurface) {
  Model model;

  std::vector<Point3d> vertices{
    {0, 0, 1},
    {0, 0, 0},
    {1, 0, 0},
    {1, 0, 1},
  };

  EXPECT_NO_THROW(SubSurface(vertices, model));
}

TEST_F(ModelFixture, 5_Vertex_SubSurface) {
  Model model;

  std::vector<Point3d> vertices{
    {0, 0, 1}, {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0.5, 0, 1.5},
  };

  EXPECT_NO_THROW(SubSurface(vertices, model));
}

TEST_F(ModelFixture, AdjacentSubSurface) {
  Model model;

  std::vector<Point3d> vertices{
    {0, 0, 3},
    {0, 0, 0},
    {3, 0, 0},
    {3, 0, 3},
  };

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.adjacentSurface());

  std::reverse(vertices.begin(), vertices.end());

  Space space2(model);
  Surface wall2(vertices, model);
  wall2.setSpace(space2);
  EXPECT_FALSE(wall2.adjacentSurface());

  vertices = {
    {1, 0, 2},
    {1, 0, 1},
    {2, 0, 1},
    {2, 0, 2},
  };

  SubSurface window1(vertices, model);
  EXPECT_FALSE(window1.adjacentSubSurface());

  std::reverse(vertices.begin(), vertices.end());

  SubSurface window2(vertices, model);
  EXPECT_FALSE(window2.adjacentSubSurface());

  EXPECT_FALSE(window1.setAdjacentSubSurface(window2));

  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));
  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));
  EXPECT_TRUE(wall2.setAdjacentSurface(wall1));
  EXPECT_TRUE(wall2.setAdjacentSurface(wall1));

  EXPECT_FALSE(window1.adjacentSubSurface());

  EXPECT_FALSE(window1.setAdjacentSubSurface(window2));

  window1.setSurface(wall1);
  window2.setSurface(wall2);

  EXPECT_TRUE(window1.setAdjacentSubSurface(window2));
  EXPECT_TRUE(window1.setAdjacentSubSurface(window2));
  EXPECT_TRUE(window2.setAdjacentSubSurface(window1));
  EXPECT_TRUE(window2.setAdjacentSubSurface(window1));

  ASSERT_TRUE(window1.adjacentSubSurface());
  EXPECT_EQ(window2.handle(), window1.adjacentSubSurface()->handle());
  ASSERT_TRUE(window2.adjacentSubSurface());
  EXPECT_EQ(window1.handle(), window2.adjacentSubSurface()->handle());

  wall1.resetAdjacentSurface();
  EXPECT_FALSE(wall1.adjacentSurface());
  EXPECT_FALSE(wall2.adjacentSurface());
  EXPECT_FALSE(window1.adjacentSubSurface());
  EXPECT_FALSE(window2.adjacentSubSurface());
}

TEST_F(ModelFixture, AdjacentSubSurface2) {
  Model model;

  std::vector<Point3d> vertices{
    {0, 0, 3},
    {0, 0, 0},
    {3, 0, 0},
    {3, 0, 3},
  };

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.adjacentSurface());

  std::reverse(vertices.begin(), vertices.end());

  Space space2(model);
  Surface wall2(vertices, model);
  wall2.setSpace(space2);
  EXPECT_FALSE(wall2.adjacentSurface());

  vertices = {
    {1, 0, 2},
    {1, 0, 1},
    {2, 0, 1},
    {2, 0, 2},
  };

  SubSurface window1(vertices, model);
  window1.setSurface(wall1);
  EXPECT_EQ("FixedWindow", window1.subSurfaceType());
  EXPECT_FALSE(window1.adjacentSubSurface());

  std::reverse(vertices.begin(), vertices.end());

  SubSurface window2(vertices, model);
  window2.setSurface(wall2);
  EXPECT_EQ("FixedWindow", window2.subSurfaceType());
  EXPECT_TRUE(window2.setSubSurfaceType("OperableWindow"));
  EXPECT_EQ("OperableWindow", window2.subSurfaceType());
  EXPECT_FALSE(window2.adjacentSubSurface());

  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));

  EXPECT_TRUE(window1.setAdjacentSubSurface(window2));
  EXPECT_EQ("OperableWindow", window1.subSurfaceType());
  EXPECT_EQ("OperableWindow", window2.subSurfaceType());

  EXPECT_TRUE(window1.setSubSurfaceType("FixedWindow"));
  EXPECT_EQ("FixedWindow", window1.subSurfaceType());
  EXPECT_EQ("FixedWindow", window2.subSurfaceType());

  EXPECT_TRUE(window2.setSubSurfaceType("OperableWindow"));
  EXPECT_EQ("OperableWindow", window1.subSurfaceType());
  EXPECT_EQ("OperableWindow", window2.subSurfaceType());

  window1.resetAdjacentSubSurface();

  EXPECT_TRUE(window1.setSubSurfaceType("FixedWindow"));
  EXPECT_EQ("FixedWindow", window1.subSurfaceType());
  EXPECT_TRUE(window2.setSubSurfaceType("OperableWindow"));
  EXPECT_EQ("OperableWindow", window2.subSurfaceType());

  EXPECT_TRUE(window2.setAdjacentSubSurface(window1));
  EXPECT_EQ("OperableWindow", window1.subSurfaceType());
  EXPECT_EQ("OperableWindow", window2.subSurfaceType());

  window1.resetAdjacentSubSurface();

  EXPECT_TRUE(window1.setSubSurfaceType("OverheadDoor"));
  EXPECT_EQ("OverheadDoor", window1.subSurfaceType());
  EXPECT_TRUE(window2.setSubSurfaceType("OperableWindow"));
  EXPECT_EQ("OperableWindow", window2.subSurfaceType());

  EXPECT_TRUE(window2.setAdjacentSubSurface(window1));
  EXPECT_EQ("OperableWindow", window1.subSurfaceType());
  EXPECT_EQ("OperableWindow", window2.subSurfaceType());

  window1.resetAdjacentSubSurface();

  EXPECT_TRUE(window1.setSubSurfaceType("OverheadDoor"));
  EXPECT_EQ("OverheadDoor", window1.subSurfaceType());
  EXPECT_TRUE(window2.setSubSurfaceType("OperableWindow"));
  EXPECT_EQ("OperableWindow", window2.subSurfaceType());

  EXPECT_TRUE(window1.setAdjacentSubSurface(window2));
  EXPECT_EQ("OverheadDoor", window1.subSurfaceType());
  EXPECT_EQ("OverheadDoor", window2.subSurfaceType());
}

TEST_F(ModelFixture, AdjacentSubSurface3) {
  Model model;

  std::vector<Point3d> vertices{
    {0, 0, 3},
    {0, 0, 0},
    {3, 0, 0},
    {3, 0, 3},
  };

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.adjacentSurface());

  std::reverse(vertices.begin(), vertices.end());

  Space space2(model);
  Surface wall2(vertices, model);
  wall2.setSpace(space2);
  EXPECT_FALSE(wall2.adjacentSurface());

  vertices = {
    {1, 0, 2},
    {1, 0, 1},
    {2, 0, 1},
    {2, 0, 2},
  };

  SubSurface window1(vertices, model);
  window1.setSurface(wall1);
  EXPECT_EQ("FixedWindow", window1.subSurfaceType());
  EXPECT_FALSE(window1.adjacentSubSurface());

  std::reverse(vertices.begin(), vertices.end());

  SubSurface window2(vertices, model);
  window2.setSurface(wall2);
  EXPECT_EQ("FixedWindow", window2.subSurfaceType());
  EXPECT_TRUE(window2.setSubSurfaceType("OperableWindow"));
  EXPECT_EQ("OperableWindow", window2.subSurfaceType());
  EXPECT_FALSE(window2.adjacentSubSurface());

  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));

  EXPECT_TRUE(window1.setMultiplier(4));

  EXPECT_FALSE(window1.setAdjacentSubSurface(window2));
  EXPECT_EQ(4, window1.multiplier());
  EXPECT_EQ(1, window2.multiplier());

  window1.resetMultiplier();

  EXPECT_TRUE(window1.setAdjacentSubSurface(window2));
  EXPECT_EQ(1, window1.multiplier());
  EXPECT_EQ(1, window2.multiplier());

  EXPECT_TRUE(window1.setMultiplier(3));
  EXPECT_EQ(3, window1.multiplier());
  EXPECT_EQ(3, window2.multiplier());

  window1.resetMultiplier();
  EXPECT_EQ(1, window1.multiplier());
  EXPECT_EQ(1, window2.multiplier());
}

TEST_F(ModelFixture, AdjacentSubSurface_SurfacePropertyOtherSideCoefficients) {
  Model model;

  std::vector<Point3d> vertices{
    {0, 0, 3},
    {0, 0, 0},
    {3, 0, 0},
    {3, 0, 3},
  };

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.adjacentSurface());

  std::reverse(vertices.begin(), vertices.end());

  Space space2(model);
  Surface wall2(vertices, model);
  wall2.setSpace(space2);
  EXPECT_FALSE(wall2.adjacentSurface());

  vertices = {
    {1, 0, 2},
    {1, 0, 1},
    {2, 0, 1},
    {2, 0, 2},
  };

  SubSurface window1(vertices, model);
  EXPECT_FALSE(window1.adjacentSubSurface());

  std::reverse(vertices.begin(), vertices.end());

  SubSurface window2(vertices, model);
  EXPECT_FALSE(window2.adjacentSubSurface());

  EXPECT_FALSE(window1.setAdjacentSubSurface(window2));

  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));

  EXPECT_FALSE(window1.adjacentSubSurface());

  EXPECT_FALSE(window1.setAdjacentSubSurface(window2));

  window1.setSurface(wall1);
  window2.setSurface(wall2);

  EXPECT_TRUE(window1.setAdjacentSubSurface(window2));

  ASSERT_TRUE(window1.adjacentSubSurface());
  EXPECT_EQ(window2.handle(), window1.adjacentSubSurface()->handle());
  ASSERT_TRUE(window2.adjacentSubSurface());
  EXPECT_EQ(window1.handle(), window2.adjacentSubSurface()->handle());

  SurfacePropertyOtherSideCoefficients osc(model);
  EXPECT_TRUE(window1.setSurfacePropertyOtherSideCoefficients(osc));
  ASSERT_TRUE(window1.surfacePropertyOtherSideCoefficients());
  EXPECT_EQ(osc.handle(), window1.surfacePropertyOtherSideCoefficients()->handle());
  EXPECT_FALSE(window2.surfacePropertyOtherSideCoefficients());
  EXPECT_FALSE(window1.adjacentSubSurface());
  EXPECT_FALSE(window2.adjacentSubSurface());

  EXPECT_TRUE(window1.setAdjacentSubSurface(window2));

  ASSERT_TRUE(window1.adjacentSubSurface());
  EXPECT_EQ(window2.handle(), window1.adjacentSubSurface()->handle());
  ASSERT_TRUE(window2.adjacentSubSurface());
  EXPECT_EQ(window1.handle(), window2.adjacentSubSurface()->handle());
}

TEST_F(ModelFixture, AdjacentSubSurface_SurfacePropertyOtherSideConditionsModel) {
  Model model;

  std::vector<Point3d> vertices{
    {0, 0, 3},
    {0, 0, 0},
    {3, 0, 0},
    {3, 0, 3},
  };

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.adjacentSurface());

  std::reverse(vertices.begin(), vertices.end());

  Space space2(model);
  Surface wall2(vertices, model);
  wall2.setSpace(space2);
  EXPECT_FALSE(wall2.adjacentSurface());

  vertices = {
    {1, 0, 2},
    {1, 0, 1},
    {2, 0, 1},
    {2, 0, 2},
  };

  SubSurface window1(vertices, model);
  EXPECT_FALSE(window1.adjacentSubSurface());

  std::reverse(vertices.begin(), vertices.end());

  SubSurface window2(vertices, model);
  EXPECT_FALSE(window2.adjacentSubSurface());

  EXPECT_FALSE(window1.setAdjacentSubSurface(window2));

  EXPECT_TRUE(wall1.setAdjacentSurface(wall2));

  EXPECT_FALSE(window1.adjacentSubSurface());

  EXPECT_FALSE(window1.setAdjacentSubSurface(window2));

  window1.setSurface(wall1);
  window2.setSurface(wall2);

  EXPECT_TRUE(window1.setAdjacentSubSurface(window2));

  ASSERT_TRUE(window1.adjacentSubSurface());
  EXPECT_EQ(window2.handle(), window1.adjacentSubSurface()->handle());
  ASSERT_TRUE(window2.adjacentSubSurface());
  EXPECT_EQ(window1.handle(), window2.adjacentSubSurface()->handle());

  SurfacePropertyOtherSideConditionsModel oscm(model);
  EXPECT_TRUE(window1.setSurfacePropertyOtherSideConditionsModel(oscm));
  ASSERT_TRUE(window1.surfacePropertyOtherSideConditionsModel());
  EXPECT_EQ(oscm.handle(), window1.surfacePropertyOtherSideConditionsModel()->handle());
  EXPECT_FALSE(window2.surfacePropertyOtherSideConditionsModel());
  EXPECT_FALSE(window1.adjacentSubSurface());
  EXPECT_FALSE(window2.adjacentSubSurface());

  EXPECT_TRUE(window1.setAdjacentSubSurface(window2));

  ASSERT_TRUE(window1.adjacentSubSurface());
  EXPECT_EQ(window2.handle(), window1.adjacentSubSurface()->handle());
  ASSERT_TRUE(window2.adjacentSubSurface());
  EXPECT_EQ(window1.handle(), window2.adjacentSubSurface()->handle());
}

TEST_F(ModelFixture, ExampleDaylightingControlPlacement) {
  Model model;

  Space space(model);

  std::vector<Point3d> vertices{
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
    {0, 0, 0},
  };
  Surface floor(vertices, model);
  floor.setSpace(space);
  EXPECT_EQ("Floor", floor.surfaceType());

  vertices = {
    {0, 0, 10},
    {0, 0, 0},
    {10, 0, 0},
    {10, 0, 10},
  };
  Surface wall(vertices, model);
  wall.setSpace(space);
  EXPECT_EQ("Wall", wall.surfaceType());

  vertices = {
    {2, 0, 8},
    {2, 0, 2},
    {8, 0, 2},
    {8, 0, 8},
  };
  SubSurface window(vertices, model);
  window.setSurface(wall);

  boost::optional<Point3d> windowCentroid = getCentroid(window.vertices());
  ASSERT_TRUE(windowCentroid);
  EXPECT_DOUBLE_EQ(5, windowCentroid->x());
  EXPECT_DOUBLE_EQ(0, windowCentroid->y());
  EXPECT_DOUBLE_EQ(5, windowCentroid->z());

  // move back 1 m
  Point3d windowCentroidPrime = windowCentroid.get() + (-1.0) * window.outwardNormal();
  EXPECT_DOUBLE_EQ(5, windowCentroidPrime.x());
  EXPECT_DOUBLE_EQ(1, windowCentroidPrime.y());
  EXPECT_DOUBLE_EQ(5, windowCentroidPrime.z());

  // project to plane of floor
  Plane floorPlane = floor.plane();
  Point3d pointOnFloor = floorPlane.project(windowCentroidPrime);
  EXPECT_DOUBLE_EQ(5, pointOnFloor.x());
  EXPECT_DOUBLE_EQ(1, pointOnFloor.y());
  EXPECT_DOUBLE_EQ(0, pointOnFloor.z());

  // move up 1 m
  Point3d point = pointOnFloor + Vector3d(0, 0, 1);
  EXPECT_DOUBLE_EQ(5, point.x());
  EXPECT_DOUBLE_EQ(1, point.y());
  EXPECT_DOUBLE_EQ(1, point.z());
}

TEST_F(ModelFixture, SkylightPattern_SingleSurface) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  std::vector<Point3dVector> pattern;
  std::vector<SubSurface> skylights;

  // no geometry yet
  pattern = building.generateSkylightPattern(0.00, 1.5, 1);
  EXPECT_EQ(0, pattern.size());

  Space space(model);

  std::vector<Point3d> vertices{
    {0, 0, 0},
    {30, 0, 0},
    {30, 20, 0},
    {0, 20, 0},
  };

  Surface roof(vertices, model);
  roof.setSpace(space);
  EXPECT_DOUBLE_EQ(600.0, roof.grossArea());
  EXPECT_EQ("RoofCeiling", roof.surfaceType());
  EXPECT_EQ(0, roof.subSurfaces().size());
  EXPECT_DOUBLE_EQ(0.0, roof.skylightToRoofRatio());
  EXPECT_DOUBLE_EQ(0.0, roof.skylightToProjectedFloorRatio());

  // bad inputs
  pattern = building.generateSkylightPattern(0.00, 1.5, 1);
  EXPECT_EQ(0, pattern.size());
  pattern = building.generateSkylightPattern(0.05, 0, 1);
  EXPECT_EQ(0, pattern.size());
  pattern = building.generateSkylightPattern(0.05, 1.5, 0);
  EXPECT_EQ(0, pattern.size());

  // 5% square
  removeSubSurfaces(roof);
  EXPECT_NEAR(0.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.0, roof.skylightToProjectedFloorRatio(), 0.001);
  pattern = building.generateSkylightPattern(0.05, 1, 1);
  skylights = roof.createSubSurfaces(pattern, 0.01, boost::none);
  EXPECT_EQ(pattern.size(), skylights.size());
  EXPECT_NEAR(0.05, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.05, roof.skylightToProjectedFloorRatio(), 0.001);

  // 3% square
  removeSubSurfaces(roof);
  EXPECT_NEAR(0.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.0, roof.skylightToProjectedFloorRatio(), 0.001);
  pattern = building.generateSkylightPattern(0.03, 1, 1);
  skylights = roof.createSubSurfaces(pattern, 0.01, boost::none);
  EXPECT_EQ(pattern.size(), skylights.size());
  EXPECT_NEAR(0.03, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.03, roof.skylightToProjectedFloorRatio(), 0.001);

  // 5% rectangle
  removeSubSurfaces(roof);
  EXPECT_NEAR(0.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.0, roof.skylightToProjectedFloorRatio(), 0.001);
  pattern = building.generateSkylightPattern(0.05, 1.5, 1);
  skylights = roof.createSubSurfaces(pattern, 0.01, boost::none);
  EXPECT_EQ(pattern.size(), skylights.size());
  EXPECT_NEAR(0.05, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.05, roof.skylightToProjectedFloorRatio(), 0.001);

  // 3% rectangle
  removeSubSurfaces(roof);
  EXPECT_NEAR(0.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.0, roof.skylightToProjectedFloorRatio(), 0.001);
  pattern = building.generateSkylightPattern(0.03, 1.5, 1);
  skylights = roof.createSubSurfaces(pattern, 0.01, boost::none);
  EXPECT_EQ(pattern.size(), skylights.size());
  EXPECT_NEAR(0.03, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.03, roof.skylightToProjectedFloorRatio(), 0.001);

  // existing subsurfaces
  removeSubSurfaces(roof);
  vertices = {
    {20, 10, 0},
    {22, 10, 0},
    {22, 12, 0},
    {20, 12, 0},
  };
  SubSurface skylight(vertices, model);
  skylight.setSurface(roof);
  EXPECT_DOUBLE_EQ(4.0, skylight.grossArea());
  EXPECT_EQ("Skylight", skylight.subSurfaceType());
  EXPECT_NEAR(4.0 / 600.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(4.0 / 600.0, roof.skylightToProjectedFloorRatio(), 0.001);

  pattern = building.generateSkylightPattern(0.03, 1.5, 1);
  EXPECT_FALSE(pattern.empty());
  skylights = roof.createSubSurfaces(pattern, 0.01, boost::none);
  EXPECT_TRUE(skylights.empty());
  EXPECT_NEAR(4.0 / 600.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(4.0 / 600.0, roof.skylightToProjectedFloorRatio(), 0.001);
}

TEST_F(ModelFixture, SkylightPattern_SingleSurface2) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  std::vector<Point3dVector> pattern;
  std::vector<SubSurface> skylights;

  // no geometry yet
  pattern = building.generateSkylightPattern(0.00, 1.5, 1);
  EXPECT_EQ(0, pattern.size());

  Space space(model);

  std::vector<Point3d> vertices{
    {10, -10, 0},
    {40, -10, 0},
    {40, 10, 0},
    {10, 10, 0},
  };

  Surface roof(vertices, model);
  roof.setSpace(space);
  EXPECT_DOUBLE_EQ(600.0, roof.grossArea());
  EXPECT_EQ("RoofCeiling", roof.surfaceType());
  EXPECT_EQ(0, roof.subSurfaces().size());
  EXPECT_DOUBLE_EQ(0.0, roof.skylightToRoofRatio());
  EXPECT_DOUBLE_EQ(0.0, roof.skylightToProjectedFloorRatio());

  // 5% square
  removeSubSurfaces(roof);
  EXPECT_NEAR(0.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.0, roof.skylightToProjectedFloorRatio(), 0.001);
  pattern = building.generateSkylightPattern(0.05, 1, 1);
  skylights = roof.createSubSurfaces(pattern, 0.01, boost::none);
  EXPECT_NEAR(0.05, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.05, roof.skylightToProjectedFloorRatio(), 0.001);

  space.setXOrigin(4);
  space.setYOrigin(6);
  space.setZOrigin(20);

  // 3% square
  removeSubSurfaces(roof);
  EXPECT_NEAR(0.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.0, roof.skylightToProjectedFloorRatio(), 0.001);
  pattern = building.generateSkylightPattern(0.03, 1, 1);
  skylights = roof.createSubSurfaces(pattern, 0.01, boost::none);
  EXPECT_NEAR(0.03, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.03, roof.skylightToProjectedFloorRatio(), 0.001);

  space.setDirectionofRelativeNorth(25);

  // 5% rectangle
  removeSubSurfaces(roof);
  EXPECT_NEAR(0.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.0, roof.skylightToProjectedFloorRatio(), 0.001);
  pattern = building.generateSkylightPattern(0.05, 1.5, 1);
  skylights = roof.createSubSurfaces(pattern, 0.01, boost::none);
  EXPECT_NEAR(0.05, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.05, roof.skylightToProjectedFloorRatio(), 0.001);

  space.setDirectionofRelativeNorth(-25);

  // 3% rectangle
  removeSubSurfaces(roof);
  EXPECT_NEAR(0.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.0, roof.skylightToProjectedFloorRatio(), 0.001);
  pattern = building.generateSkylightPattern(0.03, 1.5, 1);
  skylights = roof.createSubSurfaces(pattern, 0.01, boost::none);
  EXPECT_NEAR(0.03, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(0.03, roof.skylightToProjectedFloorRatio(), 0.001);
}

void checkExpectedSkylightRatios(const Model& model, double /*expectedRoofArea*/, double expectedSkylightRatio, double tol) {
  double totalGrossRoofArea = 0.0;
  double totalSkylightArea = 0.0;
  for (const auto& surface : model.getConcreteModelObjects<Surface>()) {
    boost::optional<Space> space = surface.space();
    ASSERT_TRUE(space);
    double multiplier = space->multiplier();
    if (istringEqual("RoofCeiling", surface.surfaceType()) && istringEqual("Outdoors", surface.outsideBoundaryCondition())) {

      double grossRoofArea = surface.grossArea();
      double netRoofArea = surface.netArea();
      double skylightToRoofRatio = surface.skylightToRoofRatio();
      double skylightArea = skylightToRoofRatio * grossRoofArea;

      EXPECT_NEAR(netRoofArea, grossRoofArea - skylightArea, 0.001);

      totalGrossRoofArea += multiplier * grossRoofArea;
      totalSkylightArea += multiplier * skylightArea;
    }
  }

  if (totalGrossRoofArea > 0) {
    double ratio = totalSkylightArea / totalGrossRoofArea;
    EXPECT_NEAR(expectedSkylightRatio, ratio, tol);
  }
}

TEST_F(ModelFixture, SkylightPattern_PerimCore) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  std::vector<Point3dVector> pattern;
  std::vector<SubSurface> skylights;

  // no geometry yet
  pattern = building.generateSkylightPattern(0.00, 1.5, 1);
  EXPECT_EQ(0, pattern.size());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  std::vector<Space> spaces;
  Space space(model);
  spaces.push_back(space);

  std::vector<Point3d> vertices;

  vertices = {
    {0, 0, 0},
    {5, 5, 0},
    {5, 15, 0},
    {0, 20, 0},
  };
  Surface roofWest(vertices, model);
  roofWest.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofWest.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices = {
    {0, 0, 0},
    {30, 0, 0},
    {15, 5, 0},
    {5, 5, 0},
  };
  Surface roofSouth(vertices, model);
  roofSouth.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofSouth.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices = {
    {30, 0, 0},
    {30, 20, 0},
    {5, 15, 0},
    {5, 5, 0},
  };
  Surface roofEast(vertices, model);
  roofEast.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofEast.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices = {
    {5, 15, 0},
    {15, 15, 0},
    {30, 20, 0},
    {0, 20, 0},
  };
  Surface roofNorth(vertices, model);
  roofNorth.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofNorth.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices = {
    {5, 5, 0},
    {15, 5, 0},
    {15, 15, 0},
    {5, 15, 0},
  };
  Surface roofCore(vertices, model);
  roofCore.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofCore.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  // 5% square
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = building.generateSkylightPattern(0.05, 1, 1);
  skylights = applySkylightPattern(pattern, spaces, boost::none);
  EXPECT_LE(pattern.size(), skylights.size());
  checkExpectedSkylightRatios(model, 600.0, 0.05, 0.005);

  // 3% square
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = building.generateSkylightPattern(0.03, 1, 1);
  skylights = applySkylightPattern(pattern, spaces, boost::none);
  EXPECT_LE(pattern.size(), skylights.size());
  checkExpectedSkylightRatios(model, 600.0, 0.03, 0.005);

  // 5% rectangle
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = building.generateSkylightPattern(0.05, 1.5, 1);
  skylights = applySkylightPattern(pattern, spaces, boost::none);
  EXPECT_LE(pattern.size(), skylights.size());
  checkExpectedSkylightRatios(model, 600.0, 0.05, 0.005);

  // 3% rectangle
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = building.generateSkylightPattern(0.03, 1.5, 1);
  skylights = applySkylightPattern(pattern, spaces, boost::none);
  EXPECT_LE(pattern.size(), skylights.size());
  checkExpectedSkylightRatios(model, 600.0, 0.03, 0.005);
}

TEST_F(ModelFixture, SkylightPattern_PerimCore_Rotated) {
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  std::vector<Point3dVector> pattern;
  std::vector<SubSurface> skylights;

  // no geometry yet
  pattern = building.generateSkylightPattern(0.00, 1.5, 1);
  EXPECT_EQ(0, pattern.size());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  std::vector<Space> spaces;
  Space space(model);
  spaces.push_back(space);

  std::vector<Point3d> vertices;

  vertices = {
    {0, 0, 0},
    {5, 5, 0},
    {5, 15, 0},
    {0, 20, 0},
  };
  Surface roofWest(vertices, model);
  roofWest.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofWest.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices = {
    {0, 0, 0},
    {30, 0, 0},
    {15, 5, 0},
    {5, 5, 0},
  };
  Surface roofSouth(vertices, model);
  roofSouth.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofSouth.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices = {
    {30, 0, 0},
    {30, 20, 0},
    {5, 15, 0},
    {5, 5, 0},
  };
  Surface roofEast(vertices, model);
  roofEast.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofEast.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices = {
    {5, 15, 0},
    {15, 15, 0},
    {30, 20, 0},
    {0, 20, 0},
  };
  Surface roofNorth(vertices, model);
  roofNorth.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofNorth.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices = {
    {5, 5, 0},
    {15, 5, 0},
    {15, 15, 0},
    {5, 15, 0},
  };
  Surface roofCore(vertices, model);
  roofCore.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofCore.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  // 5% square
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = generateSkylightPattern(spaces, 90, 0.05, 1, 1);
  skylights = applySkylightPattern(pattern, model.getConcreteModelObjects<Space>(), boost::none);
  checkExpectedSkylightRatios(model, 600.0, 0.05, 0.005);

  // 3% square
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = generateSkylightPattern(spaces, 45, 0.03, 1, 1);
  skylights = applySkylightPattern(pattern, model.getConcreteModelObjects<Space>(), boost::none);
  checkExpectedSkylightRatios(model, 600.0, 0.03, 0.006);  // BUMP this up a bit

  // 5% rectangle
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = generateSkylightPattern(spaces, 135, 0.05, 1, 1);
  skylights = applySkylightPattern(pattern, model.getConcreteModelObjects<Space>(), boost::none);
  checkExpectedSkylightRatios(model, 600.0, 0.05, 0.005);

  // 3% rectangle
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = generateSkylightPattern(spaces, 180, 0.03, 1, 1);
  skylights = applySkylightPattern(pattern, model.getConcreteModelObjects<Space>(), boost::none);
  checkExpectedSkylightRatios(model, 600.0, 0.03, 0.005);
}

TEST_F(ModelFixture, DefaultSubSurfaceType) {
  Model model;
  std::vector<Point3d> vertices;

  // with no base surface the default type is set by tilt only
  {
    // normal 0,0,1
    vertices = {
      {0, 1, 0},
      {0, 0, 0},
      {1, 0, 0},
      {1, 1, 0},
    };

    SubSurface s(vertices, model);
    s.assignDefaultSubSurfaceType();
    EXPECT_EQ("Skylight", s.subSurfaceType());

    WindowPropertyFrameAndDivider frame(model);
    EXPECT_TRUE(s.allowWindowPropertyFrameAndDivider());
    ASSERT_TRUE(s.setWindowPropertyFrameAndDivider(frame));
    ASSERT_TRUE(s.windowPropertyFrameAndDivider());
    WindowPropertyFrameAndDivider frame2 = s.windowPropertyFrameAndDivider().get();
    EXPECT_EQ(frame, frame2);
  }
  {
    // normal 0,1,0
    vertices = {
      {0, 0, 1},
      {0, 0, 0},
      {1, 0, 0},
      {1, 0, 1},
    };

    SubSurface s(vertices, model);
    s.assignDefaultSubSurfaceType();
    EXPECT_EQ("FixedWindow", s.subSurfaceType());

    WindowPropertyFrameAndDivider frame(model);
    EXPECT_TRUE(s.allowWindowPropertyFrameAndDivider());
    ASSERT_TRUE(s.setWindowPropertyFrameAndDivider(frame));
    ASSERT_TRUE(s.windowPropertyFrameAndDivider());
    WindowPropertyFrameAndDivider frame2 = s.windowPropertyFrameAndDivider().get();
    EXPECT_EQ(frame, frame2);
  }

  // with base surface the default type is set based on base surface
  vertices = {
    {0, 0, 10},
    {0, 0, 0},
    {10, 0, 0},
    {10, 0, 10},
  };
  Surface wall(vertices, model);

  vertices = {
    {0, 10, 0},
    {0, 0, 0},
    {10, 0, 0},
    {10, 10, 0},
  };
  Surface roof(vertices, model);

  {
    // normal 0,0,1
    vertices = {
      {4, 5, 0},
      {4, 4, 0},
      {5, 4, 0},
      {5, 5, 0},
    };

    SubSurface s(vertices, model);
    s.setSurface(roof);
    EXPECT_EQ("Skylight", s.subSurfaceType());
    s.assignDefaultSubSurfaceType();
    EXPECT_EQ("Skylight", s.subSurfaceType());
  }
  {
    // normal 0,1,0 on bottom edge
    vertices = {
      {0, 0, 1},
      {0, 0, 0},
      {1, 0, 0},
      {1, 0, 1},
    };

    SubSurface s(vertices, model);
    s.setSurface(wall);
    EXPECT_EQ("Door", s.subSurfaceType());
    s.assignDefaultSubSurfaceType();
    EXPECT_EQ("Door", s.subSurfaceType());

    WindowPropertyFrameAndDivider frame(model);
    EXPECT_FALSE(s.allowWindowPropertyFrameAndDivider());
    EXPECT_FALSE(s.setWindowPropertyFrameAndDivider(frame));
    ASSERT_FALSE(s.windowPropertyFrameAndDivider());

    EXPECT_TRUE(s.setSubSurfaceType("GlassDoor"));
    s.assignDefaultSubSurfaceType();
    EXPECT_EQ("GlassDoor", s.subSurfaceType());

    EXPECT_TRUE(s.allowWindowPropertyFrameAndDivider());
    ASSERT_TRUE(s.setWindowPropertyFrameAndDivider(frame));
    ASSERT_TRUE(s.windowPropertyFrameAndDivider());
    WindowPropertyFrameAndDivider frame2 = s.windowPropertyFrameAndDivider().get();
    EXPECT_EQ(frame, frame2);
  }
  {
    // normal 0,1,0 not on bottom edge
    vertices = {
      {4, 0, 5},
      {4, 0, 4},
      {5, 0, 4},
      {5, 0, 5},
    };

    SubSurface s(vertices, model);
    s.setSurface(wall);
    EXPECT_EQ("FixedWindow", s.subSurfaceType());
    s.assignDefaultSubSurfaceType();
    EXPECT_EQ("FixedWindow", s.subSurfaceType());

    WindowPropertyFrameAndDivider frame(model);
    EXPECT_TRUE(s.allowWindowPropertyFrameAndDivider());
    ASSERT_TRUE(s.setWindowPropertyFrameAndDivider(frame));
    ASSERT_TRUE(s.windowPropertyFrameAndDivider());
    WindowPropertyFrameAndDivider frame2 = s.windowPropertyFrameAndDivider().get();
    EXPECT_EQ(frame, frame2);
  }

  // set default window construction, reproduces #1924
  SimpleGlazing glazing(model);
  Construction construction(model);
  construction.insertLayer(0, glazing);
  DefaultSubSurfaceConstructions defaultSubSurfaceConstructions(model);
  defaultSubSurfaceConstructions.setFixedWindowConstruction(construction);
  defaultSubSurfaceConstructions.setGlassDoorConstruction(construction);
  DefaultConstructionSet defaultConstructions(model);
  defaultConstructions.setDefaultExteriorSubSurfaceConstructions(defaultSubSurfaceConstructions);
  Building building = model.getUniqueModelObject<Building>();
  building.setDefaultConstructionSet(defaultConstructions);

  // DLM also have to put surfaces in a space, default constructions are not working for loose surfaces
  Space space(model);
  roof.setSpace(space);
  wall.setSpace(space);

  {
    // normal 0,1,0 on bottom edge
    vertices = {
      {4, 0, 1},
      {4, 0, 0},
      {5, 0, 0},
      {5, 0, 1},
    };

    SubSurface s(vertices, model);
    s.setSurface(wall);
    EXPECT_EQ("Door", s.subSurfaceType());
    s.assignDefaultSubSurfaceType();
    EXPECT_EQ("Door", s.subSurfaceType());

    WindowPropertyFrameAndDivider frame(model);
    EXPECT_FALSE(s.allowWindowPropertyFrameAndDivider());
    EXPECT_FALSE(s.setWindowPropertyFrameAndDivider(frame));
    ASSERT_FALSE(s.windowPropertyFrameAndDivider());

    s.setConstruction(construction);
    s.assignDefaultSubSurfaceType();
    EXPECT_EQ("GlassDoor", s.subSurfaceType());

    EXPECT_TRUE(s.allowWindowPropertyFrameAndDivider());
    ASSERT_TRUE(s.setWindowPropertyFrameAndDivider(frame));
    ASSERT_TRUE(s.windowPropertyFrameAndDivider());
    WindowPropertyFrameAndDivider frame2 = s.windowPropertyFrameAndDivider().get();
    EXPECT_EQ(frame, frame2);
  }
}

TEST_F(ModelFixture, SubSurface_SurfacePropertyOtherSideCoefficients) {
  Model model;
  SurfacePropertyOtherSideCoefficients otherSideCoefficients(model);
}

TEST_F(ModelFixture, SubSurface_SurfacePropertyOtherSideConditionsModel) {
  Model model;
  SurfacePropertyOtherSideConditionsModel otherSideModel(model);
}

TEST_F(ModelFixture, SubSurface_Clone) {
  Model model;
  std::vector<Point3d> vertices;

  // normal 0,0,1
  vertices = {
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

  SubSurface s1(vertices, model);
  SurfacePropertyConvectionCoefficients cc(s1);

  Blind blind(model);
  ShadingControl shadingControl(blind);
  s1.setShadingControl(shadingControl);

  auto s2 = s1.clone(model).cast<SubSurface>();
  EXPECT_TRUE(s2.surfacePropertyConvectionCoefficients());
  EXPECT_TRUE(s2.shadingControl());
}

TEST_F(ModelFixture, SubSurface_ShadingControls) {
  Model model;

  std::vector<Point3d> vertices{
    {0, 0, 1},
    {0, 0, 0},
    {1, 0, 0},
    {1, 0, 1},
  };
  SubSurface subSurface(vertices, model);

  Blind blind1(model);
  ShadingControl shadingControl1(blind1);

  Blind blind2(model);
  ShadingControl shadingControl2(blind2);

  EXPECT_EQ(0, subSurface.numberofShadingControls());
  EXPECT_TRUE(subSurface.addShadingControl(shadingControl1));
  EXPECT_EQ(1, subSurface.numberofShadingControls());
  EXPECT_TRUE(subSurface.addShadingControl(shadingControl1));
  EXPECT_EQ(1, subSurface.numberofShadingControls());
  EXPECT_TRUE(subSurface.addShadingControl(shadingControl2));
  EXPECT_EQ(2, subSurface.numberofShadingControls());
  subSurface.removeShadingControl(shadingControl1);
  EXPECT_EQ(1, subSurface.numberofShadingControls());
  subSurface.removeShadingControl(shadingControl1);
  EXPECT_EQ(1, subSurface.numberofShadingControls());
  subSurface.removeShadingControl(shadingControl2);
  EXPECT_EQ(0, subSurface.numberofShadingControls());

  std::vector<ShadingControl> shadingControls;
  shadingControls.push_back(shadingControl1);
  shadingControls.push_back(shadingControl2);
  EXPECT_TRUE(subSurface.addShadingControls(shadingControls));
  EXPECT_EQ(2, subSurface.numberofShadingControls());
  subSurface.removeAllShadingControls();
  EXPECT_EQ(0, subSurface.numberofShadingControls());

  // Test deprecated methods
  subSurface.addShadingControls(shadingControls);
  EXPECT_EQ(2, subSurface.numberofShadingControls());
  ASSERT_TRUE(subSurface.shadingControl());
  // EXPECT_EQ(subSurface.shadingControl()->handle(), shadingControl1.handle()); // no guarantee that it's shadingControl1
  EXPECT_TRUE(subSurface.setShadingControl(shadingControl2));
  EXPECT_EQ(1, subSurface.numberofShadingControls());
  ASSERT_TRUE(subSurface.shadingControl());
  EXPECT_EQ(subSurface.shadingControl().get(), shadingControl2);
  EXPECT_TRUE(subSurface.addShadingControl(shadingControl2));
  EXPECT_EQ(1, subSurface.numberofShadingControls());
  subSurface.resetShadingControl();
  EXPECT_EQ(0, subSurface.numberofShadingControls());
}

TEST_F(ModelFixture, SubSurface_isXXXAutocalculated_4399) {
  // Test for #4399
  Model m;
  std::vector<Point3d> points{
    Point3d{0, 1, 0},
    Point3d{0, 0, 0},
    Point3d{1, 0, 0},
    Point3d{1, 1, 0},
  };

  SubSurface subSurface(points, m);
  subSurface.autocalculateViewFactortoGround();
  EXPECT_TRUE(subSurface.isViewFactortoGroundAutocalculated());

  subSurface.autocalculateNumberofVertices();
  EXPECT_TRUE(subSurface.isNumberofVerticesAutocalculated());
}

// https://github.com/NREL/OpenStudio/issues/4361
TEST_F(ModelFixture, Issue_4361) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  // Create a surface and subsurface
  // get the surface net and gross area & subsurface gross area
  // surface gross area == surface net area + subsurface gross area
  // That is before the subsurface is assigned a frame and divider

  std::vector<Point3d> vertices{
    {0, 0, 2},
    {0, 0, 0},
    {2, 0, 0},
    {2, 0, 2},
  };
  Surface surface(vertices, model);
  surface.setSpace(space);
  surface.setSurfaceType("Wall");

  vertices.clear();
  vertices = {
    {0.5, 0, 1.5},
    {0.5, 0, 0.5},
    {1.5, 0, 0.5},
    {1.5, 0, 1.5},
  };

  SubSurface subSurface(vertices, model);
  subSurface.setSurface(surface);
  subSurface.setSubSurfaceType("FixedWindow");

  double surfaceGrossArea = surface.grossArea();
  double surfaceNetArea = surface.netArea();
  double subSurfaceGrossArea = subSurface.grossArea();
  double subSurfaceTotalArea = subSurface.roughOpeningArea();

  EXPECT_EQ(surfaceGrossArea, surfaceNetArea + subSurfaceGrossArea);
  double windowWallRatio = surface.windowToWallRatio();
  EXPECT_EQ(windowWallRatio, 0.25);

  // Then assign a frame and divider to the subsurface
  // then do the same thing with the areas

  WindowPropertyFrameAndDivider frame(model);
  frame.setFrameWidth(0.03);
  frame.setDividerWidth(0.05);
  frame.setNumberOfHorizontalDividers(2);
  frame.setNumberOfVerticalDividers(3);
  subSurface.setWindowPropertyFrameAndDivider(frame);

  double frameArea = subSurface.frameArea();
  EXPECT_NEAR(frameArea, 0.1236, 0.01);
  double divArea = subSurface.dividerArea();
  EXPECT_DOUBLE_EQ(divArea, 0.25);

  surfaceNetArea = surface.netArea();
  subSurfaceGrossArea = subSurface.grossArea();
  subSurfaceTotalArea = subSurface.roughOpeningArea();
  EXPECT_EQ(surfaceGrossArea, surfaceNetArea + subSurfaceTotalArea);

  windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.28, 0.01);
}
TEST_F(ModelFixture, Issue_4361_Subsurface_Outside_Parent) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  // Create a surface and subsurface
  // get the surface net and gross area & subsurface gross area
  // surface gross area == surface net area + subsurface gross area
  // That is before the subsurface is assigned a frame and divider

  std::vector<Point3d> vertices{
    {0, 0, 2},
    {0, 0, 0},
    {2, 0, 0},
    {2, 0, 2},
  };

  Surface surface(vertices, model);
  surface.setSpace(space);
  surface.setSurfaceType("Wall");

  vertices.clear();
  vertices = {
    {0.5, 0, 1.99},
    {0.5, 0, 0.99},
    {1.5, 0, 0.99},
    {1.5, 0, 1.99},
  };

  SubSurface subSurface(vertices, model);
  subSurface.setSurface(surface);
  subSurface.setSubSurfaceType("FixedWindow");

  double surfaceGrossArea = surface.grossArea();
  double surfaceNetArea = surface.netArea();
  double subSurfaceGrossArea = subSurface.grossArea();
  double subSurfaceTotalArea = subSurface.roughOpeningArea();

  EXPECT_EQ(surfaceGrossArea, surfaceNetArea + subSurfaceGrossArea);
  double windowWallRatio = surface.windowToWallRatio();
  EXPECT_EQ(windowWallRatio, 0.25);

  // Then assign a frame and divider to the subsurface
  // then do the same thing with the areas
  // The area should be unchanged because the subsurface is not enclosed
  // by the parent surface after including the frame

  WindowPropertyFrameAndDivider frame(model);
  frame.setFrameWidth(0.030);
  subSurface.setWindowPropertyFrameAndDivider(frame);

  surfaceNetArea = surface.netArea();
  subSurfaceGrossArea = subSurface.grossArea();
  subSurfaceTotalArea = subSurface.roughOpeningArea();
  EXPECT_EQ(surfaceGrossArea, surfaceNetArea + subSurfaceTotalArea);

  windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.25, 0.01);
}

TEST_F(ModelFixture, Issue_4361_Multi_Subsurfaces_Non_Overlapping) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  std::vector<Point3d> vertices{
    {0, 0, 2},
    {0, 0, 0},
    {4, 0, 0},
    {4, 0, 2},
  };
  Surface surface(vertices, model);
  surface.setSpace(space);
  surface.setSurfaceType("Wall");

  vertices = {
    {0.5, 0, 1.5},
    {0.5, 0, 0.5},
    {1.5, 0, 0.5},
    {1.5, 0, 1.5},
  };

  SubSurface subSurface1(vertices, model);
  subSurface1.setSurface(surface);
  subSurface1.setSubSurfaceType("FixedWindow");

  vertices.clear();
  vertices = {
    {2.5, 0, 1.5},
    {2.5, 0, 0.5},
    {3.5, 0, 0.5},
    {3.5, 0, 1.5},
  };

  SubSurface subSurface2(vertices, model);
  subSurface2.setSurface(surface);
  subSurface2.setSubSurfaceType("FixedWindow");

  // Surface area is 8, sub-surface areas are 2 therefore we should expect a WWR of 0.25
  double windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.25, 0.01);

  // Set frame and divider on both sub-surfaces
  WindowPropertyFrameAndDivider frame1(model);
  frame1.setFrameWidth(0.030);
  subSurface1.setWindowPropertyFrameAndDivider(frame1);

  WindowPropertyFrameAndDivider frame2(model);
  frame2.setFrameWidth(0.030);
  subSurface2.setWindowPropertyFrameAndDivider(frame2);

  // Surface area is still 8, sub-surface areas are 1.06x1.06x2 = 2.2472 so we should expect a WWR of 0.2809 (2.2472 / 8)
  windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.281, 0.01);
}

TEST_F(ModelFixture, Issue_4361_Multi_Subsurfaces_Overlapping) {
  Model model;

  ThermalZone thermalZone(model);

  Space space(model);
  space.setThermalZone(thermalZone);

  std::vector<Point3d> vertices{
    {0, 0, 2},
    {0, 0, 0},
    {4, 0, 0},
    {4, 0, 2},
  };
  Surface surface(vertices, model);
  surface.setSpace(space);
  surface.setSurfaceType("Wall");

  vertices.clear();
  vertices = {
    {0.5, 0, 1.5},
    {0.5, 0, 0.5},
    {1.5, 0, 0.5},
    {1.5, 0, 1.5},
  };

  SubSurface subSurface1(vertices, model);
  subSurface1.setSurface(surface);
  subSurface1.setSubSurfaceType("FixedWindow");

  vertices.clear();
  vertices = {
    {1.51, 0, 1.5},
    {1.51, 0, 0.5},
    {2.51, 0, 0.5},
    {2.51, 0, 1.5},
  };

  SubSurface subSurface2(vertices, model);
  subSurface2.setSurface(surface);
  subSurface2.setSubSurfaceType("FixedWindow");

  // Surface area is 8, sub surface areas are 2 therefore we should expect a WWR of 0.25
  double windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.25, 0.01);

  // Set a frame and divider on both sub-surfaces
  WindowPropertyFrameAndDivider frame1(model);
  frame1.setFrameWidth(0.030);
  subSurface1.setWindowPropertyFrameAndDivider(frame1);

  WindowPropertyFrameAndDivider frame2(model);
  frame2.setFrameWidth(0.030);
  subSurface2.setWindowPropertyFrameAndDivider(frame2);

  // When the frame and dividers are added the surfaces will overlap to form a single
  // sub-surface which is 2.07 x 1.06 = 2.1942 which gives a WWR of 0.274 (2.1942 / 8)
  windowWallRatio = surface.windowToWallRatio();
  EXPECT_NEAR(windowWallRatio, 0.2742, 0.01);
}

TEST_F(ModelFixture, 4403_FenestrationAssembly) {
  // Test for #4403 - Add Sql helper methods to retrieve U-factors, SHGC, or VT for glazing systems

  // This one has fenestration that includes WindowProperty:FrameAndDivider
  openstudio::path idfPath = resourcesPath() / toPath("energyplus/FrameAndDivider/in.idf");
  energyplus::ReverseTranslator reverseTranslator;
  ASSERT_NO_THROW(reverseTranslator.loadModel(idfPath));
  OptionalModel _model = reverseTranslator.loadModel(idfPath);
  ASSERT_TRUE(_model);
  Model model = _model.get();

  openstudio::path sqlPath = resourcesPath() / toPath("energyplus/FrameAndDivider/eplusout.sql");
  openstudio::SqlFile sqlFile = openstudio::SqlFile(sqlPath);
  ASSERT_TRUE(sqlFile.connectionOpen());

  model.setSqlFile(sqlFile);
  ASSERT_TRUE(model.sqlFile());

  OptionalSubSurface subSurface = model.getConcreteModelObjectByName<SubSurface>("Story 1 Core Space Exterior Wall Window");
  ASSERT_TRUE(subSurface);

  ASSERT_TRUE(subSurface->assemblyUFactor());
  EXPECT_EQ(2.546, subSurface->assemblyUFactor().get());

  ASSERT_TRUE(subSurface->assemblySHGC());
  EXPECT_EQ(0.350, subSurface->assemblySHGC().get());

  ASSERT_TRUE(subSurface->assemblyVisibleTransmittance());
  EXPECT_EQ(0.440, subSurface->assemblyVisibleTransmittance().get());
}

TEST_F(ModelFixture, 4678_SubSurfaceGlassUFactorSqlError) {
  // Test for #4678 - Glass U Factor sql error

  // This one has fenestration that includes WindowProperty:FrameAndDivider
  openstudio::path idfPath = resourcesPath() / toPath("energyplus/FrameAndDivider/in.idf");
  energyplus::ReverseTranslator reverseTranslator;
  ASSERT_NO_THROW(reverseTranslator.loadModel(idfPath));
  OptionalModel _model = reverseTranslator.loadModel(idfPath);
  ASSERT_TRUE(_model);
  Model model = _model.get();

  openstudio::path sqlPath = resourcesPath() / toPath("energyplus/FrameAndDivider/eplusout.sql");
  openstudio::SqlFile sqlFile = openstudio::SqlFile(sqlPath);
  ASSERT_TRUE(sqlFile.connectionOpen());

  model.setSqlFile(sqlFile);
  ASSERT_TRUE(model.sqlFile());

  OptionalSubSurface subSurface = model.getConcreteModelObjectByName<SubSurface>("Story 1 Core Space Exterior Wall Window");
  ASSERT_TRUE(subSurface);

  OptionalConstructionBase oConstruction = subSurface->construction();
  OptionalSurface oSurface = subSurface->surface();

  ASSERT_TRUE(oConstruction);
  ASSERT_TRUE(oSurface);

  EXPECT_FALSE(oConstruction->isOpaque());
  EXPECT_TRUE(oConstruction->isFenestration());

  ASSERT_TRUE(subSurface->uFactor());
  double uFactor = subSurface->uFactor().get();
  EXPECT_DOUBLE_EQ(2.559, uFactor);

  double filmResistance = oSurface->filmResistance();
  double thermalConductance = 1.0 / (1.0 / (uFactor)-filmResistance);
  ASSERT_TRUE(subSurface->thermalConductance());
  EXPECT_DOUBLE_EQ(thermalConductance, subSurface->thermalConductance().get());
}
