/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_CELSIUSUNIT_HPP
#define UTILITIES_UNITS_CELSIUSUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "TemperatureUnit.hpp"

namespace openstudio {
namespace detail {

  class CelsiusUnit_Impl;

}  // namespace detail

/** CelsiusUnit is a TemperatureUnit that supports absolute temperatures in degrees Celsius.
 *  Kelvin and Celsius are the same in situations of relative temperature, that is, when it is the
 *  difference in temperatures that matter, rather than the absolute value. This is normally the
 *  case when the temperature base unit shows up in combination with other units, and that is not
 *  the situation being addressed here. This class is for reporting actual temperatures in degrees
 *  Celsius. Printing out K's as C's in the former situation is a feature that may be added to this
 *  library at a later date. */
class UTILITIES_API CelsiusUnit : public TemperatureUnit
{
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor creates an absolute temperature.
   *
   *  \param[in] CExp exponent on baseUnit C.
   *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
   *  \param[in] prettyString optional string to use in place of standardString. */
  CelsiusUnit(int CExp = 0, int scaleExponent = 0, const std::string& prettyString = "");

  /** Alternate constructor creates an absolute temperature. Specify the abbreviation of the scale,
   *  rather than its exponent.
   *
   *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
   *    "k" for kilo.
   *  \param[in] CExp exponent on baseUnit C.
   *  \param[in] prettyString optional string to use in place of standardString. */
  CelsiusUnit(const std::string& scaleAbbreviation, int CExp = 0, const std::string& prettyString = "");

  virtual ~CelsiusUnit() override = default;

  //@}
 protected:
  /// @cond
  using ImplType = detail::CelsiusUnit_Impl;

  explicit CelsiusUnit(std::shared_ptr<detail::CelsiusUnit_Impl> impl);

  friend class Unit;
  friend class detail::CelsiusUnit_Impl;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.units.CelsiusUnit");
};

/** \relates CelsiusUnit*/
using OptionalCelsiusUnit = boost::optional<CelsiusUnit>;

/** \relates CelsiusUnit*/
using CelsiusUnitVector = std::vector<CelsiusUnit>;

/** @name Create Functions Used by UnitFactory */
//@{

/** \relates CelsiusUnit */
UTILITIES_API CelsiusUnit createCelsiusTemperature();

//@}

}  // namespace openstudio

#endif  // UTILITIES_UNITS_CELSIUSUNIT_HPP
