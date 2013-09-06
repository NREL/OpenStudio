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
#include <utilities/data/Test/DataFixture.hpp>

#include <utilities/data/TimeSeries.hpp>
#include <utilities/time/Date.hpp>
#include <utilities/time/Time.hpp>

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
  DateTime startDateTime(startDate, Time(0,1,0,0)); // note: first data point at 1:00 am (covers period from 12:00 pm < t <= 1:00 am)

  // interval
  Time interval = Time(0,1,0,0);

  // create two timeSeries with hourly interval, should be the same
  TimeSeries timeSeries1(startDate, interval, values, units);
  TimeSeries timeSeries2(startDateTime, interval, values, units);
  ASSERT_TRUE(!timeSeries1.values().empty());
  ASSERT_TRUE(!timeSeries2.values().empty());

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
  EXPECT_EQ(-99, timeSeries1.value(startDateTime + Time(0,0,-61,0))); // out of range
  EXPECT_EQ(-99, timeSeries1.value(startDateTime + Time(0,0,-60,0))); // out of range
  EXPECT_EQ(0, timeSeries1.value(startDateTime + Time(0,0,-59,0))); // in range
  EXPECT_EQ(2, timeSeries1.value(endDateTime)); // in range
  EXPECT_EQ(-99, timeSeries1.value(endDateTime + Time(0,1,0,0))); // out of range

  EXPECT_EQ(-99, timeSeries2.value(startDateTime + Time(0,0,-61,0))); // out of range
  EXPECT_EQ(-99, timeSeries2.value(startDateTime + Time(0,0,-60,0))); // out of range
  EXPECT_EQ(0, timeSeries2.value(startDateTime + Time(0,0,-59,0))); // in range
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


TEST_F(DataFixture,TimeSeries_DetailedConstructor)
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
  EXPECT_DOUBLE_EQ(6.75, ans.value(Time(0,1,30,0)));
}
