/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../ScheduleDay.hpp"
#include "../ScheduleDay_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"

#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, Schedule_Day) {
  Model model;

  ScheduleDay daySchedule(model);

  // initialized with 0 to hour 24
  EXPECT_EQ(1u, daySchedule.times().size());
  EXPECT_EQ(Time(0, 24, 0), daySchedule.times()[0]);
  EXPECT_EQ(1u, daySchedule.values().size());
  EXPECT_EQ(0.0, daySchedule.values()[0]);

  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, -1, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, 0, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, 6, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, 8, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, 9, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, 17, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, 18, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, 24, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(1, 0, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(1, 1, 0)));

  // schedule is 1.0 until 17:00
  EXPECT_TRUE(daySchedule.addValue(Time(0, 17, 0), 1.0));
  ASSERT_EQ(2u, daySchedule.times().size());
  EXPECT_EQ(Time(0, 17, 0), daySchedule.times()[0]);
  EXPECT_EQ(Time(0, 24, 0), daySchedule.times()[1]);
  ASSERT_EQ(2u, daySchedule.values().size());
  EXPECT_EQ(1.0, daySchedule.values()[0]);
  EXPECT_EQ(0.0, daySchedule.values()[1]);

  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, -1, 0)));
  EXPECT_EQ(1.0, daySchedule.getValue(Time(0, 0, 0)));
  EXPECT_EQ(1.0, daySchedule.getValue(Time(0, 6, 0)));
  EXPECT_EQ(1.0, daySchedule.getValue(Time(0, 8, 0)));
  EXPECT_EQ(1.0, daySchedule.getValue(Time(0, 9, 0)));
  EXPECT_EQ(1.0, daySchedule.getValue(Time(0, 17, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, 18, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, 24, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(1, 0, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(1, 1, 0)));

  // schedule is 0.2 until 8:00, 1.0 until 17:00
  EXPECT_TRUE(daySchedule.addValue(Time(0, 8, 0), 0.2));

  ASSERT_EQ(3u, daySchedule.times().size());
  EXPECT_EQ(Time(0, 8, 0), daySchedule.times()[0]);
  EXPECT_EQ(Time(0, 17, 0), daySchedule.times()[1]);
  EXPECT_EQ(Time(0, 24, 0), daySchedule.times()[2]);
  ASSERT_EQ(3u, daySchedule.values().size());
  EXPECT_EQ(0.2, daySchedule.values()[0]);
  EXPECT_EQ(1.0, daySchedule.values()[1]);
  EXPECT_EQ(0.0, daySchedule.values()[2]);

  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, -1, 0)));
  EXPECT_EQ(0.2, daySchedule.getValue(Time(0, 0, 0)));
  EXPECT_EQ(0.2, daySchedule.getValue(Time(0, 6, 0)));
  EXPECT_EQ(0.2, daySchedule.getValue(Time(0, 8, 0)));
  EXPECT_EQ(1.0, daySchedule.getValue(Time(0, 9, 0)));
  EXPECT_EQ(1.0, daySchedule.getValue(Time(0, 17, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, 18, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, 24, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(1, 0, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(1, 1, 0)));

  // schedule is 0.2 until 8:00, 1.0 until 17:00, and 0.5 until 24:00
  EXPECT_TRUE(daySchedule.addValue(Time(0, 24, 0), 0.5));

  ASSERT_EQ(3u, daySchedule.times().size());
  EXPECT_EQ(Time(0, 8, 0), daySchedule.times()[0]);
  EXPECT_EQ(Time(0, 17, 0), daySchedule.times()[1]);
  EXPECT_EQ(Time(0, 24, 0), daySchedule.times()[2]);
  ASSERT_EQ(3u, daySchedule.values().size());
  EXPECT_EQ(0.2, daySchedule.values()[0]);
  EXPECT_EQ(1.0, daySchedule.values()[1]);
  EXPECT_EQ(0.5, daySchedule.values()[2]);

  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, -1, 0)));
  EXPECT_EQ(0.2, daySchedule.getValue(Time(0, 0, 0)));
  EXPECT_EQ(0.2, daySchedule.getValue(Time(0, 6, 0)));
  EXPECT_EQ(0.2, daySchedule.getValue(Time(0, 8, 0)));
  EXPECT_EQ(1.0, daySchedule.getValue(Time(0, 9, 0)));
  EXPECT_EQ(1.0, daySchedule.getValue(Time(0, 17, 0)));
  EXPECT_EQ(0.5, daySchedule.getValue(Time(0, 18, 0)));
  EXPECT_EQ(0.5, daySchedule.getValue(Time(0, 24, 0)));
  EXPECT_EQ(0.5, daySchedule.getValue(Time(1, 0, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(1, 1, 0)));

  // schedule is 0.1 until 8:00, 0.9 until 17:00, and 0.4 until 24:00
  EXPECT_TRUE(daySchedule.addValue(Time(0, 8, 0), 0.1));
  EXPECT_TRUE(daySchedule.addValue(Time(0, 17, 0), 0.9));
  EXPECT_TRUE(daySchedule.addValue(Time(0, 24, 0), 0.4));

  ASSERT_EQ(3u, daySchedule.times().size());
  EXPECT_EQ(Time(0, 8, 0), daySchedule.times()[0]);
  EXPECT_EQ(Time(0, 17, 0), daySchedule.times()[1]);
  EXPECT_EQ(Time(0, 24, 0), daySchedule.times()[2]);
  ASSERT_EQ(3u, daySchedule.values().size());
  EXPECT_EQ(0.1, daySchedule.values()[0]);
  EXPECT_EQ(0.9, daySchedule.values()[1]);
  EXPECT_EQ(0.4, daySchedule.values()[2]);

  EXPECT_EQ(0.0, daySchedule.getValue(Time(0, -1, 0)));
  EXPECT_EQ(0.1, daySchedule.getValue(Time(0, 0, 0)));
  EXPECT_EQ(0.1, daySchedule.getValue(Time(0, 6, 0)));
  EXPECT_EQ(0.1, daySchedule.getValue(Time(0, 8, 0)));
  EXPECT_EQ(0.9, daySchedule.getValue(Time(0, 9, 0)));
  EXPECT_EQ(0.9, daySchedule.getValue(Time(0, 17, 0)));
  EXPECT_EQ(0.4, daySchedule.getValue(Time(0, 18, 0)));
  EXPECT_EQ(0.4, daySchedule.getValue(Time(0, 24, 0)));
  EXPECT_EQ(0.4, daySchedule.getValue(Time(1, 0, 0)));
  EXPECT_EQ(0.0, daySchedule.getValue(Time(1, 1, 0)));

  // can't set values out of day range
  EXPECT_FALSE(daySchedule.addValue(Time(0, 25, 0), -9999));
  EXPECT_EQ(3u, daySchedule.times().size());
  EXPECT_EQ(3u, daySchedule.values().size());

  EXPECT_FALSE(daySchedule.addValue(Time(0, 0, 0), -9999));
  EXPECT_EQ(3u, daySchedule.times().size());
  EXPECT_EQ(3u, daySchedule.values().size());

  EXPECT_FALSE(daySchedule.addValue(Time(0, -1, 0), -9999));
  EXPECT_EQ(3u, daySchedule.times().size());
  EXPECT_EQ(3u, daySchedule.values().size());

  // Add small values
  daySchedule.clearValues();
  EXPECT_EQ(1u, daySchedule.times().size());
  EXPECT_EQ(1u, daySchedule.values().size());
  EXPECT_EQ(Time(0, 24, 0), daySchedule.times().front());

  EXPECT_FALSE(daySchedule.addValue(Time(0, 0, 0, 10), -9999));
  EXPECT_EQ(1u, daySchedule.times().size());
  EXPECT_EQ(1u, daySchedule.values().size());

  daySchedule.clearValues();
  // 30 seconds is not accepted
  EXPECT_FALSE(daySchedule.addValue(Time(0, 0, 0, 30), -9999));
  EXPECT_EQ(1u, daySchedule.times().size());
  EXPECT_EQ(1u, daySchedule.values().size());

  daySchedule.clearValues();
  EXPECT_TRUE(daySchedule.addValue(Time(0, 0, 0, 31), -9999));
  EXPECT_EQ(2u, daySchedule.times().size());
  EXPECT_EQ(2u, daySchedule.values().size());
  // 31 seconds becomes 1 minute
  EXPECT_EQ(Time(0, 0, 1), daySchedule.times().front());
  EXPECT_EQ(Time(0, 24, 0), daySchedule.times().back());
}

