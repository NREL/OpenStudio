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

#ifndef UTILITIES_MATH_PERMUTATION_HPP
#define UTILITIES_MATH_PERMUTATION_HPP

#include "../core/Assert.hpp"
#include <vector>

namespace openstudio {

/** Returns the permutation that transforms sequence container original into newOrder. 
 *  T is required to have .size() and [i] defined. The type of element held by T must 
 *  have == defined. original and newOrder should contain the same elements, jut in a 
 *  different order. */
template<class T> 
std::vector<unsigned> permutation(const T& original,const T& newOrder) {

  OS_ASSERT(original.size() == newOrder.size());

  std::vector<unsigned> result(original.size());
  std::vector<bool> usedAsJ(newOrder.size(),false);
  unsigned start = 0;

  for (unsigned i = 0; i < original.size(); ++i) {
    for (unsigned j = start; j < newOrder.size(); ++j) {
      if ((original[i] == newOrder[j]) && (!usedAsJ[j])) {
        result[i] = j;
        usedAsJ[j] = true;
        while ((start < newOrder.size()) && (usedAsJ[start])) { ++start; }
        break;
      }
    }
  }

  return result;
}

/** Returns the contents of original reorders as per order. That is, result[order[i]] = 
 *  original[i]. T is required to have .size() and [i] defined. T must also be constructable
 *  from .size(). */
template<class T>
T permute(const T& original,const std::vector<unsigned>& order) {
  OS_ASSERT(original.size() == order.size());
  T result(original.size());
  for (unsigned i = 0; i < original.size(); ++i) {
    result[order[i]] = original[i];
  }
  return result;
}

} // openstudio

#endif // UTILITIES_MATH_PERMUTATION_HPP
