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
#include <ruleset/StandardsActionGetObjects.hpp>
#include <ruleset/StandardsActionGetObjects_Impl.hpp>
#include <ruleset/StandardsActionGetAttribute.hpp>
#include <ruleset/StandardsActionGetAttribute_Impl.hpp>

#include <utilities/core/Containers.hpp>

#include <boost/foreach.hpp>

#include <resources.hxx>

using namespace openstudio;
using namespace openstudio::standardsinterface;
using namespace openstudio::ruleset;

TEST_F(RulesetFixture,StandardsRuleset_Options)
{
  StandardsRuleset ruleset("ruleset");

  // add option(s)
  StandardsEnumRulesetOption option(EnumAttribute(EnumAttribute::ComplianceApproach),"Performance");
  ruleset.add(option);

  StandardsRulesetOptionVector availableOptions = ruleset.options();
  ASSERT_EQ(1u,availableOptions.size());
  ASSERT_TRUE(availableOptions[0].optionalCast<StandardsEnumRulesetOption>());
  StandardsRulesetOptionVector myOptions = availableOptions;
  // edit myOptions. availableOptions will be affected, but refreshed availableOptions will not.
  // (RulesetOption data is shared, but is cloned when being added to or passed out of a Ruleset.)
  StandardsEnumRulesetOption myOption = myOptions[0].cast<StandardsEnumRulesetOption>();
  StandardsEnumRulesetOption availableOption = availableOptions[0].cast<StandardsEnumRulesetOption>();
  StringVector validValues = myOption.validValues();
  EXPECT_TRUE(validValues.size() > 1);
  EXPECT_FALSE(validValues[1] == myOption.defaultValue());
  EXPECT_TRUE(myOption.setValue(validValues[1]));
  EXPECT_EQ(myOption.value(),availableOption.value());
  EXPECT_NE(myOption.value(),option.value());
  availableOptions = ruleset.options();
  availableOption = availableOptions[0].cast<StandardsEnumRulesetOption>();
  EXPECT_NE(myOption.value(),availableOption.value());
  EXPECT_EQ(option.value(),availableOption.value());
  // all versions have same defaultValue
  EXPECT_EQ(option.defaultValue(),myOption.defaultValue());
  EXPECT_EQ(option.defaultValue(),availableOption.defaultValue());

  openstudio::path p = resourcesPath() / toPath("ruleset/scratch/options.osr");
  ruleset.save(p,true);
}

TEST_F(RulesetFixture, StandardsRuleset_CreateModelClonesForPerformanceCompliance) {
  StandardsRuleset ruleset("ruleset");

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

  LOG(Info,std::endl << ruleset.print(StandardsRulesetPrintMode::Engine));
}

TEST_F(RulesetFixture, StandardsRuleset_CheckRoofUFactorForOneClimateZone) {
  StandardsRuleset ruleset("ruleset");

  // add option
  StandardsEnumRulesetOption option(EnumAttribute(EnumAttribute::ComplianceApproach),"Prescriptive Envelope Component");
  ruleset.add(option);

  // add rule
  StandardsRule rule("Table143A_Roof_UFactor_ClimateZone1");
  // filters
  StandardsFilterRulesetOption optionFilter(option);
  StandardsFilterAvailableModel model(StandardsRulesetModelType::Original);
  StandardsFilterAvailableObject building(model,ObjectType("Building"));
  StandardsFilterObjectEnumAttribute climateZone(building,EnumAttribute("ClimateZone"),RulesetStringPredicate::Equals,"1");
  StandardsFilterAvailableObject roof(model,ObjectType("Roof"));
  StandardsFilterObjectDoubleAttribute ufactor(roof,
                                               DoubleAttribute("UFactor"),
                                               RulesetNumericalPredicate(RulesetNumericalPredicate::LessThanOrEqualTo),
                                               0.065,
                                               "W/m^2*K");
  rule.add(optionFilter);
  rule.add(climateZone);
  rule.add(ufactor);
  EXPECT_EQ(6u,rule.filters().size());
  // actions
  StandardsActionRuleSatisfied action(ufactor);
  rule.add(action);
  EXPECT_EQ(6u,rule.filters().size());
  EXPECT_EQ(1u,rule.actions().size());
  ruleset.add(rule);

  LOG(Info,"Print ruleset to file: " << std::endl << ruleset.print(StandardsRulesetPrintMode::File));
  LOG(Info,"Print ruleset to engine: " << std::endl << ruleset.print(StandardsRulesetPrintMode::Engine));
}

