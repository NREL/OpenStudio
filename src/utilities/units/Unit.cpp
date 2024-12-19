/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Unit.hpp"
#include "Unit_Impl.hpp"
#include "Quantity.hpp"
#include "QuantityConverter.hpp"

#include "ScaleFactory.hpp"
#include "QuantityRegex.hpp"
#include "UnitFactory.hpp"

#include "../core/Compare.hpp"
#include "../core/Assert.hpp"
#include "../core/Containers.hpp"

#include <algorithm>
#include <sstream>

namespace openstudio {
namespace detail {

  Unit_Impl::Unit_Impl(int scaleExponent, const std::string& prettyString)
    : m_units(), m_scale(ScaleFactory::instance().createScale(scaleExponent)), m_prettyString(prettyString), m_system(UnitSystem::Mixed) {
    if (scale().value == 0.0) {
      LOG_AND_THROW("Unit constructed with invalid scale exponent. " << "ScaleFactory::instance().registeredScales(), or << ScaleFactory::instance() "
                                                                     << "to see what is available.");
    }
  }

  Unit_Impl::Unit_Impl(const std::string& scaleAbbreviation, const std::string& prettyString)
    : m_units(), m_scale(ScaleFactory::instance().createScale(scaleAbbreviation)), m_prettyString(prettyString), m_system(UnitSystem::Mixed) {
    if (scale().value == 0.0) {
      LOG_AND_THROW("Unit constructed with invalid scaleAbbreviation. "
                    << "ScaleFactory::instance().registeredScales(), or << ScaleFactory::instance() " << "to see what is available.");
    }
  }

  Unit_Impl::Unit_Impl(int scaleExponent, const std::string& prettyString, int numBaseUnits, UnitSystem system)
    : m_units(numBaseUnits), m_scale(ScaleFactory::instance().createScale(scaleExponent)), m_prettyString(prettyString), m_system(system) {
    if (scale().value == 0.0) {
      LOG_AND_THROW("Unit constructed with invalid scale exponent. " << "ScaleFactory::instance().registeredScales(), or << ScaleFactory::instance() "
                                                                     << "to see what is available.");
    }
  }

  Unit_Impl::Unit_Impl(const std::string& scaleAbbreviation, const std::string& prettyString, int numBaseUnits, UnitSystem system)
    : m_units(numBaseUnits), m_scale(ScaleFactory::instance().createScale(scaleAbbreviation)), m_prettyString(prettyString), m_system(system) {
    if (scale().value == 0.0) {
      LOG_AND_THROW("Unit constructed with invalid scaleAbbreviation. "
                    << "ScaleFactory::instance().registeredScales(), or << ScaleFactory::instance() " << "to see what is available.");
    }
  }

  Unit Unit_Impl::clone() const {
    OS_ASSERT(system() == UnitSystem::Mixed);
    std::shared_ptr<Unit_Impl> impl(new Unit_Impl(*this));
    return Unit(impl);
  }

  Unit Unit_Impl::cloneToMixed() const {
    std::shared_ptr<Unit_Impl> impl(new Unit_Impl(*this));
    impl->m_system = UnitSystem::Mixed;
    return Unit(impl);
  }

  std::vector<std::string> Unit_Impl::baseUnits() const {
    // create vector of correct size to hold baseUnit strings
    std::vector<std::string> result(m_units.size());
    std::vector<std::string>::iterator resultIter;

    std::vector<UnitElement>::const_iterator unitsIter;
    auto unitsEnd = m_units.end();
    for (resultIter = result.begin(), unitsIter = m_units.begin(); unitsIter != unitsEnd; ++unitsIter, ++resultIter) {
      *resultIter = unitsIter->first;  // copy baseUnit into result vector
    }

    return result;
  }

  bool Unit_Impl::isBaseUnit(const std::string& baseUnit) const {
    auto loc = findBaseUnit(baseUnit);
    if (loc == m_units.end()) {
      return false;
    }
    return true;
  }

