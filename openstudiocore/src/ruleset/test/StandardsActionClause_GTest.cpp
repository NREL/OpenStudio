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

#include <ruleset/StandardsActionClause.hpp>
#include <ruleset/StandardsActionCloneModel.hpp>

#include <ruleset/StandardsFilterAvailableModel.hpp>
#include <ruleset/StandardsFilterAvailableModel_Impl.hpp>

using namespace openstudio::ruleset;

TEST_F(RulesetFixture,StandardsActionClause_StandardsActionCloneModel) {
  StandardsFilterAvailableModel filter(StandardsRulesetModelType::Original);
  StandardsActionCloneModel action(filter,StandardsRulesetModelType::Proposed);
  EXPECT_TRUE(action.modelToClone() == filter);
  ASSERT_TRUE(action.modelToClone().modelType());
  EXPECT_TRUE(action.modelToClone().modelType().get() == StandardsRulesetModelType::Original);
  EXPECT_TRUE(action.modelTypeOfClone() == StandardsRulesetModelType::Proposed);

  StandardsFilterClauseVector refs = action.referencedFilters();
  ASSERT_EQ(1u,refs.size());
  EXPECT_TRUE(refs[0] == filter);
  EXPECT_TRUE(refs[0].optionalCast<StandardsFilterAvailableModel>());

  EXPECT_FALSE(action.print().empty());
}
