/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Misc1Unit.hpp"
#include "Misc1Unit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  Misc1Unit_Impl::Misc1Unit_Impl(const Misc1Expnt& exponents, int scaleExponent, const std::string& prettyString)
    : Unit_Impl(scaleExponent, prettyString, 12, UnitSystem::Misc1) {
    m_units[0].first = "ftH_{2}O";
    m_units[0].second = exponents.m_ftH2O;
    m_units[1].first = "crL";
    m_units[1].second = exponents.m_crL;
    m_units[2].first = "day";
    m_units[2].second = exponents.m_day;
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

  Misc1Unit_Impl::Misc1Unit_Impl(const std::string& scaleAbbreviation, const Misc1Expnt& exponents, const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation, prettyString, 12, UnitSystem::Misc1) {
    m_units[0].first = "ftH_{2}O";
    m_units[0].second = exponents.m_ftH2O;
    m_units[1].first = "crL";
    m_units[1].second = exponents.m_crL;
    m_units[2].first = "day";
    m_units[2].second = exponents.m_day;
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

  Unit Misc1Unit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::Misc1);
    std::shared_ptr<Misc1Unit_Impl> impl(new Misc1Unit_Impl(*this));
    return Misc1Unit(impl).cast<Unit>();
  }

  void Misc1Unit_Impl::setBaseUnitExponent(const std::string& baseUnit, int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    } else {
      LOG_AND_THROW("Cannot add base units to an instance of Misc1Unit.");
    }
  }

}  // namespace detail

Misc1Unit::Misc1Unit(const Misc1Expnt& exponents, int scaleExponent, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::Misc1Unit_Impl(exponents, scaleExponent, prettyString))) {}

Misc1Unit::Misc1Unit(const std::string& scaleAbbreviation, const Misc1Expnt& exponents, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::Misc1Unit_Impl(scaleAbbreviation, exponents, prettyString))) {}

/// @cond
Misc1Unit::Misc1Unit(std::shared_ptr<detail::Misc1Unit_Impl> impl) : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl)) {}
/// @endcond

Misc1Unit createMisc1Pressure() {
  return Misc1Unit(Misc1Expnt(1));
}

Misc1Unit createMisc1Length() {
  Misc1Unit result;
  result.setBaseUnitExponent("crL", 1);
  return result;
}

Misc1Unit createMisc1Time() {
  return Misc1Unit(Misc1Expnt(0, 0, 1));
}

Misc1Unit createMisc1Temperature() {
  return Misc1Unit(Misc1Expnt(0, 0, 0, 1));
}

Misc1Unit createMisc1ElectricCurrent() {
  return Misc1Unit(Misc1Expnt(0, 0, 0, 0, 1));
}

Misc1Unit createMisc1LuminousIntensity() {
  return Misc1Unit(Misc1Expnt(0, 0, 0, 0, 0, 1));
}

Misc1Unit createMisc1AmountOfSubstance() {
  return Misc1Unit(Misc1Expnt(0, 0, 0, 0, 0, 0, 1));
}

Misc1Unit createMisc1Angle() {
  return Misc1Unit(Misc1Expnt(0, 0, 0, 0, 0, 0, 0, 1));
}

Misc1Unit createMisc1SolidAngle() {
  return Misc1Unit(Misc1Expnt(0, 0, 0, 0, 0, 0, 0, 0, 1));
}

Misc1Unit createMisc1People() {
  return Misc1Unit(Misc1Expnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

Misc1Unit createMisc1Cycle() {
  return Misc1Unit(Misc1Expnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

Misc1Unit createMisc1Currency() {
  return Misc1Unit(Misc1Expnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

Misc1Unit createMisc1Volume() {
  return Misc1Unit(Misc1Expnt(0, 1), 0, "L");
}

Misc1Unit createMisc1LuminousFlux() {
  return Misc1Unit(Misc1Expnt(0, 0, 0, 0, 0, 1, 0, 0, 1), 0, "lm");
}

}  // namespace openstudio
