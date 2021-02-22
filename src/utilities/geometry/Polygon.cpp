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
    for (auto p : outerPath)
      points.push_back(p);
  }

	/// <summary>
	/// Adds a point to the polygon perimeter
	/// </summary>
	/// <param name="point"></param>
	void Polygon3d::AddPoint(Point3d& point) {
		points.push_back(point);
	}

	/// <summary>
	/// Sets the perimeter of the polygonb
	/// </summary>
	/// <param name="perimeter"></param>
	void Polygon3d::SetOuterPath(Point3dVector outerPth) {
		points = outerPth;
	}

	Point3dVector Polygon3d::GetOuterPath() const {
		return points;
	}

	Point3dVectorVector Polygon3d::GetHoles() const {
		return holes;
	}

  /// <summary>
/// Adds a hole to the polygon
/// </summary>
/// <param name="hole"></param>
void Polygon3d::AddHole(Point3dVector hole) {
  holes.push_back(hole);
}

Vector3d Polygon3d::NewellVector() {
  OptionalVector3d v = openstudio::getNewallVector(points);

  if (v) {
    return v.get();
  }

  return Vector3d();
}

Vector3d Polygon3d::OutwardNormal() {
	return openstudio::getOutwardNormal(points).get();
}

double Polygon3d::GrossArea() {

  boost::optional<double> area = openstudio::getArea(points);
  if (area == boost::none)
    return 0;
  else
    return area.get();
  return 0;
}

double Polygon3d::NetArea() {

  double netArea = GrossArea();
  for (auto hole : holes) {
    boost::optional<double> area = openstudio::getArea(hole);
    if (area != boost::none) netArea -= area.get();
  }

  return netArea;
}

double Polygon3d::GetPerimeter() {

  double perimeter = 0;
  for (long i = 0; i < points.size(); i++) {
    Point3d p1 = points[i];
    Point3d p2 = points[(i + 1) % points.size()];
    perimeter += openstudio::getDistance(p1, p2);
  }

  return perimeter;
}

bool Polygon3d::GetIsClockwise() {
  return true;
}

Point3d Polygon3d::Centroid() {
  //boost::optional p = openstudio::getCentroid(points);
  //if (p == boost::none)
  return Point3d();
  //else
  //  return p.get();
}

//bool Polygon3d::PointInPolygon(Point3d testPoint) {
//  return true;
//}

//typedef boost::geometry::model::polygon<BoostPoint> BoostPolygon;
//typedef boost::geometry::model::d2::point_xy<double> BoostPoint;

// TODO: I want to inlude all the boolean joining etc code here

}  // namespace openstudio