/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
