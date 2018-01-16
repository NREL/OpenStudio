/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"

#include "../../model/Model.hpp"
#include "../../model/ScheduleInterval.hpp"
#include "../../model/ScheduleInterval_Impl.hpp"
#include "../../model/ScheduleFixedInterval.hpp"
#include "../../model/ScheduleFixedInterval_Impl.hpp"
#include "../../model/ScheduleVariableInterval.hpp"
#include "../../model/ScheduleVariableInterval_Impl.hpp"

#include <utilities/idd/IddEnums.hxx>

#include <boost/regex.hpp>

#include <sstream>


using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture,ForwardTranslator_ScheduleFixedInterval_Hourly)
{
  // Create the values vector
  Vector values = linspace(1, 8760, 8760);

  // Create an hourly interval time series that begins on 1/1 00:00
  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1), Time(0,1,0)), Time(0,1,0), values, "");

  Model model;

  // Create a schedule and make sure that it worked
  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  ASSERT_TRUE(scheduleInterval);
  EXPECT_TRUE(scheduleInterval->optionalCast<ScheduleFixedInterval>());

  // Forward translate the schedule
  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  ASSERT_EQ(1u, objects.size());

  boost::regex throughRegex("^Through:\\s*(.*)/\\s*(.*)\\s*");
  boost::regex untilRegex("^Until:\\s*(.*):(.*)\\s*");

  // Write out the schedule - keep this around for now
  //workspace.save(toPath("./ForwardTranslator_ScheduleFixedInterval_Hourly.idf"), true);

  // Check the contents of the output
  unsigned N = objects[0].numFields();
  boost::optional<Date> lastDateThrough;
  bool until24Found = false;
  bool nextValueShouldBeLast = false;
  unsigned numUntils = 0;

  int currentHour = 0;
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

      currentHour = 1;

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
      EXPECT_EQ(currentHour, hr);
      ++currentHour;
      EXPECT_EQ(0, min);

      if ((hr == 24) && (min == 0)){
        until24Found = true;
      }

      // should NOT see Until: 00:00,
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

