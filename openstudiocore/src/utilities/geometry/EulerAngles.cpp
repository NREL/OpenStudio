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

#include "EulerAngles.hpp"

namespace openstudio{

  /// default constructor with no rotation
  EulerAngles::EulerAngles()
    : m_storage(3, 0.0)
  {}

  /// constructor with psi, theta, phi
  EulerAngles::EulerAngles(double psi, double theta, double phi)
    : m_storage(3)
  {
    m_storage[0] = psi;
    m_storage[1] = theta;
    m_storage[2] = phi;
  }

  /// copy constructor
  EulerAngles::EulerAngles(const EulerAngles& other)
    : m_storage(other.m_storage)
  {}

  /// get psi
  double EulerAngles::psi() const
  {
    return m_storage[0];
  }

  /// get theta
  double EulerAngles::theta() const
  {
    return m_storage[1];
  }

  /// get phi
  double EulerAngles::phi() const
  {
    return m_storage[2];
  }

  /// ostream operator
  std::ostream& operator<<(std::ostream& os, const EulerAngles& angles)
  {
    os << "[" << angles.psi() << ", " << angles.theta() << ", " << angles.phi() << "]";
    return os;
  }

} // openstudio
