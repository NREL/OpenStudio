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
