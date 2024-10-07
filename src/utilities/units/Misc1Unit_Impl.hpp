/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_MISC1UNIT_IMPL_HPP
#define UTILITIES_UNITS_MISC1UNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit_Impl.hpp"

namespace openstudio {
namespace detail {

  /** Misc1Unit_Impl is a Unit_Impl that is the implementation class for Misc1Unit.*/
  class UTILITIES_API Misc1Unit_Impl : public Unit_Impl
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor.
       *
       *  \param[in] exponents holds the exponents for each base unit.
       *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
       *  \param[in] prettyString optional string to use in place of standardString. */
    Misc1Unit_Impl(const Misc1Expnt& exponents = Misc1Expnt(), int scaleExponent = 0, const std::string& prettyString = "");

    /** Alternate constructor. Specify the abbreviation of the scale, rather than its
       *  exponent.
       *
       *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
       *    "k" for kilo.
       *  \param[in] exponents holds the exponents for each base unit.
       *  \param[in] prettyString optional string to use in place of standardString. */
    Misc1Unit_Impl(const std::string& scaleAbbreviation, const Misc1Expnt& exponents = Misc1Expnt(), const std::string& prettyString = "");

    virtual ~Misc1Unit_Impl() override = default;

    /** Deep copy constructor. */
    virtual Unit clone() const override;

    //@}
    /** @name Base Units and Scale */
    //@{

    /** Throws if baseUnit is not a valid for Misc1Unit. */
    virtual void setBaseUnitExponent(const std::string& baseUnit, int exponent) override;

    //@}
   private:
    REGISTER_LOGGER("openstudio.units.Misc1Unit");
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_UNITS_MISC1UNIT_IMPL_HPP
