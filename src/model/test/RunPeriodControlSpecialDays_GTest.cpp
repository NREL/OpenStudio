/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../YearDescription.hpp"
#include "../YearDescription_Impl.hpp"
#include "../RunPeriodControlSpecialDays.hpp"
#include "../RunPeriodControlSpecialDays_Impl.hpp"

#include "../../utilities/time/Date.hpp"

// Unusual, but here I'm interested in finding out the actual string it stored for the Start Date
#include <utilities/idd/OS_RunPeriodControl_SpecialDays_FieldEnums.hxx>

#include <iostream>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RunPeriodControlSpecialDays) {
  Model model;
  model::YearDescription yd = model.getUniqueModelObject<model::YearDescription>();
  yd.setCalendarYear(2008);

  /* February 2008
  Su Mo Tu We Th Fr Sa
                  1  2
   3  4  5  6  7  8  9
  10 11 12 13 14 15 16
  17 18 19 20 21 22 23
  24 25 26 27 28 29
  */

  RunPeriodControlSpecialDays day(MonthOfYear::Feb, 14, model);
  ASSERT_NO_THROW(day.startDate());
  EXPECT_EQ(14u, day.startDate().dayOfMonth());
  EXPECT_EQ(MonthOfYear::Feb, day.startDate().monthOfYear().value());
  EXPECT_EQ(DayOfWeek::Thursday, day.startDate().dayOfWeek().value());

  day = RunPeriodControlSpecialDays(NthDayOfWeekInMonth::first, DayOfWeek::Monday, MonthOfYear::Feb, model);
  ASSERT_NO_THROW(day.startDate());
  EXPECT_EQ(4u, day.startDate().dayOfMonth());
  EXPECT_EQ(MonthOfYear::Feb, day.startDate().monthOfYear().value());
  EXPECT_EQ(DayOfWeek::Monday, day.startDate().dayOfWeek().value());

  day = RunPeriodControlSpecialDays(NthDayOfWeekInMonth::fourth, DayOfWeek::Monday, MonthOfYear::Feb, model);
  ASSERT_NO_THROW(day.startDate());
  EXPECT_EQ(25u, day.startDate().dayOfMonth());
  EXPECT_EQ(MonthOfYear::Feb, day.startDate().monthOfYear().value());
  EXPECT_EQ(DayOfWeek::Monday, day.startDate().dayOfWeek().value());
  EXPECT_EQ("4th Monday in February", day.getString(OS_RunPeriodControl_SpecialDaysFields::StartDate).get());

  // there is no fifth monday, this will return the last
  day = RunPeriodControlSpecialDays(NthDayOfWeekInMonth::fifth, DayOfWeek::Monday, MonthOfYear::Feb, model);
  ASSERT_NO_THROW(day.startDate());
  EXPECT_EQ(25u, day.startDate().dayOfMonth());
  EXPECT_EQ(MonthOfYear::Feb, day.startDate().monthOfYear().value());
  EXPECT_EQ(DayOfWeek::Monday, day.startDate().dayOfWeek().value());

  day = RunPeriodControlSpecialDays(NthDayOfWeekInMonth::fifth, DayOfWeek::Friday, MonthOfYear::Feb, model);
  ASSERT_NO_THROW(day.startDate());
  EXPECT_EQ(29u, day.startDate().dayOfMonth());
  EXPECT_EQ(MonthOfYear::Feb, day.startDate().monthOfYear().value());
  EXPECT_EQ(DayOfWeek::Friday, day.startDate().dayOfWeek().value());
}
