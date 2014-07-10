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

#include "../Time.hpp"

#include <string>

using namespace std;
using namespace boost;
using namespace openstudio;

int numberOfSeconds(int days, int hours, int minutes, int seconds)
{
  return 24*60*60*days + 60*60*hours + 60*minutes + seconds;
}

double numberOfDays(int days, int hours, int minutes, int seconds)
{
  return days + 1.0/(24.0)*hours + 1.0/(24.0*60.0)*minutes + 1.0/(24.0*60.0*60.0)*seconds;
}

TEST(Time, FracDaysConstructor)
{
  Time testTime(0.0);
  EXPECT_EQ(testTime.totalSeconds(), 0);

  testTime = Time(0.5);
  EXPECT_EQ(testTime.totalSeconds(), 60*60*12);

  testTime = Time(1.0);
  EXPECT_EQ(testTime.totalSeconds(), 60*60*24);
}

TEST(Time, MixedConstructor)
{
  EXPECT_EQ(Time(0,1,0,0), Time(0,0,60,0));
  EXPECT_EQ(Time(0,-1,0,0), Time(0,0,-60,0)); 
  EXPECT_EQ(Time(0,1,30,0), Time(0,0,90,0));
  EXPECT_EQ(Time(0,-1,-30,0), Time(0,0,-90,0)); 
  EXPECT_EQ(Time(0,1,0,0), Time(0,1,1,-60)); 
  EXPECT_EQ(Time(0,1,0,0), Time(0,1,-1,60)); 
  EXPECT_EQ(Time(0,0,59,0), Time(0,1,-1,0)); 
  EXPECT_EQ(Time(0,1,30,0), Time(1.5/24.0)); 
  EXPECT_EQ(Time(0,-1,-30,0), Time(-1.5/24.0)); 
}

TEST(Time, StringConstructor)
{
  Time time(0,0,0,0);

  std::stringstream ss;
  ss << time;
  Time time2(ss.str());

  EXPECT_TRUE(time == time2) << ss.str() << ", " << time << " != " << time2;

  time = Time(0,4,0,0);

  ss.str("");
  ss << time;
  time2 = Time(ss.str());

  EXPECT_TRUE(time == time2) << ss.str() << ", " << time << " != " << time2;

  time = Time(4,4,0,0);

  ss.str("");
  ss << time;
  time2 = Time(ss.str());

  EXPECT_TRUE(time == time2) << ss.str() << ", " << time << " != " << time2;
}

TEST(Time, TMConstructor)
{
  time_t t = time(0);
  tm* lt = localtime(&t);
  Time zt(*lt);

  EXPECT_EQ(zt.hours(), lt->tm_hour);
  EXPECT_EQ(zt.minutes(), lt->tm_min);
  EXPECT_EQ(zt.seconds(), lt->tm_sec);
}

TEST(Time, DaysHoursMinutesSecondsConstructor)
{
  Time testTime(0,0,0,0);
  EXPECT_DOUBLE_EQ(testTime.totalDays(), 0.0/(24*60*60));
  EXPECT_DOUBLE_EQ(testTime.totalHours(), 0.0/(60*60));
  EXPECT_DOUBLE_EQ(testTime.totalMinutes(), 0.0/60);
  EXPECT_EQ(testTime.totalSeconds(), 0);
  EXPECT_EQ(testTime.days(), 0);
  EXPECT_EQ(testTime.hours(), 0);
  EXPECT_EQ(testTime.minutes(), 0);
  EXPECT_EQ(testTime.seconds(), 0);

  testTime = Time(0,0,0,1);
  EXPECT_DOUBLE_EQ(testTime.totalDays(), 1.0/(24*60*60));
  EXPECT_DOUBLE_EQ(testTime.totalHours(), 1.0/(60*60));
  EXPECT_DOUBLE_EQ(testTime.totalMinutes(), 1.0/60);
  EXPECT_EQ(testTime.totalSeconds(), 1);
  EXPECT_EQ(testTime.days(), 0);
  EXPECT_EQ(testTime.hours(), 0);
  EXPECT_EQ(testTime.minutes(), 0);
  EXPECT_EQ(testTime.seconds(), 1);

  testTime = Time(0,0,1,0);
  EXPECT_DOUBLE_EQ(testTime.totalDays(), 1.0/(24*60));
  EXPECT_DOUBLE_EQ(testTime.totalHours(), 1.0/(60));
  EXPECT_DOUBLE_EQ(testTime.totalMinutes(), 1.0);
  EXPECT_EQ(testTime.totalSeconds(), 60);
  EXPECT_EQ(testTime.days(), 0);
  EXPECT_EQ(testTime.hours(), 0);
  EXPECT_EQ(testTime.minutes(), 1);
  EXPECT_EQ(testTime.seconds(), 0);

  testTime = Time(0,1,0,0);
  EXPECT_DOUBLE_EQ(testTime.totalDays(), 1.0/24);
  EXPECT_DOUBLE_EQ(testTime.totalHours(), 1.0);
  EXPECT_DOUBLE_EQ(testTime.totalMinutes(), 60.0);
  EXPECT_EQ(testTime.totalSeconds(), 60*60);
  EXPECT_EQ(testTime.days(), 0);
  EXPECT_EQ(testTime.hours(), 1);
  EXPECT_EQ(testTime.minutes(), 0);
  EXPECT_EQ(testTime.seconds(), 0);

  testTime = Time(1,0,0,0);
  EXPECT_DOUBLE_EQ(testTime.totalDays(), 1.0);
  EXPECT_DOUBLE_EQ(testTime.totalHours(), 24.0);
  EXPECT_DOUBLE_EQ(testTime.totalMinutes(), 24.0*60.0);
  EXPECT_EQ(testTime.totalSeconds(), 24.0*60*60);
  EXPECT_EQ(testTime.days(), 1);
  EXPECT_EQ(testTime.hours(), 0);
  EXPECT_EQ(testTime.minutes(), 0);
  EXPECT_EQ(testTime.seconds(), 0);

  testTime = Time(1,1,1,1);
  EXPECT_DOUBLE_EQ(testTime.totalDays(), 1.0+1.0/24+1.0/(24*60)+1.0/(24*60*60));
  EXPECT_DOUBLE_EQ(testTime.totalHours(), 24+1.0+1.0/60+1.0/(60*60));
  EXPECT_DOUBLE_EQ(testTime.totalMinutes(), 24*60+60+1+1.0/60);
  EXPECT_EQ(testTime.totalSeconds(), 24*60*60+60*60+60+1);
  EXPECT_EQ(testTime.days(), 1);
  EXPECT_EQ(testTime.hours(), 1);
  EXPECT_EQ(testTime.minutes(), 1);
  EXPECT_EQ(testTime.seconds(), 1);
}
