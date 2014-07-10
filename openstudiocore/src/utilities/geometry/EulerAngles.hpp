/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef UTILITIES_GEOMETRY_EULERANGLES_HPP
#define UTILITIES_GEOMETRY_EULERANGLES_HPP

#include "../UtilitiesAPI.hpp"
#include "../data/Vector.hpp"
#include "../core/Logger.hpp"

#include <vector>

namespace openstudio{

  /** Euler angles specify an orientation in space, they characterize:
  * a rotation of psi radians about the x-axis followed by
  * a rotation of theta radians about the y-axis followed by
  * a rotation of phi radians about the z-axis.
  **/
  class UTILITIES_API EulerAngles{
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
  typedef boost::optional<EulerAngles> OptionalEulerAngles;

  // vector of EulerAngles
  typedef std::vector<EulerAngles> EulerAnglesVector;

} // openstudio

#endif //UTILITIES_GEOMETRY_EULERANGLES_HPP
