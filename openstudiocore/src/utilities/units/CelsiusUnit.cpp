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

#include "CelsiusUnit.hpp"
#include "CelsiusUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  CelsiusUnit_Impl::CelsiusUnit_Impl(int CExp,int scaleExponent,const std::string& prettyString)
    : TemperatureUnit_Impl(true,scaleExponent,prettyString,1,UnitSystem::Celsius)
  {
    m_units[0].first = "C"; m_units[0].second = CExp;
  }

  CelsiusUnit_Impl::CelsiusUnit_Impl(const std::string& scaleAbbreviation,
                                     int CExp,
                                     const std::string& prettyString)
    : TemperatureUnit_Impl(true,scaleAbbreviation,prettyString,1,UnitSystem::Celsius)
  {
    m_units[0].first = "C"; m_units[0].second = CExp;
  }

  Unit CelsiusUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::Celsius);
    std::shared_ptr<CelsiusUnit_Impl> impl(new CelsiusUnit_Impl(*this));
    return CelsiusUnit(impl).cast<Unit>();
  }

  void CelsiusUnit_Impl::setBaseUnitExponent(const std::string& baseUnit,int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    }
    else {
      LOG_AND_THROW("Cannot add base units to an instance of CelsiusUnit.");
    }
  }

} // detail

CelsiusUnit::CelsiusUnit(int CExp,int scaleExponent,const std::string& prettyString)
  : TemperatureUnit(std::shared_ptr<detail::CelsiusUnit_Impl>(
                        new detail::CelsiusUnit_Impl(CExp,scaleExponent,prettyString)))
{}

CelsiusUnit::CelsiusUnit(const std::string& scaleAbbreviation,
                         int CExp,
                         const std::string& prettyString)
  : TemperatureUnit(std::shared_ptr<detail::CelsiusUnit_Impl>(
                        new detail::CelsiusUnit_Impl(scaleAbbreviation,CExp,prettyString)))
{}

/// @cond
CelsiusUnit::CelsiusUnit(std::shared_ptr<detail::CelsiusUnit_Impl> impl)
  : TemperatureUnit(impl)
{}
/// @endcond

CelsiusUnit createCelsiusTemperature() {
  return CelsiusUnit(1);
}

} // openstudio

