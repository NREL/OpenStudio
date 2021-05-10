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

#ifndef UTILITIES_GEOMETRY_VECTOR3D_HPP
#define UTILITIES_GEOMETRY_VECTOR3D_HPP

#include "../UtilitiesAPI.hpp"
#include "../data/Vector.hpp"
#include "../core/Logger.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace openstudio {

class UTILITIES_API Vector3d
{
 public:
  /// default constructor creates vector with 0, 0, 0
  Vector3d();

  /// constructor with x, y, z
  Vector3d(double x, double y, double z);

  /// copy constructor
  Vector3d(const Vector3d& other);

  /// get x
  double x() const;

  /// get y
  double y() const;

  /// get z
  double z() const;

  /// addition
  Vector3d operator+(const Vector3d& other) const;

  /// addition
  Vector3d& operator+=(const Vector3d& other);

  /// subtraction
  Vector3d operator-(const Vector3d& other) const;

  /// subtraction
  Vector3d& operator-=(const Vector3d& other);

  /// check equality
  bool operator==(const Vector3d& other) const;

  /// check inequality
  bool operator!=(const Vector3d& other) const;

  /// normalize to one
  bool normalize();

  /// get a vector which is the reverse of this
  Vector3d reverseVector() const;

  /// get length
  double length() const;

  /// set length
  bool setLength(double newLength);

  /// orthogonal right
  Vector3d orthogonalRight() const;

  /// orthogonal left
  Vector3d orthogonalLeft() const;

  /// dot product with another Vector3d
  double dot(const Vector3d& other) const;

  /// cross product with another Vector3d
  Vector3d cross(const Vector3d& other) const;

  /// get the Vector directly
  Vector vector() const;

 private:
  REGISTER_LOGGER("utilities.Vector3d");

  Vector m_storage;
};

/// ostream operator
UTILITIES_API std::ostream& operator<<(std::ostream& os, const Vector3d& vec);

/// ostream operator
UTILITIES_API std::ostream& operator<<(std::ostream& os, const std::vector<Vector3d>& vecVector);

/// negation
UTILITIES_API Vector3d operator-(const Vector3d& vec);

/// multiplication by a scalar
UTILITIES_API Vector3d operator*(double mult, const Vector3d& vec);

// optional Vector3d
typedef boost::optional<Vector3d> OptionalVector3d;

// vector of Vector3d
typedef std::vector<Vector3d> Vector3dVector;

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_VECTOR3D_HPP
