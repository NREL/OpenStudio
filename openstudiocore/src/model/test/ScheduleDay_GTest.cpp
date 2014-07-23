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
#include "../ScheduleDay.hpp"
#include "../ScheduleDay_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"

#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, Schedule_Day)
{
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
}

TEST_F(ModelFixture, Schedule_Day_Interp)
{
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
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 0, 0)), tol);
  EXPECT_NEAR(0.5, daySchedule.getValue(Time(0, 6, 0)), tol);
  EXPECT_NEAR(1.0, daySchedule.getValue(Time(0, 12, 0)), tol);
  EXPECT_NEAR(0.5, daySchedule.getValue(Time(0, 18, 0)), tol);
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 24, 0)), tol);
  EXPECT_NEAR(0.0, daySchedule.getValue(Time(0, 25, 0)), tol);
}


TEST_F(ModelFixture, Schedule_Day_Remove)
{
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
