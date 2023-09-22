/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_CONTAINERS_HPP
#define UTILITIES_CORE_CONTAINERS_HPP

#include "../UtilitiesAPI.hpp"
#include "Compare.hpp"

#include <boost/optional.hpp>
#include <vector>
#include <set>
#include <string>
#include <iterator>     // for make_move_iterator
#include <type_traits>  // for declval, conditional, decay_t, enable_if_t

namespace openstudio {

using BoolVector = std::vector<bool>;
using UnsignedVector = std::vector<unsigned int>;
using IntVector = std::vector<int>;
using DoubleVector = std::vector<double>;
using StringVector = std::vector<std::string>;

using UnsignedSet = std::set<unsigned int>;
using IntSet = std::set<int>;
using DoubleSet = std::set<double>;
using StringSet = std::set<std::string>;

/** Set of strings with case-insensitive comparison. */
using IStringSet = std::set<std::string, IstringCompare>;

using StringPair = std::pair<std::string, std::string>;

UTILITIES_API std::vector<std::string> eraseEmptyElements(const std::vector<std::string>& sv);

/** Helper function to cast a whole vector of objects. \relates IdfObject */
template <typename T, typename U>
std::vector<T> castVector(const std::vector<U>& objects) {
  std::vector<T> result;
  result.reserve(objects.size());
  for (auto& object : objects) {
    result.emplace_back(object.template cast<T>());
  }
  return result;
}

/** Returns a subset of original cast to a new type, only keeping those objects for which the
 *  optionalCast is successful. \relates IdfObject */
template <typename T, typename U>
std::vector<T> subsetCastVector(const std::vector<U>& original) {
  std::vector<T> result;
  for (const auto& elem : original) {
    boost::optional<T> oCastObject = elem.template optionalCast<T>();
    if (oCastObject) {
      result.push_back(*oCastObject);
    }
  }
  return result;
}

template <typename T, typename U>
std::vector<std::vector<T>> castArray(const std::vector<std::vector<U>>& original) {
  std::vector<std::vector<T>> result;
  for (const auto& elem : original) {
    typename std::vector<T> subVector = castVector<T>(elem);
    result.push_back(subVector);
  }
  return result;
}

}  // namespace openstudio

#endif  // UTILITIES_CORE_CONTAINERS_HPP
