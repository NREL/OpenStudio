/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_IPUNIT_IMPL_HPP
#define UTILITIES_UNITS_IPUNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit_Impl.hpp"

namespace openstudio {
namespace detail {

  /** IPUnit_Impl is a Unit_Impl that is the implementation class for IPUnit.*/
  class UTILITIES_API IPUnit_Impl : public Unit_Impl
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor. Example: \verbatim
        IPUnit myEnergy(IPExpnt(0,1,0,0,1));
        std::cout << myEnergy; // produces "ft*lb_f" \endverbatim
     *
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
     *  \param[in] prettyString optional string to use in place of standardString. */
    IPUnit_Impl(const IPExpnt& exponents = IPExpnt(), int scaleExponent = 0, const std::string& prettyString = "");

    /** Alternate constructor. Specify the abbreviation of the scale, rather than its
     *  exponent.
     *
     *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
     *    "k" for kilo.
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] prettyString optional string to use in place of standardString. */
    IPUnit_Impl(const std::string& scaleAbbreviation, const IPExpnt& exponents = IPExpnt(), const std::string& prettyString = "");

    virtual ~IPUnit_Impl() override = default;

    /** Deep copy constructor. */
    virtual Unit clone() const override;

    //@}
    /** @name Base Units and Scale */
    //@{

    /** Throws if baseUnit is not a valid for IPUnit. */
    virtual void setBaseUnitExponent(const std::string& baseUnit, int exponent) override;

    //@}
    /** @name Mathematical Operators */
    //@{

    /** Overloaded operator to account for relationship between lb_m and lb_f. */
    virtual bool operator==(const Unit& rUnit) const override;

    /** Convert any non-zero lb_m exponent to lb_f. */
    void lbmToLbf();

    /** Convert any non-zero lb_f exponent to lb_m. */
    void lbfToLbm();

    //@}
   private:
    REGISTER_LOGGER("openstudio.units.IPUnit");
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_UNITS_IPUNIT_IMPL_HPP
