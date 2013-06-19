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
#include <project/ModelRulesetRecord.hpp>
#include <project/ModelRuleRecord.hpp>
#include <project/ModelObjectFilterTypeRecord.hpp>

#include <ruleset/ModelRuleset.hpp>
#include <ruleset/ModelRule.hpp>
#include <ruleset/ModelRule_Impl.hpp>
#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectFilterType_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::project;

TEST_F(ProjectFixture, ModelRulesetRecord)
{
  ProjectDatabase database = getCleanDatabase("ModelRulesetRecord");

  ModelObjectFilterType filter(IddObjectType::Zone);

  ModelRule rule("rule");
  rule.add(filter);

  ModelRuleset ruleset("ruleset");
  ruleset.add(rule);

  ModelRulesetRecord rulesetRecord(ruleset, database);
  EXPECT_TRUE(ruleset.uuid() == rulesetRecord.handle());
  EXPECT_EQ(ruleset.name(), rulesetRecord.name());

  ModelRuleset ruleset2 = rulesetRecord.modelRuleset();
  EXPECT_TRUE(ruleset.uuid() == ruleset2.uuid());
  EXPECT_EQ(ruleset.name(), ruleset2.name());

  std::vector<ModelRule> rules = ruleset2.rules();
  ASSERT_EQ(1u, rules.size());
  EXPECT_TRUE(rule.uuid() == rules[0].uuid());
  EXPECT_EQ(rule.name(), rules[0].name());

  std::vector<FilterClause> filters = rules[0].filters();
  ASSERT_EQ(1u, filters.size());
  EXPECT_TRUE(filter.uuid() == filters[0].uuid());
  ASSERT_TRUE(filters[0].optionalCast<ModelObjectFilterType>());
  EXPECT_EQ(filter.iddObjectType().value(), filters[0].cast<ModelObjectFilterType>().iddObjectType().value());

  ASSERT_EQ(1u, ModelRulesetRecord::getModelRulesetRecords(database).size());
  ASSERT_TRUE(ModelRulesetRecord::getModelRulesetRecord(rulesetRecord.id(), database));

  ModelRuleset ruleset3 = ModelRulesetRecord::getModelRulesetRecords(database)[0].modelRuleset();
  EXPECT_TRUE(ruleset.uuid() == ruleset3.uuid());
  EXPECT_EQ(ruleset.name(), ruleset3.name());

  rules = ruleset3.rules();
  ASSERT_EQ(1u, rules.size());
  EXPECT_TRUE(rule.uuid() == rules[0].uuid());
  EXPECT_EQ(rule.name(), rules[0].name());

  filters = rules[0].filters();
  ASSERT_EQ(1u, filters.size());
  EXPECT_TRUE(filter.uuid() == filters[0].uuid());
  ASSERT_TRUE(filters[0].optionalCast<ModelObjectFilterType>());
  EXPECT_EQ(filter.iddObjectType().value(), filters[0].cast<ModelObjectFilterType>().iddObjectType().value());
}