  int Unit_Impl::baseUnitExponent(const std::string& baseUnit) const {
    auto loc = findBaseUnit(baseUnit);
    if (loc == m_units.end()) {
      return 0;
    } else {
      return loc->second;
    }
  }

  void Unit_Impl::setBaseUnitExponent(const std::string& baseUnit, int exponent) {
    auto loc = findBaseUnit(baseUnit);
    if (loc != m_units.end()) {
      loc->second = exponent;
    } else {
      m_units.push_back(UnitElement(baseUnit, exponent));
    }
  }

  Scale Unit_Impl::scale() const {
    return m_scale();
  }

  bool Unit_Impl::setScale(int scaleExponent) {
    ScaleConstant candidate = ScaleFactory::instance().createScale(scaleExponent);
    if (candidate().value == 0.0) {
      return false;
    }
    m_scale = candidate;
    return true;
  }

  bool Unit_Impl::setScale(const std::string& scaleAbbreviation) {
    ScaleConstant candidate = ScaleFactory::instance().createScale(scaleAbbreviation);
    if (candidate().value == 0.0) {
      return false;
    }
    m_scale = candidate;
    return true;
  }

  std::string Unit_Impl::standardString(bool withScale) const {
    if (withScale && (scale().abbr.empty())) {
      withScale = false;
    }

    std::stringstream result;
    bool parentheses(false);

    // determine number of non-zero, positive, and negative baseUnits
    int nnz(0);
    int npos(0);
    std::vector<UnitElement>::const_iterator unitsIter;
    std::vector<UnitElement>::const_iterator firstPosIter;
    std::vector<UnitElement>::const_iterator firstNegIter;
    auto unitsEnd = m_units.end();

    firstPosIter = unitsEnd;
    firstNegIter = unitsEnd;

    for (unitsIter = m_units.begin(); unitsIter != unitsEnd; ++unitsIter) {
      if (unitsIter->second > 0) {
        ++npos;
        ++nnz;
        if (firstPosIter == unitsEnd) {
          firstPosIter = unitsIter;
        }
      }
      if (unitsIter->second < 0) {
        ++nnz;
        if (firstNegIter == unitsEnd) {
          firstNegIter = unitsIter;
        }
      }
    }

    if (withScale) {
      if (nnz == 0) {
        parentheses = true;
        result << scale().abbr << "(";
      } else {
        ScaleConstant printScale = m_scale;
        if (npos > 0) {
          if (firstPosIter->second > 1) {
            parentheses = true;
            // see if can pull scale into exponent
            if (scale().exponent % firstPosIter->second == 0) {
              ScaleConstant candidateScale = ScaleFactory::instance().createScale(scale().exponent / firstPosIter->second);
              if (candidateScale().value != 0.0) {
                printScale = candidateScale;
                parentheses = false;
              }
            }
          }
          result << printScale().abbr;
          if (parentheses) {
            result << "(";
          }
        } else {
          // all baseUnits in the denominator
          // see if can pull scale into exponent of first baseUnit in denominator
          parentheses = true;
          if (scale().exponent % -firstNegIter->second == 0) {
            ScaleConstant candidateScale = ScaleFactory::instance().createScale(scale().exponent / firstNegIter->second);
            if (candidateScale().value != 0.0) {
              printScale = candidateScale;
              parentheses = false;
            }
          }
          if (parentheses) {
            result << printScale().abbr << "(";
          } else {
            result << 1 << "/" << printScale().abbr;
            // go ahead and print denominator of standard string
            for (unitsIter = firstNegIter; unitsIter != unitsEnd; ++unitsIter) {
              if (unitsIter->second < 0) {
                if (unitsIter != firstNegIter) {
                  result << "*";
                }
                result << unitsIter->first;
                if (unitsIter->second < -1) {
                  result << "^" << -unitsIter->second;
                }
              }
            }
            return result.str();
          }
        }
      }
    }

    // print numerator of standard string
    for (unitsIter = firstPosIter; unitsIter != unitsEnd; ++unitsIter) {
      int exp = unitsIter->second;  // determines action and may be output
      if (exp > 0) {
        if (unitsIter != firstPosIter) {
          result << "*";
        }
        result << unitsIter->first;
        if (exp > 1) {
          result << "^" << exp;
        }
      }
    }

    // print denominator of standard string
    for (unitsIter = firstNegIter; unitsIter != unitsEnd; ++unitsIter) {
      int exp = unitsIter->second;
      if (exp < 0) {
        if (unitsIter != firstNegIter) {
          result << "*";
        } else {
          if (npos > 0) {
            result << "/";
          } else {
            result << "1/";
          }
        }
        if (unitsIter->first == "people") {
          result << "person";
        } else {
          result << unitsIter->first;
        }
        if (exp < -1) {
          result << "^" << -exp;
        }
      }  // if
    }  // for

    if (parentheses) {
      result << ")";
    }

    return result.str();
  }