TEST_F(ModelFixture, Schedule_Day_Interp) {
  Model model;

  double tol = 1e-5;

  ScheduleDay daySchedule(model);
  EXPECT_FALSE(daySchedule.interpolatetoTimestep());
  EXPECT_TRUE(daySchedule.isInterpolatetoTimestepDefaulted());

  // schedule is 1 until 12:00
  EXPECT_TRUE(daySchedule.addValue(Time(0, 12, 0), 1.0));

  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, -1, 0)), tol);
  EXPECT_NEAR(1.0, daySchedule.getValue(Time(0, 0, 0)), tol);
  EXPECT_NEAR(1.0, daySchedule.getValue(Time(0, 6, 0)), tol);
  EXPECT_NEAR(1.0, daySchedule.getValue(Time(0, 12, 0)), tol);
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 18, 0)), tol);
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 24, 0)), tol);
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 25, 0)), tol);

  daySchedule.setInterpolatetoTimestep(true);
  EXPECT_TRUE(daySchedule.interpolatetoTimestep());
  EXPECT_FALSE(daySchedule.isInterpolatetoTimestepDefaulted());

  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, -1, 0)), tol);
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 0, 0)), tol);  // FIXME: Fails. Should be 1.0?
  EXPECT_NEAR(0.5, daySchedule.getValue(Time(0, 6, 0)), tol);  // FIXME: Fails. Should be 1.0?
  EXPECT_NEAR(1.0, daySchedule.getValue(Time(0, 12, 0)), tol);
  EXPECT_NEAR(0.5, daySchedule.getValue(Time(0, 18, 0)), tol);  // FIXME: Fails. Should be 0.0?
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 24, 0)), tol);
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 25, 0)), tol);
}

