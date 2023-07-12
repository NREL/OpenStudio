/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_MATH_FLOATCOMPARE_HPP
#define UTILITIES_MATH_FLOATCOMPARE_HPP

#include <limits>
#include <cmath>

namespace openstudio {

template <class T>
bool equal(T val1, T val2, T tol = std::numeric_limits<T>::epsilon()) {
  T diff = val1 - val2;
  if (std::fabs(diff) < tol) {
    return true;
  }
  return std::fabs(diff) <= ((std::fabs(val1) < std::fabs(val2) ? std::fabs(val2) : std::fabs(val1)) * tol);
}

template <class T>
bool lessThanOrEqual(T val1, T val2, T tol = std::numeric_limits<T>::epsilon()) {
  return (val1 < val2) || equal(val1, val2, tol);
}

template <class T>
bool greaterThanOrEqual(T val1, T val2, T tol = std::numeric_limits<T>::epsilon()) {
  return (val1 > val2) || equal(val1, val2, tol);
}

template <class T>
double relativeError(T expected, T actual, T tol = std::numeric_limits<T>::epsilon()) {
  if (std::fabs(expected) <= tol) {
    return std::fabs(expected - actual);
  }
  return (std::fabs(expected - actual) / std::fabs(expected));
}

}  // namespace openstudio

#endif  // UTILITIES_MATH_FLOATCOMPARE_HPP
