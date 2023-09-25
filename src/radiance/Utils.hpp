/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RADIANCE_UTILS_HPP
#define RADIANCE_UTILS_HPP

#include "RadianceAPI.hpp"

#include <string>
#include <vector>

namespace openstudio {
namespace radiance {

  RADIANCE_API std::vector<double> parseGenDayMtxLine(const std::string& t_line);

}
}  // namespace openstudio

#endif  // RADIANCE_UTILS_HPP
