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

#include "Geometry.hpp"
#include "Transformation.hpp"

#include "../core/Assert.hpp"

#include <boost/math/constants/constants.hpp>

#include <polypartition/polypartition.h>

#include <list>

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

  std::vector<Point3d> removeCollinear(const Point3dVector& points, double tol)
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

  double getDistancePointToLineSegment(const Point3d& point, const std::vector<Point3d>& lineSegment)
  {
    if (lineSegment.size() != 2){
      return 0;
    }

    // http://paulbourke.net/geometry/pointlineplane/

    Point3d point1 = lineSegment[0];
    Point3d point2 = lineSegment[1];

    Vector3d p2p1 = point2-point1;
    Vector3d p3p1 = point-point1;

    double d12 = p2p1.length();
    if (d12 < 1.0e-12){
      return p3p1.length();
    }

    Point3d closestPoint;
    double u = p3p1.dot(p2p1) / (d12*d12);
    if (u < 0){
      closestPoint = point1;
    }else if (u > 1){
      closestPoint = point2;
    }else{
      closestPoint = point1 + u*p2p1;
    }

    Vector3d diff = point - closestPoint;

    return diff.length();
  }

  double getDistancePointToTriangle(const Point3d& point, const std::vector<Point3d>& triangle)
  {
    if (triangle.size() != 3){
      return 0;
    }

    //Distance Between Point and Triangle in 3D
    //David Eberly
    //Geometric Tools, LLC
    //http://www.geometrictools.com/

    //T(s; t) = B+sE0+tE1

    Point3d B = triangle[0];
    Vector3d E0 = triangle[1] - triangle[0];
    Vector3d E1 = triangle[2] - triangle[0];
    Vector3d BminusP = B - point;

    double b = E0.dot(E1);

    if (std::abs(b) > 1.0-1.0E-12){
      // triangle is collinear
      return 0;
    }

    double a = E0.dot(E0);
    double c = E1.dot(E1);
    double d = E0.dot(BminusP);
    double e = E1.dot(BminusP);
    // double f = BminusP.dot(BminusP); // unused

    double det = a*c-b*b; 
    double s = b*e-c*d; 
    double t = b*d-a*e;

    Point3d closestPoint;

    if ( s+t <= det ) {
      if ( s < 0 ) {  
        if ( t < 0 ) { 
          //region 4, closest to point triangle[0] 
          return getDistance(point, triangle[0]);
        } else { 
          //region 3, closest to line triangle[0] to triangle[2] 
          std::vector<Point3d> line;
          line.push_back(triangle[0]);
          line.push_back(triangle[2]);
          return getDistancePointToLineSegment(point, line); 
        } 
      } else if ( t < 0 ) { 
        //region 5, closest to line triangle[0] to triangle[1] 
        std::vector<Point3d> line;
        line.push_back(triangle[0]);
        line.push_back(triangle[1]);
        return getDistancePointToLineSegment(point, line);
      } else { 
        //region 0, closest point is inside triangle
        double invDet = 1.0/det;
        closestPoint = B + invDet*s*E0 + invDet*t*E1;
      }
    } else {
      if ( s < 0 ) { 
        //region 2, closest to point triangle[2]
        return getDistance(point, triangle[2]);
      } else if ( t < 0 ) { 
        //region 6, closest to point triangle[1]
        return getDistance(point, triangle[1]);
      } else { 
        //region 1, closest to line triangle[1] to triangle[2]
        std::vector<Point3d> line;
        line.push_back(triangle[1]);
        line.push_back(triangle[2]);
        return getDistancePointToLineSegment(point, line);
      }
    }
  
    Vector3d diff = point-closestPoint;
    return diff.length();
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

  Point3d getCombinedPoint(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol)
  {
    for (const Point3d& otherPoint : allPoints){
      if (std::sqrt(std::pow(point3d.x()-otherPoint.x(), 2) + std::pow(point3d.y()-otherPoint.y(), 2) + std::pow(point3d.z()-otherPoint.z(), 2)) < tol){
        return otherPoint;
      }
    }
    allPoints.push_back(point3d);
    return point3d;
  }

  std::vector<std::vector<Point3d> > computeTriangulation(const Point3dVector& vertices, const std::vector<std::vector<Point3d> >& holes, double tol)
  {
    std::vector<std::vector<Point3d> > result;

    if (vertices.size () < 3){
      return result;
    }

    std::vector<Point3d> allPoints;

    // convert input to vector of TPPLPoly
    std::list<TPPLPoly> polys;

    TPPLPoly outerPoly; // must be counter-clockwise
    outerPoly.Init(vertices.size());
    outerPoly.SetHole(false);
    for(unsigned i = 0; i < vertices.size(); ++i){

      // should all have zero z coordinate now
      double z = vertices[i].z();
      if (abs(z) > tol){
        LOG_FREE(Error, "utilities.geometry.computeTriangulation", "All points must be on z = 0 plane for triangulation methods");
        return result;
      }

      Point3d point = getCombinedPoint(vertices[i], allPoints, tol);
      outerPoly[i].x = point.x();
      outerPoly[i].y = point.y();
    }
    outerPoly.SetOrientation(TPPL_CCW);
    polys.push_back(outerPoly);


    for (const std::vector<Point3d>& holeVertices : holes){

      if (holeVertices.size () < 3){
        LOG_FREE(Error, "utilities.geometry.computeTriangulation", "Hole has fewer than 3 points, ignoring");
        continue;
      }

      TPPLPoly innerPoly; // must be clockwise
      innerPoly.Init(holeVertices.size());
      innerPoly.SetHole(true);
      //std::cout << "inner :";
      for(unsigned i = 0; i < holeVertices.size(); ++i){

        // should all have zero z coordinate now
        double z = holeVertices[i].z();
        if (abs(z) > tol){
          LOG_FREE(Error, "utilities.geometry.computeTriangulation", "All points must be on z = 0 plane for triangulation methods");
          return result;
        }

        Point3d point = getCombinedPoint(holeVertices[i], allPoints, tol);
        innerPoly[i].x = point.x();
        innerPoly[i].y = point.y();
      }
      innerPoly.SetOrientation(TPPL_CW);
      polys.push_back(innerPoly);
    }

    // do partitioning
    TPPLPartition pp;
    std::list<TPPLPoly> resultPolys;
    int test = pp.Triangulate_EC(&polys,&resultPolys);
    if (test == 0){
      LOG_FREE(Error, "utilities.geometry.computeTriangulation", "Failed to partition polygon");
      return result;
    }

    // convert back to vertices
    std::list<TPPLPoly>::iterator it, itend;
    //std::cout << "Start" << std::endl;
    for(it = resultPolys.begin(), itend = resultPolys.end(); it != itend; ++it){
      std::vector<Point3d> triangle;
      for (long i = 0; i < it->GetNumPoints(); ++i){
        TPPLPoint point = it->GetPoint(i);
        triangle.push_back(Point3d(point.x, point.y, 0));
      }
      //std::cout << triangle << std::endl;
      result.push_back(triangle);
    }
    //std::cout << "End" << std::endl;

    return result;
  }

  std::vector<Point3d> moveVerticesTowardsPoint(const Point3dVector& vertices, const Point3d& point, double distance)
  {
    Point3dVector result;
    for (const Point3d& vertex : vertices){
      Vector3d vector = point-vertex;
      vector.setLength(distance);
      result.push_back(vertex+vector);
    }
    return result;
  }

} // openstudio
