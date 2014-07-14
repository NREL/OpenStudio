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
#include "GeometryFixture.hpp"

#include "../Geometry.hpp"
#include "../Point3d.hpp"
#include "../Vector3d.hpp"

using namespace std;
using namespace boost;
using namespace openstudio;

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
  // expected test values from http://www.nhc.noaa.gov/gccalc.shtml

  // using 10 km threshold

  // San Francisco to New York
  EXPECT_NEAR( 4138000, getDistanceLatLon(37.62, -122.38, 40.78, -73.88), 10000);

  // Anchorage to New York
  EXPECT_NEAR( 5414000, getDistanceLatLon(61.17, -150.02, 40.78, -73.88), 10000);

  // Anchorage to San Francisco
  EXPECT_NEAR( 3242000, getDistanceLatLon(61.17, -150.02, 37.62, -122.38), 10000);
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