TEST_F(RulesetFixture, StandardsRuleset_PATRuleset_ImpliedRules) {
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

  StandardsRuleVector impliedRules = ruleset.impliedRules();
  StandardsFilterAvailableModel modelFilter(StandardsRulesetModelType::Original);
  ruleset::detail::StandardsActionGetObjectsVector expectedGetObjects;
  StandardsFilterAvailableObject objectFilter(modelFilter,ObjectType::Building);
  ruleset::detail::StandardsActionGetObjects getObjectsAction(objectFilter);
  expectedGetObjects.push_back(getObjectsAction);
  objectFilter = StandardsFilterAvailableObject(modelFilter,ObjectType::Roof);
  getObjectsAction = ruleset::detail::StandardsActionGetObjects(objectFilter);
  expectedGetObjects.push_back(getObjectsAction);
  objectFilter = StandardsFilterAvailableObject(modelFilter,ObjectType::Ceiling);
  getObjectsAction = ruleset::detail::StandardsActionGetObjects(objectFilter);
  expectedGetObjects.push_back(getObjectsAction);
  objectFilter = StandardsFilterAvailableObject(modelFilter,ObjectType::Wall);
  getObjectsAction = ruleset::detail::StandardsActionGetObjects(objectFilter);
  expectedGetObjects.push_back(getObjectsAction);
  objectFilter = StandardsFilterAvailableObject(modelFilter,ObjectType::Window);
  getObjectsAction = ruleset::detail::StandardsActionGetObjects(objectFilter);
  expectedGetObjects.push_back(getObjectsAction);
  BOOST_FOREACH(const ruleset::detail::StandardsActionGetObjects& getObjects, expectedGetObjects) {
    unsigned count(0);
    BOOST_FOREACH(const StandardsRule& rule,impliedRules) {
      ruleset::detail::StandardsActionGetObjectsVector thisGetObjects = rule.getActions<ruleset::detail::StandardsActionGetObjects>();
      EXPECT_TRUE(thisGetObjects.size() <= 1u);
      if (!thisGetObjects.empty()) {
        if (getObjects.equivalentData(thisGetObjects[0])) {
          ++count;
        }
      }
    }
    EXPECT_EQ(1u,count);
  }
  // get attributes Building Climate Zone, Ceiling PartitionThermalType, Wall PartitionThermalType
  ruleset::detail::StandardsActionGetAttributeVector expectedGetAttributes;
  objectFilter = StandardsFilterAvailableObject(modelFilter,ObjectType::Building);
  ruleset::detail::StandardsActionGetAttribute getAttributeAction(objectFilter,EnumAttribute(EnumAttribute::ClimateZone));
  expectedGetAttributes.push_back(getAttributeAction);
  objectFilter = StandardsFilterAvailableObject(modelFilter,ObjectType::Ceiling);
  getAttributeAction = ruleset::detail::StandardsActionGetAttribute(objectFilter,EnumAttribute(EnumAttribute::PartitionThermalType));
  expectedGetAttributes.push_back(getAttributeAction);
  objectFilter = StandardsFilterAvailableObject(modelFilter,ObjectType::Wall);
  getAttributeAction = ruleset::detail::StandardsActionGetAttribute(objectFilter,EnumAttribute(EnumAttribute::PartitionThermalType));
  expectedGetAttributes.push_back(getAttributeAction);
  objectFilter = StandardsFilterAvailableObject(modelFilter,ObjectType::Wall);
  getAttributeAction = ruleset::detail::StandardsActionGetAttribute(objectFilter,EnumAttribute(EnumAttribute::WallConstructionType));
  expectedGetAttributes.push_back(getAttributeAction);
  BOOST_FOREACH(const ruleset::detail::StandardsActionGetAttribute& getAttribute, expectedGetAttributes) {
    bool found(false);
    BOOST_FOREACH(const StandardsRule& rule,impliedRules) {
      ruleset::detail::StandardsActionGetAttributeVector thisGetAttributes = rule.getActions<ruleset::detail::StandardsActionGetAttribute>();
      EXPECT_TRUE(thisGetAttributes.size() <= 1u);
      if (!thisGetAttributes.empty()) {
        if (getAttribute.equivalentData(thisGetAttributes[0])) {
          found = true;
          break;
        }
      }
    }
    EXPECT_TRUE(found);
  }
}
