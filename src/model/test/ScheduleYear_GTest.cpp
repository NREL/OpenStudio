/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../ScheduleYear.hpp"
#include "../ScheduleYear_Impl.hpp"
#include "../ScheduleWeek.hpp"
#include "../ScheduleWeek_Impl.hpp"
#include "../YearDescription.hpp"
#include "../YearDescription_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"

#include "../../utilities/core/UUID.hpp"
#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, Schedule_Year) {
  Model model;

  openstudio::model::YearDescription yd = model.getUniqueModelObject<openstudio::model::YearDescription>();

  ScheduleWeek weekSchedule1(model);
  ScheduleWeek weekSchedule2(model);
  ScheduleWeek weekSchedule3(model);

  // clear the schedule
  ScheduleYear yearSchedule(model);

  EXPECT_EQ(0u, yearSchedule.dates().size());
  EXPECT_EQ(0u, yearSchedule.scheduleWeeks().size());

  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(1, 1)));
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(6, 1)));
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(12, 31)));

  // until 3/1 weekSchedule1
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(3, 1), weekSchedule1));

  ASSERT_EQ(1u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(3, 1), yearSchedule.dates()[0]);
  ASSERT_EQ(1u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.scheduleWeeks()[0].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1, 1)));
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1, 1))->handle());
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(6, 1)));
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(12, 31)));

  // until 3/1 weekSchedule1, until 7/1 weekSchedule2
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(7, 1), weekSchedule2));

  ASSERT_EQ(2u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(3, 1), yearSchedule.dates()[0]);
  EXPECT_EQ(yd.makeDate(7, 1), yearSchedule.dates()[1]);
  ASSERT_EQ(2u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.scheduleWeeks()[0].handle());
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.scheduleWeeks()[1].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1, 1)));
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1, 1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(6, 1)));
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.getScheduleWeek(yd.makeDate(6, 1))->handle());
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(12, 31)));

  // until 3/1 weekSchedule1, until 7/1 weekSchedule2, until 12/31 weekSchedule3
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(12, 31), weekSchedule3));

  ASSERT_EQ(3u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(3, 1), yearSchedule.dates()[0]);
  EXPECT_EQ(yd.makeDate(7, 1), yearSchedule.dates()[1]);
  EXPECT_EQ(yd.makeDate(12, 31), yearSchedule.dates()[2]);
  ASSERT_EQ(3u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.scheduleWeeks()[0].handle());
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.scheduleWeeks()[1].handle());
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.scheduleWeeks()[2].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1, 1)));
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1, 1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(6, 1)));
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.getScheduleWeek(yd.makeDate(6, 1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(12, 31)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(12, 31))->handle());

  // clear the schedule
  yearSchedule.clearScheduleWeeks();
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(1, 1)));
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(6, 1)));
  EXPECT_FALSE(yearSchedule.getScheduleWeek(yd.makeDate(12, 31)));

  // until 12/31 weekSchedule3
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(12, 31), weekSchedule3));

  ASSERT_EQ(1u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(12, 31), yearSchedule.dates()[0]);
  ASSERT_EQ(1u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.scheduleWeeks()[0].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1, 1)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1, 1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(6, 1)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(6, 1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(12, 31)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(12, 31))->handle());

  // until 7/1 weekSchedule2, until 12/31 weekSchedule3
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(7, 1), weekSchedule2));

  ASSERT_EQ(2u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(7, 1), yearSchedule.dates()[0]);
  EXPECT_EQ(yd.makeDate(12, 31), yearSchedule.dates()[1]);
  ASSERT_EQ(2u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.scheduleWeeks()[0].handle());
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.scheduleWeeks()[1].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1, 1)));
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1, 1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(6, 1)));
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.getScheduleWeek(yd.makeDate(6, 1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(12, 31)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(12, 31))->handle());

  // until 3/1 weekSchedule1, until 7/1 weekSchedule2, until 12/31 weekSchedule3
  EXPECT_TRUE(yearSchedule.addScheduleWeek(yd.makeDate(3, 1), weekSchedule1));

  ASSERT_EQ(3u, yearSchedule.dates().size());
  EXPECT_EQ(yd.makeDate(3, 1), yearSchedule.dates()[0]);
  EXPECT_EQ(yd.makeDate(7, 1), yearSchedule.dates()[1]);
  EXPECT_EQ(yd.makeDate(12, 31), yearSchedule.dates()[2]);
  ASSERT_EQ(3u, yearSchedule.scheduleWeeks().size());
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.scheduleWeeks()[0].handle());
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.scheduleWeeks()[1].handle());
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.scheduleWeeks()[2].handle());

  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(1, 1)));
  EXPECT_EQ(weekSchedule1.handle(), yearSchedule.getScheduleWeek(yd.makeDate(1, 1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(6, 1)));
  EXPECT_EQ(weekSchedule2.handle(), yearSchedule.getScheduleWeek(yd.makeDate(6, 1))->handle());
  ASSERT_TRUE(yearSchedule.getScheduleWeek(yd.makeDate(12, 31)));
  EXPECT_EQ(weekSchedule3.handle(), yearSchedule.getScheduleWeek(yd.makeDate(12, 31))->handle());
}
