/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "QuantityRegex.hpp"
#include "../core/Logger.hpp"

#include <sstream>

namespace openstudio {

// JM@2017-12-14:
// Note from Julien Marrec: I heavily commented this file in December 2017,
// in the hope that it will make any debugging needed down the road a lot smoother
// Beware that the "standard flavor" equivalents are valid at the time I am writing this
// Especially for case where I put the "full" equivalent that adds the bit from another function returning a regex,
// if the referenced function is changed, then it'll no longer be valid

const boost::regex& regexFixedPrecisionValue() {
  // Equivalent to "-?\d*\.?\d+" in standard flavor (PRCE, python or JS should work)
  // (optional minus sign, then digits with potentially a decimal point)
  // Matches stuff like "10" "1.012", "-.12"
  static boost::regex rgx("-?[[:digit:]]*\\.?[[:digit:]]+");
  return rgx;
}

const boost::regex& regexEmbeddedFixedPrecisionValue() {
  std::stringstream regexComposer;
  // First bit is a non capturing group to ensure the regexFixedPrecisionValue will be found on the beginning of the string
  // (or with anything but whitespace)
  // Then regexFixedPrecisionValue matches a number (int or float), which is the only capturing group here
  // Last bit checks it's the end of the string, or there's a space, a "." then end, a dot then space, a comma, a semi colon
  // regexComposer in standard flavor is "(?:^| )(-?\d*\.?\d+)(?:$| |\.$|\. |,|;)"
  regexComposer << "(?:^| )(" << regexFixedPrecisionValue().str() << ")(?:$| |\\.$|\\. |,|;)";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

bool isFixedPrecisionValue(const std::string& s) {
  return boost::regex_match(s, regexFixedPrecisionValue());
}

bool containsFixedPrecisionValue(const std::string& s) {
  return boost::regex_search(s, regexEmbeddedFixedPrecisionValue());
}

const boost::regex& regexScientificNotationValue() {
  // Equivalent to "-?\d*[.]?\d+[EDed][-\+]?\d+" in standard flavor
  // will match: "-10.5e5", "10d+5", ".1E10"
  // Will not match: 1e0.5 (will actually match 1e0 here...)
  static boost::regex rgx("-?[[:digit:]]*[.]?[[:digit:]]+[EDed][-\\+]?[[:digit:]]+");
  return rgx;
}

const boost::regex& regexEmbeddedScientificNotationValue() {
  std::stringstream regexComposer;
  // Same idea as in regexEmbeddedFixedPrecisionValue
  regexComposer << "(?:^| )(" << regexScientificNotationValue().str() << ")(?:$| |\\.$|\\. |,|;)";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

bool isScientificNotationValue(const std::string& s) {
  return boost::regex_match(s, regexScientificNotationValue());
}

bool containsScientificNotationValue(const std::string& s) {
  return boost::regex_search(s, regexEmbeddedScientificNotationValue());
}

const boost::regex& regexBaseUnit() {
  // ETH@20120320 It doesn't seem like the last [\\l\\u]{0,10} should not be necessary, but now
  // is not a good time to try removing it.
  // ETH@20120320 Should also tighten up to require {} if > 1 character in denominator.
  // ETH@20120711 Trying to address both of the issues above.
  /* JM@2017-12-14:
   * \\l => [[:lower:]] => any lower case character <=> [a-z]
   * \\u => [[:upper::]] => any upper case character <=> [A-Z]
   * \\d => [[:digit:]] => any digit <=> [0-9]
   * \\$ => litteral dollar sign?
   *  Eg: [\\l\\u\\d\\$] <=> [a-zA-Z0-9\$] in a PRCE flavor
   *
   *
   * (?:...) is a non-capturing group
   * Let's decompose this regex:
   * Optionally, the string can start with a '\', then
   * First bit looks for [a-zA-Z\$], between 1 to 8 times
   * Second bit (Optional non capturing groups), looks for either _[a-zA-Z0-9\$] or  _\{[a-zA-Z0-9\$]{1,10}\}
   *    This means that "_2" will be match, "_{2}", "_{24}" but not "2", nor "_24".
   *    It can NOT be present too
   * Last bit looks for optional [a-zA-Z\$], between 0 and 2 times
   *
   * The entire regex can be tested in regular flavor using this:
   *    "\\?[a-zA-Z\$]{1,8}(?:_(?:[a-zA-Z0-9\$]|\{[a-zA-Z0-9\$]{1,10}\}))?[a-zA-Z\$]{0,2}"
   *
   * Examples:
   * Matches in full: "ft_{2}O", "ftH_2O", "ftH_{24}O" "ft", "lux"
   * Matches only the "m" in the following: m^3, m^-{-1}
   * Doesn't match: "98", "ftH_24O"
   * (Warning: will match "ftH_2" and "O" in "ft_H24O" if global flag in search)
   */
  static boost::regex rgx("\\\\?[\\l\\u\\$]{1,8}(?:_(?:[\\l\\u\\d\\$]|\\{[\\l\\u\\d\\$]{1,10}\\}))?[\\l\\u\\$]{0,2}");
  return rgx;
}

const boost::regex& regexExponent() {
  // This regex looks for "^" (litteral), optional "{", captures (optional "-"  \d+) and finally optional "}"
  // The actual exponent is in the only capturing group, so match[1] is the exponent
  // Standard flavor equivalent: "(?:\^\{?)(-?\d+)(?:\})?"
  static boost::regex rgx("(?:\\^\\{?)(-?[[:digit:]]+)(?:\\})?");

  return rgx;
}

const boost::regex& regexAtomicUnit() {
  std::stringstream regexComposer;
  // The portion written here (without regexBaseUnit) is equivalent to "(?:\^(?:\d+|\{-\d+\}))?"
  // Meaning it will match (but not capture) stuff like "^3" or "^{-3}" but not "^{3}", and its optional
  // so Overall the "m" is matched in "m", "m^3", "m^{-1}"
  //
  // Valid until anything above is changed: the full one is equivalent to
  // "\\?[a-zA-Z\$]{1,8}(?:_(?:[a-zA-Z0-9\$]|\{[a-zA-Z0-9\$]{1,10}\}))?[a-zA-Z\$]{0,2}(?:\^(?:\d+|\{-\d+\}))?"
  //
  // Currently because the exponent portion here is optional, it'll match anything that matches regexBaseUnit,
  regexComposer << regexBaseUnit().str() << "(?:\\^(?:[[:digit:]]+|\\{-[[:digit:]]+\\}))?";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

const boost::regex& regexEmbeddedAtomicUnit() {
  std::stringstream regexComposer;
  // First bit checks that it's the beggining of the string, or a space is before, or litteral "*", "/" or "("
  // then regexAtomicUnit
  // then ensures it's the end of the string, a space, or a litteral "*", "/" or ")"
  regexComposer << "(?:^| |\\*|/|\\()(" << regexAtomicUnit().str() << ")(?:$| |\\*|/|\\))";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

bool isAtomicUnit(const std::string& s) {
  return boost::regex_match(s, regexAtomicUnit());
}

bool containsAtomicUnit(const std::string& s) {
  return boost::regex_search(s, regexEmbeddedAtomicUnit());
}

const boost::regex& regexCompoundUnit() {
  // ETH@20120320 Somehow a quantity (0.12 kg) is getting by this regex. Now is not a good
  // time to try to fix.
  // JM@2017-12-14: Not fixing now, but this is because all of the groups are optional ("?"), except [a-zA-Z\$]{1,8} (from regexBaseUnit)
  std::stringstream regexComposer;
  std::stringstream tempComposer;
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
  return boost::regex_match(s, regexCompoundUnit());
}

bool containsCompoundUnit(const std::string& s) {
  return boost::regex_search(s, regexEmbeddedCompoundUnit());
}

const boost::regex& regexScaledUnit() {
  std::stringstream regexComposer;
  // "\\?[a-zA-Z\$]{1,5}" << regexCompoundUnit << "\)" in standard flavor
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
  return boost::regex_match(s, regexScaledUnit());
}

bool containsScaledUnit(const std::string& s) {
  return boost::regex_search(s, regexEmbeddedScaledUnit());
}

const boost::regex& regexDirectScaledUnit() {
  std::stringstream regexComposer;
  std::stringstream tempComposer;
  // place atomicUnit*atomicUnit*... in tempComposer
  tempComposer << regexAtomicUnit().str() << "(?:\\*" << regexAtomicUnit().str() << ")*";
  regexComposer << "((?:" << tempComposer.str() << "|1)/)" << " ?(10+)(?:(?: *|\\*)(" << tempComposer.str() << "))?";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

const boost::regex& regexEmbeddedDirectScaledUnit() {
  std::stringstream regexComposer;
  regexComposer << "(?:(?:^| )(" << regexDirectScaledUnit().str() << ")(?:$| |\\.$|\\. |,|;)|" << "\\((" << regexDirectScaledUnit().str() << ")\\)|"
                << "\\{(" << regexDirectScaledUnit().str() << ")\\}|" << "\\[(" << regexDirectScaledUnit().str() << ")\\])";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

std::pair<std::string, std::pair<unsigned, std::string>> decomposeDirectScaledUnit(const std::string& s) {
  if (!isDirectScaledUnit(s)) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex",
                       "Cannot decompose " << s << " into a numerator and scaled denominator because it is not an direct scaled unit.");
  }
  std::pair<std::string, std::pair<unsigned, std::string>> result;
  boost::match_results<std::string::const_iterator> match;

  boost::regex_search(s, match, regexDirectScaledUnit());
  result.first = std::string(match[1].first, match[1].second);
  std::string powerOfTen(match[2].first, match[2].second);
  result.second.first = powerOfTen.size() - 1;
  result.second.second = std::string(match[3].first, match[3].second);

  return result;
}

bool isDirectScaledUnit(const std::string& s) {
  return boost::regex_match(s, regexDirectScaledUnit());
}

bool containsDirectScaledUnit(const std::string& s) {
  return boost::regex_search(s, regexEmbeddedDirectScaledUnit());
}

const boost::regex& regexUnit() {
  std::stringstream regexComposer;
  regexComposer << "(" << regexCompoundUnit().str() << "|" << regexScaledUnit().str() << ")";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

const boost::regex& regexEmbeddedUnit() {
  std::stringstream regexComposer;
  regexComposer << "(?:(?:^| )" << regexUnit().str() << "(?:$| |\\.$|\\. |,|;)|" << "\\(" << regexUnit().str() << "\\)|" << "\\{" << regexUnit().str()
                << "\\}|" << "\\[" << regexUnit().str() << "\\])";
  static boost::regex rgx(regexComposer.str());
  return rgx;
}

bool isUnit(const std::string& s) {
  return boost::regex_match(s, regexUnit());
}

bool containsUnit(const std::string& s) {
  return boost::regex_search(s, regexEmbeddedUnit());
}

const boost::regex& regexQuantity() {
  std::stringstream regexComposer;
  regexComposer << "(" << regexFixedPrecisionValue().str() << "|" << regexScientificNotationValue().str() << ")( |/)" << regexUnit().str();
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
  return boost::regex_match(s, regexQuantity());
}

bool containsQuantity(const std::string& s) {
  return boost::regex_search(s, regexEmbeddedQuantity());
}

std::pair<std::string, std::string> decomposeQuantityString(const std::string& s) {

  std::pair<std::string, std::string> result;
  boost::match_results<std::string::const_iterator> match;
  if (!boost::regex_match(s, match, regexQuantity())) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex", "Cannot decompose " << s << " into (value,unit) because it is not a quantity.");
  }
  // pull out number
  result.first = std::string(match[1].first, match[1].second);
  // see if there is / between number and unit
  std::string temp(match[2].first, match[2].second);
  if (temp == "/") {
    result.second = "1/";
  }
  // pull out unit
  result.second += std::string(match[3].first, match[3].second);

  return result;
}

std::pair<std::string, std::string> decomposeScaledUnitString(const std::string& s) {
  if (!isScaledUnit(s)) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex", "Cannot decompose " << s << " into a scale and a compound unit because it is not a scaled unit.");
  }

  std::pair<std::string, std::string> result;
  boost::match_results<std::string::const_iterator> match;
  boost::regex scaleRegex("(\\\\?[\\l\\u]{1,5})\\(");
  // pull out scale
  if (!boost::regex_search(s, match, scaleRegex, boost::match_continuous)) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex", "Could not extract a scale from the scaled unit " << s << ".");
  }
  result.first = std::string(match[1].first, match[1].second);

  // pull out compound unit
  if (!boost::regex_search(s, match, regexEmbeddedCompoundUnit())) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex", "Could not extract a compound unit from " << s << ".");
  }
  int i = 1;
  while (!match[i].matched) {
    ++i;
  }
  result.second = std::string(match[1].first, match[1].second);

  return result;
}

std::pair<std::vector<std::string>, std::vector<std::string>> decomposeCompoundUnitString(const std::string& s) {

  if (!isCompoundUnit(s)) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex",
                       "Cannot decompose " << s << " into AtomicUnits in the numerator and denominator because it is not a compound unit.");
  }
  std::pair<std::vector<std::string>, std::vector<std::string>> result;
  boost::match_results<std::string::const_iterator> match;

