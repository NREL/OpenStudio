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
#include "../ScheduleFixedInterval.hpp"
#include "../ScheduleFixedInterval_Impl.hpp"
#include "../ScheduleVariableInterval.hpp"
#include "../ScheduleVariableInterval_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"

#include "../../utilities/data/TimeSeries.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, Schedule_FixedInterval)
{
  Model model;
  ScheduleFixedInterval schedule(model);
  EXPECT_EQ(0, schedule.intervalLength());

  TimeSeries timeSeries1 = schedule.timeSeries();
  EXPECT_EQ(Date(MonthOfYear::Jan, 1), timeSeries1.firstReportDateTime().date());
  EXPECT_EQ(0u, timeSeries1.values().size());

  Date startDate(MonthOfYear::Jan, 1);
  Time intervalLength(0, 0, 60);
  Vector values(8760);
  for (unsigned i = 0; i < values.size(); ++i){
    values[i] = i % 24;
  }

  TimeSeries timeSeries2(startDate, intervalLength, values, "");
  EXPECT_TRUE(schedule.setTimeSeries(timeSeries2));

  TimeSeries timeSeries3 = schedule.timeSeries();
  EXPECT_EQ(Date(MonthOfYear::Jan, 1), timeSeries3.firstReportDateTime().date());
  EXPECT_EQ(Time(0,0,60), timeSeries3.firstReportDateTime().time());

  EXPECT_EQ(timeSeries2.firstReportDateTime(), timeSeries3.firstReportDateTime());
  ASSERT_TRUE(timeSeries2.intervalLength());
  ASSERT_TRUE(timeSeries3.intervalLength());
  EXPECT_EQ(timeSeries2.intervalLength(), timeSeries3.intervalLength());
  EXPECT_EQ(timeSeries2.values().size(), timeSeries3.values().size());

  boost::optional<ScheduleInterval> newSchedule = ScheduleInterval::fromTimeSeries(timeSeries3, model);
  ASSERT_TRUE(newSchedule);
  EXPECT_NE(schedule.handle(), newSchedule->handle());

  TimeSeries timeSeries4 = newSchedule->timeSeries();
  EXPECT_EQ(timeSeries2.firstReportDateTime(), timeSeries4.firstReportDateTime());
  ASSERT_TRUE(timeSeries2.intervalLength());
  ASSERT_TRUE(timeSeries4.intervalLength());
  EXPECT_EQ(timeSeries2.intervalLength(), timeSeries4.intervalLength());
  EXPECT_EQ(timeSeries2.values().size(), timeSeries4.values().size());
}

TEST_F(ModelFixture, Schedule_FixedInterval_NaN_Infinity)
{
  Model model;
  ScheduleFixedInterval schedule(model);

  Date startDate(MonthOfYear::Jan, 1);
  Time intervalLength(0, 0, 60);


  // Make a vector of values with a NaN on position 10
  Vector values(8760, 1);
  values[10] = std::numeric_limits<double>::quiet_NaN();

  TimeSeries timeSeriesNaN(startDate, intervalLength, values, "");
  EXPECT_FALSE(schedule.setTimeSeries(timeSeriesNaN));


  // Set it to Infinity instead
  values[10] = std::numeric_limits<double>::infinity();
  TimeSeries timeSeriesInf(startDate, intervalLength, values, "");
  EXPECT_FALSE(schedule.setTimeSeries(timeSeriesInf));

}


TEST_F(ModelFixture, Schedule_VariableInterval)
{
  Model model;
  ScheduleVariableInterval schedule(model);

  TimeSeries timeSeries1 = schedule.timeSeries();
  EXPECT_EQ(Date(MonthOfYear::Jan, 1), timeSeries1.firstReportDateTime().date());
  EXPECT_EQ(0u, timeSeries1.values().size());

  Date startDate(MonthOfYear::Jan, 1);
  Time intervalLength(0, 0, 60);
  std::vector<DateTime> dateTimes;
  Vector values(8760);
  for (unsigned i = 0; i < values.size(); ++i){
    dateTimes.push_back(DateTime(startDate, intervalLength*(i+1)));
    values[i] = i % 24;
  }
  EXPECT_EQ(Time(0,0,60), dateTimes[0].time());

  TimeSeries timeSeries2(dateTimes, values, "");
  EXPECT_TRUE(schedule.setTimeSeries(timeSeries2));

  TimeSeries timeSeries3 = schedule.timeSeries();
  EXPECT_EQ(Date(MonthOfYear::Jan, 1), timeSeries3.firstReportDateTime().date());
  EXPECT_EQ(Time(0,0,60), timeSeries3.firstReportDateTime().time());

  EXPECT_EQ(timeSeries2.firstReportDateTime(), timeSeries3.firstReportDateTime());
  EXPECT_FALSE(timeSeries2.intervalLength());
  EXPECT_FALSE(timeSeries3.intervalLength());
  EXPECT_EQ(timeSeries2.values().size(), timeSeries3.values().size());

  boost::optional<ScheduleInterval> newSchedule = ScheduleInterval::fromTimeSeries(timeSeries3, model);
  ASSERT_TRUE(newSchedule);
  EXPECT_NE(schedule.handle(), newSchedule->handle());

  // this isn't really a variable schedule since all the reports are at the same interval
  EXPECT_TRUE(schedule.optionalCast<ScheduleVariableInterval>());
}

