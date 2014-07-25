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
#include "../Space.hpp"
#include "../Space_Impl.hpp"
#include "../Surface.hpp"
#include "../Surface_Impl.hpp"
#include "../SubSurface.hpp"
#include "../SubSurface_Impl.hpp"
#include "../Building.hpp"
#include "../Building_Impl.hpp"
#include "../Construction.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/geometry/Point3d.hpp"
#include "../../utilities/geometry/Vector3d.hpp"

#include <boost/algorithm/string/case_conv.hpp>

using namespace openstudio;
using namespace openstudio::model;

void removeSubSurfaces(Surface& surface){
  for (SubSurface s : surface.subSurfaces()){
    s.remove();
  }
  EXPECT_EQ(0, surface.subSurfaces().size());
}

void removeSubSurfaces(Model& model){
  for (SubSurface s : model.getModelObjects<SubSurface>()){
    s.remove();
  }
  EXPECT_EQ(0, model.getModelObjects<SubSurface>().size());
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

  SubSurfaceVector subSurfaces = model.getModelObjects<SubSurface>();
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

TEST_F(ModelFixture, 0_Vertex_SubSurface)
{
  Model model;

  std::vector<Point3d> vertices;

  EXPECT_THROW(SubSurface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 1_Vertex_SubSurface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));

  EXPECT_THROW(SubSurface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 2_Vertex_SubSurface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));

  EXPECT_THROW(SubSurface(vertices, model), openstudio::Exception);
}

TEST_F(ModelFixture, 3_Vertex_SubSurface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));

  EXPECT_NO_THROW(SubSurface(vertices, model));
}

TEST_F(ModelFixture, 4_Vertex_SubSurface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));

  EXPECT_NO_THROW(SubSurface(vertices, model));
}

TEST_F(ModelFixture, 5_Vertex_SubSurface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,1));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(1,0,0));
  vertices.push_back(Point3d(1,0,1));
  vertices.push_back(Point3d(0.5,0,1.5));

  EXPECT_NO_THROW(SubSurface(vertices, model));
}

TEST_F(ModelFixture, AdjacentSubSurface)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,3));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(3,0,0));
  vertices.push_back(Point3d(3,0,3));

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.adjacentSurface());

  std::reverse(vertices.begin(), vertices.end());

  Space space2(model);
  Surface wall2(vertices, model);
  wall2.setSpace(space2);
  EXPECT_FALSE(wall2.adjacentSurface());

  vertices.clear();
  vertices.push_back(Point3d(1,0,2));
  vertices.push_back(Point3d(1,0,1));
  vertices.push_back(Point3d(2,0,1));
  vertices.push_back(Point3d(2,0,2));

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

TEST_F(ModelFixture, AdjacentSubSurface2)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,3));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(3,0,0));
  vertices.push_back(Point3d(3,0,3));

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.adjacentSurface());

  std::reverse(vertices.begin(), vertices.end());

  Space space2(model);
  Surface wall2(vertices, model);
  wall2.setSpace(space2);
  EXPECT_FALSE(wall2.adjacentSurface());

  vertices.clear();
  vertices.push_back(Point3d(1,0,2));
  vertices.push_back(Point3d(1,0,1));
  vertices.push_back(Point3d(2,0,1));
  vertices.push_back(Point3d(2,0,2));

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

TEST_F(ModelFixture, AdjacentSubSurface3)
{
  Model model;

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,3));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(3,0,0));
  vertices.push_back(Point3d(3,0,3));

  Space space1(model);
  Surface wall1(vertices, model);
  wall1.setSpace(space1);
  EXPECT_FALSE(wall1.adjacentSurface());

  std::reverse(vertices.begin(), vertices.end());

  Space space2(model);
  Surface wall2(vertices, model);
  wall2.setSpace(space2);
  EXPECT_FALSE(wall2.adjacentSurface());

  vertices.clear();
  vertices.push_back(Point3d(1,0,2));
  vertices.push_back(Point3d(1,0,1));
  vertices.push_back(Point3d(2,0,1));
  vertices.push_back(Point3d(2,0,2));

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

