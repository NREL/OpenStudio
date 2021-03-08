/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "Polygon.hpp"
#include "Vector3d.hpp"
#include "Geometry.hpp"

namespace openstudio {

Polygon3d::Polygon3d() {}

Polygon3d::Polygon3d(Point3dVector outerPath) {
  for (auto p : outerPath) {
    outerPath.push_back(p);
  }
}

Polygon3d::Polygon3d(Point3dVector outerPath, Point3dVectorVector innerPaths) {
  for (auto p : outerPath) {
    outerPath.push_back(p);
  }

  for (auto innerPath : innerPaths) {
    addHole(innerPath);
  }
}

void Polygon3d::addPoint(const Point3d& point) {
  outerPath.push_back(point);
}


void Polygon3d::setOuterPath(Point3dVector outerPath) {
  outerPath = outerPath;
}

Point3dVector Polygon3d::getOuterPath() const {
  return outerPath;
}

Point3dVectorVector Polygon3d::getInnerPaths() const {
  return innerPaths;
}

void Polygon3d::addHole(Point3dVector hole) {
  innerPaths.push_back(hole);
}

Vector3d Polygon3d::newellVector() {
  OptionalVector3d v = openstudio::getNewallVector(outerPath);

  if (v) {
    return v.get();
  }

  return Vector3d();
}

Vector3d Polygon3d::outwardNormal() {
  return openstudio::getOutwardNormal(outerPath).get();
}

double Polygon3d::grossArea() {

  boost::optional<double> area = openstudio::getArea(outerPath);
  if (area == boost::none)
    return 0;
  else
    return area.get();
  return 0;
}

double Polygon3d::netArea() {

  double netArea = grossArea();
  for (auto hole : innerPaths) {
    boost::optional<double> area = openstudio::getArea(hole);
    if (area != boost::none) netArea -= area.get();
  }

  return netArea;
}

double Polygon3d::perimeter() {

  double perimeter = 0;
  for (size_t i = 0; i < outerPath.size(); i++) {
    Point3d p1 = outerPath[i];
    Point3d p2 = outerPath[(i + 1) % outerPath.size()];
    perimeter += openstudio::getDistance(p1, p2);
  }

  return perimeter;
}

bool Polygon3d::isClockwise() {
  OptionalVector3d normal = getOutwardNormal(outerPath);
  if (normal == boost::none)
    return true;
  else
    return normal.get().z() > 0;
}

Point3d Polygon3d::centroid() {
  boost::optional p = openstudio::getCentroid(outerPath);
  if (p == boost::none)
    return Point3d();
  else
    return p.get();
}

//bool Polygon3d::PointInPolygon(Point3d testPoint) {
//  return true;
//}

//typedef boost::geometry::model::polygon<BoostPoint> BoostPolygon;
//typedef boost::geometry::model::d2::point_xy<double> BoostPoint;

// TODO: I want to inlude all the boolean joining etc code here

}  // namespace openstudio
