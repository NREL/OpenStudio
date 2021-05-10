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

#include "BoundingBox.hpp"

#include "Point3d.hpp"

namespace openstudio {

BoundingBox::BoundingBox() {}

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
