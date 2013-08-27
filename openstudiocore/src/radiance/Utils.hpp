#ifndef OPENSTUDIO_RADIANCE_UTIL_HPP
#define OPENSTUDIO_RADIANCE_UTIL_HPP

#include "RadianceAPI.hpp"

#include <string>
#include <vector>


namespace openstudio {
namespace radiance {

  RADIANCE_API std::vector<double> parseGenDayMtxLine(const std::string &t_line);
    
}
}

#endif

