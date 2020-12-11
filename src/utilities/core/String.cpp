/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "String.hpp"

#include "Logger.hpp"

#include <codecvt>
#include <iomanip>

namespace openstudio {

/** char* to std::string. */
std::string toString(const char* s) {
  return std::string(s);
}

/** string to std::string. */
std::string toString(const std::string& s) {
  return s;
}

/** wchar_t* to std::string. */
std::string toString(const wchar_t* w) {
  return toString(std::wstring(w));
}

std::string toString(double v) {

  std::string result;

  // Return Infinity or NaN as strings, getDouble in IdfObject will fail to convert to double which will cause setDouble to fail
  if (std::isinf(v)) {
    if (v < 0) {
      result = "-Infinity";
    } else {
      result = "Infinity";
    }
  } else if (std::isnan(v)) {
    result = "NaN";
  } else {

    std::stringstream ss;
    ss << std::setprecision(std::numeric_limits<double>::digits10) << v;
    result = ss.str();
  }

  return result;
}

std::string toString(std::istream& s) {
  // istream -> string code from
  // http://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
  std::string contents;
  s.seekg(0, std::ios::end);
  contents.resize(s.tellg());
  s.seekg(0, std::ios::beg);
  s.read(&contents[0], contents.size());
  return contents;
}

///
/// Note that MSVC has linker issues with wchars for some weird reason
/// Hence these ifdef workarounds https://stackoverflow.com/questions/32055357/visual-studio-c-2015-stdcodecvt-with-char16-t-or-char32-t
///

std::wstring toWString(const std::string& s) {
#if _MSC_VER >= 1900
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
  const auto u16_conv = convert.from_bytes(s);
  return {u16_conv.begin(), u16_conv.end()};
#else
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
  const auto u16_conv = convert.from_bytes(s);
  return {u16_conv.begin(), u16_conv.end()};
#endif
}

std::string toString(const std::wstring& utf16_string) {
#if _MSC_VER >= 1900
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
  return convert.to_bytes(utf16_string.data(), utf16_string.data() + utf16_string.size());
#else
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
  const std::u16string u16string{utf16_string.begin(), utf16_string.end()};
  return convert.to_bytes(u16string);
#endif
}

}  // namespace openstudio
