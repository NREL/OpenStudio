// Note to self - would make things like ToBoostPolygon() and RemoveSpikes() class members
// but if they are deifned int he headers then we will 'leak the boost stuff so we don't 
// want to do that. Maybe thats why there is s Space and a SpaceImpl class. So anyway thats
// why they are just static methods for now
//
// Also a lot of code has been copied from intersect - the idea is that this would replace intersect
// all the 2D boolean calls would be member functions of this class. Woudl be cool if we could 
// overload operators too, as in polygonC = polygonA + polygonB is the same as 
// polygonC =  union(polygonA, polygonB), same with polygonA - polygonB. Bit what about intersect?


#include "Point3d.hpp"
#include "Transformation.hpp"
#include "Polygon.hpp"

#undef BOOST_UBLAS_TYPE_CHECK
#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4244)
#endif

#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
#include <boost/geometry/strategies/cartesian/point_in_poly_franklin.hpp>
#include <boost/geometry/strategies/cartesian/point_in_poly_crossings_multiply.hpp>
#include <boost/geometry/algorithms/within.hpp>
#include <boost/geometry/algorithms/simplify.hpp>
#include <boost/geometry/algorithms/buffer.hpp>


#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

typedef double coordinate_type;
typedef boost::geometry::model::d2::point_xy<coordinate_type> BoostPoint;
typedef boost::geometry::model::polygon<BoostPoint> BoostPolygon;
typedef boost::geometry::model::ring<BoostPoint> BoostRing;
typedef boost::geometry::model::multi_polygon<BoostPolygon> BoostMultiPolygon;

namespace openstudio {

class Convert
{
  public:
  /// <summary>
  /// COnverts a polygon to a bpoost polygon with an applied transdform (transform can be model -> face transform)
  /// </summary>
  /// <param name="polygon"></param>
  /// <param name="transform"></param>
  /// <returns></returns>
  static BoostPolygon ToBoostPolygon(const Polygon& polygon) {
    // Create a polygon, size the interior rings to the number of holes
    BoostPolygon boostPolygon;
    boost::geometry::interior_rings(boostPolygon).resize(polygon.holes.size());

    // Define the outer ring
    for (auto point : polygon.getPoints()) {
      boost::geometry::append(boostPolygon, boost::make_tuple(point.x(), point.y()));
    }
    boost::geometry::append(boostPolygon, boost::make_tuple(polygon.getPoints()[0].x(), polygon.getPoints()[0].y()));

    // And the inner rings
    int i = 0;
    for (auto hole : polygon.holes) {

      for (auto point : hole.getPoints()) {
        boost::geometry::append(boostPolygon.inners()[i], boost::make_tuple(point.x(), point.y()));
      }
      boost::geometry::append(boostPolygon.inners()[i], boost::make_tuple(hole.getPoints()[0].x(), hole.getPoints()[0].y()));

      i++;
    }

    boost::geometry::correct(boostPolygon);
    return boostPolygon;
  }

  static Polygon FromBoostPolygon(BoostPolygon polygon) {
    Polygon result;
    BoostRing outer = polygon.outer();
    if (outer.empty()) {
      return result;
    }

    std::vector<Point3d> ring1;
    for (auto i = 0; i < outer.size() - 1; i++) {
      Point3d point(outer[i].x(), outer[i].y(), 0.0);
      ring1.push_back(point);
    }

    //result.setPoints(ring1);
    std::vector<Polygon> holes;
    std::vector<Polygon*> pholes;
    for (auto inner : polygon.inners()) {
      Polygon* hole = new Polygon();
      holes.push_back(*hole);
      pholes.push_back(hole);
      hole->setName("Ahole");
      //result.holes.push_back(hole);
      std::vector<Point3d> ring;
      for (auto i = 0; i < inner.size() - 1; i++) {
        Point3d point(inner[i].x(), inner[i].y(), 0.0);
        ring.push_back(point);
      }
      hole->setPoints(ring);
    }
    return Polygon(ring1, holes);
    }
};

class Clean
{
  public:
  static BoostPolygon removeSpikes(const BoostPolygon& polygon) {
    BoostPolygon temp(polygon);
    boost::geometry::remove_spikes(temp);
    return temp;
  }

  static std::vector<BoostPolygon> removeSpikes(const std::vector<BoostPolygon>& polygons) {
    std::vector<BoostPolygon> result;
    for (const BoostPolygon& polygon : polygons) {
      result.push_back(removeSpikes(polygon));
    }
    return result;
  }

  static BoostPolygon removeSpikesEx(const BoostPolygon& polygon) {

    const double buffer_distance = 1.0;
    const int points_per_circle = 36;
    const double amount = 0.005;

    boost::geometry::strategy::buffer::distance_symmetric<coordinate_type> expand(amount);
    boost::geometry::strategy::buffer::distance_symmetric<coordinate_type> shrink(-amount);
    boost::geometry::strategy::buffer::join_miter join_strategy;
    boost::geometry::strategy::buffer::end_flat end_strategy;
    boost::geometry::strategy::buffer::side_straight side_strategy;
    boost::geometry::strategy::buffer::point_circle point_strategy;

    BoostMultiPolygon resultExpand;
    BoostMultiPolygon resultShrink;
    BoostMultiPolygon result;
    boost::geometry::buffer(polygon, resultShrink, shrink, side_strategy, join_strategy, end_strategy, point_strategy);
    boost::geometry::buffer(resultShrink, resultExpand, expand, side_strategy, join_strategy, end_strategy, point_strategy);
    boost::geometry::simplify(resultExpand, result, amount);

    if (result.size() == 0)
      return polygon;
    else
      return result[0];
  }

