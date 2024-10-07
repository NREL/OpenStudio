/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  virtual ~TemperatureUnit() override = default;

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
