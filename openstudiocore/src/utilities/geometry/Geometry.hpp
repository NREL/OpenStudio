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

#ifndef UTILITIES_GEOMETRY_GEOMETRY_HPP
#define UTILITIES_GEOMETRY_GEOMETRY_HPP

#include "../UtilitiesAPI.hpp"

#include "Point3d.hpp"
#include "Vector3d.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace openstudio{

  /// convert degrees to radians
  UTILITIES_API double degToRad(double degrees);

  /// convert radians to degrees
  UTILITIES_API double radToDeg(double radians);

  /// compute area from surface as Point3dVector
  UTILITIES_API boost::optional<double> getArea(const Point3dVector& points);

  /// compute Newall vector from surface as Point3dVector, direction is same as outward normal
  /// magnitude is twice the area
  UTILITIES_API boost::optional<Vector3d> getNewallVector(const Point3dVector& points);

  /// compute outward normal from surface as Point3dVector
  UTILITIES_API boost::optional<Vector3d> getOutwardNormal(const Point3dVector& points);

  /// compute centroid from surface as Point3dVector
  UTILITIES_API boost::optional<Point3d> getCentroid(const Point3dVector& points);

  /// reorder points to upper-left-corner convention
  UTILITIES_API std::vector<Point3d> reorderULC(const Point3dVector& points);

  /// removes collinear points, tolerance is for length of cross product after normalizing each line segment
  UTILITIES_API std::vector<Point3d> removeCollinear(const Point3dVector& points, double tol = 0.001);

  /// return distance between two points
  UTILITIES_API double getDistance(const Point3d& point1, const Point3d& point2);

  /// return distance between a point and a line segment
  /// returns 0 if lineSegment does not have length 2
  UTILITIES_API double getDistancePointToLineSegment(const Point3d& point, const std::vector<Point3d>& lineSegment);

  /// return distance between a point and a triangle
  /// returns 0 if triangle does not have length 3
  UTILITIES_API double getDistancePointToTriangle(const Point3d& point, const std::vector<Point3d>& triangle);

  /// return angle (in radians) between two vectors
  UTILITIES_API double getAngle(const Vector3d& vector1, const Vector3d& vector2);
  
  /// compute distance in meters between two points on the Earth's surface
  /// lat and lon are specified in degrees
  UTILITIES_API double getDistanceLatLon(double lat1, double lon1, double lat2, double lon2);

  /// check if two vectors of points are equal (within tolerance) irregardless of initial ordering.
  UTILITIES_API bool circularEqual(const Point3dVector& points1, const Point3dVector& points2, double tol = 0.001);

  /// if point3d is within tol of any existing points then returns existing point
  /// otherwise adds point3d to allPoints and returns point3d
  UTILITIES_API Point3d getCombinedPoint(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol = 0.001);

  /// compute triangulation of vertices, holes are removed in the triangulation
  /// requires that vertices and holes are in counter clockwise order on the z = 0 plane (e.g. in face coordinates) 
  UTILITIES_API std::vector<std::vector<Point3d> > computeTriangulation(const Point3dVector& vertices, const std::vector<std::vector<Point3d> >& holes, double tol = 0.001);

  /// move all vertices towards point by distance, pass negative distance to move away from point
  /// no guarantee that resulting polygon will be valid
  UTILITIES_API std::vector<Point3d> moveVerticesTowardsPoint(const Point3dVector& vertices, const Point3d& point, double distance);

} // openstudio

#endif //UTILITIES_GEOMETRY_GEOMETRY_HPP
