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
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../YearDescription.hpp"
#include "../YearDescription_Impl.hpp"
#include "../RunPeriodControlSpecialDays.hpp"
#include "../RunPeriodControlSpecialDays_Impl.hpp"

#include "../../utilities/time/Date.hpp"

#include <iostream>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RunPeriodControlSpecialDays)
{
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
