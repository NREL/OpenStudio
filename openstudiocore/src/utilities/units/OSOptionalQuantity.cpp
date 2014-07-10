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

#include "OSOptionalQuantity.hpp"
#include "Quantity.hpp"

#include "../core/Assert.hpp"

#include <cmath>

int _OptionalQuantityType = qRegisterMetaType<openstudio::OSOptionalQuantity>("openstudio::OSOptionalQuantity");

namespace openstudio{

OSOptionalQuantity::OSOptionalQuantity(const UnitSystem& sys)
{
  Quantity q(sys);
  m_units = q.units();
}

OSOptionalQuantity::OSOptionalQuantity(const Unit& units)
  : m_units(units.clone())
{}

OSOptionalQuantity::OSOptionalQuantity(const Quantity& quantity)
  : m_value(quantity.value()), m_units(quantity.units())
{}

OSOptionalQuantity::OSOptionalQuantity(const OSOptionalQuantity& oq)
  : m_value(oq.m_value), m_units(oq.units())
{}

OSOptionalQuantity& OSOptionalQuantity::operator=(const OSOptionalQuantity& oq)
{
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
  return m_value;
}

Quantity OSOptionalQuantity::get() const {
  OS_ASSERT(m_value);
  return Quantity(*m_value,units());
}

Unit OSOptionalQuantity::units() const {
  return m_units.clone();
}

} // openstudio
