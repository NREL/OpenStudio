/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "CelsiusUnit.hpp"
#include "CelsiusUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  CelsiusUnit_Impl::CelsiusUnit_Impl(int CExp, int scaleExponent, const std::string& prettyString)
    : TemperatureUnit_Impl(true, scaleExponent, prettyString, 1, UnitSystem::Celsius) {
    m_units[0].first = "C";
    m_units[0].second = CExp;
  }

  CelsiusUnit_Impl::CelsiusUnit_Impl(const std::string& scaleAbbreviation, int CExp, const std::string& prettyString)
    : TemperatureUnit_Impl(true, scaleAbbreviation, prettyString, 1, UnitSystem::Celsius) {
    m_units[0].first = "C";
    m_units[0].second = CExp;
  }

  Unit CelsiusUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::Celsius);
    std::shared_ptr<CelsiusUnit_Impl> impl(new CelsiusUnit_Impl(*this));
    return CelsiusUnit(impl).cast<Unit>();
  }

  void CelsiusUnit_Impl::setBaseUnitExponent(const std::string& baseUnit, int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    } else {
      LOG_AND_THROW("Cannot add base units to an instance of CelsiusUnit.");
    }
  }

}  // namespace detail

CelsiusUnit::CelsiusUnit(int CExp, int scaleExponent, const std::string& prettyString)
  : TemperatureUnit(std::shared_ptr<detail::CelsiusUnit_Impl>(new detail::CelsiusUnit_Impl(CExp, scaleExponent, prettyString))) {}

CelsiusUnit::CelsiusUnit(const std::string& scaleAbbreviation, int CExp, const std::string& prettyString)
  : TemperatureUnit(std::shared_ptr<detail::CelsiusUnit_Impl>(new detail::CelsiusUnit_Impl(scaleAbbreviation, CExp, prettyString))) {}

/// @cond
CelsiusUnit::CelsiusUnit(std::shared_ptr<detail::CelsiusUnit_Impl> impl) : TemperatureUnit(impl) {}
/// @endcond

CelsiusUnit createCelsiusTemperature() {
  return CelsiusUnit(1);
}

}  // namespace openstudio
