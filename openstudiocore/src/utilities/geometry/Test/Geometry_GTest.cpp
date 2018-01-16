/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "GeometryFixture.hpp"

#include "../Geometry.hpp"
#include "../Point3d.hpp"
#include "../PointLatLon.hpp"
#include "../Vector3d.hpp"

using namespace std;
using namespace boost;
using namespace openstudio;

// In Intersection_GTest.cpp
std::vector<Point3d> makeRectangleUp(double xmin, double ymin, double width, double height);
std::vector<Point3d> makeRectangleDown(double xmin, double ymin, double width, double height);

TEST_F(GeometryFixture, Newall_Vector)
{
  Point3dVector points;
  OptionalVector3d newall;
  OptionalVector3d normal;

  // triangle with unit area
  points.clear();
  points.push_back(Point3d(0, 2, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));

  newall = getNewallVector(points);
  ASSERT_TRUE(newall);
  EXPECT_EQ(0, newall->x());
  EXPECT_EQ(0, newall->y());
  EXPECT_EQ(2, newall->z());
  EXPECT_EQ(2, newall->length());

  normal = getOutwardNormal(points);
  ASSERT_TRUE(normal);
  EXPECT_EQ(0, normal->x());
  EXPECT_EQ(0, normal->y());
  EXPECT_EQ(1, normal->z());
  EXPECT_EQ(1, normal->length());

  // square with unit area
  points.clear();
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(1, 1, 0));

  newall = getNewallVector(points);
  ASSERT_TRUE(newall);
  EXPECT_EQ(0, newall->x());
  EXPECT_EQ(0, newall->y());
  EXPECT_EQ(2, newall->z());
  EXPECT_EQ(2, newall->length());

  normal = getOutwardNormal(points);
  ASSERT_TRUE(normal);
  EXPECT_EQ(0, normal->x());
  EXPECT_EQ(0, normal->y());
  EXPECT_EQ(1, normal->z());
  EXPECT_EQ(1, normal->length());

  // u shape (concave) with 5 unit area, includes collinear points
  points.clear();
  points.push_back(Point3d(0, 0, 2));
  points.push_back(Point3d(0, 0, 1));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(3, 0, 0));
  points.push_back(Point3d(3, 0, 1));
  points.push_back(Point3d(3, 0, 2));
  points.push_back(Point3d(2, 0, 2));
  points.push_back(Point3d(2, 0, 1));
  points.push_back(Point3d(1, 0, 1));
  points.push_back(Point3d(1, 0, 2));

  newall = getNewallVector(points);
  ASSERT_TRUE(newall);
  EXPECT_EQ(0, newall->x());
  EXPECT_EQ(-10, newall->y());
  EXPECT_EQ(0, newall->z());
  EXPECT_EQ(10, newall->length());

  normal = getOutwardNormal(points);
  ASSERT_TRUE(normal);
  EXPECT_EQ(0, normal->x());
  EXPECT_EQ(-1, normal->y());
  EXPECT_EQ(0, normal->z());
  EXPECT_EQ(1, normal->length());

  // u shape (concave) with 5 unit area, does not includes collinear points
  points.clear();
  points.push_back(Point3d(0, 0, 2));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(3, 0, 0));
  points.push_back(Point3d(3, 0, 2));
  points.push_back(Point3d(2, 0, 2));
  points.push_back(Point3d(2, 0, 1));
  points.push_back(Point3d(1, 0, 1));
  points.push_back(Point3d(1, 0, 2));

  newall = getNewallVector(points);
  ASSERT_TRUE(newall);
  EXPECT_EQ(0, newall->x());
  EXPECT_EQ(-10, newall->y());
  EXPECT_EQ(0, newall->z());
  EXPECT_EQ(10, newall->length());

  normal = getOutwardNormal(points);
  ASSERT_TRUE(normal);
  EXPECT_EQ(0, normal->x());
  EXPECT_EQ(-1, normal->y());
  EXPECT_EQ(0, normal->z());
  EXPECT_EQ(1, normal->length());
}

