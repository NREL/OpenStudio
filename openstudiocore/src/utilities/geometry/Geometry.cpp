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

#include <utilities/geometry/Geometry.hpp>
#include <utilities/geometry/Transformation.hpp>

#include <utilities/core/Assert.hpp>

#include <boost/math/constants/constants.hpp>

namespace openstudio{
  /// convert degrees to radians
  double degToRad(double degrees)
  {
    return degrees*boost::math::constants::pi<double>()/180.0;
  }

  /// convert radians to degrees
  double radToDeg(double radians)
  {
    return radians*180.0/boost::math::constants::pi<double>();
  }

  /// compute area from surface as Point3dVector
  boost::optional<double> getArea(const Point3dVector& points)
  {
    boost::optional<double> result;
    OptionalVector3d newall = getNewallVector(points);
    if (newall){
      result = newall->length() / 2.0;
    }
    return result;
  }

  // compute Newall vector from Point3dVector, direction is same as outward normal
  // magnitude is twice the area
  OptionalVector3d getNewallVector(const Point3dVector& points)
  {
    OptionalVector3d result;
    unsigned N = points.size();
    if (N >= 3){
      Vector3d vec;
      for (unsigned i = 1; i < N-1; ++i){
        Vector3d v1 = points[i] - points[0];
        Vector3d v2 = points[i+1] - points[0];
        vec += v1.cross(v2);
      }
     result = vec;
   }
   return result;
  }

  // compute outward normal from Point3dVector
  OptionalVector3d getOutwardNormal(const Point3dVector& points)
  {
    OptionalVector3d result = getNewallVector(points);
    if (result){
      if (!result->normalize()){
        result.reset();
      }
    }
    return result;
  }

  /// compute centroid from surface as Point3dVector
  OptionalPoint3d getCentroid(const Point3dVector& points)
  {
    OptionalPoint3d result;

    if (points.size() >= 3){
      // convert to face coordinates
      Transformation alignFace = Transformation::alignFace(points);
      Point3dVector surfacePoints = alignFace.inverse()*points;

      unsigned N = surfacePoints.size();
      double A = 0;
      double cx = 0;
      double cy = 0;
      for (unsigned i = 0; i < N; ++i){
        double x1, x2, y1, y2;
        if (i == N-1){
          x1 = surfacePoints[i].x();
          x2 = surfacePoints[0].x();
          y1 = surfacePoints[i].y();
          y2 = surfacePoints[0].y();
        }else{
          x1 = surfacePoints[i].x();
          x2 = surfacePoints[i+1].x();
          y1 = surfacePoints[i].y();
          y2 = surfacePoints[i+1].y();
        }

        double dA = (x1*y2-x2*y1);
        A += 0.5*dA;
        cx += (x1+x2)*dA;
        cy += (y1+y2)*dA;
      }

      if (A > 0){
        // centroid in face coordinates
        Point3d surfaceCentroid(cx/(6.0*A), cy/(6.0*A), 0.0);

        // centroid
        result = alignFace*surfaceCentroid;
      }
    }
    return result;
  }

  /// reorder points to upper-left-corner convention
  Point3dVector reorderULC(const Point3dVector& points)
  {
    unsigned N = points.size();
    if (N < 3){
      return Point3dVector();
    }

    // transformation to align face
    Transformation t = Transformation::alignFace(points);
    Point3dVector facePoints = t.inverse()*points;

    // find ulc index in face coordinates
    double maxY = std::numeric_limits<double>::min();
    double minX = std::numeric_limits<double>::max();
    unsigned ulcIndex = 0;
    for(unsigned i = 0; i < N; ++i){
      OS_ASSERT(std::abs(facePoints[i].z()) < 0.001);
      if ((maxY < facePoints[i].y()) || ((maxY < facePoints[i].y() + 0.00001) && (minX > facePoints[i].x()))){
        ulcIndex = i;
        maxY = facePoints[i].y();
        minX = facePoints[i].x();
      }
    }

    // no-op
    if (ulcIndex == 0){
      return points;
    }

    // create result
    Point3dVector result;
    std::copy (points.begin() + ulcIndex, points.end(), std::back_inserter(result));
    std::copy (points.begin(), points.begin() + ulcIndex, std::back_inserter(result));
    OS_ASSERT(result.size() == N);
    return result;
  }

  std::vector<Point3d> removeColinear(const Point3dVector& points, double tol)
  {
    unsigned N = points.size();
    if (N < 3){
      return points;
    }

    std::vector<Point3d> result;
    Point3d lastPoint = points[0];
    result.push_back(lastPoint);

    for (unsigned i = 1; i < N; ++i){
      Point3d currentPoint = points[i];
      Point3d nextPoint = points[0];
      if (i < N-1){
        nextPoint = points[i+1];
      }

      Vector3d a = (currentPoint - lastPoint);
      Vector3d b = (nextPoint - currentPoint);

      // if these fail to normalize we have zero length vectors (e.g. adjacent points)
      if (a.normalize()){
        if (b.normalize()){

          Vector3d c = a.cross(b);
          if (c.length() >= tol){
            // cross product is significant
            result.push_back(currentPoint);
            lastPoint = currentPoint;
          }else{
            // see if dot product is near -1
            double d = a.dot(b);
            if (d <= -1.0 + tol){
              // this is a line reversal
              result.push_back(currentPoint);
              lastPoint = currentPoint;
            }
          }
        }
      }
    }

    return result;
  }

  double getDistance(const Point3d& point1, const Point3d& point2) {
    double dx = point1.x() - point2.x();
    double dy = point1.y() - point2.y();
    double dz = point1.z() - point2.z();
    double result = std::sqrt(dx*dx + dy*dy + dz*dz);
    return result;
  }

  double getAngle(const Vector3d& vector1, const Vector3d& vector2) {
    Vector3d working1(vector1);
    working1.normalize();
    Vector3d working2(vector2);
    working2.normalize();
    return acos(working1.dot(working2));
  }

  /// compute distance in meters between two points on the Earth's surface
  /// lat and lon are specified in degrees
  double getDistanceLatLon(double lat1, double lon1, double lat2, double lon2)
  {

    // for more accuracy would want to use WGS-84 ellipsoid params and Vincenty formula

    // Haversine formula 
    double R = 6371000; // Earth radius meters
    double deltaLat = degToRad(lat2-lat1);
    double deltaLon = degToRad(lon2-lon1); 
    double a = sin(deltaLat/2) * sin(deltaLat/2) +
               cos(degToRad(lat1)) * cos(degToRad(lat2)) * 
               sin(deltaLon/2) * sin(deltaLon/2); 
    double c = 2 * atan2(sqrt(a), sqrt(1-a)); 
    double d = R * c;

    return d;
  }

  bool circularEqual(const Point3dVector& points1, const Point3dVector& points2, double tol)
  {
    unsigned N = points1.size();
    if (N != points2.size()){
      return false;
    }

    if (N == 0){
      return true;
    }

    bool result = false;

    // look for a common starting point
    for (unsigned i = 0; i < N; ++i){
      if (getDistance(points1[0], points2[i]) <= tol){

        result = true;

        // check all other points
        for (unsigned j = 0; j < N; ++j){
          if (getDistance(points1[j], points2[(i + j) % N]) > tol){
            result = false;
            break;
          }
        }
      }

      if (result){
        return result;
      }
    }

    return result;
  }

} // openstudio
