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

#include "WhUnit.hpp"
#include "WhUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  WhUnit_Impl::WhUnit_Impl(const WhExpnt& exponents,
                           int scaleExponent,
                           const std::string& prettyString)
    : Unit_Impl(scaleExponent,prettyString,11,UnitSystem::Wh)
  {
    m_units[0].first = "W"; m_units[0].second = exponents.m_W;
    m_units[1].first = "h"; m_units[1].second = exponents.m_h;
    m_units[2].first = "m"; m_units[2].second = exponents.m_m;
    m_units[3].first = "K"; m_units[3].second = exponents.m_K;
    m_units[4].first = "A"; m_units[4].second = exponents.m_A;
    m_units[5].first = "cd"; m_units[5].second = exponents.m_cd;
    m_units[6].first = "mol"; m_units[6].second = exponents.m_mol;
    m_units[7].first = "rad"; m_units[7].second = exponents.m_rad;
    m_units[8].first = "sr"; m_units[8].second = exponents.m_sr;
    m_units[9].first = "people"; m_units[9].second = exponents.m_people;
    m_units[10].first = "cycle"; m_units[10].second = exponents.m_cycle;
  }

  WhUnit_Impl::WhUnit_Impl(const std::string& scaleAbbreviation,
                           const WhExpnt& exponents,
                           const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation,prettyString,11,UnitSystem::Wh)
  {
    m_units[0].first = "W"; m_units[0].second = exponents.m_W;
    m_units[1].first = "h"; m_units[1].second = exponents.m_h;
    m_units[2].first = "m"; m_units[2].second = exponents.m_m;
    m_units[3].first = "K"; m_units[3].second = exponents.m_K;
    m_units[4].first = "A"; m_units[4].second = exponents.m_A;
    m_units[5].first = "cd"; m_units[5].second = exponents.m_cd;
    m_units[6].first = "mol"; m_units[6].second = exponents.m_mol;
    m_units[7].first = "rad"; m_units[7].second = exponents.m_rad;
    m_units[8].first = "sr"; m_units[8].second = exponents.m_sr;
    m_units[9].first = "people"; m_units[9].second = exponents.m_people;
    m_units[10].first = "cycle"; m_units[10].second = exponents.m_cycle;
  }

  Unit WhUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::Wh);
    std::shared_ptr<WhUnit_Impl> impl(new WhUnit_Impl(*this));
    return WhUnit(impl).cast<Unit>();
  }

  void WhUnit_Impl::setBaseUnitExponent(const std::string& baseUnit,int exponent)
  {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    }
    else {
      LOG_AND_THROW("Cannot add base units to an instance of WhUnit.");
    }
  }

} // detail

WhUnit::WhUnit(const WhExpnt& exponents,
               int scaleExponent,
               const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(
             new detail::WhUnit_Impl(exponents,scaleExponent,prettyString)))
{}

WhUnit::WhUnit(const std::string& scaleAbbreviation,
               const WhExpnt& exponents,
               const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(
             new detail::WhUnit_Impl(scaleAbbreviation,exponents,prettyString)))
{}

/// @cond
WhUnit::WhUnit(std::shared_ptr<detail::WhUnit_Impl> impl)
  : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl))
{}
/// @endcond

WhUnit createWhPower() {
  return WhUnit(WhExpnt(1));
}

WhUnit createWhTime() {
  return WhUnit(WhExpnt(0,1));
}

WhUnit createWhLength() {
  return WhUnit(WhExpnt(0,0,1));
}

WhUnit createWhTemperature() {
  return WhUnit(WhExpnt(0,0,0,1));
}

WhUnit createWhElectricCurrent() {
  return WhUnit(WhExpnt(0,0,0,0,1));
}

WhUnit createWhLuminousIntensity() {
  return WhUnit(WhExpnt(0,0,0,0,0,1));
}

WhUnit createWhAmountOfSubstance() {
  return WhUnit(WhExpnt(0,0,0,0,0,0,1));
}

WhUnit createWhAngle() {
  return WhUnit(WhExpnt(0,0,0,0,0,0,0,1));
}

WhUnit createWhSolidAngle() {
  return WhUnit(WhExpnt(0,0,0,0,0,0,0,0,1));
}

WhUnit createWhPeople() {
  return WhUnit(WhExpnt(0,0,0,0,0,0,0,0,0,1));
}

WhUnit createWhCycle() {
  return WhUnit(WhExpnt(0,0,0,0,0,0,0,0,0,0,1));
}


WhUnit createWhEnergy() {
  return WhUnit(WhExpnt(1,1),0,"Wh");
}

WhUnit createWhElectricalPotential() {
  return WhUnit(WhExpnt(1,0,0,0,-1),0,"V");
}

WhUnit createWhElectricResistance() {
  return WhUnit(WhExpnt(1,0,0,0,-2),0,"ohm");
}

WhUnit createWhLuminousFlux() {
  return WhUnit(WhExpnt(0,0,0,0,0,1,0,0,1),0,"lm");
}

WhUnit createWhIlluminance() {
  return WhUnit(WhExpnt(0,0,-2,0,0,1,0,0,1),0,"lux");
}

} // openstudio

