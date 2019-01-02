/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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


TEST_F(GeometryFixture, PointInPolygon_Down)
{
  double tol = 0.01;
  double tol2 = tol/2.0;

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

  EXPECT_TRUE(within(Point3d(0.5, tol2, 0), points, tol));
  EXPECT_TRUE(within(Point3d(1 - tol2, 0.5, 0), points, tol));
  EXPECT_TRUE(within(Point3d(0.5, 1 - tol2, 0), points, tol));
  EXPECT_TRUE(within(Point3d(tol2, 0.5, 0), points, tol));

  EXPECT_FALSE(within(Point3d(0.5, -tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1 + tol2, 0.5, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0.5, 1 + tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(-tol2, 0.5, 0), points, tol));

  // outside
  EXPECT_FALSE(pointInPolygon(Point3d(2,0,0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(1,2,0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(-1,0,0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(-1,-1,0), points, tol));

  // not on z = 0
  EXPECT_FALSE(pointInPolygon(Point3d(0.5,0.5,0.5), points, tol));
}

TEST_F(GeometryFixture, PointInPolygon_Up)
{
  double tol = 0.01;
  double tol2 = tol / 2.0;

  Point3dVector points = makeRectangleUp(0, 0, 1, 1);

  // center
  EXPECT_FALSE(pointInPolygon(Point3d(0.5, 0.5, 0), points, tol));

  // corners
  EXPECT_FALSE(pointInPolygon(Point3d(0, 0, 0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(1, 0, 0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(1, 1, 0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(0, 1, 0), points, tol));

  // edges
  EXPECT_FALSE(pointInPolygon(Point3d(0.5, 0, 0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(1, 0.5, 0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(0.5, 1, 0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(0, 0.5, 0), points, tol));

  EXPECT_FALSE(within(Point3d(0.5, tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1 - tol2, 0.5, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0.5, 1 - tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(tol2, 0.5, 0), points, tol));

  EXPECT_FALSE(within(Point3d(0.5, -tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1 + tol2, 0.5, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0.5, 1 + tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(-tol2, 0.5, 0), points, tol));

  // outside
  EXPECT_FALSE(pointInPolygon(Point3d(2, 0, 0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(1, 2, 0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(-1, 0, 0), points, tol));
  EXPECT_FALSE(pointInPolygon(Point3d(-1, -1, 0), points, tol));

  // not on z = 0
  EXPECT_FALSE(pointInPolygon(Point3d(0.5, 0.5, 0.5), points, tol));
}

TEST_F(GeometryFixture, Within_Down)
{
  double tol = 0.01;
  double tol2 = tol/2.0;

  Point3dVector points = makeRectangleDown(0, 0, 1, 1);

  // center
  EXPECT_TRUE(within(Point3d(0.5, 0.5, 0), points, tol));

  // corners
  EXPECT_FALSE(within(Point3d(0, 0, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1, 0, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1, 1, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0, 1, 0), points, tol));

  // edges
  EXPECT_FALSE(within(Point3d(0.5, 0, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1, 0.5, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0.5, 1, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0, 0.5, 0), points, tol));

  EXPECT_TRUE(within(Point3d(0.5, tol2, 0), points, tol));
  EXPECT_TRUE(within(Point3d(1 - tol2, 0.5, 0), points, tol));
  EXPECT_TRUE(within(Point3d(0.5, 1 - tol2, 0), points, tol));
  EXPECT_TRUE(within(Point3d(tol2, 0.5, 0), points, tol));

  EXPECT_FALSE(within(Point3d(0.5, -tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1 + tol2, 0.5, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0.5, 1 + tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(-tol2, 0.5, 0), points, tol));

  // outside
  EXPECT_FALSE(within(Point3d(2, 0, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1, 2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(-1, 0, 0), points, tol));
  EXPECT_FALSE(within(Point3d(-1, -1, 0), points, tol));

  // not on z = 0
  EXPECT_FALSE(within(Point3d(0.5, 0.5, 0.5), points, tol));
}

TEST_F(GeometryFixture, Within_Up)
{
  double tol = 0.01;
  double tol2 = tol / 2.0;

  Point3dVector points = makeRectangleUp(0, 0, 1, 1);

  // center
  EXPECT_FALSE(within(Point3d(0.5, 0.5, 0), points, tol));

  // corners
  EXPECT_FALSE(within(Point3d(0, 0, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1, 0, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1, 1, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0, 1, 0), points, tol));

  // edges
  EXPECT_FALSE(within(Point3d(0.5, 0, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1, 0.5, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0.5, 1, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0, 0.5, 0), points, tol));

  EXPECT_FALSE(within(Point3d(0.5, tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1 - tol2, 0.5, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0.5, 1 - tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(tol2, 0.5, 0), points, tol));

  EXPECT_FALSE(within(Point3d(0.5, -tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1 + tol2, 0.5, 0), points, tol));
  EXPECT_FALSE(within(Point3d(0.5, 1 + tol2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(-tol2, 0.5, 0), points, tol));

  // outside
  EXPECT_FALSE(within(Point3d(2, 0, 0), points, tol));
  EXPECT_FALSE(within(Point3d(1, 2, 0), points, tol));
  EXPECT_FALSE(within(Point3d(-1, 0, 0), points, tol));
  EXPECT_FALSE(within(Point3d(-1, -1, 0), points, tol));

  // not on z = 0
  EXPECT_FALSE(within(Point3d(0.5, 0.5, 0.5), points, tol));
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

TEST_F(GeometryFixture, Intersect_Overlap_Down)
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

TEST_F(GeometryFixture, Intersect_Overlap_Up)
{
  double tol = 0.01;

  boost::optional<IntersectionResult> test;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleUp(0, 0, 2, 1);
  points2 = makeRectangleUp(1, 0, 2, 1);

  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);
}

TEST_F(GeometryFixture, Intersect_Within_Down)
{
  double tol = 0.01;

  boost::optional<IntersectionResult> test;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 3, 3);
  points2 = makeRectangleDown(1, 1, 2, 2);

  test = intersect(points1, points2, tol);
  ASSERT_TRUE(test);
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon1())) << test->polygon1();
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon2())) << test->polygon2();
  //ASSERT_EQ(1, test->newPolygons1().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 1, 1), test->newPolygons1()[0])) << test->newPolygons1()[0];
  //ASSERT_EQ(1, test->newPolygons2().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(2, 0, 1, 1), test->newPolygons2()[0])) << test->newPolygons2()[0];

  test = intersect(points2, points1, tol);
  ASSERT_TRUE(test);
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon1())) << test->polygon1();
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon2())) << test->polygon2();
  //ASSERT_EQ(1, test->newPolygons1().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(2, 0, 1, 1), test->newPolygons1()[0])) << test->newPolygons1()[0];
  //ASSERT_EQ(1, test->newPolygons2().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 1, 1), test->newPolygons2()[0])) << test->newPolygons2()[0];
}

TEST_F(GeometryFixture, Intersect_Within_Up)
{
  double tol = 0.01;

  boost::optional<IntersectionResult> test;
  Point3dVector points1;
  Point3dVector points2;

  // sense is up
  points1 = makeRectangleUp(0, 0, 3, 3);
  points2 = makeRectangleUp(1, 1, 2, 2);

  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);
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

TEST_F(GeometryFixture, Join_Overlap_Down)
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

TEST_F(GeometryFixture, Join_Overlap_Up)
{
  double tol = 0.01;

  boost::optional<std::vector<Point3d> > test;
  Point3dVector points1;
  Point3dVector points2;

  // sense is up
  points1 = makeRectangleUp(0, 0, 2, 1);
  points2 = makeRectangleUp(1, 0, 2, 1);

  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);
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


TEST_F(GeometryFixture, RemoveSpikes_Down)
{
  double tol = 0.01;

  std::vector<Point3d> points;
  std::vector<Point3d> expected;
  std::vector<Point3d> result;

  // spike at beginning
  {
    // sense is down
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
    // sense is down
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
    // sense is down
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

TEST_F(GeometryFixture, RemoveSpikes_Up)
{
  double tol = 0.01;

  std::vector<Point3d> points;
  std::vector<Point3d> expected;
  std::vector<Point3d> result;

  // spike at end
  {
    // sense is up
    points.clear();
    points.push_back(Point3d(10, 5, 0));
    points.push_back(Point3d(0, 5, 0));
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(10, 0, 0));
    points.push_back(Point3d(10, 10, 0)); // the spike

    expected.clear();

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }

  // spike at end 2
  {
    // sense is up
    points.clear();
    points.push_back(Point3d(0, 5, 0));
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(10, 0, 0));
    points.push_back(Point3d(10, 10, 0)); // the spike
    points.push_back(Point3d(10, 5, 0));

    expected.clear();

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }

  // spike in middle
  {
    // sense is up
    points.clear();
    points.push_back(Point3d(0, 0, 0));
    points.push_back(Point3d(10, 0, 0));
    points.push_back(Point3d(-5, 0, 0)); // the spike
    points.push_back(Point3d(10, 5, 0));
    points.push_back(Point3d(0, 5, 0));

    expected.clear();

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }
}

TEST_F(GeometryFixture, Subtract_SamePoints)
{
  double tol = 0.01;

  std::vector<std::vector<Point3d> > test;
  std::vector<std::vector<Point3d> > holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);
  points2 = makeRectangleDown(0, 0, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  for (auto polygon : test){
    std::cout << polygon << std::endl;
  }
  EXPECT_DOUBLE_EQ(0.0, totalArea(test));
  //ASSERT_TRUE(test);
  //EXPECT_TRUE(circularEqual(points1, test->polygon1())) << test->polygon1();
  //EXPECT_TRUE(circularEqual(points2, test->polygon2())) << test->polygon2();
  //EXPECT_EQ(0, test->newPolygons1().size());
  //EXPECT_EQ(0, test->newPolygons2().size());

  holes.clear();
  holes.push_back(points1);
  test = subtract(points2, holes, tol);
  for (auto polygon : test){
    std::cout << polygon << std::endl;
  }
  EXPECT_DOUBLE_EQ(0.0, totalArea(test));
  //ASSERT_TRUE(test);
  //EXPECT_TRUE(circularEqual(points2, test->polygon1())) << test->polygon1();
  //EXPECT_TRUE(circularEqual(points1, test->polygon2())) << test->polygon2();
  //EXPECT_EQ(0, test->newPolygons1().size());
  //EXPECT_EQ(0, test->newPolygons2().size());

  // sense is up
  points1 = makeRectangleUp(0, 0, 1, 1);
  points2 = makeRectangleUp(0, 0, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_EQ(0, test.size());

  holes.clear();
  holes.push_back(points1);
  test = subtract(points2, holes, tol);
  EXPECT_EQ(0, test.size());

  // opposite sense, points1 is up, points2 is down
  points1 = makeRectangleUp(0, 0, 1, 1);
  points2 = makeRectangleDown(0, 0, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_EQ(0, test.size());

  holes.clear();
  holes.push_back(points1);
  test = subtract(points2, holes, tol);
  EXPECT_EQ(0, test.size());
}

TEST_F(GeometryFixture, Subtract_Empty_Down)
{
  double tol = 0.01;

  std::vector<std::vector<Point3d> > test;
  std::vector<std::vector<Point3d> > holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);

  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.size() == 1);
  for (auto polygon : test){
    std::cout << polygon << std::endl;
  }
  EXPECT_DOUBLE_EQ(1.0, totalArea(test));

  // can't subtract single point
  points2.push_back(Point3d(1, 1, 1));

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.empty());

  // can't subtract single point
  holes.clear();
  holes.push_back(points1);
  test = subtract(points2, holes, tol);
  EXPECT_TRUE(test.empty());
}

TEST_F(GeometryFixture, Subtract_Empty_Up)
{
  double tol = 0.01;

  std::vector<std::vector<Point3d> > test;
  std::vector<std::vector<Point3d> > holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleUp(0, 0, 1, 1);

  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.empty());

  // can't subtract single point
  points2.push_back(Point3d(1, 1, 1));

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.empty());

  // can't subtract single point
  holes.clear();
  holes.push_back(points1);
  test = subtract(points2, holes, tol);
  EXPECT_TRUE(test.empty());
}

TEST_F(GeometryFixture, Subtract_Adjacent)
{
  double tol = 0.01;

  std::vector<std::vector<Point3d> > test;
  std::vector<std::vector<Point3d> > holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);
  points2 = makeRectangleDown(1, 0, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.size() == 1);
  for (auto polygon : test){
    std::cout << polygon << std::endl;
  }
  EXPECT_DOUBLE_EQ(1.0, totalArea(test));

  holes.clear();
  holes.push_back(points1);
  test = subtract(points2, holes, tol);
  EXPECT_TRUE(test.size() == 1);
  for (auto polygon : test){
    std::cout << polygon << std::endl;
  }
  EXPECT_DOUBLE_EQ(1.0, totalArea(test));

  // sense is up
  points1 = makeRectangleUp(0, 0, 1, 1);
  points2 = makeRectangleUp(1, 0, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_EQ(0, test.size());

  holes.clear();
  holes.push_back(points1);
  test = subtract(points2, holes, tol);
  EXPECT_EQ(0, test.size());
}

TEST_F(GeometryFixture, Subtract_Overlap)
{
  double tol = 0.01;

  std::vector<std::vector<Point3d> > test;
  std::vector<std::vector<Point3d> > holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 2, 1);
  points2 = makeRectangleDown(1, 0, 2, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.size() == 1);
  for (auto polygon : test){
    std::cout << polygon << std::endl;
  }
  EXPECT_DOUBLE_EQ(1.0, totalArea(test));
  //ASSERT_TRUE(test);
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon1())) << test->polygon1();
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon2())) << test->polygon2();
  //ASSERT_EQ(1, test->newPolygons1().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 1, 1), test->newPolygons1()[0])) << test->newPolygons1()[0];
  //ASSERT_EQ(1, test->newPolygons2().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(2, 0, 1, 1), test->newPolygons2()[0])) << test->newPolygons2()[0];

  holes.clear();
  holes.push_back(points1);
  test = subtract(points2, holes, tol);
  EXPECT_TRUE(test.size() == 1);
  for (auto polygon : test){
    std::cout << polygon << std::endl;
  }
  EXPECT_DOUBLE_EQ(1.0, totalArea(test));
  //ASSERT_TRUE(test);
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon1())) << test->polygon1();
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon2())) << test->polygon2();
  //ASSERT_EQ(1, test->newPolygons1().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(2, 0, 1, 1), test->newPolygons1()[0])) << test->newPolygons1()[0];
  //ASSERT_EQ(1, test->newPolygons2().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 1, 1), test->newPolygons2()[0])) << test->newPolygons2()[0];
}

