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
#include "../RoofGeometry.hpp"
#include "../Intersection.hpp"
#include "../Point3d.hpp"
#include "GeometryFixture.hpp"
#include <math.h>

using namespace openstudio;

void matches(std::vector< std::vector<Point3d> > polygons, std::vector<Point3d> polygon)
{
  int numMatches = 0;
  for (int i = 0; i < polygons.size(); ++i) {
    if (circularEqual(polygons[i], polygon, 0.01)) {
      numMatches += 1;
    }
  }

  //if (numMatches != 1) {
  //  for (std::vector<Point3d> p : polygons) {
  //    std::cout << "EXPECTED POLYGON:" << std::endl;;
  //    for (Point3d p : p) {
  //      std::cout << p << std::endl;
  //    }
  //  }
  //  std::cout << "CALCULATED POLYGON:" << std::endl;
  //  for (Point3d p : polygon) {
  //    std::cout << p << std::endl;
  //  }
  //}

  EXPECT_EQ(numMatches, 1);
}

TEST_F(GeometryFixture, RoofSquare_6_12)
{
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(5.0, 25.0, 0.0));
  footprint.push_back(Point3d(25.0, 25.0, 0.0));
  footprint.push_back(Point3d(25.0, 5.0, 0.0));
  footprint.push_back(Point3d(5.0, 5.0, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(5.0, 5.0, 0.0), Point3d(25.0, 5.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(25.0, 5.0, 0.0), Point3d(25.0, 25.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(25.0, 25.0, 0.0), Point3d(5.0, 25.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.0, 25.0, 0.0), Point3d(5.0, 5.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 5.0), Point3d(25.0, 5.0, 0.0), Point3d(25.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 5.0), Point3d(25.0, 25.0, 0.0), Point3d(5.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 5.0), Point3d(5.0, 25.0, 0.0), Point3d(5.0, 5.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 5.0), Point3d(5.0, 5.0, 0.0), Point3d(25.0, 5.0, 0.0)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSquare_1_12) {
  double pitch = radToDeg(atan(1.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(5.0, 25.0, 0.0));
  footprint.push_back(Point3d(25.0, 25.0, 0.0));
  footprint.push_back(Point3d(25.0, 5.0, 0.0));
  footprint.push_back(Point3d(5.0, 5.0, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(5.0, 5.0, 0.0), Point3d(25.0, 5.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(25.0, 5.0, 0.0), Point3d(25.0, 25.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(25.0, 25.0, 0.0), Point3d(5.0, 25.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.0, 25.0, 0.0), Point3d(5.0, 5.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 0.0), Point3d(5.0, 25.0, 0.0), Point3d(15.0, 15.0, 0.833)});
  expectedRoofPolygons.push_back({Point3d(25.0, 5.0, 0.0), Point3d(5.0, 5.0, 0.0), Point3d(15.0, 15.0, 0.833)});
  expectedRoofPolygons.push_back({Point3d(25.0, 25.0, 0.0), Point3d(25.0, 5.0, 0.0), Point3d(15.0, 15.0, 0.833)});
  expectedRoofPolygons.push_back({Point3d(5.0, 25.0, 0.0), Point3d(25.0, 25.0, 0.0), Point3d(15.0, 15.0, 0.833)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofRectangle_3_12) {
  double pitch = radToDeg(atan(3.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(5.0, 25.0, 0.0));
  footprint.push_back(Point3d(40.0, 25.0, 0.0));
  footprint.push_back(Point3d(40.0, 5.0, 0.0));
  footprint.push_back(Point3d(5.0, 5.0, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(5.0, 5.0, 0.0), Point3d(40.0, 5.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(40.0, 5.0, 0.0), Point3d(40.0, 25.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(40.0, 25.0, 0.0), Point3d(5.0, 25.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.0, 25.0, 0.0), Point3d(5.0, 5.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(30.0, 15.0, 2.5), Point3d(40.0, 5.0, 0.0), Point3d(40.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(30.0, 15.0, 2.5), Point3d(40.0, 25.0, 0.0), Point3d(5.0, 25.0, 0.0), Point3d(15.0, 15.0, 2.5)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 2.5), Point3d(5.0, 25.0, 0.0), Point3d(5.0, 5.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 2.5), Point3d(5.0, 5.0, 0.0), Point3d(40.0, 5.0, 0.0), Point3d(30.0, 15.0, 2.5)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofHshape_6_12) {
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

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 5.0, 0.0), Point3d(10.0, 5.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(10.0, 5.0, 0.0), Point3d(10.0, 12.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(10.0, 12.0, 0.0), Point3d(20.0, 12.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(20.0, 12.0, 0.0), Point3d(20.0, 5.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(20.0, 5.0, 0.0), Point3d(30.0, 5.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(30.0, 5.0, 0.0), Point3d(30.0, 25.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(30.0, 25.0, 0.0), Point3d(20.0, 25.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(20.0, 25.0, 0.0), Point3d(20.0, 18.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(20.0, 18.0, 0.0), Point3d(10.0, 18.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(10.0, 18.0, 0.0), Point3d(10.0, 25.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(10.0, 25.0, 0.0), Point3d(0.0, 25.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(0.0, 25.0, 0.0), Point3d(0.0, 5.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

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

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTest6_9) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(119.0, 158.0, 0.0));
  footprint.push_back(Point3d(259.0, 159.0, 0.0));
  footprint.push_back(Point3d(248.0, 63.0, 0.0));
  footprint.push_back(Point3d(126.0, 60.0, 0.0));
  footprint.push_back(Point3d(90.0, 106.0, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(90.0, 106.0, 0.0), Point3d(126.0, 60.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(126.0, 60.0, 0.0), Point3d(248.0, 63.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(248.0, 63.0, 0.0), Point3d(259.0, 159.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(259.0, 159.0, 0.0), Point3d(119.0, 158.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(119.0, 158.0, 0.0), Point3d(90.0, 106.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(149.3227702994274, 109.40180634780967, 24.406769820569707), Point3d(126.0, 60.0, 0.0), Point3d(248.0, 63.0, 0.0), Point3d(204.77153555171768, 110.28151783497142, 24.16495155021603)});
  expectedRoofPolygons.push_back({Point3d(204.77153555171768, 110.28151783497142, 24.16495155021603), Point3d(248.0, 63.0, 0.0), Point3d(259.0, 159.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(149.3227702994274, 109.40180634780967, 24.406769820569707), Point3d(204.77153555171768, 110.28151783497142, 24.16495155021603), Point3d(259.0, 159.0, 0.0), Point3d(119.0, 158.0, 0.0), Point3d(147.1566715713749, 110.4476269750399, 23.876136976448187)});
  expectedRoofPolygons.push_back({Point3d(147.1566715713749, 110.4476269750399, 23.876136976448187), Point3d(119.0, 158.0, 0.0), Point3d(90.0, 106.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(149.3227702994274, 109.40180634780967, 24.406769820569707), Point3d(147.1566715713749, 110.4476269750399, 23.876136976448187), Point3d(90.0, 106.0, 0.0), Point3d(126.0, 60.0, 0.0)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTest7) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(0.0, -1.0, 0.0));
  footprint.push_back(Point3d(1.0, -1.0, 0.0));
  footprint.push_back(Point3d(1.0, 1.0, 0.0));
  footprint.push_back(Point3d(-1.0, 1.0, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(0.0, -1.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(0.0, -1.0, 0.0), Point3d(1.0, -1.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(1.0, -1.0, 0.0), Point3d(1.0, 1.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(1.0, 1.0, 0.0), Point3d(-1.0, 1.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(-1.0, 1.0, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.5, -0.5, 0.25), Point3d(0.0, -1.0, 0.0), Point3d(1.0, -1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.4142135623730951, 0.4142135623730951, 0.2928932188134524), Point3d(0.5, 0.20710678118654752, 0.25), Point3d(0.5, -0.5, 0.25), Point3d(1.0, -1.0, 0.0), Point3d(1.0, 1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.4142135623730951, 0.4142135623730951, 0.2928932188134524), Point3d(1.0, 1.0, 0.0), Point3d(-1.0, 1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.4142135623730951, 0.4142135623730951, 0.2928932188134524), Point3d(-1.0, 1.0, 0.0), Point3d(0.0, 0.0, 0.0), Point3d(0.5, 0.20710678118654752, 0.25)});
  expectedRoofPolygons.push_back({Point3d(0.5, 0.20710678118654752, 0.25), Point3d(0.0, 0.0, 0.0), Point3d(0.0, -1.0, 0.0), Point3d(0.5, -0.5, 0.25)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTest8) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(-1.0, 0.0, 0.0));
  footprint.push_back(Point3d(-1.2, -2.0, 0.0));
  footprint.push_back(Point3d(1.2, -2.0, 0.0));
  footprint.push_back(Point3d(1.0, 0.5, 0.0));
  footprint.push_back(Point3d(2.0, -0.2, 0.0));
  footprint.push_back(Point3d(2.0, 1.0, 0.0));
  footprint.push_back(Point3d(-2.0, 1.2, 0.0));
  footprint.push_back(Point3d(-2.0, -0.2, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(-1.0, 0.0, 0.0), Point3d(-1.2, -2.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(-1.2, -2.0, 0.0), Point3d(1.2, -2.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(1.2, -2.0, 0.0), Point3d(1.0, 0.5, 0.0)});
  expectedRoofEdges.push_back({Point3d(1.0, 0.5, 0.0), Point3d(2.0, -0.2, 0.0)});
  expectedRoofEdges.push_back({Point3d(2.0, -0.2, 0.0), Point3d(2.0, 1.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(2.0, 1.0, 0.0), Point3d(-2.0, 1.2, 0.0)});
  expectedRoofEdges.push_back({Point3d(-2.0, 1.2, 0.0), Point3d(-2.0, -0.2, 0.0)});
  expectedRoofEdges.push_back({Point3d(-2.0, -0.2, 0.0), Point3d(-1.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.011951105288885877, -0.90319932382556, 0.54840033808722), Point3d(-1.2, -2.0, 0.0), Point3d(1.2, -2.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.011951105288885877, -0.90319932382556, 0.54840033808722), Point3d(1.2, -2.0, 0.0), Point3d(1.0, 0.5, 0.0), Point3d(0.7847636613911987, 0.8759615123236527, 0.09228486822568349), Point3d(0.02180193069397529, 0.08986159383373349, 0.5038946796426319)});
  expectedRoofPolygons.push_back({Point3d(1.582158848784161, 0.6025289309369212, 0.20892057560791943), Point3d(0.7847636613911987, 0.8759615123236527, 0.09228486822568349), Point3d(1.0, 0.5, 0.0), Point3d(2.0, -0.2, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.582158848784161, 0.6025289309369212, 0.20892057560791943), Point3d(2.0, -0.2, 0.0), Point3d(2.0, 1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.02180193069397529, 0.08986159383373349, 0.5038946796426319), Point3d(0.7847636613911987, 0.8759615123236527, 0.09228486822568349), Point3d(1.582158848784161, 0.6025289309369212, 0.20892057560791943), Point3d(2.0, 1.0, 0.0), Point3d(-2.0, 1.2, 0.0), Point3d(-1.3835459038079945, 0.5519531123818496, 0.30822704809600276), Point3d(-0.43606459624258886, 0.6219268927095002, 0.24962633041547042)});
  expectedRoofPolygons.push_back({Point3d(-1.3835459038079945, 0.5519531123818496, 0.30822704809600276), Point3d(-2.0, 1.2, 0.0), Point3d(-2.0, -0.2, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-1.3835459038079945, 0.5519531123818496, 0.30822704809600276), Point3d(-2.0, -0.2, 0.0), Point3d(-1.0, 0.0, 0.0), Point3d(-0.43606459624258886, 0.6219268927095002, 0.24962633041547042)});
  expectedRoofPolygons.push_back({Point3d(0.011951105288885877, -0.90319932382556, 0.54840033808722), Point3d(0.02180193069397529, 0.08986159383373349, 0.5038946796426319), Point3d(-0.43606459624258886, 0.6219268927095002, 0.24962633041547042), Point3d(-1.0, 0.0, 0.0), Point3d(-1.2, -2.0, 0.0)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB1) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(0.7904833761575505, 8.520486967634694, 0.0));
  footprint.push_back(Point3d(5.978418789681697, 8.712497973454056, 0.0));
  footprint.push_back(Point3d(5.95269105167549, -2.6355979260267777, 0.0));
  footprint.push_back(Point3d(4.566910029680516, -2.6324561649763485, 0.0));
  footprint.push_back(Point3d(4.5603585630377115, -5.522203838861205, 0.0));
  footprint.push_back(Point3d(6.043569207647302, -5.525566487736131, 0.0));
  footprint.push_back(Point3d(6.038049999411376, -7.960001358506733, 0.0));
  footprint.push_back(Point3d(9.886846028372108, -7.968727126586532, 0.0));
  footprint.push_back(Point3d(9.902081573281308, -1.248570683335708, 0.0));
  footprint.push_back(Point3d(13.742215004880482, -1.2572768087753285, 0.0));
  footprint.push_back(Point3d(13.75400717659087, 3.9440624000165103, 0.0));
  footprint.push_back(Point3d(9.194585721152315, 3.9543992526769878, 0.0));
  footprint.push_back(Point3d(5.823717342947504, 17.30434988614582, 0.0));
  footprint.push_back(Point3d(5.808494957384097, 10.589997844496661, 0.0));
  footprint.push_back(Point3d(-0.13214359029800526, 10.603466113057067, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(0.7904833761575505, 8.520486967634694, 0.0)});
  expectedRoofEdges.push_back({Point3d(0.7904833761575505, 8.520486967634694, 0.0), Point3d(5.978418789681697, 8.712497973454056, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.978418789681697, 8.712497973454056, 0.0), Point3d(5.95269105167549, -2.6355979260267777, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.95269105167549, -2.6355979260267777, 0.0), Point3d(4.566910029680516, -2.6324561649763485, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.566910029680516, -2.6324561649763485, 0.0), Point3d(4.5603585630377115, -5.522203838861205, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.5603585630377115, -5.522203838861205, 0.0), Point3d(6.043569207647302, -5.525566487736131, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.043569207647302, -5.525566487736131, 0.0), Point3d(6.038049999411376, -7.960001358506733, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.038049999411376, -7.960001358506733, 0.0), Point3d(9.886846028372108, -7.968727126586532, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.886846028372108, -7.968727126586532, 0.0), Point3d(9.902081573281308, -1.248570683335708, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.902081573281308, -1.248570683335708, 0.0), Point3d(13.742215004880482, -1.2572768087753285, 0.0)});
  expectedRoofEdges.push_back({Point3d(13.742215004880482, -1.2572768087753285, 0.0), Point3d(13.75400717659087, 3.9440624000165103, 0.0)});
  expectedRoofEdges.push_back({Point3d(13.75400717659087, 3.9440624000165103, 0.0), Point3d(9.194585721152315, 3.9543992526769878, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.194585721152315, 3.9543992526769878, 0.0), Point3d(5.823717342947504, 17.30434988614582, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.823717342947504, 17.30434988614582, 0.0), Point3d(5.808494957384097, 10.589997844496661, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.808494957384097, 10.589997844496661, 0.0), Point3d(-0.13214359029800526, 10.603466113057067, 0.0)});
  expectedRoofEdges.push_back({Point3d(-0.13214359029800526, 10.603466113057067, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.9184708676614719, 9.563507938168502, 0.5187868083298253), Point3d(0.3594527136710186, 8.976136170014422, 0.23563971612419035), Point3d(0.7904833761575505, 8.520486967634694, 0.0), Point3d(5.978418789681697, 8.712497973454056, 0.0), Point3d(6.8580714064192705, 9.623241290520603, 0.4387927887494017), Point3d(6.760641991208537, 9.69627296507374, 0.47708539116320264), Point3d(6.729880870203986, 9.664424613978154, 0.4617409724533214)});
  expectedRoofPolygons.push_back({Point3d(8.562421611757856, 1.3551494866221112, 1.3003381438870851), Point3d(7.779411083575811, 2.14171847341904, 0.907942256682172), Point3d(6.8580714064192705, 9.623241290520603, 0.4387927887494017), Point3d(5.978418789681697, 8.712497973454056, 0.0), Point3d(5.95269105167549, -2.6355979260267777, 0.0), Point3d(7.394289156400893, -4.083747496844417, 0.7224387756441801), Point3d(7.923725736553589, -3.55670610186642, 0.9865589482591836), Point3d(7.933442073357339, 0.7290154681198144, 0.9865589482644922)});
  expectedRoofPolygons.push_back({Point3d(7.394289156400893, -4.083747496844417, 0.7224387756441801), Point3d(5.95269105167549, -2.6355979260267777, 0.0), Point3d(4.566910029680516, -2.6324561649763485, 0.0), Point3d(6.008508133850388, -4.080605735266273, 0.722438775380953)});
  expectedRoofPolygons.push_back({Point3d(6.008508133850388, -4.080605735266273, 0.722438775380953), Point3d(4.566910029680516, -2.6324561649763485, 0.0), Point3d(4.5603585630377115, -5.522203838861205, 0.0)});
  expectedRoofPolygons.push_back({Point3d(7.923725736553589, -3.55670610186642, 0.9865589482591836), Point3d(7.394289156400893, -4.083747496844417, 0.7224387756441801), Point3d(6.008508133850388, -4.080605735266273, 0.722438775380953), Point3d(4.5603585630377115, -5.522203838861205, 0.0), Point3d(6.043569207647302, -5.525566487736131, 0.0), Point3d(7.972330104528959, -3.6055313565934526, 0.9622014799910014)});
  expectedRoofPolygons.push_back({Point3d(7.972330104528959, -3.6055313565934526, 0.9622014799910014), Point3d(6.043569207647302, -5.525566487736131, 0.0), Point3d(6.038049999411376, -7.960001358506733, 0.0), Point3d(7.966810896364689, -6.039966228120712, 0.9622014800276865)});
  expectedRoofPolygons.push_back({Point3d(7.966810896364689, -6.039966228120712, 0.9622014800276865), Point3d(6.038049999411376, -7.960001358506733, 0.0), Point3d(9.886846028372108, -7.968727126586532, 0.0)});
  expectedRoofPolygons.push_back({Point3d(7.923725736553589, -3.55670610186642, 0.9865589482591836), Point3d(7.972330104528959, -3.6055313565934526, 0.9622014799910014), Point3d(7.966810896364689, -6.039966228120712, 0.9622014800276865), Point3d(9.886846028372108, -7.968727126586532, 0.0), Point3d(9.902081573281308, -1.248570683335708, 0.0), Point3d(7.933442073357339, 0.7290154681198144, 0.9865589482644922)});
  expectedRoofPolygons.push_back({Point3d(11.147441486461226, 1.3492888811957957, 1.3003381439477693), Point3d(8.562421611757856, 1.3551494866221112, 1.3003381438870851), Point3d(7.933442073357339, 0.7290154681198144, 0.9865589482644922), Point3d(9.902081573281308, -1.248570683335708, 0.0), Point3d(13.742215004880482, -1.2572768087753285, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.147441486461226, 1.3492888811957957, 1.3003381439477693), Point3d(13.742215004880482, -1.2572768087753285, 0.0), Point3d(13.75400717659087, 3.9440624000165103, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.147441486461226, 1.3492888811957957, 1.3003381439477693), Point3d(13.75400717659087, 3.9440624000165103, 0.0), Point3d(9.194585721152315, 3.9543992526769878, 0.0), Point3d(7.779411083575811, 2.14171847341904, 0.907942256682172), Point3d(8.562421611757856, 1.3551494866221112, 1.3003381438870851)});
  expectedRoofPolygons.push_back({Point3d(7.779411083575811, 2.14171847341904, 0.907942256682172), Point3d(9.194585721152315, 3.9543992526769878, 0.0), Point3d(5.823717342947504, 17.30434988614582, 0.0), Point3d(6.760641991208537, 9.69627296507374, 0.47708539116320264), Point3d(6.8580714064192705, 9.623241290520603, 0.4387927887494017)});
  expectedRoofPolygons.push_back({Point3d(6.760641991208537, 9.69627296507374, 0.47708539116320264), Point3d(5.823717342947504, 17.30434988614582, 0.0), Point3d(5.808494957384097, 10.589997844496661, 0.0), Point3d(6.729880870203986, 9.664424613978154, 0.4617409724533214)});
  expectedRoofPolygons.push_back({Point3d(0.9184708676614719, 9.563507938168502, 0.5187868083298253), Point3d(6.729880870203986, 9.664424613978154, 0.4617409724533214), Point3d(5.808494957384097, 10.589997844496661, 0.0), Point3d(-0.13214359029800526, 10.603466113057067, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.9184708676614719, 9.563507938168502, 0.5187868083298253), Point3d(-0.13214359029800526, 10.603466113057067, 0.0), Point3d(0.0, 0.0, 0.0), Point3d(0.3594527136710186, 8.976136170014422, 0.23563971612419035)});
  expectedRoofPolygons.push_back({Point3d(0.3594527136710186, 8.976136170014422, 0.23563971612419035), Point3d(0.0, 0.0, 0.0), Point3d(0.7904833761575505, 8.520486967634694, 0.0)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB2) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(0.7904833761549828, 8.520486967607015, 0.0));
  footprint.push_back(Point3d(5.9784187896622765, 8.712497973425755, 0.0));
  footprint.push_back(Point3d(5.952691051656153, -2.6355979260182156, 0.0));
  footprint.push_back(Point3d(4.56691002966568, -2.632456164967797, 0.0));
  footprint.push_back(Point3d(4.560358563022897, -5.522203838843264, 0.0));
  footprint.push_back(Point3d(6.0435692076276695, -5.525566487718182, 0.0));
  footprint.push_back(Point3d(6.038049999391761, -7.960001358480875, 0.0));
  footprint.push_back(Point3d(9.886846028339992, -7.968727126560646, 0.0));
  footprint.push_back(Point3d(9.902081573249141, -1.2485706833316517, 0.0));
  footprint.push_back(Point3d(13.74221500483584, -1.2572768087712447, 0.0));
  footprint.push_back(Point3d(13.754007176546189, 3.944062400003698, 0.0));
  footprint.push_back(Point3d(9.194585721122445, 3.9543992526641416, 0.0));
  footprint.push_back(Point3d(9.840828592998651, 10.391220834155359, 0.0));
  footprint.push_back(Point3d(-0.24573045314637643, 10.433085818392197, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(0.7904833761549828, 8.520486967607015, 0.0)});
  expectedRoofEdges.push_back({Point3d(0.7904833761549828, 8.520486967607015, 0.0), Point3d(5.9784187896622765, 8.712497973425755, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.9784187896622765, 8.712497973425755, 0.0), Point3d(5.952691051656153, -2.6355979260182156, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.952691051656153, -2.6355979260182156, 0.0), Point3d(4.56691002966568, -2.632456164967797, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.56691002966568, -2.632456164967797, 0.0), Point3d(4.560358563022897, -5.522203838843264, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.560358563022897, -5.522203838843264, 0.0), Point3d(6.0435692076276695, -5.525566487718182, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.0435692076276695, -5.525566487718182, 0.0), Point3d(6.038049999391761, -7.960001358480875, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.038049999391761, -7.960001358480875, 0.0), Point3d(9.886846028339992, -7.968727126560646, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.886846028339992, -7.968727126560646, 0.0), Point3d(9.902081573249141, -1.2485706833316517, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.902081573249141, -1.2485706833316517, 0.0), Point3d(13.74221500483584, -1.2572768087712447, 0.0)});
  expectedRoofEdges.push_back({Point3d(13.74221500483584, -1.2572768087712447, 0.0), Point3d(13.754007176546189, 3.944062400003698, 0.0)});
  expectedRoofEdges.push_back({Point3d(13.754007176546189, 3.944062400003698, 0.0), Point3d(9.194585721122445, 3.9543992526641416, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.194585721122445, 3.9543992526641416, 0.0), Point3d(9.840828592998651, 10.391220834155359, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.840828592998651, 10.391220834155359, 0.0), Point3d(-0.24573045314637643, 10.433085818392197, 0.0)});
  expectedRoofEdges.push_back({Point3d(-0.24573045314637643, 10.433085818392197, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.7321423860140842, 9.474000222038036, 0.47750931955493864), Point3d(0.3113773466011168, 9.026957386584131, 0.2619219898328897), Point3d(0.7904833761549828, 8.520486967607015, 0.0), Point3d(5.9784187896622765, 8.712497973425755, 0.0), Point3d(6.810341355495798, 9.573824254656804, 0.41498384218932366)});
  expectedRoofPolygons.push_back({Point3d(8.562421611730041, 1.3551494866177085, 1.3003381438828603), Point3d(7.498680114283213, 2.4237252486872443, 0.7672574589826884), Point3d(7.813149324955553, 8.56455983950241, 0.9175306079619708), Point3d(6.810341355495798, 9.573824254656804, 0.41498384218932366), Point3d(5.9784187896622765, 8.712497973425755, 0.0), Point3d(5.952691051656153, -2.6355979260182156, 0.0), Point3d(7.394289156376876, -4.083747496831154, 0.7224387756418331), Point3d(7.9237257365278495, -3.5567061018548705, 0.9865589482559782), Point3d(7.933442073331568, 0.7290154681174473, 0.9865589482612875)});
  expectedRoofPolygons.push_back({Point3d(7.394289156376876, -4.083747496831154, 0.7224387756418331), Point3d(5.952691051656153, -2.6355979260182156, 0.0), Point3d(4.56691002966568, -2.632456164967797, 0.0), Point3d(6.008508133830869, -4.080605735253016, 0.7224387753786058)});
  expectedRoofPolygons.push_back({Point3d(6.008508133830869, -4.080605735253016, 0.7224387753786058), Point3d(4.56691002966568, -2.632456164967797, 0.0), Point3d(4.560358563022897, -5.522203838843264, 0.0)});
  expectedRoofPolygons.push_back({Point3d(7.9237257365278495, -3.5567061018548705, 0.9865589482559782), Point3d(7.394289156376876, -4.083747496831154, 0.7224387756418331), Point3d(6.008508133830869, -4.080605735253016, 0.7224387753786058), Point3d(4.560358563022897, -5.522203838843264, 0.0), Point3d(6.0435692076276695, -5.525566487718182, 0.0), Point3d(7.972330104503062, -3.6055313565817433, 0.9622014799878755)});
  expectedRoofPolygons.push_back({Point3d(7.972330104503062, -3.6055313565817433, 0.9622014799878755), Point3d(6.0435692076276695, -5.525566487718182, 0.0), Point3d(6.038049999391761, -7.960001358480875, 0.0), Point3d(7.966810896338812, -6.039966228101093, 0.962201480024562)});
  expectedRoofPolygons.push_back({Point3d(7.966810896338812, -6.039966228101093, 0.962201480024562), Point3d(6.038049999391761, -7.960001358480875, 0.0), Point3d(9.886846028339992, -7.968727126560646, 0.0)});
  expectedRoofPolygons.push_back({Point3d(7.9237257365278495, -3.5567061018548705, 0.9865589482559782), Point3d(7.972330104503062, -3.6055313565817433, 0.9622014799878755), Point3d(7.966810896338812, -6.039966228101093, 0.962201480024562), Point3d(9.886846028339992, -7.968727126560646, 0.0), Point3d(9.902081573249141, -1.2485706833316517, 0.0), Point3d(7.933442073331568, 0.7290154681174473, 0.9865589482612875)});
  expectedRoofPolygons.push_back({Point3d(11.147441486425013, 1.3492888811914148, 1.3003381439435455), Point3d(8.562421611730041, 1.3551494866177085, 1.3003381438828603), Point3d(7.933442073331568, 0.7290154681174473, 0.9865589482612875), Point3d(9.902081573249141, -1.2485706833316517, 0.0), Point3d(13.74221500483584, -1.2572768087712447, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.147441486425013, 1.3492888811914148, 1.3003381439435455), Point3d(13.74221500483584, -1.2572768087712447, 0.0), Point3d(13.754007176546189, 3.944062400003698, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.147441486425013, 1.3492888811914148, 1.3003381439435455), Point3d(13.754007176546189, 3.944062400003698, 0.0), Point3d(9.194585721122445, 3.9543992526641416, 0.0), Point3d(7.498680114283213, 2.4237252486872443, 0.7672574589826884), Point3d(8.562421611730041, 1.3551494866177085, 1.3003381438828603)});
  expectedRoofPolygons.push_back({Point3d(7.813149324955553, 8.56455983950241, 0.9175306079619708), Point3d(7.498680114283213, 2.4237252486872443, 0.7672574589826884), Point3d(9.194585721122445, 3.9543992526641416, 0.0), Point3d(9.840828592998651, 10.391220834155359, 0.0)});
  expectedRoofPolygons.push_back({Point3d(7.813149324955553, 8.56455983950241, 0.9175306079619708), Point3d(9.840828592998651, 10.391220834155359, 0.0), Point3d(-0.24573045314637643, 10.433085818392197, 0.0), Point3d(0.7321423860140842, 9.474000222038036, 0.47750931955493864), Point3d(6.810341355495798, 9.573824254656804, 0.41498384218932366)});
  expectedRoofPolygons.push_back({Point3d(0.7321423860140842, 9.474000222038036, 0.47750931955493864), Point3d(-0.24573045314637643, 10.433085818392197, 0.0), Point3d(0.0, 0.0, 0.0), Point3d(0.3113773466011168, 9.026957386584131, 0.2619219898328897)});
  expectedRoofPolygons.push_back({Point3d(0.3113773466011168, 9.026957386584131, 0.2619219898328897), Point3d(0.0, 0.0, 0.0), Point3d(0.7904833761549828, 8.520486967607015, 0.0)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB3) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(0.0853589477356087, -5.32440343246266, 0.0));
  footprint.push_back(Point3d(3.934154976683839, -5.33312920054243, 0.0));
  footprint.push_back(Point3d(3.9493905215929885, 1.387027242686564, 0.0));
  footprint.push_back(Point3d(7.789523953179687, 1.378321117246971, 0.0));
  footprint.push_back(Point3d(3.2418946694662925, 6.589997178682357, 0.0));
  footprint.push_back(Point3d(-0.4480081827933864, 6.565094698194268, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(0.0853589477356087, -5.32440343246266, 0.0)});
  expectedRoofEdges.push_back({Point3d(0.0853589477356087, -5.32440343246266, 0.0), Point3d(3.934154976683839, -5.33312920054243, 0.0)});
  expectedRoofEdges.push_back({Point3d(3.934154976683839, -5.33312920054243, 0.0), Point3d(3.9493905215929885, 1.387027242686564, 0.0)});
  expectedRoofEdges.push_back({Point3d(3.9493905215929885, 1.387027242686564, 0.0), Point3d(7.789523953179687, 1.378321117246971, 0.0)});
  expectedRoofEdges.push_back({Point3d(7.789523953179687, 1.378321117246971, 0.0), Point3d(3.2418946694662925, 6.589997178682357, 0.0)});
  expectedRoofEdges.push_back({Point3d(3.2418946694662925, 6.589997178682357, 0.0), Point3d(-0.4480081827933864, 6.565094698194268, 0.0)});
  expectedRoofEdges.push_back({Point3d(-0.4480081827933864, 6.565094698194268, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.996553519848494, -3.386722145559448, 0.9710046229839351), Point3d(0.0853589477356087, -5.32440343246266, 0.0), Point3d(3.934154976683839, -5.33312920054243, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.860584700471444, 3.4853258215219385, 1.0467787901715908), Point3d(1.9726755710182728, 0.08306472324690238, 0.9868768053436059), Point3d(1.996553519848494, -3.386722145559448, 0.9710046229839351), Point3d(3.934154976683839, -5.33312920054243, 0.0), Point3d(3.9493905215929885, 1.387027242686564, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.25187865095761, 3.9032812980229954, 1.2561995495738625), Point3d(1.860584700471444, 3.4853258215219385, 1.0467787901715908), Point3d(3.9493905215929885, 1.387027242686564, 0.0), Point3d(7.789523953179687, 1.378321117246971, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.25187865095761, 3.9032812980229954, 1.2561995495738625), Point3d(7.789523953179687, 1.378321117246971, 0.0), Point3d(3.2418946694662925, 6.589997178682357, 0.0), Point3d(2.1462783279814595, 4.158151806596191, 1.2121980233450986)});
  expectedRoofPolygons.push_back({Point3d(2.1462783279814595, 4.158151806596191, 1.2121980233450986), Point3d(3.2418946694662925, 6.589997178682357, 0.0), Point3d(-0.4480081827933864, 6.565094698194268, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.25187865095761, 3.9032812980229954, 1.2561995495738625), Point3d(2.1462783279814595, 4.158151806596191, 1.2121980233450986), Point3d(-0.4480081827933864, 6.565094698194268, 0.0), Point3d(0.0, 0.0, 0.0), Point3d(1.9726755710182728, 0.08306472324690238, 0.9868768053436059), Point3d(1.860584700471444, 3.4853258215219385, 1.0467787901715908)});
  expectedRoofPolygons.push_back({Point3d(1.9726755710182728, 0.08306472324690238, 0.9868768053436059), Point3d(0.0, 0.0, 0.0), Point3d(0.0853589477356087, -5.32440343246266, 0.0), Point3d(1.996553519848494, -3.386722145559448, 0.9710046229839351)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB4) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(-1.192493260706565, -5.6367673060470285, 0.0));
  footprint.push_back(Point3d(2.656302768241665, -5.645493074126799, 0.0));
  footprint.push_back(Point3d(6.511671744737513, 1.0659572436626021, 0.0));
  footprint.push_back(Point3d(-1.7258603912355601, 6.252730824609899, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(-1.192493260706565, -5.6367673060470285, 0.0)});
  expectedRoofEdges.push_back({Point3d(-1.192493260706565, -5.6367673060470285, 0.0), Point3d(2.656302768241665, -5.645493074126799, 0.0)});
  expectedRoofEdges.push_back({Point3d(2.656302768241665, -5.645493074126799, 0.0), Point3d(6.511671744737513, 1.0659572436626021, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.511671744737513, 1.0659572436626021, 0.0), Point3d(-1.7258603912355601, 6.252730824609899, 0.0)});
  expectedRoofEdges.push_back({Point3d(-1.7258603912355601, 6.252730824609899, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.4279119335787651, -3.517645076047145, 1.0625288000004602), Point3d(-1.192493260706565, -5.6367673060470285, 0.0), Point3d(2.656302768241665, -5.645493074126799, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.812172737637799, 0.14602644606810766, 1.3748294820465696), Point3d(2.804480151931106, 0.08532357949119695, 1.3630462913156411), Point3d(1.4279119335787651, -3.517645076047145, 1.0625288000004602), Point3d(2.656302768241665, -5.645493074126799, 0.0), Point3d(6.511671744737513, 1.0659572436626021, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.812172737637799, 0.14602644606810766, 1.3748294820465696), Point3d(6.511671744737513, 1.0659572436626021, 0.0), Point3d(-1.7258603912355601, 6.252730824609899, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.812172737637799, 0.14602644606810766, 1.3748294820465696), Point3d(-1.7258603912355601, 6.252730824609899, 0.0), Point3d(0.0, 0.0, 0.0), Point3d(2.804480151931106, 0.08532357949119695, 1.3630462913156411)});
  expectedRoofPolygons.push_back({Point3d(2.804480151931106, 0.08532357949119695, 1.3630462913156411), Point3d(0.0, 0.0, 0.0), Point3d(-1.192493260706565, -5.6367673060470285, 0.0), Point3d(1.4279119335787651, -3.517645076047145, 1.0625288000004602)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB5) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(-1.192493260706565, -5.6367673060470285, 0.0));
  footprint.push_back(Point3d(2.656302768241665, -5.645493074126799, 0.0));
  footprint.push_back(Point3d(7.051209343876594, 2.9401404828825903, 0.0));
  footprint.push_back(Point3d(-1.7258603912355601, 6.252730824609899, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(-1.192493260706565, -5.6367673060470285, 0.0)});
  expectedRoofEdges.push_back({Point3d(-1.192493260706565, -5.6367673060470285, 0.0), Point3d(2.656302768241665, -5.645493074126799, 0.0)});
  expectedRoofEdges.push_back({Point3d(2.656302768241665, -5.645493074126799, 0.0), Point3d(7.051209343876594, 2.9401404828825903, 0.0)});
  expectedRoofEdges.push_back({Point3d(7.051209343876594, 2.9401404828825903, 0.0), Point3d(-1.7258603912355601, 6.252730824609899, 0.0)});
  expectedRoofEdges.push_back({Point3d(-1.7258603912355601, 6.252730824609899, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.381369491592734, -3.555283958639109, 1.0436566480369565), Point3d(-1.192493260706565, -5.6367673060470285, 0.0), Point3d(2.656302768241665, -5.645493074126799, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.795364974222883, 1.2972941167706578, 1.5198861728400568), Point3d(2.6710186548727584, 0.08126314331894204, 1.2981807231020268), Point3d(1.381369491592734, -3.555283958639109, 1.0436566480369565), Point3d(2.656302768241665, -5.645493074126799, 0.0), Point3d(7.051209343876594, 2.9401404828825903, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.795364974222883, 1.2972941167706578, 1.5198861728400568), Point3d(7.051209343876594, 2.9401404828825903, 0.0), Point3d(-1.7258603912355601, 6.252730824609899, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.795364974222883, 1.2972941167706578, 1.5198861728400568), Point3d(-1.7258603912355601, 6.252730824609899, 0.0), Point3d(0.0, 0.0, 0.0), Point3d(2.6710186548727584, 0.08126314331894204, 1.2981807231020268)});
  expectedRoofPolygons.push_back({Point3d(2.6710186548727584, 0.08126314331894204, 1.2981807231020268), Point3d(0.0, 0.0, 0.0), Point3d(-1.192493260706565, -5.6367673060470285, 0.0), Point3d(1.381369491592734, -3.555283958639109, 1.0436566480369565)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB6) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(-1.192493260706565, -5.636767306047028, 0.0));
  footprint.push_back(Point3d(2.656302768241665, -5.645493074126798, 0.0));
  footprint.push_back(Point3d(5.716563703938576, 6.120572646649897, 0.0));
  footprint.push_back(Point3d(-5.985367752852362, 6.423111118668768, 0.0));
  footprint.push_back(Point3d(-6.297731626436729, -3.6293262553813097, 0.0));
  footprint.push_back(Point3d(-3.4580600517873807, 1.3968924313579514, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(-1.192493260706565, -5.636767306047028, 0.0)});
  expectedRoofEdges.push_back({Point3d(-1.192493260706565, -5.636767306047028, 0.0), Point3d(2.656302768241665, -5.645493074126798, 0.0)});
  expectedRoofEdges.push_back({Point3d(2.656302768241665, -5.645493074126798, 0.0), Point3d(5.716563703938576, 6.120572646649897, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.716563703938576, 6.120572646649897, 0.0), Point3d(-5.985367752852362, 6.423111118668768, 0.0)});
  expectedRoofEdges.push_back({Point3d(-5.985367752852362, 6.423111118668768, 0.0), Point3d(-6.297731626436729, -3.6293262553813097, 0.0)});
  expectedRoofEdges.push_back({Point3d(-6.297731626436729, -3.6293262553813097, 0.0), Point3d(-3.4580600517873807, 1.3968924313579514, 0.0)});
  expectedRoofEdges.push_back({Point3d(-3.4580600517873807, 1.3968924313579514, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.1735930214222696, -3.723312833627361, 0.9594068976548346), Point3d(-1.192493260706565, -5.636767306047028, 0.0), Point3d(2.656302768241665, -5.645493074126798, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4934603449825767, 2.941709497335058, 1.6434738733770782), Point3d(2.3454435517151353, 1.2486300643020334, 1.018110122952239), Point3d(1.1735930214222696, -3.723312833627361, 0.9594068976548346), Point3d(2.656302768241665, -5.645493074126798, 0.0), Point3d(5.716563703938576, 6.120572646649897, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4934603449825767, 2.941709497335058, 1.6434738733770782), Point3d(5.716563703938576, 6.120572646649897, 0.0), Point3d(-5.985367752852362, 6.423111118668768, 0.0), Point3d(-3.7200362861701817, 4.025043505214006, 1.1693594394280284)});
  expectedRoofPolygons.push_back({Point3d(-3.7200362861701817, 4.025043505214006, 1.1693594394280284), Point3d(-5.985367752852362, 6.423111118668768, 0.0), Point3d(-6.297731626436729, -3.6293262553813097, 0.0), Point3d(-4.254892790432256, 3.6762164828487554, 0.9074772170550292)});
  expectedRoofPolygons.push_back({Point3d(-4.254892790432256, 3.6762164828487554, 0.9074772170550292), Point3d(-6.297731626436729, -3.6293262553813097, 0.0), Point3d(-3.4580600517873807, 1.3968924313579514, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4934603449825767, 2.941709497335058, 1.6434738733770782), Point3d(-3.7200362861701817, 4.025043505214006, 1.1693594394280284), Point3d(-4.254892790432256, 3.6762164828487554, 0.9074772170550292), Point3d(-3.4580600517873807, 1.3968924313579514, 0.0), Point3d(0.0, 0.0, 0.0), Point3d(2.3454435517151353, 1.2486300643020334, 1.018110122952239)});
  expectedRoofPolygons.push_back({Point3d(2.3454435517151353, 1.2486300643020334, 1.018110122952239), Point3d(0.0, 0.0, 0.0), Point3d(-1.192493260706565, -5.636767306047028, 0.0), Point3d(1.1735930214222696, -3.723312833627361, 0.9594068976548346)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB7) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(-1.1889778921584675, -7.356451670462243, 0.0));
  footprint.push_back(Point3d(5.7257149714503175, -12.035132476438635, 0.0));
  footprint.push_back(Point3d(11.739705976732338, -17.194940549920428, 0.0));
  footprint.push_back(Point3d(0.8357970425329011, -1.0288592710693223, 0.0));
  footprint.push_back(Point3d(7.360455718922119, -6.229013606285628, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(-1.1889778921584675, -7.356451670462243, 0.0)});
  expectedRoofEdges.push_back({Point3d(-1.1889778921584675, -7.356451670462243, 0.0), Point3d(5.7257149714503175, -12.035132476438635, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.7257149714503175, -12.035132476438635, 0.0), Point3d(11.739705976732338, -17.194940549920428, 0.0)});
  expectedRoofEdges.push_back({Point3d(11.739705976732338, -17.194940549920428, 0.0), Point3d(0.8357970425329011, -1.0288592710693223, 0.0)});
  expectedRoofEdges.push_back({Point3d(0.8357970425329011, -1.0288592710693223, 0.0), Point3d(7.360455718922119, -6.229013606285628, 0.0)});
  expectedRoofEdges.push_back({Point3d(7.360455718922119, -6.229013606285628, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955), Point3d(-1.1889778921584675, -7.356451670462243, 0.0), Point3d(5.7257149714503175, -12.035132476438635, 0.0), Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592)});
  expectedRoofPolygons.push_back({Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592), Point3d(5.7257149714503175, -12.035132476438635, 0.0), Point3d(11.739705976732338, -17.194940549920428, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955), Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592), Point3d(11.739705976732338, -17.194940549920428, 0.0), Point3d(0.8357970425329011, -1.0288592710693223, 0.0), Point3d(0.2284312718843465, -0.37117635227923845, 0.06788342500341242), Point3d(0.159929259942705, -0.4325952358032944, 0.11345123056840964)});
  expectedRoofPolygons.push_back({Point3d(0.2284312718843465, -0.37117635227923845, 0.06788342500341242), Point3d(0.8357970425329011, -1.0288592710693223, 0.0), Point3d(7.360455718922119, -6.229013606285628, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.159929259942705, -0.4325952358032944, 0.11345123056840964), Point3d(0.2284312718843465, -0.37117635227923845, 0.06788342500341242), Point3d(7.360455718922119, -6.229013606285628, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955), Point3d(0.159929259942705, -0.4325952358032944, 0.11345123056840964), Point3d(0.0, 0.0, 0.0), Point3d(-1.1889778921584675, -7.356451670462243, 0.0), Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB8) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(-1.1889778921584675, -7.356451670462243, 0.0));
  footprint.push_back(Point3d(5.7257149714503175, -12.035132476438635, 0.0));
  footprint.push_back(Point3d(11.739705976732338, -17.194940549920428, 0.0));
  footprint.push_back(Point3d(0.8357970425329011, -1.0288592710693223, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(-1.1889778921584675, -7.356451670462243, 0.0)});
  expectedRoofEdges.push_back({Point3d(-1.1889778921584675, -7.356451670462243, 0.0), Point3d(5.7257149714503175, -12.035132476438635, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.7257149714503175, -12.035132476438635, 0.0), Point3d(11.739705976732338, -17.194940549920428, 0.0)});
  expectedRoofEdges.push_back({Point3d(11.739705976732338, -17.194940549920428, 0.0), Point3d(0.8357970425329011, -1.0288592710693223, 0.0)});
  expectedRoofEdges.push_back({Point3d(0.8357970425329011, -1.0288592710693223, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955), Point3d(-1.1889778921584675, -7.356451670462243, 0.0), Point3d(5.7257149714503175, -12.035132476438635, 0.0), Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592)});
  expectedRoofPolygons.push_back({Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592), Point3d(5.7257149714503175, -12.035132476438635, 0.0), Point3d(11.739705976732338, -17.194940549920428, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955), Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592), Point3d(11.739705976732338, -17.194940549920428, 0.0), Point3d(0.8357970425329011, -1.0288592710693223, 0.0), Point3d(0.3674963650297451, -1.375941535615892, 0.29116226024306735)});
  expectedRoofPolygons.push_back({Point3d(0.3674963650297451, -1.375941535615892, 0.29116226024306735), Point3d(0.8357970425329011, -1.0288592710693223, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955), Point3d(0.3674963650297451, -1.375941535615892, 0.29116226024306735), Point3d(0.0, 0.0, 0.0), Point3d(-1.1889778921584675, -7.356451670462243, 0.0), Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTest9) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(77.0, 85.0, 0.0));
  footprint.push_back(Point3d(198.0, 85.0, 0.0));
  footprint.push_back(Point3d(196.0, 139.0, 0.0));
  footprint.push_back(Point3d(150.0, 119.0, 0.0));
  footprint.push_back(Point3d(157.0, 177.0, 0.0));
  footprint.push_back(Point3d(112.0, 179.0, 0.0));
  footprint.push_back(Point3d(125.0, 130.0, 0.0));
  footprint.push_back(Point3d(68.0, 118.0, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(77.0, 85.0, 0.0), Point3d(198.0, 85.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(198.0, 85.0, 0.0), Point3d(196.0, 139.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(196.0, 139.0, 0.0), Point3d(150.0, 119.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(150.0, 119.0, 0.0), Point3d(157.0, 177.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(157.0, 177.0, 0.0), Point3d(112.0, 179.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(112.0, 179.0, 0.0), Point3d(125.0, 130.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(125.0, 130.0, 0.0), Point3d(68.0, 118.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(68.0, 118.0, 0.0), Point3d(77.0, 85.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(175.59714332930346, 106.58848144366122, 10.794240721830585), Point3d(198.0, 85.0, 0.0), Point3d(196.0, 139.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(175.59714332930346, 106.58848144366122, 10.794240721830585), Point3d(196.0, 139.0, 0.0), Point3d(150.0, 119.0, 0.0), Point3d(134.3606956195723, 98.01182646424897, 6.505913232124485)});
  expectedRoofPolygons.push_back({Point3d(126.06659653966571, 107.36712501572094, 11.183562507860437), Point3d(134.3606956195723, 98.01182646424897, 6.505913232124485), Point3d(150.0, 119.0, 0.0), Point3d(157.0, 177.0, 0.0), Point3d(136.28719078311278, 159.44250239980386, 9.229921974315973), Point3d(138.93854961582085, 121.41610355696413, 5.635628665086813)});
  expectedRoofPolygons.push_back({Point3d(136.28719078311278, 159.44250239980386, 9.229921974315973), Point3d(157.0, 177.0, 0.0), Point3d(112.0, 179.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(136.28719078311278, 159.44250239980386, 9.229921974315973), Point3d(112.0, 179.0, 0.0), Point3d(125.0, 130.0, 0.0), Point3d(138.93854961582085, 121.41610355696413, 5.635628665086813)});
  expectedRoofPolygons.push_back({Point3d(126.06659653966571, 107.36712501572094, 11.183562507860437), Point3d(138.93854961582085, 121.41610355696413, 5.635628665086813), Point3d(125.0, 130.0, 0.0), Point3d(68.0, 118.0, 0.0), Point3d(91.32064371214591, 103.74930831439367, 9.374654157196835)});
  expectedRoofPolygons.push_back({Point3d(91.32064371214591, 103.74930831439367, 9.374654157196835), Point3d(68.0, 118.0, 0.0), Point3d(77.0, 85.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(126.06659653966571, 107.36712501572094, 11.183562507860437), Point3d(91.32064371214591, 103.74930831439367, 9.374654157196835), Point3d(77.0, 85.0, 0.0), Point3d(198.0, 85.0, 0.0), Point3d(175.59714332930346, 106.58848144366122, 10.794240721830585), Point3d(134.3606956195723, 98.01182646424897, 6.505913232124485)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB10) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(23.542862199718826, -1.0957017437087124, 0.0));
  footprint.push_back(Point3d(12.89581137652037, 1.5573908447103584, 0.0));
  footprint.push_back(Point3d(13.68678342709616, 5.195862274901293, 0.0));
  footprint.push_back(Point3d(30.92997412599037, 6.619611963708646, 0.0));
  footprint.push_back(Point3d(16.53428280871175, 7.568778425199767, 0.0));
  footprint.push_back(Point3d(13.05400578686415, 8.676139297892002, 0.0));
  footprint.push_back(Point3d(-4.188927083681472, 7.336703572978552, 0.0));
  footprint.push_back(Point3d(10.196014852102863, 4.475707108744242, 0.0));
  footprint.push_back(Point3d(8.782756714583655, 1.5573908412810287, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(23.542862199718826, -1.0957017437087124, 0.0)});
  expectedRoofEdges.push_back({Point3d(23.542862199718826, -1.0957017437087124, 0.0), Point3d(12.89581137652037, 1.5573908447103584, 0.0)});
  expectedRoofEdges.push_back({Point3d(12.89581137652037, 1.5573908447103584, 0.0), Point3d(13.68678342709616, 5.195862274901293, 0.0)});
  expectedRoofEdges.push_back({Point3d(13.68678342709616, 5.195862274901293, 0.0), Point3d(30.92997412599037, 6.619611963708646, 0.0)});
  expectedRoofEdges.push_back({Point3d(30.92997412599037, 6.619611963708646, 0.0), Point3d(16.53428280871175, 7.568778425199767, 0.0)});
  expectedRoofEdges.push_back({Point3d(16.53428280871175, 7.568778425199767, 0.0), Point3d(13.05400578686415, 8.676139297892002, 0.0)});
  expectedRoofEdges.push_back({Point3d(13.05400578686415, 8.676139297892002, 0.0), Point3d(-4.188927083681472, 7.336703572978552, 0.0)});
  expectedRoofEdges.push_back({Point3d(-4.188927083681472, 7.336703572978552, 0.0), Point3d(10.196014852102863, 4.475707108744242, 0.0)});
  expectedRoofEdges.push_back({Point3d(10.196014852102863, 4.475707108744242, 0.0), Point3d(8.782756714583655, 1.5573908412810287, 0.0)});
  expectedRoofEdges.push_back({Point3d(8.782756714583655, 1.5573908412810287, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(11.471020185145417, 0.67152109157326, 0.6020436394953249), Point3d(23.542862199718826, -1.0957017437087124, 0.0), Point3d(12.89581137652037, 1.5573908447103584, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.720279767074027, 6.390569400059585, 1.0877060529617832), Point3d(12.291810335104968, 5.518617287012664, 0.7158487331168258), Point3d(10.882442218967304, 1.4375936356513357, 0.9709841370116856), Point3d(11.471020185145417, 0.67152109157326, 0.6020436394953249), Point3d(12.89581137652037, 1.5573908447103584, 0.0), Point3d(13.68678342709616, 5.195862274901293, 0.0), Point3d(12.24155597023194, 6.845123864037758, 0.8812970821560071)});
  expectedRoofPolygons.push_back({Point3d(12.24155597023194, 6.845123864037758, 0.8812970821560071), Point3d(13.68678342709616, 5.195862274901293, 0.0), Point3d(30.92997412599037, 6.619611963708646, 0.0), Point3d(16.331902944773294, 6.498860088077869, 0.5404575403927483), Point3d(12.84763814480008, 6.893685631130523, 0.8805587371824312)});
  expectedRoofPolygons.push_back({Point3d(16.331902944773294, 6.498860088077869, 0.5404575403927483), Point3d(30.92997412599037, 6.619611963708646, 0.0), Point3d(16.53428280871175, 7.568778425199767, 0.0)});
  expectedRoofPolygons.push_back({Point3d(12.84763814480008, 6.893685631130523, 0.8805587371824312), Point3d(16.331902944773294, 6.498860088077869, 0.5404575403927483), Point3d(16.53428280871175, 7.568778425199767, 0.0), Point3d(13.05400578686415, 8.676139297892002, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.720279767074027, 6.390569400059585, 1.0877060529617832), Point3d(12.24155597023194, 6.845123864037758, 0.8812970821560071), Point3d(12.84763814480008, 6.893685631130523, 0.8805587371824312), Point3d(13.05400578686415, 8.676139297892002, 0.0), Point3d(-4.188927083681472, 7.336703572978552, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.720279767074027, 6.390569400059585, 1.0877060529617832), Point3d(-4.188927083681472, 7.336703572978552, 0.0), Point3d(10.196014852102863, 4.475707108744242, 0.0), Point3d(12.291810335104968, 5.518617287012664, 0.7158487331168258)});
  expectedRoofPolygons.push_back({Point3d(10.882442218967304, 1.4375936356513357, 0.9709841370116856), Point3d(12.291810335104968, 5.518617287012664, 0.7158487331168258), Point3d(10.196014852102863, 4.475707108744242, 0.0), Point3d(8.782756714583655, 1.5573908412810287, 0.0), Point3d(9.49692191488606, 0.6133654113232458, 0.5271089343613387)});
  expectedRoofPolygons.push_back({Point3d(9.49692191488606, 0.6133654113232458, 0.5271089343613387), Point3d(8.782756714583655, 1.5573908412810287, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(10.882442218967304, 1.4375936356513357, 0.9709841370116856), Point3d(9.49692191488606, 0.6133654113232458, 0.5271089343613387), Point3d(0.0, 0.0, 0.0), Point3d(23.542862199718826, -1.0957017437087124, 0.0), Point3d(11.471020185145417, 0.67152109157326, 0.6020436394953249)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB11) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(-0.2885918221241157, 14.001053106358517, 0.0));
  footprint.push_back(Point3d(4.899343591400031, 14.19306411217788, 0.0));
  footprint.push_back(Point3d(4.873615853393824, 2.8449682126970464, 0.0));
  footprint.push_back(Point3d(3.4878348313988496, 2.8481099737474747, 0.0));
  footprint.push_back(Point3d(3.4812833647560453, -0.04163770013738066, 0.0));
  footprint.push_back(Point3d(4.964494009365636, -0.04500034901230876, 0.0));
  footprint.push_back(Point3d(4.95897480112971, -2.4794352197829106, 0.0));
  footprint.push_back(Point3d(8.807770830090442, -2.4881609878627096, 0.0));
  footprint.push_back(Point3d(8.823006374999641, 4.231995455388115, 0.0));
  footprint.push_back(Point3d(12.663139806598815, 4.223289329948495, 0.0));
  footprint.push_back(Point3d(12.674931978309203, 9.424628538740333, 0.0));
  footprint.push_back(Point3d(8.115510522870647, 9.43496539140081, 0.0));
  footprint.push_back(Point3d(4.744642144665839, 22.784916024869645, 0.0));
  footprint.push_back(Point3d(4.729419759102431, 16.070563983220485, 0.0));
  footprint.push_back(Point3d(-1.2112187885796715, 16.08403225178089, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(-0.2885918221241157, 14.001053106358517, 0.0)});
  expectedRoofEdges.push_back({Point3d(-0.2885918221241157, 14.001053106358517, 0.0), Point3d(4.899343591400031, 14.19306411217788, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.899343591400031, 14.19306411217788, 0.0), Point3d(4.873615853393824, 2.8449682126970464, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.873615853393824, 2.8449682126970464, 0.0), Point3d(3.4878348313988496, 2.8481099737474747, 0.0)});
  expectedRoofEdges.push_back({Point3d(3.4878348313988496, 2.8481099737474747, 0.0), Point3d(3.4812833647560453, -0.04163770013738066, 0.0)});
  expectedRoofEdges.push_back({Point3d(3.4812833647560453, -0.04163770013738066, 0.0), Point3d(4.964494009365636, -0.04500034901230876, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.964494009365636, -0.04500034901230876, 0.0), Point3d(4.95897480112971, -2.4794352197829106, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.95897480112971, -2.4794352197829106, 0.0), Point3d(8.807770830090442, -2.4881609878627096, 0.0)});
  expectedRoofEdges.push_back({Point3d(8.807770830090442, -2.4881609878627096, 0.0), Point3d(8.823006374999641, 4.231995455388115, 0.0)});
  expectedRoofEdges.push_back({Point3d(8.823006374999641, 4.231995455388115, 0.0), Point3d(12.663139806598815, 4.223289329948495, 0.0)});
  expectedRoofEdges.push_back({Point3d(12.663139806598815, 4.223289329948495, 0.0), Point3d(12.674931978309203, 9.424628538740333, 0.0)});
  expectedRoofEdges.push_back({Point3d(12.674931978309203, 9.424628538740333, 0.0), Point3d(8.115510522870647, 9.43496539140081, 0.0)});
  expectedRoofEdges.push_back({Point3d(8.115510522870647, 9.43496539140081, 0.0), Point3d(4.744642144665839, 22.784916024869645, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.744642144665839, 22.784916024869645, 0.0), Point3d(4.729419759102431, 16.070563983220485, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.729419759102431, 16.070563983220485, 0.0), Point3d(-1.2112187885796715, 16.08403225178089, 0.0)});
  expectedRoofEdges.push_back({Point3d(-1.2112187885796715, 16.08403225178089, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(-0.09379527500698746, 15.045234233860004, 0.5181309987380204), Point3d(-0.6890929440306793, 14.379123828889721, 0.19631243722611377), Point3d(-0.2885918221241157, 14.001053106358517, 0.0), Point3d(4.899343591400031, 14.19306411217788, 0.0), Point3d(5.778996208137605, 15.103807429244426, 0.43879278874940125), Point3d(5.68156679292687, 15.176839103797569, 0.47708539116320225), Point3d(5.650805671922317, 15.144990752701979, 0.46174097245332135)});
  expectedRoofPolygons.push_back({Point3d(7.483346413476189, 6.8357156253459355, 1.300338143887085), Point3d(6.700335885294145, 7.622284612142864, 0.9079422566821708), Point3d(5.778996208137605, 15.103807429244426, 0.43879278874940125), Point3d(4.899343591400031, 14.19306411217788, 0.0), Point3d(4.873615853393824, 2.8449682126970464, 0.0), Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808), Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838), Point3d(6.854366875075671, 6.209581606843638, 0.9865589482644913)});
  expectedRoofPolygons.push_back({Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808), Point3d(4.873615853393824, 2.8449682126970464, 0.0), Point3d(3.4878348313988496, 2.8481099737474747, 0.0), Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532)});
  expectedRoofPolygons.push_back({Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532), Point3d(3.4878348313988496, 2.8481099737474747, 0.0), Point3d(3.4812833647560453, -0.04163770013738066, 0.0)});
  expectedRoofPolygons.push_back({Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838), Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808), Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532), Point3d(3.4812833647560453, -0.04163770013738066, 0.0), Point3d(4.964494009365636, -0.04500034901230876, 0.0), Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014)});
  expectedRoofPolygons.push_back({Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014), Point3d(4.964494009365636, -0.04500034901230876, 0.0), Point3d(4.95897480112971, -2.4794352197829106, 0.0), Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872)});
  expectedRoofPolygons.push_back({Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872), Point3d(4.95897480112971, -2.4794352197829106, 0.0), Point3d(8.807770830090442, -2.4881609878627096, 0.0)});
  expectedRoofPolygons.push_back({Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838), Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014), Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872), Point3d(8.807770830090442, -2.4881609878627096, 0.0), Point3d(8.823006374999641, 4.231995455388115, 0.0), Point3d(6.854366875075671, 6.209581606843638, 0.9865589482644913)});
  expectedRoofPolygons.push_back({Point3d(10.068366288179561, 6.829855019919622, 1.3003381439477693), Point3d(7.483346413476189, 6.8357156253459355, 1.300338143887085), Point3d(6.854366875075671, 6.209581606843638, 0.9865589482644913), Point3d(8.823006374999641, 4.231995455388115, 0.0), Point3d(12.663139806598815, 4.223289329948495, 0.0)});
  expectedRoofPolygons.push_back({Point3d(10.068366288179561, 6.829855019919622, 1.3003381439477693), Point3d(12.663139806598815, 4.223289329948495, 0.0), Point3d(12.674931978309203, 9.424628538740333, 0.0)});
  expectedRoofPolygons.push_back({Point3d(10.068366288179561, 6.829855019919622, 1.3003381439477693), Point3d(12.674931978309203, 9.424628538740333, 0.0), Point3d(8.115510522870647, 9.43496539140081, 0.0), Point3d(6.700335885294145, 7.622284612142864, 0.9079422566821708), Point3d(7.483346413476189, 6.8357156253459355, 1.300338143887085)});
  expectedRoofPolygons.push_back({Point3d(6.700335885294145, 7.622284612142864, 0.9079422566821708), Point3d(8.115510522870647, 9.43496539140081, 0.0), Point3d(4.744642144665839, 22.784916024869645, 0.0), Point3d(5.68156679292687, 15.176839103797569, 0.47708539116320225), Point3d(5.778996208137605, 15.103807429244426, 0.43879278874940125)});
  expectedRoofPolygons.push_back({Point3d(5.68156679292687, 15.176839103797569, 0.47708539116320225), Point3d(4.744642144665839, 22.784916024869645, 0.0), Point3d(4.729419759102431, 16.070563983220485, 0.0), Point3d(5.650805671922317, 15.144990752701979, 0.46174097245332135)});
  expectedRoofPolygons.push_back({Point3d(-0.09379527500698746, 15.045234233860004, 0.5181309987380204), Point3d(5.650805671922317, 15.144990752701979, 0.46174097245332135), Point3d(4.729419759102431, 16.070563983220485, 0.0), Point3d(-1.2112187885796715, 16.08403225178089, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-0.09379527500698746, 15.045234233860004, 0.5181309987380204), Point3d(-1.2112187885796715, 16.08403225178089, 0.0), Point3d(0.0, 0.0, 0.0), Point3d(-0.6890929440306793, 14.379123828889721, 0.19631243722611377)});
  expectedRoofPolygons.push_back({Point3d(-0.6890929440306793, 14.379123828889721, 0.19631243722611377), Point3d(0.0, 0.0, 0.0), Point3d(-0.2885918221241157, 14.001053106358517, 0.0)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB11_b) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(4.899343591400031, 14.19306411217788, 0.0));
  footprint.push_back(Point3d(4.873615853393824, 2.8449682126970464, 0.0));
  footprint.push_back(Point3d(3.4878348313988496, 2.8481099737474747, 0.0));
  footprint.push_back(Point3d(3.4812833647560453, -0.04163770013738066, 0.0));
  footprint.push_back(Point3d(4.964494009365636, -0.04500034901230876, 0.0));
  footprint.push_back(Point3d(4.95897480112971, -2.4794352197829106, 0.0));
  footprint.push_back(Point3d(8.807770830090442, -2.4881609878627096, 0.0));
  footprint.push_back(Point3d(8.823006374999641, 4.231995455388115, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(4.899343591400031, 14.19306411217788, 0.0), Point3d(4.873615853393824, 2.8449682126970464, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.873615853393824, 2.8449682126970464, 0.0), Point3d(3.4878348313988496, 2.8481099737474747, 0.0)});
  expectedRoofEdges.push_back({Point3d(3.4878348313988496, 2.8481099737474747, 0.0), Point3d(3.4812833647560453, -0.04163770013738066, 0.0)});
  expectedRoofEdges.push_back({Point3d(3.4812833647560453, -0.04163770013738066, 0.0), Point3d(4.964494009365636, -0.04500034901230876, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.964494009365636, -0.04500034901230876, 0.0), Point3d(4.95897480112971, -2.4794352197829106, 0.0)});
  expectedRoofEdges.push_back({Point3d(4.95897480112971, -2.4794352197829106, 0.0), Point3d(8.807770830090442, -2.4881609878627096, 0.0)});
  expectedRoofEdges.push_back({Point3d(8.807770830090442, -2.4881609878627096, 0.0), Point3d(8.823006374999641, 4.231995455388115, 0.0)});
  expectedRoofEdges.push_back({Point3d(8.823006374999641, 4.231995455388115, 0.0), Point3d(4.899343591400031, 14.19306411217788, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808), Point3d(4.873615853393824, 2.8449682126970464, 0.0), Point3d(3.4878348313988496, 2.8481099737474747, 0.0), Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532)});
  expectedRoofPolygons.push_back({Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532), Point3d(3.4878348313988496, 2.8481099737474747, 0.0), Point3d(3.4812833647560453, -0.04163770013738066, 0.0)});
  expectedRoofPolygons.push_back({Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838), Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808), Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532), Point3d(3.4812833647560453, -0.04163770013738066, 0.0), Point3d(4.964494009365636, -0.04500034901230876, 0.0), Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014)});
  expectedRoofPolygons.push_back({Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014), Point3d(4.964494009365636, -0.04500034901230876, 0.0), Point3d(4.95897480112971, -2.4794352197829106, 0.0), Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872)});
  expectedRoofPolygons.push_back({Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872), Point3d(4.95897480112971, -2.4794352197829106, 0.0), Point3d(8.807770830090442, -2.4881609878627096, 0.0)});
  expectedRoofPolygons.push_back({Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838), Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014), Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872), Point3d(8.807770830090442, -2.4881609878627096, 0.0), Point3d(8.823006374999641, 4.231995455388115, 0.0), Point3d(6.8490390285892975, 3.8595532917064257, 0.9865589482615805)});
  expectedRoofPolygons.push_back({Point3d(6.8490390285892975, 3.8595532917064257, 0.9865589482615805), Point3d(8.823006374999641, 4.231995455388115, 0.0), Point3d(4.899343591400031, 14.19306411217788, 0.0)});
  expectedRoofPolygons.push_back({Point3d(6.8490390285892975, 3.8595532917064257, 0.9865589482615805), Point3d(4.899343591400031, 14.19306411217788, 0.0), Point3d(4.873615853393824, 2.8449682126970464, 0.0), Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808), Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB12) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(1.6082838074612242, 15.395815413439262, 0.0));
  footprint.push_back(Point3d(6.796219518140479, 15.587826427398873, 0.0));
  footprint.push_back(Point3d(6.7704917786606345, 4.239729879063727, 0.0));
  footprint.push_back(Point3d(5.384710677004972, 4.2428716408656655, 0.0));
  footprint.push_back(Point3d(5.37815921027269, 1.3531237986037645, 0.0));
  footprint.push_back(Point3d(6.861369940123552, 1.3497611512508971, 0.0));
  footprint.push_back(Point3d(6.855850731428608, -1.084673859531076, 0.0));
  footprint.push_back(Point3d(10.704646980698193, -1.093399628682226, 0.0));
  footprint.push_back(Point3d(10.719882526622944, 5.626757200629533, 0.0));
  footprint.push_back(Point3d(14.560016178034793, 5.6180510758343525, 0.0));
  footprint.push_back(Point3d(14.571808350563504, 10.819390581977487, 0.0));
  footprint.push_back(Point3d(10.01238663382704, 10.829727434086928, 0.0));
  footprint.push_back(Point3d(6.64151806240239, 24.179678832787182, 0.0));
  footprint.push_back(Point3d(6.626295676252851, 17.465326408838887, 0.0));
  footprint.push_back(Point3d(0.6856567883022331, 17.478794675312955, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(1.6082838074612242, 15.395815413439262, 0.0)});
  expectedRoofEdges.push_back({Point3d(1.6082838074612242, 15.395815413439262, 0.0), Point3d(6.796219518140479, 15.587826427398873, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.796219518140479, 15.587826427398873, 0.0), Point3d(6.7704917786606345, 4.239729879063727, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.7704917786606345, 4.239729879063727, 0.0), Point3d(5.384710677004972, 4.2428716408656655, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.384710677004972, 4.2428716408656655, 0.0), Point3d(5.37815921027269, 1.3531237986037645, 0.0)});
  expectedRoofEdges.push_back({Point3d(5.37815921027269, 1.3531237986037645, 0.0), Point3d(6.861369940123552, 1.3497611512508971, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.861369940123552, 1.3497611512508971, 0.0), Point3d(6.855850731428608, -1.084673859531076, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.855850731428608, -1.084673859531076, 0.0), Point3d(10.704646980698193, -1.093399628682226, 0.0)});
  expectedRoofEdges.push_back({Point3d(10.704646980698193, -1.093399628682226, 0.0), Point3d(10.719882526622944, 5.626757200629533, 0.0)});
  expectedRoofEdges.push_back({Point3d(10.719882526622944, 5.626757200629533, 0.0), Point3d(14.560016178034793, 5.6180510758343525, 0.0)});
  expectedRoofEdges.push_back({Point3d(14.560016178034793, 5.6180510758343525, 0.0), Point3d(14.571808350563504, 10.819390581977487, 0.0)});
  expectedRoofEdges.push_back({Point3d(14.571808350563504, 10.819390581977487, 0.0), Point3d(10.01238663382704, 10.829727434086928, 0.0)});
  expectedRoofEdges.push_back({Point3d(10.01238663382704, 10.829727434086928, 0.0), Point3d(6.64151806240239, 24.179678832787182, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.64151806240239, 24.179678832787182, 0.0), Point3d(6.626295676252851, 17.465326408838887, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.626295676252851, 17.465326408838887, 0.0), Point3d(0.6856567883022331, 17.478794675312955, 0.0)});
  expectedRoofEdges.push_back({Point3d(0.6856567883022331, 17.478794675312955, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.6842202935831398, 16.437932562856407, 0.5192977794804833), Point3d(1.1408243780107237, 15.895738201571742, 0.2584350351911583), Point3d(1.6082838074612242, 15.395815413439262, 0.0), Point3d(6.796219518140479, 15.587826427398873, 0.0), Point3d(7.67587218531803, 16.498569796186654, 0.4387928139105931), Point3d(7.578442764618077, 16.571601474896294, 0.4770854184766968), Point3d(7.54768164371096, 16.539753123917784, 0.4617409998147321)});
  expectedRoofPolygons.push_back({Point3d(9.380222488057502, 8.230477519159527, 1.3003382182858605), Point3d(8.59721191524112, 9.017046550596557, 0.9079423087131434), Point3d(7.67587218531803, 16.498569796186654, 0.4387928139105931), Point3d(6.796219518140479, 15.587826427398873, 0.0), Point3d(6.7704917786606345, 4.239729879063727, 0.0), Point3d(8.21208996516684, 2.7915802254966904, 0.7224388166284438), Point3d(8.741526576487445, 3.3186216521109917, 0.986559004791438), Point3d(8.7512429138868, 7.604343464862684, 0.986559004818869)});
  expectedRoofPolygons.push_back({Point3d(6.826308864186403, 2.7947219867367816, 0.7224388169085991), Point3d(8.21208996516684, 2.7915802254966904, 0.7224388166284438), Point3d(6.7704917786606345, 4.239729879063727, 0.0), Point3d(5.384710677004972, 4.2428716408656655, 0.0)});
  expectedRoofPolygons.push_back({Point3d(6.826308864186403, 2.7947219867367816, 0.7224388169085991), Point3d(5.384710677004972, 4.2428716408656655, 0.0), Point3d(5.37815921027269, 1.3531237986037645, 0.0)});
  expectedRoofPolygons.push_back({Point3d(8.741526576487445, 3.3186216521109917, 0.986559004791438), Point3d(8.21208996516684, 2.7915802254966904, 0.7224388166284438), Point3d(6.826308864186403, 2.7947219867367816, 0.7224388169085991), Point3d(5.37815921027269, 1.3531237986037645, 0.0), Point3d(6.861369940123552, 1.3497611512508971, 0.0), Point3d(8.790130947445553, 3.269796394443114, 0.9622015350280487)});
  expectedRoofPolygons.push_back({Point3d(8.78461173886206, 0.8353613804382916, 0.962201535087427), Point3d(8.790130947445553, 3.269796394443114, 0.9622015350280487), Point3d(6.861369940123552, 1.3497611512508971, 0.0), Point3d(6.855850731428608, -1.084673859531076, 0.0)});
  expectedRoofPolygons.push_back({Point3d(8.78461173886206, 0.8353613804382916, 0.962201535087427), Point3d(6.855850731428608, -1.084673859531076, 0.0), Point3d(10.704646980698193, -1.093399628682226, 0.0)});
  expectedRoofPolygons.push_back({Point3d(8.741526576487445, 3.3186216521109917, 0.986559004791438), Point3d(8.790130947445553, 3.269796394443114, 0.9622015350280487), Point3d(8.78461173886206, 0.8353613804382916, 0.962201535087427), Point3d(10.704646980698193, -1.093399628682226, 0.0), Point3d(10.719882526622944, 5.626757200629533, 0.0), Point3d(8.7512429138868, 7.604343464862684, 0.986559004818869)});
  expectedRoofPolygons.push_back({Point3d(9.380222488057502, 8.230477519159527, 1.3003382182858605), Point3d(8.7512429138868, 7.604343464862684, 0.986559004818869), Point3d(10.719882526622944, 5.626757200629533, 0.0), Point3d(14.560016178034793, 5.6180510758343525, 0.0), Point3d(11.965242511286412, 8.224616914105267, 1.3003382183162968)});
  expectedRoofPolygons.push_back({Point3d(11.965242511286412, 8.224616914105267, 1.3003382183162968), Point3d(14.560016178034793, 5.6180510758343525, 0.0), Point3d(14.571808350563504, 10.819390581977487, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.965242511286412, 8.224616914105267, 1.3003382183162968), Point3d(14.571808350563504, 10.819390581977487, 0.0), Point3d(10.01238663382704, 10.829727434086928, 0.0), Point3d(8.59721191524112, 9.017046550596557, 0.9079423087131434), Point3d(9.380222488057502, 8.230477519159527, 1.3003382182858605)});
  expectedRoofPolygons.push_back({Point3d(8.59721191524112, 9.017046550596557, 0.9079423087131434), Point3d(10.01238663382704, 10.829727434086928, 0.0), Point3d(6.64151806240239, 24.179678832787182, 0.0), Point3d(7.578442764618077, 16.571601474896294, 0.4770854184766968), Point3d(7.67587218531803, 16.498569796186654, 0.4387928139105931)});
  expectedRoofPolygons.push_back({Point3d(7.578442764618077, 16.571601474896294, 0.4770854184766968), Point3d(6.64151806240239, 24.179678832787182, 0.0), Point3d(6.626295676252851, 17.465326408838887, 0.0), Point3d(7.54768164371096, 16.539753123917784, 0.4617409998147321)});
  expectedRoofPolygons.push_back({Point3d(1.6842202935831398, 16.437932562856407, 0.5192977794804833), Point3d(7.54768164371096, 16.539753123917784, 0.4617409998147321), Point3d(6.626295676252851, 17.465326408838887, 0.0), Point3d(0.6856567883022331, 17.478794675312955, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.6842202935831398, 16.437932562856407, 0.5192977794804833), Point3d(0.6856567883022331, 17.478794675312955, 0.0), Point3d(0.0, 0.0, 0.0), Point3d(1.1408243780107237, 15.895738201571742, 0.2584350351911583)});
  expectedRoofPolygons.push_back({Point3d(1.1408243780107237, 15.895738201571742, 0.2584350351911583), Point3d(0.0, 0.0, 0.0), Point3d(1.6082838074612242, 15.395815413439262, 0.0)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB13) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(0.0, 0.0, 0.0));
  footprint.push_back(Point3d(-0.03697835689094475, 17.903291653889664, 0.0));
  footprint.push_back(Point3d(9.36122931562474, 17.922703185404146, 0.0));
  footprint.push_back(Point3d(9.399539490923859, -0.6253964219022965, 0.0));
  footprint.push_back(Point3d(6.897780217346079, -0.6305636811510293, 0.0));
  footprint.push_back(Point3d(6.907305814387495, -5.242438102429183, 0.0));
  footprint.push_back(Point3d(9.496043768204736, -5.2367356072030695, 0.0));
  footprint.push_back(Point3d(9.673537498409361, -7.819464124646299, 0.0));
  footprint.push_back(Point3d(19.728934851080233, -7.7986952031890375, 0.0));
  footprint.push_back(Point3d(19.715280237589244, -1.1877328304801722, 0.0));
  footprint.push_back(Point3d(23.581205989632387, -1.1797479507986637, 0.0));
  footprint.push_back(Point3d(23.570459756724986, 4.023104657038741, 0.0));
  footprint.push_back(Point3d(19.065027189523686, 4.01379891209519, 0.0));
  footprint.push_back(Point3d(19.009685241927738, 30.807932065847332, 0.0));
  footprint.push_back(Point3d(9.439383865135643, 30.78816508512935, 0.0));
  footprint.push_back(Point3d(9.453189359125524, 24.10415305431124, 0.0));
  footprint.push_back(Point3d(-0.01730198014624129, 24.08459222736407, 0.0));
  footprint.push_back(Point3d(-0.030597953439544412, 30.521916694234474, 0.0));
  footprint.push_back(Point3d(-10.417861267451112, 30.500462317733504, 0.0));
  footprint.push_back(Point3d(-10.354819907553885, -0.021387367337700525, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(0.0, 0.0, 0.0), Point3d(-0.03697835689094475, 17.903291653889664, 0.0)});
  expectedRoofEdges.push_back({Point3d(-0.03697835689094475, 17.903291653889664, 0.0), Point3d(9.36122931562474, 17.922703185404146, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.36122931562474, 17.922703185404146, 0.0), Point3d(9.399539490923859, -0.6253964219022965, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.399539490923859, -0.6253964219022965, 0.0), Point3d(6.897780217346079, -0.6305636811510293, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.897780217346079, -0.6305636811510293, 0.0), Point3d(6.907305814387495, -5.242438102429183, 0.0)});
  expectedRoofEdges.push_back({Point3d(6.907305814387495, -5.242438102429183, 0.0), Point3d(9.496043768204736, -5.2367356072030695, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.496043768204736, -5.2367356072030695, 0.0), Point3d(9.673537498409361, -7.819464124646299, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.673537498409361, -7.819464124646299, 0.0), Point3d(19.728934851080233, -7.7986952031890375, 0.0)});
  expectedRoofEdges.push_back({Point3d(19.728934851080233, -7.7986952031890375, 0.0), Point3d(19.715280237589244, -1.1877328304801722, 0.0)});
  expectedRoofEdges.push_back({Point3d(19.715280237589244, -1.1877328304801722, 0.0), Point3d(23.581205989632387, -1.1797479507986637, 0.0)});
  expectedRoofEdges.push_back({Point3d(23.581205989632387, -1.1797479507986637, 0.0), Point3d(23.570459756724986, 4.023104657038741, 0.0)});
  expectedRoofEdges.push_back({Point3d(23.570459756724986, 4.023104657038741, 0.0), Point3d(19.065027189523686, 4.01379891209519, 0.0)});
  expectedRoofEdges.push_back({Point3d(19.065027189523686, 4.01379891209519, 0.0), Point3d(19.009685241927738, 30.807932065847332, 0.0)});
  expectedRoofEdges.push_back({Point3d(19.009685241927738, 30.807932065847332, 0.0), Point3d(9.439383865135643, 30.78816508512935, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.439383865135643, 30.78816508512935, 0.0), Point3d(9.453189359125524, 24.10415305431124, 0.0)});
  expectedRoofEdges.push_back({Point3d(9.453189359125524, 24.10415305431124, 0.0), Point3d(-0.01730198014624129, 24.08459222736407, 0.0)});
  expectedRoofEdges.push_back({Point3d(-0.01730198014624129, 24.08459222736407, 0.0), Point3d(-0.030597953439544412, 30.521916694234474, 0.0)});
  expectedRoofEdges.push_back({Point3d(-0.030597953439544412, 30.521916694234474, 0.0), Point3d(-10.417861267451112, 30.500462317733504, 0.0)});
  expectedRoofEdges.push_back({Point3d(-10.417861267451112, 30.500462317733504, 0.0), Point3d(-10.354819907553885, -0.021387367337700525, 0.0)});
  expectedRoofEdges.push_back({Point3d(-10.354819907553885, -0.021387367337700525, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(-5.2088937947536875, 23.086263132900534, 2.5968213677080576), Point3d(-5.225081993006609, 23.07000792440424, 2.5887104987207548), Point3d(-0.03697835689094475, 17.903291653889664, 0.0), Point3d(9.36122931562474, 17.922703185404146, 0.0), Point3d(12.445462580412869, 21.019703480686516, 1.5453116860600677), Point3d(-3.101535247093261, 20.98759193064646, 1.5453116872475992)});
  expectedRoofPolygons.push_back({Point3d(14.557918451002053, -1.1527999293121487, 2.578639316869181), Point3d(14.237504608711996, -0.833706956371332, 2.418762272064255), Point3d(14.19596815545603, 19.276414164692916, 2.4187622718784856), Point3d(12.445462580412869, 21.019703480686516, 1.5453116860600677), Point3d(9.36122931562474, 17.922703185404146, 0.0), Point3d(9.399539490923859, -0.6253964219022965, 0.0), Point3d(12.606101682729625, -3.8187399272616855, 1.599979837155215), Point3d(13.428106603203812, -3.7896778027216365, 2.0110114338303995), Point3d(14.561015138561665, -2.6520796490290395, 2.5786393168806017)});
  expectedRoofPolygons.push_back({Point3d(12.606101682729625, -3.8187399272616855, 1.599979837155215), Point3d(9.399539490923859, -0.6253964219022965, 0.0), Point3d(6.897780217346079, -0.6305636811510293, 0.0), Point3d(9.208321529781248, -2.9315800507494063, 1.152891879668805), Point3d(11.648322280432005, -2.9263727729378277, 1.1528080909886411)});
  expectedRoofPolygons.push_back({Point3d(9.208321529781248, -2.9315800507494063, 1.152891879668805), Point3d(6.897780217346079, -0.6305636811510293, 0.0), Point3d(6.907305814387495, -5.242438102429183, 0.0)});
  expectedRoofPolygons.push_back({Point3d(9.208321529781248, -2.9315800507494063, 1.152891879668805), Point3d(6.907305814387495, -5.242438102429183, 0.0), Point3d(9.496043768204736, -5.2367356072030695, 0.0), Point3d(11.648322280432005, -2.9263727729378277, 1.1528080909886411)});
  expectedRoofPolygons.push_back({Point3d(13.428106603203812, -3.7896778027216365, 2.0110114338303995), Point3d(12.606101682729625, -3.8187399272616855, 1.599979837155215), Point3d(11.648322280432005, -2.9263727729378277, 1.1528080909886411), Point3d(9.496043768204736, -5.2367356072030695, 0.0), Point3d(9.673537498409361, -7.819464124646299, 0.0)});
  expectedRoofPolygons.push_back({Point3d(14.561015138561665, -2.6520796490290395, 2.5786393168806017), Point3d(13.428106603203812, -3.7896778027216365, 2.0110114338303995), Point3d(9.673537498409361, -7.819464124646299, 0.0), Point3d(19.728934851080233, -7.7986952031890375, 0.0)});
  expectedRoofPolygons.push_back({Point3d(14.561015138561665, -2.6520796490290395, 2.5786393168806017), Point3d(19.728934851080233, -7.7986952031890375, 0.0), Point3d(19.715280237589244, -1.1877328304801722, 0.0), Point3d(17.108480813881513, 1.408320358557954, 1.3007159280980942), Point3d(14.557918451002053, -1.1527999293121487, 2.578639316869181)});
  expectedRoofPolygons.push_back({Point3d(17.108480813881513, 1.408320358557954, 1.3007159280980942), Point3d(19.715280237589244, -1.1877328304801722, 0.0), Point3d(23.581205989632387, -1.1797479507986637, 0.0), Point3d(20.974406567920887, 1.4163052362523214, 1.3007159271024618)});
  expectedRoofPolygons.push_back({Point3d(20.974406567920887, 1.4163052362523214, 1.3007159271024618), Point3d(23.581205989632387, -1.1797479507986637, 0.0), Point3d(23.570459756724986, 4.023104657038741, 0.0)});
  expectedRoofPolygons.push_back({Point3d(14.557918451002053, -1.1527999293121487, 2.578639316869181), Point3d(17.108480813881513, 1.408320358557954, 1.3007159280980942), Point3d(20.974406567920887, 1.4163052362523214, 1.3007159271024618), Point3d(23.570459756724986, 4.023104657038741, 0.0), Point3d(19.065027189523686, 4.01379891209519, 0.0), Point3d(14.237504608711996, -0.833706956371332, 2.418762272064255)});
  expectedRoofPolygons.push_back({Point3d(14.237504608711996, -0.833706956371332, 2.418762272064255), Point3d(19.065027189523686, 4.01379891209519, 0.0), Point3d(19.009685241927738, 30.807932065847332, 0.0), Point3d(14.234418043971875, 26.012897887101527, 2.392580447652291), Point3d(14.248223537950237, 19.328885855734843, 2.3925804476459653), Point3d(14.19596815545603, 19.276414164692916, 2.4187622718784856)});
  expectedRoofPolygons.push_back({Point3d(14.234418043971875, 26.012897887101527, 2.392580447652291), Point3d(19.009685241927738, 30.807932065847332, 0.0), Point3d(9.439383865135643, 30.78816508512935, 0.0)});
  expectedRoofPolygons.push_back({Point3d(14.234418043971875, 26.012897887101527, 2.392580447652291), Point3d(9.439383865135643, 30.78816508512935, 0.0), Point3d(9.453189359125524, 24.10415305431124, 0.0), Point3d(14.248223537950237, 19.328885855734843, 2.3925804476459653)});
  expectedRoofPolygons.push_back({Point3d(14.19596815545603, 19.276414164692916, 2.4187622718784856), Point3d(14.248223537950237, 19.328885855734843, 2.3925804476459653), Point3d(9.453189359125524, 24.10415305431124, 0.0), Point3d(-0.01730198014624129, 24.08459222736407, 0.0), Point3d(-3.101535247093261, 20.98759193064646, 1.5453116872475992), Point3d(12.445462580412869, 21.019703480686516, 1.5453116860600677)});
  expectedRoofPolygons.push_back({Point3d(-5.2088937947536875, 23.086263132900534, 2.5968213677080576), Point3d(-3.101535247093261, 20.98759193064646, 1.5453116872475992), Point3d(-0.01730198014624129, 24.08459222736407, 0.0), Point3d(-0.030597953439544412, 30.521916694234474, 0.0), Point3d(-5.213502422879822, 25.317557848847482, 2.596821367680282)});
  expectedRoofPolygons.push_back({Point3d(-5.213502422879822, 25.317557848847482, 2.596821367680282), Point3d(-0.030597953439544412, 30.521916694234474, 0.0), Point3d(-10.417861267451112, 30.500462317733504, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-5.213502422879822, 25.317557848847482, 2.596821367680282), Point3d(-10.417861267451112, 30.500462317733504, 0.0), Point3d(-10.354819907553885, -0.021387367337700525, 0.0), Point3d(-5.188103636084188, 5.166716270119771, 2.5887104987054257), Point3d(-5.225081993006609, 23.07000792440424, 2.5887104987207548), Point3d(-5.2088937947536875, 23.086263132900534, 2.5968213677080576)});
  expectedRoofPolygons.push_back({Point3d(-5.188103636084188, 5.166716270119771, 2.5887104987054257), Point3d(-10.354819907553885, -0.021387367337700525, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-5.188103636084188, 5.166716270119771, 2.5887104987054257), Point3d(0.0, 0.0, 0.0), Point3d(-0.03697835689094475, 17.903291653889664, 0.0), Point3d(-5.225081993006609, 23.07000792440424, 2.5887104987207548)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofCircularAddTest) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(50.0, 50.0, 0.0));
  footprint.push_back(Point3d(100.0, 50.0, 0.0));
  footprint.push_back(Point3d(100.0, 100.0, 0.0));
  footprint.push_back(Point3d(50.0, 100.0, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(50.0, 50.0, 0.0), Point3d(100.0, 50.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(100.0, 50.0, 0.0), Point3d(100.0, 100.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(100.0, 100.0, 0.0), Point3d(50.0, 100.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(50.0, 100.0, 0.0), Point3d(50.0, 50.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(100.0, 50.0, 0.0), Point3d(100.0, 100.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(100.0, 100.0, 0.0), Point3d(50.0, 100.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 100.0, 0.0), Point3d(50.0, 50.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 50.0, 0.0), Point3d(100.0, 50.0, 0.0)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofCircularAddTest2) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(50.0, 50.0, 0.0));
  footprint.push_back(Point3d(150.0, 50.0, 0.0));
  footprint.push_back(Point3d(150.0, 100.0, 0.0));
  footprint.push_back(Point3d(50.0, 100.0, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(50.0, 50.0, 0.0), Point3d(150.0, 50.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(150.0, 50.0, 0.0), Point3d(150.0, 100.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(150.0, 100.0, 0.0), Point3d(50.0, 100.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(50.0, 100.0, 0.0), Point3d(50.0, 50.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(125.0, 75.0, 12.5), Point3d(150.0, 50.0, 0.0), Point3d(150.0, 100.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(125.0, 75.0, 12.5), Point3d(150.0, 100.0, 0.0), Point3d(50.0, 100.0, 0.0), Point3d(75.0, 75.0, 12.5)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 100.0, 0.0), Point3d(50.0, 50.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 50.0, 0.0), Point3d(150.0, 50.0, 0.0), Point3d(125.0, 75.0, 12.5)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofCircularAddTest2Clockwise) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint;
  footprint.push_back(Point3d(50.0, 100.0, 0.0));
  footprint.push_back(Point3d(150.0, 100.0, 0.0));
  footprint.push_back(Point3d(150.0, 50.0, 0.0));
  footprint.push_back(Point3d(50.0, 50.0, 0.0));

  std::vector< std::vector<Point3d> > roofEdges = generateSkeletonRoofEdges(footprint);
  std::vector< std::vector<Point3d> > roofPolygons; // = generateHipRoof(footprint, pitch);

  std::vector< std::vector<Point3d> > expectedRoofEdges;
  expectedRoofEdges.push_back({Point3d(50.0, 50.0, 0.0), Point3d(150.0, 50.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(150.0, 50.0, 0.0), Point3d(150.0, 100.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(150.0, 100.0, 0.0), Point3d(50.0, 100.0, 0.0)});
  expectedRoofEdges.push_back({Point3d(50.0, 100.0, 0.0), Point3d(50.0, 50.0, 0.0)});

  EXPECT_EQ(expectedRoofEdges.size(), roofEdges.size());

  for (int i = 0; i < roofEdges.size(); ++i) {
    matches(expectedRoofEdges, roofEdges[i]);
  }

  std::vector< std::vector<Point3d> > expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(125.0, 75.0, 12.5), Point3d(150.0, 50.0, 0.0), Point3d(150.0, 100.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(125.0, 75.0, 12.5), Point3d(150.0, 100.0, 0.0), Point3d(50.0, 100.0, 0.0), Point3d(75.0, 75.0, 12.5)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 100.0, 0.0), Point3d(50.0, 50.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 50.0, 0.0), Point3d(150.0, 50.0, 0.0), Point3d(125.0, 75.0, 12.5)});

  //EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (int i = 0; i < roofPolygons.size(); ++i) {
    //matches(expectedRoofPolygons, roofPolygons[i]);
  }
}
