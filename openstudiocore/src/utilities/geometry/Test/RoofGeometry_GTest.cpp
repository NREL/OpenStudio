/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>
//#include "../RoofGeometry.hpp"
#include "../Intersection.hpp"
#include "../Point3d.hpp"
#include "GeometryFixture.hpp"
#include <math.h>

using namespace openstudio;

bool polygonMatches(std::vector< std::vector<Point3d> > polygons, std::vector<Point3d> polygon, double tol)
{
  unsigned polygonMatches = 0;
  for (unsigned i = 0; i < polygons.size(); ++i) {
    if (circularEqual(polygons[i], polygon, tol)) {
      polygonMatches += 1;
    }
  }
  return (polygonMatches == 1);
}

TEST_F(GeometryFixture, Square_6_12)
{
  double tol = 0.01;
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(5.0, 25.0, 0.0));
  footprint.push_back(Point3d(25.0, 25.0, 0.0));
  footprint.push_back(Point3d(25.0, 5.0, 0.0));
  footprint.push_back(Point3d(5.0, 5.0, 0.0));

  // temporary
  //std::vector< std::vector<Point3d> > roofPolygons = makeRoofSurfaces(footprint, pitch);
  std::vector< std::vector<Point3d> > roofPolygons = std::vector< std::vector<Point3d> >();

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 0.0), Point3d(5.0, 25.0, 0.0), Point3d(15.0, 15.0, 5.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 5.0, 0.0), Point3d(5.0, 5.0, 0.0), Point3d(15.0, 15.0, 5.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 25.0, 0.0), Point3d(25.0, 5.0, 0.0), Point3d(15.0, 15.0, 5.0)});
  expectedRoofPolygons.push_back({Point3d(5.0, 25.0, 0.0), Point3d(25.0, 25.0, 0.0), Point3d(15.0, 15.0, 5.0)});

  EXPECT_EQ(roofPolygons.size(), expectedRoofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    bool test = polygonMatches(expectedRoofPolygons, roofPolygons[i], tol);
    EXPECT_TRUE(test);
  }
}

TEST_F(GeometryFixture, Square_1_12) {
  double tol = 0.01;
  double pitch = radToDeg(atan(1.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(5.0, 25.0, 0.0));
  footprint.push_back(Point3d(25.0, 25.0, 0.0));
  footprint.push_back(Point3d(25.0, 5.0, 0.0));
  footprint.push_back(Point3d(5.0, 5.0, 0.0));

  // temporary
  //std::vector< std::vector<Point3d> > roofPolygons = makeRoofSurfaces(footprint, pitch);
  std::vector< std::vector<Point3d> > roofPolygons = std::vector< std::vector<Point3d> >();

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 0.0), Point3d(5.0, 25.0, 0.0), Point3d(15.0, 15.0, 0.833)});
  expectedRoofPolygons.push_back({Point3d(25.0, 5.0, 0.0), Point3d(5.0, 5.0, 0.0), Point3d(15.0, 15.0, 0.833)});
  expectedRoofPolygons.push_back({Point3d(25.0, 25.0, 0.0), Point3d(25.0, 5.0, 0.0), Point3d(15.0, 15.0, 0.833)});
  expectedRoofPolygons.push_back({Point3d(5.0, 25.0, 0.0), Point3d(25.0, 25.0, 0.0), Point3d(15.0, 15.0, 0.833)});

  EXPECT_EQ(roofPolygons.size(), expectedRoofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    bool test = polygonMatches(expectedRoofPolygons, roofPolygons[i], tol);
    EXPECT_TRUE(test);
  }
}

