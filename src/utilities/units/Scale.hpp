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

#ifndef UTILITIES_UNITS_SCALE_HPP
#define UTILITIES_UNITS_SCALE_HPP

#include "../UtilitiesAPI.hpp"

#include <functional>
#include <ostream>
#include <string>

namespace openstudio {

/** Structure for holding scale prefixes such as kilo and centi. Logical operators and
   *  some other related typedefs and functions are declared in Scale.hpp. Scale arithmetic is
   *  declared in ScaleFactory.hpp, since openstudio::ScaleFactory is the object that makes such
   *  operations possible by keeping track of what scales are available. */
struct UTILITIES_API Scale
{
  std::string abbr;  ///< abbreviation, such as "k", as in kN = kilo-Newtons.
  std::string name;  ///< full name, i.e. kilo, mega, etc.
  int exponent;      ///< base-10 exponent. For instance, 3 for kilo = 10^3.
  double value;      ///< full value of the scale. For instance, 1000 for kilo.
};

/** Function pointer to a constant scale object. Implemented as function
   *  pointer to ensure timely instantiation. */
using ScaleConstant = std::function<const Scale&()>;

/** Outputs all information about Scale s. Examples: \verbatim
      Scale s = {"T", "tera",12,1.0E12};
      std::cout << s;
      boost::ofstream f(path);
      f << s;
      LOG_FREE(Info,"channel",s); \endverbatim */
UTILITIES_API std::ostream& operator<<(std::ostream& os, const Scale& s);

/** Equality operator for Scales. Checks exponent and abbreviation. */
UTILITIES_API bool operator==(const Scale& firstScale, const Scale& secondScale);
/** Inequality operator for Scales. Checks exponent and abbreviation. */
UTILITIES_API bool operator!=(const Scale& firstScale, const Scale& secondScale);
/** Less than operator for Scales. Compares exponents. */
UTILITIES_API bool operator<(const Scale& firstScale, const Scale& secondScale);
/** Greater than operator for Scales. Compares exponents. */
UTILITIES_API bool operator>(const Scale& firstScale, const Scale& secondScale);
/** Less than or equal operator for Scales. Compares exponents. */
UTILITIES_API bool operator<=(const Scale& firstScale, const Scale& secondScale);
/** Greater than or equal operator for Scales. Compares exponents. */
UTILITIES_API bool operator>=(const Scale& firstScale, const Scale& secondScale);

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

/** Static constant defining SI prefix for 10^5. */
UTILITIES_API const Scale& hectokilo();

/** Static constant defining SI prefix for 10^4. */
UTILITIES_API const Scale& myria();

/** Static constant defining SI prefix for 10^3. */
UTILITIES_API const Scale& kilo();

/** Static constant defining SI prefix for 10^2. */
UTILITIES_API const Scale& hecto();

/** Static constant defining SI prefix for 10^1. */
UTILITIES_API const Scale& deka();

/** Static constant defining no scale (1.0). */
UTILITIES_API const Scale& one();

/** Static constant defining SI prefix for 10^{-1} */
UTILITIES_API const Scale& deci();

/** Static constant defining SI prefix for 10^{-2} */
UTILITIES_API const Scale& centi();

/** Static constant defining SI prefix for 10^{-3} */
UTILITIES_API const Scale& milli();

/** Static constant defining SI prefix for 10^{-4} */
UTILITIES_API const Scale& decimilli();

/** Static constant defining SI prefix for 10^{-5} */
UTILITIES_API const Scale& centimilli();

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

}  // namespace openstudio

#endif  // UTILITIES_UNITS_SCALE_HPP
