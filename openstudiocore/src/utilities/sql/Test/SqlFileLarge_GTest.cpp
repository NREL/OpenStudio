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

#include "SqlFileLargeFixture.hpp"

#include "../../time/Date.hpp"
#include "../../core/Optional.hpp"
#include "../../data/TimeSeries.hpp"
#include "../../plot/AnnotatedTimeline.hpp"
#include "../../plot/FloodPlot.hpp"
#include "../../plot/LinePlot.hpp"

#include <resources.hxx>

#include <iostream>

using namespace std;
using namespace boost;
using namespace openstudio;

TEST_F(SqlFileLargeFixture, SummaryValues)
{
  // check values
  ASSERT_TRUE(sqlFile.netSiteEnergy());
  ASSERT_TRUE(sqlFile.totalSiteEnergy());
  ASSERT_TRUE(sqlFile.netSourceEnergy());
  ASSERT_TRUE(sqlFile.totalSourceEnergy());

  EXPECT_NEAR(86730.9, *(sqlFile.netSiteEnergy()), 1);
  EXPECT_NEAR(86730.9, *(sqlFile.totalSiteEnergy()), 1);
  EXPECT_NEAR(258580, *(sqlFile.netSourceEnergy()), 1);
  EXPECT_NEAR(258580, *(sqlFile.totalSourceEnergy()), 1);
}


TEST_F(SqlFileLargeFixture, EnvPeriods)
{
  std::vector<std::string> availableEnvPeriods = sqlFile.availableEnvPeriods();
  ASSERT_TRUE(!availableEnvPeriods.empty());
  EXPECT_EQ(static_cast<unsigned>(1), availableEnvPeriods.size());
  EXPECT_EQ("Albuquerque Intl Arpt  Isis NM USA TMY3 WMO#=723650", availableEnvPeriods[0]);
}

TEST_F(SqlFileLargeFixture, TimeSeries8760)
{
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries("Albuquerque Intl Arpt  Isis NM USA TMY3 WMO#=723650", "Hourly", "Heating Coil Air Heating Rate",  "MOB FLOOR 2-4 PERIMETER 2 HEAT PUMP SUP HEAT COIL");
  ASSERT_TRUE(ts);
  EXPECT_EQ(8760, ts->values().size());
}


TEST_F(SqlFileLargeFixture, FloodPlot8760)
{
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries("Albuquerque Intl Arpt  Isis NM USA TMY3 WMO#=723650", "Hourly", "Heating Coil Air Heating Rate",  "MOB FLOOR 2-4 PERIMETER 2 HEAT PUMP SUP HEAT COIL");
  ASSERT_TRUE(ts);

  // plot it
  FloodPlot::Ptr fp = FloodPlot::create();
  fp->timeseriesData(*ts);
  fp->generateImage(toPath("SqlFileLargeFixture_FloodPlot8760.png"));
}

TEST_F(SqlFileLargeFixture, LinePlot8760)
{
  // make a timeseries
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries("Albuquerque Intl Arpt  Isis NM USA TMY3 WMO#=723650", "Hourly", "Heating Coil Air Heating Rate",  "MOB FLOOR 2-4 PERIMETER 2 HEAT PUMP SUP HEAT COIL");
  ASSERT_TRUE(ts);

  // plot it
  LinePlot::Ptr lp = LinePlot::create();
  lp->timeseriesData(*ts, "");
  lp->generateImage(toPath("SqlFileLargeFixture_LinePlot8760.png"));
}

TEST_F(SqlFileLargeFixture, TimeSeries52560)
{
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries("Albuquerque Intl Arpt  Isis NM USA TMY3 WMO#=723650", "HVAC System Timestep", "Zone Mean Air Temperature",  "FLOOR 2 LAB 1");
  ASSERT_TRUE(ts);
  EXPECT_EQ(52560, ts->values().size());
}

TEST_F(SqlFileLargeFixture, FloodPlot52560)
{
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries("Albuquerque Intl Arpt  Isis NM USA TMY3 WMO#=723650", "HVAC System Timestep", "Zone Mean Air Temperature",  "FLOOR 2 LAB 1");
  ASSERT_TRUE(ts);

  // plot it
  FloodPlot::Ptr fp = FloodPlot::create();
  fp->timeseriesData(*ts);
  fp->generateImage(toPath("SqlFileLargeFixture_FloodPlot52560.png"));
}

TEST_F(SqlFileLargeFixture, LinePlot52560)
{
  // make a timeseries
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries("Albuquerque Intl Arpt  Isis NM USA TMY3 WMO#=723650", "HVAC System Timestep", "Zone Mean Air Temperature",  "FLOOR 2 LAB 1");
  ASSERT_TRUE(ts);

  // plot it
  LinePlot::Ptr lp = LinePlot::create();
  lp->timeseriesData(*ts, "");
  lp->generateImage(toPath("SqlFileLargeFixture_LinePlot52560.png"));
}

TEST_F(SqlFileLargeFixture, TimeSeries84868)
{
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries("Albuquerque Intl Arpt  Isis NM USA TMY3 WMO#=723650", "HVAC System Timestep", "Chiller Fan Coil Part Load Ratio",  "COOLSYS1 CHILLER");
  ASSERT_TRUE(ts);
  EXPECT_EQ(84868, ts->values().size());
}

