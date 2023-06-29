/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Vector3d.hpp"

namespace openstudio {

/// default constructor creates vector with 0, 0, 0
Vector3d::Vector3d() : m_storage(3, 0.0) {}

/// constructor with x, y, z
Vector3d::Vector3d(double x, double y, double z) : m_storage(3) {
  m_storage[0] = x;
  m_storage[1] = y;
  m_storage[2] = z;
}

/// get x
double Vector3d::x() const {
  return m_storage[0];
}

/// get y
double Vector3d::y() const {
  return m_storage[1];
}

/// get z
double Vector3d::z() const {
  return m_storage[2];
}

/// addition
Vector3d Vector3d::operator+(const Vector3d& other) const {
  double newX = x() + other.x();
  double newY = y() + other.y();
  double newZ = z() + other.z();
  return {newX, newY, newZ};
}

/// addition
Vector3d& Vector3d::operator+=(const Vector3d& other) {
  m_storage[0] += other.x();
  m_storage[1] += other.y();
  m_storage[2] += other.z();
  return *this;
}

/// subtraction
Vector3d Vector3d::operator-(const Vector3d& other) const {
  double newX = x() - other.x();
  double newY = y() - other.y();
  double newZ = z() - other.z();
  return {newX, newY, newZ};
}

/// subtraction
Vector3d& Vector3d::operator-=(const Vector3d& other) {
  m_storage[0] -= other.x();
  m_storage[1] -= other.y();
  m_storage[2] -= other.z();
  return *this;
}

/// check equality
bool Vector3d::operator==(const Vector3d& other) const {
  return (m_storage == other.m_storage);
}

/// check inequality
bool Vector3d::operator!=(const Vector3d& other) const {
  return (m_storage != other.m_storage);
}

/// ostream operator
std::ostream& operator<<(std::ostream& os, const Vector3d& vec) {
  os << "[" << vec.x() << ", " << vec.y() << ", " << vec.z() << "]";
  return os;
}

/// ostream operator
std::ostream& operator<<(std::ostream& os, const std::vector<Vector3d>& vecVector) {
  os << "[";
  for (unsigned i = 0; i < vecVector.size(); ++i) {
    os << vecVector[i];
    if (i < vecVector.size() - 1) {
      os << ", ";
    }
  }
  os << "]";
  return os;
}

/// negation
Vector3d operator-(const Vector3d& vec) {
  return {-vec.x(), -vec.y(), -vec.z()};
}

/// multiplication by a scalar
Vector3d operator*(double mult, const Vector3d& vec) {
  return {mult * vec.x(), mult * vec.y(), mult * vec.z()};
}

/// normalize to one
bool Vector3d::normalize() {
  return setLength(1.0);
}

/// get a vector which is the reverse of this
Vector3d Vector3d::reverseVector() const {
  return {-m_storage[0], -m_storage[1], -m_storage[2]};
}

/// get length
double Vector3d::length() const {
  return norm_2(m_storage);
}

/// set length
bool Vector3d::setLength(double newLength) {
  bool result = false;
  double currentLength = length();
  if (currentLength > 0) {
    double mult = newLength / currentLength;
    m_storage[0] *= mult;
    m_storage[1] *= mult;
    m_storage[2] *= mult;
    result = true;
  }
  return result;
}

/// orthogonal right
Vector3d Vector3d::orthogonalRight() const {
  return {m_storage[1], -m_storage[0], m_storage[2]};
}

/// orthogonal left
Vector3d Vector3d::orthogonalLeft() const {
  return {-m_storage[1], m_storage[0], m_storage[2]};
}

/// dot product with another Vector3d
double Vector3d::dot(const Vector3d& other) const {
  return inner_prod(m_storage, other.m_storage);
}

/// cross product with another Vector3d
Vector3d Vector3d::cross(const Vector3d& other) const {
  double newX = (y() * other.z() - z() * other.y());
  double newY = (z() * other.x() - x() * other.z());
  double newZ = (x() * other.y() - y() * other.x());
  return {newX, newY, newZ};
}

/// get the Vector directly
Vector Vector3d::vector() const {
  return m_storage;
}

}  // namespace openstudio