TEST_F(GeometryFixture, Subtract_Within_Down)
{
  double tol = 0.01;

  std::vector<std::vector<Point3d> > test;
  std::vector<std::vector<Point3d> > holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 3, 3);
  points2 = makeRectangleDown(1, 1, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.size() > 1);
  for (auto polygon : test){
    std::cout << polygon << std::endl;
  }
  EXPECT_DOUBLE_EQ(8.0, totalArea(test));
  //ASSERT_TRUE(test);
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon1())) << test->polygon1();
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon2())) << test->polygon2();
  //ASSERT_EQ(1, test->newPolygons1().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 1, 1), test->newPolygons1()[0])) << test->newPolygons1()[0];
  //ASSERT_EQ(1, test->newPolygons2().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(2, 0, 1, 1), test->newPolygons2()[0])) << test->newPolygons2()[0];

  holes.clear();
  holes.push_back(points1);
  test = subtract(points2, holes, tol);
  EXPECT_TRUE(test.empty());
  for (auto polygon : test){
    std::cout << polygon << std::endl;
  }
  EXPECT_DOUBLE_EQ(0.0, totalArea(test));
  //ASSERT_TRUE(test);
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon1())) << test->polygon1();
  //EXPECT_TRUE(circularEqual(makeRectangleDown(1, 0, 1, 1), test->polygon2())) << test->polygon2();
  //ASSERT_EQ(1, test->newPolygons1().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(2, 0, 1, 1), test->newPolygons1()[0])) << test->newPolygons1()[0];
  //ASSERT_EQ(1, test->newPolygons2().size());
  //EXPECT_TRUE(circularEqual(makeRectangleDown(0, 0, 1, 1), test->newPolygons2()[0])) << test->newPolygons2()[0];
}

