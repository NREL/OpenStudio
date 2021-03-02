/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../ScheduleRuleset.hpp"
#include "../ScheduleRuleset_Impl.hpp"
#include "../ScheduleRule.hpp"
#include "../ScheduleRule_Impl.hpp"
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

TEST_F(ModelFixture, ScheduleRuleset) {
  Model model;

  model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
  yd.setCalendarYear(2009);
  /*
  RunPeriodControlSpecialDays newYears(openstudio::MonthOfYear::Jan, 1, model);

  RunPeriodControlSpecialDays thanksgiving(openstudio::NthDayOfWeekInMonth::fourth, openstudio::DayOfWeek::Thursday, openstudio::MonthOfYear::Nov, model);
  EXPECT_TRUE(thanksgiving.setDuration(2));
*/

  ScheduleRuleset schedule(model);
  ASSERT_NO_THROW(schedule.defaultDaySchedule());
  ASSERT_NO_THROW(schedule.summerDesignDaySchedule());
  ASSERT_NO_THROW(schedule.winterDesignDaySchedule());
  ASSERT_NO_THROW(schedule.holidaySchedule());
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.summerDesignDaySchedule().handle());
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.winterDesignDaySchedule().handle());
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.holidaySchedule().handle());

  // one default schedule created
  EXPECT_EQ(1u, model.getModelObjects<ScheduleDay>().size());

  // test schedules for copying
  ScheduleDay originalSummerDesign(model);
  ScheduleDay originalWeekend(model);
  EXPECT_EQ(3u, model.getModelObjects<ScheduleDay>().size());

  // setting summer design day makes a copy
  EXPECT_TRUE(schedule.setSummerDesignDaySchedule(originalSummerDesign));
  EXPECT_NE(originalSummerDesign.handle(), schedule.summerDesignDaySchedule().handle());
  EXPECT_EQ(4u, model.getModelObjects<ScheduleDay>().size());

  // test dates
  openstudio::Date jan1 = yd.makeDate(openstudio::MonthOfYear::Jan, 1);
  openstudio::Date dec31 = yd.makeDate(openstudio::MonthOfYear::Dec, 31);

  std::vector<ScheduleRule> scheduleRules;
  std::vector<int> activeRuleIndices;
  std::vector<ScheduleDay> daySchedules;

  // no rules yet
  scheduleRules = schedule.scheduleRules();
  activeRuleIndices = schedule.getActiveRuleIndices(jan1, dec31);
  daySchedules = schedule.getDaySchedules(jan1, dec31);
  EXPECT_EQ(0u, schedule.scheduleRules().size());
  ASSERT_EQ(365u, activeRuleIndices.size());
  ASSERT_EQ(365u, daySchedules.size());

  // all defaulted
  for (int i : activeRuleIndices) {
    EXPECT_EQ(-1, i);
  }
  for (ScheduleDay daySchedule : daySchedules) {
    EXPECT_EQ(schedule.defaultDaySchedule().handle(), daySchedule.handle());
  }

  // add a base rule
  ScheduleRule baseRule(schedule);
  baseRule.setApplySunday(true);
  baseRule.setApplyMonday(true);
  baseRule.setApplyTuesday(true);
  baseRule.setApplyWednesday(true);
  baseRule.setApplyThursday(true);
  baseRule.setApplyFriday(true);
  baseRule.setApplySaturday(true);
  scheduleRules = schedule.scheduleRules();
  activeRuleIndices = schedule.getActiveRuleIndices(jan1, dec31);
  daySchedules = schedule.getDaySchedules(jan1, dec31);

  ASSERT_EQ(1u, scheduleRules.size());
  ASSERT_EQ(365u, activeRuleIndices.size());
  ASSERT_EQ(365u, daySchedules.size());

  // all using the first rule
  for (int i : activeRuleIndices) {
    EXPECT_EQ(0, i);
  }
  for (ScheduleDay daySchedule : daySchedules) {
    EXPECT_EQ(scheduleRules[0].daySchedule().handle(), daySchedule.handle());
  }

  // add a weekend rule based on a copy
  ScheduleRule weekendRule(schedule, originalWeekend);
  EXPECT_NE(originalWeekend.handle(), weekendRule.daySchedule().handle());

  scheduleRules = schedule.scheduleRules();
  ASSERT_EQ(2u, scheduleRules.size());
  EXPECT_EQ(weekendRule.handle(), scheduleRules[0].handle());  // weekend rule is on top
  EXPECT_EQ(baseRule.handle(), scheduleRules[1].handle());

  weekendRule.setApplySunday(true);
  weekendRule.setApplyMonday(false);
  weekendRule.setApplyTuesday(false);
  weekendRule.setApplyWednesday(false);
  weekendRule.setApplyThursday(false);
  weekendRule.setApplyFriday(false);
  weekendRule.setApplySaturday(true);

  activeRuleIndices = schedule.getActiveRuleIndices(jan1, dec31);
  daySchedules = schedule.getDaySchedules(jan1, dec31);

  ASSERT_EQ(365u, activeRuleIndices.size());
  ASSERT_EQ(365u, daySchedules.size());

  unsigned doy = 1;
  for (int i : activeRuleIndices) {
    Date date = yd.makeDate(doy);
    if (date.dayOfWeek().value() == DayOfWeek::Saturday || date.dayOfWeek().value() == DayOfWeek::Sunday) {
      EXPECT_EQ(0, i);  // weekend rule
    } else {
      EXPECT_EQ(1, i);  // base rule
    }
    ++doy;
  }
  doy = 1;
  for (ScheduleDay daySchedule : daySchedules) {
    Date date = yd.makeDate(doy);
    if (date.dayOfWeek().value() == DayOfWeek::Saturday || date.dayOfWeek().value() == DayOfWeek::Sunday) {
      EXPECT_EQ(weekendRule.daySchedule().handle(), daySchedule.handle());  // weekend rule
    } else {
      EXPECT_EQ(baseRule.daySchedule().handle(), daySchedule.handle());  // base rule
    }
    ++doy;
  }

  /*

  ASSERT_EQ(2u, schedule.startDates().size());
  EXPECT_EQ(openstudio::Date(openstudio::MonthOfYear::Jan, 1), schedule.startDates()[0]);
  EXPECT_EQ(openstudio::Date(openstudio::MonthOfYear::May, 16), schedule.startDates()[1]);

  ASSERT_EQ(2u, schedule.endDates().size());
  EXPECT_EQ(openstudio::Date(openstudio::MonthOfYear::Dec, 31), schedule.endDates()[0]);
  EXPECT_EQ(openstudio::Date(openstudio::MonthOfYear::Aug, 31), schedule.endDates()[1]);

  ASSERT_EQ(2u, schedule.scheduleWeeks().size());
  EXPECT_EQ(week.handle(), schedule.scheduleWeeks()[0].handle());
  EXPECT_EQ(summerWeek.handle(), schedule.scheduleWeeks()[1].handle());

  std::vector<ScheduleDay> daySchedules = schedule.getDaySchedules(openstudio::Date(openstudio::MonthOfYear::Jan, 1), openstudio::Date(openstudio::MonthOfYear::Dec, 31));
  ASSERT_EQ(365u, daySchedules.size());

  std::vector<ScheduleWeek> weekSchedules = schedule.getActiveWeekSchedules(openstudio::Date(openstudio::MonthOfYear::Jan, 1), openstudio::Date(openstudio::MonthOfYear::Dec, 31));
  ASSERT_EQ(365u, weekSchedules.size());

  // check new years
  openstudio::Date date = yd.makeDate(openstudio::MonthOfYear::Jan, 1);
  unsigned dayOfYear = date.dayOfYear();
  EXPECT_EQ(holiday.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());

  // check monday
  date = yd.makeDate(openstudio::MonthOfYear::Jan, 5);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(weekday.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());

  // check friday
  date = yd.makeDate(openstudio::MonthOfYear::Jan, 9);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(weekday.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());

  // check saturday
  date = yd.makeDate(openstudio::MonthOfYear::Feb, 7);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(weekend.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());

  // check sunday
  date = yd.makeDate(openstudio::MonthOfYear::Feb, 8);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(weekend.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());

  // check saturday
  date = yd.makeDate(openstudio::MonthOfYear::May, 16);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(weekend.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(summerWeek.handle(), weekSchedules[dayOfYear-1].handle());

  // check sunday
  date = yd.makeDate(openstudio::MonthOfYear::May, 17);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(weekend.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(summerWeek.handle(), weekSchedules[dayOfYear-1].handle());

  // check monday
  date = yd.makeDate(openstudio::MonthOfYear::May, 18);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(summerWeekday.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(summerWeek.handle(), weekSchedules[dayOfYear-1].handle());

  // check friday
  date = yd.makeDate(openstudio::MonthOfYear::May, 22);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(summerWeekday.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(summerWeek.handle(), weekSchedules[dayOfYear-1].handle());


  // check friday
  date = yd.makeDate(openstudio::MonthOfYear::Aug, 28);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(summerWeekday.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(summerWeek.handle(), weekSchedules[dayOfYear-1].handle());

  // check saturday
  date = yd.makeDate(openstudio::MonthOfYear::Aug, 29);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(weekend.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());

  // check sunday
  date = yd.makeDate(openstudio::MonthOfYear::Aug, 30);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(weekend.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());

  // check monday
  date = yd.makeDate(openstudio::MonthOfYear::Aug, 31);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(weekday.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());


  // check wednesday
  date = yd.makeDate(openstudio::MonthOfYear::Nov, 25);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(weekday.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());

  // check thursday
  date = yd.makeDate(openstudio::MonthOfYear::Nov, 26);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(holiday.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());

  // check friday
  date = yd.makeDate(openstudio::MonthOfYear::Nov, 27);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(holiday.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());

  // check saturday
  date = yd.makeDate(openstudio::MonthOfYear::Nov, 28);
  dayOfYear = date.dayOfYear();
  EXPECT_EQ(weekend.handle(), daySchedules[dayOfYear-1].handle());
  EXPECT_EQ(week.handle(), weekSchedules[dayOfYear-1].handle());
  */
}

