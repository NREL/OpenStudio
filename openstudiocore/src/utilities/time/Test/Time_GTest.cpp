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
  time_t t = time(nullptr);
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
