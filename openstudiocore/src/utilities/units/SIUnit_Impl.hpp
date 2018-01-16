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
    virtual Unit clone() const override;

    //@}
    /** @name Base Units and Scale */
    //@{

    /** Throws if baseUnit is not a valid for SIUnit. */
    virtual void setBaseUnitExponent(const std::string& baseUnit, int exponent) override;

    //@}
   private:
    REGISTER_LOGGER("openstudio.units.SIUnit");
  };

} // detail
} // openstudio

#endif // UTILITIES_UNITS_SIUNIT_IMPL_HPP

