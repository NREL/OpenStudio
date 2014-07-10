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
#include "../RunPeriodControlDaylightSavingTime.hpp"
#include "../RunPeriodControlDaylightSavingTime_Impl.hpp"

#include "../../utilities/time/Date.hpp"

#include <iostream>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RunPeriodControlDaylightSavingTime)
{
  Model model;
  RunPeriodControlDaylightSavingTime dst = model.getUniqueModelObject<RunPeriodControlDaylightSavingTime>();
  ASSERT_NO_THROW(dst.startDate()); // 2nd Sunday in March, assumed year is 2009
  EXPECT_EQ(8u, dst.startDate().dayOfMonth());
  EXPECT_EQ(MonthOfYear::Mar, dst.startDate().monthOfYear().value());

  ASSERT_NO_THROW(dst.endDate()); // 1st Sunday in November, assumed year is 2009
  EXPECT_EQ(1u, dst.endDate().dayOfMonth());
  EXPECT_EQ(MonthOfYear::Nov, dst.endDate().monthOfYear().value());
}

