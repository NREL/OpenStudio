/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef UTILITIES_UNITS_MISC1UNIT_HPP
#define UTILITIES_UNITS_MISC1UNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit.hpp"

namespace openstudio {
namespace detail {

  class Misc1Unit_Impl;

} // detail

/** Structure to hold Misc1Unit exponents needed for Misc1Unit construction. For purposes of
 *  conversion we need a length base unit. Therefore, gal cannot be a base unit. For convenience
 *  of construction, Misc1Expnt takes a gal argument, but subsequently this is stored as a power of
 *  cube root of gallons (crgal). \relates Misc1Unit */
struct UTILITIES_API Misc1Expnt {
 public:
  Misc1Expnt(int ftH2O=0,
             int L=0,
             int day=0,
             int K=0,
             int A=0,
             int cd=0,
             int mol=0,
             int rad=0,
             int sr=0,
             int people=0,
             int cycle=0)
    : m_ftH2O(ftH2O),
      m_crL(3*L),
      m_day(day),
      m_K(K),
      m_A(A),
      m_cd(cd),
      m_mol(mol),
      m_rad(rad),
      m_sr(sr),
      m_people(people),
      m_cycle(cycle)
  {}
 private:
  int m_ftH2O;
  int m_crL;
  int m_day;
  int m_K;
  int m_A;
  int m_cd;
  int m_mol;
  int m_rad;
  int m_sr;
  int m_people;
  int m_cycle;

  friend class detail::Misc1Unit_Impl;
};

/** Misc1Unit is a Unit with baseUnits fixed by its constructors, see Misc1Expnt. setBaseUnitExponent
 *  throws an exception if any other string is passed in as a baseUnit. Misc1Unit.hpp declares
 *  related operators and UnitFactory callback functions. */
class UTILITIES_API Misc1Unit : public Unit {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor.
     *
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
     *  \param[in] prettyString optional string to use in place of standardString. */
  Misc1Unit(const Misc1Expnt& exponents=Misc1Expnt(),
            int scaleExponent=0,
            const std::string& prettyString="");

  /** Alternate constructor. Specify the abbreviation of the scale, rather than its
     *  exponent.
     *
     *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
     *    "k" for kilo.
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] prettyString optional string to use in place of standardString. */
  Misc1Unit(const std::string& scaleAbbreviation,
          const Misc1Expnt& exponents=Misc1Expnt(),
          const std::string& prettyString="");

  virtual ~Misc1Unit() {}

  //@}
 protected:
  /// @cond
  typedef detail::Misc1Unit_Impl ImplType;

  explicit Misc1Unit(std::shared_ptr<detail::Misc1Unit_Impl> impl);

  friend class Unit;
  friend class detail::Misc1Unit_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.units.Misc1Unit");
};

/** \relates Misc1Unit*/
typedef boost::optional<Misc1Unit> OptionalMisc1Unit;

/** \relates Misc1Unit*/
typedef std::vector<Misc1Unit> Misc1UnitVector;

/** @name Create Functions Used by UnitFactory */
//@{

// base units

/** \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1Pressure();
/** \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1Length();
/** \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1Time();
/** \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1Temperature();
/** \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1ElectricCurrent();
/** \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1LuminousIntensity();
/** \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1AmountOfSubstance();
/** \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1Angle();
/** \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1SolidAngle();
/** \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1People();
/** relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1Cycle();

// first order derived units

/** Liters (L). \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1Volume();
/** Lumen (lm) = cd*sr. \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1LuminousFlux();

//@}
} // openstudio

#endif // UTILITIES_UNITS_MISC1UNIT_HPP

