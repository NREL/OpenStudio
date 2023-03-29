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

#include "StandardShapes.hpp"

#include "Point3d.hpp"
#include "Vector3d.hpp"

#include <cmath>
#include <numbers>
#include <stdexcept>
#include <vector>

namespace openstudio {

std::vector<Point3d> convexRegularPolygon(const Point3d& center, size_t num_sides, double side_with) {
  if (num_sides < 3) {
    throw std::runtime_error("Polygon must have at least three sides.");
  }

  std::vector<Point3d> points;
  points.reserve(num_sides);
  const double angle = 2.0 * std::numbers::pi / num_sides;

  for (size_t i = 0; i < num_sides; i++) {
    double x = center.x() + side_with * std::cos(angle * i);
    double y = center.y() + side_with * std::sin(angle * i);
    double z = center.z();
    points.emplace_back(x, y, z);
  }

  return points;
}

std::vector<Point3d> hShapedPolygon(const Point3d& center, double total_length) {

  //   ▲
  //   │(1)    (9)
  // L o───┐   o───┐  ▲
  //   │   │   │   │  │  l2
  //   │   └───┘   │  │
  //   │     x  (5)│  ▼
  //   │   ┌───o   │
  //   │(2)│   │   │
  //   o───┴───┴───┴──►
  //   ◄──►         L
  //    l3

  const double l2 = total_length / 2.0;
  const double l6 = total_length / 6.0;

  const double x = center.x();
  const double y = center.y();
  const double z = center.z();

  // Counter clockwise order
  return {
    {x - l2, y + l2, z},  // Point 1 - Upper Left Corner
    {x - l2, y - l2, z},  // Point 2
    {x - l6, y - l2, z},  //
    {x - l6, y - l6, z},  //
    {x + l6, y - l6, z},  // Point 5
    {x + l6, y - l2, z},  //
    {x + l2, y - l2, z},  //
    {x + l2, y + l2, z},  //
    {x + l6, y + l2, z},  // Point 9
    {x + l6, y + l6, z},  //
    {x - l6, y + l6, z},  //
    {x - l6, y + l2, z},  //
  };
}

std::vector<Point3d> uShapedPolygon(const Point3d& center, double total_length) {

  //   ▲
  //   │(1)    (5)
  // L o───┐   o───┐  ▲
  //   │   │   │   │  │  l2
  //   │   └───┘   │  │
  //   │     x     │  ▼
  //   │           │
  //   │(2)        │
  //   o───────────┴──►
  //   ◄──►         L
  //    l3

  const double l2 = total_length / 2.0;
  const double l6 = total_length / 6.0;

  const double x = center.x();
  const double y = center.y();
  const double z = center.z();

  // Counter clockwise order
  return {
    {x - l2, y + l2, z},  // Point 1 - Upper Left Corner
    {x - l2, y - l2, z},  // Point 2
    {x + l2, y - l2, z},  //
    {x + l2, y + l2, z},  //
    {x + l6, y + l2, z},  // Point 5
    {x + l6, y + l6, z},  //
    {x - l6, y + l6, z},  //
    {x - l6, y + l2, z},  //
  };
}

std::vector<Point3d> tShapedPolygon(const Point3d& center, double total_length) {

  //   ▲
  //   │(1)
  // L o───────────┐  ▲
  //   │ (2)       │  │  l2
  //   o───┐   ┌───┘  │
  //   │   │ x │      ▼
  //   │   │   │
  //   │   │   │ (5)
  //   └───┴───o───────►
  //   ◄──►         L
  //    l3

  const double l2 = total_length / 2.0;
  const double l6 = total_length / 6.0;

  const double x = center.x();
  const double y = center.y();
  const double z = center.z();

  // Counter clockwise order
  return {
    {x - l2, y + l2, z},  // Point 1 - Upper Left Corner
    {x - l2, y + l6, z},  // Point 2
    {x - l6, y + l6, z},  //
    {x - l6, y - l2, z},  //
    {x + l6, y - l2, z},  // Point 5
    {x + l6, y + l6, z},  //
    {x + l2, y + l6, z},  //
    {x + l2, y + l2, z},  //
  };
}

std::vector<Point3d> lShapedPolygon(const Point3d& center, double total_length) {
  //   ▲
  //   │(1)
  // L o─────┐        ▲
  //   │     │        │
  //   │     │ (5)    │ l2
  //   │     x─────┐  ▼
  //   │           │
  //   │           │
  //   o───────────┴──►
  //               L

  const double l2 = total_length / 2.0;

  const double x = center.x();
  const double y = center.y();
  const double z = center.z();

  // Counter clockwise order
  return {
    {x - l2, y + l2, z},  // Point 1 - Upper Left Corner
    {x - l2, y - l2, z},  // Point 2
    {x + l2, y - l2, z},  //
    {x + l2, y, z},       //
    {x, y, z},            // Point 5
    {x, y + l2, z},       //
  };
}

}  // namespace openstudio
