/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_CFMUNIT_IMPL_HPP
#define UTILITIES_UNITS_CFMUNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit_Impl.hpp"

namespace openstudio {
namespace detail {

  /** CFMUnit_Impl is a Unit_Impl that is the implementation class for CFMUnit.*/
  class UTILITIES_API CFMUnit_Impl : public Unit_Impl
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor. Example: \verbatim
        CFMUnit myPower(CFMExpnt(0,0,1));
        std::cout << myPower; // produces "ton" \endverbatim
     *
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
     *  \param[in] prettyString optional string to use in place of standardString. */
    CFMUnit_Impl(const CFMExpnt& exponents = CFMExpnt(), int scaleExponent = 0, const std::string& prettyString = "");

    /** Alternate constructor. Specify the abbreviation of the scale, rather than its
     *  exponent.
     *
     *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
     *    "k" for kilo.
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] prettyString optional string to use in place of standardString. */
    CFMUnit_Impl(const std::string& scaleAbbreviation, const CFMExpnt& exponents = CFMExpnt(), const std::string& prettyString = "");

    virtual ~CFMUnit_Impl() override = default;

    /** Deep copy constructor. */
    virtual Unit clone() const override;

    //@}
    /** @name Base Units and Scale */
    //@{

    /** Throws if baseUnit is not a valid for SIUnit. */
    virtual void setBaseUnitExponent(const std::string& baseUnit, int exponent) override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.utilities.CFMUnit");
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_UNITS_CFMUNIT_IMPL_HPP
