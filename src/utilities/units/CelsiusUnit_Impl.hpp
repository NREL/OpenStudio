/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_CELSIUSUNIT_IMPL_HPP
#define UTILITIES_UNITS_CELSIUSUNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "TemperatureUnit_Impl.hpp"

namespace openstudio {
namespace detail {

  /** CelsiusUnit_Impl is a TemperatureUnit_Impl that is the implementation class for CelsiusUnit.*/
  class UTILITIES_API CelsiusUnit_Impl : public TemperatureUnit_Impl
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor creates an absolute temperature.
     *
     *  \param[in] CExp exponent on baseUnit C.
     *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
     *  \param[in] prettyString optional string to use in place of standardString. */
    CelsiusUnit_Impl(int CExp = 0, int scaleExponent = 0, const std::string& prettyString = "");

    /** Alternate constructor creates an absolute temperature. Specify the abbreviation of the scale,
     *  rather than its exponent.
     *
     *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
     *    "k" for kilo.
     *  \param[in] CExp exponent on baseUnit C.
     *  \param[in] prettyString optional string to use in place of standardString. */
    CelsiusUnit_Impl(const std::string& scaleAbbreviation, int CExp = 0, const std::string& prettyString = "");

    virtual ~CelsiusUnit_Impl() override = default;

    /** Deep copy constructor. */
    virtual Unit clone() const override;

    //@}
    /** @name Base Units and Scale */
    //@{

    /** Throws if baseUnit is not a valid for CelsiusUnit. */
    virtual void setBaseUnitExponent(const std::string& baseUnit, int exponent) override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.units.CelsiusUnit");
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_UNITS_CELSIUSUNIT_IMPL_HPP
