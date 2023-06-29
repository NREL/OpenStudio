/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  // Copy and move operators are implicitly declared  (Rule of 1)
  // Vector3d(const Vector3d& other) = default;
  // Vector3d(Vector3d&& other) = default;
  // Vector3d& operator=(const Vector3d&) = default;
  // Vector3d& operator=(Vector3d&&) = default;
  // ~Vector3d() noexcept = default;

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
using OptionalVector3d = boost::optional<Vector3d>;

// vector of Vector3d
using Vector3dVector = std::vector<Vector3d>;

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_VECTOR3D_HPP
