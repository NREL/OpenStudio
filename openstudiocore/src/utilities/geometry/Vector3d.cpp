/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "Vector3d.hpp"

namespace openstudio{

  /// default constructor creates vector with 0, 0, 0
  Vector3d::Vector3d()
    : m_storage(3, 0.0)
  {}

  /// constructor with x, y, z
  Vector3d::Vector3d(double x, double y, double z)
    : m_storage(3)
  {
    m_storage[0] = x;
    m_storage[1] = y;
    m_storage[2] = z;
  }

  /// copy constructor
  Vector3d::Vector3d(const Vector3d& other)
    : m_storage(other.m_storage)
  {}

  /// get x
  double Vector3d::x() const
  {
    return m_storage[0];
  }

  /// get y
  double Vector3d::y() const
  {
    return m_storage[1];
  }

  /// get z
  double Vector3d::z() const
  {
    return m_storage[2];
  }

  /// addition
  Vector3d Vector3d::operator+(const Vector3d& other) const
  {
    double newX = x()+other.x();
    double newY = y()+other.y();
    double newZ = z()+other.z();
    return Vector3d(newX, newY, newZ);
  }

  /// addition
  Vector3d& Vector3d::operator+=(const Vector3d& other)
  {
    m_storage[0] += other.x();
    m_storage[1] += other.y();
    m_storage[2] += other.z();
    return *this;
  }

  /// subtraction
  Vector3d Vector3d::operator-(const Vector3d& other) const
  {
    double newX = x()-other.x();
    double newY = y()-other.y();
    double newZ = z()-other.z();
    return Vector3d(newX, newY, newZ);
  }

  /// subtraction
  Vector3d& Vector3d::operator-=(const Vector3d& other)
  {
    m_storage[0] -= other.x();
    m_storage[1] -= other.y();
    m_storage[2] -= other.z();
    return *this;
  }

  /// check equality
  bool Vector3d::operator==(const Vector3d& other) const
  {
    return (m_storage == other.m_storage);
  }

  /// ostream operator
  std::ostream& operator<<(std::ostream& os, const Vector3d& vec)
  {
    os << "[" << vec.x() << ", " << vec.y() << ", " << vec.z() << "]";
    return os;
  }

  /// ostream operator
  std::ostream& operator<<(std::ostream& os, const std::vector<Vector3d>& vecVector)
  {
    os << "[";
    for (unsigned i = 0; i < vecVector.size(); ++i){
      os << vecVector[i];
      if (i < vecVector.size()-1){
        os << ", ";
      }
    }
    os << "]";
    return os;
  }

  /// negation
  Vector3d operator-(const Vector3d& vec)
  {
    return Vector3d(-vec.x(), -vec.y(), -vec.z());
  }

  /// multiplication by a scalar
  Vector3d operator*(double mult, const Vector3d& vec)
  {
    return Vector3d(mult*vec.x(), mult*vec.y(), mult*vec.z());
  }

  /// normalize to one
  bool Vector3d::normalize()
  {
    return setLength(1.0);
  }

  /// get a vector which is the reverse of this
  Vector3d Vector3d::reverseVector() const
  {
    return Vector3d(-m_storage[0], -m_storage[1], -m_storage[2]);
  }

  /// get length
  double Vector3d::length() const
  {
    return norm_2(m_storage);
  }

  /// set length
  bool Vector3d::setLength(double newLength)
  {
    bool result = false;
    double currentLength = length();
    if (currentLength > 0){
      double mult = newLength/currentLength;
      m_storage[0] *= mult;
      m_storage[1] *= mult;
      m_storage[2] *= mult;
      result = true;
    }
    return result;
  }

  /// dot product with another Vector3d
  double Vector3d::dot(const Vector3d& other) const
  {
    return inner_prod(m_storage, other.m_storage);
  }

  /// cross product with another Vector3d
  Vector3d Vector3d::cross(const Vector3d& other) const
  {
    double newX = (y()*other.z()-z()*other.y());
    double newY = (z()*other.x()-x()*other.z());
    double newZ = (x()*other.y()-y()*other.x());
    return Vector3d(newX, newY, newZ);
  }

  /// get the Vector directly
  Vector Vector3d::vector() const
  {
    return m_storage;
  }

} // openstudio
