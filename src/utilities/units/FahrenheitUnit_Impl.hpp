/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_FAHRENHEITUNIT_IMPL_HPP
#define UTILITIES_UNITS_FAHRENHEITUNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "TemperatureUnit_Impl.hpp"

namespace openstudio {
namespace detail {

  /** FahrenheitUnit_Impl is a TemperatureUnit_Impl that is the implementation class for
   *  FahrenheitUnit.*/
  class UTILITIES_API FahrenheitUnit_Impl : public TemperatureUnit_Impl
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Default constructor creates an absolute temperature.
     *
     *  \param[in] FExp exponent on baseUnit F.
     *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
     *  \param[in] prettyString optional string to use in place of standardString. */
    FahrenheitUnit_Impl(int FExp = 0, int scaleExponent = 0, const std::string& prettyString = "");

    /** Alternate constructor creates an absolute temperature. Specify the abbreviation of the scale,
     *  rather than its exponent.
     *
     *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
     *    "k" for kilo.
     *  \param[in] FExp exponent on baseUnit F.
     *  \param[in] prettyString optional string to use in place of standardString. */
    FahrenheitUnit_Impl(const std::string& scaleAbbreviation, int FExp = 0, const std::string& prettyString = "");

    virtual ~FahrenheitUnit_Impl() override = default;

    /** Deep copy constructor. */
    virtual Unit clone() const override;

    //@}
    /** @name Base Units and Scale */
    //@{

    /** Throws if baseUnit is not a valid for FahrenheitUnit. */
    virtual void setBaseUnitExponent(const std::string& baseUnit, int exponent) override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.units.FahrenheitUnit");
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_UNITS_FAHRENHEITUNIT_IMPL_HPP
