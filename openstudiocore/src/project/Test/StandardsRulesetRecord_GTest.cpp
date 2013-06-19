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
#include <project/Test/ProjectFixture.hpp>

#include <project/ProjectDatabase.hpp>
#include <project/StandardsRulesetRecord.hpp>
#include <project/StandardsRulesetOptionRecord.hpp>
#include <project/StandardsRuleRecord.hpp>
#include <project/StandardsAttributeCheckRuleRecord.hpp>
#include <project/StandardsEnumRulesetOptionRecord.hpp>
#include <project/ClauseRecord.hpp>
#include <project/FilterClauseRecord.hpp>
#include <project/ActionClauseRecord.hpp>

#include <ruleset/StandardsRuleset.hpp>
#include <ruleset/StandardsRulesetOption.hpp>
#include <ruleset/StandardsRule.hpp>
#include <ruleset/StandardsActionRuleSatisfied.hpp>
#include <ruleset/StandardsAttributeCheckRule.hpp>
#include <ruleset/StandardsAttributeCheckRule_Impl.hpp>
#include <ruleset/StandardsEnumRulesetOption.hpp>
#include <ruleset/StandardsEnumRulesetOption_Impl.hpp>
#include <ruleset/StandardsFilterAvailableModel.hpp>
#include <ruleset/StandardsFilterAvailableModel_Impl.hpp>
#include <ruleset/StandardsFilterAvailableObject.hpp>
#include <ruleset/StandardsFilterAvailableObject_Impl.hpp>
#include <ruleset/StandardsFilterRulesetOption.hpp>
#include <ruleset/StandardsFilterRulesetOption_Impl.hpp>
#include <ruleset/StandardsFilterObjectBooleanAttribute.hpp>
#include <ruleset/StandardsFilterObjectBooleanAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectDoubleAttribute.hpp>
#include <ruleset/StandardsFilterObjectDoubleAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectEnumAttribute.hpp>
#include <ruleset/StandardsFilterObjectEnumAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectIntegerAttribute.hpp>
#include <ruleset/StandardsFilterObjectIntegerAttribute_Impl.hpp>

#include <boost/foreach.hpp>

#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::project;
using namespace openstudio::standardsinterface;

TEST_F(ProjectFixture, StandardsRulesetRecord)
{
  ProjectDatabase database = getCleanDatabase("StandardsRulesetRecord");

  // make ruleset
  StandardsRuleset ruleset("ruleset");
  UUID originalRulesetUUID = ruleset.uuid();
  UUID originalRulesetVersionUUID = ruleset.versionUUID();

  // add option
  StandardsEnumRulesetOption option(EnumAttribute(EnumAttribute::ComplianceApproach),"Performance");
  ruleset.add(option);
  EXPECT_EQ(1u,ruleset.options().size());
  EXPECT_TRUE(originalRulesetUUID == ruleset.uuid());
  EXPECT_TRUE(originalRulesetVersionUUID != ruleset.versionUUID());

  // add rule
  StandardsRule rule("Table143A_Roof_UFactor_ClimateZone1");
  StandardsFilterRulesetOption optionFilter(option);
  EXPECT_TRUE(option.uuid() == optionFilter.option().uuid());

  StandardsFilterAvailableModel model(StandardsRulesetModelType::Original);
  StandardsFilterAvailableObject building(model,ObjectType("Building"));
  StandardsFilterObjectEnumAttribute climateZone(building,
                                                 EnumAttribute("ClimateZone"),
                                                 ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
                                                 "1");
  StandardsFilterAvailableObject roof(model,ObjectType("Roof"));
  StandardsFilterObjectDoubleAttribute ufactor(roof,
                                      DoubleAttribute("UFactor"),
                                      RulesetNumericalPredicate(RulesetNumericalPredicate::LessThanOrEqualTo),
                                      0.065,
                                      "W/m^2*K");
  rule.add(optionFilter);
  rule.add(climateZone);
  rule.add(ufactor);
  ASSERT_EQ(6u,rule.filters().size());

  ASSERT_TRUE(rule.filters()[0].optionalCast<StandardsFilterRulesetOption>());
  EXPECT_TRUE(option.uuid() == rule.filters()[0].cast<StandardsFilterRulesetOption>().option().uuid());

  ruleset.add(rule);
  ASSERT_EQ(1u,ruleset.rules().size());
  ASSERT_TRUE(ruleset.rules()[0].filters()[0].optionalCast<StandardsFilterRulesetOption>());
  EXPECT_TRUE(option.uuid() == rule.filters()[0].cast<StandardsFilterRulesetOption>().option().uuid());

  // save to database
  StandardsRulesetRecord standardsRulesetRecord(ruleset, database);
  EXPECT_EQ(1u, RulesetRecord::getRulesetRecords(database).size());
  EXPECT_EQ(1u, StandardsEnumRulesetOptionRecord::getStandardsEnumRulesetOptionRecords(database).size());
  ASSERT_EQ(1u, StandardsRulesetRecord::getStandardsRulesetRecords(database).size());

  // get out of database
  StandardsRuleset ruleset2 = StandardsRulesetRecord::getStandardsRulesetRecords(database)[0].standardsRuleset();
  EXPECT_TRUE(ruleset.uuid() == ruleset2.uuid());
  EXPECT_TRUE(ruleset.versionUUID() == ruleset2.versionUUID());

  // check options
  ASSERT_EQ(1u, ruleset2.options().size());
  ASSERT_TRUE(ruleset2.options()[0].optionalCast<StandardsEnumRulesetOption>());
  EXPECT_TRUE(option.uuid() != ruleset2.options()[0].uuid()); // options are cloned in ruleset2.options
  EXPECT_TRUE(option.versionUUID() != ruleset2.options()[0].versionUUID()); // options are cloned in ruleset2.options
  EXPECT_EQ(option.enumOption(), ruleset2.options()[0].cast<StandardsEnumRulesetOption>().enumOption());
  EXPECT_EQ(option.value(), ruleset2.options()[0].cast<StandardsEnumRulesetOption>().value());
  EXPECT_EQ(option.defaultValue(), ruleset2.options()[0].cast<StandardsEnumRulesetOption>().defaultValue());

  // check rules
  ASSERT_EQ(1u, ruleset2.rules().size());
  ASSERT_TRUE(ruleset2.options()[0].optionalCast<StandardsEnumRulesetOption>());
  EXPECT_TRUE(rule.uuid() == ruleset2.rules()[0].uuid()); // rule UUIDs are not cloned
  EXPECT_TRUE(rule.versionUUID() == ruleset2.rules()[0].versionUUID()); // rule UUIDs are not cloned
  EXPECT_EQ(6u, ruleset2.rules()[0].filters().size());
}