TEST_F(SqlFileLargeFixture, FloodPlot84868)
{
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries("Albuquerque Intl Arpt  Isis NM USA TMY3 WMO#=723650", "HVAC System Timestep", "Chiller Fan Coil Part Load Ratio",  "COOLSYS1 CHILLER");
  ASSERT_TRUE(ts);

  // plot it
  FloodPlot::Ptr fp = FloodPlot::create();
  fp->timeseriesData(*ts);
  fp->generateImage(toPath("SqlFileLargeFixture_FloodPlot84868.png"));
}

TEST_F(SqlFileLargeFixture, LinePlot84868)
{
  // make a timeseries
  openstudio::OptionalTimeSeries ts = sqlFile.timeSeries("Albuquerque Intl Arpt  Isis NM USA TMY3 WMO#=723650", "HVAC System Timestep", "Chiller Fan Coil Part Load Ratio",  "COOLSYS1 CHILLER");
  ASSERT_TRUE(ts);

  // plot it
  LinePlot::Ptr lp = LinePlot::create();
  lp->timeseriesData(*ts, "");
  lp->generateImage(toPath("SqlFileLargeFixture_LinePlot84868.png"));
}


TEST_F(SqlFileLargeFixture, tsfracdays84868)
{
  Vector daysFromFirstReport(84868);
  Vector values(84868);
  DateTime startDate(Date(MonthOfYear::Jan, 1), Time(0,1,0,0));
  for(unsigned i = 0; i < 84868; ++i){
    double fracHours = i * 8760.0/84868.0;
    daysFromFirstReport[i] = ((startDate + Time(0,fracHours,0,0)) - startDate).totalDays();
    values[i] = fracHours;
  }
  
  TimeSeries ts(startDate, daysFromFirstReport, values, "");
}

TEST_F(SqlFileLargeFixture, LinePlottsfracdays84868)
{
  Vector daysFromFirstReport(84868);
  Vector values(84868);
  DateTime startDate(Date(MonthOfYear::Jan, 1), Time(0,1,0,0));
  for(unsigned i = 0; i < 84868; ++i){
    double fracHours = i * 8760.0/84868.0;
    daysFromFirstReport[i] = ((startDate + Time(0,fracHours,0,0)) - startDate).totalDays();
    values[i] = fracHours;
  }
  
  TimeSeries ts(startDate, daysFromFirstReport, values, "");
  // plot it
  LinePlot::Ptr lp = LinePlot::create();
  lp->timeseriesData(ts, "");
  lp->generateImage(toPath("SqlFileLargeFixture_LinePlottsfracdays84868.png"));
}



TEST_F(SqlFileLargeFixture, ts84868)
{
  DateTimeVector dateTimes(84868);
  Vector values(84868);
  DateTime startDate(Date(MonthOfYear::Jan, 1), Time(0,1,0,0));
  for(unsigned i = 0; i < 84868; ++i){
    double fracHours = i * 8760.0/84868.0;
    dateTimes[i] = startDate + Time(0,fracHours,0,0);
    values[i] = fracHours;
  }
  
  TimeSeries ts(dateTimes, values, "");
}


TEST_F(SqlFileLargeFixture, LinePlotts84868)
{
  DateTimeVector dateTimes(84868);
  Vector values(84868);
  DateTime startDate(Date(MonthOfYear::Jan, 1), Time(0,1,0,0));
  for(unsigned i = 0; i < 84868; ++i){
    double fracHours = i * 8760.0/84868.0;
    dateTimes[i] = startDate + Time(0,fracHours,0,0);
    values[i] = fracHours;
  }
  
  TimeSeries ts(dateTimes, values, "");
  // plot it
  LinePlot::Ptr lp = LinePlot::create();
  lp->timeseriesData(ts, "");
  lp->generateImage(toPath("SqlFileLargeFixture_LinePlotts84868.png"));
}

TEST_F(SqlFileLargeFixture, Vector84868)
{
  Vector xVector(84868);
  Vector yVector(84868);
  for (unsigned i = 0; i < 84868; ++i){
    xVector(i) = i*365.0/84868.0;
    yVector(i) = 1e7+i%2500;
  }
}

TEST_F(SqlFileLargeFixture, LinePlotVector84868)
{
  Vector xVector(84868);
  Vector yVector(84868);
  for (unsigned i = 0; i < 84868; ++i){
    xVector(i) = i*365.0/84868.0;
    yVector(i) = 1e7+i%2500;
  }

  VectorLinePlotData::Ptr data = VectorLinePlotData::create(xVector, yVector);

  LinePlot::Ptr lp = LinePlot::create();
  std::string name = "Vector 84868 points";
  lp->linePlotData(data, name);
  lp->generateImage(toPath("SqlFileLargeFixture_LinePlotVector84868.png"));
}


TEST_F(SqlFileLargeFixture, BadStatement)
{
  OptionalDouble result = sqlFile.execAndReturnFirstDouble("SELECT * FROM NonExistantTable");
  EXPECT_FALSE(result);
}

