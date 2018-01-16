/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_UNITS_CELSIUSUNIT_HPP
#define UTILITIES_UNITS_CELSIUSUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "TemperatureUnit.hpp"

namespace openstudio {
namespace detail {

  class CelsiusUnit_Impl;

} // detail

/** CelsiusUnit is a TemperatureUnit that supports absolute temperatures in degrees Celsius.
 *  Kelvin and Celsius are the same in situations of relative temperature, that is, when it is the
 *  difference in temperatures that matter, rather than the absolute value. This is normally the
 *  case when the temperature base unit shows up in combination with other units, and that is not
 *  the situation being addressed here. This class is for reporting actual temperatures in degrees
 *  Celsius. Printing out K's as C's in the former situation is a feature that may be added to this
 *  library at a later date. */
class UTILITIES_API CelsiusUnit : public TemperatureUnit {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor creates an absolute temperature.
   *
   *  \param[in] CExp exponent on baseUnit C.
   *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
   *  \param[in] prettyString optional string to use in place of standardString. */
  CelsiusUnit(int CExp=0,int scaleExponent=0,const std::string& prettyString="");

  /** Alternate constructor creates an absolute temperature. Specify the abbreviation of the scale,
   *  rather than its exponent.
   *
   *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
   *    "k" for kilo.
   *  \param[in] CExp exponent on baseUnit C.
   *  \param[in] prettyString optional string to use in place of standardString. */
  CelsiusUnit(const std::string& scaleAbbreviation,
              int CExp=0,
              const std::string& prettyString="");

  virtual ~CelsiusUnit() {}

  //@}
 protected:
  /// @cond
  typedef detail::CelsiusUnit_Impl ImplType;

  explicit CelsiusUnit(std::shared_ptr<detail::CelsiusUnit_Impl> impl);

  friend class Unit;
  friend class detail::CelsiusUnit_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.units.CelsiusUnit");
};

/** \relates CelsiusUnit*/
typedef boost::optional<CelsiusUnit> OptionalCelsiusUnit;

/** \relates CelsiusUnit*/
typedef std::vector<CelsiusUnit> CelsiusUnitVector;

/** @name Create Functions Used by UnitFactory */
//@{

/** \relates CelsiusUnit */
UTILITIES_API CelsiusUnit createCelsiusTemperature();

//@}

} // openstudio

#endif // UTILITIES_UNITS_CELSIUSUNIT_HPP

