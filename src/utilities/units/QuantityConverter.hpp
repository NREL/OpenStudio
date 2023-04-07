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

#ifndef UTILITIES_UNITS_QUANTITYCONVERTER_HPP
#define UTILITIES_UNITS_QUANTITYCONVERTER_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Singleton.hpp"
#include "../core/Logger.hpp"

#include "Unit.hpp"
#include <string>
#include <map>

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
  enum domain
  {
    STND,
    ABS,
    REL
  };
};

/** A struct to store all the information necessary to convert from one base unit to
 *  another. All conversions will be handled by going through SI, so these ConversionFactors
 *  will be stored in either the toSI or fromSI maps.
 */
struct baseUnitConversionFactor
{
  UnitSystem system;
  std::string originalUnit;
  std::string targetUnit;
  double factor;
  double offset;
};

/** Singleton for converting quantities to different \link UnitSystem unit systems \endlink or
 *  to targeted \link Unit units \endlink */
class UTILITIES_API QuantityConverterSingleton
{

  friend class Singleton<QuantityConverterSingleton>;

 public:
  boost::optional<Quantity> convert(const Quantity& q, UnitSystem sys) const;

  boost::optional<Quantity> convert(const Quantity& original, const Unit& targetUnits) const;

 private:
  REGISTER_LOGGER("openstudio.units.QuantityConverter");
  QuantityConverterSingleton();

  using BaseUnitConversionMap = std::map<std::string, baseUnitConversionFactor>;
  using UnitSystemConversionMultiMap = std::multimap<UnitSystem, baseUnitConversionFactor>;

  BaseUnitConversionMap m_toSImap;
  UnitSystemConversionMultiMap m_fromSIBySystemMap;

  boost::optional<Quantity> m_convertToSI(const Quantity& original) const;

  Quantity m_convertFromSI(const Quantity& original, const UnitSystem& targetSys) const;

  boost::optional<Quantity> m_convertToTargetFromSI(const Quantity& original, const Unit& targetUnits) const;
};

/** \relates QuantityConverterSingleton */
using QuantityConverter = openstudio::Singleton<QuantityConverterSingleton>;

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

}  // namespace openstudio

#endif  // UTILITIES_UNITS_QUANTITYCONVERTER_HPP