TEST_F(ProjectFixture, StandardsRulesetRecord_FromFile)
{
  ProjectDatabase database = getCleanDatabase("StandardsRulesetRecord_FromFile");

  ruleset::StandardsRuleset standardsRuleset = ruleset::StandardsRuleset("Standards Ruleset");

  // add option
  ruleset::StandardsEnumRulesetOption option(standardsinterface::EnumAttribute(standardsinterface::EnumAttribute::ComplianceApproach),"Prescriptive Envelope Component");
  standardsRuleset.add(option);

  // load paths
  std::vector<openstudio::path> ruleFiles;
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_ExteriorWallUFactor.csv"));
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_RoofAndExteriorCeilingUFactor_ExteriorCeiling.csv"));
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_RoofAndExteriorCeilingUFactor_Roof.csv"));
  ruleFiles.push_back(toPath("PrescriptiveEnvelopeComponent_Table143A_WindowUFactor.csv"));

  openstudio::path basePath = resourcesPath() / toPath("/ruleset/Title24_2008/");
  BOOST_FOREACH(const openstudio::path& ruleFile, ruleFiles){
    openstudio::path rulePath = basePath / ruleFile;

    ruleset::StandardsAttributeCheckRuleVector loadedRules = ruleset::loadStandardsAttributeCheckRulesFromFile(rulePath, true); // Title24 is in IP/Btu
    EXPECT_FALSE(loadedRules.empty());

    BOOST_FOREACH(const ruleset::StandardsAttributeCheckRule& rule, loadedRules) {
      standardsRuleset.add(rule);
    }
  }

  // check that we aren't creating duplicate rules or filters
  unsigned numRules = standardsRuleset.rules().size();
  std::map<UUID, unsigned> numFiltersForRule;
  std::map<UUID, unsigned> numActionsForRule;
  BOOST_FOREACH(const ruleset::StandardsRule& rule, standardsRuleset.rules()){
    numFiltersForRule[rule.uuid()] = rule.filters().size();
    numActionsForRule[rule.uuid()] = rule.actions().size();
  }

  // start new scope
  {
    // save the ruleset
    EXPECT_TRUE(database.startTransaction());

    StandardsRulesetRecord(standardsRuleset, database);

    EXPECT_TRUE(database.commitTransaction());

    EXPECT_TRUE(database.save());

    // get the ruleset
    std::vector<StandardsRulesetRecord> standardsRulesetRecords = project::StandardsRulesetRecord::getStandardsRulesetRecords(database);
    ASSERT_EQ(1u, standardsRulesetRecords.size());
    ruleset::StandardsRuleset standardsRuleset2 = standardsRulesetRecords[0].standardsRuleset();
    EXPECT_TRUE(standardsRuleset.equivalentData(standardsRuleset2));

    EXPECT_EQ(numRules, standardsRuleset2.rules().size());
    BOOST_FOREACH(const ruleset::StandardsRule& rule, standardsRuleset2.rules()){
      ASSERT_TRUE(numFiltersForRule.find(rule.uuid()) != numFiltersForRule.end());
      ASSERT_TRUE(numActionsForRule.find(rule.uuid()) != numActionsForRule.end());
      EXPECT_EQ(numFiltersForRule[rule.uuid()], rule.filters().size());
      EXPECT_EQ(numActionsForRule[rule.uuid()], rule.actions().size());
    }

    // get the ruleset options
    std::vector<StandardsRulesetOptionRecord> standardsRulesetOptionRecords = standardsRulesetRecords[0].standardsRulesetOptionRecords();
    EXPECT_FALSE(standardsRulesetOptionRecords.empty());

    EXPECT_TRUE(database.findLoadedRecord(standardsRuleset.uuid()));
  }

  EXPECT_TRUE(database.findLoadedRecord(standardsRuleset.uuid()));
  database.unloadUnusedCleanRecords();
  EXPECT_FALSE(database.findLoadedRecord(standardsRuleset.uuid()));

  // save the ruleset again
  EXPECT_TRUE(database.startTransaction());

  StandardsRulesetRecord(standardsRuleset, database);

  EXPECT_TRUE(database.commitTransaction());

  EXPECT_TRUE(database.save());

  // get the ruleset again
  std::vector<StandardsRulesetRecord> standardsRulesetRecords = project::StandardsRulesetRecord::getStandardsRulesetRecords(database);
  ASSERT_EQ(1u, standardsRulesetRecords.size());
  ruleset::StandardsRuleset standardsRuleset2 = standardsRulesetRecords[0].standardsRuleset();
  EXPECT_TRUE(standardsRuleset.equivalentData(standardsRuleset2));

  EXPECT_EQ(numRules, standardsRuleset2.rules().size());
  BOOST_FOREACH(const ruleset::StandardsRule& rule, standardsRuleset2.rules()){
    ASSERT_TRUE(numFiltersForRule.find(rule.uuid()) != numFiltersForRule.end());
    ASSERT_TRUE(numActionsForRule.find(rule.uuid()) != numActionsForRule.end());
    EXPECT_EQ(numFiltersForRule[rule.uuid()], rule.filters().size());
    EXPECT_EQ(numActionsForRule[rule.uuid()], rule.actions().size());
  }

  // get the ruleset options again
  std::vector<StandardsRulesetOptionRecord> standardsRulesetOptionRecords = standardsRulesetRecords[0].standardsRulesetOptionRecords();
  EXPECT_FALSE(standardsRulesetOptionRecords.empty());

  // delete the ruleset
  EXPECT_TRUE(database.removeRecord(standardsRulesetRecords[0]));
  EXPECT_TRUE(database.includesRecord(standardsRulesetRecords[0]));
  EXPECT_FALSE(database.isNewRecord(standardsRulesetRecords[0]));
  EXPECT_FALSE(database.isCleanRecord(standardsRulesetRecords[0]));
  EXPECT_FALSE(database.isDirtyRecord(standardsRulesetRecords[0]));
  EXPECT_TRUE(database.isRemovedRecord(standardsRulesetRecords[0]));

  EXPECT_TRUE(database.save());

  EXPECT_FALSE(database.includesRecord(standardsRulesetRecords[0]));
  EXPECT_FALSE(database.isNewRecord(standardsRulesetRecords[0]));
  EXPECT_FALSE(database.isCleanRecord(standardsRulesetRecords[0]));
  EXPECT_FALSE(database.isDirtyRecord(standardsRulesetRecords[0]));
  EXPECT_FALSE(database.isRemovedRecord(standardsRulesetRecords[0]));

  // deleting ruleset does not delete rules or clauses as these are resources, it does delete options
  EXPECT_EQ(0u, project::RulesetRecord::getRulesetRecords(database).size());
  EXPECT_FALSE(project::RuleRecord::getRuleRecords(database).empty());
  EXPECT_FALSE(project::ClauseRecord::getClauseRecords(database).empty());
  BOOST_FOREACH(StandardsRulesetOptionRecord standardsRulesetOptionRecord, standardsRulesetOptionRecords){
    EXPECT_FALSE(project::StandardsRulesetOptionRecord::getStandardsRulesetOptionRecord(standardsRulesetOptionRecord.id(), database));
  }

}

