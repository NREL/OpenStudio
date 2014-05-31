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

#ifndef UTILITIES_UNITS_SIUNIT_IMPL_HPP
#define UTILITIES_UNITS_SIUNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit_Impl.hpp"
#include "SIUnit.hpp"

namespace openstudio {
namespace detail {

  /** SIUnit_Impl is a Unit_Impl that is the implementation class for SIUnit.*/
  class UTILITIES_API SIUnit_Impl : public Unit_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor. Example: \verbatim
        SIUnit myEnergy(SIExpnt(1,2,-2),3,"J");
        std::cout << myEnergy; // produces "kJ" \endverbatim
     *
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
     *  \param[in] prettyString optional string to use in place of standardString. */
    SIUnit_Impl(const SIExpnt& exponents=SIExpnt(),
                int scaleExponent=0,
                const std::string& prettyString="");

    /** Alternate constructor. Specify the abbreviation of the scale, rather than its
     *  exponent. Example: \verbatim
        SIUnit myEnergy("k",SIExpnt(1,2,-2),"J");
        std::cout << myEnergy; // produces "kJ \endverbatim
     *
     *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
     *    "k" for kilo.
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] prettyString optional string to use in place of standardString. */
    SIUnit_Impl(const std::string& scaleAbbreviation,
                const SIExpnt& exponents=SIExpnt(),
                const std::string& prettyString="");

    virtual ~SIUnit_Impl() {}

    /** Deep copy constructor. */
    virtual Unit clone() const;

    //@}
    /** @name Base Units and Scale */
    //@{

    /** Throws if baseUnit is not a valid for SIUnit. */
    virtual void setBaseUnitExponent(const std::string& baseUnit, int exponent);

    //@}
   private:
    REGISTER_LOGGER("openstudio.units.SIUnit");
  };

} // detail
} // openstudio

#endif // UTILITIES_UNITS_SIUNIT_IMPL_HPP

