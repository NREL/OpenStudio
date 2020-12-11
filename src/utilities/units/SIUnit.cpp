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

#include "SIUnit.hpp"
#include "SIUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  SIUnit_Impl::SIUnit_Impl(const SIExpnt& exponents, int scaleExponent, const std::string& prettyString)
    : Unit_Impl(scaleExponent, prettyString, 12, UnitSystem::SI) {
    m_units[0].first = "kg";
    m_units[0].second = exponents.m_kg;
    m_units[1].first = "m";
    m_units[1].second = exponents.m_m;
    m_units[2].first = "s";
    m_units[2].second = exponents.m_s;
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

  SIUnit_Impl::SIUnit_Impl(const std::string& scaleAbbreviation, const SIExpnt& exponents, const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation, prettyString, 12, UnitSystem::SI) {
    m_units[0].first = "kg";
    m_units[0].second = exponents.m_kg;
    m_units[1].first = "m";
    m_units[1].second = exponents.m_m;
    m_units[2].first = "s";
    m_units[2].second = exponents.m_s;
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

  Unit SIUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::SI);
    std::shared_ptr<SIUnit_Impl> impl(new SIUnit_Impl(*this));
    return SIUnit(impl).cast<Unit>();
  }

  void SIUnit_Impl::setBaseUnitExponent(const std::string& baseUnit, int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    } else {
      LOG_AND_THROW("Cannot add baseUnit " << baseUnit << " to an instance of SIUnit.");
    }
  }

}  // namespace detail

SIUnit::SIUnit(const SIExpnt& exponents, int scaleExponent, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::SIUnit_Impl(exponents, scaleExponent, prettyString))) {}

SIUnit::SIUnit(const std::string& scaleAbbreviation, const SIExpnt& exponents, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::SIUnit_Impl(scaleAbbreviation, exponents, prettyString))) {}

/// @cond
SIUnit::SIUnit(std::shared_ptr<detail::SIUnit_Impl> impl) : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl)) {}
/// @endcond

SIUnit createSIMass() {
  return SIUnit(SIExpnt(1));
}
SIUnit createSILength() {
  return SIUnit(SIExpnt(0, 1));
}
SIUnit createSITime() {
  return SIUnit(SIExpnt(0, 0, 1));
}
SIUnit createSITemperature() {
  return SIUnit(SIExpnt(0, 0, 0, 1));
}

SIUnit createSIElectricCurrent() {
  return SIUnit(SIExpnt(0, 0, 0, 0, 1));
}

SIUnit createSILuminousIntensity() {
  return SIUnit(SIExpnt(0, 0, 0, 0, 0, 1));
}

SIUnit createSIAmountOfSubstance() {
  return SIUnit(SIExpnt(0, 0, 0, 0, 0, 0, 1));
}

SIUnit createSIAngle() {
  return SIUnit(SIExpnt(0, 0, 0, 0, 0, 0, 0, 1));
}

SIUnit createSISolidAngle() {
  return SIUnit(SIExpnt(0, 0, 0, 0, 0, 0, 0, 0, 1));
}

SIUnit createSIPeople() {
  return SIUnit(SIExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

SIUnit createSICycle() {
  return SIUnit(SIExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

SIUnit createSICurrency() {
  return SIUnit(SIExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

SIUnit createSIForce() {
  return SIUnit(SIExpnt(1, 1, -2), 0, "N");
}

SIUnit createSIEnergy() {
  return SIUnit(SIExpnt(1, 2, -2), 0, "J");
}

SIUnit createSIPower() {
  return SIUnit(SIExpnt(1, 2, -3), 0, "W");
}

SIUnit createSIElectricCharge() {
  return SIUnit(SIExpnt(0, 0, 1, 0, 1), 0, "C");
}

SIUnit createSIElectricalPotential() {
  return SIUnit(SIExpnt(1, 2, -3, 0, -1), 0, "V");
}

SIUnit createSIElectricCapacitance() {
  return SIUnit(SIExpnt(-1, -2, 4, 0, 2), 0, "F");
}

SIUnit createSIElectricResistance() {
  return SIUnit(SIExpnt(1, 2, -3, 0, -2), 0, "ohm");
}

SIUnit createSIMagneticFlux() {
  return SIUnit(SIExpnt(1, 2, -2, 0, -1), 0, "Wb");
}

SIUnit createSIMagneticFieldStrength() {
  return SIUnit(SIExpnt(1, 0, -2, 0, -1), 0, "T");
}

SIUnit createSIInductance() {
  return SIUnit(SIExpnt(1, 2, -2, 0, -2), 0, "H");
}

SIUnit createSILuminousFlux() {
  return SIUnit(SIExpnt(0, 0, 0, 0, 0, 1, 0, 0, 1), 0, "lm");
}

SIUnit createSIIlluminance() {
  return SIUnit(SIExpnt(0, -2, 0, 0, 0, 1, 0, 0, 1), 0, "lux");
}

SIUnit createSIFrequency() {
  return SIUnit(SIExpnt(0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 1), 0, "Hz");
}

SIUnit createSIEnergyUseIntensity() {
  return SIUnit(SIExpnt(1, 0, -2), 0, "J/m^2");
}

SIUnit createSIPowerDensity() {
  return SIUnit(SIExpnt(1, 0, -3), 0, "W/m^2");
}

SIUnit createSIPowerPerPerson() {
  return SIUnit(SIExpnt(1, 2, -3, 0, 0, 0, 0, 0, 0, -1), 0, "W/person");
}

SIUnit createSIPressure() {
  return SIUnit(SIExpnt(1, -1, -2), 0, "Pa");
}

SIUnit createSIThermalConductance() {
  return SIUnit(SIExpnt(1, 0, -3, -1), 0, "W/m^2*K");
}

SIUnit createSIThermalResistance() {
  return SIUnit(SIExpnt(-1, 0, 3, 1), 0, "m^2*K/W");
}

SIUnit createSIHeatCapacity() {
  return SIUnit(SIExpnt(1, 0, -2, -1), 0, "J/m^2*K");
}

}  // namespace openstudio
