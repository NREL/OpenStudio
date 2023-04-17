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
