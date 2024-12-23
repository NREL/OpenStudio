/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "QuantityConverter.hpp"
#include "Quantity.hpp"
#include "OSQuantityVector.hpp"

#include "UnitFactory.hpp"
#include "ScaleFactory.hpp"
#include "Unit.hpp"
#include "BTUUnit.hpp"
#include "CelsiusUnit.hpp"
#include "CFMUnit.hpp"
#include "FahrenheitUnit.hpp"
#include "IPUnit.hpp"
#include "SIUnit.hpp"
#include "ThermUnit.hpp"
#include "WhUnit.hpp"

#include "../core/Assert.hpp"

namespace openstudio {

boost::optional<Quantity> QuantityConverterSingleton::convert(const Quantity& q, UnitSystem sys) const {
  if ((q.system() != UnitSystem::Mixed) && (q.system() == sys)) {
    return q;
  }
  if (sys == UnitSystem::Mixed) {
    LOG(Error, "Cannot convert a quantity to UnitSystem::Mixed.")
    return boost::none;
  }

  Quantity working;
  Quantity converted;
  OptionalQuantity candidate;

  // All conversions go through SI, so convert to SI if not already there,
  if ((q.system() != UnitSystem::SI)) {
    // If not already SI or NA, go through SI
    candidate = m_convertToSI(q);
    if (!candidate) {
      return boost::none;
    }
    working = *candidate;
  } else {
    // Otherwise we are already in SI
    working = q;
  }

  if ((sys == UnitSystem::SI)) {
    converted = working;
  } else {
    converted = m_convertFromSI(working, sys);
  }

  if (q.isTemperature() && converted.isTemperature()) {
    if (q.isRelative()) {
      converted.setAsRelative();
    }
  }

  return converted;
}

boost::optional<Quantity> QuantityConverterSingleton::convert(const Quantity& original, const Unit& targetUnits) const {
  Quantity working(original);
  OptionalQuantity candidate;

  // See if nothing to be done. (Check for equality of system and base units + exponents.)
  if ((working.system() == targetUnits.system()) && (working.units() == targetUnits)) {
    // Assume targetUnits has desired scale.
    working.setScale(targetUnits.scale().exponent);
    return working;
  }

  // All conversions go through SI
  if (working.system() != UnitSystem::SI) {
    candidate = m_convertToSI(working);
    if (!candidate) {
      return boost::none;
    }
    working = *candidate;
  }

  // Retain pretty string
  OptionalQuantity result = m_convertToTargetFromSI(working, targetUnits);
  if (result && result->prettyUnitsString(false).empty() && !targetUnits.prettyString(false).empty()) {
    result->setPrettyUnitsString(targetUnits.prettyString(false));
  }

  return result;
}

QuantityConverterSingleton::QuantityConverterSingleton() {
  // initialize the quantity converter maps here

  const baseUnitConversionFactor toSI[] = {{UnitSystem::SI, "kg", "kg", 1.0, 0},
                                           {UnitSystem::SI, "m", "m", 1.0, 0},
                                           {UnitSystem::SI, "s", "s", 1.0, 0},
                                           {UnitSystem::SI, "K", "K", 1.0, 0},
                                           {UnitSystem::SI, "A", "A", 1.0, 0},
                                           {UnitSystem::SI, "cd", "cd", 1.0, 0},
                                           {UnitSystem::SI, "mol", "mol", 1.0, 0},
                                           {UnitSystem::SI, "rad", "rad", 1.0, 0},
                                           {UnitSystem::SI, "sr", "sr", 1.0, 0},
                                           {UnitSystem::SI, "people", "people", 1.0, 0},
                                           {UnitSystem::SI, "cycle", "cycle", 1.0, 0},
                                           {UnitSystem::SI, "$", "$", 1.0, 0},
                                           {UnitSystem::IP, "lb_m", "kg", .45359237, 0},
                                           {UnitSystem::IP, "ft", "m", 0.3048, 0},
                                           {UnitSystem::IP, "R", "K", 0.55555555555555556, 0},
                                           {UnitSystem::IP, "lbmol", "mol", 453.59237, 0},
                                           {UnitSystem::IP, "lb_f", "kg*m/s^2", 4.44822161526, 0},
                                           {UnitSystem::IP, "deg", "rad", 0.0174532925199433, 0},
                                           {UnitSystem::BTU, "Btu", "kg*m^2/s^2", 1055.05585262, 0},
                                           {UnitSystem::BTU, "h", "s", 3600.0, 0},
                                           {UnitSystem::CFM, "ton", "kg*m^2/s^3", 3516.85284207, 0},
                                           {UnitSystem::CFM, "min", "s", 60.0, 0},
                                           {UnitSystem::GPD, "inH_{2}O", "kg/m*s^2", 249.08891, 0},
                                           {UnitSystem::GPD, "crgal", "m", 0.155849127967, 0},
                                           {UnitSystem::GPD, "day", "s", 86400.0, 0},
                                           {UnitSystem::MPH, "inHg", "kg/m*s^2", 3386.38815789, 0},
                                           {UnitSystem::MPH, "mi", "m", 1609.344, 0},
                                           {UnitSystem::Wh, "W", "kg*m^2/s^3", 1.0, 0},
                                           {UnitSystem::Therm, "therm", "kg*m^2/s^2", 105505585.262, 0},
                                           {UnitSystem::Therm, "in", "m", 0.0254, 0},
                                           {UnitSystem::Therm, "yr", "s", 31556925.9747, 0},
                                           {UnitSystem::Misc1, "ftH_{2}O", "kg/m*s^2", 2989.06692, 0},
                                           {UnitSystem::Misc1, "crL", "m", 0.1, 0},
                                           {UnitSystem::Celsius, "C", "K", 1.0, 273.15},
                                           {UnitSystem::Fahrenheit, "F", "K", 0.555555555555555555556, 255.37222222222222222222222},
                                           {UnitSystem::Mixed, "", "", 0.0, 0.0}};

  //loop over them while from != ""?
  int i = 0;
  while (!(toSI[i].originalUnit.empty() && toSI[i].targetUnit.empty())) {
    this->m_toSImap.insert(BaseUnitConversionMap::value_type(toSI[i].originalUnit, toSI[i]));
    ++i;
  }

  const baseUnitConversionFactor fromSI[] = {{UnitSystem::IP, "kg", "lb_m", 2.20462262185, 0},
                                             {UnitSystem::IP, "m", "ft", 3.28083989501, 0},
                                             {UnitSystem::IP, "s", "s", 1.0, 0},
                                             {UnitSystem::IP, "K", "R", 1.8, 0},
                                             {UnitSystem::IP, "A", "A", 1.0, 0},
                                             {UnitSystem::IP, "cd", "cd", 1.0, 0},
                                             {UnitSystem::IP, "mol", "lbmol", 0.00220462262185, 0},
                                             {UnitSystem::IP, "rad", "deg", 57.2957795130823, 0},
                                             {UnitSystem::IP, "sr", "sr", 1.0, 0},
                                             {UnitSystem::IP, "people", "people", 1.0, 0},
                                             {UnitSystem::IP, "cycle", "cycle", 1.0, 0},
                                             {UnitSystem::IP, "$", "$", 1.0, 0},
                                             {UnitSystem::BTU, "kg", "Btu*h^2/ft^2", 6.79437437047e-12, 0},
                                             {UnitSystem::BTU, "m", "ft", 3.28083989501, 0},
                                             {UnitSystem::BTU, "s", "h", 2.77777777777778e-4, 0},
                                             {UnitSystem::BTU, "K", "R", 1.8, 0},
                                             {UnitSystem::BTU, "A", "A", 1.0, 0},
                                             {UnitSystem::BTU, "cd", "cd", 1.0, 0},
                                             {UnitSystem::BTU, "mol", "lbmol", 0.00220462262185, 0},
                                             {UnitSystem::BTU, "rad", "deg", 57.2957795130823, 0},
                                             {UnitSystem::BTU, "sr", "sr", 1.0, 0},
                                             {UnitSystem::BTU, "people", "people", 1.0, 0},
                                             {UnitSystem::BTU, "cycle", "cycle", 1.0, 0},
                                             {UnitSystem::BTU, "$", "$", 1.0, 0},
                                             {UnitSystem::CFM, "kg", "ton*min^3/ft^2", 1.22298738668e-10, 0},
                                             {UnitSystem::CFM, "m", "ft", 3.28083989501, 0},
                                             {UnitSystem::CFM, "s", "min", 0.0166666666666667, 0},
                                             {UnitSystem::CFM, "K", "R", 1.8, 0},
                                             {UnitSystem::CFM, "A", "A", 1.0, 0},
                                             {UnitSystem::CFM, "cd", "cd", 1.0, 0},
                                             {UnitSystem::CFM, "mol", "lbmol", 0.00220462262185, 0},
                                             {UnitSystem::CFM, "rad", "deg", 57.2957795130823, 0},
                                             {UnitSystem::CFM, "sr", "sr", 1.0, 0},
                                             {UnitSystem::CFM, "people", "people", 1.0, 0},
                                             {UnitSystem::CFM, "cycle", "cycle", 1.0, 0},
                                             {UnitSystem::CFM, "$", "$", 1.0, 0},
                                             {UnitSystem::GPD, "kg", "inH_{2}O*crgal*day^2", 3.45075198328e-12, 0},
                                             {UnitSystem::GPD, "m", "crgal", 6.41646195293, 0},
                                             {UnitSystem::GPD, "s", "day", 1.157407407407e-5, 0},
                                             {UnitSystem::GPD, "K", "R", 1.8, 0},
                                             {UnitSystem::GPD, "A", "A", 1.0, 0},
                                             {UnitSystem::GPD, "cd", "cd", 1.0, 0},
                                             {UnitSystem::GPD, "mol", "lbmol", 0.00220462262185, 0},
                                             {UnitSystem::GPD, "rad", "deg", 57.2957795130823, 0},
                                             {UnitSystem::GPD, "sr", "sr", 1.0, 0},
                                             {UnitSystem::GPD, "people", "people", 1.0, 0},
                                             {UnitSystem::GPD, "cycle", "cycle", 1.0, 0},
                                             {UnitSystem::GPD, "$", "$", 1.0, 0},
                                             {UnitSystem::MPH, "kg", "inHg*mi*h^2", 1.41582434758e-14, 0},
                                             {UnitSystem::MPH, "m", "mi", 6.21371192237e-4, 0},
                                             {UnitSystem::MPH, "s", "h", 2.77777777777778e-4, 0},
                                             {UnitSystem::MPH, "K", "R", 1.8, 0},
                                             {UnitSystem::MPH, "A", "A", 1.0, 0},
                                             {UnitSystem::MPH, "cd", "cd", 1.0, 0},
                                             {UnitSystem::MPH, "mol", "lbmol", 0.00220462262185, 0},
                                             {UnitSystem::MPH, "rad", "deg", 57.2957795130823, 0},
                                             {UnitSystem::MPH, "sr", "sr", 1.0, 0},
                                             {UnitSystem::MPH, "people", "people", 1.0, 0},
                                             {UnitSystem::MPH, "cycle", "cycle", 1.0, 0},
                                             {UnitSystem::MPH, "$", "$", 1.0, 0},
                                             {UnitSystem::Wh, "kg", "W*h^3/m^2", 2.1433470507544582e-11, 0},
                                             {UnitSystem::Wh, "m", "m", 1.0, 0},
                                             {UnitSystem::Wh, "s", "h", 2.77777777777778e-4, 0},
                                             {UnitSystem::Wh, "K", "K", 1.0, 0},
                                             {UnitSystem::Wh, "A", "A", 1.0, 0},
                                             {UnitSystem::Wh, "cd", "cd", 1.0, 0},
                                             {UnitSystem::Wh, "mol", "mol", 1.0, 0},
                                             {UnitSystem::Wh, "rad", "rad", 1.0, 0},
                                             {UnitSystem::Wh, "sr", "sr", 1.0, 0},
                                             {UnitSystem::Wh, "people", "people", 1.0, 0},
                                             {UnitSystem::Wh, "cycle", "cycle", 1.0, 0},
                                             {UnitSystem::Wh, "$", "$", 1.0, 0},
                                             {UnitSystem::Therm, "kg", "therm*h^2/ft^2", 6.79437437047e-17, 0},
                                             {UnitSystem::Therm, "m", "ft", 3.28083989501, 0},
                                             {UnitSystem::Therm, "s", "h", 2.77777777777778e-4, 0},
                                             {UnitSystem::Therm, "K", "R", 1.8, 0},
                                             {UnitSystem::Therm, "A", "A", 1.0, 0},
                                             {UnitSystem::Therm, "cd", "cd", 1.0, 0},
                                             {UnitSystem::Therm, "mol", "lbmol", 0.00220462262185, 0},
                                             {UnitSystem::Therm, "rad", "deg", 57.2957795130823, 0},
                                             {UnitSystem::Therm, "sr", "sr", 1.0, 0},
                                             {UnitSystem::Therm, "people", "people", 1.0, 0},
                                             {UnitSystem::Therm, "cycle", "cycle", 1.0, 0},
                                             {UnitSystem::Therm, "$", "$", 1.0, 0},
                                             {UnitSystem::Misc1, "kg", "ftH_{2}O*crL*day^2", 4.48163906187e-13, 0},
                                             {UnitSystem::Misc1, "m", "crL", 10.0, 0},
                                             {UnitSystem::Misc1, "s", "day", 1.157407407407e-5, 0},
                                             {UnitSystem::Misc1, "K", "K", 1.0, 0},
                                             {UnitSystem::Misc1, "A", "A", 1.0, 0},
                                             {UnitSystem::Misc1, "cd", "cd", 1.0, 0},
                                             {UnitSystem::Misc1, "mol", "mol", 1.0, 0},
                                             {UnitSystem::Misc1, "rad", "rad", 1.0, 0},
                                             {UnitSystem::Misc1, "sr", "sr", 1.0, 0},
                                             {UnitSystem::Misc1, "people", "people", 1.0, 0},
                                             {UnitSystem::Misc1, "cycle", "cycle", 1.0, 0},
                                             {UnitSystem::Misc1, "$", "$", 1.0, 0},
                                             {UnitSystem::Celsius, "K", "C", 1.0, -273.15},
                                             {UnitSystem::Fahrenheit, "K", "F", 1.8, -459.67},
                                             {UnitSystem::Mixed, "", "", 0, 0}};

  i = 0;
  while (!(fromSI[i].originalUnit.empty() && fromSI[i].targetUnit.empty())) {
    this->m_fromSIBySystemMap.insert(UnitSystemConversionMultiMap::value_type(fromSI[i].system, fromSI[i]));
    ++i;
  }
}

boost::optional<Quantity> QuantityConverterSingleton::m_convertToSI(const Quantity& original) const {
  // create a working copy of the original
  Quantity working(original);
  // Make sure to work unscaled: 10^0
  int scaleExponent = working.scale().exponent;
  if (working.scale().exponent != 0) {
    working.setScale(0);
  }
  // build a result quantity with SI units and value equal to original
  Quantity result(working.value(), UnitSystem(UnitSystem::SI));

  // Get the base units of original
  std::vector<std::string> baseOfOriginal = original.baseUnits();
  // Loop over base units in original and apply conversions found in m_toSImap
  auto it = baseOfOriginal.begin();
  auto end = baseOfOriginal.end();
  while (it != end) {
    int baseExponent = working.baseUnitExponent(*it);
    // apply conversion factor
    auto mapItr = m_toSImap.find(*it);
    if (mapItr == m_toSImap.end()) {
      LOG(Error, "Cannot convert base unit '" << *it << "' to SI because it is not " << "registered with the QuantityConverter.");
      return boost::none;
    }
    baseUnitConversionFactor factor = mapItr->second;
    if (factor.offset != 0.0) {
      for (int i = 0; i < std::abs(baseExponent); ++i) {
        if (baseExponent > 0) {
          result.setValue((result.value() * factor.factor) + factor.offset);
        } else {
          result.setValue((result.value() / factor.factor) + factor.offset);
        }
      }
    } else {
      result.setValue(result.value() * std::pow(factor.factor, baseExponent));
    }
    // Parse the conversion string in case the original converts to more than one SI base unit
    Unit targetBase = parseUnitString(factor.targetUnit);
    std::vector<std::string> targetStrings = targetBase.baseUnits();
    auto targetItr = targetStrings.begin();
    auto targetEnd = targetStrings.end();
    while (targetItr != targetEnd) {
      int exp = targetBase.baseUnitExponent(*targetItr);
      if (exp != 0) {
        result.setBaseUnitExponent(*targetItr, result.baseUnitExponent(*targetItr) + baseExponent * exp);
      }
      ++targetItr;
    }  //End while( convItr != convEnd )
    ++it;
  }  //End while( it != end )

  // Set result scale to match original scale
  if (scaleExponent != 0) {
    result.setScale(scaleExponent);
  }

  // Check if there is a pretty string for the result
  std::string pretty = UnitFactory::instance().lookupPrettyString(result.standardUnitsString(false));
  if (!(pretty.empty())) {
    result.setPrettyUnitsString(pretty);
  }

  return result;
}

Quantity QuantityConverterSingleton::m_convertFromSI(const Quantity& original, const UnitSystem& targetSys) const {
  Quantity working(original);

  // Make sure to work unscaled: 10^0
  int scaleExponent = working.scale().exponent;
  if (working.scale().exponent != 0) {
    working.setScale(0);
  }
  Quantity converted(working.value(), targetSys);

  UnitSystemConversionMultiMap::const_iterator factorItr;
  // m_fromSImap is a multi-map, so get the range of values that match the key 'targetSys'
  std::pair<UnitSystemConversionMultiMap::const_iterator, UnitSystemConversionMultiMap::const_iterator> systemFactors;
  systemFactors = m_fromSIBySystemMap.equal_range(targetSys);
  for (factorItr = systemFactors.first; factorItr != systemFactors.second; ++factorItr) {
    baseUnitConversionFactor fromFactor = (*factorItr).second;
    int workingExp = working.baseUnitExponent(fromFactor.originalUnit);
    if (fromFactor.offset != 0.0) {
      for (int i = 0; i < std::abs(workingExp); ++i) {
        if (workingExp > 0) {
          converted.setValue((converted.value() * fromFactor.factor) + fromFactor.offset);
        } else {
          converted.setValue((converted.value() / fromFactor.factor) + fromFactor.offset);
        }
      }
    } else {
      converted.setValue(converted.value() * std::pow(fromFactor.factor, workingExp));
    }
    // Parse the conversion string incase the SI unit converts to more than one target base unit
    Unit targetBase = parseUnitString(fromFactor.targetUnit);
    std::vector<std::string> targetStrings = targetBase.baseUnits();
    auto targetItr = targetStrings.begin();
    auto targetEnd = targetStrings.end();
    while (targetItr != targetEnd) {
      int exp = targetBase.baseUnitExponent(*targetItr);
      if (exp != 0) {
        converted.setBaseUnitExponent(*targetItr, converted.baseUnitExponent(*targetItr) + workingExp * exp);
      }
      ++targetItr;
    }  //End while( convItr != targetEnd )
  }

  // Set result scale to match original scale
  if (scaleExponent != 0) {
    converted.setScale(scaleExponent);
  }

  // Check if there is a pretty string for the result
  std::string pretty = UnitFactory::instance().lookupPrettyString(converted.standardUnitsString(false));
  if (!(pretty.empty())) {
    converted.setPrettyUnitsString(pretty);
  }

  return converted;
}

boost::optional<Quantity> QuantityConverterSingleton::m_convertToTargetFromSI(const Quantity& original, const Unit& targetUnits) const {
  Quantity working(original);

  // Make sure to work unscaled: 10^0
  if (working.scale().exponent != 0) {
    working.setScale(0);
  }
  Quantity converted(working.value(), targetUnits.system());

  if ((working.units() == targetUnits) && (working.system() == targetUnits.system())) {
    converted = working;
  } else {
    // Get the base units of targetUnits
    std::vector<std::string> baseOfTarget = targetUnits.baseUnits();
    // Loop over base units in original and apply (inverse of) conversions in m_toSImap
    auto it = baseOfTarget.begin();
    auto end = baseOfTarget.end();
    while (it != end) {
      int baseExponent = targetUnits.baseUnitExponent(*it);
      // apply conversion factor
      auto mapItr = m_toSImap.find(*it);
      if (mapItr == m_toSImap.end()) {
        LOG(Error, "Cannot convert to a target Unit containing base unit '" << *it << "', because it is not registered with the QuantityConverter.");
        return boost::none;
      }
      baseUnitConversionFactor factor = m_toSImap.find(*it)->second;
      if (factor.offset != 0.0) {
        for (int i = 0; i < std::abs(baseExponent); ++i) {
          if (baseExponent > 0) {
            converted.setValue((converted.value() - factor.offset) / factor.factor);
          } else {
            converted.setValue((converted.value() - factor.offset) * factor.factor);
          }
        }
      } else {
        converted.setValue(converted.value() * std::pow(factor.factor, -baseExponent));
      }
      // Set units in converted
      converted.setBaseUnitExponent(*it, converted.baseUnitExponent(*it) + baseExponent);
      // Monitor left over units in working
      // Parse the conversion string in case the target converts to more than one SI base unit
      Unit sourceBase = parseUnitString(factor.targetUnit);
      std::vector<std::string> sourceStrings = sourceBase.baseUnits();
      auto sourceItr = sourceStrings.begin();
      auto sourceEnd = sourceStrings.end();
      while (sourceItr != sourceEnd) {
        int exp = sourceBase.baseUnitExponent(*sourceItr);
        if (exp != 0) {
          working.setBaseUnitExponent(*sourceItr, working.baseUnitExponent(*sourceItr) - baseExponent * exp);
        }
        ++sourceItr;
      }  // end while( sourceItr != sourceEnd )
      ++it;
    }

    if (!working.standardUnitsString().empty()) {
      LOG(Error, "Could not convert " << original << " to " << targetUnits << ". Have " << working.standardUnitsString() << "left over.");
      return boost::none;
    }
  }

  // Set result scale to match targetUnits scale
  if (targetUnits.scale().exponent != 0) {
    converted.setScale(targetUnits.scale().exponent);
  }

  // Check if there is a pretty string for the result
  std::string pretty = UnitFactory::instance().lookupPrettyString(converted.standardUnitsString(false));
  if (!(pretty.empty())) {
    converted.setPrettyUnitsString(pretty);
  }

  return converted;
}

boost::optional<double> convert(double original, const std::string& originalUnits, const std::string& finalUnits) {
  if (originalUnits == finalUnits) {
    return original;
  }

  //create the units from the strings
  boost::optional<Unit> originalUnit = UnitFactory::instance().createUnit(originalUnits);
  boost::optional<Unit> finalUnit = UnitFactory::instance().createUnit(finalUnits);

  //make sure both unit strings were valid
  if (originalUnit && finalUnit) {

    //make the original quantity
    Quantity originalQuant = Quantity(original, *originalUnit);

    //convert to final units
    boost::optional<Quantity> finalQuant = QuantityConverter::instance().convert(originalQuant, *finalUnit);

    //if the conversion
    if (finalQuant) {
      return finalQuant->value();
    }
  }

  return boost::none;
}

boost::optional<Quantity> convert(const Quantity& original, UnitSystem sys) {
  return QuantityConverter::instance().convert(original, sys);
}

OSQuantityVector convert(const OSQuantityVector& original, UnitSystem sys) {
  OSQuantityVector result;
  Quantity testQuantity(0.0, original.units());
  OptionalQuantity offset = convert(testQuantity, sys);
  if (!offset) {
    return result;
  }
  testQuantity.setValue(1.0);
  OptionalQuantity factorPlusOffset = convert(testQuantity, sys);
  OS_ASSERT(factorPlusOffset);
  OS_ASSERT(offset->units() == factorPlusOffset->units());
  result = OSQuantityVector(offset->units(), original.values());
  result = result * (factorPlusOffset->value() - offset->value()) + offset.get();
  return result;
}

boost::optional<Quantity> convert(const Quantity& original, const Unit& targetUnits) {
  return QuantityConverter::instance().convert(original, targetUnits);
}

OSQuantityVector convert(const OSQuantityVector& original, const Unit& targetUnits) {
  OSQuantityVector result;
  Quantity testQuantity(0.0, original.units());
  OptionalQuantity offset = convert(testQuantity, targetUnits);
  if (!offset) {
    return result;
  }
  testQuantity.setValue(1.0);
  OptionalQuantity factorPlusOffset = convert(testQuantity, targetUnits);
  OS_ASSERT(factorPlusOffset);
  OS_ASSERT(offset->units() == factorPlusOffset->units());
  result = OSQuantityVector(offset->units(), original.values());
  result = result * (factorPlusOffset->value() - offset->value()) + offset.get();
  return result;
}

}  // namespace openstudio
