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
#include "Intersection.hpp"
#include "../data/Matrix.hpp"
#include "../core/Assert.hpp"
#include "../core/Logger.hpp"

#undef BOOST_UBLAS_TYPE_CHECK
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#include <boost/geometry/geometries/adapted/boost_tuple.hpp>
#include <boost/geometry/strategies/cartesian/point_in_poly_franklin.hpp> 
#include <boost/geometry/strategies/cartesian/point_in_poly_crossings_multiply.hpp> 

typedef boost::geometry::model::d2::point_xy<double> BoostPoint;
typedef boost::geometry::model::polygon<BoostPoint> BoostPolygon;
typedef boost::geometry::model::ring<BoostPoint> BoostRing;
typedef boost::geometry::model::multi_polygon<BoostPolygon> BoostMultiPolygon;

#include <polypartition/polypartition.h>

#include <list>

// remove_spikes 
// adapted from https://github.com/boostorg/geometry/commits/develop/include/boost/geometry/algorithms/remove_spikes.hpp eb3260708eb241d8da337f4be73b41d69d33cd09

/*
Remove spikes from a ring/polygon.
Ring (having 8 vertices, including closing vertex)
+------+
| |
| +--+
| | ^this "spike" is removed, can be located outside/inside the ring
+------+
(the actual determination if it is removed is done by a strategy)

*/

namespace openstudio {

  template <typename Point1, typename Point2, typename Point3>
  static inline bool point_is_spike_or_equal(Point1 const& last_point, Point2 const& segment_a, Point3 const& segment_b)
  {
    // adapted from boost\geometry\algorithms\detail\point_is_spike_or_equal.hpp to include tolerance checking

    // segment_a is at the beginning
    // segment_b is in the middle
    // last_point is at the end

    // segment_b is being considered for deletion

    double normTol = 0.001; // 1 mm
    double tol = 0.001; // relative to 1
      
    double diff1_x = last_point.x()-segment_b.x();
    double diff1_y = last_point.y()-segment_b.y();
    double norm1 = sqrt(pow(diff1_x, 2) + pow(diff1_y, 2)); 
    if (norm1 > normTol){
      diff1_x = diff1_x/norm1;
      diff1_y = diff1_y/norm1;
    }else{
      // last point is too close to segment b
      return true;
    }

    double diff2_x = segment_b.x()-segment_a.x();
    double diff2_y = segment_b.y()-segment_a.y();
    double norm2 = sqrt(pow(diff2_x, 2) + pow(diff2_y, 2));
    if (norm2 > normTol){
      diff2_x = diff2_x/norm2;
      diff2_y = diff2_y/norm2;
    }else{
      // segment b is too close to segment a
      return true;
    }

    double crossProduct = diff1_x*diff2_y-diff1_y*diff2_x;
    if (abs(crossProduct) < tol){
      double dotProduct = diff1_x*diff2_x+diff1_y*diff2_y;
      if (dotProduct <= -1.0 + tol){
        // reversal
        return true;
      }
    }

    return false;
  }

}