TEST_F(EnergyPlusFixture, ForwardTranslator_ScheduleFixedInterval_Hourly_Shifted)
{
  // Create the values vector
  Vector values = linspace(1, 8760, 8760);

  // Create a time series that starts at 12/31 23:00
  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1)), Time(0, 1, 0), values, "");

  Model model;

  // Create a schedule and make sure it worked
  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  ASSERT_TRUE(scheduleInterval);
  EXPECT_TRUE(scheduleInterval->optionalCast<ScheduleFixedInterval>());

  // Verify that the schedule gives us back the time series
  TimeSeries ts = scheduleInterval->timeSeries();
  // Oops, it doesn't. Maybe it shouldn't give back the exact time series, but it can't do this.
  // Without this check, the schedule manages to pass everything else and the test succeeds.
  EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,0)), ts.firstReportDateTime());

  // Forward translate the schedule
  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  ASSERT_EQ(1u, objects.size());

  boost::regex throughRegex("^Through:\\s*(.*)/\\s*(.*)\\s*");
  boost::regex untilRegex("^Until:\\s*(.*):(.*)\\s*");

  // Write out the schedule -  keep this around for now
  //workspace.save(toPath("./ForwardTranslator_ScheduleFixedInterval_Hourly_Shifted.idf"), true);

  // Check the contents of the output
  unsigned N = objects[0].numFields();
  boost::optional<Date> lastDateThrough;
  bool until24Found = false;
  bool nextValueShouldBeLast = false;
  unsigned numUntils = 0;

  int currentHour = 0;
  for (unsigned i = 0; i < N; ++i) {
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast) {
      double value = boost::lexical_cast<double>(*field);
      EXPECT_EQ(8760.0, value);
      nextValueShouldBeLast = false;
    }

    boost::smatch throughMatches;
    if (boost::regex_search(*field, throughMatches, throughRegex)) {

      currentHour = 1;

      std::string monthText(throughMatches[1].first, throughMatches[1].second);
      std::string dayText(throughMatches[2].first, throughMatches[2].second);

      int month = boost::lexical_cast<int>(monthText);
      int day = boost::lexical_cast<int>(dayText);

      Date date(MonthOfYear(month), day);
      if (lastDateThrough) {
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
    if (boost::regex_search(*field, untilMatches, untilRegex)) {

      numUntils += 1;

      std::string hrText(untilMatches[1].first, untilMatches[1].second);
      std::string minText(untilMatches[2].first, untilMatches[2].second);

      int hr = boost::lexical_cast<int>(hrText);
      int min = boost::lexical_cast<int>(minText);
      EXPECT_EQ(currentHour, hr);
      ++currentHour;
      EXPECT_EQ(0, min);

      if ((hr == 24) && (min == 0)) {
        until24Found = true;
      }

      // should NOT see Until: 00:00,
      EXPECT_FALSE((hr == 0) && (min == 0));

      if ((lastDateThrough == Date(MonthOfYear(12), 31)) && until24Found) {
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
  // Create the values vector
  Vector values = linspace(20, 8760, 438);

  // Create an interval time series starting 1/1 00:00 with 20 hour interval
  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1),Time(0,20,0)), Time(0,20,0), values, "");

  Model model;

  // Create a schedule and make sure it worked
  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  ASSERT_TRUE(scheduleInterval);
  EXPECT_TRUE(scheduleInterval->optionalCast<ScheduleFixedInterval>());

  // Forward translate the schedule
  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  ASSERT_EQ(1u, objects.size());

  boost::regex throughRegex("^Through:\\s*(.*)/\\s*(.*)\\s*");
  boost::regex untilRegex("^Until:\\s*(.*):(.*)\\s*");

  // Write out the schedule - keep this around for now
  //workspace.save(toPath("./ForwardTranslator_ScheduleFixedInterval_20hours.idf"), true);

  // Check the contents of the output
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

      // should NOT see Until: 00:00,
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
  // Create the values vector
  std::vector<double> vec(3,0.0);
  vec[2] = 1.0;
  Vector values = openstudio::createVector(vec);

  // Create an interval time series starting 1/1 00:00 with interval 182.5 days
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

TEST_F(EnergyPlusFixture,ForwardTranslator_ScheduleVariableInterval_Hourly)
{
  // Create the values vector and a vector of seconds from the start
  Vector values = linspace(1, 8760, 8760);
  std::vector<long> seconds(8760);
  for(unsigned i=0;i<seconds.size();i++) {
    seconds[i]=(i+1)*3600;
  }

  // Create an hourly time series starting 1/1 00:00
  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1), Time(0, 0, 0, 3600)), seconds, values, "");

  Model model;

  // Creat a schedule and make sure it worked
  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  ASSERT_TRUE(scheduleInterval);
  ASSERT_TRUE(scheduleInterval->optionalCast<ScheduleVariableInterval>());

  // Forward translate the schedule
  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  ASSERT_EQ(1u, objects.size());

  boost::regex throughRegex("^Through:\\s*(.*)/\\s*(.*)\\s*");
  boost::regex untilRegex("^Until:\\s*(.*):(.*)\\s*");

  // Write out the schedule - keep this around for now
  //workspace.save(toPath("./ForwardTranslator_ScheduleVariableInterval_Hourly.idf"), true);

  // Check the contents of the output
  unsigned N = objects[0].numFields();
  boost::optional<Date> lastDateThrough;
  bool until24Found = false;
  bool nextValueShouldBeLast = false;
  unsigned numUntils = 0;

  int currentHour = 0;
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

      currentHour = 1;

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
      EXPECT_EQ(currentHour, hr);
      ++currentHour;
      EXPECT_EQ(0, min);

      if ((hr == 24) && (min == 0)){
        until24Found = true;
      }

      // should NOT see Until: 00:00,
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