TEST_F(GeometryFixture, Rectangle_3_12) {
  double tol = 0.01;
  double pitch = radToDeg(atan(3.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(5.0, 25.0, 0.0));
  footprint.push_back(Point3d(40.0, 25.0, 0.0));
  footprint.push_back(Point3d(40.0, 5.0, 0.0));
  footprint.push_back(Point3d(5.0, 5.0, 0.0));

  // temporary
  //std::vector< std::vector<Point3d> > roofPolygons = makeRoofSurfaces(footprint, pitch);
  std::vector< std::vector<Point3d> > roofPolygons = std::vector< std::vector<Point3d> >();

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 0.0), Point3d(5.0, 25.0, 0.0), Point3d(15.0, 15.0, 2.5)});
  expectedRoofPolygons.push_back({Point3d(40.0, 5.0, 0.0), Point3d(5.0, 5.0, 0.0), Point3d(15.0, 15.0, 2.5), Point3d(30.0, 15.0, 2.5)});
  expectedRoofPolygons.push_back({Point3d(40.0, 25.0, 0.0), Point3d(40.0, 5.0, 0.0), Point3d(30.0, 15.0, 2.5)});
  expectedRoofPolygons.push_back({Point3d(5.0, 25.0, 0.0), Point3d(40.0, 25.0, 0.0), Point3d(30.0, 15.0, 2.5), Point3d(15.0, 15.0, 2.5)});

  EXPECT_EQ(roofPolygons.size(), expectedRoofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    bool test = polygonMatches(expectedRoofPolygons, roofPolygons[i], tol);
    EXPECT_TRUE(test);
  }
}

TEST_F(GeometryFixture, Hshape_6_12) {
  double tol = 0.01;
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0, 25, 0));
  footprint.push_back(Point3d(10, 25, 0));
  footprint.push_back(Point3d(10, 18, 0));
  footprint.push_back(Point3d(20, 18, 0));
  footprint.push_back(Point3d(20, 25, 0));
  footprint.push_back(Point3d(30, 25, 0));
  footprint.push_back(Point3d(30, 5, 0));
  footprint.push_back(Point3d(20, 5, 0));
  footprint.push_back(Point3d(20, 12, 0));
  footprint.push_back(Point3d(10, 12, 0));
  footprint.push_back(Point3d(10, 5, 0));
  footprint.push_back(Point3d(0, 5, 0));

  // temporary
  //std::vector< std::vector<Point3d> > roofPolygons = makeRoofSurfaces(footprint, pitch);
  std::vector< std::vector<Point3d> > roofPolygons = std::vector< std::vector<Point3d> >();

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(20, 5, 0), Point3d(20, 12, 0), Point3d(23, 15, 0.75), Point3d(20, 18, 0), Point3d(20, 25, 0), Point3d(25, 20, 1.25), Point3d(25, 10, 1.25)});
  expectedRoofPolygons.push_back({Point3d(10, 25, 0), Point3d(10, 18, 0), Point3d(7, 15, 0.75), Point3d(10, 12, 0), Point3d(10, 5, 0), Point3d(5, 10, 1.25), Point3d(5, 20, 1.25)});
  expectedRoofPolygons.push_back({Point3d(0, 5, 0), Point3d(0, 25, 0), Point3d(5, 20, 1.25), Point3d(5, 10, 1.25)});
  expectedRoofPolygons.push_back({Point3d(10, 5, 0), Point3d(0, 5, 0), Point3d(5, 10, 1.25)});
  expectedRoofPolygons.push_back({Point3d(20, 12, 0), Point3d(10, 12, 0), Point3d(7, 15, 0.75), Point3d(23, 15, 0.75)});
  expectedRoofPolygons.push_back({Point3d(30, 5, 0), Point3d(20, 5, 0), Point3d(25, 10, 1.25)});
  expectedRoofPolygons.push_back({Point3d(30, 25, 0), Point3d(30, 5, 0), Point3d(25, 10, 1.25), Point3d(25, 20, 1.25)});
  expectedRoofPolygons.push_back({Point3d(20, 25, 0), Point3d(30, 25, 0), Point3d(25, 20, 1.25)});
  expectedRoofPolygons.push_back({Point3d(10, 18, 0), Point3d(20, 18, 0), Point3d(23, 15, 0.75), Point3d(7, 15, 0.75)});
  expectedRoofPolygons.push_back({Point3d(0, 25, 0), Point3d(10, 25, 0), Point3d(5, 20, 1.25)});

  EXPECT_EQ(roofPolygons.size(), expectedRoofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    bool test = polygonMatches(expectedRoofPolygons, roofPolygons[i], tol);
    EXPECT_TRUE(test);
  }
}

// FIXME: Add skeleton tests