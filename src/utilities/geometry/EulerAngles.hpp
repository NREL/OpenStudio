/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_GEOMETRY_EULERANGLES_HPP
#define UTILITIES_GEOMETRY_EULERANGLES_HPP

#include "../UtilitiesAPI.hpp"
#include "../data/Vector.hpp"
#include "../core/Logger.hpp"

#include <vector>

#include <boost/optional.hpp>

namespace openstudio {

/** Euler angles specify an orientation in space, they characterize:
  * a rotation of psi radians about the x-axis followed by
  * a rotation of theta radians about the y-axis followed by
  * a rotation of phi radians about the z-axis.
  **/
class UTILITIES_API EulerAngles
{
 public:
  /// default constructor with no rotation
  EulerAngles();

  /// constructor with psi, theta, phi
  EulerAngles(double psi, double theta, double phi);

  /// copy constructor
  EulerAngles(const EulerAngles& other);

  /// get psi
  double psi() const;

  /// get theta
  double theta() const;

  /// get phi
  double phi() const;

 private:
  REGISTER_LOGGER("utilities.EulerAngles");
  Vector m_storage;
};

/// ostream operator
UTILITIES_API std::ostream& operator<<(std::ostream& os, const EulerAngles& angles);

// optional EulerAngles
using OptionalEulerAngles = boost::optional<EulerAngles>;

// vector of EulerAngles
using EulerAnglesVector = std::vector<EulerAngles>;

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_EULERANGLES_HPP
