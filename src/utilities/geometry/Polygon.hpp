#ifndef UTILITIES_GEOMETRY_POLYGON_HPP
#define UTILITIES_GEOMETRY_POLYGON_HPP

#include "../UtilitiesAPI.hpp"
#include <vector>
#include "Point3d.hpp"
#include "Plane.hpp"

namespace openstudio {

    class IntersectionResultEx;
/// <summary>
/// Polygon class is a list of vertices defining an outer ring of points
/// and a number of inner paths (holes). Vertices are in model coordinates
/// </summary>
    class UTILITIES_API Polygon
    {
  public:
      Polygon(std::vector<Point3d> vertices) {
        points = vertices;
      }
      Polygon(std::vector<Point3d> vertices, std::vector<Polygon>aholes) {
        points = vertices;
        holes = aholes;
      }
      Polygon() : reference(nullptr) {}

      std::vector<Polygon> holes;

      boost::optional<IntersectionResultEx> computeIntersection(Polygon& polygon);
      void reverse();

      void setPoints(std::vector<Point3d> value) {
        points = value;
      }
      std::vector<Point3d> getPoints() const {
        return points;
      }

      void setName(std::string value);
      std::string getName();
      void setHandle(std::string value);
      std::string getHandle();
      void setReference(void* value);
      void* getReference();

      Plane getPlane() {
        if (!cachedPlane) {
          cachedPlane = Plane(points);
        }
        return cachedPlane.get();
      }
      Polygon* getAdjacentPolygon() {
        return adjacentPolygon;
      }
      void setAdjacentPolygon(Polygon* value) {
        adjacentPolygon = value;
      }
  private:
      boost::optional<IntersectionResultEx> intersect(const Polygon& polygon, double tol);
      std::vector<Point3d> points;
      std::string name;
      std::string handle;
      void* reference;
      mutable boost::optional<Plane> cachedPlane;
      Polygon* adjacentPolygon;
    };

    // holds the results of intersecting two polygons. 
    // matcheedPolygons contains the common areas between the two polygons
    // polygons contains the areas of the original polygon that does not overlap the other polygon
class IntersectionResultEx
{
  public:

  Polygon getMatchedPolygon1() {
    return matchedPolygon1;
  }
  void setMatchedPolygon1(Polygon& value) {
    matchedPolygon1 = value;
  }
  Polygon getMatchedPolygon2() {
    return matchedPolygon2;
  }
  void setMatchedPolygon2(Polygon& value) {
    matchedPolygon2 = value;
  }
  std::vector<Polygon>& polygons1() {
    return mPolygons1;
  }
  std::vector<Polygon>& polygons2() {
    return mPolygons2;
  }

  private:
  Polygon matchedPolygon1;
      Polygon matchedPolygon2;

    /// <summary>
    /// List of polygons defining the areas of polygon 1 that don't overlap with polygon 2
    /// </summary>
    std::vector<Polygon> mPolygons1;
    /// <summary>
    /// List of polygons defining the areas of polygon 2 that don't overlap with polygon 1
    /// </summary>
    std::vector<Polygon> mPolygons2;

};

  typedef std::vector<Polygon> PolygonVector;


}  // namespace openstudio

#endif

