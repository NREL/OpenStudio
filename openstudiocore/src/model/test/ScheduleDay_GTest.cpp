/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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


  // Add small values
  daySchedule.clearValues();
  EXPECT_EQ(1u, daySchedule.times().size());
  EXPECT_EQ(1u, daySchedule.values().size());
  EXPECT_EQ(Time(0,24,0),daySchedule.times().front());

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
  EXPECT_EQ(Time(0,0,1),daySchedule.times().front());
  EXPECT_EQ(Time(0,24,0),daySchedule.times().back());
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

TEST_F(ModelFixture, Schedule_Day_Clone)
{
  Model model;

  ScheduleTypeLimits limits(model);

  ScheduleDay daySchedule(model);
  daySchedule.setScheduleTypeLimits(limits);

  ASSERT_EQ(1u, daySchedule.resources().size());
  EXPECT_EQ(limits.handle(), daySchedule.resources()[0].handle());

  EXPECT_EQ(1u, model.getConcreteModelObjects<ScheduleTypeLimits>().size());
  EXPECT_EQ(1u, model.getConcreteModelObjects<ScheduleDay>().size());

  ScheduleDay daySchedule2 = daySchedule.clone(model).cast<ScheduleDay>();

  EXPECT_EQ(1u, model.getConcreteModelObjects<ScheduleTypeLimits>().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<ScheduleDay>().size());
  ASSERT_TRUE(daySchedule.scheduleTypeLimits());
  ASSERT_TRUE(daySchedule2.scheduleTypeLimits());
  EXPECT_EQ(limits.handle(), daySchedule.scheduleTypeLimits()->handle());
  EXPECT_EQ(daySchedule.scheduleTypeLimits()->handle(), daySchedule2.scheduleTypeLimits()->handle());
}

// Test that addValue (which doesn't call IdfObject::setDouble...) will not affect NaNs/Infinity
TEST_F(ModelFixture, Schedule_Day_addValue_NaN_Infinity)
{
  Model model;

  ScheduleDay sch_day(model);

  Time t(0,6,0,0);
  EXPECT_FALSE(sch_day.addValue(t, std::numeric_limits<double>::quiet_NaN()));
  EXPECT_FALSE(sch_day.addValue(t, std::numeric_limits<double>::infinity()));
  EXPECT_FALSE(sch_day.addValue(t, -std::numeric_limits<double>::infinity()));
  EXPECT_TRUE(sch_day.addValue(t, 1.0));
  EXPECT_FALSE(sch_day.addValue(t, std::numeric_limits<double>::quiet_NaN()));
  EXPECT_FALSE(sch_day.addValue(t, std::numeric_limits<double>::infinity()));
  EXPECT_FALSE(sch_day.addValue(t, -std::numeric_limits<double>::infinity()));
}


