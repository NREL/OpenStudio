/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../AvailabilityManagerScheduledOn.hpp"
#include "../AvailabilityManagerScheduledOn_Impl.hpp"

#include "../ScheduleConstant.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AvailabilityManagerScheduledOn_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      AvailabilityManagerScheduledOn avm(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AvailabilityManagerScheduledOn_Setters_Getters) {
  Model m;

  AvailabilityManagerScheduledOn avm(m);

  // Test Schedule
  ASSERT_EQ(m.alwaysOnDiscreteSchedule(), avm.schedule());
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);
  EXPECT_TRUE(avm.setSchedule(tempSch));
  ASSERT_EQ(tempSch.handle(), avm.schedule().handle());
}
