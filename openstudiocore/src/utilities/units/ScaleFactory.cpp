/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
  registerScale(hectokilo);
  registerScale(myria);
  registerScale(kilo);
  registerScale(hecto);
  registerScale(deka);
  registerScale(one);
  registerScale(deci);
  registerScale(centi);
  registerScale(milli);
  registerScale(decimilli);
  registerScale(centimilli);
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
  if (m_exponentMap.empty()) {
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