TEST_F(GeometryFixture, Subtract_Within_Up)
{
  double tol = 0.01;

  std::vector<std::vector<Point3d> > test;
  std::vector<std::vector<Point3d> > holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is up
  points1 = makeRectangleUp(0, 0, 3, 3);
  points2 = makeRectangleUp(1, 1, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.empty());

  holes.clear();
  holes.push_back(points1);
  test = subtract(points2, holes, tol);
  EXPECT_TRUE(test.empty());
}


TEST_F(GeometryFixture, selfIntersects)
{
  double tol = 0.01;

  std::vector<std::vector<Point3d> > test;
  std::vector<std::vector<Point3d> > holes;
  Point3dVector points;

  points.clear();
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(2, 1, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  EXPECT_FALSE(selfIntersects(points,tol));

  points.clear();
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(2, 1, 0));
  points.push_back(Point3d(0, 0, 0));
  points.push_back(Point3d(2, 0, 0));
  EXPECT_TRUE(selfIntersects(points,tol));

  points.clear();
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 0.2, 0));
  points.push_back(Point3d(2, 1, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  EXPECT_FALSE(selfIntersects(points,tol));

  points.clear();
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, 0, 0));
  points.push_back(Point3d(2, 1, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  EXPECT_FALSE(selfIntersects(points,tol));

  points.clear();
  points.push_back(Point3d(0, 1, 0));
  points.push_back(Point3d(1, -1, 0));
  points.push_back(Point3d(2, 1, 0));
  points.push_back(Point3d(2, 0, 0));
  points.push_back(Point3d(0, 0, 0));
  EXPECT_TRUE(selfIntersects(points,tol));
}

TEST_F(GeometryFixture, simplify)
{
  double tol = 0.01;

  Point3dVector points;

  points.clear();
  points.push_back(Point3d(158, 168, 0));
  points.push_back(Point3d(200, 168, 0));
  points.push_back(Point3d(200, 30, 0));
  points.push_back(Point3d(158, 30, 0));
  points.push_back(Point3d(158, 40, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 40, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 40, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 40, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 138, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 40, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 138, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 138, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 138, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 138, 0));

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << std::endl;

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol*tol);

  Point3dVector expectedPoints;
  expectedPoints.push_back(Point3d(200, 168, 0));
  expectedPoints.push_back(Point3d(200, 30, 0));
  expectedPoints.push_back(Point3d(158, 30, 0));
  expectedPoints.push_back(Point3d(158, 40, 0));
  expectedPoints.push_back(Point3d(158, 70, 0));
  expectedPoints.push_back(Point3d(158, 98, 0));
  expectedPoints.push_back(Point3d(158, 128, 0));
  expectedPoints.push_back(Point3d(158, 138, 0));
  expectedPoints.push_back(Point3d(158, 168, 0));

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol*tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (auto i = 0; i < expectedPoints.size(); ++i){
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }
}