TEST_F(GeometryFixture, DistanceLatLon)
{
  // expected test values from http://williams.best.vwh.net/gccalc.htm

  // using 10 m threshold

  // San Francisco to New York
  EXPECT_NEAR( 4150765, getDistanceLatLon(37.62, -122.38, 40.78, -73.88), 10);

  // Anchorage to New York
  EXPECT_NEAR( 5432233, getDistanceLatLon(61.17, -150.02, 40.78, -73.88), 10);

  // Anchorage to San Francisco
  EXPECT_NEAR( 3248030, getDistanceLatLon(61.17, -150.02, 37.62, -122.38), 10);
}

TEST_F(GeometryFixture, Centroid)
{
  Point3dVector points;
  OptionalPoint3d c;

  c = getCentroid(points);
  EXPECT_FALSE(c);

  // square
  points.clear();
  points.push_back(Point3d(0,0,1));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(1,0,1));

  c = getCentroid(points);
  ASSERT_TRUE(c);
  EXPECT_EQ(0.5, c->x());
  EXPECT_EQ(0.0, c->y());
  EXPECT_EQ(0.5, c->z());

  // large rectangle
  points.clear();
  points.push_back(Point3d(3,0,2));
  points.push_back(Point3d(3,0,1));
  points.push_back(Point3d(3,100,1));
  points.push_back(Point3d(3,100,2));

  c = getCentroid(points);
  ASSERT_TRUE(c);
  EXPECT_EQ(3.0, c->x());
  EXPECT_EQ(50.0, c->y());
  EXPECT_EQ(1.5, c->z());

  // L
  points.clear();
  points.push_back(Point3d(0,0,2));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(2,0,0));
  points.push_back(Point3d(2,0,1));
  points.push_back(Point3d(1,0,1));
  points.push_back(Point3d(1,0,2));

  c = getCentroid(points);
  ASSERT_TRUE(c);
  EXPECT_EQ(2.5/3.0, c->x());
  EXPECT_EQ(0.0, c->y());
  EXPECT_EQ(2.5/3.0, c->z());

  // 3x3 square on x, y plane at z = 1 with center missing
  points.clear();
  points.push_back(Point3d(0,3,1));
  points.push_back(Point3d(0,0,1));
  points.push_back(Point3d(3,0,1));
  points.push_back(Point3d(3,3,1));
  points.push_back(Point3d(1,3,1));
  points.push_back(Point3d(1,2,1));
  points.push_back(Point3d(2,2,1));
  points.push_back(Point3d(2,1,1));
  points.push_back(Point3d(1,1,1));
  points.push_back(Point3d(1,3,1));

  c = getCentroid(points);
  ASSERT_TRUE(c);
  EXPECT_EQ(1.5, c->x());
  EXPECT_EQ(1.5, c->y());
  EXPECT_EQ(1.0, c->z());
}

TEST_F(GeometryFixture, ReorderULC)
{
  Point3dVector points(4);

  points[0] = Point3d(0, 0, 1);
  points[1] = Point3d(0, 0, 0);
  points[2] = Point3d(1, 0, 0);
  points[3] = Point3d(1, 0, 1);
  points = reorderULC(points);
  EXPECT_EQ(points[0], Point3d(0, 0, 1));
  EXPECT_EQ(points[1], Point3d(0, 0, 0));
  EXPECT_EQ(points[2], Point3d(1, 0, 0));
  EXPECT_EQ(points[3], Point3d(1, 0, 1));

  points[1] = Point3d(0, 0, 1);
  points[2] = Point3d(0, 0, 0);
  points[3] = Point3d(1, 0, 0);
  points[0] = Point3d(1, 0, 1);
  points = reorderULC(points);
  EXPECT_EQ(points[0], Point3d(0, 0, 1));
  EXPECT_EQ(points[1], Point3d(0, 0, 0));
  EXPECT_EQ(points[2], Point3d(1, 0, 0));
  EXPECT_EQ(points[3], Point3d(1, 0, 1));

  points[2] = Point3d(0, 0, 1);
  points[3] = Point3d(0, 0, 0);
  points[0] = Point3d(1, 0, 0);
  points[1] = Point3d(1, 0, 1);
  points = reorderULC(points);
  EXPECT_EQ(points[0], Point3d(0, 0, 1));
  EXPECT_EQ(points[1], Point3d(0, 0, 0));
  EXPECT_EQ(points[2], Point3d(1, 0, 0));
  EXPECT_EQ(points[3], Point3d(1, 0, 1));

  points[3] = Point3d(0, 0, 1);
  points[0] = Point3d(0, 0, 0);
  points[1] = Point3d(1, 0, 0);
  points[2] = Point3d(1, 0, 1);
  points = reorderULC(points);
  EXPECT_EQ(points[0], Point3d(0, 0, 1));
  EXPECT_EQ(points[1], Point3d(0, 0, 0));
  EXPECT_EQ(points[2], Point3d(1, 0, 0));
  EXPECT_EQ(points[3], Point3d(1, 0, 1));

}

