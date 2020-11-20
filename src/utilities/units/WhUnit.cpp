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

#include "WhUnit.hpp"
#include "WhUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  WhUnit_Impl::WhUnit_Impl(const WhExpnt& exponents, int scaleExponent, const std::string& prettyString)
    : Unit_Impl(scaleExponent, prettyString, 12, UnitSystem::Wh) {
    m_units[0].first = "W";
    m_units[0].second = exponents.m_W;
    m_units[1].first = "h";
    m_units[1].second = exponents.m_h;
    m_units[2].first = "m";
    m_units[2].second = exponents.m_m;
    m_units[3].first = "K";
    m_units[3].second = exponents.m_K;
    m_units[4].first = "A";
    m_units[4].second = exponents.m_A;
    m_units[5].first = "cd";
    m_units[5].second = exponents.m_cd;
    m_units[6].first = "mol";
    m_units[6].second = exponents.m_mol;
    m_units[7].first = "rad";
    m_units[7].second = exponents.m_rad;
    m_units[8].first = "sr";
    m_units[8].second = exponents.m_sr;
    m_units[9].first = "people";
    m_units[9].second = exponents.m_people;
    m_units[10].first = "cycle";
    m_units[10].second = exponents.m_cycle;
    m_units[11].first = "$";
    m_units[11].second = exponents.m_dollar;
  }

  WhUnit_Impl::WhUnit_Impl(const std::string& scaleAbbreviation, const WhExpnt& exponents, const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation, prettyString, 12, UnitSystem::Wh) {
    m_units[0].first = "W";
    m_units[0].second = exponents.m_W;
    m_units[1].first = "h";
    m_units[1].second = exponents.m_h;
    m_units[2].first = "m";
    m_units[2].second = exponents.m_m;
    m_units[3].first = "K";
    m_units[3].second = exponents.m_K;
    m_units[4].first = "A";
    m_units[4].second = exponents.m_A;
    m_units[5].first = "cd";
    m_units[5].second = exponents.m_cd;
    m_units[6].first = "mol";
    m_units[6].second = exponents.m_mol;
    m_units[7].first = "rad";
    m_units[7].second = exponents.m_rad;
    m_units[8].first = "sr";
    m_units[8].second = exponents.m_sr;
    m_units[9].first = "people";
    m_units[9].second = exponents.m_people;
    m_units[10].first = "cycle";
    m_units[10].second = exponents.m_cycle;
    m_units[11].first = "$";
    m_units[11].second = exponents.m_dollar;
  }

  Unit WhUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::Wh);
    std::shared_ptr<WhUnit_Impl> impl(new WhUnit_Impl(*this));
    return WhUnit(impl).cast<Unit>();
  }

  void WhUnit_Impl::setBaseUnitExponent(const std::string& baseUnit, int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    } else {
      LOG_AND_THROW("Cannot add base units to an instance of WhUnit.");
    }
  }

}  // namespace detail

WhUnit::WhUnit(const WhExpnt& exponents, int scaleExponent, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::WhUnit_Impl(exponents, scaleExponent, prettyString))) {}

WhUnit::WhUnit(const std::string& scaleAbbreviation, const WhExpnt& exponents, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::WhUnit_Impl(scaleAbbreviation, exponents, prettyString))) {}

/// @cond
WhUnit::WhUnit(std::shared_ptr<detail::WhUnit_Impl> impl) : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl)) {}
/// @endcond

WhUnit createWhPower() {
  return WhUnit(WhExpnt(1));
}

WhUnit createWhTime() {
  return WhUnit(WhExpnt(0, 1));
}

WhUnit createWhLength() {
  return WhUnit(WhExpnt(0, 0, 1));
}

WhUnit createWhTemperature() {
  return WhUnit(WhExpnt(0, 0, 0, 1));
}

WhUnit createWhElectricCurrent() {
  return WhUnit(WhExpnt(0, 0, 0, 0, 1));
}

WhUnit createWhLuminousIntensity() {
  return WhUnit(WhExpnt(0, 0, 0, 0, 0, 1));
}

WhUnit createWhAmountOfSubstance() {
  return WhUnit(WhExpnt(0, 0, 0, 0, 0, 0, 1));
}

WhUnit createWhAngle() {
  return WhUnit(WhExpnt(0, 0, 0, 0, 0, 0, 0, 1));
}

WhUnit createWhSolidAngle() {
  return WhUnit(WhExpnt(0, 0, 0, 0, 0, 0, 0, 0, 1));
}

WhUnit createWhPeople() {
  return WhUnit(WhExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

WhUnit createWhCycle() {
  return WhUnit(WhExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

WhUnit createWhCurrency() {
  return WhUnit(WhExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

WhUnit createWhEnergy() {
  return WhUnit(WhExpnt(1, 1), 0, "Wh");
}

WhUnit createWhElectricalPotential() {
  return WhUnit(WhExpnt(1, 0, 0, 0, -1), 0, "V");
}

WhUnit createWhElectricResistance() {
  return WhUnit(WhExpnt(1, 0, 0, 0, -2), 0, "ohm");
}

WhUnit createWhLuminousFlux() {
  return WhUnit(WhExpnt(0, 0, 0, 0, 0, 1, 0, 0, 1), 0, "lm");
}

WhUnit createWhIlluminance() {
  return WhUnit(WhExpnt(0, 0, -2, 0, 0, 1, 0, 0, 1), 0, "lux");
}

}  // namespace openstudio
