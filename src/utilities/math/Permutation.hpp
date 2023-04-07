/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
