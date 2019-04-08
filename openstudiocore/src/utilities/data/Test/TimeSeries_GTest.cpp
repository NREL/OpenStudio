/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "DataFixture.hpp"

#include "../TimeSeries.hpp"
#include "../Vector.hpp"
#include "../../time/Date.hpp"
#include "../../time/Time.hpp"

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(DataFixture,TimeSeries_IntervalConstructor)
{
  std::string units = "W";

  // fill vector with 3 hours of data
  Vector values(3);
  for (unsigned i = 0; i < 3; ++i){
    values(i) = i;
  }
  unsigned numValues = values.size();

  // start date
  Date startDate(MonthOfYear(MonthOfYear::Feb), 21);

  // start date and time
  DateTime firstDateTime(startDate, Time(0,1,0,0)); // note: first data point at 1:00 am (covers period from 12:00 pm < t <= 1:00 am)

  // interval
  Time interval = Time(0,1,0,0);

  // create two timeSeries with hourly interval, should be the same
  TimeSeries timeSeries1(startDate, interval, values, units);
  TimeSeries timeSeries2(firstDateTime, interval, values, units);
  ASSERT_TRUE(!timeSeries1.values().empty());
  ASSERT_TRUE(!timeSeries2.values().empty());

  // Check computations
  EXPECT_EQ(10800, timeSeries1.integrate());
  EXPECT_EQ(1, timeSeries1.averageValue());

  // check interval
  OptionalTime interval1 = timeSeries1.intervalLength();
  OptionalTime interval2 = timeSeries2.intervalLength();
  ASSERT_TRUE(interval1);
  ASSERT_TRUE(interval2);
  EXPECT_EQ(*interval1, *interval2);
  EXPECT_EQ(interval, *interval1);

  // check start date and time
//  DateTime firstDateTime1 = timeSeries1.dateTimes().front();
//  DateTime firstDateTime2 = timeSeries2.dateTimes().front();
  DateTime firstDateTime1 = timeSeries1.firstReportDateTime();
  DateTime firstDateTime2 = timeSeries2.firstReportDateTime();
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0,1,0,0)), firstDateTime1);
  EXPECT_EQ(firstDateTime1, firstDateTime2);

  // check end date and time
  DateTime endDateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0,3,0,0));
//  DateTime endDateTime1 = timeSeries1.dateTimes().back();
//  DateTime endDateTime2 = timeSeries2.dateTimes().back();
  DateTime endDateTime1 = timeSeries1.firstReportDateTime() + Time(timeSeries1.daysFromFirstReport(timeSeries1.daysFromFirstReport().size()-1));
  DateTime endDateTime2 = timeSeries2.firstReportDateTime() + Time(timeSeries2.daysFromFirstReport(timeSeries2.daysFromFirstReport().size()-1));
  EXPECT_EQ(endDateTime, endDateTime1);
  EXPECT_EQ(endDateTime1, endDateTime2);

  // check out of range
  timeSeries1.setOutOfRangeValue(-99);
  timeSeries2.setOutOfRangeValue(-99);
  EXPECT_EQ(-99, timeSeries1.value(firstDateTime + Time(0, 0, -61, 0))); // out of range
  EXPECT_EQ(-99, timeSeries1.value(firstDateTime + Time(0, 0, -60, 0))); // out of range
  EXPECT_EQ(0, timeSeries1.value(firstDateTime + Time(0, 0, -59, 0))); // in range
  EXPECT_EQ(0, timeSeries1.value(firstDateTime)); // in range
  EXPECT_EQ(1, timeSeries1.value(firstDateTime + Time(0, 0, 59, 0))); // in range
  EXPECT_EQ(1, timeSeries1.value(firstDateTime + Time(0, 0, 60, 0))); // in range
  EXPECT_EQ(2, timeSeries1.value(firstDateTime + Time(0, 0, 61, 0))); // in range
  EXPECT_EQ(2, timeSeries1.value(endDateTime)); // in range
  EXPECT_EQ(-99, timeSeries1.value(endDateTime + Time(0,1,0,0))); // out of range

  EXPECT_EQ(-99, timeSeries2.value(firstDateTime + Time(0, 0, -61, 0))); // out of range
  EXPECT_EQ(-99, timeSeries2.value(firstDateTime + Time(0, 0, -60, 0))); // out of range
  EXPECT_EQ(0, timeSeries2.value(firstDateTime + Time(0, 0, -59, 0))); // in range
  EXPECT_EQ(0, timeSeries2.value(firstDateTime)); // in range
  EXPECT_EQ(1, timeSeries2.value(firstDateTime + Time(0, 0, 59, 0))); // in range
  EXPECT_EQ(1, timeSeries2.value(firstDateTime + Time(0, 0, 60, 0))); // in range
  EXPECT_EQ(2, timeSeries2.value(firstDateTime + Time(0, 0, 61, 0))); // in range
  EXPECT_EQ(2, timeSeries2.value(endDateTime)); // in range
  EXPECT_EQ(-99, timeSeries2.value(endDateTime + Time(0,1,0,0))); // out of range

  // check values
  for (unsigned i = 0; i < numValues; ++i){
    double numPeriods = (double)i;
    double lastPeriodEnd = (numPeriods-1.0)*interval.totalDays();
    double periodBegin = (numPeriods-0.99)*interval.totalDays();
    double periodMiddle = (numPeriods-0.5)*interval.totalDays();
    double periodEnd = (numPeriods)*interval.totalDays();

    if (i > 0){
      EXPECT_EQ(numPeriods-1, timeSeries1.value(lastPeriodEnd));
    }

    EXPECT_EQ(numPeriods, timeSeries1.value(periodBegin));
    EXPECT_EQ(numPeriods, timeSeries1.value(periodMiddle));
    EXPECT_EQ(numPeriods, timeSeries1.value(periodEnd));
    EXPECT_EQ(numPeriods, timeSeries2.value(periodBegin));
    EXPECT_EQ(numPeriods, timeSeries2.value(periodMiddle));
    EXPECT_EQ(numPeriods, timeSeries2.value(periodEnd));
  }
}

TEST_F(DataFixture,TimeSeries_DetailedConstructor_FirstReport)
{
  std::string units = "W";

  Date startDate(Date(MonthOfYear(MonthOfYear::Feb),21));
  DateTime startDateTime(startDate, Time(0,1,0,0));

  // fill vector with 3 hours of data
  Vector values(3);
  DateTimeVector dateTimes;
  for (unsigned i = 0; i < 3; ++i){
    values(i) = i;
    dateTimes.push_back(startDateTime + Time(0,i,0,0));
  }
  unsigned numValues = values.size();

  // create detailed timeSeries
  TimeSeries timeSeries(dateTimes, values, units);
  ASSERT_TRUE(!timeSeries.values().empty());

  // check interval
  EXPECT_FALSE(timeSeries.intervalLength());

  // Check computations
  EXPECT_EQ(10800, timeSeries.integrate());
  EXPECT_EQ(1, timeSeries.averageValue());

  // check start date and time
//  DateTime firstDateTime = timeSeries.dateTimes().front();
  DateTime firstDateTime = timeSeries.firstReportDateTime();
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0,1,0,0)), firstDateTime);

  // check end date and time
//  DateTime endDateTime = timeSeries.dateTimes().back();
  DateTime endDateTime = timeSeries.firstReportDateTime() + Time(timeSeries.daysFromFirstReport(timeSeries.daysFromFirstReport().size()-1));
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0,3,0,0)), endDateTime);

  // check out of range
  timeSeries.setOutOfRangeValue(-99);

  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0,0,-61,0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0,0,-60,0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0,0,-59,0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0,0,-1,0))); // out of range
  EXPECT_EQ(0, timeSeries.value(startDateTime)); // in range
  EXPECT_EQ(2, timeSeries.value(endDateTime)); // in range
  EXPECT_EQ(-99, timeSeries.value(endDateTime + Time(0,1,0,0))); // out of range

  // check values
  for (unsigned i = 0; i < numValues; ++i){
    double numPeriods = (double)i;
    Time interval(0,1,0,0);
    double lastPeriodEnd = (numPeriods-1.0)*interval.totalDays();
    double periodBegin = (numPeriods-0.99)*interval.totalDays();
    double periodMiddle = (numPeriods-0.5)*interval.totalDays();
    double periodEnd = (numPeriods)*interval.totalDays();

    if (i > 0){
      EXPECT_EQ(numPeriods-1, timeSeries.value(lastPeriodEnd));
      EXPECT_EQ(numPeriods, timeSeries.value(periodBegin));
      EXPECT_EQ(numPeriods, timeSeries.value(periodMiddle));
    }

    EXPECT_EQ(numPeriods, timeSeries.value(periodEnd));
  }

  // check date/time objects
  DateTimeVector fromSeries = timeSeries.dateTimes();
  ASSERT_EQ(numValues,fromSeries.size());
  for (unsigned i = 0; i < numValues; ++i){
    EXPECT_TRUE(fromSeries[i]==dateTimes[i]);
  }

}

