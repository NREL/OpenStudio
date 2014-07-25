/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "ModelFixture.hpp"
#include "../ScheduleWeek.hpp"
#include "../ScheduleWeek_Impl.hpp"
#include "../ScheduleDay.hpp"
#include "../ScheduleDay_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"

#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, Schedule_Week)
{
  Model model;

  ScheduleDay schedule1(model);
  ScheduleDay schedule2(model);
  ScheduleDay schedule3(model);

  ScheduleWeek weekSchedule(model);
  EXPECT_FALSE(weekSchedule.mondaySchedule());
  EXPECT_FALSE(weekSchedule.sundaySchedule());
  EXPECT_FALSE(weekSchedule.holidaySchedule());

  EXPECT_TRUE(weekSchedule.setMondaySchedule(schedule1));
  ASSERT_TRUE(weekSchedule.mondaySchedule());
  EXPECT_EQ(schedule1.handle(), weekSchedule.mondaySchedule()->handle());
  EXPECT_FALSE(weekSchedule.sundaySchedule());
  EXPECT_FALSE(weekSchedule.holidaySchedule());

  EXPECT_TRUE(weekSchedule.setWeekendSchedule(schedule2));
  ASSERT_TRUE(weekSchedule.mondaySchedule());
  EXPECT_EQ(schedule1.handle(), weekSchedule.mondaySchedule()->handle());
  ASSERT_TRUE(weekSchedule.sundaySchedule());
  EXPECT_EQ(schedule2.handle(), weekSchedule.sundaySchedule()->handle());
  EXPECT_FALSE(weekSchedule.holidaySchedule());

  EXPECT_TRUE(weekSchedule.setAllSchedules(schedule3));
  ASSERT_TRUE(weekSchedule.mondaySchedule());
  EXPECT_EQ(schedule3.handle(), weekSchedule.mondaySchedule()->handle());
  ASSERT_TRUE(weekSchedule.sundaySchedule());
  EXPECT_EQ(schedule3.handle(), weekSchedule.sundaySchedule()->handle());
  ASSERT_TRUE(weekSchedule.holidaySchedule());
  EXPECT_EQ(schedule3.handle(), weekSchedule.holidaySchedule()->handle());
}