  std::string Unit_Impl::prettyString(bool withScale) const {

    if (withScale && !m_prettyString.empty() && (scale().exponent != 0)) {
      Unit wUnit = parseUnitString(m_prettyString);
      wUnit.setScale(scale().exponent);
      return wUnit.standardString(withScale);
    }

    return m_prettyString;
  }

  void Unit_Impl::setPrettyString(const std::string& str) {
    m_prettyString = str;
  }

  std::string Unit_Impl::print(bool withScale) const {
    if (!prettyString(false).empty()) {
      return prettyString(withScale);
    }
    return standardString(withScale);
  }

  UnitSystem Unit_Impl::system() const {
    return m_system;
  }

  bool Unit_Impl::operator==(const Unit& rUnit) const {

    // construct union of base units
    StringSet allBaseUnits;
    StringVector baseUnits;
    baseUnits = this->baseUnits();
    allBaseUnits.insert(baseUnits.begin(), baseUnits.end());
    baseUnits = rUnit.baseUnits();
    allBaseUnits.insert(baseUnits.begin(), baseUnits.end());

    // loop through and see if exponents are equal
    for (const std::string& baseUnit : allBaseUnits) {
      if (baseUnitExponent(baseUnit) != rUnit.baseUnitExponent(baseUnit)) {
        return false;
      }
    }

    return true;
  }

