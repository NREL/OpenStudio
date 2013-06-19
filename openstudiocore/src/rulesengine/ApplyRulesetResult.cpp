/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <rulesengine/ApplyRulesetResult.hpp>

#include <utilities/document/Section.hpp>
#include <utilities/document/Section_Impl.hpp>
#include <utilities/document/Text.hpp>

#include <boost/foreach.hpp>

namespace openstudio {
namespace rulesengine {

ApplyRulesetResult::ApplyRulesetResult(const std::string& reportTitle) 
  : m_runComplete(false), m_overallResultFlag(ruleset::StandardsRulesetResult::Pass)
{
  m_report.setTitle(reportTitle);
  m_report.setTopHeadingLevel(1);
  m_report.appendSection("Summary");
  m_report.appendSection("Errors");
  m_report.appendSection("Missing Information");
  Section details = m_report.appendSection("Details");
  details.appendSubsection("Fail Messages");
  details.appendSubsection("Pass Messages");
}

bool ApplyRulesetResult::runComplete() const { return m_runComplete; }

ruleset::StandardsRulesetResult ApplyRulesetResult::overallResultFlag() const { 
  return m_overallResultFlag; 
}

Document ApplyRulesetResult::report() const { return m_report; }

/// @cond

void ApplyRulesetResult::setRunComplete() { 
  m_runComplete = true; 

  // delete empty report sections
  SectionElementVector elements = m_report.contents();
  for (int i = elements.size() - 1; i >= 0; --i) {
    OptionalSection oSection = elements[i].optionalCast<Section>();
    if (oSection && oSection->contents().empty()) {
      m_report.erase(static_cast<unsigned>(i));
    }
  }  
}

void ApplyRulesetResult::setOverallResultFlag(
    const ruleset::StandardsRulesetResult& flag) 
{
  if (flag.value() > m_overallResultFlag.value()) {
    m_overallResultFlag = flag;
  }
}

void ApplyRulesetResult::addToReport(const std::string& sectionName, const std::string& text) {
  SectionVector sections = m_report.sections();
  BOOST_FOREACH(Section& section,sections) {
    if (section.heading().text() == sectionName) {
      section.append(Text(text));
      return;
    }
    // kludgy to make basic rule statisfied/rule violated sections for demo package
    // to improve, should probably push this functionality down into Document and Section, where 
    // it will be easier to make it recursive
    SectionVector subsections = section.subsections();
    BOOST_FOREACH(Section& subsection,subsections) {
      if (subsection.heading().text() == sectionName) {
        subsection.append(Text(text));
        return;
      }
    }
  }
  Section newSection = m_report.appendSection(sectionName);
  newSection.append(Text(text));
}

/// @endcond

} // rulesengine
} // openstudio
