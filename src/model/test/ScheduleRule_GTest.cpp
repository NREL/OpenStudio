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
