/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "GeometryFixture.hpp"

#include "../Intersection.hpp"

#include "../Geometry.hpp"
#include "../PointLatLon.hpp"
#include "../Vector3d.hpp"

#include "../../core/StringStreamLogSink.hpp"
#include "../../core/LogMessage.hpp"

#undef BOOST_UBLAS_TYPE_CHECK
#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4244)
#endif
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

using BoostPoint = boost::geometry::model::d2::point_xy<double>;
using BoostPolygon = boost::geometry::model::polygon<BoostPoint>;
using BoostRing = boost::geometry::model::ring<BoostPoint>;
using BoostMultiPolygon = boost::geometry::model::multi_polygon<BoostPolygon>;

using namespace std;
using namespace boost;
using namespace openstudio;

std::string printPolygon(const BoostPolygon& polygon) {
  std::stringstream ss;

  ss << boost::geometry::area(polygon);

  BoostRing outer = polygon.outer();
  if (outer.empty()) {
    return ss.str();
  }

  // add point for each vertex except final vertex
  ss << " [";
  for (unsigned i = 0; i < outer.size(); ++i) {
    ss << outer[i].x() << " " << outer[i].y();
    if (i < outer.size() - 1) {
      ss << ", ";
    }
  }

  for (const BoostRing& inner : polygon.inners()) {
    if (!inner.empty()) {
      ss << "], [";
      // inner loop already in reverse order
      for (unsigned i = 0; i < inner.size(); ++i) {
        ss << inner[i].x() << " " << inner[i].y();
        if (i < inner.size() - 1) {
          ss << ", ";
        }
      }
    }
  }
  ss << "]";

  return ss.str();
}

std::vector<Point3d> makeRectangleUp(double xmin, double ymin, double width, double height) {
  std::vector<Point3d> result{
    {xmin, ymin, 0},
    {xmin + width, ymin, 0},
    {xmin + width, ymin + height, 0},
    {xmin, ymin + height, 0},
  };
  return result;
}

std::vector<Point3d> makeRectangleDown(double xmin, double ymin, double width, double height) {
  std::vector<Point3d> result{
    {xmin + width, ymin + height, 0},
    {xmin + width, ymin, 0},
    {xmin, ymin, 0},
    {xmin, ymin + height, 0},
  };
  return result;
}

TEST_F(GeometryFixture, BoostGeometry_Polygon1) {
  BoostPolygon blue;
  BoostPolygon yellow;

  // blue
  boost::geometry::read_wkt("POLYGON((-11.379200508 -12.0396003048 , -11.379200508 12.0395996952 , 3.555999492 12.0395996952 , 3.555999492 "
                            "-12.0396003048 , -11.379200508 -12.0396003048))",
                            blue);
  //Surface Intersection Model 74
  //Surface 37,                             !- Name
  //-11.379200508, -12.0396003048, 3.3528,  !- X,Y,Z Vertex 1 {m}
  //-11.379200508, 12.0395996952, 3.3528,   !- X,Y,Z Vertex 2 {m}
  //3.555999492, 12.0395996952, 3.3528,     !- X,Y,Z Vertex 3 {m}
  //3.555999492, -12.0396003048, 3.3528;    !- X,Y,Z Vertex 4 {m}

  // yellow
  boost::geometry::read_wkt("POLYGON((3.2512004064 -7.4676003048 , 3.2512004064 7.4675996952 , 3.555999492 7.7723987808 , 3.555999492 -7.7723993904 "
                            ", 3.2512004064 -7.4676003048))",
                            yellow);
  //Surface Intersection Model 74
  //Surface 44,                             !- Name
  //3.555999492, -7.7723993904, 3.3528,     !- X,Y,Z Vertex 1 {m}
  //3.555999492, 7.7723987808, 3.3528,      !- X,Y,Z Vertex 2 {m}
  //3.2512004064, 7.4675996952, 3.3528,     !- X,Y,Z Vertex 3 {m}
  //3.2512004064, -7.4676003048, 3.3528;    !- X,Y,Z Vertex 4 {m}

  std::vector<BoostPolygon> output;
  boost::geometry::difference(yellow, blue, output);

  int i = 0;
  std::cout << "yellow - blue:" << '\n';
  for (BoostPolygon const& p : output) {
    std::cout << i++ << ": " << printPolygon(p) << '\n';
  }

  output.clear();
  boost::geometry::difference(blue, yellow, output);

  i = 0;
  std::cout << "blue - yellow:" << '\n';
  for (BoostPolygon const& p : output) {
    std::cout << i++ << ": " << printPolygon(p) << '\n';
  }
}