TEST_F(ModelFixture, Schedule_Day_Remove) {
  Model model;

  ScheduleDay daySchedule(model);

  std::vector<openstudio::Time> times = daySchedule.times();
  std::vector<double> values = daySchedule.values();
  ASSERT_EQ(1u, times.size());
  ASSERT_EQ(1u, values.size());
  EXPECT_EQ(1.0, times[0].totalDays());
  EXPECT_EQ(0.0, values[0]);

  // schedule is 1 until 24:00
  EXPECT_TRUE(daySchedule.addValue(Time(1, 0, 0), 1.0));

  times = daySchedule.times();
  values = daySchedule.values();
  ASSERT_EQ(1u, times.size());
  ASSERT_EQ(1u, values.size());
  EXPECT_EQ(1.0, times[0].totalDays());
  EXPECT_EQ(1.0, values[0]);

  boost::optional<double> removed = daySchedule.removeValue(openstudio::Time(0.5));
  EXPECT_FALSE(removed);

  times = daySchedule.times();
  values = daySchedule.values();
  ASSERT_EQ(1u, times.size());
  ASSERT_EQ(1u, values.size());
  EXPECT_EQ(1.0, times[0].totalDays());
  EXPECT_EQ(1.0, values[0]);

  removed = daySchedule.removeValue(openstudio::Time(1.0));
  ASSERT_TRUE(removed);
  EXPECT_EQ(1.0, *removed);

  times = daySchedule.times();
  values = daySchedule.values();
  ASSERT_EQ(1u, times.size());
  ASSERT_EQ(1u, values.size());
  EXPECT_EQ(1.0, times[0].totalDays());
  EXPECT_EQ(0.0, values[0]);

  // schedule is 1 until 12:00 then 2 until 24:00
  EXPECT_TRUE(daySchedule.addValue(Time(0, 12, 0), 1.0));
  EXPECT_TRUE(daySchedule.addValue(Time(1, 0, 0), 2.0));

  times = daySchedule.times();
  values = daySchedule.values();
  ASSERT_EQ(2u, times.size());
  ASSERT_EQ(2u, values.size());
  EXPECT_EQ(0.5, times[0].totalDays());
  EXPECT_EQ(1.0, values[0]);
  EXPECT_EQ(1.0, times[1].totalDays());
  EXPECT_EQ(2.0, values[1]);

  removed = daySchedule.removeValue(openstudio::Time(0.4));
  EXPECT_FALSE(removed);

  times = daySchedule.times();
  values = daySchedule.values();
  ASSERT_EQ(2u, times.size());
  ASSERT_EQ(2u, values.size());
  EXPECT_EQ(0.5, times[0].totalDays());
  EXPECT_EQ(1.0, values[0]);
  EXPECT_EQ(1.0, times[1].totalDays());
  EXPECT_EQ(2.0, values[1]);

  removed = daySchedule.removeValue(openstudio::Time(0.5));
  ASSERT_TRUE(removed);
  EXPECT_EQ(1.0, *removed);

  times = daySchedule.times();
  values = daySchedule.values();
  ASSERT_EQ(1u, times.size());
  ASSERT_EQ(1u, values.size());
  EXPECT_EQ(1.0, times[0].totalDays());
  EXPECT_EQ(2.0, values[0]);
}

