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

#ifndef UTILITIES_UNITS_SCALEFACTORY_HPP
#define UTILITIES_UNITS_SCALEFACTORY_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Singleton.hpp"
#include "../core/Logger.hpp"

#include "Scale.hpp"

#include <ostream>
#include <set>
#include <map>

namespace openstudio{

/** Singleton that creates scales based on std::string or exponent. 
 *  Access using ScaleFactory::instance().  */
class UTILITIES_API ScaleFactorySingleton {

  friend class Singleton<ScaleFactorySingleton>;

 public:
  
  /// Destructor.
  ~ScaleFactorySingleton();

  /** Store a scale callback function for future lookup using the scale exponent or 
   *  abbreviation. By default, SI scales are registered during factory construction. */
  bool registerScale(ScaleConstant scale);

  /** Returns a vector of Scale objects, one for each scale registered in the factory. */
  std::vector<Scale> registeredScales();

  /** Outputs the scales registered in the factory. */
  friend UTILITIES_API std::ostream& operator<<(std::ostream& os,const ScaleFactorySingleton& factory);

  /** Creates a scale from an exponent. Throws if the factory does not have any scales registered. 
   *  Returns ScaleConstant with ().value == 0.0 if exponent not in factory. */
  ScaleConstant createScale(int exponent) const;

  /** Creates a scale from an abbreviation. Throws if the factory does not have any scales registered. 
   *  Returns ScaleConstant with ().value == 0.0 if abbr not in factory. */
  ScaleConstant createScale(const std::string& abbr) const;
  
 private:  
  REGISTER_LOGGER("openstudio.units.ScaleFactory");
  /// Private constructor initializes ScaleFactory with standard SI scales.
  ScaleFactorySingleton();

  typedef std::map<int,ScaleConstant> ExponentLookupMap;
  typedef std::map<std::string,ScaleConstant> AbbreviationLookupMap;
  ExponentLookupMap m_exponentMap;
  AbbreviationLookupMap m_abbreviationMap;

  ScaleConstant m_recoverFromFailedCreate() const;
};  

typedef openstudio::Singleton<ScaleFactorySingleton> ScaleFactory;

/** Prints scales that are registered in the factory (and thus available for use in 
 *  Units and Quantities). */
UTILITIES_API std::ostream& operator<<(std::ostream& os,const ScaleFactorySingleton& factory);

/** Wrapper around << for SWIG bindings. Prints each scale registered in ScaleFactory, 
 *  and thus available for use in Units, Quantities, and bare Scale operations. */
UTILITIES_API std::string printScales();

/** If possible, extracts scale abbreviation from the front of str, returning abbreviation in .first 
 *  and remainder of string in .second. Otherwise, returns empty strings. */
UTILITIES_API std::pair<std::string,std::string> extractScaleAbbreviation(const std::string& str);

/** Return type for scale multiplication and division. See the corresponding operators
 *  for its use. */
typedef std::pair<ScaleConstant,double> ScaleOpReturnType;

/** Multiplication of scales. If result implied by adding exponents is not 
 *  available, .second is amount multiplication of values should be multiplied 
 *  by so result will be correct when using .first. */
UTILITIES_API ScaleOpReturnType operator*(const Scale& firstScale,const Scale& secondScale);

/** Division of scales. If result implied by subtracting exponents is not 
 *  available, .second is amount final value should be multiplied by so result 
 *  will be correct when using .first. */
UTILITIES_API ScaleOpReturnType operator/(const Scale& firstScale,const Scale& secondScale);

/** Raise scale to an integer power. If result implied by the operation is not 
 *  available, .second is amount final value should be multiplied by so result 
 *  will be correct when using .first. */
UTILITIES_API ScaleOpReturnType pow(const Scale& scale,int expNum,int expDenom=1);

/** Returns scale with desiredExponent, if possible. Otherwise backtracks to nearby 
 *  scale, and returns that scale along with a factor that the value should be 
 *  multiplied by to make the end result consistent with the returned scale. */
UTILITIES_API ScaleOpReturnType scaleOpHelperFunction(int desiredExponent);

} // openstudio

#endif // UTILITIES_UNITS_SCALEFACTORY_HPP
