/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "Polygon3d.hpp"
#include "Vector3d.hpp"
#include "Geometry.hpp"
#include "Intersection.hpp"
#include <utilities/geometry/Transformation.hpp>

namespace openstudio {

Polygon3d::Polygon3d() = default;

Polygon3d::Polygon3d(const Point3dVector& outerPath) {
  for (const auto& p : outerPath) {
    m_outerPath.emplace_back(p);
  }
}

Polygon3d::Polygon3d(const Point3dVector& outerPath, const Point3dVectorVector& innerPaths) {
  for (const auto& p : outerPath) {
    m_outerPath.emplace_back(p);
  }

  for (const auto& innerPath : innerPaths) {
    addHole(innerPath);
  }
}

/// Adds a point to the polygon perimeter
void Polygon3d::addPoint(const Point3d& point) {
  m_outerPath.emplace_back(point);
}

/// Sets the perimeter of the polygon
void Polygon3d::setOuterPath(const Point3dVector& outerPath) {
  m_outerPath = outerPath;
}

Point3dVector Polygon3d::getOuterPath() const {
  return m_outerPath;
}

Point3dVectorVector Polygon3d::getInnerPaths() const {
  return m_innerPaths;
}

void Polygon3d::addHole(const Point3dVector& hole) {
  m_innerPaths.emplace_back(hole);
}

Vector3d Polygon3d::newellVector() const {
  if (OptionalVector3d v = openstudio::getNewellVector(m_outerPath)) {
    return v.get();
  }

  return {};
}

Vector3d Polygon3d::outwardNormal() const {
  return openstudio::getOutwardNormal(m_outerPath).get();
}

double Polygon3d::grossArea() const {

  if (boost::optional<double> area = openstudio::getArea(m_outerPath)) {
    return area.get();
  }

  return 0;
}

double Polygon3d::netArea() const {

  double netArea = grossArea();
  for (const auto& hole : m_innerPaths) {
    if (boost::optional<double> area = openstudio::getArea(hole)) {
      netArea -= area.get();
    }
  }

  return netArea;
}

double Polygon3d::perimeter() const {

  double perimeter = 0;
  for (size_t i = 0; i < m_outerPath.size(); i++) {
    Point3d p1 = m_outerPath[i];
    Point3d p2 = m_outerPath[(i + 1) % m_outerPath.size()];
    perimeter += openstudio::getDistance(p1, p2);
  }

  return perimeter;
}

bool Polygon3d::isClockwise() const {
  if (OptionalVector3d normal = getOutwardNormal(m_outerPath)) {
    return normal.get().z() > 0;
  }

  return true;
}

Point3d Polygon3d::centroid() const {
  if (auto p = openstudio::getCentroid(m_outerPath)) {
    return p.get();
  }

  return {};
}

bool Polygon3d::pointInPolygon(const Point3d& point, double tol) const {
  bool inside = false;
  if (openstudio::pointInPolygon(point, m_outerPath, tol)) {
    inside = true;
  } else {
    for (const auto& innerPath : m_innerPaths) {
      if (openstudio::pointInPolygon(point, innerPath, tol)) {
        inside = true;
        break;
      }
    }
  }
  return inside;
}

bool Polygon3d::within(const Point3d& point, double tol) const {
  bool inside = false;
  if (openstudio::within(point, m_outerPath, tol)) {
    inside = true;
    for (const auto& innerPath : m_innerPaths) {
      if (openstudio::within(point, innerPath, tol)) {
        inside = false;
        break;
      }
    }
  }
  return inside;
}

bool Polygon3d::inside(const Point3d& point, double tol) const {
  return pointInPolygon(point, tol) || within(point, tol);
}

size_t Polygon3d::getSize() const {
  return m_outerPath.size();
}

Point3dVector Polygon3d::getEdge(size_t i) const {
  Point3d p1 = m_outerPath[i];
  Point3d p2 = m_outerPath[(i + 1) % m_outerPath.size()];
  return Point3dVector{p1, p2};
}

std::vector<Point3dVector> Polygon3d::overlap(const Point3dVector& line) const {
  Point3dVectorVector result;

  Point3d sp = line[0];
  Vector3d v = line[1] - line[0];
  double rad = atan2(v.y(), v.x()) - atan2(0, 1);
  // Make a transform so that the line being compared starts at 0 and is x = length
  Transformation t = Transformation::translation(Vector3d(sp.x(), sp.y(), 0)) * Transformation::rotation(Vector3d(0, 0, 1), rad);
  Transformation ti = t.inverse();
  Point3dVector lbase = ti * line;

  // Do a little swaperoo if the transformed line's arent along the +ve x axis
  if (lbase[1].x() < lbase[0].x()) {
    std::reverse(lbase.begin(), lbase.end());
  }

  for (size_t i = 0; i < m_outerPath.size(); i++) {
    Point3d pp1 = m_outerPath[i];
    Point3d pp2 = m_outerPath[(i + 1) % m_outerPath.size()];

    Point3d p1 = ti * m_outerPath[i];
    Point3d p2 = ti * m_outerPath[(i + 1) % m_outerPath.size()];

    // The two line segments must be parallel and coincident within tolerance
    if (std::abs(p1.y()) > 0.01 || std::abs(p2.y()) > 0.01) {
      continue;
    }

    // Do a little swaperoo if the transformed line's arent along the +ve x axis
    Point3dVector edge;
    if (p2.x() > p1.x()) {
      edge.push_back(p1);
      edge.push_back(p2);
    } else {
      edge.push_back(p2);
      edge.push_back(p1);
    }

    // Now we have two lines in the +ve x direction that are coincident and parallel but do they overlap?
    //if (line1.ep.X - line2.sp.X >= 0 && line2.ep.X - line1.sp.X >= 0) {
    if ((lbase[1].x() - edge[0].x() >= 0) && (edge[1].x() - lbase[0].x() >= 0)) {
      Point3d overlap1 = Point3d(std::max(edge[0].x(), lbase[0].x()), 0, 0);
      Point3d overlap2 = Point3d(std::min(edge[1].x(), lbase[1].x()), 0, 0);
      if (getDistance(overlap1, overlap2) > 0.01) {
        Point3dVector overlap;
        overlap.push_back(t * overlap1);
        overlap.push_back(t * overlap2);
        result.push_back(overlap);
      }
    }
  }
  return result;
}

}  // namespace openstudio
