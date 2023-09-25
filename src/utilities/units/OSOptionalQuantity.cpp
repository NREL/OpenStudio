/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OSOptionalQuantity.hpp"
#include "Quantity.hpp"

#include "../core/Assert.hpp"

#include <cmath>

namespace openstudio {

OSOptionalQuantity::OSOptionalQuantity(const UnitSystem& sys) {
  Quantity q(sys);
  m_units = q.units();
}

OSOptionalQuantity::OSOptionalQuantity(const Unit& units) : m_units(units.clone()) {}

OSOptionalQuantity::OSOptionalQuantity(const Quantity& quantity) : m_value(quantity.value()), m_units(quantity.units()) {}

OSOptionalQuantity::OSOptionalQuantity(const OSOptionalQuantity& oq) : m_value(oq.m_value), m_units(oq.units()) {}

OSOptionalQuantity& OSOptionalQuantity::operator=(const OSOptionalQuantity& oq) {
  if (this == &oq) {
    return *this;
  }

  m_value = oq.m_value;
  m_units = oq.units();
  return *this;
}

bool OSOptionalQuantity::empty() const {
  return !m_value;
}

bool OSOptionalQuantity::isSet() const {
  return m_value.has_value();
}

Quantity OSOptionalQuantity::get() const {
  OS_ASSERT(m_value);
  return Quantity(*m_value, units());
}

Unit OSOptionalQuantity::units() const {
  return m_units.clone();
}

}  // namespace openstudio
