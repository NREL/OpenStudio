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
#include <energyplus/Test/EnergyPlusFixture.hpp>

#include <energyplus/ForwardTranslator.hpp>
#include <energyplus/ReverseTranslator.hpp>

#include <utilities/data/TimeSeries.hpp>
#include <utilities/time/Date.hpp>
#include <utilities/time/Time.hpp>

#include <model/Model.hpp>
#include <model/ScheduleInterval.hpp>
#include <model/ScheduleInterval_Impl.hpp>
#include <model/ScheduleFixedInterval.hpp>
#include <model/ScheduleFixedInterval_Impl.hpp>
#include <model/ScheduleVariableInterval.hpp>
#include <model/ScheduleVariableInterval_Impl.hpp>

#include <boost/regex.hpp>

#include <sstream>


using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_ScheduleFixedInterval_Hourly)
{
  //Vector values = linspace(0, 100, 8760);
  Vector values = linspace(1, 8760, 8760);

  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1), Time(0,1,0)), Time(0,1,0), values, "");

  Model model;

  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  ASSERT_TRUE(scheduleInterval);
  EXPECT_TRUE(scheduleInterval->optionalCast<ScheduleFixedInterval>());

  ForwardTranslator ft;

  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  ASSERT_EQ(1u, objects.size());

  boost::regex throughRegex("^Through:\\s*(.*)/\\s*(.*)\\s*");
  boost::regex untilRegex("^Until:\\s*(.*):(.*)\\s*");

  workspace.save(toPath("./ForwardTranslator_ScheduleFixedInterval_Hourly.idf"), true);

  unsigned N = objects[0].numFields();
  boost::optional<Date> lastDateThrough;
  bool until24Found = false;
  bool nextValueShouldBeLast = false;
  unsigned numUntils = 0;
  
  for ( unsigned i = 0; i < N; ++i){
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast){
      double value = boost::lexical_cast<double>(*field);
      EXPECT_EQ(8760.0, value);
      nextValueShouldBeLast = false;
    }

    boost::smatch throughMatches;
    if (boost::regex_search(*field, throughMatches, throughRegex)){

      std::string monthText(throughMatches[1].first, throughMatches[1].second);
      std::string dayText(throughMatches[2].first, throughMatches[2].second);

      int month = boost::lexical_cast<int>(monthText);
      int day = boost::lexical_cast<int>(dayText);

      Date date(MonthOfYear(month), day);
      if (lastDateThrough){
        // check that this date is greater than last date
        EXPECT_TRUE(date > *lastDateThrough) << date << " <= " << *lastDateThrough;

        // DLM: this schedule should not wrap around to 1/1, it should end on 12/31 at 24:00

        // check that last date was closed at 24:00
        EXPECT_TRUE(until24Found);
      }
      lastDateThrough = date;
      until24Found = false;
    }

    boost::smatch untilMatches;
    if (boost::regex_search(*field, untilMatches, untilRegex)){

      numUntils += 1;

      std::string hrText(untilMatches[1].first, untilMatches[1].second);
      std::string minText(untilMatches[2].first, untilMatches[2].second);

      int hr = boost::lexical_cast<int>(hrText);
      int min = boost::lexical_cast<int>(minText);
      EXPECT_TRUE(hr <= 24);
      EXPECT_TRUE(min < 60);

      if ((hr == 24) && (min == 0)){
        until24Found = true;
      }

      // should see Until: 00:00,
      EXPECT_FALSE((hr==0) && (min==0));

      if ((lastDateThrough == Date(MonthOfYear(12),31)) && until24Found){
        nextValueShouldBeLast = true;
      }
    }
  }
  bool lastUntil24Found = until24Found;

  // check last date was closed
  EXPECT_TRUE(lastUntil24Found);

  // check that there were 8760 untils
  EXPECT_EQ(8760, numUntils);
}

TEST_F(EnergyPlusFixture,ForwardTranslator_ScheduleFixedInterval_20hours)
{
  //Vector values = linspace(0, 100, 8760);
  Vector values = linspace(20, 8760, 438);

  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1),Time(0,20,0)), Time(0,20,0), values, "");

  Model model;

  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  ASSERT_TRUE(scheduleInterval);
  EXPECT_TRUE(scheduleInterval->optionalCast<ScheduleFixedInterval>());

  ForwardTranslator ft;

  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  ASSERT_EQ(1u, objects.size());

  boost::regex throughRegex("^Through:\\s*(.*)/\\s*(.*)\\s*");
  boost::regex untilRegex("^Until:\\s*(.*):(.*)\\s*");

  workspace.save(toPath("./ForwardTranslator_ScheduleFixedInterval_20hours.idf"), true);

  unsigned N = objects[0].numFields();
  boost::optional<Date> lastDateThrough;
  bool until24Found = false;
  bool nextValueShouldBeLast = false;
  unsigned numUntils = 0;
  
  for ( unsigned i = 0; i < N; ++i){
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast){
      double value = boost::lexical_cast<double>(*field);
      EXPECT_EQ(8760.0, value);
      nextValueShouldBeLast = false;
    }

    boost::smatch throughMatches;
    if (boost::regex_search(*field, throughMatches, throughRegex)){

      std::string monthText(throughMatches[1].first, throughMatches[1].second);
      std::string dayText(throughMatches[2].first, throughMatches[2].second);

      int month = boost::lexical_cast<int>(monthText);
      int day = boost::lexical_cast<int>(dayText);

      Date date(MonthOfYear(month), day);
      if (lastDateThrough){
        // check that this date is greater than last date
        EXPECT_TRUE(date > *lastDateThrough) << date << " <= " << *lastDateThrough;

        // DLM: this schedule should not wrap around to 1/1, it should end on 12/31 at 24:00

        // check that last date was closed at 24:00
        EXPECT_TRUE(until24Found);
      }
      lastDateThrough = date;
      until24Found = false;
    }

    boost::smatch untilMatches;
    if (boost::regex_search(*field, untilMatches, untilRegex)){

      numUntils += 1;

      std::string hrText(untilMatches[1].first, untilMatches[1].second);
      std::string minText(untilMatches[2].first, untilMatches[2].second);

      int hr = boost::lexical_cast<int>(hrText);
      int min = boost::lexical_cast<int>(minText);
      EXPECT_TRUE(hr <= 24);
      EXPECT_TRUE(min < 60);

      if ((hr == 24) && (min == 0)){
        until24Found = true;
      }

      // should see Until: 00:00,
      EXPECT_FALSE((hr==0) && (min==0));

      if ((lastDateThrough == Date(MonthOfYear(12),31)) && until24Found){
        nextValueShouldBeLast = true;
      }
    }
  }
  bool lastUntil24Found = until24Found;

  // check last date was closed
  EXPECT_TRUE(lastUntil24Found);

  // every 5 days a data point coincides with the end of the day, 365/5 = 73
  // then: 365 - 73 + 438 = 730 untils
  EXPECT_EQ(730, numUntils);
}


