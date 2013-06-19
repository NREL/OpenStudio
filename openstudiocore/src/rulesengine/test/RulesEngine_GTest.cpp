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

#include <gtest/gtest.h>
#include <rulesengine/test/RulesEngineFixture.hpp>

#include <rulesengine/OpenStudioRulesEngine.hpp>

#include <ruleset/StandardsRuleset.hpp>
#include <ruleset/StandardsRulesetOption.hpp>
#include <ruleset/StandardsEnumRulesetOption.hpp>

#include <standardsinterface/Title24Interface.hpp>

#include <model/ModelObject.hpp>

#include <utilities/document/HeadersAndFooters.hpp>

#include <utilities/core/Checksum.hpp>

using namespace openstudio;
using namespace openstudio::standardsinterface;
using namespace openstudio::ruleset;
using namespace openstudio::rulesengine;

TEST_F(RulesEngineFixture,RulesEngine_OneOption_OneRule)
{
  StandardsRuleset ruleset("Test Ruleset");
  // add option(s)
  StandardsEnumRulesetOption option(EnumAttribute(EnumAttribute::ComplianceApproach),"Performance");
  ruleset.add(option);
  StandardsRulesetOptionVector myOptions = ruleset.options();
  ASSERT_EQ(1u,myOptions.size());

  Title24Interface model;

  OpenStudioRulesEngine re;
  re.applyRules(ruleset,myOptions,model);

  ApplyRulesetResult result = re.result();
  EXPECT_TRUE(result.runComplete());
  EXPECT_TRUE(result.overallResultFlag() == ruleset::StandardsRulesetResult::Pass);

  Document report = result.report();
  std::stringstream ss;
  std::string uuidString = checksum(toString(re.currentUuid()));
  ss << "rulesengine/scratch/RulesEngine_Run_" << uuidString << ".html";
  openstudio::path p = resourcesPath() / toPath(ss.str()); ss.str("");
  report.setFormat(DocumentFormat::XHTML,basicXHTMLHeader(),basicXHTMLFooter());
  report.save(p);
}
