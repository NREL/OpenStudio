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

#include "../LinePlot.hpp"

#include "../../data/Matrix.hpp"
#include "../../core/Application.hpp"

using namespace std;
using namespace boost;
using namespace openstudio;

TEST(LinePlot,RepeatedConstructor)
{
  LinePlot::Ptr lp;
  lp = LinePlot::create();
  lp = LinePlot::create();
  lp = LinePlot::create();
  lp = LinePlot::create();
}

TEST(LinePlot, VectorLinePlotData)
{
  Vector xVector(11);
  Vector yVector(11);
  for (unsigned i = 0; i < 11; ++i){
    xVector(i) = i;
    yVector(i) = 10 - i;
  }

  VectorLinePlotData::Ptr data = VectorLinePlotData::create(xVector, yVector);

  LinePlot::Ptr lp = LinePlot::create();
  std::string name = "Vector Test";
  lp->linePlotData(data, name, Qt::blue, 0.0);
  lp->axesFontSize(12);
  lp->tickFontSize(10);
  lp->bottomAxisTitle("X axis");
  lp->leftAxisTitle("Y axis");
  lp->generateImage(toPath("testVectorLinePlotData.png"));

};

TEST(LinePlot, TimeSeriesLinePlot_DetailedYear)
{
  DateTimeVector dateTimes(8760);
  Vector values(8760);
  DateTime startDate(Date(MonthOfYear::Jan, 1), Time(0,1,0,0));
  for(unsigned i = 0; i < 8760; ++i){
    dateTimes[i] = startDate + Time(0,i,0,0);
    values[i] = i % 24;
  }
  
  TimeSeries ts(dateTimes, values, "");
  TimeSeriesLinePlotData::Ptr data = TimeSeriesLinePlotData::create(ts);
  
  EXPECT_DOUBLE_EQ(1.0+1.0/24.0, data->minX());
  EXPECT_DOUBLE_EQ(365.0-1.0/24.0+1+1.0/24.0, data->maxX());
  EXPECT_DOUBLE_EQ(0.0, data->minY());
  EXPECT_DOUBLE_EQ(23.0, data->maxY());
  EXPECT_DOUBLE_EQ(0.0, data->minValue());
  EXPECT_DOUBLE_EQ(23.0, data->maxValue());
  ASSERT_EQ(static_cast<unsigned>(8760), data->size());
  EXPECT_DOUBLE_EQ(1.0+1.0/24.0, data->x(0));
  EXPECT_DOUBLE_EQ(366, data->x(8759));
  EXPECT_DOUBLE_EQ(0.0, data->y(0));
  EXPECT_DOUBLE_EQ(23.0, data->y(8759));

 /* update 1/22/11 - remove value wrapper from line plot data 
  double epsilon=1e-6;
  EXPECT_DOUBLE_EQ(0.0, data->value(0.0/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(0.5/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(1.0/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(1.0+0.5/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(1.0+1.0/24.0-epsilon));
  EXPECT_DOUBLE_EQ(0.0, data->value(2.0-23.0/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(25.0/24.0-epsilon));
  EXPECT_DOUBLE_EQ(0.0, data->value(24.0/24.0));
  EXPECT_DOUBLE_EQ(1.0, data->value(1+2.0/24.0));
  EXPECT_DOUBLE_EQ(22.0, data->value(365.0-1.0/24.0));
  EXPECT_DOUBLE_EQ(22.0, data->value(364.0+23.0/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(365.0-23.0/24.0-epsilon)); 
  EXPECT_DOUBLE_EQ(0.0, data->value(364.0+1.0/24.0-epsilon));
*/

  LinePlot::Ptr lp = LinePlot::create();
  std::string name = "TimeSeries Detailed Line Plot Year Test";
  lp->linePlotData(data, name, Qt::blue, 0.0);
  lp->axesFontSize(12);
  lp->tickFontSize(10);
  lp->bottomAxisTitle("Time");
  lp->leftAxisTitle("Y axis");
  lp->setLineThickness(1);
  lp->generateImage(toPath("testTimeSeriesLinePlot_DetailedYear.png"));

}


