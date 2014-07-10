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

#include "QuantityRegex.hpp"
#include "../core/Logger.hpp"

#include <sstream>

namespace openstudio {

const boost::regex& regexFixedPrecisionValue() {
  static boost::regex rgx("-?[[:digit:]]*\\.?[[:digit:]]+");
  return rgx;
}

const boost::regex& regexEmbeddedFixedPrecisionValue() {
  std::stringstream regexComposer;
  regexComposer << "(?:^| )(" << regexFixedPrecisionValue().str() << ")(?:$| |\\.$|\\. |,|;)";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

bool isFixedPrecisionValue(const std::string& s) {
  return boost::regex_match(s,regexFixedPrecisionValue());
}

bool containsFixedPrecisionValue(const std::string& s) {
  return boost::regex_search(s,regexEmbeddedFixedPrecisionValue());
}


const boost::regex& regexScientificNotationValue() {
  static boost::regex rgx("-?[[:digit:]]*[.]?[[:digit:]]+[EDed][-\\+]?[[:digit:]]+");
  return rgx;
}

const boost::regex& regexEmbeddedScientificNotationValue() {
  std::stringstream regexComposer;
  regexComposer << "(?:^| )(" << regexScientificNotationValue().str() << ")(?:$| |\\.$|\\. |,|;)";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

bool isScientificNotationValue(const std::string& s) {
  return boost::regex_match(s,regexScientificNotationValue());
}

bool containsScientificNotationValue(const std::string& s) {
  return boost::regex_search(s,regexEmbeddedScientificNotationValue());
}


const boost::regex& regexBaseUnit() {
  // ETH@20120320 It doesn't seem like the last [\\l\\u]{0,10} should be necessary, but now
  // is not a good time to try removing it.
  // ETH@20120320 Should also tighten up to require {} if > 1 character in denominator.
  // ETH@20120711 Trying to address both of the issues above.
  static boost::regex rgx("\\\\?[\\l\\u]{1,8}(?:_(?:[\\l\\u\\d]|\\{[\\l\\u\\d]{1,10}\\}))?[\\l\\u]{0,2}");
  return rgx;
}

const boost::regex& regexExponent() {
  static boost::regex rgx("-?[[:digit:]]+");
  return rgx;
}

const boost::regex& regexAtomicUnit() {
  std::stringstream regexComposer;
  regexComposer << regexBaseUnit().str() << "(?:\\^(?:[[:digit:]]+|\\{-[[:digit:]]+\\}))?";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

const boost::regex& regexEmbeddedAtomicUnit() {
  std::stringstream regexComposer;
  regexComposer << "(?:^| |\\*|/|\\()(" << regexAtomicUnit().str() << ")(?:$| |\\*|/|\\))";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

bool isAtomicUnit(const std::string& s) {
  return boost::regex_match(s,regexAtomicUnit());
}

bool containsAtomicUnit(const std::string& s) {
  return boost::regex_search(s,regexEmbeddedAtomicUnit());
}


const boost::regex& regexCompoundUnit() {
  // ETH@20120320 Somehow a quantity (0.12 kg) is getting by this regex. Now is not a good
  // time to try to fix.
  std::stringstream regexComposer, tempComposer;
  // place atomicUnit*atomicUnit*... in tempComposer
  tempComposer << regexAtomicUnit().str() << "(?:\\*" << regexAtomicUnit().str() << ")*";
  // possibilities: tC.str(), tC.str()/tC.str(), 1/tC.str().
  regexComposer << "(?:(?:" << tempComposer.str() << "|1)/)?" << tempComposer.str();
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

const boost::regex& regexEmbeddedCompoundUnit() {
  std::stringstream regexComposer;
  // can be surrounded by ^$ ().
  regexComposer << "(?:^| |\\()(" << regexCompoundUnit().str() << ")(?:$| |\\)|\\.$|\\. |,|;)";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

bool isCompoundUnit(const std::string& s) {
  return boost::regex_match(s,regexCompoundUnit());
}

bool containsCompoundUnit(const std::string& s) {
  return boost::regex_search(s,regexEmbeddedCompoundUnit());
}


const boost::regex& regexScaledUnit() {
  std::stringstream regexComposer;
  regexComposer << "\\\\?[\\l\\u]{1,5}\\(" << regexCompoundUnit().str() << "\\)";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

const boost::regex& regexEmbeddedScaledUnit() {
  std::stringstream regexComposer;
  regexComposer << "(?:^| )(" << regexScaledUnit().str() << ")(?:$| |\\.$|\\. |,|;)";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

bool isScaledUnit(const std::string& s) {
  return boost::regex_match(s,regexScaledUnit());
}

bool containsScaledUnit(const std::string& s) {
  return boost::regex_search(s,regexEmbeddedScaledUnit());
}


const boost::regex& regexDirectScaledUnit() {
  std::stringstream regexComposer, tempComposer;
  // place atomicUnit*atomicUnit*... in tempComposer
  tempComposer << regexAtomicUnit().str() << "(?:\\*" << regexAtomicUnit().str() << ")*";
  regexComposer << "((?:" << tempComposer.str() << "|1)/)"
                << " ?(10+)(?:(?: *|\\*)(" << tempComposer.str() << "))?";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

const boost::regex& regexEmbeddedDirectScaledUnit() {
  std::stringstream regexComposer;
  regexComposer << "(?:(?:^| )(" << regexDirectScaledUnit().str() << ")(?:$| |\\.$|\\. |,|;)|"
                       << "\\((" << regexDirectScaledUnit().str() << ")\\)|" 
                       << "\\{(" << regexDirectScaledUnit().str() << ")\\}|"
                       << "\\[(" << regexDirectScaledUnit().str() << ")\\])";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

std::pair<std::string,std::pair<unsigned,std::string> > 
decomposeDirectScaledUnit(const std::string& s) {
  if (!isDirectScaledUnit(s)) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex","Cannot decompose " << s
      << " into a numerator and scaled denominator because it is not an direct scaled unit.");
  }
  std::pair<std::string,std::pair<unsigned,std::string> > result;
  boost::match_results<std::string::const_iterator> match;

  boost::regex_search(s,match,regexDirectScaledUnit());
  result.first = std::string(match[1].first,match[1].second);
  std::string powerOfTen(match[2].first,match[2].second);
  result.second.first = powerOfTen.size() - 1;
  result.second.second = std::string(match[3].first,match[3].second);

  return result;
}

bool isDirectScaledUnit(const std::string& s) {
  return boost::regex_match(s,regexDirectScaledUnit());
}

bool containsDirectScaledUnit(const std::string& s) {
  return boost::regex_search(s,regexEmbeddedDirectScaledUnit());
}


const boost::regex& regexUnit() {
  std::stringstream regexComposer;
  regexComposer << "(" << regexCompoundUnit().str() << "|" << regexScaledUnit().str() << ")";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

const boost::regex& regexEmbeddedUnit() {
  std::stringstream regexComposer;
  regexComposer << "(?:(?:^| )" << regexUnit().str() << "(?:$| |\\.$|\\. |,|;)|"
                       << "\\(" << regexUnit().str() << "\\)|" 
                       << "\\{" << regexUnit().str() << "\\}|"
                       << "\\[" << regexUnit().str() << "\\])";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

bool isUnit(const std::string& s) {
  return boost::regex_match(s,regexUnit());
}

bool containsUnit(const std::string& s) {
  return boost::regex_search(s,regexEmbeddedUnit());
}


const boost::regex& regexQuantity() {
  std::stringstream regexComposer;
  regexComposer << "(" << regexFixedPrecisionValue().str() << "|"
    << regexScientificNotationValue().str() << ")( |/)" << regexUnit().str();
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

const boost::regex& regexEmbeddedQuantity() {
  std::stringstream regexComposer;
  regexComposer << "(?:^| )(" << regexQuantity().str() << ")(?:$| |\\.$|\\. |,|;)";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

bool isQuantity(const std::string& s) {
  return boost::regex_match(s,regexQuantity());
}

bool containsQuantity(const std::string& s) {
  return boost::regex_search(s,regexEmbeddedQuantity());
}

std::pair<std::string,std::string> decomposeQuantityString(const std::string& s) {

  std::pair<std::string,std::string> result;
  boost::match_results<std::string::const_iterator> match;
  if (!boost::regex_match(s,match,regexQuantity())) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex","Cannot decompose " << s
      << " into (value,unit) because it is not a quantity.");
  }
  // pull out number
  result.first = std::string(match[1].first,match[1].second);
  // see if there is / between number and unit
  std::string temp(match[2].first,match[2].second);
  if (temp == "/") {
    result.second = "1/";
  }
  // pull out unit
  result.second += std::string(match[3].first,match[3].second);

  return result;

}

std::pair<std::string,std::string> decomposeScaledUnitString(const std::string& s) {
  if (!isScaledUnit(s)) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex","Cannot decompose " << s
      << " into a scale and a compound unit because it is not a scaled unit.");
  }

  std::pair<std::string,std::string> result;
  boost::match_results<std::string::const_iterator> match;
  boost::regex scaleRegex("(\\\\?[\\l\\u]{1,5})\\(");
  // pull out scale
  if (!boost::regex_search(s,match,scaleRegex,boost::match_continuous)) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex","Could not extract a scale from the scaled unit "
      << s << ".");
  }
  result.first = std::string(match[1].first,match[1].second);

  // pull out compound unit
  if (!boost::regex_search(s,match,regexEmbeddedCompoundUnit())) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex","Could not extract a compound unit from " << s << ".");
  }
  int i = 1;
  while (!match[i].matched) {++i;}
  result.second = std::string(match[1].first,match[1].second);

  return result;
}

std::pair< std::vector<std::string>,std::vector<std::string> > decomposeCompoundUnitString(
    const std::string& s) {

  if (!isCompoundUnit(s)) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex","Cannot decompose " << s
      << " into AtomicUnits in the numerator and denominator because it is not a compound unit.");
  }
  std::pair< std::vector<std::string>,std::vector<std::string> > result;
  boost::match_results<std::string::const_iterator> match;

  std::string ws(s);

  // remove leading 1, if applicable
  std::string::iterator firstCharEnd = ws.begin(); ++firstCharEnd;
  std::string::const_iterator tempStrConstIter; // for gcc
  std::string firstChar(ws.begin(),firstCharEnd);
  if (firstChar == "1") {
    ws = std::string(firstCharEnd,ws.end());
    firstCharEnd = ws.begin(); ++firstCharEnd;
    firstChar = std::string(ws.begin(),firstCharEnd);
  }
  while (!ws.empty() && (firstChar != "/")) {
    // populate numerator
    boost::regex_search(ws,match,regexAtomicUnit());
    result.first.push_back(std::string(match[0].first,match[0].second));
    tempStrConstIter = ws.end(); // for gcc
    ws = std::string(match[0].second,tempStrConstIter);
    if (!ws.empty()) {
      firstCharEnd = ws.begin(); ++firstCharEnd;
      firstChar = std::string(ws.begin(),firstCharEnd);
    }
  }
  if (firstChar == "/") {
    ws = std::string(firstCharEnd,ws.end());
    while (!ws.empty()) {
      // populate denominator
      boost::regex_search(ws,match,regexAtomicUnit());
      result.second.push_back(std::string(match[0].first,match[0].second));
      tempStrConstIter = ws.end(); // for gcc
      ws = std::string(match[0].second,tempStrConstIter);
    }
  }

  return result;

}

std::pair<std::string,int> decomposeAtomicUnitString(const std::string& s) {

  if (!isAtomicUnit(s)) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex","Cannot decompose " << s
      << " into a base unit and exponent because it is not an atomic unit.");
  }
  std::pair<std::string,int> result;
  boost::smatch match;

  boost::regex_search(s,match,regexBaseUnit());
  result.first = std::string(match[0].first,match[0].second);
  if (boost::regex_search(s,match,regexExponent())) {
    std::istringstream iss(std::string(match[0].first,match[0].second));
    iss >> result.second;
  }
  else {
    result.second = 1;
  }

  return result;
}

}
