/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_SIUNIT_IMPL_HPP
#define UTILITIES_UNITS_SIUNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit_Impl.hpp"
#include "SIUnit.hpp"

namespace openstudio {
namespace detail {

  /** SIUnit_Impl is a Unit_Impl that is the implementation class for SIUnit.*/
  class UTILITIES_API SIUnit_Impl : public Unit_Impl
  {
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
    SIUnit_Impl(const SIExpnt& exponents = SIExpnt(), int scaleExponent = 0, const std::string& prettyString = "");

    /** Alternate constructor. Specify the abbreviation of the scale, rather than its
     *  exponent. Example: \verbatim
        SIUnit myEnergy("k",SIExpnt(1,2,-2),"J");
        std::cout << myEnergy; // produces "kJ \endverbatim
     *
     *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
     *    "k" for kilo.
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] prettyString optional string to use in place of standardString. */
    SIUnit_Impl(const std::string& scaleAbbreviation, const SIExpnt& exponents = SIExpnt(), const std::string& prettyString = "");

    virtual ~SIUnit_Impl() override = default;

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

}  // namespace detail
}  // namespace openstudio

#endif  // UTILITIES_UNITS_SIUNIT_IMPL_HPP