TEST(LinePlot, TimeSeriesLinePlot_DetailedDay)
{
  DateTimeVector dateTimes(24);
  Vector values(24);
  DateTime startDate(Date(MonthOfYear::Jan, 1), Time(0,1,0,0));
  for(unsigned i = 0; i < 24; ++i){
    dateTimes[i] = startDate + Time(0,i,0,0);
    values[i] = i % 24;
  }
  
  TimeSeries ts(dateTimes, values, "");
  TimeSeriesLinePlotData::Ptr data = TimeSeriesLinePlotData::create(ts);
  
  EXPECT_DOUBLE_EQ(1.0+1.0/24.0, data->minX());
  EXPECT_DOUBLE_EQ(2, data->maxX());
  EXPECT_DOUBLE_EQ(0.0, data->minY());
  EXPECT_DOUBLE_EQ(23.0, data->maxY());
  EXPECT_DOUBLE_EQ(0.0, data->minValue());
  EXPECT_DOUBLE_EQ(23.0, data->maxValue());
  ASSERT_EQ(static_cast<unsigned>(24), data->size());
  EXPECT_DOUBLE_EQ(1.0+1.0/24.0, data->x(0));
  EXPECT_DOUBLE_EQ(2, data->x(23));
  EXPECT_DOUBLE_EQ(0.0, data->y(0));
  EXPECT_DOUBLE_EQ(23.0, data->y(23));

 /* update 1/22/11 - remove value wrapper from line plot data 
  double epsilon=1e-6;
  EXPECT_DOUBLE_EQ(0.0, data->value(0.0/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(0.5/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(1.0/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(1.0+1.0/24.0-epsilon));
  EXPECT_DOUBLE_EQ(0.0, data->value(2.0-23.0/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(25.0/24.0-epsilon));
  EXPECT_DOUBLE_EQ(0.0, data->value(24.0/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(1.0+0.5/24.0));
  EXPECT_DOUBLE_EQ(1.0, data->value(1+2.0/24.0));
  EXPECT_DOUBLE_EQ(2.0, data->value(1+3.0/24.0));
  EXPECT_DOUBLE_EQ(3.0, data->value(1+4.0/24.0));
  EXPECT_DOUBLE_EQ(4.0, data->value(1+5.0/24.0));
  EXPECT_DOUBLE_EQ(5.0, data->value(1+6.0/24.0));
  EXPECT_DOUBLE_EQ(6.0, data->value(1+7.0/24.0));
  EXPECT_DOUBLE_EQ(7.0, data->value(1+8.0/24.0));
  EXPECT_DOUBLE_EQ(8.0, data->value(1+9.0/24.0));
  EXPECT_DOUBLE_EQ(9.0, data->value(1+10.0/24.0));
  EXPECT_DOUBLE_EQ(11.0, data->value(1+12.0/24.0));
  EXPECT_DOUBLE_EQ(12.0, data->value(1+13.0/24.0));
  EXPECT_DOUBLE_EQ(13.0, data->value(1+14.0/24.0-epsilon));
  EXPECT_DOUBLE_EQ(14.0, data->value(1+15.0/24.0));
  EXPECT_DOUBLE_EQ(15.0, data->value(1+16.0/24.0));
  EXPECT_DOUBLE_EQ(16.0, data->value(1+17.0/24.0-epsilon));
  EXPECT_DOUBLE_EQ(17.0, data->value(1+18.0/24.0));
  EXPECT_DOUBLE_EQ(18.0, data->value(1+19.0/24.0));
  EXPECT_DOUBLE_EQ(0.0, data->value(364.0+1.0/24.0-epsilon));
*/

  LinePlot::Ptr lp = LinePlot::create();
  std::string name = "TimeSeries Detailed Line Plot Day Test";
  lp->linePlotData(data, name, Qt::blue, 0.0);
  lp->axesFontSize(12);
  lp->tickFontSize(10);
  lp->bottomAxisTitle("Time");
  lp->leftAxisTitle("Y axis");
  lp->setLineThickness(1);
  lp->generateImage(toPath("testTimeSeriesLinePlot_DetailedDay.png"));

}

