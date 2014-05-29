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

#include "UnitFactory.hpp"

#include "ScaleFactory.hpp"
#include "QuantityRegex.hpp"
#include "SIUnit.hpp"
#include "SIUnit_Impl.hpp"
#include "IPUnit.hpp"
#include "IPUnit_Impl.hpp"
#include "BTUUnit.hpp"
#include "BTUUnit_Impl.hpp"
#include "CFMUnit.hpp"
#include "CFMUnit_Impl.hpp"
#include "GPDUnit.hpp"
#include "GPDUnit_Impl.hpp"
#include "MPHUnit.hpp"
#include "MPHUnit_Impl.hpp"
#include "WhUnit.hpp"
#include "WhUnit_Impl.hpp"
#include "ThermUnit.hpp"
#include "ThermUnit_Impl.hpp"
#include "Misc1Unit.hpp"
#include "Misc1Unit_Impl.hpp"
#include "CelsiusUnit.hpp"
#include "CelsiusUnit_Impl.hpp"
#include "FahrenheitUnit.hpp"
#include "FahrenheitUnit_Impl.hpp"

#include "../core/Exception.hpp"
#include "../core/Containers.hpp"
#include "../core/Assert.hpp"

#include <boost/pointer_cast.hpp>

#include <map>
#include <vector>

