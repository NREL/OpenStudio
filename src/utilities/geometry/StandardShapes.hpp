/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_GEOMETRY_STANDARDSHAPES_HPP
#define UTILITIES_GEOMETRY_STANDARDSHAPES_HPP

#include "../UtilitiesAPI.hpp"
#include "Point3d.hpp"

#include <vector>

namespace openstudio {

// Convenience functions for testing. All them returns point in Counterclockwise order, so if creating a floor, reverse the vertices

UTILITIES_API std::vector<Point3d> convexRegularPolygon(const Point3d& center, size_t num_sides, double side_with = 1.0);

UTILITIES_API std::vector<Point3d> hShapedPolygon(const Point3d& center, double total_length = 10.0);

UTILITIES_API std::vector<Point3d> uShapedPolygon(const Point3d& center, double total_length = 10.0);

UTILITIES_API std::vector<Point3d> tShapedPolygon(const Point3d& center, double total_length = 10.0);

UTILITIES_API std::vector<Point3d> lShapedPolygon(const Point3d& center, double total_length = 10.0);

UTILITIES_API std::vector<Point3d> squaredPolygon(const Point3d& center, double side_with = 10.0);

UTILITIES_API std::vector<Point3d> rectangularPolygon(const Point3d& center, double length_x = 20.0, double length_y = 10.0);

}  // namespace openstudio

#endif  // UTILITIES_GEOMETRY_STANDARDSHAPES_HPP
