/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

TEST_F(ModelFixture, RunPeriodControlDaylightSavingTime) {
  Model model;
  auto dst = model.getUniqueModelObject<RunPeriodControlDaylightSavingTime>();
  ASSERT_NO_THROW(dst.startDate());  // 2nd Sunday in March, assumed year is 2009
  EXPECT_EQ(8u, dst.startDate().dayOfMonth());
  EXPECT_EQ(MonthOfYear::Mar, dst.startDate().monthOfYear().value());

  ASSERT_NO_THROW(dst.endDate());  // 1st Sunday in November, assumed year is 2009
  EXPECT_EQ(1u, dst.endDate().dayOfMonth());
  EXPECT_EQ(MonthOfYear::Nov, dst.endDate().monthOfYear().value());
}
