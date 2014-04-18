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

#include <utilities/units/IddUnitString.hpp>

#include <utilities/units/Unit.hpp>

#include <boost/regex.hpp>

namespace openstudio {

IddUnitString::IddUnitString (const std::string &s)
  : m_original(s), m_converted(s)
{
  // remove differentiation between kg-H2O and kg-Air (will be kg/kg).
  // better to add kg-H2O and kg-air as separate SI base units?
  m_converted = boost::regex_replace(m_converted,boost::regex("-H2O"),"");
  m_converted = boost::regex_replace(m_converted,boost::regex("-[aA]ir"),"");
  m_converted = boost::regex_replace(m_converted,boost::regex("Water"),"");
  m_converted = boost::regex_replace(m_converted,boost::regex("DryAir"),"");

  // basic replacements
  m_converted = boost::regex_replace(m_converted,boost::regex("-"),"*");
  if (!boost::regex_search(m_converted,boost::regex("H2O"))) {
    m_converted = boost::regex_replace(m_converted,boost::regex("([2-9]+)"),"^\\1");
  }
  else {
    m_converted = boost::regex_replace(m_converted,boost::regex("H2O"),"H_{2}O");
  }
  m_converted = boost::regex_replace(m_converted,boost::regex("deltaC"),"K");
  m_converted = boost::regex_replace(m_converted,boost::regex("minutes"),"min");
  m_converted = boost::regex_replace(m_converted,boost::regex("dimensionless"),"");
  m_converted = boost::regex_replace(m_converted,boost::regex("Person"),"person");
  m_converted = boost::regex_replace(m_converted,boost::regex("Rotations Per Minute"),"rpm");
  m_converted = boost::regex_replace(m_converted,boost::regex("ohms"),"ohm");
  m_converted = boost::regex_replace(m_converted,boost::regex("VA"),"V*A");
  m_converted = boost::regex_replace(m_converted,boost::regex("deltaJ"),"J");
  m_converted = boost::regex_replace(m_converted,boost::regex("rev"),"cycle");
  m_converted = boost::regex_replace(m_converted,boost::regex("Ah"),"A*h");

  // relative temperatures
  if (!boost::regex_match(m_converted,boost::regex("C|F"))) {
    m_converted = boost::regex_replace(m_converted,boost::regex("C"),"K");
    m_converted = boost::regex_replace(m_converted,boost::regex("F"),"R");
  }

  // Multiple /'s
  boost::smatch matches;
  if (boost::regex_match(m_converted,matches,boost::regex("(.*)/\\((.*)/(.*)\\)"))) {
    std::string part1(matches[1].first, matches[1].second);
    std::string part2(matches[2].first, matches[2].second);
    std::string part3(matches[3].first, matches[3].second);
    m_converted = part1 + "*" + part3 + "/" + part2;
  }

  if (boost::regex_match(m_converted,matches,boost::regex("\\((.*)/(.*)\\)/(.*)"))) {
    std::string part1(matches[1].first, matches[1].second);
    std::string part2(matches[2].first, matches[2].second);
    std::string part3(matches[3].first, matches[3].second);
    m_converted = part1 + "/" + part2 + "*" + part3;
  }

  if ((!m_converted.empty()) && boost::regex_search(m_converted,boost::regex("g"))) {
    Unit temp = parseUnitString(m_converted);

    // g -> m(kg)
    int gExp = temp.baseUnitExponent("g");
    if (gExp != 0) {
      temp.setBaseUnitExponent("g",0);
      temp.setBaseUnitExponent("kg",gExp);
      bool ok = temp.setScale(-3 * gExp);
      if (ok) {
        m_converted = temp.standardString();
      }
    }
  }

  if ((!m_converted.empty()) && boost::regex_search(m_converted,boost::regex("micron"))) {
    Unit temp = parseUnitString(m_converted);

    int gExp = temp.baseUnitExponent("micron");
    if (gExp != 0) {
      temp.setBaseUnitExponent("micron",0);
      temp.setBaseUnitExponent("m",gExp);
      bool ok = temp.setScale(-6 * gExp);
      if (ok) {
        m_converted = temp.standardString();
      }
    }
  }
}

IddUnitString::~IddUnitString ()
{}

std::string IddUnitString::toStandardUnitString () {
  return m_converted;
}

std::string IddUnitString::prettyString() {
  std::string result;
  if (boost::regex_match(m_original,boost::regex("kg-H2O/kg-[aA]ir"))) {
    result = "kg_{H2O}/kg_{air}";
  }
  return result;
}

} // openstudio
