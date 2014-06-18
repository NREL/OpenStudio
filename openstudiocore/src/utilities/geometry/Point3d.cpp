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

#include "Point3d.hpp"
#include "Vector3d.hpp"

namespace openstudio{

  /// default constructor creates point at 0, 0, 0
  Point3d::Point3d()
    : m_storage(3, 0.0)
  {}

  /// constructor with x, y, z
  Point3d::Point3d(double x, double y, double z)
    : m_storage(3)
  {
    m_storage[0] = x;
    m_storage[1] = y;
    m_storage[2] = z;
  }

  /// copy constructor
  Point3d::Point3d(const Point3d& other)
    : m_storage(other.m_storage)
  {}

  /// get x
  double Point3d::x() const
  {
    return m_storage[0];
  }

  /// get y
  double Point3d::y() const
  {
    return m_storage[1];
  }

  /// get z
  double Point3d::z() const
  {
    return m_storage[2];
  }

  /// point plus a vector is a new point
  Point3d Point3d::operator+(const Vector3d& vec) const
  {
    double newX = x()+vec.x();
    double newY = y()+vec.y();
    double newZ = z()+vec.z();
    return Point3d(newX, newY, newZ);
  }

  /// point plus a vector is a new point
  Point3d& Point3d::operator+=(const Vector3d& vec)
  {
    m_storage[0] += vec.x();
    m_storage[1] += vec.y();
    m_storage[2] += vec.z();
    return *this;
  }

  /// point minus another point is a vector
  Vector3d Point3d::operator-(const Point3d& other) const
  {
    double newX = x()-other.x();
    double newY = y()-other.y();
    double newZ = z()-other.z();
    return Vector3d(newX, newY, newZ);
  }

  /// check equality
  bool Point3d::operator==(const Point3d& other) const
  {
    return (m_storage == other.m_storage);
  }

  /// ostream operator
  std::ostream& operator<<(std::ostream& os, const Point3d& point)
  {
    os << "[" << point.x() << ", " << point.y() << ", " << point.z() << "]";
    return os;
  }

  /// ostream operator
  std::ostream& operator<<(std::ostream& os, const std::vector<Point3d>& pointVector)
  {
    os << "[";
    for (unsigned i = 0; i < pointVector.size(); ++i){
      os << pointVector[i];
      if (i < pointVector.size()-1){
        os << ", ";
      }
    }
    os << "]";
    return os;
  }


} // openstudio
