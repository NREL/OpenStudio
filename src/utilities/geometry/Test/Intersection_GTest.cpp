/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#if defined(_MSC_VER)
  #pragma warning(push)
  #pragma warning(disable:4244)
#endif
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
#if defined(_MSC_VER)
  #pragma warning(pop)
#endif

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

  // out of order
  polygons.clear();
  polygons.push_back(makeRectangleDown(0, 0, 1, 1));
  polygons.push_back(makeRectangleDown(3, 0, 1, 1));
  polygons.push_back(makeRectangleDown(2, 0, 1, 1));
  polygons.push_back(makeRectangleDown(1, 0, 1, 1));
  test = joinAll(polygons, tol);
  ASSERT_EQ(1u, test.size());
  EXPECT_EQ(4.0, totalArea(test));
}

// joinAll fails on cases with an inner loop
TEST_F(GeometryFixture, JoinAll_1614) {
  double tol = 0.01;

  std::vector<Point3dVector> test;
  std::vector<Point3dVector> polygons;

  std::vector<Point3d> poly1;
  poly1.push_back(Point3d(0, 7000, 0));
  poly1.push_back(Point3d(0, 9000, 0));
  poly1.push_back(Point3d(10000, 9000, 0));
  poly1.push_back(Point3d(10000, 7000, 0));

  std::vector<Point3d> poly2;
  poly2.push_back(Point3d(0, 1000, 0));
  poly2.push_back(Point3d(0, 3000, 0));
  poly2.push_back(Point3d(10000, 3000, 0));
  poly2.push_back(Point3d(10000, 1000, 0));

  std::vector<Point3d> poly3;
  poly3.push_back(Point3d(1000, 0, 0));
  poly3.push_back(Point3d(1000, 10000, 0));
  poly3.push_back(Point3d(3000, 10000, 0));
  poly3.push_back(Point3d(3000, 0, 0));

  std::vector<Point3d> poly4;
  poly4.push_back(Point3d(7000, 0, 0));
  poly4.push_back(Point3d(7000, 10000, 0));
  poly4.push_back(Point3d(9000, 10000, 0));
  poly4.push_back(Point3d(9000, 0, 0));

  polygons.push_back(poly1);
  polygons.push_back(poly4);
  polygons.push_back(poly2);
  polygons.push_back(poly3);

  ASSERT_EQ(getArea(poly1), getArea(poly2));
  ASSERT_EQ(getArea(poly2), getArea(poly3));
  ASSERT_EQ(getArea(poly3), getArea(poly4));
  ASSERT_EQ(getArea(poly4), getArea(poly1));

  test = joinAll(polygons, tol);

  // Should return one polygon
  ASSERT_EQ(1u, test.size());

  // That polygon should have 28 points
  // We know this fails because we know joinAll gives up when it ends up with a polygon with a hole
  ASSERT_EQ(28, test[0].size());

}

TEST_F(GeometryFixture, JoinAll_2527) {
  double tol = 0.01;

  std::vector<Point3dVector> test;
  std::vector<Point3dVector> polygons;

  // North
  std::vector<Point3d> poly1;
  poly1.push_back(Point3d(40.869, 30439.131, 0));
  poly1.push_back(Point3d(30439.131, 30439.131, 0));
  poly1.push_back(Point3d(25867, 25867, 0));
  poly1.push_back(Point3d(4612, 25867, 0));
  // East
  std::vector<Point3d> poly2;
  poly2.push_back(Point3d(30439.131, 30439.131, 0));
  poly2.push_back(Point3d(30439.131, 40.869, 0));
  poly2.push_back(Point3d(25867, 4612, 0));
  poly2.push_back(Point3d(25867, 25867, 0));
  std::vector<Point3d> poly3;
  // West
  poly3.push_back(Point3d(40.869, 40.869, 0));
  poly3.push_back(Point3d(40.869, 30439.131, 0));
  poly3.push_back(Point3d(4612, 25867, 0));
  poly3.push_back(Point3d(4612, 4612, 0));
  // Core
  std::vector<Point3d> poly4;
  poly4.push_back(Point3d(25867, 4612, 0));
  poly4.push_back(Point3d(4612, 4612, 0));
  poly4.push_back(Point3d(4612, 25867, 0));
  poly4.push_back(Point3d(25867, 25867, 0));
  // divide the bottom poly left to right, tri, quad, quad, tri
  std::vector<Point3d> poly5;
  poly5.push_back(Point3d(4612, 4612, 0));
  poly5.push_back(Point3d(4612, 40.869, 0));
  poly5.push_back(Point3d(40.869, 40.869, 0));
  std::vector<Point3d> poly6;
  poly6.push_back(Point3d(4612, 4612, 0));
  poly6.push_back(Point3d(4612, 40.869, 0));
  poly6.push_back(Point3d(15219.565, 40.869, 0));
  poly6.push_back(Point3d(15219.565, 4612, 0));
  std::vector<Point3d> poly7;
  poly7.push_back(Point3d(15219.565, 4612, 0));
  poly7.push_back(Point3d(15219.565, 40.869, 0));
  poly7.push_back(Point3d(25867, 40.869, 0));
  poly7.push_back(Point3d(25867, 4612, 0));
  std::vector<Point3d> poly8;
  poly8.push_back(Point3d(25867, 4612, 0));
  poly8.push_back(Point3d(30439.131, 40.869, 0));
  poly8.push_back(Point3d(25867, 40.869, 0));

  std::vector<Point3d> polyx;
  polyx.push_back(Point3d(30439.131, 40.869, 0));
  polyx.push_back(Point3d(40.869, 40.869, 0));
  polyx.push_back(Point3d(4612, 4612, 0));
  polyx.push_back(Point3d(25867, 4612, 0));

  polygons.push_back(poly1);
  polygons.push_back(poly2);
  polygons.push_back(poly3);
  polygons.push_back(poly4);
  polygons.push_back(poly5);
  polygons.push_back(poly6);
  polygons.push_back(poly7);
  polygons.push_back(poly8);

  joinAllWithBuffer(polygons, tol, 5.0);

  test = joinAll(polygons, tol);

  // We know this fails because join all does not in fact join all
  ASSERT_EQ(1u, test.size());

  std::vector<Point3d> poly9;
  poly9.push_back(Point3d(40.869, 30439.131, 0));
  poly9.push_back(Point3d(30439.131, 30439.131, 0));
  poly9.push_back(Point3d(30439.131, 40.869, 0));
  poly9.push_back(Point3d(40.869, 40.869, 0));
  polygons.push_back(poly6);

  EXPECT_TRUE(circularEqual(poly6, test[0]));
}

