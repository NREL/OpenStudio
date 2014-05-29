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

#include "ThermUnit.hpp"
#include "ThermUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  ThermUnit_Impl::ThermUnit_Impl(const ThermExpnt& exponents,
                                 int scaleExponent,
                                 const std::string& prettyString)
    : Unit_Impl(scaleExponent,prettyString,11,UnitSystem::Therm)
  {
    m_units[0].first = "therm"; m_units[0].second = exponents.m_therm;
    m_units[1].first = "in"; m_units[1].second = exponents.m_in;
    m_units[2].first = "yr"; m_units[2].second = exponents.m_yr;
    m_units[3].first = "R"; m_units[3].second = exponents.m_R;
    m_units[4].first = "A"; m_units[4].second = exponents.m_A;
    m_units[5].first = "cd"; m_units[5].second = exponents.m_cd;
    m_units[6].first = "lbmol"; m_units[6].second = exponents.m_lbmol;
    m_units[7].first = "deg"; m_units[7].second = exponents.m_deg;
    m_units[8].first = "sr"; m_units[8].second = exponents.m_sr;
    m_units[9].first = "people"; m_units[9].second = exponents.m_people;
    m_units[10].first = "cycle"; m_units[10].second = exponents.m_cycle;
  }

  ThermUnit_Impl::ThermUnit_Impl(const std::string& scaleAbbreviation,
                                 const ThermExpnt& exponents,
                                 const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation,prettyString,11,UnitSystem::Therm)
  {
    m_units[0].first = "therm"; m_units[0].second = exponents.m_therm;
    m_units[1].first = "in"; m_units[1].second = exponents.m_in;
    m_units[2].first = "yr"; m_units[2].second = exponents.m_yr;
    m_units[3].first = "R"; m_units[3].second = exponents.m_R;
    m_units[4].first = "A"; m_units[4].second = exponents.m_A;
    m_units[5].first = "cd"; m_units[5].second = exponents.m_cd;
    m_units[6].first = "lbmol"; m_units[6].second = exponents.m_lbmol;
    m_units[7].first = "deg"; m_units[7].second = exponents.m_deg;
    m_units[8].first = "sr"; m_units[8].second = exponents.m_sr;
    m_units[9].first = "people"; m_units[9].second = exponents.m_people;
    m_units[10].first = "cycle"; m_units[10].second = exponents.m_cycle;
  }

  Unit ThermUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::Therm);
    std::shared_ptr<ThermUnit_Impl> impl(new ThermUnit_Impl(*this));
    return ThermUnit(impl).cast<Unit>();
  }

  void ThermUnit_Impl::setBaseUnitExponent(const std::string& baseUnit,int exponent)
  {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    }
    else {
      LOG_AND_THROW("Cannot add base units to an instance of ThermUnit.");
    }
  }

} // detail

ThermUnit::ThermUnit(const ThermExpnt& exponents,
                     int scaleExponent,
                     const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(
             new detail::ThermUnit_Impl(exponents,scaleExponent,prettyString)))
{}

ThermUnit::ThermUnit(const std::string& scaleAbbreviation,
                     const ThermExpnt& exponents,
                     const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(
             new detail::ThermUnit_Impl(scaleAbbreviation,exponents,prettyString)))
{}

/// @cond
ThermUnit::ThermUnit(std::shared_ptr<detail::ThermUnit_Impl> impl)
  : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl))
{}
/// @endcond

ThermUnit createThermEnergy() {
  return ThermUnit(ThermExpnt(1)); // therm
}

ThermUnit createThermLength() {
  return ThermUnit(ThermExpnt(0,1));
}

ThermUnit createThermTime() {
  return ThermUnit(ThermExpnt(0,0,1));
}

ThermUnit createThermTemperature() {
  return ThermUnit(ThermExpnt(0,0,0,1));
}

ThermUnit createThermElectricCurrent() {
  return ThermUnit(ThermExpnt(0,0,0,0,1));
}

ThermUnit createThermLuminousIntensity() {
  return ThermUnit(ThermExpnt(0,0,0,0,0,1));
}

ThermUnit createThermAmountOfSubstance() {
  return ThermUnit(ThermExpnt(0,0,0,0,0,0,1));
}

ThermUnit createThermAngle() {
  return ThermUnit(ThermExpnt(0,0,0,0,0,0,0,1));
}

ThermUnit createThermSolidAngle() {
  return ThermUnit(ThermExpnt(0,0,0,0,0,0,0,0,1));
}

ThermUnit createThermPeople() {
  return ThermUnit(ThermExpnt(0,0,0,0,0,0,0,0,0,1));
}

ThermUnit createThermCycle() {
  return ThermUnit(ThermExpnt(0,0,0,0,0,0,0,0,0,0,1));
}

ThermUnit createThermLuminousFlux() {
  return ThermUnit(ThermExpnt(0,0,0,0,0,1,0,0,1),0,"lm");
}

} // openstudio

