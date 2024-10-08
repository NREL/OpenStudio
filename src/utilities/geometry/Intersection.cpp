/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Geometry.hpp"
#include "Vector3d.hpp"
#include "Intersection.hpp"
#include "../data/Matrix.hpp"
#include "../core/Assert.hpp"
#include "../core/ContainersMove.hpp"
#include "../core/Logger.hpp"

#undef BOOST_UBLAS_TYPE_CHECK
#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4244)
#endif
#include <boost/geometry.hpp>
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
#include <boost/geometry/policies/robustness/get_rescale_policy.hpp>
#include <boost/geometry/policies/robustness/robust_point_type.hpp>
#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#include <algorithm>
#include <cmath>
#include <iterator>

using coordinate_type = double;
using BoostPoint = boost::geometry::model::d2::point_xy<double>;
using BoostPolygon = boost::geometry::model::polygon<BoostPoint>;
using BoostRing = boost::geometry::model::ring<BoostPoint>;
using BoostMultiPolygon = boost::geometry::model::multi_polygon<BoostPolygon>;

#include <polypartition/polypartition.h>

namespace openstudio {

// Private implementation functions

// Boost 1.73.0 has this bit in has_self_intersections.hpp:
//
// For backward compatibility
// template <typename Geometry>
// inline bool has_self_intersections(Geometry const& geometry, bool throw_on_self_intersection = true) {
//   typedef typename geometry::point_type<Geometry>::type point_type;
//   typedef typename geometry::rescale_policy_type<point_type>::type rescale_policy_type;

//   typename strategy::intersection::services::default_strategy<typename cs_tag<Geometry>::type>::type strategy;

//   rescale_policy_type robust_policy = geometry::get_rescale_policy<rescale_policy_type>(geometry, strategy);

//   return has_self_intersections(geometry, strategy, robust_policy, throw_on_self_intersection);
// }

// For backward compatibility
template <typename Geometry>
inline bool has_self_intersections(Geometry const& geometry, bool throw_on_self_intersection = true) {
  using point_type = typename boost::geometry::point_type<Geometry>::type;
  using rescale_policy_type = typename boost::geometry::rescale_policy_type<point_type>::type;

  typename boost::geometry::strategies::relate::services::default_strategy<Geometry, Geometry>::type const strategy;

  // typename boost::geometry::strategy::intersection::services::default_strategy<typename boost::geometry::cs_tag<Geometry>::type>::type strategy;

  auto robust_policy = boost::geometry::get_rescale_policy<rescale_policy_type>(geometry, strategy);

  return boost::geometry::detail::overlay::has_self_intersections(geometry, strategy, robust_policy, throw_on_self_intersection);
}

// Some operators for Boost data types that are useful for extra logging
std::ostream& operator<<(std::ostream& os, const BoostRing& boostRing) {
  os << "[";
  for (unsigned int i = 0; i < boostRing.size(); i++) {
    os << "{";
    os << R"("X":)" << boostRing[i].x() << ",";
    os << R"("Y":)" << boostRing[i].y() << ",";
    os << R"("Z":0.0},)";
  }
  os << "],";
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<BoostRing>& boostRings) {
  os << "[";
  for (const auto& ring : boostRings) {
    os << ring;
  }
  os << "]";
  return os;
}

std::ostream& operator<<(std::ostream& os, const BoostPolygon& boostPolygon) {
  os << "{";
  os << R"("Color":"Black",)";
  os << R"("OuterPath":)" << boostPolygon.outer();
  os << R"("InnerPaths":)" << boostPolygon.inners();
  os << "},";
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<BoostPolygon>& boostPolygons) {
  os << "[";
  for (const auto& poly : boostPolygons) {
    os << poly;
  }
  os << "]";
  return os;
}

std::ostream& operator<<(std::ostream& os, const BoostMultiPolygon& boostPolygons) {
  os << "[";
  for (const auto& poly : boostPolygons) {
    os << poly;
  }
  os << "]";
  return os;
}

// Scale factor for parameters when converting between openstudio and boost data formats to improve the numerical accuracy of the boolean operations.
// Idea came from this comment in the boostorg/geometry repo: https://github.com/boostorg/geometry/issues/1034#issuecomment-1284180101
// where the author indicates that scaling the values by 10 improved the result and talks about rounding to an integer grid,
// so by increasing the range of values we pass to boost we should be improving the resolution of the integer rounding
static constexpr double scaleBy = 1000.0;

// Cleans a polygon by shrinking and expanding. Can return multiple polygons
std::vector<BoostPolygon> removeSpikesEx(const BoostPolygon& polygon) {

  //const double buffer_distance = 1.0;
  //const int points_per_circle = 36;
  constexpr bool extraLogging = false;
  // The amount to shrink and expand the polygons by
  constexpr double offsetBy = 0.01;
  constexpr double tol = offsetBy;
  // Sets the limit to how far miters are extended for sharp corners
  constexpr double mitreLimit = 100;
  const boost::geometry::strategy::buffer::distance_symmetric<coordinate_type> expand(offsetBy * scaleBy);
  const boost::geometry::strategy::buffer::distance_symmetric<coordinate_type> shrink(-offsetBy * scaleBy);
  const boost::geometry::strategy::buffer::join_miter join_strategy(mitreLimit * scaleBy);
  const boost::geometry::strategy::buffer::end_flat end_strategy;
  const boost::geometry::strategy::buffer::side_straight side_strategy;
  const boost::geometry::strategy::buffer::point_circle point_strategy;

  BoostMultiPolygon resultExpand;
  BoostMultiPolygon resultShrink;
  BoostMultiPolygon result;

  // Used for extra logging
  BoostMultiPolygon shrinkExpand;
  // cppcheck-suppress knownConditionTrueFalse
  if constexpr (extraLogging) {
    shrinkExpand.push_back(polygon);
  }

  // Shrink the polygon
  boost::geometry::buffer(polygon, resultShrink, shrink, side_strategy, join_strategy, end_strategy, point_strategy);
  // cppcheck-suppress knownConditionTrueFalse
  if constexpr (extraLogging) {
    shrinkExpand.push_back(resultShrink[0]);
  }

  // Inflate the polygon
  boost::geometry::buffer(resultShrink, resultExpand, expand, side_strategy, join_strategy, end_strategy, point_strategy);
  // cppcheck-suppress knownConditionTrueFalse
  if constexpr (extraLogging) {
    shrinkExpand.push_back(resultExpand[0]);
  }

  // Very small tolerance to remove artifacts from the inflate
  constexpr double tol1 = 0.001;
  boost::geometry::simplify(resultExpand, result, tol1 * scaleBy);
  // cppcheck-suppress knownConditionTrueFalse
  if constexpr (extraLogging) {
    shrinkExpand.push_back(result[0]);
    LOG_FREE(Debug, "Shrink/Expand", shrinkExpand);
  }

  std::vector<BoostPolygon> solution;
  if (result.empty()) {
    // There's no result because the polygon was completely removed so return nothing
    return solution;
  } else if (result.size() == 1 && result[0].outer().size() == polygon.outer().size()) {
    // Number of vertices didn't change so no spikes were removed
    solution.push_back(polygon);
    return solution;
  } else {
    for (auto& boostPolygon : result) {

      // The returned points are adjusted to the input polygon (which defines the canonical set)
      for (auto& cleanedRing : boostPolygon.outer()) {
        const Point3d point3d(cleanedRing.x(), cleanedRing.y(), 0.0);

        // Outer ring of the original polygon
        for (const auto& oriRing : polygon.outer()) {
          const Point3d p1(oriRing.x(), oriRing.y(), 0);
          // Two points are within tolerance set the result to the original input point
          if (getDistance(point3d, p1) <= tol * scaleBy) {
            cleanedRing.x(oriRing.x());
            cleanedRing.y(oriRing.y());
            break;
          }
        }
      }

      solution.push_back(boostPolygon);
    }
    return solution;
  }
}

BoostPolygon removeSpikes(const BoostPolygon& polygon) {
  BoostPolygon temp(polygon);
  boost::geometry::remove_spikes(temp);
  return temp;
}

std::vector<BoostPolygon> removeSpikes(const std::vector<BoostPolygon>& polygons) {
  std::vector<BoostPolygon> result;
  for (const BoostPolygon& polygon : polygons) {
    openstudio::detail::concat_helper(result, removeSpikesEx(polygon));
  }
  return result;
}

std::vector<BoostPolygon> removeHoles(const BoostPolygon& boostPolygon) {
  std::vector<BoostPolygon> result;

  // convert to vector of TPPLPoly
  std::list<TPPLPoly> polys;

  BoostRing outer = boostPolygon.outer();
  TPPLPoly outerPoly;  // must be counter-clockwise
  outerPoly.Init(outer.size() - 1);
  outerPoly.SetHole(false);
  //std::cout << "outer :";
  // Note - no scaling conversion here as this accepts boost data and returns boost data
  for (size_t i = 0; i < outer.size() - 1; ++i) {
    outerPoly[i].x = outer[i].x();
    outerPoly[i].y = outer[i].y();
    //std::cout << "(" << outer[i].x() << ", " << outer[i].y() << ") ";
  }
  //std::cout << '\n';
  outerPoly.SetOrientation(TPPL_CCW);
  polys.push_back(outerPoly);

  const std::vector<BoostRing>& inners = boostPolygon.inners();
  for (const BoostRing& inner : inners) {
    if (inner.size() < 3) {
      continue;
    }
    TPPLPoly innerPoly;  // must be clockwise
    innerPoly.Init(inner.size() - 1);
    innerPoly.SetHole(true);
    //std::cout << "inner :";
    // Note - no scaling conversion here as this accepts boost data and returns boost data
    for (unsigned i = 0; i < inner.size() - 1; ++i) {
      innerPoly[i].x = inner[i].x();
      innerPoly[i].y = inner[i].y();
      //std::cout << "(" << inner[i].x() << ", " << inner[i].y() << ") ";
    }
    //std::cout << '\n';
    innerPoly.SetOrientation(TPPL_CW);
    polys.push_back(innerPoly);
  }

  // do partitioning
  TPPLPartition pp;
  std::list<TPPLPoly> resultPolys;
  const int test = pp.ConvexPartition_HM(&polys, &resultPolys);
  if (test == 0) {
    LOG_FREE(Error, "utilities.geometry.removeHoles", "Failed to partition polygon");
    return result;
  }

  // convert back to BoostPolygon
  std::list<TPPLPoly>::iterator it;
  std::list<TPPLPoly>::iterator itend;
  for (it = resultPolys.begin(), itend = resultPolys.end(); it != itend; ++it) {
    BoostPolygon newBoostPolygon;
    //std::cout << "result :";
    for (long i = 0; i < it->GetNumPoints(); ++i) {
      const TPPLPoint& point = it->GetPoint(i);
      boost::geometry::append(newBoostPolygon, boost::make_tuple(point.x, point.y));
      //std::cout << "(" << point.x << ", " << point.y << ") ";
    }
    const TPPLPoint& point = it->GetPoint(0);
    boost::geometry::append(newBoostPolygon, boost::make_tuple(point.x, point.y));
    //std::cout << "(" << point.x << ", " << point.y << ") ";
    //std::cout << '\n';

    boost::geometry::correct(newBoostPolygon);
    result.push_back(newBoostPolygon);
  }

  return result;
}

std::vector<BoostPolygon> removeHoles(const std::vector<BoostPolygon>& polygons) {
  std::vector<BoostPolygon> result;
  for (const BoostPolygon& polygon : polygons) {
    if (polygon.inners().empty()) {
      // DLM: might also want to partition if this polygon is self intersecting?
      result.push_back(polygon);
    } else {
      std::vector<BoostPolygon> temp = removeHoles(polygon);
      result.insert(result.end(), temp.begin(), temp.end());
    }
  }
  return result;
}

// convert a Point3d to a BoostPoint
boost::tuple<double, double> boostPointFromPoint3d(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol) {
  OS_ASSERT(std::abs(point3d.z()) <= tol);

  // simple method
  //return boost::make_tuple(point3d.x(), point3d.y());

  // detailed method, try to combine points within tolerance
  const Point3d resultPoint = getCombinedPoint(point3d, allPoints, tol);

  return boost::make_tuple(resultPoint.x() * scaleBy, resultPoint.y() * scaleBy);
}

// convert vertices to a boost polygon, all vertices must lie on z = 0 plane
boost::optional<BoostPolygon> boostPolygonFromVertices(const std::vector<Point3d>& vertices, std::vector<Point3d>& allPoints, double tol) {
  if (vertices.size() < 3) {
    return boost::none;
  }

  BoostPolygon polygon;
  for (const Point3d& vertex : vertices) {

    // should all have zero z coordinate now
    const double z = vertex.z();
    if (std::abs(z) > tol) {
      LOG_FREE(Error, "utilities.geometry.boostPolygonFromVertices", "All points must be on z = 0 plane");
      return boost::none;
    }

    // use helper method which combines close points
    boost::geometry::append(polygon, boostPointFromPoint3d(vertex, allPoints, tol));
  }

  // close polygon, use helper method which combines close points
  boost::geometry::append(polygon, boostPointFromPoint3d(vertices[0], allPoints, tol));

  //boost::geometry::correct(polygon);

  boost::optional<double> testArea = boost::geometry::area(polygon);
  if (!testArea || (*testArea < 0)) {
    // DLM: we could offer to reverse these vertices here but that might not be the best idea
    return boost::none;
  }

  return polygon;
}

boost::optional<BoostPolygon> nonIntersectingBoostPolygonFromVertices(const std::vector<Point3d>& polygon, std::vector<Point3d>& allPoints,
                                                                      double tol) {
  boost::optional<BoostPolygon> result = boostPolygonFromVertices(polygon, allPoints, tol);
  if (!result) {
    return boost::none;
  }
  // check if polygon overlaps itself
  try {
    has_self_intersections(*result);
  } catch (const boost::geometry::overlay_invalid_input_exception&) {
    //LOG_FREE(Error, "utilities.geometry.nonIntersectingBoostPolygonFromVertices", "Self intersecting polygon");
    return boost::none;
  }
  return result;
}

// convert vertices to a boost ring, all vertices must lie on z = 0 plane
boost::optional<BoostRing> boostRingFromVertices(const std::vector<Point3d>& vertices, std::vector<Point3d>& allPoints, double tol) {
  if (vertices.size() < 3) {
    return boost::none;
  }

  BoostRing ring;
  for (const Point3d& vertex : vertices) {

    // should all have zero z coordinate now
    const double z = vertex.z();
    if (std::abs(z) > tol) {
      LOG_FREE(Error, "utilities.geometry.boostRingFromVertices", "All points must be on z = 0 plane");
      return boost::none;
    }

    // use helper method which combines close points
    boost::geometry::append(ring, boostPointFromPoint3d(vertex, allPoints, tol));
  }

  // close polygon, use helper method which combines close points
  boost::geometry::append(ring, boostPointFromPoint3d(vertices[0], allPoints, tol));

  //boost::geometry::correct(ring);

  boost::optional<double> testArea = boost::geometry::area(ring);
  if (!testArea || (*testArea < 0)) {
    // DLM: we could offer to reverse these vertices here but that might not be the best idea
    return boost::none;
  }

  return ring;
}

boost::optional<BoostRing> nonIntersectingBoostRingFromVertices(const std::vector<Point3d>& polygon, std::vector<Point3d>& allPoints, double tol) {
  boost::optional<BoostRing> result = boostRingFromVertices(polygon, allPoints, tol);
  if (!result) {
    return boost::none;
  }
  // check if polygon overlaps itself
  try {
    has_self_intersections(*result);
  } catch (const boost::geometry::overlay_invalid_input_exception&) {
    //LOG_FREE(Error, "utilities.geometry.nonIntersectingBoostRingFromVertices", "Self intersecting polygon");
    return boost::none;
  }
  return result;
}

// convert a boost polygon to vertices
std::vector<Point3d> verticesFromBoostPolygon(const BoostPolygon& polygon, std::vector<Point3d>& allPoints, double tol,
                                              bool removeCollinear = false) {
  std::vector<Point3d> result;
  BoostRing outer = polygon.outer();
  if (outer.empty()) {
    return result;
  }

  // add point for each vertex except final vertex
  for (unsigned i = 0; i < outer.size() - 1; ++i) {
    const Point3d point3d(outer[i].x() / scaleBy, outer[i].y() / scaleBy, 0.0);

    // try to combine points within tolerance
    Point3d resultPoint = getCombinedPoint(point3d, allPoints, tol);

    // don't keep repeated vertices
    if ((i > 0) && (result.back() == resultPoint)) {
      continue;
    }
    result.emplace_back(std::move(resultPoint));
  }

  if (!polygon.inners().empty()) {
    LOG_FREE(Warn, "utilities.geometry.verticesFromBoostPolygon",
             "Converting polygon with " << polygon.inners().size() << " inner loops to OpenStudio vertices, inner loops ignored");
  }

  if (removeCollinear) {
    result = removeCollinearLegacy(result);
  }

  // don't keep repeated vertices
  if (result.front() == result.back()) {
    result.pop_back();
  }

  if (result.size() < 3) {
    return {};
  }

  return result;
}

// convert a boost ring to vertices
std::vector<Point3d> verticesFromBoostRing(const BoostRing& ring, std::vector<Point3d>& allPoints, double tol) {
  std::vector<Point3d> result;

  // add point for each vertex except final vertex
  for (unsigned i = 0; i < ring.size() - 1; ++i) {
    const Point3d point3d(ring[i].x(), ring[i].y(), 0.0);

    // try to combine points within tolerance
    Point3d resultPoint = getCombinedPoint(point3d, allPoints, tol);

    // don't keep repeated vertices
    if ((i > 0) && (result.back() == resultPoint)) {
      continue;
    }
    result.emplace_back(std::move(resultPoint));
  }

  result = removeCollinearLegacy(result);

  // don't keep repeated vertices
  if (result.front() == result.back()) {
    result.pop_back();
  }

  if (result.size() < 3) {
    return {};
  }

  return result;
}

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

// Public functions

IntersectionResult::IntersectionResult(std::vector<Point3d> polygon1, std::vector<Point3d> polygon2, std::vector<std::vector<Point3d>> newPolygons1,
                                       std::vector<std::vector<Point3d>> newPolygons2)
  : m_polygon1(std::move(polygon1)),
    m_polygon2(std::move(polygon2)),
    m_newPolygons1(std::move(newPolygons1)),
    m_newPolygons2(std::move(newPolygons2)) {}

std::vector<Point3d> IntersectionResult::polygon1() const {
  return m_polygon1;
}

std::vector<Point3d> IntersectionResult::polygon2() const {
  return m_polygon2;
}

std::vector<std::vector<Point3d>> IntersectionResult::newPolygons1() const {
  return m_newPolygons1;
}

std::vector<std::vector<Point3d>> IntersectionResult::newPolygons2() const {
  return m_newPolygons2;
}

double IntersectionResult::area1() const {
  double result = 0;
  boost::optional<double> d;
  d = getArea(m_polygon1);
  if (d) {
    result += *d;
  } else {
    LOG_FREE(Warn, "utilities.geometry.IntersectionResult", "Cannot calculate area for polygon1");
  }

  for (const auto& polygon : m_newPolygons1) {
    d = getArea(polygon);
    if (d) {
      result += *d;
    } else {
      LOG_FREE(Warn, "utilities.geometry.IntersectionResult", "Cannot calculate area for polygon in polygons1");
    }
  }

  return result;
}

double IntersectionResult::area2() const {
  double result = 0;
  boost::optional<double> d;
  d = getArea(m_polygon2);
  if (d) {
    result += *d;
  } else {
    LOG_FREE(Warn, "utilities.geometry.IntersectionResult", "Cannot calculate area for polygon2");
  }

  for (const auto& polygon : m_newPolygons2) {
    d = getArea(polygon);
    if (d) {
      result += *d;
    } else {
      LOG_FREE(Warn, "utilities.geometry.IntersectionResult", "Cannot calculate area for polygon in polygons2");
    }
  }

  return result;
}

std::vector<Point3d> removeSpikes(const std::vector<Point3d>& polygon, double tol) {
  // convert vertices to boost rings
  std::vector<Point3d> allPoints;

  boost::optional<BoostPolygon> boostPolygon = boostPolygonFromVertices(polygon, allPoints, tol);
  if (!boostPolygon) {
    return {};
  }

  const BoostPolygon boostResult = removeSpikes(*boostPolygon);

  std::vector<Point3d> result = verticesFromBoostPolygon(boostResult, allPoints, tol);

  return result;
}

// cppcheck-suppress constParameterReference
bool polygonInPolygon(std::vector<Point3d>& points, const std::vector<Point3d>& polygon, double tol) {

  // convert vertices to boost rings
  std::vector<Point3d> allPoints;

  boost::optional<BoostRing> boostPolygon = nonIntersectingBoostRingFromVertices(polygon, allPoints, tol);
  if (!boostPolygon) {
    return false;
  }

  if (points.empty()) {
    return false;
  }

  for (const Point3d& point : points) {
    if (std::abs(point.z()) > tol) {
      return false;
    }
  }

  for (const Point3d& point : points) {
    boost::tuple<double, double> p = boostPointFromPoint3d(point, allPoints, tol);
    const BoostPoint boostPoint(p.get<0>(), p.get<1>());
    const double distance = boost::geometry::distance(boostPoint, *boostPolygon);
    if (distance >= 0.0001) {
      return false;
    }
  }
  return true;
}

bool pointInPolygon(const Point3d& point, const std::vector<Point3d>& polygon, double tol) {
  // convert vertices to boost rings
  std::vector<Point3d> allPoints;

  boost::optional<BoostRing> boostPolygon = nonIntersectingBoostRingFromVertices(polygon, allPoints, tol);
  if (!boostPolygon) {
    return false;
  }

  if (std::abs(point.z()) > tol) {
    return false;
  }

  boost::tuple<double, double> p = boostPointFromPoint3d(point, allPoints, tol);
  const BoostPoint boostPoint(p.get<0>(), p.get<1>());

  //boost::geometry::strategy::within::winding<BoostPoint> strategy;
  //boost::geometry::strategy::within::franklin<BoostPoint> strategy;
  //boost::geometry::strategy::within::crossings_multiply<BoostPoint> strategy;
  //bool result = boost::geometry::within(boostPoint, *boostPolygon, strategy);

  //bool result = boost::geometry::intersects(boostPoint, *boostPolygon);

  //bool result = boost::geometry::overlaps(boostPoint, *boostPolygon);

  const double distance = boost::geometry::distance(boostPoint, *boostPolygon);
  const bool result = (distance <= 0.0001);

  return result;
}

boost::optional<std::vector<Point3d>> join(const std::vector<Point3d>& polygon1, const std::vector<Point3d>& polygon2, double tol) {
  // convert vertices to boost rings
  std::vector<Point3d> allPoints;

  boost::optional<BoostRing> boostPolygon1 = nonIntersectingBoostRingFromVertices(polygon1, allPoints, tol);
  if (!boostPolygon1) {
    return boost::none;
  }

  boost::optional<BoostRing> boostPolygon2 = nonIntersectingBoostRingFromVertices(polygon2, allPoints, tol);
  if (!boostPolygon2) {
    return boost::none;
  }

  // union the points in face coordinates,
  std::vector<BoostPolygon> unionResult;
  try {
    boost::geometry::union_(*boostPolygon1, *boostPolygon2, unionResult);
  } catch (const boost::geometry::overlay_invalid_input_exception&) {
    LOG_FREE(Error, "utilities.geometry.join", "overlay_invalid_input_exception");
    return boost::none;
  }

  unionResult = removeSpikes(unionResult);

  // should not be any holes, check for that below

  // check that union is ok
  if (unionResult.empty() or (unionResult.size() > 1)) {
    return boost::none;
  } else if (!unionResult[0].inners().empty()) {
    // check for holes
    LOG_FREE(Error, "utilities.geometry.join", "Union has inner loops");
    return boost::none;
  };

  std::vector<Point3d> unionVertices = verticesFromBoostPolygon(unionResult[0], allPoints, tol);
  boost::optional<double> testArea = boost::geometry::area(unionResult[0]);
  if (!testArea || unionVertices.empty()) {
    LOG_FREE(Info, "utilities.geometry.join", "Cannot compute area of union");
    return boost::none;
  } else if (*testArea < tol * tol) {
    LOG_FREE(Info, "utilities.geometry.join", "Union has very small area of " << *testArea << " m^2");
    return boost::none;
  }

  try {
    has_self_intersections(unionResult[0]);
  } catch (const boost::geometry::overlay_invalid_input_exception&) {
    LOG_FREE(Error, "utilities.geometry.join", "Union is self intersecting");
    return boost::none;
  }

  unionVertices = reorderULC(unionVertices);
  unionVertices = removeCollinearLegacy(unionVertices);

  return unionVertices;
}

std::vector<std::vector<Point3d>> joinAll(const std::vector<std::vector<Point3d>>& polygons, double tol) {
  std::vector<std::vector<Point3d>> result;

  const size_t N = polygons.size();
  if (N <= 1) {
    return polygons;
  }

  std::vector<double> polygonAreas(N, 0.0);
  for (unsigned i = 0; i < N; ++i) {
    auto area = getArea(polygons[i]);
    if (area) {
      polygonAreas[i] = *area;
    }
  }

  // compute adjacency matrix
  Matrix A(N, N, 0.0);
  for (unsigned i = 0; i < N; ++i) {
    A(i, i) = 1.0;
    for (unsigned j = i + 1; j < N; ++j) {
      if (join(polygons[i], polygons[j], tol)) {
        A(i, j) = 1.0;
        A(j, i) = 1.0;
      }
    }
  }

  const std::vector<std::vector<unsigned>> connectedComponents = findConnectedComponents(A);
  for (const std::vector<unsigned>& component : connectedComponents) {
    std::vector<unsigned> orderedComponent(component);
    // #4831 - Use a stable_sort to produce consistent results between Windows and Unix in case you have polygons with the same area
    std::stable_sort(orderedComponent.begin(), orderedComponent.end(),
                     [&polygonAreas](int ia, int ib) { return polygonAreas[ia] > polygonAreas[ib]; });

    std::vector<Point3d> points;
    std::set<unsigned> joinedComponents;

    // try to join at most component.size() times
    for (unsigned n = 0; n < component.size(); ++n) {

      // loop over polygons to join in order
      for (const unsigned i : orderedComponent) {
        if (points.empty()) {
          points = polygons[i];
          joinedComponents.insert(i);
        } else {
          // if not already joined
          if (joinedComponents.find(i) == joinedComponents.end()) {
            boost::optional<std::vector<Point3d>> joined = join(points, polygons[i], tol);
            if (joined) {
              points = *joined;
              joinedComponents.insert(i);
            }
          }
        }
      }

      // if all polygons have been joined then we are done
      if (joinedComponents.size() == component.size()) {
        break;
      }
    }

    if (joinedComponents.size() != component.size()) {
      LOG_FREE(Error, "utilities.geometry.joinAll", "Could not join all connected components");
    }
    result.push_back(points);
  }

  return result;
}

boost::optional<IntersectionResult> intersect(const std::vector<Point3d>& polygon1, const std::vector<Point3d>& polygon2, double tol) {

  constexpr bool extraLogging = false;
  //std::cout << "Initial polygon1 area " << getArea(polygon1).get() << '\n';
  //std::cout << "Initial polygon2 area " << getArea(polygon2).get() << '\n';

  // convert vertices to boost rings
  std::vector<Point3d> allPoints;

  boost::optional<BoostRing> boostPolygon1 = nonIntersectingBoostRingFromVertices(polygon1, allPoints, tol);
  if (!boostPolygon1) {
    return boost::none;
  }

  boost::optional<BoostRing> boostPolygon2 = nonIntersectingBoostRingFromVertices(polygon2, allPoints, tol);
  if (!boostPolygon2) {
    return boost::none;
  }

  BoostMultiPolygon polys;
  if constexpr (extraLogging) {
    boost::optional<BoostPolygon> poly1 = boostPolygonFromVertices(polygon1, allPoints, tol);
    if (poly1.has_value()) {
      polys.push_back(*poly1);
    }
    boost::optional<BoostPolygon> poly2 = boostPolygonFromVertices(polygon2, allPoints, tol);
    if (poly2.has_value()) {
      polys.push_back(*poly2);
    }
  }

  // intersect the points in face coordinates,
  std::vector<BoostPolygon> intersectionResult;
  try {
    boost::geometry::intersection(*boostPolygon1, *boostPolygon2, intersectionResult);
  } catch (const boost::geometry::overlay_invalid_input_exception&) {
    LOG_FREE(Error, "utilities.geometry.intersect", "overlay_invalid_input_exception");
    return boost::none;
  }

  if constexpr (extraLogging) {
    polys.push_back(intersectionResult[0]);
  }

  // check if intersection is empty
  if (intersectionResult.empty()) {
    //LOG_FREE(Info, "utilities.geometry.intersect", "Intersection is empty");
    return boost::none;
  }

  intersectionResult = removeSpikes(intersectionResult);
  if (intersectionResult.empty()) {
    return boost::none;
  }
  if constexpr (extraLogging) {
    polys.push_back(intersectionResult[0]);
    LOG_FREE(Debug, "intersection result", polys);
  }
  intersectionResult = removeHoles(intersectionResult);

  // check for multiple intersections
  if (intersectionResult.size() > 1) {
    LOG_FREE(Info, "utilities.geometry.intersect", "Intersection has " << intersectionResult.size() << " elements");
    std::sort(intersectionResult.begin(), intersectionResult.end(), BoostPolygonAreaGreater());
  }

  // check that largest intersection is ok
  std::vector<Point3d> intersectionVertices = verticesFromBoostPolygon(intersectionResult[0], allPoints, tol, false);
  boost::optional<double> testArea = boost::geometry::area(intersectionResult[0]);
  if (!testArea || intersectionVertices.empty()) {
    LOG_FREE(Info, "utilities.geometry.intersect", "Cannot compute area of largest intersection");
    return boost::none;
  } else if (*testArea < tol * tol) {
    LOG_FREE(Info, "utilities.geometry.intersect", "Largest intersection has very small area of " << *testArea << " m^2");
    return boost::none;
  }
  try {
    has_self_intersections(intersectionResult[0]);
  } catch (const boost::geometry::overlay_invalid_input_exception&) {
    LOG_FREE(Error, "utilities.geometry.intersect", "Largest intersection is self intersecting");
    return boost::none;
  }
  if (!intersectionResult[0].inners().empty()) {
    LOG_FREE(Error, "utilities.geometry.intersect", "Largest intersection has inner loops");
    return boost::none;
  };

  std::vector<std::vector<Point3d>> newPolygons1;
  newPolygons1.reserve(intersectionResult.size() - 1);

  // create new polygon for each **remaining** intersection (all but the largest one)
  for (unsigned i = 1; i < intersectionResult.size(); ++i) {

    std::vector<Point3d> newPolygon = verticesFromBoostPolygon(intersectionResult[i], allPoints, tol, false);

    testArea = boost::geometry::area(intersectionResult[i]);
    if (!testArea || newPolygon.empty()) {
      LOG_FREE(Info, "utilities.geometry.intersect", "Cannot compute area of intersection, result will not include this polygon, " << newPolygon);
      continue;
    } else if (*testArea < tol * tol) {
      LOG_FREE(Info, "utilities.geometry.intersect",
               "Intersection has very small area of " << *testArea << " m^2, result will not include this polygon, " << newPolygon);
      continue;
    }
    try {
      has_self_intersections(intersectionResult[i]);
    } catch (const boost::geometry::overlay_invalid_input_exception&) {
      LOG_FREE(Error, "utilities.geometry.intersect", "Intersection is self intersecting, result will not include this polygon, " << newPolygon);
      continue;
    }
    if (!intersectionResult[i].inners().empty()) {
      LOG_FREE(Error, "utilities.geometry.intersect", "Intersection has inner loops, result will not include this polygon, " << newPolygon);
      continue;
    };

    newPolygons1.push_back(std::move(newPolygon));
  }

  std::vector<std::vector<Point3d>> newPolygons2 = newPolygons1;

  // polygon1 minus polygon2
  std::vector<BoostPolygon> differenceResult1;
  boost::geometry::difference(*boostPolygon1, *boostPolygon2, differenceResult1);
  differenceResult1 = removeSpikes(differenceResult1);
  differenceResult1 = removeHoles(differenceResult1);

  newPolygons1.reserve(newPolygons1.size() + differenceResult1.size());

  // create new polygon for each difference
  for (auto& pp : differenceResult1) {
    std::vector<Point3d> newPolygon1 = verticesFromBoostPolygon(pp, allPoints, tol, false);

    testArea = boost::geometry::area(pp);
    if (!testArea || newPolygon1.empty()) {
      LOG_FREE(Info, "utilities.geometry.intersect", "Cannot compute area of face difference, result will not include this polygon, " << newPolygon1);
      continue;
    } else if (*testArea < tol * tol) {
      LOG_FREE(Info, "utilities.geometry.intersect",
               "Face difference has very small area of " << *testArea << " m^2, result will not include this polygon, " << newPolygon1);
      continue;
    }
    try {
      has_self_intersections(pp);
    } catch (const boost::geometry::overlay_invalid_input_exception&) {
      LOG_FREE(Error, "utilities.geometry.intersect", "Face difference is self intersecting, result will not include this polygon, " << newPolygon1);
      continue;
    }

    newPolygons1.push_back(std::move(newPolygon1));
  }

  // polygon2 minus polygon1
  std::vector<BoostPolygon> differenceResult2;
  boost::geometry::difference(*boostPolygon2, *boostPolygon1, differenceResult2);
  differenceResult2 = removeSpikes(differenceResult2);
  differenceResult2 = removeHoles(differenceResult2);

  newPolygons2.reserve(newPolygons2.size() + differenceResult2.size());

  // create new polygon for each difference
  for (auto& pp : differenceResult2) {
    std::vector<Point3d> newPolygon2 = verticesFromBoostPolygon(pp, allPoints, tol, false);

    testArea = boost::geometry::area(pp);
    if (!testArea || newPolygon2.empty()) {
      LOG_FREE(Info, "utilities.geometry.intersect", "Cannot compute area of face difference, result will not include this polygon, " << newPolygon2);
      continue;
    } else if (*testArea < tol * tol) {
      LOG_FREE(Info, "utilities.geometry.intersect",
               "Face difference has very small area of " << *testArea << " m^2, result will not include this polygon, " << newPolygon2);
      continue;
    }
    try {
      has_self_intersections(pp);
    } catch (const boost::geometry::overlay_invalid_input_exception&) {
      LOG_FREE(Error, "utilities.geometry.intersect", "Face difference is self intersecting, result will not include this polygon, " << newPolygon2);
      continue;
    }

    newPolygons2.push_back(std::move(newPolygon2));
  }

  // intersections are the same
  std::vector<Point3d> resultPolygon1 = intersectionVertices;
  IntersectionResult result(std::move(resultPolygon1), std::move(intersectionVertices), std::move(newPolygons1), std::move(newPolygons2));

  //std::cout << "Result area1 " << result.area1() << '\n';
  //std::cout << "Result area2 " << result.area2() << '\n';

  return result;
}

std::vector<std::vector<Point3d>> subtract(const std::vector<Point3d>& polygon, const std::vector<std::vector<Point3d>>& holes, double tol) {
  std::vector<std::vector<Point3d>> result;

  // convert vertices to boost rings
  std::vector<Point3d> allPoints;

  boost::optional<BoostPolygon> initialBoostPolygon = nonIntersectingBoostPolygonFromVertices(polygon, allPoints, tol);
  if (!initialBoostPolygon) {
    return result;
  }

  std::vector<BoostPolygon> boostPolygons;
  boostPolygons.push_back(*initialBoostPolygon);

  for (const auto& hole : holes) {
    boost::optional<BoostPolygon> boostHole = nonIntersectingBoostPolygonFromVertices(hole, allPoints, tol);
    if (!boostHole) {
      return result;
    }

    std::vector<BoostPolygon> newBoostPolygons;

    for (const BoostPolygon& boostPolygon : boostPolygons) {
      std::vector<BoostPolygon> diffResult;
      boost::geometry::difference(boostPolygon, *boostHole, diffResult);
      newBoostPolygons.reserve(newBoostPolygons.size() + diffResult.size());
      newBoostPolygons.insert(newBoostPolygons.end(), std::make_move_iterator(diffResult.begin()), std::make_move_iterator(diffResult.end()));
    }
    boostPolygons = std::move(newBoostPolygons);
  }

  // Remove the holes and spikes and convert back to our data types
  for (const BoostPolygon& boostPolygon : boostPolygons) {
    const BoostPolygon removedSpikes = removeSpikes(boostPolygon);
    const std::vector<BoostPolygon>& removedHoles = removeHoles(removedSpikes);
    result.reserve(result.size() + removedHoles.size());
    for (const BoostPolygon& removedHole : removedHoles) {
      result.push_back(verticesFromBoostPolygon(removedHole, allPoints, tol));
    }
  }

  return result;
}

bool selfIntersects(const std::vector<Point3d>& polygon, double tol) {
  // convert vertices to boost rings
  std::vector<Point3d> allPoints;

  const boost::optional<BoostPolygon> bp = nonIntersectingBoostPolygonFromVertices(polygon, allPoints, tol);
  // if bp has a value, we're able to get a non intersecting polygon, so does not self intersect
  return !bp.has_value();
}

bool intersects(const std::vector<Point3d>& polygon1, const std::vector<Point3d>& polygon2, double tol) {
  // convert vertices to boost rings
  std::vector<Point3d> allPoints;

  boost::optional<BoostPolygon> bp1 = boostPolygonFromVertices(polygon1, allPoints, tol);
  boost::optional<BoostPolygon> bp2 = boostPolygonFromVertices(polygon2, allPoints, tol);

  if (bp1 && bp2) {
    return boost::geometry::intersects(*bp1, *bp2);
  }

  return false;
}

bool within(const Point3d& point1, const std::vector<Point3d>& polygon2, double tol) {
  const std::vector<Point3d> geometry1 = {point1};
  return within(geometry1, polygon2, tol);
}

bool within(const std::vector<Point3d>& geometry1, const std::vector<Point3d>& polygon2, double tol) {
  // convert vertices to boost rings
  std::vector<Point3d> allPoints;

  if (geometry1.size() == 1) {
    if (geometry1[0].z() > tol) {
      return false;
    }

    boost::tuple<double, double> p = boostPointFromPoint3d(geometry1[0], allPoints, tol);
    const BoostPoint boostPoint(p.get<0>(), p.get<1>());

    boost::optional<BoostPolygon> bp2 = boostPolygonFromVertices(polygon2, allPoints, tol);

    if (bp2) {
      return boost::geometry::within(boostPoint, *bp2);
    }

    return false;
  }

  /*
    // DLM: this is the better implementation, requires boost 1.57
    boost::optional<BoostPolygon> bp1 = boostPolygonFromVertices(geometry1, allPoints, tol);
    boost::optional<BoostPolygon> bp2 = boostPolygonFromVertices(polygon2, allPoints, tol);
    if (bp1 && bp2){
      return boost::geometry::within(*bp1, *bp2);
    }
    */

  // DLM: temp code
  if (geometry1.size() < 3) {
    return false;
  }
  boost::optional<BoostPolygon> bp2 = boostPolygonFromVertices(polygon2, allPoints, tol);
  if (bp2) {
    for (const Point3d& point : geometry1) {
      boost::tuple<double, double> p = boostPointFromPoint3d(point, allPoints, tol);
      const BoostPoint boostPoint(p.get<0>(), p.get<1>());

      if (!boost::geometry::within(boostPoint, *bp2)) {
        return false;
      }
    }
  } else {
    return false;
  }

  return true;
}

boost::optional<double> getLinearAlpha(const Point3d& point0, const Point3d& point1, const Point3d& test) {
  // test = point0 + a*(point1 - point0)
  Vector3d diff1 = point1 - point0;
  const Vector3d diffTest = test - point0;

  const double length = diff1.length();
  if (length < 0.001) {
    return boost::none;
  }

  double a = 0.0;
  if (std::abs(diff1.x()) > std::abs(diff1.y())) {
    if (std::abs(diff1.x()) > std::abs(diff1.z())) {
      a = diffTest.x() / diff1.x();
    } else {
      a = diffTest.z() / diff1.z();
    }
  } else {
    if (std::abs(diff1.y()) > std::abs(diff1.z())) {
      a = diffTest.y() / diff1.y();
    } else {
      a = diffTest.z() / diff1.z();
    }
  }

  diff1.setLength(a * length);
  const Point3d test2 = point0 + diff1;
  const double d = getDistance(test, test2);
  if (d < 0.001) {
    if (a > 0 && a < 1) {
      return a;
    }
  }

  return boost::none;
}

std::vector<Point3d> simplify(const std::vector<Point3d>& vertices, bool removeCollinear, double tol) {
  std::vector<Point3d> allPoints;

  bool reversed = false;
  boost::optional<Vector3d> outwardNormal = getOutwardNormal(vertices);
  if (!outwardNormal) {
    return {};
  } else if (outwardNormal->z() > 0) {
    reversed = true;
  }

  boost::optional<BoostPolygon> bp;
  if (reversed) {
    bp = boostPolygonFromVertices(reorderULC(reverse(vertices)), allPoints, tol);
  } else {
    bp = boostPolygonFromVertices(reorderULC(vertices), allPoints, tol);
  }

  if (!bp) {
    return {};
  }

  boost::geometry::remove_spikes(*bp);

  BoostPolygon out;

  // this uses the Douglas-Peucker algorithm with a max difference of 0 so no non-collinear points will be removed
  // if we want to allow this algorithm to be called with a non-zero value I suggest naming it "approximate" or something
  if (!removeCollinear) {
    boost::geometry::simplify(*bp, out, 0.0);
  } else {
    boost::geometry::simplify(*bp, out, tol);  // points within tol would already be merged
  }

  std::vector<Point3d> tmp = verticesFromBoostPolygon(out, allPoints, tol, removeCollinear);

  if (reversed) {
    tmp = reorderULC(reverse(tmp));
  } else {
    tmp = reorderULC(tmp);
  }

  if (removeCollinear) {
    // collinear points already removed
    return tmp;
  }

  if (tmp.empty()) {
    return tmp;
  }

  // we want to add back in all the unique points, have to put them in the right place
  std::set<size_t> pointsToAdd;
  for (size_t i = 0; i < allPoints.size(); ++i) {
    bool found = false;
    for (const auto& tmpPoint : tmp) {
      if (getDistance(tmpPoint, allPoints[i]) < tol) {
        found = true;
      }
    }
    if (!found) {
      pointsToAdd.insert(i);
    }
  }

  std::vector<Point3d> result;
  result.push_back(tmp[0]);
  for (size_t i = 1; i < tmp.size(); ++i) {
    // see which remaining points fit in this segment, double is index in allPoints, alpha along line
    std::vector<std::pair<size_t, double>> pointsInSegment;
    for (const size_t j : pointsToAdd) {
      boost::optional<double> alpha = getLinearAlpha(tmp[i - 1], tmp[i], allPoints[j]);
      if (alpha) {
        pointsInSegment.emplace_back(j, *alpha);
      }
    }

    std::sort(pointsInSegment.begin(), pointsInSegment.end(),
              [](std::pair<size_t, double> a, std::pair<size_t, double> b) { return a.second < b.second; });

    for (const auto& pointInSegment : pointsInSegment) {
      result.push_back(allPoints[pointInSegment.first]);
      pointsToAdd.erase(pointInSegment.first);
    }

    // push the next point in simplified polygon
    result.push_back(tmp[i]);
  }

  // now check between last point and first point
  std::vector<std::pair<size_t, double>> pointsInSegment;
  for (const size_t j : pointsToAdd) {
    boost::optional<double> alpha = getLinearAlpha(tmp[tmp.size() - 1], tmp[0], allPoints[j]);
    if (alpha) {
      pointsInSegment.emplace_back(j, *alpha);
    }
  }

  std::sort(pointsInSegment.begin(), pointsInSegment.end(),
            [](std::pair<size_t, double> a, std::pair<size_t, double> b) { return a.second < b.second; });

  for (const auto& pointInSegment : pointsInSegment) {
    result.push_back(allPoints[pointInSegment.first]);
    pointsToAdd.erase(pointInSegment.first);
  }

  if (!pointsToAdd.empty()) {
    LOG_FREE(Warn, "utilities.geometry.simplify", pointsToAdd.size() << " unique vertices were not added back to the polygon");
  }

  return result;
}

/// Converts a Polygon to a BoostPolygon
boost::optional<BoostPolygon> BoostPolygonFromPolygon(const Polygon3d& polygon, Point3dVector& allPoints, double tol) {
  BoostPolygon boostPolygon;

  for (const Point3d& vertex : polygon.getOuterPath()) {

    // should all have zero z coordinate now
    //double z = vertex.z();
    //if (std::abs(z) > tol) {
    //  LOG_FREE(Error, "utilities.geometry.boostPolygonFromVertices", "All points must be on z = 0 plane");
    //  return boost::none;
    //}
    boost::geometry::append(boostPolygon, boostPointFromPoint3d(vertex, allPoints, tol));
  }

  const Point3dVector& path = polygon.getOuterPath();
  const Point3d& first = path.front();
  boost::geometry::append(boostPolygon, boostPointFromPoint3d(first, allPoints, tol));

  return boostPolygon;
}

Polygon3d PolygonFromBoostPolygon(const BoostPolygon& boostPolygon, Point3dVector& allPoints, double tol) {
  Polygon3d p;
  BoostRing outer = boostPolygon.outer();
  if (outer.empty()) {
    return p;
  }

  Point3dVector points;
  for (unsigned i = 0; i < outer.size() - 1; ++i) {
    const Point3d point3d(outer[i].x() / scaleBy, outer[i].y() / scaleBy, 0.0);
    Point3d resultPoint = getCombinedPoint(point3d, allPoints, tol);
    // don't keep repeated vertices
    if ((i > 0) && (points.back() == resultPoint)) {
      continue;
    }
    points.emplace_back(std::move(resultPoint));
  }

  points = removeCollinearLegacy(points);
  for (const auto& point : points) {
    p.addPoint(point);
  }

  for (const auto& inner : boostPolygon.inners()) {
    Point3dVector hole;
    for (unsigned i = 0; i < inner.size() - 1; ++i) {
      Point3d point3d(inner[i].x() / scaleBy, inner[i].y() / scaleBy, 0.0);
      const Point3d resultPoint = getCombinedPoint(point3d, allPoints, tol);
      // don't keep repeated vertices
      if ((i > 0) && (hole.back() == resultPoint)) {
        continue;
      }
      hole.emplace_back(std::move(resultPoint));
    }
    p.addHole(hole);
  }

  return p;
}

// Non class member stuff
boost::optional<Polygon3d> join(const Polygon3d& polygon1, const Polygon3d& polygon2) {
  Point3dVector allPoints;

  constexpr double tol = 0.01;

  // Convert polygons to boost polygon (not ring obvs)
  boost::optional<BoostPolygon> boostPolygon1 = BoostPolygonFromPolygon(polygon1, allPoints, tol);
  if (!boostPolygon1) {
    return boost::none;
  }

  boost::optional<BoostPolygon> boostPolygon2 = BoostPolygonFromPolygon(polygon2, allPoints, tol);
  if (!boostPolygon2) {
    return boost::none;
  }

  std::vector<BoostPolygon> unionResult;
  try {
    boost::geometry::union_(*boostPolygon1, *boostPolygon2, unionResult);
  } catch (const boost::geometry::overlay_invalid_input_exception&) {
    LOG_FREE(Error, "utilities.geometry.join", "overlay_invalid_input_exception");
    return boost::none;
  }

  // Smooth the result
  unionResult = removeSpikes(unionResult);

  // Check the result - we do not have to bail for holes but we bail for > 1 poly
  if (unionResult.empty() or (unionResult.size() > 1)) {
    return boost::none;
  }

  // Convert back to polygon
  Polygon3d p = PolygonFromBoostPolygon(unionResult.front(), allPoints, tol);
  return p;
}

std::vector<Polygon3d> joinAllPolygons(const std::vector<std::vector<Point3d>>& polygons, double tol) {
  std::vector<Polygon3d> inputPolygons;

  // Create Polygon3d from point3dvectors
  for (const auto& polygon : polygons) {
    Polygon3d inputPolygon;
    for (const auto& point : polygon) {
      inputPolygon.addPoint(point);
    }
    inputPolygons.push_back(inputPolygon);
  }

  return joinAll(inputPolygons, tol);
}

std::vector<Polygon3d> joinAll(const std::vector<Polygon3d>& polygons, double /*tol*/) {

  std::vector<Polygon3d> result;

  const size_t N = polygons.size();
  if (N <= 1) {
    return polygons;
  }

  std::vector<double> polygonAreas(N, 0.0);
  for (unsigned i = 0; i < N; ++i) {
    auto area = getArea(polygons[i].getOuterPath());
    if (area) {
      polygonAreas[i] = *area;
    }
  }

  // compute adjacency matrix
  Matrix A(N, N, 0.0);
  for (unsigned i = 0; i < N; ++i) {
    A(i, i) = 1.0;
    for (unsigned j = i + 1; j < N; ++j) {
      if (join(polygons[i], polygons[j] /*,tol*/)) {
        A(i, j) = 1.0;
        A(j, i) = 1.0;
      }
    }
  }

  const std::vector<std::vector<unsigned>> connectedComponents = findConnectedComponents(A);
  for (const std::vector<unsigned>& component : connectedComponents) {

    std::vector<unsigned> orderedComponent(component);
    // #4831 - Use a stable_sort to produce consistent results between Windows and Unix in case you have polygons with the same area
    std::stable_sort(orderedComponent.begin(), orderedComponent.end(),
                     [&polygonAreas](int ia, int ib) { return polygonAreas[ia] > polygonAreas[ib]; });

    Polygon3d polygon;
    std::set<unsigned> joinedComponents;
    // try to join at most component.size() times
    for (unsigned n = 0; n < component.size(); ++n) {

      // loop over polygons to join in order
      for (const unsigned i : orderedComponent) {
        if (polygon.getOuterPath().empty()) {
          polygon = polygons[i];
          joinedComponents.insert(i);
        } else {
          // if not already joined
          if (joinedComponents.find(i) == joinedComponents.end()) {
            boost::optional<Polygon3d> joined = join(polygon, polygons[i] /*, tol*/);
            if (joined) {
              polygon = *joined;
              joinedComponents.insert(i);
            }
          }
        }
      }

      // if all polygons have been joined then we are done
      if (joinedComponents.size() == component.size()) {
        break;
      }
    }

    if (joinedComponents.size() != component.size()) {
      LOG_FREE(Error, "utilities.geometry.joinAll", "Could not join all connected components");
    }
    result.push_back(polygon);
  }

  return result;
}

std::vector<Polygon3d> bufferAll(const std::vector<Polygon3d>& polygons, double tol) {
  BoostMultiPolygon source;
  std::vector<Point3d> allPoints;

  for (const Polygon3d& polygon : polygons) {
    boost::optional<BoostPolygon> boostPolygon = BoostPolygonFromPolygon(polygon, allPoints, tol);
    source.push_back(*boostPolygon);
  }

  const boost::geometry::strategy::buffer::distance_symmetric<coordinate_type> expand(tol * scaleBy);
  const boost::geometry::strategy::buffer::distance_symmetric<coordinate_type> shrink(-tol * scaleBy);
  const boost::geometry::strategy::buffer::join_miter join_strategy;
  const boost::geometry::strategy::buffer::end_flat end_strategy;
  const boost::geometry::strategy::buffer::side_straight side_strategy;
  const boost::geometry::strategy::buffer::point_circle point_strategy;

  BoostMultiPolygon resultExpand;
  BoostMultiPolygon resultShrink;
  boost::geometry::buffer(source, resultExpand, expand, side_strategy, join_strategy, end_strategy, point_strategy);
  boost::geometry::buffer(resultExpand, resultShrink, shrink, side_strategy, join_strategy, end_strategy, point_strategy);

  std::vector<Polygon3d> result;
  result.reserve(resultShrink.size());
  for (const auto& boostPolygon : resultShrink) {
    BoostPolygon simplified;
    boost::geometry::simplify(boostPolygon, simplified, tol);
    result.push_back(PolygonFromBoostPolygon(simplified, allPoints, tol));
  }

  return result;
}

boost::optional<std::vector<Point3d>> buffer(const std::vector<Point3d>& polygon1, double amount, double tol) {
  std::vector<Point3d> allPoints;
  boost::optional<BoostPolygon> boostPolygon1 = nonIntersectingBoostPolygonFromVertices(polygon1, allPoints, tol);

  if (!boostPolygon1) {
    return boost::none;
  }

  constexpr double miterLimit = 15;
  //const double buffer_distance = 1.0;
  //const int points_per_circle = 36;
  const boost::geometry::strategy::buffer::distance_symmetric<coordinate_type> distance_strategy(amount * scaleBy);
  const boost::geometry::strategy::buffer::join_miter join_strategy(miterLimit * scaleBy);
  const boost::geometry::strategy::buffer::end_flat end_strategy;
  const boost::geometry::strategy::buffer::side_straight side_strategy;
  const boost::geometry::strategy::buffer::point_circle point_strategy;

  BoostMultiPolygon polygons;
  polygons.push_back(*boostPolygon1);

  BoostMultiPolygon buffered;
  BoostMultiPolygon result;

  boost::geometry::buffer(polygons, buffered, distance_strategy, side_strategy, join_strategy, end_strategy, point_strategy);
  boost::geometry::simplify(buffered, result, 0.0005);

  std::vector<Point3d> vertices = verticesFromBoostPolygon(result[0], allPoints, tol);
  return vertices;
}

boost::optional<std::vector<std::vector<Point3d>>> buffer(const std::vector<std::vector<Point3d>>& polygons, double amount, double tol) {
  std::vector<Point3d> allPoints;

  BoostMultiPolygon boostPolygons;
  boostPolygons.reserve(polygons.size());
  for (const auto& polygon : polygons) {
    boost::optional<BoostPolygon> boostPolygon = nonIntersectingBoostPolygonFromVertices(polygon, allPoints, tol);
    if (!boostPolygon) {
      return boost::none;
    }
    boostPolygons.push_back(boostPolygon.get());
  }

  constexpr double miterLimit = 15;
  const boost::geometry::strategy::buffer::distance_symmetric<coordinate_type> distance_strategy(amount * scaleBy);
  const boost::geometry::strategy::buffer::join_miter join_strategy(miterLimit * scaleBy);
  const boost::geometry::strategy::buffer::end_flat end_strategy;
  const boost::geometry::strategy::buffer::side_straight side_strategy;
  const boost::geometry::strategy::buffer::point_circle point_strategy;

  BoostMultiPolygon offset;
  BoostMultiPolygon result;
  boost::geometry::buffer(boostPolygons, offset, distance_strategy, side_strategy, join_strategy, end_strategy, point_strategy);
  boost::geometry::simplify(offset, result, 0.0005);

  std::vector<Point3dVector> results;
  results.reserve(result.size());
  for (const auto& boostPolygon : result) {
    results.push_back(verticesFromBoostPolygon(boostPolygon, allPoints, tol));
  }
  return results;
}

// buffers the polygons to increase reliability of joining
std::vector<std::vector<Point3d>> joinAllWithBuffer(const std::vector<std::vector<Point3d>>& polygons, double offset, double tol) {
  std::vector<std::vector<Point3d>> result;

  const size_t N = polygons.size();
  if (N <= 1) {
    return polygons;
  }

  std::vector<double> polygonAreas(N, 0.0);
  for (unsigned i = 0; i < N; ++i) {
    auto area = getArea(polygons[i]);
    if (area) {
      polygonAreas[i] = *area;
    }
  }

  std::vector<std::vector<Point3d>> modifiedPolygons;

  for (unsigned i = 0; i < N; i++) {
    modifiedPolygons.push_back(*buffer(polygons[i], offset, tol));
  }

  // compute adjacency matrix
  Matrix A(N, N, 0.0);
  for (unsigned i = 0; i < N; ++i) {
    A(i, i) = 1.0;
    for (unsigned j = i + 1; j < N; ++j) {
      if (join(modifiedPolygons[i], modifiedPolygons[j], tol)) {
        A(i, j) = 1.0;
        A(j, i) = 1.0;
      }
    }
  }

  const std::vector<std::vector<unsigned>> connectedComponents = findConnectedComponents(A);
  for (const std::vector<unsigned>& component : connectedComponents) {
    std::vector<unsigned> orderedComponent(component);
    std::sort(orderedComponent.begin(), orderedComponent.end(), [&polygonAreas](int ia, int ib) { return polygonAreas[ia] > polygonAreas[ib]; });

    std::vector<Point3d> points;
    std::set<unsigned> joinedComponents;

    // try to join at most component.size() times
    for (unsigned n = 0; n < component.size(); ++n) {

      // loop over polygons to join in order
      for (const unsigned i : orderedComponent) {
        if (points.empty()) {
          points = modifiedPolygons[i];
          joinedComponents.insert(i);
        } else {
          // if not already joined
          if (joinedComponents.find(i) == joinedComponents.end()) {
            boost::optional<std::vector<Point3d>> joined = join(points, modifiedPolygons[i], tol);
            if (joined) {
              points = *joined;
              joinedComponents.insert(i);
            }
          }
        }
      }

      // if all polygons have been joined then we are done
      if (joinedComponents.size() == component.size()) {
        break;
      }
    }

    if (joinedComponents.size() != component.size()) {
      LOG_FREE(Error, "utilities.geometry.joinAll", "Could not join all connected components");
    }
    points = simplify(points, true, tol);
    result.push_back(points);
  }

  return result;
}
}  // namespace openstudio