namespace boost { namespace geometry
{


#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace remove_spikes
{


template <typename Range>
struct range_remove_spikes
{
    typedef typename strategy::side::services::default_strategy
    <
        typename cs_tag<Range>::type
    >::type side_strategy;

    typedef typename coordinate_type<Range>::type coordinate_type;
    typedef typename point_type<Range>::type point_type;


    static inline void apply(Range& range)
    {
        std::size_t n = boost::size(range);
        std::size_t const min_num_points = core_detail::closure::minimum_ring_size
            <
                geometry::closure<Range>::value
            >::value;
        if (n < min_num_points)
        {
            return;
        }

        typedef typename boost::range_iterator<Range>::type iterator;

        std::deque<point_type> cleaned;
        for (typename boost::range_iterator<Range const>::type it = boost::begin(range);
            it != boost::end(range); ++it)
        {
            // Add point
            cleaned.push_back(*it);

            while(cleaned.size() >= 3
                    && openstudio::point_is_spike_or_equal(cleaned.back(), *(cleaned.end() - 3), *(cleaned.end() - 2)))
            {
                // Remove pen-ultimate point causing the spike (or which was equal)
                cleaned.erase(cleaned.end() - 2);
            }
        }

        // For a closed-polygon, remove closing point, this makes checking first point(s) easier and consistent
        if (geometry::closure<Range>::value == geometry::closed)
        {
            cleaned.pop_back();
        }

        bool found = false;
        do
        {
            found = false;
            // Check for spike in first point
            int const penultimate = 2;
            while(cleaned.size() > 3 && openstudio::point_is_spike_or_equal(cleaned.front(), *(cleaned.end() - penultimate), cleaned.back()))
            {
                cleaned.pop_back();
                found = true;
            }
            // Check for spike in second point
            while(cleaned.size() > 3 && openstudio::point_is_spike_or_equal(*(cleaned.begin() + 1), cleaned.back(), cleaned.front()))
            {
                cleaned.pop_front();
                found = true;
            }
        }
        while (found);

        // Close if necessary
        if (geometry::closure<Range>::value == geometry::closed)
        {
            cleaned.push_back(cleaned.front());
        }

        // Copy output
        geometry::clear(range);
        std::copy(cleaned.begin(), cleaned.end(), std::back_inserter(range));
    }
};


template <typename Polygon>
struct polygon_remove_spikes
{
    static inline void apply(Polygon& polygon)
    {
        typedef typename geometry::ring_type<Polygon>::type ring_type;

        typedef range_remove_spikes<ring_type> per_range;
        per_range::apply(exterior_ring(polygon));

        typename interior_return_type<Polygon>::type rings
                    = interior_rings(polygon);
        for (BOOST_AUTO_TPL(it, boost::begin(rings)); it != boost::end(rings); ++it)
        {
            per_range::apply(*it);
        }
    }
};


}} // namespace detail::remove_spikes
#endif // DOXYGEN_NO_DETAIL



#ifndef DOXYGEN_NO_DISPATCH
namespace dispatch
{


template
<
    typename Geometry,
    typename Tag = typename tag<Geometry>::type
>
struct remove_spikes
{
    static inline void apply(Geometry&)
    {}
};


template <typename Ring>
struct remove_spikes<Ring, ring_tag>
    : detail::remove_spikes::range_remove_spikes<Ring>
{};



template <typename Polygon>
struct remove_spikes<Polygon, polygon_tag>
    : detail::remove_spikes::polygon_remove_spikes<Polygon>
{};



} // namespace dispatch
#endif


namespace resolve_variant {

template <typename Geometry>
struct remove_spikes
{
    static void apply(Geometry& geometry)
    {
        concept::check<Geometry>();
        dispatch::remove_spikes<Geometry>::apply(geometry);
    }
};

template <BOOST_VARIANT_ENUM_PARAMS(typename T)>
struct remove_spikes<boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)> >
{
    struct visitor: boost::static_visitor<void>
    {
        template <typename Geometry>
        void operator()(Geometry& geometry) const
        {
            remove_spikes<Geometry>::apply(geometry);
        }
    };

    static inline void apply(boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>& geometry)
    {
        boost::apply_visitor(visitor(), geometry);
    }
};

} // namespace resolve_variant


/*!
\ingroup remove_spikes
\tparam Geometry geometry type
\param geometry the geometry to make remove_spikes
*/
template <typename Geometry>
inline void remove_spikes(Geometry& geometry)
{
    resolve_variant::remove_spikes<Geometry>::apply(geometry);
}


}} // namespace boost::geometry
// remove_spikes 


namespace openstudio{

  // Private implementation functions

  BoostPolygon removeSpikes(const BoostPolygon& polygon)
  {
    BoostPolygon temp(polygon);
    boost::geometry::remove_spikes(temp);
    return temp;
  }

  std::vector<BoostPolygon> removeSpikes(const std::vector<BoostPolygon>& polygons)
  {
    std::vector<BoostPolygon> result;
    for (const BoostPolygon& polygon : polygons){
      result.push_back(removeSpikes(polygon));
    }
    return result;
  }

