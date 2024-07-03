/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_STRINGHELPERS_HPP
#define UTILITIES_CORE_STRINGHELPERS_HPP

#include "../UtilitiesAPI.hpp"

#include "./Enum.hpp"

#include <boost/regex.hpp>

#include <string>
#include <sstream>
#include <cstdint>

namespace openstudio {

// clang-format off

/** \class DocumentFormat
 * \brief Document output formats supported by OpenStudio.
 * \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual
 * macro call is:
 * \code
OPENSTUDIO_ENUM(DocumentFormat,
  ((COUT))
  ((LaTeX))
  ((XHTML))
);
 * \endcode */
OPENSTUDIO_ENUM(DocumentFormat,
  ((COUT))
  ((LaTeX))
  ((XHTML))
);

// clang-format on

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
UTILITIES_API std::string toNeatString(double value, unsigned numFractionalDigits = 0, bool applyCommas = true);

/** Prints value in numSigFigs, inserting commas to separate into thousands,
  millions, etc. if desired. */
UTILITIES_API std::string toNeatStringBySigFigs(double value, unsigned numSigFigs = 3, bool applyCommas = true);

/** Returns the number of digits past the decimal point in str. */
UTILITIES_API unsigned numFractionalDigits(const std::string& str);

/** Returns the number of significant digits past the decimal point in value, given numSigFigs.
   *  Throws if numSigFigs == 0. */
UTILITIES_API unsigned numFractionalDigits(double value, unsigned numSigFigs);

/** Returns the minimum and maximum number of significant digits past the decimal
   *  point in values, given numSigFigs. Throws if numSigFigs == 0. */
UTILITIES_API std::pair<unsigned, unsigned> numFractionalDigits(const std::vector<double>& values, unsigned numSigFigs);

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
UTILITIES_API std::vector<std::string> splitString(const std::string& string, char delimiter);

/** This takes the line of an EMS Program or Subroutine, and will split it into tokens,
   *  Before returning, it filters out the reserved keywords (IF, WHILE, RETURN, RUN, etc) and any function (starts with '@')
   *  As a result, what is returned is the only "words" that could be potential objects for UUID substitution.
   *  Note: JM 2018-08-16: The second parameters defaults to the operators currently accepted as of 8.9.0.
   *  The reserved keywords are hardcoded per the same 8.9.0 version.
   */
UTILITIES_API std::vector<std::string> splitEMSLineToTokens(const std::string& line, const std::string& delimiters=" +-*/^=<>&|");

/** trim from start (in place) */
UTILITIES_API void ltrim(std::string& s);

/** trim from end (in place) */
UTILITIES_API void rtrim(std::string& s);

/** trim from both ends (in place) */
UTILITIES_API void trim(std::string& s);

/** trim from start (copying) */
UTILITIES_API std::string ltrim_copy(std::string s);

/** trim from end (copying) */
UTILITIES_API std::string rtrim_copy(std::string s);

/** trim from both ends (copying) */
UTILITIES_API std::string trim_copy(std::string s);

UTILITIES_API std::string replace(std::string input, const std::string& before, const std::string& after);

enum struct FloatFormat
{
  fixed,
  general,
  general_capital
};

namespace string_conversions {
  /** Converts a number into a string, replicating the behavior of QString::number.
     *  This is necessary because QString does *not* respect the locale, and stringstring
     *  and std::string::to_string *do* respect the locale
     */
  UTILITIES_API std::string number(std::int32_t, int base = 10);
  UTILITIES_API std::string number(std::uint32_t, int base = 10);
  UTILITIES_API std::string number(std::int64_t, int base = 10);
  UTILITIES_API std::string number(std::uint64_t, int base = 10);
  UTILITIES_API std::string number(double, FloatFormat format = FloatFormat::general, int precision = 6);

  template <typename DesiredType, typename InputType>
  boost::optional<DesiredType> to_no_throw(const InputType& inp) {
    std::stringstream ss;
    ss.imbue(std::locale::classic());
    ss.str(inp);
    DesiredType o;
    ss >> o;
    if (!ss.eof() || ss.fail()) {
      return {};
    }
    return o;
  }

  template <typename DesiredType, typename InputType>
  DesiredType to(const InputType& inp) {
    const auto result = to_no_throw<DesiredType>(inp);
    if (result) {
      return result.value();
    } else {
      throw std::bad_cast{};
    }
  }

  template <typename DesiredType, typename InputType>
  bool is_valid(const InputType& input) {
    try {
      to<DesiredType>(input);
      return true;
    } catch (const std::bad_cast&) {
      return false;
    }
  }

  template <typename DesiredType, typename InputType, typename DestinationType>
  bool assign_if_valid(const InputType& input, DestinationType& dest) {
    if (is_valid<DesiredType>(input)) {
      dest = input;
      return true;
    } else {
      return false;
    }
  }
}  // namespace string_conversions
}  // namespace openstudio

#endif  // UTILITIES_CORE_STRINGHELPERS_HPP
