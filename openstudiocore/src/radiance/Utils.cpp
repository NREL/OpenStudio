
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
        double d;
        ss >> d;
        colors[color] = std::max(d, 0.0);
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