  void Unit_Impl::operator*=(const Unit& rUnit) {

    if ((m_system != rUnit.system()) && (m_system != UnitSystem::Mixed)) {
      LOG_AND_THROW("Cannot " << m_system.valueName() << " *= " << rUnit.system().valueName()
                              << ". Must have same systems, or UnitSystem::Mixed on the left-hand side.");
    }

    if (this == rUnit.getImpl<Unit_Impl>().get()) {
      // u1 * u1
      this->pow(2);
      return;
    }

    // u1 * u2, loop through both
    std::vector<UnitElement>::iterator lUnitsIter;
    auto lUnitsEnd = m_units.end();
    std::vector<UnitElement>::const_iterator rUnitsIter;
    auto rUnitsEnd = rUnit.getImpl<detail::Unit_Impl>()->m_units.end();
    bool ordered = true;
    for (lUnitsIter = m_units.begin(), rUnitsIter = rUnit.getImpl<detail::Unit_Impl>()->m_units.begin();
         (lUnitsIter != lUnitsEnd) && (rUnitsIter != rUnitsEnd); ++lUnitsIter, ++rUnitsIter) {
      if (lUnitsIter->first != rUnitsIter->first) {
        // different order
        ordered = false;
        break;
      } else {
        lUnitsIter->second += rUnitsIter->second;
      }
    }

    if (ordered && (rUnitsIter != rUnitsEnd)) {
      // finish looping through rUnits and append any non-zero exponented baseUnits
      for (; rUnitsIter != rUnitsEnd; ++rUnitsIter) {
        if (rUnitsIter->second != 0) {
          setBaseUnitExponent(rUnitsIter->first, rUnitsIter->second);
        }
      }
    } else if (!ordered) {
      // mark baseUnits as checked
      std::vector<std::string> checkedBaseUnits;
      std::vector<UnitElement>::const_iterator tmpLUnitsIter;
      for (tmpLUnitsIter = m_units.begin(); tmpLUnitsIter != lUnitsIter; ++tmpLUnitsIter) {
        checkedBaseUnits.push_back(tmpLUnitsIter->first);
      }
      // pick up with lUnits
      for (; lUnitsIter != lUnitsEnd; ++lUnitsIter) {
        rUnitsIter = rUnit.getImpl<detail::Unit_Impl>()->findBaseUnit(lUnitsIter->first);
        if (rUnitsIter != rUnitsEnd) {
          lUnitsIter->second += rUnitsIter->second;
          checkedBaseUnits.push_back(lUnitsIter->first);
        }
      }
      // check rUnits
      auto checkedBaseUnitsEnd = checkedBaseUnits.end();
      for (rUnitsIter = rUnit.getImpl<detail::Unit_Impl>()->m_units.begin(); rUnitsIter != rUnitsEnd; ++rUnitsIter) {
        auto loc = std::find(checkedBaseUnits.begin(), checkedBaseUnitsEnd, rUnitsIter->first);
        if (loc == checkedBaseUnitsEnd) {
          // this base unit not checked yet, and is not in lUnits
          if (rUnitsIter->second != 0) {
            setBaseUnitExponent(rUnitsIter->first, rUnitsIter->second);
          }
        }  // if
      }  // for
    }

    ScaleOpReturnType resultScale = scale() * rUnit.scale();
    setScale(resultScale.first().exponent);

    std::string prettyStringFromFactory = UnitFactory::instance().lookupPrettyString(standardString(false));
    if (!prettyStringFromFactory.empty()) {
      setPrettyString(prettyStringFromFactory);
    } else {
      // otherwise need to handle pretty string
      if (!prettyString(false).empty()) {
        Unit wThisPretty = parseUnitString(prettyString(false));
        if (!rUnit.prettyString(false).empty()) {
          Unit wRUnitPretty = parseUnitString(rUnit.prettyString(false));
          wThisPretty *= wRUnitPretty;
        } else {
          wThisPretty *= rUnit;
        }
        setPrettyString(wThisPretty.standardString(false));
      }
    }
  }

  void Unit_Impl::operator/=(const Unit& rUnit) {
    if ((m_system != rUnit.system()) && (m_system != UnitSystem::Mixed)) {
      LOG_AND_THROW("Cannot " << m_system.valueName() << " /= " << rUnit.system().valueName()
                              << ". Must have same systems, or UnitSystem::Mixed on the left-hand side.");
    }

    // to avoid making any copies, use the identity:
    // this/rUnit = (rUnit/this)^{-1} = (this^{-1} * rUnit)^{-1}
    this->pow(-1);
    (*this) *= rUnit;
    this->pow(-1);
  }

