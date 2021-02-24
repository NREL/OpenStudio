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
  Polygon3d();
  Polygon3d(Point3dVector outerPth);

  void setOuterPath(Point3dVector outerPth);
  Point3dVector getOuterPath() const;
  Point3dVectorVector getInnerPaths() const;

  void addPoint(Point3d& point);
  void addHole(Point3dVector hole);
  Vector3d newellVector();
  Vector3d outwardNormal();
  /// <summary>
  /// Calculates the gross area of the polygon (area excluding holes)
  /// </summary>
  /// <returns></returns>
  double grossArea();
  /// <summary>
  /// Calculates the net area of the polygon (gross area - area of holes)
  /// </summary>
  /// <returns></returns>
  double netArea();
  /// <summary>
  /// Gets the perimeter of the outer path of the polygon
  /// </summary>
  /// <returns></returns>
  double getPerimeter();
  /// <summary>
  /// 
  /// </summary>
  /// <returns></returns>
  bool getIsClockwise();
  /// <summary>
  /// 
  /// </summary>
  /// <returns></returns>
  Point3d getCentroid();
  /// <summary>
  /// 
  /// </summary>
  /// <param name="testPoint"></param>
  /// <returns></returns>
  //bool PointInPolygon(Point3d testPoint);
 private:
  Point3dVector points;
  Point3dVectorVector innerPaths;
};
}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_POLYGON_HPP
