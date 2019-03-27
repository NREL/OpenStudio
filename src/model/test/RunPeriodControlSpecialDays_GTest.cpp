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
