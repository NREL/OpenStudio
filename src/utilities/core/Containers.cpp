/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Containers.hpp"

namespace openstudio {

StringVector eraseEmptyElements(const StringVector& sv) {
  StringVector result;
  for (const std::string& str : sv) {
    if (!str.empty()) {
      result.push_back(str);
    }
  }
  return result;
}

}  // namespace openstudio
