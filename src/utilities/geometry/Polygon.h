#pragma once

#include <vector>
#include "Point3d.hpp"

namespace openstudio {

    class IntersectionResultEx;
/// <summary>
/// Polygon class is a list of vertices defining an outer ring of points
/// and a number of inner paths (holes)
/// </summary>
class Polygon
{
  public:
  std::vector<Point3d> points;
  std::vector<Polygon> holes;

  boost::optional<IntersectionResultEx> intersect(const Polygon& polygon, double tol);
};

class IntersectionResultEx
{
  public:
  Polygon polygon1() {
    return mPolygon1;
  }
  Polygon polygon2() {
    return mPolygon2;
  }

  std::vector<Polygon> polygons1() {
    return mPolygons1;
  }

  std::vector<Polygon> polygons2() {
    return mPolygons2;
  }

  private:
  Polygon mPolygon1;
  Polygon mPolygon2;
  std::vector<Polygon> mPolygons1;
  std::vector<Polygon> mPolygons2;
};

}  // namespace openstudio
