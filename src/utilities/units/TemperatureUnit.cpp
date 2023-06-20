/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "TemperatureUnit.hpp"
#include "TemperatureUnit_Impl.hpp"

namespace openstudio {
namespace detail {

  bool TemperatureUnit_Impl::isAbsolute() const {
    return m_absolute;
  }

  void TemperatureUnit_Impl::setAsAbsolute() {
    m_absolute = true;
  }

  void TemperatureUnit_Impl::setAsRelative() {
    m_absolute = false;
  }

  bool TemperatureUnit_Impl::operator==(const Unit& rUnit) const {
    bool result = Unit_Impl::operator==(rUnit);
    if (result) {
      if (rUnit.optionalCast<TemperatureUnit>()) {
        result = (this->isAbsolute() == rUnit.cast<TemperatureUnit>().isAbsolute());
      } else {
        result = false;
      }
    }
    return result;
  }

  void TemperatureUnit_Impl::operator*=(const Unit& rUnit) {
    Unit_Impl::operator*=(rUnit);
    OptionalTemperatureUnit rTUnit = rUnit.optionalCast<TemperatureUnit>();
    if (!isAbsolute() && rTUnit && rTUnit->isAbsolute()) {
      setAsAbsolute();
    }
  }

  void TemperatureUnit_Impl::operator/=(const Unit& rUnit) {
    Unit_Impl::operator/=(rUnit);
    OptionalTemperatureUnit rTUnit = rUnit.optionalCast<TemperatureUnit>();
    if (!isAbsolute() && rTUnit && rTUnit->isAbsolute()) {
      setAsAbsolute();
    }
  }

  TemperatureUnit_Impl::TemperatureUnit_Impl(bool absolute, int scaleExponent, const std::string& prettyString, int numBaseUnits, UnitSystem system)
    : Unit_Impl(scaleExponent, prettyString, numBaseUnits, system), m_absolute(absolute) {}

  TemperatureUnit_Impl::TemperatureUnit_Impl(bool absolute, const std::string& scaleAbbreviation, const std::string& prettyString, int numBaseUnits,
                                             UnitSystem system)
    : Unit_Impl(scaleAbbreviation, prettyString, numBaseUnits, system), m_absolute(absolute) {}

}  // namespace detail

bool TemperatureUnit::isAbsolute() const {
  return getImpl<detail::TemperatureUnit_Impl>()->isAbsolute();
}

void TemperatureUnit::setAsAbsolute() {
  getImpl<detail::TemperatureUnit_Impl>()->setAsAbsolute();
}

void TemperatureUnit::setAsRelative() {
  getImpl<detail::TemperatureUnit_Impl>()->setAsRelative();
}

/// @cond
TemperatureUnit::TemperatureUnit(std::shared_ptr<detail::TemperatureUnit_Impl> impl) : Unit(std::dynamic_pointer_cast<detail::Unit_Impl>(impl)) {}
/// @endcond

}  // namespace openstudio
