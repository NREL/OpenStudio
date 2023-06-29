/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "GeometryFixture.hpp"

#include "../Plane.hpp"
#include "../Point3d.hpp"
#include "../Vector3d.hpp"
#include "../Transformation.hpp"

using namespace openstudio;

void checkPlaneFromPointAndNormal(const Plane& plane, const Point3d& point, const Vector3d& normal) {
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

TEST_F(GeometryFixture, Plane_FromPointAndNormal) {
  Point3d point(0, 0, 0);
  Vector3d normal(0, 0, 1);
  Plane plane(point, normal);
  checkPlaneFromPointAndNormal(plane, point, normal);
  EXPECT_TRUE(plane.pointOnPlane(Point3d(0, 0, 0)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(0, 0, 1)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(1, 0, 0)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(1, 0, 1)));
  EXPECT_TRUE(pointEqual(Point3d(0, 0, 0), plane.project(Point3d(0, 0, 0))));
  EXPECT_TRUE(pointEqual(Point3d(0, 0, 0), plane.project(Point3d(0, 0, 1))));
  EXPECT_TRUE(pointEqual(Point3d(1, 0, 0), plane.project(Point3d(1, 0, 0))));
  EXPECT_TRUE(pointEqual(Point3d(1, 0, 0), plane.project(Point3d(1, 0, 1))));

  point = Point3d(0, 0, 1);
  normal = Vector3d(0, 0, 1);
  plane = Plane(point, normal);
  checkPlaneFromPointAndNormal(plane, point, normal);
  EXPECT_FALSE(plane.pointOnPlane(Point3d(0, 0, 0)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(0, 0, 1)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(1, 0, 0)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(1, 0, 1)));
  EXPECT_TRUE(pointEqual(Point3d(0, 0, 1), plane.project(Point3d(0, 0, 0))));
  EXPECT_TRUE(pointEqual(Point3d(0, 0, 1), plane.project(Point3d(0, 0, 1))));
  EXPECT_TRUE(pointEqual(Point3d(1, 0, 1), plane.project(Point3d(1, 0, 0))));
  EXPECT_TRUE(pointEqual(Point3d(1, 0, 1), plane.project(Point3d(1, 0, 1))));

  point = Point3d(0, 0, 1);
  normal = Vector3d(1, 0, 0);
  plane = Plane(point, normal);
  checkPlaneFromPointAndNormal(plane, point, normal);
  EXPECT_TRUE(plane.pointOnPlane(Point3d(0, 0, 0)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(0, 0, 1)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(1, 0, 0)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(1, 0, 1)));
  EXPECT_TRUE(pointEqual(Point3d(0, 0, 0), plane.project(Point3d(0, 0, 0))));
  EXPECT_TRUE(pointEqual(Point3d(0, 0, 1), plane.project(Point3d(0, 0, 1))));
  EXPECT_TRUE(pointEqual(Point3d(0, 0, 0), plane.project(Point3d(1, 0, 0))));
  EXPECT_TRUE(pointEqual(Point3d(0, 0, 1), plane.project(Point3d(1, 0, 1))));

  point = Point3d(1, 0, 1);
  normal = Vector3d(1, 0, 0);
  plane = Plane(point, normal);
  checkPlaneFromPointAndNormal(plane, point, normal);
  EXPECT_FALSE(plane.pointOnPlane(Point3d(0, 0, 0)));
  EXPECT_FALSE(plane.pointOnPlane(Point3d(0, 0, 1)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(1, 0, 0)));
  EXPECT_TRUE(plane.pointOnPlane(Point3d(1, 0, 1)));
  EXPECT_TRUE(pointEqual(Point3d(1, 0, 0), plane.project(Point3d(0, 0, 0))));
  EXPECT_TRUE(pointEqual(Point3d(1, 0, 1), plane.project(Point3d(0, 0, 1))));
  EXPECT_TRUE(pointEqual(Point3d(1, 0, 0), plane.project(Point3d(1, 0, 0))));
  EXPECT_TRUE(pointEqual(Point3d(1, 0, 1), plane.project(Point3d(1, 0, 1))));

  // throws if normal is undefined
  point = Point3d(0, 0, 0);
  normal = Vector3d(0, 0, 0);
  EXPECT_THROW(Plane(point, normal), openstudio::Exception);
}

TEST_F(GeometryFixture, Plane_FromPointAndNormal2) {
  Point3d point1(0, 0, 0);
  Point3d point2(1, 1, 0);
  Vector3d normal(0, 0, 1);

  Plane plane1(point1, normal);
  Plane plane2(point2, normal);
  EXPECT_TRUE(plane1.parallel(plane2));
  EXPECT_TRUE(plane1.equal(plane2));
}

TEST_F(GeometryFixture, Plane_FromPointAndNormal3) {
  Point3d point1(0, 0, 0);
  Point3d point2(0, 0, 1);
  Vector3d normal(0, 0, 1);

  Plane plane1(point1, normal);
  Plane plane2(point2, normal);
  EXPECT_TRUE(plane1.parallel(plane2));
  EXPECT_FALSE(plane1.equal(plane2));
}

TEST_F(GeometryFixture, Plane_FromPointsDegenerate) {
  std::vector<Point3d> points;

  // size 0, throws if size points < 3
  EXPECT_THROW(Plane p(points), openstudio::Exception);

  // size 1, throws if size points < 3
  points.push_back(Point3d(0, 0, 0));
  EXPECT_THROW(Plane p(points), openstudio::Exception);

  // size 2, throws if size points < 3
  points.push_back(Point3d(0, 0, 0));
  EXPECT_THROW(Plane p(points), openstudio::Exception);

  // size 3, all points the same
  points.push_back(Point3d(0, 0, 0));
  EXPECT_THROW(Plane p(points), openstudio::Exception);

  // size 3, all points on a line
  points = {
    {0, 0, 0},
    {1, 0, 0},
    {2, 0, 0},
  };
  EXPECT_THROW(Plane p(points), openstudio::Exception);
}

TEST_F(GeometryFixture, Plane_FromPoints1) {
  std::vector<Point3d> points{
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
    {0, 1, 0},
  };

  Plane p1(points);
  Plane p2(Point3d(0, 0, 0), Vector3d(0, 0, 1));

  EXPECT_TRUE(p1.equal(p2));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints2) {
  std::vector<Point3d> points{
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
  };

  Plane p1(points);
  Plane p2(Point3d(0, 1, 0), Vector3d(0, 0, -1));

  EXPECT_TRUE(p1.equal(p2));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints3) {
  std::vector<Point3d> points{
    {5, 0, 2},
    {5, 0, 0},
    {5, 2, 0},
    {5, 2, 2},
  };

  Point3d point(5, 0, 2);
  Vector3d normal(1, 0, 0);

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

TEST_F(GeometryFixture, Plane_FromPoints4) {
  std::vector<Point3d> points{
    {5, -2, 4},
    {5, -2, 2},
    {7, -2, 2},
    {7, -2, 4},
  };

  Point3d point(0, -2, 0);
  Vector3d normal(0, -1, 0);

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

TEST_F(GeometryFixture, Plane_FromPoints4_Small) {
  // points have area of 1 cm^2
  std::vector<Point3d> points{
    {0, 0, 0},
    {0.01, 0, 0},
    {0.01, 0.01, 0},
    {0, 0.01, 0},
  };

  Point3d point(10, 10, 0);
  Vector3d normal(0, 0, 1);

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

TEST_F(GeometryFixture, Plane_FromPoints_NotEqual) {
  std::vector<Point3d> points1{
    {0, 1, 0},
    {0, 0, 0},
    {1, 0, 0},
    {1, 1, 0},
  };

  std::vector<Point3d> points2{
    {0, 1, 2},
    {0, 0, 2},
    {1, 0, 2},
    {1, 1, 2},
  };

  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_Reverse) {
  std::vector<Point3d> points1{
    {1, 1, 2},
    {1, 0, 2},
    {0, 0, 2},
    {0, 1, 2},
  };

  std::vector<Point3d> points2{
    {0, 1, 2},
    {0, 0, 2},
    {1, 0, 2},
    {1, 1, 2},
  };

  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_TRUE(p1.reverseEqual(p2));
  EXPECT_FALSE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_Separated_X) {
  std::vector<Point3d> points1{
    {10, 0, 1},
    {10, 0, 0},
    {10, 1, 0},
    {10, 1, 1},
  };

  std::vector<Point3d> points2{
    {20, 0, 1},
    {20, 0, 0},
    {20, 1, 0},
    {20, 1, 1},
  };

  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(1, 0, 0), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(1, 0, 0), p2.outwardNormal()));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_Reverse_Separated_X) {
  std::vector<Point3d> points1{
    {-10, 0, 1},
    {-10, 0, 0},
    {-10, 1, 0},
    {-10, 1, 1},
  };

  std::vector<Point3d> points2{
    {-20, 1, 1},
    {-20, 1, 0},
    {-20, 0, 0},
    {-20, 0, 1},
  };

  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(1, 0, 0), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(-1, 0, 0), p2.outwardNormal()));
  EXPECT_FALSE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_Separated_Y) {
  std::vector<Point3d> points1{
    {0, -10, 1},
    {0, -10, 0},
    {1, -10, 0},
    {1, -10, 1},
  };

  std::vector<Point3d> points2{
    {0, 10, 1},
    {0, 10, 0},
    {1, 10, 0},
    {1, 10, 1},
  };

  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(0, -1, 0), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(0, -1, 0), p2.outwardNormal()));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_Reverse_Separated_Y) {
  std::vector<Point3d> points1{
    {0, -20, 1},
    {0, -20, 0},
    {1, -20, 0},
    {1, -20, 1},
  };

  std::vector<Point3d> points2{
    {1, 20, 1},
    {1, 20, 0},
    {0, 20, 0},
    {0, 20, 1},
  };

  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(0, -1, 0), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(0, 1, 0), p2.outwardNormal()));
  EXPECT_FALSE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_Separated_Z) {
  std::vector<Point3d> points1{
    {0, 1, 10},
    {0, 0, 10},
    {1, 0, 10},
    {1, 1, 10},
  };

  std::vector<Point3d> points2{
    {0, 1, -10},
    {0, 0, -10},
    {1, 0, -10},
    {1, 1, -10},
  };

  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(0, 0, 1), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(0, 0, 1), p2.outwardNormal()));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_Reverse_Separated_Z) {
  std::vector<Point3d> points1{
    {0, 1, -10},
    {0, 0, -10},
    {1, 0, -10},
    {1, 1, -10},
  };

  std::vector<Point3d> points2{
    {1, 1, -20},
    {1, 0, -20},
    {0, 0, -20},
    {0, 1, -20},
  };

  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(0, 0, 1), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(0, 0, -1), p2.outwardNormal()));
  EXPECT_FALSE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_FromPoints_ProblemSurface_IntersectModel22) {
  // from Surface 1292
  std::vector<Point3d> points1{
    {33.5280021336, -29.717998476, 91.44},
    {33.5280021336, -29.717998476, 86.868},
    {33.5280021336, 29.718001524, 86.868},
    {33.5280021336, 29.718001524, 91.44},
  };

  // from Surface 1384
  std::vector<Point3d> points2{
    {28.9560021336, -25.145998476, 96.012},
    {28.9560021336, -25.145998476, 91.44},
    {28.9560021336, 25.146001524, 91.44},
    {28.9560021336, 25.146001524, 96.012},
  };

  Plane p1(points1);
  Plane p2(points2);

  EXPECT_TRUE(p1.parallel(p2));
  EXPECT_FALSE(p1.equal(p2));
  EXPECT_FALSE(p1.reverseEqual(p2));
  EXPECT_TRUE(vectorEqual(Vector3d(1, 0, 0), p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(Vector3d(1, 0, 0), p2.outwardNormal()));
  EXPECT_TRUE(vectorEqual(p1.outwardNormal(), p2.outwardNormal()));

  OptionalVector3d n1 = getOutwardNormal(points1);
  OptionalVector3d n2 = getOutwardNormal(points2);
  ASSERT_TRUE(n1);
  ASSERT_TRUE(n2);
  EXPECT_TRUE(vectorEqual(*n1, p1.outwardNormal()));
  EXPECT_TRUE(vectorEqual(*n2, p2.outwardNormal()));
}

TEST_F(GeometryFixture, Plane_Transformation) {
  // translation by 1 meter in x then rotation of 90 degrees about z
  Transformation rotation = Transformation::rotation(Vector3d(0, 0, 1), degToRad(90));
  Transformation translation = Transformation::translation(Vector3d(1, 0, 0));
  Transformation transformation = rotation * translation;

  Point3d startingPoint(1, 2, 0);
  Vector3d startingOutwardNormal(1, 0, 0);
  Point3d startingRefPoint = startingPoint + startingOutwardNormal;

  LOG(Debug, "Starting point = " << startingPoint);
  LOG(Debug, "Starting ref point = " << startingRefPoint);

  Point3d transformedPoint = transformation * startingPoint;
  Point3d transformedRefPoint = transformation * startingRefPoint;
  Vector3d transformedOutwardNormal = transformedRefPoint - transformedPoint;

  LOG(Debug, "Transformed point = " << transformedPoint);
  LOG(Debug, "Transformed ref point = " << transformedRefPoint);
  LOG(Debug, "Transformed Outward Normal = " << transformedOutwardNormal);

  Point3d endingPoint(-2, 2, 0);
  Vector3d endingOutwardNormal(0, 1, 0);
  Point3d endingRefPoint = endingPoint + endingOutwardNormal;

  LOG(Debug, "Ending point = " << endingPoint);
  LOG(Debug, "Ending ref point = " << endingRefPoint);

  Plane startingPlane(startingPoint, startingOutwardNormal);
  Plane translatedPlane = transformation * startingPlane;
  Plane finishingPlane(endingPoint, endingOutwardNormal);

  LOG(Debug, "Starting plane = " << startingPlane);
  LOG(Debug, "Translated plane = " << translatedPlane);
  LOG(Debug, "Finishing plane = " << finishingPlane);

  EXPECT_FALSE(startingPlane.equal(finishingPlane));
  EXPECT_TRUE(translatedPlane.equal(finishingPlane));
}

TEST_F(GeometryFixture, Plane_ProblemSurface1) {
  // the following code in lu.hpp is called in debug mode but not release
  // in debug mode this causes an exception to be thrown which allows this test to pass correctly
  //#if BOOST_UBLAS_TYPE_CHECK
  //  BOOST_UBLAS_CHECK (detail::expression_type_check (prod (triangular_adaptor<const_matrix_type, upper> (m), e), cm2), internal_logic ());
  //#endif

  std::vector<Point3d> points{
    {68.534, 44.165, 46.3392},
    {68.534, 44.165, 43.5952},
    {73.1072, 48.7381, 43.5952},
    {73.1072, 48.7381, 46.3392},
  };

  std::vector<Point3d> points2 = points;
  points2.pop_back();
  EXPECT_EQ(3u, points2.size());

  unsigned n = points.size();
  EXPECT_EQ(4u, n);

  boost::optional<Plane> plane;
  try {
    plane = Plane(points);
  } catch (const std::exception&) {
  }
  ASSERT_TRUE(plane);

  boost::optional<Plane> plane2;
  try {
    plane2 = Plane(points2);
  } catch (const std::exception&) {
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

TEST_F(GeometryFixture, Plane_ProblemSurface2) {
  // http://code.google.com/p/cbecc/issues/detail?id=394

  {
    std::vector<Point3d> points{
      {88.0317, 363.526, 4.77521},
      {88.0317, 363.526, 2.7432},
      {88.0317, 362.611, 2.7432},
      {88.0317, 362.611, 4.77521},
    };

    std::vector<Point3d> points2 = points;
    points2.pop_back();
    EXPECT_EQ(3u, points2.size());

    unsigned n = points.size();
    EXPECT_EQ(4u, n);

    boost::optional<Plane> plane;
    try {
      plane = Plane(points);
    } catch (const std::exception&) {
    }
    ASSERT_TRUE(plane);

    boost::optional<Plane> plane2;
    try {
      plane2 = Plane(points2);
    } catch (const std::exception&) {
    }
    ASSERT_TRUE(plane2);

    EXPECT_TRUE(plane->equal(*plane));
    EXPECT_TRUE(plane->equal(*plane2));
    EXPECT_TRUE(plane2->equal(*plane));
  }

  {
    std::vector<Point3d> points{
      {88.03172639999999700000, 363.52581600000002000000, 4.77521016000000030000},
      {88.03172639999999700000, 363.52581600000002000000, 2.74320000000000030000},
      {88.03172639999999700000, 362.61141600000002000000, 2.74320000000000030000},
      {88.03172639999999700000, 362.61141600000002000000, 4.77521016000000030000},
    };

    std::vector<Point3d> points2 = points;
    points2.pop_back();
    EXPECT_EQ(3u, points2.size());

    unsigned n = points.size();
    EXPECT_EQ(4u, n);

    boost::optional<Plane> plane;
    try {
      plane = Plane(points);
    } catch (const std::exception&) {
    }
    ASSERT_TRUE(plane);

    boost::optional<Plane> plane2;
    try {
      plane2 = Plane(points2);
    } catch (const std::exception&) {
    }
    ASSERT_TRUE(plane2);

    EXPECT_TRUE(plane->equal(*plane));
    EXPECT_TRUE(plane->equal(*plane2));
    EXPECT_TRUE(plane2->equal(*plane));
  }

  {
    double footToMeter = 0.3048;

    std::vector<Point3d> points{
      {footToMeter * 288.818, footToMeter * 1193.02, footToMeter * 18},
      {footToMeter * 288.818, footToMeter * 1193.02, footToMeter * 9},
      {footToMeter * 288.818, footToMeter * 1188.52, footToMeter * 9},
      {footToMeter * 288.818, footToMeter * 1188.52, footToMeter * 18},
    };

    std::vector<Point3d> points2 = points;
    points2.pop_back();
    EXPECT_EQ(3u, points2.size());

    unsigned n = points.size();
    EXPECT_EQ(4u, n);

    boost::optional<Plane> plane;
    try {
      plane = Plane(points);
    } catch (const std::exception&) {
    }
    ASSERT_TRUE(plane);

    boost::optional<Plane> plane2;
    try {
      plane2 = Plane(points2);
    } catch (const std::exception&) {
    }
    ASSERT_TRUE(plane2);

    EXPECT_TRUE(plane->equal(*plane));
    EXPECT_TRUE(plane->equal(*plane2));
    EXPECT_TRUE(plane2->equal(*plane));
  }
}

TEST_F(GeometryFixture, Plane_RayIntersection) {

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

  // Putting extra vertices here on purpose to show that the Space::volume was miscalculating due to averaging foor and ceiling heights
  std::vector<Point3d> roof{
    {+30.0, +0.0, +10.3},  //
    {+30.0, +10.0, +0.3},  //
    {+0.0, +10.0, +0.3},   //
    {+0.0, +0.0, +10.3},   //
    {+10.0, +0.0, +10.3},  //
    {+20.0, +0.0, +10.3},
  };

  std::vector<Point3d> east{
    {+30.0, +0.0, +10.3},
    {+30.0, +0.0, +0.0},
    {+30.0, +10.0, +0.0},
    {+30.0, +10.0, +0.3},
  };

  std::vector<Point3d> north{
    {+30.0, +10.0, +0.3},
    {+30.0, +10.0, +0.0},
    {+0.0, +10.0, +0.0},
    {+0.0, +10.0, +0.3},
  };

  std::vector<Point3d> west{
    {+0.0, +10.0, +0.3},
    {+0.0, +10.0, +0.0},
    {+0.0, +0.0, +0.0},
    {+0.0, +0.0, +10.3},
  };

  std::vector<Point3d> south1{
    {+0.0, +0.0, +10.3},
    {+0.0, +0.0, +0.0},
    {+15.0, +0.0, +0.0},
    {+15.0, +0.0, +10.3},
  };

  std::vector<Point3d> south2{
    {+15.0, +0.0, +10.3},
    {+15.0, +0.0, +0.0},
    {+30.0, +0.0, +0.0},
    {+30.0, +0.0, +10.3},
  };

  // Clockwise because normal is pointing down
  std::vector<Point3d> floor{
    {+0.0, +0.0, +0.0},
    {+0.0, +10.0, +0.0},
    {+30.0, +10.0, +0.0},
    {+30.0, +0.0, +0.0},
  };

  Plane floorPlane(floor);
  EXPECT_DOUBLE_EQ(0.0, floorPlane.a());
  EXPECT_DOUBLE_EQ(0.0, floorPlane.b());
  EXPECT_DOUBLE_EQ(-1.0, floorPlane.c());
  EXPECT_DOUBLE_EQ(0.0, floorPlane.d());

  Plane roofPlane(roof);
  Vector3d roofNormal(0, 1, 1);
  ASSERT_TRUE(roofNormal.normalize());
  EXPECT_DOUBLE_EQ(0.0, roofPlane.a());
  EXPECT_DOUBLE_EQ(roofNormal.y(), roofPlane.b());
  EXPECT_DOUBLE_EQ(roofNormal.z(), roofPlane.c());
  EXPECT_DOUBLE_EQ(-roofNormal.y() * 10.3, roofPlane.d());
  EXPECT_DOUBLE_EQ(-7.2831998462214402, roofPlane.d());

  Plane south1Plane(south1);
  EXPECT_DOUBLE_EQ(0.0, south1Plane.a());
  EXPECT_DOUBLE_EQ(-1.0, south1Plane.b());
  EXPECT_DOUBLE_EQ(0.0, south1Plane.c());
  EXPECT_DOUBLE_EQ(0.0, south1Plane.d());

  Plane south2Plane(south2);
  EXPECT_DOUBLE_EQ(0.0, south2Plane.a());
  EXPECT_DOUBLE_EQ(-1.0, south2Plane.b());
  EXPECT_DOUBLE_EQ(0.0, south2Plane.c());
  EXPECT_DOUBLE_EQ(0.0, south2Plane.d());

  Plane northPlane(north);
  EXPECT_DOUBLE_EQ(0.0, northPlane.a());
  EXPECT_DOUBLE_EQ(1.0, northPlane.b());
  EXPECT_DOUBLE_EQ(0.0, northPlane.c());
  EXPECT_DOUBLE_EQ(-10.0, northPlane.d());

  Plane westPlane(west);
  EXPECT_DOUBLE_EQ(-1.0, westPlane.a());
  EXPECT_DOUBLE_EQ(0.0, westPlane.b());
  EXPECT_DOUBLE_EQ(0.0, westPlane.c());
  EXPECT_DOUBLE_EQ(0.0, westPlane.d());

  Plane eastPlane(east);
  EXPECT_DOUBLE_EQ(1.0, eastPlane.a());
  EXPECT_DOUBLE_EQ(0.0, eastPlane.b());
  EXPECT_DOUBLE_EQ(0.0, eastPlane.c());
  EXPECT_DOUBLE_EQ(-30.0, eastPlane.d());

  for (const auto& plane : {floorPlane, roofPlane, south1Plane, south2Plane, northPlane, westPlane, eastPlane}) {
    const Point3d p = plane.anyPointOnPlane();
    EXPECT_TRUE(plane.pointOnPlane(p));
  }

  // Floor shoots to Roof (only)
  {
    Point3d r0(5.0, 5.0, 0.0);
    EXPECT_TRUE(floorPlane.pointOnPlane(r0));
    Vector3d rD = floorPlane.outwardNormal().reverseVector();
    EXPECT_TRUE(vectorEqual(Vector3d(0.0, 0.0, 1.0), rD));

    EXPECT_FALSE(south1Plane.rayIntersection(r0, rD));
    EXPECT_FALSE(south2Plane.rayIntersection(r0, rD));
    EXPECT_FALSE(northPlane.rayIntersection(r0, rD));
    EXPECT_FALSE(eastPlane.rayIntersection(r0, rD));
    EXPECT_FALSE(westPlane.rayIntersection(r0, rD));

    {
      boost::optional<Point3d> intersection_ = roofPlane.rayIntersection(r0, rD);
      ASSERT_TRUE(intersection_);
      EXPECT_TRUE(pointEqual({5.0, 5.0, 5.3}, intersection_.get()));
      EXPECT_TRUE(roofPlane.pointOnPlane(intersection_.get()));
    }

    // Reverse shoots nowhere
    EXPECT_FALSE(south1Plane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(south2Plane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(northPlane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(westPlane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(eastPlane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(roofPlane.rayIntersection(r0, rD.reverseVector()));
  }

  // Roof shoots to floor and the south walls
  // it shouldn't shoot to the north wall because even though they aren't orthogonal, we should expect (t < 0)
  {
    Point3d r0(5.0, 5.0, 5.3);
    EXPECT_TRUE(roofPlane.pointOnPlane(r0));
    Vector3d rD = roofPlane.outwardNormal().reverseVector();
    EXPECT_TRUE(vectorEqual(Vector3d(0.0, -0.707107, -0.707107), rD));

    EXPECT_FALSE(northPlane.rayIntersection(r0, rD));
    EXPECT_FALSE(eastPlane.rayIntersection(r0, rD));
    EXPECT_FALSE(westPlane.rayIntersection(r0, rD));

    {
      boost::optional<Point3d> intersection_ = floorPlane.rayIntersection(r0, rD);
      ASSERT_TRUE(intersection_);
      EXPECT_TRUE(pointEqual({5.0, -0.3, 0.0}, intersection_.get()));
      EXPECT_TRUE(floorPlane.pointOnPlane(intersection_.get()));
    }

    {
      boost::optional<Point3d> intersection_ = south1Plane.rayIntersection(r0, rD);
      ASSERT_TRUE(intersection_);
      EXPECT_TRUE(pointEqual({5.0, 0.0, 0.3}, intersection_.get())) << intersection_.get();
      EXPECT_TRUE(south1Plane.pointOnPlane(intersection_.get()));
    }
    {
      boost::optional<Point3d> intersection_ = south2Plane.rayIntersection(r0, rD);
      ASSERT_TRUE(intersection_);
      EXPECT_TRUE(pointEqual({5.0, 0.0, 0.3}, intersection_.get())) << intersection_.get();
      EXPECT_TRUE(south2Plane.pointOnPlane(intersection_.get()));
    }

    // Reverse shoots to the north plane...
    EXPECT_FALSE(south1Plane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(south2Plane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(westPlane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(eastPlane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(floorPlane.rayIntersection(r0, rD.reverseVector()));

    {
      boost::optional<Point3d> intersection_ = northPlane.rayIntersection(r0, rD.reverseVector());
      ASSERT_TRUE(intersection_);
      EXPECT_TRUE(pointEqual({5.0, 10.0, 10.3}, intersection_.get())) << intersection_.get();
      EXPECT_TRUE(northPlane.pointOnPlane(intersection_.get()));
    }
  }

  // East wall shoots to West wall (only)
  {
    Point3d r0(30.0, 5.0, 5.0);
    EXPECT_TRUE(eastPlane.pointOnPlane(r0));
    Vector3d rD = eastPlane.outwardNormal().reverseVector();
    EXPECT_FALSE(south1Plane.rayIntersection(r0, rD));
    EXPECT_FALSE(south2Plane.rayIntersection(r0, rD));
    EXPECT_FALSE(northPlane.rayIntersection(r0, rD));
    EXPECT_FALSE(floorPlane.rayIntersection(r0, rD));
    EXPECT_FALSE(roofPlane.rayIntersection(r0, rD));

    {
      boost::optional<Point3d> intersection_ = westPlane.rayIntersection(r0, rD);
      ASSERT_TRUE(intersection_);
      EXPECT_TRUE(pointEqual({0.0, 5.0, 5.0}, intersection_.get()));
      EXPECT_TRUE(westPlane.pointOnPlane(intersection_.get()));
    }

    // Reverse shoots nowhere
    EXPECT_FALSE(south1Plane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(south2Plane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(northPlane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(westPlane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(floorPlane.rayIntersection(r0, rD.reverseVector()));
    EXPECT_FALSE(roofPlane.rayIntersection(r0, rD.reverseVector()));
  }
}