TEST_F(EnergyPlusFixture,ForwardTranslator_ScheduleVariableInterval_Hourly_Shifted)
{
  // Create the values vector and a vector of seconds from the start
  Vector values = linspace(1, 8760, 8760);
  std::vector<long> seconds(8760);
  for(unsigned i=0;i<seconds.size();i++) {
    seconds[i]=(i+1)*3600;
  }

  // Create an hourly time series starting 12/31 23:00:00
  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1)), seconds, values, "");

  Model model;

  // Create a schedule and make sure it worked
  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  ASSERT_TRUE(scheduleInterval);
  ASSERT_TRUE(scheduleInterval->optionalCast<ScheduleVariableInterval>());

  // Forward translate the schedule
  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  ASSERT_EQ(1u, objects.size());

  boost::regex throughRegex("^Through:\\s*(.*)/\\s*(.*)\\s*");
  boost::regex untilRegex("^Until:\\s*(.*):(.*)\\s*");

  // Write out the schedule - keep this around for now
  //workspace.save(toPath("./ForwardTranslator_ScheduleVariableInterval_Hourly_Shifted.idf"), true);

  // Check the contents of the output
  unsigned N = objects[0].numFields();
  boost::optional<Date> lastDateThrough;
  bool until24Found = false;
  bool nextValueShouldBeLast = false;
  unsigned numUntils = 0;

  int currentHour = 0;
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

      currentHour = 1;

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
      EXPECT_EQ(currentHour, hr);
      ++currentHour;
      EXPECT_EQ(0, min);

      if ((hr == 24) && (min == 0)){
        until24Found = true;
      }

      // should NOT see Until: 00:00,
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

