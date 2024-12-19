/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "GeometryFixture.hpp"

#include "../Geometry.hpp"
#include "../Point3d.hpp"
#include "../Polyhedron.hpp"
#include "../PointLatLon.hpp"
#include "../Vector3d.hpp"
#include "../StandardShapes.hpp"

#include <fmt/core.h>

#include <vector>

using namespace openstudio;

std::vector<Point3d> makeRectangle(double min_x = 0.0, double max_x = 10.0, double min_y = 0.0, double max_y = 10.0, double min_z = 0.0,
                                   double max_z = 0.0) {
  return {
    {min_x, min_y, max_z},
    {min_x, max_y, min_z},
    {max_x, max_y, min_z},
    {max_x, min_y, max_z},
  };
}

TEST_F(GeometryFixture, Polyhedron_Enclosed) {

  // This is a shoebox model of a lengthy plenum-like zone. 30x10x0.3m. The south wall is split in the middle

  const Surface3d south1({{+0.0, +0.0, +0.3}, {+0.0, +0.0, +0.0}, {+15.0, +0.0, +0.0}, {+15.0, +0.0, +0.3}}, "1-SOUTH-1", 0);
  const Surface3d south2({{+15.0, +0.0, +0.3}, {+15.0, +0.0, +0.0}, {+30.0, +0.0, +0.0}, {+30.0, +0.0, +0.3}}, "1-SOUTH-2", 1);

  const Surface3d north({{+30.0, +10.0, +0.3}, {+30.0, +10.0, +0.0}, {+0.0, +10.0, +0.0}, {+0.0, +10.0, +0.3}}, "4-NORTH", 2);

  const Surface3d east({{+30.0, +0.0, +0.3}, {+30.0, +0.0, +0.0}, {+30.0, +10.0, +0.0}, {+30.0, +10.0, +0.3}}, "3-EAST", 3);

  const Surface3d west({{+0.0, +10.0, +0.3}, {+0.0, +10.0, +0.0}, {+0.0, +0.0, +0.0}, {+0.0, +0.0, +0.3}}, "2-WEST", 4);

  const Surface3d roof({{+30.0, +0.0, +0.3}, {+30.0, +10.0, +0.3}, {+0.0, +10.0, +0.3}, {+0.0, +0.0, +0.3}}, "ROOF", 5);

  const Surface3d floor({{+0.0, +0.0, +0.0}, {+0.0, +10.0, +0.0}, {+30.0, +10.0, +0.0}, {+30.0, +0.0, +0.0}}, "FLOOR", 6);

  std::vector<Surface3d> surfaces = {south1, south2, north, east, west, roof, floor};
  constexpr double volume = 30.0 * 10.0 * 0.3;

  {
    const Polyhedron zonePoly(surfaces);

    // Test individual components
    // 6 faces for a box, 7 in this case since one face is split in two, each with 4 vertices =>  4*7 = 28
    // The Polyhedron is not enclosed, so it'll go through updateZonePolygonsForMissingColinearPoints
    // We expect the two points from the split south walls to have been added to the roof and floor
    EXPECT_EQ(30, zonePoly.numVertices());
    EXPECT_TRUE(zonePoly.isEnclosedVolume());
    EXPECT_EQ(0, zonePoly.edgesNotTwo().size());
    EXPECT_FALSE(zonePoly.hasAnySurfaceWithIncorrectOrientation());
    // 8 for the box, plus two for the split of the south wall
    auto uniqVertices = zonePoly.uniqueVertices();
    EXPECT_EQ(10, uniqVertices.size());

    EXPECT_EQ(volume, zonePoly.polyhedronVolume());
    EXPECT_EQ(volume, zonePoly.calcDivergenceTheoremVolume());
  }

  {
    // Check the corner case where NONE of the surfaces are correctly oriented
    for (auto& sf : surfaces) {
      std::reverse(sf.vertices.begin(), sf.vertices.end());
    }
    const Polyhedron flippedPoly(surfaces);
    EXPECT_EQ(30, flippedPoly.numVertices());
    EXPECT_TRUE(flippedPoly.isEnclosedVolume());
    EXPECT_EQ(0, flippedPoly.edgesNotTwo().size());
    EXPECT_TRUE(flippedPoly.hasAnySurfaceWithIncorrectOrientation());
    EXPECT_TRUE(flippedPoly.isCompletelyInsideOut());
    EXPECT_EQ(surfaces.size(), flippedPoly.findSurfacesWithIncorrectOrientation().size());
    EXPECT_EQ(-volume, flippedPoly.polyhedronVolume());
  }
}