TEST_F(DataFixture, TimeSeries_DetailedConstructor_Start)
{
  std::string units = "W";

  Date startDate(Date(MonthOfYear(MonthOfYear::Feb), 21));
  DateTime startDateTime(startDate, Time(0, 1, 0, 0));
  DateTime firstDateTime(startDate, Time(0, 2, 0, 0));

  // fill vector with 3 hours of data
  Vector values(3);
  DateTimeVector dateTimes;
  dateTimes.push_back(startDateTime);
  for (unsigned i = 0; i < 3; ++i){
    values(i) = i;
    dateTimes.push_back(startDateTime + Time(0, i+1, 0, 0));
  }
  unsigned numValues = values.size();

  // create detailed timeSeries
  TimeSeries timeSeries(dateTimes, values, units);
  ASSERT_TRUE(!timeSeries.values().empty());

  // check interval
  EXPECT_FALSE(timeSeries.intervalLength());

  // Check computations
  EXPECT_EQ(10800, timeSeries.integrate());
  EXPECT_EQ(1, timeSeries.averageValue());

  // check start date and time
  EXPECT_EQ(firstDateTime, timeSeries.firstReportDateTime());

  // check end date and time
  //  DateTime endDateTime = timeSeries.dateTimes().back();
  DateTime endDateTime = timeSeries.firstReportDateTime() + Time(timeSeries.daysFromFirstReport(timeSeries.daysFromFirstReport().size() - 1));
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0, 4, 0, 0)), endDateTime);

  // check out of range
  timeSeries.setOutOfRangeValue(-99);

  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -61, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -60, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -59, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -1, 0))); // out of range
  EXPECT_EQ(0, timeSeries.value(firstDateTime)); // in range
  EXPECT_EQ(2, timeSeries.value(endDateTime)); // in range
  EXPECT_EQ(-99, timeSeries.value(endDateTime + Time(0, 1, 0, 0))); // out of range

  // check values
  for (unsigned i = 0; i < numValues; ++i){
    double numPeriods = (double)i;
    Time interval(0, 1, 0, 0);
    double lastPeriodEnd = (numPeriods - 1.0)*interval.totalDays();
    double periodBegin = (numPeriods - 0.99)*interval.totalDays();
    double periodMiddle = (numPeriods - 0.5)*interval.totalDays();
    double periodEnd = (numPeriods)*interval.totalDays();

    if (i > 0){
      EXPECT_EQ(numPeriods - 1, timeSeries.value(lastPeriodEnd));
      EXPECT_EQ(numPeriods, timeSeries.value(periodBegin));
      EXPECT_EQ(numPeriods, timeSeries.value(periodMiddle));
    }

    EXPECT_EQ(numPeriods, timeSeries.value(periodEnd));
  }

  // check date/time objects
  DateTimeVector fromSeries = timeSeries.dateTimes();
  ASSERT_EQ(numValues, fromSeries.size());
  for (unsigned i = 0; i < numValues; ++i){
    EXPECT_TRUE(fromSeries[i] == dateTimes[i+1]);
  }

}

TEST_F(DataFixture, TimeSeries_SecondsConstructor_FirstReport)
{
  std::string units = "W";

  Date startDate(Date(MonthOfYear(MonthOfYear::Feb), 21));
  DateTime startDateTime(startDate, Time(0, 1, 0, 0));

  // fill vector with 3 hours of data
  Vector values(3);
  DateTimeVector dateTimes;
  std::vector<long> seconds;
  for (unsigned i = 0; i < 3; ++i){
    values(i) = i;
    seconds.push_back(i * 3600);
    dateTimes.push_back(startDateTime + Time(0, i, 0, 0));
  }
  unsigned numValues = values.size();

  // fail to create a detailed time series
  ASSERT_THROW(TimeSeries timeSeries0(DateTime(startDate), seconds, values, units), openstudio::Exception);

  // create detailed timeSeries
  TimeSeries timeSeries(startDateTime, seconds, values, units);
  ASSERT_TRUE(!timeSeries.values().empty());

  // check interval
  EXPECT_FALSE(timeSeries.intervalLength());

  // Check computations
  EXPECT_EQ(10800, timeSeries.integrate());
  EXPECT_EQ(1, timeSeries.averageValue());

  // check start date and time
  //  DateTime firstDateTime = timeSeries.dateTimes().front();
  DateTime firstDateTime = timeSeries.firstReportDateTime();
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0, 1, 0, 0)), firstDateTime);

  // check end date and time
  //  DateTime endDateTime = timeSeries.dateTimes().back();
  DateTime endDateTime = timeSeries.firstReportDateTime() + Time(timeSeries.daysFromFirstReport(timeSeries.daysFromFirstReport().size() - 1));
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0, 3, 0, 0)), endDateTime);

  // check out of range
  timeSeries.setOutOfRangeValue(-99);

  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -61, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -60, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -59, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -1, 0))); // out of range
  EXPECT_EQ(0, timeSeries.value(startDateTime)); // in range
  EXPECT_EQ(2, timeSeries.value(endDateTime)); // in range
  EXPECT_EQ(-99, timeSeries.value(endDateTime + Time(0, 1, 0, 0))); // out of range

  // check values
  for (unsigned i = 0; i < numValues; ++i){
    double numPeriods = (double)i;
    Time interval(0, 1, 0, 0);
    double lastPeriodEnd = (numPeriods - 1.0)*interval.totalDays();
    double periodBegin = (numPeriods - 0.99)*interval.totalDays();
    double periodMiddle = (numPeriods - 0.5)*interval.totalDays();
    double periodEnd = (numPeriods)*interval.totalDays();

    if (i > 0){
      EXPECT_EQ(numPeriods - 1, timeSeries.value(lastPeriodEnd));
      EXPECT_EQ(numPeriods, timeSeries.value(periodBegin));
      EXPECT_EQ(numPeriods, timeSeries.value(periodMiddle));
    } else {
      EXPECT_EQ(-99, timeSeries.value(periodBegin));
      EXPECT_EQ(-99, timeSeries.value(periodMiddle));
    }

    EXPECT_EQ(numPeriods, timeSeries.value(periodEnd));
  }

  // check date/time objects
  DateTimeVector fromSeries = timeSeries.dateTimes();
  ASSERT_EQ(numValues, fromSeries.size());
  for (unsigned i = 0; i < numValues; ++i){
    EXPECT_TRUE(fromSeries[i] == dateTimes[i]);
  }

}

TEST_F(DataFixture, TimeSeries_SecondsConstructor_Start)
{
  std::string units = "W";

  Date startDate(Date(MonthOfYear(MonthOfYear::Feb), 21));
  DateTime startDateTime(startDate);
  DateTime firstDateTime = startDateTime + Time(0, 1, 0, 0);

  // fill vector with 3 hours of data
  Vector values(3);
  DateTimeVector dateTimes;
  std::vector<long> seconds;
  for (unsigned i = 0; i < 3; ++i){
    values(i) = i;
    seconds.push_back((i+1) * 3600);
    dateTimes.push_back(startDateTime + Time(0, i+1, 0, 0));
  }
  unsigned numValues = values.size();

  // create detailed timeSeries
  TimeSeries timeSeries(firstDateTime, seconds, values, units);
  ASSERT_TRUE(!timeSeries.values().empty());

  // check interval
  EXPECT_FALSE(timeSeries.intervalLength());

  // Check computations
  EXPECT_EQ(10800, timeSeries.integrate());
  EXPECT_EQ(1, timeSeries.averageValue());

  // check start date and time
  EXPECT_EQ(firstDateTime, timeSeries.firstReportDateTime());

  // check end date and time
  //  DateTime endDateTime = timeSeries.dateTimes().back();
  DateTime endDateTime = timeSeries.firstReportDateTime() + Time(timeSeries.daysFromFirstReport(timeSeries.daysFromFirstReport().size() - 1));
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0, 3, 0, 0)), endDateTime);

  // check out of range
  timeSeries.setOutOfRangeValue(-99);

  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -61, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -60, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -59, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -1, 0))); // out of range
  EXPECT_EQ(0, timeSeries.value(firstDateTime)); // in range
  EXPECT_EQ(2, timeSeries.value(endDateTime)); // in range
  EXPECT_EQ(-99, timeSeries.value(endDateTime + Time(0, 1, 0, 0))); // out of range

  // check values
  for (unsigned i = 0; i < numValues; ++i){
    double numPeriods = (double)i;
    Time interval(0, 1, 0, 0);
    double lastPeriodEnd = (numPeriods - 1.0)*interval.totalDays();
    double periodBegin = (numPeriods - 0.99)*interval.totalDays();
    double periodMiddle = (numPeriods - 0.5)*interval.totalDays();
    double periodEnd = (numPeriods)*interval.totalDays();

    if (i > 0){
      EXPECT_EQ(numPeriods - 1, timeSeries.value(lastPeriodEnd));
      EXPECT_EQ(numPeriods, timeSeries.value(periodBegin));
      EXPECT_EQ(numPeriods, timeSeries.value(periodMiddle));
    } else {
      EXPECT_EQ(-99, timeSeries.value(periodBegin));
      EXPECT_EQ(-99, timeSeries.value(periodMiddle));
    }

    EXPECT_EQ(numPeriods, timeSeries.value(periodEnd));
  }

  // check date/time objects
  DateTimeVector fromSeries = timeSeries.dateTimes();
  ASSERT_EQ(numValues, fromSeries.size());
  for (unsigned i = 0; i < numValues; ++i){
    EXPECT_EQ(dateTimes[i], fromSeries[i]);
  }

}

