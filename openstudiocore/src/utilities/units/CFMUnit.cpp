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

#include "CFMUnit.hpp"
#include "CFMUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {

namespace detail {

  CFMUnit_Impl::CFMUnit_Impl(const CFMExpnt& exponents,
                             int scaleExponent,
                             const std::string& prettyString)
    : Unit_Impl(scaleExponent,prettyString,11,UnitSystem::CFM)
  {
    m_units[0].first = "ft"; m_units[0].second = exponents.m_ft;
    m_units[1].first = "min"; m_units[1].second = exponents.m_min;
    m_units[2].first = "ton"; m_units[2].second = exponents.m_ton;
    m_units[3].first = "R"; m_units[3].second = exponents.m_R;
    m_units[4].first = "A"; m_units[4].second = exponents.m_A;
    m_units[5].first = "cd"; m_units[5].second = exponents.m_cd;
    m_units[6].first = "lbmol"; m_units[6].second = exponents.m_lbmol;
    m_units[7].first = "deg"; m_units[7].second = exponents.m_deg;
    m_units[8].first = "sr"; m_units[8].second = exponents.m_sr;
    m_units[9].first = "people"; m_units[9].second = exponents.m_people;
    m_units[10].first = "cycle"; m_units[10].second = exponents.m_cycle;
  }

  CFMUnit_Impl::CFMUnit_Impl(const std::string& scaleAbbreviation,
                             const CFMExpnt& exponents,
                             const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation,prettyString,11,UnitSystem::CFM)
  {
    m_units[0].first = "ft"; m_units[0].second = exponents.m_ft;
    m_units[1].first = "min"; m_units[1].second = exponents.m_min;
    m_units[2].first = "ton"; m_units[2].second = exponents.m_ton;
    m_units[3].first = "R"; m_units[3].second = exponents.m_R;
    m_units[4].first = "A"; m_units[4].second = exponents.m_A;
    m_units[5].first = "cd"; m_units[5].second = exponents.m_cd;
    m_units[6].first = "lbmol"; m_units[6].second = exponents.m_lbmol;
    m_units[7].first = "deg"; m_units[7].second = exponents.m_deg;
    m_units[8].first = "sr"; m_units[8].second = exponents.m_sr;
    m_units[9].first = "people"; m_units[9].second = exponents.m_people;
    m_units[10].first = "cycle"; m_units[10].second = exponents.m_cycle;
  }

  Unit CFMUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::CFM);
    std::shared_ptr<CFMUnit_Impl> impl(new CFMUnit_Impl(*this));
    return CFMUnit(impl).cast<Unit>();
  }

  void CFMUnit_Impl::setBaseUnitExponent(const std::string& baseUnit,int exponent)
  {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    }
    else {
      LOG_AND_THROW("Cannot add base units to an instance of CFMUnit.");
    }
  }

} // detail

CFMUnit::CFMUnit(const CFMExpnt& exponents,
                 int scaleExponent,
                 const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(
             new detail::CFMUnit_Impl(exponents, scaleExponent, prettyString)))
{}

CFMUnit::CFMUnit(const std::string& scaleAbbreviation,
                 const CFMExpnt& exponents,
                 const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(
             new detail::CFMUnit_Impl(scaleAbbreviation,exponents,prettyString)))
{}

/// @cond
CFMUnit::CFMUnit(std::shared_ptr<detail::CFMUnit_Impl> impl)
  : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl))
{}
/// @endcond

CFMUnit createCFMLength() {
  return CFMUnit(CFMExpnt(1));
}

CFMUnit createCFMTime() {
  return CFMUnit(CFMExpnt(0,1));
}

CFMUnit createCFMPower() {
  return CFMUnit(CFMExpnt(0,0,1));
}

CFMUnit createCFMTemperature() {
  return CFMUnit(CFMExpnt(0,0,0,1));
}

CFMUnit createCFMElectricCurrent() {
  return CFMUnit(CFMExpnt(0,0,0,0,1));
}

CFMUnit createCFMLuminousIntensity() {
  return CFMUnit(CFMExpnt(0,0,0,0,0,1));
}

CFMUnit createCFMAmountOfSubstance() {
  return CFMUnit(CFMExpnt(0,0,0,0,0,0,1));
}

CFMUnit createCFMAngle() {
  return CFMUnit(CFMExpnt(0,0,0,0,0,0,0,1));
}

CFMUnit createCFMSolidAngle() {
  return CFMUnit(CFMExpnt(0,0,0,0,0,0,0,0,1));
}

CFMUnit createCFMPeople() {
  return CFMUnit(CFMExpnt(0,0,0,0,0,0,0,0,0,1));
}

CFMUnit createCFMCycle() {
  return CFMUnit(CFMExpnt(0,0,0,0,0,0,0,0,0,0,1));
}


CFMUnit createCFMVolumetricFlowrate() {
  return CFMUnit(CFMExpnt(3,-1),0,"cfm");
}

CFMUnit createCFMLuminousFlux() {
  return CFMUnit(CFMExpnt(0,0,0,0,0,1,0,0,1),0,"lm");
}

CFMUnit createCFMIlluminance() {
  return CFMUnit(CFMExpnt(-2,0,0,0,0,1,0,0,1),0,"fc");
}

CFMUnit createCFMFrequency() {
  return CFMUnit(CFMExpnt(0,-1,0,0,0,0,0,0,0,0,1),0,"rpm");
}

} // openstudio