TEST_F(GeometryFixture, Polyhedron_Titled_Roof) {

  // This is a 30x10x0.3 base, with a rectangle triangle on top of 30x10x10
  //                       ▲ z
  //                       │
  //                      x├─ 10.0
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

  const Surface3d south2({{+15.0, +0.0, +10.3}, {+15.0, +0.0, +0.0}, {+30.0, +0.0, +0.0}, {+30.0, +0.0, +10.3}}, "1-SOUTH-2", 0);

  // We put extra vertices here to skew the calculate that Space::volume does
  const Surface3d roof(
    {{+30.0, +0.0, +10.3}, {+30.0, +10.0, +0.3}, {+0.0, +10.0, +0.3}, {+0.0, +0.0, +10.3}, {+10.0, +0.0, +10.3}, {+20.0, +0.0, +10.3}}, "ROOF", 1);

  const Surface3d east({{+30.0, +0.0, +10.3}, {+30.0, +0.0, +0.0}, {+30.0, +10.0, +0.0}, {+30.0, +10.0, +0.3}}, "3-EAST", 2);

  const Surface3d north({{+30.0, +10.0, +0.3}, {+30.0, +10.0, +0.0}, {+0.0, +10.0, +0.0}, {+0.0, +10.0, +0.3}}, "4-NORTH", 3);

  const Surface3d west({{+0.0, +10.0, +0.3}, {+0.0, +10.0, +0.0}, {+0.0, +0.0, +0.0}, {+0.0, +0.0, +10.3}}, "2-WEST", 4);

  const Surface3d south1({{+0.0, +0.0, +10.3}, {+0.0, +0.0, +0.0}, {+15.0, +0.0, +0.0}, {+15.0, +0.0, +10.3}}, "1-SOUTH-1", 5);

  const Surface3d floor({{+0.0, +0.0, +0.0}, {+0.0, +10.0, +0.0}, {+30.0, +10.0, +0.0}, {+30.0, +0.0, +0.0}}, "FLOOR", 6);

  const Polyhedron zonePoly({south1, south2, north, east, west, roof, floor});

  EXPECT_TRUE(zonePoly.isEnclosedVolume());
  EXPECT_TRUE(zonePoly.edgesNotTwo().empty());
  EXPECT_FALSE(zonePoly.hasAnySurfaceWithIncorrectOrientation());

  constexpr double volume = 30.0 * 10.0 * 0.3 + 30.0 * 10.0 * 10.0 / 2.0;
  EXPECT_DOUBLE_EQ(volume, zonePoly.polyhedronVolume());
  EXPECT_DOUBLE_EQ(volume, zonePoly.calcDivergenceTheoremVolume());
}