TEST_F(GeometryFixture, simplify2)
{
  double tol = 0.01;

  Point3dVector points;

  points.clear();
  points.push_back(Point3d(158, 168, 0));
  points.push_back(Point3d(200, 168, 0));
  points.push_back(Point3d(200, 30, 0));
  points.push_back(Point3d(158, 30, 0));
  points.push_back(Point3d(158, 40, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 40, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 40, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 40, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 138, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 40, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 138, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 70, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 138, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 98, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 138, 0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 138, 0));
  points = reverse(points);

  std::cout << points << std::endl << std::endl;

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << std::endl;

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol*tol);

  Point3dVector expectedPoints;
  expectedPoints.push_back(Point3d(200, 30, 0));
  expectedPoints.push_back(Point3d(200, 168, 0));
  expectedPoints.push_back(Point3d(158, 168, 0));
  expectedPoints.push_back(Point3d(158, 138, 0));
  expectedPoints.push_back(Point3d(158, 128, 0));
  expectedPoints.push_back(Point3d(158, 98, 0));
  expectedPoints.push_back(Point3d(158, 70, 0));
  expectedPoints.push_back(Point3d(158, 40, 0));
  expectedPoints.push_back(Point3d(158, 30, 0));

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol*tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (auto i = 0; i < expectedPoints.size(); ++i){
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }
}


