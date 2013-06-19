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
#include <ruleset/test/RulesetFixture.hpp>

#include <ruleset/StandardsRule.hpp>
#include <ruleset/StandardsAttributeCheckRule.hpp>

#include <ruleset/StandardsFilterAvailableModel.hpp>
#include <ruleset/StandardsFilterRulesetOption.hpp>
#include <ruleset/StandardsFilterRulesetOption_Impl.hpp>
#include <ruleset/StandardsFilterObjectAttribute.hpp>
#include <ruleset/StandardsFilterObjectDoubleAttribute.hpp>
#include <ruleset/StandardsFilterObjectDoubleAttribute_Impl.hpp>
#include <ruleset/StandardsActionCloneModel.hpp>
#include <ruleset/StandardsActionRuleSatisfied.hpp>
#include <ruleset/StandardsActionRuleSatisfied_Impl.hpp>
#include <ruleset/StandardsEnumRulesetOption.hpp>
#include <ruleset/StandardsEnumRulesetOption_Impl.hpp>

#include <standardsinterface/DataDictionary.hpp>

#include <utilities/core/Containers.hpp>
#include <utilities/core/Path.hpp>

#include <boost/foreach.hpp>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::standardsinterface;

TEST_F(RulesetFixture,StandardsRule_CreateBudgetAndProposedModels) {
  StandardsRule rule("CreateBudgetAndProposedModels");
  
  // only if performance compliance
  StandardsEnumRulesetOption option(EnumAttribute::ComplianceApproach,"Performance");
  StandardsFilterRulesetOption performanceComplianceFilter(option);
  EXPECT_TRUE(rule.add(performanceComplianceFilter));
  EXPECT_FALSE(rule.add(performanceComplianceFilter)); // does not allow exact same filter
  EXPECT_EQ(1u,rule.numFilters());
  EXPECT_EQ(0u,rule.numActions());

  // get available model
  StandardsFilterAvailableModel modelFilter(StandardsRulesetModelType::Original);

  // make clones (adding this to Rule adds referenced filter too)
  StandardsActionCloneModel createBudgetModel(modelFilter,StandardsRulesetModelType::Budget);
  StandardsActionCloneModel createProposedModel(modelFilter,StandardsRulesetModelType::Proposed);
  EXPECT_TRUE(rule.add(createBudgetModel));
  EXPECT_EQ(2u,rule.numFilters());
  EXPECT_EQ(1u,rule.numActions());
  EXPECT_TRUE(rule.add(createProposedModel));
  EXPECT_EQ(2u,rule.numFilters());
  EXPECT_EQ(2u,rule.numActions());

  LOG(Info,std::endl << rule.print(StandardsRulesetPrintMode::Engine));

  EXPECT_TRUE(rule.remove(modelFilter));
  EXPECT_EQ(1u,rule.numFilters());
  EXPECT_EQ(0u,rule.numActions()); // removes actions that depend on the removed filter

  EXPECT_EQ(1u,rule.clearFilters());
}

TEST_F(RulesetFixture,StandardsRule_LoadFromFile) {
  std::vector<openstudio::path> ruleFiles;
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_ExteriorWallUFactor.csv"));
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_RoofAndExteriorCeilingUFactor_ExteriorCeiling.csv"));
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_RoofAndExteriorCeilingUFactor_Roof.csv"));
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_WindowUFactor.csv"));

  UnsignedVector expectedNumberOfRules;
  expectedNumberOfRules.push_back(80);
  expectedNumberOfRules.push_back(17);
  expectedNumberOfRules.push_back(17);
  expectedNumberOfRules.push_back(16);

  UnsignedVector expectedNumberOfFilters;
  expectedNumberOfFilters.push_back(8);
  expectedNumberOfFilters.push_back(8);
  expectedNumberOfFilters.push_back(7);
  expectedNumberOfFilters.push_back(7);

  unsigned numNullFilters(0);
  UnsignedVector::const_iterator numRulesIt = expectedNumberOfRules.begin();
  UnsignedVector::const_iterator numFiltersIt = expectedNumberOfFilters.begin();
  for (std::vector<openstudio::path>::const_iterator fileIt = ruleFiles.begin(), 
       fileEnd = ruleFiles.end(); fileIt != fileEnd; ++fileIt, ++numRulesIt, ++numFiltersIt)
  {
    openstudio::path p = resourcesPath() / toPath("ruleset/Title24_2008") / *fileIt;
    StandardsAttributeCheckRuleVector loadedRules = loadStandardsAttributeCheckRulesFromFile(p,true); // Title24 is in IP/Btu
    // test number of rules loaded
    EXPECT_EQ(*numRulesIt,loadedRules.size());
    // test number of filters and actions per rule. 
    // some filter values are null, and so a filter is not actually created.
    unsigned maxFilters(0);
    bool first = true;
    BOOST_FOREACH(const StandardsRule& rule,loadedRules) {
      if (first) {
        LOG(Debug,"The first rule from '" << toString(p) << "' is :" << std::endl 
            << rule.print(StandardsRulesetPrintMode::Engine));
        first = false;
      }
      EXPECT_TRUE(rule.numFilters() <= *numFiltersIt);
      if (rule.numFilters() > maxFilters) { maxFilters = rule.numFilters(); }
      numNullFilters += *numFiltersIt - rule.numFilters(); // number of null values
      EXPECT_EQ(1u,rule.getFilters<StandardsFilterRulesetOption>().size());
      if (rule.getFilters<StandardsFilterRulesetOption>().size() == 1) {
        ASSERT_TRUE(rule.getFilters<StandardsFilterRulesetOption>()[0].option().optionalCast<StandardsEnumRulesetOption>());
        EXPECT_EQ("ComplianceApproach",rule.getFilters<StandardsFilterRulesetOption>()[0].option().cast<StandardsEnumRulesetOption>().name());
      }
      else {
        LOG(Debug,"The following rule does not include a StandardsFilterRulesetOption: " 
            << std::endl << rule.print(StandardsRulesetPrintMode::Engine));
      }
      EXPECT_EQ(1u,rule.numActions());
      ASSERT_EQ(1u,rule.getActions<StandardsActionRuleSatisfied>().size());
      ASSERT_TRUE(rule.getActions<StandardsActionRuleSatisfied>()[0].attributeTest().optionalCast<StandardsFilterObjectDoubleAttribute>());
      EXPECT_TRUE(rule.getActions<StandardsActionRuleSatisfied>()[0].attributeTest().cast<StandardsFilterObjectDoubleAttribute>().attribute() == DoubleAttribute::UFactor);
    }
    EXPECT_EQ(*numFiltersIt,maxFilters);
  }
  EXPECT_TRUE(numNullFilters > 0);
}