TEST_F(GeometryFixture, Polyhedron_Box_With_Hole) {

  // This is a 10x10x1 box, with a 5x5x1 hole cut in the middle

  // We put extra vertices here to skew the calculate that Space::volume does
  const Surface3d roof({{+10.0, +0.0, +1.0},
                        {+10.0, +10.0, +1.0},
                        {+0.0, +10.0, +1.0},
                        {+0.0, +0.0, +1.0},
                        {+2.50, +2.5, +1.0},
                        {+2.50, +7.50, +1.0},
                        {+7.5, +7.50, +1.0},
                        {+7.5, +2.5, +1.0},
                        {+2.50, +2.5, +1.0},
                        {+0.0, +0.0, +1.0}},
                       "ROOF", 1);
  EXPECT_FALSE(roof.isConvex());

  const Surface3d east1({{+10.0, +0.0, +1.0}, {+10.0, +0.0, +0.0}, {+10.0, +10.0, +0.0}, {+10.0, +10.0, +1.0}}, "EAST1", 2);
  EXPECT_TRUE(east1.isConvex());

  const Surface3d east2({{+7.50, +7.50, +1.0}, {+7.50, +7.50, +0.0}, {+7.50, +2.5, +0.0}, {+7.50, +2.5, +1.0}}, "EAST2", 7);
  EXPECT_TRUE(east2.isConvex());

  const Surface3d north1({{+10.0, +10.0, +1.0}, {+10.0, +10.0, +0.0}, {+0.0, +10.0, +0.0}, {+0.0, +10.0, +1.0}}, "NORTH1", 3);
  EXPECT_TRUE(north1.isConvex());

  const Surface3d north2({{+2.5, +7.50, +1.0}, {+2.5, +7.50, +0.0}, {+7.50, +7.50, +0.0}, {+7.50, +7.50, +1.0}}, "NORTH2", 8);
  EXPECT_TRUE(north2.isConvex());

  const Surface3d west1({{+0.0, +10.0, +1.0}, {+0.0, +10.0, +0.0}, {+0.0, +0.0, +0.0}, {+0.0, +0.0, +1.0}}, "WEST1", 4);
  EXPECT_TRUE(west1.isConvex());

  const Surface3d west2({{+2.5, +2.5, +1.0}, {+2.5, +2.5, +0.0}, {+2.5, +7.50, +0.0}, {+2.5, +7.50, +1.0}}, "WEST2", 9);
  EXPECT_TRUE(west2.isConvex());

  const Surface3d south1({{+0.0, +0.0, +1.0}, {+0.0, +0.0, +0.0}, {+10.0, +0.0, +0.0}, {+10.0, +0.0, +1.0}}, "SOUTH1", 5);
  EXPECT_TRUE(south1.isConvex());

  const Surface3d south2({{+7.5, +2.5, +1.0}, {+7.5, +2.5, +0.0}, {+2.5, +2.5, +0.0}, {+2.5, +2.5, +1.0}}, "SOUTH2", 10);
  EXPECT_TRUE(south2.isConvex());

  const Surface3d floor({{+0.0, +10.0, +0.0},
                         {+10.0, +10.0, +0.0},
                         {+10.0, +0.0, +0.0},
                         {+0.0, +0.0, +0.0},
                         {+2.50, +2.5, +0.0},
                         {+7.50, +2.50, +0.0},
                         {+7.50, +7.5, +0.0},
                         {+2.50, +7.50, +0.0},
                         {+2.50, +2.5, +0.0},
                         {+0.0, +0.0, +0.0}},
                        "FLOOR", 6);
  EXPECT_FALSE(floor.isConvex());

  const Polyhedron zonePoly({south1, north1, east1, west1, roof, floor, south2, north2, east2, west2});

  EXPECT_TRUE(zonePoly.isEnclosedVolume());
  EXPECT_TRUE(zonePoly.edgesNotTwo().empty());
  EXPECT_FALSE(zonePoly.hasAnySurfaceWithIncorrectOrientation());

  constexpr double volume = 10.0 * 10.0 * 1.0 - 5.0 * 5.0 * 1.0;
  EXPECT_DOUBLE_EQ(volume, zonePoly.polyhedronVolume());
  EXPECT_DOUBLE_EQ(volume, zonePoly.calcDivergenceTheoremVolume());
}

TEST_F(GeometryFixture, Surface3d_Convexity) {
  const Point3d p0{};
  for (size_t i = 3; i <= 16; ++i) {
    const std::vector<Point3d> points = convexRegularPolygon(p0, i, 1.0);
    const Surface3d surface(points, fmt::format("Regular {}-sided polygon", i), 0);
    EXPECT_EQ(i, surface.edges.size());
    EXPECT_TRUE(surface.isConvex());
  }

  constexpr double total_length = 10.0;

  {
    const std::vector<Point3d> points = hShapedPolygon(p0, total_length);
    const Surface3d surface(points, "hShapedPolygon", 0);
    EXPECT_EQ(12, surface.edges.size());
    EXPECT_FALSE(surface.isConvex());
  }

  {
    const std::vector<Point3d> points = uShapedPolygon(p0, total_length);
    const Surface3d surface(points, "uShapedPolygon", 0);
    EXPECT_EQ(8, surface.edges.size());
    EXPECT_FALSE(surface.isConvex());
  }

  {
    const std::vector<Point3d> points = tShapedPolygon(p0, total_length);
    const Surface3d surface(points, "tShapedPolygon", 0);
    EXPECT_EQ(8, surface.edges.size());
    EXPECT_FALSE(surface.isConvex());
  }

  {
    const std::vector<Point3d> points = lShapedPolygon(p0, total_length);
    const Surface3d surface(points, "lShapedPolygon", 0);
    EXPECT_EQ(6, surface.edges.size());
    EXPECT_FALSE(surface.isConvex());
  }
}
