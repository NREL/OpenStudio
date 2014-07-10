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

#include "../Plane.hpp"
#include "../Point3d.hpp"
#include "../Vector3d.hpp"
#include "../Transformation.hpp"

using namespace openstudio;

void checkPlaneFromPointAndNormal(const Plane& plane, const Point3d& point, const Vector3d& normal)
{ 
  Vector3d reverseNormal = normal.reverseVector();

  EXPECT_DOUBLE_EQ(1.0, normal.dot(plane.outwardNormal()));
  EXPECT_DOUBLE_EQ(-1.0, reverseNormal.dot(plane.outwardNormal()));
  EXPECT_TRUE(plane.pointOnPlane(point));
  EXPECT_FALSE(plane.pointOnPlane(point + normal));
  EXPECT_FALSE(plane.pointOnPlane(point + reverseNormal));

  Plane reversePlane = plane.reversePlane();
  EXPECT_DOUBLE_EQ(-1.0, normal.dot(reversePlane.outwardNormal()));
  EXPECT_DOUBLE_EQ(1.0, reverseNormal.dot(reversePlane.outwardNormal()));
  EXPECT_TRUE(reversePlane.pointOnPlane(point));
  EXPECT_FALSE(reversePlane.pointOnPlane(point + normal));
  EXPECT_FALSE(reversePlane.pointOnPlane(point + reverseNormal));

  EXPECT_TRUE(plane.parallel(plane));
  EXPECT_TRUE(plane.parallel(reversePlane));

  EXPECT_TRUE(plane.equal(plane));
  EXPECT_FALSE(plane.equal(reversePlane));
  EXPECT_FALSE(plane.reverseEqual(plane));
  EXPECT_TRUE(plane.reverseEqual(reversePlane));
}

TEST_F(GeometryFixture, Plane_FromPointAndNormal)
{
  Point3d point(0,0,0);
  Vector3d normal(0,0,1);
  Plane plane(point, normal);
  checkPlaneFromPointAndNormal(plane, point, normal);
  EXPECT_TRUE(plane.pointOnPlane(Point3d(0,0,0)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(0,0,1)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(1,0,0)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(1,0,1)));
  EXPECT_TRUE(pointEqual(Point3d(0,0,0), plane.project(Point3d(0,0,0))));
  EXPECT_TRUE(pointEqual(Point3d(0,0,0), plane.project(Point3d(0,0,1))));
  EXPECT_TRUE(pointEqual(Point3d(1,0,0), plane.project(Point3d(1,0,0))));
  EXPECT_TRUE(pointEqual(Point3d(1,0,0), plane.project(Point3d(1,0,1))));

  point = Point3d(0,0,1);
  normal = Vector3d(0,0,1);
  plane = Plane(point, normal);
  checkPlaneFromPointAndNormal(plane, point, normal);
  EXPECT_FALSE(plane.pointOnPlane(Point3d(0,0,0)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(0,0,1)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(1,0,0)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(1,0,1)));
  EXPECT_TRUE(pointEqual(Point3d(0,0,1), plane.project(Point3d(0,0,0))));
  EXPECT_TRUE(pointEqual(Point3d(0,0,1), plane.project(Point3d(0,0,1))));
  EXPECT_TRUE(pointEqual(Point3d(1,0,1), plane.project(Point3d(1,0,0))));
  EXPECT_TRUE(pointEqual(Point3d(1,0,1), plane.project(Point3d(1,0,1))));

  point = Point3d(0,0,1);
  normal = Vector3d(1,0,0);
  plane = Plane(point, normal);
  checkPlaneFromPointAndNormal(plane, point, normal);
  EXPECT_TRUE(plane.pointOnPlane(Point3d(0,0,0)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(0,0,1)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(1,0,0)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(1,0,1)));
  EXPECT_TRUE(pointEqual(Point3d(0,0,0), plane.project(Point3d(0,0,0))));
  EXPECT_TRUE(pointEqual(Point3d(0,0,1), plane.project(Point3d(0,0,1))));
  EXPECT_TRUE(pointEqual(Point3d(0,0,0), plane.project(Point3d(1,0,0))));
  EXPECT_TRUE(pointEqual(Point3d(0,0,1), plane.project(Point3d(1,0,1))));

  point = Point3d(1,0,1);
  normal = Vector3d(1,0,0);
  plane = Plane(point, normal);
  checkPlaneFromPointAndNormal(plane, point, normal);
  EXPECT_FALSE(plane.pointOnPlane(Point3d(0,0,0)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(0,0,1)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(1,0,0)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(1,0,1)));
  EXPECT_TRUE(pointEqual(Point3d(1,0,0), plane.project(Point3d(0,0,0))));
  EXPECT_TRUE(pointEqual(Point3d(1,0,1), plane.project(Point3d(0,0,1))));
  EXPECT_TRUE(pointEqual(Point3d(1,0,0), plane.project(Point3d(1,0,0))));
  EXPECT_TRUE(pointEqual(Point3d(1,0,1), plane.project(Point3d(1,0,1))));

  // throws if normal is undefined
  point = Point3d(0,0,0);
  normal = Vector3d(0,0,0);
  EXPECT_THROW(Plane(point, normal), openstudio::Exception);
}

