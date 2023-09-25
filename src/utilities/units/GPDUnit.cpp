/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "GPDUnit.hpp"
#include "GPDUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  GPDUnit_Impl::GPDUnit_Impl(const GPDExpnt& exponents, int scaleExponent, const std::string& prettyString)
    : Unit_Impl(scaleExponent, prettyString, 12, UnitSystem::GPD) {
    m_units[0].first = "inH_{2}O";
    m_units[0].second = exponents.m_inH2O;
    m_units[1].first = "crgal";
    m_units[1].second = exponents.m_crgal;
    m_units[2].first = "day";
    m_units[2].second = exponents.m_day;
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

  GPDUnit_Impl::GPDUnit_Impl(const std::string& scaleAbbreviation, const GPDExpnt& exponents, const std::string& prettyString)
    : Unit_Impl(scaleAbbreviation, prettyString, 12, UnitSystem::GPD) {
    m_units[0].first = "inH_{2}O";
    m_units[0].second = exponents.m_inH2O;
    m_units[1].first = "crgal";
    m_units[1].second = exponents.m_crgal;
    m_units[2].first = "day";
    m_units[2].second = exponents.m_day;
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

  Unit GPDUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::GPD);
    std::shared_ptr<GPDUnit_Impl> impl(new GPDUnit_Impl(*this));
    return GPDUnit(impl).cast<Unit>();
  }

  void GPDUnit_Impl::setBaseUnitExponent(const std::string& baseUnit, int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    } else {
      LOG_AND_THROW("Cannot add base units to an instance of GPDUnit.");
    }
  }

}  // namespace detail

GPDUnit::GPDUnit(const GPDExpnt& exponents, int scaleExponent, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::GPDUnit_Impl(exponents, scaleExponent, prettyString))) {}

GPDUnit::GPDUnit(const std::string& scaleAbbreviation, const GPDExpnt& exponents, const std::string& prettyString)
  : Unit(std::shared_ptr<detail::Unit_Impl>(new detail::GPDUnit_Impl(scaleAbbreviation, exponents, prettyString))) {}

/// @cond
GPDUnit::GPDUnit(std::shared_ptr<detail::GPDUnit_Impl> impl) : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl)) {}
/// @endcond

GPDUnit createGPDPressure() {
  return GPDUnit(GPDExpnt(1));
}

GPDUnit createGPDLength() {
  GPDUnit result;
  result.setBaseUnitExponent("crgal", 1);
  return result;
}

GPDUnit createGPDTime() {
  return GPDUnit(GPDExpnt(0, 0, 1));
}

GPDUnit createGPDTemperature() {
  return GPDUnit(GPDExpnt(0, 0, 0, 1));
}

GPDUnit createGPDElectricCurrent() {
  return GPDUnit(GPDExpnt(0, 0, 0, 0, 1));
}

GPDUnit createGPDLuminousIntensity() {
  return GPDUnit(GPDExpnt(0, 0, 0, 0, 0, 1));
}

GPDUnit createGPDAmountOfSubstance() {
  return GPDUnit(GPDExpnt(0, 0, 0, 0, 0, 0, 1));
}

GPDUnit createGPDAngle() {
  return GPDUnit(GPDExpnt(0, 0, 0, 0, 0, 0, 0, 1));
}

GPDUnit createGPDSolidAngle() {
  return GPDUnit(GPDExpnt(0, 0, 0, 0, 0, 0, 0, 0, 1));
}

GPDUnit createGPDPeople() {
  return GPDUnit(GPDExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

GPDUnit createGPDCycle() {
  return GPDUnit(GPDExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

GPDUnit createGPDCurrency() {
  return GPDUnit(GPDExpnt(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1));
}

GPDUnit createGPDVolume() {
  return GPDUnit(GPDExpnt(0, 1), 0, "gal");
}

GPDUnit createGPDVolumetricFlowrate() {
  return GPDUnit(GPDExpnt(0, 1, -1), 0, "gpd");
}

GPDUnit createGPDLuminousFlux() {
  return GPDUnit(GPDExpnt(0, 0, 0, 0, 0, 1, 0, 0, 1), 0, "lm");
}

}  // namespace openstudio