  void Unit_Impl::pow(int expNum, int expDenom, bool okToCallFactory) {
    std::vector<UnitElement>::iterator unitsIter;
    auto unitsEnd = m_units.end();

    // check for errors first
    if (expDenom != 1) {
      // baseUnit exponents ok in integer arithmetic?
      for (unitsIter = m_units.begin(); unitsIter != unitsEnd; ++unitsIter) {
        if ((unitsIter->second * expNum) % expDenom != 0) {
          LOG_AND_THROW("Unit raised to fractional power " << expNum << "/" << expDenom << " that resulted in non-integer exponent on baseUnit "
                                                           << unitsIter->first);
        }
      }

      // scale exponent ok in integer arithmetic?
      if ((scale().exponent * expNum) % expDenom != 0) {
        LOG_AND_THROW("Scale " << scale().name << " cannot be raised to the power " << expNum << "/" << expDenom
                               << " since that results in a fractional exponent.");
      }
    }

    // baseUnit exponents
    for (unitsIter = m_units.begin(); unitsIter != unitsEnd; ++unitsIter) {
      unitsIter->second *= expNum;
      unitsIter->second /= expDenom;
    }

    // scale
    const Scale& sc = scale();
    if (sc.exponent != 0) {
      ScaleOpReturnType resultScale = openstudio::pow(sc, expNum, expDenom);
      const Scale& sc2 = resultScale.first();
      int exponent = sc2.exponent;
      // DLM: check that there are no extra multipliers not accounted for in scale
      if (resultScale.second != 1.0) {
        LOG_AND_THROW("Cannot raise scale '" << sc << "' to the " << expNum << "/" << expDenom << " power.")
      }
      bool test = setScale(exponent);
      OS_ASSERT(test);
    }

    // pretty string
    std::string stdStr = standardString(false);

    std::string prettyStringFromFactory;
    if (okToCallFactory) {
      prettyStringFromFactory = UnitFactory::instance().lookupPrettyString(stdStr);
    }
    if (!prettyStringFromFactory.empty()) {
      setPrettyString(prettyStringFromFactory);
    } else {
      // otherwise handle
      if (!prettyString(false).empty()) {
        Unit wThisPretty = parseUnitString(prettyString(false));
        wThisPretty.pow(expNum, expDenom, okToCallFactory);
        setPrettyString(wThisPretty.standardString(false));
      }
    }  // if
  }

  std::vector<Unit_Impl::UnitElement>::iterator Unit_Impl::findBaseUnit(const std::string& baseUnit) {
    auto result = std::find_if(m_units.begin(), m_units.end(),
                               [&baseUnit](const auto& unitElement) { return firstOfPairEqual<std::string, int>(unitElement, baseUnit); });
    return result;
  }

