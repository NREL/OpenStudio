/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef UTILITIES_UNITS_MISC1UNIT_HPP
#define UTILITIES_UNITS_MISC1UNIT_HPP

#include "../UtilitiesAPI.hpp"
#include "Unit.hpp"

namespace openstudio {
namespace detail {

  class Misc1Unit_Impl;

}  // namespace detail

/** Structure to hold Misc1Unit exponents needed for Misc1Unit construction. For purposes of
 *  conversion we need a length base unit. Therefore, gal cannot be a base unit. For convenience
 *  of construction, Misc1Expnt takes a gal argument, but subsequently this is stored as a power of
 *  cube root of gallons (crgal). \relates Misc1Unit */
struct UTILITIES_API Misc1Expnt
{
 public:
  Misc1Expnt(int ftH2O = 0, int L = 0, int day = 0, int K = 0, int A = 0, int cd = 0, int mol = 0, int rad = 0, int sr = 0, int people = 0,
             int cycle = 0, int dollar = 0)
    : m_ftH2O(ftH2O),
      m_crL(3 * L),
      m_day(day),
      m_K(K),
      m_A(A),
      m_cd(cd),
      m_mol(mol),
      m_rad(rad),
      m_sr(sr),
      m_people(people),
      m_cycle(cycle),
      m_dollar(dollar) {}

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
  int m_dollar;

  friend class detail::Misc1Unit_Impl;
};

/** Misc1Unit is a Unit with baseUnits fixed by its constructors, see Misc1Expnt. setBaseUnitExponent
 *  throws an exception if any other string is passed in as a baseUnit. Misc1Unit.hpp declares
 *  related operators and UnitFactory callback functions. */
class UTILITIES_API Misc1Unit : public Unit
{
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Default constructor.
     *
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] scaleExponent exponent for scale. For instance 3 for kilo.
     *  \param[in] prettyString optional string to use in place of standardString. */
  Misc1Unit(const Misc1Expnt& exponents = Misc1Expnt(), int scaleExponent = 0, const std::string& prettyString = "");

  /** Alternate constructor. Specify the abbreviation of the scale, rather than its
     *  exponent.
     *
     *  \param[in] scaleAbbreviation is string equal to a scale abbreviation. For instance
     *    "k" for kilo.
     *  \param[in] exponents holds the exponents for each base unit.
     *  \param[in] prettyString optional string to use in place of standardString. */
  Misc1Unit(const std::string& scaleAbbreviation, const Misc1Expnt& exponents = Misc1Expnt(), const std::string& prettyString = "");

  virtual ~Misc1Unit() = default;

  //@}
 protected:
  /// @cond
  using ImplType = detail::Misc1Unit_Impl;

  explicit Misc1Unit(std::shared_ptr<detail::Misc1Unit_Impl> impl);

  friend class Unit;
  friend class detail::Misc1Unit_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.units.Misc1Unit");
};

/** \relates Misc1Unit*/
using OptionalMisc1Unit = boost::optional<Misc1Unit>;

/** \relates Misc1Unit*/
using Misc1UnitVector = std::vector<Misc1Unit>;

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
/** relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1Currency();

// first order derived units

/** Liters (L). \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1Volume();
/** Lumen (lm) = cd*sr. \relates Misc1Unit */
UTILITIES_API Misc1Unit createMisc1LuminousFlux();

//@}
}  // namespace openstudio

#endif  // UTILITIES_UNITS_MISC1UNIT_HPP
