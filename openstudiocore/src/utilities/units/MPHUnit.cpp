/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "MPHUnit.hpp"
#include "MPHUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  MPHUnit_Impl::MPHUnit_Impl(const MPHExpnt& exponents,
                             int scaleExponent,
                             const std::string& prettyString)
    : Unit_Impl(scaleExponent,prettyString,12,UnitSystem::MPH)
  {
    m_units[0].first = "inHg"; m_units[0].second = exponents.m_inHg;
    m_units[1].first = "mi"; m_units[1].second = exponents.m_mi;
    m_units[2].first = "h"; m_units[2].second = exponents.m_h;
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

  MPHUnit_Impl::MPHUnit_Impl(const std::string &scaleAbbreviation,
                             const MPHExpnt &exponents,
                             const std::string &prettyString)
    : Unit_Impl(scaleAbbreviation,prettyString,12,UnitSystem::MPH)
  {
    m_units[0].first = "inHg"; m_units[0].second = exponents.m_inHg;
    m_units[1].first = "mi"; m_units[1].second = exponents.m_mi;
    m_units[2].first = "h"; m_units[2].second = exponents.m_h;
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

  Unit MPHUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::MPH);
    std::shared_ptr<MPHUnit_Impl> impl(new MPHUnit_Impl(*this));
    return MPHUnit(impl).cast<Unit>();
  }

  void MPHUnit_Impl::setBaseUnitExponent(const std::string& baseUnit,int exponent)
  {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    }
    else {
      LOG_AND_THROW("Cannot add base units to an instance of MPHUnit.");
    }
  }

} // detail

MPHUnit::MPHUnit(const MPHExpnt& exponents,
                 int scaleExponent,
                 const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(
             new detail::MPHUnit_Impl(exponents,scaleExponent,prettyString)))
{}

MPHUnit::MPHUnit(const std::string& scaleAbbreviation,
                 const MPHExpnt& exponents,
                 const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(
             new detail::MPHUnit_Impl(scaleAbbreviation,exponents,prettyString)))
{}

/// @cond
MPHUnit::MPHUnit(std::shared_ptr<detail::MPHUnit_Impl> impl)
  : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl))
{}
/// @endcond

MPHUnit createMPHPressure() {
  return MPHUnit(MPHExpnt(1));
}

MPHUnit createMPHLength() {
  return MPHUnit(MPHExpnt(0,1));
}

MPHUnit createMPHTime() {
  return MPHUnit(MPHExpnt(0,0,1));
}

MPHUnit createMPHTemperature() {
  return MPHUnit(MPHExpnt(0,0,0,1));
}

MPHUnit createMPHElectricCurrent() {
  return MPHUnit(MPHExpnt(0,0,0,0,1));
}

MPHUnit createMPHLuminousIntensity() {
  return MPHUnit(MPHExpnt(0,0,0,0,0,1));
}

MPHUnit createMPHAmountOfSubstance() {
  return MPHUnit(MPHExpnt(0,0,0,0,0,0,1));
}

MPHUnit createMPHAngle() {
  return MPHUnit(MPHExpnt(0,0,0,0,0,0,0,1));
}

MPHUnit createMPHSolidAngle() {
  return MPHUnit(MPHExpnt(0,0,0,0,0,0,0,0,1));
}

MPHUnit createMPHPeople() {
  return MPHUnit(MPHExpnt(0,0,0,0,0,0,0,0,0,1));
}

MPHUnit createMPHCycle() {
  return MPHUnit(MPHExpnt(0,0,0,0,0,0,0,0,0,0,1));
}

MPHUnit createMPHCurrency() {
  return MPHUnit(MPHExpnt(0,0,0,0,0,0,0,0,0,0,0,1));
}

MPHUnit createMPHVelocity() {
  return MPHUnit(MPHExpnt(0,1,-1),0,"mph");
}

MPHUnit createMPHLuminousFlux() {
  return MPHUnit(MPHExpnt(0,0,0,0,0,1,0,0,1),0,"lm");
}

} // openstudio

