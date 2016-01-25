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

#ifndef UTILITIES_MATH_FLOATCOMPARE_HPP
#define UTILITIES_MATH_FLOATCOMPARE_HPP

#include <limits>
#include <cmath>

namespace openstudio {

template<class T>
bool equal(T val1,T val2,T tol=std::numeric_limits<T>::epsilon()) {
  T diff = val1 - val2;
  if (fabs(diff) < tol) { return true; }
  return fabs(diff) <= ((fabs(val1) < fabs(val2) ? fabs(val2) : fabs(val1) )*tol);
}

template<class T>
bool lessThanOrEqual(T val1,T val2,T tol=std::numeric_limits<T>::epsilon()) {
  return (val1 < val2) || equal(val1,val2,tol); 
}

template<class T>
bool greaterThanOrEqual(T val1,T val2,T tol=std::numeric_limits<T>::epsilon()) {
  return (val1 > val2) || equal(val1,val2,tol);
}

template<class T>
double relativeError(T expected,T actual,T tol = std::numeric_limits<T>::epsilon()) {
  if (fabs(expected) <= tol) { return fabs(expected - actual); }
  return (fabs(expected - actual)/fabs(expected));
}

} // openstudio

#endif // UTILITIES_MATH_FLOATCOMPARE_HPP
