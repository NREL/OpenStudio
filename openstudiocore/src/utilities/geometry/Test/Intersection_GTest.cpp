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
#include "../Intersection.hpp"
#include "GeometryFixture.hpp"

#undef BOOST_UBLAS_TYPE_CHECK
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>

typedef boost::geometry::model::d2::point_xy<double> BoostPoint;
typedef boost::geometry::model::polygon<BoostPoint> BoostPolygon;
typedef boost::geometry::model::ring<BoostPoint> BoostRing;
typedef boost::geometry::model::multi_polygon<BoostPolygon> BoostMultiPolygon;

using namespace std;
using namespace boost;
using namespace openstudio;

std::string printPolygon(const BoostPolygon& polygon)
{
  std::stringstream ss;

  ss << boost::geometry::area(polygon);

  BoostRing outer = polygon.outer();
  if (outer.empty()){
    return ss.str();
  }

  // add point for each vertex except final vertex
  ss << " [";
  for(unsigned i = 0; i < outer.size(); ++i){
    ss << outer[i].x() << " " << outer[i].y();
    if (i < outer.size()-1){
      ss << ", ";
    }
  }

  for (const BoostRing& inner : polygon.inners()){
    if (!inner.empty()){
      ss << "], [";
      // inner loop already in reverse order
      for(unsigned i = 0; i < inner.size(); ++i){
        ss << inner[i].x() << " " << inner[i].y();
        if (i < inner.size()-1){
          ss << ", ";
        }
      }
    }
  }
  ss << "]";

  return ss.str();
}

std::vector<Point3d> makeRectangleUp(double xmin, double ymin, double width, double height)
{
  std::vector<Point3d> result;
  result.push_back(Point3d(xmin, ymin, 0));
  result.push_back(Point3d(xmin+width, ymin, 0));
  result.push_back(Point3d(xmin+width, ymin+height, 0));
  result.push_back(Point3d(xmin, ymin+height, 0));
  return result;
}

std::vector<Point3d> makeRectangleDown(double xmin, double ymin, double width, double height)
{
  std::vector<Point3d> result;
  result.push_back(Point3d(xmin+width, ymin+height, 0));
  result.push_back(Point3d(xmin+width, ymin, 0));
  result.push_back(Point3d(xmin, ymin, 0));
  result.push_back(Point3d(xmin, ymin+height, 0));
  return result;
}


TEST_F(GeometryFixture, BoostGeometry_Polygon1)
{
  BoostPolygon blue, yellow;

  // blue
  boost::geometry::read_wkt(
      "POLYGON((-11.379200508 -12.0396003048 , -11.379200508 12.0395996952 , 3.555999492 12.0395996952 , 3.555999492 -12.0396003048 , -11.379200508 -12.0396003048))", blue);
  //Surface Intersection Model 74
  //Surface 37,                             !- Name
  //-11.379200508, -12.0396003048, 3.3528,  !- X,Y,Z Vertex 1 {m}
  //-11.379200508, 12.0395996952, 3.3528,   !- X,Y,Z Vertex 2 {m}
  //3.555999492, 12.0395996952, 3.3528,     !- X,Y,Z Vertex 3 {m}
  //3.555999492, -12.0396003048, 3.3528;    !- X,Y,Z Vertex 4 {m}

  // yellow
  boost::geometry::read_wkt(
      "POLYGON((3.2512004064 -7.4676003048 , 3.2512004064 7.4675996952 , 3.555999492 7.7723987808 , 3.555999492 -7.7723993904 , 3.2512004064 -7.4676003048))", yellow);
  //Surface Intersection Model 74
  //Surface 44,                             !- Name
  //3.555999492, -7.7723993904, 3.3528,     !- X,Y,Z Vertex 1 {m}
  //3.555999492, 7.7723987808, 3.3528,      !- X,Y,Z Vertex 2 {m}
  //3.2512004064, 7.4675996952, 3.3528,     !- X,Y,Z Vertex 3 {m}
  //3.2512004064, -7.4676003048, 3.3528;    !- X,Y,Z Vertex 4 {m}

  std::vector<BoostPolygon> output;
  boost::geometry::difference(yellow, blue, output);

  int i = 0;
  std::cout << "yellow - blue:" << std::endl;
  for (BoostPolygon const& p : output)
  {
    std::cout << i++ << ": " << printPolygon(p) << std::endl;
  }

  
  output.clear();
  boost::geometry::difference(blue, yellow, output);

  i = 0;
  std::cout << "blue - yellow:" << std::endl;
  for (BoostPolygon const& p : output)
  {
    std::cout << i++ << ": " << printPolygon(p) << std::endl;
  }

}


