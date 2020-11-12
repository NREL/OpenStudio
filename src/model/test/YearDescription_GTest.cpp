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
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../YearDescription.hpp"
#include "../YearDescription_Impl.hpp"

#include <iostream>

using namespace openstudio::model;

TEST_F(ModelFixture, YearDescription) {
  Model model;
  YearDescription yd = model.getUniqueModelObject<YearDescription>();

  EXPECT_FALSE(yd.calendarYear());
  EXPECT_EQ("UseWeatherFile", yd.dayofWeekforStartDay());
  EXPECT_TRUE(yd.isDayofWeekforStartDayDefaulted());
  EXPECT_FALSE(yd.isLeapYear());
  EXPECT_TRUE(yd.isIsLeapYearDefaulted());

  yd.setCalendarYear(2008);
  ASSERT_TRUE(yd.calendarYear());
  EXPECT_EQ(2008, yd.calendarYear().get());
  EXPECT_EQ("Tuesday", yd.dayofWeekforStartDay());
  EXPECT_TRUE(yd.isDayofWeekforStartDayDefaulted());
  EXPECT_TRUE(yd.isLeapYear());
  EXPECT_TRUE(yd.isIsLeapYearDefaulted());

  EXPECT_FALSE(yd.setDayofWeekforStartDay("Monday"));
  EXPECT_EQ("Tuesday", yd.dayofWeekforStartDay());
  EXPECT_TRUE(yd.isDayofWeekforStartDayDefaulted());

  EXPECT_FALSE(yd.setIsLeapYear(false));
  EXPECT_TRUE(yd.isLeapYear());
  EXPECT_TRUE(yd.isIsLeapYearDefaulted());

  yd.resetCalendarYear();
  EXPECT_EQ("UseWeatherFile", yd.dayofWeekforStartDay());
  EXPECT_TRUE(yd.isDayofWeekforStartDayDefaulted());
  EXPECT_FALSE(yd.isLeapYear());
  EXPECT_TRUE(yd.isIsLeapYearDefaulted());

  EXPECT_TRUE(yd.setDayofWeekforStartDay("Monday"));
  EXPECT_EQ("Monday", yd.dayofWeekforStartDay());
  EXPECT_FALSE(yd.isDayofWeekforStartDayDefaulted());

  EXPECT_TRUE(yd.setIsLeapYear(true));
  EXPECT_TRUE(yd.isLeapYear());
  EXPECT_FALSE(yd.isIsLeapYearDefaulted());

  yd.setCalendarYear(2009);
  ASSERT_TRUE(yd.calendarYear());
  EXPECT_EQ(2009, yd.calendarYear().get());
  EXPECT_EQ("Thursday", yd.dayofWeekforStartDay());
  EXPECT_TRUE(yd.isDayofWeekforStartDayDefaulted());
  EXPECT_FALSE(yd.isLeapYear());
  EXPECT_TRUE(yd.isIsLeapYearDefaulted());
}
