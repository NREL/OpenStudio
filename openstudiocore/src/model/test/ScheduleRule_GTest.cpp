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

#include "ModelFixture.hpp"
#include "../ScheduleRule.hpp"
#include "../ScheduleRule_Impl.hpp"
#include "../ScheduleRuleset.hpp"
#include "../ScheduleRuleset_Impl.hpp"
#include "../ScheduleDay.hpp"
#include "../ScheduleDay_Impl.hpp"
#include "../YearDescription.hpp"
#include "../YearDescription_Impl.hpp"
#include "../RunPeriodControlSpecialDays.hpp"
#include "../RunPeriodControlSpecialDays_Impl.hpp"
#include "../ScheduleTypeLimits.hpp"
#include "../ScheduleTypeLimits_Impl.hpp"

#include "../../utilities/core/UUID.hpp"
#include "../../utilities/time/Date.hpp"
#include "../../utilities/time/Time.hpp"

using namespace openstudio::model;
using namespace openstudio;

TEST_F(ModelFixture, ScheduleRule)
{
  Model model;

  model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
  yd.setCalendarYear(2009);

  ScheduleRuleset ruleset(model);
  ScheduleRule rule(ruleset);

  ASSERT_EQ(1u, ruleset.scheduleRules().size());
  EXPECT_EQ(rule.handle(), ruleset.scheduleRules()[0].handle());

  // test defaults
  EXPECT_FALSE(rule.applySunday());
  EXPECT_FALSE(rule.applyMonday());
  EXPECT_FALSE(rule.applyTuesday());
  EXPECT_FALSE(rule.applyWednesday());
  EXPECT_FALSE(rule.applyThursday());
  EXPECT_FALSE(rule.applyFriday());
  EXPECT_FALSE(rule.applySaturday());

  EXPECT_EQ("DateRange", rule.dateSpecificationType());
  ASSERT_TRUE(rule.startDate());
  EXPECT_EQ(MonthOfYear::Jan, rule.startDate()->monthOfYear().value());
  EXPECT_EQ(1u, rule.startDate()->dayOfMonth());
  ASSERT_TRUE(rule.endDate());
  EXPECT_EQ(MonthOfYear::Dec, rule.endDate()->monthOfYear().value());
  EXPECT_EQ(31u, rule.endDate()->dayOfMonth());
  EXPECT_EQ(0u, rule.specificDates().size());

  // change to specific dates
  rule.addSpecificDate(yd.makeDate(MonthOfYear::Dec, 25));
  EXPECT_EQ("SpecificDates", rule.dateSpecificationType());
  EXPECT_FALSE(rule.startDate());
  EXPECT_FALSE(rule.endDate());
  ASSERT_EQ(1u, rule.specificDates().size());
  EXPECT_EQ(MonthOfYear::Dec, rule.specificDates()[0].monthOfYear().value());
  EXPECT_EQ(25u, rule.specificDates()[0].dayOfMonth());

  // change back to date range, end date automatically set
  rule.setStartDate(yd.makeDate(MonthOfYear::Dec, 1));
  EXPECT_EQ("DateRange", rule.dateSpecificationType());
  ASSERT_TRUE(rule.startDate());
  EXPECT_EQ(MonthOfYear::Dec, rule.startDate()->monthOfYear().value());
  EXPECT_EQ(1u, rule.startDate()->dayOfMonth());
  ASSERT_TRUE(rule.endDate());
  EXPECT_EQ(MonthOfYear::Dec, rule.endDate()->monthOfYear().value());
  EXPECT_EQ(31u, rule.endDate()->dayOfMonth());
  EXPECT_EQ(0u, rule.specificDates().size());

  // check weekends
  rule.setApplySunday(true);
  rule.setApplySaturday(true);
  
  EXPECT_TRUE(rule.applySunday());
  EXPECT_FALSE(rule.applyMonday());
  EXPECT_FALSE(rule.applyTuesday());
  EXPECT_FALSE(rule.applyWednesday());
  EXPECT_FALSE(rule.applyThursday());
  EXPECT_FALSE(rule.applyFriday());
  EXPECT_TRUE(rule.applySaturday());

  // check weekdays and uncheck weekends
  rule.setApplySunday(false);
  rule.setApplyMonday(true);
  rule.setApplyTuesday(true);
  rule.setApplyWednesday(true);
  rule.setApplyThursday(true);
  rule.setApplyFriday(true);
  rule.setApplySaturday(false);

  EXPECT_FALSE(rule.applySunday());
  EXPECT_TRUE(rule.applyMonday());
  EXPECT_TRUE(rule.applyTuesday());
  EXPECT_TRUE(rule.applyWednesday());
  EXPECT_TRUE(rule.applyThursday());
  EXPECT_TRUE(rule.applyFriday());
  EXPECT_FALSE(rule.applySaturday());

  // out of range
  EXPECT_FALSE(rule.containsDate(yd.makeDate(MonthOfYear::Jan, 1)));
  EXPECT_FALSE(rule.containsDate(yd.makeDate(MonthOfYear::Nov, 30)));

  // weekdays in december
  EXPECT_TRUE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 1)));
  EXPECT_TRUE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 2)));
  EXPECT_TRUE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 3)));
  EXPECT_TRUE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 4)));
  EXPECT_TRUE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 7)));
  EXPECT_TRUE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 8)));
  EXPECT_TRUE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 30)));
  EXPECT_TRUE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 31)));

  // weekends in december
  EXPECT_FALSE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 5)));
  EXPECT_FALSE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 6)));
  EXPECT_FALSE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 26)));
  EXPECT_FALSE(rule.containsDate(yd.makeDate(MonthOfYear::Dec, 27)));
}

