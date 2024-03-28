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
#include "../Timestep.hpp"
#include "../Timestep_Impl.hpp"

#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"
#include "../../utilities/data/TimeSeries.hpp"

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
  EXPECT_EQ(1.0, daySchedule.getValue(Time(0, 0, 1)));
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
  EXPECT_EQ("No", daySchedule.interpolatetoTimestep());
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

  EXPECT_TRUE(daySchedule.setInterpolatetoTimestep("Linear"));
  EXPECT_EQ("Linear", daySchedule.interpolatetoTimestep());
  EXPECT_FALSE(daySchedule.isInterpolatetoTimestepDefaulted());

  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, -1, 0)), tol);
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 0, 0)), tol);
  EXPECT_NEAR(0.013890, daySchedule.getValue(Time(0, 0, 1)), tol);
  EXPECT_NEAR(0.5, daySchedule.getValue(Time(0, 6, 0)), tol);
  EXPECT_NEAR(1.0, daySchedule.getValue(Time(0, 12, 0)), tol);
  EXPECT_NEAR(0.5, daySchedule.getValue(Time(0, 18, 0)), tol);
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 24, 0)), tol);
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 25, 0)), tol);

  daySchedule.setInterpolatetoTimestep("Average");
  EXPECT_EQ("Average", daySchedule.interpolatetoTimestep());
  EXPECT_FALSE(daySchedule.isInterpolatetoTimestepDefaulted());

  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, -1, 0)), tol);
  EXPECT_NEAR(1.0, daySchedule.getValue(Time(0, 0, 0)), tol);
  EXPECT_NEAR(1.0, daySchedule.getValue(Time(0, 0, 1)), tol);
  EXPECT_NEAR(1.0, daySchedule.getValue(Time(0, 6, 0)), tol);
  EXPECT_NEAR(1.0, daySchedule.getValue(Time(0, 12, 0)), tol);
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 18, 0)), tol);
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

