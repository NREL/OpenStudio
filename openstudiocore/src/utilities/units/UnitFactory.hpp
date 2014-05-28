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

#ifndef UTILITIES_UNITS_UNITFACTORY_HPP
#define UTILITIES_UNITS_UNITFACTORY_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Singleton.hpp"
#include "Unit.hpp"

#include "../core/Logger.hpp"

#include <boost/functional.hpp>

#include <set>
#include <map>

namespace openstudio{

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
class UTILITIES_API UnitFactorySingleton {

  friend class Singleton<UnitFactorySingleton>;

 public:
  
  /** Units are created by functions that accept no arguments and return 
   *  something that can be converted into a Unit::Ptr. */
  typedef boost::function<Unit ()> CreateUnitCallback;

  // FACTORY MAINTENANCE 

  /** Associate a unit creation function with a standard string. Also registers
   *  the pretty string if one exists. Returns false if standardString or prettyString is 
   *  already used as a key in one or more internal maps. 
   *
   *  Option to specify a unit system is for conflict resolution. For instance, seconds
   *  is a base unit in SI and IP. For efficiency, only specify system if you expect a 
   *  conflict. */
  bool registerUnit(CreateUnitCallback createFn,UnitSystem system=UnitSystem::Mixed);

  /** Register alternative symbols for standardString. For instance, J is a pseudonym for 
   *  kg*m^2/s^2. Returns false if equivalentString is already used as a key in the internal map. */
  bool registerEquivalentString(const std::string& equivalentString, 
                                const std::string& standardString);

  // FACTORY USE

  /** Parses unitStr to create a unit based on the internal factory maps. The maps are populated
   *  using the functions registerUnit and registerEquivalentString. Specify system to alert the 
   *  factory of your preferred unit system. The returned pointer is not guaranteed to be from 
   *  that system, but if there is a conflict, it will be preferred. */
  boost::optional<Unit> createUnit(const std::string& unitString,
                                   UnitSystem system=UnitSystem::Mixed) const;

  /** Returns the prettyString registered for standardString. Returns the empty std::string for 
   *  unregistered standardStrings. */
  std::string lookupPrettyString(const std::string& standardString) const;
  
 private:
  REGISTER_LOGGER("openstudio.units.UnitFactory");
  UnitFactorySingleton();

  typedef std::map<std::string,boost::optional<Unit> > ResultCacheMap;

  mutable ResultCacheMap m_resultCacheMap;

  typedef std::map<std::string,CreateUnitCallback> StandardStringCallbackMap;
  typedef std::map<UnitSystem,StandardStringCallbackMap> CallbackMapMap;

  CallbackMapMap m_callbackMaps;

  typedef std::map<std::string,std::string> PrettyStringLookupMap;
  typedef std::map<std::string, std::vector<std::string> > StandardStringLookupMap;

  StandardStringLookupMap m_standardStringLookupMap; // look up alias, get back standardString
  PrettyStringLookupMap m_prettyStringLookupMap;   // look up standardString, get back prettyString

  // helper create function--does simple lookups
  boost::optional<Unit> createUnitSimple(const std::string& unitString,
                                         UnitSystem system=UnitSystem::Mixed) const;
};  

/** Typedef for accessing the UnitFactorySingleton as UnitFactory::instance().
 *  \relates UnitFactorySingleton */
typedef openstudio::Singleton<UnitFactorySingleton> UnitFactory;

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
UTILITIES_API std::string replaceUnitString(const std::string& text,
                                            const std::string& newUnitString);

/** Returns true if unitString can be turned into a Unit. \relates UnitFactorySingleton */
UTILITIES_API bool isUnitString(const std::string& unitString);

/** Returns a Unit of the type that corresponds to system. \relates UnitFactorySingleton */
UTILITIES_API Unit createDimensionlessUnit(UnitSystem system);

/** Creates a Unit corresponding to unitString (using UnitFactory). User may specify preferred
 *  UnitSystem, and can cast the result to the appropriate class, as desired.
 *  \relates UnitFactorySingleton */
UTILITIES_API boost::optional<Unit> createUnit(const std::string& unitString,UnitSystem system);

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

} // openstudio

#endif // UTILITIES_UNITS_UNITFACTORY_HPP
