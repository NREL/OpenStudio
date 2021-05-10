/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

/// compute area from surface as Point3dVector
UTILITIES_API boost::optional<double> getArea(const std::vector<Point3d>& points);

/// compute Newall vector from surface as Point3dVector, direction is same as outward normal
/// magnitude is twice the area
UTILITIES_API boost::optional<Vector3d> getNewallVector(const std::vector<Point3d>& points);

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

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_GEOMETRY_HPP