TEST_F(GeometryFixture, simplify3)
{
  double tol = 0.01;

  Point3dVector points;

  points.clear();
  points.push_back(Point3d(158, 49, 0));
  points.push_back(Point3d(158, 108, 0));
  points.push_back(Point3d(158, 89, 0));
  points.push_back(Point3d(158, 21, 0));
  points.push_back(Point3d(157, 21, 0));
  points.push_back(Point3d(144, 21, 0));
  points.push_back(Point3d(143, 21, 0));
  points.push_back(Point3d(114, 21, 0));
  points.push_back(Point3d(113, 21, 0));
  points.push_back(Point3d(113, 28, 0));
  points.push_back(Point3d(113, 50, 0));
  points.push_back(Point3d(113, 70, 0));
  points.push_back(Point3d(113, 91, 0));
  points.push_back(Point3d(113, 98, 0));
  points.push_back(Point3d(113, 119, 0));
  points.push_back(Point3d(113, 120, 0));
  points.push_back(Point3d(114, 120, 0));
  points.push_back(Point3d(143, 120, 0));
  points.push_back(Point3d(157, 120, 0));
  points.push_back(Point3d(158, 120, 0));

  std::cout << points << std::endl << std::endl;

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << std::endl;

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol*tol);

  Point3dVector expectedPoints;
  expectedPoints.push_back(Point3d(158, 120, 0));
  expectedPoints.push_back(Point3d(158, 108, 0));
  expectedPoints.push_back(Point3d(158, 89, 0));
  expectedPoints.push_back(Point3d(158, 49, 0));
  expectedPoints.push_back(Point3d(158, 21, 0));
  expectedPoints.push_back(Point3d(157, 21, 0));
  expectedPoints.push_back(Point3d(144, 21, 0));
  expectedPoints.push_back(Point3d(143, 21, 0));
  expectedPoints.push_back(Point3d(114, 21, 0));
  expectedPoints.push_back(Point3d(113, 21, 0));
  expectedPoints.push_back(Point3d(113, 28, 0));
  expectedPoints.push_back(Point3d(113, 50, 0));
  expectedPoints.push_back(Point3d(113, 70, 0));
  expectedPoints.push_back(Point3d(113, 91, 0));
  expectedPoints.push_back(Point3d(113, 98, 0));
  expectedPoints.push_back(Point3d(113, 119, 0));
  expectedPoints.push_back(Point3d(113, 120, 0));
  expectedPoints.push_back(Point3d(114, 120, 0));
  expectedPoints.push_back(Point3d(143, 120, 0));
  expectedPoints.push_back(Point3d(157, 120, 0));

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol*tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (auto i = 0; i < expectedPoints.size(); ++i){
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }
}


