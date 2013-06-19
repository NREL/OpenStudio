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
#include <project/StandardsRulesetOptionRecord.hpp>
#include <project/StandardsAttributeCheckRuleRecord.hpp>
#include <project/StandardsEnumRulesetOptionRecord.hpp>
#include <project/ClauseRecord.hpp>
#include <project/FilterClauseRecord.hpp>
#include <project/ActionClauseRecord.hpp>

#include <ruleset/StandardsRuleset.hpp>
#include <ruleset/StandardsRulesetEnums.hpp>
#include <ruleset/StandardsRulesetOption.hpp>
#include <ruleset/StandardsRule.hpp>
#include <ruleset/StandardsActionRuleSatisfied.hpp>
#include <ruleset/StandardsActionRuleSatisfied_Impl.hpp>
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

TEST_F(ProjectFixture, StandardsAttributeCheckRuleRecord_ReferencedFilters)
{
  ProjectDatabase database = getCleanDatabase("StandardsAttributeCheckRuleRecord_ReferencedFilters");

  // make rule
  StandardsFilterRulesetOptionVector options;
  StandardsFilterObjectAttributeVector qualifyingFilters;
  StandardsFilterAvailableModel model(StandardsRulesetModelType::Original);
  StandardsFilterAvailableObject building(model,ObjectType("Building"));
  StandardsFilterObjectEnumAttribute climateZone(
      building,
      EnumAttribute("ClimateZone"),
      ruleset::RulesetStringPredicate(ruleset::RulesetStringPredicate::Equals),
      "1");
  qualifyingFilters.push_back(climateZone);
  StandardsFilterAvailableObject roof(model,ObjectType("Roof"));
  StandardsFilterObjectDoubleAttribute ufactor(
      roof,
      DoubleAttribute("UFactor"),
      RulesetNumericalPredicate(RulesetNumericalPredicate::LessThanOrEqualTo),
      0.065,
      "W/m^2*K");
  StandardsActionRuleSatisfied action(ufactor);
  StandardsAttributeCheckRule rule(options,qualifyingFilters,action);

  // save to database
  StandardsAttributeCheckRuleRecord ruleRecord(rule,database);
  EXPECT_EQ(1u, StandardsAttributeCheckRuleRecord::getRuleRecords(database).size());
  EXPECT_EQ(6u, ClauseRecord::getClauseRecords(database).size());

  // get out of database
  StandardsRule tempRule2 = StandardsAttributeCheckRuleRecord::getStandardsAttributeCheckRuleRecords(database)[0].standardsRule();
  ASSERT_TRUE(tempRule2.optionalCast<StandardsAttributeCheckRule>());
  EXPECT_FALSE(tempRule2.uuid().isNull());
  StandardsAttributeCheckRule rule2 = tempRule2.cast<StandardsAttributeCheckRule>();
  EXPECT_EQ(1u,rule2.getFilters<StandardsFilterAvailableModel>().size());
  EXPECT_EQ(2u,rule2.getFilters<StandardsFilterAvailableObject>().size());
  EXPECT_EQ(2u,rule2.getFilters<StandardsFilterObjectAttribute>().size());
  ASSERT_EQ(1u,rule2.actions().size());
  ASSERT_TRUE(rule2.actions()[0].optionalCast<StandardsActionRuleSatisfied>());
  BOOST_FOREACH(const FilterClause& temp,rule2.filters()) {
    StandardsFilterClause filter = temp.cast<StandardsFilterClause>();
    EXPECT_FALSE(filter.uuid().isNull());
    BOOST_FOREACH(const StandardsFilterClause& ref, filter.referencedFilters()) {
      EXPECT_TRUE(rule2.contains(ref)); // uses impl compare (not equivalent data)
      if (!rule2.contains(ref)) {
        LOG_FREE(Debug,"Project","rule2 filter: " << std::endl << filter.print());
        LOG_FREE(Debug,"Project","referenced filter not found in rule2: " << std::endl << ref.print());
      }
    }
  }
  StandardsActionRuleSatisfied action2 = rule2.actions()[0].cast<StandardsActionRuleSatisfied>();
  EXPECT_FALSE(action2.uuid().isNull());
  ASSERT_EQ(1u,action2.referencedFilters().size());
  EXPECT_TRUE(action2.referencedFilters()[0] == action2.attributeTest());
  EXPECT_TRUE(rule2.contains(action2.attributeTest()));
  if (!rule2.contains(action2.attributeTest())) {
    LOG_FREE(Debug,"Project","rule2 action: " << std::endl << action2.print());
    LOG_FREE(Debug,"Project","referenced filter not found in rule2: " << std::endl << action2.attributeTest().print());
  }
  StandardsFilterAvailableObjectVector objects = rule2.getFilters<StandardsFilterAvailableObject>();
  ASSERT_EQ(2u,objects.size());
  EXPECT_TRUE(objects[0].model());
  EXPECT_TRUE(objects[0].model() == objects[1].model());
}
