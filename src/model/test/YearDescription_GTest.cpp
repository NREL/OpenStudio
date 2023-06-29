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
