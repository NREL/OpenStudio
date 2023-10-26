/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_GEOMETRY_POINT3D_HPP
#define UTILITIES_GEOMETRY_POINT3D_HPP

#include "../UtilitiesAPI.hpp"
#include "../data/Vector.hpp"
#include "../core/Logger.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace openstudio {

// forward declaration
class Vector3d;

class UTILITIES_API Point3d
{
 public:
  /// default constructor creates point at 0, 0, 0
  Point3d();

  /// constructor with x, y, z
  Point3d(double x, double y, double z);

  // Copy and move operators are implicitly declared
  // Point3d(const Point3d& other) = default;
  // Point3d(Point3d&& other) = default;
  // Point3d& operator=(const Point3d&) = default;
  // Point3d& operator=(Point3d&&) = default;
  // ~Point3d() noexcept = default;

  /// get x
  double x() const;

  /// get y
  double y() const;

  /// get z
  double z() const;

  /// point plus a vector is a new point
  Point3d operator+(const Vector3d& vec) const;

  /// point plus a vector is a new point
  Point3d& operator+=(const Vector3d& vec);

  /// point minus another point is a vector
  Vector3d operator-(const Point3d& other) const;

  /// check equality
  bool operator==(const Point3d& other) const;

  /// check inequality
  bool operator!=(const Point3d& other) const;

 private:
  REGISTER_LOGGER("utilities.Point3d");
  Vector m_storage;
};

/// ostream operator
UTILITIES_API std::ostream& operator<<(std::ostream& os, const Point3d& point);

/// ostream operator
UTILITIES_API std::ostream& operator<<(std::ostream& os, const std::vector<Point3d>& pointVector);

/// ostream operator
UTILITIES_API std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<Point3d>>& pointVectorVector);

// optional Point3d
using OptionalPoint3d = boost::optional<Point3d>;

// vector of Point3d
using Point3dVector = std::vector<Point3d>;

// vector of Point3dVector
using Point3dVectorVector = std::vector<Point3dVector>;

}  // namespace openstudio

#endif  //UTILITIES_GEOMETRY_POINT3D_HPP