namespace openstudio{

bool UnitFactorySingleton::registerUnit(CreateUnitCallback createFn,UnitSystem system) {

  Unit thisUnit = createFn();
  std::string standardString = thisUnit.standardString(false);
  std::string prettyString = thisUnit.prettyString(false);

  StandardStringCallbackMap::value_type callbackMapEntry(standardString,createFn);

  // register the callback in main map
  bool result = m_callbackMaps[UnitSystem(UnitSystem::Mixed)].insert(callbackMapEntry).second;
  // already registered--recover if possible
  if (!result) {
    std::stringstream message;
    message << "A callback function for " << standardString << " is already registered"
            << " and there is not enough information to resolve the conflict. "
            << "Note that C and F are assumed to not conflict--all other units use K and R.";
    // mask registry in main map, save in case need to reconstitute
    m_callbackMaps[UnitSystem(UnitSystem::Mixed)][standardString] = 0;

    // make sure string registered elsewhere
    bool found(false);
    for (const auto & callbackMap : m_callbackMaps)
    {
      if ((callbackMap.first == UnitSystem::Mixed) || (callbackMap.first == system)) {
        continue;
      }
      auto lookupPair = callbackMap.second.find(standardString);
      if (lookupPair != callbackMap.second.end()) {
        found = true;
        break;
      }
    }

    if (!found) {
      LOG_AND_THROW(message.str());
    }

  } // if already registered in m_callbackMap

  // register the callback in system-specific map
  result = m_callbackMaps[system].insert(callbackMapEntry).second;
  if (!result && (system != UnitSystem::Mixed)) {
    return result;
  }

  // register standardString as a pseudonym for itself
  registerEquivalentString(standardString,standardString);

  // register prettyString if applicable
  if (prettyString != "") {

    // get prettyString from standardString
    m_prettyStringLookupMap.insert(PrettyStringLookupMap::value_type(standardString,prettyString));

    // get standardString from prettyString
    registerEquivalentString(prettyString,standardString);

    // be able to print things like 1/J
    Unit clonedUnit = thisUnit.clone();
    clonedUnit.setPrettyString("");
    std::string tempPrettyString = boost::regex_replace(prettyString,
                                                        boost::regex("person"),
                                                        "people");
    Unit prettyUnit = parseUnitString(tempPrettyString);
    clonedUnit.pow(-1,1,false); std::string clonedUnitString = clonedUnit.standardString(false);
    prettyUnit.pow(-1,1,false); std::string prettyUnitString = prettyUnit.standardString(false);
    m_prettyStringLookupMap.insert(
          PrettyStringLookupMap::value_type(clonedUnitString,prettyUnitString));
  }

  return result;
}

bool UnitFactorySingleton::registerEquivalentString(const std::string& equivalentString,
                                                    const std::string& standardString) {
  // register the pair
  bool result = m_standardStringLookupMap.insert(
      StandardStringLookupMap::value_type(equivalentString,StringVector(1u,standardString))).second;

  if (!result) {
    // equivalentString is already a key. add this one
    m_standardStringLookupMap[equivalentString].push_back(standardString);
  }

  return result;
}

boost::optional<Unit> UnitFactorySingleton::createUnit(const std::string& unitString,
                                                       UnitSystem system) const
{
  if (m_callbackMaps.size() == 0) {
    LOG(Warn,"UnitFactorySingleton::createUnit called, but the maps appear to be empty.");
  }

  std::string resultCacheKey = unitString + " in unit system " + system.valueName();
  ResultCacheMap::const_iterator findIt = m_resultCacheMap.find(resultCacheKey);
  if (findIt != m_resultCacheMap.end()){
    return findIt->second;
  }

  if (!unitString.empty() && !isUnit(unitString)) {
    LOG(Error,unitString << " is not properly formatted.");
    m_resultCacheMap[resultCacheKey] = boost::none;
    return boost::none;
  }

  OptionalUnit result = createUnitSimple(unitString,system);
  if (result) {
    m_resultCacheMap[resultCacheKey] = result;
    return *result;
  }

  // no luck--start parsing
  std::string wUnitString(unitString);
  ScaleConstant scale = ScaleFactory::instance().createScale(0);

  if (isScaledUnit(wUnitString)) {
    std::pair<std::string,std::string> scaleAndUnit = decomposeScaledUnitString(wUnitString);
    scale = ScaleFactory::instance().createScale(scaleAndUnit.first);
    if (scale().value == 0.0) {
      LOG(Error,"Scaled unit string " << wUnitString << " uses invalid scale abbreviation "
          << scaleAndUnit.first << ".");
      m_resultCacheMap[resultCacheKey] = boost::none;
      return boost::none;
    }
    wUnitString = scaleAndUnit.second;
  }

  // wUnitString should now be compound unit
  std::pair< std::vector<std::string>,std::vector<std::string> > atomicUnits =
      decomposeCompoundUnitString(wUnitString);
  // loop through numerator
  std::vector<std::string>::const_iterator atomicUnitIter;
  std::vector<std::string>::const_iterator vectorEnd = atomicUnits.first.end();
  std::pair<std::string,int> atomicUnit;
  for (atomicUnitIter = atomicUnits.first.begin(); atomicUnitIter != vectorEnd; ++atomicUnitIter) {
    // decompose into baseUnit and exponent
    atomicUnit = decomposeAtomicUnitString(*atomicUnitIter);
    // look for baseUnit
    OptionalUnit baseUnit = createUnitSimple(atomicUnit.first,system);
    if (!baseUnit) {
      // decompose into scale, baseUnit
      std::pair<std::string,std::string> scaleAndBaseUnit = extractScaleAbbreviation(atomicUnit.first);
      if (!scaleAndBaseUnit.first.empty()) {
        baseUnit = createUnitSimple(scaleAndBaseUnit.second,system);
        if (!baseUnit) {
          baseUnit = Unit();
          baseUnit->setBaseUnitExponent(scaleAndBaseUnit.second,1);
        }
        baseUnit->setScale(scaleAndBaseUnit.first);
      }
      else {
        baseUnit = Unit();
        baseUnit->setBaseUnitExponent(atomicUnit.first,1);
      }
    }
    baseUnit->pow(atomicUnit.second);
    if (!result) {
      result = baseUnit;
    }
    else {
      result = (*result) * (*baseUnit);
    }
  }
  // loop through denominator
  vectorEnd = atomicUnits.second.end();
  for (atomicUnitIter = atomicUnits.second.begin(); atomicUnitIter != vectorEnd; ++atomicUnitIter) {
    // decompose into baseUnit and exponent
    atomicUnit = decomposeAtomicUnitString(*atomicUnitIter);
    // look for baseUnit
    OptionalUnit baseUnit = createUnitSimple(atomicUnit.first,system);
    if (!baseUnit) {
      // decompose into scale, baseUnit
      std::pair<std::string,std::string> scaleAndBaseUnit = extractScaleAbbreviation(atomicUnit.first);
      if (!scaleAndBaseUnit.first.empty()) {
        baseUnit = createUnitSimple(scaleAndBaseUnit.second,system);
        if (!baseUnit) {
          LOG(Info,scaleAndBaseUnit.second << " is not a registered baseUnit (in the selected system). "
              << "Returning it as-is in a mixed Unit (not SI, IP, etc.).");
          baseUnit = Unit();
          baseUnit->setBaseUnitExponent(scaleAndBaseUnit.second,1);
        }
        baseUnit->setScale(scaleAndBaseUnit.first);
      }
      else {
        LOG(Info,scaleAndBaseUnit.second << " is not a registered baseUnit (in the selected system). "
              << "Returning it as-is in a mixed Unit (not SI, IP, etc.).");
        baseUnit = Unit();
        baseUnit->setBaseUnitExponent(atomicUnit.first,1);
      }
    }
    baseUnit->pow(atomicUnit.second);
    if (!result) {
      baseUnit->pow(-1);
      result = baseUnit;
    }
    else {
      result = (*result) / (*baseUnit);
    }
  }

  OS_ASSERT(result);

  // impose overall scale
  if (scale().exponent != 0) {
    ScaleOpReturnType resultScale = scale()*result->scale();
    result->setScale(resultScale.first().exponent);
  }

  m_resultCacheMap[resultCacheKey] = result;
  return result;
}

boost::optional<Unit> UnitFactorySingleton::createUnitSimple(const std::string& unitString,
                                                             UnitSystem system) const
{
  if (unitString.empty()) {
    Unit result = createDimensionlessUnit(system);
    if (OptionalTemperatureUnit T = result.optionalCast<TemperatureUnit>()) {
      T->setAsRelative();
    }
    return result;
  }

  OptionalUnit candidate;
  StandardStringLookupMap::const_iterator lookupPair;
  auto standardStringMapEnd = m_standardStringLookupMap.end();
  lookupPair = m_standardStringLookupMap.find(unitString);
  if (lookupPair != standardStringMapEnd) {

    // unitString is registered
    for (const std::string& standardString : lookupPair->second) {

      // instantiate the standardString
      CallbackMapMap::const_iterator callbackMap;
      StandardStringCallbackMap::const_iterator callbackPair;
      OptionalUnit temp;
      // try base map
      callbackMap = m_callbackMaps.find(UnitSystem(UnitSystem::Mixed));
      OS_ASSERT(callbackMap != m_callbackMaps.end());
      callbackPair = callbackMap->second.find(standardString);
      if ((callbackPair != callbackMap->second.end()) && (callbackPair->second != NULL)) {
        temp = callbackPair->second();
      }
      else {
        // try system map
        callbackMap = m_callbackMaps.find(system);
        if (callbackMap != m_callbackMaps.end()) {
          callbackPair = callbackMap->second.find(standardString);
          if ((callbackPair != callbackMap->second.end()) && (callbackPair->second != NULL)) {
            temp = callbackPair->second();
          }
        }

        // try all other maps
        if (!temp) {
          for (int sysValue : UnitSystem::getValues()) {
            UnitSystem tempSystem(sysValue);
            if ((tempSystem == UnitSystem::Mixed) || (tempSystem == system)) {
              continue;
            }
            callbackMap = m_callbackMaps.find(tempSystem);
            if (callbackMap != m_callbackMaps.end()) {
              callbackPair = callbackMap->second.find(standardString);
              if ((callbackPair != callbackMap->second.end()) && (callbackPair->second != NULL)) {
                temp = callbackPair->second();
                break;
              }
            }
          }
        }
      }

      OS_ASSERT(temp);

      // decide whether to keep temp
      if (!candidate || (temp->system() == system)) {
        candidate = temp;
      }

      // decide whether to keep looking
      if ((candidate->system() == system) || (system == UnitSystem::Mixed)) {
        break;
      }

    } // foreach

  } // if

  return candidate;
}

std::string UnitFactorySingleton::lookupPrettyString(const std::string& standardString) const {

  PrettyStringLookupMap::const_iterator lookupPair;
  lookupPair = m_prettyStringLookupMap.find(standardString);
  if (lookupPair == m_prettyStringLookupMap.end()) {
    return "";
  }
  else {
    return lookupPair->second;
  }

}

UnitFactorySingleton::UnitFactorySingleton() {

  // Celsius Base Units ========================================================
  registerUnit(createCelsiusTemperature);


  // Fahrenheit Base Units =====================================================
  registerUnit(createFahrenheitTemperature);


  // SI Base Units =============================================================
  registerUnit(createSIMass);
  registerUnit(createSILength,UnitSystem::SI);
  registerUnit(createSITime,UnitSystem::SI);
  registerUnit(createSITemperature,UnitSystem::SI);
  registerUnit(createSIElectricCurrent,UnitSystem::SI);
  registerUnit(createSILuminousIntensity,UnitSystem::SI);
  registerUnit(createSIAmountOfSubstance,UnitSystem::SI);
  registerUnit(createSIAngle,UnitSystem::SI);
  registerUnit(createSISolidAngle,UnitSystem::SI);
  registerUnit(createSIPeople,UnitSystem::SI);
  registerUnit(createSICycle,UnitSystem::SI);

  // SI Derived Units ==========================================================
  registerUnit(createSIForce);
  registerUnit(createSIEnergy);
  registerUnit(createSIPower);
  registerUnit(createSIElectricCharge,UnitSystem::SI);
  registerUnit(createSIElectricalPotential,UnitSystem::SI);
  registerUnit(createSIElectricCapacitance);
  registerUnit(createSIElectricResistance);
  registerUnit(createSIMagneticFlux);
  registerUnit(createSIMagneticFieldStrength);
  registerUnit(createSIInductance);
  registerUnit(createSILuminousFlux,UnitSystem::SI);
  registerUnit(createSIIlluminance,UnitSystem::SI);
  registerUnit(createSIFrequency,UnitSystem::SI);

  // SI Common Compound Units ==================================================
  registerUnit(createSIEnergyUseIntensity);
  registerUnit(createSIPowerDensity,UnitSystem::SI);
  registerUnit(createSIPowerPerPerson,UnitSystem::SI);
  registerUnit(createSIPressure);
  registerUnit(createSIThermalConductance);
  registerUnit(createSIThermalResistance);
  registerUnit(createSIHeatCapacity);


  // IP Base Units =============================================================
  registerUnit(createIPMass);
  registerUnit(createIPLength,UnitSystem::IP);
  registerUnit(createIPTime,UnitSystem::IP);
  registerUnit(createIPTemperature,UnitSystem::IP);
  registerUnit(createIPElectricCurrent,UnitSystem::IP);
  registerUnit(createIPLuminousIntensity,UnitSystem::IP);
  registerUnit(createIPAmountOfSubstance,UnitSystem::IP);
  registerUnit(createIPAngle,UnitSystem::IP);
  registerUnit(createIPSolidAngle,UnitSystem::IP);
  registerUnit(createIPPeople,UnitSystem::IP);
  registerUnit(createIPCycle,UnitSystem::IP);

  // IP Derived Units ==========================================================
  registerUnit(createIPForce);
  registerUnit(createIPEnergy); // ft*lb-f
  registerUnit(createIPPower); // ft*lb-f/s
  registerUnit(createIPElectricCharge,UnitSystem::IP);
  registerUnit(createIPLuminousFlux,UnitSystem::IP);
  registerUnit(createIPIlluminance,UnitSystem::IP);
  registerUnit(createIPFrequency,UnitSystem::IP);


  // BTU Base Units ============================================================
  registerUnit(createBTUEnergy);
  registerUnit(createBTULength,UnitSystem::BTU);
  registerUnit(createBTUTime,UnitSystem::BTU);
  registerUnit(createBTUTemperature,UnitSystem::BTU);
  registerUnit(createBTUElectricCurrent,UnitSystem::BTU);
  registerUnit(createBTULuminousIntensity,UnitSystem::BTU);
  registerUnit(createBTUAmountOfSubstance,UnitSystem::BTU);
  registerUnit(createBTUAngle,UnitSystem::BTU);
  registerUnit(createBTUSolidAngle,UnitSystem::BTU);
  registerUnit(createBTUPeople,UnitSystem::BTU);
  registerUnit(createBTUCycle,UnitSystem::BTU);

  // BTU Derived Units =========================================================
  registerUnit(createBTUPower);
  registerUnit(createBTULuminousFlux,UnitSystem::BTU);
  registerUnit(createBTUIlluminance,UnitSystem::BTU);


  // CFM Base Units ============================================================
  registerUnit(createCFMLength,UnitSystem::CFM);
  registerUnit(createCFMTime);
  registerUnit(createCFMPower);
  registerUnit(createCFMTemperature,UnitSystem::CFM);
  registerUnit(createCFMElectricCurrent,UnitSystem::CFM);
  registerUnit(createCFMLuminousIntensity,UnitSystem::CFM);
  registerUnit(createCFMAmountOfSubstance,UnitSystem::CFM);
  registerUnit(createCFMAngle,UnitSystem::CFM);
  registerUnit(createCFMSolidAngle,UnitSystem::CFM);
  registerUnit(createCFMPeople,UnitSystem::CFM);
  registerUnit(createCFMCycle,UnitSystem::CFM);

  // CFM Derived Units =========================================================
  registerUnit(createCFMVolumetricFlowrate);
  registerUnit(createCFMLuminousFlux,UnitSystem::CFM);
  registerUnit(createCFMIlluminance,UnitSystem::CFM);
  registerUnit(createCFMFrequency);


  // GPD Base Units ============================================================
  registerUnit(createGPDPressure);
  registerUnit(createGPDLength);
  registerUnit(createGPDTime,UnitSystem::GPD);
  registerUnit(createGPDTemperature,UnitSystem::GPD);
  registerUnit(createGPDElectricCurrent,UnitSystem::GPD);
  registerUnit(createGPDLuminousIntensity,UnitSystem::GPD);
  registerUnit(createGPDAmountOfSubstance,UnitSystem::GPD);
  registerUnit(createGPDAngle,UnitSystem::GPD);
  registerUnit(createGPDSolidAngle,UnitSystem::GPD);
  registerUnit(createGPDPeople,UnitSystem::GPD);
  registerUnit(createGPDCycle,UnitSystem::GPD);

  // GPD Derived Units =========================================================
  registerUnit(createGPDVolume);
  registerUnit(createGPDVolumetricFlowrate);
  registerUnit(createGPDLuminousFlux,UnitSystem::GPD);


  // MPH Base Units ============================================================
  registerUnit(createMPHPressure);
  registerUnit(createMPHLength);
  registerUnit(createMPHTime,UnitSystem::MPH);
  registerUnit(createMPHTemperature,UnitSystem::MPH);
  registerUnit(createMPHElectricCurrent,UnitSystem::MPH);
  registerUnit(createMPHLuminousIntensity,UnitSystem::MPH);
  registerUnit(createMPHAmountOfSubstance,UnitSystem::MPH);
  registerUnit(createMPHAngle,UnitSystem::MPH);
  registerUnit(createMPHSolidAngle,UnitSystem::MPH);
  registerUnit(createMPHPeople,UnitSystem::MPH);
  registerUnit(createMPHCycle,UnitSystem::MPH);

  // MPH Derived Units =========================================================
  registerUnit(createMPHVelocity);
  registerUnit(createMPHLuminousFlux,UnitSystem::MPH);


  // Wh Base Units =============================================================
  registerUnit(createWhPower);
  registerUnit(createWhTime,UnitSystem::Wh);
  registerUnit(createWhLength,UnitSystem::Wh);
  registerUnit(createWhTemperature,UnitSystem::Wh);
  registerUnit(createWhElectricCurrent,UnitSystem::Wh);
  registerUnit(createWhLuminousIntensity,UnitSystem::Wh);
  registerUnit(createWhAmountOfSubstance,UnitSystem::Wh);
  registerUnit(createWhAngle,UnitSystem::Wh);
  registerUnit(createWhSolidAngle,UnitSystem::Wh);
  registerUnit(createWhPeople,UnitSystem::Wh);
  registerUnit(createWhCycle,UnitSystem::Wh);

  // Wh Derived Units ==========================================================
  registerUnit(createWhEnergy);
  registerUnit(createWhElectricalPotential,UnitSystem::Wh);
  registerUnit(createWhLuminousFlux,UnitSystem::Wh);
  registerUnit(createWhIlluminance,UnitSystem::Wh);


  // Therm Base Units ==========================================================
  registerUnit(createThermEnergy);
  registerUnit(createThermLength);
  registerUnit(createThermTime);
  registerUnit(createThermTemperature,UnitSystem::Therm);
  registerUnit(createThermElectricCurrent,UnitSystem::Therm);
  registerUnit(createThermLuminousIntensity,UnitSystem::Therm);
  registerUnit(createThermAmountOfSubstance,UnitSystem::Therm);
  registerUnit(createThermAngle,UnitSystem::Therm);
  registerUnit(createThermSolidAngle,UnitSystem::Therm);
  registerUnit(createThermPeople,UnitSystem::Therm);
  registerUnit(createThermCycle,UnitSystem::Therm);

  // Therm Derived Units =======================================================
  registerUnit(createThermLuminousFlux,UnitSystem::Therm);


  // Misc1 Base Units ==========================================================
  registerUnit(createMisc1Pressure);
  registerUnit(createMisc1Length);
  registerUnit(createMisc1Time,UnitSystem::Misc1);
  registerUnit(createMisc1Temperature,UnitSystem::Misc1);
  registerUnit(createMisc1ElectricCurrent,UnitSystem::Misc1);
  registerUnit(createMisc1LuminousIntensity,UnitSystem::Misc1);
  registerUnit(createMisc1AmountOfSubstance,UnitSystem::Misc1);
  registerUnit(createMisc1Angle,UnitSystem::Misc1);
  registerUnit(createMisc1SolidAngle,UnitSystem::Misc1);
  registerUnit(createMisc1People,UnitSystem::Misc1);
  registerUnit(createMisc1Cycle,UnitSystem::Misc1);

  // Misc1 Derived Units =======================================================
  registerUnit(createMisc1Volume);
  registerUnit(createMisc1LuminousFlux,UnitSystem::Misc1);

  // Mixed Derived Units =======================================================
  registerUnit(createIPPowerDensity);
  registerUnit(createIPPressure);

  // Equivalent Strings ========================================================
  registerEquivalentString("person","people");
  registerEquivalentString("cycles","cycle");
  registerEquivalentString("lb","lb_m");
  registerEquivalentString("CFM","ft^3/min");
  registerEquivalentString("Therm","therm");
  registerEquivalentString("thm","therm");
  registerEquivalentString("mile","mi");
  registerEquivalentString("miles","mi");
  registerEquivalentString("days","day");
  registerEquivalentString("year","yr");
  registerEquivalentString("years","yr");
  registerEquivalentString("hr","h");
  registerEquivalentString("hrs","h");
}

UnitSystem getSystem(const std::string& unitString) {

  OptionalUnit unit;

  // pick up preferred unit system for strings like "C" and "F"
  unit = createUnit(unitString,UnitSystem::Mixed);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  unit = createUnit(unitString,UnitSystem::SI);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  unit = createUnit(unitString,UnitSystem::IP);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  unit = createUnit(unitString,UnitSystem::BTU);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  unit = createUnit(unitString,UnitSystem::CFM);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  unit = createUnit(unitString,UnitSystem::GPD);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  unit = createUnit(unitString,UnitSystem::MPH);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  unit = createUnit(unitString,UnitSystem::Wh);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  unit = createUnit(unitString,UnitSystem::Therm);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  unit = createUnit(unitString,UnitSystem::Misc1);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  unit = createUnit(unitString,UnitSystem::Celsius);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  unit = createUnit(unitString,UnitSystem::Fahrenheit);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit->system();
  }

