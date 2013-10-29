/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <utilities/UtilitiesAPI.hpp>

#include <utilities/geometry/Point3d.hpp>
#include <utilities/geometry/Vector3d.hpp>

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

  /// removes colinear points, tolerance is for length of cross product after normalizing each line segment
  UTILITIES_API std::vector<Point3d> removeColinear(const Point3dVector& points, double tol = 0.001);

  /// removes points which cause sudden reversal of lines, tolerance is for length of dot product after normalizing each line segment
  UTILITIES_API std::vector<Point3d> removeSpikes(const Point3dVector& points, double tol = 0.001);

  /// return distance between two points
  UTILITIES_API double getDistance(const Point3d& point1, const Point3d& point2);

  /// return angle (in radians) between two vectors
  UTILITIES_API double getAngle(const Vector3d& vector1, const Vector3d& vector2);
  
  /// compute distance in meters between two points on the Earth's surface
  /// lat and lon are specified in degrees
  UTILITIES_API double getDistanceLatLon(double lat1, double lon1, double lat2, double lon2);

  /// check if two vectors of points are equal (within tolerance) irregardless of initial ordering.
  UTILITIES_API bool circularEqual(const Point3dVector& points1, const Point3dVector& points2, double tol = 0.001);

} // openstudio

#endif //UTILITIES_GEOMETRY_GEOMETRY_HPP
