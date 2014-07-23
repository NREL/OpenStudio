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

#ifndef UTILITIES_UNITS_SCALE_HPP
#define UTILITIES_UNITS_SCALE_HPP

#include "../UtilitiesAPI.hpp"

#include <functional>
#include <ostream>
#include <string>

namespace openstudio{

  /** Structure for holding scale prefixes such as kilo and centi. Logical operators and 
   *  some other related typedefs and functions are declared in Scale.hpp. Scale arithmetic is 
   *  declared in ScaleFactory.hpp, since openstudio::ScaleFactory is the object that makes such 
   *  operations possible by keeping track of what scales are available. */
  struct UTILITIES_API Scale {
    std::string abbr; ///< abbreviation, such as "k", as in kN = kilo-Newtons.
    std::string name; ///< full name, i.e. kilo, mega, etc.
    int exponent;     ///< base-10 exponent. For instance, 3 for kilo = 10^3.
    double value;     ///< full value of the scale. For instance, 1000 for kilo.
  };

  /** Function pointer to a constant scale object. Implemented as function 
   *  pointer to ensure timely instantiation. */
  typedef std::function<const Scale& ()> ScaleConstant;

  /** Outputs all information about Scale s. Examples: \verbatim 
      Scale s = {"T", "tera",12,1.0E12};
      std::cout << s; 
      boost::ofstream f(path); 
      f << s; 
      LOG_FREE(Info,"channel",s); \endverbatim */
  UTILITIES_API std::ostream& operator<<(std::ostream& os,const Scale& s);

  /** Equality operator for Scales. Checks exponent and abbreviation. */
  UTILITIES_API bool operator==(const Scale& firstScale,const Scale& secondScale);
  /** Inequality operator for Scales. Checks exponent and abbreviation. */
  UTILITIES_API bool operator!=(const Scale& firstScale,const Scale& secondScale);
  /** Less than operator for Scales. Compares exponents. */
  UTILITIES_API bool operator<(const Scale& firstScale,const Scale& secondScale);
  /** Greater than operator for Scales. Compares exponents. */
  UTILITIES_API bool operator>(const Scale& firstScale,const Scale& secondScale);
  /** Less than or equal operator for Scales. Compares exponents. */
  UTILITIES_API bool operator<=(const Scale& firstScale,const Scale& secondScale);
  /** Greater than or equal operator for Scales. Compares exponents. */
  UTILITIES_API bool operator>=(const Scale& firstScale,const Scale& secondScale);

  /** Static constant indicating that the scale was not found. */
  UTILITIES_API const Scale& notDefined();

  /** Static constant defining SI prefix for 10^24. */
  UTILITIES_API const Scale& yotta();

  /** Static constant defining SI prefix for 10^21. */
  UTILITIES_API const Scale& zetta();

  /** Static constant defining SI prefix for 10^18. */
  UTILITIES_API const Scale& exa();

  /** Static constant defining SI prefix for 10^15. */
  UTILITIES_API const Scale& peta();

  /** Static constant defining SI prefix for 10^12. */
  UTILITIES_API const Scale& tera();

  /** Static constant defining SI prefix for 10^9. */
  UTILITIES_API const Scale& giga();

  /** Static constant defining SI prefix for 10^6. */
  UTILITIES_API const Scale& mega();

  /** Static constant defining SI prefix for 10^3. */
  UTILITIES_API const Scale& kilo();

  /** Static constant defining no scale (1.0). */
  UTILITIES_API const Scale& one();

  /** Static constant defining SI prefix for 10^{-2} */
  UTILITIES_API const Scale& centi();

  /** Static constant defining SI prefix for 10^{-3} */
  UTILITIES_API const Scale& milli();

  /** Static constant defining SI prefix for 10^{-6} */
  UTILITIES_API const Scale& micro();

  /** Static constant defining SI prefix for 10^{-9} */
  UTILITIES_API const Scale& nano();

  /** Static constant defining SI prefix for 10^{-12} */
  UTILITIES_API const Scale& pico();

  /** Static constant defining SI prefix for 10^{-15} */
  UTILITIES_API const Scale& femto();

  /** Static constant defining SI prefix for 10^{-18) */
  UTILITIES_API const Scale& atto();

  /** Static constant defining SI prefix for 10^{-21} */
  UTILITIES_API const Scale& zepto();

  /** Static constant defining SI prefix for 10^{-24} */
  UTILITIES_API const Scale& yocto();

} // openstudio

#endif // UTILITIES_UNITS_SCALE_HPP
