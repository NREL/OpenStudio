/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef UTILITIES_GEOMETRY_INTERSECTION_HPP
#define UTILITIES_GEOMETRY_INTERSECTION_HPP

#include "../UtilitiesAPI.hpp"

#include "Point3d.hpp"
#include "Polygon3d.hpp"
#include <vector>
#include <boost/optional.hpp>

namespace openstudio {

/** IntersectionResult contains detailed information about an intersection. */
class UTILITIES_API IntersectionResult
{
 public:
  IntersectionResult(const std::vector<Point3d>& polygon1, const std::vector<Point3d>& polygon2,
                     const std::vector<std::vector<Point3d>>& newPolygons1, const std::vector<std::vector<Point3d>>& newPolygons2);

  // vertices of first polygon after intersection
  std::vector<Point3d> polygon1() const;

  // vertices of second polygon after intersection
  std::vector<Point3d> polygon2() const;

  // new polygons generated from the first surface
  std::vector<std::vector<Point3d>> newPolygons1() const;

  // new polygons generated from the second surface
  std::vector<std::vector<Point3d>> newPolygons2() const;

  // total area from the first set of polygons
  double area1() const;

  // total area from the second set of polygons
  double area2() const;

 private:
  std::vector<Point3d> m_polygon1;
  std::vector<Point3d> m_polygon2;
  std::vector<std::vector<Point3d>> m_newPolygons1;
  std::vector<std::vector<Point3d>> m_newPolygons2;
};

/// removes spikes from a polygon, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
UTILITIES_API std::vector<Point3d> removeSpikes(const std::vector<Point3d>& polygon, double tol);

/// returns true if point is inside polygon, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
UTILITIES_API bool pointInPolygon(const Point3d& point, const std::vector<Point3d>& polygon, double tol);

/// compute the union of two overlapping polygons, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
UTILITIES_API boost::optional<std::vector<Point3d>> join(const std::vector<Point3d>& polygon1, const std::vector<Point3d>& polygon2, double tol);

UTILITIES_API boost::optional<Polygon3d> join(const Polygon3d& polygon1, const Polygon3d& polygon2);

/// compute the union of many polygons, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
UTILITIES_API std::vector<std::vector<Point3d>> joinAll(const std::vector<std::vector<Point3d>>& polygons, double tol);

/// compute the union of many polygons, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
UTILITIES_API std::vector<Polygon3d> joinAll(const std::vector<Polygon3d>& polygons, double tol);

/// compute the union of many polygons, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
UTILITIES_API std::vector<Polygon3d> joinAllPolygons(const std::vector<std::vector<Point3d>>& polygons, double tol);

UTILITIES_API std::vector<std::vector<Point3d>> joinAllWithBuffer(const std::vector<std::vector<Point3d>>& polygons, double offset, double tol);

UTILITIES_API boost::optional<std::vector<Point3d>> buffer(const std::vector<Point3d>& polygon1, double amount, double tol);
UTILITIES_API boost::optional<std::vector<std::vector<Point3d>>> buffer(const std::vector<std::vector<Point3d>>& polygons, double amount, double tol);

/// compute the union of many polygons using boost::buffer
UTILITIES_API std::vector<Polygon3d> bufferAll(const std::vector<Polygon3d>& polygons, double tol);

/// intersect two polygons, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
UTILITIES_API boost::optional<IntersectionResult> intersect(const std::vector<Point3d>& polygon1, const std::vector<Point3d>& polygon2, double tol);

/// subtract all holes from polygon, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
UTILITIES_API std::vector<std::vector<Point3d>> subtract(const std::vector<Point3d>& polygon, const std::vector<std::vector<Point3d>>& holes,
                                                         double tol);

/// returns true polygon intersects iteself, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
/// returns false if polygon has less than three vertices
UTILITIES_API bool selfIntersects(const std::vector<Point3d>& polygon, double tol);

/// returns true if polygon1 intersects polygon2, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
/// returns false if either polygon has less than three vertices
UTILITIES_API bool intersects(const std::vector<Point3d>& polygon1, const std::vector<Point3d>& polygon2, double tol);

/// returns true if geometry1 is completely within polygon2, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates but reversed)
/// geometry1 can be a point or a polygon
/// currently only tests that all points of geometry1 are within polygon2, better support when upgrade to boost 1.57
UTILITIES_API bool within(const Point3d& point1, const std::vector<Point3d>& polygon2, double tol);
UTILITIES_API bool within(const std::vector<Point3d>& geometry1, const std::vector<Point3d>& polygon2, double tol);

/// simplify a list of vertices, requires that all vertices are in clockwise order on the z = 0 plane (i.e. in face coordinates)
UTILITIES_API std::vector<Point3d> simplify(const std::vector<Point3d>& vertices, bool removeCollinear, double tol);

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_INTERSECTION_HPP
