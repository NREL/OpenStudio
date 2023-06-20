/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "MPHUnit.hpp"
#include "MPHUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  MPHUnit_Impl::MPHUnit_Impl(const MPHExpnt& exponents, int scaleExponent, const std::string& prettyString)
    : Unit_Impl(scaleExponent, prettyString, 12, UnitSystem::MPH) {
    m_units[0].first = "inHg";
    m_units[0].second = exponents.m_inHg;
    m_units[1].first = "mi";
    m_units[1].second = exponents.m_mi;
    m_units[2].first = "h";
    m_units[2].second = exponents.m_h;
    m_units[3].first = "R";
    m_units[3].second = exponents.m_R;
    m_units[4].first = "A";
    m_units[4].second = exponents.m_A;
    m_units[5].first = "cd";
    m_units[5].second = exponents.m_cd;
    m_units[6].first = "lbmol";
    m_units[6].second = exponents.m_lbmol;
    m_units[7].first = "deg";
    m_units[7].second = exponents.m_deg;
    m_units[8].first = "sr";
    m_units[8].second = exponents.m_sr;
    m_units[9].first = "people";
    m_units[9].second = exponents.m_people;
    m_units[10].first = "cycle";
    m_units[10].second = exponents.m_cycle;
    m_units[11].first = "$";
    m_units[11].second = exponents.m_dollar;
  }

  MPHUnit_Impl::MPHUnit_Impl(const std::string& scaleAbbreviation, const MPHExpnt& exponents, const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation, prettyString, 12, UnitSystem::MPH) {
    m_units[0].first = "inHg";
    m_units[0].second = exponents.m_inHg;
    m_units[1].first = "mi";
    m_units[1].second = exponents.m_mi;
    m_units[2].first = "h";
    m_units[2].second = exponents.m_h;
    m_units[3].first = "R";
    m_units[3].second = exponents.m_R;
    m_units[4].first = "A";
    m_units[4].second = exponents.m_A;
    m_units[5].first = "cd";
    m_units[5].second = exponents.m_cd;
    m_units[6].first = "lbmol";
    m_units[6].second = exponents.m_lbmol;
    m_units[7].first = "deg";
    m_units[7].second = exponents.m_deg;
    m_units[8].first = "sr";
    m_units[8].second = exponents.m_sr;
    m_units[9].first = "people";
    m_units[9].second = exponents.m_people;
    m_units[10].first = "cycle";
    m_units[10].second = exponents.m_cycle;
    m_units[11].first = "$";
    m_units[11].second = exponents.m_dollar;
  }

  Unit MPHUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::MPH);
    std::shared_ptr<MPHUnit_Impl> impl(new MPHUnit_Impl(*this));
    return MPHUnit(impl).cast<Unit>();
  }

  void MPHUnit_Impl::setBaseUnitExponent(const std::string& baseUnit, int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    } else {
      LOG_AND_THROW("Cannot add base units to an instance of MPHUnit.");
    }
  }

}  // namespace detail

MPHUnit::MPHUnit(const MPHExpnt& exponents, int scaleExponent, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::MPHUnit_Impl(exponents, scaleExponent, prettyString))) {}

MPHUnit::MPHUnit(const std::string& scaleAbbreviation, const MPHExpnt& exponents, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::MPHUnit_Impl(scaleAbbreviation, exponents, prettyString))) {}

/// @cond
MPHUnit::MPHUnit(std::shared_ptr<detail::MPHUnit_Impl> impl) : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl)) {}
/// @endcond

MPHUnit createMPHPressure() {
  return MPHUnit(MPHExpnt(1));
}

MPHUnit createMPHLength() {
  return MPHUnit(MPHExpnt(0, 1));
}

MPHUnit createMPHTime() {
  return MPHUnit(MPHExpnt(0, 0, 1));
}

MPHUnit createMPHTemperature() {
  return MPHUnit(MPHExpnt(0, 0, 0, 1));
}

MPHUnit createMPHElectricCurrent() {
  return MPHUnit(MPHExpnt(0, 0, 0, 0, 1));
}

MPHUnit createMPHLuminousIntensity() {
  return MPHUnit(MPHExpnt(0, 0, 0, 0, 0, 1));
}

MPHUnit createMPHAmountOfSubstance() {
  return MPHUnit(MPHExpnt(0, 0, 0, 0, 0, 0, 1));
}

MPHUnit createMPHAngle() {
  return MPHUnit(MPHExpnt(0, 0, 0, 0, 0, 0, 0, 1));
}

MPHUnit createMPHSolidAngle() {
  return MPHUnit(MPHExpnt(0, 0, 0, 0, 0, 0, 0, 0, 1));
}

MPHUnit createMPHPeople() {
  return MPHUnit(MPHExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

MPHUnit createMPHCycle() {
  return MPHUnit(MPHExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

MPHUnit createMPHCurrency() {
  return MPHUnit(MPHExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

MPHUnit createMPHVelocity() {
  return MPHUnit(MPHExpnt(0, 1, -1), 0, "mph");
}

MPHUnit createMPHLuminousFlux() {
  return MPHUnit(MPHExpnt(0, 0, 0, 0, 0, 1, 0, 0, 1), 0, "lm");
}

}  // namespace openstudio
