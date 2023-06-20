/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_GEOMETRY_POLYGON_HPP
#define UTILITIES_GEOMETRY_POLYGON_HPP

#include "../UtilitiesAPI.hpp"

#include "Point3d.hpp"
#include <vector>

namespace openstudio {

class Vector3d;

class UTILITIES_API Polygon3d
{
 public:
  // Constructs an empty polygon
  Polygon3d();

  // Construct a polygon with an outer path
  Polygon3d(const Point3dVector& outerPath);

  // Constructs a polygon with an outer path and one or more inner paths
  Polygon3d(const Point3dVector& outerPath, const Point3dVectorVector& innerPaths);

  // Assigns an outer path for the polygon
  void setOuterPath(const Point3dVector& outerPath);

  // Returns the polygon's outer path
  Point3dVector getOuterPath() const;

  // Returns the polygon's inner paths
  Point3dVectorVector getInnerPaths() const;

  // Adds a point to the outer path of the polygon
  void addPoint(const Point3d& point);

  // Adds an inner path to the polygon
  void addHole(const Point3dVector& hole);

  // Calculates the Newell Vector for the polygon
  Vector3d newellVector() const;

  // Calculates the outward normal of the polygon
  Vector3d outwardNormal() const;

  // Calculates the gross area of the polygon (area excluding holes)
  double grossArea() const;

  // Calculates the net area of the polygon (gross area - area of holes)
  double netArea() const;

  // Gets the perimeter of the outer path of the polygon
  double perimeter() const;

  // Determines whether the polygon os clockwise (normal down) or anti-clockwise (normal up)
  bool isClockwise() const;

  // Calculates the centroid of the polygon
  Point3d centroid() const;

  // Point is on the outer path or one of the inner paths
  bool pointInPolygon(const Point3d& point, double tol = 0.01) const;

  // Point inside outer path and not inside a hole
  bool within(const Point3d& point, double tol = 0.01) const;

  // Point is inside the outer path and not inside a hole or on the outer path or one of the inner paths
  bool inside(const Point3d& point, double tol = 0.01) const;

  // gets the size of the outer path
  size_t getSize() const;

  // gets an edge on thge outer path
  Point3dVector getEdge(size_t i) const;

  // Returns the ovelapping segments between this polygon and the line
  std::vector<Point3dVector> overlap(const Point3dVector& line) const;

 private:
  // The polygon's outer path
  Point3dVector m_outerPath;

  // The polygons inner paths
  Point3dVectorVector m_innerPaths;
};
}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_POLYGON_HPP
