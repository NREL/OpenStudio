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

#include "IddUnitString.hpp"

#include "Unit.hpp"

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
