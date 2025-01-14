/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "StandardShapes.hpp"

#include "Point3d.hpp"
#include "Vector3d.hpp"

#include <boost/math/constants/constants.hpp>

#include <cmath>
#include <stdexcept>
#include <vector>

// GCC <10 doesn't have numbers..
// #include <numbers> / PI
// static constexpr double PI = td::numbers::pi;
static constexpr double PI = boost::math::constants::pi<double>();

namespace openstudio {

std::vector<Point3d> convexRegularPolygon(const Point3d& center, size_t num_sides, double side_with) {
  if (num_sides < 3) {
    throw std::runtime_error("Polygon must have at least three sides.");
  }

  std::vector<Point3d> points;
  points.reserve(num_sides);
  const double angle = 2.0 * PI / num_sides;

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

std::vector<Point3d> squaredPolygon(const Point3d& center, double side_with) {

  //   ▲
  //   │(1)
  // L o───────────┐  ▲
  //   │           │  l2
  //   │           │  │
  //   │     x     │  ▼
  //   │           │
  //   │(2)        │
  //   o───────────┴──►
  //               L

  const double l2 = side_with / 2.0;

  const double x = center.x();
  const double y = center.y();
  const double z = center.z();

  // Counter clockwise order
  return {
    {x - l2, y + l2, z},  // Point 1 - Upper Left Corner
    {x - l2, y - l2, z},  // Point 2
    {x + l2, y - l2, z},  //
    {x + l2, y + l2, z},  //
  };
}

std::vector<Point3d> rectangularPolygon(const Point3d& center, double length_x, double length_y) {

  //   ▲
  //   │(1)
  //L_yo───────────┐  ▲
  //   │           │  ly2
  //   │           │  │
  //   │     x     │  ▼
  //   │           │
  //   │(2)        │
  //   o───────────┴──►
  //               L_x

  const double lx2 = length_x / 2.0;
  const double ly2 = length_y / 2.0;

  const double x = center.x();
  const double y = center.y();
  const double z = center.z();

  // Counter clockwise order
  return {
    {x - lx2, y + ly2, z},  // Point 1 - Upper Left Corner
    {x - lx2, y - ly2, z},  // Point 2
    {x + lx2, y - ly2, z},  //
    {x + lx2, y + ly2, z},  //
  };
}

}  // namespace openstudio