  return UnitSystem::Mixed;
}

bool isInSystem(const std::string& unitString, UnitSystem system) {
  OptionalUnit unit = createUnit(unitString,system);
  if (unit && ((unit->system() == system) || (system == UnitSystem::Mixed))) {
    return true;
  }
  return false;
}

bool containsRegisteredBaseUnit(const std::string& unitString) {
  if (isUnitString(unitString)) {
    Unit u = createUnit(unitString).get();
    StringVector bus = u.baseUnits();
    for (const std::string& bu : bus) {
      if (getSystem(bu) != UnitSystem::Mixed) {
        return true;
      }
    }
  }
  return false;
}

std::string extractUnitString(const std::string& text) {
  boost::smatch m;
  std::string wtext(text);
  std::string result;

  // first handle people/100 m^2
  if (boost::regex_search(wtext,m,regexEmbeddedDirectScaledUnit())) {
    // main match at 1, 5, 9, or 13
    for (unsigned i = 1; i < 14; i += 4) {
      result = std::string(m[i].first,m[i].second);
      if (!result.empty()) { 
        return result;
      }
    }
  }

  std::string::const_iterator wbegin = wtext.begin();
  std::string::const_iterator wend = wtext.end();

  while (boost::regex_search(wbegin, wend, m,regexEmbeddedUnit())) {
    unsigned i = 1;
    result = std::string(m[i].first,m[i].second);
    while (result.empty()) {
      ++i;
      if (i < m.size()) { result = std::string(m[i].first,m[i].second); }
      else { break; }
    }
    if (containsRegisteredBaseUnit(result)) { break; }
    result.clear();
    wbegin = m[0].second;
  }

  return result;
}