TEST_F(GeometryFixture, PointInPolygon_Down) {
  double tol = 0.01;
  double tol2 = tol / 2.0;

  Point3dVector points = makeRectangleDown(0, 0, 1, 1);

  // center
  EXPECT_TRUE(pointInPolygon(Point3d(0.5, 0.5, 0), points, tol));

  // corners
  EXPECT_TRUE(pointInPolygon(Point3d(0, 0, 0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(1, 0, 0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(1, 1, 0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(0, 1, 0), points, tol));

  // edges
  EXPECT_TRUE(pointInPolygon(Point3d(0.5, 0, 0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(1, 0.5, 0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(0.5, 1, 0), points, tol));
  EXPECT_TRUE(pointInPolygon(Point3d(0, 0.5, 0), points, tol));

  EXPECT_TRUE(within(Point3d(0.5, tol2, 0), points, tol));
  EXPECT_TRUE(within(Point3d(1 - tol2, 0.5, 0), points, tol));
  EXPECT_TRUE(within(Point3d(0.5, 1 - tol2, 0), points, tol));
  EXPECT_TRUE(within(Point3d(tol2, 0.5, 0), points, tol));

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

TEST_F(GeometryFixture, PointInPolygon_Up) {
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

TEST_F(GeometryFixture, Within_Down) {
  double tol = 0.01;
  double tol2 = tol / 2.0;

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

TEST_F(GeometryFixture, Within_Up) {
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

TEST_F(GeometryFixture, Intersect_False) {
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
  points1 = {
    {1, 1, 3},
    {1, 0, 3},
    {0, 0, 3},
    {0, 1, 3},
  };

  points2 = {
    {1, 1, 3},
    {1, 0, 3},
    {0, 0, 3},
    {0, 1, 3},
  };

  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);

  // sense is up, same points but not on z=0 plane
  points1 = {
    {0, 0, 3},
    {1, 0, 3},
    {1, 1, 3},
    {0, 1, 3},
  };

  points2 = {
    {0, 0, 3},
    {1, 0, 3},
    {1, 1, 3},
    {0, 1, 3},
  };

  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);

  // sense is right, same points but not on z=0 plane
  points1 = {
    {0, 0, 0},
    {0, 0, 1},
    {0, 1, 1},
    {0, 1, 0},
  };

  points2 = {
    {0, 0, 0},
    {0, 0, 1},
    {0, 1, 1},
    {0, 1, 0},
  };

  test = intersect(points1, points2, tol);
  EXPECT_FALSE(test);

  test = intersect(points2, points1, tol);
  EXPECT_FALSE(test);
}

TEST_F(GeometryFixture, Intersect_SamePoints) {
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

TEST_F(GeometryFixture, Intersect_Adjacent) {
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

TEST_F(GeometryFixture, Intersect_Overlap_Down) {
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

TEST_F(GeometryFixture, Intersect_Overlap_Up) {
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

TEST_F(GeometryFixture, Intersect_Within_Down) {
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

TEST_F(GeometryFixture, Intersect_Within_Up) {
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

TEST_F(GeometryFixture, Join_False) {
  double tol = 0.01;

  boost::optional<std::vector<Point3d>> test;
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
  points1 = {
    {1, 1, 3},
    {1, 0, 3},
    {0, 0, 3},
    {0, 1, 3},
  };

  points2 = {
    {1, 1, 3},
    {1, 0, 3},
    {0, 0, 3},
    {0, 1, 3},
  };

  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);

  // sense is up, same points but not on z=0 plane
  points1 = {
    {0, 0, 3},
    {1, 0, 3},
    {1, 1, 3},
    {0, 1, 3},
  };

  points2 = {
    {0, 0, 3},
    {1, 0, 3},
    {1, 1, 3},
    {0, 1, 3},
  };

  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);

  // sense is right, same points but not on z=0 plane
  points1 = {
    {0, 0, 0},
    {0, 0, 1},
    {0, 1, 1},
    {0, 1, 0},
  };

  points2 = {
    {0, 0, 0},
    {0, 0, 1},
    {0, 1, 1},
    {0, 1, 0},
  };

  test = join(points1, points2, tol);
  EXPECT_FALSE(test);

  test = join(points2, points1, tol);
  EXPECT_FALSE(test);
}

TEST_F(GeometryFixture, Join_SamePoints) {
  double tol = 0.01;

  boost::optional<std::vector<Point3d>> test;
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

TEST_F(GeometryFixture, Join_Adjacent) {
  double tol = 0.01;

  boost::optional<std::vector<Point3d>> test;
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

TEST_F(GeometryFixture, Join_Overlap_Down) {
  double tol = 0.01;

  boost::optional<std::vector<Point3d>> test;
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

TEST_F(GeometryFixture, Join_Overlap_Up) {
  double tol = 0.01;

  boost::optional<std::vector<Point3d>> test;
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

TEST_F(GeometryFixture, Join_Overlap2) {
  double tol = 0.01;

  boost::optional<std::vector<Point3d>> test;
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

TEST_F(GeometryFixture, JoinAll) {
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
  for (unsigned i = 0; i < 2; ++i) {
    if (circularEqual(makeRectangleDown(0, 0, 2, 1), test[i])) {
      found1 = true;
    } else if (circularEqual(makeRectangleDown(3, 0, 1, 1), test[i])) {
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

TEST_F(GeometryFixture, RemoveSpikes_Down) {
  double tol = 0.01;

  std::vector<Point3d> points;
  std::vector<Point3d> expected;
  std::vector<Point3d> result;

  // spike at beginning
  {
    // sense is down
    points = {
      {10, 10, 0}, {10, 0, 0}, {0, 0, 0}, {0, 5, 0}, {10, 5, 0},
    };

    expected = {
      {10, 5, 0},
      {10, 0, 0},
      {0, 0, 0},
      {0, 5, 0},
    };

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }

  // spike at beginning 2
  {
    // sense is down
    points = {
      {10, 5, 0}, {10, 10, 0}, {10, 0, 0}, {0, 0, 0}, {0, 5, 0},
    };

    expected = {
      {10, 5, 0},
      {10, 0, 0},
      {0, 0, 0},
      {0, 5, 0},
    };

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }

  // spike in middle
  {
    // sense is down
    points = {
      {10, 5, 0}, {10, 0, 0}, {-5, 0, 0}, {0, 0, 0}, {0, 5, 0},
    };

    expected = {
      {10, 5, 0},
      {10, 0, 0},
      {0, 0, 0},
      {0, 5, 0},
    };

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }
}

TEST_F(GeometryFixture, RemoveSpikes_Up) {
  double tol = 0.01;

  std::vector<Point3d> points;
  std::vector<Point3d> expected;
  std::vector<Point3d> result;

  // spike at end
  {
    // sense is up
    points = {
      {10, 5, 0}, {0, 5, 0}, {0, 0, 0}, {10, 0, 0}, {10, 10, 0},
    };

    expected.clear();

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }

  // spike at end 2
  {
    // sense is up
    points = {
      {0, 5, 0}, {0, 0, 0}, {10, 0, 0}, {10, 10, 0}, {10, 5, 0},
    };

    expected.clear();

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }

  // spike in middle
  {
    // sense is up
    points = {
      {0, 0, 0}, {10, 0, 0}, {-5, 0, 0}, {10, 5, 0}, {0, 5, 0},
    };

    expected.clear();

    result = removeSpikes(points, tol);
    EXPECT_TRUE(circularEqual(expected, result)) << result;
  }
}

TEST_F(GeometryFixture, Subtract_SamePoints) {
  double tol = 0.01;

  std::vector<std::vector<Point3d>> test;
  std::vector<std::vector<Point3d>> holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);
  points2 = makeRectangleDown(0, 0, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  for (const auto& polygon : test) {
    std::cout << polygon << '\n';
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
  for (const auto& polygon : test) {
    std::cout << polygon << '\n';
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

TEST_F(GeometryFixture, Subtract_Empty_Down) {
  double tol = 0.01;

  std::vector<std::vector<Point3d>> test;
  std::vector<std::vector<Point3d>> holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);

  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.size() == 1);
  for (const auto& polygon : test) {
    std::cout << polygon << '\n';
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

TEST_F(GeometryFixture, Subtract_Empty_Up) {
  double tol = 0.01;

  std::vector<std::vector<Point3d>> test;
  std::vector<std::vector<Point3d>> holes;
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

TEST_F(GeometryFixture, Subtract_Adjacent) {
  double tol = 0.01;

  std::vector<std::vector<Point3d>> test;
  std::vector<std::vector<Point3d>> holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 1, 1);
  points2 = makeRectangleDown(1, 0, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.size() == 1);
  for (const auto& polygon : test) {
    std::cout << polygon << '\n';
  }
  EXPECT_DOUBLE_EQ(1.0, totalArea(test));

  holes.clear();
  holes.push_back(points1);
  test = subtract(points2, holes, tol);
  EXPECT_TRUE(test.size() == 1);
  for (const auto& polygon : test) {
    std::cout << polygon << '\n';
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

TEST_F(GeometryFixture, Subtract_Overlap) {
  double tol = 0.01;

  std::vector<std::vector<Point3d>> test;
  std::vector<std::vector<Point3d>> holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 2, 1);
  points2 = makeRectangleDown(1, 0, 2, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.size() == 1);
  for (const auto& polygon : test) {
    std::cout << polygon << '\n';
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
  for (const auto& polygon : test) {
    std::cout << polygon << '\n';
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

TEST_F(GeometryFixture, Subtract_Within_Down) {
  double tol = 0.01;

  std::vector<std::vector<Point3d>> test;
  std::vector<std::vector<Point3d>> holes;
  Point3dVector points1;
  Point3dVector points2;

  // sense is down
  points1 = makeRectangleDown(0, 0, 3, 3);
  points2 = makeRectangleDown(1, 1, 1, 1);

  holes.clear();
  holes.push_back(points2);
  test = subtract(points1, holes, tol);
  EXPECT_TRUE(test.size() > 1);
  for (const auto& polygon : test) {
    std::cout << polygon << '\n';
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
  for (const auto& polygon : test) {
    std::cout << polygon << '\n';
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

TEST_F(GeometryFixture, Subtract_Within_Up) {
  double tol = 0.01;

  std::vector<std::vector<Point3d>> test;
  std::vector<std::vector<Point3d>> holes;
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

TEST_F(GeometryFixture, selfIntersects) {
  double tol = 0.01;

  Point3dVector points;

  points = {
    {0, 1, 0},
    {2, 1, 0},
    {2, 0, 0},
    {0, 0, 0},
  };
  EXPECT_FALSE(selfIntersects(points, tol));

  points = {
    {0, 1, 0},
    {2, 1, 0},
    {0, 0, 0},
    {2, 0, 0},
  };
  EXPECT_TRUE(selfIntersects(points, tol));

  points = {
    {0, 1, 0}, {1, 0.2, 0}, {2, 1, 0}, {2, 0, 0}, {0, 0, 0},
  };
  EXPECT_FALSE(selfIntersects(points, tol));

  points = {
    {0, 1, 0}, {1, 0, 0}, {2, 1, 0}, {2, 0, 0}, {0, 0, 0},
  };
  EXPECT_FALSE(selfIntersects(points, tol));

  points = {
    {0, 1, 0}, {1, -1, 0}, {2, 1, 0}, {2, 0, 0}, {0, 0, 0},
  };
  EXPECT_TRUE(selfIntersects(points, tol));
}

TEST_F(GeometryFixture, simplify) {
  double tol = 0.01;

  Point3dVector points;

  points = {
    {158, 168, 0}, {200, 168, 0}, {200, 30, 0},  {158, 30, 0},  {158, 40, 0},  {158, 70, 0},  {158, 40, 0},  {158, 70, 0},  {158, 98, 0},
    {158, 70, 0},  {158, 40, 0},  {158, 70, 0},  {158, 98, 0},  {158, 70, 0},  {158, 98, 0},  {158, 128, 0}, {158, 98, 0},  {158, 70, 0},
    {158, 40, 0},  {158, 70, 0},  {158, 98, 0},  {158, 70, 0},  {158, 98, 0},  {158, 128, 0}, {158, 98, 0},  {158, 70, 0},  {158, 98, 0},
    {158, 128, 0}, {158, 98, 0},  {158, 128, 0}, {158, 138, 0}, {158, 128, 0}, {158, 98, 0},  {158, 70, 0},  {158, 40, 0},  {158, 70, 0},
    {158, 98, 0},  {158, 70, 0},  {158, 98, 0},  {158, 128, 0}, {158, 98, 0},  {158, 70, 0},  {158, 98, 0},  {158, 128, 0}, {158, 98, 0},
    {158, 128, 0}, {158, 138, 0}, {158, 128, 0}, {158, 98, 0},  {158, 70, 0},  {158, 98, 0},  {158, 128, 0}, {158, 98, 0},  {158, 128, 0},
    {158, 138, 0}, {158, 128, 0}, {158, 98, 0},  {158, 128, 0}, {158, 138, 0}, {158, 128, 0}, {158, 138, 0},
  };

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << '\n';

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol * tol);

  Point3dVector expectedPoints{
    {200, 168, 0}, {200, 30, 0}, {158, 30, 0}, {158, 40, 0}, {158, 70, 0}, {158, 98, 0}, {158, 128, 0}, {158, 138, 0}, {158, 168, 0},
  };

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol * tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i) {
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }
}

TEST_F(GeometryFixture, simplify2) {
  double tol = 0.01;

  Point3dVector points;

  points = {
    {158, 168, 0}, {200, 168, 0}, {200, 30, 0},  {158, 30, 0},  {158, 40, 0},  {158, 70, 0},  {158, 40, 0},  {158, 70, 0},  {158, 98, 0},
    {158, 70, 0},  {158, 40, 0},  {158, 70, 0},  {158, 98, 0},  {158, 70, 0},  {158, 98, 0},  {158, 128, 0}, {158, 98, 0},  {158, 70, 0},
    {158, 40, 0},  {158, 70, 0},  {158, 98, 0},  {158, 70, 0},  {158, 98, 0},  {158, 128, 0}, {158, 98, 0},  {158, 70, 0},  {158, 98, 0},
    {158, 128, 0}, {158, 98, 0},  {158, 128, 0}, {158, 138, 0}, {158, 128, 0}, {158, 98, 0},  {158, 70, 0},  {158, 40, 0},  {158, 70, 0},
    {158, 98, 0},  {158, 70, 0},  {158, 98, 0},  {158, 128, 0}, {158, 98, 0},  {158, 70, 0},  {158, 98, 0},  {158, 128, 0}, {158, 98, 0},
    {158, 128, 0}, {158, 138, 0}, {158, 128, 0}, {158, 98, 0},  {158, 70, 0},  {158, 98, 0},  {158, 128, 0}, {158, 98, 0},  {158, 128, 0},
    {158, 138, 0}, {158, 128, 0}, {158, 98, 0},  {158, 128, 0}, {158, 138, 0}, {158, 128, 0}, {158, 138, 0},
  };
  points = reverse(points);

  std::cout << points << '\n' << '\n';

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << '\n';

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol * tol);

  Point3dVector expectedPoints{
    {200, 30, 0}, {200, 168, 0}, {158, 168, 0}, {158, 138, 0}, {158, 128, 0}, {158, 98, 0}, {158, 70, 0}, {158, 40, 0}, {158, 30, 0},
  };

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol * tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i) {
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }
}

TEST_F(GeometryFixture, simplify3) {
  double tol = 0.01;

  Point3dVector points;

  points = {
    {158, 49, 0}, {158, 108, 0}, {158, 89, 0}, {158, 21, 0}, {157, 21, 0},  {144, 21, 0},  {143, 21, 0},  {114, 21, 0},  {113, 21, 0},  {113, 28, 0},
    {113, 50, 0}, {113, 70, 0},  {113, 91, 0}, {113, 98, 0}, {113, 119, 0}, {113, 120, 0}, {114, 120, 0}, {143, 120, 0}, {157, 120, 0}, {158, 120, 0},
  };

  std::cout << points << '\n' << '\n';

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << '\n';

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol * tol);

  Point3dVector expectedPoints{
    {158, 120, 0}, {158, 108, 0}, {158, 89, 0}, {158, 49, 0}, {158, 21, 0}, {157, 21, 0},  {144, 21, 0},  {143, 21, 0},  {114, 21, 0},  {113, 21, 0},
    {113, 28, 0},  {113, 50, 0},  {113, 70, 0}, {113, 91, 0}, {113, 98, 0}, {113, 119, 0}, {113, 120, 0}, {114, 120, 0}, {143, 120, 0}, {157, 120, 0},
  };

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol * tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i) {
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }
}

TEST_F(GeometryFixture, simplify4) {
  double tol = 0.01;

  Point3dVector points;
  points = {
    {158, 120, 0}, {158, 49, 0}, {158, 108, 0}, {158, 21, 0}, {113, 21, 0}, {113, 120, 0},
  };

  std::cout << points << '\n' << '\n';

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << '\n' << '\n';

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol * tol);

  Point3dVector expectedPoints{
    {158, 120, 0}, {158, 108, 0}, {158, 49, 0}, {158, 21, 0}, {113, 21, 0}, {113, 120, 0},
  };

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol * tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  std::cout << points2 << '\n';
  std::cout << expectedPoints << '\n';
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i) {
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }
}

TEST_F(GeometryFixture, simplify5) {
  double tol = 0.01;

  Point3dVector points;
  points = {
    {158, 98, 0}, {158, 138, 0}, {158, 40, 0}, {158, 70, 0}, {158, 128, 0}, {158, 168, 0}, {200, 168, 0}, {200, 30, 0}, {158, 30, 0},
  };

  std::cout << points << '\n' << '\n';

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << '\n' << '\n';

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol * tol);

  Point3dVector expectedPoints{
    {200, 168, 0}, {200, 30, 0}, {158, 30, 0}, {158, 40, 0}, {158, 70, 0}, {158, 98, 0}, {158, 128, 0}, {158, 138, 0}, {158, 168, 0},
  };

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol * tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i) {
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }

  // remove colinear
  Point3dVector points4 = simplify(points, true, tol);
  EXPECT_EQ(4u, points4.size());

  std::cout << points4 << '\n' << '\n';

  auto area4 = getArea(points4);
  ASSERT_TRUE(area4);

  EXPECT_NEAR(*area, *area4, tol * tol);
}

TEST_F(GeometryFixture, simplify6) {
  double tol = 0.01;

  Point3dVector points;
  points = {
    {3.6576, 3.048, 0},   {5.4864, 3.048, 0},   {5.4864, 5.1816, 0},  {5.4864, 11.5824, 0}, {5.4864, 5.1816, 0},
    {5.4864, 11.5824, 0}, {5.4864, 17.0688, 0}, {3.6576, 17.0688, 0}, {3.6576, 14.9352, 0}, {3.6576, 11.5824, 0},
    {3.6576, 7.0104, 0},  {3.6576, 11.5824, 0}, {3.6576, 7.0104, 0},
  };

  std::cout << points << '\n' << '\n';

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << '\n' << '\n';

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol * tol);

  Point3dVector expectedPoints{
    {5.4864, 3.048, 0},   {5.4864, 5.1816, 0},  {5.4864, 11.5824, 0}, {5.4864, 17.0688, 0}, {3.6576, 17.0688, 0},
    {3.6576, 14.9352, 0}, {3.6576, 11.5824, 0}, {3.6576, 7.0104, 0},  {3.6576, 3.048, 0},
  };

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol * tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i) {
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }

  // remove colinear
  Point3dVector points4 = simplify(points, true, tol);
  EXPECT_EQ(4u, points4.size());

  std::cout << points4 << '\n' << '\n';

  auto area4 = getArea(points4);
  ASSERT_TRUE(area4);

  EXPECT_NEAR(*area, *area4, tol * tol);
}

TEST_F(GeometryFixture, simplify7) {
  double tol = 0.01;

  Point3dVector points;
  points = {
    {3.6576, 3.048, 0},   {3.6576, 11.5824, 0}, {3.6576, 7.0104, 0},  {3.6576, 14.9352, 0}, {3.6576, 17.0688, 0},
    {5.4864, 17.0688, 0}, {5.4864, 5.1816, 0},  {5.4864, 11.5824, 0}, {5.4864, 3.048, 0},
  };

  std::cout << points << '\n' << '\n';

  auto area = getArea(points);
  ASSERT_TRUE(area);

  Point3dVector points2 = simplify(points, false, tol);

  std::cout << points2 << '\n' << '\n';

  auto area2 = getArea(points2);
  ASSERT_TRUE(area2);

  EXPECT_NEAR(*area, *area2, tol * tol);

  Point3dVector expectedPoints{
    {5.4864, 17.0688, 0}, {5.4864, 11.5824, 0}, {5.4864, 5.1816, 0},  {5.4864, 3.048, 0},   {3.6576, 3.048, 0},
    {3.6576, 7.0104, 0},  {3.6576, 11.5824, 0}, {3.6576, 14.9352, 0}, {3.6576, 17.0688, 0},
  };

  auto area3 = getArea(expectedPoints);
  ASSERT_TRUE(area3);

  EXPECT_NEAR(*area, *area3, tol * tol);

  ASSERT_EQ(points2.size(), expectedPoints.size());
  for (Point3dVector::size_type i = 0; i < expectedPoints.size(); ++i) {
    EXPECT_EQ(expectedPoints[i].x(), points2[i].x()) << i;
    EXPECT_EQ(expectedPoints[i].y(), points2[i].y()) << i;
    EXPECT_EQ(expectedPoints[i].z(), points2[i].z()) << i;
  }

  // remove colinear
  Point3dVector points4 = simplify(points, true, tol);
  EXPECT_EQ(4u, points4.size());

  std::cout << points4 << '\n' << '\n';

  auto area4 = getArea(points4);
  ASSERT_TRUE(area4);

  EXPECT_NEAR(*area, *area4, tol * tol);
}

/// Simple test of join using Polygons. Joins a U shape polygon
/// with a rectangle to create a rectangle with a hole.
///
/// Validates number of holes, hole dimension, outer perimeter dimension
/// gross and net area and preimeter
///
/// +----------------------------+
/// |       Polygon A            |
/// |                            |
/// |        +---------+         |
/// |        |         |         |
/// |        |         |         |
/// |        |         |         |
/// |--------+---------+---------|
/// |                            |
/// |       Polygon B            |
/// |                            |
/// |                            |
/// +----------------------------+
///
TEST_F(GeometryFixture, Polygon3d_Join) {

  Polygon3d polygonA;
  polygonA.addPoint(Point3d(0, 20, 0));
  polygonA.addPoint(Point3d(0, 40, 0));
  polygonA.addPoint(Point3d(30, 40, 0));
  polygonA.addPoint(Point3d(30, 20, 0));
  polygonA.addPoint(Point3d(20, 20, 0));
  polygonA.addPoint(Point3d(20, 30, 0));
  polygonA.addPoint(Point3d(10, 30, 0));
  polygonA.addPoint(Point3d(10, 20, 0));

  Polygon3d polygonB;
  polygonB.addPoint(Point3d(0, 0, 0));
  polygonB.addPoint(Point3d(0, 20, 0));
  polygonB.addPoint(Point3d(30, 20, 0));
  polygonB.addPoint(Point3d(30, 0, 0));

  boost::optional<Polygon3d> result = openstudio::join(polygonA, polygonB);

  // We should have a result
  ASSERT_TRUE(result != boost::none);
  Polygon3d res = result.get();
  // The outer should have 4 points
  ASSERT_TRUE(res.getOuterPath().size() == 4);
  // Check the points

  // There should be one hole
  ASSERT_TRUE(res.getInnerPaths().size() == 1);
  // Check the points
  Point3dVector hole = res.getInnerPaths().front();
  ASSERT_TRUE(hole.size() == 4);

  double perimeter = res.perimeter();
  EXPECT_NEAR(perimeter, 140, 0.001);

  double grossArea = res.grossArea();
  EXPECT_NEAR(grossArea, 1200, 0.001);

  double netArea = res.netArea();
  EXPECT_NEAR(netArea, 1100, 0.001);
}

Polygon3d GetTestPolygon() {
  Polygon3d testPolygon;
  testPolygon.addPoint(Point3d(10.0, 10.0, 0));
  testPolygon.addPoint(Point3d(0, 10.0, 0));
  testPolygon.addPoint(Point3d(0, 30.0, 0));
  testPolygon.addPoint(Point3d(10.0, 30.0, 0));
  testPolygon.addPoint(Point3d(10.0, 70.0, 0));
  testPolygon.addPoint(Point3d(0, 70.0, 0));
  testPolygon.addPoint(Point3d(0, 90, 0));
  testPolygon.addPoint(Point3d(10.0, 90, 0));
  testPolygon.addPoint(Point3d(10.0, 100.0, 0));
  testPolygon.addPoint(Point3d(30.0, 100.0, 0));
  testPolygon.addPoint(Point3d(30.0, 90, 0));
  testPolygon.addPoint(Point3d(70.0, 90, 0));
  testPolygon.addPoint(Point3d(70.0, 100.0, 0));
  testPolygon.addPoint(Point3d(90, 100.0, 0));
  testPolygon.addPoint(Point3d(90, 90, 0));
  testPolygon.addPoint(Point3d(100.0, 90, 0));
  testPolygon.addPoint(Point3d(100.0, 70.0, 0));
  testPolygon.addPoint(Point3d(90, 70.0, 0));
  testPolygon.addPoint(Point3d(90, 30.0, 0));
  testPolygon.addPoint(Point3d(100.0, 30.0, 0));
  testPolygon.addPoint(Point3d(100.0, 10.0, 0));
  testPolygon.addPoint(Point3d(90, 10.0, 0));
  testPolygon.addPoint(Point3d(90, 0, 0));
  testPolygon.addPoint(Point3d(70.0, 0, 0));
  testPolygon.addPoint(Point3d(70.0, 10.0, 0));
  testPolygon.addPoint(Point3d(30.0, 10.0, 0));
  testPolygon.addPoint(Point3d(30.0, 0, 0));
  testPolygon.addPoint(Point3d(10.0, 0, 0));

  Point3dVector testHole{
    {30.0, 30.0, 0},
    {70.0, 30.0, 0},
    {70.0, 70.0, 0},
    {30.0, 70.0, 0},
  };
  testPolygon.addHole(testHole);

  return testPolygon;
}

// joinAll fails on cases with an inner loop
/// joinAll method that takes a list of polygons and returns a list of polygons
#ifdef _WIN32  // Linux, Mac OS need additional review
TEST_F(GeometryFixture, Polygon3d_JoinAll_1614) {
  double tol = 0.01;

  std::vector<Polygon3d> polygons;

  Polygon3d poly1;
  poly1.addPoint(Point3d(0, 70.0, 0));
  poly1.addPoint(Point3d(0, 90.0, 0));
  poly1.addPoint(Point3d(100.0, 90.0, 0));
  poly1.addPoint(Point3d(100.0, 70., 0));

  Polygon3d poly2;
  poly2.addPoint(Point3d(0, 10.0, 0));
  poly2.addPoint(Point3d(0, 30.0, 0));
  poly2.addPoint(Point3d(100.0, 30.0, 0));
  poly2.addPoint(Point3d(100.0, 10.0, 0));

  Polygon3d poly3;
  poly3.addPoint(Point3d(10.00, 0, 0));
  poly3.addPoint(Point3d(10.0, 100.0, 0));
  poly3.addPoint(Point3d(30.0, 100.0, 0));
  poly3.addPoint(Point3d(30.0, 0, 0));

  Polygon3d poly4;
  poly4.addPoint(Point3d(70.00, 0, 0));
  poly4.addPoint(Point3d(70.00, 100.0, 0));
  poly4.addPoint(Point3d(90.00, 100.0, 0));
  poly4.addPoint(Point3d(90.00, 0, 0));

  polygons.push_back(poly1);
  polygons.push_back(poly4);
  polygons.push_back(poly2);
  polygons.push_back(poly3);

  std::vector<Polygon3d> result = joinAll(polygons, tol);

  // Should return one polygon
  ASSERT_EQ(1u, result.size());

  // The outer polygon should have 28 points
  ASSERT_EQ(28, result[0].getOuterPath().size());

  Polygon3d testPolygon = GetTestPolygon();
  bool b1 = circularEqual(result[0].getOuterPath(), testPolygon.getOuterPath(), 0.01);
  ASSERT_TRUE(b1);

  // The polygon should have one hole with 4 points
  ASSERT_EQ(1, result.front().getInnerPaths().size());
  ASSERT_EQ(4, result.front().getInnerPaths()[0].size());

  bool b2 = circularEqual(result[0].getInnerPaths()[0], testPolygon.getInnerPaths()[0], 0.01);
  ASSERT_TRUE(b2);

  double grossArea = result.front().grossArea();
  ASSERT_NEAR(grossArea, 8000, 0.01);

  double netArea = result.front().netArea();
  ASSERT_NEAR(6400, netArea, 0.001);
  ASSERT_GT(grossArea, netArea);

  boost::optional<double> holeArea = openstudio::getArea(result.front().getInnerPaths().front());
  ASSERT_EQ(grossArea, netArea + *holeArea);

  // double perimeter = result.front().getPerimeter();
}
#endif

#ifdef _WIN32  // Linux, Mac OS need additional review
TEST_F(GeometryFixture, Polygon3d_JoinAllPolygons_1614) {
  double tol = 0.01;

  std::vector<Point3dVector> polygons;

  Point3dVector poly1{
    {0, 70, 0},
    {0, 90, 0},
    {100, 90, 0},
    {100, 70, 0},
  };

  Point3dVector poly2{
    {0, 10, 0},
    {0, 30, 0},
    {100, 30, 0},
    {100, 10, 0},
  };

  Point3dVector poly3{
    {10, 0, 0},
    {10, 100, 0},
    {30, 100, 0},
    {30, 0, 0},
  };

  Point3dVector poly4{
    {70, 0, 0},
    {70, 100, 0},
    {90, 100, 0},
    {90, 0, 0},
  };

  polygons.push_back(poly1);
  polygons.push_back(poly4);
  polygons.push_back(poly2);
  polygons.push_back(poly3);

  std::vector<Polygon3d> result = joinAllPolygons(polygons, tol);

  // Should return one polygon
  ASSERT_EQ(1u, result.size());

  // Should return one polygon
  ASSERT_EQ(1u, result.size());

  // The outer polygon should have 28 points
  ASSERT_EQ(28, result[0].getOuterPath().size());

  Polygon3d testPolygon = GetTestPolygon();
  bool b1 = circularEqual(result[0].getOuterPath(), testPolygon.getOuterPath(), 0.01);
  ASSERT_TRUE(b1);

  // The polygon should have one hole with 4 points
  ASSERT_EQ(1, result.front().getInnerPaths().size());
  ASSERT_EQ(4, result.front().getInnerPaths()[0].size());

  bool b2 = circularEqual(result[0].getInnerPaths()[0], testPolygon.getInnerPaths()[0], 0.01);
  ASSERT_TRUE(b2);

  double grossArea = result.front().grossArea();
  ASSERT_NEAR(grossArea, 8000, 0.01);

  double netArea = result.front().netArea();
  ASSERT_NEAR(6400, netArea, 0.001);
  ASSERT_GT(grossArea, netArea);

  boost::optional<double> holeArea = openstudio::getArea(result.front().getInnerPaths().front());
  ASSERT_EQ(grossArea, netArea + *holeArea);

  // double perimeter = result.front().getPerimeter();
}
#endif

TEST_F(GeometryFixture, Polygon3d_PointInPolygonUp) {
  double tol = 0.01;
  double tol2 = tol / 2.0;

  Polygon3d polygonUp = Polygon3d(makeRectangleUp(0, 0, 1, 1));

  // center
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(0.5, 0.5, 0), tol));

  // corners
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(0, 0, 0), tol));
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(1, 0, 0), tol));
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(1, 1, 0), tol));
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(0, 1, 0), tol));

  // edges
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(0.5, 0, 0), tol));
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(1, 0.5, 0), tol));
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(0.5, 1, 0), tol));
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(0, 0.5, 0), tol));

  EXPECT_FALSE(polygonUp.within(Point3d(0.5, tol2, 0), tol));
  EXPECT_FALSE(polygonUp.within(Point3d(1 - tol2, 0.5, 0), tol));
  EXPECT_FALSE(polygonUp.within(Point3d(0.5, 1 - tol2, 0), tol));
  EXPECT_FALSE(polygonUp.within(Point3d(tol2, 0.5, 0), tol));

  EXPECT_FALSE(polygonUp.within(Point3d(0.5, -tol2, 0), tol));
  EXPECT_FALSE(polygonUp.within(Point3d(1 + tol2, 0.5, 0), tol));
  EXPECT_FALSE(polygonUp.within(Point3d(0.5, 1 + tol2, 0), tol));
  EXPECT_FALSE(polygonUp.within(Point3d(-tol2, 0.5, 0), tol));

  // outside
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(2, 0, 0), tol));
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(1, 2, 0), tol));
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(-1, 0, 0), tol));
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(-1, -1, 0), tol));

  // not on z = 0
  EXPECT_FALSE(polygonUp.pointInPolygon(Point3d(0.5, 0.5, 0.5), tol));
}

