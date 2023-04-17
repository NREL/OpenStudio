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

#ifndef UTILITIES_UNITS_SCALEFACTORY_HPP
#define UTILITIES_UNITS_SCALEFACTORY_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Singleton.hpp"
#include "../core/Logger.hpp"

#include "Scale.hpp"

#include <ostream>
#include <set>
#include <map>

namespace openstudio {

/** Singleton that creates scales based on std::string or exponent.
 *  Access using ScaleFactory::instance().  */
class UTILITIES_API ScaleFactorySingleton
{

  friend class Singleton<ScaleFactorySingleton>;

 public:
  /// Destructor.
  ~ScaleFactorySingleton() = default;

  /** Store a scale callback function for future lookup using the scale exponent or
   *  abbreviation. By default, SI scales are registered during factory construction. */
  bool registerScale(ScaleConstant scale);

  /** Returns a vector of Scale objects, one for each scale registered in the factory. */
  std::vector<Scale> registeredScales();

  /** Outputs the scales registered in the factory. */
  friend UTILITIES_API std::ostream& operator<<(std::ostream& os, const ScaleFactorySingleton& factory);

  /** Creates a scale from an exponent. Throws if the factory does not have any scales registered.
   *  Returns ScaleConstant with ().value == 0.0 if exponent not in factory. */
  ScaleConstant createScale(int exponent) const;

  /** Creates a scale from an abbreviation. Throws if the factory does not have any scales registered.
   *  Returns ScaleConstant with ().value == 0.0 if abbr not in factory. */
  ScaleConstant createScale(const std::string& abbr) const;

 private:
  REGISTER_LOGGER("openstudio.units.ScaleFactory");
  /// Private constructor initializes ScaleFactory with standard SI scales.
  ScaleFactorySingleton();

  using ExponentLookupMap = std::map<int, ScaleConstant>;
  using AbbreviationLookupMap = std::map<std::string, ScaleConstant>;
  ExponentLookupMap m_exponentMap;
  AbbreviationLookupMap m_abbreviationMap;

  ScaleConstant m_recoverFromFailedCreate() const;
};

using ScaleFactory = openstudio::Singleton<ScaleFactorySingleton>;

/** Prints scales that are registered in the factory (and thus available for use in
 *  Units and Quantities). */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const ScaleFactorySingleton& factory);

/** Wrapper around << for SWIG bindings. Prints each scale registered in ScaleFactory,
 *  and thus available for use in Units, Quantities, and bare Scale operations. */
UTILITIES_API std::string printScales();

/** If possible, extracts scale abbreviation from the front of str, returning abbreviation in .first
 *  and remainder of string in .second. Otherwise, returns empty strings. */
UTILITIES_API std::pair<std::string, std::string> extractScaleAbbreviation(const std::string& str);

/** Return type for scale multiplication and division. See the corresponding operators
 *  for its use. */
// DLM: this is ill-conceived and easy to misuse (which is what was happening) in #2240
// all operations (e.g. *) between two Scales should return a Scale
using ScaleOpReturnType = std::pair<ScaleConstant, double>;

/** Multiplication of scales. If result implied by adding exponents is not
 *  available, .second is amount multiplication of values should be multiplied
 *  by so result will be correct when using .first. */
UTILITIES_API ScaleOpReturnType operator*(const Scale& firstScale, const Scale& secondScale);

/** Division of scales. If result implied by subtracting exponents is not
 *  available, .second is amount final value should be multiplied by so result
 *  will be correct when using .first. */
UTILITIES_API ScaleOpReturnType operator/(const Scale& firstScale, const Scale& secondScale);

/** Raise scale to an integer power. If result implied by the operation is not
 *  available, .second is amount final value should be multiplied by so result
 *  will be correct when using .first. */
UTILITIES_API ScaleOpReturnType pow(const Scale& scale, int expNum, int expDenom = 1);

/** Returns scale with desiredExponent, if possible. Otherwise backtracks to nearby
 *  scale, and returns that scale along with a factor that the value should be
 *  multiplied by to make the end result consistent with the returned scale. */
UTILITIES_API ScaleOpReturnType scaleOpHelperFunction(int desiredExponent);

}  // namespace openstudio

#endif  // UTILITIES_UNITS_SCALEFACTORY_HPP