TEST_F(ModelFixture, ScheduleRuleset_ScheduleRule) {
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);

  EXPECT_EQ(0, scheduleRule1.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_setScheduleRuleIndex) {
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u, schedule.scheduleRules().size());

  EXPECT_EQ(0, scheduleRule4.ruleIndex());
  EXPECT_EQ(1, scheduleRule3.ruleIndex());
  EXPECT_EQ(2, scheduleRule2.ruleIndex());
  EXPECT_EQ(3, scheduleRule1.ruleIndex());

  // Move scheduleRule3 to the end
  schedule.setScheduleRuleIndex(scheduleRule3, 3);

  EXPECT_EQ(0, scheduleRule4.ruleIndex());
  EXPECT_EQ(3, scheduleRule3.ruleIndex());
  EXPECT_EQ(1, scheduleRule2.ruleIndex());
  EXPECT_EQ(2, scheduleRule1.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_setScheduleRuleIndex2) {
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u, schedule.scheduleRules().size());

  EXPECT_EQ(0, scheduleRule4.ruleIndex());
  EXPECT_EQ(1, scheduleRule3.ruleIndex());
  EXPECT_EQ(2, scheduleRule2.ruleIndex());
  EXPECT_EQ(3, scheduleRule1.ruleIndex());

  // Move scheduleRule1 to index 1
  schedule.setScheduleRuleIndex(scheduleRule1, 1);

  EXPECT_EQ(0, scheduleRule4.ruleIndex());
  EXPECT_EQ(2, scheduleRule3.ruleIndex());
  EXPECT_EQ(3, scheduleRule2.ruleIndex());
  EXPECT_EQ(1, scheduleRule1.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_setScheduleRuleIndex3) {
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u, schedule.scheduleRules().size());

  EXPECT_EQ(0, scheduleRule4.ruleIndex());
  EXPECT_EQ(1, scheduleRule3.ruleIndex());
  EXPECT_EQ(2, scheduleRule2.ruleIndex());
  EXPECT_EQ(3, scheduleRule1.ruleIndex());

  // Move scheduleRule3 index 2
  schedule.setScheduleRuleIndex(scheduleRule3, 2);

  EXPECT_EQ(0, scheduleRule4.ruleIndex());
  EXPECT_EQ(2, scheduleRule3.ruleIndex());
  EXPECT_EQ(1, scheduleRule2.ruleIndex());
  EXPECT_EQ(3, scheduleRule1.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_setScheduleRuleIndex4) {
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u, schedule.scheduleRules().size());

  EXPECT_EQ(0, scheduleRule4.ruleIndex());
  EXPECT_EQ(1, scheduleRule3.ruleIndex());
  EXPECT_EQ(2, scheduleRule2.ruleIndex());
  EXPECT_EQ(3, scheduleRule1.ruleIndex());

  // Move scheduleRule3 to index 0
  schedule.setScheduleRuleIndex(scheduleRule3, 0);

  EXPECT_EQ(1, scheduleRule4.ruleIndex());
  EXPECT_EQ(0, scheduleRule3.ruleIndex());
  EXPECT_EQ(2, scheduleRule2.ruleIndex());
  EXPECT_EQ(3, scheduleRule1.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_setScheduleRuleIndex5) {
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);

  std::vector<ScheduleRule> rules = schedule.scheduleRules();
  ASSERT_EQ(2u, rules.size());

  EXPECT_EQ(scheduleRule2.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule1.handle(), rules[1].handle());
  EXPECT_EQ(1, scheduleRule1.ruleIndex());
  EXPECT_EQ(0, scheduleRule2.ruleIndex());

  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  rules = schedule.scheduleRules();
  ASSERT_EQ(4u, rules.size());

  EXPECT_EQ(scheduleRule4.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule3.handle(), rules[1].handle());
  EXPECT_EQ(scheduleRule2.handle(), rules[2].handle());
  EXPECT_EQ(scheduleRule1.handle(), rules[3].handle());
  EXPECT_EQ(3, scheduleRule1.ruleIndex());
  EXPECT_EQ(2, scheduleRule2.ruleIndex());
  EXPECT_EQ(1, scheduleRule3.ruleIndex());
  EXPECT_EQ(0, scheduleRule4.ruleIndex());

  // Move scheduleRule3 to index 0
  schedule.setScheduleRuleIndex(scheduleRule3, 0);

  rules = schedule.scheduleRules();
  ASSERT_EQ(4u, rules.size());

  EXPECT_EQ(scheduleRule3.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule4.handle(), rules[1].handle());
  EXPECT_EQ(scheduleRule2.handle(), rules[2].handle());
  EXPECT_EQ(scheduleRule1.handle(), rules[3].handle());
  EXPECT_EQ(3, scheduleRule1.ruleIndex());
  EXPECT_EQ(2, scheduleRule2.ruleIndex());
  EXPECT_EQ(0, scheduleRule3.ruleIndex());
  EXPECT_EQ(1, scheduleRule4.ruleIndex());

  // Move scheduleRule4 to index 1
  schedule.setScheduleRuleIndex(scheduleRule4, 1);

  rules = schedule.scheduleRules();
  ASSERT_EQ(4u, rules.size());

  EXPECT_EQ(scheduleRule3.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule4.handle(), rules[1].handle());
  EXPECT_EQ(scheduleRule2.handle(), rules[2].handle());
  EXPECT_EQ(scheduleRule1.handle(), rules[3].handle());
  EXPECT_EQ(3, scheduleRule1.ruleIndex());
  EXPECT_EQ(2, scheduleRule2.ruleIndex());
  EXPECT_EQ(0, scheduleRule3.ruleIndex());
  EXPECT_EQ(1, scheduleRule4.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_setScheduleRuleIndex6) {
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  std::vector<ScheduleRule> rules = schedule.scheduleRules();
  ASSERT_EQ(2u, rules.size());

  EXPECT_EQ(scheduleRule4.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule3.handle(), rules[1].handle());
  EXPECT_EQ(1, scheduleRule3.ruleIndex());
  EXPECT_EQ(0, scheduleRule4.ruleIndex());

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);

  rules = schedule.scheduleRules();
  ASSERT_EQ(4u, rules.size());

  EXPECT_EQ(scheduleRule2.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule1.handle(), rules[1].handle());
  EXPECT_EQ(scheduleRule4.handle(), rules[2].handle());
  EXPECT_EQ(scheduleRule3.handle(), rules[3].handle());
  EXPECT_EQ(1, scheduleRule1.ruleIndex());
  EXPECT_EQ(0, scheduleRule2.ruleIndex());
  EXPECT_EQ(3, scheduleRule3.ruleIndex());
  EXPECT_EQ(2, scheduleRule4.ruleIndex());

  // Move scheduleRule3 to index 0
  schedule.setScheduleRuleIndex(scheduleRule3, 0);

  rules = schedule.scheduleRules();
  ASSERT_EQ(4u, rules.size());

  EXPECT_EQ(scheduleRule3.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule2.handle(), rules[1].handle());
  EXPECT_EQ(scheduleRule1.handle(), rules[2].handle());
  EXPECT_EQ(scheduleRule4.handle(), rules[3].handle());
  EXPECT_EQ(2, scheduleRule1.ruleIndex());
  EXPECT_EQ(1, scheduleRule2.ruleIndex());
  EXPECT_EQ(0, scheduleRule3.ruleIndex());
  EXPECT_EQ(3, scheduleRule4.ruleIndex());

  // Move scheduleRule4 to index 1
  schedule.setScheduleRuleIndex(scheduleRule4, 1);

  rules = schedule.scheduleRules();
  ASSERT_EQ(4u, rules.size());

  EXPECT_EQ(scheduleRule3.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule4.handle(), rules[1].handle());
  EXPECT_EQ(scheduleRule2.handle(), rules[2].handle());
  EXPECT_EQ(scheduleRule1.handle(), rules[3].handle());
  EXPECT_EQ(3, scheduleRule1.ruleIndex());
  EXPECT_EQ(2, scheduleRule2.ruleIndex());
  EXPECT_EQ(0, scheduleRule3.ruleIndex());
  EXPECT_EQ(1, scheduleRule4.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_setScheduleRuleIndex7) {
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  std::vector<ScheduleRule> rules = schedule.scheduleRules();
  ASSERT_EQ(2u, rules.size());

  EXPECT_EQ(scheduleRule4.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule3.handle(), rules[1].handle());
  EXPECT_EQ(1, scheduleRule3.ruleIndex());
  EXPECT_EQ(0, scheduleRule4.ruleIndex());

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);

  rules = schedule.scheduleRules();
  ASSERT_EQ(4u, rules.size());

  EXPECT_EQ(scheduleRule2.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule1.handle(), rules[1].handle());
  EXPECT_EQ(scheduleRule4.handle(), rules[2].handle());
  EXPECT_EQ(scheduleRule3.handle(), rules[3].handle());
  EXPECT_EQ(1, scheduleRule1.ruleIndex());
  EXPECT_EQ(0, scheduleRule2.ruleIndex());
  EXPECT_EQ(3, scheduleRule3.ruleIndex());
  EXPECT_EQ(2, scheduleRule4.ruleIndex());

  // Move scheduleRule4 to index 0
  schedule.setScheduleRuleIndex(scheduleRule4, 0);

  rules = schedule.scheduleRules();
  ASSERT_EQ(4u, rules.size());

  EXPECT_EQ(scheduleRule4.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule2.handle(), rules[1].handle());
  EXPECT_EQ(scheduleRule1.handle(), rules[2].handle());
  EXPECT_EQ(scheduleRule3.handle(), rules[3].handle());
  EXPECT_EQ(2, scheduleRule1.ruleIndex());
  EXPECT_EQ(1, scheduleRule2.ruleIndex());
  EXPECT_EQ(3, scheduleRule3.ruleIndex());
  EXPECT_EQ(0, scheduleRule4.ruleIndex());

  // Move scheduleRule3 to index 0
  schedule.setScheduleRuleIndex(scheduleRule3, 0);

  rules = schedule.scheduleRules();
  ASSERT_EQ(4u, rules.size());

  EXPECT_EQ(scheduleRule3.handle(), rules[0].handle());
  EXPECT_EQ(scheduleRule4.handle(), rules[1].handle());
  EXPECT_EQ(scheduleRule2.handle(), rules[2].handle());
  EXPECT_EQ(scheduleRule1.handle(), rules[3].handle());
  EXPECT_EQ(3, scheduleRule1.ruleIndex());
  EXPECT_EQ(2, scheduleRule2.ruleIndex());
  EXPECT_EQ(0, scheduleRule3.ruleIndex());
  EXPECT_EQ(1, scheduleRule4.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_Clone) {
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u, schedule.scheduleRules().size());

  Model model2;

  ModelObject modelObject = schedule.clone(model2);
  ScheduleRuleset schedule2 = modelObject.cast<ScheduleRuleset>();
  ASSERT_EQ(4u, schedule2.scheduleRules().size());
  ASSERT_NO_THROW(schedule2.scheduleRules()[0].daySchedule());
  ASSERT_NO_THROW(schedule2.scheduleRules()[1].daySchedule());
  ASSERT_NO_THROW(schedule2.scheduleRules()[2].daySchedule());
  ASSERT_NO_THROW(schedule2.scheduleRules()[3].daySchedule());

  EXPECT_EQ(model.objects().size(), model2.objects().size());
}