TEST_F(GeometryFixture, Polygon3d_PointInPolygonDown) {
  double tol = 0.01;
  double tol2 = tol / 2.0;

  Polygon3d polygonDown(makeRectangleDown(0, 0, 1, 1));

  // center
  EXPECT_TRUE(polygonDown.pointInPolygon(Point3d(0.5, 0.5, 0), tol));

  // corners
  EXPECT_TRUE(polygonDown.pointInPolygon(Point3d(0, 0, 0), tol));
  EXPECT_TRUE(polygonDown.pointInPolygon(Point3d(1, 0, 0), tol));
  EXPECT_TRUE(polygonDown.pointInPolygon(Point3d(1, 1, 0), tol));
  EXPECT_TRUE(polygonDown.pointInPolygon(Point3d(0, 1, 0), tol));

  // edges
  EXPECT_TRUE(polygonDown.pointInPolygon(Point3d(0.5, 0, 0), tol));
  EXPECT_TRUE(polygonDown.pointInPolygon(Point3d(1, 0.5, 0), tol));
  EXPECT_TRUE(polygonDown.pointInPolygon(Point3d(0.5, 1, 0), tol));
  EXPECT_TRUE(polygonDown.pointInPolygon(Point3d(0, 0.5, 0), tol));

  EXPECT_TRUE(polygonDown.within(Point3d(0.5, tol2, 0), tol));
  EXPECT_TRUE(polygonDown.within(Point3d(1 - tol2, 0.5, 0), tol));
  EXPECT_TRUE(polygonDown.within(Point3d(0.5, 1 - tol2, 0), tol));
  EXPECT_TRUE(polygonDown.within(Point3d(tol2, 0.5, 0), tol));

  EXPECT_FALSE(polygonDown.within(Point3d(0.5, -tol2, 0), tol));
  EXPECT_FALSE(polygonDown.within(Point3d(1 + tol2, 0.5, 0), tol));
  EXPECT_FALSE(polygonDown.within(Point3d(0.5, 1 + tol2, 0), tol));
  EXPECT_FALSE(polygonDown.within(Point3d(-tol2, 0.5, 0), tol));

  // outside
  EXPECT_FALSE(polygonDown.pointInPolygon(Point3d(2, 0, 0), tol));
  EXPECT_FALSE(polygonDown.pointInPolygon(Point3d(1, 2, 0), tol));
  EXPECT_FALSE(polygonDown.pointInPolygon(Point3d(-1, 0, 0), tol));
  EXPECT_FALSE(polygonDown.pointInPolygon(Point3d(-1, -1, 0), tol));

  // not on z = 0
  EXPECT_FALSE(polygonDown.pointInPolygon(Point3d(0.5, 0.5, 0.5), tol));
}

