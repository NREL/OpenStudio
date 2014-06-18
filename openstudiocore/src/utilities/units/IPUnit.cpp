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

#include "IPUnit.hpp"
#include "IPUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  IPUnit_Impl::IPUnit_Impl(const IPExpnt& exponents,
                           int scaleExponent,
                           const std::string& prettyString)
    : Unit_Impl(scaleExponent,prettyString,12,UnitSystem::IP)
  {
    m_units[0].first = "lb_m"; m_units[0].second = exponents.m_lbm;
    m_units[1].first = "ft"; m_units[1].second = exponents.m_ft;
    m_units[2].first = "s"; m_units[2].second = exponents.m_s;
    m_units[3].first = "R"; m_units[3].second = exponents.m_R;
    m_units[4].first = "A"; m_units[4].second = exponents.m_A;
    m_units[5].first = "cd"; m_units[5].second = exponents.m_cd;
    m_units[6].first = "lbmol"; m_units[6].second = exponents.m_lbmol;
    m_units[7].first = "lb_f"; m_units[7].second = exponents.m_lbf;
    m_units[8].first = "deg"; m_units[8].second = exponents.m_deg;
    m_units[9].first = "sr"; m_units[9].second = exponents.m_sr;
    m_units[10].first = "people"; m_units[10].second = exponents.m_people;
    m_units[11].first = "cycle"; m_units[11].second = exponents.m_cycle;
  }

  IPUnit_Impl::IPUnit_Impl(const std::string& scaleAbbreviation,
                           const IPExpnt& exponents,
                           const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation,prettyString,12,UnitSystem::IP)
  {
    m_units[0].first = "lb_m"; m_units[0].second = exponents.m_lbm;
    m_units[1].first = "ft"; m_units[1].second = exponents.m_ft;
    m_units[2].first = "s"; m_units[2].second = exponents.m_s;
    m_units[3].first = "R"; m_units[3].second = exponents.m_R;
    m_units[4].first = "A"; m_units[4].second = exponents.m_A;
    m_units[5].first = "cd"; m_units[5].second = exponents.m_cd;
    m_units[6].first = "lbmol"; m_units[6].second = exponents.m_lbmol;
    m_units[7].first = "lb_f"; m_units[7].second = exponents.m_lbf;
    m_units[8].first = "deg"; m_units[8].second = exponents.m_deg;
    m_units[9].first = "sr"; m_units[9].second = exponents.m_sr;
    m_units[10].first = "people"; m_units[10].second = exponents.m_people;
    m_units[11].first = "cycle"; m_units[11].second = exponents.m_cycle;
  }

  Unit IPUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::IP);
    std::shared_ptr<IPUnit_Impl> impl(new IPUnit_Impl(*this));
    return IPUnit(impl).cast<Unit>();
  }

  void IPUnit_Impl::setBaseUnitExponent(const std::string& baseUnit,int exponent)
  {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    }
    else {
      LOG_AND_THROW("Cannot add base units to an instance of IPUnit.");
    }
  }

  bool IPUnit_Impl::operator==(const Unit& rUnit) const {
    // compare using lb_m
    IPUnit wLUnit = clone().cast<IPUnit>();
    wLUnit.lbfToLbm();

    Unit wRUnit = rUnit.clone();
    if (OptionalIPUnit temp = wRUnit.optionalCast<IPUnit>()) {
      temp->lbfToLbm();

      // loop through and check each exponent
      for (const std::string& baseUnit : wLUnit.baseUnits()) {
        if (wLUnit.baseUnitExponent(baseUnit) != wRUnit.baseUnitExponent(baseUnit)) {
          return false;
        }
      }

      return true;
    }

    return (wRUnit == wLUnit);
  }

  void IPUnit_Impl::lbmToLbf() {
    if (m_units[0].second != 0) {
      m_units[7].second += m_units[0].second;
      m_units[2].second += 2*m_units[0].second;
      m_units[1].second -= m_units[0].second;
      m_units[0].second = 0;
    }
    return;
  }

  void IPUnit_Impl::lbfToLbm() {
    if (m_units[7].second != 0) {
      m_units[0].second += m_units[7].second;
      m_units[1].second += m_units[7].second;
      m_units[2].second -= 2*m_units[7].second;
      m_units[7].second = 0;
    }
    return;
  }

} // detail

IPUnit::IPUnit(const IPExpnt& exponents,
               int scaleExponent,
               const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(
             new detail::IPUnit_Impl(exponents,scaleExponent,prettyString)))
{}

IPUnit::IPUnit(const std::string& scaleAbbreviation,
               const IPExpnt& exponents,
               const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(
             new detail::IPUnit_Impl(scaleAbbreviation,exponents,prettyString)))
{}

double IPUnit::gc() {
  return 32.1740485564; // lb_m*ft/lb_f*s^2
}

void IPUnit::lbmToLbf() {
  getImpl<detail::IPUnit_Impl>()->lbmToLbf();
}

void IPUnit::lbfToLbm() {
  getImpl<detail::IPUnit_Impl>()->lbfToLbm();
}

/// @cond
IPUnit::IPUnit(std::shared_ptr<detail::IPUnit_Impl> impl)
  : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl))
{}
/// @endcond


IPUnit createIPMass() {
  return IPUnit(IPExpnt(1));
}

IPUnit createIPLength() {
  return IPUnit(IPExpnt(0,1));
}

IPUnit createIPTime() {
  return IPUnit(IPExpnt(0,0,1));
}

IPUnit createIPTemperature() {
  return IPUnit(IPExpnt(0,0,0,1));
}

IPUnit createIPElectricCurrent() {
  return IPUnit(IPExpnt(0,0,0,0,1));
}

IPUnit createIPLuminousIntensity() {
  return IPUnit(IPExpnt(0,0,0,0,0,1));
}

IPUnit createIPAmountOfSubstance() {
  return IPUnit(IPExpnt(0,0,0,0,0,0,1));
}

IPUnit createIPAngle() {
  return IPUnit(IPExpnt(0,0,0,0,0,0,0,0,1));
}

IPUnit createIPSolidAngle() {
  return IPUnit(IPExpnt(0,0,0,0,0,0,0,0,0,1));
}

IPUnit createIPPeople() {
  return IPUnit(IPExpnt(0,0,0,0,0,0,0,0,0,0,1));
}

IPUnit createIPCycle() {
  return IPUnit(IPExpnt(0,0,0,0,0,0,0,0,0,0,0,1));
}


IPUnit createIPForce() {
  return IPUnit(IPExpnt(0,0,0,0,0,0,0,1)); // lb_f
}

IPUnit createIPEnergy() {
  return IPUnit(IPExpnt(0,1,0,0,0,0,0,1)); // ft*lb_f
}

IPUnit createIPPower() {
  return IPUnit(IPExpnt(0,1,-1,0,0,0,0,1)); // ft*lb_f/s
}

IPUnit createIPElectricCharge() {
  return IPUnit(IPExpnt(0,0,1,0,1),0,"C");
}

IPUnit createIPLuminousFlux() {
  return IPUnit(IPExpnt(0,0,0,0,0,1,0,0,0,1),0,"lm");
}

IPUnit createIPIlluminance() {
  return IPUnit(IPExpnt(0,-2,0,0,0,1,0,0,0,1),0,"fc");
}

IPUnit createIPFrequency() {
  return IPUnit(IPExpnt(0,0,-1,0,0,0,0,0,0,0,0,1),0,"Hz");
}

} // openstudio