TEST_F(ModelFixture, ScheduleRuleset_Clone2) {
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u, schedule.scheduleRules().size());

  ModelObject modelObject = schedule.clone(model);
  ScheduleRuleset schedule2 = modelObject.cast<ScheduleRuleset>();

  ASSERT_EQ(4u, schedule2.scheduleRules().size());

  EXPECT_FALSE(schedule.handle() == schedule2.handle());
  EXPECT_FALSE(schedule.scheduleRules()[0].handle() == schedule2.scheduleRules()[0].handle());
  EXPECT_FALSE(schedule.scheduleRules()[1].handle() == schedule2.scheduleRules()[1].handle());
  EXPECT_FALSE(schedule.scheduleRules()[2].handle() == schedule2.scheduleRules()[2].handle());
  EXPECT_FALSE(schedule.scheduleRules()[3].handle() == schedule2.scheduleRules()[3].handle());
}

TEST_F(ModelFixture, ScheduleRuleset_Clone3) {
  Model model;
  ScheduleTypeLimits typeLimits(model);

  ScheduleRuleset schedule(model);
  schedule.setScheduleTypeLimits(typeLimits);

  ScheduleRule scheduleRule1(schedule);
  ASSERT_TRUE(scheduleRule1.daySchedule().scheduleTypeLimits());
  EXPECT_EQ(typeLimits.handle(), scheduleRule1.daySchedule().scheduleTypeLimits()->handle());
  ScheduleRule scheduleRule2(schedule);
  ASSERT_TRUE(scheduleRule2.daySchedule().scheduleTypeLimits());
  EXPECT_EQ(typeLimits.handle(), scheduleRule2.daySchedule().scheduleTypeLimits()->handle());
  ScheduleRule scheduleRule3(schedule);
  ASSERT_TRUE(scheduleRule3.daySchedule().scheduleTypeLimits());
  EXPECT_EQ(typeLimits.handle(), scheduleRule3.daySchedule().scheduleTypeLimits()->handle());
  ScheduleRule scheduleRule4(schedule);
  ASSERT_TRUE(scheduleRule4.daySchedule().scheduleTypeLimits());
  EXPECT_EQ(typeLimits.handle(), scheduleRule4.daySchedule().scheduleTypeLimits()->handle());

  ASSERT_EQ(4u, schedule.scheduleRules().size());

  ModelObject modelObject = schedule.clone(model);
  ScheduleRuleset schedule2 = modelObject.cast<ScheduleRuleset>();

  ASSERT_EQ(4u, schedule2.scheduleRules().size());

  EXPECT_FALSE(schedule.handle() == schedule2.handle());
  EXPECT_FALSE(schedule.scheduleRules()[0].handle() == schedule2.scheduleRules()[0].handle());
  EXPECT_FALSE(schedule.scheduleRules()[1].handle() == schedule2.scheduleRules()[1].handle());
  EXPECT_FALSE(schedule.scheduleRules()[2].handle() == schedule2.scheduleRules()[2].handle());
  EXPECT_FALSE(schedule.scheduleRules()[3].handle() == schedule2.scheduleRules()[3].handle());

  modelObject = schedule.clone(model);
}