TEST_F(GeometryFixture, Polygon3d_Overlap) {

  Polygon3d p;
  p.addPoint(Point3d(0, 0, 0));
  p.addPoint(Point3d(0, 70, 0));
  p.addPoint(Point3d(158, 70, 0));
  p.addPoint(Point3d(158, 98, 0));
  p.addPoint(Point3d(0, 98, 0));
  p.addPoint(Point3d(0, 168, 0));
  p.addPoint(Point3d(158, 168, 0));
  p.addPoint(Point3d(158, 268, 0));
  p.addPoint(Point3d(220, 268, 0));
  p.addPoint(Point3d(220, 200, 0));
  p.addPoint(Point3d(288, 200, 0));
  p.addPoint(Point3d(288, 0, 0));

  // NOTE: LINE is the line being tested to overlap and EDGE on the polygon
  // 1 - line/edge start and end points are the same
  Point3dVector line{
    {158, 98, 0},
    {0, 98, 0},
  };
  Point3dVectorVector overlap = p.overlap(line);
  ASSERT_TRUE(overlap.size() == 1);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][0], Point3d(158, 98, 0)) < 0.1);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][1], Point3d(0, 98, 0)) < 0.1);

  // 2 - line/edge partial overlap from the start (line.sp, edge.sp, line.ep, edge.ep)
  line = {
    {200, 200, 0},
    {250, 200, 0},
  };
  overlap = p.overlap(line);
  ASSERT_TRUE(overlap.size() == 1);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][0], Point3d(220, 200, 0)) < 0.1);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][1], Point3d(250, 200, 0)) < 0.1);

  // 3 - line/edge partially overlap past the end (edge.sp, line.sp, edge.ep, line.ep)
  line = {
    {288, 125, 0},
    {288, -25, 0},
  };
  overlap = p.overlap(line);
  ASSERT_TRUE(overlap.size() == 1);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][0], Point3d(288, 125, 0)) < 0.1);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][1], Point3d(288, 0, 0)) < 0.1);

  // 4 - edge is fully enclosed in line
  line = {
    {150, 268, 0},
    {250, 268, 0},
  };
  overlap = p.overlap(line);
  ASSERT_TRUE(overlap.size() == 1);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][0], Point3d(158, 268, 0)) < 0.1);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][1], Point3d(220, 268, 0)) < 0.1);

  // 5 - line is fully enclosed in edge
  line = {
    {50, 168, 0},
    {100, 168, 0},
  };
  overlap = p.overlap(line);
  ASSERT_TRUE(overlap.size() == 1);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][0], Point3d(50, 168, 0)) < 0.1);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][1], Point3d(100, 168, 0)) < 0.1);

  // 6 - Line overlaps two edges
  line = {
    {158, 25, 0},
    {158, 275, 0},
  };
  overlap = p.overlap(line);
  ASSERT_TRUE(overlap.size() == 2);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][0], Point3d(158, 70, 0)) < 0.1);
  ASSERT_TRUE(openstudio::getDistance(overlap[0][1], Point3d(158, 98, 0)) < 0.1);
  ASSERT_TRUE(openstudio::getDistance(overlap[1][0], Point3d(158, 168, 0)) < 0.1);
  ASSERT_TRUE(openstudio::getDistance(overlap[1][1], Point3d(158, 268, 0)) < 0.1);

  // 7 - No overlap
  line = {
    {50, 50, 0},
    {250, 50, 0},
  };
  overlap = p.overlap(line);
  ASSERT_TRUE(overlap.empty());

  // 8 - No overlap
  line = {
    {50, 50, 0},
    {250, 250, 0},
  };
  overlap = p.overlap(line);
  ASSERT_TRUE(overlap.empty());

  // 9 - No overlap (External corner to 90 deg on external edge)
  line = {
    {158, 168, 0},
    {220, 160, 0},
  };
  overlap = p.overlap(line);
  ASSERT_TRUE(overlap.empty());
}