TEST_F(DataFixture, TimeSeries_DaysConstructor_std_vector_FirstReport)
{
  std::string units = "W";

  Date startDate(Date(MonthOfYear(MonthOfYear::Feb), 21));
  DateTime startDateTime(startDate, Time(0, 3, 0, 0));

  // fill vector with 9 hours of data
  std::vector<double> values(3);
  DateTimeVector dateTimes;
  std::vector<double> days(3);
  for (unsigned i = 0; i < 3; ++i){
    values[i] = i;
    days[i] = i * 0.125;
    dateTimes.push_back(startDateTime + Time(0, i*3, 0, 0));
  }
  unsigned numValues = values.size();

  // fail to create a detailed time series
  ASSERT_THROW(TimeSeries timeSeries0(DateTime(startDate), days, values, units), openstudio::Exception);

  // create detailed timeSeries
  TimeSeries timeSeries(startDateTime, days, values, units);
  ASSERT_TRUE(!timeSeries.values().empty());

  // check interval
  EXPECT_FALSE(timeSeries.intervalLength());

  // Check computations
  EXPECT_EQ(32400, timeSeries.integrate());
  EXPECT_EQ(1, timeSeries.averageValue());

  // check start date and time
  //  DateTime firstDateTime = timeSeries.dateTimes().front();
  DateTime firstDateTime = timeSeries.firstReportDateTime();
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0, 3, 0, 0)), firstDateTime);

  // check end date and time
  //  DateTime endDateTime = timeSeries.dateTimes().back();
  DateTime endDateTime = timeSeries.firstReportDateTime() + Time(timeSeries.daysFromFirstReport(timeSeries.daysFromFirstReport().size() - 1));
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0, 9, 0, 0)), endDateTime);

  // check out of range
  timeSeries.setOutOfRangeValue(-99);

  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -61, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -60, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -59, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -1, 0))); // out of range
  EXPECT_EQ(0, timeSeries.value(startDateTime)); // in range
  EXPECT_EQ(2, timeSeries.value(endDateTime)); // in range
  EXPECT_EQ(-99, timeSeries.value(endDateTime + Time(0, 1, 0, 0))); // out of range

  // check values
  for (unsigned i = 0; i < numValues; ++i){
    double numPeriods = (double)i;
    Time interval(0, 3, 0, 0);
    double lastPeriodEnd = (numPeriods - 1.0)*interval.totalDays();
    double periodBegin = (numPeriods - 0.99)*interval.totalDays();
    double periodMiddle = (numPeriods - 0.5)*interval.totalDays();
    double periodEnd = (numPeriods)*interval.totalDays();

    if (i > 0){
      EXPECT_EQ(numPeriods - 1, timeSeries.value(lastPeriodEnd));
      EXPECT_EQ(numPeriods, timeSeries.value(periodBegin));
      EXPECT_EQ(numPeriods, timeSeries.value(periodMiddle));
    } else {
      EXPECT_EQ(-99, timeSeries.value(periodBegin));
      EXPECT_EQ(-99, timeSeries.value(periodMiddle));
    }

    EXPECT_EQ(numPeriods, timeSeries.value(periodEnd));
  }

  // check date/time objects
  DateTimeVector fromSeries = timeSeries.dateTimes();
  ASSERT_EQ(numValues, fromSeries.size());
  for (unsigned i = 0; i < numValues; ++i){
    EXPECT_TRUE(fromSeries[i] == dateTimes[i]);
  }

}

TEST_F(DataFixture, TimeSeries_DaysConstructor_std_vector_Start)
{
  std::string units = "W";

  Date startDate(Date(MonthOfYear(MonthOfYear::Feb), 21));
  DateTime startDateTime(startDate);
  DateTime firstDateTime = startDateTime + Time(0, 3, 0, 0);

  // fill vector with 3 hours of data
  std::vector<double> values(3);
  DateTimeVector dateTimes;
  std::vector<double> days(3);
  for (unsigned i = 0; i < 3; ++i){
    values[i] = i;
    days[i] = (i + 1) * 0.125;
    dateTimes.push_back(startDateTime + Time(0, (i + 1)*3, 0, 0));
  }
  unsigned numValues = values.size();

  // create detailed timeSeries
  TimeSeries timeSeries(firstDateTime, days, values, units);
  ASSERT_TRUE(!timeSeries.values().empty());

  // check interval
  EXPECT_FALSE(timeSeries.intervalLength());

  // Check computations
  EXPECT_EQ(32400, timeSeries.integrate());
  EXPECT_EQ(1, timeSeries.averageValue());

  // check start date and time
  EXPECT_EQ(firstDateTime, timeSeries.firstReportDateTime());

  // check end date and time
  //  DateTime endDateTime = timeSeries.dateTimes().back();
  DateTime endDateTime = timeSeries.firstReportDateTime() + Time(timeSeries.daysFromFirstReport(timeSeries.daysFromFirstReport().size() - 1));
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0, 9, 0, 0)), endDateTime);

  // check out of range
  timeSeries.setOutOfRangeValue(-99);

  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -61, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -60, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -59, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -1, 0))); // out of range
  EXPECT_EQ(0, timeSeries.value(firstDateTime)); // in range
  EXPECT_EQ(2, timeSeries.value(endDateTime)); // in range
  EXPECT_EQ(-99, timeSeries.value(endDateTime + Time(0, 1, 0, 0))); // out of range

  // check values
  for (unsigned i = 0; i < numValues; ++i){
    double numPeriods = (double)i;
    Time interval(0, 3, 0, 0);
    double lastPeriodEnd = (numPeriods - 1.0)*interval.totalDays();
    double periodBegin = (numPeriods - 0.99)*interval.totalDays();
    double periodMiddle = (numPeriods - 0.5)*interval.totalDays();
    double periodEnd = (numPeriods)*interval.totalDays();

    if (i > 0){
      EXPECT_EQ(numPeriods - 1, timeSeries.value(lastPeriodEnd));
      EXPECT_EQ(numPeriods, timeSeries.value(periodBegin));
      EXPECT_EQ(numPeriods, timeSeries.value(periodMiddle));
    } else {
      EXPECT_EQ(-99, timeSeries.value(periodBegin));
      EXPECT_EQ(-99, timeSeries.value(periodMiddle));
    }

    EXPECT_EQ(numPeriods, timeSeries.value(periodEnd));
  }

  // check date/time objects
  DateTimeVector fromSeries = timeSeries.dateTimes();
  ASSERT_EQ(numValues, fromSeries.size());
  for (unsigned i = 0; i < numValues; ++i){
    EXPECT_EQ(dateTimes[i], fromSeries[i]);
  }

}