TEST_F(ModelFixture, Schedule_VariableInterval2)
{
  Model model;
  ScheduleVariableInterval schedule(model);

  Date startDate(MonthOfYear::Jan, 1);
  Time intervalLength(0, 0, 60);
  std::vector<DateTime> dateTimes;
  Vector values(4380);
  dateTimes.push_back(DateTime(startDate, Time(0,0,0))); // start of first interval
  unsigned i = 0;
  for (unsigned j = 0; j < 8760; ++j){
    unsigned hr = j % 24 + 1;
    // everyday report 0 until 8:00, then report the hour until 7:00 pm
    if (hr == 8){
      dateTimes.push_back(DateTime(startDate, intervalLength*(j + 1)));
      values[i] = 0;
      ++i;
    }else if (hr > 8 && hr < 20){
      dateTimes.push_back(DateTime(startDate, intervalLength*(j + 1)));
      values[i] = hr;
      ++i;
    }
  }
  EXPECT_EQ(4380+1, dateTimes.size()); // date times includes start time of first interval
  EXPECT_EQ(4380, values.size());
  EXPECT_EQ(Time(0, 0, 0), dateTimes[0].time());
  EXPECT_EQ(Time(0, 8, 0), dateTimes[1].time());

  TimeSeries timeSeries2(dateTimes, values, "");
  EXPECT_TRUE(schedule.setTimeSeries(timeSeries2));

  TimeSeries timeSeries3 = schedule.timeSeries();
  EXPECT_EQ(Date(MonthOfYear::Jan, 1), timeSeries3.firstReportDateTime().date());
  EXPECT_EQ(Time(0, 8, 0), timeSeries3.firstReportDateTime().time());

  EXPECT_EQ(timeSeries2.firstReportDateTime(), timeSeries3.firstReportDateTime());
  EXPECT_FALSE(timeSeries2.intervalLength());
  EXPECT_FALSE(timeSeries3.intervalLength());
  EXPECT_EQ(timeSeries2.values().size(), timeSeries3.values().size());

  boost::optional<ScheduleInterval> newSchedule = ScheduleInterval::fromTimeSeries(timeSeries3, model);
  ASSERT_TRUE(newSchedule);
  EXPECT_NE(schedule.handle(), newSchedule->handle());

  EXPECT_TRUE(schedule.optionalCast<ScheduleVariableInterval>());
}

TEST_F(ModelFixture, Schedule_VariableInterval3)
{
  Model model;
  ScheduleVariableInterval schedule(model);

  Date startDate(MonthOfYear::Jan, 1);
  Time intervalLength(0, 0, 60);
  std::vector<DateTime> dateTimes;
  Vector values(4380);
  //dateTimes.push_back(DateTime(startDate, Time(0, 0, 0))); // do not report start of first interval
  unsigned i = 0;
  for (unsigned j = 0; j < 8760; ++j){
    unsigned hr = j % 24 + 1;
    // everyday report 0 until 8:00, then report the hour until 7:00 pm
    if (hr == 8){
      dateTimes.push_back(DateTime(startDate, intervalLength*(j + 1)));
      values[i] = 0;
      ++i;
    } else if (hr > 8 && hr < 20){
      dateTimes.push_back(DateTime(startDate, intervalLength*(j + 1)));
      values[i] = hr;
      ++i;
    }
  }
  EXPECT_EQ(4380, dateTimes.size()); // date times does not include start time of first interval
  EXPECT_EQ(4380, values.size());
  EXPECT_EQ(Time(0, 8, 0), dateTimes[0].time());

  TimeSeries timeSeries2(dateTimes, values, "");
  EXPECT_TRUE(schedule.setTimeSeries(timeSeries2));

  TimeSeries timeSeries3 = schedule.timeSeries();
  EXPECT_EQ(Date(MonthOfYear::Jan, 1), timeSeries3.firstReportDateTime().date());
  EXPECT_EQ(Time(0, 8, 0), timeSeries3.firstReportDateTime().time());

  EXPECT_EQ(timeSeries2.firstReportDateTime(), timeSeries3.firstReportDateTime());
  EXPECT_FALSE(timeSeries2.intervalLength());
  EXPECT_FALSE(timeSeries3.intervalLength());
  EXPECT_EQ(timeSeries2.values().size(), timeSeries3.values().size());

  boost::optional<ScheduleInterval> newSchedule = ScheduleInterval::fromTimeSeries(timeSeries3, model);
  ASSERT_TRUE(newSchedule);
  EXPECT_NE(schedule.handle(), newSchedule->handle());

  EXPECT_TRUE(schedule.optionalCast<ScheduleVariableInterval>());
}
