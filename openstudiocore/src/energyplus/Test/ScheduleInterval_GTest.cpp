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
  bool hitNextValueShouldBeLast = false;
  unsigned numUntils = 0;

  int currentHour = 0;
  for ( unsigned i = 0; i < N; ++i){
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast){
      double value = boost::lexical_cast<double>(*field);
      hitNextValueShouldBeLast = true;
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
  EXPECT_TRUE(hitNextValueShouldBeLast);
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

  // Create a time series that starts at 01/01 00:00 (first timestamp = 01/01 01:00)
  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1)), Time(0, 1, 0), values, "");

  Model model;

  // Create a schedule and make sure it worked
  boost::optional<ScheduleInterval> scheduleInterval = ScheduleInterval::fromTimeSeries(timeseries, model);
  ASSERT_TRUE(scheduleInterval);
  EXPECT_TRUE(scheduleInterval->optionalCast<ScheduleFixedInterval>());

  // Verify that the schedule gives us back the time series
  TimeSeries ts = scheduleInterval->timeSeries();
  EXPECT_EQ(DateTime(Date(MonthOfYear::Jan, 1), Time(0, 1, 0)), ts.firstReportDateTime());

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
  bool hitNextValueShouldBeLast = false;
  unsigned numUntils = 0;

  int currentHour = 0;
  for (unsigned i = 0; i < N; ++i) {
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast) {
      double value = boost::lexical_cast<double>(*field);
      hitNextValueShouldBeLast = true;
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
  EXPECT_TRUE(hitNextValueShouldBeLast);
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
  bool hitNextValueShouldBeLast = false;
  unsigned numUntils = 0;

  for ( unsigned i = 0; i < N; ++i){
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast){
      double value = boost::lexical_cast<double>(*field);
      hitNextValueShouldBeLast = true;
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
  EXPECT_TRUE(hitNextValueShouldBeLast);
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

// TODO: This test was disabled since it is failing, need to fix it
TEST_F(EnergyPlusFixture, DISABLED_ForwardTranslator_ScheduleFixedInterval_TwoPoint)
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
  bool hitNextValueShouldBeLast = false;
  unsigned numUntils = 0;

  int currentHour = 0;
  for ( unsigned i = 0; i < N; ++i){
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast){
      double value = boost::lexical_cast<double>(*field);
      hitNextValueShouldBeLast = true;
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
  EXPECT_TRUE(hitNextValueShouldBeLast);
  bool lastUntil24Found = until24Found;

  // check last date was closed
  EXPECT_TRUE(lastUntil24Found);

  // check that there were 8760 untils
  EXPECT_EQ(8760, numUntils);
}

// TODO: This test was disabled since it is failing, need to fix it
TEST_F(EnergyPlusFixture, DISABLED_ForwardTranslator_ScheduleVariableInterval_Hourly_Shifted)
{
  // Create the values vector and a vector of seconds from the start
  Vector values = linspace(1, 8760, 8760);
  std::vector<long> seconds(8760);
  for(unsigned i=0;i<seconds.size();i++) {
    seconds[i]=(i+1)*3600;
  }

  // Create an hourly time series starting 12/31 23:00:00 (first tstamp = 1/1 00:00 with 3600seconds since the start)
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
  workspace.save(toPath("./ForwardTranslator_ScheduleVariableInterval_Hourly_Shifted.idf"), true);

  // Check the contents of the output
  unsigned N = objects[0].numFields();
  boost::optional<Date> lastDateThrough;
  bool until24Found = false;
  bool nextValueShouldBeLast = false;
  bool hitNextValueShouldBeLast = false;
  unsigned numUntils = 0;

  int currentHour = 0;
  for ( unsigned i = 0; i < N; ++i){
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);
    std::cout << "i=," << i << ", field=" << field << "\n";

    if (nextValueShouldBeLast){
      double value = boost::lexical_cast<double>(*field);
      hitNextValueShouldBeLast = true;
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

      // TODO: The next check fails at the end of the schedule
      // because the last lines of the schedule are:
      /*
       *Until: 22:00,                           !- Field 18245
       *8759,                                   !- Field 18246
       *Until: 24:00,                           !- Field 18247
       *8760;                                   !- Field 18248
       */

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
  EXPECT_TRUE(hitNextValueShouldBeLast);
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
  bool hitNextValueShouldBeLast = false;
  unsigned numUntils = 0;

  for (unsigned i = 0; i < N; ++i){
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast){
      double value = boost::lexical_cast<double>(*field);
      hitNextValueShouldBeLast = true;
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
  EXPECT_TRUE(hitNextValueShouldBeLast);
  bool lastUntil24Found = until24Found;

  // check last date was closed
  EXPECT_TRUE(lastUntil24Found);

  // check that there were 8760 untils
  EXPECT_EQ(864, numUntils);
}

// TODO: This test was disabled since it is failing, need to fix it
TEST_F(EnergyPlusFixture, DISABLED_ForwardTranslator_ScheduleVariableInterval_500_Shifted)
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
  bool hitNextValueShouldBeLast = false;
  unsigned numUntils = 0;

  for (unsigned i = 0; i < N; ++i) {
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast) {
      double value = boost::lexical_cast<double>(*field);
      hitNextValueShouldBeLast = true;
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
  EXPECT_TRUE(hitNextValueShouldBeLast);
  bool lastUntil24Found = until24Found;

  // check last date was closed
  EXPECT_TRUE(lastUntil24Found);

  // check that there were 8760 untils
  EXPECT_EQ(864, numUntils);
}

// TODO: This test was disabled since it is failing, need to fix it
TEST_F(EnergyPlusFixture, DISABLED_ForwardTranslator_ScheduleVariableInterval_DaysTimeSeries)
{
  // The vector of time deltas, randomly generated
  std::vector<double> timeInDays = { 5.756944444444445, 5.763888888888889, 5.770833333333333, 5.777777777777778, 5.784722222222222,
    12.993055555555555, 13.0, 13.402777777777779, 13.409722222222221, 17.291666666666668, 17.29861111111111, 26.395833333333332,
    26.40277777777778, 28.36111111111111, 28.368055555555557, 29.819444444444443, 29.82638888888889, 35.40277777777778,
    35.40972222222222, 40.736111111111114, 40.74305555555556, 40.75, 40.84722222222222, 40.854166666666664, 40.861111111111114,
    48.22222222222222, 48.229166666666664, 48.236111111111114, 56.388888888888886, 56.395833333333336, 60.666666666666664,
    60.673611111111114, 60.68055555555556, 67.84027777777777, 67.84722222222223, 67.85416666666667, 68.24305555555556, 68.25,
    68.875, 68.88194444444444, 68.88888888888889, 71.25694444444444, 71.26388888888889, 76.02083333333333, 76.02777777777777,
    77.9375, 77.94444444444444, 77.95138888888889, 82.27083333333333, 82.27777777777777, 82.28472222222223, 83.96527777777777,
    83.97222222222223, 87.61111111111111, 87.61805555555556, 88.75694444444444, 88.76388888888889, 93.28472222222223, 93.29166666666667,
    93.29861111111111, 100.25, 100.25694444444444, 100.26388888888889, 101.34027777777777, 101.34722222222223, 103.59027777777777,
    103.59722222222223, 103.60416666666667, 104.86805555555556, 104.88194444444444, 105.88888888888889, 105.89583333333333,
    106.21527777777777, 106.22222222222223, 106.22916666666667, 106.80555555555556, 106.8125, 109.77777777777777, 109.78472222222223,
    109.97916666666667, 109.98611111111111, 110.75694444444444, 110.76388888888889, 112.53472222222223, 112.54166666666667,
    112.54861111111111, 114.09722222222223, 114.10416666666667, 119.88888888888889, 119.89583333333333, 120.84027777777777,
    120.84722222222223, 120.85416666666667, 122.29166666666667, 122.29861111111111, 125.81944444444444, 125.82638888888889,
    125.83333333333333, 130.96527777777777, 130.97222222222223, 132.39583333333334, 132.40277777777777, 132.40972222222223,
    134.27777777777777, 134.28472222222223, 134.29166666666666, 138.46527777777777, 138.47222222222223, 138.47916666666666,
    140.64583333333334, 140.65277777777777, 144.27777777777777, 144.29166666666666, 148.9375, 148.94444444444446, 148.95138888888889,
    151.73611111111111, 151.74305555555554, 154.625, 154.63194444444446, 162.34722222222223, 162.35416666666666, 162.36111111111111,
    170.59722222222223, 170.60416666666666, 171.875, 171.88194444444446, 174.75694444444446, 174.76388888888889, 174.8125,
    174.81944444444446, 180.86805555555554, 180.88194444444446, 183.79166666666666, 183.79861111111111, 183.80555555555554,
    190.88888888888889, 190.89583333333334, 190.90277777777777, 195.99305555555554, 196.0, 196.00694444444446, 210.90277777777777,
    210.90972222222223, 210.91666666666666, 214.90972222222223, 214.91666666666666, 214.92361111111111, 216.98611111111111,
    216.99305555555554, 219.8125, 219.82638888888889, 232.6875, 232.69444444444446, 236.81944444444446, 236.83333333333334,
    247.36805555555554, 247.375, 248.30555555555554, 248.3125, 249.86805555555554, 249.875, 253.20138888888889, 253.20833333333334,
    253.34027777777777, 253.34722222222223, 253.35416666666666, 257.7986111111111, 257.80555555555554, 257.8125, 258.96527777777777,
    258.97222222222223, 258.9791666666667, 269.90972222222223, 269.9166666666667, 270.28472222222223, 270.2916666666667,
    270.2986111111111, 275.77777777777777, 275.78472222222223, 275.7916666666667, 279.8263888888889, 279.8333333333333,
    281.4166666666667, 281.4236111111111, 281.43055555555554, 287.31944444444446, 287.3263888888889, 287.3333333333333,
    287.3958333333333, 287.40277777777777, 287.40972222222223, 299.96527777777777, 299.97222222222223, 300.61805555555554,
    300.625, 300.63194444444446, 303.5625, 303.56944444444446, 306.69444444444446, 306.7013888888889, 307.88194444444446,
    307.8888888888889, 316.56944444444446, 316.5763888888889, 316.5833333333333, 319.9791666666667, 319.9861111111111,
    322.84027777777777, 322.84722222222223, 322.8541666666667, 325.0416666666667, 325.0486111111111, 326.84027777777777,
    326.84722222222223, 326.8541666666667, 327.7013888888889, 327.7083333333333, 327.71527777777777, 333.75, 333.75694444444446,
    334.5833333333333, 334.59027777777777, 335.8888888888889, 335.8958333333333, 337.3958333333333, 337.40277777777777,
    343.96527777777777, 343.97222222222223, 347.2916666666667, 347.2986111111111, 349.27777777777777, 349.28472222222223,
    351.96527777777777, 351.9791666666667, 352.90972222222223, 352.9236111111111, 359.3611111111111, 359.36805555555554,
    359.8263888888889, 359.8333333333333, 365.0 };
  std::vector<double> values = { 0.0, 0.2005415, 0.0401083, 0.1557844, 0.23367659999999998, 0.0, 0.2684628, 0.0, 0.2907288, 0.0,
    0.23589539999999998, 0.0, 0.24801920000000002, 0.0, 0.32789219999999997, 0.0, 0.2, 0.0, 0.2147385, 0.0, 0.1881668, 0.0470417,
    0.0, 0.1040148, 0.0260037, 0.0, 0.10277339999999999, 0.30832019999999993, 0.0, 0.5604991, 0.0, 0.28898579999999996,
    0.24082149999999997, 0.0, 0.370206, 0.12340199999999998, 0.0, 0.5014266000000001, 0.0, 0.0979134, 0.2937402, 0.0, 0.0948494,
    0.0, 0.3474904, 0.0, 0.0438457, 0.2192285, 0.0, 0.0962757, 0.0320919, 0.0, 0.1309298, 0.0, 0.0369915, 0.0, 0.0958796, 0.0,
    0.33058620000000005, 0.11019540000000001, 0.0, 0.419506, 0.1678024, 0.0, 0.1865292, 0.0, 0.290742, 0.0581484, 0.0, 0.358558,
    0.0, 0.2745246, 0.0, 0.1878369, 0.37567379999999995, 0.0, 0.5625992, 0.0, 0.5054014, 0.0, 0.403249, 0.0, 0.4173932000000001,
    0.0, 0.0578315, 0.231326, 0.0, 0.38299, 0.0, 0.5692554000000001, 0.0, 0.0481775, 0.1445325, 0.0, 0.21553079999999997, 0.0,
    0.075317, 0.451902, 0.0, 0.269678, 0.0, 0.2527735, 0.10110939999999999, 0.0, 0.2254359, 0.0751453, 0.0, 0.0987322,
    0.14809830000000002, 0.0, 0.271857, 0.0, 0.1546356, 0.0, 0.157686, 0.09461159999999999, 0.0, 0.09569459999999999, 0.0,
    0.237982, 0.0, 0.2733755, 0.16402529999999999, 0.0, 0.2867936, 0.0, 0.45252269999999994, 0.0, 0.3679872, 0.0, 0.2070785, 0.0,
    0.0578051, 0.0, 0.0443476, 0.1773904, 0.0, 0.207871, 0.12472259999999999, 0.0, 0.1103276, 0.3309828, 0.0, 0.4419835, 0.1894215,
    0.0, 0.2180136, 0.1635102, 0.0, 0.3376255, 0.0, 0.2937136, 0.0, 0.368727, 0.0, 0.1937004, 0.0, 0.20402829999999997, 0.0, 0.1961964,
    0.0, 0.3432648, 0.0, 0.3979795, 0.0, 0.10929739999999999, 0.32789219999999997, 0.0, 0.0758056, 0.2274168, 0.0, 0.11019540000000001,
    0.33058620000000005, 0.0, 0.38811419999999996, 0.0, 0.2277336, 0.1138668, 0.0, 0.1480718, 0.2961436, 0.0, 0.5244056999999999, 0.0,
    0.15546759999999998, 0.2332014, 0.0, 0.1625991, 0.21679879999999999, 0.0, 0.08553870000000001, 0.0570258, 0.0, 0.1840992, 0.0,
    0.35110949999999996, 0.10031699999999999, 0.0, 0.3194665, 0.0, 0.1801902, 0.0, 0.12595079999999997, 0.0, 0.054384600000000005,
    0.10876920000000001, 0.0, 0.2735868, 0.0, 0.080877, 0.12131549999999999, 0.0, 0.5322107, 0.0, 0.0677364, 0.2032092, 0.0,
    0.28298999999999996, 0.07074749999999999, 0.0, 0.1815768, 0.0, 0.17072099999999998, 0.0, 0.14098, 0.0, 0.260222, 0.0,
    0.48543319999999995, 0.0, 0.19251179999999998, 0.0, 0.292525, 0.0, 0.2233492, 0.0, 0.136186, 0.0, 0.44833619999999996, 0.0,
    0.2968304, 0.0 };

  // The vector of values
  //Vector values = linspace(1, 500, 500);
  //std::vector<DateTime> dts;
  //for (double days : timeInDays) {
  //  dts.push_back(DateTime(Date(MonthOfYear::Jan, 1), Time(days)));
  //}

  //std::vector<long> timeInSeconds;
  //for (double days : timeInDays) {
  //  timeInSeconds.push_back((long)(days * 24 * 60 * 60));
  //}

  // Create a time series from the data
  TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1), Time(timeInDays[0])), timeInDays, values, "");
  //TimeSeries timeseries(dts, createVector(values), "");
  //TimeSeries timeseries(DateTime(Date(MonthOfYear::Jan, 1), Time(timeInDays[0])), timeInSeconds, createVector(values), "");

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
  workspace.save(toPath("./ForwardTranslator_ScheduleVariableInterval_DaysTimeSeries.idf"), true);

  // Check the contents of the output
  unsigned N = objects[0].numFields();
  boost::optional<Date> lastDateThrough;
  bool until24Found = false;
  bool nextValueShouldBeLast = false;
  bool hitNextValueShouldBeLast = false;
  unsigned numUntils = 0;

  for (unsigned i = 0; i < N; ++i) {
    boost::optional<std::string> field = objects[0].getString(i, true, false);
    ASSERT_TRUE(field);

    if (nextValueShouldBeLast) {
      double value = boost::lexical_cast<double>(*field);
      hitNextValueShouldBeLast = true;
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
  EXPECT_TRUE(hitNextValueShouldBeLast);
  bool lastUntil24Found = until24Found;

  // check last date was closed
  EXPECT_TRUE(lastUntil24Found);

  // check that there were XX untils
  //EXPECT_EQ(864, numUntils);
}