TEST_F(GeometryFixture, simplify4)
{
  double tol = 0.01;

  Point3dVector points;
  points.clear();
  points.push_back(Point3d(158, 120, 0));
  points.push_back(Point3d(158, 49, 0));
  points.push_back(Point3d(158, 108, 0));
  points.push_back(Point3d(158, 21, 0));
  points.push_back(Point3d(113, 21, 0));
  points.push_back(Point3d(113, 120, 0));

  std::cout << points << std::endl << std::endl;

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << std::endl << std::endl;

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol*tol);

  Point3dVector expectedPoints;
  expectedPoints.push_back(Point3d(158, 120, 0));
  expectedPoints.push_back(Point3d(158, 108, 0));
  expectedPoints.push_back(Point3d(158, 49, 0));
  expectedPoints.push_back(Point3d(158, 21, 0));
  expectedPoints.push_back(Point3d(113, 21, 0));
  expectedPoints.push_back(Point3d(113, 120, 0));

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol*tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  std::cout << points2 << std::endl;
  std::cout << expectedPoints << std::endl;
  for (auto i = 0; i < expectedPoints.size(); ++i){
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }
}

TEST_F(GeometryFixture, simplify5)
{
  double tol = 0.01;

  Point3dVector points;
  points.clear();
  points.push_back(Point3d(158, 98,  0));
  points.push_back(Point3d(158, 138, 0));
  points.push_back(Point3d(158, 40,  0));
  points.push_back(Point3d(158, 70,  0));
  points.push_back(Point3d(158, 128, 0));
  points.push_back(Point3d(158, 168, 0));
  points.push_back(Point3d(200, 168, 0));
  points.push_back(Point3d(200, 30,  0));
  points.push_back(Point3d(158, 30,  0));

  std::cout << points << std::endl << std::endl;

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << std::endl << std::endl;

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol*tol);

  Point3dVector expectedPoints;
  expectedPoints.push_back(Point3d(200, 168, 0));
  expectedPoints.push_back(Point3d(200, 30, 0));
  expectedPoints.push_back(Point3d(158, 30, 0));
  expectedPoints.push_back(Point3d(158, 40, 0));
  expectedPoints.push_back(Point3d(158, 70, 0));
  expectedPoints.push_back(Point3d(158, 98, 0));
  expectedPoints.push_back(Point3d(158, 128, 0));
  expectedPoints.push_back(Point3d(158, 138, 0));
  expectedPoints.push_back(Point3d(158, 168, 0));

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol*tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (auto i = 0; i < expectedPoints.size(); ++i){
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }

  // remove colinear
  Point3dVector points4 = simplify(points, true, tol);
  EXPECT_EQ(4u, points4.size());

  std::cout << points4 << std::endl << std::endl;

  auto area4 = getArea(points4);
  ASSERT_TRUE(area4);

  EXPECT_NEAR(*area, *area4, tol*tol);
}

