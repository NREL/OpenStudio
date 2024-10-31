/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_MPHUNIT_HPP
#define UTILITIES_UNITS_MPHUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit.hpp"

namespace openstudio {
namespace detail {

  class MPHUnit_Impl;

}  // namespace detail

/** Structure to hold MPHUnit exponents needed for MPHUnit construction. \relates MPHUnit */
struct UTILITIES_API MPHExpnt
{
 public:
  MPHExpnt(int inHg = 0, int mi = 0, int h = 0, int R = 0, int A = 0, int cd = 0, int lbmol = 0, int deg = 0, int sr = 0, int people = 0,
           int cycle = 0, int dollar = 0)
    : m_inHg(inHg),
      m_mi(mi),
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
  int m_inHg;
  int m_mi;
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

  friend class detail::MPHUnit_Impl;
};

/** MPHUnit is a Unit with baseUnits fixed by its constructors, see MPHExpnt. setBaseUnitExponent
 *  throws an exception if any other string is passed in as a baseUnit. MPHUnit.hpp declares
 *  related operators and UnitFactory callback functions. */
class UTILITIES_API MPHUnit : public Unit
{
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor.
   *
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
   *  \param[in] prettyString optional string to use in place of standardString. */
  MPHUnit(const MPHExpnt& exponents = MPHExpnt(), int scaleExponent = 0, const std::string& prettyString = "");

  /** Alternate constructor. Specify the abbreviation of the scale, rather than its
   *  exponent.
   *
   *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
   *    "k" for kilo.
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] prettyString optional string to use in place of standardString. */
  MPHUnit(const std::string& scaleAbbreviation, const MPHExpnt& exponents = MPHExpnt(), const std::string& prettyString = "");

  virtual ~MPHUnit() override = default;

  //@}
 protected:
  /// @cond
  using ImplType = detail::MPHUnit_Impl;

  explicit MPHUnit(std::shared_ptr<detail::MPHUnit_Impl> impl);

  friend class Unit;
  friend class detail::MPHUnit_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.units.MPHUnit");
};

/** \relates MPHUnit*/
using OptionalMPHUnit = boost::optional<MPHUnit>;

/** \relates MPHUnit*/
using MPHUnitVector = std::vector<MPHUnit>;

/** @name Create Functions Used by UnitFactory */
//@{

// base units

/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHPressure();
/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHLength();
/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHTime();
/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHTemperature();
/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHElectricCurrent();
/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHLuminousIntensity();
/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHAmountOfSubstance();
/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHAngle();
/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHSolidAngle();
/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHPeople();
/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHCycle();
/** \relates MPHUnit */
UTILITIES_API MPHUnit createMPHCurrency();

// first order derived units

/** Miles per hour (mph) = mi/h. \relates MPHUnit */
UTILITIES_API MPHUnit createMPHVelocity();
/** Lumen (lm) = cd*sr. \relates MPHUnit */
UTILITIES_API MPHUnit createMPHLuminousFlux();

//@}

}  // namespace openstudio

#endif  // UTILITIES_UNITS_MPHUNIT_HPP
