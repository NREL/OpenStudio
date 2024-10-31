/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_CFMUNIT_HPP
#define UTILITIES_UNITS_CFMUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit.hpp"

namespace openstudio {
namespace detail {

  class CFMUnit_Impl;

}  // namespace detail

/** Structure to hold CFMUnit exponents needed for CFMUnit construction. \relates CFMUnit */
struct UTILITIES_API CFMExpnt
{
 public:
  CFMExpnt(int ft = 0, int min = 0, int ton = 0, int R = 0, int A = 0, int cd = 0, int lbmol = 0, int deg = 0, int sr = 0, int people = 0,
           int cycle = 0, int dollar = 0)
    : m_ft(ft),
      m_min(min),
      m_ton(ton),
      m_R(R),
      m_A(A),
      m_cd(cd),
      m_lbmol(lbmol),
      m_deg(deg),
      m_sr(sr),
      m_people(people),
      m_cycle(cycle),
      m_dollar(dollar) {}

 private:
  int m_ft;
  int m_min;
  int m_ton;
  int m_R;
  int m_A;
  int m_cd;
  int m_lbmol;
  int m_deg;
  int m_sr;
  int m_people;
  int m_cycle;
  int m_dollar;

  friend class detail::CFMUnit_Impl;
};

/** CFMUnit is a Unit with baseUnits fixed by its constructors, see CFMExpnt.
 *  setBaseUnitExponent throws an exception if any other string is passed in as a
 *  baseUnit. CFMUnit.hpp declares related operators and UnitFactory callback functions.
 */
class UTILITIES_API CFMUnit : public Unit
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
  CFMUnit(const CFMExpnt& exponents = CFMExpnt(), int scaleExponent = 0, const std::string& prettyString = "");

  /** Alternate constructor. Specify the abbreviation of the scale, rather than its
   *  exponent.
   *
   *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
   *    "k" for kilo.
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] prettyString optional string to use in place of standardString. */
  CFMUnit(const std::string& scaleAbbreviation, const CFMExpnt& exponents = CFMExpnt(), const std::string& prettyString = "");

  virtual ~CFMUnit() override = default;

  //@}
 protected:
  /// @cond
  using ImplType = detail::CFMUnit_Impl;

  explicit CFMUnit(std::shared_ptr<detail::CFMUnit_Impl> impl);

  friend class Unit;
  friend class detail::CFMUnit_Impl;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.units.CFMUnit");
};

/** \relates CFMUnit*/
using OptionalCFMUnit = boost::optional<CFMUnit>;

/** \relates CFMUnit*/
using CFMUnitVector = std::vector<CFMUnit>;

/** @name Create Functions Used by UnitFactory */
//@{

/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMLength();
/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMTime();
/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMPower();
/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMTemperature();
/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMElectricCurrent();
/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMLuminousIntensity();
/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMAmountOfSubstance();
/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMAngle();
/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMSolidAngle();
/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMPeople();
/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMCycle();
/** \relates CFMUnit */
UTILITIES_API CFMUnit createCFMCurrency();

/** Cubic feet per minute (cfm) = ft^3/min. \relates CFMUnit */
UTILITIES_API CFMUnit createCFMVolumetricFlowrate();
/** Lumen (lm) = cd*sr. \relates CFMUnit */
UTILITIES_API CFMUnit createCFMLuminousFlux();
/** Foot-candles (fc) = lm/ft^2 = cd*sr/ft^2. \relates CFMUnit */
UTILITIES_API CFMUnit createCFMIlluminance();
/** Rotations per minute (rpm) = cycles/min. \relates CFMUnit */
UTILITIES_API CFMUnit createCFMFrequency();

//@}

}  // namespace openstudio

#endif  // UTILITIES_UNITS_CFMUNIT_HPP