  std::string ws(s);

  // remove leading 1, if applicable
  std::string::iterator firstCharEnd = ws.begin();
  ++firstCharEnd;
  std::string::const_iterator tempStrConstIter;  // for gcc
  std::string firstChar(ws.begin(), firstCharEnd);
  if (firstChar == "1") {
    ws = std::string(firstCharEnd, ws.end());
    firstCharEnd = ws.begin();
    ++firstCharEnd;
    firstChar = std::string(ws.begin(), firstCharEnd);
  }
  while (!ws.empty() && (firstChar != "/")) {
    // populate numerator
    boost::regex_search(ws, match, regexAtomicUnit());
    result.first.push_back(std::string(match[0].first, match[0].second));
    tempStrConstIter = ws.end();  // for gcc
    // cppcheck-suppress invalidContainer
    ws = std::string(match[0].second, tempStrConstIter);
    if (!ws.empty()) {
      firstCharEnd = ws.begin();
      ++firstCharEnd;
      firstChar = std::string(ws.begin(), firstCharEnd);
    }
  }
  if (firstChar == "/") {
    // cppcheck-suppress invalidContainer
    ws = std::string(firstCharEnd, ws.end());
    while (!ws.empty()) {
      // populate denominator
      boost::regex_search(ws, match, regexAtomicUnit());
      result.second.push_back(std::string(match[0].first, match[0].second));
      tempStrConstIter = ws.end();  // for gcc
      // cppcheck-suppress invalidContainer
      ws = std::string(match[0].second, tempStrConstIter);
    }
  }

  return result;
}

std::pair<std::string, int> decomposeAtomicUnitString(const std::string& s) {

  if (!isAtomicUnit(s)) {
    LOG_FREE_AND_THROW("openstudio.QuantityRegex", "Cannot decompose " << s << " into a base unit and exponent because it is not an atomic unit.");
  }

  std::pair<std::string, int> result;
  boost::smatch match;

  boost::regex_search(s, match, regexBaseUnit());

  // First is the baseUnit (match[0].first is the start of sequence that matched, match[0].second is the end of sequence)
  result.first = std::string(match[0].first, match[0].second);
  // Match an exponent and put that in result.second
  if (boost::regex_search(s, match, regexExponent())) {
    // We want the submatch (capturing group)
    std::istringstream iss(match[1]);
    iss >> result.second;
  } else {
    result.second = 1;
  }

  return result;
}

}  // namespace openstudio