TEST_F(ModelFixture, ScheduleRuleset_Clone4) {
  Model model;
  ScheduleTypeLimits typeLimits(model);
  typeLimits.setName(toString(createUUID()));

  ScheduleRuleset schedule(model);
  schedule.setName(toString(createUUID()));
  schedule.setScheduleTypeLimits(typeLimits);

  ScheduleRule scheduleRule1(schedule);
  scheduleRule1.setName(toString(createUUID()));
  ASSERT_TRUE(scheduleRule1.daySchedule().scheduleTypeLimits());
  EXPECT_EQ(typeLimits.handle(), scheduleRule1.daySchedule().scheduleTypeLimits()->handle());

  ScheduleRule scheduleRule2(schedule);
  scheduleRule2.setName(toString(createUUID()));
  ASSERT_TRUE(scheduleRule2.daySchedule().scheduleTypeLimits());
  EXPECT_EQ(typeLimits.handle(), scheduleRule2.daySchedule().scheduleTypeLimits()->handle());

  ScheduleRule scheduleRule3(schedule);
  scheduleRule3.setName(toString(createUUID()));
  ASSERT_TRUE(scheduleRule3.daySchedule().scheduleTypeLimits());
  EXPECT_EQ(typeLimits.handle(), scheduleRule3.daySchedule().scheduleTypeLimits()->handle());

  ScheduleRule scheduleRule4(schedule);
  scheduleRule4.setName(toString(createUUID()));
  ASSERT_TRUE(scheduleRule4.daySchedule().scheduleTypeLimits());
  EXPECT_EQ(typeLimits.handle(), scheduleRule4.daySchedule().scheduleTypeLimits()->handle());

  ASSERT_EQ(4u, schedule.scheduleRules().size());

  ModelObject modelObject = schedule.clone(model);
  ScheduleRuleset schedule2 = modelObject.cast<ScheduleRuleset>();

  ASSERT_EQ(4u, schedule2.scheduleRules().size());

  EXPECT_FALSE(schedule.handle() == schedule2.handle());
  EXPECT_FALSE(schedule.scheduleRules()[0].handle() == schedule2.scheduleRules()[0].handle());
  EXPECT_FALSE(schedule.scheduleRules()[1].handle() == schedule2.scheduleRules()[1].handle());
  EXPECT_FALSE(schedule.scheduleRules()[2].handle() == schedule2.scheduleRules()[2].handle());
  EXPECT_FALSE(schedule.scheduleRules()[3].handle() == schedule2.scheduleRules()[3].handle());

  modelObject = schedule.clone(model);
}

