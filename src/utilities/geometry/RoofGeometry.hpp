/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_GEOMETRY_ROOFGEOMETRY_HPP
#define UTILITIES_GEOMETRY_ROOFGEOMETRY_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Logger.hpp"
#include "Point3d.hpp"

namespace openstudio {

REGISTER_LOGGER("utilities.RoofGeometry");

/// Generate shed roof polygons
UTILITIES_API std::vector<std::vector<Point3d>> generateShedRoof(std::vector<Point3d>& polygon, double roofPitchDegrees, double directionDegrees);

/// Generate gable roof polygons
UTILITIES_API std::vector<std::vector<Point3d>> generateGableRoof(std::vector<Point3d>& polygon, double roofPitchDegrees);

/// Generate hip roof polygons
UTILITIES_API std::vector<std::vector<Point3d>> generateHipRoof(std::vector<Point3d>& polygon, double roofPitchDegrees);

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_ROOFGEOMETRY_HPP
