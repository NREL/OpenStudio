/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "Geometry.hpp"
#include "Intersection.hpp"
#include "Transformation.hpp"
#include "Point3d.hpp"
#include "PointLatLon.hpp"
#include "Vector3d.hpp"

#include "../core/Assert.hpp"

#include <boost/math/constants/constants.hpp>

#include <polypartition/polypartition.h>

#include <list>

#include <QPolygon>

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
    PointLatLon p1(lat1, lon1);
    PointLatLon p2(lat2, lon2);
    return (p1 - p2);
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

    // check input
    if (vertices.size () < 3){
      return result;
    }

    boost::optional<Vector3d> normal = getOutwardNormal(vertices);
    if (!normal || normal->z() > -0.999){
      return result;
    }

    for (const auto& hole : holes){
      normal = getOutwardNormal(hole);
      if (!normal || normal->z() > -0.999){
        return result;
      }
    }

    std::vector<Point3d> allPoints;

    // PolyPartition does not support holes which intersect the polygon or share an edge
    // if any hole is not fully contained we will use boost to remove all the holes
    bool polyPartitionHoles = true;
    for (const std::vector<Point3d>& hole : holes){
      if (!within(hole, vertices, tol)){
        // PolyPartition can't handle this
        polyPartitionHoles = false;
        break;
      }
    }

    if (!polyPartitionHoles){
      // use boost to do all the intersections
      std::vector<std::vector<Point3d> > allFaces = subtract(vertices, holes, tol);
      std::vector<std::vector<Point3d> > noHoles;
      for (const std::vector<Point3d>& face : allFaces){
        std::vector<std::vector<Point3d> > temp = computeTriangulation(face, noHoles);
        result.insert(result.end(), temp.begin(), temp.end());
      }
      return result;
    }

    // convert input to vector of TPPLPoly
    std::list<TPPLPoly> polys;

    TPPLPoly outerPoly; // must be counter-clockwise, input vertices are clockwise
    outerPoly.Init(vertices.size());
    outerPoly.SetHole(false);
    unsigned n = vertices.size();
    for(unsigned i = 0; i < n; ++i){

      // should all have zero z coordinate now
      double z = vertices[n-i-1].z();
      if (abs(z) > tol){
        LOG_FREE(Error, "utilities.geometry.computeTriangulation", "All points must be on z = 0 plane for triangulation methods");
        return result;
      }

      Point3d point = getCombinedPoint(vertices[n-i-1], allPoints, tol);
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

      TPPLPoly innerPoly; // must be clockwise, input vertices are clockwise
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
      test = pp.Triangulate_MONO(&polys, &resultPolys);
    }
    if (test == 0){
      LOG_FREE(Error, "utilities.geometry.computeTriangulation", "Failed to partition polygon");
      return result;
    }

    // convert back to vertices
    std::list<TPPLPoly>::iterator it, itend;
    //std::cout << "Start" << std::endl;
    for(it = resultPolys.begin(), itend = resultPolys.end(); it != itend; ++it){

      it->SetOrientation(TPPL_CW);

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

  std::vector<Point3d> reverse(const Point3dVector& vertices)
  {
    std::vector<Point3d> result(vertices);
    std::reverse(result.begin(), result.end());
    return result;
  }

  bool applyViewAndDaylightingGlassRatios(double viewGlassToWallRatio, double daylightingGlassToWallRatio,
    double desiredViewGlassSillHeight, double desiredDaylightingGlassHeaderHeight,
    double exteriorShadingProjectionFactor, double interiorShelfProjectionFactor,
    const Point3dVector& vertices, Point3dVector& viewVertices,
    Point3dVector& daylightingVertices, Point3dVector& exteriorShadingVertices,
    Point3dVector& interiorShelfVertices)
  {
    // check inputs for reasonableness
    double totalWWR = viewGlassToWallRatio + daylightingGlassToWallRatio;
    if (totalWWR == 0){
      // requesting no glass? remove existing windows?
      return false;
    }else if (totalWWR < 0.0 || totalWWR >= 1.0){
      return false;
    }

    boost::optional<double> grossArea = getArea(vertices);
    if (!grossArea){
      return false;
    }

    Transformation transformation = Transformation::alignFace(vertices);
    Point3dVector faceVertices = transformation.inverse() * vertices;

    if (faceVertices.empty()){
      return false;
    }

    bool doViewGlass = (viewGlassToWallRatio > 0);
    bool doDaylightGlass = (daylightingGlassToWallRatio > 0);
    bool doExteriorShading = (doViewGlass && (exteriorShadingProjectionFactor > 0));
    bool doInteriorShelf = (doDaylightGlass && (interiorShelfProjectionFactor > 0));
    bool doViewAndDaylightGlass = (doViewGlass && doDaylightGlass);

    // ignore these user arguments?
    if (!doViewGlass){
      desiredViewGlassSillHeight = 0.0;
    }
    if (!doDaylightGlass){
      desiredDaylightingGlassHeaderHeight = 0.0;
    }

    // new coordinate system has z' in direction of outward normal, y' is up
    double xmin = std::numeric_limits<double>::max();
    double xmax = std::numeric_limits<double>::min();
    double ymin = std::numeric_limits<double>::max();
    double ymax = std::numeric_limits<double>::min();
    for (const Point3d& faceVertex : faceVertices){
      xmin = std::min(xmin, faceVertex.x());
      xmax = std::max(xmax, faceVertex.x());
      ymin = std::min(ymin, faceVertex.y());
      ymax = std::max(ymax, faceVertex.y());
    }
    if ((xmin > xmax) || (ymin > ymax)){
      return false;
    }

    double oneInch = 0.0254;

    // DLM: preserve a 1" gap between window and edge to keep SketchUp happy
    double minGlassToEdgeDistance = oneInch;
    double minViewToDaylightDistance = 0;
    if (doViewAndDaylightGlass){
      minViewToDaylightDistance = oneInch;
    }

    // wall parameters
    double wallWidth = xmax - xmin;
    double wallHeight = ymax - ymin;
    double wallArea = wallWidth*wallHeight;

    if (wallWidth < 2*minGlassToEdgeDistance){
      return false;
    }

    if (wallHeight < 2*minGlassToEdgeDistance + minViewToDaylightDistance){
      return false;
    }

    // check against actual surface area to ensure this is a rectangle?
    if (std::abs(wallArea - grossArea.get()) > oneInch*oneInch){
      return false;
    }

    double maxWindowArea = wallArea - 2*wallHeight*minGlassToEdgeDistance - (wallWidth-2*minGlassToEdgeDistance)*(2*minGlassToEdgeDistance + minViewToDaylightDistance);
    double requestedViewArea = viewGlassToWallRatio*wallArea;
    double requestedDaylightingArea = daylightingGlassToWallRatio*wallArea;
    double requestedTotalWindowArea = totalWWR*wallArea;

    if (requestedTotalWindowArea > maxWindowArea){
      return false;
    }

    // view glass parameters
    double viewMinX = 0;
    double viewMinY = 0;
    double viewWidth = 0;
    double viewHeight = 0;

    // daylighting glass parameters
    double daylightingWidth = 0;
    double daylightingHeight = 0;
    double daylightingMinX = 0;
    double daylightingMinY = 0;

    // initial free parameters
    double viewWidthInset = minGlassToEdgeDistance;
    double viewSillHeight = std::max(desiredViewGlassSillHeight, minGlassToEdgeDistance);
    double daylightingWidthInset = minGlassToEdgeDistance;
    double daylightingHeaderHeight = std::max(desiredDaylightingGlassHeaderHeight, minGlassToEdgeDistance);

    bool converged = false;
    for (unsigned i = 0; i < 100; ++i){

      // view glass parameters
      viewMinX = viewWidthInset;
      viewMinY = viewSillHeight;
      viewWidth = wallWidth - 2*viewWidthInset;
      viewHeight = requestedViewArea/viewWidth;

      // daylighting glass parameters
      daylightingWidth = wallWidth - 2*daylightingWidthInset;
      daylightingHeight = requestedDaylightingArea/daylightingWidth;
      daylightingMinX = viewWidthInset;
      daylightingMinY = wallHeight - daylightingHeaderHeight - daylightingHeight;

      if (viewMinY + viewHeight + minViewToDaylightDistance > daylightingMinY){
        // windows overlap or exceed maximum size

        if (doViewAndDaylightGlass){

          // try shrinking vertical offsets
          viewSillHeight = std::max(viewSillHeight - oneInch, minGlassToEdgeDistance);
          daylightingHeaderHeight = std::max(daylightingHeaderHeight - oneInch, minGlassToEdgeDistance);

        }else if (doViewGlass){

          // solve directly
          viewSillHeight = wallHeight - minGlassToEdgeDistance - viewHeight;

          if (viewSillHeight < minGlassToEdgeDistance){
            // cannot make window this large
            return false;
          }

        }else if (doDaylightGlass){

          // solve directly
          daylightingHeaderHeight = wallHeight - minGlassToEdgeDistance - daylightingHeight;

          if (daylightingHeaderHeight < minGlassToEdgeDistance){
            // cannot make window this large
            return false;
          }

        }

      }else{

        converged = true;
        break;

      }
    }

    if (!converged){
      return false;
    }

    QPolygonF surfacePolygon;
    for (const Point3d& point : faceVertices){
      if (std::abs(point.z()) > 0.001){
        LOG_FREE(Warn, "utilities.geometry.applyViewAndDaylightingGlassRatios", "Surface point z not on plane, z =" << point.z());
      }
      surfacePolygon << QPointF(point.x(),point.y());
    }
    // close the polygon
    surfacePolygon << QPointF(faceVertices[0].x(), faceVertices[0].y());

    if (doViewGlass){
      viewVertices.push_back(Point3d(viewMinX, viewMinY + viewHeight, 0));
      viewVertices.push_back(Point3d(viewMinX, viewMinY, 0));
      viewVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY, 0));
      viewVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY + viewHeight, 0));

      QPolygonF windowPolygon;
      for (const Point3d& point : viewVertices){
        if (std::abs(point.z()) > 0.001){
          LOG_FREE(Warn, "utilities.geometry.applyViewAndDaylightingGlassRatios", "Surface point z not on plane, z =" << point.z());
        }
        windowPolygon << QPointF(point.x(),point.y());
      }
      // close the polygon
      windowPolygon << QPointF(viewVertices[0].x(), viewVertices[0].y());

      // sub surface must be fully contained by base surface
      for (const QPointF& point : windowPolygon){
        if (!surfacePolygon.containsPoint(point, Qt::OddEvenFill)){
          LOG_FREE(Debug, "utilities.geometry.applyViewAndDaylightingGlassRatios", "Surface does not fully contain SubSurface");
          return false;
        }
      }
    }

    if (doDaylightGlass){
      daylightingVertices.push_back(Point3d(daylightingMinX, daylightingMinY + daylightingHeight, 0));
      daylightingVertices.push_back(Point3d(daylightingMinX, daylightingMinY, 0));
      daylightingVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY, 0));
      daylightingVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY + daylightingHeight, 0));

      QPolygonF windowPolygon;
      for (const Point3d& point : daylightingVertices){
        if (std::abs(point.z()) > 0.001){
          LOG_FREE(Warn, "utilities.geometry.applyViewAndDaylightingGlassRatios", "Surface point z not on plane, z =" << point.z());
        }
        windowPolygon << QPointF(point.x(),point.y());
      }
      // close the polygon
      windowPolygon << QPointF(daylightingVertices[0].x(), daylightingVertices[0].y());

      // sub surface must be fully contained by base surface
      for (const QPointF& point : windowPolygon){
        if (!surfacePolygon.containsPoint(point, Qt::OddEvenFill)){
          LOG_FREE(Debug, "utilities.geometry.applyViewAndDaylightingGlassRatios", "Surface does not fully contain SubSurface");
          return false;
        }
      }
    }

    if (doExteriorShading) {
      exteriorShadingVertices.push_back(Point3d(viewMinX, viewMinY + viewHeight, 0));
      exteriorShadingVertices.push_back(Point3d(viewMinX, viewMinY + viewHeight, exteriorShadingProjectionFactor*viewHeight));
      exteriorShadingVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY + viewHeight, exteriorShadingProjectionFactor*viewHeight));
      exteriorShadingVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY + viewHeight, 0));
    }

    if (doInteriorShelf) {
      interiorShelfVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY, 0));
      interiorShelfVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY, -interiorShelfProjectionFactor*daylightingHeight));
      interiorShelfVertices.push_back(Point3d(daylightingMinX, daylightingMinY, -interiorShelfProjectionFactor*daylightingHeight));
      interiorShelfVertices.push_back(Point3d(daylightingMinX, daylightingMinY, 0));
    }

    // put all vertices back into input coordinate system
    viewVertices = transformation*viewVertices;
    daylightingVertices = transformation*daylightingVertices;
    exteriorShadingVertices = transformation*exteriorShadingVertices;
    interiorShelfVertices = transformation*interiorShelfVertices;

    return true;
  }

} // openstudio
