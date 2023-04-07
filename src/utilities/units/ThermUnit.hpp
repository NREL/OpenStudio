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

  virtual ~ThermUnit() = default;

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