TEST_F(ProjectFixture, StandardsRulesetRecord_Transactions)
{
  ProjectDatabase database = getCleanDatabase("StandardsRulesetRecord_Transactions");

  ruleset::StandardsRuleset standardsRuleset = ruleset::StandardsRuleset("Standards Ruleset");
  StandardsRulesetRecord standardsRulesetRecord1(standardsRuleset, database);

  database.save();

  EXPECT_TRUE(database.startTransaction());

  ASSERT_EQ(1u, StandardsRulesetRecord::getStandardsRulesetRecords(database).size());
  EXPECT_EQ("Standards Ruleset", StandardsRulesetRecord::getStandardsRulesetRecords(database)[0].name());

  EXPECT_TRUE(database.removeRecord(standardsRulesetRecord1));

  EXPECT_EQ(0u, StandardsRulesetRecord::getStandardsRulesetRecords(database).size());

  // have to save the deletion because otherwise the uuids will be matched to the removed record
  EXPECT_TRUE(database.save());

  StandardsRulesetRecord standardsRulesetRecord2(standardsRuleset, database);

  ASSERT_EQ(1u, StandardsRulesetRecord::getStandardsRulesetRecords(database).size());
  EXPECT_EQ("Standards Ruleset", StandardsRulesetRecord::getStandardsRulesetRecords(database)[0].name());

  EXPECT_TRUE(database.save());

  EXPECT_TRUE(database.commitTransaction());

  ASSERT_EQ(1u, StandardsRulesetRecord::getStandardsRulesetRecords(database).size());
  EXPECT_EQ("Standards Ruleset", StandardsRulesetRecord::getStandardsRulesetRecords(database)[0].name());

}