TEST_F(ModelFixture, ScheduleRuleset_DaySchedule) {
  Model model;
  ScheduleRuleset schedule(model);
  ScheduleDay daySchedule = schedule.defaultDaySchedule();
  EXPECT_TRUE(daySchedule.setName("My Day Schedule"));
  ASSERT_TRUE(daySchedule.name());
  EXPECT_EQ("My Day Schedule", daySchedule.name().get());
}

TEST_F(ModelFixture, ScheduleRuleset_DaySchedule2) {
  Model model;
  ScheduleRuleset schedule(model);
  ScheduleRule rule(schedule);
  ScheduleDay daySchedule = rule.daySchedule();
  EXPECT_TRUE(daySchedule.setName("My Day Schedule"));
  ASSERT_TRUE(daySchedule.name());
  EXPECT_EQ("My Day Schedule", daySchedule.name().get());
}

TEST_F(ModelFixture, ScheduleRuleset_InsertObjects) {
  Model model;
  ScheduleTypeLimits typeLimits(model);

  ScheduleRuleset schedule(model);
  schedule.setName("Always_On");
  EXPECT_EQ("Always_On", schedule.name().get());
  schedule.setScheduleTypeLimits(typeLimits);

  ScheduleRule rule(schedule);
  rule.setName("Always_On Rule 1");
  EXPECT_EQ("Always_On Rule 1", rule.name().get());

  ScheduleDay daySchedule = rule.daySchedule();
  daySchedule.setName("Always_On Rule 1 Day Schedule");
  EXPECT_EQ("Always_On Rule 1 Day Schedule", daySchedule.name().get());
  daySchedule.setScheduleTypeLimits(typeLimits);

  // clone does not preserve handles
  Workspace workspace1 = model.clone();
  Workspace workspace2 = model.clone();

  IdfObjectVector idfObjects1;
  for (const WorkspaceObject& object : workspace1.objects()) {
    // this handle should not exist in the original model
    EXPECT_FALSE(model.getObject(object.handle()));

    idfObjects1.push_back(object.idfObject());
  }

  IdfObjectVector idfObjects2;
  for (const WorkspaceObject& object : workspace2.objects()) {
    // this handle should not exist in the original model
    EXPECT_FALSE(model.getObject(object.handle()));

    idfObjects2.push_back(object.idfObject());
  }

  std::vector<WorkspaceObject> insertedObjects = model.insertObjects(idfObjects1);

  // DLM: I do not know what the correct operation is here, should the number of objects in
  // the model stay constant?

  // the insert should not fail
  EXPECT_FALSE(insertedObjects.empty());

  std::vector<WorkspaceObject> addedObjects = model.addObjects(idfObjects2);

  // the add should not fail
  EXPECT_FALSE(addedObjects.empty());
}

