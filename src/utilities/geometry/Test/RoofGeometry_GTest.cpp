/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "../RoofGeometry.hpp"
#include "../RoofGeometry_Details.hpp"
#include "../Intersection.hpp"
#include "../Point3d.hpp"
#include "GeometryFixture.hpp"
#include <math.h>

using namespace openstudio;

void polygonMatches(const std::vector<std::vector<Point3d>>& expectedPolygons, const std::vector<Point3d>& polygon) {
  int numMatches = 0;
  for (const std::vector<Point3d>& expectedPolygon : expectedPolygons) {
    if (circularEqual(expectedPolygon, polygon, 0.01)) {
      numMatches += 1;
    }
  }

  //if (numMatches != 1) {
  //  std::cout << "NOT MATCHED" << '\n';
  //  for (std::vector<Point3d> expectedPolygon : expectedPolygons) {
  //    std::cout << "EXPECTED POLYGON:" << '\n';;
  //    for (Point3d p : expectedPolygon) {
  //      std::cout << p << '\n';
  //    }
  //  }
  //  std::cout << "CALCULATED POLYGON:" << '\n';
  //  for (Point3d p : polygon) {
  //    std::cout << p << '\n';
  //  }
  //} else {
  //  std::cout << "MATCHED!" << '\n';
  //}

  EXPECT_EQ(numMatches, 1);
}

// Try to find index of last vertex after opposite edge is found.
// Index is calculated relatively from given starting vertex.
int findSplitIndex(std::shared_ptr<Vertex> /*vertex*/, std::vector<std::shared_ptr<Vertex>>& lav, std::shared_ptr<Edge> oppositeEdge) {
  for (unsigned i = 0; i < lav.size(); i++) {
    std::shared_ptr<Vertex> currentVertex = lav[i];
    if (oppositeEdge == currentVertex->previousEdge || oppositeEdge == Vertex::previous(currentVertex, lav)->nextEdge) {
      return i;
    }
  }
  return -1;
}

TEST_F(GeometryFixture, SkeletonInternal_testEdgeBehindBisector_1) {
  Point3d p = Point3d(0, -1, 0);
  Vector3d v = Vector3d(0, 1, 0);
  std::shared_ptr<Ray2d> bisector(new Ray2d(p, v));
  Point3d e1 = Point3d(-1, 0, 0);
  Point3d e2 = Point3d(1, 0, 0);
  LineLinear2d edge = LineLinear2d(e1, e2);
  EXPECT_FALSE(edgeBehindBisector(bisector, edge));
}

TEST_F(GeometryFixture, SkeletonInternal_testEdgeBehindBisector_2) {
  Point3d p = Point3d(0, 0, 0);
  Vector3d v = Vector3d(1, 0, 0);
  std::shared_ptr<Ray2d> bisector(new Ray2d(p, v));
  Point3d e1 = Point3d(-1, 0, 0);
  Point3d e2 = Point3d(1, 0, 0);
  LineLinear2d edge = LineLinear2d(e1, e2);
  EXPECT_TRUE(edgeBehindBisector(bisector, edge));
}

TEST_F(GeometryFixture, SkeletonInternal_testEdgeBehindBisector_3) {
  Point3d p = Point3d(0, 0, 0);
  Vector3d v = Vector3d(0, 1, 0);
  std::shared_ptr<Ray2d> bisector(new Ray2d(p, v));
  Point3d e1 = Point3d(0, 1, 0);
  Point3d e2 = Point3d(0, -1, 0);
  LineLinear2d edge = LineLinear2d(e1, e2);
  EXPECT_TRUE(edgeBehindBisector(bisector, edge));
}

TEST_F(GeometryFixture, SkeletonInternal_testEdgeBehindBisector_4) {
  Point3d p = Point3d(-1, 0.0000001, 0);
  Vector3d v = Vector3d(1, 0, 0);
  std::shared_ptr<Ray2d> bisector(new Ray2d(p, v));
  Point3d e1 = Point3d(-1, 0, 0);
  Point3d e2 = Point3d(1, 0, 0);
  LineLinear2d edge = LineLinear2d(e1, e2);
  EXPECT_TRUE(edgeBehindBisector(bisector, edge));
}

