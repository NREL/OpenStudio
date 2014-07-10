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

#include <iostream>

using namespace openstudio::model;

TEST_F(ModelFixture, YearDescription)
{
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