TEST_F(ModelFixture, ScheduleRuleset_SpecialDays) {
  Model model;
  ScheduleTypeLimits typeLimits(model);

  ScheduleRuleset schedule(model);
  schedule.setName("Always_On");
  EXPECT_EQ("Always_On", schedule.name().get());
  schedule.setScheduleTypeLimits(typeLimits);

  EXPECT_EQ(1u, model.getConcreteModelObjects<ScheduleDay>().size());

  // For each of these, the setter clones the original schedule
  ScheduleDay winterSchedule(model);
  schedule.setWinterDesignDaySchedule(winterSchedule);
  EXPECT_NE(winterSchedule.handle(), schedule.winterDesignDaySchedule().handle());

  ScheduleDay summerSchedule(model);
  schedule.setSummerDesignDaySchedule(summerSchedule);
  EXPECT_NE(summerSchedule.handle(), schedule.summerDesignDaySchedule().handle());

  ScheduleDay holidaySchedule(model);
  schedule.setHolidaySchedule(holidaySchedule);
  EXPECT_NE(holidaySchedule.handle(), schedule.holidaySchedule().handle());

  // DefaultDay, the 3 schedules we created, and the 3 clones made by setters = 7
  EXPECT_EQ(7u, model.getConcreteModelObjects<ScheduleDay>().size());

  schedule.remove();

  EXPECT_EQ(3u, model.getConcreteModelObjects<ScheduleDay>().size());
  EXPECT_FALSE(winterSchedule.handle().isNull());
  EXPECT_FALSE(summerSchedule.handle().isNull());
}