TEST_F(DataFixture, TimeSeries_DaysConstructor_Vector_FirstReport)
{
  std::string units = "W";

  Date startDate(Date(MonthOfYear(MonthOfYear::Feb), 21));
  DateTime startDateTime(startDate, Time(0, 3, 0, 0));

  // fill vector with 9 hours of data
  Vector values(3);
  DateTimeVector dateTimes;
  Vector days(3);
  for (unsigned i = 0; i < 3; ++i) {
    values[i] = i;
    days[i] = i * 0.125;
    dateTimes.push_back(startDateTime + Time(0, i * 3, 0, 0));
  }
  unsigned numValues = values.size();

  // fail to create a detailed time series
  ASSERT_THROW(TimeSeries timeSeries0(DateTime(startDate), days, values, units), openstudio::Exception);

  // create detailed timeSeries
  TimeSeries timeSeries(startDateTime, days, values, units);
  ASSERT_TRUE(!timeSeries.values().empty());

  // check interval
  EXPECT_FALSE(timeSeries.intervalLength());

  // Check computations
  EXPECT_EQ(32400, timeSeries.integrate());
  EXPECT_EQ(1, timeSeries.averageValue());

  // check start date and time
  //  DateTime firstDateTime = timeSeries.dateTimes().front();
  DateTime firstDateTime = timeSeries.firstReportDateTime();
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0, 3, 0, 0)), firstDateTime);

  // check end date and time
  //  DateTime endDateTime = timeSeries.dateTimes().back();
  DateTime endDateTime = timeSeries.firstReportDateTime() + Time(timeSeries.daysFromFirstReport(timeSeries.daysFromFirstReport().size() - 1));
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0, 9, 0, 0)), endDateTime);

  // check out of range
  timeSeries.setOutOfRangeValue(-99);

  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -61, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -60, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -59, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(startDateTime + Time(0, 0, -1, 0))); // out of range
  EXPECT_EQ(0, timeSeries.value(startDateTime)); // in range
  EXPECT_EQ(2, timeSeries.value(endDateTime)); // in range
  EXPECT_EQ(-99, timeSeries.value(endDateTime + Time(0, 1, 0, 0))); // out of range

  // check values
  for (unsigned i = 0; i < numValues; ++i) {
    double numPeriods = (double)i;
    Time interval(0, 3, 0, 0);
    double lastPeriodEnd = (numPeriods - 1.0)*interval.totalDays();
    double periodBegin = (numPeriods - 0.99)*interval.totalDays();
    double periodMiddle = (numPeriods - 0.5)*interval.totalDays();
    double periodEnd = (numPeriods)*interval.totalDays();

    if (i > 0) {
      EXPECT_EQ(numPeriods - 1, timeSeries.value(lastPeriodEnd));
      EXPECT_EQ(numPeriods, timeSeries.value(periodBegin));
      EXPECT_EQ(numPeriods, timeSeries.value(periodMiddle));
    } else {
      EXPECT_EQ(-99, timeSeries.value(periodBegin));
      EXPECT_EQ(-99, timeSeries.value(periodMiddle));
    }

    EXPECT_EQ(numPeriods, timeSeries.value(periodEnd));
  }

  // check date/time objects
  DateTimeVector fromSeries = timeSeries.dateTimes();
  ASSERT_EQ(numValues, fromSeries.size());
  for (unsigned i = 0; i < numValues; ++i) {
    EXPECT_TRUE(fromSeries[i] == dateTimes[i]);
  }

}

TEST_F(DataFixture, TimeSeries_DaysConstructor_Vector_Start)
{
  std::string units = "W";

  Date startDate(Date(MonthOfYear(MonthOfYear::Feb), 21));
  DateTime startDateTime(startDate);
  DateTime firstDateTime = startDateTime + Time(0, 3, 0, 0);

  // fill vector with 3 hours of data
  Vector values(3);
  DateTimeVector dateTimes;
  Vector days(3);
  for (unsigned i = 0; i < 3; ++i) {
    values[i] = i;
    days[i] = (i + 1) * 0.125;
    dateTimes.push_back(startDateTime + Time(0, (i + 1) * 3, 0, 0));
  }
  unsigned numValues = values.size();

  // create detailed timeSeries
  TimeSeries timeSeries(firstDateTime, days, values, units);
  ASSERT_TRUE(!timeSeries.values().empty());

  // check interval
  EXPECT_FALSE(timeSeries.intervalLength());

  // Check computations
  EXPECT_EQ(32400, timeSeries.integrate());
  EXPECT_EQ(1, timeSeries.averageValue());

  // check start date and time
  EXPECT_EQ(firstDateTime, timeSeries.firstReportDateTime());

  // check end date and time
  //  DateTime endDateTime = timeSeries.dateTimes().back();
  DateTime endDateTime = timeSeries.firstReportDateTime() + Time(timeSeries.daysFromFirstReport(timeSeries.daysFromFirstReport().size() - 1));
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Feb), 21), Time(0, 9, 0, 0)), endDateTime);

  // check out of range
  timeSeries.setOutOfRangeValue(-99);

  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -61, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -60, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -59, 0))); // out of range
  EXPECT_EQ(-99, timeSeries.value(firstDateTime + Time(0, 0, -1, 0))); // out of range
  EXPECT_EQ(0, timeSeries.value(firstDateTime)); // in range
  EXPECT_EQ(2, timeSeries.value(endDateTime)); // in range
  EXPECT_EQ(-99, timeSeries.value(endDateTime + Time(0, 1, 0, 0))); // out of range

  // check values
  for (unsigned i = 0; i < numValues; ++i) {
    double numPeriods = (double)i;
    Time interval(0, 3, 0, 0);
    double lastPeriodEnd = (numPeriods - 1.0)*interval.totalDays();
    double periodBegin = (numPeriods - 0.99)*interval.totalDays();
    double periodMiddle = (numPeriods - 0.5)*interval.totalDays();
    double periodEnd = (numPeriods)*interval.totalDays();

    if (i > 0) {
      EXPECT_EQ(numPeriods - 1, timeSeries.value(lastPeriodEnd));
      EXPECT_EQ(numPeriods, timeSeries.value(periodBegin));
      EXPECT_EQ(numPeriods, timeSeries.value(periodMiddle));
    } else {
      EXPECT_EQ(-99, timeSeries.value(periodBegin));
      EXPECT_EQ(-99, timeSeries.value(periodMiddle));
    }

    EXPECT_EQ(numPeriods, timeSeries.value(periodEnd));
  }

  // check date/time objects
  DateTimeVector fromSeries = timeSeries.dateTimes();
  ASSERT_EQ(numValues, fromSeries.size());
  for (unsigned i = 0; i < numValues; ++i) {
    EXPECT_EQ(dateTimes[i], fromSeries[i]);
  }

}

TEST_F(DataFixture,TimeSeries_IntervalConstructor_WrapAroundDates)
{
  std::string units = "W";

    // start date and time
  Date startDate(MonthOfYear(MonthOfYear::Jan),1);
  DateTime startDateTime(startDate, Time(1,0,0,0));

  // interval
  Time interval = Time(1,0,0,0);

  // fill vector with 365 days of data
  Vector values(365);
  DateTimeVector dateTimes;
  for (unsigned i = 0; i < 365; ++i){
    values(i) = i;
    dateTimes.push_back(startDate + Time(i+1,0,0,0));
  }
  unsigned numValues = values.size();

  ASSERT_EQ(365u, numValues);

  // create timeSeries
  TimeSeries timeSeries(startDate, interval, values, units);
  ASSERT_EQ(365u, timeSeries.values().size());

  // check interval
  ASSERT_TRUE(timeSeries.intervalLength());
  EXPECT_EQ(interval, timeSeries.intervalLength().get());

  // check start date and time
  DateTime firstDateTime = timeSeries.firstReportDateTime();
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Jan), 1), Time(1,0,0,0)), firstDateTime);

  // check values
  for (unsigned i = 0; i < numValues; ++i){
    DateTime dateTime = dateTimes[i];
    EXPECT_EQ(i, timeSeries.value(dateTime)) << dateTime;
  }

  Vector test = timeSeries.values(DateTime(Date(MonthOfYear(MonthOfYear::Apr), 1), Time(1,0,0,0)), DateTime(Date(MonthOfYear(MonthOfYear::Apr), 30), Time(1,0,0,0)));
  ASSERT_EQ(30, test.size());
  for (unsigned i = 0; i < 30; ++i){
    EXPECT_EQ(i + 90, test[i]);
  }

  // now change start date time to make a wrap around year
  startDate = Date(MonthOfYear(MonthOfYear::Apr),11);
  startDateTime = DateTime(startDate, Time(1,0,0,0));

  Vector wrappedValues(365);
  unsigned j = 0;
  for (unsigned i = 100; i < 365; ++i, ++j){
    wrappedValues[j] = values[i];
  }
  for (unsigned i = 0; i < 100; ++i, ++j){
    wrappedValues[j] = values[i];
  }
  ASSERT_EQ(365u, wrappedValues.size());
  EXPECT_EQ(100, wrappedValues[0]);
  EXPECT_EQ(99, wrappedValues[364]);
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Apr), 11), Time(1,0,0,0)), startDateTime.date());

  // create detailed timeSeries
  TimeSeries wrappedTimeSeries(startDateTime, interval, wrappedValues, units);
  ASSERT_EQ(365u, wrappedTimeSeries.values().size());

  // check interval
  ASSERT_TRUE(timeSeries.intervalLength());
  EXPECT_EQ(interval, timeSeries.intervalLength().get());

  // check start date and time
  firstDateTime = wrappedTimeSeries.firstReportDateTime();
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Apr), 11), Time(1,0,0,0)), firstDateTime);

  // check values
  for (unsigned i = 0; i < numValues; ++i){
    DateTime dateTime = dateTimes[i];
    EXPECT_EQ(i, wrappedTimeSeries.value(dateTime)) << dateTime;
  }

  test = timeSeries.values(DateTime(Date(MonthOfYear(MonthOfYear::Apr), 1), Time(1,0,0,0)), DateTime(Date(MonthOfYear(MonthOfYear::Apr), 30), Time(1,0,0,0)));
  ASSERT_EQ(30, test.size());
  for (unsigned i = 0; i < 30; ++i){
    EXPECT_EQ(i + 90, test[i]);
  }

}

