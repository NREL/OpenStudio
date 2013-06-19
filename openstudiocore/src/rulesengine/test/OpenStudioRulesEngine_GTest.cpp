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

#include <project/ProjectDatabase.hpp>
#include <project/StandardsRulesetRecord.hpp>
#include <project/StandardsRuleRecord.hpp>
#include <project/FilterClauseRecord.hpp>
#include <project/ActionClauseRecord.hpp>

#include <ruleset/RulesetEnums.hpp>
#include <ruleset/StandardsRuleset.hpp>
#include <ruleset/StandardsRulesetEnums.hpp>

#include <ruleset/StandardsRulesetOption.hpp>
#include <ruleset/StandardsEnumRulesetOption.hpp>
#include <ruleset/StandardsEnumRulesetOption_Impl.hpp>
#include <ruleset/StandardsRule.hpp>
#include <ruleset/StandardsAttributeCheckRule.hpp>
#include <ruleset/StandardsFilterRulesetOption.hpp>
#include <ruleset/StandardsFilterAvailableModel.hpp>
#include <ruleset/StandardsFilterAvailableObject.hpp>
#include <ruleset/StandardsFilterObjectBooleanAttribute.hpp>
#include <ruleset/StandardsFilterObjectDoubleAttribute.hpp>
#include <ruleset/StandardsFilterObjectEnumAttribute.hpp>
#include <ruleset/StandardsFilterObjectIntegerAttribute.hpp>
#include <ruleset/StandardsActionCloneModel.hpp>
#include <ruleset/StandardsActionRuleSatisfied.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>

#include <standardsinterface/Title24Interface.hpp>

#include <utilities/sql/SqlFile.hpp>

#include <utilities/document/HeadersAndFooters.hpp>

#include <utilities/units/QuantityConverter.hpp>
#include <utilities/units/BTUUnit.hpp>
#include <utilities/units/SIUnit.hpp>

#include <utilities/core/Checksum.hpp>

using namespace openstudio;
using namespace openstudio::model;
using namespace openstudio::standardsinterface;
using namespace openstudio::ruleset;
using namespace openstudio::rulesengine;
using namespace openstudio::project;

TEST_F(RulesEngineFixture,OpenStudioRulesEngine_CloneModelForPerformanceCompliance)
{
  // CREATE RULESET
  StandardsRuleset ruleset("Test Ruleset");
  // add option
  StandardsEnumRulesetOption option(EnumAttribute(EnumAttribute::ComplianceApproach),"Performance");
  ruleset.add(option);

  // add rule
  StandardsRule rule("CreateBudgetAndProposedModels");
  // filters
  StandardsFilterRulesetOption performanceComplianceFilter(option);
  EXPECT_TRUE(rule.add(performanceComplianceFilter));
  StandardsFilterAvailableModel modelFilter(StandardsRulesetModelType::Original);
  EXPECT_TRUE(rule.add(modelFilter));
  // actions
  StandardsActionCloneModel createBudgetModel(modelFilter,StandardsRulesetModelType::Budget);
  StandardsActionCloneModel createProposedModel(modelFilter,StandardsRulesetModelType::Proposed);
  EXPECT_TRUE(rule.add(createBudgetModel));
  EXPECT_TRUE(rule.add(createProposedModel));
  EXPECT_EQ(2u,rule.numFilters());
  EXPECT_EQ(2u,rule.numActions());

  ruleset.add(rule);

  // CREATE MY OPTIONS
  StandardsRulesetOptionVector myOptions;
  myOptions.push_back(option);

  // RUN RULESENGINE
  Title24Interface model;
  boost::optional<project::ProjectDatabase> oComponentLibrary;
  OpenStudioRulesEngine re;
  re.applyRules(ruleset,myOptions,model,oComponentLibrary);
  EXPECT_EQ(3u,re.numModels());

  // GET RESULT
  ApplyRulesetResult result = re.result();
  EXPECT_TRUE(result.runComplete());
  EXPECT_TRUE(result.overallResultFlag() == ruleset::StandardsRulesetResult::Pass);

  // OUTPUT REPORT
  Document report = result.report();
  std::stringstream ss;
  std::string uuidString = checksum(toString(re.currentUuid()));
  ss << "rulesengine/scratch/RulesEngine_Run_" << uuidString << ".html";
  openstudio::path p = resourcesPath() / toPath(ss.str()); ss.str("");
  report.setFormat(DocumentFormat::XHTML,basicXHTMLHeader(),basicXHTMLFooter());
  report.save(p);
}

