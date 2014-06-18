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

#ifndef UTILITIES_UNITS_TEMPERATUREUNIT_IMPL_HPP
#define UTILITIES_UNITS_TEMPERATUREUNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit_Impl.hpp"

namespace openstudio {
namespace detail {

  /** TemperatureUnit_Impl is a Unit_Impl that is the implementation class for TemperatureUnit.*/
  class UTILITIES_API TemperatureUnit_Impl : public Unit_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~TemperatureUnit_Impl() {}

    //@}
    /** @name Mathematical Operators */
    //@{

    bool isAbsolute() const;

    void setAsAbsolute();

    void setAsRelative();

    virtual void operator*=(const Unit& rUnit);

    virtual void operator/=(const Unit& rUnit);

    //@}
   protected:
    bool m_absolute;

    TemperatureUnit_Impl(bool absolute,
                         int scaleExponent,
                         const std::string& prettyString,
                         int numBaseUnits,
                         UnitSystem system);

    TemperatureUnit_Impl(bool absolute,
                         const std::string& scaleAbbreviation,
                         const std::string& prettyString,
                         int numBaseUnits,
                         UnitSystem system);
   private:
    REGISTER_LOGGER("openstudio.units.TemperatureUnit");
  };

} // detail

} // openstudio

#endif // UTILITIES_UNITS_TEMPERATUREUNIT_IMPL_HPP