TEST_F(GeometryFixture, PointInPolygon)
{
  double tol = 0.01;

  Point3dVector points = makeRectangleDown(0, 0, 1, 1);

  // center
  EXPECT_TRUE(pointInPolygon(Point3d(0.5,0.5,0), points, tol));

  // corners
  EXPECT_TRUE(pointInPolygon(Point3d(0,0,0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(1,0,0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(1,1,0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(0,1,0), points, tol));

  // edges
  EXPECT_TRUE(pointInPolygon(Point3d(0.5,0,0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(1,0.5,0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(0.5,1,0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(0,0.5,0), points, tol));

  // outside
  EXPECT_FALSE(pointInPolygon(Point3d(2,0,0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(1,2,0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(-1,0,0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(-1,-1,0), points, tol));

  // not on z = 0
  EXPECT_FALSE(pointInPolygon(Point3d(0.5,0.5,0.5), points, tol));
}

TEST_F(GeometryFixture, Intersect_False)
{
  double tol = 0.01;

  boost::optional<IntersectionResult> test;
  Point3dVector points1;
  Point3dVector points2;

  // non intersecting, sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);
  points2 = makeRectangleDown(3, 0, 1, 1);

  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);

  // non intersecting, sense is up
  points1 = makeRectangleUp(0, 0, 1, 1);
  points2 = makeRectangleUp(3, 0, 1, 1);
  
  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);

  // sense is down, same points but not on z=0 plane
  points1.clear();
  points1.push_back(Point3d(1, 1, 3));
  points1.push_back(Point3d(1, 0, 3));
  points1.push_back(Point3d(0, 0, 3));
  points1.push_back(Point3d(0, 1, 3));

  points2.clear();
  points2.push_back(Point3d(1, 1, 3));
  points2.push_back(Point3d(1, 0, 3));
  points2.push_back(Point3d(0, 0, 3));
  points2.push_back(Point3d(0, 1, 3));
  
  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);

  // sense is up, same points but not on z=0 plane
  points1.clear();
  points1.push_back(Point3d(0, 0, 3));
  points1.push_back(Point3d(1, 0, 3));
  points1.push_back(Point3d(1, 1, 3));
  points1.push_back(Point3d(0, 1, 3));

  points2.clear();
  points2.push_back(Point3d(0, 0, 3));
  points2.push_back(Point3d(1, 0, 3));
  points2.push_back(Point3d(1, 1, 3));
  points2.push_back(Point3d(0, 1, 3));
  
  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);

  // sense is right, same points but not on z=0 plane
  points1.clear();
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(0, 0, 1));
  points1.push_back(Point3d(0, 1, 1));
  points1.push_back(Point3d(0, 1, 0));

  points2.clear();
  points2.push_back(Point3d(0, 0, 0));
  points2.push_back(Point3d(0, 0, 1));
  points2.push_back(Point3d(0, 1, 1));
  points2.push_back(Point3d(0, 1, 0));
  
  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);
}

TEST_F(GeometryFixture, Intersect_SamePoints)
{
  double tol = 0.01;

  boost::optional<IntersectionResult> test;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);
  points2 = makeRectangleDown(0, 0, 1, 1);

  test = intersect(points1, points2, tol);
  ASSERT_TRUE(test);
  EXPECT_TRUE(circularEqual(points1, test->polygon1())) << test->polygon1();
  EXPECT_TRUE(circularEqual(points2, test->polygon2())) << test->polygon2();
  EXPECT_EQ(0, test->newPolygons1().size());
  EXPECT_EQ(0, test->newPolygons2().size());

  test = intersect(points2, points1, tol);
  ASSERT_TRUE(test);
  EXPECT_TRUE(circularEqual(points2, test->polygon1())) << test->polygon1();
  EXPECT_TRUE(circularEqual(points1, test->polygon2())) << test->polygon2();
  EXPECT_EQ(0, test->newPolygons1().size());
  EXPECT_EQ(0, test->newPolygons2().size());

  // sense is up
  points1 = makeRectangleUp(0, 0, 1, 1);
  points2 = makeRectangleUp(0, 0, 1, 1);
  
  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);

  // opposite sense, points1 is up, points2 is down
  points1 = makeRectangleUp(0, 0, 1, 1);
  points2 = makeRectangleDown(0, 0, 1, 1);
  
  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);
}

TEST_F(GeometryFixture, Intersect_Adjacent)
{
  double tol = 0.01;

  boost::optional<IntersectionResult> test;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);
  points2 = makeRectangleDown(1, 0, 1, 1);

  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);

  // sense is up
  points1 = makeRectangleUp(0, 0, 1, 1);
  points2 = makeRectangleUp(1, 0, 1, 1);

  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);
}

