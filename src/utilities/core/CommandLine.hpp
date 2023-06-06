/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_COMMANDLINE_HPP
#define UTILITIES_CORE_COMMANDLINE_HPP

#include "../UtilitiesAPI.hpp"
#include <boost/program_options.hpp>

#include <string>
#include <vector>

/** Extend boost::program_options namespace to include platform specific functions for parsing
*   command line arguments.  Assume UTF-16LE on Windows and UTF-8 elsewhere.
**/

namespace boost {
namespace program_options {

#if (defined(_WIN32) || defined(_WIN64))
  typedef std::wstring string;
  typedef std::vector<std::wstring> string_vector;

  boost::program_options::typed_value<string, wchar_t>* value(string* str) {
    return boost::program_options::wvalue<string>(str);
  }
  boost::program_options::typed_value<string_vector, wchar_t>* value(string_vector* vec) {
    return boost::program_options::wvalue<string_vector>(vec)->multitoken();
  }
#else
  using string = std::string;
  using string_vector = std::vector<std::string>;

  boost::program_options::typed_value<string, char>* value(string* str) {
    return boost::program_options::value<string>(str);
  }
  boost::program_options::typed_value<string_vector, char>* value(string_vector* vec) {
    return boost::program_options::value<string_vector>(vec)->multitoken();
  }
#endif
}  // namespace program_options
}  // namespace boost

#endif  // UTILITIES_CORE_COMMANDLINE_HPP
