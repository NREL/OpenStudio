/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_MATH_PERMUTATION_HPP
#define UTILITIES_MATH_PERMUTATION_HPP

#include "../core/Assert.hpp"
#include <vector>

namespace openstudio {

/** Returns the permutation that transforms sequence container original into newOrder.
 *  T is required to have .size() and [i] defined. The type of element held by T must
 *  have == defined. original and newOrder should contain the same elements, jut in a
 *  different order. */
template <class T>
std::vector<unsigned> permutation(const T& original, const T& newOrder) {

  OS_ASSERT(original.size() == newOrder.size());

  std::vector<unsigned> result(original.size());
  std::vector<bool> usedAsJ(newOrder.size(), false);
  unsigned start = 0;

  for (unsigned i = 0; i < original.size(); ++i) {
    for (unsigned j = start; j < newOrder.size(); ++j) {
      if ((original[i] == newOrder[j]) && (!usedAsJ[j])) {
        result[i] = j;
        usedAsJ[j] = true;
        while ((start < newOrder.size()) && (usedAsJ[start])) {
          ++start;
        }
        break;
      }
    }
  }

  return result;
}

/** Returns the contents of original reorders as per order. That is, result[order[i]] =
 *  original[i]. T is required to have .size() and [i] defined. T must also be constructable
 *  from .size(). */
template <class T>
T permute(const T& original, const std::vector<unsigned>& order) {
  OS_ASSERT(original.size() == order.size());
  T result(original.size());
  for (unsigned i = 0; i < original.size(); ++i) {
    result[order[i]] = original[i];
  }
  return result;
}

}  // namespace openstudio

#endif  // UTILITIES_MATH_PERMUTATION_HPP