TEST_F(GeometryFixture, simplify6)
{
  double tol = 0.01;

  Point3dVector points;
  points.clear();
  points.push_back(Point3d(3.6576, 3.048, 0));
  points.push_back(Point3d(5.4864, 3.048, 0));
  points.push_back(Point3d(5.4864, 5.1816, 0));
  points.push_back(Point3d(5.4864, 11.5824, 0));
  points.push_back(Point3d(5.4864, 5.1816, 0));
  points.push_back(Point3d(5.4864, 11.5824, 0));
  points.push_back(Point3d(5.4864, 17.0688, 0));
  points.push_back(Point3d(3.6576, 17.0688, 0));
  points.push_back(Point3d(3.6576, 14.9352, 0));
  points.push_back(Point3d(3.6576, 11.5824, 0));
  points.push_back(Point3d(3.6576, 7.0104, 0));
  points.push_back(Point3d(3.6576, 11.5824, 0));
  points.push_back(Point3d(3.6576, 7.0104, 0));

  std::cout << points << std::endl << std::endl;

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << std::endl << std::endl;

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol*tol);

  Point3dVector expectedPoints;
  expectedPoints.push_back(Point3d(5.4864, 3.048, 0));
  expectedPoints.push_back(Point3d(5.4864, 5.1816, 0));
  expectedPoints.push_back(Point3d(5.4864, 11.5824, 0));
  expectedPoints.push_back(Point3d(5.4864, 17.0688, 0));
  expectedPoints.push_back(Point3d(3.6576, 17.0688, 0));
  expectedPoints.push_back(Point3d(3.6576, 14.9352, 0));
  expectedPoints.push_back(Point3d(3.6576, 11.5824, 0));
  expectedPoints.push_back(Point3d(3.6576, 7.0104, 0));
  expectedPoints.push_back(Point3d(3.6576, 3.048, 0));

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol*tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (auto i = 0; i < expectedPoints.size(); ++i){
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }

  // remove colinear
  Point3dVector points4 = simplify(points, true, tol);
  EXPECT_EQ(4u, points4.size());

  std::cout << points4 << std::endl << std::endl;

  auto area4 = getArea(points4);
  ASSERT_TRUE(area4);

  EXPECT_NEAR(*area, *area4, tol*tol);
}

TEST_F(GeometryFixture, simplify7)
{
  double tol = 0.01;

  Point3dVector points;
  points.clear();
  points.push_back(Point3d(3.6576, 3.048, 0));
  points.push_back(Point3d(3.6576, 11.5824, 0));
  points.push_back(Point3d(3.6576, 7.0104, 0));
  points.push_back(Point3d(3.6576, 14.9352, 0));
  points.push_back(Point3d(3.6576, 17.0688, 0));
  points.push_back(Point3d(5.4864, 17.0688, 0));
  points.push_back(Point3d(5.4864, 5.1816, 0));
  points.push_back(Point3d(5.4864, 11.5824, 0));
  points.push_back(Point3d(5.4864, 3.048, 0));

  std::cout << points << std::endl << std::endl;

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << std::endl << std::endl;

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol*tol);

  Point3dVector expectedPoints;
  expectedPoints.push_back(Point3d(5.4864, 17.0688, 0));
  expectedPoints.push_back(Point3d(5.4864, 11.5824, 0));
  expectedPoints.push_back(Point3d(5.4864, 5.1816, 0));
  expectedPoints.push_back(Point3d(5.4864, 3.048, 0));
  expectedPoints.push_back(Point3d(3.6576, 3.048, 0));
  expectedPoints.push_back(Point3d(3.6576, 7.0104, 0));
  expectedPoints.push_back(Point3d(3.6576, 11.5824, 0));
  expectedPoints.push_back(Point3d(3.6576, 14.9352, 0));
  expectedPoints.push_back(Point3d(3.6576, 17.0688, 0));

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol*tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (auto i = 0; i < expectedPoints.size(); ++i){
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }

  // remove colinear
  Point3dVector points4 = simplify(points, true, tol);
  EXPECT_EQ(4u, points4.size());

  std::cout << points4 << std::endl << std::endl;

  auto area4 = getArea(points4);
  ASSERT_TRUE(area4);

  EXPECT_NEAR(*area, *area4, tol*tol);
}


