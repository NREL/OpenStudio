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

#include <ruleset/ModelObjectFilterClause.hpp>
#include <ruleset/ModelObjectFilterClause_Impl.hpp>
#include <ruleset/ModelObjectFilterStringAttribute.hpp>
#include <ruleset/ModelObjectFilterNumericAttribute.hpp>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>
#include <model/Space.hpp>

using namespace openstudio::ruleset;
using namespace openstudio::model;

TEST_F(RulesetFixture, ModelObjectFilterClause) {

  Model model;
  Space space(model);
  space.setName("Space 1");

  ModelObjectFilterStringAttribute nameEqualsFilter("name", RulesetStringPredicate::Equals, "Space 1");
  ModelObjectFilterStringAttribute nameNotEqualsFilter("name", RulesetStringPredicate::NotEquals, "Space 1");
  ModelObjectFilterStringAttribute nameIEqualsFilter("name", RulesetStringPredicate::IEquals, "space 1");
  ModelObjectFilterStringAttribute nameNotIEqualsFilter("name", RulesetStringPredicate::NotIEquals, "space 1");
  ModelObjectFilterStringAttribute nameMatchFilter("name", RulesetStringPredicate::Match, "^Space.*");
  ModelObjectFilterStringAttribute nameNotMatchFilter("name", RulesetStringPredicate::NotMatch, "^Space.*");

  EXPECT_TRUE(nameEqualsFilter.check(space));
  EXPECT_FALSE(nameNotEqualsFilter.check(space));
  EXPECT_TRUE(nameIEqualsFilter.check(space));
  EXPECT_FALSE(nameNotIEqualsFilter.check(space));
  EXPECT_TRUE(nameMatchFilter.check(space));
  EXPECT_FALSE(nameNotMatchFilter.check(space));

  // zone has no floor area
  ModelObjectFilterNumericAttribute floorAreaEqualsFilter("floorArea", RulesetNumericalPredicate::Equals, 0.0);
  ModelObjectFilterNumericAttribute floorAreaNotEqualsFilter("floorArea", RulesetNumericalPredicate::NotEquals, 0.0);
  ModelObjectFilterNumericAttribute floorAreaGreaterThanFilter("floorArea", RulesetNumericalPredicate::GreaterThan, 0.0);
  ModelObjectFilterNumericAttribute floorAreaGreaterThanOrEqualToFilter("floorArea", RulesetNumericalPredicate::GreaterThanOrEqualTo, 0.0);
  ModelObjectFilterNumericAttribute floorAreaLessThan("floorArea", RulesetNumericalPredicate::LessThan, 0.0);
  ModelObjectFilterNumericAttribute floorAreaLessThanOrEqualTo("floorArea", RulesetNumericalPredicate::LessThanOrEqualTo, 0.0);

  EXPECT_TRUE(floorAreaEqualsFilter.check(space));
  EXPECT_FALSE(floorAreaNotEqualsFilter.check(space));
  EXPECT_FALSE(floorAreaGreaterThanFilter.check(space));
  EXPECT_TRUE(floorAreaGreaterThanOrEqualToFilter.check(space));
  EXPECT_FALSE(floorAreaLessThan.check(space));
  EXPECT_TRUE(floorAreaLessThanOrEqualTo.check(space));
}
