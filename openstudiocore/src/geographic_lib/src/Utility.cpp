/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include <cstdlib>
#include <GeographicLib/Utility.hpp>

#if defined(_MSC_VER)
// Squelch warnings about unsafe use of getenv
#  pragma warning (disable: 4996)
#endif

namespace GeographicLib {

  using namespace std;

  bool Utility::ParseLine(const std::string& line,
                          std::string& key, std::string& val) {
    const char* spaces = " \t\n\v\f\r";
    string::size_type n0 = line.find_first_not_of(spaces);
    if (n0 == string::npos)
      return false;             // Blank line
    string::size_type n1 = line.find_first_of('#', n0);
    if (n0 == n1)
      return false;             // Only a comment
    val = line.substr(n0, n1 == string::npos ? n1 : n1 - n0);
    n0 = val.find_first_of(spaces);
    key = val.substr(0, n0);
    if (n0 == string::npos) {
      val = "";
      return true;
    }
    n0 = val.find_first_not_of(spaces, n0);
    if (n0 == string::npos) {
      val = "";
      return true;
    }
    n1 = val.find_last_not_of(spaces);
    val = val.substr(n0, n1 + 1 - n0);
    return true;
  }

  int Utility::set_digits(int ndigits) {
#if GEOGRAPHICLIB_PRECISION == 5
    if (ndigits <= 0) {
      char* digitenv = getenv("GEOGRAPHICLIB_DIGITS");
      if (digitenv)
        ndigits = strtol(digitenv, NULL, 0);
      if (ndigits <= 0)
        ndigits = 256;
    }
#endif
    return Math::set_digits(ndigits);
  }

} // namespace GeographicLib
