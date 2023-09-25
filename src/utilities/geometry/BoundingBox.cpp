/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "BoundingBox.hpp"

#include "Point3d.hpp"

namespace openstudio {

void BoundingBox::add(const BoundingBox& other) {
  this->addPoints(other.corners());
}

void BoundingBox::addPoint(const Point3d& point) {
  if (isEmpty()) {
    m_minX = point.x();
    m_minY = point.y();
    m_minZ = point.z();

    m_maxX = point.x();
    m_maxY = point.y();
    m_maxZ = point.z();
  } else {
    m_minX = std::min(*m_minX, point.x());
    m_minY = std::min(*m_minY, point.y());
    m_minZ = std::min(*m_minZ, point.z());

    m_maxX = std::max(*m_maxX, point.x());
    m_maxY = std::max(*m_maxY, point.y());
    m_maxZ = std::max(*m_maxZ, point.z());
  }
}

void BoundingBox::addPoints(const std::vector<Point3d>& points) {
  for (const Point3d& point : points) {
    addPoint(point);
  }
}

bool BoundingBox::intersects(const BoundingBox& other, double tol) const {
  if (isEmpty() || other.isEmpty()) {
    return false;
  }

  bool test = ((*m_minX > other.maxX().get() + tol) || (*m_minY > other.maxY().get() + tol) || (*m_minZ > other.maxZ().get() + tol)
               || (other.minX().get() > *m_maxX + tol) || (other.minY().get() > *m_maxY + tol) || (other.minZ().get() > *m_maxZ + tol));

  return (!test);
}

bool BoundingBox::isEmpty() const {
  return !(m_minX);
}

boost::optional<double> BoundingBox::minX() const {
  return m_minX;
}

boost::optional<double> BoundingBox::minY() const {
  return m_minY;
}

boost::optional<double> BoundingBox::minZ() const {
  return m_minZ;
}

boost::optional<double> BoundingBox::maxX() const {
  return m_maxX;
}

boost::optional<double> BoundingBox::maxY() const {
  return m_maxY;
}

boost::optional<double> BoundingBox::maxZ() const {
  return m_maxZ;
}

std::vector<Point3d> BoundingBox::corners() const {
  std::vector<Point3d> result;

  if (isEmpty()) {
    return result;
  }

  result.push_back(Point3d(*m_minX, *m_minY, *m_minZ));
  result.push_back(Point3d(*m_maxX, *m_minY, *m_minZ));
  result.push_back(Point3d(*m_minX, *m_maxY, *m_minZ));
  result.push_back(Point3d(*m_maxX, *m_maxY, *m_minZ));
  result.push_back(Point3d(*m_minX, *m_minY, *m_maxZ));
  result.push_back(Point3d(*m_maxX, *m_minY, *m_maxZ));
  result.push_back(Point3d(*m_minX, *m_maxY, *m_maxZ));
  result.push_back(Point3d(*m_maxX, *m_maxY, *m_maxZ));

  return result;
}

}  // namespace openstudio
