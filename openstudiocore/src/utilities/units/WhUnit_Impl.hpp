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

#ifndef UTILITIES_UNITS_WHUNIT_IMPL_HPP
#define UTILITIES_UNITS_WHUNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit_Impl.hpp"

namespace openstudio {
namespace detail {

  /** WhUnit_Impl is a Unit_Impl that is the implementation class for WhUnit.*/
  class UTILITIES_API WhUnit_Impl : public Unit_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor. Example: \verbatim
        WhUnit myEnergy(WhExpnt(1,1),3,"Wh");
        std::cout << myEnergy; // produces "kWh" \endverbatim
     *
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
     *  \param[in] prettyString optional string to use in place of standardString. */
    WhUnit_Impl(const WhExpnt& exponents=WhExpnt(),
                int scaleExponent=0,
                const std::string& prettyString="");

    /** Alternate constructor. Specify the abbreviation of the scale, rather than its
     *  exponent. Example: \verbatim
        WhUnit myEnergy("k",WhExpnt(1,1),"Wh");
        std::cout << myEnergy; // produces "kWh" \endverbatim
     *
     *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
     *    "k" for kilo.
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] prettyString optional string to use in place of standardString. */
    WhUnit_Impl(const std::string& scaleAbbreviation,
                const WhExpnt& exponents=WhExpnt(),
                const std::string& prettyString="");

    virtual ~WhUnit_Impl() {}

    /** Deep copy constructor. */
    virtual Unit clone() const;

    //@}
    /** @name Base Units and Scale */
    //@{

    /** Throws if baseUnit is not a valid for WhUnit. */
    virtual void setBaseUnitExponent(const std::string& baseUnit, int exponent);

    //@}
   private:
    REGISTER_LOGGER("openstudio.units.WhUnit");
  };

} // detail

} // openstudio

#endif // UTILITIES_UNITS_WHUNIT_IMPL_HPP