TEST_F(RulesEngineFixture,OpenStudioRulesEngine_PathfinderForPAT)
{
  // ASSEMBLE MODEL (idf/osm and sql)
  Model model = exampleModel(true).clone().cast<Model>();
  ASSERT_TRUE(model.sqlFile());

  // SET CLIMATE ZONE
  Title24Interface wrappedModel(model);
  Building building = model.getUniqueModelObject<Building>();
  VoidResult ok = wrappedModel.setAttribute("building",building,"ClimateZone","1");
  EXPECT_TRUE(ok);

  // CREATE RULESET
  StandardsRuleset ruleset("Test Ruleset");
  // add option
  StandardsEnumRulesetOption option(EnumAttribute(EnumAttribute::ComplianceApproach),"Prescriptive Envelope Component");
  ruleset.add(option);
  // add rule
  // filters
  StandardsFilterRulesetOption optionFilter(option);
  StandardsFilterAvailableModel modelFilter(StandardsRulesetModelType::Original);
  StandardsFilterAvailableObject buildingFilter(modelFilter,ObjectType("Building"));
  StandardsFilterObjectEnumAttribute climateZone(buildingFilter,
                                                 EnumAttribute("ClimateZone"),
                                                 RulesetStringPredicate(RulesetStringPredicate::Equals),
                                                 "1");
  StandardsFilterAvailableObject roof(modelFilter,ObjectType("Roof"));
  Quantity maximumUFactorIP(0.065,BTUUnit(BTUExpnt(1,-2,-1,-1)));
  Quantity maximumUFactorSI = QuantityConverter::instance().convert(maximumUFactorIP,UnitSystem(UnitSystem::SI)).get();
  StandardsFilterObjectDoubleAttribute ufactor(roof,
                                               DoubleAttribute("UFactor"),
                                               RulesetNumericalPredicate(RulesetNumericalPredicate::LessThanOrEqualTo),
                                               maximumUFactorSI.value(),
                                               "W/m^2*K");
  // actions
  StandardsActionRuleSatisfied action(ufactor);
  // construct and add rule
  StandardsAttributeCheckRule rule(StandardsFilterRulesetOptionVector(1u,optionFilter),
                                   StandardsFilterObjectAttributeVector(),
                                   action);
  rule.add(climateZone);
  EXPECT_EQ(6u,rule.filters().size());
  EXPECT_EQ(1u,rule.actions().size());
  ruleset.add(rule);

  // CREATE MY OPTIONS
  StandardsRulesetOptionVector myOptions;
  myOptions.push_back(option);

  // RUN RULESENGINE
  boost::optional<project::ProjectDatabase> oComponentLibrary;
  OpenStudioRulesEngine re;
  re.applyRules(ruleset,myOptions,wrappedModel,oComponentLibrary);
  EXPECT_EQ(1u,re.numModels());

  // GET RESULT
  ApplyRulesetResult result = re.result();
  EXPECT_TRUE(result.runComplete());
  EXPECT_TRUE(result.overallResultFlag() == ruleset::StandardsRulesetResult::Fail); // Fails because attic construction not yet handled properly.

  // OUTPUT CLIPS CONSTRUCTS
  LOG(Debug,re.printInternalConstructs(myOptions));
  LOG(Debug,ruleset.print(ruleset::StandardsRulesetPrintMode::Engine));

  // OUTPUT REPORT
  Document report = result.report();
  std::stringstream ss;
  std::string uuidString = checksum(toString(re.currentUuid()));
  ss << "rulesengine/scratch/RulesEngine_Run_" << uuidString << ".html";
  openstudio::path p = resourcesPath() / toPath(ss.str()); ss.str("");
  report.setFormat(DocumentFormat::XHTML,basicXHTMLHeader(),basicXHTMLFooter());
  report.save(p);

  // CHANGE RULESET AND EXPECT SUCCESS
  ufactor.setValue((maximumUFactorSI*20.0).value());
  re.applyRules(ruleset,myOptions,wrappedModel,oComponentLibrary);
  EXPECT_EQ(1u,re.numModels());
  result = re.result();
  EXPECT_TRUE(result.runComplete());
  EXPECT_TRUE(result.overallResultFlag() == ruleset::StandardsRulesetResult::Pass);

  // OUTPUT REPORT
  report = result.report();
  uuidString = checksum(toString(re.currentUuid()));
  ss << "rulesengine/scratch/RulesEngine_Run_" << uuidString << ".html";
  p = resourcesPath() / toPath(ss.str()); ss.str("");
  report.setFormat(DocumentFormat::XHTML,basicXHTMLHeader(),basicXHTMLFooter());
  report.save(p);
}

