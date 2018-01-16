/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "Assert.hpp"
#include "Optional.hpp"
#include "StringHelpers.hpp"
#include "../math/FloatCompare.hpp"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <cmath>
#include <iomanip>
#include <sstream>

namespace openstudio {

std::string toCamelCase(const std::string& s) {
  std::string result = boost::regex_replace(s,boost::regex("[^a-zA-Z0-9]")," ");
  result = boost::regex_replace(result,boost::regex(" ([a-z])"),"\\u$1");
  result = boost::regex_replace(result,boost::regex(" "),"");
  return result;
}

std::string toUpperCamelCase(const std::string& s) {
  std::string result = toCamelCase(s);
  result = boost::regex_replace(result,boost::regex("^([a-z])"),"\\u$1");
  return result;
}

std::string toLowerCamelCase(const std::string& s) {
  std::string result = toCamelCase(s);
  result = boost::regex_replace(result,boost::regex("^([A-Z])"),"\\l$1");
  return result;
}

std::string toUnderscoreCase(const std::string& s) {
  // DLM: previous implementation
  //std::string result = toLowerCamelCase(s);
  //result = boost::regex_replace(result,boost::regex("(.)([A-Z])"),"$1_\\l$2");

  std::string result = boost::replace_all_copy(s, "OpenStudio", "Openstudio");
  boost::replace_all(result, "EnergyPlus", "Energyplus");

  //http://stackoverflow.com/questions/1509915/converting-camel-case-to-underscore-case-in-ruby
  // DLM: there is a to_underscore method in the BCL gem, this should be synchronized
  result = boost::regex_replace(result, boost::regex("[^a-zA-Z0-9]"), " ");
  result = boost::regex_replace(result, boost::regex("([\\-]+)"), "_");
  result = boost::regex_replace(result, boost::regex("([\\s]+)"), "_");
  result = boost::regex_replace(result, boost::regex("([A-Za-z])([0-9])"), "$1_$2");
  result = boost::regex_replace(result, boost::regex("([0-9]+)([A-Za-z])"), "$1_$2");
  result = boost::regex_replace(result, boost::regex("([A-Z]+)([A-Z][a-z])"), "$1_$2");
  result = boost::regex_replace(result, boost::regex("([a-z])([A-Z])"), "$1_$2");
  result = boost::regex_replace(result, boost::regex("([A-Z])"), "\\l$1");
  return result;
}

// ETH@20110614 Implementation copied from IddFileFactoryData.cpp for use in Ruby script.
std::string convertIddName(const std::string& s) {
  std::string result(s);
  boost::trim(result);
  result = boost::regex_replace(result,boost::regex("^100 ?%"),"All");
  result = boost::regex_replace(result,boost::regex("[ \\-]+"),"\\u");
  result = boost::regex_replace(result,boost::regex("[:/,\\(\\)%]+"),"_");
  result = boost::regex_replace(result,boost::regex("[\\.\\?\\]\\[]"),"");
  result = boost::regex_replace(result,boost::regex("="),"_EQUAL_");
  result = boost::regex_replace(result,boost::regex("\\*\\*"),"_POW_");
  result = boost::regex_replace(result,boost::regex("\\+"),"_PLUS_");
  result = boost::regex_replace(result,boost::regex("\\*"),"_TIMES_");
  return result;
}

std::string iddObjectNameToIdfObjectName(const std::string& s) {
  std::string result(s);
  boost::trim(result);
  result = boost::regex_replace(result,boost::regex("OS:"),"");
  result = boost::regex_replace(result,boost::regex(":")," ");
  result = boost::regex_replace(result,boost::regex("([a-z])([A-Z])"),"$1 $2");
  boost::trim(result);
  return result;
}

std::string toNeatString(double value,
                         unsigned numFractionalDigits,
                         bool applyCommas)
{

  std::string result;

  if (std::isinf(value)) {
    if (value > 0) {
      result = "+Inf";
    } else {
      result = "-Inf";
    }
  } else if (std::isnan(value)) {
    result = "NaN";

  } else {

    std::stringstream ss;
    ss << std::fixed << std::setprecision(numFractionalDigits) << value;
    result = ss.str();

    if (applyCommas) {
      boost::smatch m;
      bool ok = boost::regex_match(result,
          m,
          boost::regex("(-?[0-9]{1,3})([0-9]{3})*(\\.[0-9]+|$)"),
          boost::match_extra);
      OS_ASSERT(ok);
      ss.str("");
      ss << std::string(m[1].first,m[1].second);
      for (std::string::const_iterator start = m[1].second; start != m[3].first; ) {
        std::string::const_iterator end = start;
        ++(++(++end));
        ss << "," << std::string(start,end);
        start = end;
      }
      ss << std::string(m[3].first,m[3].second);
      result = ss.str();
    }
  }
  return result;
}

std::string toNeatStringBySigFigs(double value,unsigned numSigFigs,bool applyCommas) {
  return toNeatString(toNumSigFigs(value,numSigFigs),numFractionalDigits(value,numSigFigs),applyCommas);
}

unsigned numFractionalDigits(const std::string& str) {
  unsigned result(0);
  boost::smatch m;
  if (boost::regex_search(str,m,boost::regex("\\.([0-9]+)"))) {
    result = std::string(m[1].first,m[1].second).size();
  }
  return result;
}

unsigned numFractionalDigits(double value,unsigned numSigFigs) {
  if (numSigFigs == 0u) {
    LOG_FREE_AND_THROW("openstudio.core.StringHelpers","Number of significant figures must be > 0.");
  }

  if (equal<double>(value,0.0)) {
    return numSigFigs - 1u;
  }

  value = fabs(value);
  int orderOfMagnitude = int(floor(log10(value))); // 1683 => 3
                                                   // 0.001683892 => -3
  int figsBeforeDecimal = std::min(std::max(orderOfMagnitude + 1,0),int(numSigFigs));
  OS_ASSERT(figsBeforeDecimal >= 0);
  OS_ASSERT(figsBeforeDecimal <= int(numSigFigs));
  int numZerosAfterDecimal = std::max(-1 - orderOfMagnitude,0);
  OS_ASSERT(numZerosAfterDecimal >= 0);
  return numSigFigs - figsBeforeDecimal + numZerosAfterDecimal;
}

std::pair<unsigned,unsigned> numFractionalDigits(const std::vector<double>& values,
                                                 unsigned numSigFigs)
{
  if (numSigFigs == 0u) {
    LOG_FREE_AND_THROW("openstudio.core.StringHelpers","Number of significant figures must be > 0.");
  }

  std::pair<unsigned,unsigned> result(0u,0u);
  for (unsigned i = 0, n = values.size(); i < n; ++i) {
    double value = values[i];
    unsigned numDigits = numFractionalDigits(value,numSigFigs);
    if (i == 0) {
      result = std::pair<unsigned,unsigned>(numDigits,numDigits);
    }
    else {
      if (numDigits < result.first) {
        result.first = numDigits;
      }
      else if (numDigits > result.second) {
        result.second = numDigits;
      }
    }
  }

  return result;
}

double toNumSigFigs(double value, unsigned numSigFigs) {
  if (numSigFigs == 0u) {
    LOG_FREE_AND_THROW("openstudio.core.StringHelpers","Number of significant figures must be > 0.");
  }

  if (equal<double>(value,0.0)) {
    return value;
  }

  double absValue = fabs(value);
  bool negative = (value != absValue);

  double orderOfMagnitude = floor(log10(absValue)); // 1683 => 3
                                                    // 0.001683892 => -3
  //                             X.XXXXX             add more sig-figs
  double positioningPowerOfTen = -orderOfMagnitude + double(int(numSigFigs) - 1);
  // 1683, 2 sig-figs => 1683 * 10**-2 => 16.83
  // 0.001683892, 2 sig-figs => 0.001683892 * 10**4 => 16.83892

  double temp = absValue * pow(10.0,positioningPowerOfTen);
  temp = floor(temp + 0.5); // round doesn't exist in VS2008 at least
  double result = temp * pow(10.0,-positioningPowerOfTen);

  if (negative) {
    return -result;
  }
  return result;
}

boost::regex superscript() {
  //static boost::regex rgx("([\\d\\l\\u])(\\^(?:([\\d\\l\\u]+)|\\{(-?[\\d\\l\\u\\*\\+\\\\\\- ]+)\\}))");
  //return rgx;
  return boost::regex("([\\d\\l\\u])(\\^(?:([\\d\\l\\u]+)|\\{(-?[\\d\\l\\u\\*\\+\\\\\\- ]+)\\}))");
}

boost::regex subscript() {
  //static boost::regex rgx("([\\d\\l\\u])(_(?:([\\d\\l\\u]+)|\\{(-?[\\d\\l\\u\\*\\+\\\\\\- ]+)\\}))");
  //return rgx;
  return boost::regex("([\\d\\l\\u])(_(?:([\\d\\l\\u]+)|\\{(-?[\\d\\l\\u\\*\\+\\\\\\- ]+)\\}))");
}

boost::regex underscore() {
  //static boost::regex rgx("(\\\\_)");
  //return rgx;
  return boost::regex("(\\\\_)");
}

std::string formatText(const std::string& str, DocumentFormat fmt) {
  std::string wStr = formatSuperAndSubscripts(str, fmt);
  if (fmt == DocumentFormat::LaTeX) {
    wStr = boost::regex_replace(wStr, boost::regex("%"), "\\\\$&");
    wStr = boost::regex_replace(wStr, boost::regex("&"), "\\\\$&");
  }
  if (fmt == DocumentFormat::XHTML) {
    wStr = boost::regex_replace(wStr, boost::regex("$"), "<br/>");
  }
  return wStr;
}

std::string formatSuperAndSubscripts(const std::string& str, DocumentFormat fmt) {
  if (fmt == DocumentFormat::LaTeX) {
    std::string wStr(str);
    wStr = boost::regex_replace(wStr, superscript(), "$1\\\\ensuremath{$2}");
    wStr = boost::regex_replace(wStr, subscript(), "$1\\\\ensuremath{$2}");
    return wStr;
  }
  if (fmt == DocumentFormat::XHTML) {
    std::string wStr(str);
    wStr = boost::regex_replace(wStr, superscript(), "$1<sup>$3$4</sup>");
    wStr = boost::regex_replace(wStr, subscript(), "$1<sub>$3$4</sub>");
    wStr = boost::regex_replace(wStr, underscore(), "_");
    return wStr;
  }
  return str;
}

std::string formatUnitString(const std::string& str, DocumentFormat fmt) {
  std::string result = formatSuperAndSubscripts(str, fmt);
  boost::regex asterix("\\*");
  if (fmt == DocumentFormat::LaTeX) {
    result = boost::regex_replace(result, asterix, "\\\\ensuremath{\\\\cdot}");
  }
  if (fmt == DocumentFormat::XHTML) {
    result = boost::regex_replace(result, asterix, "&middot;");
  }
  return result;
}

std::string formatUnderscore(const std::string& str) {
  std::string wStr(str);
  wStr = boost::regex_replace(wStr, boost::regex("_"), "\\\\_");
  return wStr;
}

std::vector <std::string> splitString(const std::string &string, char delimiter)
{
  std::vector<std::string> results;
  if(!string.empty()) { // Only do work if there is work to do
    std::stringstream stream(string);
    std::string substring;
    while(std::getline(stream, substring, delimiter)) { // Loop and fill the results vector
      results.push_back(substring);
    }
    if(*(string.end() - 1) == ',') { // Add an empty string if the last char is the delimiter
      results.push_back(std::string());
    }
  }
  return results;
}

} // openstudio
