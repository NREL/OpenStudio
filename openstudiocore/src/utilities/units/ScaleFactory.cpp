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

#include "../core/Assert.hpp"
#include "ScaleFactory.hpp"

#include "../core/String.hpp"
#include "../core/Exception.hpp"

#include <map>
#include <vector>
#include <boost/regex.hpp>

namespace openstudio{

ScaleFactorySingleton::~ScaleFactorySingleton() {}

bool ScaleFactorySingleton::registerScale(ScaleConstant scale) {

  bool result = m_exponentMap.insert(ExponentLookupMap::value_type(scale().exponent,scale)).second;
  if (!result) {
    LOG(Info,"A scale with exponent " << scale().exponent << " is already registered.");
    return result;
  }

  result = m_abbreviationMap.insert(AbbreviationLookupMap::value_type(scale().abbr,scale)).second;
  if (!result) {
    LOG(Info,"A scale with abbreviation " << scale().abbr << " is already registered.");
  }

  return result;

}

std::vector<Scale> ScaleFactorySingleton::registeredScales() {

  // make vector<Scale> of the correct length
  std::vector<Scale> result(m_exponentMap.size());
  // iterate over each element
  std::vector<Scale>::iterator resultIter;

  ExponentLookupMap::const_iterator mapIter;
  ExponentLookupMap::const_iterator mapEnd = m_exponentMap.end();
  for (mapIter = m_exponentMap.begin(), resultIter = result.begin();
       mapIter != mapEnd; ++mapIter, ++resultIter) {
    *resultIter = mapIter->second(); // copy registered scale into vector
  }

  return result;
}


ScaleConstant ScaleFactorySingleton::createScale(int exponent) const {

  ExponentLookupMap::const_iterator lookupPair;
  lookupPair = m_exponentMap.find(exponent);

  if (lookupPair == m_exponentMap.end()) {
    // fall back method to return something
    return m_recoverFromFailedCreate();
  }

  return lookupPair->second; // return function pointer that encloses const Scale&

}

ScaleConstant ScaleFactorySingleton::createScale(const std::string& abbr) const {

  AbbreviationLookupMap::const_iterator lookupPair;
  lookupPair = m_abbreviationMap.find(abbr);

  if (lookupPair == m_abbreviationMap.end()) {
    // fall back method to return something
    return m_recoverFromFailedCreate();
  }

  return lookupPair->second; // return function pointer that encloses const Scale&

}

ScaleFactorySingleton::ScaleFactorySingleton() {
  // default ScaleFactory initialization
  /** \todo Provide functionality for swapping out entire ScaleFactory library or for
   * complete user customization. */
  registerScale(yotta);
  registerScale(zetta);
  registerScale(exa);
  registerScale(peta);
  registerScale(tera);
  registerScale(giga);
  registerScale(mega);
  registerScale(kilo);
  registerScale(one);
  registerScale(centi);
  registerScale(milli);
  registerScale(micro);
  registerScale(nano);
  registerScale(pico);
  registerScale(femto);
  registerScale(atto);
  registerScale(zepto);
  registerScale(yocto);
}

ScaleConstant ScaleFactorySingleton::m_recoverFromFailedCreate() const {

  // fatal error if no scales are registered since will never get valid scale.
  if (m_exponentMap.size() == 0) {
    LOG_AND_THROW("No scales are registered with ScaleFactory.");
  }

  // otherwise, alert user that scale asked for does not exist
  return notDefined;

}

std::ostream& operator<<(std::ostream& os,const ScaleFactorySingleton& factory) {
  for (const auto & map : factory.m_exponentMap) {
    os << map.second() << std::endl; // output scale and go to next line
  }

  return os;
}

std::string printScales() {
  std::stringstream outStream;
  outStream << ScaleFactory::instance();
  return outStream.str();
}

std::pair<std::string,std::string> extractScaleAbbreviation(const std::string& str) {

  std::pair<std::string,std::string> result;
  std::vector<Scale> scales = ScaleFactory::instance().registeredScales();
  std::string escapeChar("\\");

  // loop through scales and look for regex match
  for (const auto & scale : scales) {
    std::string abbr = scale.abbr;
    if (abbr == "") { continue; }
    std::stringstream regexComposer;
    regexComposer << "^(" ;
    if (abbr.find(escapeChar) == 0) { regexComposer << "\\"; } // add extra escape character for regex
    regexComposer << abbr << ")";
    // LOG_FREE(Debug,"openstudio.ScaleFactory","Scale abbreviation regex = " << regexComposer.str());
    boost::regex re(regexComposer.str());
    boost::match_results<std::string::const_iterator> match;
    if (boost::regex_search(str,match,re)) {
      result.first = scale.abbr;
      result.second = std::string(match[0].second,str.end());
      break;
    }
  }

  return result;
}

ScaleOpReturnType operator*(const Scale& firstScale,const Scale& secondScale) {
  return scaleOpHelperFunction(firstScale.exponent + secondScale.exponent);
}

ScaleOpReturnType operator/(const Scale& firstScale,const Scale& secondScale) {
  return scaleOpHelperFunction(firstScale.exponent - secondScale.exponent);
}

ScaleOpReturnType pow(const Scale& scale,int expNum,int expDenom) {
  return scaleOpHelperFunction((scale.exponent*expNum)/expDenom);
}

ScaleOpReturnType scaleOpHelperFunction(int desiredExponent) {

  ScaleOpReturnType result;
  result.second = 1.0;

  // try to get scale with desired exponent
  result.first = ScaleFactory::instance().createScale(desiredExponent);
  while (result.first().value == 0.0) {
    // if fails, move toward 10^0
    if (desiredExponent > 0) {
      --desiredExponent;
      result.second *= 10.0;
      result.first = ScaleFactory::instance().createScale(desiredExponent);
    }
    else {
      ++desiredExponent;
      result.second /= 10.0;
      result.first = ScaleFactory::instance().createScale(desiredExponent);
    }
  }

  return result;

}

} // openstudio
