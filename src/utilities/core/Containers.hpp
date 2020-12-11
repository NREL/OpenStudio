/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_CORE_CONTAINERS_HPP
#define UTILITIES_CORE_CONTAINERS_HPP

#include "../UtilitiesAPI.hpp"
#include "Compare.hpp"

#include <boost/optional.hpp>

#include <vector>
#include <set>
#include <string>

namespace openstudio {

typedef std::vector<bool> BoolVector;
typedef std::vector<unsigned> UnsignedVector;
typedef std::vector<int> IntVector;
typedef std::vector<double> DoubleVector;
typedef std::vector<std::string> StringVector;

typedef std::set<unsigned> UnsignedSet;
typedef std::set<int> IntSet;
typedef std::set<double> DoubleSet;
typedef std::set<std::string> StringSet;

/** Set of strings with case-insensitive comparison. */
typedef std::set<std::string, IstringCompare> IStringSet;

typedef std::pair<std::string, std::string> StringPair;

UTILITIES_API std::vector<std::string> eraseEmptyElements(const std::vector<std::string>& sv);

/** Helper function to cast a whole vector of objects. \relates IdfObject */
template <typename T, typename U>
std::vector<T> castVector(const std::vector<U>& objects) {
  std::vector<T> result;
  for (auto& object : objects) {
    T castObject = object.template cast<T>();
    result.push_back(castObject);
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
