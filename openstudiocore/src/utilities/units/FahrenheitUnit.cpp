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

#include "FahrenheitUnit.hpp"
#include "FahrenheitUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  FahrenheitUnit_Impl::FahrenheitUnit_Impl(int FExp,
                                           int scaleExponent,
                                           const std::string& prettyString)
    : TemperatureUnit_Impl(true,scaleExponent,prettyString,1,UnitSystem::Fahrenheit)
  {
    m_units[0].first = "F"; m_units[0].second = FExp;
  }

  FahrenheitUnit_Impl::FahrenheitUnit_Impl(const std::string& scaleAbbreviation,
                                           int FExp,
                                           const std::string& prettyString)
    : TemperatureUnit_Impl(true,scaleAbbreviation,prettyString,1,UnitSystem::Fahrenheit)
  {
    m_units[0].first = "F"; m_units[0].second = FExp;
  }

  Unit FahrenheitUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::Fahrenheit);
    std::shared_ptr<FahrenheitUnit_Impl> impl(new FahrenheitUnit_Impl(*this));
    return FahrenheitUnit(impl).cast<Unit>();
  }

  void FahrenheitUnit_Impl::setBaseUnitExponent(const std::string& baseUnit,int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    }
    else {
      LOG_AND_THROW("Cannot add base units to an instance of FahrenheitUnit.");
    }
  }

} // detail

FahrenheitUnit::FahrenheitUnit(int FExp,int scaleExponent,const std::string& prettyString)
  : TemperatureUnit(std::shared_ptr<detail::FahrenheitUnit_Impl>(
                        new detail::FahrenheitUnit_Impl(FExp,scaleExponent,prettyString)))
{}

FahrenheitUnit::FahrenheitUnit(const std::string& scaleAbbreviation,
                               int FExp,
                               const std::string& prettyString)
  : TemperatureUnit(std::shared_ptr<detail::FahrenheitUnit_Impl>(
                        new detail::FahrenheitUnit_Impl(scaleAbbreviation,FExp,prettyString)))
{}

FahrenheitUnit& FahrenheitUnit::operator/=(const FahrenheitUnit& rUnit) {
  getImpl<detail::FahrenheitUnit_Impl>()->operator/=(rUnit);
  return *this;
}

/// @cond
FahrenheitUnit::FahrenheitUnit(std::shared_ptr<detail::FahrenheitUnit_Impl> impl)
  : TemperatureUnit(impl)
{}
/// @endcond

FahrenheitUnit createFahrenheitTemperature() {
  return FahrenheitUnit(1);
}

} // openstudio

