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

#ifndef UTILITIES_GEOMETRY_POINT3D_HPP
#define UTILITIES_GEOMETRY_POINT3D_HPP

#include "../UtilitiesAPI.hpp"
#include "../data/Vector.hpp"
#include "../core/Logger.hpp"

#include <vector>
#include <boost/optional.hpp>

namespace openstudio{

  // forward declaration
  class Vector3d;

  class UTILITIES_API Point3d{
  public:

    /// default constructor creates point at 0, 0, 0
    Point3d();

    /// constructor with x, y, z
    Point3d(double x, double y, double z);

    /// copy constructor
    Point3d(const Point3d& other);

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

  private:

    REGISTER_LOGGER("utilities.Point3d");
    Vector m_storage;

  };

  /// ostream operator
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const Point3d& point);

  /// ostream operator
  UTILITIES_API std::ostream& operator<<(std::ostream& os, const std::vector<Point3d>& pointVector);

  // optional Point3d
  typedef boost::optional<Point3d> OptionalPoint3d;

  // vector of Point3d
  typedef std::vector<Point3d> Point3dVector;

} // openstudio

#endif //UTILITIES_GEOMETRY_POINT3D_HPP
