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

#ifndef UTILITIES_GEOMETRY_VECTOR3D_HPP
#define UTILITIES_GEOMETRY_VECTOR3D_HPP

#include "../UtilitiesAPI.hpp"
#include "../data/Vector.hpp"
#include "../core/Logger.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace openstudio{

  class UTILITIES_API Vector3d{
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

    /// normalize to one
    bool normalize();

    /// get a vector which is the reverse of this
    Vector3d reverseVector() const;

    /// get length
    double length() const;

    /// set length
    bool setLength(double newLength);

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

} // openstudio

#endif //UTILITIES_GEOMETRY_VECTOR3D_HPP