// core and perimeter with the south perimeter subdivided into 4
// add them all together
TEST_F(GeometryFixture, JoinAll_2527) {
  double tol = 0.01;

  std::vector<Point3dVector> polygons;

  // North
  std::vector<Point3d> poly1{
    {40.869, 30439.131, 0},
    {30439.131, 30439.131, 0},
    {25867, 25867, 0},
    {4612, 25867, 0},
  };
  // East
  std::vector<Point3d> poly2{
    {30439.131, 30439.131, 0},
    {30439.131, 40.869, 0},
    {25867, 4612, 0},
    {25867, 25867, 0},
  };
  std::vector<Point3d> poly3;
  // West
  poly3.push_back(Point3d(40.869, 40.869, 0));
  poly3.push_back(Point3d(40.869, 30439.131, 0));
  poly3.push_back(Point3d(4612, 25867, 0));
  poly3.push_back(Point3d(4612, 4612, 0));
  // Core
  std::vector<Point3d> poly4{
    {25867, 4612, 0},
    {4612, 4612, 0},
    {4612, 25867, 0},
    {25867, 25867, 0},
  };
  // divide the bottom poly left to right, tri, quad, quad, tri
  std::vector<Point3d> poly5{
    {4612, 4612, 0},
    {4612, 40.869, 0},
    {40.869, 40.869, 0},
  };
  std::vector<Point3d> poly6{
    {4612, 4612, 0},
    {4612, 40.869, 0},
    {15219.565, 40.869, 0},
    {15219.565, 4612, 0},
  };
  std::vector<Point3d> poly7{
    {15219.565, 4612, 0},
    {15219.565, 40.869, 0},
    {25867, 40.869, 0},
    {25867, 4612, 0},
  };
  std::vector<Point3d> poly8{
    {25867, 4612, 0},
    {30439.131, 40.869, 0},
    {25867, 40.869, 0},
  };

  // std::vector<Point3d> polyx{
  //   {30439.131, 40.869, 0},
  //   {40.869, 40.869, 0},
  //   {4612, 4612, 0},
  //   {25867, 4612, 0},
  // };

  polygons.push_back(poly1);
  polygons.push_back(poly2);
  polygons.push_back(poly3);
  polygons.push_back(poly4);
  polygons.push_back(poly5);
  polygons.push_back(poly6);
  polygons.push_back(poly7);
  polygons.push_back(poly8);

  // We know this fails because join all does not in fact join all
  std::vector<Point3dVector> test = joinAll(polygons, tol);
  ASSERT_EQ(3u, test.size());

  std::vector<Polygon3d> resultPolygons = joinAllPolygons(polygons, tol);
  ASSERT_EQ(1u, resultPolygons.size());
}

