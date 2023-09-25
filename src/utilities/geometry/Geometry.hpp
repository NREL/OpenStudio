/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_GEOMETRY_GEOMETRY_HPP
#define UTILITIES_GEOMETRY_GEOMETRY_HPP

#include "../UtilitiesAPI.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace openstudio {

class Point3d;
class PointLatLon;
class Vector3d;

/// convert degrees to radians
UTILITIES_API double degToRad(double degrees);

/// convert radians to degrees
UTILITIES_API double radToDeg(double radians);

// Forces an angle in degrees to be in the [0,360[ range
UTILITIES_API constexpr double normalizeAngle0to360(double angleDegrees) {
  while (angleDegrees < 0.0) {
    angleDegrees += 360.0;
  }

  // This is twice as fast as using std::fmod(angleDegrees, 360.0)
  while (angleDegrees >= 360.0) {
    angleDegrees -= 360.0;
  }
  return angleDegrees;
}

/// compute area from surface as Point3dVector
UTILITIES_API boost::optional<double> getArea(const std::vector<Point3d>& points);

/// compute Newell vector from surface as Point3dVector, direction is same as outward normal
/// magnitude is twice the area
UTILITIES_API boost::optional<Vector3d> getNewellVector(const std::vector<Point3d>& points);

/// compute outward normal from surface as Point3dVector
UTILITIES_API boost::optional<Vector3d> getOutwardNormal(const std::vector<Point3d>& points);

/// compute centroid from surface as Point3dVector
UTILITIES_API boost::optional<Point3d> getCentroid(const std::vector<Point3d>& points);

/// reorder points to upper-left-corner convention
UTILITIES_API std::vector<Point3d> reorderULC(const std::vector<Point3d>& points);

/// removes collinear points
UTILITIES_API std::vector<Point3d> removeCollinear(const std::vector<Point3d>& points, double tol = 0.001);

/// removes collinear points, tolerance is for length of cross product after normalizing each line segment
UTILITIES_API std::vector<Point3d> removeCollinearLegacy(const std::vector<Point3d>& points, double tol = 0.001);

/// return distance between two points
UTILITIES_API double getDistance(const Point3d& point1, const Point3d& point2);

/// return distance squared between two points
/// faster than getDistance; can be used to compare distances
UTILITIES_API double getDistanceSquared(const Point3d& point1, const Point3d& point2);

/// return distance between a point and a line segment
/// returns 0 if lineSegment does not have length 2
UTILITIES_API double getDistancePointToLineSegment(const Point3d& point, const std::vector<Point3d>& lineSegment);

/// return distance between a point and a triangle
/// returns 0 if triangle does not have length 3
UTILITIES_API double getDistancePointToTriangle(const Point3d& point, const std::vector<Point3d>& triangle);

/// return angle (in radians) between two vectors
UTILITIES_API double getAngle(const Vector3d& vector1, const Vector3d& vector2);

/// check if two vectors of points are equal (within tolerance) irregardless of initial ordering.
UTILITIES_API bool circularEqual(const std::vector<Point3d>& points1, const std::vector<Point3d>& points2, double tol = 0.001);

/// if point3d is within tol of any existing points then returns existing point
/// otherwise adds point3d to allPoints and returns point3d
UTILITIES_API Point3d getCombinedPoint(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol = 0.001);

/// compute triangulation of vertices, holes are removed in the triangulation
/// requires that vertices and holes are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
UTILITIES_API std::vector<std::vector<Point3d>> computeTriangulation(const std::vector<Point3d>& vertices,
                                                                     const std::vector<std::vector<Point3d>>& holes, double tol = 0.001);

/// move all vertices towards point by distance, pass negative distance to move away from point
/// no guarantee that resulting polygon will be valid
UTILITIES_API std::vector<Point3d> moveVerticesTowardsPoint(const std::vector<Point3d>& vertices, const Point3d& point, double distance);

/// reverse order of vertices
UTILITIES_API std::vector<Point3d> reverse(const std::vector<Point3d>& vertices);

/// Sets view and daylighting window, overhang and light shelf vertices by reference.  Returns true if successful, false otherwise.
UTILITIES_API bool applyViewAndDaylightingGlassRatios(double viewGlassToWallRatio, double daylightingGlassToWallRatio,
                                                      double desiredViewGlassSillHeight, double desiredDaylightingGlassHeaderHeight,
                                                      double exteriorShadingProjectionFactor, double interiorShelfProjectionFactor,
                                                      const std::vector<Point3d>& surfaceVertices, std::vector<Point3d>& viewVertices,
                                                      std::vector<Point3d>& daylightingVertices, std::vector<Point3d>& exteriorShadingVertices,
                                                      std::vector<Point3d>& interiorShelfVertices);

// Checks that a point is **almost** equal to another (with some tolerance)
UTILITIES_API bool isAlmostEqual3dPt(const Point3d& lhs, const Point3d& rhs, double tol = 0.0127);

UTILITIES_API bool isPointOnLineBetweenPoints(const Point3d& start, const Point3d& end, const Point3d& test, double tol = 0.0127);

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_GEOMETRY_HPP