  std::vector<BoostPolygon> removeHoles(const BoostPolygon& boostPolygon)
  {
    std::vector<BoostPolygon> result;

    // convert to vector of TPPLPoly
    std::list<TPPLPoly> polys;

    BoostRing outer = boostPolygon.outer();
    TPPLPoly outerPoly; // must be counter-clockwise
    outerPoly.Init(outer.size() - 1);
    outerPoly.SetHole(false);
    //std::cout << "outer :";
    for(unsigned i = 0; i < outer.size() - 1; ++i){
      outerPoly[i].x = outer[i].x();
      outerPoly[i].y = outer[i].y();
      //std::cout << "(" << outer[i].x() << ", " << outer[i].y() << ") ";
    }
    //std::cout << std::endl;
    outerPoly.SetOrientation(TPPL_CCW);
    polys.push_back(outerPoly);

    std::vector<BoostRing> inners = boostPolygon.inners();
    for (const BoostRing& inner : inners){
      TPPLPoly innerPoly; // must be clockwise
      innerPoly.Init(inner.size() - 1);
      innerPoly.SetHole(true);
      //std::cout << "inner :";
      for(unsigned i = 0; i < inner.size() - 1; ++i){
        innerPoly[i].x = inner[i].x();
        innerPoly[i].y = inner[i].y();
        //std::cout << "(" << inner[i].x() << ", " << inner[i].y() << ") ";
      }
      //std::cout << std::endl;
      innerPoly.SetOrientation(TPPL_CW);
      polys.push_back(innerPoly);
    }

    // do partitioning
    TPPLPartition pp;
    std::list<TPPLPoly> resultPolys;
    int test = pp.ConvexPartition_HM(&polys,&resultPolys);
    if (test == 0){
      LOG_FREE(Error, "utilities.geometry.removeHoles", "Failed to partition polygon");
      return result;
    }

    // convert back to BoostPolygon
    std::list<TPPLPoly>::iterator it, itend;
    for(it = resultPolys.begin(), itend = resultPolys.end(); it != itend; ++it){
      BoostPolygon newBoostPolygon;
      //std::cout << "result :";
      for (long i = 0; i < it->GetNumPoints(); ++i){
        TPPLPoint point = it->GetPoint(i);
        boost::geometry::append(newBoostPolygon, boost::make_tuple(point.x, point.y));
        //std::cout << "(" << point.x << ", " << point.y << ") ";
      }
      TPPLPoint point = it->GetPoint(0);
      boost::geometry::append(newBoostPolygon, boost::make_tuple(point.x, point.y));
      //std::cout << "(" << point.x << ", " << point.y << ") ";
      //std::cout << std::endl;

      boost::geometry::correct(newBoostPolygon);
      result.push_back(newBoostPolygon);
    }

    return result;
  }

  std::vector<BoostPolygon> removeHoles(const std::vector<BoostPolygon>& polygons)
  {
    std::vector<BoostPolygon> result;
    for (const BoostPolygon polygon : polygons){
      if (polygon.inners().empty()){
        // DLM: might also want to partition if this polygon is self intersecting?
        result.push_back(polygon);
      }else{
        std::vector<BoostPolygon> temp = removeHoles(polygon); 
        result.insert(result.end(), temp.begin(), temp.end());
      }
    }
    return result;
  }

  // convert a Point3d to a BoostPoint
  boost::tuple<double, double> boostPointFromPoint3d(const Point3d& point3d, std::vector<Point3d>& allPoints, double tol)
  {
    OS_ASSERT(abs(point3d.z()) <= tol);

    // simple method
    //return boost::make_tuple(point3d.x(), point3d.y());

    // detailed method, try to combine points within tolerance
    Point3d resultPoint = getCombinedPoint(point3d, allPoints, tol);

    return boost::make_tuple(resultPoint.x(), resultPoint.y());
  }

