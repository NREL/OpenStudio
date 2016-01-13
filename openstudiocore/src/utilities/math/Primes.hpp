/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef UTILITIES_MATH_PRIMES_HPP
#define UTILITIES_MATH_PRIMES_HPP

#include <cmath>

namespace openstudio {

template<class T>
bool isPrime(T val) {
  bool prime = true;

  if (val < 2) {
    return false;
  }
  else if (val == 2) {
    return true;
  }
  else if (val % 2 == 0) {
    return false;
  }
  else {
    T divisor(3);
    double dval = static_cast<double>(val);
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

}

#endif // UTILITIES_MATH_PRIMES_HPP
