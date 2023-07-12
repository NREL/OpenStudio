/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FahrenheitUnit.hpp"
#include "FahrenheitUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  FahrenheitUnit_Impl::FahrenheitUnit_Impl(int FExp, int scaleExponent, const std::string& prettyString)
    : TemperatureUnit_Impl(true, scaleExponent, prettyString, 1, UnitSystem::Fahrenheit) {
    m_units[0].first = "F";
    m_units[0].second = FExp;
  }

  FahrenheitUnit_Impl::FahrenheitUnit_Impl(const std::string& scaleAbbreviation, int FExp, const std::string& prettyString)
    : TemperatureUnit_Impl(true, scaleAbbreviation, prettyString, 1, UnitSystem::Fahrenheit) {
    m_units[0].first = "F";
    m_units[0].second = FExp;
  }

  Unit FahrenheitUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::Fahrenheit);
    std::shared_ptr<FahrenheitUnit_Impl> impl(new FahrenheitUnit_Impl(*this));
    return FahrenheitUnit(impl).cast<Unit>();
  }

  void FahrenheitUnit_Impl::setBaseUnitExponent(const std::string& baseUnit, int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    } else {
      LOG_AND_THROW("Cannot add base units to an instance of FahrenheitUnit.");
    }
  }

}  // namespace detail

FahrenheitUnit::FahrenheitUnit(int FExp, int scaleExponent, const std::string& prettyString)
  : TemperatureUnit(std::shared_ptr<detail::FahrenheitUnit_Impl>(new detail::FahrenheitUnit_Impl(FExp, scaleExponent, prettyString))) {}

FahrenheitUnit::FahrenheitUnit(const std::string& scaleAbbreviation, int FExp, const std::string& prettyString)
  : TemperatureUnit(std::shared_ptr<detail::FahrenheitUnit_Impl>(new detail::FahrenheitUnit_Impl(scaleAbbreviation, FExp, prettyString))) {}

FahrenheitUnit& FahrenheitUnit::operator/=(const FahrenheitUnit& rUnit) {
  getImpl<detail::FahrenheitUnit_Impl>()->operator/=(rUnit);
  return *this;
}

/// @cond
FahrenheitUnit::FahrenheitUnit(std::shared_ptr<detail::FahrenheitUnit_Impl> impl) : TemperatureUnit(impl) {}
/// @endcond

FahrenheitUnit createFahrenheitTemperature() {
  return FahrenheitUnit(1);
}

}  // namespace openstudio
