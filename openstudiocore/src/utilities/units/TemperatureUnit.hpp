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

#ifndef UTILITIES_UNITS_TEMPERATUREUNIT_HPP
#define UTILITIES_UNITS_TEMPERATUREUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit.hpp"

namespace openstudio {
namespace detail {

  class TemperatureUnit_Impl;

} // detail

/** TemperatureUnit is an abstract Unit for handling temperatures. Temperatures can be absolute
 *  or relative. The temperature of a material is absolute temperature. Temperature differences are
 *  relative temperatures. In the former case C != K (F != R), but in the latter, C == K (F == R).
 *
 *  There are no public constructors, because this is an abstract class. By default, derived
 *  classes should set absoute=true. Temperatures that start as relative should be handled by regular
 *  Unit classes. */
class UTILITIES_API TemperatureUnit : public Unit {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~TemperatureUnit() {}

  //@}
  /** @name Mathematical Operators */
  //@{

  bool isAbsolute() const;

  void setAsAbsolute();

  void setAsRelative();

  //@}
 protected:
  /// @cond
  typedef detail::TemperatureUnit_Impl ImplType;

  explicit TemperatureUnit(std::shared_ptr<detail::TemperatureUnit_Impl> impl);

  friend class Unit;
  friend class detail::TemperatureUnit_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.units.TemperatureUnit");
};

/** \relates TemperatureUnit*/
typedef boost::optional<TemperatureUnit> OptionalTemperatureUnit;

/** \relates TemperatureUnit*/
typedef std::vector<TemperatureUnit> TemperatureUnitVector;

} // openstudio

#endif // UTILITIES_UNITS_TEMPERATUREUNIT_HPP

