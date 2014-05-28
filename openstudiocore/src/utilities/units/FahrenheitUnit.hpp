/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef UTILITIES_UNITS_FAHRENHEITUNIT_HPP
#define UTILITIES_UNITS_FAHRENHEITUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "TemperatureUnit.hpp"

namespace openstudio {
namespace detail {

  class FahrenheitUnit_Impl;

} // detail

/** FahrenheitUnit is a TemperatureUnit that supports absolute temperatures in degrees Fahrenheit.
 *  Rankine and Fahrenheit are the same in situations of relative temperature, that is, when it is the
 *  difference in temperatures that matter, rather than the absolute value. This is normally the
 *  case when the temperature base unit shows up in combination with other units, and that is not
 *  the situation being addressed here. This class is for reporting actual temperatures in degrees
 *  Fahrenheit. Printing out R's as F's in the former situation is a feature that may be added to this
 *  library at a later date. */
class UTILITIES_API FahrenheitUnit : public TemperatureUnit {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor creates an absolute temperature.
   *
   *  \param[in] FExp exponent on baseUnit F.
   *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
   *  \param[in] prettyString optional string to use in place of standardString. */
  FahrenheitUnit(int FExp=0,int scaleExponent=0,const std::string& prettyString="");

  /** Alternate constructor creates an absolute temperature. Specify the abbreviation of the scale,
   *  rather than its exponent.
   *
   *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
   *    "k" for kilo.
   *  \param[in] FExp exponent on baseUnit F.
   *  \param[in] prettyString optional string to use in place of standardString. */
  FahrenheitUnit(const std::string& scaleAbbreviation,
                 int FExp=0,
                 const std::string& prettyString="");

  virtual ~FahrenheitUnit() {}

  //@}
  /** @name Mathematical Operators */
  //@{

  // needed to take advantage of base class *= while retaining System check
  FahrenheitUnit& operator/=(const FahrenheitUnit& rUnit);

  //@}
 protected:
  /// @cond
  typedef detail::FahrenheitUnit_Impl ImplType;

  explicit FahrenheitUnit(std::shared_ptr<detail::FahrenheitUnit_Impl> impl);

  friend class Unit;
  friend class detail::FahrenheitUnit_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.units.FahrenheitUnit");
};

/** \relates FahrenheitUnit*/
typedef boost::optional<FahrenheitUnit> OptionalFahrenheitUnit;

/** \relates FahrenheitUnit*/
typedef std::vector<FahrenheitUnit> FahrenheitUnitVector;

/** @name Create Functions Used by UnitFactory */
//@{

/** \relates FahrenheitUnit */
UTILITIES_API FahrenheitUnit createFahrenheitTemperature();

//@}

} // openstudio

#endif // UTILITIES_UNITS_FAHRENHEITUNIT_HPP

