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

#ifndef UTILITIES_UNITS_UNITFACTORY_HPP
#define UTILITIES_UNITS_UNITFACTORY_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Singleton.hpp"
#include "Unit.hpp"

#include "../core/Logger.hpp"

#include <boost/functional.hpp>

#include <set>
#include <map>

namespace openstudio {

/** Singleton that creates units based on std::string representation.
 *  Access using UnitFactory::instance(). The class keeps up with standard
 *  unit strings, pseudonyms for those strings, and pretty strings.
 *
 *  Standard unit strings use base units like kg, m, s, ft, etc. and follow
 *  LaTeX formatting. For example, the standard unit std::string for SI energy is
 *  <pre>kg*m^2/s^2</pre>.
 *
 *  Also provides a function for looking up pretty units without having to
 *  create a unit. */
class UTILITIES_API UnitFactorySingleton
{

  friend class Singleton<UnitFactorySingleton>;

 public:
  /** Units are created by functions that accept no arguments and return
   *  something that can be converted into a Unit::Ptr. */
  using CreateUnitCallback = boost::function<Unit()>;

  // FACTORY MAINTENANCE

  /** Associate a unit creation function with a standard string. Also registers
   *  the pretty string if one exists. Returns false if standardString or prettyString is
   *  already used as a key in one or more internal maps.
   *
   *  Option to specify a unit system is for conflict resolution. For instance, seconds
   *  is a base unit in SI and IP. For efficiency, only specify system if you expect a
   *  conflict. */
  bool registerUnit(CreateUnitCallback createFn, UnitSystem system = UnitSystem::Mixed);

  /** Register alternative symbols for standardString. For instance, J is a pseudonym for
   *  kg*m^2/s^2. Returns false if equivalentString is already used as a key in the internal map. */
  bool registerEquivalentString(const std::string& equivalentString, const std::string& standardString);

  // FACTORY USE

  /** Parses unitStr to create a unit based on the internal factory maps. The maps are populated
   *  using the functions registerUnit and registerEquivalentString. Specify system to alert the
   *  factory of your preferred unit system. The returned pointer is not guaranteed to be from
   *  that system, but if there is a conflict, it will be preferred. */
  boost::optional<Unit> createUnit(const std::string& unitString, UnitSystem system = UnitSystem::Mixed) const;

  /** Returns the prettyString registered for standardString. Returns the empty std::string for
   *  unregistered standardStrings. */
  std::string lookupPrettyString(const std::string& standardString) const;

 private:
  REGISTER_LOGGER("openstudio.units.UnitFactory");
  UnitFactorySingleton();

  using ResultCacheMap = std::map<std::string, boost::optional<Unit>>;

  mutable ResultCacheMap m_resultCacheMap;

  using StandardStringCallbackMap = std::map<std::string, CreateUnitCallback>;
  using CallbackMapMap = std::map<UnitSystem, StandardStringCallbackMap>;

  CallbackMapMap m_callbackMaps;

  using PrettyStringLookupMap = std::map<std::string, std::string>;
  using StandardStringLookupMap = std::map<std::string, std::vector<std::string>>;

  StandardStringLookupMap m_standardStringLookupMap;  // look up alias, get back standardString
  PrettyStringLookupMap m_prettyStringLookupMap;      // look up standardString, get back prettyString

  // helper create function--does simple lookups
  boost::optional<Unit> createUnitSimple(const std::string& unitString, UnitSystem system = UnitSystem::Mixed) const;
};

/** Typedef for accessing the UnitFactorySingleton as UnitFactory::instance().
 *  \relates UnitFactorySingleton */
using UnitFactory = openstudio::Singleton<UnitFactorySingleton>;

/** Returns the best match UnitSystem for unitString, in priority order SI, IP, ..., Mixed
 *  \relates UnitFactorySingleton */
UTILITIES_API UnitSystem getSystem(const std::string& unitString);

/** Returns true if unitString can be created as a unit in system. \relates UnitFactorySingleton */
UTILITIES_API bool isInSystem(const std::string& unitString, UnitSystem system);

/** Returns true if any of unitString's baseUnits are registered in the factory.
 *  \relates UnitFactorySingleton */
UTILITIES_API bool containsRegisteredBaseUnit(const std::string& unitString);

/** Extracts a unitString from text, if possible. Only returns strings that contain at least one
 *  registered base unit. \relates UnitFactorySingleton */
UTILITIES_API std::string extractUnitString(const std::string& text);

/** Converts unitString to standard form. Right now, does conversion like people/100 m^2 -->
 *  c(people/m^2). \relates UnitFactorySingleton */
UTILITIES_API std::string convertToStandardForm(const std::string& unitString);

/** Replaces unitString in text, using same finder algorithm as extractUnitString.
 *  \relates UnitFactorySingleton */
UTILITIES_API std::string replaceUnitString(const std::string& text, const std::string& newUnitString);

/** Returns true if unitString can be turned into a Unit. \relates UnitFactorySingleton */
UTILITIES_API bool isUnitString(const std::string& unitString);

/** Returns a Unit of the type that corresponds to system. \relates UnitFactorySingleton */
UTILITIES_API Unit createDimensionlessUnit(UnitSystem system);

/** Creates a Unit corresponding to unitString (using UnitFactory). User may specify preferred
 *  UnitSystem, and can cast the result to the appropriate class, as desired.
 *  \relates UnitFactorySingleton */
UTILITIES_API boost::optional<Unit> createUnit(const std::string& unitString, UnitSystem system);

/** Creates a Unit corresponding to unitString (using UnitFactory). Attempts to find the best
 *  UnitSystem for unitString, in priority order SI, IP, ..., Mixed. \relates UnitFactorySingleton */
UTILITIES_API boost::optional<Unit> createUnit(const std::string& unitString);

/** Returns true if uStr1 and uStr2 evaluate to the same unit. \relates UnitFactorySingleton */
UTILITIES_API bool unitStringsEqual(const std::string& uStr1, const std::string& uStr2);

/** In IP units, power density is often expressed as W/ft^2, which is actually a
 *  mixed unit. \relates UnitFactorySingleton */
UTILITIES_API Unit createIPPowerDensity();

/** Make sure crgal^3/min shows up as gal/min. \relates UnitFactorySingleton */
UTILITIES_API Unit createGPMVolumetricFlowrate();

/** In IP units, pressure is often expressed as lb_f/in^2, which is actually a mixed
 *  unit. \relates UnitFactorySingleton */
UTILITIES_API Unit createIPPressure();

}  // namespace openstudio

#endif  // UTILITIES_UNITS_UNITFACTORY_HPP
