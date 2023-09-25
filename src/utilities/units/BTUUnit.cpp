/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "BTUUnit.hpp"
#include "BTUUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {

namespace detail {

  BTUUnit_Impl::BTUUnit_Impl(const BTUExpnt& exponents, int scaleExponent, const std::string& prettyString)
    : Unit_Impl(scaleExponent, prettyString, 12, UnitSystem::BTU) {
    m_units[0].first = "Btu";
    m_units[0].second = exponents.m_btu;
    m_units[1].first = "ft";
    m_units[1].second = exponents.m_ft;
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

  BTUUnit_Impl::BTUUnit_Impl(const std::string& scaleAbbreviation, const BTUExpnt& exponents, const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation, prettyString, 12, UnitSystem::BTU) {
    m_units[0].first = "Btu";
    m_units[0].second = exponents.m_btu;
    m_units[1].first = "ft";
    m_units[1].second = exponents.m_ft;
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

  Unit BTUUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::BTU);
    std::shared_ptr<BTUUnit_Impl> impl(new BTUUnit_Impl(*this));
    return BTUUnit(impl).cast<Unit>();
  }

  void BTUUnit_Impl::setBaseUnitExponent(const std::string& baseUnit, int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    } else {
      LOG_AND_THROW("Cannot add base units to an instance of BTUUnit.");
    }
  }

}  // namespace detail

BTUUnit::BTUUnit(const BTUExpnt& exponents, int scaleExponent, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::BTUUnit_Impl(exponents, scaleExponent, prettyString))) {}

BTUUnit::BTUUnit(const std::string& scaleAbbreviation, const BTUExpnt& exponents, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::BTUUnit_Impl(scaleAbbreviation, exponents, prettyString))) {}

/// @cond
BTUUnit::BTUUnit(std::shared_ptr<detail::BTUUnit_Impl> impl) : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl)) {}
/// @endcond

BTUUnit createBTUEnergy() {
  return BTUUnit(BTUExpnt(1));
}

BTUUnit createBTULength() {
  return BTUUnit(BTUExpnt(0, 1));
}

BTUUnit createBTUTime() {
  return BTUUnit(BTUExpnt(0, 0, 1));
}

BTUUnit createBTUTemperature() {
  return BTUUnit(BTUExpnt(0, 0, 0, 1));
}

BTUUnit createBTUElectricCurrent() {
  return BTUUnit(BTUExpnt(0, 0, 0, 0, 1));
}

BTUUnit createBTULuminousIntensity() {
  return BTUUnit(BTUExpnt(0, 0, 0, 0, 0, 1));
}

BTUUnit createBTUAmountOfSubstance() {
  return BTUUnit(BTUExpnt(0, 0, 0, 0, 0, 0, 1));
}

BTUUnit createBTUAngle() {
  return BTUUnit(BTUExpnt(0, 0, 0, 0, 0, 0, 0, 1));
}

BTUUnit createBTUSolidAngle() {
  return BTUUnit(BTUExpnt(0, 0, 0, 0, 0, 0, 0, 0, 1));
}

BTUUnit createBTUPeople() {
  return BTUUnit(BTUExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

BTUUnit createBTUCycle() {
  return BTUUnit(BTUExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

BTUUnit createBTUCurrency() {
  return BTUUnit(BTUExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

BTUUnit createBTUPower() {
  return BTUUnit(BTUExpnt(1, 0, -1));
}

BTUUnit createBTULuminousFlux() {
  return BTUUnit(BTUExpnt(0, 0, 0, 0, 0, 1, 0, 0, 1), 0, "lm");
}

BTUUnit createBTUIlluminance() {
  return BTUUnit(BTUExpnt(0, -2, 0, 0, 0, 1, 0, 0, 1), 0, "fc");
}

}  // namespace openstudio
