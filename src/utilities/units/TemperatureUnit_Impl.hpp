/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_TEMPERATUREUNIT_IMPL_HPP
#define UTILITIES_UNITS_TEMPERATUREUNIT_IMPL_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit_Impl.hpp"

namespace openstudio {
namespace detail {

  /** TemperatureUnit_Impl is a Unit_Impl that is the implementation class for TemperatureUnit.*/
  class UTILITIES_API TemperatureUnit_Impl : public Unit_Impl
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~TemperatureUnit_Impl() override = default;

    //@}
    /** @name Mathematical Operators */
    //@{

    bool isAbsolute() const;

    void setAsAbsolute();

    void setAsRelative();

    virtual bool operator==(const Unit& rUnit) const override;

    virtual void operator*=(const Unit& rUnit) override;

    virtual void operator/=(const Unit& rUnit) override;

    //@}
   protected:
    bool m_absolute;

    TemperatureUnit_Impl(bool absolute, int scaleExponent, const std::string& prettyString, int numBaseUnits, UnitSystem system);

    TemperatureUnit_Impl(bool absolute, const std::string& scaleAbbreviation, const std::string& prettyString, int numBaseUnits, UnitSystem system);

   private:
    REGISTER_LOGGER("openstudio.units.TemperatureUnit");
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_UNITS_TEMPERATUREUNIT_IMPL_HPP
