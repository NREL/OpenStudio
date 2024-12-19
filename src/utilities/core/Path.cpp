/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Path.hpp"
#include "String.hpp"

#if (defined(_WIN32) || defined(_WIN64))
#  include <locale>
#  include <codecvt>
#  include "Logger.hpp"
#endif

namespace openstudio {

/*
  | Class       | Internal      | Platform  |
  | std::string | UTF-8, char   | All       |
  | path        | UTF-8, char   | Unix, Mac |
  | path        | UTF-16, wchar | Windows   |
  | QString     | UTF-16, wchar | All       |

*/

// allow path to be written to cout on Windows
std::ostream& operator<<(std::ostream& os, const path& p) {
  os << toString(p);
  return os;
}

/** path to a temporary directory. */
path tempDir() {
  return openstudio::filesystem::temp_directory_path();
}

/** path to UTF-8 encoding. */
std::string toString(const path& p) {
#if (defined(_WIN32) || defined(_WIN64))
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> converter;
  std::string s = converter.to_bytes(p.generic_wstring());
  return s;
#endif

  // cppcheck-suppress duplicateBreak
  return p.generic_string();
}

/** UTF-8 encoded char* to path*/
path toPath(const char* s) {
  return toPath(toString(s));
}

/** UTF-8 encoded std::string to path*/
path toPath(const std::string& s) {
#if (defined(_WIN32) || defined(_WIN64))
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  try {
    std::wstring wstr = converter.from_bytes(s);
    return path(wstr);
  } catch (std::range_error& e) {
    // Fallback to basically decoding latin-1
    // Masking with 0xFF reduces any negative values into the range 0-255.
    // TODO: another option is to write a codecvt_latin1 class
    // cf: https://stackoverflow.com/questions/49669048/why-mask-a-char-with-0xff-when-converting-narrow-string-to-wide-string#49669048
    size_t length = s.length();
    std::wstring wstr;
    wstr.reserve(length);
    for (size_t i = 0; i < length; ++i) {
      wstr.push_back(s[i] & 0xFF);
    }
    LOG_FREE(Debug, "Path",
             "An error occurred trying to convert the input to a wide string." << "\n  Reason: " << e.what() << "\n  Input =" << s
                                                                               << "\n  Result=" << wstr);
    return path(wstr);
  }
#endif

  // else
  return path(s);
}

path toPath(std::string_view s) {
  return toPath(std::string(s));
}

#ifdef _WIN32
/** UTF-16 encoded std::wstring for opening fstreams*/
std::wstring toSystemFilename(const path& p) {
  return p.wstring();
}
#else
/** UTF-8 encoded std::string for opening fstreams*/
std::string toSystemFilename(const path& p) {
  return p.string();
}
#endif

}  // namespace openstudio
