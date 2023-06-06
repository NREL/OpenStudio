/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
Point3d::Point3d(const Point3d& other) = default;

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
  return {newX, newY, newZ};
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
  return {newX, newY, newZ};
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

// Writes a json string representing the vector of point vectors
// Will deserialize in to a simple class that defines three double properties for X,Y,Z
std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<Point3d>>& pointVectorVector) {
  os << "[\n";
  for (const auto& points : pointVectorVector) {
    os << "  [";
    for (const auto& point : points) {
      os << "\n    " << point << ", ";
    }
    os << "\n  ],\n";
  }
  os << "]";

  return os;
}

}  // namespace openstudio