TEST_F(GeometryFixture, GetDistance)
{
  Point3d point(0,0,0);

  EXPECT_EQ(0, getDistance(point, Point3d(0,0,0)));

  EXPECT_EQ(1, getDistance(point, Point3d(1,0,0)));

  EXPECT_EQ(1, getDistance(point, Point3d(0,1,0)));

  EXPECT_EQ(1, getDistance(point, Point3d(0,0,1)));

  EXPECT_EQ(1, getDistance(point, Point3d(-1,0,0)));

  EXPECT_EQ(1, getDistance(point, Point3d(0,-1,0)));

  EXPECT_EQ(1, getDistance(point, Point3d(0,0,-1)));

  EXPECT_EQ(5, getDistance(point, Point3d(3,4,0)));

  EXPECT_EQ(5, getDistance(point, Point3d(0,-4,-3)));

  EXPECT_EQ(5, getDistance(point, Point3d(-4,0,3)));

}

TEST_F(GeometryFixture, GetDistancePointToLineSegment)
{
  Point3d point1(0,0,0);
  Point3d point2(1,0,0);
  std::vector<Point3d> line;
  line.push_back(point1);
  line.push_back(point2);
  EXPECT_EQ(2u, line.size());

  EXPECT_DOUBLE_EQ(0, getDistancePointToLineSegment(Point3d(0,0,0), line));

  EXPECT_DOUBLE_EQ(0, getDistancePointToLineSegment(Point3d(1,0,0), line));

  EXPECT_DOUBLE_EQ(0, getDistancePointToLineSegment(Point3d(0.5,0,0), line));

  EXPECT_DOUBLE_EQ(1, getDistancePointToLineSegment(Point3d(2,0,0), line));

  EXPECT_DOUBLE_EQ(1, getDistancePointToLineSegment(Point3d(-1,0,0), line));

  EXPECT_DOUBLE_EQ(1, getDistancePointToLineSegment(Point3d(0,1,0), line));

  EXPECT_DOUBLE_EQ(1, getDistancePointToLineSegment(Point3d(1,1,0), line));

  EXPECT_DOUBLE_EQ(1, getDistancePointToLineSegment(Point3d(0.5,1,0), line));

  EXPECT_DOUBLE_EQ(std::sqrt(2.0), getDistancePointToLineSegment(Point3d(2,1,0), line));

  EXPECT_DOUBLE_EQ(std::sqrt(2.0), getDistancePointToLineSegment(Point3d(-1,1,0), line));

  line.push_back(Point3d(2,0,0));
  EXPECT_EQ(3u, line.size());
  EXPECT_EQ(0, getDistancePointToLineSegment(Point3d(3,0,0), line));
}