/// <summary>
/// Tests the offset buffer method
/// Note the two tests are taken from
/// https://www.boost.org/doc/libs/1_65_0/libs/geometry/doc/html/geometry/reference/strategies/strategy_buffer_join_miter.html
/// </summary>
/// <param name=""></param>
/// <param name=""></param>
TEST_F(GeometryFixture, Offset) {

  // A simple rectangle, when offset should produce a polygon with four points
  Point3dVector poly1{{8, 7, 0}, {8, 10, 0}, {11, 10, 0}, {11, 7, 0}};
  Point3dVector poly2{
    {7.5, 10.5, 0.0},
    {11.5, 10.5, 0.0},
    {11.5, 6.5, 0.0},
    {7.5, 6.5, 0.0},
  };

  boost::optional<std::vector<Point3d>> result1 = openstudio::buffer(poly1, 0.5, 0.01);
  ASSERT_TRUE(result1);
  Point3dVectorVector tmp{poly1, *result1};
  LOG(Debug, tmp);

  ASSERT_EQ(4, result1.get().size());
  EXPECT_TRUE(circularEqual(poly2, *result1));

  // Two shapes, a triangle and a rectangle. when offset will combine into 1 shape
  // with 8 vertices
  Point3dVector poly3{
    {5, 5, 0},
    {7, 8, 0},
    {9, 5, 0},
  };

  std::vector<Point3dVector> polygons;
  polygons.push_back(poly1);
  polygons.push_back(poly3);

  boost::optional<std::vector<Point3dVector>> result3 = openstudio::buffer(polygons, 0.5, 0.01);
  ASSERT_TRUE(result3);
  ASSERT_EQ(1, result3->size());
  polygons.push_back(result3->begin()[0]);
  LOG(Debug, polygons);

  Point3dVector resultPoly = result3.get().front();
  ASSERT_EQ(8, resultPoly.size());
  boost::optional<std::vector<Point3dVector>> result2 = openstudio::buffer(*result3, -0.5, 0.01);
  ASSERT_EQ(2, result2->size());
  ASSERT_EQ(3, result2.get()[0].size());
  ASSERT_EQ(4, result2.get()[1].size());
}

