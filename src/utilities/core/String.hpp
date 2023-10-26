/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_STRING_HPP
#define UTILITIES_CORE_STRING_HPP

#include "../UtilitiesAPI.hpp"

#include <string>
#include <vector>

/** \file String.hpp
 *
 *  All strings are assumed to be UTF-8 encoded std::string.  Note that length of the std::string
 *  may therefore not match number of characters in the std::string. */

namespace openstudio {

/** string to std::string. */
UTILITIES_API std::string toString(const std::string& s);

/** char* to std::string. */
UTILITIES_API std::string toString(const char* s);

/** wstring to std::string. */
UTILITIES_API std::string toString(const std::wstring& utf16_string);

/** string to std::wstring. */
UTILITIES_API std::wstring toWString(const std::string& s);

/** wchar_t* to std::string. */
UTILITIES_API std::string toString(const wchar_t* w);

/** Double to std::string at full precision. */
UTILITIES_API std::string toString(double v);

/** Load data in istream into string. */
UTILITIES_API std::string toString(std::istream& s);

}  // namespace openstudio

#endif  // UTILITIES_CORE_STRING_HPP
