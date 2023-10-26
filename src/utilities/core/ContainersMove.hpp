/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