TEST_F(GeometryFixture, GetDistancePointToTriangle)
{
  Point3d point1(0,0,0);
  Point3d point2(1,0,0);
  Point3d point3(0,1,0);
  std::vector<Point3d> triangle;
  triangle.push_back(point1);
  triangle.push_back(point2);
  triangle.push_back(point3);
  EXPECT_EQ(3u, triangle.size());

  EXPECT_DOUBLE_EQ(0, getDistancePointToTriangle(Point3d(0,0,0), triangle));
  EXPECT_DOUBLE_EQ(0, getDistancePointToTriangle(Point3d(1,0,0), triangle));
  EXPECT_DOUBLE_EQ(0, getDistancePointToTriangle(Point3d(0.5,0,0), triangle));
  EXPECT_DOUBLE_EQ(0, getDistancePointToTriangle(Point3d(0,1,0), triangle));
  EXPECT_DOUBLE_EQ(0, getDistancePointToTriangle(Point3d(0,0.5,0), triangle));
  EXPECT_DOUBLE_EQ(0, getDistancePointToTriangle(Point3d(0.1,0.1,0), triangle));
  EXPECT_DOUBLE_EQ(0, getDistancePointToTriangle(Point3d(0.5,0.5,0), triangle));

  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(2,0,0), triangle));
  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(-1,0,0), triangle));
  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(-1,1,0), triangle));
  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(0,2,0), triangle));
  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(0,-1,0), triangle));
  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(1,-1,0), triangle));
  EXPECT_DOUBLE_EQ(std::sqrt(2.0), getDistancePointToTriangle(Point3d(-1,-1,0), triangle));
  EXPECT_DOUBLE_EQ(std::sqrt(2.0)/2.0, getDistancePointToTriangle(Point3d(1,1,0), triangle));

  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(0,0,1), triangle));
  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(1,0,1), triangle));
  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(0.5,0,1), triangle));
  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(0,1,1), triangle));
  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(0,0.5,1), triangle));
  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(0.1,0.1,1), triangle));
  EXPECT_DOUBLE_EQ(1, getDistancePointToTriangle(Point3d(0.5,0.5,1), triangle));

  EXPECT_DOUBLE_EQ(std::sqrt(2.0), getDistancePointToTriangle(Point3d(2,0,1), triangle));
  EXPECT_DOUBLE_EQ(std::sqrt(2.0), getDistancePointToTriangle(Point3d(-1,0,1), triangle));
  EXPECT_DOUBLE_EQ(std::sqrt(2.0), getDistancePointToTriangle(Point3d(-1,1,1), triangle));
  EXPECT_DOUBLE_EQ(std::sqrt(2.0), getDistancePointToTriangle(Point3d(0,2,1), triangle));
  EXPECT_DOUBLE_EQ(std::sqrt(2.0), getDistancePointToTriangle(Point3d(0,-1,1), triangle));
  EXPECT_DOUBLE_EQ(std::sqrt(2.0), getDistancePointToTriangle(Point3d(1,-1,1), triangle));
  EXPECT_DOUBLE_EQ(std::sqrt(3.0), getDistancePointToTriangle(Point3d(-1,-1,1), triangle));
  EXPECT_DOUBLE_EQ(std::sqrt(6.0)/2.0, getDistancePointToTriangle(Point3d(1,1,1), triangle));
}


