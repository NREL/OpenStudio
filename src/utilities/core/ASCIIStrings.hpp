/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_ASCIISTRINGS_HPP
#define UTILITIES_CORE_ASCIISTRINGS_HPP

#include <string>
#include <string_view>

namespace openstudio {

inline std::string ascii_to_lower_copy(std::string_view input) {
  std::string result{input};
  constexpr auto to_lower_diff = 'a' - 'A';
  for (auto& c : result) {
    if (c >= 'A' && c <= 'Z') {
      c += to_lower_diff;
    }
  }
  return result;
}

inline std::string ascii_to_upper_copy(std::string_view input) {
  std::string result{input};
  constexpr auto to_upper_diff = 'A' - 'a';
  for (auto& c : result) {
    if (c >= 'a' && c <= 'z') {
      c += to_upper_diff;
    }
  }
  return result;
}

inline std::string_view ascii_trim_left(std::string_view s) {
  return s.substr(std::min(s.find_first_not_of(" \f\n\r\t\v"), s.size()));
}

inline std::string_view ascii_trim_right(std::string_view s) {
  return s.substr(0, std::min(s.find_last_not_of(" \f\n\r\t\v") + 1, s.size()));
}

inline std::string_view ascii_trim(std::string_view s) {
  return ascii_trim_left(ascii_trim_right(s));
}

inline void ascii_trim(std::string& str) {
  str = std::string(ascii_trim(std::string_view(str)));
}

}  // namespace openstudio

#endif
