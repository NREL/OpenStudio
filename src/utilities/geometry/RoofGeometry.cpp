/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "RoofGeometry.hpp"
#include "RoofGeometry_Details.hpp"

#include "Geometry.hpp"
#include "Vector3d.hpp"
#include <boost/optional.hpp>
#include <boost/math/constants/constants.hpp>

namespace openstudio {

/// Generate shed roof polygons
std::vector<std::vector<Point3d>> generateShedRoof(std::vector<Point3d>& polygon, double roofPitchDegrees, double directionDegrees) {
  std::vector<std::vector<Point3d>> surfaces;
  try {
    surfaces = doShedRoof(polygon, roofPitchDegrees, directionDegrees);
  } catch (...) {
    return surfaces;
  }

  return surfaces;
}

/// Generate gable roof polygons
std::vector<std::vector<Point3d>> generateGableRoof(std::vector<Point3d>& polygon, double roofPitchDegrees) {
  std::vector<std::vector<Point3d>> surfaces;
  try {
    surfaces = doStraightSkeleton(polygon, roofPitchDegrees);
    if (surfaces.empty()) {
      return surfaces;
    }

    applyGables(surfaces);

  } catch (...) {
    return surfaces;
  }

  return surfaces;
}

/// Generate hip roof polygons
std::vector<std::vector<Point3d>> generateHipRoof(std::vector<Point3d>& polygon, double roofPitchDegrees) {
  std::vector<std::vector<Point3d>> surfaces;
  try {
    surfaces = doStraightSkeleton(polygon, roofPitchDegrees);
  } catch (...) {
    return surfaces;
  }

  return surfaces;
}

}  // namespace openstudio
