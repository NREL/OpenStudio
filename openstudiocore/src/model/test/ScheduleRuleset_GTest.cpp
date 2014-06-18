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

TEST_F(ModelFixture, ScheduleRuleset)
{
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
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.summerDesignDaySchedule().handle());
  EXPECT_EQ(schedule.defaultDaySchedule().handle(), schedule.winterDesignDaySchedule().handle());

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
  for (int i : activeRuleIndices){
    EXPECT_EQ(-1, i);
  }
  for (ScheduleDay daySchedule : daySchedules){
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
  for (int i : activeRuleIndices){
    EXPECT_EQ(0, i);
  }
  for (ScheduleDay daySchedule : daySchedules){
    EXPECT_EQ(scheduleRules[0].daySchedule().handle(), daySchedule.handle());
  }

  // add a weekend rule based on a copy
  ScheduleRule weekendRule(schedule, originalWeekend);
  EXPECT_NE(originalWeekend.handle(), weekendRule.daySchedule().handle());

  scheduleRules = schedule.scheduleRules();
  ASSERT_EQ(2u, scheduleRules.size());
  EXPECT_EQ(weekendRule.handle(), scheduleRules[0].handle()); // weekend rule is on top
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
  for (int i : activeRuleIndices){
    Date date = yd.makeDate(doy);
    if (date.dayOfWeek().value() == DayOfWeek::Saturday || date.dayOfWeek().value() == DayOfWeek::Sunday){
      EXPECT_EQ(0, i); // weekend rule
    }else{
      EXPECT_EQ(1, i); // base rule
    }
    ++doy;
  }
  doy = 1;
  for (ScheduleDay daySchedule : daySchedules){
    Date date = yd.makeDate(doy);
    if (date.dayOfWeek().value() == DayOfWeek::Saturday || date.dayOfWeek().value() == DayOfWeek::Sunday){
      EXPECT_EQ(weekendRule.daySchedule().handle(), daySchedule.handle()); // weekend rule
    }else{
      EXPECT_EQ(baseRule.daySchedule().handle(), daySchedule.handle()); // base rule
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

TEST_F(ModelFixture, ScheduleRuleset_ScheduleRule)
{
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);

  EXPECT_EQ(0,scheduleRule1.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_setScheduleRuleIndex)
{
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u,schedule.scheduleRules().size());

  EXPECT_EQ(0,scheduleRule4.ruleIndex());
  EXPECT_EQ(1,scheduleRule3.ruleIndex());
  EXPECT_EQ(2,scheduleRule2.ruleIndex());
  EXPECT_EQ(3,scheduleRule1.ruleIndex());

  // Move scheduleRule3 to the end
  schedule.setScheduleRuleIndex(scheduleRule3,3);

  EXPECT_EQ(0,scheduleRule4.ruleIndex());
  EXPECT_EQ(3,scheduleRule3.ruleIndex());
  EXPECT_EQ(1,scheduleRule2.ruleIndex());
  EXPECT_EQ(2,scheduleRule1.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_setScheduleRuleIndex2)
{
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u,schedule.scheduleRules().size());

  EXPECT_EQ(0,scheduleRule4.ruleIndex());
  EXPECT_EQ(1,scheduleRule3.ruleIndex());
  EXPECT_EQ(2,scheduleRule2.ruleIndex());
  EXPECT_EQ(3,scheduleRule1.ruleIndex());

  // Move scheduleRule1 to index 1
  schedule.setScheduleRuleIndex(scheduleRule1,1);

  EXPECT_EQ(0,scheduleRule4.ruleIndex());
  EXPECT_EQ(2,scheduleRule3.ruleIndex());
  EXPECT_EQ(3,scheduleRule2.ruleIndex());
  EXPECT_EQ(1,scheduleRule1.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_setScheduleRuleIndex3)
{
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u,schedule.scheduleRules().size());

  EXPECT_EQ(0,scheduleRule4.ruleIndex());
  EXPECT_EQ(1,scheduleRule3.ruleIndex());
  EXPECT_EQ(2,scheduleRule2.ruleIndex());
  EXPECT_EQ(3,scheduleRule1.ruleIndex());

  // Move scheduleRule3 index 2
  schedule.setScheduleRuleIndex(scheduleRule3,2);

  EXPECT_EQ(0,scheduleRule4.ruleIndex());
  EXPECT_EQ(2,scheduleRule3.ruleIndex());
  EXPECT_EQ(1,scheduleRule2.ruleIndex());
  EXPECT_EQ(3,scheduleRule1.ruleIndex());
}

TEST_F(ModelFixture, ScheduleRuleset_setScheduleRuleIndex4)
{
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u,schedule.scheduleRules().size());

  EXPECT_EQ(0,scheduleRule4.ruleIndex());
  EXPECT_EQ(1,scheduleRule3.ruleIndex());
  EXPECT_EQ(2,scheduleRule2.ruleIndex());
  EXPECT_EQ(3,scheduleRule1.ruleIndex());

  // Move scheduleRule3 to index 0
  schedule.setScheduleRuleIndex(scheduleRule3,0);

  EXPECT_EQ(1,scheduleRule4.ruleIndex());
  EXPECT_EQ(0,scheduleRule3.ruleIndex());
  EXPECT_EQ(2,scheduleRule2.ruleIndex());
  EXPECT_EQ(3,scheduleRule1.ruleIndex());
}


TEST_F(ModelFixture, ScheduleRuleset_Clone)
{
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u,schedule.scheduleRules().size());

  Model model2;

  ModelObject modelObject = schedule.clone(model2);
  ScheduleRuleset schedule2 = modelObject.cast<ScheduleRuleset>();
  ASSERT_EQ(4u,schedule2.scheduleRules().size());
  ASSERT_NO_THROW(schedule2.scheduleRules()[0].daySchedule());
  ASSERT_NO_THROW(schedule2.scheduleRules()[1].daySchedule());
  ASSERT_NO_THROW(schedule2.scheduleRules()[2].daySchedule());
  ASSERT_NO_THROW(schedule2.scheduleRules()[3].daySchedule());

  EXPECT_EQ(model.objects().size(), model2.objects().size());
}