TEST_F(ModelFixture, Schedule_Day_Clone) {
  Model model;

  ScheduleTypeLimits limits(model);

  ScheduleDay daySchedule(model);
  daySchedule.setScheduleTypeLimits(limits);

  ASSERT_EQ(1u, daySchedule.resources().size());
  EXPECT_EQ(limits.handle(), daySchedule.resources()[0].handle());

  EXPECT_EQ(1u, model.getConcreteModelObjects<ScheduleTypeLimits>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<ScheduleDay>().size());

  auto daySchedule2 = daySchedule.clone(model).cast<ScheduleDay>();

  EXPECT_EQ(1u, model.getConcreteModelObjects<ScheduleTypeLimits>().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<ScheduleDay>().size());
  ASSERT_TRUE(daySchedule.scheduleTypeLimits());
  ASSERT_TRUE(daySchedule2.scheduleTypeLimits());
  EXPECT_EQ(limits.handle(), daySchedule.scheduleTypeLimits()->handle());
  EXPECT_EQ(daySchedule.scheduleTypeLimits()->handle(), daySchedule2.scheduleTypeLimits()->handle());
}

// Test that addValue (which doesn't call IdfObject::setDouble...) will not affect NaNs/Infinity
TEST_F(ModelFixture, Schedule_Day_addValue_NaN_Infinity) {
  Model model;

  ScheduleDay sch_day(model);

  Time t(0, 6, 0, 0);
  EXPECT_FALSE(sch_day.addValue(t, std::numeric_limits<double>::quiet_NaN()));
  EXPECT_FALSE(sch_day.addValue(t, std::numeric_limits<double>::infinity()));
  EXPECT_FALSE(sch_day.addValue(t, -std::numeric_limits<double>::infinity()));
  EXPECT_TRUE(sch_day.addValue(t, 1.0));
  EXPECT_FALSE(sch_day.addValue(t, std::numeric_limits<double>::quiet_NaN()));
  EXPECT_FALSE(sch_day.addValue(t, std::numeric_limits<double>::infinity()));
  EXPECT_FALSE(sch_day.addValue(t, -std::numeric_limits<double>::infinity()));
}

