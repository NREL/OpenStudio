/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "IddUnitString.hpp"

#include "Unit.hpp"

#include <boost/regex.hpp>

namespace openstudio {

IddUnitString::IddUnitString(const std::string& s) : m_original(s), m_converted(s) {
  // remove differentiation between kg-H2O and kg-Air (will be kg/kg).
  // better to add kg-H2O and kg-air as separate SI base units?
  m_converted = boost::regex_replace(m_converted, boost::regex("-H2O"), "");
  m_converted = boost::regex_replace(m_converted, boost::regex("-[aA]ir"), "");
  m_converted = boost::regex_replace(m_converted, boost::regex("Water"), "");
  m_converted = boost::regex_replace(m_converted, boost::regex("DryAir"), "");

  // basic replacements
  m_converted = boost::regex_replace(m_converted, boost::regex("-"), "*");

  // If H2O, convert to H_{2}O
  m_converted = boost::regex_replace(m_converted, boost::regex("H2O"), "H_{2}O");

  // Now, replace numbers by exponents, excluding the stuff that's preceding by curly brace `{` or a litteral `^`
  // I use a negative lookbehind for that condition. Standard regex flavor: (?<![{\^])([2-9]+)
  // The entire regex can be tested in regular flavor using this:
  //    "(?<![{\^])([2-9]+)"
  // ft3 => ft^3
  // ft^3 => no match
  // inH_{2}O => no match
  m_converted = boost::regex_replace(m_converted, boost::regex("(?<![\\^\\{])([2-9]+)"), "^\\1");

  m_converted = boost::regex_replace(m_converted, boost::regex("deltaC"), "K");
  m_converted = boost::regex_replace(m_converted, boost::regex("minutes"), "min");
  m_converted = boost::regex_replace(m_converted, boost::regex("dimensionless"), "");
  m_converted = boost::regex_replace(m_converted, boost::regex("Person"), "person");
  m_converted = boost::regex_replace(m_converted, boost::regex("Rotations Per Minute"), "rpm");
  m_converted = boost::regex_replace(m_converted, boost::regex("rev/min"), "rpm");
  m_converted = boost::regex_replace(m_converted, boost::regex("ohms"), "ohm");
  m_converted = boost::regex_replace(m_converted, boost::regex("VA"), "V*A");
  m_converted = boost::regex_replace(m_converted, boost::regex("deltaJ"), "J");
  m_converted = boost::regex_replace(m_converted, boost::regex("rev"), "cycle");
  m_converted = boost::regex_replace(m_converted, boost::regex("Ah"), "A*h");

  // relative temperatures
  if (!boost::regex_match(m_converted, boost::regex("C|F"))) {
    m_converted = boost::regex_replace(m_converted, boost::regex("C"), "K");
    m_converted = boost::regex_replace(m_converted, boost::regex("F"), "R");
  }

  // Multiple /'s
  boost::smatch matches;
  if (boost::regex_match(m_converted, matches, boost::regex("(.*)/\\((.*)/(.*)\\)"))) {
    std::string part1(matches[1].first, matches[1].second);
    std::string part2(matches[2].first, matches[2].second);
    std::string part3(matches[3].first, matches[3].second);
    m_converted = part1 + "*" + part3 + "/" + part2;
  }

  if (boost::regex_match(m_converted, matches, boost::regex("\\((.*)/(.*)\\)/(.*)"))) {
    std::string part1(matches[1].first, matches[1].second);
    std::string part2(matches[2].first, matches[2].second);
    std::string part3(matches[3].first, matches[3].second);
    m_converted = part1 + "/" + part2 + "*" + part3;
  }

  if ((!m_converted.empty()) && boost::regex_search(m_converted, boost::regex("g"))) {
    Unit temp = parseUnitString(m_converted);

    // g -> m(kg)
    int gExp = temp.baseUnitExponent("g");
    if (gExp != 0) {
      temp.setBaseUnitExponent("g", 0);
      temp.setBaseUnitExponent("kg", gExp);
      bool ok = temp.setScale(-3 * gExp);
      if (ok) {
        m_converted = temp.standardString();
      }
    }
  }

  if ((!m_converted.empty()) && boost::regex_search(m_converted, boost::regex("micron"))) {
    Unit temp = parseUnitString(m_converted);

    int gExp = temp.baseUnitExponent("micron");
    if (gExp != 0) {
      temp.setBaseUnitExponent("micron", 0);
      temp.setBaseUnitExponent("m", gExp);
      bool ok = temp.setScale(-6 * gExp);
      if (ok) {
        m_converted = temp.standardString();
      }
    }
  }

  if ((!m_converted.empty()) && boost::regex_search(m_converted, boost::regex("umol"))) {
    Unit temp = parseUnitString(m_converted);

    // g -> m(kg)
    int gExp = temp.baseUnitExponent("umol");
    if (gExp != 0) {
      temp.setBaseUnitExponent("umol", 0);
      temp.setBaseUnitExponent("mol", gExp);
      bool ok = temp.setScale(-6 * gExp);
      if (ok) {
        m_converted = temp.standardString();
      }
    }
  }
}

std::string IddUnitString::toStandardUnitString() const {
  return m_converted;
}

std::string IddUnitString::prettyString() const {
  std::string result;
  if (boost::regex_match(m_original, boost::regex("kg-H2O/kg-[aA]ir"))) {
    result = "kg_{H2O}/kg_{air}";
  }
  return result;
}

}  // namespace openstudio