TEST_F(EnergyPlusFixture,ForwardTranslator_ScheduleFixedInterval_ThreePoint)
{
  std::vector<double> vec(3,0.0);
  vec[2] = 1.0;
  Vector values = openstudio::createVector(vec);

  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,0)), Time(0,4380,0), values, "");

  Model model;

  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  // this should fail - the interval is 182.5 days
  ASSERT_FALSE(scheduleInterval);
}

/*
TEST_F(EnergyPlusFixture,ForwardTranslator_ScheduleFixedInterval_TwoPoint)
{
  std::vector<double> vals(2,0.0);
  vals[1] = 1.0;
  Vector values = openstudio::createVector(vals);

  std::vector<DateTime> dts;
  dts.push_back(DateTime(Date(MonthOfYear::Jul, 2), Time(0,12,0)));
  dts.push_back(DateTime(Date(MonthOfYear::Dec,31)) + Time(0,24,0));

  //TimeSeries timeseries(DateTime(Date(MonthOfYear::Jul, 2), Time(0,12,0)), Time(0,4380,0), values, "");
  TimeSeries timeseries(dts, values, "");

  Model model;

  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  ASSERT_TRUE(scheduleInterval);
  EXPECT_TRUE(scheduleInterval->optionalCast<ScheduleFixedInterval>());

  ForwardTranslator ft;

  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  ASSERT_EQ(1u, objects.size());

  boost::regex throughRegex("^Through:\\s*(.*)/\\s*(.*)\\s*");
  boost::regex untilRegex("^Until:\\s*(.*):(.*)\\s*");

  workspace.save(toPath("./ForwardTranslator_ScheduleFixedInterval_ThreePoint.idf"), true);

  unsigned N = objects[0].numFields();
  boost::optional<Date> lastDateThrough;
  bool until24Found = false;
  bool nextValueShouldBeLast = false;
  unsigned numUntils = 0;
  
  for ( unsigned i = 0; i < N; ++i){
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast){
      double value = boost::lexical_cast<double>(*field);
      EXPECT_EQ(1.0, value);
      nextValueShouldBeLast = false;
    }

    boost::smatch throughMatches;
    if (boost::regex_search(*field, throughMatches, throughRegex)){

      std::string monthText(throughMatches[1].first, throughMatches[1].second);
      std::string dayText(throughMatches[2].first, throughMatches[2].second);

      int month = boost::lexical_cast<int>(monthText);
      int day = boost::lexical_cast<int>(dayText);

      Date date(MonthOfYear(month), day);
      if (lastDateThrough){
        // check that this date is greater than last date
        EXPECT_TRUE(date > *lastDateThrough) << date << " <= " << *lastDateThrough;

        // DLM: this schedule should not wrap around to 1/1, it should end on 12/31 at 24:00

        // check that last date was closed at 24:00
        EXPECT_TRUE(until24Found);
      }
      lastDateThrough = date;
      until24Found = false;
    }

    boost::smatch untilMatches;
    if (boost::regex_search(*field, untilMatches, untilRegex)){

      numUntils += 1;

      std::string hrText(untilMatches[1].first, untilMatches[1].second);
      std::string minText(untilMatches[2].first, untilMatches[2].second);

      int hr = boost::lexical_cast<int>(hrText);
      int min = boost::lexical_cast<int>(minText);
      EXPECT_TRUE(hr <= 24);
      EXPECT_TRUE(min < 60);

      if ((hr == 24) && (min == 0)){
        until24Found = true;
      }

      // should see Until: 00:00,
      EXPECT_FALSE((hr==0) && (min==0));

      if ((lastDateThrough == Date(MonthOfYear(12),31)) && until24Found){
        nextValueShouldBeLast = true;
      }
    }
  }
  bool lastUntil24Found = until24Found;

  // check last date was closed
  EXPECT_TRUE(lastUntil24Found);

  // check that there were 366 untils
  EXPECT_EQ(366, numUntils);
}
*/