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

#include "Point3d.hpp"
#include "Vector3d.hpp"

namespace openstudio {

/// default constructor creates point at 0, 0, 0
Point3d::Point3d() : m_storage(3, 0.0) {}

/// constructor with x, y, z
Point3d::Point3d(double x, double y, double z) : m_storage(3) {
  m_storage[0] = x;
  m_storage[1] = y;
  m_storage[2] = z;
}

/// copy constructor
Point3d::Point3d(const Point3d& other) : m_storage(other.m_storage) {}

/// get x
double Point3d::x() const {
  return m_storage[0];
}

/// get y
double Point3d::y() const {
  return m_storage[1];
}

/// get z
double Point3d::z() const {
  return m_storage[2];
}

/// point plus a vector is a new point
Point3d Point3d::operator+(const Vector3d& vec) const {
  double newX = x() + vec.x();
  double newY = y() + vec.y();
  double newZ = z() + vec.z();
  return Point3d(newX, newY, newZ);
}

/// point plus a vector is a new point
Point3d& Point3d::operator+=(const Vector3d& vec) {
  m_storage[0] += vec.x();
  m_storage[1] += vec.y();
  m_storage[2] += vec.z();
  return *this;
}

/// point minus another point is a vector
Vector3d Point3d::operator-(const Point3d& other) const {
  double newX = x() - other.x();
  double newY = y() - other.y();
  double newZ = z() - other.z();
  return Vector3d(newX, newY, newZ);
}

/// check equality
bool Point3d::operator==(const Point3d& other) const {
  return (m_storage == other.m_storage);
}

/// check inequality
bool Point3d::operator!=(const Point3d& other) const {
  return (m_storage != other.m_storage);
}

/// ostream operator
std::ostream& operator<<(std::ostream& os, const Point3d& point) {
  os << "[" << point.x() << ", " << point.y() << ", " << point.z() << "]";
  return os;
}

/// ostream operator
std::ostream& operator<<(std::ostream& os, const std::vector<Point3d>& pointVector) {
  os << "[";
  for (unsigned i = 0; i < pointVector.size(); ++i) {
    os << pointVector[i];
    if (i < pointVector.size() - 1) {
      os << ", ";
    }
  }
  os << "]";
  return os;
}

}  // namespace openstudio
