/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_IPUNIT_HPP
#define UTILITIES_UNITS_IPUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit.hpp"

namespace openstudio {
namespace detail {

  class IPUnit_Impl;

}  // namespace detail

/** Structure to hold IPUnit exponents needed for IPUnit construction. \relates IPUnit */
struct UTILITIES_API IPExpnt
{
 public:
  IPExpnt(int lbm = 0, int ft = 0, int s = 0, int R = 0, int A = 0, int cd = 0, int lbmol = 0, int lbf = 0, int deg = 0, int sr = 0, int people = 0,
          int cycle = 0, int dollar = 0)
    : m_lbm(lbm),
      m_ft(ft),
      m_s(s),
      m_R(R),
      m_A(A),
      m_cd(cd),
      m_lbmol(lbmol),
      m_lbf(lbf),
      m_deg(deg),
      m_sr(sr),
      m_people(people),
      m_cycle(cycle),
      m_dollar(dollar) {}

 private:
  int m_lbm;
  int m_ft;
  int m_s;
  int m_R;
  int m_A;
  int m_cd;
  int m_lbmol;
  int m_lbf;
  int m_deg;
  int m_sr;
  int m_people;
  int m_cycle;
  int m_dollar;

  friend class detail::IPUnit_Impl;
};

/** IPUnit is a Unit with baseUnits fixed by its constructors, see IPExpnt. setBaseUnitExponent
 *  throws an exception if any other string is passed in as a baseUnit. IPUnit.hpp declares
 *  related operators and UnitFactory callback functions. */
class UTILITIES_API IPUnit : public Unit
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
  IPUnit(const IPExpnt& exponents = IPExpnt(), int scaleExponent = 0, const std::string& prettyString = "");

  /** Alternate constructor. Specify the abbreviation of the scale, rather than its
   *  exponent.
   *
   *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
   *    "k" for kilo.
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] prettyString optional string to use in place of standardString. */
  IPUnit(const std::string& scaleAbbreviation, const IPExpnt& exponents = IPExpnt(), const std::string& prettyString = "");

  virtual ~IPUnit() override = default;

  //@}

  static double gc();

  /** @name Mathematical Operators */
  //@{

  /** Convert any non-zero lb_m exponent to lb_f. */
  void lbmToLbf();

  /** Convert any non-zero lb_f exponent to lb_m. */
  void lbfToLbm();

  //@}
 protected:
  /// @cond
  using ImplType = detail::IPUnit_Impl;

  explicit IPUnit(std::shared_ptr<detail::IPUnit_Impl> impl);

  friend class Unit;
  friend class detail::IPUnit_Impl;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.units.IPUnit");
};

/** \relates IPUnit*/
using OptionalIPUnit = boost::optional<IPUnit>;

/** \relates IPUnit*/
using IPUnitVector = std::vector<IPUnit>;

/** @name Create Functions Used by UnitFactory */
//@{

// base units

/** \relates IPUnit */
UTILITIES_API IPUnit createIPMass();
/** \relates IPUnit */
UTILITIES_API IPUnit createIPLength();
/** \relates IPUnit */
UTILITIES_API IPUnit createIPTime();
/** \relates IPUnit */
UTILITIES_API IPUnit createIPTemperature();
/** \relates IPUnit */
UTILITIES_API IPUnit createIPElectricCurrent();
/** \relates IPUnit */
UTILITIES_API IPUnit createIPLuminousIntensity();
/** \relates IPUnit */
UTILITIES_API IPUnit createIPAmountOfSubstance();
/** \relates IPUnit */
UTILITIES_API IPUnit createIPAngle();
/** \relates IPUnit */
UTILITIES_API IPUnit createIPSolidAngle();
/** \relates IPUnit */
UTILITIES_API IPUnit createIPPeople();
/** relates IPUnit */
UTILITIES_API IPUnit createIPCycle();
/** relates IPUnit */
UTILITIES_API IPUnit createIPCurrency();

// first order derived units

/** lb_f is preferred over equivalent using lb_m. \relates IPUnit */
UTILITIES_API IPUnit createIPForce();
/** ft*lb_f is preferred over equivalent using lb_m. \relates IPUnit */
UTILITIES_API IPUnit createIPEnergy();
/** ft*lb_f/s is preferred over equivalent using lb_m. \relates IPUnit */
UTILITIES_API IPUnit createIPPower();
/** Coulomb (C) = s*A. \relates IPUnit */
UTILITIES_API IPUnit createIPElectricCharge();
/** Lumen (lm) = cd*sr. \relates IPUnit */
UTILITIES_API IPUnit createIPLuminousFlux();
/** Foot-candles (fc) = lm/ft^2 = cd*sr/ft^2. \relates IPUnit */
UTILITIES_API IPUnit createIPIlluminance();
/** Hertz (Hz) = cycles/s. Making the distinction between 1/s and cycles/s here mainly to be
 *  consistent with the rotations per minute (rpm) designation made elsewhere. \relates IPUnit */
UTILITIES_API IPUnit createIPFrequency();

//@}

}  // namespace openstudio

#endif  // UTILITIES_UNITS_IPUNIT_HPP
