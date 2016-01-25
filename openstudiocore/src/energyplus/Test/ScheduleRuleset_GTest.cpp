/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../utilities/data/TimeSeries.hpp"

#include "../../model/Model.hpp"
#include "../../model/ScheduleRuleset.hpp"
#include "../../model/ScheduleRuleset_Impl.hpp"
#include "../../model/ScheduleRule.hpp"
#include "../../model/ScheduleRule_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/Schedule_Year_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_ScheduleRuleset_Simple)
{
  Model model;
  ScheduleRuleset scheduleRuleset(model);

  // annual weekday rule
  ScheduleRule rule2(scheduleRuleset);
  rule2.setApplySunday(false);
  rule2.setApplyMonday(true);
  rule2.setApplyTuesday(true);
  rule2.setApplyWednesday(true);
  rule2.setApplyThursday(true);
  rule2.setApplyFriday(true);
  rule2.setApplySaturday(false);

  // annual weekend rule
  ScheduleRule rule1(scheduleRuleset);
  rule1.setApplySunday(true);
  rule1.setApplyMonday(false);
  rule1.setApplyTuesday(false);
  rule1.setApplyWednesday(false);
  rule1.setApplyThursday(false);
  rule1.setApplyFriday(false);
  rule1.setApplySaturday(true);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
  ASSERT_EQ(1u, scheduleYears.size());
  std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
  ASSERT_EQ(2u, extensibleGroups.size());

  // based on 2009 assumed year

  // 1/1-1/3
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(3, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 1/4-12/31
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(4, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(31, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ScheduleRuleset_Bug804)
{
  Model model;
  ScheduleRuleset scheduleRuleset(model);

  // annual weekday rule
  ScheduleRule rule4(scheduleRuleset);
  rule4.setApplySunday(false);
  rule4.setApplyMonday(true);
  rule4.setApplyTuesday(true);
  rule4.setApplyWednesday(true);
  rule4.setApplyThursday(true);
  rule4.setApplyFriday(true);
  rule4.setApplySaturday(false);

  // annual weekend rule
  ScheduleRule rule3(scheduleRuleset);
  rule3.setApplySunday(true);
  rule3.setApplyMonday(false);
  rule3.setApplyTuesday(false);
  rule3.setApplyWednesday(false);
  rule3.setApplyThursday(false);
  rule3.setApplyFriday(false);
  rule3.setApplySaturday(true);

  // special weekday rule
  ScheduleRule rule2(scheduleRuleset);
  rule2.setApplySunday(false);
  rule2.setApplyMonday(true);
  rule2.setApplyTuesday(true);
  rule2.setApplyWednesday(true);
  rule2.setApplyThursday(true);
  rule2.setApplyFriday(true);
  rule2.setApplySaturday(false);
  rule2.setStartDate(Date(5,1));
  rule2.setEndDate(Date(12,13));

  // special weekend rule
  ScheduleRule rule1(scheduleRuleset);
  rule1.setApplySunday(true);
  rule1.setApplyMonday(false);
  rule1.setApplyTuesday(false);
  rule1.setApplyWednesday(false);
  rule1.setApplyThursday(false);
  rule1.setApplyFriday(false);
  rule1.setApplySaturday(true);
  rule1.setStartDate(Date(5,1));
  rule1.setEndDate(Date(12,13));

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> scheduleYears = workspace.getObjectsByType(IddObjectType::Schedule_Year);
  ASSERT_EQ(1u, scheduleYears.size());
  std::vector<IdfExtensibleGroup> extensibleGroups = scheduleYears[0].extensibleGroups();
  ASSERT_EQ(6u, extensibleGroups.size());

  // based on 2009 assumed year

  // 1/1-1/3
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(1, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(3, extensibleGroups[0].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 1/4-4/25
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(1, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(4, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(4, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(25, extensibleGroups[1].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 4/26-5/2
  ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(4, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(26, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(5, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(2, extensibleGroups[2].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 5/3-12/12
  ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(5, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(3, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(12, extensibleGroups[3].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 12/13-12/19
  ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(12, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(13, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(19, extensibleGroups[4].getInt(Schedule_YearExtensibleFields::EndDay).get());

  // 12/20-12/31
  ASSERT_TRUE(extensibleGroups[5].getInt(Schedule_YearExtensibleFields::StartMonth));
  EXPECT_EQ(12, extensibleGroups[5].getInt(Schedule_YearExtensibleFields::StartMonth).get());
  ASSERT_TRUE(extensibleGroups[5].getInt(Schedule_YearExtensibleFields::StartDay));
  EXPECT_EQ(20, extensibleGroups[5].getInt(Schedule_YearExtensibleFields::StartDay).get());
  ASSERT_TRUE(extensibleGroups[5].getInt(Schedule_YearExtensibleFields::EndMonth));
  EXPECT_EQ(12, extensibleGroups[5].getInt(Schedule_YearExtensibleFields::EndMonth).get());
  ASSERT_TRUE(extensibleGroups[5].getInt(Schedule_YearExtensibleFields::EndDay));
  EXPECT_EQ(31, extensibleGroups[5].getInt(Schedule_YearExtensibleFields::EndDay).get());

}


