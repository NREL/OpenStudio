/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
typedef boost::optional<EulerAngles> OptionalEulerAngles;

// vector of EulerAngles
typedef std::vector<EulerAngles> EulerAnglesVector;

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_EULERANGLES_HPP