TEST_F(GeometryFixture, CircularEqual)
{
  Point3dVector points1;
  Point3dVector points2;

  EXPECT_TRUE(circularEqual(points1, points2));

  points1.push_back(Point3d(0,0,0));

  EXPECT_FALSE(circularEqual(points1, points2));

  points2.push_back(Point3d(0,0,0));

  EXPECT_TRUE(circularEqual(points1, points2));

  points1.clear();
  points1.push_back(Point3d(0,0,0));
  points1.push_back(Point3d(1,0,0));
  points1.push_back(Point3d(1,1,0));
  points1.push_back(Point3d(0,1,0));

  points2.clear();
  points2.push_back(Point3d(0,0,0));
  points2.push_back(Point3d(1,0,0));
  points2.push_back(Point3d(1,1,0));
  points2.push_back(Point3d(0,1,0));

  EXPECT_TRUE(circularEqual(points1, points2));

  points2.clear();
  points2.push_back(Point3d(1,0,0));
  points2.push_back(Point3d(1,1,0));
  points2.push_back(Point3d(0,1,0));
  points2.push_back(Point3d(0,0,0));

  EXPECT_TRUE(circularEqual(points1, points2));

  points2.clear();
  points2.push_back(Point3d(1,1,0));
  points2.push_back(Point3d(0,1,0));
  points2.push_back(Point3d(0,0,0));
  points2.push_back(Point3d(1,0,0));

  EXPECT_TRUE(circularEqual(points1, points2));

  points2.clear();
  points2.push_back(Point3d(0,1,0));
  points2.push_back(Point3d(0,0,0));
  points2.push_back(Point3d(1,0,0));
  points2.push_back(Point3d(1,1,0));

  EXPECT_TRUE(circularEqual(points1, points2));

  points2.clear();
  points2.push_back(Point3d(0,0,0.1));
  points2.push_back(Point3d(1,0,0.1));
  points2.push_back(Point3d(1,1,0.1));
  points2.push_back(Point3d(0,1,0.1));

  EXPECT_FALSE(circularEqual(points1, points2));

  points2.clear();
  points2.push_back(Point3d(0,0,0));
  points2.push_back(Point3d(0,1,0));
  points2.push_back(Point3d(1,1,0));
  points2.push_back(Point3d(1,0,0));

  EXPECT_FALSE(circularEqual(points1, points2));

  points2.clear();
  points2.push_back(Point3d(0,0,0));
  points2.push_back(Point3d(1,0,0));
  points2.push_back(Point3d(1.1,1.1,0));
  points2.push_back(Point3d(0,1,0));

  EXPECT_FALSE(circularEqual(points1, points2));

  points2.clear();
  points2.push_back(Point3d(1,0,0));
  points2.push_back(Point3d(1.1,1.1,0));
  points2.push_back(Point3d(0,1,0));
  points2.push_back(Point3d(0,0,0));

  EXPECT_FALSE(circularEqual(points1, points2));

  points2.clear();
  points2.push_back(Point3d(1.1,1.1,0));
  points2.push_back(Point3d(0,1,0));
  points2.push_back(Point3d(0,0,0));
  points2.push_back(Point3d(1,0,0));

  EXPECT_FALSE(circularEqual(points1, points2));

  points2.clear();
  points2.push_back(Point3d(0,1,0));
  points2.push_back(Point3d(0,0,0));
  points2.push_back(Point3d(1,0,0));
  points2.push_back(Point3d(1.1,1.1,0));

  EXPECT_FALSE(circularEqual(points1, points2));
}


TEST_F(GeometryFixture, ReorderULC2)
{
  Point3dVector points1;
  points1.push_back(Point3d(23.765661384,-48.74745648,12.954));
  points1.push_back(Point3d(34.4951304,-41.77963704,12.954));
  points1.push_back(Point3d(34.4951304,-41.77963704,16.06061304));
  points1.push_back(Point3d(23.765661384,-48.74745648,16.06061304));

  Point3dVector points2 = reorderULC(points1);
  ASSERT_EQ(4u, points2.size());
  EXPECT_EQ(points1[3], points2[0]);
  EXPECT_EQ(points1[0], points2[1]);
  EXPECT_EQ(points1[1], points2[2]);
  EXPECT_EQ(points1[2], points2[3]);
}

