/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "FahrenheitUnit.hpp"
#include "FahrenheitUnit_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
namespace detail {

  FahrenheitUnit_Impl::FahrenheitUnit_Impl(int FExp,
                                           int scaleExponent,
                                           const std::string& prettyString)
    : TemperatureUnit_Impl(true,scaleExponent,prettyString,1,UnitSystem::Fahrenheit)
  {
    m_units[0].first = "F"; m_units[0].second = FExp;
  }

  FahrenheitUnit_Impl::FahrenheitUnit_Impl(const std::string& scaleAbbreviation,
                                           int FExp,
                                           const std::string& prettyString)
    : TemperatureUnit_Impl(true,scaleAbbreviation,prettyString,1,UnitSystem::Fahrenheit)
  {
    m_units[0].first = "F"; m_units[0].second = FExp;
  }

  Unit FahrenheitUnit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::Fahrenheit);
    std::shared_ptr<FahrenheitUnit_Impl> impl(new FahrenheitUnit_Impl(*this));
    return FahrenheitUnit(impl).cast<Unit>();
  }

  void FahrenheitUnit_Impl::setBaseUnitExponent(const std::string& baseUnit,int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    }
    else {
      LOG_AND_THROW("Cannot add base units to an instance of FahrenheitUnit.");
    }
  }

} // detail

FahrenheitUnit::FahrenheitUnit(int FExp,int scaleExponent,const std::string& prettyString)
  : TemperatureUnit(std::shared_ptr<detail::FahrenheitUnit_Impl>(
                        new detail::FahrenheitUnit_Impl(FExp,scaleExponent,prettyString)))
{}

FahrenheitUnit::FahrenheitUnit(const std::string& scaleAbbreviation,
                               int FExp,
                               const std::string& prettyString)
  : TemperatureUnit(std::shared_ptr<detail::FahrenheitUnit_Impl>(
                        new detail::FahrenheitUnit_Impl(scaleAbbreviation,FExp,prettyString)))
{}

FahrenheitUnit& FahrenheitUnit::operator/=(const FahrenheitUnit& rUnit) {
  getImpl<detail::FahrenheitUnit_Impl>()->operator/=(rUnit);
  return *this;
}

/// @cond
FahrenheitUnit::FahrenheitUnit(std::shared_ptr<detail::FahrenheitUnit_Impl> impl)
  : TemperatureUnit(impl)
{}
/// @endcond

FahrenheitUnit createFahrenheitTemperature() {
  return FahrenheitUnit(1);
}

} // openstudio

