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

#ifndef UTILITIES_GEOMETRY_INTERSECTION_HPP
#define UTILITIES_GEOMETRY_INTERSECTION_HPP

#include "../UtilitiesAPI.hpp"

#include "Point3d.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace openstudio{

  /** IntersectionResult contains detailed information about an intersection. */
  class UTILITIES_API IntersectionResult {
  public:
    IntersectionResult(const std::vector<Point3d>& polygon1, 
                       const std::vector<Point3d>& polygon2, 
                       const std::vector< std::vector<Point3d> >& newPolygons1, 
                       const std::vector< std::vector<Point3d> >& newPolygons2); 

    // vertices of first polygon after intersection
    std::vector<Point3d> polygon1() const;

    // vertices of second polygon after intersection
    std::vector<Point3d> polygon2() const;

    // new polygons generated from the first surface
    std::vector< std::vector<Point3d> > newPolygons1() const;

    // new polygons generated from the second surface
    std::vector< std::vector<Point3d> > newPolygons2() const;

  private:
    std::vector<Point3d> m_polygon1;
    std::vector<Point3d> m_polygon2;
    std::vector< std::vector<Point3d> > m_newPolygons1;
    std::vector< std::vector<Point3d> > m_newPolygons2;
  };

  /// removes spikes from a polygon, requires that all vertices are in counter clockwise order on the z = 0 plane (e.g. in face coordinates) 
  UTILITIES_API std::vector<Point3d> removeSpikes(const std::vector<Point3d>& polygon, double tol);
  
  /// returns true if point is inside polygon, requires that all vertices are in counter clockwise order on the z = 0 plane (e.g. in face coordinates) 
  UTILITIES_API bool pointInPolygon(const Point3d& point, const std::vector<Point3d>& polygon, double tol);
  
  /// compute the union of two overlapping polygons, requires that all vertices are in counter clockwise order on the z = 0 plane (e.g. in face coordinates) 
  UTILITIES_API boost::optional<std::vector<Point3d> > join(const std::vector<Point3d>& polygon1, const std::vector<Point3d>& polygon2, double tol);
  
  /// compute the union of many polygons, requires that all vertices are in counter clockwise order on the z = 0 plane (e.g. in face coordinates) 
  UTILITIES_API std::vector<std::vector<Point3d> > joinAll(const std::vector<std::vector<Point3d> >& polygons, double tol);

  /// intersect two polygons, requires that all vertices are in counter clockwise order on the z = 0 plane (e.g. in face coordinates) 
  UTILITIES_API boost::optional<IntersectionResult> intersect(const std::vector<Point3d>& polygon1, const std::vector<Point3d>& polygon2, double tol);

} // openstudio

#endif //UTILITIES_GEOMETRY_INTERSECTION_HPP