TEST_F(ProjectFixture, StandardsActionRuleSatisfiedRecord)
{
  ProjectDatabase database = getCleanDatabase("StandardsActionRuleSatisfiedRecord");

  std::vector<ruleset::StandardsFilterRulesetOption> optionFilters;
  std::vector<ruleset::StandardsFilterObjectAttribute> qualifyingFilters;

  ruleset::StandardsFilterAvailableModel filterAvailableModel(ruleset::StandardsRulesetModelType::Original);
  ruleset::StandardsFilterAvailableObject filterAvailableObject(filterAvailableModel, standardsinterface::ObjectType::Building);
  ruleset::StandardsFilterObjectBooleanAttribute filterObjectBooleanAttribute(filterAvailableObject,
                                                   standardsinterface::BooleanAttribute::StateBuilding, true);
  ruleset::StandardsActionRuleSatisfied actionRuleSatisfied(filterObjectBooleanAttribute);


  ruleset::StandardsAttributeCheckRule rule(optionFilters, qualifyingFilters, actionRuleSatisfied);

  StandardsAttributeCheckRuleRecord ruleRecord(rule, database);
  EXPECT_TRUE(ruleRecord.id() > 0);
  EXPECT_EQ(rule.uuid(), ruleRecord.handle());
  EXPECT_EQ(3u, ruleRecord.filterClauseRecords().size());
  EXPECT_EQ(1u, ruleRecord.actionClauseRecords().size());

  database.save();

  ASSERT_EQ(1u, StandardsAttributeCheckRuleRecord::getStandardsAttributeCheckRuleRecords(database).size());
  EXPECT_TRUE(StandardsAttributeCheckRuleRecord::getStandardsAttributeCheckRuleRecords(database)[0].standardsRule().optionalCast<ruleset::StandardsAttributeCheckRule>());
  ASSERT_EQ(1u, StandardsRuleRecord::getStandardsRuleRecords(database).size());
  EXPECT_TRUE(StandardsRuleRecord::getStandardsRuleRecords(database)[0].standardsRule().optionalCast<ruleset::StandardsAttributeCheckRule>());

  StandardsAttributeCheckRuleRecord ruleRecord2(rule, database);

  EXPECT_EQ(ruleRecord.id(), ruleRecord2.id());
  EXPECT_EQ(rule.uuid(), ruleRecord2.handle());
  EXPECT_EQ(3u, ruleRecord2.filterClauseRecords().size());
  EXPECT_EQ(1u, ruleRecord2.actionClauseRecords().size());

  database.save();

  ASSERT_EQ(1u, StandardsAttributeCheckRuleRecord::getStandardsAttributeCheckRuleRecords(database).size());
  EXPECT_TRUE(StandardsAttributeCheckRuleRecord::getStandardsAttributeCheckRuleRecords(database)[0].standardsRule().optionalCast<ruleset::StandardsAttributeCheckRule>());
  ASSERT_EQ(1u, StandardsRuleRecord::getStandardsRuleRecords(database).size());
  EXPECT_TRUE(StandardsRuleRecord::getStandardsRuleRecords(database)[0].standardsRule().optionalCast<ruleset::StandardsAttributeCheckRule>());

}