TEST_F(ModelFixture, ScheduleRuleset_Clone2)
{
  Model model;
  ScheduleRuleset schedule(model);

  ScheduleRule scheduleRule1(schedule);
  ScheduleRule scheduleRule2(schedule);
  ScheduleRule scheduleRule3(schedule);
  ScheduleRule scheduleRule4(schedule);

  ASSERT_EQ(4u,schedule.scheduleRules().size());

  ModelObject modelObject = schedule.clone(model);
  ScheduleRuleset schedule2 = modelObject.cast<ScheduleRuleset>();

  ASSERT_EQ(4u,schedule2.scheduleRules().size());

  EXPECT_FALSE(schedule.handle() == schedule2.handle());
  EXPECT_FALSE(schedule.scheduleRules()[0].handle() == schedule2.scheduleRules()[0].handle());
  EXPECT_FALSE(schedule.scheduleRules()[1].handle() == schedule2.scheduleRules()[1].handle());
  EXPECT_FALSE(schedule.scheduleRules()[2].handle() == schedule2.scheduleRules()[2].handle());
  EXPECT_FALSE(schedule.scheduleRules()[3].handle() == schedule2.scheduleRules()[3].handle());
}

TEST_F(ModelFixture, ScheduleRuleset_Clone3)
{
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

  ASSERT_EQ(4u,schedule.scheduleRules().size());

  ModelObject modelObject = schedule.clone(model);
  ScheduleRuleset schedule2 = modelObject.cast<ScheduleRuleset>();

  ASSERT_EQ(4u,schedule2.scheduleRules().size());

  EXPECT_FALSE(schedule.handle() == schedule2.handle());
  EXPECT_FALSE(schedule.scheduleRules()[0].handle() == schedule2.scheduleRules()[0].handle());
  EXPECT_FALSE(schedule.scheduleRules()[1].handle() == schedule2.scheduleRules()[1].handle());
  EXPECT_FALSE(schedule.scheduleRules()[2].handle() == schedule2.scheduleRules()[2].handle());
  EXPECT_FALSE(schedule.scheduleRules()[3].handle() == schedule2.scheduleRules()[3].handle());

  modelObject = schedule.clone(model);
}

TEST_F(ModelFixture, ScheduleRuleset_Clone4)
{
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

  ASSERT_EQ(4u,schedule.scheduleRules().size());

  ModelObject modelObject = schedule.clone(model);
  ScheduleRuleset schedule2 = modelObject.cast<ScheduleRuleset>();

  ASSERT_EQ(4u,schedule2.scheduleRules().size());

  EXPECT_FALSE(schedule.handle() == schedule2.handle());
  EXPECT_FALSE(schedule.scheduleRules()[0].handle() == schedule2.scheduleRules()[0].handle());
  EXPECT_FALSE(schedule.scheduleRules()[1].handle() == schedule2.scheduleRules()[1].handle());
  EXPECT_FALSE(schedule.scheduleRules()[2].handle() == schedule2.scheduleRules()[2].handle());
  EXPECT_FALSE(schedule.scheduleRules()[3].handle() == schedule2.scheduleRules()[3].handle());

  modelObject = schedule.clone(model);
}

TEST_F(ModelFixture, ScheduleRuleset_DaySchedule)
{
  Model model;
  ScheduleRuleset schedule(model);
  ScheduleDay daySchedule = schedule.defaultDaySchedule();
  EXPECT_TRUE(daySchedule.setName("My Day Schedule"));
  ASSERT_TRUE(daySchedule.name());
  EXPECT_EQ("My Day Schedule", daySchedule.name().get());
}

TEST_F(ModelFixture, ScheduleRuleset_DaySchedule2)
{
  Model model;
  ScheduleRuleset schedule(model);
  ScheduleRule rule(schedule);
  ScheduleDay daySchedule = rule.daySchedule();
  EXPECT_TRUE(daySchedule.setName("My Day Schedule"));
  ASSERT_TRUE(daySchedule.name());
  EXPECT_EQ("My Day Schedule", daySchedule.name().get());
}


TEST_F(ModelFixture, ScheduleRuleset_InsertObjects)
{
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
  for (const WorkspaceObject& object : workspace1.objects()){
    // this handle should not exist in the original model
    EXPECT_FALSE(model.getObject(object.handle()));

    idfObjects1.push_back(object.idfObject());
  }

  IdfObjectVector idfObjects2;
  for (const WorkspaceObject& object : workspace2.objects()){
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