TEST_F(GeometryFixture, Intersect_Overlap)
{
  double tol = 0.01;

  boost::optional<IntersectionResult> test;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 2, 1);
  points2 = makeRectangleDown(1, 0, 2, 1);

  test = intersect(points1, points2, tol);
  ASSERT_TRUE(test);
  EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon1())) << test->polygon1();
  EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon2())) << test->polygon2();
  ASSERT_EQ(1, test->newPolygons1().size());
  EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 1, 1), test->newPolygons1()[0])) << test->newPolygons1()[0];
  ASSERT_EQ(1, test->newPolygons2().size());
  EXPECT_TRUE(circularEqual(makeRectangleDown(2, 0, 1, 1), test->newPolygons2()[0])) << test->newPolygons2()[0];

  test = intersect(points2, points1, tol);
  ASSERT_TRUE(test);
  EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon1())) << test->polygon1();
  EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon2())) << test->polygon2();
  ASSERT_EQ(1, test->newPolygons1().size());
  EXPECT_TRUE(circularEqual(makeRectangleDown(2, 0, 1, 1), test->newPolygons1()[0])) << test->newPolygons1()[0];
  ASSERT_EQ(1, test->newPolygons2().size());
  EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 1, 1), test->newPolygons2()[0])) << test->newPolygons2()[0];
}

TEST_F(GeometryFixture, Join_False)
{
  double tol = 0.01;

  boost::optional<std::vector<Point3d> > test;
  Point3dVector points1;
  Point3dVector points2;

  // non intersecting, sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);
  points2 = makeRectangleDown(3, 0, 1, 1);

  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);

  // non intersecting, sense is up
  points1 = makeRectangleUp(0, 0, 1, 1);
  points2 = makeRectangleUp(3, 0, 1, 1);
  
  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);

  // sense is down, same points but not on z=0 plane
  points1.clear();
  points1.push_back(Point3d(1, 1, 3));
  points1.push_back(Point3d(1, 0, 3));
  points1.push_back(Point3d(0, 0, 3));
  points1.push_back(Point3d(0, 1, 3));

  points2.clear();
  points2.push_back(Point3d(1, 1, 3));
  points2.push_back(Point3d(1, 0, 3));
  points2.push_back(Point3d(0, 0, 3));
  points2.push_back(Point3d(0, 1, 3));
  
  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);

  // sense is up, same points but not on z=0 plane
  points1.clear();
  points1.push_back(Point3d(0, 0, 3));
  points1.push_back(Point3d(1, 0, 3));
  points1.push_back(Point3d(1, 1, 3));
  points1.push_back(Point3d(0, 1, 3));

  points2.clear();
  points2.push_back(Point3d(0, 0, 3));
  points2.push_back(Point3d(1, 0, 3));
  points2.push_back(Point3d(1, 1, 3));
  points2.push_back(Point3d(0, 1, 3));
  
  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);

  // sense is right, same points but not on z=0 plane
  points1.clear();
  points1.push_back(Point3d(0, 0, 0));
  points1.push_back(Point3d(0, 0, 1));
  points1.push_back(Point3d(0, 1, 1));
  points1.push_back(Point3d(0, 1, 0));

  points2.clear();
  points2.push_back(Point3d(0, 0, 0));
  points2.push_back(Point3d(0, 0, 1));
  points2.push_back(Point3d(0, 1, 1));
  points2.push_back(Point3d(0, 1, 0));
  
  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);
}

TEST_F(GeometryFixture, Join_SamePoints)
{
  double tol = 0.01;

  boost::optional<std::vector<Point3d> > test;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);
  points2 = makeRectangleDown(0, 0, 1, 1);

  test = join(points1, points2, tol);
  ASSERT_TRUE(test);
  EXPECT_TRUE(circularEqual(points1, *test)) << *test;

  test = join(points2, points1, tol);
  ASSERT_TRUE(test);
  EXPECT_TRUE(circularEqual(points1, *test)) << *test;

  // sense is up
  points1 = makeRectangleUp(0, 0, 1, 1);
  points2 = makeRectangleUp(0, 0, 1, 1);
  
  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);

  // opposite sense, points1 is up, points2 is down
  points1 = makeRectangleUp(0, 0, 1, 1);
  points2 = makeRectangleDown(0, 0, 1, 1);
  
  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);
}

TEST_F(GeometryFixture, Join_Adjacent)
{
  double tol = 0.01;

  boost::optional<std::vector<Point3d> > test;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);
  points2 = makeRectangleDown(1, 0, 1, 1);

  test = join(points1, points2, tol);
  ASSERT_TRUE(test);
  EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 2, 1), *test)) << *test;

  test = join(points2, points1, tol);
  ASSERT_TRUE(test);
  EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 2, 1), *test)) << *test;

  // sense is up
  points1 = makeRectangleUp(0, 0, 1, 1);
  points2 = makeRectangleUp(1, 0, 1, 1);

  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);
}