TEST_F(ModelFixture, ExampleDaylightingControlPlacement)
{
  Model model;

  Space space(model);

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,10,0));
  vertices.push_back(Point3d(10,10,0));
  vertices.push_back(Point3d(10,0,0));
  vertices.push_back(Point3d(0,0,0));
  Surface floor(vertices, model);
  floor.setSpace(space);
  EXPECT_EQ("Floor", floor.surfaceType());

  vertices.clear();
  vertices.push_back(Point3d(0,0,10));
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(10,0,0));
  vertices.push_back(Point3d(10,0,10));
  Surface wall(vertices, model);
  wall.setSpace(space);
  EXPECT_EQ("Wall", wall.surfaceType());

  vertices.clear();
  vertices.push_back(Point3d(2,0,8));
  vertices.push_back(Point3d(2,0,2));
  vertices.push_back(Point3d(8,0,2));
  vertices.push_back(Point3d(8,0,8));
  SubSurface window(vertices, model);
  window.setSurface(wall);

  boost::optional<Point3d> windowCentroid = getCentroid(window.vertices());
  ASSERT_TRUE(windowCentroid);
  EXPECT_DOUBLE_EQ(5, windowCentroid->x());
  EXPECT_DOUBLE_EQ(0, windowCentroid->y());
  EXPECT_DOUBLE_EQ(5, windowCentroid->z());

  // move back 1 m
  Point3d windowCentroidPrime = windowCentroid.get() + (-1.0)*window.outwardNormal();
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
  Point3d point = pointOnFloor + Vector3d(0,0,1);
  EXPECT_DOUBLE_EQ(5, point.x());
  EXPECT_DOUBLE_EQ(1, point.y());
  EXPECT_DOUBLE_EQ(1, point.z());
}

TEST_F(ModelFixture, SkylightPattern_SingleSurface)
{
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  std::vector<Point3dVector> pattern; 
  std::vector<SubSurface> skylights; 

  // no geometry yet
  pattern = building.generateSkylightPattern(0.00, 1.5, 1);
  EXPECT_EQ(0, pattern.size());

  Space space(model);

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(30,0,0));
  vertices.push_back(Point3d(30,20,0));
  vertices.push_back(Point3d(0,20,0));
  
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
  vertices.clear();
  vertices.push_back(Point3d(20,10,0));
  vertices.push_back(Point3d(22,10,0));
  vertices.push_back(Point3d(22,12,0));
  vertices.push_back(Point3d(20,12,0));
  SubSurface skylight(vertices, model);
  skylight.setSurface(roof);
  EXPECT_DOUBLE_EQ(4.0, skylight.grossArea());
  EXPECT_EQ("Skylight", skylight.subSurfaceType());
  EXPECT_NEAR(4.0/600.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(4.0/600.0, roof.skylightToProjectedFloorRatio(), 0.001);

  pattern = building.generateSkylightPattern(0.03, 1.5, 1);
  EXPECT_FALSE(pattern.empty());
  skylights = roof.createSubSurfaces(pattern, 0.01, boost::none);
  EXPECT_TRUE(skylights.empty());
  EXPECT_NEAR(4.0/600.0, roof.skylightToRoofRatio(), 0.001);
  EXPECT_NEAR(4.0/600.0, roof.skylightToProjectedFloorRatio(), 0.001);
}


TEST_F(ModelFixture, SkylightPattern_SingleSurface2)
{
  Model model;

  Building building = model.getUniqueModelObject<Building>();
  std::vector<Point3dVector> pattern; 
  std::vector<SubSurface> skylights; 

  // no geometry yet
  pattern = building.generateSkylightPattern(0.00, 1.5, 1);
  EXPECT_EQ(0, pattern.size());

  Space space(model);

  std::vector<Point3d> vertices;
  vertices.push_back(Point3d(10,-10,0));
  vertices.push_back(Point3d(40,-10,0));
  vertices.push_back(Point3d(40,10,0));
  vertices.push_back(Point3d(10,10,0));
  
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

void checkExpectedSkylightRatios(const Model& model, double expectedRoofArea, double expectedSkylightRatio, double tol)
{
  double totalGrossRoofArea = 0.0;
  double totalSkylightArea = 0.0;
  for (Surface surface : model.getModelObjects<Surface>()){
    boost::optional<Space> space = surface.space();
    ASSERT_TRUE(space);
    double multiplier = space->multiplier();
    if (istringEqual("RoofCeiling", surface.surfaceType()) &&
        istringEqual("Outdoors", surface.outsideBoundaryCondition())){

      double grossRoofArea = surface.grossArea();
      double netRoofArea = surface.netArea();
      double skylightToRoofRatio = surface.skylightToRoofRatio();
      double skylightArea = skylightToRoofRatio*grossRoofArea;
      
      EXPECT_NEAR(netRoofArea, grossRoofArea-skylightArea, 0.001);

      totalGrossRoofArea += multiplier*grossRoofArea;
      totalSkylightArea += multiplier*skylightArea;
    }
  }

  if (totalGrossRoofArea > 0){
    double ratio = totalSkylightArea/totalGrossRoofArea;
    EXPECT_NEAR(expectedSkylightRatio, ratio, tol);
  }
}

TEST_F(ModelFixture, SkylightPattern_PerimCore)
{
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

  vertices.clear();
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(5,5,0));
  vertices.push_back(Point3d(5,15,0));
  vertices.push_back(Point3d(0,20,0));
  Surface roofWest(vertices, model);
  roofWest.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofWest.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices.clear();
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(30,0,0));
  vertices.push_back(Point3d(15,5,0));
  vertices.push_back(Point3d(5,5,0));
  Surface roofSouth(vertices, model);
  roofSouth.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofSouth.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices.clear();
  vertices.push_back(Point3d(30,0,0));
  vertices.push_back(Point3d(30,20,0));
  vertices.push_back(Point3d(5,15,0));
  vertices.push_back(Point3d(5,5,0));
  Surface roofEast(vertices, model);
  roofEast.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofEast.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices.clear();
  vertices.push_back(Point3d(5,15,0));
  vertices.push_back(Point3d(15,15,0));
  vertices.push_back(Point3d(30,20,0));
  vertices.push_back(Point3d(0,20,0));
  Surface roofNorth(vertices, model);
  roofNorth.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofNorth.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices.clear();
  vertices.push_back(Point3d(5,5,0));
  vertices.push_back(Point3d(15,5,0));
  vertices.push_back(Point3d(15,15,0));
  vertices.push_back(Point3d(5,15,0));
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


