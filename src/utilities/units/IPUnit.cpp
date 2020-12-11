/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "IPUnit.hpp"
#include "IPUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  IPUnit_Impl::IPUnit_Impl(const IPExpnt& exponents, int scaleExponent, const std::string& prettyString)
    : Unit_Impl(scaleExponent, prettyString, 13, UnitSystem::IP) {
    m_units[0].first = "lb_m";
    m_units[0].second = exponents.m_lbm;
    m_units[1].first = "ft";
    m_units[1].second = exponents.m_ft;
    m_units[2].first = "s";
    m_units[2].second = exponents.m_s;
    m_units[3].first = "R";
    m_units[3].second = exponents.m_R;
    m_units[4].first = "A";
    m_units[4].second = exponents.m_A;
    m_units[5].first = "cd";
    m_units[5].second = exponents.m_cd;
    m_units[6].first = "lbmol";
    m_units[6].second = exponents.m_lbmol;
    m_units[7].first = "lb_f";
    m_units[7].second = exponents.m_lbf;
    m_units[8].first = "deg";
    m_units[8].second = exponents.m_deg;
    m_units[9].first = "sr";
    m_units[9].second = exponents.m_sr;
    m_units[10].first = "people";
    m_units[10].second = exponents.m_people;
    m_units[11].first = "cycle";
    m_units[11].second = exponents.m_cycle;
    m_units[12].first = "$";
    m_units[12].second = exponents.m_dollar;
  }

  IPUnit_Impl::IPUnit_Impl(const std::string& scaleAbbreviation, const IPExpnt& exponents, const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation, prettyString, 13, UnitSystem::IP) {
    m_units[0].first = "lb_m";
    m_units[0].second = exponents.m_lbm;
    m_units[1].first = "ft";
    m_units[1].second = exponents.m_ft;
    m_units[2].first = "s";
    m_units[2].second = exponents.m_s;
    m_units[3].first = "R";
    m_units[3].second = exponents.m_R;
    m_units[4].first = "A";
    m_units[4].second = exponents.m_A;
    m_units[5].first = "cd";
    m_units[5].second = exponents.m_cd;
    m_units[6].first = "lbmol";
    m_units[6].second = exponents.m_lbmol;
    m_units[7].first = "lb_f";
    m_units[7].second = exponents.m_lbf;
    m_units[8].first = "deg";
    m_units[8].second = exponents.m_deg;
    m_units[9].first = "sr";
    m_units[9].second = exponents.m_sr;
    m_units[10].first = "people";
    m_units[10].second = exponents.m_people;
    m_units[11].first = "cycle";
    m_units[11].second = exponents.m_cycle;
    m_units[12].first = "$";
    m_units[12].second = exponents.m_dollar;
  }

  Unit IPUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::IP);
    std::shared_ptr<IPUnit_Impl> impl(new IPUnit_Impl(*this));
    return IPUnit(impl).cast<Unit>();
  }

  void IPUnit_Impl::setBaseUnitExponent(const std::string& baseUnit, int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    } else {
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
      m_units[2].second += 2 * m_units[0].second;
      m_units[1].second -= m_units[0].second;
      m_units[0].second = 0;
    }
    return;
  }

  void IPUnit_Impl::lbfToLbm() {
    if (m_units[7].second != 0) {
      m_units[0].second += m_units[7].second;
      m_units[1].second += m_units[7].second;
      m_units[2].second -= 2 * m_units[7].second;
      m_units[7].second = 0;
    }
    return;
  }

}  // namespace detail

IPUnit::IPUnit(const IPExpnt& exponents, int scaleExponent, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::IPUnit_Impl(exponents, scaleExponent, prettyString))) {}

IPUnit::IPUnit(const std::string& scaleAbbreviation, const IPExpnt& exponents, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::IPUnit_Impl(scaleAbbreviation, exponents, prettyString))) {}

double IPUnit::gc() {
  return 32.1740485564;  // lb_m*ft/lb_f*s^2
}

void IPUnit::lbmToLbf() {
  getImpl<detail::IPUnit_Impl>()->lbmToLbf();
}

void IPUnit::lbfToLbm() {
  getImpl<detail::IPUnit_Impl>()->lbfToLbm();
}

/// @cond
IPUnit::IPUnit(std::shared_ptr<detail::IPUnit_Impl> impl) : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl)) {}
/// @endcond

IPUnit createIPMass() {
  return IPUnit(IPExpnt(1));
}

IPUnit createIPLength() {
  return IPUnit(IPExpnt(0, 1));
}

IPUnit createIPTime() {
  return IPUnit(IPExpnt(0, 0, 1));
}

IPUnit createIPTemperature() {
  return IPUnit(IPExpnt(0, 0, 0, 1));
}

IPUnit createIPElectricCurrent() {
  return IPUnit(IPExpnt(0, 0, 0, 0, 1));
}

IPUnit createIPLuminousIntensity() {
  return IPUnit(IPExpnt(0, 0, 0, 0, 0, 1));
}

IPUnit createIPAmountOfSubstance() {
  return IPUnit(IPExpnt(0, 0, 0, 0, 0, 0, 1));
}

IPUnit createIPAngle() {
  return IPUnit(IPExpnt(0, 0, 0, 0, 0, 0, 0, 0, 1));
}

IPUnit createIPSolidAngle() {
  return IPUnit(IPExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

IPUnit createIPPeople() {
  return IPUnit(IPExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

IPUnit createIPCycle() {
  return IPUnit(IPExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

IPUnit createIPCurrency() {
  return IPUnit(IPExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

IPUnit createIPForce() {
  return IPUnit(IPExpnt(0, 0, 0, 0, 0, 0, 0, 1));  // lb_f
}

IPUnit createIPEnergy() {
  return IPUnit(IPExpnt(0, 1, 0, 0, 0, 0, 0, 1));  // ft*lb_f
}

IPUnit createIPPower() {
  return IPUnit(IPExpnt(0, 1, -1, 0, 0, 0, 0, 1));  // ft*lb_f/s
}

IPUnit createIPElectricCharge() {
  return IPUnit(IPExpnt(0, 0, 1, 0, 1), 0, "C");
}

IPUnit createIPLuminousFlux() {
  return IPUnit(IPExpnt(0, 0, 0, 0, 0, 1, 0, 0, 0, 1), 0, "lm");
}

IPUnit createIPIlluminance() {
  return IPUnit(IPExpnt(0, -2, 0, 0, 0, 1, 0, 0, 0, 1), 0, "fc");
}

IPUnit createIPFrequency() {
  return IPUnit(IPExpnt(0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 1), 0, "Hz");
}

}  // namespace openstudio
