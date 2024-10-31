/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_GEOMETRY_POLYHEDRON_HPP
#define UTILITIES_GEOMETRY_POLYHEDRON_HPP

#include "../UtilitiesAPI.hpp"
#include "Point3d.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace openstudio {

namespace model {
  class Space;
  class ThermalZone;
}  // namespace model

class Surface3d;

class UTILITIES_API Surface3dEdge
{
 public:
  Surface3dEdge(Point3d start, Point3d end, const Surface3d& firstSurface);
  Surface3dEdge(Point3d start, Point3d end, std::string t_name, size_t t_surfNum);

  const Point3d& start() const;
  const Point3d& end() const;

  /// check equality: this uses a tolerance
  bool operator==(const Surface3dEdge& other) const;

  // Asserts that isAlmostEqual3dPt(start, other.end) && isAlmostEqual3dPt(end, other.start)
  bool reverseEqual(const Surface3dEdge& other) const;

  /// check inequality
  bool operator!=(const Surface3dEdge& other) const;

  size_t count() const;

  size_t firstSurfNum() const;
  std::string firstSurfaceName() const;
  const std::vector<size_t>& allSurfNums() const;
  void appendSurface(const Surface3d& surface);

  // Checks whether a Point: is not almost equal to the start and end points, and that isPointOnLineBetweenPoints(start, end, testVertex) is true
  bool containsPoint(const Point3d& testVertex) const;

  void markConflictedOrientation();
  bool hasConflictedOrientation() const;

  // Indicates this edge has been created and wasn't part of the original surface (cf updateZonePolygonsForMissingColinearPoints)
  void markCreated();
  bool hasBeenCreated() const;

  void resetEdgeMatching();

  Vector3d asVector() const;

  // If it containsPoint the testVertex, Split in place the current edge from [start, vertex] vertex and returns the new next [vertex, end]
  boost::optional<Surface3dEdge> splitEdge(Point3d testVertex);

 private:
  Point3d m_start;
  Point3d m_end;
  bool m_conflictedOrientation = false;
  bool m_hasBeenCreated = false;
  std::string m_firstSurfaceName;
  std::vector<size_t> m_allSurfNums;
};

// A thin wrapper to improve reporting, by attaching a name to the vertices
class UTILITIES_API Surface3d
{
 public:
  Surface3d(std::vector<Point3d> t_vertices, std::string t_name, size_t t_surfNum);
  std::vector<Point3d> vertices;
  std::string name;
  size_t surfNum;
  std::vector<Surface3dEdge> edges;

  size_t numConflictedEdges() const;
  double ratioOfConflictedEdges() const;

  // There is no check done whatsoevever to ensure the surface is planar.
  bool isConvex() const;

  void resetEdgeMatching();

  bool operator<(const Surface3d& rhs) const;
};

class UTILITIES_API Polyhedron
{
 public:
  /** This will call updateZonePolygonsForMissingColinearPoints if needed (if original Polyhedron isn't enclosed) to add any missing colinear point
    * (=splitting up edges in case surface intersection hasn't been performed already) */
  Polyhedron(std::vector<Surface3d> surfaces);

  /** Test if the volume described by the polyhedron if full enclosed (would not leak).
    * This is done by checking that every edge is used exactly TWICE. */
  bool isEnclosedVolume() const;

  /** All unique edges that aren't used exactly twice */
  std::vector<Surface3dEdge> edgesNotTwo(bool includeCreatedEdges = false) const;

  std::vector<Point3d> uniqueVertices() const;

  /** Ignores orientation of edges to return the unique edges */
  std::vector<Surface3dEdge> uniqueEdges() const;

  size_t numVertices() const;

  // Call calculatedVolume as it will handle the logic to dispatch to inidivual methods based on checks
  // Note JM 2022-05-23: Actually a lot of the needed logic is in PlanarSurface,
  // so I don't think it's worth trying to fallback to the other calculation modes E+ does (like checking if floor and roof are same and use that etc)
  // double calculatedVolume() const;

  // Polyhedron MUST be enclosed
  double polyhedronVolume() const;
  // Polyhedron MUST be enclosed
  double calcDivergenceTheoremVolume() const;

  // Whether updateZonePolygonsForMissingColinearPoints was called and ended up actually splitting up edges
  bool hasAddedColinearPoints() const;

  /** In an ENCLOSED Polyhedron, all edges should be used twice. And if all surfaces are correctly oriented, each edge should match an edge that is in
   * reversed order */
  bool hasAnySurfaceWithIncorrectOrientation() const;

  bool isCompletelyInsideOut() const;

  /** Finds all edges that two surface define in the **same** order (if all surfaces are correctly oriented, they should match in reverse order)
    * For each matching edge, we pick a single surface between the two surfaces that define it but retainign the surface that has the largest
    * proportion of conflicted edges / total number of edges */
  std::vector<Surface3d> findSurfacesWithIncorrectOrientation() const;

  std::vector<Surface3d> surface3ds() const;

 protected:
  void performEdgeMatching();
  void resetEdgeMatching();
  void updateZonePolygonsForMissingColinearPoints();
  bool hasEdgesNot2() const;
  // Internal method, result is cached. We have to call it initially to detect if polyhedron is completely inside-out
  double calcPolyhedronVolume() const;

 private:
  REGISTER_LOGGER("utilities.Polyhedron");

  std::vector<Surface3d> m_surfaces;
  bool m_isEnclosedVolume = false;
  bool m_hasAddedColinearPoints = false;
  bool m_hasAnySurfaceWithIncorrectOrientation = false;
  bool m_isCompletelyInsideOut = false;
  double m_polyhedronVolume = 0.0;
};

using OptionalSurface3dEdge = boost::optional<Surface3dEdge>;
using Surface3dEdgeVector = std::vector<Surface3dEdge>;
using Surface3dVector = std::vector<Surface3d>;
using PolyhedronVector = std::vector<Polyhedron>;

/// ostream operator
UTILITIES_API std::ostream& operator<<(std::ostream& os, const Surface3dEdge& edge);
UTILITIES_API std::ostream& operator<<(std::ostream& os, const Surface3d& surface3d);

}  // namespace openstudio

#endif  // UTILITIES_GEOMETRY_POLYHEDRON_HPP
