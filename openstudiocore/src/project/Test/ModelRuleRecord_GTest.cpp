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

#include <ruleset/ModelRule.hpp>
#include <ruleset/ModelObjectFilterType.hpp>
#include <ruleset/ModelObjectFilterType_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>

using namespace openstudio;
using namespace openstudio::ruleset;
using namespace openstudio::project;

TEST_F(ProjectFixture, ModelRuleRecord)
{
  ProjectDatabase database = getCleanDatabase("ModelRuleRecord");

  ModelObjectFilterType filter(IddObjectType::Zone);
  ModelRule rule("rule");
  rule.add(filter);

  ModelRuleRecord ruleRecord(rule, database);
  EXPECT_TRUE(rule.uuid() == ruleRecord.handle());
  EXPECT_EQ(rule.name(), ruleRecord.name());

  ModelRule rule2 = ruleRecord.modelRule();
  EXPECT_TRUE(rule.uuid() == rule2.uuid());
  EXPECT_EQ(rule.name(), rule2.name());

  std::vector<FilterClause> filters = rule2.filters();
  ASSERT_EQ(1u, filters.size());
  EXPECT_TRUE(filter.uuid() == filters[0].uuid());
  ASSERT_TRUE(filters[0].optionalCast<ModelObjectFilterType>());
  EXPECT_EQ(filter.iddObjectType().value(), filters[0].cast<ModelObjectFilterType>().iddObjectType().value());

  ASSERT_EQ(1u, ModelRuleRecord::getModelRuleRecords(database).size());
  ASSERT_TRUE(ModelRuleRecord::getModelRuleRecord(ruleRecord.id(), database));

  ModelRule rule3 = ModelRuleRecord::getModelRuleRecords(database)[0].modelRule();
  EXPECT_TRUE(rule.uuid() == rule3.uuid());
  EXPECT_EQ(rule.name(), rule3.name());

  filters = rule3.filters();
  ASSERT_EQ(1u, filters.size());
  EXPECT_TRUE(filter.uuid() == filters[0].uuid());
  ASSERT_TRUE(filters[0].optionalCast<ModelObjectFilterType>());
  EXPECT_EQ(filter.iddObjectType().value(), filters[0].cast<ModelObjectFilterType>().iddObjectType().value());
}
