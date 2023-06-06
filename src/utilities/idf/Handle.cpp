/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Handle.hpp"

namespace openstudio {

Handle applyHandleMap(const Handle& original, const HandleMap& handleMap) {
  Handle result;
  auto it = handleMap.find(original);
  if (it != handleMap.end()) {
    result = it->second;
  }
  return result;
}

HandleVector applyHandleMap(const HandleVector& original, const HandleMap& handleMap) {
  HandleVector result;
  for (const Handle& oh : original) {
    auto it = handleMap.find(oh);
    if (it != handleMap.end()) {
      result.push_back(it->second);
    }
  }
  return result;
}

}  // namespace openstudio
