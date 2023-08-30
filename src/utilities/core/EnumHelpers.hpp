/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_ENUMHELPERS_HPP
#define UTILITIES_CORE_ENUMHELPERS_HPP

#include "Containers.hpp"

namespace openstudio {

template <class _enum_name>
bool isMember(const _enum_name& e, const StringVector& strValues) {
  for (const std::string& str : strValues) {
    try {
      _enum_name setMember(str);
      if (e == setMember) {
        return true;
      }
    } catch (...) {
    }
  }
  return false;
}

}  // namespace openstudio

#endif  // UTILITIES_CORE_ENUMHELPERS_HPP
