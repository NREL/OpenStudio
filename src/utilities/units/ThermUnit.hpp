/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_THERMUNIT_HPP
#define UTILITIES_UNITS_THERMUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit.hpp"

namespace openstudio {
namespace detail {

  class ThermUnit_Impl;

}  // namespace detail

/** Structure to hold ThermUnit exponents needed for ThermUnit construction. \relates ThermUnit */
struct UTILITIES_API ThermExpnt
{
 public:
  ThermExpnt(int therm = 0, int t_in = 0, int yr = 0, int R = 0, int A = 0, int cd = 0, int lbmol = 0, int deg = 0, int sr = 0, int people = 0,
             int cycle = 0, int dollar = 0)
    : m_therm(therm),
      m_in(t_in),
      m_yr(yr),
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
  int m_therm;
  int m_in;
  int m_yr;
  int m_R;
  int m_A;
  int m_cd;
  int m_lbmol;
  int m_deg;
  int m_sr;
  int m_people;
  int m_cycle;
  int m_dollar;

  friend class detail::ThermUnit_Impl;
};

/** ThermUnit is a Unit with baseUnits  Btu, ft, h, R, and people.
 *  setBaseUnitExponent throws and exception if any other string is passed in as a
 *  baseUnit. ThermUnit.hpp declares related operators and UnitFactory callback functions. */
class UTILITIES_API ThermUnit : public Unit
{
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor. Example: \verbatim
      ThermUnit myEnergy(ThermExpnt(1));
      std::cout << myEnergy; // produces "therm" \endverbatim
   *
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
   *  \param[in] prettyString optional string to use in place of standardString. */
  ThermUnit(const ThermExpnt& exponents = ThermExpnt(), int scaleExponent = 0, const std::string& prettyString = "");

  /** Alternate constructor. Specify the abbreviation of the scale, rather than its
   *  exponent.
   *
   *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
   *    "k" for kilo.
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] prettyString optional string to use in place of standardString. */
  ThermUnit(const std::string& scaleAbbreviation, const ThermExpnt& exponents = ThermExpnt(), const std::string& prettyString = "");

  virtual ~ThermUnit() override = default;

  //@}
 protected:
  /// @cond
  using ImplType = detail::ThermUnit_Impl;

  explicit ThermUnit(std::shared_ptr<detail::ThermUnit_Impl> impl);

  friend class Unit;
  friend class detail::ThermUnit_Impl;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.units.ThermUnit");
};

/** \relates ThermUnit*/
using OptionalThermUnit = boost::optional<ThermUnit>;

/** \relates ThermUnit*/
using ThermUnitVector = std::vector<ThermUnit>;

/** @name Create Functions Used by UnitFactory */
//@{

/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermEnergy();
/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermLength();
/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermTime();
/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermTemperature();
/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermElectricCurrent();
/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermLuminousIntensity();
/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermAmountOfSubstance();
/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermAngle();
/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermSolidAngle();
/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermPeople();
/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermCycle();
/** \relates ThermUnit */
UTILITIES_API ThermUnit createThermCurrency();

/** Lumen (lm) = cd*sr. \relates ThermUnit */
UTILITIES_API ThermUnit createThermLuminousFlux();

//@}

}  // namespace openstudio

#endif  // UTILITIES_UNITS_THERMUNIT_HPP