TEST_F(GeometryFixture, Plane_FromPointAndNormal2)
{
  Point3d point1(0,0,0);
  Point3d point2(1,1,0);
  Vector3d normal(0,0,1);

  Plane plane1(point1, normal);
  Plane plane2(point2, normal);
  EXPECT_TRUE(plane1.parallel(plane2));
  EXPECT_TRUE(plane1.equal(plane2));
}

TEST_F(GeometryFixture, Plane_FromPointAndNormal3)
{
  Point3d point1(0,0,0);
  Point3d point2(0,0,1);
  Vector3d normal(0,0,1);

  Plane plane1(point1, normal);
  Plane plane2(point2, normal);
  EXPECT_TRUE(plane1.parallel(plane2));
  EXPECT_FALSE(plane1.equal(plane2));
}

TEST_F(GeometryFixture, Plane_FromPointsDegenerate)
{
  std::vector<Point3d> points;

  // size 0, throws if size points < 3
  EXPECT_THROW(Plane p(points), openstudio::Exception);

  // size 1, throws if size points < 3
  points.push_back(Point3d(0,0,0));
  EXPECT_THROW(Plane p(points), openstudio::Exception);

  // size 2, throws if size points < 3
  points.push_back(Point3d(0,0,0));
  EXPECT_THROW(Plane p(points), openstudio::Exception);

  // size 3, all points the same
  points.push_back(Point3d(0,0,0));
  EXPECT_THROW(Plane p(points), openstudio::Exception);

  // size 3, all points on a line
  points.clear();
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(2,0,0));
  EXPECT_THROW(Plane p(points), openstudio::Exception);
}

