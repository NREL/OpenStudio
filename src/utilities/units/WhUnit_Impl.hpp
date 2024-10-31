/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_WHUNIT_IMPL_HPP
#define UTILITIES_UNITS_WHUNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit_Impl.hpp"

namespace openstudio {
namespace detail {

  /** WhUnit_Impl is a Unit_Impl that is the implementation class for WhUnit.*/
  class UTILITIES_API WhUnit_Impl : public Unit_Impl
  {
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
    WhUnit_Impl(const WhExpnt& exponents = WhExpnt(), int scaleExponent = 0, const std::string& prettyString = "");

    /** Alternate constructor. Specify the abbreviation of the scale, rather than its
     *  exponent. Example: \verbatim
        WhUnit myEnergy("k",WhExpnt(1,1),"Wh");
        std::cout << myEnergy; // produces "kWh" \endverbatim
     *
     *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
     *    "k" for kilo.
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] prettyString optional string to use in place of standardString. */
    WhUnit_Impl(const std::string& scaleAbbreviation, const WhExpnt& exponents = WhExpnt(), const std::string& prettyString = "");

    virtual ~WhUnit_Impl() override = default;

    /** Deep copy constructor. */
    virtual Unit clone() const override;

    //@}
    /** @name Base Units and Scale */
    //@{

    /** Throws if baseUnit is not a valid for WhUnit. */
    virtual void setBaseUnitExponent(const std::string& baseUnit, int exponent) override;

    //@}
   private:
    REGISTER_LOGGER("openstudio.units.WhUnit");
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_UNITS_WHUNIT_IMPL_HPP
