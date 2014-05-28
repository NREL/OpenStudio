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

#ifndef UTILITIES_UNITS_WHUNIT_HPP
#define UTILITIES_UNITS_WHUNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit.hpp"

namespace openstudio {
namespace detail {

  class WhUnit_Impl;

} // detail

/** Structure to hold WhUnit exponents needed for WhUnit construction. \relates WhUnit */
struct UTILITIES_API WhExpnt {
 public:
  WhExpnt(int W=0,
          int h=0,
          int m=0,
          int K=0,
          int A=0,
          int cd=0,
          int mol=0,
          int rad=0,
          int sr=0,
          int people=0,
          int cycle=0)
  : m_W(W),
    m_h(h),
    m_m(m),
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
  int m_W;
  int m_h;
  int m_m;
  int m_K;
  int m_A;
  int m_cd;
  int m_mol;
  int m_rad;
  int m_sr;
  int m_people;
  int m_cycle;

  friend class detail::WhUnit_Impl;
};

/** WhUnit is a Unit with baseUnits W, h, m, K, and people.
 *  setBaseUnitExponent throws and exception if any other string is passed in as a
 *  baseUnit. WhUnit.hpp declares related operators and UnitFactory callback functions. */
class UTILITIES_API WhUnit : public Unit {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor. Example: \verbatim
      WhUnit myEnergy(WhExpnt(1,1),3,"Wh");
      std::cout << myEnergy; // produces "kWh" \endverbatim
   *
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
   *  \param[in] prettyString optional string to use in place of standardString. */
  WhUnit(const WhExpnt& exponents=WhExpnt(),
         int scaleExponent=0,
         const std::string& prettyString="");

  /** Alternate constructor. Specify the abbreviation of the scale, rather than its
   *  exponent. Example: \verbatim
      WhUnit myEnergy("k",WhExpnt(1,1),"Wh");
      std::cout << myEnergy; // produces "kWh" \endverbatim
   *
   *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
   *    "k" for kilo.
   *  \param[in] exponents holds the exponents for each base unit.
   *  \param[in] prettyString optional string to use in place of standardString. */
  WhUnit(const std::string& scaleAbbreviation,
         const WhExpnt& exponents=WhExpnt(),
         const std::string& prettyString="");

  virtual ~WhUnit() {}

  //@}
 protected:
  /// @cond
  typedef detail::WhUnit_Impl ImplType;

  explicit WhUnit(std::shared_ptr<detail::WhUnit_Impl> impl);

  friend class Unit;
  friend class detail::WhUnit_Impl;

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.units.WhUnit");
};

/** \relates WhUnit*/
typedef boost::optional<WhUnit> OptionalWhUnit;

/** \relates WhUnit*/
typedef std::vector<WhUnit> WhUnitVector;

/** @name Create Functions Used by UnitFactory */
//@{

/** \relates WhUnit */
UTILITIES_API WhUnit createWhPower();
/** \relates WhUnit */
UTILITIES_API WhUnit createWhTime();
/** \relates WhUnit */
UTILITIES_API WhUnit createWhLength();
/** \relates WhUnit */
UTILITIES_API WhUnit createWhTemperature();
/** \relates WhUnit */
UTILITIES_API WhUnit createWhElectricCurrent();
/** \relates WhUnit */
UTILITIES_API WhUnit createWhLuminousIntensity();
/** \relates WhUnit */
UTILITIES_API WhUnit createWhAmountOfSubstance();
/** \relates WhUnit */
UTILITIES_API WhUnit createWhAngle();
/** \relates WhUnit */
UTILITIES_API WhUnit createWhSolidAngle();
/** \relates WhUnit */
UTILITIES_API WhUnit createWhPeople();
/** \relates WhUnit */
UTILITIES_API WhUnit createWhCycle();

/** \relates WhUnit */
UTILITIES_API WhUnit createWhEnergy();
/** Volt (V) = W/A. \relates WhUnit */
UTILITIES_API WhUnit createWhElectricalPotential();
/** Ohm (ohm) = V/A = W/A^2. \relates WhUnit */
UTILITIES_API WhUnit createWhElectricResistance();
/** Lumen (lm) = cd*sr. \relates WhUnit */
UTILITIES_API WhUnit createWhLuminousFlux();
/** Lux (lux or lx) = lm/m^2 = cd*sr/m^2. \relates WhUnit */
UTILITIES_API WhUnit createWhIlluminance();

//@}

} // openstudio

#endif // UTILITIES_UNITS_WHUNIT_HPP