TEST_F(DataFixture,TimeSeries_DetailedConstructor_WrapAroundDates)
{
  std::string units = "W";

  Date startDate(MonthOfYear(MonthOfYear::Jan),1);

  // fill vector with 365 days of data
  Vector values(365);
  DateTimeVector dateTimes;
  for (unsigned i = 0; i < 365; ++i){
    values(i) = i;
    dateTimes.push_back(startDate + Time(i+1,0,0,0));
  }
  unsigned numValues = values.size();

  ASSERT_EQ(365u, numValues);
  ASSERT_EQ(365u, dateTimes.size());
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Jan), 1), Time(1,0,0,0)), dateTimes[0].date());
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Dec), 31), Time(1,0,0,0)), dateTimes[364].date());

  // create detailed timeSeries
  TimeSeries timeSeries(dateTimes, values, units);
  ASSERT_EQ(365u, timeSeries.values().size());

  // check interval
  EXPECT_FALSE(timeSeries.intervalLength());

  // check start date and time
  DateTime firstDateTime = timeSeries.firstReportDateTime();
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Jan), 1), Time(1,0,0,0)), firstDateTime);

  // check values
  for (unsigned i = 0; i < numValues; ++i){
    DateTime dateTime = dateTimes[i];
    EXPECT_EQ(i, timeSeries.value(dateTime)) << dateTime;
  }

  Vector test = timeSeries.values(DateTime(Date(MonthOfYear(MonthOfYear::Apr), 1), Time(1,0,0,0)), DateTime(Date(MonthOfYear(MonthOfYear::Apr), 30), Time(1,0,0,0)));
  ASSERT_EQ(30, test.size());
  for (unsigned i = 0; i < 30; ++i){
    EXPECT_EQ(i + 90, test[i]);
  }

  // now rearrange date times to make a wrap around year
  Vector wrappedValues(365);
  DateTimeVector wrappedDateTimes;
  unsigned j = 0;
  for (unsigned i = 100; i < 365; ++i, ++j){
    wrappedValues[j] = values[i];
    wrappedDateTimes.push_back(dateTimes[i]);
  }
  for (unsigned i = 0; i < 100; ++i, ++j){
    wrappedValues[j] = values[i];
    wrappedDateTimes.push_back(dateTimes[i]);
  }
  ASSERT_EQ(365u, wrappedValues.size());
  ASSERT_EQ(365u, wrappedDateTimes.size());
  EXPECT_EQ(100, wrappedValues[0]);
  EXPECT_EQ(99, wrappedValues[364]);
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Apr), 11), Time(1,0,0,0)), wrappedDateTimes[0].date());
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Apr), 10), Time(1,0,0,0)), wrappedDateTimes[364].date());

  // create detailed timeSeries
  TimeSeries wrappedTimeSeries(wrappedDateTimes, wrappedValues, units);
  ASSERT_EQ(365u, wrappedTimeSeries.values().size());

  // check interval
  EXPECT_FALSE(wrappedTimeSeries.intervalLength());

  // check start date and time
  firstDateTime = wrappedTimeSeries.firstReportDateTime();
  EXPECT_EQ(DateTime(Date(MonthOfYear(MonthOfYear::Apr), 11), Time(1,0,0,0)), firstDateTime);

  // check values
  for (unsigned i = 0; i < numValues; ++i){
    DateTime dateTime = dateTimes[i];
    EXPECT_EQ(i, wrappedTimeSeries.value(dateTime)) << dateTime;
  }

  test = timeSeries.values(DateTime(Date(MonthOfYear(MonthOfYear::Apr), 1), Time(1,0,0,0)), DateTime(Date(MonthOfYear(MonthOfYear::Apr), 30), Time(1,0,0,0)));
  ASSERT_EQ(30, test.size());
  for (unsigned i = 0; i < 30; ++i){
    EXPECT_EQ(i + 90, test[i]);
  }

}

TEST_F(DataFixture,TimeSeries_AddSubtract8760)
{
  // Test out various addition and subtraction combinations - some of the tests look a little
  // strange but were the best way to get at some strange rounding issues that came up.
  std::string units = "C";

  // interval
  Time interval = Time(0,1);
  Vector values = linspace(1, 8760, 8760);

  Date startDate(Date(MonthOfYear(MonthOfYear::Jan),1));
  DateTime startDateTime(startDate, Time(0,1,0,0));
  Date endDate(Date(MonthOfYear(MonthOfYear::Dec),31));
  DateTime endDateTime(endDate, Time(0,24,0,0));
  Time delta(0,1,0,0);
  std::vector<DateTime> dateTimes;
  for(openstudio::DateTime current=startDateTime; current <= endDateTime; current += delta)
  {
    dateTimes.push_back(current);
  }

  TimeSeries intervalTimeSeries(startDateTime, interval, values, units);
  TimeSeries detailedTimeSeries(dateTimes, values, units);

  // Addition
  TimeSeries intervalPlusDetailed = intervalTimeSeries + detailedTimeSeries;
  EXPECT_EQ(8760,intervalPlusDetailed.values().size());

  TimeSeries detailedPlusDetailed = detailedTimeSeries + detailedTimeSeries;
  EXPECT_EQ(8760,detailedPlusDetailed.values().size());

  TimeSeries intervalSelfPlusDetailed = intervalTimeSeries;
  intervalSelfPlusDetailed = intervalSelfPlusDetailed + detailedTimeSeries;
  EXPECT_EQ(8760,intervalSelfPlusDetailed.values().size());

  TimeSeries detailedSelfPlusDetailed = detailedTimeSeries;
  detailedSelfPlusDetailed = detailedSelfPlusDetailed + detailedTimeSeries;
  EXPECT_EQ(8760,detailedSelfPlusDetailed.values().size());

  TimeSeries dpdSelfPlusDetailed = detailedTimeSeries + detailedTimeSeries;
  dpdSelfPlusDetailed = dpdSelfPlusDetailed + detailedTimeSeries;
  EXPECT_EQ(8760,dpdSelfPlusDetailed.values().size());

  // Subtraction
  TimeSeries intervalMinusDetailed = intervalTimeSeries - detailedTimeSeries;
  EXPECT_EQ(8760,intervalMinusDetailed.values().size());

  TimeSeries detailedMinusDetailed = detailedTimeSeries - detailedTimeSeries;
  EXPECT_EQ(8760,detailedMinusDetailed.values().size());

  TimeSeries intervalSelfMinusDetailed = intervalTimeSeries;
  intervalSelfMinusDetailed = intervalSelfMinusDetailed - detailedTimeSeries;
  EXPECT_EQ(8760,intervalSelfPlusDetailed.values().size());

  TimeSeries detailedSelfMinusDetailed = detailedTimeSeries;
  detailedSelfMinusDetailed = detailedSelfMinusDetailed - detailedTimeSeries;
  EXPECT_EQ(8760,detailedSelfMinusDetailed.values().size());

  TimeSeries dmdSelfMinusDetailed = detailedTimeSeries - detailedTimeSeries;
  dmdSelfMinusDetailed = dmdSelfMinusDetailed - detailedTimeSeries;
  EXPECT_EQ(8760,dmdSelfMinusDetailed.values().size());
}

