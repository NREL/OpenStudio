/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
