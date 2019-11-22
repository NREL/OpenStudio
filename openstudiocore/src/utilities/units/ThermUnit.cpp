/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "ThermUnit.hpp"
#include "ThermUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  ThermUnit_Impl::ThermUnit_Impl(const ThermExpnt& exponents,
                                 int scaleExponent,
                                 const std::string& prettyString)
    : Unit_Impl(scaleExponent,prettyString,12,UnitSystem::Therm)
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
    m_units[11].first = "$"; m_units[11].second = exponents.m_dollar;
  }

  ThermUnit_Impl::ThermUnit_Impl(const std::string& scaleAbbreviation,
                                 const ThermExpnt& exponents,
                                 const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation,prettyString,12,UnitSystem::Therm)
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
    m_units[11].first = "$"; m_units[11].second = exponents.m_dollar;
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

ThermUnit createThermCurrency() {
  return ThermUnit(ThermExpnt(0,0,0,0,0,0,0,0,0,0,0,1));
}

ThermUnit createThermLuminousFlux() {
  return ThermUnit(ThermExpnt(0,0,0,0,0,1,0,0,1),0,"lm");
}

} // openstudio