/*
January

Su Mo Tu We Th Fr Sa 
             1  2  3 
 4  5  6  7  8  9 10 
11 12 13 14 15 16 17 
18 19 20 21 22 23 24 
25 26 27 28 29 30 31 

February

Su Mo Tu We Th Fr Sa 
 1  2  3  4  5  6  7 
 8  9 10 11 12 13 14 
15 16 17 18 19 20 21 
22 23 24 25 26 27 28 

March

Su Mo Tu We Th Fr Sa 
 1  2  3  4  5  6  7 
 8  9 10 11 12 13 14 
15 16 17 18 19 20 21 
22 23 24 25 26 27 28 
29 30 31 

April

Su Mo Tu We Th Fr Sa 
          1  2  3  4 
 5  6  7  8  9 10 11 
12 13 14 15 16 17 18 
19 20 21 22 23 24 25 
26 27 28 29 30 

May

Su Mo Tu We Th Fr Sa 
                1  2 
 3  4  5  6  7  8  9 
10 11 12 13 14 15 16 
17 18 19 20 21 22 23 
24 25 26 27 28 29 30 
31 

June

Su Mo Tu We Th Fr Sa 
    1  2  3  4  5  6 
 7  8  9 10 11 12 13 
14 15 16 17 18 19 20 
21 22 23 24 25 26 27 
28 29 30 

July

Su Mo Tu We Th Fr Sa 
          1  2  3  4 
 5  6  7  8  9 10 11 
12 13 14 15 16 17 18 
19 20 21 22 23 24 25 
26 27 28 29 30 31 

August

Su Mo Tu We Th Fr Sa 
                   1 
 2  3  4  5  6  7  8 
 9 10 11 12 13 14 15 
16 17 18 19 20 21 22 
23 24 25 26 27 28 29 
30 31 

September

Su Mo Tu We Th Fr Sa 
       1  2  3  4  5 
 6  7  8  9 10 11 12 
13 14 15 16 17 18 19 
20 21 22 23 24 25 26 
27 28 29 30 

October

Su Mo Tu We Th Fr Sa 
             1  2  3 
 4  5  6  7  8  9 10 
11 12 13 14 15 16 17 
18 19 20 21 22 23 24 
25 26 27 28 29 30 31 

November

Su Mo Tu We Th Fr Sa 
 1  2  3  4  5  6  7 
 8  9 10 11 12 13 14 
15 16 17 18 19 20 21 
22 23 24 25 26 27 28 
29 30 

December

Su Mo Tu We Th Fr Sa 
       1  2  3  4  5 
 6  7  8  9 10 11 12 
13 14 15 16 17 18 19 
20 21 22 23 24 25 26 
27 28 29 30 31 

Holidays and Observances:
Jan 1   New Year's Day
Jan 19  Martin Luther King Day
Feb 16  Presidents' Day
May 25  Memorial Day

Jul 3   'Independence Day' observed
Jul 4   Independence Day
Sep 7   Labor Day
Oct 12  Columbus Day (Most regions)

Nov 11  Veterans Day
Nov 26  Thanksgiving Day
Dec 25  Christmas Day
*/