TEST_F(EnergyPlusFixture,ForwardTranslator_ScheduleVariableInterval_500)
{
  // A vector of time deltas, randomly generated
  long numbers[500] = {86313,48668,46739,86313,86313,35939,28787,81175,41086,60467,71308,36332,
    75050,44913,86313,36150,86313,86313,86313,86313,49633,23793,86313,86313,86313,78168,72707,
    56754,35868,29986,25413,40812,62829,31364,77455,76379,86313,50167,34835,70623,86313,70817,
    48215,52955,59462,27876,27334,52308,32575,86313,86313,53883,57044,86313,45829,38304,42861,
    86313,86313,29215,35662,86313,86313,63866,73697,58074,80833,85161,60866,85214,38748,86313,
    86313,36676,33285,86313,38269,86313,67373,56445,86313,85745,54775,51067,86313,23644,34226,
    86313,71364,64214,45392,86313,49499,71086,50232,86313,86313,75456,62390,78586,67472,66497,
    51741,76133,86313,52833,72670,65395,86313,86180,67179,86313,86313,86313,33630,48006,42881,
    69935,39664,63806,41471,35258,31512,83115,86313,73282,86313,86313,53505,47981,61876,86313,
    31543,86313,37030,66163,47482,58352,86313,73380,86313,46900,59984,57889,41704,42690,86313,
    86313,51497,71154,42596,73589,27242,61291,43573,86313,65535,86313,58071,86313,79620,86313,
    26578,32739,86313,34133,70385,27867,81956,72324,38580,29633,33298,59005,27369,32838,61259,
    35188,77004,86313,60872,86313,49717,43171,27070,35573,30796,86313,86313,86313,86313,63163,
    77254,45155,86313,86313,57614,59737,48050,86313,86313,72316,52488,60663,50962,35766,81130,
    70904,34878,76407,68502,82977,86313,55383,33518,86313,29334,28864,44800,64615,86313,78882,
    66482,50578,78395,66541,68202,83235,61364,81762,86313,47870,34782,46302,50788,49417,44120,
    74002,25114,86313,24457,53465,26107,33636,86313,80121,64977,30723,40309,86313,86313,76107,
    86313,33956,52007,62567,86313,58138,41487,31619,33507,62906,35965,86313,43492,86313,24031,
    83944,86313,86313,71510,86284,56508,28345,39127,86313,37952,54719,79620,36024,86313,86313,
    86313,86313,30591,83941,64038,74265,75851,56856,44849,27458,30350,24754,31329,56821,55979,
    39576,50515,52438,84957,85891,51660,86313,49117,86313,63442,24063,77802,29621,37905,53515,
    72884,81821,86313,86313,86313,86313,84048,86313,76408,70785,55767,86313,41679,83722,85612,
    86313,86313,29925,86313,52021,86313,78981,86313,86313,50598,74333,67904,86313,31582,86313,
    70034,86313,35070,74448,45073,86313,86313,71613,35444,63476,53592,84978,26717,58140,61863,
    84549,58604,34180,62200,36225,83922,33905,83064,24935,51077,86313,49476,61010,41624,65246,
    30697,86313,26883,86313,75916,48961,86313,53663,86313,38177,51527,79510,86313,26712,74984,
    64740,86313,49378,86313,86313,47440,32930,25094,76695,68608,35416,86313,86313,26731,86313,
    65665,57451,86313,64561,47067,76935,31435,72573,86313,38846,72254,42634,86313,86313,71265,
    86313,56154,86313,66939,86313,30369,36409,67475,86313,86313,86313,86313,38236,86313,57408,
    66435,29802,72062,86313,43899,86313,47590,64377,53064,68569,37843,86313,86313,86313,86313,
    69253,54914,33398,26446,49428,59356,86313,86313,86313,78915,44437,61977,75481,86313,86313,
    47774,71126,86313,84201,86313,66183,44427,59010,24018,34729,86313,32745,86313,48847,61975,
    76417,86313,86313,66541,51775,86313,86129,36853,86313,70306,46162,86313,86313,29768,46103,
    24318,26234,59933,43230,65930,86313,86313,54580};

  Vector values = linspace(1, 500, 500);

  // Create the vector of seconds from start
  std::vector<long> seconds(500);
  seconds[0] = numbers[0];
  for(unsigned i=1;i<500;i++) {
    seconds[i]=numbers[i]+seconds[i-1];
  }

  // Create a time series starting 1/1 00:00 with irregular interval
  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1), Time(0,0,0,numbers[0])), seconds, values, "");

  Model model;

  // Create a schedule and make sure it worked
  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  ASSERT_TRUE(scheduleInterval);
  ASSERT_TRUE(scheduleInterval->optionalCast<ScheduleVariableInterval>());

  // Forward translate the schedule
  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  ASSERT_EQ(1u, objects.size());

  boost::regex throughRegex("^Through:\\s*(.*)/\\s*(.*)\\s*");
  boost::regex untilRegex("^Until:\\s*(.*):(.*)\\s*");

  // Write out the schedule - keep this around for now
  //workspace.save(toPath("./ForwardTranslator_ScheduleVariableInterval_500.idf"), true);


  // Check the contents of the output
  unsigned N = objects[0].numFields();
  boost::optional<Date> lastDateThrough;
  bool until24Found = false;
  bool nextValueShouldBeLast = false;
  unsigned numUntils = 0;

  for (unsigned i = 0; i < N; ++i){
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast){
      double value = boost::lexical_cast<double>(*field);
      EXPECT_EQ(500.0, value);
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

      // should NOT see Until: 00:00,
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
  EXPECT_EQ(864, numUntils);
}