TEST_F(RulesEngineFixture,OpenStudioRulesEngine_NominallyRunPATRuleset) {
  // construct PAT ruleset, then expect errors when run on deficient model
  StandardsRuleset ruleset("PAT Ruleset");
  StandardsAttributeCheckRuleVector rulesFromFile;

  std::vector<openstudio::path> ruleFiles;
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_ExteriorWallUFactor.csv"));
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_RoofAndExteriorCeilingUFactor_ExteriorCeiling.csv"));
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_RoofAndExteriorCeilingUFactor_Roof.csv"));
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_WindowUFactor.csv"));

  BOOST_FOREACH(const openstudio::path& ruleFile,ruleFiles) {
    openstudio::path p = resourcesPath() / toPath("ruleset/Title24_2008") / ruleFile;
    rulesFromFile = loadStandardsAttributeCheckRulesFromFile(p);
    BOOST_FOREACH(const StandardsRule& rule,rulesFromFile) {
      ruleset.add(rule);
    }
  }

  ASSERT_EQ(1u,ruleset.options().size());
  StandardsRulesetOptionVector myOptions;
  myOptions.push_back(ruleset.options()[0].clone().cast<StandardsRulesetOption>());

  // ASSEMBLE MODEL (idf/osm and sql)
  Model model = exampleModel(true).clone().cast<Model>();
  ASSERT_TRUE(model.sqlFile());
  Title24Interface wrappedModel(model);

  // RUN RULESENGINE
  OpenStudioRulesEngine re;
  LOG(Debug,"The RulesEngine's internal constructs: " << std::endl
      << re.printInternalConstructs(myOptions));
  LOG(Debug,"The ruleset, as built for CLIPS: " << std::endl
      << ruleset.print(StandardsRulesetPrintMode::Engine));
  re.applyRules(ruleset,myOptions,wrappedModel,boost::none);
  ApplyRulesetResult result = re.result();
  EXPECT_TRUE(result.runComplete());
  EXPECT_TRUE(result.overallResultFlag() == ruleset::StandardsRulesetResult::Incomplete);

  // OUTPUT REPORT
  Document report = result.report();
  std::string uuidString = checksum(toString(re.currentUuid()));
  std::stringstream ss;
  ss << "rulesengine/scratch/RulesEngine_Run_" << uuidString << ".html";
  openstudio::path p = resourcesPath() / toPath(ss.str()); ss.str("");
  report.setFormat(DocumentFormat::XHTML,basicXHTMLHeader(),basicXHTMLFooter());
  report.save(p);
}