TEST_F(ModelFixture, SkylightPattern_PerimCore_Rotated)
{
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

  vertices.clear();
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(5,5,0));
  vertices.push_back(Point3d(5,15,0));
  vertices.push_back(Point3d(0,20,0));
  Surface roofWest(vertices, model);
  roofWest.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofWest.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices.clear();
  vertices.push_back(Point3d(0,0,0));
  vertices.push_back(Point3d(30,0,0));
  vertices.push_back(Point3d(15,5,0));
  vertices.push_back(Point3d(5,5,0));
  Surface roofSouth(vertices, model);
  roofSouth.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofSouth.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices.clear();
  vertices.push_back(Point3d(30,0,0));
  vertices.push_back(Point3d(30,20,0));
  vertices.push_back(Point3d(5,15,0));
  vertices.push_back(Point3d(5,5,0));
  Surface roofEast(vertices, model);
  roofEast.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofEast.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices.clear();
  vertices.push_back(Point3d(5,15,0));
  vertices.push_back(Point3d(15,15,0));
  vertices.push_back(Point3d(30,20,0));
  vertices.push_back(Point3d(0,20,0));
  Surface roofNorth(vertices, model);
  roofNorth.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofNorth.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  vertices.clear();
  vertices.push_back(Point3d(5,5,0));
  vertices.push_back(Point3d(15,5,0));
  vertices.push_back(Point3d(15,15,0));
  vertices.push_back(Point3d(5,15,0));
  Surface roofCore(vertices, model);
  roofCore.setSpace(space);
  EXPECT_EQ("RoofCeiling", roofCore.surfaceType());
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);

  // 5% square
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = generateSkylightPattern(spaces, 90, 0.05, 1, 1);
  skylights = applySkylightPattern(pattern, model.getModelObjects<Space>(), boost::none);
  checkExpectedSkylightRatios(model, 600.0, 0.05, 0.005);

  // 3% square
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = generateSkylightPattern(spaces, 45, 0.03, 1, 1);
  skylights = applySkylightPattern(pattern, model.getModelObjects<Space>(), boost::none);
  checkExpectedSkylightRatios(model, 600.0, 0.03, 0.006);  // BUMP this up a bit

  // 5% rectangle
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = generateSkylightPattern(spaces, 135, 0.05, 1, 1);
  skylights = applySkylightPattern(pattern, model.getModelObjects<Space>(), boost::none);
  checkExpectedSkylightRatios(model, 600.0, 0.05, 0.005);

  // 3% rectangle
  removeSubSurfaces(model);
  checkExpectedSkylightRatios(model, 600.0, 0.0, 0.0);
  pattern = generateSkylightPattern(spaces, 180, 0.03, 1, 1);
  skylights = applySkylightPattern(pattern, model.getModelObjects<Space>(), boost::none);
  checkExpectedSkylightRatios(model, 600.0, 0.03, 0.005);
}