  // convert vertices to a boost polygon, all vertices must lie on z = 0 plane
  boost::optional<BoostPolygon> boostPolygonFromVertices(const std::vector<Point3d>& vertices, std::vector<Point3d>& allPoints, double tol)
  {
    if (vertices.size () < 3){
      return boost::none;
    }

    BoostPolygon polygon;
    for (const Point3d& vertex : vertices){

      // should all have zero z coordinate now
      double z = vertex.z();
      if (abs(z) > tol){
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
    if (!testArea || (*testArea < 0)){
      // DLM: we could offer to reverse these vertices here but that might not be the best idea
      return boost::none;
    }

    return polygon;
  }

  boost::optional<BoostPolygon> nonIntersectingBoostPolygonFromVertices(const std::vector<Point3d>& polygon, std::vector<Point3d>& allPoints, double tol)
  {
    boost::optional<BoostPolygon> result = boostPolygonFromVertices(polygon, allPoints, tol);
    if (!result){
      return boost::none;
    }
    // check if polygon overlaps itself
    try{
      boost::geometry::detail::overlay::has_self_intersections(*result);
    }catch(const boost::geometry::overlay_invalid_input_exception&){
      //LOG_FREE(Error, "utilities.geometry.nonIntersectingBoostPolygonFromVertices", "Self intersecting polygon");
      return boost::none;
    }
    return result;
  }

  // convert vertices to a boost ring, all vertices must lie on z = 0 plane
  boost::optional<BoostRing> boostRingFromVertices(const std::vector<Point3d>& vertices, std::vector<Point3d>& allPoints, double tol)
  {
    if (vertices.size () < 3){
      return boost::none;
    }

    BoostRing ring;
    for (const Point3d& vertex : vertices){

      // should all have zero z coordinate now
      double z = vertex.z();
      if (abs(z) > tol){
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
    if (!testArea || (*testArea < 0)){
      // DLM: we could offer to reverse these vertices here but that might not be the best idea
      return boost::none;
    }

    return ring;
  }

  boost::optional<BoostRing> nonIntersectingBoostRingFromVertices(const std::vector<Point3d>& polygon, std::vector<Point3d>& allPoints, double tol)
  {
    boost::optional<BoostRing> result = boostRingFromVertices(polygon, allPoints, tol);
    if (!result){
      return boost::none;
    }
    // check if polygon overlaps itself
    try{
      boost::geometry::detail::overlay::has_self_intersections(*result);
    }catch(const boost::geometry::overlay_invalid_input_exception&){
      //LOG_FREE(Error, "utilities.geometry.nonIntersectingBoostRingFromVertices", "Self intersecting polygon");
      return boost::none;
    }
    return result;
  }

  // convert a boost polygon to vertices
  std::vector<Point3d> verticesFromBoostPolygon(const BoostPolygon& polygon, std::vector<Point3d>& allPoints, double tol)
  {
    std::vector<Point3d> result;

    BoostRing outer = polygon.outer();
    if (outer.empty()){
      return result;
    }
    
    // add point for each vertex except final vertex
    for(unsigned i = 0; i < outer.size() - 1; ++i){
      Point3d point3d(outer[i].x(), outer[i].y(), 0.0);
      
      // try to combine points within tolerance
      Point3d resultPoint = getCombinedPoint(point3d, allPoints, tol);

      // don't keep repeated vertices
      if ((i > 0) && (result.back() == resultPoint)){
        continue;
      }
      result.push_back(resultPoint);
    }

    OS_ASSERT(polygon.inners().empty());

    result = removeCollinear(result);

    // don't keep repeated vertices
    if (result.front() == result.back()){
      result.pop_back();
    }

    if (result.size() < 3){
      return std::vector<Point3d>();
    }

    return result;
  }

  // struct used to sort polygons in descending order by area
  struct BoostPolygonAreaGreater{
    bool operator()(const BoostPolygon& left, const BoostPolygon& right){
      boost::optional<double> leftA = boost::geometry::area(left);
      if (!leftA){
        leftA = 0;
      }
      boost::optional<double> rightA = boost::geometry::area(right);
      if (!rightA){
        rightA = 0;
      }
      return (*leftA > *rightA);
    }
  };

  // Public functions
  
  IntersectionResult::IntersectionResult(const std::vector<Point3d>& polygon1, 
                                         const std::vector<Point3d>& polygon2, 
                                         const std::vector< std::vector<Point3d> >& newPolygons1, 
                                         const std::vector< std::vector<Point3d> >& newPolygons2)
    : m_polygon1(polygon1), m_polygon2(polygon2), m_newPolygons1(newPolygons1), m_newPolygons2(newPolygons2)
  {
  }

  std::vector<Point3d> IntersectionResult::polygon1() const
  {
    return m_polygon1;
  }

  std::vector<Point3d> IntersectionResult::polygon2() const
  {
    return m_polygon2;
  }

  std::vector< std::vector<Point3d> > IntersectionResult::newPolygons1() const
  {
    return m_newPolygons1;
  }

  std::vector< std::vector<Point3d> > IntersectionResult::newPolygons2() const
  {
    return m_newPolygons2;
  }
  
  std::vector<Point3d> removeSpikes(const std::vector<Point3d>& polygon, double tol)
  {
    // convert vertices to boost rings
    std::vector<Point3d> allPoints;
    
    boost::optional<BoostPolygon> boostPolygon = boostPolygonFromVertices(polygon, allPoints, tol);
    if (!boostPolygon){
      return std::vector<Point3d>();
    }

    BoostPolygon boostResult = removeSpikes(*boostPolygon);

    std::vector<Point3d> result = verticesFromBoostPolygon(boostResult, allPoints, tol);

    return result;
  }
  
  bool pointInPolygon(const Point3d& point, const std::vector<Point3d>& polygon, double tol)
  {
    // convert vertices to boost rings
    std::vector<Point3d> allPoints;
    
    boost::optional<BoostRing> boostPolygon = nonIntersectingBoostRingFromVertices(polygon, allPoints, tol);
    if (!boostPolygon){
      return false;
    }

    if (abs(point.z()) > tol){
      return false;
    }

    boost::tuple<double, double> p = boostPointFromPoint3d(point, allPoints, tol);
    BoostPoint boostPoint(p.get<0>(), p.get<1>());

    //boost::geometry::strategy::within::winding<BoostPoint> strategy;
    //boost::geometry::strategy::within::franklin<BoostPoint> strategy;
    //boost::geometry::strategy::within::crossings_multiply<BoostPoint> strategy;
    //bool result = boost::geometry::within(boostPoint, *boostPolygon, strategy);

    //bool result = boost::geometry::intersects(boostPoint, *boostPolygon);

    //bool result = boost::geometry::overlaps(boostPoint, *boostPolygon);

    double distance = boost::geometry::distance(boostPoint, *boostPolygon);
    bool result = (distance <= 0.0001);

    return result; 
      
  }

  boost::optional<std::vector<Point3d> > join(const std::vector<Point3d>& polygon1, const std::vector<Point3d>& polygon2, double tol)
  {
    // convert vertices to boost rings
    std::vector<Point3d> allPoints;
    
    boost::optional<BoostRing> boostPolygon1 = nonIntersectingBoostRingFromVertices(polygon1, allPoints, tol);
    if (!boostPolygon1){
      return boost::none;
    }

    boost::optional<BoostRing> boostPolygon2 = nonIntersectingBoostRingFromVertices(polygon2, allPoints, tol);
    if (!boostPolygon2){
      return boost::none;
    }

    // union the points in face coordinates, 
    std::vector<BoostPolygon> unionResult;
    try{
      boost::geometry::union_(*boostPolygon1, *boostPolygon2, unionResult);
    }catch(const boost::geometry::overlay_invalid_input_exception&){
      LOG_FREE(Error, "utilities.geometry.join", "overlay_invalid_input_exception");
      return boost::none;
    }

    unionResult = removeSpikes(unionResult);

    // should not be any holes, check for that below

    // check that union is ok
    if (unionResult.empty()){
      return boost::none;
    }else if (unionResult.size() > 1){
      return boost::none;
    }

    std::vector<Point3d> unionVertices = verticesFromBoostPolygon(unionResult[0], allPoints, tol);
    boost::optional<double> testArea = boost::geometry::area(unionResult[0]);
    if (!testArea || unionVertices.empty()){
      LOG_FREE(Info, "utilities.geometry.join", "Cannot compute area of union");
      return boost::none;
    }else if (*testArea < tol*tol){
      LOG_FREE(Info, "utilities.geometry.join", "Union has very small area of " << *testArea << " m^2");
      return boost::none;
    }
    try{
      boost::geometry::detail::overlay::has_self_intersections(unionResult[0]);
    }catch(const boost::geometry::overlay_invalid_input_exception&){
      LOG_FREE(Error, "utilities.geometry.join", "Union is self intersecting");
      return boost::none;
    }

    // check for holes
    if (!unionResult[0].inners().empty()){
      LOG_FREE(Error, "utilities.geometry.join", "Union has inner loops");
      return boost::none;
    };

    unionVertices = reorderULC(unionVertices);
    unionVertices = removeCollinear(unionVertices);

    return unionVertices;
  }

  std::vector<std::vector<Point3d> > joinAll(const std::vector<std::vector<Point3d> >& polygons, double tol)
  {
    std::vector<std::vector<Point3d> > result;

    unsigned N = polygons.size();
    if (N <= 1){
      return polygons;
    }

    // compute adjacency matrix
    Matrix A(N,N,0.0);
    for (unsigned i = 0; i < polygons.size(); ++i){
      A(i,i) = 1.0;
      for (unsigned j = i+1; j < polygons.size(); ++j){
        if (join(polygons[i], polygons[j], tol)){
          A(i,j) = 1.0;
          A(j,i) = 1.0;
        }
      }
    }

    

    std::vector<std::vector<unsigned> > connectedComponents = findConnectedComponents(A);
    for (const std::vector<unsigned>& component : connectedComponents){
      std::vector<Point3d> points;
      for (unsigned i : component){
        if (points.empty()){
          points = polygons[i];
        }else{
          boost::optional<std::vector<Point3d> > joined = join(points, polygons[i], tol);
          if (!joined){
            LOG_FREE(Error, "utilities.geometry.joinAll", "Expected polygons to join together");
          }else{
            points = *joined;
          }
        }
      }
      result.push_back(points);
    }

    return result;
  }

  boost::optional<IntersectionResult> intersect(const std::vector<Point3d>& polygon1, const std::vector<Point3d>& polygon2, double tol)
  {
    std::vector<Point3d> resultPolygon1;
    std::vector<Point3d> resultPolygon2;
    std::vector< std::vector<Point3d> > newPolygons1;
    std::vector< std::vector<Point3d> > newPolygons2;

    // convert vertices to boost rings
    std::vector<Point3d> allPoints;
    
    boost::optional<BoostRing> boostPolygon1 = nonIntersectingBoostRingFromVertices(polygon1, allPoints, tol);
    if (!boostPolygon1){
      return boost::none;
    }

    boost::optional<BoostRing> boostPolygon2 = nonIntersectingBoostRingFromVertices(polygon2, allPoints, tol);
    if (!boostPolygon2){
      return boost::none;
    }

    // intersect the points in face coordinates, 
    std::vector<BoostPolygon> intersectionResult;
    try{
      boost::geometry::intersection(*boostPolygon1, *boostPolygon2, intersectionResult);
    }catch(const boost::geometry::overlay_invalid_input_exception&){
      LOG_FREE(Error, "utilities.geometry.intersect", "overlay_invalid_input_exception");
      return boost::none;
    }

    // check if intersection is empty
    if (intersectionResult.empty()){
      //LOG_FREE(Info, "utilities.geometry.intersect", "Intersection is empty");
      return boost::none;
    }

    intersectionResult = removeSpikes(intersectionResult);
    intersectionResult = removeHoles(intersectionResult);

    // check for multiple intersections
    if (intersectionResult.size() > 1){
      LOG_FREE(Info, "utilities.geometry.intersect", "Intersection has " << intersectionResult.size() << " elements");
      std::sort(intersectionResult.begin(), intersectionResult.end(), BoostPolygonAreaGreater());
    }
    
    // check that largest intersection is ok
    std::vector<Point3d> intersectionVertices = verticesFromBoostPolygon(intersectionResult[0], allPoints, tol);
    boost::optional<double> testArea = boost::geometry::area(intersectionResult[0]);
    if (!testArea || intersectionVertices.empty()){
      LOG_FREE(Info, "utilities.geometry.intersect", "Cannot compute area of largest intersection");
      return boost::none;
    }else if (*testArea < tol*tol){
      LOG_FREE(Info, "utilities.geometry.intersect", "Largest intersection has very small area of " << *testArea << " m^2");
      return boost::none;
    }
    try{
      boost::geometry::detail::overlay::has_self_intersections(intersectionResult[0]);
    }catch(const boost::geometry::overlay_invalid_input_exception&){
      LOG_FREE(Error, "utilities.geometry.intersect", "Largest intersection is self intersecting");
      return boost::none;
    }
    if (!intersectionResult[0].inners().empty()){
      LOG_FREE(Error, "utilities.geometry.intersect", "Largest intersection has inner loops");
      return boost::none;
    };

    // intersections are the same
    resultPolygon1 = intersectionVertices;
    resultPolygon2 = intersectionVertices;

    // create new polygon for each remaining intersection
    for (unsigned i = 1; i < intersectionResult.size(); ++i){

      std::vector<Point3d> newPolygon = verticesFromBoostPolygon(intersectionResult[i], allPoints, tol);

      testArea = boost::geometry::area(intersectionResult[i]);
      if (!testArea || newPolygon.empty()){
        LOG_FREE(Info, "utilities.geometry.intersect", "Cannot compute area of intersection, result will not include this polygon, " << newPolygon);
        continue;
      }else if (*testArea < tol*tol){
        LOG_FREE(Info, "utilities.geometry.intersect", "Intersection has very small area of " << *testArea << " m^2, result will not include this polygon, " << newPolygon);
        continue;
      }
      try{
        boost::geometry::detail::overlay::has_self_intersections(intersectionResult[i]);
      }catch(const boost::geometry::overlay_invalid_input_exception&){
        LOG_FREE(Error, "utilities.geometry.intersect", "Intersection is self intersecting, result will not include this polygon, " << newPolygon);
        continue;
      }
      if (!intersectionResult[i].inners().empty()){
        LOG_FREE(Error, "utilities.geometry.intersect", "Intersection has inner loops, result will not include this polygon, " << newPolygon);
        continue;
      };

      newPolygons1.push_back(newPolygon);
      newPolygons2.push_back(newPolygon);
    }

    // polygon1 minus polygon2
    std::vector<BoostPolygon> differenceResult1;
    boost::geometry::difference(*boostPolygon1, *boostPolygon2, differenceResult1);
    differenceResult1 = removeSpikes(differenceResult1);
    differenceResult1 = removeHoles(differenceResult1);
    
    // create new polygon for each difference
    for (unsigned i = 0; i < differenceResult1.size(); ++i){

      std::vector<Point3d> newPolygon1 = verticesFromBoostPolygon(differenceResult1[i], allPoints, tol);

      testArea = boost::geometry::area(differenceResult1[i]);
      if (!testArea || newPolygon1.empty()){
        LOG_FREE(Info, "utilities.geometry.intersect", "Cannot compute area of face difference, result will not include this polygon, " << newPolygon1);
        continue;
      }else if (*testArea < tol*tol){
        LOG_FREE(Info, "utilities.geometry.intersect", "Face difference has very small area of " << *testArea << " m^2, result will not include this polygon, " << newPolygon1);
        continue;
      }
      try{
        boost::geometry::detail::overlay::has_self_intersections(differenceResult1[i]);
      }catch(const boost::geometry::overlay_invalid_input_exception&){
        LOG_FREE(Error, "utilities.geometry.intersect", "Face difference is self intersecting, result will not include this polygon, " << newPolygon1);
        continue;
      }

      newPolygons1.push_back(newPolygon1);
    }

    // polygon2 minus polygon1
    std::vector<BoostPolygon> differenceResult2;
    boost::geometry::difference(*boostPolygon2, *boostPolygon1, differenceResult2);
    differenceResult2 = removeSpikes(differenceResult2);
    differenceResult2 = removeHoles(differenceResult2);

    // create new polygon for each difference
    for (unsigned i = 0; i < differenceResult2.size(); ++i){

      std::vector<Point3d> newPolygon2 = verticesFromBoostPolygon(differenceResult2[i], allPoints, tol);

      testArea = boost::geometry::area(differenceResult2[i]);
      if (!testArea || newPolygon2.empty()){
        LOG_FREE(Info, "utilities.geometry.intersect", "Cannot compute area of face difference, result will not include this polygon, " << newPolygon2);
        continue;
      }else if (*testArea < tol*tol){
        LOG_FREE(Info, "utilities.geometry.intersect", "Face difference has very small area of " << *testArea << " m^2, result will not include this polygon, " << newPolygon2);
        continue;
      }
      try{
        boost::geometry::detail::overlay::has_self_intersections(differenceResult2[i]);
      }catch(const boost::geometry::overlay_invalid_input_exception&){
        LOG_FREE(Error, "utilities.geometry.intersect", "Face difference is self intersecting, result will not include this polygon, " << newPolygon2);
        continue;
      }

      newPolygons2.push_back(newPolygon2);
    }

    return IntersectionResult(resultPolygon1, resultPolygon2, newPolygons1, newPolygons2);
  }

} // openstudio
