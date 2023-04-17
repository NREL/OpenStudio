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

#ifndef UTILITIES_CORE_CONTAINERSMOVE_HPP
#define UTILITIES_CORE_CONTAINERSMOVE_HPP

#include <iterator>     // for make_move_iterator
#include <type_traits>  // for declval, conditional, decay_t, enable_if_t
#include <vector>       // for vector

namespace openstudio {

namespace detail {

  // Define a helper test whether every predicate in a pack is true
  template <typename... Conds>
  struct and_ : std::true_type
  {
  };

  template <typename Cond, typename... Conds>
  struct and_<Cond, Conds...> : std::conditional<Cond::value, and_<Conds...>, std::false_type>::type
  {
  };

  // Check that 1) it's a container, and 2) it contains elements that are convertible to ElementType
  template <typename ElementType, typename Container, typename T = std::decay_t<decltype(*begin(std::declval<Container>()))>>
  using VectorConvertible = std::enable_if_t<std::is_convertible_v<T, ElementType>>;

  // Same, but for a parameter pack
  template <typename ElementType, typename... Vectors>
  using AllVectorsConvertible = and_<VectorConvertible<ElementType, Vectors>...>;

  // Helper: when lvalue is passed
  template <class Container1, class Container2>
  void concat_helper(Container1& lhs, Container2& rhs) {
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());
  }

  // Helper: when rvalue is passed
  template <class Container1, class Container2>
  void concat_helper(Container1& lhs, Container2&& rhs) {
    lhs.insert(lhs.end(), std::make_move_iterator(rhs.begin()), std::make_move_iterator(rhs.end()));
  }
}  // namespace detail

// This will concatenate any number of containers (Vectors are the primary target) into a single vector
// It has move-semantics in mind, for efficiency
// Usage: in this example
// * spaces is taken as an lvalue so elements are **copied** spaces is left intact
// * surfaces is taken as an xvalue so elements are **moved**, surfaces is left in an undefined state and shouldn't be used anymore
// * getSubSurfaces is a prvalue so it's **moved**
//   ```c++
//   std::vector<Space> spaces = getSpaces();
//   std::vector<Surface> surfaces = getSurfaces();
//   std::vector<ModelObject> mos = concat(spaces, std::move(surfaces), getSubSurfaces());
//   ```
template <typename RT, typename... Containers, typename = detail::AllVectorsConvertible<RT, Containers...>>
std::vector<RT> concat(Containers&&... containers) {
  std::size_t s = (containers.size() + ...);
  std::vector<RT> result;
  result.reserve(s);
  (detail::concat_helper(result, std::forward<Containers>(containers)), ...);
  return result;
}

}  // namespace openstudio
#endif  // UTILITIES_CORE_CONTAINERSMOVE_HPP
