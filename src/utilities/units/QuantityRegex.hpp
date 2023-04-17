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

#ifndef UTILITIES_UNITS_QUANTITYREGEX_HPP
#define UTILITIES_UNITS_QUANTITYREGEX_HPP

#include "../UtilitiesAPI.hpp"
#include <boost/regex.hpp>

#include <string>

namespace openstudio {

/// Fixed precision values: 23, 0.126, .1
UTILITIES_API const boost::regex& regexFixedPrecisionValue();

/** Fixed precision value embedded in larger string. Must be set off with spaces or newline
 *  characters. May be terminated with a '. ', ', ', or ';'. */
UTILITIES_API const boost::regex& regexEmbeddedFixedPrecisionValue();

/** Returns true if s is a FixedPrecisionValue. No other text is tolerated, including whitespace. */
UTILITIES_API bool isFixedPrecisionValue(const std::string& s);

/** Returns true if s contains a FixedPrecisionValue. Other text separated by whitespace is tolerated,
 *  but ScientificNotationValues and the like are not. */
UTILITIES_API bool containsFixedPrecisionValue(const std::string& s);

/// Scientific notation values: 23E10, 1.2E+0, 0.361D-32, .3E100
UTILITIES_API const boost::regex& regexScientificNotationValue();

/* Scientific notation value embedded in larger string. Must be set off with spaces or newline
 *  characters. May be terminated with a '. ', ', ', or ';'. */
UTILITIES_API const boost::regex& regexEmbeddedScientificNotationValue();

/** Returns true if s is a ScientificNotationValue. No other text is tolerated, including whitespace. */
UTILITIES_API bool isScientificNotationValue(const std::string& s);

/** Returns true if s contains a ScientificNotationValue. Other text is tolerated as long as it is
 *  separated from the value by whitespace. */
UTILITIES_API bool containsScientificNotationValue(const std::string& s);

/// Base unit: kg, kN, etc. Limits unit strings to 10 characters.
UTILITIES_API const boost::regex& regexBaseUnit();

/// Exponent: 0, 1, -2, etc.
UTILITIES_API const boost::regex& regexExponent();

/// Atomic unit: kN^2, s, ms. Limits unit strings to 10 characters.
UTILITIES_API const boost::regex& regexAtomicUnit();

/** Atomic unit embedded in larger string. Must be set off with spaces, newline characters,
 *  *, /, (, or ) */
UTILITIES_API const boost::regex& regexEmbeddedAtomicUnit();

/** Returns true if s is an atomic unit. No other text is tolerated, including whitespace. */
UTILITIES_API bool isAtomicUnit(const std::string& s);

/** Returns true if s contains an atomic unit. Other text is tolerated as long as it is separated
 *  from the unit by whitespace, *, /, (, or ) */
UTILITIES_API bool containsAtomicUnit(const std::string& s);

/** Compound unit: kg*m^2/s^2, kN, s, 1/s. An atomic unit is a compound unit. A compound unit is not
 *  surrounded by Scale.abbr( ). */
UTILITIES_API const boost::regex& regexCompoundUnit();

/** Compound unit embedded in larger string. Must be set off with spaces, newline characters, or
 *  ( ). May be terminated with a '. ', ', ', or ';'. */
UTILITIES_API const boost::regex& regexEmbeddedCompoundUnit();

/** Returns true if s is a compound unit. No other text is tolerated, including whitespace. */
UTILITIES_API bool isCompoundUnit(const std::string& s);

/** Returns true if s contains a compound unit. Other text is tolerated as long as the compound unit
 *  is set off by whitespace or parentheses. */
UTILITIES_API bool containsCompoundUnit(const std::string& s);

/** Scaled unit: k(kg^2/m^2). A compound unit surrounded by Scale.abbr\(\). */
UTILITIES_API const boost::regex& regexScaledUnit();

/** Scaled unit embedded in larger string. Must be set off with spaces or newline characters.
 *  May be terminated with a '. ', ', ', or ';'. */
UTILITIES_API const boost::regex& regexEmbeddedScaledUnit();

/** Returns true if s is a scaled unit. No other text is tolerated, including whitespace. */
UTILITIES_API bool isScaledUnit(const std::string& s);

/** Returns true if s contains a scaled unit. Other text is tolerated as long as the scaled unit
 *  is set off by spaces or newline characters. May be terminated with a '. ', ', ', or ';'. */
UTILITIES_API bool containsScaledUnit(const std::string& s);

/** Textual scaled unit: people/1000 ft^2. A compound unit with 10+ embedded in the denominator.
 *
 *  match[1] = "numerator units/"
 *  match[2] = "10+"
 *  match[3] = "denominator units" */
UTILITIES_API const boost::regex& regexDirectScaledUnit();

/** Textual scaled unit embedded in larger string. Must be set off with spaces, newline characters,
 *  or matching parentheses or brackets. Match will be in [1], [5], [9], or [13]. */
UTILITIES_API const boost::regex& regexEmbeddedDirectScaledUnit();

/** Returns true if s is a direct scaled unit. No other text is tolerated, including whitespaces.
 */
UTILITIES_API bool isDirectScaledUnit(const std::string& s);

/** Returns true if s contains a direct scaled unit. Other text is tolerated as long as the direct
 *  scaled unit is set off by spaces, newline characters, or matching parentheses or brackes. May
 *  be terminated with a '. ', ', ', or ';'. */
UTILITIES_API bool containsDirectScaledUnit(const std::string& s);

/** Decomposes a direct scaled unit string into numerator, exponent of scale in denominator
 *  (3 for /1000), and denominator. */
UTILITIES_API std::pair<std::string, std::pair<unsigned, std::string>> decomposeDirectScaledUnit(const std::string& s);

/** Unit: kg/s, kBtu/h, k(m^2), 1/m^2. A compound unit (which includes atomic units) or a scaled
 *  unit. */
UTILITIES_API const boost::regex& regexUnit();

/** Unit embedded in larger string. Must be set off with spaces, newline characters, or matching
 *  parentheses or brackets. May be terminated with a '. ', ', ', or ';'. */
UTILITIES_API const boost::regex& regexEmbeddedUnit();

/** Returns true if s is a unit. No other text is tolerated, including whitespace. */
UTILITIES_API bool isUnit(const std::string& s);

/** Returns true if s contains a scaled unit. Other text is tolerated as long as the scaled unit
 *  is set off by spaces or newline characters. May be terminated with  a '. ', ', ', or ';'. */
UTILITIES_API bool containsUnit(const std::string& s);

/** Quantity: 3 kN, -1.021D-2 k(m^2), 2 Hz, 3.0 1/ms, 3.0/ms. A value (fixed precision, or
 *  scientific noation) and a unit separated by a space or a '/'.
 *
 *  \li matches[1] - value
 *  \li matches[2] - one character--space or '/'
 *  \li matches[3] - unit string */
UTILITIES_API const boost::regex& regexQuantity();

/** Quantity embedded in larger string. Must be set off with spaces or newline characters. May
 *  be terminated with a '. ', ', ', or ';'. */
UTILITIES_API const boost::regex& regexEmbeddedQuantity();

/** Returns true if s is a quantity. No other text is tolerated, including whitespace. */
UTILITIES_API bool isQuantity(const std::string& s);

/** Returns true if s contains a quantity. Other text is tolerated as long as the scaled unit
 *  is set off by spaces or newline characters. May be terminated with  a '. ', ', ', or ';'. */
UTILITIES_API bool containsQuantity(const std::string& s);

/** Returns value string as .first and unit string as .second. Precondition: isQuantity(s) == true.
 *  Throws otherwise. */
UTILITIES_API std::pair<std::string, std::string> decomposeQuantityString(const std::string& s);

/** Returns scale abbreviation as .first and compoundUnitString as .second. Precondition:
 *  isScaledUnit(s) == true. Throws otherwise. */
UTILITIES_API std::pair<std::string, std::string> decomposeScaledUnitString(const std::string& s);

/** Returns vectors of strings, where each string contains atomic unit. First vector is numerator,
 *  second is denominator. Precondition: isCompoundUnit(s) == true. Throws otherwise. */
UTILITIES_API std::pair<std::vector<std::string>, std::vector<std::string>> decomposeCompoundUnitString(const std::string& s);

/** Returns baseUnit string and integer exponent. Precondition: isAtomicUnit(s) == true. Throws
 *  otherwise. */
UTILITIES_API std::pair<std::string, int> decomposeAtomicUnitString(const std::string& s);

}  // namespace openstudio

#endif  // UTIILITIES_UNITS_QUANTITYREGEX_HPP