TEST_F(GeometryFixture, Join_Overlap)
{
  double tol = 0.01;

  boost::optional<std::vector<Point3d> > test;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 2, 1);
  points2 = makeRectangleDown(1, 0, 2, 1);

  test = join(points1, points2, tol);
  ASSERT_TRUE(test);
  EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 3, 1), *test)) << *test;

  test = join(points2, points1, tol);
  ASSERT_TRUE(test);
  EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 3, 1), *test)) << *test;
}

TEST_F(GeometryFixture, Join_Overlap2)
{
  double tol = 0.01;

  boost::optional<std::vector<Point3d> > test;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 4, 1);
  points2 = makeRectangleDown(1, -1, 4, 1);

  test = join(points1, points2, tol);
  ASSERT_TRUE(test);

  test = join(points2, points1, tol);
  ASSERT_TRUE(test);
}



TEST_F(GeometryFixture, JoinAll)
{
  double tol = 0.01;

  std::vector<Point3dVector> test;
  std::vector<Point3dVector> polygons;

  // empty
  test = joinAll(polygons, tol);
  EXPECT_EQ(0, test.size());

  // two overlapping
  polygons.clear();
  polygons.push_back(makeRectangleDown(0, 0, 1, 1));
  polygons.push_back(makeRectangleDown(1, 0, 1, 1));
  test = joinAll(polygons, tol);
  ASSERT_EQ(1u, test.size());
  EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 2, 1), test[0])) << test[0];

  // three overlapping
  polygons.clear();
  polygons.push_back(makeRectangleDown(0, 0, 1, 1));
  polygons.push_back(makeRectangleDown(1, 0, 1, 1));
  polygons.push_back(makeRectangleDown(2, 0, 1, 1));
  test = joinAll(polygons, tol);
  ASSERT_EQ(1u, test.size());
  EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 3, 1), test[0])) << test[0];

  // three not overlapping
  polygons.clear();
  polygons.push_back(makeRectangleDown(0, 0, 1, 1));
  polygons.push_back(makeRectangleDown(2, 0, 1, 1));
  polygons.push_back(makeRectangleDown(4, 0, 1, 1));
  test = joinAll(polygons, tol);
  ASSERT_EQ(3u, test.size());

  // two overlapping, one not
  polygons.clear();
  polygons.push_back(makeRectangleDown(0, 0, 1, 1));
  polygons.push_back(makeRectangleDown(1, 0, 1, 1));
  polygons.push_back(makeRectangleDown(3, 0, 1, 1));
  test = joinAll(polygons, tol);
  ASSERT_EQ(2u, test.size());

  bool found1 = false;
  bool found2 = false;
  for (unsigned i = 0; i < 2; ++i){
    if (circularEqual(makeRectangleDown(0, 0, 2, 1), test[i])){
      found1 = true;
    }else if (circularEqual(makeRectangleDown(3, 0, 1, 1), test[i])){
      found2 = true;
    }
  }
  EXPECT_TRUE(found1);
  EXPECT_TRUE(found2);
}


TEST_F(GeometryFixture, RemoveSpikes)
{
  double tol = 0.01;

  std::vector<Point3d> points;
  std::vector<Point3d> expected;
  std::vector<Point3d> result;

  // spike at beginning
  { 
    points.clear();
    points.push_back(Point3d(10, 10, 0)); // the spike
    points.push_back(Point3d(10, 0, 0));
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(0, 5, 0));
    points.push_back(Point3d(10, 5, 0));

    expected.clear();
    expected.push_back(Point3d(10, 5, 0)); 
    expected.push_back(Point3d(10, 0, 0));
    expected.push_back(Point3d(0, 0, 0));
    expected.push_back(Point3d(0, 5, 0));

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }

  // spike at beginning 2
  { 
    points.clear();
    points.push_back(Point3d(10, 5, 0));
    points.push_back(Point3d(10, 10, 0)); // the spike
    points.push_back(Point3d(10, 0, 0));
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(0, 5, 0));

    expected.clear();
    expected.push_back(Point3d(10, 5, 0)); 
    expected.push_back(Point3d(10, 0, 0));
    expected.push_back(Point3d(0, 0, 0));
    expected.push_back(Point3d(0, 5, 0));

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }

  // spike in middle
  { 
    points.clear();
    points.push_back(Point3d(10, 5, 0));
    points.push_back(Point3d(10, 0, 0));
    points.push_back(Point3d(-5, 0, 0)); // the spike
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(0, 5, 0));

    expected.clear();
    expected.push_back(Point3d(10, 5, 0)); 
    expected.push_back(Point3d(10, 0, 0));
    expected.push_back(Point3d(0, 0, 0));
    expected.push_back(Point3d(0, 5, 0));

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }
}