TEST_F(GeometryFixture, RemoveCollinear)
{
  Point3dVector points;
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(0,0,0));

  Point3dVector testPoints = removeCollinear(points);
  ASSERT_EQ(1u, testPoints.size());
  EXPECT_TRUE(pointEqual(Point3d(0,0,0), testPoints[0]));

  points.clear();
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(2,0,0));

  testPoints = removeCollinear(points);
  ASSERT_EQ(2u, testPoints.size());
  EXPECT_TRUE(pointEqual(Point3d(0,0,0), testPoints[0]));
  EXPECT_TRUE(pointEqual(Point3d(2,0,0), testPoints[1]));

  points.clear();
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(2,0,0));
  points.push_back(Point3d(2,1,0));
  points.push_back(Point3d(2,2,0));
  points.push_back(Point3d(1,2,0));
  points.push_back(Point3d(0,2,0));
  points.push_back(Point3d(0,1,0));

  testPoints = removeCollinear(points);
  ASSERT_EQ(4u, testPoints.size());
  EXPECT_TRUE(pointEqual(Point3d(0,0,0), testPoints[0]));
  EXPECT_TRUE(pointEqual(Point3d(2,0,0), testPoints[1]));
  EXPECT_TRUE(pointEqual(Point3d(2,2,0), testPoints[2]));
  EXPECT_TRUE(pointEqual(Point3d(0,2,0), testPoints[3]));

  points.clear();
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(2,0,0));
  points.push_back(Point3d(2,2,0));
  points.push_back(Point3d(0,2,0));

  testPoints = removeCollinear(points);
  ASSERT_EQ(4u, testPoints.size());
  EXPECT_TRUE(pointEqual(Point3d(0,0,0), testPoints[0]));
  EXPECT_TRUE(pointEqual(Point3d(2,0,0), testPoints[1]));
  EXPECT_TRUE(pointEqual(Point3d(2,2,0), testPoints[2]));
  EXPECT_TRUE(pointEqual(Point3d(0,2,0), testPoints[3]));
}
/*
TEST_F(GeometryFixture, RemoveSpikes)
{
  Point3dVector points;
  Point3dVector resultPoints;
  Point3dVector testPoints;

  points.clear();
  points.push_back(Point3d(3,2,0));
  points.push_back(Point3d(2,2,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(3,0,0));

  resultPoints.clear();
  resultPoints.push_back(Point3d(3,2,0));
  resultPoints.push_back(Point3d(2,2,0));
  resultPoints.push_back(Point3d(0,0,0));
  resultPoints.push_back(Point3d(3,0,0));

  testPoints = removeSpikes(points);
  ASSERT_EQ(4u, testPoints.size()) << testPoints;
  EXPECT_TRUE(circularEqual(resultPoints, testPoints));

  points.clear();
  points.push_back(Point3d(5,0,0)); // spike
  points.push_back(Point3d(3,2,0));
  points.push_back(Point3d(2,2,0));
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(5,0,0)); // spike
  points.push_back(Point3d(3,0,0));
  points.push_back(Point3d(3,2,0));

  resultPoints.clear();
  resultPoints.push_back(Point3d(3,2,0));
  resultPoints.push_back(Point3d(2,2,0));
  resultPoints.push_back(Point3d(0,0,0));
  resultPoints.push_back(Point3d(3,0,0));
  resultPoints.push_back(Point3d(3,2,0));

  testPoints = removeSpikes(points);
  ASSERT_EQ(5u, testPoints.size()) << testPoints;
  EXPECT_TRUE(circularEqual(resultPoints, testPoints));
}
*/

