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

#include "Utils.hpp"

#include <cassert>
#include <sstream>

namespace openstudio {
namespace radiance {

  std::vector<double> parseGenDayMtxLine(const std::string& t_line) {
    std::stringstream ss(t_line);
    std::vector<double> retval(8760);

    int valuenum = 0;
    while (ss.good() && valuenum < 8760) {
      std::vector<double> colors(3, 0);
      int color = 0;
      for (; ss.good() && color < 3; ++color) {
        char c = static_cast<char>(ss.peek());
        if (c == ' ' || c == '\t') {
          ss.ignore();
        }

        char p = static_cast<char>(ss.peek());
        if (p < '0' || p > '9') {
          // this is either a nan, or a -nan or a -number, which we cannot handle
          // and don't care about
          colors[color] = 0;
          // eat the string of whatever it is
          std::string str;
          ss >> str;
        } else {
          double d;
          ss >> d;
          colors[color] = d;
        }
      }

      assert(color == 3 && "not an even multiple of 3 read");
      retval[valuenum] = 179 * ((colors[0] * 0.265) + (colors[1] * 0.67) + (colors[2] * 0.065));
      ++valuenum;
    }

    retval.resize(valuenum);
    return retval;
  }

}  // namespace radiance
}  // namespace openstudio