// Adds 9 squares (3x3) together
TEST_F(GeometryFixture, Offset1) {

  Point3dVector poly1{
    {0, 20, 0},
    {0, 30, 0},
    {10, 30, 0},
    {10, 20, 0},
  };

  Point3dVector poly2{
    {10, 20, 0},
    {10, 30, 0},
    {20, 30, 0},
    {20, 20, 0},
  };

  Point3dVector poly3{
    {20, 20, 0},
    {20, 30, 0},
    {30, 30, 0},
    {30, 20, 0},
  };

  Point3dVector poly4{
    {0, 10, 0},
    {0, 20, 0},
    {10, 20, 0},
    {10, 10, 0},
  };

  Point3dVector poly5{
    {10, 10, 0},
    {10, 20, 0},
    {20, 20, 0},
    {20, 10, 0},
  };

  Point3dVector poly6{
    {20, 10, 0},
    {20, 20, 0},
    {30, 20, 0},
    {30, 10, 0},
  };

  Point3dVector poly7{
    {0, 0, 0},
    {0, 10, 0},
    {10, 10, 0},
    {10, 0, 0},
  };

  Point3dVector poly8{
    {10, 0, 0},
    {10, 10, 0},
    {20, 10, 0},
    {20, 0, 0},
  };

  Point3dVector poly9{
    {20, 0, 0},
    {20, 10, 0},
    {30, 10, 0},
    {30, 0, 0},
  };

  std::vector<Point3dVector> polygons;
  polygons.push_back(poly1);
  polygons.push_back(poly2);
  polygons.push_back(poly3);
  polygons.push_back(poly4);
  polygons.push_back(poly5);
  polygons.push_back(poly6);
  polygons.push_back(poly7);
  polygons.push_back(poly8);
  polygons.push_back(poly9);

  boost::optional<std::vector<Point3dVector>> result1 = openstudio::buffer(polygons, 0.5, 0.01);
  ASSERT_EQ(1, result1->size());
  boost::optional<std::vector<Point3dVector>> result2 = openstudio::buffer(*result1, -0.5, 0.01);
  ASSERT_EQ(1, result2->size());

  std::vector<Point3d> result{
    {0, 30, 0},
    {30, 30, 0},
    {30, 0, 0},
    {0, 0, 0},
  };
  bool b1 = circularEqual(result2->front(), result);
  ASSERT_TRUE(b1);
}

// Core and perimeter polygons
TEST_F(GeometryFixture, Offset2) {

  // North
  Point3dVector poly1{
    {0, 30, 0},
    {30, 30, 0},
    {20, 20, 0},
    {10, 20, 0},
  };

  // East
  Point3dVector poly2{
    {30, 30, 0},
    {30, 0, 0},
    {20, 10, 0},
    {20, 20, 0},
  };

  // South
  Point3dVector poly3{
    {30, 0, 0},
    {0, 0, 0},
    {10, 10, 0},
    {20, 10, 0},
  };

  // West
  Point3dVector poly4{
    {0, 00, 0},
    {0, 30, 0},
    {10, 20, 0},
    {10, 10, 0},
  };

  // Core
  Point3dVector poly5{
    {10, 10, 0},
    {10, 20, 0},
    {20, 20, 0},
    {20, 10, 0},
  };

  std::vector<Point3dVector> polygons;
  polygons.push_back(poly1);
  polygons.push_back(poly2);
  polygons.push_back(poly3);
  polygons.push_back(poly4);
  polygons.push_back(poly5);

  // buffer the core polygon
  std::vector<Point3dVector> singlePolygon;
  singlePolygon.push_back(poly1);
  boost::optional<std::vector<Point3dVector>> result1 = openstudio::buffer(singlePolygon, 5, 0.01);
  boost::optional<std::vector<Point3dVector>> result2 = openstudio::buffer(*result1, -5, 0.01);

  bool b1 = circularEqual(poly1, result2->front(), 0.01);
  ASSERT_TRUE(b1);

  boost::optional<std::vector<Point3dVector>> result3 = openstudio::buffer(polygons, 0.5, 0.01);
  boost::optional<std::vector<Point3dVector>> result4 = openstudio::buffer(*result3, -0.5, 0.01);

  Point3dVector testPolygon{
    {0, 0, 0},
    {0, 30, 0},
    {30, 30, 0},
    {30, 0, 0},
  };

  bool b2 = circularEqual(testPolygon, result4->front(), 0.01);
  ASSERT_TRUE(b2);
}

TEST_F(GeometryFixture, Issue_3982) {
  double tol = 0.1;

  // Create a rectangular surface and an overlapping triangular surface and intersect them
  Point3dVector faceVertices{
    {0, 0, 0},
    {0, 10, 0},
    {50, 10, 0},
    {50, 0, 0},
  };

  Point3dVector otherFaceVertices{
    {25, 0, 0},
    {37.50, 8, 0},
    {50, 0, 0},
  };

  // Returns a concave surface
  boost::optional<IntersectionResult> intersection1 = openstudio::intersect(faceVertices, otherFaceVertices, tol);
  ASSERT_EQ(3, intersection1->polygon1().size());
  ASSERT_EQ(3, intersection1->polygon2().size());
  ASSERT_EQ(1, intersection1->newPolygons1().size());
  ASSERT_EQ(6, intersection1->newPolygons1()[0].size());
  ASSERT_EQ(0, intersection1->newPolygons2().size());

  otherFaceVertices = {
    {24, 1, 0},
    {36.50, 8, 0},
    {49, 1, 0},
  };

  // Returns a triangulated hole
  boost::optional<IntersectionResult> intersection2 = openstudio::intersect(faceVertices, otherFaceVertices, tol);
  ASSERT_EQ(3, intersection2->polygon1().size());
  ASSERT_EQ(3, intersection2->polygon2().size());
  ASSERT_EQ(5, intersection2->newPolygons1().size());
  ASSERT_EQ(4, intersection2->newPolygons1()[0].size());
  ASSERT_EQ(3, intersection2->newPolygons1()[1].size());
  ASSERT_EQ(3, intersection2->newPolygons1()[2].size());
  ASSERT_EQ(3, intersection2->newPolygons1()[3].size());
  ASSERT_EQ(4, intersection2->newPolygons1()[4].size());
  ASSERT_EQ(0, intersection1->newPolygons2().size());
}

