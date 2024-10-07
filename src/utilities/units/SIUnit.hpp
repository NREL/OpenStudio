/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_SIUNIT_HPP
#define UTILITIES_UNITS_SIUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit.hpp"

namespace openstudio {
namespace detail {

  class SIUnit_Impl;

}  // namespace detail

/** Structure to hold SIUnit exponents needed for SIUnit construction. The first seven units
 *  are true base units in the SI system. The last four are, strictly speaking, derived and/or
 *  dimensionless units, but have meanings and conversions that are easier to express when they
 *  are broken out into distinct elements. \relates SIUnit */
struct UTILITIES_API SIExpnt
{
 public:
  SIExpnt(int kg = 0, int m = 0, int s = 0, int K = 0, int A = 0, int cd = 0, int mol = 0, int rad = 0, int sr = 0, int people = 0, int cycle = 0,
          int dollar = 0)
    : m_kg(kg), m_m(m), m_s(s), m_K(K), m_A(A), m_cd(cd), m_mol(mol), m_rad(rad), m_sr(sr), m_people(people), m_cycle(cycle), m_dollar(dollar) {}

 private:
  int m_kg;
  int m_m;
  int m_s;
  int m_K;
  int m_A;
  int m_cd;
  int m_mol;
  int m_rad;
  int m_sr;
  int m_people;
  int m_cycle;
  int m_dollar;

  friend class detail::SIUnit_Impl;
};

/** SIUnit is a Unit with baseUnits are fixed by its constructors, see SIExpnt. setBaseUnitExponent
 *  throws an exception if any other string is passed in as a baseUnit. SIUnit.hpp declares related
*   operators and UnitFactory callback functions. */
class UTILITIES_API SIUnit : public Unit
{
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor. Example: \verbatim
      SIUnit myEnergy(SIExpnt(1,2,-2),3,"J");
      std::cout << myEnergy; // produces "kJ" \endverbatim
   *
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
   *  \param[in] prettyString optional string to use in place of standardString. */
  SIUnit(const SIExpnt& exponents = SIExpnt(), int scaleExponent = 0, const std::string& prettyString = "");

  /** Alternate constructor. Specify the abbreviation of the scale, rather than its
   *  exponent. Example: \verbatim
      SIUnit myEnergy("k",SIExpnt(1,2,-2),"J");
      std::cout << myEnergy; // produces "kJ \endverbatim
   *
   *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
   *    "k" for kilo.
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] prettyString optional string to use in place of standardString. */
  SIUnit(const std::string& scaleAbbreviation, const SIExpnt& exponents = SIExpnt(), const std::string& prettyString = "");

  virtual ~SIUnit() override = default;

  //@}
 protected:
  /// @cond
  using ImplType = detail::SIUnit_Impl;

  explicit SIUnit(std::shared_ptr<detail::SIUnit_Impl> impl);

  friend class Unit;
  friend class detail::SIUnit_Impl;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.units.SIUnit");
};

/** \relates SIUnit*/
using OptionalSIUnit = boost::optional<SIUnit>;

/** \relates SIUnit*/
using SIUnitVector = std::vector<SIUnit>;

/** @name Create Functions Used by UnitFactory */
//@{

// base units

/** \relates SIUnit */
UTILITIES_API SIUnit createSIMass();
/** \relates SIUnit */
UTILITIES_API SIUnit createSILength();
/** \relates SIUnit */
UTILITIES_API SIUnit createSITime();
/** \relates SIUnit */
UTILITIES_API SIUnit createSITemperature();
/** \relates SIUnit */
UTILITIES_API SIUnit createSIElectricCurrent();
/** \relates SIUnit */
UTILITIES_API SIUnit createSILuminousIntensity();
/** \relates SIUnit */
UTILITIES_API SIUnit createSIAmountOfSubstance();
/** \relates SIUnit */
UTILITIES_API SIUnit createSIAngle();
/** \relates SIUnit */
UTILITIES_API SIUnit createSISolidAngle();
/** \relates SIUnit */
UTILITIES_API SIUnit createSIPeople();
/** \relates SIUnit */
UTILITIES_API SIUnit createSICycle();
/** \relates SIUnit */
UTILITIES_API SIUnit createSICurrency();

// first order derived units

/** Newton (N) = kg*m/s^2. \relates SIUnit */
UTILITIES_API SIUnit createSIForce();
/** Joule (J) = N*m = kg*m^2/s^2. \relates SIUnit */
UTILITIES_API SIUnit createSIEnergy();
/** Watt (W) = J/s = N*m/s = kg*m^2/s^3. \relates SIUnit */
UTILITIES_API SIUnit createSIPower();
/** Coulomb (C) = s*A. \relates SIUnit */
UTILITIES_API SIUnit createSIElectricCharge();
/** Volt (V) = W/A = J/C = kg*m^2/s^3*A. \relates SIUnit */
UTILITIES_API SIUnit createSIElectricalPotential();
/** Farad (F) = C/V = C^2/J = s^4*A^2/kg*m^2. \relates SIUnit */
UTILITIES_API SIUnit createSIElectricCapacitance();
/** Ohm (ohm) = V/A = W/A^2 = kg*m^2/s^3*A^2. \relates SIUnit */
UTILITIES_API SIUnit createSIElectricResistance();
/** Weber (Wb) = J/A = kg*m^2/s^2*A. \relates SIUnit */
UTILITIES_API SIUnit createSIMagneticFlux();
/** Tesla (T) = Wb/m^2 = J/m^2*A = kg/s^2*A. \relates SIUnit */
UTILITIES_API SIUnit createSIMagneticFieldStrength();
/** Henry (H) = Wb/A = s*V/A = kg*m^2/s^2*A^2. \relates SIUnit */
UTILITIES_API SIUnit createSIInductance();
/** Lumen (lm) = cd*sr. \relates SIUnit */
UTILITIES_API SIUnit createSILuminousFlux();
/** Lux (lux or lx) = lm/m^2 = cd*sr/m^2. \relates SIUnit */
UTILITIES_API SIUnit createSIIlluminance();
/** Hertz (Hz) = cycles/s. Making the distinction between 1/s and cycles/s here mainly to be
 *  consistent with the rotations per minute (rpm) designation made elsewhere. \relates SIUnit */
UTILITIES_API SIUnit createSIFrequency();

// second order derived units

/** J/m^2 = kg/s^2. That this is measured on an annual basis is implied. \relates SIUnit */
UTILITIES_API SIUnit createSIEnergyUseIntensity();
/** W/m^2 = kg/s^3. \relates SIUnit */
UTILITIES_API SIUnit createSIPowerDensity();
/** W/person = kg*m^2/s^3*person. \relates SIUnit */
UTILITIES_API SIUnit createSIPowerPerPerson();
/** Pa = N/m^2 = kg/m*s^2. \relates SIUnit */
UTILITIES_API SIUnit createSIPressure();
/** W/m^2*K = kg/s^3*K \relates SIUnit */
UTILITIES_API SIUnit createSIThermalConductance();
/** m^2*K/W = s^3*K/kg \relates SIUnit */
UTILITIES_API SIUnit createSIThermalResistance();
/** J/m^2*K = kg/s^2*K \relates SIUnit */
UTILITIES_API SIUnit createSIHeatCapacity();

//@}

}  // namespace openstudio

#endif  // UTILITIES_UNITS_SIUNIT_HPP
