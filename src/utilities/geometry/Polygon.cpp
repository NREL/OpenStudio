// Note to self - would make things like ToBoostPolygon() and RemoveSpikes() class members
// but if they are deifned int he headers then we will 'leak the boost stuff so we don't 
// want to do that. Maybe thats why there is s Space and a SpaceImpl class. So anyway thats
// why they are just static methods for now
//
// Also a lot of code has been copied from intersect - the idea is that this would replace intersect
// all the 2D boolean calls would be member functions of this class. Woudl be cool if we could 
// overload operators too, as in polygonC = polygonA + polygonB is the same as 
// polygonC =  union(polygonA, polygonB), same with polygonA - polygonB. Bit what about intersect?

#include "Polygon.h"
#include "Point3d.hpp"

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
  static BoostPolygon ToBoostPolygon(const Polygon& polygon) {
    // Create a polygon, size the interior rings to the number of holes
    BoostPolygon boostPolygon;
    boost::geometry::interior_rings(boostPolygon).resize(polygon.holes.size());

    // Define the outer ring
    for (auto point : polygon.points) {
      boost::geometry::append(boostPolygon, boost::make_tuple(point.x(), point.y()));
    }
    boost::geometry::append(boostPolygon, boost::make_tuple(polygon.points[0].x(), polygon.points[0].y()));

    // And the inner rings
    int i = 0;
    for (auto hole : polygon.holes) {

      for (auto point : hole.points) {
        boost::geometry::append(boostPolygon.inners()[i], boost::make_tuple(point.x(), point.y()));
      }
      boost::geometry::append(boostPolygon.inners()[i], boost::make_tuple(hole.points[0].x(), hole.points[0].y()));

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

    for (auto i = 0; i < outer.size(); i++) {
      Point3d point(outer[i].x(), outer[i].y(), 0.0);
      result.points.push_back(point);
    }

    for (auto inner : polygon.inners()) {
      Polygon hole;
      result.holes.push_back(hole);
      for (auto i = 0; i < inner.size(); i++) {
        Point3d point(inner[i].x(), inner[i].y(), 0.0);
        hole.points.push_back(point);
      }
    }
    return result;
  }
};

class Cleaner
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

// Not sure what this is returning yet - something similar to
boost::optional<IntersectionResultEx> openstudio::Polygon::intersect(const Polygon& polygon, double tol) {
  IntersectionResultEx solution;

  // Convert our polygons to boost polygons
  BoostPolygon thisPolygon = Convert::ToBoostPolygon(*this);
  BoostPolygon otherPolygon = Convert::ToBoostPolygon(polygon);

  // Step 1 - Intersect the two polygons to get the common area
  std::vector<BoostPolygon> result;
  try {
    boost::geometry::intersection(thisPolygon, otherPolygon, result);
  } catch (boost::geometry::overlay_invalid_input_exception&) {
    return boost::none;
  }

  // May not intersect
  if (result.empty()) return boost::none;

  // Do sone cleanup
  result = Cleaner::removeSpikes(result);

  // If we have multiple intersections we want the primary intersection to be the largest so re-order the
  // boost polygons
  std::sort(result.begin(), result.end(), BoostPolygonAreaGreater());

  // production code converts back to vertex lists and checks for self-intersections, the presence of holes etc
  // but we won't worry about that right now as this is just proof of concept

  solution.polygon1() = Convert::FromBoostPolygon(result[0]);
  solution.polygon2() = Convert::FromBoostPolygon(result[0]);

  for (unsigned i = 1; i < result.size(); i++) {
    solution.polygons1().push_back(Convert::FromBoostPolygon(result[i]));
    solution.polygons2().push_back(Convert::FromBoostPolygon(result[i]));
  }

  // Step 2 - subtract polygon 2 from polygon 1. Result is the area(s) for polygon 1 that
  // excludes the common areas between polygon 1 and polygon 2
  std::vector<BoostPolygon> differenceResult1;
  boost::geometry::difference(thisPolygon, otherPolygon, differenceResult1);
  result = Cleaner::removeSpikesEx(result);

  for (auto boostPolygon : differenceResult1) {
    Polygon newPolygon = Convert::FromBoostPolygon(boostPolygon);

    // checks self intersection etc
    solution.polygons1().push_back(newPolygon);
  }

  // Step 2 - subtract polygon 1 from polygon 2. Result is the area(s) for polygon 2 that
  // excludes the common areas between polygon 1 and polygon 2
  std::vector<BoostPolygon> differenceResult2;
  boost::geometry::difference(thisPolygon, otherPolygon, differenceResult2);
  result = Cleaner::removeSpikesEx(result);

  for (auto boostPolygon : differenceResult2) {
    Polygon newPolygon = Convert::FromBoostPolygon(boostPolygon);

    // checks self intersection etc
    solution.polygons2().push_back(newPolygon);
  } 

  return solution;
}
}