TEST(LinePlot, TimeSeriesLinePlot_DetailedEndOfMonth)
{
  // testing end of month values - from Small Office Electricity:Facility Monthly
  DateTimeVector dateTimes(12);
  Vector values(12);


  dateTimes[0] = DateTime(Date(MonthOfYear::Jan, 31), Time(0,24,0,0));
  values[0] = 10;
  dateTimes[1] = DateTime(Date(MonthOfYear::Feb, 28), Time(0,24,0,0));
  values[1] = 11;
  dateTimes[2] = DateTime(Date(MonthOfYear::Mar, 31), Time(0,24,0,0));
  values[2] = 12;
  dateTimes[3] = DateTime(Date(MonthOfYear::Apr, 30), Time(0,24,0,0));
  values[3] = 13;
  dateTimes[4] = DateTime(Date(MonthOfYear::May, 31), Time(0,24,0,0));
  values[4] = 14;
  dateTimes[5] = DateTime(Date(MonthOfYear::Jun, 30), Time(0,24,0,0));
  values[5] = 15;
  dateTimes[6] = DateTime(Date(MonthOfYear::Jul, 31), Time(0,24,0,0));
  values[6] = 16;
  dateTimes[7] = DateTime(Date(MonthOfYear::Aug, 31), Time(0,24,0,0));
  values[7] = 17;
  dateTimes[8] = DateTime(Date(MonthOfYear::Sep, 30), Time(0,24,0,0));
  values[8] = 18;
  dateTimes[9] = DateTime(Date(MonthOfYear::Oct, 31), Time(0,24,0,0));
  values[9] = 19;
  dateTimes[10] = DateTime(Date(MonthOfYear::Nov, 30), Time(0,24,0,0));
  values[10] = 20;
  dateTimes[11] = DateTime(Date(MonthOfYear::Dec, 31), Time(0,24,0,0));
  values[11] = 21;
  
  TimeSeries ts(dateTimes, values, "");
  TimeSeriesLinePlotData::Ptr data = TimeSeriesLinePlotData::create(ts);
  
  EXPECT_DOUBLE_EQ(31.0+24.0/24.0, data->minX());
  EXPECT_DOUBLE_EQ(365+24.0/24.0, data->maxX());
  EXPECT_DOUBLE_EQ(10.0, data->minY());
  EXPECT_DOUBLE_EQ(21.0, data->maxY());
  EXPECT_DOUBLE_EQ(10.0, data->minValue());
  EXPECT_DOUBLE_EQ(21.0, data->maxValue());
  ASSERT_EQ(static_cast<unsigned>(12), data->size());
  EXPECT_DOUBLE_EQ(31.0+24.0/24.0, data->x(0));
  EXPECT_DOUBLE_EQ(366, data->x(11));
  EXPECT_DOUBLE_EQ(10.0, data->y(0));
  EXPECT_DOUBLE_EQ(21.0, data->y(11));

  LinePlot::Ptr lp = LinePlot::create();
  std::string name = "TimeSeries Detailed Line Plot End of Month Test";
  lp->topAxisTitle("Test"); // coverage
  lp->linePlotData(data, name, Qt::blue, 0.0);
  lp->axesFontSize(12);
  lp->tickFontSize(10);
  lp->bottomAxisTitle("Time");
  lp->leftAxisTitle("Y axis");
  lp->setLineThickness(1);
  lp->generateImage(toPath("testTimeSeriesLinePlot_DetailedEndOfMonth.png"));

}
