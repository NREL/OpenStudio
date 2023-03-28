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
#include "GeometryFixture.hpp"

#include "../Geometry.hpp"
#include "../Point3d.hpp"
#include "../Polyhedron.hpp"
#include "../PointLatLon.hpp"
#include "../Vector3d.hpp"

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