TEST_F(ModelFixture, Schedule_Day_timeSeries) {
  Model model;

  double tol = 1e-5;

  ScheduleDay sch_day(model);

  Time t1("08:05:00");
  sch_day.addValue(t1, 0.1);
  Time t2("21:45:00");
  sch_day.addValue(t2, 1.0);

  Time t0800("08:00:00");
  EXPECT_NEAR(8.0 / 24.0, t0800.totalDays(), tol);
  Time t0805("08:05:00");
  EXPECT_NEAR((8.0 + (5.0 / 60.0)) / 24.0, t0805.totalDays(), tol);
  Time t0810("08:10:00");
  EXPECT_NEAR((8.0 + (10.0 / 60.0)) / 24.0, t0810.totalDays(), tol);
  Time t0818("08:18:00");
  EXPECT_NEAR((8.0 + (18.0 / 60.0)) / 24.0, t0818.totalDays(), tol);
  Time t0820("08:20:00");
  EXPECT_NEAR((8.0 + (20.0 / 60.0)) / 24.0, t0820.totalDays(), tol);
  Time t0900("09:00:00");
  EXPECT_NEAR(9.0 / 24.0, t0900.totalDays(), tol);

  {  // Interpolate to Timestep = No
    EXPECT_TRUE(sch_day.setInterpolatetoTimestep("No"));
    EXPECT_EQ("No", sch_day.interpolatetoTimestep());

    auto timestep = model.getUniqueModelObject<Timestep>();

    openstudio::TimeSeries timeSeries;
    double value;

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(1));
    timeSeries = sch_day.timeSeries();
    EXPECT_EQ(24 * 1, timeSeries.dateTimes().size());
    EXPECT_EQ(24 * 1, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.1, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(1.0, value, tol);

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(4));
    timeSeries = sch_day.timeSeries();
    EXPECT_EQ(24 * 4, timeSeries.dateTimes().size());
    EXPECT_EQ(24 * 4, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.1, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(1.0, value, tol);

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(6));
    timeSeries = sch_day.timeSeries();
    ASSERT_EQ(24 * 6, timeSeries.dateTimes().size());
    ASSERT_EQ(24 * 6, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.1, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(1.0, value, tol);

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(10));
    timeSeries = sch_day.timeSeries();
    EXPECT_EQ(24 * 10, timeSeries.dateTimes().size());
    EXPECT_EQ(24 * 10, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.1, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(1.0, value, tol);

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(12));
    timeSeries = sch_day.timeSeries();
    EXPECT_EQ(24 * 12, timeSeries.dateTimes().size());
    EXPECT_EQ(24 * 12, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.1, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(0.1, value, tol);  // at this timestep, we do return the 0.1
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(1.0, value, tol);
  }

  {  // Interpolate to Timestep = Average
    EXPECT_TRUE(sch_day.setInterpolatetoTimestep("Average"));
    EXPECT_EQ("Average", sch_day.interpolatetoTimestep());

    auto timestep = model.getUniqueModelObject<Timestep>();

    openstudio::TimeSeries timeSeries;
    double value;

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(1));
    timeSeries = sch_day.timeSeries();
    EXPECT_EQ(24 * 1, timeSeries.dateTimes().size());
    EXPECT_EQ(24 * 1, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.1, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(0.925, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(0.925, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(0.925, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(0.925, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(0.925, value, tol);

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(4));
    timeSeries = sch_day.timeSeries();
    EXPECT_EQ(24 * 4, timeSeries.dateTimes().size());
    EXPECT_EQ(24 * 4, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.1, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(0.7, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(0.7, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(1.0, value, tol);

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(6));
    timeSeries = sch_day.timeSeries();
    ASSERT_EQ(24 * 6, timeSeries.dateTimes().size());
    ASSERT_EQ(24 * 6, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.1, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(0.55, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(0.55, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(1.0, value, tol);

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(10));
    timeSeries = sch_day.timeSeries();
    EXPECT_EQ(24 * 10, timeSeries.dateTimes().size());
    EXPECT_EQ(24 * 10, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.1, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(0.25, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(1.0, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(1.0, value, tol);
  }

  {  // Interpolate to Timestep = Linear
    EXPECT_TRUE(sch_day.setInterpolatetoTimestep("Linear"));
    EXPECT_EQ("Linear", sch_day.interpolatetoTimestep());

    auto timestep = model.getUniqueModelObject<Timestep>();

    openstudio::TimeSeries timeSeries;
    double value;

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(1));
    timeSeries = sch_day.timeSeries();
    EXPECT_EQ(24 * 1, timeSeries.dateTimes().size());
    EXPECT_EQ(24 * 1, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.098969, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(0.160365, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(0.160365, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(0.160365, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(0.160365, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(0.160365, value, tol);

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(4));
    timeSeries = sch_day.timeSeries();
    EXPECT_EQ(24 * 4, timeSeries.dateTimes().size());
    EXPECT_EQ(24 * 4, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.098969, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(0.110975, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(0.110975, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(0.127439, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(0.127439, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(0.160365, value, tol);

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(6));
    timeSeries = sch_day.timeSeries();
    ASSERT_EQ(24 * 6, timeSeries.dateTimes().size());
    ASSERT_EQ(24 * 6, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.098969, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(0.105487, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(0.105487, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(0.116463, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(0.116463, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(0.160365, value, tol);

    EXPECT_TRUE(timestep.setNumberOfTimestepsPerHour(10));
    timeSeries = sch_day.timeSeries();
    EXPECT_EQ(24 * 10, timeSeries.dateTimes().size());
    EXPECT_EQ(24 * 10, timeSeries.values().size());
    value = sch_day.getValue(t0800);
    EXPECT_NEAR(0.098969, value, tol);
    value = sch_day.getValue(t0805);
    EXPECT_NEAR(0.101097, value, tol);
    value = sch_day.getValue(t0810);
    EXPECT_NEAR(0.107682, value, tol);
    value = sch_day.getValue(t0818);
    EXPECT_NEAR(0.114268, value, tol);
    value = sch_day.getValue(t0820);
    EXPECT_NEAR(0.120853, value, tol);
    value = sch_day.getValue(t0900);
    EXPECT_NEAR(0.160365, value, tol);
  }
}
