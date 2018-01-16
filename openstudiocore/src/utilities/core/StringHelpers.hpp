/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_CORE_STRINGHELPERS_HPP
#define UTILITIES_CORE_STRINGHELPERS_HPP

#include "../UtilitiesAPI.hpp"

#include "./Enum.hpp"

#include <boost/regex.hpp>

#include <string>

namespace openstudio {

  /** \class DocumentFormat
  * \brief Document output formats supported by OpenStudio.
  * \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
  * macro call is:
  * \code
  OPENSTUDIO_ENUM(DocumentFormat,
  ((COUT))
  ((LaTeX))
  ((XHTML)) );
  * \endcode */
  OPENSTUDIO_ENUM(DocumentFormat,
    ((COUT))
    ((LaTeX))
    ((XHTML)));

  /** Replace all non-letters and digits with spaces, and then remove spaces by to make camel
   *  case. */
  UTILITIES_API std::string toCamelCase(const std::string& s);

  /** Make camel case then upcase the first letter. */
  UTILITIES_API std::string toUpperCamelCase(const std::string& s);

  /** Make camel case then downcase the first letter. */
  UTILITIES_API std::string toLowerCamelCase(const std::string& s);

  /** Make underscore case. */
  UTILITIES_API std::string toUnderscoreCase(const std::string& s);

  /** Convert IDD object and field names to something reasonable for computers. */
  UTILITIES_API std::string convertIddName(const std::string& s);

  /** Convert IDD object name to default name for IDF objects of this type. */
  UTILITIES_API std::string iddObjectNameToIdfObjectName(const std::string& s);

  /** Prints value with numFractionalDigits, inserting commas to separate into
   *  thousands, millions, etc. if desired. */
  UTILITIES_API std::string toNeatString(double value,
                                         unsigned numFractionalDigits = 0,
                                         bool applyCommas = true);

  /** Prints value in numSigFigs, inserting commas to separate into thousands,
  millions, etc. if desired. */
  UTILITIES_API std::string toNeatStringBySigFigs(double value,
                                                  unsigned numSigFigs = 3,
                                                  bool applyCommas = true);

  /** Returns the number of digits past the decimal point in str. */
  UTILITIES_API unsigned numFractionalDigits(const std::string& str);

  /** Returns the number of significant digits past the decimal point in value, given numSigFigs.
   *  Throws if numSigFigs == 0. */
  UTILITIES_API unsigned numFractionalDigits(double value,unsigned numSigFigs);

  /** Returns the minimum and maximum number of significant digits past the decimal
   *  point in values, given numSigFigs. Throws if numSigFigs == 0. */
  UTILITIES_API std::pair<unsigned,unsigned> numFractionalDigits(const std::vector<double>& values,
                                                                 unsigned numSigFigs);

  /** Rounds value to a number of significant figures (rather than to a particular
   *  decimal place). Throws if numSigFigs == 0. */
  UTILITIES_API double toNumSigFigs(double value, unsigned numSigFigs);

  UTILITIES_API boost::regex superscript();

  UTILITIES_API boost::regex subscript();

  UTILITIES_API std::string formatText(const std::string& str, DocumentFormat fmt);

  /** Ensures that underscores will remain as underscores (not changed to subscripts). */
  UTILITIES_API std::string formatUnderscore(const std::string& str);

  UTILITIES_API std::string formatSuperAndSubscripts(const std::string& str, DocumentFormat fmt);

  /** Applies formatSuperAndSubscripts, and replaces '*' with small dot. Does nothing for fmt ==
  * DocumentFormat::COUT. */
  UTILITIES_API std::string formatUnitString(const std::string& str, DocumentFormat fmt);

  /** Split a string into a vector of strings given a delimiter character. An empty string results
   *  in an empty vector, strings that begin or end with the delimiter will result in a vector
   *  beginning or ending with an empty string, and non-empty strings with no delimiters returns
   *  a vector with the input string as the only element. */
  UTILITIES_API std::vector <std::string> splitString(const std::string & string, char delimiter);

}

#endif // UTILITIES_CORE_STRINGHELPERS_HPP