TEST_F(GeometryFixture, Plane_FromPoints1)
{
  std::vector<Point3d> points;
  points.push_back(Point3d(0,0,0));
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(1,1,0));
  points.push_back(Point3d(0,1,0));

  Plane p1(points);
  Plane p2(Point3d(0,0,0), Vector3d(0,0,1));

  EXPECT_TRUE(p1.equal(p2));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints2)
{
  std::vector<Point3d> points;
  points.push_back(Point3d(0,1,0));
  points.push_back(Point3d(1,1,0));
  points.push_back(Point3d(1,0,0));
  points.push_back(Point3d(0,0,0));
  
  Plane p1(points);
  Plane p2(Point3d(0,1,0), Vector3d(0,0,-1));

  EXPECT_TRUE(p1.equal(p2));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints3)
{
  std::vector<Point3d> points;
  points.push_back(Point3d(5,0,2));
  points.push_back(Point3d(5,0,0));
  points.push_back(Point3d(5,2,0));
  points.push_back(Point3d(5,2,2));

  Point3d point(5,0,2);
  Vector3d normal(1,0,0);
  
  Plane p1(points);
  Plane p2(point, normal);

  EXPECT_TRUE(p1.pointOnPlane(points[0]));
  EXPECT_TRUE(p1.pointOnPlane(points[1]));
  EXPECT_TRUE(p1.pointOnPlane(points[2]));
  EXPECT_TRUE(p1.pointOnPlane(points[3]));
  EXPECT_TRUE(p1.pointOnPlane(point));
  EXPECT_TRUE(p2.pointOnPlane(points[0]));
  EXPECT_TRUE(p2.pointOnPlane(points[1]));
  EXPECT_TRUE(p2.pointOnPlane(points[2]));
  EXPECT_TRUE(p2.pointOnPlane(points[3]));
  EXPECT_TRUE(p2.pointOnPlane(point));

  EXPECT_TRUE(p1.equal(p2));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints4)
{
  std::vector<Point3d> points;
  points.push_back(Point3d(5,-2,4));
  points.push_back(Point3d(5,-2,2));
  points.push_back(Point3d(7,-2,2));
  points.push_back(Point3d(7,-2,4));

  Point3d point(0,-2,0);
  Vector3d normal(0,-1,0);
  
  Plane p1(points);
  Plane p2(point, normal);

  EXPECT_TRUE(p1.pointOnPlane(points[0]));
  EXPECT_TRUE(p1.pointOnPlane(points[1]));
  EXPECT_TRUE(p1.pointOnPlane(points[2]));
  EXPECT_TRUE(p1.pointOnPlane(points[3]));
  EXPECT_TRUE(p1.pointOnPlane(point));
  EXPECT_TRUE(p2.pointOnPlane(points[0]));
  EXPECT_TRUE(p2.pointOnPlane(points[1]));
  EXPECT_TRUE(p2.pointOnPlane(points[2]));
  EXPECT_TRUE(p2.pointOnPlane(points[3]));
  EXPECT_TRUE(p2.pointOnPlane(point));

  EXPECT_TRUE(p1.equal(p2));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints4_Small)
{
  // points have area of 1 cm^2
  std::vector<Point3d> points;
  points.push_back(Point3d(0,       0, 0));
  points.push_back(Point3d(0.01,    0, 0));
  points.push_back(Point3d(0.01, 0.01, 0));
  points.push_back(Point3d(0,    0.01, 0));

  Point3d point(10,10,0);
  Vector3d normal(0,0,1);
  
  Plane p1(points);
  Plane p2(point, normal);

  EXPECT_TRUE(p1.pointOnPlane(points[0]));
  EXPECT_TRUE(p1.pointOnPlane(points[1]));
  EXPECT_TRUE(p1.pointOnPlane(points[2]));
  EXPECT_TRUE(p1.pointOnPlane(points[3]));
  EXPECT_TRUE(p1.pointOnPlane(point));
  EXPECT_TRUE(p2.pointOnPlane(points[0]));
  EXPECT_TRUE(p2.pointOnPlane(points[1]));
  EXPECT_TRUE(p2.pointOnPlane(points[2]));
  EXPECT_TRUE(p2.pointOnPlane(points[3]));
  EXPECT_TRUE(p2.pointOnPlane(point));

  EXPECT_TRUE(p1.equal(p2));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_NotEqual)
{
  std::vector<Point3d> points1;
  points1.push_back(Point3d(0,1,0));
  points1.push_back(Point3d(0,0,0));
  points1.push_back(Point3d(1,0,0));
  points1.push_back(Point3d(1,1,0));

  std::vector<Point3d> points2;
  points2.push_back(Point3d(0,1,2));
  points2.push_back(Point3d(0,0,2));
  points2.push_back(Point3d(1,0,2));
  points2.push_back(Point3d(1,1,2));
  
  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_Reverse)
{
  std::vector<Point3d> points1;
  points1.push_back(Point3d(1,1,2));
  points1.push_back(Point3d(1,0,2));
  points1.push_back(Point3d(0,0,2));
  points1.push_back(Point3d(0,1,2));

  std::vector<Point3d> points2;
  points2.push_back(Point3d(0,1,2));
  points2.push_back(Point3d(0,0,2));
  points2.push_back(Point3d(1,0,2));
  points2.push_back(Point3d(1,1,2));
  
  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_TRUE(p1.reverseEqual(p2));
  EXPECT_FALSE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_Separated_X)
{
  std::vector<Point3d> points1;
  points1.push_back(Point3d(10,0,1));
  points1.push_back(Point3d(10,0,0));
  points1.push_back(Point3d(10,1,0));
  points1.push_back(Point3d(10,1,1));

  std::vector<Point3d> points2;
  points2.push_back(Point3d(20,0,1));
  points2.push_back(Point3d(20,0,0));
  points2.push_back(Point3d(20,1,0));
  points2.push_back(Point3d(20,1,1));
  
  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(1,0,0), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(1,0,0), p2.outwardNormal()));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}