std::string convertToStandardForm(const std::string& unitString) {
  std::string result(unitString);
  if (isDirectScaledUnit(unitString)) {
    std::pair<std::string,std::pair<unsigned,std::string> > fragments = 
        decomposeDirectScaledUnit(unitString);
    ScaleConstant s;
    s = ScaleFactory::instance().createScale(-static_cast<int>(fragments.second.first));
    if (s().value != 0.0) {
      std::stringstream ss;
      ss << s().abbr << "(";
      if (!fragments.second.second.empty()) {
        ss << fragments.first << fragments.second.second;
      }
      else {
        fragments.first.resize(fragments.first.size() - 1); // remove '/'
        ss << fragments.first;
      }
      ss << ")";
      result = ss.str();
    }
  }
  return result;
}

std::string replaceUnitString(const std::string& text,const std::string& newUnitString) {
  std::string toReplace = extractUnitString(text);
  std::string result(text);
  if (!toReplace.empty()) {
    result = boost::regex_replace(text,boost::regex(toReplace,boost::regex_constants::literal),
                                  newUnitString,boost::regex_constants::format_literal);
  }
  return result;
}

bool isUnitString(const std::string& unitString) {
  OptionalUnit unit = createUnit(unitString);
  return unit;
}

Unit createDimensionlessUnit(UnitSystem system) {
  switch (system.value()) {
  case UnitSystem::Mixed:
    return Unit();
  case UnitSystem::SI:
    return SIUnit();
  case UnitSystem::IP:
    return IPUnit();
  case UnitSystem::BTU:
    return BTUUnit();
  case UnitSystem::CFM:
    return CFMUnit();
  case UnitSystem::GPD:
    return GPDUnit();
  case UnitSystem::MPH:
    return MPHUnit();
  case UnitSystem::Therm:
    return ThermUnit();
  case UnitSystem::Wh:
    return WhUnit();
  case UnitSystem::Misc1:
    return Misc1Unit();
  case UnitSystem::Celsius:
    return CelsiusUnit();
  case UnitSystem::Fahrenheit:
    return FahrenheitUnit();
  default:
    OS_ASSERT(false);
  }
  return Unit();
}

