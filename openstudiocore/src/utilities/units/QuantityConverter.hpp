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
#ifndef UTILITIES_UNITS_QUANTITYCONVERTER_HPP
#define UTILITIES_UNITS_QUANTITYCONVERTER_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Singleton.hpp"
#include "../core/Logger.hpp"

#include "Unit.hpp"
#include <string>
#include <map>

class QDomElement;

namespace openstudio {

class Quantity;
class OSQuantityVector;

// JMT@20100902 - it's necessary to move the temperature conversion
//                rule enum into a class that is *not* %ignored by swig, if we want
//                it usable by swig
// ETH@20120918 - at some point, the TemperatureConversionRule was removed from the interface,
//                but it may need to be re-added when we start converting more temperature data.
struct TemperatureConversionRule
{
  /** Enum to allow user to communicate temperature conversion preferences. 
   * 
   *  In STND mode, convertToCelsiusUnits and convertToFahrenheit units assume that temperatures 
   *  like 273 K and 480 R are absolute temperatures in need of offset, but that quantities with 
   *  other exponents on K or R, or other base units mixed in, are relative temperature quantities 
   *  whose values can be transferred directly. 
   *
   *  In ABS mode, convertToCelsiusUnits and convertToFahrenheit units assume that all quantities
   *  were derived from absolute temperatures, and the offset should be applied accordingly, as in
   *  280 K^3 = (pow(280,1.0/3.0) - 273.15)^3 C^3. 
   *
   *  In REL mode, convertToCelsiusUnits and convertToFahrenheit units assume that all
   *   temperatures are relative, such that values can be transferred directly, i.e., 2 K = 2 C. 
    *
   *  When converting Celsius and Fahrenheit to other units, either ABS or REL mode is used, 
   *  depending on the value of q.isAbsolute(). 
   */
  enum domain { STND, ABS, REL };
};

/** A struct to store all the information necessary to convert from one base unit to
 *  another. All conversions will be handled by going through SI, so these ConversionFactors
 *  will be stored in either the toSI or fromSI maps.
 */
struct baseUnitConversionFactor {
  UnitSystem system;
  std::string originalUnit;
  std::string targetUnit;
  double factor;
  double offset;
};

/** Singleton for converting quantities to different \link UnitSystem unit systems \endlink or
 *  to targeted \link Unit units \endlink */
class UTILITIES_API QuantityConverterSingleton {

  friend class Singleton<QuantityConverterSingleton>;

 public:

  boost::optional<Quantity> convert(const Quantity &q, UnitSystem sys) const;

  boost::optional<Quantity> convert(const Quantity &original, const Unit& targetUnits) const;

 private:
  REGISTER_LOGGER("openstudio.units.QuantityConverter");
  QuantityConverterSingleton();

  typedef std::map<std::string, baseUnitConversionFactor> BaseUnitConversionMap;
  typedef std::multimap<UnitSystem, baseUnitConversionFactor> UnitSystemConversionMultiMap;

  BaseUnitConversionMap m_toSImap;
  UnitSystemConversionMultiMap m_fromSIBySystemMap;

  boost::optional<Quantity> m_convertToSI(const Quantity& original) const;

  Quantity m_convertFromSI(const Quantity& original, const UnitSystem targetSys) const;

  boost::optional<Quantity> m_convertToTargetFromSI(const Quantity& original,
                                                    const Unit& targetUnits) const;

};

/** \relates QuantityConverterSingleton */
typedef openstudio::Singleton<QuantityConverterSingleton> QuantityConverter;

/** Non-member function to simplify interface for users. \relates QuantityConverterSingleton */
UTILITIES_API boost::optional<double> convert(double original, const std::string& originalUnits, const std::string& finalUnits);

/** Non-member function to simplify interface for users. \relates QuantityConverterSingleton */
UTILITIES_API boost::optional<Quantity> convert(const Quantity& original, UnitSystem sys);

/** Non-member function that uses just two calls to QuantityConverter to convert an entire 
 *  OSQuantityVector. \relates QuantityConverterSingleton \relates OSQuantityVector */
UTILITIES_API OSQuantityVector convert(const OSQuantityVector& original, UnitSystem sys);

/** Non-member function to simplify interface for users. \relates QuantityConverterSingleton */
UTILITIES_API boost::optional<Quantity> convert(const Quantity& original, const Unit& targetUnits);

/** Non-member function that uses just two calls to QuantityConverter to convert an entire 
 *  OSQuantityVector. \relates QuantityConverterSingleton \relates OSQuantityVector */
UTILITIES_API OSQuantityVector convert(const OSQuantityVector& original, const Unit& targetUnits);

}// namespace openstudio

#endif // UTILITIES_UNITS_QUANTITYCONVERTER_HPP

