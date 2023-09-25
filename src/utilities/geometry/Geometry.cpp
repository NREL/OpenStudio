/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Geometry.hpp"
#include "Intersection.hpp"
#include "Transformation.hpp"
#include "Vector3d.hpp"

#include "../core/Assert.hpp"

#include <boost/math/constants/constants.hpp>

#include <polypartition/polypartition.h>

#include <cmath>

namespace openstudio {
/// convert degrees to radians
double degToRad(double degrees) {
  return degrees * boost::math::constants::pi<double>() / 180.0;
}

/// convert radians to degrees
double radToDeg(double radians) {
  return radians * 180.0 / boost::math::constants::pi<double>();
}

/// compute area from surface as Point3dVector
boost::optional<double> getArea(const Point3dVector& points) {
  boost::optional<double> result;
  OptionalVector3d newell = getNewellVector(points);
  if (newell) {
    result = newell->length() / 2.0;
  }
  return result;
}

// compute Newell vector from Point3dVector, direction is same as outward normal
// magnitude is twice the area
OptionalVector3d getNewellVector(const Point3dVector& points) {
  OptionalVector3d result;
  const size_t N = points.size();
  if (N >= 3) {
    Vector3d vec;
    for (unsigned i = 1; i < N - 1; ++i) {
      const Vector3d v1 = points[i] - points[0];
      const Vector3d v2 = points[i + 1] - points[0];
      vec += v1.cross(v2);
    }
    result = vec;
  }
  return result;
}

// compute outward normal from Point3dVector
OptionalVector3d getOutwardNormal(const Point3dVector& points) {
  OptionalVector3d result = getNewellVector(points);
  if (result) {
    if (!result->normalize()) {
      result.reset();
    }
  }
  return result;
}

/// compute centroid from surface as Point3dVector
OptionalPoint3d getCentroid(const Point3dVector& points) {
  OptionalPoint3d result;

  if (points.size() >= 3) {
    // convert to face coordinates
    const Transformation alignFace = Transformation::alignFace(points);
    Point3dVector surfacePoints = alignFace.inverse() * points;

    const size_t N = surfacePoints.size();
    double A = 0;
    double cx = 0;
    double cy = 0;
    for (size_t i = 0; i < N; ++i) {
      double x1 = 0.0;
      double x2 = 0.0;
      double y1 = 0.0;
      double y2 = 0.0;
      if (i == N - 1) {
        x1 = surfacePoints[i].x();
        x2 = surfacePoints[0].x();
        y1 = surfacePoints[i].y();
        y2 = surfacePoints[0].y();
      } else {
        x1 = surfacePoints[i].x();
        x2 = surfacePoints[i + 1].x();
        y1 = surfacePoints[i].y();
        y2 = surfacePoints[i + 1].y();
      }

      const double dA = (x1 * y2 - x2 * y1);
      A += 0.5 * dA;
      cx += (x1 + x2) * dA;
      cy += (y1 + y2) * dA;
    }

    if (A > 0) {
      // centroid in face coordinates
      const Point3d surfaceCentroid(cx / (6.0 * A), cy / (6.0 * A), 0.0);

      // centroid
      result = alignFace * surfaceCentroid;
    }
  }
  return result;
}

/// reorder points to upper-left-corner convention
Point3dVector reorderULC(const Point3dVector& points) {
  const size_t N = points.size();
  if (N < 3) {
    return {};
  }

  // transformation to align face
  const Transformation t = Transformation::alignFace(points);
  Point3dVector facePoints = t.inverse() * points;

  // find ulc index in face coordinates
  double maxY = std::numeric_limits<double>::min();
  double minX = std::numeric_limits<double>::max();
  size_t ulcIndex = 0;
  for (size_t i = 0; i < N; ++i) {
    OS_ASSERT(std::abs(facePoints[i].z()) < 0.001);
    if ((maxY < facePoints[i].y()) || ((maxY < facePoints[i].y() + 0.00001) && (minX > facePoints[i].x()))) {
      ulcIndex = i;
      maxY = facePoints[i].y();
      minX = facePoints[i].x();
    }
  }

  // no-op
  if (ulcIndex == 0) {
    return points;
  }

  // create result
  Point3dVector result = points;
  std::rotate(result.begin(), std::next(result.begin(), ulcIndex), result.end());
  return result;
}

std::vector<Point3d> removeCollinear(const Point3dVector& points, double tol) {
  const Transformation t = Transformation::alignFace(points);
  std::vector<Point3d> result = t * simplify(t.inverse() * points, true, tol);
  return result;
}

std::vector<Point3d> removeCollinearLegacy(const Point3dVector& points, double tol) {
  const size_t N = points.size();
  if (N < 3) {
    return points;
  }

  std::vector<Point3d> result;
  Point3d lastPoint = points[0];
  result.push_back(lastPoint);

  for (unsigned i = 1; i < N; ++i) {
    const Point3d& currentPoint = points[i];
    Point3d nextPoint = points[0];
    if (i < N - 1) {
      nextPoint = points[i + 1];
    }

    Vector3d a = (currentPoint - lastPoint);
    Vector3d b = (nextPoint - currentPoint);

    // if these fail to normalize we have zero length vectors (e.g. adjacent points)
    if (a.normalize()) {
      if (b.normalize()) {

        const Vector3d c = a.cross(b);
        if (c.length() >= tol) {
          // cross product is significant
          result.push_back(currentPoint);
          lastPoint = currentPoint;
        } else {
          // see if dot product is near -1
          const double d = a.dot(b);
          if (d <= -1.0 + tol) {
            // this is a line reversal
            result.push_back(currentPoint);
            lastPoint = currentPoint;
          }
        }
      }
    }
  }

  size_t iBegin = 0;
  size_t iEnd = result.size();

  bool resizeBegin = true;
  while (resizeBegin) {
    resizeBegin = false;
    const unsigned newN = iEnd - iBegin;
    if (newN > 3) {
      Vector3d a = (result[iBegin] - result[iEnd - 1]);
      Vector3d b = (result[iBegin + 1] - result[iBegin]);
      if (a.normalize()) {
        if (b.normalize()) {
          const double d = a.dot(b);
          if (d >= 1.0 - tol) {
            iBegin++;
            resizeBegin = true;
          }
        } else {
          iBegin++;
          resizeBegin = true;
        }
      } else {
        iBegin++;
        resizeBegin = true;
      }
    }
  }

  bool resizeEnd = true;
  while (resizeEnd) {
    resizeEnd = false;
    const unsigned newN = iEnd - iBegin;
    if (newN > 3) {
      Vector3d a = (result[iEnd - 1] - result[iEnd - 2]);
      Vector3d b = (result[iBegin] - result[iEnd - 1]);
      if (a.normalize()) {
        if (b.normalize()) {
          const double d = a.dot(b);
          if (d >= 1.0 - tol) {
            iEnd--;
            resizeEnd = true;
          }
        } else {
          iEnd--;
          resizeEnd = true;
        }
      } else {
        iEnd--;
        resizeEnd = true;
      }
    }
  }

  result = std::vector<Point3d>(result.begin() + iBegin, result.begin() + iEnd);
  return result;
}

double getDistanceSquared(const Point3d& point1, const Point3d& point2) {
  const double dx = point1.x() - point2.x();
  const double dy = point1.y() - point2.y();
  const double dz = point1.z() - point2.z();
  const double result = dx * dx + dy * dy + dz * dz;
  return result;
}

double getDistance(const Point3d& point1, const Point3d& point2) {
  double result = std::sqrt(getDistanceSquared(point1, point2));
  return result;
}

double getDistancePointToLineSegment(const Point3d& point, const std::vector<Point3d>& lineSegment) {
  if (lineSegment.size() != 2) {
    return 0;
  }

  // http://paulbourke.net/geometry/pointlineplane/

  Point3d point1 = lineSegment[0];
  Point3d point2 = lineSegment[1];

  Vector3d p2p1 = point2 - point1;
  Vector3d p3p1 = point - point1;

  double d12 = p2p1.length();
  if (d12 < 1.0e-12) {
    return p3p1.length();
  }

  Point3d closestPoint;
  double u = p3p1.dot(p2p1) / (d12 * d12);
  if (u < 0) {
    closestPoint = point1;
  } else if (u > 1) {
    closestPoint = point2;
  } else {
    closestPoint = point1 + u * p2p1;
  }

  Vector3d diff = point - closestPoint;

  return diff.length();
}

double getDistancePointToTriangle(const Point3d& point, const std::vector<Point3d>& triangle) {
  if (triangle.size() != 3) {
    return 0;
  }

  //Distance Between Point and Triangle in 3D
  //David Eberly
  //Geometric Tools, LLC
  //http://www.geometrictools.com/

  //T(s; t) = B+sE0+tE1

  const Point3d& B = triangle[0];
  const Vector3d E0 = triangle[1] - triangle[0];
  const Vector3d E1 = triangle[2] - triangle[0];
  const Vector3d BminusP = B - point;

  const double b = E0.dot(E1);

  if (std::abs(b) > 1.0 - 1.0E-12) {
    // triangle is collinear
    return 0;
  }

  const double a = E0.dot(E0);
  const double c = E1.dot(E1);
  const double d = E0.dot(BminusP);
  const double e = E1.dot(BminusP);
  // const double f = BminusP.dot(BminusP); // unused

  const double det = a * c - b * b;
  const double s = b * e - c * d;
  const double t = b * d - a * e;

  Point3d closestPoint;

  if (s + t <= det) {
    if (s < 0) {
      if (t < 0) {
        //region 4, closest to point triangle[0]
        return getDistance(point, triangle[0]);
      } else {
        //region 3, closest to line triangle[0] to triangle[2]
        std::vector<Point3d> line;
        line.push_back(triangle[0]);
        line.push_back(triangle[2]);
        return getDistancePointToLineSegment(point, line);
      }
    } else if (t < 0) {
      //region 5, closest to line triangle[0] to triangle[1]
      std::vector<Point3d> line;
      line.push_back(triangle[0]);
      line.push_back(triangle[1]);
      return getDistancePointToLineSegment(point, line);
    } else {
      //region 0, closest point is inside triangle
      const double invDet = 1.0 / det;
      closestPoint = B + invDet * s * E0 + invDet * t * E1;
    }
  } else {
    if (s < 0) {
      //region 2, closest to point triangle[2]
      return getDistance(point, triangle[2]);
    } else if (t < 0) {
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

  const Vector3d diff = point - closestPoint;
  return diff.length();
}

double getAngle(const Vector3d& vector1, const Vector3d& vector2) {
  Vector3d working1(vector1);
  working1.normalize();
  Vector3d working2(vector2);
  working2.normalize();
  return acos(working1.dot(working2));
}

bool circularEqual(const Point3dVector& points1, const Point3dVector& points2, double tol) {
  const size_t N = points1.size();
  if (N != points2.size()) {
    return false;
  }

  if (N == 0) {
    return true;
  }

  bool result = false;

  // look for a common starting point
  for (size_t i = 0; i < N; ++i) {
    if (getDistance(points1[0], points2[i]) <= tol) {

      result = true;

      // check all other points
      for (unsigned j = 0; j < N; ++j) {
        if (getDistance(points1[j], points2[(i + j) % N]) > tol) {
          result = false;
          break;
        }
      }
    }

    if (result) {
      return result;
    }
  }

  return result;
}

Point3d getCombinedPoint(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol) {
  for (const Point3d& otherPoint : allPoints) {
    if (std::sqrt(std::pow(point3d.x() - otherPoint.x(), 2) + std::pow(point3d.y() - otherPoint.y(), 2) + std::pow(point3d.z() - otherPoint.z(), 2))
        < tol) {
      return otherPoint;
    }
  }
  allPoints.push_back(point3d);
  return point3d;
}

std::vector<std::vector<Point3d>> computeTriangulation(const Point3dVector& vertices, const std::vector<std::vector<Point3d>>& holes, double tol) {
  std::vector<std::vector<Point3d>> result;

  // check input
  if (vertices.size() < 3) {
    return result;
  }

  boost::optional<Vector3d> normal = getOutwardNormal(vertices);
  if (!normal || normal->z() > -0.999) {
    return result;
  }

  for (const auto& hole : holes) {
    normal = getOutwardNormal(hole);
    if (!normal || normal->z() > -0.999) {
      return result;
    }
  }

  // if holes have been triangulated, rejoin them here before subtraction
  const std::vector<std::vector<Point3d>> newHoles = joinAll(holes, tol);

  std::vector<Point3d> allPoints;

  // PolyPartition does not support holes which intersect the polygon or share an edge
  // if any hole is not fully contained we will use boost to remove all the holes
  bool polyPartitionHoles = true;
  for (const std::vector<Point3d>& hole : newHoles) {
    if (!within(hole, vertices, tol)) {
      // PolyPartition can't handle this
      polyPartitionHoles = false;
      break;
    }
  }

  if (!polyPartitionHoles) {
    // use boost to do all the intersections
    const std::vector<std::vector<Point3d>> allFaces = subtract(vertices, newHoles, tol);
    const std::vector<std::vector<Point3d>> noHoles;
    for (const std::vector<Point3d>& face : allFaces) {
      std::vector<std::vector<Point3d>> temp = computeTriangulation(face, noHoles);
      result.insert(result.end(), temp.begin(), temp.end());
    }
    return result;
  }

  // convert input to vector of TPPLPoly
  std::list<TPPLPoly> polys;

  TPPLPoly outerPoly;  // must be counter-clockwise, input vertices are clockwise
  outerPoly.Init(vertices.size());
  outerPoly.SetHole(false);
  const size_t n = vertices.size();
  for (size_t i = 0; i < n; ++i) {

    // should all have zero z coordinate now
    const double z = vertices[n - i - 1].z();
    if (std::abs(z) > tol) {
      LOG_FREE(Error, "utilities.geometry.computeTriangulation", "All points must be on z = 0 plane for triangulation methods");
      return result;
    }

    const Point3d point = getCombinedPoint(vertices[n - i - 1], allPoints, tol);
    outerPoly[i].x = point.x();
    outerPoly[i].y = point.y();
  }
  outerPoly.SetOrientation(TPPL_CCW);
  polys.push_back(outerPoly);

  for (const std::vector<Point3d>& holeVertices : newHoles) {

    if (holeVertices.size() < 3) {
      LOG_FREE(Error, "utilities.geometry.computeTriangulation", "Hole has fewer than 3 points, ignoring");
      continue;
    }

    TPPLPoly innerPoly;  // must be clockwise, input vertices are clockwise
    innerPoly.Init(holeVertices.size());
    innerPoly.SetHole(true);
    //std::cout << "inner :";
    for (unsigned i = 0; i < holeVertices.size(); ++i) {

      // should all have zero z coordinate now
      const double z = holeVertices[i].z();
      if (std::abs(z) > tol) {
        LOG_FREE(Error, "utilities.geometry.computeTriangulation", "All points must be on z = 0 plane for triangulation methods");
        return result;
      }

      const Point3d point = getCombinedPoint(holeVertices[i], allPoints, tol);
      innerPoly[i].x = point.x();
      innerPoly[i].y = point.y();
    }
    innerPoly.SetOrientation(TPPL_CW);
    polys.push_back(innerPoly);
  }

  // do partitioning
  TPPLPartition pp;
  std::list<TPPLPoly> resultPolys;
  int test = pp.Triangulate_EC(&polys, &resultPolys);
  if (test == 0) {
    test = pp.Triangulate_MONO(&polys, &resultPolys);
  }
  if (test == 0) {
    //std::stringstream ss;
    //ss << "Vertices: " << vertices << '\n';
    //for (const auto& hole : holes){ ss << "Hole:" << hole << '\n'; }
    //std::string testStr = ss.str();
    LOG_FREE(Error, "utilities.geometry.computeTriangulation", "Failed to partition polygon");
    return result;
  }

  // convert back to vertices
  std::list<TPPLPoly>::iterator it;
  std::list<TPPLPoly>::iterator itend;
  //std::cout << "Start" << '\n';
  for (it = resultPolys.begin(), itend = resultPolys.end(); it != itend; ++it) {

    it->SetOrientation(TPPL_CW);

    std::vector<Point3d> triangle;
    for (long i = 0; i < it->GetNumPoints(); ++i) {
      const TPPLPoint point = it->GetPoint(i);
      triangle.emplace_back(point.x, point.y, 0);
    }
    //std::cout << triangle << '\n';
    result.push_back(triangle);
  }
  //std::cout << "End" << '\n';

  return result;
}

std::vector<Point3d> moveVerticesTowardsPoint(const Point3dVector& vertices, const Point3d& point, double distance) {
  Point3dVector result;
  for (const Point3d& vertex : vertices) {
    Vector3d vector = point - vertex;
    vector.setLength(distance);
    result.push_back(vertex + vector);
  }
  return result;
}

std::vector<Point3d> reverse(const Point3dVector& vertices) {
  std::vector<Point3d> result(vertices);
  std::reverse(result.begin(), result.end());
  return result;
}

bool applyViewAndDaylightingGlassRatios(double viewGlassToWallRatio, double daylightingGlassToWallRatio, double desiredViewGlassSillHeight,
                                        double desiredDaylightingGlassHeaderHeight, double exteriorShadingProjectionFactor,
                                        double interiorShelfProjectionFactor, const Point3dVector& surfaceVertices, Point3dVector& viewVertices,
                                        Point3dVector& daylightingVertices, Point3dVector& exteriorShadingVertices,
                                        Point3dVector& interiorShelfVertices) {
  // check inputs for reasonableness
  const double totalWWR = viewGlassToWallRatio + daylightingGlassToWallRatio;
  if (totalWWR == 0) {
    // requesting no glass? remove existing windows?
    return false;
  } else if (totalWWR < 0.0 || totalWWR >= 1.0) {
    return false;
  }

  boost::optional<double> grossArea = getArea(surfaceVertices);
  if (!grossArea) {
    return false;
  }

  const Transformation transformation = Transformation::alignFace(surfaceVertices);
  const Point3dVector faceVertices = transformation.inverse() * surfaceVertices;

  if (faceVertices.empty()) {
    return false;
  }

  const bool doViewGlass = (viewGlassToWallRatio > 0);
  const bool doDaylightGlass = (daylightingGlassToWallRatio > 0);
  const bool doExteriorShading = (doViewGlass && (exteriorShadingProjectionFactor > 0));
  const bool doInteriorShelf = (doDaylightGlass && (interiorShelfProjectionFactor > 0));
  const bool doViewAndDaylightGlass = (doViewGlass && doDaylightGlass);

  // ignore these user arguments?
  if (!doViewGlass) {
    desiredViewGlassSillHeight = 0.0;
  }
  if (!doDaylightGlass) {
    desiredDaylightingGlassHeaderHeight = 0.0;
  }

  // new coordinate system has z' in direction of outward normal, y' is up
  double xmin = std::numeric_limits<double>::max();
  double xmax = std::numeric_limits<double>::min();
  double ymin = std::numeric_limits<double>::max();
  double ymax = std::numeric_limits<double>::min();
  for (const Point3d& faceVertex : faceVertices) {
    xmin = std::min(xmin, faceVertex.x());
    xmax = std::max(xmax, faceVertex.x());
    ymin = std::min(ymin, faceVertex.y());
    ymax = std::max(ymax, faceVertex.y());
  }
  if ((xmin > xmax) || (ymin > ymax)) {
    return false;
  }

  constexpr double oneInch = 0.0254;

  // DLM: preserve a 1" gap between window and edge to keep SketchUp happy
  constexpr double minGlassToEdgeDistance = oneInch;
  double minViewToDaylightDistance = 0;
  if (doViewAndDaylightGlass) {
    minViewToDaylightDistance = oneInch;
  }

  // wall parameters
  const double wallWidth = xmax - xmin;
  const double wallHeight = ymax - ymin;
  const double wallArea = wallWidth * wallHeight;

  if (wallWidth < 2 * minGlassToEdgeDistance) {
    LOG_FREE(Warn, "utilities.geometry.applyViewAndDaylightingGlassRatios",
             "Wall Width (" << wallWidth << "m) is too small to preserve 0.0254m (1 inch) on both sides.");
    return false;
  }

  if (wallHeight < 2 * minGlassToEdgeDistance + minViewToDaylightDistance) {
    LOG_FREE(Warn, "utilities.geometry.applyViewAndDaylightingGlassRatios",
             "Wall Height (" << wallHeight << "m) is too small to preserve 0.0254m (1 inch) on both sides plus the minViewToDaylightDistance ("
                             << minViewToDaylightDistance << "m).");
    return false;
  }

  // check against actual surface area to ensure this is a rectangle?
  if (std::abs(wallArea - grossArea.get()) > oneInch * oneInch) {
    return false;
  }

  const double maxWindowArea = wallArea - 2 * wallHeight * minGlassToEdgeDistance
                               - (wallWidth - 2 * minGlassToEdgeDistance) * (2 * minGlassToEdgeDistance + minViewToDaylightDistance);
  const double requestedViewArea = viewGlassToWallRatio * wallArea;
  const double requestedDaylightingArea = daylightingGlassToWallRatio * wallArea;
  const double requestedTotalWindowArea = totalWWR * wallArea;

  if (requestedTotalWindowArea > maxWindowArea) {
    LOG_FREE(
      Warn, "utilities.geometry.applyViewAndDaylightingGlassRatios",
      "Requested WWR ("
        << totalWWR << ") is greater than the Max WWR (" << maxWindowArea / wallArea
        << "). This limit is meant to preserve mandatory space between window and wall: 0.0254m (1 inch) on all sides + minViewToDaylightDistance ("
        << minViewToDaylightDistance << "m).");
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
  const double viewWidthInset = minGlassToEdgeDistance;
  double viewSillHeight = std::max(desiredViewGlassSillHeight, minGlassToEdgeDistance);
  const double daylightingWidthInset = minGlassToEdgeDistance;
  double daylightingHeaderHeight = std::max(desiredDaylightingGlassHeaderHeight, minGlassToEdgeDistance);

  bool converged = false;
  for (unsigned i = 0; i < 100; ++i) {

    // view glass parameters
    viewMinX = viewWidthInset;
    viewMinY = viewSillHeight;
    viewWidth = wallWidth - 2 * viewWidthInset;
    viewHeight = requestedViewArea / viewWidth;

    // daylighting glass parameters
    daylightingWidth = wallWidth - 2 * daylightingWidthInset;
    daylightingHeight = requestedDaylightingArea / daylightingWidth;
    daylightingMinX = viewWidthInset;
    daylightingMinY = wallHeight - daylightingHeaderHeight - daylightingHeight;

    if (viewMinY + viewHeight + minViewToDaylightDistance > daylightingMinY) {
      // windows overlap or exceed maximum size

      if (doViewAndDaylightGlass) {

        // try shrinking vertical offsets
        viewSillHeight = std::max(viewSillHeight - oneInch, minGlassToEdgeDistance);
        daylightingHeaderHeight = std::max(daylightingHeaderHeight - oneInch, minGlassToEdgeDistance);

      } else if (doViewGlass) {

        // solve directly
        viewSillHeight = wallHeight - minGlassToEdgeDistance - viewHeight;

        if (viewSillHeight < minGlassToEdgeDistance) {
          // cannot make window this large
          return false;
        }

      } else if (doDaylightGlass) {

        // solve directly
        daylightingHeaderHeight = wallHeight - minGlassToEdgeDistance - daylightingHeight;

        if (daylightingHeaderHeight < minGlassToEdgeDistance) {
          // cannot make window this large
          return false;
        }
      }

    } else {

      converged = true;
      break;
    }
  }

  if (!converged) {
    return false;
  }

  Point3dVector surfacePolygon;
  for (const Point3d& point : faceVertices) {
    if (std::abs(point.z()) > 0.001) {
      LOG_FREE(Warn, "utilities.geometry.applyViewAndDaylightingGlassRatios", "Surface point z not on plane, z =" << point.z());
    }
    surfacePolygon.push_back(Point3d(point.x(), point.y(), 0.0));
  }
  std::reverse(surfacePolygon.begin(), surfacePolygon.end());

  if (doViewGlass) {
    viewVertices.push_back(Point3d(viewMinX, viewMinY + viewHeight, 0));
    viewVertices.push_back(Point3d(viewMinX, viewMinY, 0));
    viewVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY, 0));
    viewVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY + viewHeight, 0));

    Point3dVector windowPolygon;
    for (const Point3d& point : viewVertices) {
      if (std::abs(point.z()) > 0.001) {
        LOG_FREE(Warn, "utilities.geometry.applyViewAndDaylightingGlassRatios", "Surface point z not on plane, z =" << point.z());
      }
      windowPolygon.push_back(Point3d(point.x(), point.y(), 0.0));
    }

    // sub surface must be fully contained by base surface
    for (const Point3d& point : windowPolygon) {
      if (!within(point, surfacePolygon, 0.001)) {
        std::cout << "point: " << point << '\n';
        std::cout << "surfacePolygon: " << surfacePolygon << '\n';
        LOG_FREE(Debug, "utilities.geometry.applyViewAndDaylightingGlassRatios", "Surface does not fully contain SubSurface");
        return false;
      }
    }
  }

