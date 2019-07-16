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

#include "ModelFixture.hpp"
#include "../ScheduleFixedInterval.hpp"
#include "../ScheduleFixedInterval_Impl.hpp"
#include "../ExternalFile.hpp"
#include "../ExternalFile_Impl.hpp"
#include "../ScheduleFile.hpp"
#include "../ScheduleFile_Impl.hpp"
#include "../ScheduleVariableInterval.hpp"
#include "../ScheduleVariableInterval_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"

#include "../../utilities/core/PathHelpers.hpp"
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

TEST_F(ModelFixture, ScheduleFile)
{
  Model model;
  EXPECT_EQ(0u, model.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<ScheduleFile>().size());

  path p = resourcesPath() / toPath("model/schedulefile.csv");
  EXPECT_TRUE(exists(p));

  path expectedDestDir;
  std::vector<path> absoluteFilePaths = model.workflowJSON().absoluteFilePaths();
  if (absoluteFilePaths.empty()) {
    expectedDestDir = model.workflowJSON().absoluteRootDir();
  } else {
    expectedDestDir = absoluteFilePaths[0];
  }

  if (exists(expectedDestDir)) {
    removeDirectory(expectedDestDir);
  }
  ASSERT_FALSE(exists(expectedDestDir));
  
  boost::optional<ExternalFile> externalfile = ExternalFile::getExternalFile(model, openstudio::toString(p));
  ASSERT_TRUE(externalfile);
  EXPECT_EQ(1u, model.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0u, externalfile->scheduleFiles().size());
  EXPECT_EQ(openstudio::toString(p.filename()), externalfile->fileName());
  //EXPECT_TRUE(externalfile.isColumnSeparatorDefaulted());
  EXPECT_TRUE(equivalent(expectedDestDir / externalfile->fileName(), externalfile->filePath()));
  EXPECT_TRUE(exists(externalfile->filePath()));
  EXPECT_NE(p, externalfile->filePath());

  ScheduleFile schedule(*externalfile);
  EXPECT_EQ(1u, model.getConcreteModelObjects<ScheduleFile>().size());
  EXPECT_EQ(1u, externalfile->scheduleFiles().size());
  EXPECT_EQ(externalfile->handle(), schedule.externalFile().handle());
  EXPECT_TRUE(schedule.isNumberofHoursofDataDefaulted());
  EXPECT_EQ(1, schedule.columnNumber());
  schedule.setColumnNumber(1);
  EXPECT_EQ(1, schedule.columnNumber());
  EXPECT_EQ(0, schedule.rowstoSkipatTop());
  schedule.setRowstoSkipatTop(1);
  EXPECT_EQ(1, schedule.rowstoSkipatTop());

  EXPECT_EQ("Comma", schedule.columnSeparator());
  EXPECT_TRUE(schedule.isColumnSeparatorDefaulted());
  EXPECT_TRUE(schedule.setColumnSeparator("Tab"));
  EXPECT_EQ("Tab", schedule.columnSeparator());

  ScheduleFile schedule2(*externalfile);
  EXPECT_EQ(2u, model.getConcreteModelObjects<ScheduleFile>().size());
  EXPECT_EQ(2u, externalfile->scheduleFiles().size());
  EXPECT_EQ(externalfile->handle(), schedule2.externalFile().handle());
  EXPECT_TRUE(schedule2.isNumberofHoursofDataDefaulted());
  EXPECT_EQ(1, schedule2.columnNumber());
  schedule2.setColumnNumber(2);
  EXPECT_EQ(2, schedule2.columnNumber());
  EXPECT_EQ(0, schedule2.rowstoSkipatTop());
  schedule2.setRowstoSkipatTop(1);
  EXPECT_EQ(1, schedule2.rowstoSkipatTop());

  ScheduleFile schedule3(*externalfile);
  EXPECT_EQ(3u, model.getConcreteModelObjects<ScheduleFile>().size());
  EXPECT_EQ(3u, externalfile->scheduleFiles().size());
  EXPECT_EQ(externalfile->handle(), schedule3.externalFile().handle());
  EXPECT_TRUE(schedule3.isNumberofHoursofDataDefaulted());
  EXPECT_EQ(1, schedule3.columnNumber());
  schedule3.setColumnNumber(3);
  EXPECT_EQ(3, schedule3.columnNumber());
  EXPECT_EQ(0, schedule3.rowstoSkipatTop());
  schedule3.setRowstoSkipatTop(1);
  EXPECT_EQ(1, schedule3.rowstoSkipatTop());

  //EXPECT_TRUE(externalfile.setColumnSeparator("Tab"));
  //EXPECT_EQ("Tab", externalfile.columnSeparator().get());
  //EXPECT_EQ("Comma", externalfile.columnSeparator().get());

  // shouldn't create a new object
  boost::optional<ExternalFile> externalfile2 = ExternalFile::getExternalFile(model, openstudio::toString(p));
  ASSERT_TRUE(externalfile2);
  EXPECT_EQ(1u, model.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(externalfile->handle(), externalfile2->handle());

  // shouldn't create a new object
  boost::optional<ExternalFile> externalfile3 = ExternalFile::getExternalFile(model, "totally_not_a_file.csv");
  EXPECT_FALSE(externalfile3);
  EXPECT_EQ(1u, model.getConcreteModelObjects<ExternalFile>().size());

  // no validation of external file
  path p2 = resourcesPath() / toPath("model/OpenStudio.idd");
  boost::optional<ExternalFile> externalfile4 = ExternalFile::getExternalFile(model, openstudio::toString(p2));
  ASSERT_TRUE(externalfile4);
  EXPECT_EQ(2u, model.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_NE(externalfile->handle(), externalfile4->handle());

  schedule3.remove();
  EXPECT_EQ(2u, model.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(2u, model.getConcreteModelObjects<ScheduleFile>().size());
  EXPECT_EQ(2u, externalfile->scheduleFiles().size());

  path filePath = externalfile->filePath();
  EXPECT_TRUE(exists(p));
  EXPECT_TRUE(exists(filePath));

  externalfile->remove();
  externalfile4->remove();
  EXPECT_EQ(0u, model.getConcreteModelObjects<ExternalFile>().size());
  EXPECT_EQ(0u, model.getConcreteModelObjects<ScheduleFile>().size());
  EXPECT_EQ(0u, externalfile->scheduleFiles().size());

  EXPECT_TRUE(exists(p));
  EXPECT_FALSE(exists(filePath));

}
