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

#include "TemperatureUnit.hpp"
#include "TemperatureUnit_Impl.hpp"

namespace openstudio {
namespace detail {

  bool TemperatureUnit_Impl::isAbsolute() const {
    return m_absolute;
  }

  void TemperatureUnit_Impl::setAsAbsolute() {
    m_absolute = true;
  }

  void TemperatureUnit_Impl::setAsRelative() {
    m_absolute = false;
  }

  void TemperatureUnit_Impl::operator*=(const Unit& rUnit) {
    Unit_Impl::operator*=(rUnit);
    OptionalTemperatureUnit rTUnit = rUnit.optionalCast<TemperatureUnit>();
    if (!isAbsolute() && rTUnit && rTUnit->isAbsolute()) {
      setAsAbsolute();
    }
  }

  void TemperatureUnit_Impl::operator/=(const Unit& rUnit) {
    Unit_Impl::operator/=(rUnit);
    OptionalTemperatureUnit rTUnit = rUnit.optionalCast<TemperatureUnit>();
    if (!isAbsolute() && rTUnit && rTUnit->isAbsolute()) {
      setAsAbsolute();
    }
  }

  TemperatureUnit_Impl::TemperatureUnit_Impl(bool absolute,
                                             int scaleExponent,
                                             const std::string& prettyString,
                                             int numBaseUnits,
                                             UnitSystem system)
    : Unit_Impl(scaleExponent,prettyString,numBaseUnits,system), m_absolute(absolute)
  {}

  TemperatureUnit_Impl::TemperatureUnit_Impl(bool absolute,
                                             const std::string& scaleAbbreviation,
                                             const std::string& prettyString,
                                             int numBaseUnits,
                                             UnitSystem system)
    : Unit_Impl(scaleAbbreviation,prettyString,numBaseUnits,system), m_absolute(absolute)
  {}

} // detail

bool TemperatureUnit::isAbsolute() const {
  return getImpl<detail::TemperatureUnit_Impl>()->isAbsolute();
}

void TemperatureUnit::setAsAbsolute() {
  getImpl<detail::TemperatureUnit_Impl>()->setAsAbsolute();
}

void TemperatureUnit::setAsRelative() {
  getImpl<detail::TemperatureUnit_Impl>()->setAsRelative();
}

/// @cond
TemperatureUnit::TemperatureUnit(std::shared_ptr<detail::TemperatureUnit_Impl> impl)
  : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl))
{}
/// @endcond

} // openstudio

