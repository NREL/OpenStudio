/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_BTUUNIT_HPP
#define UTILITIES_UNITS_BTUUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit.hpp"

namespace openstudio {
namespace detail {

  class BTUUnit_Impl;

}  // namespace detail

/** Structure to hold BTUUnit exponents needed for BTUUnit construction. \relates BTUUnit */
struct UTILITIES_API BTUExpnt
{
 public:
  BTUExpnt(int btu = 0, int ft = 0, int h = 0, int R = 0, int A = 0, int cd = 0, int lbmol = 0, int deg = 0, int sr = 0, int people = 0,
           int cycle = 0, int dollar = 0)
    : m_btu(btu),
      m_ft(ft),
      m_h(h),
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
  int m_btu;
  int m_ft;
  int m_h;
  int m_R;
  int m_A;
  int m_cd;
  int m_lbmol;
  int m_deg;
  int m_sr;
  int m_people;
  int m_cycle;
  int m_dollar;

  friend class detail::BTUUnit_Impl;
};

/** BTUUnit is a Unit with baseUnits fixed by its constructors, see BTUExpnt.
 *  setBaseUnitExponent throws an exception if any other string is passed in as a baseUnit.
 *  BTUUnit.hpp declares related operators and UnitFactory callback functions. */
class UTILITIES_API BTUUnit : public Unit
{
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor. Example: \verbatim
      BTUUnit myEnergy(BtuExpnt(1),3);
      std::cout << myEnergy; // produces "kBtu" \endverbatim
   *
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
   *  \param[in] prettyString optional string to use in place of standardString. */
  BTUUnit(const BTUExpnt& exponents = BTUExpnt(), int scaleExponent = 0, const std::string& prettyString = "");

  /** Alternate constructor. Specify the abbreviation of the scale, rather than its
   *  exponent.
   *
   *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
   *    "k" for kilo.
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] prettyString optional string to use in place of standardString. */
  BTUUnit(const std::string& scaleAbbreviation, const BTUExpnt& exponents = BTUExpnt(), const std::string& prettyString = "");

  virtual ~BTUUnit() override = default;

  //@}
 protected:
  /// @cond
  using ImplType = detail::BTUUnit_Impl;

  explicit BTUUnit(std::shared_ptr<detail::BTUUnit_Impl> impl);

  friend class Unit;
  friend class detail::BTUUnit_Impl;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.units.BTUUnit");
};

/** \relates BTUUnit*/
using OptionalBTUUnit = boost::optional<BTUUnit>;

/** \relates BTUUnit*/
using BTUUnitVector = std::vector<BTUUnit>;

/** @name Create Functions Used by UnitFactory */
//@{

// base units

/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTUEnergy();
/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTULength();
/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTUTime();
/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTUTemperature();
/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTUElectricCurrent();
/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTULuminousIntensity();
/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTUAmountOfSubstance();
/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTUAngle();
/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTUSolidAngle();
/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTUPeople();
/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTUCycle();
/** \relates BTUUnit */
UTILITIES_API BTUUnit createBTUCurrency();

// first order derived units

/** Btu/h. \relates BTUUnit */
UTILITIES_API BTUUnit createBTUPower();
/** Lumen (lm) = cd*sr. \relates BTUUnit */
UTILITIES_API BTUUnit createBTULuminousFlux();
/** Foot-candles (fc) = lm/ft^2 = cd*sr/ft^2. \relates BTUUnit */
UTILITIES_API BTUUnit createBTUIlluminance();

//@}

}  // namespace openstudio

#endif  // UTILITIES_UNITS_BTUUNIT_HPP