  if (doDaylightGlass) {
    daylightingVertices.push_back(Point3d(daylightingMinX, daylightingMinY + daylightingHeight, 0));
    daylightingVertices.push_back(Point3d(daylightingMinX, daylightingMinY, 0));
    daylightingVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY, 0));
    daylightingVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY + daylightingHeight, 0));

    Point3dVector windowPolygon;
    for (const Point3d& point : daylightingVertices) {
      if (std::abs(point.z()) > 0.001) {
        LOG_FREE(Warn, "utilities.geometry.applyViewAndDaylightingGlassRatios", "Surface point z not on plane, z =" << point.z());
      }
      windowPolygon.push_back(Point3d(point.x(), point.y(), 0.0));
    }

    // sub surface must be fully contained by base surface
    for (const Point3d& point : windowPolygon) {
      if (!within(point, surfacePolygon, 0.001)) {
        LOG_FREE(Debug, "utilities.geometry.applyViewAndDaylightingGlassRatios", "Surface does not fully contain SubSurface");
        return false;
      }
    }
  }

  if (doExteriorShading) {
    exteriorShadingVertices.push_back(Point3d(viewMinX, viewMinY + viewHeight, 0));
    exteriorShadingVertices.push_back(Point3d(viewMinX, viewMinY + viewHeight, exteriorShadingProjectionFactor * viewHeight));
    exteriorShadingVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY + viewHeight, exteriorShadingProjectionFactor * viewHeight));
    exteriorShadingVertices.push_back(Point3d(viewMinX + viewWidth, viewMinY + viewHeight, 0));
  }

  if (doInteriorShelf) {
    interiorShelfVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY, 0));
    interiorShelfVertices.push_back(Point3d(daylightingMinX + daylightingWidth, daylightingMinY, -interiorShelfProjectionFactor * daylightingHeight));
    interiorShelfVertices.push_back(Point3d(daylightingMinX, daylightingMinY, -interiorShelfProjectionFactor * daylightingHeight));
    interiorShelfVertices.push_back(Point3d(daylightingMinX, daylightingMinY, 0));
  }

  // put all vertices back into input coordinate system
  viewVertices = transformation * viewVertices;
  daylightingVertices = transformation * daylightingVertices;
  exteriorShadingVertices = transformation * exteriorShadingVertices;
  interiorShelfVertices = transformation * interiorShelfVertices;

  return true;
}

bool isAlmostEqual3dPt(const Point3d& lhs, const Point3d& rhs, double tol) {
  // TODO: this is what E+ does... I think I would prefer just calling getDistance...
  return ((std::abs(lhs.x() - rhs.x()) < tol) && (std::abs(lhs.y() - rhs.y()) < tol) && (std::abs(lhs.z() - rhs.z()) < tol));
  // return getDistance(lhs, rhs) < tol;
}

bool isPointOnLineBetweenPoints(const Point3d& start, const Point3d& end, const Point3d& test, double tol) {
  // The tolerance has to be low enough. Take for eg a plenum that has an edge that's 30meters long, you risk adding point from the floor to
  // the roof, cf E+ #7383
  // compute the shortest distance from the point to the line first to avoid false positive
  const double distance = getDistancePointToLineSegment(test, {start, end});
  if (distance < tol) {  // getDistancePointToLineSegment always positive, it's calculated as norml_L2
    return (std::abs((getDistance(start, end) - (getDistance(start, test) + getDistance(test, end)))) < tol);
  }
  return false;
}

}  // namespace openstudio