  std::vector<Unit_Impl::UnitElement>::const_iterator Unit_Impl::findBaseUnit(const std::string& baseUnit) const {
    auto result = std::find_if(m_units.cbegin(), m_units.cend(),
                               [&baseUnit](const auto& unitElement) { return firstOfPairEqual<std::string, int>(unitElement, baseUnit); });
    return result;
  }

}  // namespace detail

Unit::Unit(int scaleExponent, const std::string& prettyString) : m_impl(new detail::Unit_Impl(scaleExponent, prettyString)) {}

Unit::Unit(const std::string& scaleAbbreviation, const std::string& prettyString) : m_impl(new detail::Unit_Impl(scaleAbbreviation, prettyString)) {}

Unit Unit::clone() const {
  return getImpl<detail::Unit_Impl>()->clone();
}

Unit Unit::cloneToMixed() const {
  return getImpl<detail::Unit_Impl>()->cloneToMixed();
}

std::vector<std::string> Unit::baseUnits() const {
  return getImpl<detail::Unit_Impl>()->baseUnits();
}

bool Unit::isBaseUnit(const std::string& baseUnit) const {
  return getImpl<detail::Unit_Impl>()->isBaseUnit(baseUnit);
}

int Unit::baseUnitExponent(const std::string& baseUnit) const {
  return getImpl<detail::Unit_Impl>()->baseUnitExponent(baseUnit);
}

void Unit::setBaseUnitExponent(const std::string& baseUnit, int exponent) {
  getImpl<detail::Unit_Impl>()->setBaseUnitExponent(baseUnit, exponent);
}

Scale Unit::scale() const {
  return getImpl<detail::Unit_Impl>()->scale();
}

bool Unit::setScale(int scaleExponent) {
  return getImpl<detail::Unit_Impl>()->setScale(scaleExponent);
}

bool Unit::setScale(const std::string& scaleAbbreviation) {
  return getImpl<detail::Unit_Impl>()->setScale(scaleAbbreviation);
}

std::string Unit::standardString(bool withScale) const {
  return getImpl<detail::Unit_Impl>()->standardString(withScale);
}

std::string Unit::prettyString(bool withScale) const {
  return getImpl<detail::Unit_Impl>()->prettyString(withScale);
}

void Unit::setPrettyString(const std::string& str) {
  getImpl<detail::Unit_Impl>()->setPrettyString(str);
}

std::string Unit::print(bool withScale) const {
  return getImpl<detail::Unit_Impl>()->print(withScale);
}

UnitSystem Unit::system() const {
  return getImpl<detail::Unit_Impl>()->system();
}

bool Unit::operator==(const Unit& rUnit) const {
  return getImpl<detail::Unit_Impl>()->operator==(rUnit);
}

Unit& Unit::operator*=(const Unit& rUnit) {
  getImpl<detail::Unit_Impl>()->operator*=(rUnit);
  return *this;
}

Unit& Unit::operator/=(const Unit& rUnit) {
  getImpl<detail::Unit_Impl>()->operator/=(rUnit);
  return *this;
}

Unit& Unit::pow(int expNum, int expDenom, bool okToCallFactory) {
  getImpl<detail::Unit_Impl>()->pow(expNum, expDenom, okToCallFactory);
  return *this;
}

/// @cond
Unit::Unit(std::shared_ptr<detail::Unit_Impl> impl) : m_impl(impl) {}
/// @endcond

std::ostream& operator<<(std::ostream& os, const Unit& u) {
  os << u.print();
  return os;
}

Unit parseUnitString(const std::string& unitString) {

  if (!isUnit(unitString)) {
    LOG_FREE_AND_THROW("openstudio.Unit", unitString << " is not a UnitString, per openstudio::isUnit.");
  }

  std::string compoundUnitString(unitString);
  int scaleExponent = 0;
  if (isScaledUnit(unitString)) {
    std::pair<std::string, std::string> scaledUnitParseResult = decomposeScaledUnitString(unitString);
    ScaleConstant scale = ScaleFactory::instance().createScale(scaledUnitParseResult.first);
    if (scale().value != 0.0) {
      scaleExponent = scale().exponent;
    }
    compoundUnitString = scaledUnitParseResult.second;
  }

  Unit result(scaleExponent);

  std::pair<std::vector<std::string>, std::vector<std::string>> compoundUnitParseResult = decomposeCompoundUnitString(compoundUnitString);

  std::vector<std::string>::const_iterator i;
  auto theEnd = compoundUnitParseResult.first.end();

  // loop over numerator
  for (i = compoundUnitParseResult.first.begin(); i != theEnd; ++i) {
    std::pair<std::string, int> atom = decomposeAtomicUnitString(*i);
    if (result.isBaseUnit(atom.first)) {
      result.setBaseUnitExponent(atom.first, result.baseUnitExponent(atom.first) + atom.second);
    } else {
      result.setBaseUnitExponent(atom.first, atom.second);
    }
  }

  // loop over denominator
  theEnd = compoundUnitParseResult.second.end();
  for (i = compoundUnitParseResult.second.begin(); i != theEnd; ++i) {
    std::pair<std::string, int> atom = decomposeAtomicUnitString(*i);
    if (result.isBaseUnit(atom.first)) {
      result.setBaseUnitExponent(atom.first, result.baseUnitExponent(atom.first) - atom.second);
    } else {
      result.setBaseUnitExponent(atom.first, -atom.second);
    }
  }

  return result;
}

bool operator!=(const Unit& lUnit, const Unit& rUnit) {
  return !(lUnit == rUnit);
}

Unit operator*(const Unit& lUnit, const Unit& rUnit) {
  Unit result = lUnit.clone();
  if ((lUnit.system() != rUnit.system()) && (lUnit.system() != UnitSystem::Mixed)) {
    result = lUnit.cloneToMixed();
  }
  result *= rUnit;
  return result;
}

Unit operator/(const Unit& lUnit, const Unit& rUnit) {
  Unit result = lUnit.clone();
  if ((lUnit.system() != rUnit.system()) && (lUnit.system() != UnitSystem::Mixed)) {
    result = lUnit.cloneToMixed();
  }
  result /= rUnit;
  return result;
}

Unit pow(const Unit& rUnit, int expNum, int expDenom) {
  Unit result = rUnit.clone();
  return result.pow(expNum, expDenom);
}

}  // namespace openstudio