TEST_F(ModelFixture, ScheduleRuleset_resetSpecialDays) {
  Model model;
  ScheduleTypeLimits typeLimits(model);

  ScheduleRuleset schedule(model);
  schedule.setName("Always_On");
  EXPECT_EQ("Always_On", schedule.name().get());
  schedule.setScheduleTypeLimits(typeLimits);

  EXPECT_EQ(1u, model.getConcreteModelObjects<ScheduleDay>().size());
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.winterDesignDaySchedule().handle());
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.summerDesignDaySchedule().handle());
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.holidaySchedule().handle());

  // For each of these, the setter clones the original schedule
  ScheduleDay winterSchedule(model);
  ScheduleDay summerSchedule(model);
  ScheduleDay holidaySchedule(model);
  EXPECT_EQ(4u, model.getConcreteModelObjects<ScheduleDay>().size());

  schedule.setWinterDesignDaySchedule(winterSchedule);
  EXPECT_EQ(5u, model.getConcreteModelObjects<ScheduleDay>().size());

  schedule.setSummerDesignDaySchedule(summerSchedule);
  EXPECT_EQ(6u, model.getConcreteModelObjects<ScheduleDay>().size());

  schedule.setHolidaySchedule(holidaySchedule);
  // DefaultDay, the 3 schedules we created, and the 3 clones made by setters = 7
  EXPECT_EQ(7u, model.getConcreteModelObjects<ScheduleDay>().size());

  EXPECT_NE(winterSchedule.handle(), schedule.winterDesignDaySchedule().handle());
  EXPECT_NE(summerSchedule.handle(), schedule.summerDesignDaySchedule().handle());
  EXPECT_NE(holidaySchedule.handle(), schedule.holidaySchedule().handle());
  EXPECT_NE(schedule.defaultDaySchedule().handle(), schedule.winterDesignDaySchedule().handle());
  EXPECT_NE(schedule.defaultDaySchedule().handle(), schedule.summerDesignDaySchedule().handle());
  EXPECT_NE(schedule.defaultDaySchedule().handle(), schedule.holidaySchedule().handle());

  // reset should delete the special clone it made when setting
  schedule.resetWinterDesignDaySchedule();
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.winterDesignDaySchedule().handle());
  EXPECT_NE(schedule.defaultDaySchedule().handle(), schedule.summerDesignDaySchedule().handle());
  EXPECT_NE(schedule.defaultDaySchedule().handle(), schedule.holidaySchedule().handle());
  EXPECT_EQ(6u, model.getConcreteModelObjects<ScheduleDay>().size());

  schedule.resetSummerDesignDaySchedule();
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.winterDesignDaySchedule().handle());
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.summerDesignDaySchedule().handle());
  EXPECT_NE(schedule.defaultDaySchedule().handle(), schedule.holidaySchedule().handle());
  EXPECT_EQ(5u, model.getConcreteModelObjects<ScheduleDay>().size());

  schedule.resetHolidaySchedule();
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.winterDesignDaySchedule().handle());
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.summerDesignDaySchedule().handle());
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.holidaySchedule().handle());
  EXPECT_EQ(4u, model.getConcreteModelObjects<ScheduleDay>().size());
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
