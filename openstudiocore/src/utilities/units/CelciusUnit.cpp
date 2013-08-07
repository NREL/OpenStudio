/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <utilities/units/CelciusUnit.hpp>
#include <utilities/units/CelciusUnit_Impl.hpp>

#include <utilities/core/Assert.hpp>

namespace openstudio {
namespace detail {

  CelciusUnit_Impl::CelciusUnit_Impl(int CExp,int scaleExponent,const std::string& prettyString)
    : TemperatureUnit_Impl(true,scaleExponent,prettyString,1,UnitSystem::Celcius)
  {
    m_units[0].first = "C"; m_units[0].second = CExp;
  }

  CelciusUnit_Impl::CelciusUnit_Impl(const std::string& scaleAbbreviation,
                                     int CExp,
                                     const std::string& prettyString)
    : TemperatureUnit_Impl(true,scaleAbbreviation,prettyString,1,UnitSystem::Celcius)
  {
    m_units[0].first = "C"; m_units[0].second = CExp;
  }

  Unit CelciusUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::Celcius);
    boost::shared_ptr<CelciusUnit_Impl> impl(new CelciusUnit_Impl(*this));
    return CelciusUnit(impl).cast<Unit>();
  }

  void CelciusUnit_Impl::setBaseUnitExponent(const std::string& baseUnit,int exponent) {
    std::vector<UnitElement>::iterator loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    }
    else {
      LOG_AND_THROW("Cannot add base units to an instance of CelciusUnit.");
    }
  }

} // detail

CelciusUnit::CelciusUnit(int CExp,int scaleExponent,const std::string& prettyString)
  : TemperatureUnit(boost::shared_ptr<detail::CelciusUnit_Impl>(
                        new detail::CelciusUnit_Impl(CExp,scaleExponent,prettyString)))
{}

CelciusUnit::CelciusUnit(const std::string& scaleAbbreviation,
                         int CExp,
                         const std::string& prettyString)
  : TemperatureUnit(boost::shared_ptr<detail::CelciusUnit_Impl>(
                        new detail::CelciusUnit_Impl(scaleAbbreviation,CExp,prettyString)))
{}

/// @cond
CelciusUnit::CelciusUnit(boost::shared_ptr<detail::CelciusUnit_Impl> impl)
  : TemperatureUnit(impl)
{}
/// @endcond

CelciusUnit createCelciusTemperature() {
  return CelciusUnit(1);
}

} // openstudio