TEST_F(EnergyPlusFixture, ForwardTranslator_ScheduleVariableInterval_500_Shifted)
{
  // The vector of time deltas, randomly generated
  long numbers[500] = { 86313, 48668, 46739, 86313, 86313, 35939, 28787, 81175, 41086, 60467, 71308, 36332,
    75050, 44913, 86313, 36150, 86313, 86313, 86313, 86313, 49633, 23793, 86313, 86313, 86313, 78168, 72707,
    56754, 35868, 29986, 25413, 40812, 62829, 31364, 77455, 76379, 86313, 50167, 34835, 70623, 86313, 70817,
    48215, 52955, 59462, 27876, 27334, 52308, 32575, 86313, 86313, 53883, 57044, 86313, 45829, 38304, 42861,
    86313, 86313, 29215, 35662, 86313, 86313, 63866, 73697, 58074, 80833, 85161, 60866, 85214, 38748, 86313,
    86313, 36676, 33285, 86313, 38269, 86313, 67373, 56445, 86313, 85745, 54775, 51067, 86313, 23644, 34226,
    86313, 71364, 64214, 45392, 86313, 49499, 71086, 50232, 86313, 86313, 75456, 62390, 78586, 67472, 66497,
    51741, 76133, 86313, 52833, 72670, 65395, 86313, 86180, 67179, 86313, 86313, 86313, 33630, 48006, 42881,
    69935, 39664, 63806, 41471, 35258, 31512, 83115, 86313, 73282, 86313, 86313, 53505, 47981, 61876, 86313,
    31543, 86313, 37030, 66163, 47482, 58352, 86313, 73380, 86313, 46900, 59984, 57889, 41704, 42690, 86313,
    86313, 51497, 71154, 42596, 73589, 27242, 61291, 43573, 86313, 65535, 86313, 58071, 86313, 79620, 86313,
    26578, 32739, 86313, 34133, 70385, 27867, 81956, 72324, 38580, 29633, 33298, 59005, 27369, 32838, 61259,
    35188, 77004, 86313, 60872, 86313, 49717, 43171, 27070, 35573, 30796, 86313, 86313, 86313, 86313, 63163,
    77254, 45155, 86313, 86313, 57614, 59737, 48050, 86313, 86313, 72316, 52488, 60663, 50962, 35766, 81130,
    70904, 34878, 76407, 68502, 82977, 86313, 55383, 33518, 86313, 29334, 28864, 44800, 64615, 86313, 78882,
    66482, 50578, 78395, 66541, 68202, 83235, 61364, 81762, 86313, 47870, 34782, 46302, 50788, 49417, 44120,
    74002, 25114, 86313, 24457, 53465, 26107, 33636, 86313, 80121, 64977, 30723, 40309, 86313, 86313, 76107,
    86313, 33956, 52007, 62567, 86313, 58138, 41487, 31619, 33507, 62906, 35965, 86313, 43492, 86313, 24031,
    83944, 86313, 86313, 71510, 86284, 56508, 28345, 39127, 86313, 37952, 54719, 79620, 36024, 86313, 86313,
    86313, 86313, 30591, 83941, 64038, 74265, 75851, 56856, 44849, 27458, 30350, 24754, 31329, 56821, 55979,
    39576, 50515, 52438, 84957, 85891, 51660, 86313, 49117, 86313, 63442, 24063, 77802, 29621, 37905, 53515,
    72884, 81821, 86313, 86313, 86313, 86313, 84048, 86313, 76408, 70785, 55767, 86313, 41679, 83722, 85612,
    86313, 86313, 29925, 86313, 52021, 86313, 78981, 86313, 86313, 50598, 74333, 67904, 86313, 31582, 86313,
    70034, 86313, 35070, 74448, 45073, 86313, 86313, 71613, 35444, 63476, 53592, 84978, 26717, 58140, 61863,
    84549, 58604, 34180, 62200, 36225, 83922, 33905, 83064, 24935, 51077, 86313, 49476, 61010, 41624, 65246,
    30697, 86313, 26883, 86313, 75916, 48961, 86313, 53663, 86313, 38177, 51527, 79510, 86313, 26712, 74984,
    64740, 86313, 49378, 86313, 86313, 47440, 32930, 25094, 76695, 68608, 35416, 86313, 86313, 26731, 86313,
    65665, 57451, 86313, 64561, 47067, 76935, 31435, 72573, 86313, 38846, 72254, 42634, 86313, 86313, 71265,
    86313, 56154, 86313, 66939, 86313, 30369, 36409, 67475, 86313, 86313, 86313, 86313, 38236, 86313, 57408,
    66435, 29802, 72062, 86313, 43899, 86313, 47590, 64377, 53064, 68569, 37843, 86313, 86313, 86313, 86313,
    69253, 54914, 33398, 26446, 49428, 59356, 86313, 86313, 86313, 78915, 44437, 61977, 75481, 86313, 86313,
    47774, 71126, 86313, 84201, 86313, 66183, 44427, 59010, 24018, 34729, 86313, 32745, 86313, 48847, 61975,
    76417, 86313, 86313, 66541, 51775, 86313, 86129, 36853, 86313, 70306, 46162, 86313, 86313, 29768, 46103,
    24318, 26234, 59933, 43230, 65930, 86313, 86313, 54580 };

  // The vector of values
  Vector values = linspace(1, 500, 500);

  // Create the vector of seconds from start
  std::vector<long> seconds(500);
  seconds[0] = numbers[0];
  for (unsigned i = 1; i<500; i++) {
    seconds[i] = numbers[i] + seconds[i - 1];
  }

  // Create a time series starting 12/31 00:00:-86313 with irregular interval
  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1)), seconds, values, "");

  Model model;

  // Create a schedule and make sure it worked
  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  ASSERT_TRUE(scheduleInterval);
  ASSERT_TRUE(scheduleInterval->optionalCast<ScheduleVariableInterval>());

  // Forward translate the schedule
  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  std::vector<WorkspaceObject> objects = workspace.getObjectsByType(IddObjectType::Schedule_Compact);
  ASSERT_EQ(1u, objects.size());

  boost::regex throughRegex("^Through:\\s*(.*)/\\s*(.*)\\s*");
  boost::regex untilRegex("^Until:\\s*(.*):(.*)\\s*");

  // Write out the schedule - keep this around for now
  //workspace.save(toPath("./ForwardTranslator_ScheduleVariableInterval_500_Shifted.idf"), true);

  // Check the contents of the output
  unsigned N = objects[0].numFields();
  boost::optional<Date> lastDateThrough;
  bool until24Found = false;
  bool nextValueShouldBeLast = false;
  unsigned numUntils = 0;

  for (unsigned i = 0; i < N; ++i) {
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast) {
      double value = boost::lexical_cast<double>(*field);
      EXPECT_EQ(500.0, value);
      nextValueShouldBeLast = false;
    }

    boost::smatch throughMatches;
    if (boost::regex_search(*field, throughMatches, throughRegex)) {

      std::string monthText(throughMatches[1].first, throughMatches[1].second);
      std::string dayText(throughMatches[2].first, throughMatches[2].second);

      int month = boost::lexical_cast<int>(monthText);
      int day = boost::lexical_cast<int>(dayText);

      Date date(MonthOfYear(month), day);
      if (lastDateThrough) {
        // check that this date is greater than last date
        EXPECT_TRUE(date > *lastDateThrough) << date << " <= " << *lastDateThrough;

        // check that last date was closed at 24:00
        EXPECT_TRUE(until24Found);
      }
      lastDateThrough = date;
      until24Found = false;
    }

    boost::smatch untilMatches;
    if (boost::regex_search(*field, untilMatches, untilRegex)) {

      numUntils += 1;

      std::string hrText(untilMatches[1].first, untilMatches[1].second);
      std::string minText(untilMatches[2].first, untilMatches[2].second);

      int hr = boost::lexical_cast<int>(hrText);
      int min = boost::lexical_cast<int>(minText);
      EXPECT_TRUE(hr <= 24);
      EXPECT_TRUE(min < 60);

      if ((hr == 24) && (min == 0)) {
        until24Found = true;
      }

      // should NOT see Until: 00:00,
      EXPECT_FALSE((hr == 0) && (min == 0));

      if ((lastDateThrough == Date(MonthOfYear(12), 31)) && until24Found) {
        nextValueShouldBeLast = true;
      }
    }
  }
  bool lastUntil24Found = until24Found;

  // check last date was closed
  EXPECT_TRUE(lastUntil24Found);

  // check that there were 8760 untils
  EXPECT_EQ(864, numUntils);
}