TEST_F(DataFixture,TimeSeries_AddSubtractSameTimePeriod)
{
  std::string units = "W";

  Date startDate(Date(MonthOfYear(MonthOfYear::Feb),21));
  DateTime startDateTime(startDate, Time(0,1,0,0));

  // interval
  Time interval = Time(0,1,0,0);
  Vector intervalValues(3);
  intervalValues(0) = 0;
  intervalValues(1) = 1;
  intervalValues(2) = 2;

  TimeSeries intervalTimeSeries(startDateTime, interval, intervalValues, units);
  ASSERT_TRUE(!intervalTimeSeries.values().empty());

  // detailed
  DateTimeVector dateTimes;
  dateTimes.push_back(startDateTime + Time(0,0,0,0));
  dateTimes.push_back(startDateTime + Time(0,0,30,0));
  dateTimes.push_back(startDateTime + Time(0,1,0,0));
  dateTimes.push_back(startDateTime + Time(0,1,30,0));
  dateTimes.push_back(startDateTime + Time(0,2,0,0));
  Vector detailedValues(5);
  detailedValues(0) = 0.0; // 1:00
  detailedValues(1) = 0.5; // 1:30
  detailedValues(2) = 1.0; // 2:00
  detailedValues(3) = 1.5; // 2:30
  detailedValues(4) = 2.0; // 3:00

  TimeSeries detailedTimeSeries(dateTimes, detailedValues, units);
  ASSERT_TRUE(!detailedTimeSeries.values().empty());

  // sum and difference
  TimeSeries sum = intervalTimeSeries + detailedTimeSeries;
  TimeSeries diff1 = intervalTimeSeries - detailedTimeSeries;
  TimeSeries diff2 = detailedTimeSeries - intervalTimeSeries;
  ASSERT_TRUE(!sum.values().empty());
  ASSERT_TRUE(!diff1.values().empty());
  ASSERT_TRUE(!diff2.values().empty());

//  EXPECT_EQ((unsigned)5, sum.dateTimes().size());
//  EXPECT_EQ((unsigned)5, diff1.dateTimes().size());
//  EXPECT_EQ((unsigned)5, diff2.dateTimes().size());
  EXPECT_EQ((unsigned)5, sum.daysFromFirstReport().size());
  EXPECT_EQ((unsigned)5, diff1.daysFromFirstReport().size());
  EXPECT_EQ((unsigned)5, diff2.daysFromFirstReport().size());

//  EXPECT_EQ(startDateTime, sum.dateTimes().front());
//  EXPECT_EQ(startDateTime, diff1.dateTimes().front());
//  EXPECT_EQ(startDateTime, diff2.dateTimes().front());
  EXPECT_EQ(startDateTime, sum.firstReportDateTime());
  EXPECT_EQ(startDateTime, diff1.firstReportDateTime());
  EXPECT_EQ(startDateTime, diff2.firstReportDateTime());

  DateTime endDateTime = startDateTime + Time(0,2,0,0);
//  EXPECT_EQ(endDateTime, sum.dateTimes().back());
//  EXPECT_EQ(endDateTime, diff1.dateTimes().back());
//  EXPECT_EQ(endDateTime, diff2.dateTimes().back());
  EXPECT_EQ(endDateTime, sum.firstReportDateTime() + Time(sum.daysFromFirstReport(sum.daysFromFirstReport().size()-1)));
  EXPECT_EQ(endDateTime, diff1.firstReportDateTime() +  Time(diff1.daysFromFirstReport(diff1.daysFromFirstReport().size()-1)));
  EXPECT_EQ(endDateTime, diff2.firstReportDateTime() +  Time(diff2.daysFromFirstReport(diff2.daysFromFirstReport().size()-1)));

  // 1:00
  EXPECT_EQ(0, sum.value(Time(0,0,0,0)));
  EXPECT_EQ(0, diff1.value(Time(0,0,0,0)));
  EXPECT_EQ(0, diff2.value(Time(0,0,0,0)));

  // 1:30
  EXPECT_EQ(1.5, sum.value(Time(0,0,30,0)));
  EXPECT_EQ(0.5, diff1.value(Time(0,0,30,0)));
  EXPECT_EQ(-0.5, diff2.value(Time(0,0,30,0)));

  // 2:00
  EXPECT_EQ(2, sum.value(Time(0,1,0,0)));
  EXPECT_EQ(0.0, diff1.value(Time(0,1,0,0)));
  EXPECT_EQ(0.0, diff2.value(Time(0,1,0,0)));

  // 2:30
  EXPECT_EQ(3.5, sum.value(Time(0,1,30,0)));
  EXPECT_EQ(0.5, diff1.value(Time(0,1,30,0)));
  EXPECT_EQ(-0.5, diff2.value(Time(0,1,30,0)));

  // Test helper function for summing a vector.
  TimeSeriesVector sumAndDiffs;
  sumAndDiffs.push_back(sum);
  sumAndDiffs.push_back(diff1);
  sumAndDiffs.push_back(diff2);

  TimeSeries ans = openstudio::sum(sumAndDiffs);
  EXPECT_FALSE(ans.values().empty());

  // 1:00
  EXPECT_DOUBLE_EQ(0, ans.value(Time(0,0,0,0)));
  // 1:30
  EXPECT_DOUBLE_EQ(1.5, ans.value(Time(0,0,30,0)));
  // 2:00
  EXPECT_DOUBLE_EQ(2.0, ans.value(Time(0,1,0,0)));
  // 2:30
  EXPECT_DOUBLE_EQ(3.5, ans.value(Time(0,1,30,0)));

  // Test multiplication and division with a scalar
  sumAndDiffs.push_back(sum/2.0);
  /*
  sumAndDiffs.push_back(3.0*diff1);
  ans = openstudio::sum(sumAndDiffs);
  EXPECT_FALSE(ans.values().empty());
  // 1:00
  EXPECT_DOUBLE_EQ(0, ans.value(Time(0,0,0,0)));
  // 1:30
  EXPECT_DOUBLE_EQ(3.75, ans.value(Time(0,0,30,0)));
  // 2:00
  EXPECT_DOUBLE_EQ(3.0, ans.value(Time(0,1,0,0)));
  // 2:30
  EXPECT_DOUBLE_EQ(6.75, ans.value(Time(0,1,30,0)));*/
}

TEST_F(DataFixture, TimeSeries_Multiply8760)
{
  // Test out mulitplication on a detailed series and an iterval series
  std::string units = "C";

  // Interval
  Time interval = Time(0, 1);

  // Values
  Vector values = linspace(1, 8760, 8760);

  // Date/times for detailed series
  Date startDate(Date(MonthOfYear(MonthOfYear::Jan), 1));
  DateTime firstReportDateTime(startDate, Time(0, 1, 0, 0));
  Date endDate(Date(MonthOfYear(MonthOfYear::Dec), 31));
  DateTime endDateTime(endDate, Time(0, 24, 0, 0));
  Time delta(0, 1, 0, 0);
  std::vector<DateTime> dateTimes;
  for (openstudio::DateTime current = firstReportDateTime; current <= endDateTime; current += delta) {
    dateTimes.push_back(current);
  }

  // Time series objects
  TimeSeries intervalTimeSeries(firstReportDateTime, interval, values, units);
  TimeSeries detailedTimeSeries(dateTimes, values, units);

  // Multiply interval series
  TimeSeries mult = 3 * intervalTimeSeries;

  // Run tests
  Vector mvals = mult.values();
  for (unsigned i = 0; i < 8760; i++) {
    EXPECT_EQ(3 * values[i], mvals[i]);
  }
  openstudio::OptionalTime minter = mult.intervalLength();
  ASSERT_TRUE(minter);
  EXPECT_EQ(interval, minter.get());
  EXPECT_EQ(firstReportDateTime, mult.firstReportDateTime());

  // Multiply detailed series
  mult = 3 * detailedTimeSeries;

  // Run tests
  mvals = mult.values();
  for (unsigned i = 0; i < 8760; i++) {
    EXPECT_EQ(3 * values[i], mvals[i]);
  }
  minter = mult.intervalLength();
  EXPECT_FALSE(minter);
  EXPECT_EQ(firstReportDateTime, mult.firstReportDateTime());

}