TEST_F(GeometryFixture, Intersection_Stability) {
  // Test for #4831

  std::vector<std::vector<Point3d>> toplit_polygons = {
    // Polygon 0
    {
      {-3.2099999999999995, -3.3206999999999995, 0.0},
      {-3.2099999999999995, 9.0985, 0.0},
      {9.209999999999999, 9.0985, 0.0},
      {9.209999999999999, -3.3206999999999995, 0.0},
    },
    // Polygon 1
    {
      {2.790000000000001, 8.2348, 0.0},
      {2.790000000000001, 20.654, 0.0},
      {15.209999999999999, 20.654, 0.0},
      {15.209999999999999, 8.2348, 0.0},
    },
    // Polygon 2
    {
      {8.790000000000001, 8.2348, 0.0},
      {8.790000000000001, 20.654, 0.0},
      {21.21, 20.654, 0.0},
      {21.21, 8.2348, 0.0},
    },
    // Polygon 3
    {
      {14.790000000000001, 8.2348, 0.0},
      {14.790000000000001, 20.654, 0.0},
      {27.21, 20.654, 0.0},
      {27.21, 8.2348, 0.0},
    },
    // Polygon 4
    {
      {-3.2099999999999995, 14.0126, 0.0},
      {-3.2099999999999995, 26.431800000000003, 0.0},
      {9.209999999999999, 26.431800000000003, 0.0},
      {9.209999999999999, 14.0126, 0.0},
    },
    // Polygon 5
    {
      {2.790000000000001, 14.0126, 0.0},
      {2.790000000000001, 26.431800000000003, 0.0},
      {15.209999999999999, 26.431800000000003, 0.0},
      {15.209999999999999, 14.0126, 0.0},
    },
    // Polygon 6
    {
      {8.790000000000001, 14.0126, 0.0},
      {8.790000000000001, 26.431800000000003, 0.0},
      {21.21, 26.431800000000003, 0.0},
      {21.21, 14.0126, 0.0},
    },
    // Polygon 7
    {
      {14.790000000000001, 14.0126, 0.0},
      {14.790000000000001, 26.431800000000003, 0.0},
      {27.21, 26.431800000000003, 0.0},
      {27.21, 14.0126, 0.0},
    },
    // Polygon 8
    {
      {-3.2099999999999995, 19.790399999999998, 0.0},
      {-3.2099999999999995, 32.2096, 0.0},
      {9.209999999999999, 32.2096, 0.0},
      {9.209999999999999, 19.790399999999998, 0.0},
    },
    // Polygon 9
    {
      {2.790000000000001, 19.790399999999998, 0.0},
      {2.790000000000001, 32.2096, 0.0},
      {15.209999999999999, 32.2096, 0.0},
      {15.209999999999999, 19.790399999999998, 0.0},
    },
    // Polygon 10
    {
      {8.790000000000001, 19.790399999999998, 0.0},
      {8.790000000000001, 32.2096, 0.0},
      {21.21, 32.2096, 0.0},
      {21.21, 19.790399999999998, 0.0},
    },
    // Polygon 11
    {
      {2.790000000000001, -3.3206999999999995, 0.0},
      {2.790000000000001, 9.0985, 0.0},
      {15.209999999999999, 9.0985, 0.0},
      {15.209999999999999, -3.3206999999999995, 0.0},
    },
    // Polygon 12
    {
      {14.790000000000001, 19.790399999999998, 0.0},
      {14.790000000000001, 32.2096, 0.0},
      {27.21, 32.2096, 0.0},
      {27.21, 19.790399999999998, 0.0},
    },
    // Polygon 13
    {
      {-3.2099999999999995, 25.568199999999997, 0.0},
      {-3.2099999999999995, 37.9874, 0.0},
      {9.209999999999999, 37.9874, 0.0},
      {9.209999999999999, 25.568199999999997, 0.0},
    },
    // Polygon 14
    {
      {2.790000000000001, 25.568199999999997, 0.0},
      {2.790000000000001, 37.9874, 0.0},
      {15.209999999999999, 37.9874, 0.0},
      {15.209999999999999, 25.568199999999997, 0.0},
    },
    // Polygon 15
    {
      {8.790000000000001, 25.568199999999997, 0.0},
      {8.790000000000001, 37.9874, 0.0},
      {21.21, 37.9874, 0.0},
      {21.21, 25.568199999999997, 0.0},
    },
    // Polygon 16
    {
      {14.790000000000001, 25.568199999999997, 0.0},
      {14.790000000000001, 37.9874, 0.0},
      {27.21, 37.9874, 0.0},
      {27.21, 25.568199999999997, 0.0},
    },
    // Polygon 17
    {
      {-3.2099999999999995, 31.345999999999997, 0.0},
      {-3.2099999999999995, 43.7652, 0.0},
      {9.209999999999999, 43.7652, 0.0},
      {9.209999999999999, 31.345999999999997, 0.0},
    },
    // Polygon 18
    {
      {2.790000000000001, 31.345999999999997, 0.0},
      {2.790000000000001, 43.7652, 0.0},
      {15.209999999999999, 43.7652, 0.0},
      {15.209999999999999, 31.345999999999997, 0.0},
    },
    // Polygon 19
    {
      {8.790000000000001, 31.345999999999997, 0.0},
      {8.790000000000001, 43.7652, 0.0},
      {21.21, 43.7652, 0.0},
      {21.21, 31.345999999999997, 0.0},
    },
    // Polygon 20
    {
      {14.790000000000001, 31.345999999999997, 0.0},
      {14.790000000000001, 43.7652, 0.0},
      {27.21, 43.7652, 0.0},
      {27.21, 31.345999999999997, 0.0},
    },
    // Polygon 21
    {
      {-3.2099999999999995, 37.1237, 0.0},
      {-3.2099999999999995, 49.5429, 0.0},
      {9.209999999999999, 49.5429, 0.0},
      {9.209999999999999, 37.1237, 0.0},
    },
    // Polygon 22
    {
      {8.790000000000001, -3.3206999999999995, 0.0},
      {8.790000000000001, 9.0985, 0.0},
      {21.21, 9.0985, 0.0},
      {21.21, -3.3206999999999995, 0.0},
    },
    // Polygon 23
    {
      {2.790000000000001, 37.1237, 0.0},
      {2.790000000000001, 49.5429, 0.0},
      {15.209999999999999, 49.5429, 0.0},
      {15.209999999999999, 37.1237, 0.0},
    },
    // Polygon 24
    {
      {8.790000000000001, 37.1237, 0.0},
      {8.790000000000001, 49.5429, 0.0},
      {21.21, 49.5429, 0.0},
      {21.21, 37.1237, 0.0},
    },
    // Polygon 25
    {
      {14.790000000000001, 37.1237, 0.0},
      {14.790000000000001, 49.5429, 0.0},
      {27.21, 49.5429, 0.0},
      {27.21, 37.1237, 0.0},
    },
    // Polygon 26
    {
      {-3.2099999999999995, 42.9015, 0.0},
      {-3.2099999999999995, 55.3207, 0.0},
      {9.209999999999999, 55.3207, 0.0},
      {9.209999999999999, 42.9015, 0.0},
    },
    // Polygon 27
    {
      {2.790000000000001, 42.9015, 0.0},
      {2.790000000000001, 55.3207, 0.0},
      {15.209999999999999, 55.3207, 0.0},
      {15.209999999999999, 42.9015, 0.0},
    },
    // Polygon 28
    {
      {8.790000000000001, 42.9015, 0.0},
      {8.790000000000001, 55.3207, 0.0},
      {21.21, 55.3207, 0.0},
      {21.21, 42.9015, 0.0},
    },
    // Polygon 29
    {
      {14.790000000000001, 42.9015, 0.0},
      {14.790000000000001, 55.3207, 0.0},
      {27.21, 55.3207, 0.0},
      {27.21, 42.9015, 0.0},
    },
    // Polygon 30
    {
      {14.790000000000001, -3.3206999999999995, 0.0},
      {14.790000000000001, 9.0985, 0.0},
      {27.21, 9.0985, 0.0},
      {27.21, -3.3206999999999995, 0.0},
    },
    // Polygon 31
    {
      {-3.2099999999999995, 2.4571000000000005, 0.0},
      {-3.2099999999999995, 14.8763, 0.0},
      {9.209999999999999, 14.8763, 0.0},
      {9.209999999999999, 2.4571000000000005, 0.0},
    },
    // Polygon 32
    {
      {2.790000000000001, 2.4571000000000005, 0.0},
      {2.790000000000001, 14.8763, 0.0},
      {15.209999999999999, 14.8763, 0.0},
      {15.209999999999999, 2.4571000000000005, 0.0},
    },
    // Polygon 33
    {
      {8.790000000000001, 2.4571000000000005, 0.0},
      {8.790000000000001, 14.8763, 0.0},
      {21.21, 14.8763, 0.0},
      {21.21, 2.4571000000000005, 0.0},
    },
    // Polygon 34
    {
      {14.790000000000001, 2.4571000000000005, 0.0},
      {14.790000000000001, 14.8763, 0.0},
      {27.21, 14.8763, 0.0},
      {27.21, 2.4571000000000005, 0.0},
    },
    // Polygon 35
    {
      {-3.2099999999999995, 8.2348, 0.0},
      {-3.2099999999999995, 20.654, 0.0},
      {9.209999999999999, 20.654, 0.0},
      {9.209999999999999, 8.2348, 0.0},
    },
  };

  constexpr double tol = 0.01;

  StringStreamLogSink sink;
  sink.setLogLevel(Error);

  auto combinedPolygons = openstudio::joinAll(toplit_polygons, tol);
  EXPECT_TRUE(sink.logMessages().empty());
  ASSERT_EQ(1, combinedPolygons.size());
  auto& polygon = combinedPolygons.front();
  std::vector<Point3d> expectedPolygon{
    {27.21, 55.3207, 0.0},
    {27.21, -3.3207, 0},
    {-3.21, -3.3207, 0},
    {-3.21, 55.3207, 0},
  };
  EXPECT_EQ(4, polygon.size());
  size_t i = 0;
  for (const auto& pt : polygon) {
    EXPECT_TRUE(openstudio::isAlmostEqual3dPt(expectedPolygon[i++], pt, 0.001));
  }
}
