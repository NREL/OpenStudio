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

#ifndef UTILITIES_UNITS_CELSIUSUNIT_IMPL_HPP
#define UTILITIES_UNITS_CELSIUSUNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "TemperatureUnit_Impl.hpp"

namespace openstudio {
namespace detail {

  /** CelsiusUnit_Impl is a TemperatureUnit_Impl that is the implementation class for CelsiusUnit.*/
  class UTILITIES_API CelsiusUnit_Impl : public TemperatureUnit_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor creates an absolute temperature.
     *
     *  \param[in] CExp exponent on baseUnit C.
     *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
     *  \param[in] prettyString optional string to use in place of standardString. */
    CelsiusUnit_Impl(int CExp=0,int scaleExponent=0,const std::string& prettyString="");

    /** Alternate constructor creates an absolute temperature. Specify the abbreviation of the scale,
     *  rather than its exponent.
     *
     *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
     *    "k" for kilo.
     *  \param[in] CExp exponent on baseUnit C.
     *  \param[in] prettyString optional string to use in place of standardString. */
    CelsiusUnit_Impl(const std::string& scaleAbbreviation,
                     int CExp=0,
                     const std::string& prettyString="");

    virtual ~CelsiusUnit_Impl() {}

    /** Deep copy constructor. */
    virtual Unit clone() const;

    //@}
    /** @name Base Units and Scale */
    //@{

    /** Throws if baseUnit is not a valid for CelsiusUnit. */
    virtual void setBaseUnitExponent(const std::string& baseUnit, int exponent);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.units.CelsiusUnit");
  };

} // detail

} // openstudio

#endif // UTILITIES_UNITS_CELSIUSUNIT_IMPL_HPP