/// <summary>
/// Polygons from roof slabs from BaseCase inmport (from our test data)
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(GeometryFixture, JoinAll_Points4) {
  double tol = 2;

  std::vector<Point3dVector> test;
  std::vector<Point3dVector> polygons;

  std::vector<Point3d> poly1;
  poly1.push_back(Point3d(-14302, 37295, 0));
  poly1.push_back(Point3d(-16228, 37295, 0));
  poly1.push_back(Point3d(-16086, 41746, 0));
  poly1.push_back(Point3d(-14302, 41746, 0));
  poly1.push_back(Point3d(-14302, 37295, 0));

  std::vector<Point3d> poly2;
  poly2.push_back(Point3d(-11564, 43144, 0));
  poly2.push_back(Point3d(-13645, 43144, 0));
  poly2.push_back(Point3d(-13645, 44355, 0));
  poly2.push_back(Point3d(-11564, 44355, 0));
  poly2.push_back(Point3d(-11564, 43144, 0));

  std::vector<Point3d> poly3;
  poly3.push_back(Point3d(-11564, 41746, 0));
  poly3.push_back(Point3d(-19601, 41746, 0));
  poly3.push_back(Point3d(-19601, 43144, 0));
  poly3.push_back(Point3d(-11564, 43144, 0));
  poly3.push_back(Point3d(-11564, 41746, 0));

  std::vector<Point3d> poly4;
  poly4.push_back(Point3d(-11297, 37295, 0));
  poly4.push_back(Point3d(-14302, 37295, 0));
  poly4.push_back(Point3d(-14302, 39004, 0));
  poly4.push_back(Point3d(-11297, 39004, 0));
  poly4.push_back(Point3d(-11297, 37295, 0));

  std::vector<Point3d> poly5;
  poly5.push_back(Point3d(-2764, 43461, 0));
  poly5.push_back(Point3d(-4041, 43461, 0));
  poly5.push_back(Point3d(-3956, 45336, 0));
  poly5.push_back(Point3d(-2764, 45282, 0));
  poly5.push_back(Point3d(-2764, 43461, 0));

  std::vector<Point3d> poly6;
  poly6.push_back(Point3d(-2764, 41746, 0));
  poly6.push_back(Point3d(-9244, 41746, 0));
  poly6.push_back(Point3d(-9166, 43461, 0));
  poly6.push_back(Point3d(-2764, 43461, 0));
  poly6.push_back(Point3d(-2764, 41746, 0));

  std::vector<Point3d> poly7;
  poly7.push_back(Point3d(-2764, 39004, 0));
  poly7.push_back(Point3d(-14302, 39004, 0));
  poly7.push_back(Point3d(-14302, 41746, 0));
  poly7.push_back(Point3d(-2764, 41746, 0));
  poly7.push_back(Point3d(-2764, 39004, 0));

  std::vector<Point3d> poly8;
  poly8.push_back(Point3d(-19601, 41551, 0));
  poly8.push_back(Point3d(-20590, 41551, 0));
  poly8.push_back(Point3d(-20510, 43144, 0));
  poly8.push_back(Point3d(-19601, 43144, 0));
  poly8.push_back(Point3d(-19601, 41551, 0));

  std::vector<Point3d> poly9;
  poly9.push_back(Point3d(-13645, 43144, 0));
  poly9.push_back(Point3d(-20510, 43144, 0));
  poly9.push_back(Point3d(-20494, 43461, 0));
  poly9.push_back(Point3d(-13645, 43461, 0));
  poly9.push_back(Point3d(-13645, 43144, 0));

  std::vector<Point3d> poly10;
  poly10.push_back(Point3d(-9244, 41746, 0));
  poly10.push_back(Point3d(-11564, 41746, 0));
  poly10.push_back(Point3d(-11564, 43461, 0));
  poly10.push_back(Point3d(-9166, 43461, 0));
  poly10.push_back(Point3d(-9244, 41746, 0));

  std::vector<Point3d> poly11;
  poly11.push_back(Point3d(-13645, 43461, 0));
  poly11.push_back(Point3d(-20494, 43461, 0));
  poly11.push_back(Point3d(-20450, 44355, 0));
  poly11.push_back(Point3d(-13645, 44355, 0));
  poly11.push_back(Point3d(-13645, 43461, 0));

  std::vector<Point3d> poly12;
  poly12.push_back(Point3d(-11564, 44355, 0));
  poly12.push_back(Point3d(-20450, 44355, 0));
  poly12.push_back(Point3d(-20364, 46080, 0));
  poly12.push_back(Point3d(-11564, 45681, 0));
  poly12.push_back(Point3d(-11564, 44355, 0));

  std::vector<Point3d> poly13;
  poly13.push_back(Point3d(-4041, 43461, 0));
  poly13.push_back(Point3d(-11564, 43461, 0));
  poly13.push_back(Point3d(-11564, 45681, 0));
  poly13.push_back(Point3d(-3956, 45336, 0));
  poly13.push_back(Point3d(-4041, 43461, 0));

  std::vector<Point3d> poly14;
  poly14.push_back(Point3d(-46862, 25939, 0));
  poly14.push_back(Point3d(-47174, 25939, 0));
  poly14.push_back(Point3d(-47238, 27327, 0));
  poly14.push_back(Point3d(-46987, 27523, 0));
  poly14.push_back(Point3d(-46862, 25939, 0));

  std::vector<Point3d> poly15;
  poly15.push_back(Point3d(-46488, 21176, 0));
  poly15.push_back(Point3d(-46987, 27523, 0));
  poly15.push_back(Point3d(-46607, 27819, 0));
  poly15.push_back(Point3d(-46488, 21176, 0));

  std::vector<Point3d> poly16;
  poly16.push_back(Point3d(-44219, 17156, 0));
  poly16.push_back(Point3d(-46415, 17117, 0));
  poly16.push_back(Point3d(-46607, 27819, 0));
  poly16.push_back(Point3d(-46044, 28258, 0));
  poly16.push_back(Point3d(-43736, 28258, 0));
  poly16.push_back(Point3d(-44219, 17156, 0));

  std::vector<Point3d> poly17;
  poly17.push_back(Point3d(-30808, 28712, 0));
  poly17.push_back(Point3d(-38011, 28712, 0));
  poly17.push_back(Point3d(-37501, 42003, 0));
  poly17.push_back(Point3d(-30808, 41746, 0));
  poly17.push_back(Point3d(-30808, 28712, 0));

  std::vector<Point3d> poly18;
  poly18.push_back(Point3d(-37835, 47875, 0));
  poly18.push_back(Point3d(-44738, 41123, 0));
  poly18.push_back(Point3d(-44731, 48139, 0));
  poly18.push_back(Point3d(-37835, 47875, 0));

  std::vector<Point3d> poly19;
  poly19.push_back(Point3d(-43736, 28258, 0));
  poly19.push_back(Point3d(-46044, 28258, 0));
  poly19.push_back(Point3d(-46496, 29382, 0));
  poly19.push_back(Point3d(-46675, 39229, 0));
  poly19.push_back(Point3d(-43108, 42717, 0));
  poly19.push_back(Point3d(-43736, 28258, 0));

  std::vector<Point3d> poly20;
  poly20.push_back(Point3d(-40615, 20247, 0));
  poly20.push_back(Point3d(-44084, 20247, 0));
  poly20.push_back(Point3d(-43717, 28712, 0));
  poly20.push_back(Point3d(-40615, 28712, 0));
  poly20.push_back(Point3d(-40615, 20247, 0));

  std::vector<Point3d> poly21;
  poly21.push_back(Point3d(-38011, 28712, 0));
  poly21.push_back(Point3d(-43717, 28712, 0));
  poly21.push_back(Point3d(-43108, 42717, 0));
  poly21.push_back(Point3d(-37835, 47875, 0));
  poly21.push_back(Point3d(-37276, 47853, 0));
  poly21.push_back(Point3d(-38011, 28712, 0));

  std::vector<Point3d> poly22;
  poly22.push_back(Point3d(-46496, 29382, 0));
  poly22.push_back(Point3d(-47669, 32299, 0));
  poly22.push_back(Point3d(-48139, 41675, 0));
  poly22.push_back(Point3d(-46721, 41746, 0));
  poly22.push_back(Point3d(-46496, 29382, 0));

  std::vector<Point3d> poly23;
  poly23.push_back(Point3d(-30808, 20247, 0));
  poly23.push_back(Point3d(-36869, 20247, 0));
  poly23.push_back(Point3d(-36869, 26461, 0));
  poly23.push_back(Point3d(-30808, 26461, 0));
  poly23.push_back(Point3d(-30808, 20247, 0));

  std::vector<Point3d> poly24;
  poly24.push_back(Point3d(-16373, 32765, 0));
  poly24.push_back(Point3d(-17694, 31941, 0));
  poly24.push_back(Point3d(-18761, 31941, 0));
  poly24.push_back(Point3d(-18761, 37295, 0));
  poly24.push_back(Point3d(-16228, 37295, 0));
  poly24.push_back(Point3d(-16373, 32765, 0));

  std::vector<Point3d> poly25;
  poly25.push_back(Point3d(-23109, 29807, 0));
  poly25.push_back(Point3d(-23109, 32671, 0));
  poly25.push_back(Point3d(-22146, 32299, 0));
  poly25.push_back(Point3d(-23109, 29807, 0));

  std::vector<Point3d> poly26;
  poly26.push_back(Point3d(-30808, 41746, 0));
  poly26.push_back(Point3d(-30974, 46061, 0));
  poly26.push_back(Point3d(-30808, 46058, 0));
  poly26.push_back(Point3d(-30808, 41746, 0));

  std::vector<Point3d> poly27;
  poly27.push_back(Point3d(-24688, 28712, 0));
  poly27.push_back(Point3d(-30808, 28712, 0));
  poly27.push_back(Point3d(-30808, 46058, 0));
  poly27.push_back(Point3d(-24056, 45909, 0));
  poly27.push_back(Point3d(-24688, 28712, 0));

  std::vector<Point3d> poly28;
  poly28.push_back(Point3d(-24402, 26461, 0));
  poly28.push_back(Point3d(-24771, 26461, 0));
  poly28.push_back(Point3d(-24056, 45909, 0));
  poly28.push_back(Point3d(-23109, 45888, 0));
  poly28.push_back(Point3d(-23109, 29807, 0));
  poly28.push_back(Point3d(-24402, 26461, 0));

  std::vector<Point3d> poly29;
  poly29.push_back(Point3d(-26122, 20247, 0));
  poly29.push_back(Point3d(-26803, 20247, 0));
  poly29.push_back(Point3d(-25316, 24095, 0));
  poly29.push_back(Point3d(-25053, 22835, 0));
  poly29.push_back(Point3d(-26122, 20247, 0));

  std::vector<Point3d> poly30;
  poly30.push_back(Point3d(-26803, 20247, 0));
  poly30.push_back(Point3d(-30808, 20247, 0));
  poly30.push_back(Point3d(-30808, 26461, 0));
  poly30.push_back(Point3d(-24402, 26461, 0));
  poly30.push_back(Point3d(-26803, 20247, 0));

  std::vector<Point3d> poly31;
  poly31.push_back(Point3d(-27224, 17579, 0));
  poly31.push_back(Point3d(-28480, 16930, 0));
  poly31.push_back(Point3d(-35780, 16930, 0));
  poly31.push_back(Point3d(-35780, 20247, 0));
  poly31.push_back(Point3d(-26122, 20247, 0));
  poly31.push_back(Point3d(-27224, 17579, 0));

  std::vector<Point3d> poly32;
  poly32.push_back(Point3d(2649, 32120, 0));
  poly32.push_back(Point3d(687, 32120, 0));
  poly32.push_back(Point3d(2059, 32554, 0));
  poly32.push_back(Point3d(2649, 32120, 0));

  std::vector<Point3d> poly33;
  poly33.push_back(Point3d(11822, 25369, 0));
  poly33.push_back(Point3d(9904, 26780, 0));
  poly33.push_back(Point3d(11822, 26780, 0));
  poly33.push_back(Point3d(11822, 25369, 0));

  std::vector<Point3d> poly34;
  poly34.push_back(Point3d(12857, 8600, 0));
  poly34.push_back(Point3d(4742, 15985, 0));
  poly34.push_back(Point3d(6889, 21534, 0));
  poly34.push_back(Point3d(12857, 8600, 0));

  std::vector<Point3d> poly35;
  poly35.push_back(Point3d(4742, 15985, 0));
  poly35.push_back(Point3d(1490, 18944, 0));
  poly35.push_back(Point3d(7393, 22835, 0));
  poly35.push_back(Point3d(4742, 15985, 0));

  std::vector<Point3d> poly36;
  poly36.push_back(Point3d(8785, 23752, 0));
  poly36.push_back(Point3d(1490, 18944, 0));
  poly36.push_back(Point3d(-3792, 23752, 0));
  poly36.push_back(Point3d(8785, 23752, 0));

  std::vector<Point3d> poly37;
  poly37.push_back(Point3d(11822, 23752, 0));
  poly37.push_back(Point3d(-3792, 23752, 0));
  poly37.push_back(Point3d(-12988, 32120, 0));
  poly37.push_back(Point3d(2649, 32120, 0));
  poly37.push_back(Point3d(11822, 25369, 0));
  poly37.push_back(Point3d(11822, 23752, 0));

  std::vector<Point3d> poly38;
  poly38.push_back(Point3d(21882, 4655, 0));
  poly38.push_back(Point3d(19153, 4655, 0));
  poly38.push_back(Point3d(14896, 6843, 0));
  poly38.push_back(Point3d(21882, 6843, 0));
  poly38.push_back(Point3d(21882, 4655, 0));

  std::vector<Point3d> poly39;
  poly39.push_back(Point3d(19153, 4655, 0));
  poly39.push_back(Point3d(17193, 4655, 0));
  poly39.push_back(Point3d(14650, 6969, 0));
  poly39.push_back(Point3d(19153, 4655, 0));

  std::vector<Point3d> poly40;
  poly40.push_back(Point3d(17193, 4655, 0));
  poly40.push_back(Point3d(14678, 4655, 0));
  poly40.push_back(Point3d(13287, 7670, 0));
  poly40.push_back(Point3d(14650, 6969, 0));
  poly40.push_back(Point3d(17193, 4655, 0));

  std::vector<Point3d> poly41;
  poly41.push_back(Point3d(-635, 4874, 0));
  poly41.push_back(Point3d(-5034, 2956, 0));
  poly41.push_back(Point3d(-5508, 4045, 0));
  poly41.push_back(Point3d(-489, 6843, 0));
  poly41.push_back(Point3d(-635, 4874, 0));

  std::vector<Point3d> poly42;
  poly42.push_back(Point3d(-8290, 7537, 0));
  poly42.push_back(Point3d(-14302, 7537, 0));
  poly42.push_back(Point3d(-14302, 7670, 0));
  poly42.push_back(Point3d(-8640, 7670, 0));
  poly42.push_back(Point3d(-8290, 7537, 0));

  std::vector<Point3d> poly43;
  poly43.push_back(Point3d(-7030, 7062, 0));
  poly43.push_back(Point3d(-8640, 7670, 0));
  poly43.push_back(Point3d(-7469, 7670, 0));
  poly43.push_back(Point3d(-7030, 7062, 0));

  std::vector<Point3d> poly44;
  poly44.push_back(Point3d(-6508, 6338, 0));
  poly44.push_back(Point3d(-7469, 7670, 0));
  poly44.push_back(Point3d(-7089, 7670, 0));
  poly44.push_back(Point3d(-6508, 6338, 0));

  std::vector<Point3d> poly45;
  poly45.push_back(Point3d(993, 7670, 0));
  poly45.push_back(Point3d(-5508, 4045, 0));
  poly45.push_back(Point3d(-7089, 7670, 0));
  poly45.push_back(Point3d(993, 7670, 0));

  std::vector<Point3d> poly46;
  poly46.push_back(Point3d(9242, 7670, 0));
  poly46.push_back(Point3d(-14302, 7670, 0));
  poly46.push_back(Point3d(-14302, 26424, 0));
  poly46.push_back(Point3d(9242, 7670, 0));

  std::vector<Point3d> poly47;
  poly47.push_back(Point3d(10768, 6453, 0));
  poly47.push_back(Point3d(-14302, 26424, 0));
  poly47.push_back(Point3d(-14302, 33315, 0));
  poly47.push_back(Point3d(8432, 12627, 0));
  poly47.push_back(Point3d(10768, 6453, 0));

  std::vector<Point3d> poly48;
  poly48.push_back(Point3d(14678, 4655, 0));
  poly48.push_back(Point3d(11449, 4655, 0));
  poly48.push_back(Point3d(8432, 12627, 0));
  poly48.push_back(Point3d(12857, 8600, 0));
  poly48.push_back(Point3d(14678, 4655, 0));

  std::vector<Point3d> poly49;
  poly49.push_back(Point3d(-25053, 22835, 0));
  poly49.push_back(Point3d(-25316, 24095, 0));
  poly49.push_back(Point3d(-22146, 32299, 0));
  poly49.push_back(Point3d(-21283, 31965, 0));
  poly49.push_back(Point3d(-25053, 22835, 0));

  std::vector<Point3d> poly50;
  poly50.push_back(Point3d(-14302, 34056, 0));
  poly50.push_back(Point3d(-16373, 32765, 0));
  poly50.push_back(Point3d(-16228, 37295, 0));
  poly50.push_back(Point3d(-14302, 37295, 0));
  poly50.push_back(Point3d(-14302, 34056, 0));

  std::vector<Point3d> poly51;
  poly51.push_back(Point3d(-14302, 31941, 0));
  poly51.push_back(Point3d(-17694, 31941, 0));
  poly51.push_back(Point3d(-14302, 34056, 0));
  poly51.push_back(Point3d(-14302, 31941, 0));

  std::vector<Point3d> poly52;
  poly52.push_back(Point3d(-14302, 29267, 0));
  poly52.push_back(Point3d(-21221, 31941, 0));
  poly52.push_back(Point3d(-14302, 31941, 0));
  poly52.push_back(Point3d(-14302, 29267, 0));

  std::vector<Point3d> poly53;
  poly53.push_back(Point3d(-14302, 24258, 0));
  poly53.push_back(Point3d(-27224, 17579, 0));
  poly53.push_back(Point3d(-21283, 31965, 0));
  poly53.push_back(Point3d(-14302, 29267, 0));
  poly53.push_back(Point3d(-14302, 24258, 0));

  std::vector<Point3d> poly54;
  poly54.push_back(Point3d(-14302, 16930, 0));
  poly54.push_back(Point3d(-28480, 16930, 0));
  poly54.push_back(Point3d(-14302, 24258, 0));
  poly54.push_back(Point3d(-14302, 16930, 0));

  std::vector<Point3d> poly55;
  poly55.push_back(Point3d(-32955, 11085, 0));
  poly55.push_back(Point3d(-34626, 10650, 0));
  poly55.push_back(Point3d(-35780, 12361, 0));
  poly55.push_back(Point3d(-35780, 12538, 0));
  poly55.push_back(Point3d(-32955, 11085, 0));

  std::vector<Point3d> poly56;
  poly56.push_back(Point3d(-31609, 10392, 0));
  poly56.push_back(Point3d(-35780, 12538, 0));
  poly56.push_back(Point3d(-35780, 15168, 0));
  poly56.push_back(Point3d(-31609, 10392, 0));

  std::vector<Point3d> poly57;
  poly57.push_back(Point3d(-20303, 5921, 0));
  poly57.push_back(Point3d(-23641, 5921, 0));
  poly57.push_back(Point3d(-19457, 7256, 0));
  poly57.push_back(Point3d(-20303, 5921, 0));

  std::vector<Point3d> poly58;
  poly58.push_back(Point3d(-18577, 7537, 0));
  poly58.push_back(Point3d(-23641, 5921, 0));
  poly58.push_back(Point3d(-27704, 5921, 0));
  poly58.push_back(Point3d(-29116, 7537, 0));
  poly58.push_back(Point3d(-18577, 7537, 0));

  std::vector<Point3d> poly59;
  poly59.push_back(Point3d(-14302, 7537, 0));
  poly59.push_back(Point3d(-29116, 7537, 0));
  poly59.push_back(Point3d(-35780, 15168, 0));
  poly59.push_back(Point3d(-35780, 16930, 0));
  poly59.push_back(Point3d(-14302, 16930, 0));
  poly59.push_back(Point3d(-14302, 7537, 0));

  std::vector<Point3d> poly60;
  poly60.push_back(Point3d(20568, -3237, 0));
  poly60.push_back(Point3d(15885, 4655, 0));
  poly60.push_back(Point3d(16436, 4655, 0));
  poly60.push_back(Point3d(20568, -3237, 0));

  std::vector<Point3d> poly61;
  poly61.push_back(Point3d(16251, 26780, 0));
  poly61.push_back(Point3d(15885, 26780, 0));
  poly61.push_back(Point3d(15885, 34180, 0));
  poly61.push_back(Point3d(16251, 34180, 0));
  poly61.push_back(Point3d(16251, 26780, 0));

  std::vector<Point3d> poly62;
  poly62.push_back(Point3d(16251, 23752, 0));
  poly62.push_back(Point3d(11822, 23752, 0));
  poly62.push_back(Point3d(11822, 26780, 0));
  poly62.push_back(Point3d(16251, 26780, 0));
  poly62.push_back(Point3d(16251, 23752, 0));

  std::vector<Point3d> poly63;
  poly63.push_back(Point3d(21882, 6843, 0));
  poly63.push_back(Point3d(16251, 6843, 0));
  poly63.push_back(Point3d(16251, 34180, 0));
  poly63.push_back(Point3d(21882, 34180, 0));
  poly63.push_back(Point3d(21882, 6843, 0));

  std::vector<Point3d> poly64;
  poly64.push_back(Point3d(54136, 30997, 0));
  poly64.push_back(Point3d(36405, 30997, 0));
  poly64.push_back(Point3d(36405, 44997, 0));
  poly64.push_back(Point3d(48522, 44532, 0));
  poly64.push_back(Point3d(51939, 41757, 0));
  poly64.push_back(Point3d(54136, 30997, 0));

  std::vector<Point3d> poly65;
  poly65.push_back(Point3d(59516, 4655, 0));
  poly65.push_back(Point3d(33396, 4655, 0));
  poly65.push_back(Point3d(33396, 30997, 0));
  poly65.push_back(Point3d(54136, 30997, 0));
  poly65.push_back(Point3d(59516, 4655, 0));

  std::vector<Point3d> poly66;
  poly66.push_back(Point3d(33396, 4655, 0));
  poly66.push_back(Point3d(21882, 4655, 0));
  poly66.push_back(Point3d(21882, 34180, 0));
  poly66.push_back(Point3d(33396, 34180, 0));
  poly66.push_back(Point3d(33396, 4655, 0));

  std::vector<Point3d> poly67;
  poly67.push_back(Point3d(59320, -4681, 0));
  poly67.push_back(Point3d(21324, -4681, 0));
  poly67.push_back(Point3d(16436, 4655, 0));
  poly67.push_back(Point3d(59516, 4655, 0));
  poly67.push_back(Point3d(60982, -2522, 0));
  poly67.push_back(Point3d(59320, -4681, 0));

  std::vector<Point3d> poly68;
  poly68.push_back(Point3d(58688, -5503, 0));
  poly68.push_back(Point3d(17848, -5503, 0));
  poly68.push_back(Point3d(17848, -4681, 0));
  poly68.push_back(Point3d(59320, -4681, 0));
  poly68.push_back(Point3d(58688, -5503, 0));

  std::vector<Point3d> poly69;
  poly69.push_back(Point3d(-40615, 18792, 0));
  poly69.push_back(Point3d(-42998, 17156, 0));
  poly69.push_back(Point3d(-44219, 17156, 0));
  poly69.push_back(Point3d(-44084, 20247, 0));
  poly69.push_back(Point3d(-40615, 20247, 0));
  poly69.push_back(Point3d(-40615, 18792, 0));

  std::vector<Point3d> poly70;
  poly70.push_back(Point3d(-35780, 17156, 0));
  poly70.push_back(Point3d(-36869, 17156, 0));
  poly70.push_back(Point3d(-36869, 20247, 0));
  poly70.push_back(Point3d(-35780, 20247, 0));
  poly70.push_back(Point3d(-35780, 17156, 0));

  std::vector<Point3d> poly71;
  poly71.push_back(Point3d(16251, 6843, 0));
  poly71.push_back(Point3d(14896, 6843, 0));
  poly71.push_back(Point3d(13287, 7670, 0));
  poly71.push_back(Point3d(6889, 21534, 0));
  poly71.push_back(Point3d(7393, 22835, 0));
  poly71.push_back(Point3d(8785, 23752, 0));
  poly71.push_back(Point3d(16251, 23752, 0));
  poly71.push_back(Point3d(16251, 6843, 0));

  std::vector<Point3d> poly72;
  poly72.push_back(Point3d(2059, 32554, 0));
  poly72.push_back(Point3d(687, 32120, 0));
  poly72.push_back(Point3d(-12988, 32120, 0));
  poly72.push_back(Point3d(-14302, 33315, 0));
  poly72.push_back(Point3d(-14302, 34180, 0));
  poly72.push_back(Point3d(-150, 34180, 0));
  poly72.push_back(Point3d(2059, 32554, 0));

  std::vector<Point3d> poly73;
  poly73.push_back(Point3d(15885, 26780, 0));
  poly73.push_back(Point3d(9904, 26780, 0));
  poly73.push_back(Point3d(-150, 34180, 0));
  poly73.push_back(Point3d(15885, 34180, 0));
  poly73.push_back(Point3d(15885, 26780, 0));

  std::vector<Point3d> poly74;
  poly74.push_back(Point3d(-2764, 45282, 0));
  poly74.push_back(Point3d(-3956, 45336, 0));
  poly74.push_back(Point3d(-3901, 46545, 0));
  poly74.push_back(Point3d(-2764, 46501, 0));
  poly74.push_back(Point3d(-2764, 45282, 0));

  std::vector<Point3d> poly75;
  poly75.push_back(Point3d(33396, 39004, 0));
  poly75.push_back(Point3d(-2764, 39004, 0));
  poly75.push_back(Point3d(-2764, 46501, 0));
  poly75.push_back(Point3d(33396, 45113, 0));
  poly75.push_back(Point3d(33396, 39004, 0));

  std::vector<Point3d> poly76;
  poly76.push_back(Point3d(33396, 37295, 0));
  poly76.push_back(Point3d(-11297, 37295, 0));
  poly76.push_back(Point3d(-11297, 39004, 0));
  poly76.push_back(Point3d(33396, 39004, 0));
  poly76.push_back(Point3d(33396, 37295, 0));

  std::vector<Point3d> poly77;
  poly77.push_back(Point3d(33396, 34180, 0));
  poly77.push_back(Point3d(-14302, 34180, 0));
  poly77.push_back(Point3d(-14302, 37295, 0));
  poly77.push_back(Point3d(33396, 37295, 0));
  poly77.push_back(Point3d(33396, 34180, 0));

  std::vector<Point3d> poly78;
  poly78.push_back(Point3d(36405, 30997, 0));
  poly78.push_back(Point3d(33396, 30997, 0));
  poly78.push_back(Point3d(33396, 45113, 0));
  poly78.push_back(Point3d(36405, 44997, 0));
  poly78.push_back(Point3d(36405, 30997, 0));

  std::vector<Point3d> poly79;
  poly79.push_back(Point3d(-23109, 45888, 0));
  poly79.push_back(Point3d(-30974, 46061, 0));
  poly79.push_back(Point3d(-30994, 46562, 0));
  poly79.push_back(Point3d(-23109, 46204, 0));
  poly79.push_back(Point3d(-23109, 45888, 0));

  std::vector<Point3d> poly80;
  poly80.push_back(Point3d(-20590, 41551, 0));
  poly80.push_back(Point3d(-23109, 41551, 0));
  poly80.push_back(Point3d(-23109, 46204, 0));
  poly80.push_back(Point3d(-20364, 46080, 0));
  poly80.push_back(Point3d(-20590, 41551, 0));

  std::vector<Point3d> poly81;
  poly81.push_back(Point3d(-3956, 45336, 0));
  poly81.push_back(Point3d(-30994, 46562, 0));
  poly81.push_back(Point3d(-31033, 47587, 0));
  poly81.push_back(Point3d(-3901, 46545, 0));
  poly81.push_back(Point3d(-3956, 45336, 0));

  std::vector<Point3d> poly82;
  poly82.push_back(Point3d(20568, -3237, 0));
  poly82.push_back(Point3d(14436, -3237, 0));
  poly82.push_back(Point3d(11449, 4655, 0));
  poly82.push_back(Point3d(15885, 4655, 0));
  poly82.push_back(Point3d(20568, -3237, 0));

  std::vector<Point3d> poly83;
  poly83.push_back(Point3d(14436, -3237, 0));
  poly83.push_back(Point3d(3075, -3237, 0));
  poly83.push_back(Point3d(-610, 5215, 0));
  poly83.push_back(Point3d(-489, 6843, 0));
  poly83.push_back(Point3d(993, 7670, 0));
  poly83.push_back(Point3d(9242, 7670, 0));
  poly83.push_back(Point3d(10768, 6453, 0));
  poly83.push_back(Point3d(14436, -3237, 0));

  std::vector<Point3d> poly84;
  poly84.push_back(Point3d(-29277, 1704, 0));
  poly84.push_back(Point3d(-31968, 2681, 0));
  poly84.push_back(Point3d(-34844, 5214, 0));
  poly84.push_back(Point3d(-36576, 9001, 0));
  poly84.push_back(Point3d(-36614, 9310, 0));
  poly84.push_back(Point3d(-34626, 10650, 0));
  poly84.push_back(Point3d(-32955, 11085, 0));
  poly84.push_back(Point3d(-31609, 10392, 0));
  poly84.push_back(Point3d(-27704, 5921, 0));
  poly84.push_back(Point3d(-29277, 1704, 0));

  std::vector<Point3d> poly85;
  poly85.push_back(Point3d(-4173, -3237, 0));
  poly85.push_back(Point3d(-19239, -3237, 0));
  poly85.push_back(Point3d(-5182, 2891, 0));
  poly85.push_back(Point3d(-4173, 576, 0));
  poly85.push_back(Point3d(-4173, -3237, 0));

  std::vector<Point3d> poly86;
  poly86.push_back(Point3d(-5034, 2956, 0));
  poly86.push_back(Point3d(-19239, -3237, 0));
  poly86.push_back(Point3d(-20041, -3237, 0));
  poly86.push_back(Point3d(-20303, -2535, 0));
  poly86.push_back(Point3d(-20303, 5921, 0));
  poly86.push_back(Point3d(-19457, 7256, 0));
  poly86.push_back(Point3d(-18577, 7537, 0));
  poly86.push_back(Point3d(-8290, 7537, 0));
  poly86.push_back(Point3d(-7030, 7062, 0));
  poly86.push_back(Point3d(-6508, 6338, 0));
  poly86.push_back(Point3d(-5034, 2956, 0));

  std::vector<Point3d> poly87;
  poly87.push_back(Point3d(-46721, 41746, 0));
  poly87.push_back(Point3d(-48139, 41675, 0));
  poly87.push_back(Point3d(-48471, 48283, 0));
  poly87.push_back(Point3d(-46839, 48220, 0));
  poly87.push_back(Point3d(-46721, 41746, 0));

  std::vector<Point3d> poly88;
  poly88.push_back(Point3d(-44738, 41123, 0));
  poly88.push_back(Point3d(-46675, 39229, 0));
  poly88.push_back(Point3d(-46839, 48220, 0));
  poly88.push_back(Point3d(-44731, 48139, 0));
  poly88.push_back(Point3d(-44738, 41123, 0));

  std::vector<Point3d> poly89;
  poly89.push_back(Point3d(-36576, 9001, 0));
  poly89.push_back(Point3d(-40525, 6338, 0));
  poly89.push_back(Point3d(-40680, 6569, 0));
  poly89.push_back(Point3d(-36614, 9310, 0));
  poly89.push_back(Point3d(-36576, 9001, 0));

  std::vector<Point3d> poly90;
  poly90.push_back(Point3d(-30704, -2118, 0));
  poly90.push_back(Point3d(-35947, -2118, 0));
  poly90.push_back(Point3d(-38236, 2109, 0));
  poly90.push_back(Point3d(-30393, 2109, 0));
  poly90.push_back(Point3d(-29277, 1704, 0));
  poly90.push_back(Point3d(-30704, -2118, 0));

  std::vector<Point3d> poly91;
  poly91.push_back(Point3d(-30393, 2109, 0));
  poly91.push_back(Point3d(-31318, 2109, 0));
  poly91.push_back(Point3d(-31968, 2681, 0));
  poly91.push_back(Point3d(-30393, 2109, 0));

  std::vector<Point3d> poly92;
  poly92.push_back(Point3d(-31318, 2109, 0));
  poly92.push_back(Point3d(-33423, 2109, 0));
  poly92.push_back(Point3d(-34844, 5214, 0));
  poly92.push_back(Point3d(-31318, 2109, 0));

  std::vector<Point3d> poly93;
  poly93.push_back(Point3d(-33423, 2109, 0));
  poly93.push_back(Point3d(-38236, 2109, 0));
  poly93.push_back(Point3d(-40525, 6338, 0));
  poly93.push_back(Point3d(-36576, 9001, 0));
  poly93.push_back(Point3d(-33423, 2109, 0));

  std::vector<Point3d> poly94;
  poly94.push_back(Point3d(-35947, -2118, 0));
  poly94.push_back(Point3d(-45292, -2118, 0));
  poly94.push_back(Point3d(-45516, 2972, 0));
  poly94.push_back(Point3d(-40525, 6337, 0));
  poly94.push_back(Point3d(-35947, -2118, 0));

  std::vector<Point3d> poly95;
  poly95.push_back(Point3d(-46488, 21176, 0));
  poly95.push_back(Point3d(-46954, 21139, 0));
  poly95.push_back(Point3d(-47174, 25939, 0));
  poly95.push_back(Point3d(-46862, 25939, 0));
  poly95.push_back(Point3d(-46488, 21176, 0));

  polygons.push_back(poly1);
  polygons.push_back(poly2);
  polygons.push_back(poly3);
  polygons.push_back(poly4);
  polygons.push_back(poly5);
  polygons.push_back(poly6);
  polygons.push_back(poly7);
  polygons.push_back(poly8);
  polygons.push_back(poly9);
  polygons.push_back(poly10);
  polygons.push_back(poly11);
  polygons.push_back(poly12);
  polygons.push_back(poly13);
  polygons.push_back(poly14);
  polygons.push_back(poly15);
  polygons.push_back(poly16);
  polygons.push_back(poly17);
  polygons.push_back(poly18);
  polygons.push_back(poly19);
  polygons.push_back(poly20);
  polygons.push_back(poly21);
  polygons.push_back(poly22);
  polygons.push_back(poly23);
  polygons.push_back(poly24);
  polygons.push_back(poly25);
  polygons.push_back(poly26);
  polygons.push_back(poly27);
  polygons.push_back(poly28);
  polygons.push_back(poly29);
  polygons.push_back(poly30);
  polygons.push_back(poly31);
  polygons.push_back(poly32);
  polygons.push_back(poly33);
  polygons.push_back(poly34);
  polygons.push_back(poly35);
  polygons.push_back(poly36);
  polygons.push_back(poly37);
  polygons.push_back(poly38);
  polygons.push_back(poly39);
  polygons.push_back(poly40);
  polygons.push_back(poly41);
  polygons.push_back(poly42);
  polygons.push_back(poly43);
  polygons.push_back(poly44);
  polygons.push_back(poly45);
  polygons.push_back(poly46);
  polygons.push_back(poly47);
  polygons.push_back(poly48);
  polygons.push_back(poly49);
  polygons.push_back(poly50);
  polygons.push_back(poly51);
  polygons.push_back(poly52);
  polygons.push_back(poly53);
  polygons.push_back(poly54);
  polygons.push_back(poly55);
  polygons.push_back(poly56);
  polygons.push_back(poly57);
  polygons.push_back(poly58);
  polygons.push_back(poly59);
  polygons.push_back(poly60);
  polygons.push_back(poly61);
  polygons.push_back(poly62);
  polygons.push_back(poly63);
  polygons.push_back(poly64);
  polygons.push_back(poly65);
  polygons.push_back(poly66);
  polygons.push_back(poly67);
  polygons.push_back(poly68);
  polygons.push_back(poly69);
  polygons.push_back(poly70);
  polygons.push_back(poly71);
  polygons.push_back(poly72);
  polygons.push_back(poly73);
  polygons.push_back(poly74);
  polygons.push_back(poly75);
  polygons.push_back(poly76);
  polygons.push_back(poly77);
  polygons.push_back(poly78);
  polygons.push_back(poly79);
  polygons.push_back(poly80);
  polygons.push_back(poly81);
  polygons.push_back(poly82);
  polygons.push_back(poly83);
  polygons.push_back(poly84);
  polygons.push_back(poly85);
  polygons.push_back(poly86);
  polygons.push_back(poly87);
  polygons.push_back(poly88);
  polygons.push_back(poly89);
  polygons.push_back(poly90);
  polygons.push_back(poly91);
  polygons.push_back(poly92);
  polygons.push_back(poly93);
  polygons.push_back(poly94);
  polygons.push_back(poly95);

  /* ASSERT_EQ(getArea(poly1), getArea(poly2));
  ASSERT_EQ(getArea(poly2), getArea(poly3));
  ASSERT_EQ(getArea(poly3), getArea(poly4));
  ASSERT_EQ(getArea(poly4), getArea(poly1));*/

  // Offsets the polygon slightly
  test = joinAllWithBuffer(polygons, tol * 2, tol);
  double a = getArea(test[0]).value();
  double p = getPerimeter(test[0]);

  // Original method
  test = joinAll(polygons, tol);
  a = getArea(test[0]).value();
  // Completely different method
  test = buffer(polygons, tol * 2, tol);
  a = getArea(test[0]).value();
  p = getPerimeter(test[0]);

  // Should return one polygon
  ASSERT_EQ(1u, test.size());

  ofstream output("C:/Users/ggart/OneDrive/Desktop/joinAll_points4.csv");

  for (auto point : test[0]) {
    output << point.x() << "," << point.y() << "," << point.z() << endl;
  }
  output << " ";
  output.close();
  
  // That polygon should have 75 points
  // We know this fails because we know joinAll gives up when it ends up with a polygon with a hole
  ASSERT_EQ(45, test[0].size());

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
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i){
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
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i){
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
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i){
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
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i){
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
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i){
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
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i){
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
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i){
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