TEST_F(DataFixture, TimeSeries_Yearly)
{
  std::string units = "W";

  Date startDate(MonthOfYear(MonthOfYear::Jan), 1);
  DateTime startDateTime(startDate);
  Time interval(0, 8760, 0, 0);
  DateTime firstReportDateTime(startDate, interval);

  std::vector<double> values = { 1.0 };
  Vector idioticVector(1);
  idioticVector[0] = 1.0;

  std::vector<double> daysFromFirstReport0 = { 0.0 };
  std::vector<double> daysFromFirstReport8760 = { 365.0 };

  TimeSeries intervalTimeSeries(firstReportDateTime, interval, idioticVector, units);
  ASSERT_THROW(TimeSeries firstAndDaysTimeSeries0(firstReportDateTime, daysFromFirstReport0, values, units), openstudio::Exception);
  TimeSeries firstAndDaysTimeSeries8760(firstReportDateTime, daysFromFirstReport8760, values, units);

  // Check computations
  EXPECT_EQ(31536000, intervalTimeSeries.integrate());
  EXPECT_EQ(1, intervalTimeSeries.averageValue());
  EXPECT_EQ(31536000, firstAndDaysTimeSeries8760.integrate());
  EXPECT_EQ(1, firstAndDaysTimeSeries8760.averageValue());

}

TEST_F(DataFixture, TimeSeries_Monthly)
{
  std::string units = "W";

  Date startDate(MonthOfYear(MonthOfYear::Jan), 1);
  DateTime startDateTime(startDate);
  Time interval = Date(MonthOfYear(MonthOfYear::Feb), 1) - startDate;
  DateTime firstReportDateTime(startDate, interval);

  std::vector<double> values = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0 };

  std::vector<double> daysFromFirstReport = { 0.0, 28.0, 59.0, 89.0, 120.0, 150.0, 181.0, 212.0, 242.0, 273.0, 303.0, 334.0 };
  std::vector<double> daysFromStart = { 31.0, 59.0, 90.0, 120.0, 151.0, 181.0, 212.0, 243.0, 273.0, 304.0, 334.0, 365.0 };
  // 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31

  ASSERT_THROW(TimeSeries firstTimeSeries(firstReportDateTime, daysFromFirstReport, values, units), openstudio::Exception);
  TimeSeries startTimeSeries(firstReportDateTime, daysFromStart, values, units);

  // Check computations
  EXPECT_EQ(205804800, startTimeSeries.integrate());
}

double calculate_sum(int dayStart, int dayEnd) {
  return 24 * (dayEnd - dayStart + 1) / 2*(dayEnd + dayStart);
}

TEST_F(DataFixture, TimeSeries_values_Leap_FullYear) {

  // picking 2012 because it's a Leap Year
  int year = 2012;

  // Simulate having a SQL timeseries for a full year, hourly
  Date startDate(MonthOfYear(MonthOfYear::Jan), 1, year);

  // Each day has the same value corresponding to the day number (eg 1 for Jan 1, 2 for Jan 2, etc) for all timesteps
  int n_vals = 366 * 24;
  std::vector<double> values;
  values.resize(n_vals);
  for (int i=0; i < n_vals; ++i) {
    values[i] = (i / 24) + 1;
  }

  TimeSeries timeSeries(startDate, openstudio::Time(0,1), openstudio::createVector(values), "lux");

  Time startTime(0, 0, 1, 0);
  Time endTime(0, 24, 0, 0);

  std::vector<std::pair<openstudio::MonthOfYear, int>> daysInMonth = {
    {MonthOfYear::Jan, 31},
    {MonthOfYear::Feb, 29},
    {MonthOfYear::Mar, 31},
    {MonthOfYear::Apr, 30},
    {MonthOfYear::May, 31},
    {MonthOfYear::Jun, 30},
    {MonthOfYear::Jul, 31},
    {MonthOfYear::Aug, 31},
    {MonthOfYear::Sep, 30},
    {MonthOfYear::Oct, 31},
    {MonthOfYear::Nov, 30},
    {MonthOfYear::Dec, 31},
  };

  // Hardcode years
  {
    int dayStart = 1;
    for (const auto& monthEntry: daysInMonth) {
      const MonthOfYear& month = monthEntry.first;
      const int& ndays= monthEntry.second;

      DateTime startDateTime(Date(month, 1, year), startTime);
      DateTime endDateTime(Date(month, ndays, year), endTime);
      Vector vals = timeSeries.values(startDateTime, endDateTime);
      EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;

      int dayEnd = ndays + dayStart - 1;
      // Calculate the Sum and compare it to the values we actually retrieved to ensure we sliced correctly
      EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
        << " Failed between " << startDateTime << " and " << endDateTime;

      dayStart += ndays;

    }
  }

  // Cannot *not* harcode year because it'll assume a non leap year (2009) and produce a problem with Feb 29
}

TEST_F(DataFixture, TimeSeries_values_NonLeap_FullYear) {

  // picking 2011 because it's a non Leap Year
  int year = 2011;

  // Simulate having a SQL timeseries for a full year, hourly
  Date startDate(MonthOfYear(MonthOfYear::Jan), 1, year);

  // Each day has the same value corresponding to the day number (eg 1 for Jan 1, 2 for Jan 2, etc) for all timesteps
  int n_vals = 365 * 24;
  std::vector<double> values;
  values.resize(n_vals);
  for (int i=0; i < n_vals; ++i) {
    values[i] = (i / 24) + 1;
  }

  TimeSeries timeSeries(startDate, openstudio::Time(0,1), openstudio::createVector(values), "lux");

  Time startTime(0, 1, 0, 0);
  Time endTime(0, 24, 1, 0);

  std::vector<std::pair<openstudio::MonthOfYear, int>> daysInMonth = {
    {MonthOfYear::Jan, 31},
    {MonthOfYear::Feb, 28},
    {MonthOfYear::Mar, 31},
    {MonthOfYear::Apr, 30},
    {MonthOfYear::May, 31},
    {MonthOfYear::Jun, 30},
    {MonthOfYear::Jul, 31},
    {MonthOfYear::Aug, 31},
    {MonthOfYear::Sep, 30},
    {MonthOfYear::Oct, 31},
    {MonthOfYear::Nov, 30},
    {MonthOfYear::Dec, 31},
  };

  // Hardcode years
  {
    int dayStart = 1;
    for (const auto& monthEntry: daysInMonth) {
      const MonthOfYear& month = monthEntry.first;
      const int& ndays= monthEntry.second;


      DateTime startDateTime(Date(month, 1, year), startTime);
      DateTime endDateTime(Date(month, ndays, year), endTime);
      Vector vals = timeSeries.values(startDateTime, endDateTime);
      EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;

      int dayEnd = ndays + dayStart - 1;
      // Calculate the Sum and compare it to the values we actually retrieved to ensure we sliced correctly
      EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
         << " Failed between " << startDateTime << " and " << endDateTime;

      dayStart += ndays;

    }
  }

  // Don't hardcode years
  {
    int dayStart = 1;
    for (const auto& monthEntry: daysInMonth) {
      const MonthOfYear& month = monthEntry.first;
      const int& ndays= monthEntry.second;

      DateTime startDateTime(Date(month, 1), startTime);
      DateTime endDateTime(Date(month, ndays), endTime);
      Vector vals = timeSeries.values(startDateTime, endDateTime);
      EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;

      int dayEnd = ndays + dayStart - 1;
      // Calculate the Sum and compare it to the values we actually retrieved to ensure we sliced correctly
      EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
        << " Failed between " << startDateTime << " and " << endDateTime;

      dayStart += ndays;

    }
  }

}