boost::optional<Unit> createUnit(const std::string& unitString,UnitSystem system) {
  return UnitFactory::instance().createUnit(unitString,system);
}

boost::optional<Unit> createUnit(const std::string& unitString) {
  OptionalUnit unit;

  // pick up preferred unit system for strings like "C" and "F"
  unit = createUnit(unitString,UnitSystem::Mixed);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  unit = createUnit(unitString,UnitSystem::SI);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  unit = createUnit(unitString,UnitSystem::IP);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  unit = createUnit(unitString,UnitSystem::BTU);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  unit = createUnit(unitString,UnitSystem::CFM);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  unit = createUnit(unitString,UnitSystem::GPD);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  unit = createUnit(unitString,UnitSystem::MPH);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  unit = createUnit(unitString,UnitSystem::Wh);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  unit = createUnit(unitString,UnitSystem::Therm);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  unit = createUnit(unitString,UnitSystem::Misc1);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  unit = createUnit(unitString,UnitSystem::Celsius);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  unit = createUnit(unitString,UnitSystem::Fahrenheit);
  if (unit && (unit->system() != UnitSystem::Mixed)) {
    return unit;
  }

  return createUnit(unitString,UnitSystem::Mixed);
}

bool unitStringsEqual(const std::string& uStr1,const std::string& uStr2) {
  if (isUnitString(uStr1) && isUnitString(uStr2)) {
    Unit u1 = createUnit(uStr1).get();
    Unit u2 = createUnit(uStr2).get();
    return (u1 == u2);
  }
  return false;
}

Unit createIPPowerDensity() {
  Unit result;
  result.setBaseUnitExponent("kg",1);
  result.setBaseUnitExponent("m",2);
  result.setBaseUnitExponent("s",-3);
  result.setBaseUnitExponent("ft",-2);
  result.setPrettyString("W/ft^2");
  return result;
}

Unit createGPMVolumetricFlowrate() {
  Unit result;
  result.setBaseUnitExponent("crgal",3);
  result.setBaseUnitExponent("min",-1);
  result.setPrettyString("gal/min");
  return result;
}

Unit createIPPressure() {
  Unit result;
  result.setBaseUnitExponent("lb_f",1);
  result.setBaseUnitExponent("in",-2);
  result.setPrettyString("psi");
  return result;
}

} // openstudio