  static std::vector<BoostPolygon> removeSpikesEx(const std::vector<BoostPolygon>& polygons) {
    std::vector<BoostPolygon> result;
    for (const BoostPolygon& polygon : polygons) {
      result.push_back(removeSpikesEx(polygon));
    }
    return result;
  }
};

// struct used to sort polygons in descending order by area
struct BoostPolygonAreaGreater
{
  bool operator()(const BoostPolygon& left, const BoostPolygon& right) const {
    boost::optional<double> leftA = boost::geometry::area(left);
    if (!leftA) {
      leftA = 0;
    }
    boost::optional<double> rightA = boost::geometry::area(right);
    if (!rightA) {
      rightA = 0;
    }
    return (*leftA > *rightA);
  }
};

 Polygon::Polygon(std::vector<Point3d> vertices) {
  points = vertices;
}
Polygon::Polygon(std::vector<Point3d> vertices, std::vector<Polygon> aholes) {
  points = vertices;
  holes = aholes;
}

//Polygon::Polygon(const Polygon& other) {
//  points = other.points;
//  holes = other.holes;
//}

void Polygon::setName(std::string value) {
  name = value;
}

std::string Polygon::getName() {
  return name;
}

void Polygon::setHandle(std::string value) {
  handle = value;
}

std::string Polygon::getHandle() {
  return handle;
}

void Polygon::setReference(void* value) {
  reference = value;
}

void* Polygon::getReference() {
  return reference;
}

void Polygon::reverse() {
  std::reverse(points.begin(), points.end());
}

  // Not sure what this is returning yet - something similar to
boost::optional<IntersectionResultEx> Polygon::intersect(const Polygon& polygon, double tol) {

  IntersectionResultEx solution;

  // goes from face coordinates of building vertices to building coordinates
  Transformation faceTransform;
  Transformation faceTransformInverse;
  faceTransform = Transformation::alignFace(points);
  faceTransformInverse = faceTransform.inverse();

  // Transform polygons to face coordinates
  Polygon facePolygon = faceTransformInverse * *this;
  Polygon otherFacePolygon = faceTransformInverse * polygon;

  // boost polygon wants vertices in clockwise order, faceVertices must be reversed, otherFaceVertices already CCW
  // not sure if holes are meant to be CW...?
  facePolygon.reverse();
  
  // Convert our polygons to boost polygons convertiung to face coordinates
  BoostPolygon thisBoostPolygon = Convert::ToBoostPolygon(facePolygon);
  BoostPolygon otherBoostPolygon = Convert::ToBoostPolygon(otherFacePolygon);

  // Step 1 - Intersect the two polygons to get the common area
  std::vector<BoostPolygon> result;
  try {
    boost::geometry::intersection(thisBoostPolygon, otherBoostPolygon, result);
  } catch (boost::geometry::overlay_invalid_input_exception&) {
    return boost::none;
  }

  // May not intersect
  if (result.empty()) return boost::none;

  // Do sone cleanup
  result = Clean::removeSpikes(result);

  // If we have multiple intersections we want the primary intersection to be the largest so re-order the
  // boost polygons
  std::sort(result.begin(), result.end(), BoostPolygonAreaGreater());

  Polygon p1 = faceTransform * Convert::FromBoostPolygon(result[0]);
  Polygon p2 = faceTransform * Convert::FromBoostPolygon(result[0]);
  solution.setMatchedPolygon1(p1);
  solution.setMatchedPolygon2(p2);

  // production code converts back to vertex lists and checks for self-intersections, the presence of holes etc
  // but we won't worry about that right now as this is just proof of concept
  // These are all the matched polygons., i.e. common areas
  for (unsigned i = 1; i < result.size(); i++) {
    Polygon p1 = faceTransform * Convert::FromBoostPolygon(result[i]);
    Polygon p2 = faceTransform * Convert::FromBoostPolygon(result[i]);
    solution.polygons1().push_back(p1);
    solution.polygons2().push_back(p2);
    p1.setAdjacentPolygon(&p2);
    p2.setAdjacentPolygon(&p1);
  }

  // Step 2 - subtract polygon 2 from polygon 1. Result is the area(s) for polygon 1 that
  // excludes the common areas between polygon 1 and polygon 2
  std::vector<BoostPolygon> differenceResult1;
  boost::geometry::difference(thisBoostPolygon, otherBoostPolygon, differenceResult1);
  result = Clean::removeSpikesEx(result);

  for (auto boostPolygon : differenceResult1) {
    Polygon newPolygon = faceTransform * Convert::FromBoostPolygon(boostPolygon);

    // checks self intersection etc
    solution.polygons1().push_back(newPolygon);
  }

  // Step 2 - subtract polygon 1 from polygon 2. Result is the area(s) for polygon 2 that
  // excludes the common areas between polygon 1 and polygon 2
  std::vector<BoostPolygon> differenceResult2;
  boost::geometry::difference(otherBoostPolygon, thisBoostPolygon, differenceResult2);
  result = Clean::removeSpikesEx(result);

  for (auto boostPolygon : differenceResult2) {
    Polygon newPolygon = faceTransform * Convert::FromBoostPolygon(boostPolygon);

    // checks self intersection etc
    solution.polygons2().push_back(newPolygon);
  }

  return solution;
}

boost::optional<IntersectionResultEx> Polygon::computeIntersection(Polygon& polygon) {

  // plane coordinates are in model coordinates
  Plane plane = getPlane();
  Plane otherPlane = polygon.getPlane();

  if (!plane.reverseEqual(otherPlane)) {
    //LOG(Info, "Planes are not reverse equal, intersection of '" << this->name().get() << "' with '" << otherSurface.name().get() << "' fails");
    return boost::none;
  }

  // Intersect the two polygons
  boost::optional<IntersectionResultEx> result = intersect(polygon, 0.01);

  return result;
}
}