TEST_F(DataFixture, TimeSeries_values_WrapAround_Hardcode) {

  // picking 2011 because it's a not a Leap Year but 2012 is
  int year = 2011;

  // Simulate having a SQL timeseries for two full years, one non leap and one leap, hourly
  Date startDate(MonthOfYear(MonthOfYear::Jan), 1, year);

  // Each day has the same value corresponding to the day number (eg 1 for Jan 1, 2 for Jan 2, etc) for all timesteps
  int n_vals = (365+366) * 24;
  std::vector<double> values;
  values.resize(n_vals);
  for (int i=0; i < n_vals; ++i) {
    values[i] = (i / 24) + 1;
  }

  TimeSeries timeSeries(startDate, openstudio::Time(0,1), openstudio::createVector(values), "lux");

  Time startTime(0, 1, 0, 0);
  Time endTime(0, 24, 1, 0);

  std::vector<std::pair<openstudio::MonthOfYear, int>> daysInMonth = {
    {MonthOfYear::Jan, 31},
    {MonthOfYear::Feb, 28},
    {MonthOfYear::Mar, 31},
    {MonthOfYear::Apr, 30},
    {MonthOfYear::May, 31},
    {MonthOfYear::Jun, 30},
    {MonthOfYear::Jul, 31},
    {MonthOfYear::Aug, 31},
    {MonthOfYear::Sep, 30},
    {MonthOfYear::Oct, 31},
    {MonthOfYear::Nov, 30},
    {MonthOfYear::Dec, 31},

    {MonthOfYear::Jan, 31},
    {MonthOfYear::Feb, 29},
    {MonthOfYear::Mar, 31},
    {MonthOfYear::Apr, 30},
    {MonthOfYear::May, 31},
    {MonthOfYear::Jun, 30},
    {MonthOfYear::Jul, 31},
    {MonthOfYear::Aug, 31},
    {MonthOfYear::Sep, 30},
    {MonthOfYear::Oct, 31},
    {MonthOfYear::Nov, 30},
    {MonthOfYear::Dec, 31},
  };

  // Hardcode years
  {
    int n = 0;
    int dayStart = 1;
    for (const auto& monthEntry: daysInMonth) {
      ++n;
      int thisYear = year;
      if (n > 12) {
        thisYear = year + 1;
      }
      const MonthOfYear& month = monthEntry.first;
      const int& ndays= monthEntry.second;

      DateTime startDateTime(Date(month, 1, thisYear), startTime);
      DateTime endDateTime(Date(month, ndays, thisYear), endTime);
      Vector vals = timeSeries.values(startDateTime, endDateTime);
      EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;

      int dayEnd = ndays + dayStart - 1;
      // Calculate the Sum and compare it to the values we actually retrieved to ensure we sliced correctly
      EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
         << " Failed between " << startDateTime << " and " << endDateTime;

      dayStart += ndays;

    }
  }

  // Try weird stuff now
  {
    DateTime startDateTime(Date(MonthOfYear::Dec, 1, year), startTime);
    DateTime endDateTime(Date(MonthOfYear::Jan, 31, year + 1), endTime);
    Vector vals = timeSeries.values(startDateTime, endDateTime);

    int dayStart = 335;
    int ndays = 31 + 31;
    int dayEnd = ndays + dayStart - 1;

    EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;


    EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
         << " Failed between " << startDateTime << " and " << endDateTime;
  }

  {
    DateTime startDateTime(Date(MonthOfYear::Dec, 1, year), startTime);
    DateTime endDateTime(Date(MonthOfYear::Mar, 31, year + 1), endTime);
    Vector vals = timeSeries.values(startDateTime, endDateTime);

    int dayStart = 335;
    int ndays = 31 + 31 + 29 + 31;
    int dayEnd = ndays + dayStart - 1;

    EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;


    EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
         << " Failed between " << startDateTime << " and " << endDateTime;
  }

}


TEST_F(DataFixture, TimeSeries_values_WrapAround_NoHardcode) {

  // picking 2011 because it's a not a Leap Year but 2012 is
  int year = 2011;

  // Simulate having a SQL timeseries for two full years, one non leap and one leap, hourly
  Date startDate(MonthOfYear(MonthOfYear::Jan), 1, year);

  // Each day has the same value corresponding to the day number (eg 1 for Jan 1, 2 for Jan 2, etc) for all timesteps
  int n_vals = (365+366) * 24;
  std::vector<double> values;
  values.resize(n_vals);
  for (int i=0; i < n_vals; ++i) {
    values[i] = (i / 24) + 1;
  }

  TimeSeries timeSeries(startDate, openstudio::Time(0,1), openstudio::createVector(values), "lux");

  Time startTime(0, 1, 0, 0);
  Time endTime(0, 24, 1, 0);

  std::vector<std::pair<openstudio::MonthOfYear, int>> daysInMonth = {
    {MonthOfYear::Jan, 31},
    {MonthOfYear::Feb, 28},
    {MonthOfYear::Mar, 31},
    {MonthOfYear::Apr, 30},
    {MonthOfYear::May, 31},
    {MonthOfYear::Jun, 30},
    {MonthOfYear::Jul, 31},
    {MonthOfYear::Aug, 31},
    {MonthOfYear::Sep, 30},
    {MonthOfYear::Oct, 31},
    {MonthOfYear::Nov, 30},
    {MonthOfYear::Dec, 31},

    {MonthOfYear::Jan, 31},
    {MonthOfYear::Feb, 29},
    {MonthOfYear::Mar, 31},
    {MonthOfYear::Apr, 30},
    {MonthOfYear::May, 31},
    {MonthOfYear::Jun, 30},
    {MonthOfYear::Jul, 31},
    {MonthOfYear::Aug, 31},
    {MonthOfYear::Sep, 30},
    {MonthOfYear::Oct, 31},
    {MonthOfYear::Nov, 30},
    {MonthOfYear::Dec, 31},
  };

  // Don't Hardcode years: can only retrieve months of first years
  {
    int n = 0;
    int dayStart = 1;
    for (const auto& monthEntry: daysInMonth) {
      ++n;
      int thisYear = year;
      if (n > 12) {
        break;
      }
      const MonthOfYear& month = monthEntry.first;
      const int& ndays= monthEntry.second;

      DateTime startDateTime(Date(month, 1, thisYear), startTime);
      DateTime endDateTime(Date(month, ndays, thisYear), endTime);
      Vector vals = timeSeries.values(startDateTime, endDateTime);
      EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;

      int dayEnd = ndays + dayStart - 1;
      // Calculate the Sum and compare it to the values we actually retrieved to ensure we sliced correctly
      EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
         << " Failed between " << startDateTime << " and " << endDateTime;

      dayStart += ndays;

    }
  }

  // Try some stuff now

  // No years passed, no leap, end < start
  {
    DateTime startDateTime(Date(MonthOfYear::Dec, 1), startTime);
    DateTime endDateTime(Date(MonthOfYear::Jan, 31), endTime);
    Vector vals = timeSeries.values(startDateTime, endDateTime);

    int dayStart = 335;
    int ndays = 31 + 31;
    int dayEnd = ndays + dayStart - 1;

    EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;


    EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
         << " Failed between " << startDateTime << " and " << endDateTime;
  }

  // No years passed, leap, end < start
  {
    DateTime startDateTime(Date(MonthOfYear::Dec, 1), startTime);
    DateTime endDateTime(Date(MonthOfYear::Mar, 31), endTime);
    Vector vals = timeSeries.values(startDateTime, endDateTime);

    int dayStart = 335;
    int ndays = 31 + 31 + 29 + 31;
    int dayEnd = ndays + dayStart - 1;

    EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;


    EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
         << " Failed between " << startDateTime << " and " << endDateTime;
  }

  // Start Year passed, end year defaulted but after start
  {
    DateTime startDateTime(Date(MonthOfYear::Jan, 1, year), startTime);
    // Expect this to default to Feb 28, year
    DateTime endDateTime(Date(MonthOfYear::Feb, 28), endTime);

    Vector vals = timeSeries.values(startDateTime, endDateTime);

    int dayStart = 1;
    int ndays = 31 + 28;
    int dayEnd = ndays + dayStart - 1;

    EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;


    EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
         << " Failed between " << startDateTime << " and " << endDateTime;

  }

  // Start Year passed, end year defaulted but before start in terms of Month/Day combo
  {
    DateTime startDateTime(Date(MonthOfYear::Dec, 1, year), startTime);
    // Will default to Jan 31 year+1
    DateTime endDateTime(Date(MonthOfYear::Jan, 31), endTime);

    Vector vals = timeSeries.values(startDateTime, endDateTime);

    int dayStart = 335;
    int ndays = 31 + 31;
    int dayEnd = ndays + dayStart - 1;

    EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;


    EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
         << " Failed between " << startDateTime << " and " << endDateTime;

  }

  // Start Year not passed, end year hard assigned
  {
    // Will default to Jan 1, year
    DateTime startDateTime(Date(MonthOfYear::Jan, 1), startTime);
    DateTime endDateTime(Date(MonthOfYear::Jan, 31, year), endTime);

    Vector vals = timeSeries.values(startDateTime, endDateTime);

    int dayStart = 1;
    int ndays = 31;
    int dayEnd = ndays + dayStart - 1;

    EXPECT_EQ(ndays * 24, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;


    EXPECT_EQ(calculate_sum(dayStart, dayEnd), openstudio::sum(vals))
         << " Failed between " << startDateTime << " and " << endDateTime;

  }

  // Start Year not passed, end year hard assigned but incoherent (before start)
  {
    // Will default to Feb 1, year
    DateTime startDateTime(Date(MonthOfYear::Feb, 1), startTime);
    DateTime endDateTime(Date(MonthOfYear::Jan, 1, year), endTime);

    Vector vals = timeSeries.values(startDateTime, endDateTime);

    EXPECT_EQ(0, vals.size())
        << " Failed between " << startDateTime << " and " << endDateTime;

  }

}
