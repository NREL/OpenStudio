/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "Utils.hpp"

#include <cassert>
#include <sstream>

namespace openstudio {
namespace radiance {

  std::vector<double> parseGenDayMtxLine(const std::string &t_line)
  {
    std::stringstream ss(t_line);
    std::vector<double> retval(8760);

    int valuenum=0;
    while (ss.good() && valuenum < 8760)
    {
      std::vector<double> colors(3, 0);
      int color = 0;
      for (; ss.good() && color < 3; ++color)
      {
        char c = static_cast<char>(ss.peek());
        if (c == ' ' || c == '\t')
        {
          ss.ignore();
        }

        char p = static_cast<char>(ss.peek());
        if (p < '0' || p > '9')
        {
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
      retval[valuenum] = 179*((colors[0] * 0.265) + (colors[1] * 0.67) + (colors[2] * 0.065));
      ++valuenum;
    }

    retval.resize(valuenum);
    return retval;
  }

}
}