TEST_F(GeometryFixture, SkeletonInternal_testFindSplitIndex_1) {
  std::shared_ptr<Edge> anyEdge(new Edge());
  std::shared_ptr<Edge> oppositeEdge(new Edge());

  std::shared_ptr<Vertex> v1(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v2(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v3(new Vertex(oppositeEdge, anyEdge));
  std::shared_ptr<Vertex> v4(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v5(new Vertex(anyEdge, anyEdge));

  std::vector<std::shared_ptr<Vertex>> lav = {v1, v2, v3, v4, v5};

  int split = findSplitIndex(v1, lav, oppositeEdge);

  EXPECT_EQ(2, split);
}

TEST_F(GeometryFixture, SkeletonInternal_testFindSplitIndex_2) {
  std::shared_ptr<Edge> anyEdge(new Edge());
  std::shared_ptr<Edge> oppositeEdge(new Edge());

  std::shared_ptr<Vertex> v1(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v2(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v3(new Vertex(anyEdge, oppositeEdge));
  std::shared_ptr<Vertex> v4(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v5(new Vertex(anyEdge, anyEdge));

  std::vector<std::shared_ptr<Vertex>> lav = {v1, v2, v3, v4, v5};

  int split = findSplitIndex(v1, lav, oppositeEdge);

  EXPECT_EQ(3, split);
}

TEST_F(GeometryFixture, SkeletonInternal_testFindSplitIndex_3) {
  std::shared_ptr<Edge> anyEdge(new Edge());
  std::shared_ptr<Edge> oppositeEdge(new Edge());

  std::shared_ptr<Vertex> v1(new Vertex(oppositeEdge, anyEdge));
  std::shared_ptr<Vertex> v2(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v3(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v4(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v5(new Vertex(anyEdge, anyEdge));

  std::vector<std::shared_ptr<Vertex>> lav = {v1, v2, v3, v4, v5};

  int split = findSplitIndex(v1, lav, oppositeEdge);

  EXPECT_EQ(0, split);
}

TEST_F(GeometryFixture, SkeletonInternal_testFindSplitIndex_4) {
  std::shared_ptr<Edge> anyEdge(new Edge());
  std::shared_ptr<Edge> oppositeEdge(new Edge());

  std::shared_ptr<Vertex> v1(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v2(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v3(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v4(new Vertex(anyEdge, anyEdge));
  std::shared_ptr<Vertex> v5(new Vertex(anyEdge, oppositeEdge));

  std::vector<std::shared_ptr<Vertex>> lav = {v1, v2, v3, v4, v5};

  int split = findSplitIndex(v1, lav, oppositeEdge);

  EXPECT_EQ(0, split);
}

TEST_F(GeometryFixture, SkeletonInternal_chooseOppositeEdgeLav) {
  Point3d p1 = Point3d(0, 0, 0);
  Point3d p2 = Point3d(1, 0, 0);
  std::shared_ptr<Edge> oppositeEdge(new Edge(p1, p2));

  Point3d center = Point3d(0.5, 1, 0);

  std::shared_ptr<Vertex> v1(new Vertex(p1, 0, nullptr, nullptr, nullptr));
  std::shared_ptr<Vertex> v2(new Vertex(p2, 0, nullptr, nullptr, nullptr));

  std::vector<std::shared_ptr<Vertex>> edgeLavs = {v1, v2};

  std::vector<std::shared_ptr<Vertex>> lav = {v1, v2};
  std::vector<std::vector<std::shared_ptr<Vertex>>> sLav = {lav};

  int vertexIndex = chooseOppositeEdgeLavIndex(edgeLavs, oppositeEdge, center, sLav);

  EXPECT_EQ(edgeLavs[vertexIndex], v1);
}

TEST_F(GeometryFixture, SkeletonInternal_chooseOppositeEdgeLav_2) {
  Point3d p1 = Point3d(0, 0, 0);
  Point3d p2 = Point3d(0.2, 0, 0);
  Point3d p3 = Point3d(0.2, 0, 0);
  Point3d p4 = Point3d(1, 0, 0);
  std::shared_ptr<Edge> oppositeEdge(new Edge(p1, p3));

  Point3d center = Point3d(0.5, 1, 0);

  std::shared_ptr<Vertex> v1(new Vertex(p1, 0, nullptr, nullptr, nullptr));
  std::shared_ptr<Vertex> v2(new Vertex(p2, 0, nullptr, nullptr, nullptr));
  std::shared_ptr<Vertex> v3(new Vertex(p3, 0, nullptr, nullptr, nullptr));
  std::shared_ptr<Vertex> v4(new Vertex(p4, 0, nullptr, nullptr, nullptr));

  std::vector<std::shared_ptr<Vertex>> edgeLavs = {v1, v3};

  std::vector<std::shared_ptr<Vertex>> lav1 = {v1, v2};
  std::vector<std::shared_ptr<Vertex>> lav2 = {v3, v4};
  std::vector<std::vector<std::shared_ptr<Vertex>>> sLav = {lav1, lav2};

  int vertexIndex = chooseOppositeEdgeLavIndex(edgeLavs, oppositeEdge, center, sLav);

  EXPECT_EQ(edgeLavs[vertexIndex], v3);
}

TEST_F(GeometryFixture, SkeletonInternal_createEdgeChain_1) {
  std::shared_ptr<Vertex> v1(new Vertex());
  std::shared_ptr<Vertex> v2(new Vertex());
  std::shared_ptr<Vertex> v3(new Vertex());
  std::shared_ptr<Vertex> v4(new Vertex());
  std::shared_ptr<Vertex> v5(new Vertex());

  Point3d p = Point3d();
  std::shared_ptr<QueueEvent> e1(new QueueEvent(p, 0, v1, v2));
  std::shared_ptr<QueueEvent> e2(new QueueEvent(p, 0, v3, v4));
  std::shared_ptr<QueueEvent> e3(new QueueEvent(p, 0, v2, v5));

  std::vector<std::shared_ptr<QueueEvent>> edgeCluster = {e1, e2, e3};

  std::vector<std::shared_ptr<QueueEvent>> chain = createEdgeChain(edgeCluster);

  EXPECT_EQ(2, chain.size());
  EXPECT_EQ(e1, chain[0]);
  EXPECT_EQ(e3, chain[1]);
}

TEST_F(GeometryFixture, SkeletonInternal_createEdgeChain_2) {
  std::shared_ptr<Vertex> v1(new Vertex());
  std::shared_ptr<Vertex> v2(new Vertex());
  std::shared_ptr<Vertex> v3(new Vertex());
  std::shared_ptr<Vertex> v4(new Vertex());

  Point3d p = Point3d();
  std::shared_ptr<QueueEvent> e1(new QueueEvent(p, 0, v1, v2));
  std::shared_ptr<QueueEvent> e2(new QueueEvent(p, 0, v3, v1));
  std::shared_ptr<QueueEvent> e3(new QueueEvent(p, 0, v2, v4));

  std::vector<std::shared_ptr<QueueEvent>> edgeCluster = {e1, e2, e3};

  std::vector<std::shared_ptr<QueueEvent>> chain = createEdgeChain(edgeCluster);

  EXPECT_EQ(3, chain.size());
  EXPECT_EQ(e2, chain[0]);
  EXPECT_EQ(e1, chain[1]);
  EXPECT_EQ(e3, chain[2]);
}

TEST_F(GeometryFixture, SkeletonInternal_groupLevelEvents_1) {
  std::shared_ptr<Vertex> v1(new Vertex());
  std::shared_ptr<Vertex> v2(new Vertex());
  std::shared_ptr<Vertex> v3(new Vertex());
  std::shared_ptr<Vertex> v4(new Vertex());

  Point3d p = Point3d(0, 0, 0);
  std::shared_ptr<QueueEvent> e1(new QueueEvent(p, 0, v1, v2));
  std::shared_ptr<QueueEvent> e2(new QueueEvent(p, 0, v2, v3));
  std::shared_ptr<QueueEvent> e3(new QueueEvent(p, 0, v3, v4));
  std::shared_ptr<QueueEvent> e4(new QueueEvent(p, 0, v4, v1));

  std::vector<std::shared_ptr<QueueEvent>> edgeCluster = {e1, e2, e3, e4};

  std::vector<std::shared_ptr<Vertex>> lav = {v1, v2, v3, v4};
  std::vector<std::vector<std::shared_ptr<Vertex>>> sLav = {lav};

  std::vector<LevelEvent> chain = groupLevelEvents(edgeCluster, sLav);

  EXPECT_TRUE(chain[0].eventType == LevelEvent::TYPE_PICK);
}

TEST_F(GeometryFixture, SkeletonInternal_cutLavPart) {
  std::shared_ptr<Vertex> v1(new Vertex());
  std::shared_ptr<Vertex> v2(new Vertex());
  std::shared_ptr<Vertex> v3(new Vertex());
  std::shared_ptr<Vertex> v4(new Vertex());
  std::shared_ptr<Vertex> v5(new Vertex());

  std::vector<std::shared_ptr<Vertex>> lav = {v1, v2, v3, v4, v5};

  std::vector<std::shared_ptr<Vertex>> lavPart = cutLavPart(lav, v2, v3);

  EXPECT_EQ(2, lavPart.size());
  EXPECT_EQ(v2, lavPart[0]);
  EXPECT_EQ(v3, lavPart[1]);
}

TEST_F(GeometryFixture, SkeletonInternal_cutLavPart_2) {
  std::shared_ptr<Vertex> v1(new Vertex());
  std::shared_ptr<Vertex> v2(new Vertex());
  std::shared_ptr<Vertex> v3(new Vertex());
  std::shared_ptr<Vertex> v4(new Vertex());
  std::shared_ptr<Vertex> v5(new Vertex());

  std::vector<std::shared_ptr<Vertex>> lav = {v1, v2, v3, v4, v5};

  std::vector<std::shared_ptr<Vertex>> lavPart = cutLavPart(lav, v1, v1);

  EXPECT_EQ(1, lavPart.size());
  EXPECT_EQ(v1, lavPart[0]);
}

TEST_F(GeometryFixture, SkeletonLevelEvents_pickEvent) {
  std::vector<Point3d> footprint{
    {-1, -1, 0},
    {1, -1, 0},
    {1, 1, 0},
    {-1, 1, 0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, 0.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.0, 0.0, 0.0), Point3d(1.0, -1.0, 0.0), Point3d(1.0, 1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.0, 0.0, 0.0), Point3d(1.0, 1.0, 0.0), Point3d(-1.0, 1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.0, 0.0, 0.0), Point3d(-1.0, 1.0, 0.0), Point3d(-1.0, -1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.0, 0.0, 0.0), Point3d(-1.0, -1.0, 0.0), Point3d(1.0, -1.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, SkeletonLevelEvents_multiEdgeEvent) {
  std::vector<Point3d> footprint{
    {0, 1, 0}, {-1, 0, 0}, {0, -1, 0}, {5, -2, 0}, {7, 0, 0}, {5, 2, 0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, 0.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.5351837584879964, 1.0062969024153899E-16, 0.0), Point3d(-1.0, 0.0, 0.0), Point3d(0.0, -1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(4.39444872453601, 2.6834584064410396E-16, 0.0), Point3d(0.5351837584879964, 1.0062969024153899E-16, 0.0),
                                  Point3d(0.0, -1.0, 0.0), Point3d(5.0, -2.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(4.39444872453601, 2.6834584064410396E-16, 0.0), Point3d(5.0, -2.0, 0.0), Point3d(7.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(4.39444872453601, 2.6834584064410396E-16, 0.0), Point3d(7.0, 0.0, 0.0), Point3d(5.0, 2.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(4.39444872453601, 2.6834584064410396E-16, 0.0), Point3d(5.0, 2.0, 0.0), Point3d(0.0, 1.0, 0.0),
                                  Point3d(0.5351837584879964, 1.0062969024153899E-16, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.5351837584879964, 1.0062969024153899E-16, 0.0), Point3d(0.0, 1.0, 0.0), Point3d(-1.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, SkeletonLevelEvents_cross_T1) {
  std::vector<Point3d> footprint{
    {-3.0, -1.0, 0.0}, {3.0, -1.0, 0.0}, {3.0, 1.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 3.0, 0.0}, {-1.0, 3.0, 0.0}, {-1.0, 1.0, 0.0}, {-3.0, 1.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, 0.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(2.0, 0.0, 0.0), Point3d(3.0, -1.0, 0.0), Point3d(3.0, 1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.0, 0.0, 0.0), Point3d(3.0, 1.0, 0.0), Point3d(1.0, 1.0, 0.0), Point3d(-0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-0.0, 0.0, 0.0), Point3d(1.0, 1.0, 0.0), Point3d(1.0, 3.0, 0.0), Point3d(-0.0, 2.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-0.0, 2.0, 0.0), Point3d(1.0, 3.0, 0.0), Point3d(-1.0, 3.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-0.0, 2.0, 0.0), Point3d(-1.0, 3.0, 0.0), Point3d(-1.0, 1.0, 0.0), Point3d(-0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-0.0, 0.0, 0.0), Point3d(-1.0, 1.0, 0.0), Point3d(-3.0, 1.0, 0.0), Point3d(-2.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-2.0, 0.0, 0.0), Point3d(-3.0, 1.0, 0.0), Point3d(-3.0, -1.0, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-0.0, 0.0, 0.0), Point3d(-2.0, 0.0, 0.0), Point3d(-3.0, -1.0, 0.0), Point3d(3.0, -1.0, 0.0), Point3d(2.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, SkeletonLevelEvents_cross_X1) {
  std::vector<Point3d> footprint{
    {-3.0, -1.0, 0.0}, {-1.0, -1.0, 0.0}, {-1.0, -3.0, 0.0}, {1.0, -3.0, 0.0}, {1.0, -1.0, 0.0}, {3.0, -1.0, 0.0},
    {3.0, 1.0, 0.0},   {1.0, 1.0, 0.0},   {1.0, 3.0, 0.0},   {-1.0, 3.0, 0.0}, {-1.0, 1.0, 0.0}, {-3.0, 1.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, 0.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.0, 0.0, 0.0), Point3d(-1.0, -1.0, 0.0), Point3d(-1.0, -3.0, 0.0), Point3d(-0.0, -2.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-0.0, -2.0, 0.0), Point3d(-1.0, -3.0, 0.0), Point3d(1.0, -3.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-0.0, -2.0, 0.0), Point3d(1.0, -3.0, 0.0), Point3d(1.0, -1.0, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.0, 0.0, 0.0), Point3d(1.0, -1.0, 0.0), Point3d(3.0, -1.0, 0.0), Point3d(2.0, -0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.0, -0.0, 0.0), Point3d(3.0, -1.0, 0.0), Point3d(3.0, 1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.0, -0.0, 0.0), Point3d(3.0, 1.0, 0.0), Point3d(1.0, 1.0, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.0, 0.0, 0.0), Point3d(1.0, 1.0, 0.0), Point3d(1.0, 3.0, 0.0), Point3d(0.0, 2.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.0, 2.0, 0.0), Point3d(1.0, 3.0, 0.0), Point3d(-1.0, 3.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.0, 2.0, 0.0), Point3d(-1.0, 3.0, 0.0), Point3d(-1.0, 1.0, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.0, 0.0, 0.0), Point3d(-1.0, 1.0, 0.0), Point3d(-3.0, 1.0, 0.0), Point3d(-2.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-2.0, 0.0, 0.0), Point3d(-3.0, 1.0, 0.0), Point3d(-3.0, -1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-2.0, 0.0, 0.0), Point3d(-3.0, -1.0, 0.0), Point3d(-1.0, -1.0, 0.0), Point3d(0.0, 0.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, SkeletonLevelEvents_double_split) {
  std::vector<Point3d> footprint{
    {-6.0, 0.0, 0.0}, {-3.0, -6.0, 0.0}, {-1.0, -2.0, 0.0}, {1.0, -2.0, 0.0}, {3.0, -6.0, 0.0}, {6.0, 0.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, 0.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(-3.0000000000000004, -1.854101966249685, 0.0), Point3d(-3.0, -6.0, 0.0), Point3d(-1.0, -2.0, 0.0),
                                  Point3d(-1.6180339887498951, -1.0000000000000002, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-1.6180339887498951, -1.0000000000000002, 0.0), Point3d(-1.0, -2.0, 0.0), Point3d(1.0, -2.0, 0.0),
                                  Point3d(1.6180339887498951, -1.0000000000000002, 0.0)});
  expectedRoofPolygons.push_back({Point3d(3.0000000000000004, -1.854101966249685, 0.0), Point3d(1.6180339887498951, -1.0000000000000002, 0.0),
                                  Point3d(1.0, -2.0, 0.0), Point3d(3.0, -6.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(3.0000000000000004, -1.854101966249685, 0.0), Point3d(3.0, -6.0, 0.0), Point3d(6.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-3.0000000000000004, -1.854101966249685, 0.0), Point3d(-1.6180339887498951, -1.0000000000000002, 0.0),
                                  Point3d(1.6180339887498951, -1.0000000000000002, 0.0), Point3d(3.0000000000000004, -1.854101966249685, 0.0),
                                  Point3d(6.0, 0.0, 0.0), Point3d(-6.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-3.0000000000000004, -1.854101966249685, 0.0), Point3d(-6.0, 0.0, 0.0), Point3d(-3.0, -6.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, SkeletonLevelEvents_double_split2) {
  std::vector<Point3d> footprint{
    {-6.0, 0.0, 0.0}, {-3.0, -6.0, 0.0}, {-1.0, -2.0, 0.0}, {0.0, -3.0, 0.0}, {1.0, -2.0, 0.0}, {3.0, -6.0, 0.0}, {6.0, 0.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, 0.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(-3.0000000000000004, -1.854101966249685, 0.0), Point3d(-3.0, -6.0, 0.0), Point3d(-1.0, -2.0, 0.0),
                                  Point3d(-1.2038204263767998, -0.7440019398522527, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-0.0, -1.242640687119285, 0.0), Point3d(-1.2038204263767998, -0.7440019398522527, 0.0),
                                  Point3d(-1.0, -2.0, 0.0), Point3d(0.0, -3.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-0.0, -1.242640687119285, 0.0), Point3d(0.0, -3.0, 0.0), Point3d(1.0, -2.0, 0.0),
                                  Point3d(1.2038204263767998, -0.7440019398522527, 0.0)});
  expectedRoofPolygons.push_back({Point3d(3.0000000000000004, -1.854101966249685, 0.0), Point3d(1.2038204263767998, -0.7440019398522527, 0.0),
                                  Point3d(1.0, -2.0, 0.0), Point3d(3.0, -6.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(3.0000000000000004, -1.854101966249685, 0.0), Point3d(3.0, -6.0, 0.0), Point3d(6.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-3.0000000000000004, -1.854101966249685, 0.0), Point3d(-1.2038204263767998, -0.7440019398522527, 0.0),
                                  Point3d(-0.0, -1.242640687119285, 0.0), Point3d(1.2038204263767998, -0.7440019398522527, 0.0),
                                  Point3d(3.0000000000000004, -1.854101966249685, 0.0), Point3d(6.0, 0.0, 0.0), Point3d(-6.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-3.0000000000000004, -1.854101966249685, 0.0), Point3d(-6.0, 0.0, 0.0), Point3d(-3.0, -6.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonErrorNotHorizontal) {
  std::vector<Point3d> footprint{
    {5.0, 25.0, 1.0},
    {25.0, 25.0, 2.0},
    {25.0, 5.0, 3.0},
    {5.0, 5.0, 4.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, 0);
  EXPECT_EQ(0, roofPolygons.size());
}

TEST_F(GeometryFixture, RoofSkeletonErrorTooFewPoints) {
  std::vector<Point3d> footprint{
    {5.0, 25.0, 0.0},
    {25.0, 25.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, 0);
  EXPECT_EQ(0, roofPolygons.size());
}

TEST_F(GeometryFixture, RoofSkeletonErrorStartEndSame) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {5.0, 25.0, 0.0}, {25.0, 25.0, 0.0}, {25.0, 5.0, 0.0}, {5.0, 5.0, 0.0}, {5.0, 25.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);
  EXPECT_EQ(0, roofPolygons.size());
}

TEST_F(GeometryFixture, RoofSkeletonSquare_6_12) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {5.0, 25.0, 0.0},
    {25.0, 25.0, 0.0},
    {25.0, 5.0, 0.0},
    {5.0, 5.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 5.0), Point3d(25.0, 5.0, 0.0), Point3d(25.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 5.0), Point3d(25.0, 25.0, 0.0), Point3d(5.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 5.0), Point3d(5.0, 25.0, 0.0), Point3d(5.0, 5.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 5.0), Point3d(5.0, 5.0, 0.0), Point3d(25.0, 5.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonSquare_6_12_Gable) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {5.0, 25.0, 0.0},
    {25.0, 25.0, 0.0},
    {25.0, 5.0, 0.0},
    {5.0, 5.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateGableRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(25.0, 15.0, 5.0), Point3d(25.0, 5.0, 0.0), Point3d(25.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 15.0, 5.0), Point3d(25.0, 25.0, 0.0), Point3d(5.0, 25.0, 0.0), Point3d(5.0, 15.0, 5.0)});
  expectedRoofPolygons.push_back({Point3d(5.0, 15.0, 5.0), Point3d(5.0, 25.0, 0.0), Point3d(5.0, 5.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(5.0, 15.0, 5.0), Point3d(5.0, 5.0, 0.0), Point3d(25.0, 5.0, 0.0), Point3d(25.0, 15.0, 5.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonSquare_6_12_Shed_0deg) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {5.0, 25.0, 8.0},
    {25.0, 25.0, 8.0},
    {25.0, 5.0, 8.0},
    {5.0, 5.0, 8.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateShedRoof(footprint, pitch, 0.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 8.0), Point3d(25.0, 5.0, 8.0), Point3d(25.0, 25.0, 18.0), Point3d(5.0, 25.0, 18.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 5.0, 8.0), Point3d(25.0, 25.0, 8.0), Point3d(25.0, 25.0, 18.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 25.0, 8.0), Point3d(5.0, 25.0, 8.0), Point3d(5.0, 25.0, 18.0), Point3d(25.0, 25.0, 18.0)});
  expectedRoofPolygons.push_back({Point3d(5.0, 25.0, 8.0), Point3d(5.0, 5.0, 8.0), Point3d(5.0, 25.0, 18.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonSquare_6_12_Shed_90deg) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {5.0, 25.0, 8.0},
    {25.0, 25.0, 8.0},
    {25.0, 5.0, 8.0},
    {5.0, 5.0, 8.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateShedRoof(footprint, pitch, 90.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 8.0), Point3d(25.0, 5.0, 18.0), Point3d(25.0, 25.0, 18.0), Point3d(5.0, 25.0, 8.0)});
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 8.0), Point3d(25.0, 5.0, 8.0), Point3d(25.0, 5.0, 18.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 5.0, 8.0), Point3d(25.0, 25.0, 8.0), Point3d(25.0, 25.0, 18.0), Point3d(25.0, 5.0, 18.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 25.0, 8.0), Point3d(5.0, 25.0, 8.0), Point3d(25.0, 25.0, 18.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonSquare_6_12_Shed_180deg) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {5.0, 25.0, 8.0},
    {25.0, 25.0, 8.0},
    {25.0, 5.0, 8.0},
    {5.0, 5.0, 8.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateShedRoof(footprint, pitch, 180.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 18.0), Point3d(25.0, 5.0, 18.0), Point3d(25.0, 25.0, 8.0), Point3d(5.0, 25.0, 8.0)});
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 8.0), Point3d(25.0, 5.0, 8.0), Point3d(25.0, 5.0, 18.0), Point3d(5.0, 5.0, 18.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 5.0, 8.0), Point3d(25.0, 25.0, 8.0), Point3d(25.0, 5.0, 18.0)});
  expectedRoofPolygons.push_back({Point3d(5.0, 25.0, 8.0), Point3d(5.0, 5.0, 8.0), Point3d(5.0, 5.0, 18.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonSquare_6_12_Shed_270deg) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {5.0, 25.0, 8.0},
    {25.0, 25.0, 8.0},
    {25.0, 5.0, 8.0},
    {5.0, 5.0, 8.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateShedRoof(footprint, pitch, 270.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 18.0), Point3d(25.0, 5.0, 8.0), Point3d(25.0, 25.0, 8.0), Point3d(5.0, 25.0, 18.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 25.0, 8.0), Point3d(5.0, 25.0, 8.0), Point3d(5.0, 25.0, 18.0)});
  expectedRoofPolygons.push_back({Point3d(5.0, 25.0, 8.0), Point3d(5.0, 5.0, 8.0), Point3d(5.0, 5.0, 18.0), Point3d(5.0, 25.0, 18.0)});
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 8.0), Point3d(25.0, 5.0, 8.0), Point3d(5.0, 5.0, 18.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonSquare_6_12_Shed_45deg) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {5.0, 25.0, 8.0},
    {25.0, 25.0, 8.0},
    {25.0, 5.0, 8.0},
    {5.0, 5.0, 8.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateShedRoof(footprint, pitch, 45.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back(
    {Point3d(5.0, 5.0, 8.0), Point3d(25.0, 5.0, 15.071067812), Point3d(25.0, 25.0, 22.14213562), Point3d(5.0, 25.0, 15.071067812)});
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 8.0), Point3d(25.0, 5.0, 8.0), Point3d(25.0, 5.0, 15.071067812)});
  expectedRoofPolygons.push_back(
    {Point3d(25.0, 5.0, 8.0), Point3d(25.0, 25.0, 8.0), Point3d(25.0, 25.0, 22.14213562), Point3d(25.0, 5.0, 15.071067812)});
  expectedRoofPolygons.push_back(
    {Point3d(25.0, 25.0, 8.0), Point3d(5.0, 25.0, 8.0), Point3d(5.0, 25.0, 15.071067812), Point3d(25.0, 25.0, 22.14213562)});
  expectedRoofPolygons.push_back({Point3d(5.0, 25.0, 8.0), Point3d(5.0, 5.0, 8.0), Point3d(5.0, 25.0, 15.071067812)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonSquare_1_12) {
  double pitch = radToDeg(atan(1.0 / 12.0));

  std::vector<Point3d> footprint{
    {5.0, 25.0, 0.0},
    {25.0, 25.0, 0.0},
    {25.0, 5.0, 0.0},
    {5.0, 5.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 0.833333), Point3d(25.0, 5.0, 0.0), Point3d(25.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 0.833333), Point3d(25.0, 25.0, 0.0), Point3d(5.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 0.833333), Point3d(5.0, 25.0, 0.0), Point3d(5.0, 5.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 0.833333), Point3d(5.0, 5.0, 0.0), Point3d(25.0, 5.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonRectangle_3_12) {
  double pitch = radToDeg(atan(3.0 / 12.0));

  std::vector<Point3d> footprint{
    {5.0, 25.0, 0.0},
    {40.0, 25.0, 0.0},
    {40.0, 5.0, 0.0},
    {5.0, 5.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(30.0, 15.0, 2.5), Point3d(40.0, 5.0, 0.0), Point3d(40.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(30.0, 15.0, 2.5), Point3d(40.0, 25.0, 0.0), Point3d(5.0, 25.0, 0.0), Point3d(15.0, 15.0, 2.5)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 2.5), Point3d(5.0, 25.0, 0.0), Point3d(5.0, 5.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(15.0, 15.0, 2.5), Point3d(5.0, 5.0, 0.0), Point3d(40.0, 5.0, 0.0), Point3d(30.0, 15.0, 2.5)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonQuadrilateral_6_12) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {10.0, 10.0, 0.0},
    {-20.0, 20.0, 0.0},
    {-30.0, 10.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(-1.7082039324993694, 7.23606797749979, 3.1622776601683795), Point3d(10.0, 10.0, 0.0),
                                  Point3d(-20.0, 20.0, 0.0), Point3d(-18.29179606750063, 12.76393202250021, 3.1622776601683795)});
  expectedRoofPolygons.push_back(
    {Point3d(-18.29179606750063, 12.76393202250021, 3.1622776601683795), Point3d(-20.0, 20.0, 0.0), Point3d(-30.0, 10.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-18.29179606750063, 12.76393202250021, 3.1622776601683795), Point3d(-30.0, 10.0, 0.0),
                                  Point3d(0.0, 0.0, 0.0), Point3d(-1.7082039324993694, 7.23606797749979, 3.1622776601683795)});
  expectedRoofPolygons.push_back(
    {Point3d(-1.7082039324993694, 7.23606797749979, 3.1622776601683795), Point3d(0.0, 0.0, 0.0), Point3d(10.0, 10.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonQuadrilateral_6_12_Gable) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {10.0, 10.0, 0.0},
    {-20.0, 20.0, 0.0},
    {-30.0, 10.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateGableRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back(
    {Point3d(5.0, 5.0, 3.1622776601683795), Point3d(10.0, 10.0, 0.0), Point3d(-20.0, 20.0, 0.0), Point3d(-25.0, 15.0, 3.1622776601683795)});
  expectedRoofPolygons.push_back({Point3d(-25.0, 15.0, 3.1622776601683795), Point3d(-20.0, 20.0, 0.0), Point3d(-30.0, 10.0, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-25.0, 15.0, 3.1622776601683795), Point3d(-30.0, 10.0, 0.0), Point3d(0.0, 0.0, 0.0), Point3d(5.0, 5.0, 3.1622776601683795)});
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 3.1622776601683795), Point3d(0.0, 0.0, 0.0), Point3d(10.0, 10.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonHshape_3_12) {
  double pitch = radToDeg(atan(3.0 / 12.0));

  std::vector<Point3d> footprint{
    {0, 25, 0}, {10, 25, 0}, {10, 18, 0}, {20, 18, 0}, {20, 25, 0}, {30, 25, 0},
    {30, 5, 0}, {20, 5, 0},  {20, 12, 0}, {10, 12, 0}, {10, 5, 0},  {0, 5, 0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(5.0, 10.0, 1.25), Point3d(10.0, 5.0, 0.0), Point3d(10.0, 12.0, 0.0), Point3d(7.0, 15.0, 0.75)});
  expectedRoofPolygons.push_back({Point3d(7.0, 15.0, 0.75), Point3d(10.0, 12.0, 0.0), Point3d(20.0, 12.0, 0.0), Point3d(23.0, 15.0, 0.75)});
  expectedRoofPolygons.push_back({Point3d(23.0, 15.0, 0.75), Point3d(20.0, 12.0, 0.0), Point3d(20.0, 5.0, 0.0), Point3d(25.0, 10.0, 1.25)});
  expectedRoofPolygons.push_back({Point3d(25.0, 10.0, 1.25), Point3d(20.0, 5.0, 0.0), Point3d(30.0, 5.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 10.0, 1.25), Point3d(30.0, 5.0, 0.0), Point3d(30.0, 25.0, 0.0), Point3d(25.0, 20.0, 1.25)});
  expectedRoofPolygons.push_back({Point3d(25.0, 20.0, 1.25), Point3d(30.0, 25.0, 0.0), Point3d(20.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 20.0, 1.25), Point3d(20.0, 25.0, 0.0), Point3d(20.0, 18.0, 0.0), Point3d(23.0, 15.0, 0.75)});
  expectedRoofPolygons.push_back({Point3d(23.0, 15.0, 0.75), Point3d(20.0, 18.0, 0.0), Point3d(10.0, 18.0, 0.0), Point3d(7.0, 15.0, 0.75)});
  expectedRoofPolygons.push_back({Point3d(7.0, 15.0, 0.75), Point3d(10.0, 18.0, 0.0), Point3d(10.0, 25.0, 0.0), Point3d(5.0, 20.0, 1.25)});
  expectedRoofPolygons.push_back({Point3d(5.0, 20.0, 1.25), Point3d(10.0, 25.0, 0.0), Point3d(0.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(5.0, 20.0, 1.25), Point3d(0.0, 25.0, 0.0), Point3d(0.0, 5.0, 0.0), Point3d(5.0, 10.0, 1.25)});
  expectedRoofPolygons.push_back({Point3d(5.0, 10.0, 1.25), Point3d(0.0, 5.0, 0.0), Point3d(10.0, 5.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonHshape_3_12_Gable) {
  double pitch = radToDeg(atan(3.0 / 12.0));

  std::vector<Point3d> footprint{
    {0, 25, 0}, {10, 25, 0}, {10, 18, 0}, {20, 18, 0}, {20, 25, 0}, {30, 25, 0},
    {30, 5, 0}, {20, 5, 0},  {20, 12, 0}, {10, 12, 0}, {10, 5, 0},  {0, 5, 0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateGableRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 1.25), Point3d(10.0, 5.0, 0.0), Point3d(10.0, 12.0, 0.0), Point3d(7.0, 15.0, 0.75)});
  expectedRoofPolygons.push_back({Point3d(7.0, 15.0, 0.75), Point3d(10.0, 12.0, 0.0), Point3d(20.0, 12.0, 0.0), Point3d(23.0, 15.0, 0.75)});
  expectedRoofPolygons.push_back({Point3d(23.0, 15.0, 0.75), Point3d(20.0, 12.0, 0.0), Point3d(20.0, 5.0, 0.0), Point3d(25.0, 5.0, 1.25)});
  expectedRoofPolygons.push_back({Point3d(25.0, 5.0, 1.25), Point3d(20.0, 5.0, 0.0), Point3d(30.0, 5.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 5.0, 1.25), Point3d(30.0, 5.0, 0.0), Point3d(30.0, 25.0, 0.0), Point3d(25.0, 25.0, 1.25)});
  expectedRoofPolygons.push_back({Point3d(25.0, 25.0, 1.25), Point3d(30.0, 25.0, 0.0), Point3d(20.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(25.0, 25.0, 1.25), Point3d(20.0, 25.0, 0.0), Point3d(20.0, 18.0, 0.0), Point3d(23.0, 15.0, 0.75)});
  expectedRoofPolygons.push_back({Point3d(23.0, 15.0, 0.75), Point3d(20.0, 18.0, 0.0), Point3d(10.0, 18.0, 0.0), Point3d(7.0, 15.0, 0.75)});
  expectedRoofPolygons.push_back({Point3d(7.0, 15.0, 0.75), Point3d(10.0, 18.0, 0.0), Point3d(10.0, 25.0, 0.0), Point3d(5.0, 25.0, 1.25)});
  expectedRoofPolygons.push_back({Point3d(5.0, 25.0, 1.25), Point3d(10.0, 25.0, 0.0), Point3d(0.0, 25.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(5.0, 25.0, 1.25), Point3d(0.0, 25.0, 0.0), Point3d(0.0, 5.0, 0.0), Point3d(5.0, 5.0, 1.25)});
  expectedRoofPolygons.push_back({Point3d(5.0, 5.0, 1.25), Point3d(0.0, 5.0, 0.0), Point3d(10.0, 5.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonHshape_3_12_Shed_0deg) {
  double pitch = radToDeg(atan(3.0 / 12.0));

  std::vector<Point3d> footprint{
    {0, 25, 0}, {10, 25, 0}, {10, 18, 0}, {20, 18, 0}, {20, 25, 0}, {30, 25, 0},
    {30, 5, 0}, {20, 5, 0},  {20, 12, 0}, {10, 12, 0}, {10, 5, 0},  {0, 5, 0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateShedRoof(footprint, pitch, 0.0);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0, 5, 0), Point3d(10, 5, 0), Point3d(10, 12, 1.75), Point3d(20, 12, 1.75), Point3d(20, 5, 0),
                                  Point3d(30, 5, 0), Point3d(30, 25, 5), Point3d(20, 25, 5), Point3d(20, 18, 3.25), Point3d(10, 18, 3.25),
                                  Point3d(10, 25, 5), Point3d(0, 25, 5)});
  expectedRoofPolygons.push_back({Point3d(10, 5, 0), Point3d(10, 12, 0), Point3d(10, 12, 1.75)});
  expectedRoofPolygons.push_back({Point3d(10, 12, 0), Point3d(20, 12, 0), Point3d(20, 12, 1.75), Point3d(10, 12, 1.75)});
  expectedRoofPolygons.push_back({Point3d(20, 12, 0), Point3d(20, 5, 0), Point3d(20, 12, 1.75)});
  expectedRoofPolygons.push_back({Point3d(30, 5, 0), Point3d(30, 25, 0), Point3d(30, 25, 5)});
  expectedRoofPolygons.push_back({Point3d(30, 25, 0), Point3d(20, 25, 0), Point3d(20, 25, 5), Point3d(30, 25, 5)});
  expectedRoofPolygons.push_back({Point3d(20, 25, 0), Point3d(20, 18, 0), Point3d(20, 18, 3.25), Point3d(20, 25, 5)});
  expectedRoofPolygons.push_back({Point3d(20, 18, 0), Point3d(10, 18, 0), Point3d(10, 18, 3.25), Point3d(20, 18, 3.25)});
  expectedRoofPolygons.push_back({Point3d(10, 18, 0), Point3d(10, 25, 0), Point3d(10, 25, 5), Point3d(10, 18, 3.25)});
  expectedRoofPolygons.push_back({Point3d(10, 25, 0), Point3d(0, 25, 0), Point3d(0, 25, 5), Point3d(10, 25, 5)});
  expectedRoofPolygons.push_back({Point3d(0, 25, 0), Point3d(0, 5, 0), Point3d(0, 25, 5)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTest6_9) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {119.0, 158.0, 0.0}, {259.0, 159.0, 0.0}, {248.0, 63.0, 0.0}, {126.0, 60.0, 0.0}, {90.0, 106.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(149.3227702994274, 109.40180634780967, 24.406769820569707), Point3d(126.0, 60.0, 0.0),
                                  Point3d(248.0, 63.0, 0.0), Point3d(204.77153555171768, 110.28151783497142, 24.16495155021603)});
  expectedRoofPolygons.push_back(
    {Point3d(204.77153555171768, 110.28151783497142, 24.16495155021603), Point3d(248.0, 63.0, 0.0), Point3d(259.0, 159.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(149.3227702994274, 109.40180634780967, 24.406769820569707),
                                  Point3d(204.77153555171768, 110.28151783497142, 24.16495155021603), Point3d(259.0, 159.0, 0.0),
                                  Point3d(119.0, 158.0, 0.0), Point3d(147.1566715713749, 110.4476269750399, 23.876136976448187)});
  expectedRoofPolygons.push_back(
    {Point3d(147.1566715713749, 110.4476269750399, 23.876136976448187), Point3d(119.0, 158.0, 0.0), Point3d(90.0, 106.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(149.3227702994274, 109.40180634780967, 24.406769820569707),
                                  Point3d(147.1566715713749, 110.4476269750399, 23.876136976448187), Point3d(90.0, 106.0, 0.0),
                                  Point3d(126.0, 60.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTest7) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {1.0, -1.0, 0.0}, {1.0, 1.0, 0.0}, {-1.0, 1.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.5, -0.5, 0.25), Point3d(0.0, -1.0, 0.0), Point3d(1.0, -1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.4142135623730951, 0.4142135623730951, 0.2928932188134524), Point3d(0.5, 0.20710678118654752, 0.25),
                                  Point3d(0.5, -0.5, 0.25), Point3d(1.0, -1.0, 0.0), Point3d(1.0, 1.0, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(0.4142135623730951, 0.4142135623730951, 0.2928932188134524), Point3d(1.0, 1.0, 0.0), Point3d(-1.0, 1.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.4142135623730951, 0.4142135623730951, 0.2928932188134524), Point3d(-1.0, 1.0, 0.0),
                                  Point3d(0.0, 0.0, 0.0), Point3d(0.5, 0.20710678118654752, 0.25)});
  expectedRoofPolygons.push_back(
    {Point3d(0.5, 0.20710678118654752, 0.25), Point3d(0.0, 0.0, 0.0), Point3d(0.0, -1.0, 0.0), Point3d(0.5, -0.5, 0.25)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTest8) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {-1.0, 0.0, 0.0}, {-1.2, -2.0, 0.0}, {1.2, -2.0, 0.0}, {1.0, 0.5, 0.0}, {2.0, -0.2, 0.0}, {2.0, 1.0, 0.0}, {-2.0, 1.2, 0.0}, {-2.0, -0.2, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back(
    {Point3d(0.011951105288885877, -0.90319932382556, 0.54840033808722), Point3d(-1.2, -2.0, 0.0), Point3d(1.2, -2.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.011951105288885877, -0.90319932382556, 0.54840033808722), Point3d(1.2, -2.0, 0.0), Point3d(1.0, 0.5, 0.0),
                                  Point3d(0.7847636613911987, 0.8759615123236527, 0.09228486822568349),
                                  Point3d(0.02180193069397529, 0.08986159383373349, 0.5038946796426319)});
  expectedRoofPolygons.push_back({Point3d(1.582158848784161, 0.6025289309369212, 0.20892057560791943),
                                  Point3d(0.7847636613911987, 0.8759615123236527, 0.09228486822568349), Point3d(1.0, 0.5, 0.0),
                                  Point3d(2.0, -0.2, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(1.582158848784161, 0.6025289309369212, 0.20892057560791943), Point3d(2.0, -0.2, 0.0), Point3d(2.0, 1.0, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(0.02180193069397529, 0.08986159383373349, 0.5038946796426319), Point3d(0.7847636613911987, 0.8759615123236527, 0.09228486822568349),
     Point3d(1.582158848784161, 0.6025289309369212, 0.20892057560791943), Point3d(2.0, 1.0, 0.0), Point3d(-2.0, 1.2, 0.0),
     Point3d(-1.3835459038079945, 0.5519531123818496, 0.30822704809600276), Point3d(-0.43606459624258886, 0.6219268927095002, 0.24962633041547042)});
  expectedRoofPolygons.push_back(
    {Point3d(-1.3835459038079945, 0.5519531123818496, 0.30822704809600276), Point3d(-2.0, 1.2, 0.0), Point3d(-2.0, -0.2, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-1.3835459038079945, 0.5519531123818496, 0.30822704809600276), Point3d(-2.0, -0.2, 0.0),
                                  Point3d(-1.0, 0.0, 0.0), Point3d(-0.43606459624258886, 0.6219268927095002, 0.24962633041547042)});
  expectedRoofPolygons.push_back(
    {Point3d(0.011951105288885877, -0.90319932382556, 0.54840033808722), Point3d(0.02180193069397529, 0.08986159383373349, 0.5038946796426319),
     Point3d(-0.43606459624258886, 0.6219268927095002, 0.24962633041547042), Point3d(-1.0, 0.0, 0.0), Point3d(-1.2, -2.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB1) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {0.7904833761575505, 8.520486967634694, 0.0},
    {5.978418789681697, 8.712497973454056, 0.0},
    {5.95269105167549, -2.6355979260267777, 0.0},
    {4.566910029680516, -2.6324561649763485, 0.0},
    {4.5603585630377115, -5.522203838861205, 0.0},
    {6.043569207647302, -5.525566487736131, 0.0},
    {6.038049999411376, -7.960001358506733, 0.0},
    {9.886846028372108, -7.968727126586532, 0.0},
    {9.902081573281308, -1.248570683335708, 0.0},
    {13.742215004880482, -1.2572768087753285, 0.0},
    {13.75400717659087, 3.9440624000165103, 0.0},
    {9.194585721152315, 3.9543992526769878, 0.0},
    {5.823717342947504, 17.30434988614582, 0.0},
    {5.808494957384097, 10.589997844496661, 0.0},
    {-0.13214359029800526, 10.603466113057067, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back(
    {Point3d(0.9184708676614719, 9.563507938168502, 0.5187868083298253), Point3d(0.3594527136710186, 8.976136170014422, 0.23563971612419035),
     Point3d(0.7904833761575505, 8.520486967634694, 0.0), Point3d(5.978418789681697, 8.712497973454056, 0.0),
     Point3d(6.8580714064192705, 9.623241290520603, 0.4387927887494017), Point3d(6.760641991208537, 9.69627296507374, 0.47708539116320264),
     Point3d(6.729880870203986, 9.664424613978154, 0.4617409724533214)});
  expectedRoofPolygons.push_back(
    {Point3d(8.562421611757856, 1.3551494866221112, 1.3003381438870851), Point3d(7.779411083575811, 2.14171847341904, 0.907942256682172),
     Point3d(6.8580714064192705, 9.623241290520603, 0.4387927887494017), Point3d(5.978418789681697, 8.712497973454056, 0.0),
     Point3d(5.95269105167549, -2.6355979260267777, 0.0), Point3d(7.394289156400893, -4.083747496844417, 0.7224387756441801),
     Point3d(7.923725736553589, -3.55670610186642, 0.9865589482591836), Point3d(7.933442073357339, 0.7290154681198144, 0.9865589482644922)});
  expectedRoofPolygons.push_back({Point3d(7.394289156400893, -4.083747496844417, 0.7224387756441801),
                                  Point3d(5.95269105167549, -2.6355979260267777, 0.0), Point3d(4.566910029680516, -2.6324561649763485, 0.0),
                                  Point3d(6.008508133850388, -4.080605735266273, 0.722438775380953)});
  expectedRoofPolygons.push_back({Point3d(6.008508133850388, -4.080605735266273, 0.722438775380953),
                                  Point3d(4.566910029680516, -2.6324561649763485, 0.0), Point3d(4.5603585630377115, -5.522203838861205, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(7.923725736553589, -3.55670610186642, 0.9865589482591836), Point3d(7.394289156400893, -4.083747496844417, 0.7224387756441801),
     Point3d(6.008508133850388, -4.080605735266273, 0.722438775380953), Point3d(4.5603585630377115, -5.522203838861205, 0.0),
     Point3d(6.043569207647302, -5.525566487736131, 0.0), Point3d(7.972330104528959, -3.6055313565934526, 0.9622014799910014)});
  expectedRoofPolygons.push_back({Point3d(7.972330104528959, -3.6055313565934526, 0.9622014799910014),
                                  Point3d(6.043569207647302, -5.525566487736131, 0.0), Point3d(6.038049999411376, -7.960001358506733, 0.0),
                                  Point3d(7.966810896364689, -6.039966228120712, 0.9622014800276865)});
  expectedRoofPolygons.push_back({Point3d(7.966810896364689, -6.039966228120712, 0.9622014800276865),
                                  Point3d(6.038049999411376, -7.960001358506733, 0.0), Point3d(9.886846028372108, -7.968727126586532, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(7.923725736553589, -3.55670610186642, 0.9865589482591836), Point3d(7.972330104528959, -3.6055313565934526, 0.9622014799910014),
     Point3d(7.966810896364689, -6.039966228120712, 0.9622014800276865), Point3d(9.886846028372108, -7.968727126586532, 0.0),
     Point3d(9.902081573281308, -1.248570683335708, 0.0), Point3d(7.933442073357339, 0.7290154681198144, 0.9865589482644922)});
  expectedRoofPolygons.push_back({Point3d(11.147441486461226, 1.3492888811957957, 1.3003381439477693),
                                  Point3d(8.562421611757856, 1.3551494866221112, 1.3003381438870851),
                                  Point3d(7.933442073357339, 0.7290154681198144, 0.9865589482644922),
                                  Point3d(9.902081573281308, -1.248570683335708, 0.0), Point3d(13.742215004880482, -1.2572768087753285, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.147441486461226, 1.3492888811957957, 1.3003381439477693),
                                  Point3d(13.742215004880482, -1.2572768087753285, 0.0), Point3d(13.75400717659087, 3.9440624000165103, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.147441486461226, 1.3492888811957957, 1.3003381439477693),
                                  Point3d(13.75400717659087, 3.9440624000165103, 0.0), Point3d(9.194585721152315, 3.9543992526769878, 0.0),
                                  Point3d(7.779411083575811, 2.14171847341904, 0.907942256682172),
                                  Point3d(8.562421611757856, 1.3551494866221112, 1.3003381438870851)});
  expectedRoofPolygons.push_back({Point3d(7.779411083575811, 2.14171847341904, 0.907942256682172),
                                  Point3d(9.194585721152315, 3.9543992526769878, 0.0), Point3d(5.823717342947504, 17.30434988614582, 0.0),
                                  Point3d(6.760641991208537, 9.69627296507374, 0.47708539116320264),
                                  Point3d(6.8580714064192705, 9.623241290520603, 0.4387927887494017)});
  expectedRoofPolygons.push_back({Point3d(6.760641991208537, 9.69627296507374, 0.47708539116320264),
                                  Point3d(5.823717342947504, 17.30434988614582, 0.0), Point3d(5.808494957384097, 10.589997844496661, 0.0),
                                  Point3d(6.729880870203986, 9.664424613978154, 0.4617409724533214)});
  expectedRoofPolygons.push_back({Point3d(0.9184708676614719, 9.563507938168502, 0.5187868083298253),
                                  Point3d(6.729880870203986, 9.664424613978154, 0.4617409724533214),
                                  Point3d(5.808494957384097, 10.589997844496661, 0.0), Point3d(-0.13214359029800526, 10.603466113057067, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.9184708676614719, 9.563507938168502, 0.5187868083298253),
                                  Point3d(-0.13214359029800526, 10.603466113057067, 0.0), Point3d(0.0, 0.0, 0.0),
                                  Point3d(0.3594527136710186, 8.976136170014422, 0.23563971612419035)});
  expectedRoofPolygons.push_back({Point3d(0.3594527136710186, 8.976136170014422, 0.23563971612419035), Point3d(0.0, 0.0, 0.0),
                                  Point3d(0.7904833761575505, 8.520486967634694, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB2) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {0.7904833761549828, 8.520486967607015, 0.0},
    {5.9784187896622765, 8.712497973425755, 0.0},
    {5.952691051656153, -2.6355979260182156, 0.0},
    {4.56691002966568, -2.632456164967797, 0.0},
    {4.560358563022897, -5.522203838843264, 0.0},
    {6.0435692076276695, -5.525566487718182, 0.0},
    {6.038049999391761, -7.960001358480875, 0.0},
    {9.886846028339992, -7.968727126560646, 0.0},
    {9.902081573249141, -1.2485706833316517, 0.0},
    {13.74221500483584, -1.2572768087712447, 0.0},
    {13.754007176546189, 3.944062400003698, 0.0},
    {9.194585721122445, 3.9543992526641416, 0.0},
    {9.840828592998651, 10.391220834155359, 0.0},
    {-0.24573045314637643, 10.433085818392197, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.7321423860140842, 9.474000222038036, 0.47750931955493864),
                                  Point3d(0.3113773466011168, 9.026957386584131, 0.2619219898328897),
                                  Point3d(0.7904833761549828, 8.520486967607015, 0.0), Point3d(5.9784187896622765, 8.712497973425755, 0.0),
                                  Point3d(6.810341355495798, 9.573824254656804, 0.41498384218932366)});
  expectedRoofPolygons.push_back(
    {Point3d(8.562421611730041, 1.3551494866177085, 1.3003381438828603), Point3d(7.498680114283213, 2.4237252486872443, 0.7672574589826884),
     Point3d(7.813149324955553, 8.56455983950241, 0.9175306079619708), Point3d(6.810341355495798, 9.573824254656804, 0.41498384218932366),
     Point3d(5.9784187896622765, 8.712497973425755, 0.0), Point3d(5.952691051656153, -2.6355979260182156, 0.0),
     Point3d(7.394289156376876, -4.083747496831154, 0.7224387756418331), Point3d(7.9237257365278495, -3.5567061018548705, 0.9865589482559782),
     Point3d(7.933442073331568, 0.7290154681174473, 0.9865589482612875)});
  expectedRoofPolygons.push_back({Point3d(7.394289156376876, -4.083747496831154, 0.7224387756418331),
                                  Point3d(5.952691051656153, -2.6355979260182156, 0.0), Point3d(4.56691002966568, -2.632456164967797, 0.0),
                                  Point3d(6.008508133830869, -4.080605735253016, 0.7224387753786058)});
  expectedRoofPolygons.push_back({Point3d(6.008508133830869, -4.080605735253016, 0.7224387753786058),
                                  Point3d(4.56691002966568, -2.632456164967797, 0.0), Point3d(4.560358563022897, -5.522203838843264, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(7.9237257365278495, -3.5567061018548705, 0.9865589482559782), Point3d(7.394289156376876, -4.083747496831154, 0.7224387756418331),
     Point3d(6.008508133830869, -4.080605735253016, 0.7224387753786058), Point3d(4.560358563022897, -5.522203838843264, 0.0),
     Point3d(6.0435692076276695, -5.525566487718182, 0.0), Point3d(7.972330104503062, -3.6055313565817433, 0.9622014799878755)});
  expectedRoofPolygons.push_back({Point3d(7.972330104503062, -3.6055313565817433, 0.9622014799878755),
                                  Point3d(6.0435692076276695, -5.525566487718182, 0.0), Point3d(6.038049999391761, -7.960001358480875, 0.0),
                                  Point3d(7.966810896338812, -6.039966228101093, 0.962201480024562)});
  expectedRoofPolygons.push_back({Point3d(7.966810896338812, -6.039966228101093, 0.962201480024562),
                                  Point3d(6.038049999391761, -7.960001358480875, 0.0), Point3d(9.886846028339992, -7.968727126560646, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(7.9237257365278495, -3.5567061018548705, 0.9865589482559782), Point3d(7.972330104503062, -3.6055313565817433, 0.9622014799878755),
     Point3d(7.966810896338812, -6.039966228101093, 0.962201480024562), Point3d(9.886846028339992, -7.968727126560646, 0.0),
     Point3d(9.902081573249141, -1.2485706833316517, 0.0), Point3d(7.933442073331568, 0.7290154681174473, 0.9865589482612875)});
  expectedRoofPolygons.push_back({Point3d(11.147441486425013, 1.3492888811914148, 1.3003381439435455),
                                  Point3d(8.562421611730041, 1.3551494866177085, 1.3003381438828603),
                                  Point3d(7.933442073331568, 0.7290154681174473, 0.9865589482612875),
                                  Point3d(9.902081573249141, -1.2485706833316517, 0.0), Point3d(13.74221500483584, -1.2572768087712447, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.147441486425013, 1.3492888811914148, 1.3003381439435455),
                                  Point3d(13.74221500483584, -1.2572768087712447, 0.0), Point3d(13.754007176546189, 3.944062400003698, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.147441486425013, 1.3492888811914148, 1.3003381439435455),
                                  Point3d(13.754007176546189, 3.944062400003698, 0.0), Point3d(9.194585721122445, 3.9543992526641416, 0.0),
                                  Point3d(7.498680114283213, 2.4237252486872443, 0.7672574589826884),
                                  Point3d(8.562421611730041, 1.3551494866177085, 1.3003381438828603)});
  expectedRoofPolygons.push_back({Point3d(7.813149324955553, 8.56455983950241, 0.9175306079619708),
                                  Point3d(7.498680114283213, 2.4237252486872443, 0.7672574589826884),
                                  Point3d(9.194585721122445, 3.9543992526641416, 0.0), Point3d(9.840828592998651, 10.391220834155359, 0.0)});
  expectedRoofPolygons.push_back({Point3d(7.813149324955553, 8.56455983950241, 0.9175306079619708),
                                  Point3d(9.840828592998651, 10.391220834155359, 0.0), Point3d(-0.24573045314637643, 10.433085818392197, 0.0),
                                  Point3d(0.7321423860140842, 9.474000222038036, 0.47750931955493864),
                                  Point3d(6.810341355495798, 9.573824254656804, 0.41498384218932366)});
  expectedRoofPolygons.push_back({Point3d(0.7321423860140842, 9.474000222038036, 0.47750931955493864),
                                  Point3d(-0.24573045314637643, 10.433085818392197, 0.0), Point3d(0.0, 0.0, 0.0),
                                  Point3d(0.3113773466011168, 9.026957386584131, 0.2619219898328897)});
  expectedRoofPolygons.push_back({Point3d(0.3113773466011168, 9.026957386584131, 0.2619219898328897), Point3d(0.0, 0.0, 0.0),
                                  Point3d(0.7904833761549828, 8.520486967607015, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB3) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {0.0853589477356087, -5.32440343246266, 0.0},
    {3.934154976683839, -5.33312920054243, 0.0},
    {3.9493905215929885, 1.387027242686564, 0.0},
    {7.789523953179687, 1.378321117246971, 0.0},
    {3.2418946694662925, 6.589997178682357, 0.0},
    {-0.4480081827933864, 6.565094698194268, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.996553519848494, -3.386722145559448, 0.9710046229839351),
                                  Point3d(0.0853589477356087, -5.32440343246266, 0.0), Point3d(3.934154976683839, -5.33312920054243, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.860584700471444, 3.4853258215219385, 1.0467787901715908),
                                  Point3d(1.9726755710182728, 0.08306472324690238, 0.9868768053436059),
                                  Point3d(1.996553519848494, -3.386722145559448, 0.9710046229839351),
                                  Point3d(3.934154976683839, -5.33312920054243, 0.0), Point3d(3.9493905215929885, 1.387027242686564, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.25187865095761, 3.9032812980229954, 1.2561995495738625),
                                  Point3d(1.860584700471444, 3.4853258215219385, 1.0467787901715908),
                                  Point3d(3.9493905215929885, 1.387027242686564, 0.0), Point3d(7.789523953179687, 1.378321117246971, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.25187865095761, 3.9032812980229954, 1.2561995495738625),
                                  Point3d(7.789523953179687, 1.378321117246971, 0.0), Point3d(3.2418946694662925, 6.589997178682357, 0.0),
                                  Point3d(2.1462783279814595, 4.158151806596191, 1.2121980233450986)});
  expectedRoofPolygons.push_back({Point3d(2.1462783279814595, 4.158151806596191, 1.2121980233450986),
                                  Point3d(3.2418946694662925, 6.589997178682357, 0.0), Point3d(-0.4480081827933864, 6.565094698194268, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(2.25187865095761, 3.9032812980229954, 1.2561995495738625), Point3d(2.1462783279814595, 4.158151806596191, 1.2121980233450986),
     Point3d(-0.4480081827933864, 6.565094698194268, 0.0), Point3d(0.0, 0.0, 0.0),
     Point3d(1.9726755710182728, 0.08306472324690238, 0.9868768053436059), Point3d(1.860584700471444, 3.4853258215219385, 1.0467787901715908)});
  expectedRoofPolygons.push_back({Point3d(1.9726755710182728, 0.08306472324690238, 0.9868768053436059), Point3d(0.0, 0.0, 0.0),
                                  Point3d(0.0853589477356087, -5.32440343246266, 0.0),
                                  Point3d(1.996553519848494, -3.386722145559448, 0.9710046229839351)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB4) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {-1.192493260706565, -5.6367673060470285, 0.0},
    {2.656302768241665, -5.645493074126799, 0.0},
    {6.511671744737513, 1.0659572436626021, 0.0},
    {-1.7258603912355601, 6.252730824609899, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.4279119335787651, -3.517645076047145, 1.0625288000004602),
                                  Point3d(-1.192493260706565, -5.6367673060470285, 0.0), Point3d(2.656302768241665, -5.645493074126799, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.812172737637799, 0.14602644606810766, 1.3748294820465696),
                                  Point3d(2.804480151931106, 0.08532357949119695, 1.3630462913156411),
                                  Point3d(1.4279119335787651, -3.517645076047145, 1.0625288000004602),
                                  Point3d(2.656302768241665, -5.645493074126799, 0.0), Point3d(6.511671744737513, 1.0659572436626021, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.812172737637799, 0.14602644606810766, 1.3748294820465696),
                                  Point3d(6.511671744737513, 1.0659572436626021, 0.0), Point3d(-1.7258603912355601, 6.252730824609899, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.812172737637799, 0.14602644606810766, 1.3748294820465696),
                                  Point3d(-1.7258603912355601, 6.252730824609899, 0.0), Point3d(0.0, 0.0, 0.0),
                                  Point3d(2.804480151931106, 0.08532357949119695, 1.3630462913156411)});
  expectedRoofPolygons.push_back({Point3d(2.804480151931106, 0.08532357949119695, 1.3630462913156411), Point3d(0.0, 0.0, 0.0),
                                  Point3d(-1.192493260706565, -5.6367673060470285, 0.0),
                                  Point3d(1.4279119335787651, -3.517645076047145, 1.0625288000004602)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB5) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {-1.192493260706565, -5.6367673060470285, 0.0},
    {2.656302768241665, -5.645493074126799, 0.0},
    {7.051209343876594, 2.9401404828825903, 0.0},
    {-1.7258603912355601, 6.252730824609899, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.381369491592734, -3.555283958639109, 1.0436566480369565),
                                  Point3d(-1.192493260706565, -5.6367673060470285, 0.0), Point3d(2.656302768241665, -5.645493074126799, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.795364974222883, 1.2972941167706578, 1.5198861728400568),
                                  Point3d(2.6710186548727584, 0.08126314331894204, 1.2981807231020268),
                                  Point3d(1.381369491592734, -3.555283958639109, 1.0436566480369565),
                                  Point3d(2.656302768241665, -5.645493074126799, 0.0), Point3d(7.051209343876594, 2.9401404828825903, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.795364974222883, 1.2972941167706578, 1.5198861728400568),
                                  Point3d(7.051209343876594, 2.9401404828825903, 0.0), Point3d(-1.7258603912355601, 6.252730824609899, 0.0)});
  expectedRoofPolygons.push_back({Point3d(2.795364974222883, 1.2972941167706578, 1.5198861728400568),
                                  Point3d(-1.7258603912355601, 6.252730824609899, 0.0), Point3d(0.0, 0.0, 0.0),
                                  Point3d(2.6710186548727584, 0.08126314331894204, 1.2981807231020268)});
  expectedRoofPolygons.push_back({Point3d(2.6710186548727584, 0.08126314331894204, 1.2981807231020268), Point3d(0.0, 0.0, 0.0),
                                  Point3d(-1.192493260706565, -5.6367673060470285, 0.0),
                                  Point3d(1.381369491592734, -3.555283958639109, 1.0436566480369565)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB6) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {-1.192493260706565, -5.636767306047028, 0.0},
    {2.656302768241665, -5.645493074126798, 0.0},
    {5.716563703938576, 6.120572646649897, 0.0},
    {-5.985367752852362, 6.423111118668768, 0.0},
    {-6.297731626436729, -3.6293262553813097, 0.0},
    {-3.4580600517873807, 1.3968924313579514, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.1735930214222696, -3.723312833627361, 0.9594068976548346),
                                  Point3d(-1.192493260706565, -5.636767306047028, 0.0), Point3d(2.656302768241665, -5.645493074126798, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4934603449825767, 2.941709497335058, 1.6434738733770782),
                                  Point3d(2.3454435517151353, 1.2486300643020334, 1.018110122952239),
                                  Point3d(1.1735930214222696, -3.723312833627361, 0.9594068976548346),
                                  Point3d(2.656302768241665, -5.645493074126798, 0.0), Point3d(5.716563703938576, 6.120572646649897, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4934603449825767, 2.941709497335058, 1.6434738733770782),
                                  Point3d(5.716563703938576, 6.120572646649897, 0.0), Point3d(-5.985367752852362, 6.423111118668768, 0.0),
                                  Point3d(-3.7200362861701817, 4.025043505214006, 1.1693594394280284)});
  expectedRoofPolygons.push_back({Point3d(-3.7200362861701817, 4.025043505214006, 1.1693594394280284),
                                  Point3d(-5.985367752852362, 6.423111118668768, 0.0), Point3d(-6.297731626436729, -3.6293262553813097, 0.0),
                                  Point3d(-4.254892790432256, 3.6762164828487554, 0.9074772170550292)});
  expectedRoofPolygons.push_back({Point3d(-4.254892790432256, 3.6762164828487554, 0.9074772170550292),
                                  Point3d(-6.297731626436729, -3.6293262553813097, 0.0), Point3d(-3.4580600517873807, 1.3968924313579514, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(1.4934603449825767, 2.941709497335058, 1.6434738733770782), Point3d(-3.7200362861701817, 4.025043505214006, 1.1693594394280284),
     Point3d(-4.254892790432256, 3.6762164828487554, 0.9074772170550292), Point3d(-3.4580600517873807, 1.3968924313579514, 0.0),
     Point3d(0.0, 0.0, 0.0), Point3d(2.3454435517151353, 1.2486300643020334, 1.018110122952239)});
  expectedRoofPolygons.push_back({Point3d(2.3454435517151353, 1.2486300643020334, 1.018110122952239), Point3d(0.0, 0.0, 0.0),
                                  Point3d(-1.192493260706565, -5.636767306047028, 0.0),
                                  Point3d(1.1735930214222696, -3.723312833627361, 0.9594068976548346)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB7) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {-1.1889778921584675, -7.356451670462243, 0.0},
    {5.7257149714503175, -12.035132476438635, 0.0},
    {11.739705976732338, -17.194940549920428, 0.0},
    {0.8357970425329011, -1.0288592710693223, 0.0},
    {7.360455718922119, -6.229013606285628, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955),
                                  Point3d(-1.1889778921584675, -7.356451670462243, 0.0), Point3d(5.7257149714503175, -12.035132476438635, 0.0),
                                  Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592)});
  expectedRoofPolygons.push_back({Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592),
                                  Point3d(5.7257149714503175, -12.035132476438635, 0.0), Point3d(11.739705976732338, -17.194940549920428, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955), Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592),
     Point3d(11.739705976732338, -17.194940549920428, 0.0), Point3d(0.8357970425329011, -1.0288592710693223, 0.0),
     Point3d(0.2284312718843465, -0.37117635227923845, 0.06788342500341242), Point3d(0.159929259942705, -0.4325952358032944, 0.11345123056840964)});
  expectedRoofPolygons.push_back({Point3d(0.2284312718843465, -0.37117635227923845, 0.06788342500341242),
                                  Point3d(0.8357970425329011, -1.0288592710693223, 0.0), Point3d(7.360455718922119, -6.229013606285628, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.159929259942705, -0.4325952358032944, 0.11345123056840964),
                                  Point3d(0.2284312718843465, -0.37117635227923845, 0.06788342500341242),
                                  Point3d(7.360455718922119, -6.229013606285628, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955),
                                  Point3d(0.159929259942705, -0.4325952358032944, 0.11345123056840964), Point3d(0.0, 0.0, 0.0),
                                  Point3d(-1.1889778921584675, -7.356451670462243, 0.0),
                                  Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB8) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {-1.1889778921584675, -7.356451670462243, 0.0},
    {5.7257149714503175, -12.035132476438635, 0.0},
    {11.739705976732338, -17.194940549920428, 0.0},
    {0.8357970425329011, -1.0288592710693223, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955),
                                  Point3d(-1.1889778921584675, -7.356451670462243, 0.0), Point3d(5.7257149714503175, -12.035132476438635, 0.0),
                                  Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592)});
  expectedRoofPolygons.push_back({Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592),
                                  Point3d(5.7257149714503175, -12.035132476438635, 0.0), Point3d(11.739705976732338, -17.194940549920428, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955),
                                  Point3d(6.380715062677296, -11.177062010044407, 0.5388674595320592),
                                  Point3d(11.739705976732338, -17.194940549920428, 0.0), Point3d(0.8357970425329011, -1.0288592710693223, 0.0),
                                  Point3d(0.3674963650297451, -1.375941535615892, 0.29116226024306735)});
  expectedRoofPolygons.push_back({Point3d(0.3674963650297451, -1.375941535615892, 0.29116226024306735),
                                  Point3d(0.8357970425329011, -1.0288592710693223, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955),
                                  Point3d(0.3674963650297451, -1.375941535615892, 0.29116226024306735), Point3d(0.0, 0.0, 0.0),
                                  Point3d(-1.1889778921584675, -7.356451670462243, 0.0),
                                  Point3d(1.4340349467659332, -6.223122037874707, 1.2042918694294955)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTest9) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {77.0, 85.0, 0.0},   {198.0, 85.0, 0.0},  {196.0, 139.0, 0.0}, {150.0, 119.0, 0.0},
    {157.0, 177.0, 0.0}, {112.0, 179.0, 0.0}, {125.0, 130.0, 0.0}, {68.0, 118.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back(
    {Point3d(175.59714332930346, 106.58848144366122, 10.794240721830585), Point3d(198.0, 85.0, 0.0), Point3d(196.0, 139.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(175.59714332930346, 106.58848144366122, 10.794240721830585), Point3d(196.0, 139.0, 0.0),
                                  Point3d(150.0, 119.0, 0.0), Point3d(134.3606956195723, 98.01182646424897, 6.505913232124485)});
  expectedRoofPolygons.push_back({Point3d(126.06659653966571, 107.36712501572094, 11.183562507860437),
                                  Point3d(134.3606956195723, 98.01182646424897, 6.505913232124485), Point3d(150.0, 119.0, 0.0),
                                  Point3d(157.0, 177.0, 0.0), Point3d(136.28719078311278, 159.44250239980386, 9.229921974315973),
                                  Point3d(138.93854961582085, 121.41610355696413, 5.635628665086813)});
  expectedRoofPolygons.push_back(
    {Point3d(136.28719078311278, 159.44250239980386, 9.229921974315973), Point3d(157.0, 177.0, 0.0), Point3d(112.0, 179.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(136.28719078311278, 159.44250239980386, 9.229921974315973), Point3d(112.0, 179.0, 0.0),
                                  Point3d(125.0, 130.0, 0.0), Point3d(138.93854961582085, 121.41610355696413, 5.635628665086813)});
  expectedRoofPolygons.push_back({Point3d(126.06659653966571, 107.36712501572094, 11.183562507860437),
                                  Point3d(138.93854961582085, 121.41610355696413, 5.635628665086813), Point3d(125.0, 130.0, 0.0),
                                  Point3d(68.0, 118.0, 0.0), Point3d(91.32064371214591, 103.74930831439367, 9.374654157196835)});
  expectedRoofPolygons.push_back(
    {Point3d(91.32064371214591, 103.74930831439367, 9.374654157196835), Point3d(68.0, 118.0, 0.0), Point3d(77.0, 85.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(126.06659653966571, 107.36712501572094, 11.183562507860437),
                                  Point3d(91.32064371214591, 103.74930831439367, 9.374654157196835), Point3d(77.0, 85.0, 0.0),
                                  Point3d(198.0, 85.0, 0.0), Point3d(175.59714332930346, 106.58848144366122, 10.794240721830585),
                                  Point3d(134.3606956195723, 98.01182646424897, 6.505913232124485)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB10) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {23.542862199718826, -1.0957017437087124, 0.0},
    {12.89581137652037, 1.5573908447103584, 0.0},
    {13.68678342709616, 5.195862274901293, 0.0},
    {30.92997412599037, 6.619611963708646, 0.0},
    {16.53428280871175, 7.568778425199767, 0.0},
    {13.05400578686415, 8.676139297892002, 0.0},
    {-4.188927083681472, 7.336703572978552, 0.0},
    {10.196014852102863, 4.475707108744242, 0.0},
    {8.782756714583655, 1.5573908412810287, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(11.471020185145417, 0.67152109157326, 0.6020436394953249),
                                  Point3d(23.542862199718826, -1.0957017437087124, 0.0), Point3d(12.89581137652037, 1.5573908447103584, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(11.720279767074027, 6.390569400059585, 1.0877060529617832), Point3d(12.291810335104968, 5.518617287012664, 0.7158487331168258),
     Point3d(10.882442218967304, 1.4375936356513357, 0.9709841370116856), Point3d(11.471020185145417, 0.67152109157326, 0.6020436394953249),
     Point3d(12.89581137652037, 1.5573908447103584, 0.0), Point3d(13.68678342709616, 5.195862274901293, 0.0),
     Point3d(12.24155597023194, 6.845123864037758, 0.8812970821560071)});
  expectedRoofPolygons.push_back({Point3d(12.24155597023194, 6.845123864037758, 0.8812970821560071),
                                  Point3d(13.68678342709616, 5.195862274901293, 0.0), Point3d(30.92997412599037, 6.619611963708646, 0.0),
                                  Point3d(16.331902944773294, 6.498860088077869, 0.5404575403927483),
                                  Point3d(12.84763814480008, 6.893685631130523, 0.8805587371824312)});
  expectedRoofPolygons.push_back({Point3d(16.331902944773294, 6.498860088077869, 0.5404575403927483),
                                  Point3d(30.92997412599037, 6.619611963708646, 0.0), Point3d(16.53428280871175, 7.568778425199767, 0.0)});
  expectedRoofPolygons.push_back({Point3d(12.84763814480008, 6.893685631130523, 0.8805587371824312),
                                  Point3d(16.331902944773294, 6.498860088077869, 0.5404575403927483),
                                  Point3d(16.53428280871175, 7.568778425199767, 0.0), Point3d(13.05400578686415, 8.676139297892002, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.720279767074027, 6.390569400059585, 1.0877060529617832),
                                  Point3d(12.24155597023194, 6.845123864037758, 0.8812970821560071),
                                  Point3d(12.84763814480008, 6.893685631130523, 0.8805587371824312),
                                  Point3d(13.05400578686415, 8.676139297892002, 0.0), Point3d(-4.188927083681472, 7.336703572978552, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.720279767074027, 6.390569400059585, 1.0877060529617832),
                                  Point3d(-4.188927083681472, 7.336703572978552, 0.0), Point3d(10.196014852102863, 4.475707108744242, 0.0),
                                  Point3d(12.291810335104968, 5.518617287012664, 0.7158487331168258)});
  expectedRoofPolygons.push_back({Point3d(10.882442218967304, 1.4375936356513357, 0.9709841370116856),
                                  Point3d(12.291810335104968, 5.518617287012664, 0.7158487331168258),
                                  Point3d(10.196014852102863, 4.475707108744242, 0.0), Point3d(8.782756714583655, 1.5573908412810287, 0.0),
                                  Point3d(9.49692191488606, 0.6133654113232458, 0.5271089343613387)});
  expectedRoofPolygons.push_back(
    {Point3d(9.49692191488606, 0.6133654113232458, 0.5271089343613387), Point3d(8.782756714583655, 1.5573908412810287, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(10.882442218967304, 1.4375936356513357, 0.9709841370116856),
                                  Point3d(9.49692191488606, 0.6133654113232458, 0.5271089343613387), Point3d(0.0, 0.0, 0.0),
                                  Point3d(23.542862199718826, -1.0957017437087124, 0.0),
                                  Point3d(11.471020185145417, 0.67152109157326, 0.6020436394953249)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB11) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {-0.2885918221241157, 14.001053106358517, 0.0},
    {4.899343591400031, 14.19306411217788, 0.0},
    {4.873615853393824, 2.8449682126970464, 0.0},
    {3.4878348313988496, 2.8481099737474747, 0.0},
    {3.4812833647560453, -0.04163770013738066, 0.0},
    {4.964494009365636, -0.04500034901230876, 0.0},
    {4.95897480112971, -2.4794352197829106, 0.0},
    {8.807770830090442, -2.4881609878627096, 0.0},
    {8.823006374999641, 4.231995455388115, 0.0},
    {12.663139806598815, 4.223289329948495, 0.0},
    {12.674931978309203, 9.424628538740333, 0.0},
    {8.115510522870647, 9.43496539140081, 0.0},
    {4.744642144665839, 22.784916024869645, 0.0},
    {4.729419759102431, 16.070563983220485, 0.0},
    {-1.2112187885796715, 16.08403225178089, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back(
    {Point3d(-0.09379527500698746, 15.045234233860004, 0.5181309987380204), Point3d(-0.6890929440306793, 14.379123828889721, 0.19631243722611377),
     Point3d(-0.2885918221241157, 14.001053106358517, 0.0), Point3d(4.899343591400031, 14.19306411217788, 0.0),
     Point3d(5.778996208137605, 15.103807429244426, 0.43879278874940125), Point3d(5.68156679292687, 15.176839103797569, 0.47708539116320225),
     Point3d(5.650805671922317, 15.144990752701979, 0.46174097245332135)});
  expectedRoofPolygons.push_back(
    {Point3d(7.483346413476189, 6.8357156253459355, 1.300338143887085), Point3d(6.700335885294145, 7.622284612142864, 0.9079422566821708),
     Point3d(5.778996208137605, 15.103807429244426, 0.43879278874940125), Point3d(4.899343591400031, 14.19306411217788, 0.0),
     Point3d(4.873615853393824, 2.8449682126970464, 0.0), Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808),
     Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838), Point3d(6.854366875075671, 6.209581606843638, 0.9865589482644913)});
  expectedRoofPolygons.push_back({Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808),
                                  Point3d(4.873615853393824, 2.8449682126970464, 0.0), Point3d(3.4878348313988496, 2.8481099737474747, 0.0),
                                  Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532)});
  expectedRoofPolygons.push_back({Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532),
                                  Point3d(3.4878348313988496, 2.8481099737474747, 0.0), Point3d(3.4812833647560453, -0.04163770013738066, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838), Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808),
     Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532), Point3d(3.4812833647560453, -0.04163770013738066, 0.0),
     Point3d(4.964494009365636, -0.04500034901230876, 0.0), Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014)});
  expectedRoofPolygons.push_back({Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014),
                                  Point3d(4.964494009365636, -0.04500034901230876, 0.0), Point3d(4.95897480112971, -2.4794352197829106, 0.0),
                                  Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872)});
  expectedRoofPolygons.push_back({Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872),
                                  Point3d(4.95897480112971, -2.4794352197829106, 0.0), Point3d(8.807770830090442, -2.4881609878627096, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838), Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014),
     Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872), Point3d(8.807770830090442, -2.4881609878627096, 0.0),
     Point3d(8.823006374999641, 4.231995455388115, 0.0), Point3d(6.854366875075671, 6.209581606843638, 0.9865589482644913)});
  expectedRoofPolygons.push_back({Point3d(10.068366288179561, 6.829855019919622, 1.3003381439477693),
                                  Point3d(7.483346413476189, 6.8357156253459355, 1.300338143887085),
                                  Point3d(6.854366875075671, 6.209581606843638, 0.9865589482644913),
                                  Point3d(8.823006374999641, 4.231995455388115, 0.0), Point3d(12.663139806598815, 4.223289329948495, 0.0)});
  expectedRoofPolygons.push_back({Point3d(10.068366288179561, 6.829855019919622, 1.3003381439477693),
                                  Point3d(12.663139806598815, 4.223289329948495, 0.0), Point3d(12.674931978309203, 9.424628538740333, 0.0)});
  expectedRoofPolygons.push_back({Point3d(10.068366288179561, 6.829855019919622, 1.3003381439477693),
                                  Point3d(12.674931978309203, 9.424628538740333, 0.0), Point3d(8.115510522870647, 9.43496539140081, 0.0),
                                  Point3d(6.700335885294145, 7.622284612142864, 0.9079422566821708),
                                  Point3d(7.483346413476189, 6.8357156253459355, 1.300338143887085)});
  expectedRoofPolygons.push_back({Point3d(6.700335885294145, 7.622284612142864, 0.9079422566821708),
                                  Point3d(8.115510522870647, 9.43496539140081, 0.0), Point3d(4.744642144665839, 22.784916024869645, 0.0),
                                  Point3d(5.68156679292687, 15.176839103797569, 0.47708539116320225),
                                  Point3d(5.778996208137605, 15.103807429244426, 0.43879278874940125)});
  expectedRoofPolygons.push_back({Point3d(5.68156679292687, 15.176839103797569, 0.47708539116320225),
                                  Point3d(4.744642144665839, 22.784916024869645, 0.0), Point3d(4.729419759102431, 16.070563983220485, 0.0),
                                  Point3d(5.650805671922317, 15.144990752701979, 0.46174097245332135)});
  expectedRoofPolygons.push_back({Point3d(-0.09379527500698746, 15.045234233860004, 0.5181309987380204),
                                  Point3d(5.650805671922317, 15.144990752701979, 0.46174097245332135),
                                  Point3d(4.729419759102431, 16.070563983220485, 0.0), Point3d(-1.2112187885796715, 16.08403225178089, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-0.09379527500698746, 15.045234233860004, 0.5181309987380204),
                                  Point3d(-1.2112187885796715, 16.08403225178089, 0.0), Point3d(0.0, 0.0, 0.0),
                                  Point3d(-0.6890929440306793, 14.379123828889721, 0.19631243722611377)});
  expectedRoofPolygons.push_back({Point3d(-0.6890929440306793, 14.379123828889721, 0.19631243722611377), Point3d(0.0, 0.0, 0.0),
                                  Point3d(-0.2885918221241157, 14.001053106358517, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB11_b) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {4.899343591400031, 14.19306411217788, 0.0},     {4.873615853393824, 2.8449682126970464, 0.0},   {3.4878348313988496, 2.8481099737474747, 0.0},
    {3.4812833647560453, -0.04163770013738066, 0.0}, {4.964494009365636, -0.04500034901230876, 0.0}, {4.95897480112971, -2.4794352197829106, 0.0},
    {8.807770830090442, -2.4881609878627096, 0.0},   {8.823006374999641, 4.231995455388115, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808),
                                  Point3d(4.873615853393824, 2.8449682126970464, 0.0), Point3d(3.4878348313988496, 2.8481099737474747, 0.0),
                                  Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532)});
  expectedRoofPolygons.push_back({Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532),
                                  Point3d(3.4878348313988496, 2.8481099737474747, 0.0), Point3d(3.4812833647560453, -0.04163770013738066, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838), Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808),
     Point3d(4.929432935568722, 1.3999604034575501, 0.7224387753809532), Point3d(3.4812833647560453, -0.04163770013738066, 0.0),
     Point3d(4.964494009365636, -0.04500034901230876, 0.0), Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014)});
  expectedRoofPolygons.push_back({Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014),
                                  Point3d(4.964494009365636, -0.04500034901230876, 0.0), Point3d(4.95897480112971, -2.4794352197829106, 0.0),
                                  Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872)});
  expectedRoofPolygons.push_back({Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872),
                                  Point3d(4.95897480112971, -2.4794352197829106, 0.0), Point3d(8.807770830090442, -2.4881609878627096, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838), Point3d(6.893254906247293, 1.875034782130368, 0.9622014799910014),
     Point3d(6.8877356980830235, -0.5594000893968922, 0.9622014800276872), Point3d(8.807770830090442, -2.4881609878627096, 0.0),
     Point3d(8.823006374999641, 4.231995455388115, 0.0), Point3d(6.8490390285892975, 3.8595532917064257, 0.9865589482615805)});
  expectedRoofPolygons.push_back({Point3d(6.8490390285892975, 3.8595532917064257, 0.9865589482615805),
                                  Point3d(8.823006374999641, 4.231995455388115, 0.0), Point3d(4.899343591400031, 14.19306411217788, 0.0)});
  expectedRoofPolygons.push_back({Point3d(6.8490390285892975, 3.8595532917064257, 0.9865589482615805),
                                  Point3d(4.899343591400031, 14.19306411217788, 0.0), Point3d(4.873615853393824, 2.8449682126970464, 0.0),
                                  Point3d(6.315213958119228, 1.396818641879405, 0.7224387756441808),
                                  Point3d(6.844650538271922, 1.9238600368574004, 0.9865589482591838)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB12) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {1.6082838074612242, 15.395815413439262, 0.0},
    {6.796219518140479, 15.587826427398873, 0.0},
    {6.7704917786606345, 4.239729879063727, 0.0},
    {5.384710677004972, 4.2428716408656655, 0.0},
    {5.37815921027269, 1.3531237986037645, 0.0},
    {6.861369940123552, 1.3497611512508971, 0.0},
    {6.855850731428608, -1.084673859531076, 0.0},
    {10.704646980698193, -1.093399628682226, 0.0},
    {10.719882526622944, 5.626757200629533, 0.0},
    {14.560016178034793, 5.6180510758343525, 0.0},
    {14.571808350563504, 10.819390581977487, 0.0},
    {10.01238663382704, 10.829727434086928, 0.0},
    {6.64151806240239, 24.179678832787182, 0.0},
    {6.626295676252851, 17.465326408838887, 0.0},
    {0.6856567883022331, 17.478794675312955, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back(
    {Point3d(1.6842202935831398, 16.437932562856407, 0.5192977794804833), Point3d(1.1408243780107237, 15.895738201571742, 0.2584350351911583),
     Point3d(1.6082838074612242, 15.395815413439262, 0.0), Point3d(6.796219518140479, 15.587826427398873, 0.0),
     Point3d(7.67587218531803, 16.498569796186654, 0.4387928139105931), Point3d(7.578442764618077, 16.571601474896294, 0.4770854184766968),
     Point3d(7.54768164371096, 16.539753123917784, 0.4617409998147321)});
  expectedRoofPolygons.push_back(
    {Point3d(9.380222488057502, 8.230477519159527, 1.3003382182858605), Point3d(8.59721191524112, 9.017046550596557, 0.9079423087131434),
     Point3d(7.67587218531803, 16.498569796186654, 0.4387928139105931), Point3d(6.796219518140479, 15.587826427398873, 0.0),
     Point3d(6.7704917786606345, 4.239729879063727, 0.0), Point3d(8.21208996516684, 2.7915802254966904, 0.7224388166284438),
     Point3d(8.741526576487445, 3.3186216521109917, 0.986559004791438), Point3d(8.7512429138868, 7.604343464862684, 0.986559004818869)});
  expectedRoofPolygons.push_back({Point3d(6.826308864186403, 2.7947219867367816, 0.7224388169085991),
                                  Point3d(8.21208996516684, 2.7915802254966904, 0.7224388166284438),
                                  Point3d(6.7704917786606345, 4.239729879063727, 0.0), Point3d(5.384710677004972, 4.2428716408656655, 0.0)});
  expectedRoofPolygons.push_back({Point3d(6.826308864186403, 2.7947219867367816, 0.7224388169085991),
                                  Point3d(5.384710677004972, 4.2428716408656655, 0.0), Point3d(5.37815921027269, 1.3531237986037645, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(8.741526576487445, 3.3186216521109917, 0.986559004791438), Point3d(8.21208996516684, 2.7915802254966904, 0.7224388166284438),
     Point3d(6.826308864186403, 2.7947219867367816, 0.7224388169085991), Point3d(5.37815921027269, 1.3531237986037645, 0.0),
     Point3d(6.861369940123552, 1.3497611512508971, 0.0), Point3d(8.790130947445553, 3.269796394443114, 0.9622015350280487)});
  expectedRoofPolygons.push_back({Point3d(8.78461173886206, 0.8353613804382916, 0.962201535087427),
                                  Point3d(8.790130947445553, 3.269796394443114, 0.9622015350280487),
                                  Point3d(6.861369940123552, 1.3497611512508971, 0.0), Point3d(6.855850731428608, -1.084673859531076, 0.0)});
  expectedRoofPolygons.push_back({Point3d(8.78461173886206, 0.8353613804382916, 0.962201535087427),
                                  Point3d(6.855850731428608, -1.084673859531076, 0.0), Point3d(10.704646980698193, -1.093399628682226, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(8.741526576487445, 3.3186216521109917, 0.986559004791438), Point3d(8.790130947445553, 3.269796394443114, 0.9622015350280487),
     Point3d(8.78461173886206, 0.8353613804382916, 0.962201535087427), Point3d(10.704646980698193, -1.093399628682226, 0.0),
     Point3d(10.719882526622944, 5.626757200629533, 0.0), Point3d(8.7512429138868, 7.604343464862684, 0.986559004818869)});
  expectedRoofPolygons.push_back({Point3d(9.380222488057502, 8.230477519159527, 1.3003382182858605),
                                  Point3d(8.7512429138868, 7.604343464862684, 0.986559004818869), Point3d(10.719882526622944, 5.626757200629533, 0.0),
                                  Point3d(14.560016178034793, 5.6180510758343525, 0.0),
                                  Point3d(11.965242511286412, 8.224616914105267, 1.3003382183162968)});
  expectedRoofPolygons.push_back({Point3d(11.965242511286412, 8.224616914105267, 1.3003382183162968),
                                  Point3d(14.560016178034793, 5.6180510758343525, 0.0), Point3d(14.571808350563504, 10.819390581977487, 0.0)});
  expectedRoofPolygons.push_back({Point3d(11.965242511286412, 8.224616914105267, 1.3003382183162968),
                                  Point3d(14.571808350563504, 10.819390581977487, 0.0), Point3d(10.01238663382704, 10.829727434086928, 0.0),
                                  Point3d(8.59721191524112, 9.017046550596557, 0.9079423087131434),
                                  Point3d(9.380222488057502, 8.230477519159527, 1.3003382182858605)});
  expectedRoofPolygons.push_back({Point3d(8.59721191524112, 9.017046550596557, 0.9079423087131434),
                                  Point3d(10.01238663382704, 10.829727434086928, 0.0), Point3d(6.64151806240239, 24.179678832787182, 0.0),
                                  Point3d(7.578442764618077, 16.571601474896294, 0.4770854184766968),
                                  Point3d(7.67587218531803, 16.498569796186654, 0.4387928139105931)});
  expectedRoofPolygons.push_back({Point3d(7.578442764618077, 16.571601474896294, 0.4770854184766968),
                                  Point3d(6.64151806240239, 24.179678832787182, 0.0), Point3d(6.626295676252851, 17.465326408838887, 0.0),
                                  Point3d(7.54768164371096, 16.539753123917784, 0.4617409998147321)});
  expectedRoofPolygons.push_back({Point3d(1.6842202935831398, 16.437932562856407, 0.5192977794804833),
                                  Point3d(7.54768164371096, 16.539753123917784, 0.4617409998147321),
                                  Point3d(6.626295676252851, 17.465326408838887, 0.0), Point3d(0.6856567883022331, 17.478794675312955, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.6842202935831398, 16.437932562856407, 0.5192977794804833),
                                  Point3d(0.6856567883022331, 17.478794675312955, 0.0), Point3d(0.0, 0.0, 0.0),
                                  Point3d(1.1408243780107237, 15.895738201571742, 0.2584350351911583)});
  expectedRoofPolygons.push_back({Point3d(1.1408243780107237, 15.895738201571742, 0.2584350351911583), Point3d(0.0, 0.0, 0.0),
                                  Point3d(1.6082838074612242, 15.395815413439262, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonTestB13) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {-0.03697835689094475, 17.903291653889664, 0.0},
    {9.36122931562474, 17.922703185404146, 0.0},
    {9.399539490923859, -0.6253964219022965, 0.0},
    {6.897780217346079, -0.6305636811510293, 0.0},
    {6.907305814387495, -5.242438102429183, 0.0},
    {9.496043768204736, -5.2367356072030695, 0.0},
    {9.673537498409361, -7.819464124646299, 0.0},
    {19.728934851080233, -7.7986952031890375, 0.0},
    {19.715280237589244, -1.1877328304801722, 0.0},
    {23.581205989632387, -1.1797479507986637, 0.0},
    {23.570459756724986, 4.023104657038741, 0.0},
    {19.065027189523686, 4.01379891209519, 0.0},
    {19.009685241927738, 30.807932065847332, 0.0},
    {9.439383865135643, 30.78816508512935, 0.0},
    {9.453189359125524, 24.10415305431124, 0.0},
    {-0.01730198014624129, 24.08459222736407, 0.0},
    {-0.030597953439544412, 30.521916694234474, 0.0},
    {-10.417861267451112, 30.500462317733504, 0.0},
    {-10.354819907553885, -0.021387367337700525, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back(
    {Point3d(-5.2088937947536875, 23.086263132900534, 2.5968213677080576), Point3d(-5.225081993006609, 23.07000792440424, 2.5887104987207548),
     Point3d(-0.03697835689094475, 17.903291653889664, 0.0), Point3d(9.36122931562474, 17.922703185404146, 0.0),
     Point3d(12.445462580412869, 21.019703480686516, 1.5453116860600677), Point3d(-3.101535247093261, 20.98759193064646, 1.5453116872475992)});
  expectedRoofPolygons.push_back(
    {Point3d(14.557918451002053, -1.1527999293121487, 2.578639316869181), Point3d(14.237504608711996, -0.833706956371332, 2.418762272064255),
     Point3d(14.19596815545603, 19.276414164692916, 2.4187622718784856), Point3d(12.445462580412869, 21.019703480686516, 1.5453116860600677),
     Point3d(9.36122931562474, 17.922703185404146, 0.0), Point3d(9.399539490923859, -0.6253964219022965, 0.0),
     Point3d(12.606101682729625, -3.8187399272616855, 1.599979837155215), Point3d(13.428106603203812, -3.7896778027216365, 2.0110114338303995),
     Point3d(14.561015138561665, -2.6520796490290395, 2.5786393168806017)});
  expectedRoofPolygons.push_back({Point3d(12.606101682729625, -3.8187399272616855, 1.599979837155215),
                                  Point3d(9.399539490923859, -0.6253964219022965, 0.0), Point3d(6.897780217346079, -0.6305636811510293, 0.0),
                                  Point3d(9.208321529781248, -2.9315800507494063, 1.152891879668805),
                                  Point3d(11.648322280432005, -2.9263727729378277, 1.1528080909886411)});
  expectedRoofPolygons.push_back({Point3d(9.208321529781248, -2.9315800507494063, 1.152891879668805),
                                  Point3d(6.897780217346079, -0.6305636811510293, 0.0), Point3d(6.907305814387495, -5.242438102429183, 0.0)});
  expectedRoofPolygons.push_back({Point3d(9.208321529781248, -2.9315800507494063, 1.152891879668805),
                                  Point3d(6.907305814387495, -5.242438102429183, 0.0), Point3d(9.496043768204736, -5.2367356072030695, 0.0),
                                  Point3d(11.648322280432005, -2.9263727729378277, 1.1528080909886411)});
  expectedRoofPolygons.push_back({Point3d(13.428106603203812, -3.7896778027216365, 2.0110114338303995),
                                  Point3d(12.606101682729625, -3.8187399272616855, 1.599979837155215),
                                  Point3d(11.648322280432005, -2.9263727729378277, 1.1528080909886411),
                                  Point3d(9.496043768204736, -5.2367356072030695, 0.0), Point3d(9.673537498409361, -7.819464124646299, 0.0)});
  expectedRoofPolygons.push_back({Point3d(14.561015138561665, -2.6520796490290395, 2.5786393168806017),
                                  Point3d(13.428106603203812, -3.7896778027216365, 2.0110114338303995),
                                  Point3d(9.673537498409361, -7.819464124646299, 0.0), Point3d(19.728934851080233, -7.7986952031890375, 0.0)});
  expectedRoofPolygons.push_back({Point3d(14.561015138561665, -2.6520796490290395, 2.5786393168806017),
                                  Point3d(19.728934851080233, -7.7986952031890375, 0.0), Point3d(19.715280237589244, -1.1877328304801722, 0.0),
                                  Point3d(17.108480813881513, 1.408320358557954, 1.3007159280980942),
                                  Point3d(14.557918451002053, -1.1527999293121487, 2.578639316869181)});
  expectedRoofPolygons.push_back({Point3d(17.108480813881513, 1.408320358557954, 1.3007159280980942),
                                  Point3d(19.715280237589244, -1.1877328304801722, 0.0), Point3d(23.581205989632387, -1.1797479507986637, 0.0),
                                  Point3d(20.974406567920887, 1.4163052362523214, 1.3007159271024618)});
  expectedRoofPolygons.push_back({Point3d(20.974406567920887, 1.4163052362523214, 1.3007159271024618),
                                  Point3d(23.581205989632387, -1.1797479507986637, 0.0), Point3d(23.570459756724986, 4.023104657038741, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(14.557918451002053, -1.1527999293121487, 2.578639316869181), Point3d(17.108480813881513, 1.408320358557954, 1.3007159280980942),
     Point3d(20.974406567920887, 1.4163052362523214, 1.3007159271024618), Point3d(23.570459756724986, 4.023104657038741, 0.0),
     Point3d(19.065027189523686, 4.01379891209519, 0.0), Point3d(14.237504608711996, -0.833706956371332, 2.418762272064255)});
  expectedRoofPolygons.push_back(
    {Point3d(14.237504608711996, -0.833706956371332, 2.418762272064255), Point3d(19.065027189523686, 4.01379891209519, 0.0),
     Point3d(19.009685241927738, 30.807932065847332, 0.0), Point3d(14.234418043971875, 26.012897887101527, 2.392580447652291),
     Point3d(14.248223537950237, 19.328885855734843, 2.3925804476459653), Point3d(14.19596815545603, 19.276414164692916, 2.4187622718784856)});
  expectedRoofPolygons.push_back({Point3d(14.234418043971875, 26.012897887101527, 2.392580447652291),
                                  Point3d(19.009685241927738, 30.807932065847332, 0.0), Point3d(9.439383865135643, 30.78816508512935, 0.0)});
  expectedRoofPolygons.push_back({Point3d(14.234418043971875, 26.012897887101527, 2.392580447652291),
                                  Point3d(9.439383865135643, 30.78816508512935, 0.0), Point3d(9.453189359125524, 24.10415305431124, 0.0),
                                  Point3d(14.248223537950237, 19.328885855734843, 2.3925804476459653)});
  expectedRoofPolygons.push_back(
    {Point3d(14.19596815545603, 19.276414164692916, 2.4187622718784856), Point3d(14.248223537950237, 19.328885855734843, 2.3925804476459653),
     Point3d(9.453189359125524, 24.10415305431124, 0.0), Point3d(-0.01730198014624129, 24.08459222736407, 0.0),
     Point3d(-3.101535247093261, 20.98759193064646, 1.5453116872475992), Point3d(12.445462580412869, 21.019703480686516, 1.5453116860600677)});
  expectedRoofPolygons.push_back({Point3d(-5.2088937947536875, 23.086263132900534, 2.5968213677080576),
                                  Point3d(-3.101535247093261, 20.98759193064646, 1.5453116872475992),
                                  Point3d(-0.01730198014624129, 24.08459222736407, 0.0), Point3d(-0.030597953439544412, 30.521916694234474, 0.0),
                                  Point3d(-5.213502422879822, 25.317557848847482, 2.596821367680282)});
  expectedRoofPolygons.push_back({Point3d(-5.213502422879822, 25.317557848847482, 2.596821367680282),
                                  Point3d(-0.030597953439544412, 30.521916694234474, 0.0), Point3d(-10.417861267451112, 30.500462317733504, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-5.213502422879822, 25.317557848847482, 2.596821367680282), Point3d(-10.417861267451112, 30.500462317733504, 0.0),
     Point3d(-10.354819907553885, -0.021387367337700525, 0.0), Point3d(-5.188103636084188, 5.166716270119771, 2.5887104987054257),
     Point3d(-5.225081993006609, 23.07000792440424, 2.5887104987207548), Point3d(-5.2088937947536875, 23.086263132900534, 2.5968213677080576)});
  expectedRoofPolygons.push_back({Point3d(-5.188103636084188, 5.166716270119771, 2.5887104987054257),
                                  Point3d(-10.354819907553885, -0.021387367337700525, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-5.188103636084188, 5.166716270119771, 2.5887104987054257), Point3d(0.0, 0.0, 0.0),
                                  Point3d(-0.03697835689094475, 17.903291653889664, 0.0),
                                  Point3d(-5.225081993006609, 23.07000792440424, 2.5887104987207548)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonCircularAddTest) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {50.0, 50.0, 0.0},
    {100.0, 50.0, 0.0},
    {100.0, 100.0, 0.0},
    {50.0, 100.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(100.0, 50.0, 0.0), Point3d(100.0, 100.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(100.0, 100.0, 0.0), Point3d(50.0, 100.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 100.0, 0.0), Point3d(50.0, 50.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 50.0, 0.0), Point3d(100.0, 50.0, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonCircularAddTest2) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {50.0, 50.0, 0.0},
    {150.0, 50.0, 0.0},
    {150.0, 100.0, 0.0},
    {50.0, 100.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(125.0, 75.0, 12.5), Point3d(150.0, 50.0, 0.0), Point3d(150.0, 100.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(125.0, 75.0, 12.5), Point3d(150.0, 100.0, 0.0), Point3d(50.0, 100.0, 0.0), Point3d(75.0, 75.0, 12.5)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 100.0, 0.0), Point3d(50.0, 50.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 50.0, 0.0), Point3d(150.0, 50.0, 0.0), Point3d(125.0, 75.0, 12.5)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofSkeletonCircularAddTest2Clockwise) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {50.0, 100.0, 0.0},
    {150.0, 100.0, 0.0},
    {150.0, 50.0, 0.0},
    {50.0, 50.0, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(125.0, 75.0, 12.5), Point3d(150.0, 50.0, 0.0), Point3d(150.0, 100.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(125.0, 75.0, 12.5), Point3d(150.0, 100.0, 0.0), Point3d(50.0, 100.0, 0.0), Point3d(75.0, 75.0, 12.5)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 100.0, 0.0), Point3d(50.0, 50.0, 0.0)});
  expectedRoofPolygons.push_back({Point3d(75.0, 75.0, 12.5), Point3d(50.0, 50.0, 0.0), Point3d(150.0, 50.0, 0.0), Point3d(125.0, 75.0, 12.5)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofExtendedSkeleton_2) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {0.8276566632200648, 11.888928006049232, 0.0},
    {-27.730160414042565, 13.959474241319969, 0.0},
    {-24.199801903849206, 64.16488993327617, 0.0},
    {-35.2962607066056, 62.973758421480234, 0.0},
    {-38.51167022106408, 16.998503173051198, 0.0},
    {-39.01705349327869, 17.065295035165335, 0.0},
    {-39.830061365976746, 5.732953688918397, 0.0},
    {-39.31735369851071, 5.643898052309142, 0.0},
    {-42.53276321298834, -39.9524008863303, 0.0},
    {-31.53884594371366, -40.8652136123464, 0.0},
    {-28.42597796271775, 2.014882080669223, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(-33.12576076178471, 8.348995741201023, 2.9929832175950524),
                                  Point3d(-5.534565848874263, 6.370910126769063, 2.9818240214159712),
                                  Point3d(0.8276566632200648, 11.888928006049232, 0.0), Point3d(-27.730160414042565, 13.959474241319969, 0.0),
                                  Point3d(-33.5880306861987, 8.882472833839651, 2.743657268726208)});
  expectedRoofPolygons.push_back(
    {Point3d(-33.73029206163186, 10.502279413716543, 2.8714234725358816), Point3d(-33.495115727941425, 10.201937386209625, 2.7435911921023313),
     Point3d(-33.5880306861987, 8.882472833839651, 2.743657268726208), Point3d(-27.730160414042565, 13.959474241319969, 0.0),
     Point3d(-24.199801903849206, 64.16488993327617, 0.0), Point3d(-30.13850707997531, 58.0039423706306, 2.7459584235439918),
     Point3d(-33.448913912591436, 10.798769300265931, 2.741479595567126), Point3d(-33.7252549837291, 10.57319397068576, 2.8713982906248874)});
  expectedRoofPolygons.push_back({Point3d(-30.13850707997531, 58.0039423706306, 2.7459584235439918),
                                  Point3d(-24.199801903849206, 64.16488993327617, 0.0), Point3d(-35.2962607066056, 62.973758421480234, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-30.13850707997531, 58.0039423706306, 2.7459584235439918),
                                  Point3d(-35.2962607066056, 62.973758421480234, 0.0), Point3d(-38.51167022106408, 16.998503173051198, 0.0),
                                  Point3d(-33.448913912591436, 10.798769300265931, 2.741479595567126)});
  expectedRoofPolygons.push_back({Point3d(-33.7252549837291, 10.57319397068576, 2.8713982906248874),
                                  Point3d(-33.448913912591436, 10.798769300265931, 2.741479595567126),
                                  Point3d(-38.51167022106408, 16.998503173051198, 0.0), Point3d(-39.01705349327869, 17.065295035165335, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-33.73029206163186, 10.502279413716543, 2.8714234725358816),
                                  Point3d(-33.7252549837291, 10.57319397068576, 2.8713982906248874),
                                  Point3d(-39.01705349327869, 17.065295035165335, 0.0), Point3d(-39.830061365976746, 5.732953688918397, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-33.73029206163186, 10.502279413716543, 2.8714234725358816),
                                  Point3d(-39.830061365976746, 5.732953688918397, 0.0), Point3d(-39.31735369851071, 5.643898052309142, 0.0),
                                  Point3d(-33.495115727941425, 10.201937386209625, 2.7435911921023313)});
  expectedRoofPolygons.push_back(
    {Point3d(-33.12576076178471, 8.348995741201023, 2.9929832175950524), Point3d(-33.5880306861987, 8.882472833839651, 2.743657268726208),
     Point3d(-33.495115727941425, 10.201937386209625, 2.7435911921023313), Point3d(-39.31735369851071, 5.643898052309142, 0.0),
     Point3d(-42.53276321298834, -39.9524008863303, 0.0), Point3d(-36.64164137666454, -34.90132272042725, 2.760606544351742),
     Point3d(-33.5746660958561, 7.959374567061363, 2.782790406093265)});
  expectedRoofPolygons.push_back({Point3d(-36.64164137666454, -34.90132272042725, 2.760606544351742),
                                  Point3d(-42.53276321298834, -39.9524008863303, 0.0), Point3d(-31.53884594371366, -40.8652136123464, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-33.5746660958561, 7.959374567061363, 2.782790406093265),
                                  Point3d(-36.64164137666454, -34.90132272042725, 2.760606544351742),
                                  Point3d(-31.53884594371366, -40.8652136123464, 0.0), Point3d(-28.42597796271775, 2.014882080669223, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-33.12576076178471, 8.348995741201023, 2.9929832175950524),
                                  Point3d(-33.5746660958561, 7.959374567061363, 2.782790406093265),
                                  Point3d(-28.42597796271775, 2.014882080669223, 0.0), Point3d(0.0, 0.0, 0.0),
                                  Point3d(-5.534565848874263, 6.370910126769063, 2.9818240214159712)});
  expectedRoofPolygons.push_back({Point3d(-5.534565848874263, 6.370910126769063, 2.9818240214159712), Point3d(0.0, 0.0, 0.0),
                                  Point3d(0.8276566632200648, 11.888928006049232, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofExtendedSkeleton_3) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {-0.695841208347133, -9.161985802862006, 0.0},
    {-9.338921480375689, -8.571967728493888, 0.0},
    {-17.703665269065453, -7.447593490720266, 0.0},
    {-17.42532878570745, -3.373126538389357, 0.0},
    {-34.86530686005406, -2.0261026523114936, 0.0},
    {-37.17989445833143, -1.81458647758825, 0.0},
    {-55.19119478795656, -0.5677541139705511, 0.0},
    {-58.11372786299535, -38.75186865681468, 0.0},
    {-50.07126842338943, -39.34188353361671, 0.0},
    {-51.126017202341586, -51.888034167850435, 0.0},
    {-51.536197283087745, -56.652669598441975, 0.0},
    {-64.58138877837769, -55.528305063057076, 0.0},
    {-72.68244537234855, -54.81583632791774, 0.0},
    {-69.18859004203122, -7.625712198986719, 0.0},
    {-79.09883377766633, -6.7239861902833695, 0.0},
    {-78.63738118687476, 1.1689057121689643, 0.0},
    {-88.04222446591587, 2.070633143170312, 0.0},
    {-87.65401831812835, 7.380808750431726, 0.0},
    {-92.65675037390812, 7.748179577427749, 0.0},
    {-107.56972616530382, 8.816894846881922, 0.0},
    {-112.57245822106444, 9.184265769474024, 0.0},
    {-112.96066436885195, 3.77389646371617, 0.0},
    {-114.92366904082473, 3.8629559986240167, 0.0},
    {-115.04086334961207, 2.538195570451535, 0.0},
    {-116.69623296100607, 2.638387525545255, 0.0},
    {-116.62298651802115, 3.952015535370265, 0.0},
    {-122.2629626277448, 4.308253694611125, 0.0},
    {-122.84893417162414, -3.573510226744844, 0.0},
    {-133.24992907525242, -2.7719754218481114, 0.0},
    {-136.29698110334067, -45.63166089550802, 0.0},
    {-136.56066829809785, -49.739493961847415, 0.0},
    {-136.5972915195616, -49.87308197764537, 0.0},
    {-144.44198556309482, -49.038156822071414, 0.0},
    {-154.15446390269554, -47.99171711813307, 0.0},
    {-152.87997579476948, -31.693949992923137, 0.0},
    {-148.39729348418453, -32.06131821982851, 0.0},
    {-145.6944997381005, 6.067179931716822, 0.0},
    {-183.84124724381752, 8.939351818832364, 0.0},
    {-184.18550552584088, 4.33051857910408, 0.0},
    {-188.32392955442162, 4.675624318771895, 0.0},
    {-199.20835098172356, 5.499425199435763, 0.0},
    {-201.0981092106731, 5.655279433782141, 0.0},
    {-201.0981092106731, 5.911325686340149, 0.0},
    {-200.28507369355776, 14.483315691783218, 0.0},
    {-199.75037465980046, 23.623075402548864, 0.0},
    {-195.63392456412095, 23.222306088178982, 0.0},
    {-192.47700287154572, 67.60768288193731, 0.0},
    {-193.30468767722357, 67.66334572802566, 0.0},
    {-193.24609052287775, 68.36469764229747, 0.0},
    {-193.13622085837167, 68.36469764229747, 0.0},
    {-192.38178249564044, 80.51034556245332, 0.0},
    {-192.50630144874737, 80.54374334940206, 0.0},
    {-192.41108107288036, 81.5902073856052, 0.0},
    {-191.62734413292836, 81.5456769983088, 0.0},
    {-188.2946309771817, 125.5975377539537, 0.0},
    {-192.32318534125633, 125.93151831483829, 0.0},
    {-190.80698397153176, 143.8217730301796, 0.0},
    {-173.83578313229629, 142.43018301674232, 0.0},
    {-173.9456527967258, 138.0550262829267, 0.0},
    {-107.15954608457682, 132.88945125194346, 0.0},
    {-106.35383521178105, 132.900583951564, 0.0},
    {-105.02074994949004, 133.14550334995624, 0.0},
    {-103.43862678102352, 133.8691289086305, 0.0},
    {-102.2813329819231, 134.82654132846721, 0.0},
    {-101.31447993453753, 136.14020049587162, 0.0},
    {-100.20845864548255, 136.05113884865818, 0.0},
    {-80.4392436842769, 134.5036929371352, 0.0},
    {-79.47971528123003, 134.42576401589253, 0.0},
    {-78.85712051582951, 133.11210525048187, 0.0},
    {-77.91224140138345, 132.0099680511679, 0.0},
    {-76.60845471625578, 131.07482150365024, 0.0},
    {-75.64892631317062, 130.64064637716376, 0.0},
    {-73.99355670177661, 130.28440014311866, 0.0},
    {-7.85201868781445, 125.31922063111779, 0.0},
    {-7.49311111722858, 130.0506135632292, 0.0},
    {9.60993331941421, 128.65902699125058, 0.0},
    {9.236376460189206, 123.92763524122142, 0.0},
    {8.181627681237046, 110.5350351927255, 0.0},
    {4.255618337310636, 110.85788219852725, 0.0},
    {0.9448791144652091, 67.1512475606807, 0.0},
    {1.8091871416834238, 67.01765674256961, 0.0},
    {1.7579146315805474, 66.12705137122322, 0.0},
    {1.5674738798274461, 66.1604490693166, 0.0},
    {0.6592179868259791, 53.858976831483176, 0.0},
    {0.8936066043815458, 53.88124191758578, 0.0},
    {0.7690876513129297, 53.05743388498746, 0.0},
    {-0.1171943088256171, 53.09083150464306, 0.0},
    {-3.2448174241800416, 9.039543887906007, 0.0},
    {-0.16846681885186854, 8.816894846881922, 0.0},
    {-0.14649288598894827, 8.995014077862805, 0.0},
    {0.6811919197272118, 8.89482200817639, 0.0},
    {0.6299194096243355, 8.015358375794648, 0.0},
    {0.4980758122744646, 8.02649082539466, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.22498766501530973, 8.482519288941951, 0.21571726906224928),
                                  Point3d(0.6299194096243355, 8.015358375794648, 0.0), Point3d(0.6811919197272118, 8.89482200817639, 0.0),
                                  Point3d(0.22804291428188161, 8.516190461338422, 0.21517208062316445)});
  expectedRoofPolygons.push_back({Point3d(0.22804291428188161, 8.516190461338422, 0.21517208062316445),
                                  Point3d(0.6811919197272118, 8.89482200817639, 0.0), Point3d(-0.14649288598894827, 8.995014077862805, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(0.22498766501530973, 8.482519288941951, 0.21571726906224928), Point3d(0.22804291428188161, 8.516190461338422, 0.21517208062316445),
     Point3d(-0.14649288598894827, 8.995014077862805, 0.0), Point3d(-0.16846681885186854, 8.816894846881922, 0.0),
     Point3d(0.15399233874051674, 8.42451956393686, 0.18403737747332619), Point3d(0.15402882814217608, 8.424913516260558, 0.1840313674902289)});
  expectedRoofPolygons.push_back(
    {Point3d(-8.54200371397651, 0.8238610560514159, 4.288315220750146), Point3d(0.15399233874051674, 8.42451956393686, 0.18403737747332619),
     Point3d(-0.16846681885186854, 8.816894846881922, 0.0), Point3d(-3.2448174241800416, 9.039543887906007, 0.0),
     Point3d(-10.420788871410501, 2.8235982695492625, 3.3588656661105745), Point3d(-10.26903498604995, 2.812246226874768, 3.3590496577074944)});
  expectedRoofPolygons.push_back(
    {Point3d(-61.03194322798886, 67.36487419298273, 30.88634973645842), Point3d(-42.70885984371319, 46.18933004077511, 20.997972658943333),
     Point3d(-43.10454118850721, 40.69777951729232, 21.00085716757909), Point3d(-34.5562630110808, 30.860444838236432, 16.389103917802835),
     Point3d(-32.31508760437524, 28.223224732309113, 15.177944303266205), Point3d(-10.420788871410501, 2.8235982695492625, 3.3588656661105745),
     Point3d(-3.2448174241800416, 9.039543887906007, 0.0), Point3d(-0.1171943088256171, 53.09083150464306, 0.0),
     Point3d(-33.924443240636386, 90.78233450985698, 18.19586051354917)});
  expectedRoofPolygons.push_back(
    {Point3d(-33.924443240636386, 90.78233450985698, 18.19586051354917), Point3d(-0.1171943088256171, 53.09083150464306, 0.0),
     Point3d(0.7690876513129297, 53.05743388498746, 0.0), Point3d(0.44132245629059363, 53.45330511858461, 0.19162405094516738),
     Point3d(0.2568264964007361, 53.448041591527556, 0.1855204537244768), Point3d(-6.2775029556709985, 60.75381589825485, 3.71278810244436),
     Point3d(-7.024664018198711, 60.8253933087784, 3.7344838345469777), Point3d(-33.75867049403901, 90.77299217258242, 18.19431383644973)});
  expectedRoofPolygons.push_back({Point3d(0.44132245629059363, 53.45330511858461, 0.19162405094516738),
                                  Point3d(0.7690876513129297, 53.05743388498746, 0.0), Point3d(0.8936066043815458, 53.88124191758578, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.44132245629059363, 53.45330511858461, 0.19162405094516738),
                                  Point3d(0.8936066043815458, 53.88124191758578, 0.0), Point3d(0.6592179868259791, 53.858976831483176, 0.0),
                                  Point3d(0.2568264964007361, 53.448041591527556, 0.1855204537244768)});
  expectedRoofPolygons.push_back({Point3d(-6.2775029556709985, 60.75381589825485, 3.71278810244436),
                                  Point3d(0.2568264964007361, 53.448041591527556, 0.1855204537244768),
                                  Point3d(0.6592179868259791, 53.858976831483176, 0.0), Point3d(1.5674738798274461, 66.1604490693166, 0.0),
                                  Point3d(1.1670534531361396, 66.67452948203062, 0.21859328791869365)});
  expectedRoofPolygons.push_back({Point3d(1.347945148670707, 66.64469142554623, 0.21952162974258888),
                                  Point3d(1.1670534531361396, 66.67452948203062, 0.21859328791869365),
                                  Point3d(1.5674738798274461, 66.1604490693166, 0.0), Point3d(1.7579146315805474, 66.12705137122322, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.347945148670707, 66.64469142554623, 0.21952162974258888),
                                  Point3d(1.7579146315805474, 66.12705137122322, 0.0), Point3d(1.8091871416834238, 67.01765674256961, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-7.024664018198711, 60.8253933087784, 3.7344838345469777), Point3d(-6.2775029556709985, 60.75381589825485, 3.71278810244436),
     Point3d(1.1670534531361396, 66.67452948203062, 0.21859328791869365), Point3d(1.347945148670707, 66.64469142554623, 0.21952162974258888),
     Point3d(1.8091871416834238, 67.01765674256961, 0.0), Point3d(0.9448791144652091, 67.1512475606807, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-33.75867049403901, 90.77299217258242, 18.19431383644973),
                                  Point3d(-7.024664018198711, 60.8253933087784, 3.7344838345469777),
                                  Point3d(0.9448791144652091, 67.1512475606807, 0.0), Point3d(4.255618337310636, 110.85788219852725, 0.0),
                                  Point3d(-1.946448792095577, 118.12378669561315, 3.366581720843165)});
  expectedRoofPolygons.push_back({Point3d(0.31749588484094743, 119.77603117590006, 4.2826970132879),
                                  Point3d(-1.64604883910226, 118.10016025919867, 3.367118066990641),
                                  Point3d(-1.946448792095577, 118.12378669561315, 3.366581720843165),
                                  Point3d(4.255618337310636, 110.85788219852725, 0.0), Point3d(8.181627681237046, 110.5350351927255, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.3976890543497383, 120.81337575173194, 4.283446707132655),
                                  Point3d(0.31749588484094743, 119.77603117590006, 4.2826970132879),
                                  Point3d(8.181627681237046, 110.5350351927255, 0.0), Point3d(9.236376460189206, 123.92763524122142, 0.0),
                                  Point3d(4.49313037957261, 124.30166138509239, 2.378985010522967)});
  expectedRoofPolygons.push_back({Point3d(4.49313037957261, 124.30166138509239, 2.378985010522967),
                                  Point3d(9.236376460189206, 123.92763524122142, 0.0), Point3d(9.60993331941421, 128.65902699125058, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.3976890543497383, 120.81337575173194, 4.283446707132655),
                                  Point3d(4.49313037957261, 124.30166138509239, 2.378985010522967),
                                  Point3d(9.60993331941421, 128.65902699125058, 0.0), Point3d(-7.49311111722858, 130.0506135632292, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.3976890543497383, 120.81337575173194, 4.283446707132655),
                                  Point3d(-7.49311111722858, 130.0506135632292, 0.0), Point3d(-7.85201868781445, 125.31922063111779, 0.0),
                                  Point3d(-1.64604883910226, 118.10016025919867, 3.367118066990641),
                                  Point3d(0.31749588484094743, 119.77603117590006, 4.2826970132879)});
  expectedRoofPolygons.push_back(
    {Point3d(-82.84787179397482, 68.9847390401527, 30.895242327493126), Point3d(-61.03194322798886, 67.36487419298273, 30.88634973645842),
     Point3d(-33.924443240636386, 90.78233450985698, 18.19586051354917), Point3d(-33.75867049403901, 90.77299217258242, 18.19431383644973),
     Point3d(-1.946448792095577, 118.12378669561315, 3.366581720843165), Point3d(-1.64604883910226, 118.10016025919867, 3.367118066990641),
     Point3d(-7.85201868781445, 125.31922063111779, 0.0), Point3d(-73.99355670177661, 130.28440014311866, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-88.1587841936477, 69.74745652292202, 31.081099741796788),
                                  Point3d(-82.84787179397482, 68.9847390401527, 30.895242327493126),
                                  Point3d(-73.99355670177661, 130.28440014311866, 0.0), Point3d(-75.64892631317062, 130.64064637716376, 0.0),
                                  Point3d(-95.38349396241996, 70.77852777631168, 31.337105026142268)});
  expectedRoofPolygons.push_back(
    {Point3d(-96.05017083682696, 70.94750272264358, 31.397549305890063), Point3d(-95.38349396241996, 70.77852777631168, 31.337105026142268),
     Point3d(-75.64892631317062, 130.64064637716376, 0.0), Point3d(-76.60845471625578, 131.07482150365024, 0.0),
     Point3d(-93.0937410803582, 102.51935396169492, 16.40605343574981), Point3d(-93.079037281847, 95.258235108459, 19.71072105086341)});
  expectedRoofPolygons.push_back({Point3d(-93.0937410803582, 102.51935396169492, 16.40605343574981),
                                  Point3d(-76.60845471625578, 131.07482150365024, 0.0), Point3d(-77.91224140138345, 132.0099680511679, 0.0),
                                  Point3d(-85.44762264934579, 123.79275707510793, 5.534555323732934),
                                  Point3d(-93.04921921413377, 102.97870948112161, 16.20644492032387)});
  expectedRoofPolygons.push_back({Point3d(-85.44762264934579, 123.79275707510793, 5.534555323732934),
                                  Point3d(-77.91224140138345, 132.0099680511679, 0.0), Point3d(-78.85712051582951, 133.11210525048187, 0.0),
                                  Point3d(-81.62541506414908, 131.31555179454497, 1.6354919449422638)});
  expectedRoofPolygons.push_back({Point3d(-81.62541506414908, 131.31555179454497, 1.6354919449422638),
                                  Point3d(-78.85712051582951, 133.11210525048187, 0.0), Point3d(-79.47971528123003, 134.42576401589253, 0.0),
                                  Point3d(-80.57146143536926, 132.84569707591066, 0.8316288092653049)});
  expectedRoofPolygons.push_back({Point3d(-80.57146143536926, 132.84569707591066, 0.8316288092653049),
                                  Point3d(-79.47971528123003, 134.42576401589253, 0.0), Point3d(-80.4392436842769, 134.5036929371352, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-93.04921921413377, 102.97870948112161, 16.20644492032387), Point3d(-85.44762264934579, 123.79275707510793, 5.534555323732934),
     Point3d(-81.62541506414908, 131.31555179454497, 1.6354919449422638), Point3d(-80.57146143536926, 132.84569707591066, 0.8316288092653049),
     Point3d(-80.4392436842769, 134.5036929371352, 0.0), Point3d(-100.20845864548255, 136.05113884865818, 0.0),
     Point3d(-100.36640306568131, 134.06191051530428, 0.9977438086248633), Point3d(-99.52440324495109, 132.2216016700931, 1.8822386987350008),
     Point3d(-96.58802312702588, 122.94381884235717, 6.3924108008683405)});
  expectedRoofPolygons.push_back({Point3d(-100.36640306568131, 134.06191051530428, 0.9977438086248633),
                                  Point3d(-100.20845864548255, 136.05113884865818, 0.0), Point3d(-101.31447993453753, 136.14020049587162, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-99.52440324495109, 132.2216016700931, 1.8822386987350008),
                                  Point3d(-100.36640306568131, 134.06191051530428, 0.9977438086248633),
                                  Point3d(-101.31447993453753, 136.14020049587162, 0.0), Point3d(-102.2813329819231, 134.82654132846721, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-96.58802312702588, 122.94381884235717, 6.3924108008683405),
                                  Point3d(-99.52440324495109, 132.2216016700931, 1.8822386987350008),
                                  Point3d(-102.2813329819231, 134.82654132846721, 0.0), Point3d(-103.43862678102352, 133.8691289086305, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-93.079037281847, 95.258235108459, 19.71072105086341), Point3d(-93.0937410803582, 102.51935396169492, 16.40605343574981),
     Point3d(-93.04921921413377, 102.97870948112161, 16.20644492032387), Point3d(-96.58802312702588, 122.94381884235717, 6.3924108008683405),
     Point3d(-103.43862678102352, 133.8691289086305, 0.0), Point3d(-105.02074994949004, 133.14550334995624, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-96.05017083682696, 70.94750272264358, 31.397549305890063), Point3d(-93.079037281847, 95.258235108459, 19.71072105086341),
     Point3d(-105.02074994949004, 133.14550334995624, 0.0), Point3d(-106.35383521178105, 132.900583951564, 0.0),
     Point3d(-100.25538994648302, 70.71565252737079, 31.131625927357657), Point3d(-96.05017083682696, 70.94750272264358, 31.397549305890063)});
  expectedRoofPolygons.push_back({Point3d(-100.25538994648302, 70.71565252737079, 31.131625927357657),
                                  Point3d(-106.35383521178105, 132.900583951564, 0.0), Point3d(-107.15954608457682, 132.88945125194346, 0.0),
                                  Point3d(-109.12205517651584, 70.97696845487435, 30.9397298648524),
                                  Point3d(-103.07315010584149, 70.79700241858225, 31.071489889292227)});
  expectedRoofPolygons.push_back(
    {Point3d(-130.04846339460948, 72.55459509445708, 30.960132695532852), Point3d(-109.12205517651584, 70.97696845487435, 30.9397298648524),
     Point3d(-107.15954608457682, 132.88945125194346, 0.0), Point3d(-173.9456527967258, 138.0550262829267, 0.0),
     Point3d(-180.95914413698014, 131.72465762799146, 3.4261808938560043), Point3d(-154.41532924384578, 100.78324415201128, 17.827359685098976),
     Point3d(-154.37302261423963, 100.78018553816237, 17.82725320576591)});
  expectedRoofPolygons.push_back(
    {Point3d(-182.83155419538986, 133.9549226026316, 4.390085434491951), Point3d(-180.95226691409226, 131.8612428089489, 3.4244578005396242),
     Point3d(-180.95914413698014, 131.72465762799146, 3.4261808938560043), Point3d(-173.9456527967258, 138.0550262829267, 0.0),
     Point3d(-173.83578313229629, 142.43018301674232, 0.0), Point3d(-182.80884697953078, 134.36866502388702, 4.383928751811345)});
  expectedRoofPolygons.push_back({Point3d(-182.80884697953078, 134.36866502388702, 4.383928751811345),
                                  Point3d(-173.83578313229629, 142.43018301674232, 0.0), Point3d(-190.80698397153176, 143.8217730301796, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-182.83155419538986, 133.9549226026316, 4.390085434491951),
                                  Point3d(-182.80884697953078, 134.36866502388702, 4.383928751811345),
                                  Point3d(-190.80698397153176, 143.8217730301796, 0.0), Point3d(-192.32318534125633, 125.93151831483829, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-182.83155419538986, 133.9549226026316, 4.390085434491951),
                                  Point3d(-192.32318534125633, 125.93151831483829, 0.0), Point3d(-188.2946309771817, 125.5975377539537, 0.0),
                                  Point3d(-180.95226691409226, 131.8612428089489, 3.4244578005396242)});
  expectedRoofPolygons.push_back(
    {Point3d(-154.41532924384578, 100.78324415201128, 17.827359685098976), Point3d(-180.95914413698014, 131.72465762799146, 3.4261808938560043),
     Point3d(-180.95226691409226, 131.8612428089489, 3.4244578005396242), Point3d(-188.2946309771817, 125.5975377539537, 0.0),
     Point3d(-191.62734413292836, 81.5456769983088, 0.0), Point3d(-185.14914103411456, 74.14850324834707, 3.508888225261896)});
  expectedRoofPolygons.push_back(
    {Point3d(-185.14914103411456, 74.14850324834707, 3.508888225261896), Point3d(-191.62734413292836, 81.5456769983088, 0.0),
     Point3d(-192.41108107288036, 81.5902073856052, 0.0), Point3d(-191.8833170920917, 80.97823536977444, 0.29052418358597576),
     Point3d(-191.75885179775784, 80.9582255955295, 0.2969827178222758), Point3d(-185.74646660333013, 74.18555712287022, 3.507333226389516)});
  expectedRoofPolygons.push_back({Point3d(-191.8833170920917, 80.97823536977444, 0.29052418358597576),
                                  Point3d(-192.41108107288036, 81.5902073856052, 0.0), Point3d(-192.50630144874737, 80.54374334940206, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-191.75885179775784, 80.9582255955295, 0.2969827178222758),
                                  Point3d(-191.8833170920917, 80.97823536977444, 0.29052418358597576),
                                  Point3d(-192.50630144874737, 80.54374334940206, 0.0), Point3d(-192.38178249564044, 80.51034556245332, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-185.74646660333013, 74.18555712287022, 3.507333226389516),
                                  Point3d(-191.75885179775784, 80.9582255955295, 0.2969827178222758),
                                  Point3d(-192.38178249564044, 80.51034556245332, 0.0), Point3d(-193.13622085837167, 68.36469764229747, 0.0),
                                  Point3d(-192.790802030354, 67.9971570595962, 0.18377029135063339)});
  expectedRoofPolygons.push_back({Point3d(-192.790802030354, 67.9971570595962, 0.18377029135063339),
                                  Point3d(-193.13622085837167, 68.36469764229747, 0.0), Point3d(-193.24609052287775, 68.36469764229747, 0.0),
                                  Point3d(-192.91171294260738, 68.00121817853841, 0.18173973187953618)});
  expectedRoofPolygons.push_back({Point3d(-192.91171294260738, 68.00121817853841, 0.18173973187953618),
                                  Point3d(-193.24609052287775, 68.36469764229747, 0.0), Point3d(-193.30468767722357, 67.66334572802566, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-154.41532924384578, 100.78324415201128, 17.827359685098976), Point3d(-185.14914103411456, 74.14850324834707, 3.508888225261896),
     Point3d(-185.74646660333013, 74.18555712287022, 3.507333226389516), Point3d(-192.790802030354, 67.9971570595962, 0.18377029135063339),
     Point3d(-192.91171294260738, 68.00121817853841, 0.18173973187953618), Point3d(-193.30468767722357, 67.66334572802566, 0.0),
     Point3d(-192.47700287154572, 67.60768288193731, 0.0), Point3d(-154.37302261423963, 100.78018553816237, 17.82725320576591)});
  expectedRoofPolygons.push_back(
    {Point3d(-130.04846339460948, 72.55459509445708, 30.960132695532852), Point3d(-154.37302261423963, 100.78018553816237, 17.82725320576591),
     Point3d(-192.47700287154572, 67.60768288193731, 0.0), Point3d(-195.63392456412095, 23.222306088178982, 0.0),
     Point3d(-189.52636703457816, 15.99127614854962, 3.302590186602625), Point3d(-151.7889624016801, 48.580184914836835, 20.967719274937355),
     Point3d(-151.395684217932, 54.085295129682756, 20.96857998067297)});
  expectedRoofPolygons.push_back({Point3d(-191.86576355588355, 14.406244269410973, 4.204720228103409),
                                  Point3d(-189.93997467912087, 16.026976659688845, 3.3048631888427127),
                                  Point3d(-189.52636703457816, 15.99127614854962, 3.302590186602625),
                                  Point3d(-195.63392456412095, 23.222306088178982, 0.0), Point3d(-199.75037465980046, 23.623075402548864, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-191.86576355588355, 14.406244269410973, 4.204720228103409),
                                  Point3d(-199.75037465980046, 23.623075402548864, 0.0), Point3d(-200.28507369355776, 14.483315691783218, 0.0),
                                  Point3d(-191.90341501448117, 13.840861024550493, 4.202436577519525)});
  expectedRoofPolygons.push_back(
    {Point3d(-191.94222460277786, 13.383001452589115, 4.204734984692814), Point3d(-191.90341501448117, 13.840861024550493, 4.202436577519525),
     Point3d(-200.28507369355776, 14.483315691783218, 0.0), Point3d(-201.0981092106731, 5.911325686340149, 0.0),
     Point3d(-200.83366413488636, 5.898812731214532, 0.13222253789336946), Point3d(-199.05930751289122, 7.3841722081856656, 0.9453100802665045)});
  expectedRoofPolygons.push_back({Point3d(-200.83366413488636, 5.898812731214532, 0.13222253789336946),
                                  Point3d(-201.0981092106731, 5.911325686340149, 0.0), Point3d(-201.0981092106731, 5.655279433782141, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-199.05930751289122, 7.3841722081856656, 0.9453100802665045),
                                  Point3d(-200.83366413488636, 5.898812731214532, 0.13222253789336946),
                                  Point3d(-201.0981092106731, 5.655279433782141, 0.0), Point3d(-199.20835098172356, 5.499425199435763, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-191.94222460277786, 13.383001452589115, 4.204734984692814),
                                  Point3d(-199.05930751289122, 7.3841722081856656, 0.9453100802665045),
                                  Point3d(-199.20835098172356, 5.499425199435763, 0.0), Point3d(-188.32392955442162, 4.675624318771895, 0.0),
                                  Point3d(-187.99625509168078, 8.795385136091523, 2.066370578582637)});
  expectedRoofPolygons.push_back({Point3d(-187.99625509168078, 8.795385136091523, 2.066370578582637),
                                  Point3d(-188.32392955442162, 4.675624318771895, 0.0), Point3d(-184.18550552584088, 4.33051857910408, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-191.94222460277786, 13.383001452589115, 4.204734984692814), Point3d(-187.99625509168078, 8.795385136091523, 2.066370578582637),
     Point3d(-184.18550552584088, 4.33051857910408, 0.0), Point3d(-183.84124724381752, 8.939351818832364, 0.0),
     Point3d(-189.93997467912087, 16.026976659688845, 3.3048631888427127), Point3d(-191.86576355588355, 14.406244269410973, 4.204720228103409),
     Point3d(-191.90341501448117, 13.840861024550493, 4.202436577519525)});
  expectedRoofPolygons.push_back(
    {Point3d(-151.7889624016801, 48.580184914836835, 20.967719274937355), Point3d(-189.52636703457816, 15.99127614854962, 3.302590186602625),
     Point3d(-189.93997467912087, 16.026976659688845, 3.3048631888427127), Point3d(-183.84124724381752, 8.939351818832364, 0.0),
     Point3d(-145.6944997381005, 6.067179931716822, 0.0), Point3d(-128.16598548965533, 21.207923480625293, 8.20702651237758)});
  expectedRoofPolygons.push_back(
    {Point3d(-128.16598548965533, 21.207923480625293, 8.20702651237758), Point3d(-145.6944997381005, 6.067179931716822, 0.0),
     Point3d(-148.39729348418453, -32.06131821982851, 0.0), Point3d(-142.36398338374596, -39.093021096907265, 3.2577067152762935),
     Point3d(-139.28829638605566, 4.232583370577451, 3.2599455237183004), Point3d(-133.69801956583996, 9.052755916650366, 5.8776728297469125),
     Point3d(-133.13637875553084, 16.787190970589442, 5.88434318849204), Point3d(-128.1716717205728, 21.128194314745024, 8.207009298740529)});
  expectedRoofPolygons.push_back({Point3d(-144.25945998535835, -40.71903018667486, 4.145405021871242),
                                  Point3d(-142.36398338374596, -39.093021096907265, 3.2577067152762935),
                                  Point3d(-148.39729348418453, -32.06131821982851, 0.0), Point3d(-152.87997579476948, -31.693949992923137, 0.0),
                                  Point3d(-145.2751618131424, -40.62270950905692, 4.138886325567087)});
  expectedRoofPolygons.push_back({Point3d(-145.2751618131424, -40.62270950905692, 4.138886325567087),
                                  Point3d(-152.87997579476948, -31.693949992923137, 0.0), Point3d(-154.15446390269554, -47.99171711813307, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-144.25945998535835, -40.71903018667486, 4.145405021871242),
                                  Point3d(-145.2751618131424, -40.62270950905692, 4.138886325567087),
                                  Point3d(-154.15446390269554, -47.99171711813307, 0.0), Point3d(-144.44198556309482, -49.038156822071414, 0.0),
                                  Point3d(-143.63155810770348, -41.47020488624024, 3.8056098352328)});
  expectedRoofPolygons.push_back({Point3d(-143.63155810770348, -41.47020488624024, 3.8056098352328),
                                  Point3d(-144.44198556309482, -49.038156822071414, 0.0), Point3d(-136.5972915195616, -49.87308197764537, 0.0),
                                  Point3d(-136.70487119110868, -49.71535770209509, 0.07272643410188817),
                                  Point3d(-140.37958427782107, -45.35550673546696, 2.0459540674851255)});
  expectedRoofPolygons.push_back({Point3d(-136.70487119110868, -49.71535770209509, 0.07272643410188817),
                                  Point3d(-136.5972915195616, -49.87308197764537, 0.0), Point3d(-136.56066829809785, -49.739493961847415, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-140.37958427782107, -45.35550673546696, 2.0459540674851255),
                                  Point3d(-136.70487119110868, -49.71535770209509, 0.07272643410188817),
                                  Point3d(-136.56066829809785, -49.739493961847415, 0.0), Point3d(-136.29698110334067, -45.63166089550802, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-144.25945998535835, -40.71903018667486, 4.145405021871242), Point3d(-143.63155810770348, -41.47020488624024, 3.8056098352328),
     Point3d(-140.37958427782107, -45.35550673546696, 2.0459540674851255), Point3d(-136.29698110334067, -45.63166089550802, 0.0),
     Point3d(-133.24992907525242, -2.7719754218481114, 0.0), Point3d(-139.28829638605566, 4.232583370577451, 3.2599455237183004),
     Point3d(-142.36398338374596, -39.093021096907265, 3.2577067152762935)});
  expectedRoofPolygons.push_back({Point3d(-133.69801956583996, 9.052755916650366, 5.8776728297469125),
                                  Point3d(-139.28829638605566, 4.232583370577451, 3.2599455237183004),
                                  Point3d(-133.24992907525242, -2.7719754218481114, 0.0), Point3d(-122.84893417162414, -3.573510226744844, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-133.13637875553084, 16.787190970589442, 5.88434318849204),
                                  Point3d(-133.69801956583996, 9.052755916650366, 5.8776728297469125),
                                  Point3d(-122.84893417162414, -3.573510226744844, 0.0), Point3d(-122.2629626277448, 4.308253694611125, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-128.1716717205728, 21.128194314745024, 8.207009298740529), Point3d(-133.13637875553084, 16.787190970589442, 5.88434318849204),
     Point3d(-122.2629626277448, 4.308253694611125, 0.0), Point3d(-116.62298651802115, 3.952015535370265, 0.0),
     Point3d(-115.71209195305535, 4.760665687614364, 0.4322311704283591), Point3d(-115.8930581774467, 4.971317216729401, 0.5316436353681535),
     Point3d(-115.78567551496056, 6.95029279409076, 1.5225480580334527)});
  expectedRoofPolygons.push_back({Point3d(-115.71209195305535, 4.760665687614364, 0.4322311704283591),
                                  Point3d(-116.62298651802115, 3.952015535370265, 0.0), Point3d(-116.69623296100607, 2.638387525545255, 0.0),
                                  Point3d(-115.80810094628717, 3.42891897234015, 0.4213719027043363)});
  expectedRoofPolygons.push_back({Point3d(-115.80810094628717, 3.42891897234015, 0.4213719027043363),
                                  Point3d(-116.69623296100607, 2.638387525545255, 0.0), Point3d(-115.04086334961207, 2.538195570451535, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-115.8930581774467, 4.971317216729401, 0.5316436353681535),
                                  Point3d(-115.71209195305535, 4.760665687614364, 0.4322311704283591),
                                  Point3d(-115.80810094628717, 3.42891897234015, 0.4213719027043363),
                                  Point3d(-115.04086334961207, 2.538195570451535, 0.0), Point3d(-114.92366904082473, 3.8629559986240167, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-115.78567551496056, 6.95029279409076, 1.5225480580334527),
                                  Point3d(-115.8930581774467, 4.971317216729401, 0.5316436353681535),
                                  Point3d(-114.92366904082473, 3.8629559986240167, 0.0), Point3d(-112.96066436885195, 3.77389646371617, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-151.395684217932, 54.085295129682756, 20.96857998067297), Point3d(-151.7889624016801, 48.580184914836835, 20.967719274937355),
     Point3d(-128.16598548965533, 21.207923480625293, 8.20702651237758), Point3d(-128.1716717205728, 21.128194314745024, 8.207009298740529),
     Point3d(-115.78567551496056, 6.95029279409076, 1.5225480580334527), Point3d(-112.96066436885195, 3.77389646371617, 0.0),
     Point3d(-112.57245822106444, 9.184265769474024, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-103.07315010584149, 70.79700241858225, 31.071489889292227), Point3d(-109.12205517651584, 70.97696845487435, 30.9397298648524),
     Point3d(-130.04846339460948, 72.55459509445708, 30.960132695532852), Point3d(-151.395684217932, 54.085295129682756, 20.96857998067297),
     Point3d(-112.57245822106444, 9.184265769474024, 0.0), Point3d(-107.56972616530382, 8.816894846881922, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-96.05017083682696, 70.94750272264358, 31.397549305890063), Point3d(-100.25538994648302, 70.71565252737079, 31.131625927357657),
     Point3d(-103.07315010584149, 70.79700241858225, 31.071489889292227), Point3d(-107.56972616530382, 8.816894846881922, 0.0),
     Point3d(-92.65675037390812, 7.748179577427749, 0.0), Point3d(-88.1587841936477, 69.74745652292202, 31.081099741796788),
     Point3d(-95.38349396241996, 70.77852777631168, 31.337105026142268)});
  expectedRoofPolygons.push_back(
    {Point3d(-88.1587841936477, 69.74745652292202, 31.081099741796788), Point3d(-92.65675037390812, 7.748179577427749, 0.0),
     Point3d(-87.65401831812835, 7.380808750431726, 0.0), Point3d(-42.70885984371319, 46.18933004077511, 20.997972658943333),
     Point3d(-61.03194322798886, 67.36487419298273, 30.88634973645842), Point3d(-82.84787179397482, 68.9847390401527, 30.895242327493126)});
  expectedRoofPolygons.push_back(
    {Point3d(-43.10454118850721, 40.69777951729232, 21.00085716757909), Point3d(-42.70885984371319, 46.18933004077511, 20.997972658943333),
     Point3d(-87.65401831812835, 7.380808750431726, 0.0), Point3d(-88.04222446591587, 2.070633143170312, 0.0),
     Point3d(-70.40372469188209, 16.961036189377545, 8.252935835825348), Point3d(-70.39852981923862, 17.030466134490204, 8.25299523789691)});
  expectedRoofPolygons.push_back({Point3d(-70.40372469188209, 16.961036189377545, 8.252935835825348),
                                  Point3d(-88.04222446591587, 2.070633143170312, 0.0), Point3d(-78.63738118687476, 1.1689057121689643, 0.0),
                                  Point3d(-66.13181959009381, 11.883176833821494, 5.929454466446827)});
  expectedRoofPolygons.push_back({Point3d(-66.13181959009381, 11.883176833821494, 5.929454466446827),
                                  Point3d(-78.63738118687476, 1.1689057121689643, 0.0), Point3d(-79.09883377766633, -6.7239861902833695, 0.0),
                                  Point3d(-66.66517916226239, 3.98104312167081, 5.893831962137644)});
  expectedRoofPolygons.push_back({Point3d(-66.66517916226239, 3.98104312167081, 5.893831962137644),
                                  Point3d(-79.09883377766633, -6.7239861902833695, 0.0), Point3d(-69.18859004203122, -7.625712198986719, 0.0),
                                  Point3d(-62.00204163227642, -1.5346672314106609, 3.358597598847626)});
  expectedRoofPolygons.push_back(
    {Point3d(-63.474199556768475, -46.98678351805689, 4.302523541774291), Point3d(-65.26752960327565, -44.90808428113797, 3.3315648755263947),
     Point3d(-62.00204163227642, -1.5346672314106609, 3.358597598847626), Point3d(-69.18859004203122, -7.625712198986719, 0.0),
     Point3d(-72.68244537234855, -54.81583632791774, 0.0), Point3d(-63.86680204425184, -47.32114798065856, 4.119102145601586)});
  expectedRoofPolygons.push_back({Point3d(-63.86680204425184, -47.32114798065856, 4.119102145601586),
                                  Point3d(-72.68244537234855, -54.81583632791774, 0.0), Point3d(-64.58138877837769, -55.528305063057076, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-59.40280006237284, -47.311571131294606, 4.31553836306936), Point3d(-63.474199556768475, -46.98678351805689, 4.302523541774291),
     Point3d(-63.86680204425184, -47.32114798065856, 4.119102145601586), Point3d(-64.58138877837769, -55.528305063057076, 0.0),
     Point3d(-51.536197283087745, -56.652669598441975, 0.0), Point3d(-55.8858137547639, -51.48307593289708, 2.388494895939638)});
  expectedRoofPolygons.push_back({Point3d(-55.8858137547639, -51.48307593289708, 2.388494895939638),
                                  Point3d(-51.536197283087745, -56.652669598441975, 0.0), Point3d(-51.126017202341586, -51.888034167850435, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-59.40280006237284, -47.311571131294606, 4.31553836306936),
                                  Point3d(-55.8858137547639, -51.48307593289708, 2.388494895939638),
                                  Point3d(-51.126017202341586, -51.888034167850435, 0.0), Point3d(-50.07126842338943, -39.34188353361671, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-59.40280006237284, -47.311571131294606, 4.31553836306936),
                                  Point3d(-50.07126842338943, -39.34188353361671, 0.0), Point3d(-58.11372786299535, -38.75186865681468, 0.0),
                                  Point3d(-65.26752960327565, -44.90808428113797, 3.3315648755263947),
                                  Point3d(-63.474199556768475, -46.98678351805689, 4.302523541774291)});
  expectedRoofPolygons.push_back(
    {Point3d(-70.39852981923862, 17.030466134490204, 8.25299523789691), Point3d(-70.40372469188209, 16.961036189377545, 8.252935835825348),
     Point3d(-66.13181959009381, 11.883176833821494, 5.929454466446827), Point3d(-66.66517916226239, 3.98104312167081, 5.893831962137644),
     Point3d(-62.00204163227642, -1.5346672314106609, 3.358597598847626), Point3d(-65.26752960327565, -44.90808428113797, 3.3315648755263947),
     Point3d(-58.11372786299535, -38.75186865681468, 0.0), Point3d(-55.19119478795656, -0.5677541139705511, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-43.10454118850721, 40.69777951729232, 21.00085716757909),
                                  Point3d(-70.39852981923862, 17.030466134490204, 8.25299523789691),
                                  Point3d(-55.19119478795656, -0.5677541139705511, 0.0), Point3d(-37.17989445833143, -1.81458647758825, 0.0),
                                  Point3d(-34.5562630110808, 30.860444838236432, 16.389103917802835)});
  expectedRoofPolygons.push_back({Point3d(-34.5562630110808, 30.860444838236432, 16.389103917802835),
                                  Point3d(-37.17989445833143, -1.81458647758825, 0.0), Point3d(-34.86530686005406, -2.0261026523114936, 0.0),
                                  Point3d(-32.31508760437524, 28.223224732309113, 15.177944303266205)});
  expectedRoofPolygons.push_back({Point3d(-32.31508760437524, 28.223224732309113, 15.177944303266205),
                                  Point3d(-34.86530686005406, -2.0261026523114936, 0.0), Point3d(-17.42532878570745, -3.373126538389357, 0.0),
                                  Point3d(-10.26903498604995, 2.812246226874768, 3.3590496577074944),
                                  Point3d(-10.420788871410501, 2.8235982695492625, 3.3588656661105745)});
  expectedRoofPolygons.push_back(
    {Point3d(-8.55721925107371, 0.590431430331766, 4.288679668005958), Point3d(-8.54200371397651, 0.8238610560514159, 4.288315220750146),
     Point3d(-10.26903498604995, 2.812246226874768, 3.3590496577074944), Point3d(-17.42532878570745, -3.373126538389357, 0.0),
     Point3d(-17.703665269065453, -7.447593490720266, 0.0), Point3d(-8.601175396264287, -0.018974469694361673, 4.287519279647097)});
  expectedRoofPolygons.push_back({Point3d(-8.601175396264287, -0.018974469694361673, 4.287519279647097),
                                  Point3d(-17.703665269065453, -7.447593490720266, 0.0), Point3d(-9.338921480375689, -8.571967728493888, 0.0),
                                  Point3d(-8.487318836690127, -0.15957288669922365, 4.225430676645588)});
  expectedRoofPolygons.push_back({Point3d(-8.487318836690127, -0.15957288669922365, 4.225430676645588),
                                  Point3d(-9.338921480375689, -8.571967728493888, 0.0), Point3d(-0.695841208347133, -9.161985802862006, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-8.55721925107371, 0.590431430331766, 4.288679668005958),
                                  Point3d(-8.601175396264287, -0.018974469694361673, 4.287519279647097),
                                  Point3d(-8.487318836690127, -0.15957288669922365, 4.225430676645588),
                                  Point3d(-0.695841208347133, -9.161985802862006, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-8.55721925107371, 0.590431430331766, 4.288679668005958), Point3d(0.0, 0.0, 0.0), Point3d(0.4980758122744646, 8.02649082539466, 0.0),
     Point3d(0.15402882814217608, 8.424913516260558, 0.1840313674902289), Point3d(0.15399233874051674, 8.42451956393686, 0.18403737747332619),
     Point3d(-8.54200371397651, 0.8238610560514159, 4.288315220750146)});
  expectedRoofPolygons.push_back({Point3d(0.22498766501530973, 8.482519288941951, 0.21571726906224928),
                                  Point3d(0.15402882814217608, 8.424913516260558, 0.1840313674902289),
                                  Point3d(0.4980758122744646, 8.02649082539466, 0.0), Point3d(0.6299194096243355, 8.015358375794648, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofExtendedSkeleton_5) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {-0.6958026548881477, -9.161478178076948, 0.0},
    {-9.338404052373633, -8.571492793979743, 0.0},
    {-17.70268438790386, -7.447180852769691, 0.0},
    {-17.424363325929452, -3.372939648486792, 0.0},
    {-16.142621593290144, 9.773744233129612, 0.0},
    {-12.810093088301556, 54.43485287683303, 0.0},
    {-32.90780345674236, 55.90426731787754, 0.0},
    {-50.625136321556965, 57.284626693320064, 0.0},
    {-49.63636412778366, 70.36466192934482, 0.0},
    {-29.34822355908187, 68.67260457721952, 0.0},
    {-11.997102617897646, 67.4814855658616, 0.0},
    {-8.642601397542933, 112.13192467030846, 0.0},
    {-7.851583642466842, 125.31227725183234, 0.0},
    {-7.492695957349284, 130.04340803855604, 0.0},
    {9.60940087585437, 128.6518985681851, 0.0},
    {9.235864713749464, 123.92076896347811, 0.0},
    {8.181174373724609, 110.5289109392406, 0.0},
    {4.255382552472042, 110.85174005752916, 0.0},
    {0.9448267629261693, 67.14752700944396, 0.0},
    {1.8090869026900336, 67.01394359300451, 0.0},
    {1.7578172333683781, 66.123387566131, 0.0},
    {1.567387033088238, 66.15678341380686, 0.0},
    {0.6591814624963845, 53.8559927457046, 0.0},
    {0.8935570936236559, 53.87825659819807, 0.0},
    {0.7690450395950705, 53.05449420912991, 0.0},
    {-0.11718781561150848, 53.08788997837232, 0.0},
    {-3.2446376433134674, 9.039043047081556, 0.0},
    {-0.16845748485657452, 8.816406342048168, 0.0},
    {-0.14648476947130007, 8.994515704236598, 0.0},
    {0.6811541779199501, 8.894329185746045, 0.0},
    {0.6298845085982947, 8.014914280523122, 0.0},
    {0.49804821611433425, 8.026046113323751, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.22497519945185107, 8.482049310367099, 0.2157053171301373),
                                  Point3d(0.6298845085982947, 8.014914280523122, 0.0), Point3d(0.6811541779199501, 8.894329185746045, 0.0),
                                  Point3d(0.228030279440679, 8.51571861719398, 0.215160158897514)});
  expectedRoofPolygons.push_back({Point3d(0.228030279440679, 8.51571861719398, 0.215160158897514),
                                  Point3d(0.6811541779199501, 8.894329185746045, 0.0), Point3d(-0.14648476947130007, 8.994515704236598, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(0.22497519945185107, 8.482049310367099, 0.2157053171301373), Point3d(0.228030279440679, 8.51571861719398, 0.215160158897514),
     Point3d(-0.14648476947130007, 8.994515704236598, 0.0), Point3d(-0.16845748485657452, 8.816406342048168, 0.0),
     Point3d(0.1539838067115609, 8.424052798868134, 0.18402718078276917), Point3d(0.15402029409151105, 8.424446729364753, 0.1840211711326573)});
  expectedRoofPolygons.push_back({Point3d(-8.46470046215179, 0.8909679724959663, 4.251815927287789),
                                  Point3d(0.1539838067115609, 8.424052798868134, 0.18402718078276917),
                                  Point3d(-0.16845748485657452, 8.816406342048168, 0.0), Point3d(-3.2446376433134674, 9.039043047081556, 0.0),
                                  Point3d(-10.232090839434656, 2.9863950680172815, 3.2706256994743454)});
  expectedRoofPolygons.push_back({Point3d(-10.232090839434656, 2.9863950680172815, 3.2706256994743454),
                                  Point3d(-3.2446376433134674, 9.039043047081556, 0.0), Point3d(-0.11718781561150848, 53.08788997837232, 0.0),
                                  Point3d(-6.0328581287001075, 59.683236018902456, 3.183953597591316),
                                  Point3d(-9.707017583147685, 9.219988757292096, 3.2294839184122597)});
  expectedRoofPolygons.push_back(
    {Point3d(-5.638142363831541, 60.035994979870125, 3.367639712679712), Point3d(-6.0328581287001075, 59.683236018902456, 3.183953597591316),
     Point3d(-0.11718781561150848, 53.08788997837232, 0.0), Point3d(0.7690450395950705, 53.05449420912991, 0.0),
     Point3d(0.4412980045808891, 53.45034350926698, 0.19161343391084962), Point3d(0.2568122667896132, 53.445080273838485, 0.18551017486326438)});
  expectedRoofPolygons.push_back({Point3d(0.4412980045808891, 53.45034350926698, 0.19161343391084962),
                                  Point3d(0.7690450395950705, 53.05449420912991, 0.0), Point3d(0.8935570936236559, 53.87825659819807, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.4412980045808891, 53.45034350926698, 0.19161343391084962),
                                  Point3d(0.8935570936236559, 53.87825659819807, 0.0), Point3d(0.6591814624963845, 53.8559927457046, 0.0),
                                  Point3d(0.2568122667896132, 53.445080273838485, 0.18551017486326438)});
  expectedRoofPolygons.push_back(
    {Point3d(-5.667182887598463, 60.74743072530452, 3.4083130185782604), Point3d(-5.591816917235063, 60.66011175458134, 3.367517561592367),
     Point3d(-5.638142363831541, 60.035994979870125, 3.367639712679712), Point3d(0.2568122667896132, 53.445080273838485, 0.18551017486326438),
     Point3d(0.6591814624963845, 53.8559927457046, 0.0), Point3d(1.567387033088238, 66.15678341380686, 0.0),
     Point3d(1.166988791907558, 66.67083534361724, 0.2185811766391756), Point3d(-5.628537161719527, 61.26630072622317, 3.40814550327218)});
  expectedRoofPolygons.push_back({Point3d(1.3478704650397642, 66.64099894032651, 0.219509467027797),
                                  Point3d(1.166988791907558, 66.67083534361724, 0.2185811766391756),
                                  Point3d(1.567387033088238, 66.15678341380686, 0.0), Point3d(1.7578172333683781, 66.123387566131, 0.0)});
  expectedRoofPolygons.push_back({Point3d(1.3478704650397642, 66.64099894032651, 0.219509467027797),
                                  Point3d(1.7578172333683781, 66.123387566131, 0.0), Point3d(1.8090869026900336, 67.01394359300451, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(-5.628537161719527, 61.26630072622317, 3.40814550327218), Point3d(1.166988791907558, 66.67083534361724, 0.2185811766391756),
     Point3d(1.3478704650397642, 66.64099894032651, 0.219509467027797), Point3d(1.8090869026900336, 67.01394359300451, 0.0),
     Point3d(0.9448267629261693, 67.14752700944396, 0.0), Point3d(-5.951801518399619, 61.673302877522865, 3.2317218692160656)});
  expectedRoofPolygons.push_back({Point3d(-1.7573361753414014, 117.89581731960725, 3.2638003651479477),
                                  Point3d(-2.1782246133383856, 111.69514191374755, 3.239466436905973),
                                  Point3d(-5.951801518399619, 61.673302877522865, 3.2317218692160656),
                                  Point3d(0.9448267629261693, 67.14752700944396, 0.0), Point3d(4.255382552472042, 110.85174005752916, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.35384514304874415, 119.72666090736905, 4.262654846857816),
                                  Point3d(-1.7573361753414014, 117.89581731960725, 3.2638003651479477),
                                  Point3d(4.255382552472042, 110.85174005752916, 0.0), Point3d(8.181174373724609, 110.5289109392406, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.43073023369972396, 120.83484356072353, 4.267834328351732),
                                  Point3d(0.35384514304874415, 119.72666090736905, 4.262654846857816),
                                  Point3d(8.181174373724609, 110.5289109392406, 0.0), Point3d(9.235864713749464, 123.92076896347811, 0.0),
                                  Point3d(4.492881435278868, 124.29477438423137, 2.3788532015561956)});
  expectedRoofPolygons.push_back({Point3d(4.492881435278868, 124.29477438423137, 2.3788532015561956),
                                  Point3d(9.235864713749464, 123.92076896347811, 0.0), Point3d(9.60940087585437, 128.6518985681851, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.43073023369972396, 120.83484356072353, 4.267834328351732),
                                  Point3d(4.492881435278868, 124.29477438423137, 2.3788532015561956),
                                  Point3d(9.60940087585437, 128.6518985681851, 0.0), Point3d(-7.492695957349284, 130.04340803855604, 0.0),
                                  Point3d(-3.1800990745474937, 124.99493613135351, 2.341052683524369)});
  expectedRoofPolygons.push_back({Point3d(-3.1800990745474937, 124.99493613135351, 2.341052683524369),
                                  Point3d(-7.492695957349284, 130.04340803855604, 0.0), Point3d(-7.851583642466842, 125.31227725183234, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(0.43073023369972396, 120.83484356072353, 4.267834328351732), Point3d(-3.1800990745474937, 124.99493613135351, 2.341052683524369),
     Point3d(-7.851583642466842, 125.31227725183234, 0.0), Point3d(-8.642601397542933, 112.13192467030846, 0.0),
     Point3d(-2.1782246133383856, 111.69514191374755, 3.239466436905973), Point3d(-1.7573361753414014, 117.89581731960725, 3.2638003651479477),
     Point3d(0.35384514304874415, 119.72666090736905, 4.262654846857816)});
  expectedRoofPolygons.push_back(
    {Point3d(-5.667182887598463, 60.74743072530452, 3.4083130185782604), Point3d(-5.628537161719527, 61.26630072622317, 3.40814550327218),
     Point3d(-5.951801518399619, 61.673302877522865, 3.2317218692160656), Point3d(-2.1782246133383856, 111.69514191374755, 3.239466436905973),
     Point3d(-8.642601397542933, 112.13192467030846, 0.0), Point3d(-11.997102617897646, 67.4814855658616, 0.0),
     Point3d(-5.952986384098678, 60.50107151272664, 3.2750410954001192)});
  expectedRoofPolygons.push_back({Point3d(-5.952986384098678, 60.50107151272664, 3.2750410954001192),
                                  Point3d(-11.997102617897646, 67.4814855658616, 0.0), Point3d(-29.34822355908187, 68.67260457721952, 0.0),
                                  Point3d(-29.840711971214105, 62.194242804209985, 3.248439840097433)});
  expectedRoofPolygons.push_back({Point3d(-43.61111816270654, 63.298712425414124, 3.27036381978287),
                                  Point3d(-32.41761135191654, 62.395898981650134, 3.2550471941541264),
                                  Point3d(-29.840711971214105, 62.194242804209985, 3.248439840097433),
                                  Point3d(-29.34822355908187, 68.67260457721952, 0.0), Point3d(-49.63636412778366, 70.36466192934482, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-43.61111816270654, 63.298712425414124, 3.27036381978287),
                                  Point3d(-49.63636412778366, 70.36466192934482, 0.0), Point3d(-50.625136321556965, 57.284626693320064, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-43.61111816270654, 63.298712425414124, 3.27036381978287),
                                  Point3d(-50.625136321556965, 57.284626693320064, 0.0), Point3d(-32.90780345674236, 55.90426731787754, 0.0),
                                  Point3d(-32.41761135191654, 62.395898981650134, 3.2550471941541264)});
  expectedRoofPolygons.push_back(
    {Point3d(-5.667182887598463, 60.74743072530452, 3.4083130185782604), Point3d(-5.952986384098678, 60.50107151272664, 3.2750410954001192),
     Point3d(-29.840711971214105, 62.194242804209985, 3.248439840097433), Point3d(-32.41761135191654, 62.395898981650134, 3.2550471941541264),
     Point3d(-32.90780345674236, 55.90426731787754, 0.0), Point3d(-12.810093088301556, 54.43485287683303, 0.0),
     Point3d(-5.591816917235063, 60.66011175458134, 3.367517561592367)});
  expectedRoofPolygons.push_back(
    {Point3d(-5.638142363831541, 60.035994979870125, 3.367639712679712), Point3d(-5.591816917235063, 60.66011175458134, 3.367517561592367),
     Point3d(-12.810093088301556, 54.43485287683303, 0.0), Point3d(-16.142621593290144, 9.773744233129612, 0.0),
     Point3d(-9.707017583147685, 9.219988757292096, 3.2294839184122597), Point3d(-6.0328581287001075, 59.683236018902456, 3.183953597591316)});
  expectedRoofPolygons.push_back(
    {Point3d(-8.547206138620584, -0.08503012741680582, 4.258110923914474), Point3d(-8.489043344467849, 0.5857274258099088, 4.254511486038224),
     Point3d(-8.46470046215179, 0.8909679724959663, 4.251815927287789), Point3d(-10.232090839434656, 2.9863950680172815, 3.2706256994743454),
     Point3d(-9.707017583147685, 9.219988757292096, 3.2294839184122597), Point3d(-16.142621593290144, 9.773744233129612, 0.0),
     Point3d(-17.424363325929452, -3.372939648486792, 0.0), Point3d(-13.145027272273616, -3.7276383085200995, 2.1467799499206666)});
  expectedRoofPolygons.push_back({Point3d(-13.145027272273616, -3.7276383085200995, 2.1467799499206666),
                                  Point3d(-17.424363325929452, -3.372939648486792, 0.0), Point3d(-17.70268438790386, -7.447180852769691, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-8.547206138620584, -0.08503012741680582, 4.258110923914474),
                                  Point3d(-13.145027272273616, -3.7276383085200995, 2.1467799499206666),
                                  Point3d(-17.70268438790386, -7.447180852769691, 0.0), Point3d(-9.338404052373633, -8.571492793979743, 0.0),
                                  Point3d(-8.486848592193729, -0.15956404547695954, 4.225196564370697)});
  expectedRoofPolygons.push_back({Point3d(-8.486848592193729, -0.15956404547695954, 4.225196564370697),
                                  Point3d(-9.338404052373633, -8.571492793979743, 0.0), Point3d(-0.6958026548881477, -9.161478178076948, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-8.547206138620584, -0.08503012741680582, 4.258110923914474),
                                  Point3d(-8.486848592193729, -0.15956404547695954, 4.225196564370697),
                                  Point3d(-0.6958026548881477, -9.161478178076948, 0.0), Point3d(0.0, 0.0, 0.0),
                                  Point3d(-8.489043344467849, 0.5857274258099088, 4.254511486038224)});
  expectedRoofPolygons.push_back(
    {Point3d(-8.489043344467849, 0.5857274258099088, 4.254511486038224), Point3d(0.0, 0.0, 0.0), Point3d(0.49804821611433425, 8.026046113323751, 0.0),
     Point3d(0.15402029409151105, 8.424446729364753, 0.1840211711326573), Point3d(0.1539838067115609, 8.424052798868134, 0.18402718078276917),
     Point3d(-8.46470046215179, 0.8909679724959663, 4.251815927287789)});
  expectedRoofPolygons.push_back({Point3d(0.22497519945185107, 8.482049310367099, 0.2157053171301373),
                                  Point3d(0.15402029409151105, 8.424446729364753, 0.1840211711326573),
                                  Point3d(0.49804821611433425, 8.026046113323751, 0.0), Point3d(0.6298845085982947, 8.014914280523122, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofExtendedSkeleton_6) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {12.897989139805986, -1.2801851278900926, 0.0},
    {9.587432018184284, -44.984415761338475, 0.0},
    {10.45169250570246, -45.11799923152818, 0.0},
    {10.40042281575131, -46.008555616736736, 0.0},
    {10.209992538847342, -45.97515975562331, 0.0},
    {9.301786602818757, -58.27595537322258, 0.0},
    {9.536162328252283, -58.25369151177075, 0.0},
    {9.4116502241235, -59.077454232298045, 0.0},
    {8.525417012321396, -59.044058449618106, 0.0},
    {5.397965926220366, -103.09292310494317, 0.0},
    {-7.500023213547323, -102.35822162327173, 0.0},
    {-4.167493367641692, -57.69709500917689, 0.0},
    {-41.982551816621694, -54.84732004601991, 0.0},
    {-40.99377922499388, -41.76727954695175, 0.0},
    {-3.3545025701129614, -44.650457070545166, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(-34.95236405380182, -48.803894914360306, 3.277319833875921),
                                  Point3d(-40.99377922499388, -41.76727954695175, 0.0), Point3d(-41.982551816621694, -54.84732004601991, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(2.974315495160843, -51.39934012787623, 3.4083191763503375), Point3d(2.668507270977346, -51.66234808659685, 3.2656966347693226),
     Point3d(-34.95236405380182, -48.803894914360306, 3.277319833875921), Point3d(-41.982551816621694, -54.84732004601991, 0.0),
     Point3d(-4.167493367641692, -57.69709500917689, 0.0), Point3d(3.0496714771194053, -51.486845032739296, 3.367521854595256)});
  expectedRoofPolygons.push_back(
    {Point3d(3.004460242622362, -52.09595065239559, 3.3676410677244997), Point3d(3.0496714771194053, -51.486845032739296, 3.367521854595256),
     Point3d(-4.167493367641692, -57.69709500917689, 0.0), Point3d(-7.500023213547323, -102.35822162327173, 0.0),
     Point3d(-0.5826270913313756, -96.29477300440011, 3.223514864156848), Point3d(2.609744318931162, -52.44870975530364, 3.1839548787259475)});
  expectedRoofPolygons.push_back({Point3d(-0.5826270913313756, -96.29477300440011, 3.223514864156848),
                                  Point3d(-7.500023213547323, -102.35822162327173, 0.0), Point3d(5.397965926220366, -103.09292310494317, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-0.5826270913313756, -96.29477300440011, 3.223514864156848),
                                  Point3d(5.397965926220366, -103.09292310494317, 0.0), Point3d(8.525417012321396, -59.044058449618106, 0.0),
                                  Point3d(2.609744318931162, -52.44870975530364, 3.1839548787259475)});
  expectedRoofPolygons.push_back(
    {Point3d(3.004460242622362, -52.09595065239559, 3.3676410677244997), Point3d(2.609744318931162, -52.44870975530364, 3.1839548787259475),
     Point3d(8.525417012321396, -59.044058449618106, 0.0), Point3d(9.4116502241235, -59.077454232298045, 0.0),
     Point3d(9.083903057233032, -58.68160477288223, 0.19161351101076526), Point3d(8.899417245209744, -58.68686801042851, 0.18551024950739273)});
  expectedRoofPolygons.push_back({Point3d(9.083903057233032, -58.68160477288223, 0.19161351101076526),
                                  Point3d(9.4116502241235, -59.077454232298045, 0.0), Point3d(9.536162328252283, -58.25369151177075, 0.0)});
  expectedRoofPolygons.push_back({Point3d(9.083903057233032, -58.68160477288223, 0.19161351101076526),
                                  Point3d(9.536162328252283, -58.25369151177075, 0.0), Point3d(9.301786602818757, -58.27595537322258, 0.0),
                                  Point3d(8.899417245209744, -58.68686801042851, 0.18551024950739273)});
  expectedRoofPolygons.push_back(
    {Point3d(2.974315495160843, -51.39934012787623, 3.4083191763503375), Point3d(3.0496714771194053, -51.486845032739296, 3.367521854595256),
     Point3d(3.004460242622362, -52.09595065239559, 3.3676410677244997), Point3d(8.899417245209744, -58.68686801042851, 0.18551024950739273),
     Point3d(9.301786602818757, -58.27595537322258, 0.0), Point3d(10.209992538847342, -45.97515975562331, 0.0),
     Point3d(9.809594136557513, -45.461107618972676, 0.21858126459016727), Point3d(3.0140654485992524, -50.86564441100163, 3.4081468746154)});
  expectedRoofPolygons.push_back({Point3d(9.990475882471465, -45.490944034268786, 0.21950955535230401),
                                  Point3d(9.809594136557513, -45.461107618972676, 0.21858126459016727),
                                  Point3d(10.209992538847342, -45.97515975562331, 0.0), Point3d(10.40042281575131, -46.008555616736736, 0.0)});
  expectedRoofPolygons.push_back({Point3d(9.990475882471465, -45.490944034268786, 0.21950955535230401),
                                  Point3d(10.40042281575131, -46.008555616736736, 0.0), Point3d(10.45169250570246, -45.11799923152818, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(3.0140654485992524, -50.86564441100163, 3.4081468746154), Point3d(9.809594136557513, -45.461107618972676, 0.21858126459016727),
     Point3d(9.990475882471465, -45.490944034268786, 0.21950955535230401), Point3d(10.45169250570246, -45.11799923152818, 0.0),
     Point3d(9.587432018184284, -44.984415761338475, 0.0), Point3d(2.6908009618465503, -50.458642095935566, 3.231723169571309)});
  expectedRoofPolygons.push_back({Point3d(5.961683861298499, -7.100422025402652, 3.238436050919135),
                                  Point3d(2.6908009618465503, -50.458642095935566, 3.231723169571309),
                                  Point3d(9.587432018184284, -44.984415761338475, 0.0), Point3d(12.897989139805986, -1.2801851278900926, 0.0)});
  expectedRoofPolygons.push_back({Point3d(5.961683861298499, -7.100422025402652, 3.238436050919135),
                                  Point3d(12.897989139805986, -1.2801851278900926, 0.0), Point3d(0.0, 0.0, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(2.974315495160843, -51.39934012787623, 3.4083191763503375), Point3d(3.0140654485992524, -50.86564441100163, 3.4081468746154),
     Point3d(2.6908009618465503, -50.458642095935566, 3.231723169571309), Point3d(5.961683861298499, -7.100422025402652, 3.238436050919135),
     Point3d(0.0, 0.0, 0.0), Point3d(-3.3545025701129614, -44.650457070545166, 0.0),
     Point3d(2.668507270977346, -51.66234808659685, 3.2656966347693226)});
  expectedRoofPolygons.push_back({Point3d(-34.95236405380182, -48.803894914360306, 3.277319833875921),
                                  Point3d(2.668507270977346, -51.66234808659685, 3.2656966347693226),
                                  Point3d(-3.3545025701129614, -44.650457070545166, 0.0), Point3d(-40.99377922499388, -41.76727954695175, 0.0)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofExtendedSkeleton_7) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {0.0, 0.0, 0.0},
    {3.3325122039160604, 44.660890183765225, 0.0},
    {-16.76509985651811, 46.13029743716488, 0.0},
    {-34.48234605694931, 47.51065006057564, 0.0},
    {-33.493578699757975, 60.59062131557136, 0.0},
    {-13.205537370552673, 58.89857224014935, 0.0},
    {4.145498697574807, 57.70745905515341, 0.0},
    {7.499983509377387, 102.35767975185216, 0.0},
    {8.29099739518791, 115.53796786164443, 0.0},
    {8.649883324805351, 120.26907550602861, 0.0},
    {25.751896503054375, 118.87757284223014, 0.0},
    {25.378362168102658, 114.14646637985689, 0.0},
    {24.32367698709856, 100.75467386193046, 0.0},
    {20.397904368867692, 101.07750140109947, 0.0},
    {17.08736477291475, 57.37350213229498, 0.0},
    {17.95162068514772, 57.23991936927919, 0.0},
    {17.900351266611796, 56.349367698562986, 0.0},
    {17.709921997821514, 56.38276338288297, 0.0},
    {16.80172086971976, 44.08203288413275, 0.0},
    {17.036095354397972, 44.104296627722526, 0.0},
    {16.91158390942045, 43.2805382680904, 0.0},
    {16.025355389224362, 43.31393387397731, 0.0},
    {12.89792085945201, -0.7346975922551309, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back(
    {Point3d(10.504427846768982, 50.262004888800014, 3.3676232398605963), Point3d(10.109714012653706, 49.90924765335381, 3.1839380232733676),
     Point3d(16.025355389224362, 43.31393387397731, 0.0), Point3d(16.91158390942045, 43.2805382680904, 0.0),
     Point3d(16.583838477581835, 43.6763856319295, 0.19161249663318744), Point3d(16.399353642203014, 43.67112242224613, 0.18550926743971421)});
  expectedRoofPolygons.push_back({Point3d(16.583838477581835, 43.6763856319295, 0.19161249663318744),
                                  Point3d(16.91158390942045, 43.2805382680904, 0.0), Point3d(17.036095354397972, 44.104296627722526, 0.0)});
  expectedRoofPolygons.push_back({Point3d(16.583838477581835, 43.6763856319295, 0.19161249663318744),
                                  Point3d(17.036095354397972, 44.104296627722526, 0.0), Point3d(16.80172086971976, 44.08203288413275, 0.0),
                                  Point3d(16.399353642203014, 43.67112242224613, 0.18550926743971421)});
  expectedRoofPolygons.push_back(
    {Point3d(10.475387465053853, 50.973437154244564, 3.408296346805568), Point3d(10.550753066764416, 50.88611861064228, 3.36750108937077),
     Point3d(10.504427846768982, 50.262004888800014, 3.3676232398605963), Point3d(16.399353642203014, 43.67112242224613, 0.18550926743971421),
     Point3d(16.80172086971976, 44.08203288413275, 0.0), Point3d(17.709921997821514, 56.38276338288297, 0.0),
     Point3d(17.30952571518998, 56.89681279820688, 0.21858010744873552), Point3d(10.51403300189711, 51.49230461710896, 3.4081288323188823)});
  expectedRoofPolygons.push_back({Point3d(17.490406503539052, 56.866976540860996, 0.21950839329659286),
                                  Point3d(17.30952571518998, 56.89681279820688, 0.21858010744873552),
                                  Point3d(17.709921997821514, 56.38276338288297, 0.0), Point3d(17.900351266611796, 56.349367698562986, 0.0)});
  expectedRoofPolygons.push_back({Point3d(17.490406503539052, 56.866976540860996, 0.21950839329659286),
                                  Point3d(17.900351266611796, 56.349367698562986, 0.0), Point3d(17.95162068514772, 57.23991936927919, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(10.51403300189711, 51.49230461710896, 3.4081288323188823), Point3d(17.30952571518998, 56.89681279820688, 0.21858010744873552),
     Point3d(17.490406503539052, 56.866976540860996, 0.21950839329659286), Point3d(17.95162068514772, 57.23991936927919, 0.0),
     Point3d(17.08736477291475, 57.37350213229498, 0.0), Point3d(10.1907702264655, 51.89930477755651, 3.2317060612394553)});
  expectedRoofPolygons.push_back({Point3d(14.385215052284758, 108.12154420705387, 3.2637844002593006),
                                  Point3d(13.96432867306472, 101.92089913181533, 3.2394505910467895),
                                  Point3d(10.1907702264655, 51.89930477755651, 3.2317060612394553),
                                  Point3d(17.08736477291475, 57.37350213229498, 0.0), Point3d(20.397904368867692, 101.07750140109947, 0.0)});
  expectedRoofPolygons.push_back({Point3d(16.496386043825577, 109.95237883923916, 4.2626339960696775),
                                  Point3d(14.385215052284758, 108.12154420705387, 3.2637844002593006),
                                  Point3d(20.397904368867692, 101.07750140109947, 0.0), Point3d(24.32367698709856, 100.75467386193046, 0.0)});
  expectedRoofPolygons.push_back({Point3d(16.573270758394443, 111.06055607193703, 4.267813452228237),
                                  Point3d(16.496386043825577, 109.95237883923916, 4.2626339960696775),
                                  Point3d(24.32367698709856, 100.75467386193046, 0.0), Point3d(25.378362168102658, 114.14646637985689, 0.0),
                                  Point3d(20.635402089919857, 114.52046997115824, 2.37884156540302)});
  expectedRoofPolygons.push_back({Point3d(20.635402089919857, 114.52046997115824, 2.37884156540302),
                                  Point3d(25.378362168102658, 114.14646637985689, 0.0), Point3d(25.751896503054375, 118.87757284223014, 0.0)});
  expectedRoofPolygons.push_back({Point3d(16.573270758394443, 111.06055607193703, 4.267813452228237),
                                  Point3d(20.635402089919857, 114.52046997115824, 2.37884156540302),
                                  Point3d(25.751896503054375, 118.87757284223014, 0.0), Point3d(8.649883324805351, 120.26907550602861, 0.0),
                                  Point3d(12.96245911252736, 115.2206282934406, 2.341041232258593)});
  expectedRoofPolygons.push_back({Point3d(12.96245911252736, 115.2206282934406, 2.341041232258593),
                                  Point3d(8.649883324805351, 120.26907550602861, 0.0), Point3d(8.29099739518791, 115.53796786164443, 0.0)});
  expectedRoofPolygons.push_back(
    {Point3d(16.573270758394443, 111.06055607193703, 4.267813452228237), Point3d(12.96245911252736, 115.2206282934406, 2.341041232258593),
     Point3d(8.29099739518791, 115.53796786164443, 0.0), Point3d(7.499983509377387, 102.35767975185216, 0.0),
     Point3d(13.96432867306472, 101.92089913181533, 3.2394505910467895), Point3d(14.385215052284758, 108.12154420705387, 3.2637844002593006),
     Point3d(16.496386043825577, 109.95237883923916, 4.2626339960696775)});
  expectedRoofPolygons.push_back(
    {Point3d(10.475387465053853, 50.973437154244564, 3.408296346805568), Point3d(10.51403300189711, 51.49230461710896, 3.4081288323188823),
     Point3d(10.1907702264655, 51.89930477755651, 3.2317060612394553), Point3d(13.96432867306472, 101.92089913181533, 3.2394505910467895),
     Point3d(7.499983509377387, 102.35767975185216, 0.0), Point3d(4.145498697574807, 57.70745905515341, 0.0),
     Point3d(10.189585366562266, 50.72707914673348, 3.2750250755274117)});
  expectedRoofPolygons.push_back({Point3d(10.189585366562266, 50.72707914673348, 3.2750250755274117),
                                  Point3d(4.145498697574807, 57.70745905515341, 0.0), Point3d(-13.205537370552673, 58.89857224014935, 0.0),
                                  Point3d(-13.698023373676454, 52.42024215606471, 3.2484239503449044)});
  expectedRoofPolygons.push_back({Point3d(-27.468362207188378, 53.52470637475261, 3.2703478227890734),
                                  Point3d(-16.274910149468344, 52.621897347103115, 3.2550312720816463),
                                  Point3d(-13.698023373676454, 52.42024215606471, 3.2484239503449044),
                                  Point3d(-13.205537370552673, 58.89857224014935, 0.0), Point3d(-33.493578699757975, 60.59062131557136, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-27.468362207188378, 53.52470637475261, 3.2703478227890734),
                                  Point3d(-33.493578699757975, 60.59062131557136, 0.0), Point3d(-34.48234605694931, 47.51065006057564, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-27.468362207188378, 53.52470637475261, 3.2703478227890734),
                                  Point3d(-34.48234605694931, 47.51065006057564, 0.0), Point3d(-16.76509985651811, 46.13029743716488, 0.0),
                                  Point3d(-16.274910149468344, 52.621897347103115, 3.2550312720816463)});
  expectedRoofPolygons.push_back(
    {Point3d(10.475387465053853, 50.973437154244564, 3.408296346805568), Point3d(10.189585366562266, 50.72707914673348, 3.2750250755274117),
     Point3d(-13.698023373676454, 52.42024215606471, 3.2484239503449044), Point3d(-16.274910149468344, 52.621897347103115, 3.2550312720816463),
     Point3d(-16.76509985651811, 46.13029743716488, 0.0), Point3d(3.3325122039160604, 44.660890183765225, 0.0),
     Point3d(10.550753066764416, 50.88611861064228, 3.36750108937077)});
  expectedRoofPolygons.push_back(
    {Point3d(10.504427846768982, 50.262004888800014, 3.3676232398605963), Point3d(10.550753066764416, 50.88611861064228, 3.36750108937077),
     Point3d(3.3325122039160604, 44.660890183765225, 0.0), Point3d(0.0, 0.0, 0.0), Point3d(6.917359502399792, 6.063416519745084, 3.2234977992787495),
     Point3d(10.109714012653706, 49.90924765335381, 3.1839380232733676)});
  expectedRoofPolygons.push_back({Point3d(6.917359502399792, 6.063416519745084, 3.2234977992787495), Point3d(0.0, 0.0, 0.0),
                                  Point3d(12.89792085945201, -0.7346975922551309, 0.0)});
  expectedRoofPolygons.push_back({Point3d(6.917359502399792, 6.063416519745084, 3.2234977992787495),
                                  Point3d(12.89792085945201, -0.7346975922551309, 0.0), Point3d(16.025355389224362, 43.31393387397731, 0.0),
                                  Point3d(10.109714012653706, 49.90924765335381, 3.1839380232733676)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofExtendedSkeleton_8) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {-73.99355670177661, 130.28440014311866, 0.0}, {-7.85201868781445, 125.31922063111779, 0.0}, {-7.49311111722858, 130.0506135632292, 0.0},
    {9.60993331941421, 128.65902699125058, 0.0},   {9.236376460189206, 123.92763524122142, 0.0}, {8.181627681237046, 110.5350351927255, 0.0},
    {4.255618337310636, 110.85788219852725, 0.0},  {0.9448791144652091, 67.1512475606807, 0.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.31749588484094743, 119.77603117590006, 4.2826970132879),
                                  Point3d(-1.64604883910226, 118.10016025919867, 3.367118066990641),
                                  Point3d(-1.946448792095577, 118.12378669561315, 3.366581720843165),
                                  Point3d(4.255618337310636, 110.85788219852725, 0.0), Point3d(8.181627681237046, 110.5350351927255, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.3976890543497383, 120.81337575173194, 4.283446707132655),
                                  Point3d(0.31749588484094743, 119.77603117590006, 4.2826970132879),
                                  Point3d(8.181627681237046, 110.5350351927255, 0.0), Point3d(9.236376460189206, 123.92763524122142, 0.0),
                                  Point3d(4.49313037957261, 124.30166138509239, 2.378985010522967)});
  expectedRoofPolygons.push_back({Point3d(4.49313037957261, 124.30166138509239, 2.378985010522967),
                                  Point3d(9.236376460189206, 123.92763524122142, 0.0), Point3d(9.60993331941421, 128.65902699125058, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.3976890543497383, 120.81337575173194, 4.283446707132655),
                                  Point3d(4.49313037957261, 124.30166138509239, 2.378985010522967),
                                  Point3d(9.60993331941421, 128.65902699125058, 0.0), Point3d(-7.49311111722858, 130.0506135632292, 0.0)});
  expectedRoofPolygons.push_back({Point3d(0.3976890543497383, 120.81337575173194, 4.283446707132655),
                                  Point3d(-7.49311111722858, 130.0506135632292, 0.0), Point3d(-7.85201868781445, 125.31922063111779, 0.0),
                                  Point3d(-1.64604883910226, 118.10016025919867, 3.367118066990641),
                                  Point3d(0.31749588484094743, 119.77603117590006, 4.2826970132879)});
  expectedRoofPolygons.push_back({Point3d(-15.548141335420517, 106.42963023576449, 9.706354379316265),
                                  Point3d(-1.946448792095577, 118.12378669561315, 3.366581720843165),
                                  Point3d(-1.64604883910226, 118.10016025919867, 3.367118066990641),
                                  Point3d(-7.85201868781445, 125.31922063111779, 0.0), Point3d(-73.99355670177661, 130.28440014311866, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-15.548141335420517, 106.42963023576449, 9.706354379316265),
                                  Point3d(-73.99355670177661, 130.28440014311866, 0.0), Point3d(0.9448791144652091, 67.1512475606807, 0.0)});
  expectedRoofPolygons.push_back({Point3d(-15.548141335420517, 106.42963023576449, 9.706354379316265),
                                  Point3d(0.9448791144652091, 67.1512475606807, 0.0), Point3d(4.255618337310636, 110.85788219852725, 0.0),
                                  Point3d(-1.946448792095577, 118.12378669561315, 3.366581720843165)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}

TEST_F(GeometryFixture, RoofExtendedSkeleton_8_Zoffset) {
  double pitch = radToDeg(atan(6.0 / 12.0));

  std::vector<Point3d> footprint{
    {-73.99355670177661, 130.28440014311866, 8.0}, {-7.85201868781445, 125.31922063111779, 8.0}, {-7.49311111722858, 130.0506135632292, 8.0},
    {9.60993331941421, 128.65902699125058, 8.0},   {9.236376460189206, 123.92763524122142, 8.0}, {8.181627681237046, 110.5350351927255, 8.0},
    {4.255618337310636, 110.85788219852725, 8.0},  {0.9448791144652091, 67.1512475606807, 8.0},
  };

  std::vector<std::vector<Point3d>> roofPolygons = generateHipRoof(footprint, pitch);

  std::vector<std::vector<Point3d>> expectedRoofPolygons;
  expectedRoofPolygons.push_back({Point3d(0.31749588484094743, 119.77603117590006, 12.2826970132879),
                                  Point3d(-1.64604883910226, 118.10016025919867, 11.367118066990641),
                                  Point3d(-1.946448792095577, 118.12378669561315, 11.366581720843165),
                                  Point3d(4.255618337310636, 110.85788219852725, 8.0), Point3d(8.181627681237046, 110.5350351927255, 8.0)});
  expectedRoofPolygons.push_back({Point3d(0.3976890543497383, 120.81337575173194, 12.283446707132655),
                                  Point3d(0.31749588484094743, 119.77603117590006, 12.2826970132879),
                                  Point3d(8.181627681237046, 110.5350351927255, 8.0), Point3d(9.236376460189206, 123.92763524122142, 8.0),
                                  Point3d(4.49313037957261, 124.30166138509239, 10.378985010522967)});
  expectedRoofPolygons.push_back({Point3d(4.49313037957261, 124.30166138509239, 10.378985010522967),
                                  Point3d(9.236376460189206, 123.92763524122142, 8.0), Point3d(9.60993331941421, 128.65902699125058, 8.0)});
  expectedRoofPolygons.push_back({Point3d(0.3976890543497383, 120.81337575173194, 12.283446707132655),
                                  Point3d(4.49313037957261, 124.30166138509239, 10.378985010522967),
                                  Point3d(9.60993331941421, 128.65902699125058, 8.0), Point3d(-7.49311111722858, 130.0506135632292, 8.0)});
  expectedRoofPolygons.push_back({Point3d(0.3976890543497383, 120.81337575173194, 12.283446707132655),
                                  Point3d(-7.49311111722858, 130.0506135632292, 8.0), Point3d(-7.85201868781445, 125.31922063111779, 8.0),
                                  Point3d(-1.64604883910226, 118.10016025919867, 11.367118066990641),
                                  Point3d(0.31749588484094743, 119.77603117590006, 12.2826970132879)});
  expectedRoofPolygons.push_back({Point3d(-15.548141335420517, 106.42963023576449, 17.706354379316265),
                                  Point3d(-1.946448792095577, 118.12378669561315, 11.366581720843165),
                                  Point3d(-1.64604883910226, 118.10016025919867, 11.367118066990641),
                                  Point3d(-7.85201868781445, 125.31922063111779, 8.0), Point3d(-73.99355670177661, 130.28440014311866, 8.0)});
  expectedRoofPolygons.push_back({Point3d(-15.548141335420517, 106.42963023576449, 17.706354379316265),
                                  Point3d(-73.99355670177661, 130.28440014311866, 8.0), Point3d(0.9448791144652091, 67.1512475606807, 8.0)});
  expectedRoofPolygons.push_back({Point3d(-15.548141335420517, 106.42963023576449, 17.706354379316265),
                                  Point3d(0.9448791144652091, 67.1512475606807, 8.0), Point3d(4.255618337310636, 110.85788219852725, 8.0),
                                  Point3d(-1.946448792095577, 118.12378669561315, 11.366581720843165)});

  EXPECT_EQ(expectedRoofPolygons.size(), roofPolygons.size());

  for (unsigned i = 0; i < roofPolygons.size(); ++i) {
    polygonMatches(expectedRoofPolygons, roofPolygons[i]);
  }
}
