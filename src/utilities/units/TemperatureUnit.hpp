/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_UNITS_TEMPERATUREUNIT_HPP
#define UTILITIES_UNITS_TEMPERATUREUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit.hpp"

namespace openstudio {
namespace detail {

  class TemperatureUnit_Impl;

}  // namespace detail

/** TemperatureUnit is an abstract Unit for handling temperatures. Temperatures can be absolute
 *  or relative. The temperature of a material is absolute temperature. Temperature differences are
 *  relative temperatures. In the former case C != K (F != R), but in the latter, C == K (F == R).
 *
 *  There are no public constructors, because this is an abstract class. By default, derived
 *  classes should set absoute=true. Temperatures that start as relative should be handled by regular
 *  Unit classes. */
class UTILITIES_API TemperatureUnit : public Unit
{
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~TemperatureUnit() = default;

  //@}
  /** @name Mathematical Operators */
  //@{

  bool isAbsolute() const;

  void setAsAbsolute();

  void setAsRelative();

  //@}
 protected:
  /// @cond
  using ImplType = detail::TemperatureUnit_Impl;

  explicit TemperatureUnit(std::shared_ptr<detail::TemperatureUnit_Impl> impl);

  friend class Unit;
  friend class detail::TemperatureUnit_Impl;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.units.TemperatureUnit");
};

/** \relates TemperatureUnit*/
using OptionalTemperatureUnit = boost::optional<TemperatureUnit>;

/** \relates TemperatureUnit*/
using TemperatureUnitVector = std::vector<TemperatureUnit>;

}  // namespace openstudio

#endif  // UTILITIES_UNITS_TEMPERATUREUNIT_HPP
