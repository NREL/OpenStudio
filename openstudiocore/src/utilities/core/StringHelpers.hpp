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

#ifndef UTILITIES_CORE_STRINGHELPERS_HPP
#define UTILITIES_CORE_STRINGHELPERS_HPP

#include <utilities/UtilitiesAPI.hpp>

#include <string>

namespace openstudio {

  /** Replace all non-letters and digits with spaces, and then remove spaces by to make camel 
   *  case. */
  UTILITIES_API std::string toCamelCase(const std::string& s);

  /** Make camel case then upcase the first letter. */
  UTILITIES_API std::string toUpperCamelCase(const std::string& s);

  /** Make camel case then downcase the first letter. */
  UTILITIES_API std::string toLowerCamelCase(const std::string& s);

  /** Make underscore case. */
  UTILITIES_API std::string toUnderscoreCase(const std::string& s);

  /** Convert IDD object and field names to something reasonable for computers. */
  UTILITIES_API std::string convertIddName(const std::string& s);

  /** Convert IDD object name to default name for IDF objects of this type. */
  UTILITIES_API std::string iddObjectNameToIdfObjectName(const std::string& s);

  /** Prints value with numFractionalDigits, inserting commas to separate into 
   *  thousands, millions, etc. if desired. */
  UTILITIES_API std::string toNeatString(double value, 
                                         unsigned numFractionalDigits = 0, 
                                         bool applyCommas = true);

  /** Prints value in numSigFigs, inserting commas to separate into thousands, 
  millions, etc. if desired. */
  UTILITIES_API std::string toNeatStringBySigFigs(double value,
                                                  unsigned numSigFigs = 3,
                                                  bool applyCommas = true);

  /** Returns the number of digits past the decimal point in str. */
  UTILITIES_API unsigned numFractionalDigits(const std::string& str);

  /** Returns the number of significant digits past the decimal point in value, given numSigFigs. 
   *  Throws if numSigFigs == 0. */
  UTILITIES_API unsigned numFractionalDigits(double value,unsigned numSigFigs);

  /** Returns the minimum and maximum number of significant digits past the decimal 
   *  point in values, given numSigFigs. Throws if numSigFigs == 0. */
  UTILITIES_API std::pair<unsigned,unsigned> numFractionalDigits(const std::vector<double>& values,
                                                                 unsigned numSigFigs);

  /** Rounds value to a number of significant figures (rather than to a particular
   *  decimal place). Throws if numSigFigs == 0. */
  UTILITIES_API double toNumSigFigs(double value, unsigned numSigFigs);

}

#endif // UTILITIES_CORE_STRINGHELPERS_HPP