TEST_F(GeometryFixture, Plane_FromPoints_Reverse_Separated_X)
{
  std::vector<Point3d> points1;
  points1.push_back(Point3d(-10,0,1));
  points1.push_back(Point3d(-10,0,0));
  points1.push_back(Point3d(-10,1,0));
  points1.push_back(Point3d(-10,1,1));

  std::vector<Point3d> points2;
  points2.push_back(Point3d(-20,1,1));
  points2.push_back(Point3d(-20,1,0));
  points2.push_back(Point3d(-20,0,0));
  points2.push_back(Point3d(-20,0,1));
  
  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(1,0,0), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(-1,0,0), p2.outwardNormal()));
  EXPECT_FALSE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_Separated_Y)
{
  std::vector<Point3d> points1;
  points1.push_back(Point3d(0,-10,1));
  points1.push_back(Point3d(0,-10,0));
  points1.push_back(Point3d(1,-10,0));
  points1.push_back(Point3d(1,-10,1));

  std::vector<Point3d> points2;
  points2.push_back(Point3d(0,10,1));
  points2.push_back(Point3d(0,10,0));
  points2.push_back(Point3d(1,10,0));
  points2.push_back(Point3d(1,10,1));
  
  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(0,-1,0), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(0,-1,0), p2.outwardNormal()));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}


TEST_F(GeometryFixture, Plane_FromPoints_Reverse_Separated_Y)
{
  std::vector<Point3d> points1;
  points1.push_back(Point3d(0,-20,1));
  points1.push_back(Point3d(0,-20,0));
  points1.push_back(Point3d(1,-20,0));
  points1.push_back(Point3d(1,-20,1));

  std::vector<Point3d> points2;
  points2.push_back(Point3d(1,20,1));
  points2.push_back(Point3d(1,20,0));
  points2.push_back(Point3d(0,20,0));
  points2.push_back(Point3d(0,20,1));
  
  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(0,-1,0), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(0,1,0), p2.outwardNormal()));
  EXPECT_FALSE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_Separated_Z)
{
  std::vector<Point3d> points1;
  points1.push_back(Point3d(0,1,10));
  points1.push_back(Point3d(0,0,10));
  points1.push_back(Point3d(1,0,10));
  points1.push_back(Point3d(1,1,10));

  std::vector<Point3d> points2;
  points2.push_back(Point3d(0,1,-10));
  points2.push_back(Point3d(0,0,-10));
  points2.push_back(Point3d(1,0,-10));
  points2.push_back(Point3d(1,1,-10));
  
  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(0,0,1), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(0,0,1), p2.outwardNormal()));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}


TEST_F(GeometryFixture, Plane_FromPoints_Reverse_Separated_Z)
{
  std::vector<Point3d> points1;
  points1.push_back(Point3d(0,1,-10));
  points1.push_back(Point3d(0,0,-10));
  points1.push_back(Point3d(1,0,-10));
  points1.push_back(Point3d(1,1,-10));

  std::vector<Point3d> points2;
  points2.push_back(Point3d(1,1,-20));
  points2.push_back(Point3d(1,0,-20));
  points2.push_back(Point3d(0,0,-20));
  points2.push_back(Point3d(0,1,-20));
  
  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(0,0,1), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(0,0,-1), p2.outwardNormal()));
  EXPECT_FALSE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_ProblemSurface_IntersectModel22)
{
  // from Surface 1292
  std::vector<Point3d> points1;
  points1.push_back(Point3d(33.5280021336, -29.717998476, 91.44));
  points1.push_back(Point3d(33.5280021336, -29.717998476, 86.868));
  points1.push_back(Point3d(33.5280021336,  29.718001524, 86.868));
  points1.push_back(Point3d(33.5280021336,  29.718001524, 91.44));

  // from Surface 1384
  std::vector<Point3d> points2;
  points2.push_back(Point3d(28.9560021336, -25.145998476, 96.012));
  points2.push_back(Point3d(28.9560021336, -25.145998476, 91.44));
  points2.push_back(Point3d(28.9560021336,  25.146001524, 91.44));
  points2.push_back(Point3d(28.9560021336,  25.146001524, 96.012));

  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(1,0,0), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(1,0,0), p2.outwardNormal()));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));

  OptionalVector3d n1 = getOutwardNormal(points1);
  OptionalVector3d n2 = getOutwardNormal(points2);
  ASSERT_TRUE(n1);
  ASSERT_TRUE(n2);
  EXPECT_TRUE(vectorEqual(*n1, p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(*n2, p2.outwardNormal()));
}




