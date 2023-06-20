/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EulerAngles.hpp"

namespace openstudio {

/// default constructor with no rotation
EulerAngles::EulerAngles() : m_storage(3, 0.0) {}

/// constructor with psi, theta, phi
EulerAngles::EulerAngles(double psi, double theta, double phi) : m_storage(3) {
  m_storage[0] = psi;
  m_storage[1] = theta;
  m_storage[2] = phi;
}

/// copy constructor
EulerAngles::EulerAngles(const EulerAngles& other) = default;

/// get psi
double EulerAngles::psi() const {
  return m_storage[0];
}

/// get theta
double EulerAngles::theta() const {
  return m_storage[1];
}

/// get phi
double EulerAngles::phi() const {
  return m_storage[2];
}

/// ostream operator
std::ostream& operator<<(std::ostream& os, const EulerAngles& angles) {
  os << "[" << angles.psi() << ", " << angles.theta() << ", " << angles.phi() << "]";
  return os;
}

}  // namespace openstudio
