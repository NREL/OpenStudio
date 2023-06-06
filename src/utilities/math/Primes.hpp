/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_MATH_PRIMES_HPP
#define UTILITIES_MATH_PRIMES_HPP

#include <cmath>

namespace openstudio {

template <class T>
bool isPrime(T val) {
  bool prime = true;

  if (val < 2) {
    return false;
  } else if (val == 2) {
    return true;
  } else if (val % 2 == 0) {
    return false;
  } else {
    T divisor(3);
    auto dval = static_cast<double>(val);
    T upperLimit = static_cast<T>(std::sqrt(dval) + 1);

    while (divisor <= upperLimit) {
      if (val % divisor == 0) {
        prime = false;
        break;
      }
      divisor += 2;
    }
  }
  return prime;
}

}  // namespace openstudio

#endif  // UTILITIES_MATH_PRIMES_HPP