TEST_F(GeometryFixture, Plane_Transformation)
{
  // translation by 1 meter in x then rotation of 90 degrees about z
  Transformation rotation = Transformation::rotation(Vector3d(0,0,1), degToRad(90));
  Transformation translation = Transformation::translation(Vector3d(1,0,0));
  Transformation transformation = rotation*translation;

  Point3d startingPoint(1,2,0);
  Vector3d startingOutwardNormal(1,0,0);
  Point3d startingRefPoint = startingPoint + startingOutwardNormal;

  LOG(Debug, "Starting point = " << startingPoint);
  LOG(Debug, "Starting ref point = " << startingRefPoint);

  Point3d transformedPoint = transformation*startingPoint;
  Point3d transformedRefPoint = transformation*startingRefPoint;
  Vector3d transformedOutwardNormal = transformedRefPoint-transformedPoint;

  LOG(Debug, "Transformed point = " << transformedPoint);
  LOG(Debug, "Transformed ref point = " << transformedRefPoint);

  Point3d endingPoint(-2,2,0);
  Vector3d endingOutwardNormal(0,1,0);
  Point3d endingRefPoint = endingPoint + endingOutwardNormal;

  LOG(Debug, "Ending point = " << endingPoint);
  LOG(Debug, "Ending ref point = " << endingRefPoint);

  Plane startingPlane(startingPoint, startingOutwardNormal);
  Plane translatedPlane = transformation*startingPlane;
  Plane finishingPlane(endingPoint, endingOutwardNormal);

  LOG(Debug, "Starting plane = " << startingPlane);
  LOG(Debug, "Translated plane = " << translatedPlane);
  LOG(Debug, "Finishing plane = " << finishingPlane);

  EXPECT_FALSE(startingPlane.equal(finishingPlane));
  EXPECT_TRUE(translatedPlane.equal(finishingPlane));
}


TEST_F(GeometryFixture, Plane_ProblemSurface1)
{
  // the following code in lu.hpp is called in debug mode but not release
  // in debug mode this causes an exception to be thrown which allows this test to pass correctly
  //#if BOOST_UBLAS_TYPE_CHECK
  //  BOOST_UBLAS_CHECK (detail::expression_type_check (prod (triangular_adaptor<const_matrix_type, upper> (m), e), cm2), internal_logic ());
  //#endif

  std::vector<Point3d> points;
  points.push_back(Point3d(68.534, 44.165, 46.3392));
  points.push_back(Point3d(68.534, 44.165, 43.5952));
  points.push_back(Point3d(73.1072, 48.7381, 43.5952));
  points.push_back(Point3d(73.1072, 48.7381, 46.3392));

  std::vector<Point3d> points2 = points;
  points2.pop_back();
  EXPECT_EQ(3u, points2.size());

  unsigned n = points.size();
  EXPECT_EQ(4u, n);

  boost::optional<Plane> plane;
  try {
    plane = Plane(points);
  }catch (const std::exception&){
  }
  ASSERT_TRUE(plane);

  boost::optional<Plane> plane2;
  try {
    plane2 = Plane(points2);
  }catch (const std::exception&){
  }
  ASSERT_TRUE(plane2);

  EXPECT_TRUE(plane->equal(*plane));
  EXPECT_TRUE(plane->equal(*plane2));
  EXPECT_TRUE(plane2->equal(*plane));

  for (unsigned i = 0; i < n; ++i) {
    EXPECT_TRUE(plane->pointOnPlane(points[i]));

    // project point to plane
    Point3d projected = plane->project(points[i]);

    EXPECT_TRUE(plane->pointOnPlane(projected));

    EXPECT_TRUE(pointEqual(points[i], projected));
  }
}