TEST_F(GeometryFixture, Triangulate_Down)
{
  double tol = 0.01;
  Vector3d normal(0, 0, -1);

  std::vector<std::vector<Point3d> > test;
  std::vector<std::vector<Point3d> > holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 4, 4);
  test = computeTriangulation(points1, holes, tol);
  EXPECT_FALSE(test.empty());
  EXPECT_DOUBLE_EQ(16.0, totalArea(test));
  EXPECT_TRUE(checkNormals(normal, test));

  // sense is down with a hole in middle
  points1 = makeRectangleDown(0, 0, 4, 4);
  points2 = makeRectangleDown(1, 1, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = computeTriangulation(points1, holes, tol);
  EXPECT_FALSE(test.empty());
  EXPECT_DOUBLE_EQ(15.0, totalArea(test));
  EXPECT_TRUE(checkNormals(normal, test));

  // sense is down with a hole on edge
  points1 = makeRectangleDown(0, 0, 4, 4);
  points2 = makeRectangleDown(1, 0, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = computeTriangulation(points1, holes, tol);
  EXPECT_FALSE(test.empty());
  EXPECT_DOUBLE_EQ(15.0, totalArea(test));
  EXPECT_TRUE(checkNormals(normal, test));

  // sense is down with hole same size
  points1 = makeRectangleDown(0, 0, 4, 4);
  points2 = makeRectangleDown(0, 0, 4, 4);

  holes.clear();
  holes.push_back(points2);
  test = computeTriangulation(points1, holes, tol);
  EXPECT_TRUE(test.empty());
  EXPECT_TRUE(checkNormals(normal, test));

  // sense is down with a bigger hole
  points1 = makeRectangleDown(1, 1, 1, 1);
  points2 = makeRectangleDown(0, 0, 4, 4);

  holes.clear();
  holes.push_back(points2);
  test = computeTriangulation(points1, holes, tol);
  EXPECT_TRUE(test.empty());
  EXPECT_TRUE(checkNormals(normal, test));
}

TEST_F(GeometryFixture, Triangulate_Up)
{
  double tol = 0.01;
  Vector3d normal(0, 0, 1);

  std::vector<std::vector<Point3d> > test;
  std::vector<std::vector<Point3d> > holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is up
  points1 = makeRectangleUp(0, 0, 4, 4);
  test = computeTriangulation(points1, holes, tol);
  EXPECT_TRUE(test.empty());
  EXPECT_TRUE(checkNormals(normal, test));

  // sense is up with a hole in middle
  points1 = makeRectangleUp(0, 0, 4, 4);
  points2 = makeRectangleUp(1, 1, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = computeTriangulation(points1, holes, tol);
  EXPECT_TRUE(test.empty());
  EXPECT_TRUE(checkNormals(normal, test));

  // sense is up with a hole on edge
  points1 = makeRectangleUp(0, 0, 4, 4);
  points2 = makeRectangleUp(1, 0, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = computeTriangulation(points1, holes, tol);
  EXPECT_TRUE(test.empty());
  EXPECT_TRUE(checkNormals(normal, test));

  // sense is up with hole same size
  points1 = makeRectangleUp(0, 0, 4, 4);
  points2 = makeRectangleUp(0, 0, 4, 4);

  holes.clear();
  holes.push_back(points2);
  test = computeTriangulation(points1, holes, tol);
  EXPECT_TRUE(test.empty());
  EXPECT_TRUE(checkNormals(normal, test));

  // sense is up with a bigger hole
  points1 = makeRectangleUp(1, 1, 1, 1);
  points2 = makeRectangleUp(0, 0, 4, 4);

  holes.clear();
  holes.push_back(points2);
  test = computeTriangulation(points1, holes, tol);
  EXPECT_TRUE(test.empty());
  EXPECT_TRUE(checkNormals(normal, test));
}

TEST_F(GeometryFixture, PointLatLon)
{
  // building in Portland
  PointLatLon origin(45.521272355398, -122.686472758865);
  EXPECT_TRUE(origin == origin);

  PointLatLon originCopy(45.521272355398, -122.686472758865);
  EXPECT_TRUE(origin == originCopy);

  Point3d localOrigin = origin.toLocalCartesian(origin);
  EXPECT_DOUBLE_EQ(0, localOrigin.x());
  EXPECT_DOUBLE_EQ(0, localOrigin.y());
  EXPECT_DOUBLE_EQ(0, localOrigin.z());

  PointLatLon origin2 = origin.fromLocalCartesian(localOrigin);
  EXPECT_DOUBLE_EQ(origin.lon(), origin2.lon());
  EXPECT_DOUBLE_EQ(origin.lat(), origin2.lat());
  EXPECT_NEAR(origin.height(), origin2.height(), 0.001);
  //EXPECT_TRUE(origin == origin2);

  EXPECT_DOUBLE_EQ(0, (origin - origin2));

  PointLatLonVector footprint;
  footprint.push_back(PointLatLon(45.521272355398, -122.686472758865));
  footprint.push_back(PointLatLon(45.5214185583437, -122.687017007901));
  footprint.push_back(PointLatLon(45.5216756691633, -122.686878595312));
  footprint.push_back(PointLatLon(45.5215377823024, -122.686365888764));
  footprint.push_back(PointLatLon(45.5214801020189, -122.686152903546));
  footprint.push_back(PointLatLon(45.5212238483817, -122.686291351916));
  footprint.push_back(PointLatLon(45.521272355398, -122.686472758865));

  Point3dVector localFootprint = origin.toLocalCartesian(footprint);
  ASSERT_EQ(footprint.size(), localFootprint.size());

  double expectedArea = 1853.0906095305727; // from GIS
  boost::optional<double> calcArea = getArea(localFootprint);
  ASSERT_TRUE(calcArea);
  EXPECT_NEAR(expectedArea, *calcArea, 0.5);

  PointLatLonVector footprint2 = origin.fromLocalCartesian(localFootprint);
  ASSERT_EQ(footprint2.size(), localFootprint.size());

  unsigned n = footprint.size();
  for (unsigned i = 0; i < n; ++i){
    EXPECT_DOUBLE_EQ(footprint2[i].lat(), footprint[i].lat());
    EXPECT_DOUBLE_EQ(footprint2[i].lon(), footprint[i].lon());
    EXPECT_NEAR(footprint2[i].height(), footprint[i].height(), 0.001);

    double localDistance = getDistance(localFootprint[i], localOrigin);
    double geodesicDistance = (footprint[i] - origin);
    EXPECT_NEAR(localDistance, geodesicDistance, 0.001);
  }
}

TEST_F(GeometryFixture, PointLatLon_Elevation)
{
  // building in Portland
  PointLatLon origin(45.521272355398, -122.686472758865);
  EXPECT_TRUE(origin == origin);

  Point3d localOrigin = origin.toLocalCartesian(origin);
  EXPECT_DOUBLE_EQ(0.0, localOrigin.x());
  EXPECT_DOUBLE_EQ(0.0, localOrigin.y());
  EXPECT_DOUBLE_EQ(0.0, localOrigin.z());

  PointLatLon testLatLon(45.521272355398, -122.686472758865, 30.0);
  EXPECT_FALSE(origin == testLatLon);
  EXPECT_DOUBLE_EQ(origin.lat(), testLatLon.lat());
  EXPECT_DOUBLE_EQ(origin.lon(), testLatLon.lon());
  EXPECT_DOUBLE_EQ(30.0, testLatLon.height());

  Point3d test = origin.toLocalCartesian(testLatLon);
  EXPECT_NEAR(0.0, test.x(), 0.001);
  EXPECT_NEAR(0.0, test.y(), 0.001);
  EXPECT_NEAR(30.0, test.z(), 0.001);

  // calculated distance using:
  // http://www.movable-type.co.uk/scripts/latlong.html
  testLatLon = PointLatLon(45.521272355398, -122.687017007901, 0.0);
  EXPECT_FALSE(origin == testLatLon);
  EXPECT_DOUBLE_EQ(origin.lat(), testLatLon.lat());
  EXPECT_DOUBLE_EQ(-122.687017007901, testLatLon.lon());
  EXPECT_DOUBLE_EQ(0.0, testLatLon.height());

  test = origin.toLocalCartesian(testLatLon);
  EXPECT_NEAR(-42.521429845143913, test.x(), 0.001);
  EXPECT_NEAR(0.0, test.y(), 0.001);
  EXPECT_NEAR(0.0, test.z(), 0.001);

  // calculated distance using:
  // http://www.movable-type.co.uk/scripts/latlong.html
  testLatLon = PointLatLon(45.521272355398, -122.687017007901, 30.0);
  EXPECT_FALSE(origin == testLatLon);
  EXPECT_DOUBLE_EQ(origin.lat(), testLatLon.lat());
  EXPECT_DOUBLE_EQ(-122.687017007901, testLatLon.lon());
  EXPECT_DOUBLE_EQ(30.0, testLatLon.height());

  test = origin.toLocalCartesian(testLatLon);
  EXPECT_NEAR(-42.521429845143913, test.x(), 0.001);
  EXPECT_NEAR(0.0, test.y(), 0.001);
  EXPECT_NEAR(30.0, test.z(), 0.001);
}