TEST_F(ModelFixture, Schedule_Day_getValues) {
  Model model;

  double tol = 1e-5;

  ScheduleDay sch_day(model);

  Time t1("08:15:00");
  sch_day.addValue(t1, 0);
  Time t2("21:45:00");
  sch_day.addValue(t2, 1);

  std::vector<openstudio::Time> times = sch_day.getTimes(7);
  EXPECT_EQ(0, times.size());

  std::vector<openstudio::Time> times24 = sch_day.getTimes(1);
  EXPECT_EQ(24 * 1, times24.size());

  std::vector<openstudio::Time> times15 = sch_day.getTimes(4);
  EXPECT_EQ(24 * 4, times15.size());

  std::vector<openstudio::Time> times10 = sch_day.getTimes(6);
  EXPECT_EQ(24 * 6, times10.size());
  Time times10_1("00:10:00");
  EXPECT_EQ(times10_1, times10[0]);
  Time times10_48("08:00:00");
  EXPECT_EQ(times10_48, times10[47]);
  Time times10_49("08:10:00");
  EXPECT_EQ(times10_49, times10[48]);
  Time times10_50("08:20:00");
  EXPECT_EQ(times10_50, times10[49]);
  Time times10_144("24:00:00");
  EXPECT_EQ(times10_144, times10[143]);

  std::vector<double> values = sch_day.getValues(18);
  EXPECT_EQ(0, values.size());

  std::vector<double> values24 = sch_day.getValues(1);
  EXPECT_EQ(24 * 1, values24.size());

  std::vector<double> values15 = sch_day.getValues(4);
  EXPECT_EQ(24 * 4, values15.size());

  std::vector<double> values10 = sch_day.getValues(6);
  EXPECT_EQ(24 * 6, values10.size());
  EXPECT_DOUBLE_EQ(0.0, values10[0]);
  EXPECT_DOUBLE_EQ(0.0, values10[47]);
  EXPECT_DOUBLE_EQ(0.0, values10[48]);
  EXPECT_DOUBLE_EQ(1.0, values10[49]);
  EXPECT_DOUBLE_EQ(0.0, values10[143]);

  double value;

  Time t3("08:10:00");
  EXPECT_NEAR((8.0 + (10.0 / 60.0)) / 24.0, t3.totalDays(), tol);
  value = sch_day.getValue(t3);
  EXPECT_DOUBLE_EQ(0.0, value);
  value = sch_day.getValue(t3, 1);
  EXPECT_NEAR(10.0 / 60.0, value, tol);  // 10 min in a 60 min interval
  value = sch_day.getValue(t3, 4);
  EXPECT_DOUBLE_EQ(0.0, value);
  value = sch_day.getValue(t3, 6);
  EXPECT_DOUBLE_EQ(0.0, value);
  value = sch_day.getValue(t3, 10);
  EXPECT_DOUBLE_EQ(0.0, value);

  Time t4("08:18:00");
  EXPECT_NEAR((8.0 + (18.0 / 60.0)) / 24.0, t4.totalDays(), tol);
  value = sch_day.getValue(t4);
  EXPECT_DOUBLE_EQ(1.0, value);
  value = sch_day.getValue(t4, 1);
  EXPECT_NEAR(18.0 / 60.0, value, tol);  // 18 min in a 60 min interval
  value = sch_day.getValue(t4, 4);
  EXPECT_NEAR(3.0 / 15.0, value, tol);  // 3 min in a 15 min interval
  value = sch_day.getValue(t4, 6);
  EXPECT_NEAR(8.0 / 10.0, value, tol);  // 8 min in a 10 min interval
  value = sch_day.getValue(t4, 10);
  EXPECT_DOUBLE_EQ(1.0, value);

  Time t5("09:00:00");
  EXPECT_NEAR(9.0 / 24.0, t5.totalDays(), tol);
  value = sch_day.getValue(t5);
  EXPECT_DOUBLE_EQ(1.0, value);
  value = sch_day.getValue(t5, 1);
  EXPECT_DOUBLE_EQ(1.0, value);
  value = sch_day.getValue(t5, 4);
  EXPECT_DOUBLE_EQ(1.0, value);
  value = sch_day.getValue(t5, 6);
  EXPECT_DOUBLE_EQ(1.0, value);
  value = sch_day.getValue(t5, 10);
  EXPECT_DOUBLE_EQ(1.0, value);
}