TEST_F(GeometryFixture, Plane_ProblemSurface2)
{
  // http://code.google.com/p/cbecc/issues/detail?id=394

  {
    std::vector<Point3d> points;
    points.push_back(Point3d(88.0317, 363.526, 4.77521));
    points.push_back(Point3d(88.0317, 363.526, 2.7432));
    points.push_back(Point3d(88.0317, 362.611, 2.7432));
    points.push_back(Point3d(88.0317, 362.611, 4.77521));

    std::vector<Point3d> points2 = points;
    points2.pop_back();
    EXPECT_EQ(3u, points2.size());

    unsigned n = points.size();
    EXPECT_EQ(4u, n);

    boost::optional<Plane> plane;
    try {
      plane = Plane(points);
    }catch (const std::exception&){
    }
    ASSERT_TRUE(plane);

    boost::optional<Plane> plane2;
    try {
      plane2 = Plane(points2);
    }catch (const std::exception&){
    }
    ASSERT_TRUE(plane2);

    EXPECT_TRUE(plane->equal(*plane));
    EXPECT_TRUE(plane->equal(*plane2));
    EXPECT_TRUE(plane2->equal(*plane));
  }

  {
    std::vector<Point3d> points;
    points.push_back(Point3d(88.03172639999999700000, 363.52581600000002000000, 4.77521016000000030000));
    points.push_back(Point3d(88.03172639999999700000, 363.52581600000002000000, 2.74320000000000030000));
    points.push_back(Point3d(88.03172639999999700000, 362.61141600000002000000, 2.74320000000000030000));
    points.push_back(Point3d(88.03172639999999700000, 362.61141600000002000000, 4.77521016000000030000));

    std::vector<Point3d> points2 = points;
    points2.pop_back();
    EXPECT_EQ(3u, points2.size());

    unsigned n = points.size();
    EXPECT_EQ(4u, n);

    boost::optional<Plane> plane;
    try {
      plane = Plane(points);
    }catch (const std::exception&){
    }
    ASSERT_TRUE(plane);

    boost::optional<Plane> plane2;
    try {
      plane2 = Plane(points2);
    }catch (const std::exception&){
    }
    ASSERT_TRUE(plane2);

    EXPECT_TRUE(plane->equal(*plane));
    EXPECT_TRUE(plane->equal(*plane2));
    EXPECT_TRUE(plane2->equal(*plane));
  }

  {
    double footToMeter =  0.3048;

    std::vector<Point3d> points;
    points.push_back(Point3d(footToMeter*QString("288.818").toDouble(), footToMeter*QString("1193.02").toDouble(), footToMeter*QString("18").toDouble()));
    points.push_back(Point3d(footToMeter*QString("288.818").toDouble(), footToMeter*QString("1193.02").toDouble(), footToMeter*QString("9").toDouble()));
    points.push_back(Point3d(footToMeter*QString("288.818").toDouble(), footToMeter*QString("1188.52").toDouble(), footToMeter*QString("9").toDouble()));
    points.push_back(Point3d(footToMeter*QString("288.818").toDouble(), footToMeter*QString("1188.52").toDouble(), footToMeter*QString("18").toDouble()));

    std::vector<Point3d> points2 = points;
    points2.pop_back();
    EXPECT_EQ(3u, points2.size());

    unsigned n = points.size();
    EXPECT_EQ(4u, n);

    boost::optional<Plane> plane;
    try {
      plane = Plane(points);
    }catch (const std::exception&){
    }
    ASSERT_TRUE(plane);

    boost::optional<Plane> plane2;
    try {
      plane2 = Plane(points2);
    }catch (const std::exception&){
    }
    ASSERT_TRUE(plane2);

    EXPECT_TRUE(plane->equal(*plane));
    EXPECT_TRUE(plane->equal(*plane2));
    EXPECT_TRUE(plane2->equal(*plane));
  }
}
