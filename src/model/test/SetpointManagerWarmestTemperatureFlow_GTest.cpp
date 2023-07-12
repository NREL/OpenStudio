/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../SetpointManagerWarmestTemperatureFlow.hpp"
#include "../SetpointManagerWarmestTemperatureFlow_Impl.hpp"
#include "../Node.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerWarmestTemperatureFlow) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SetpointManagerWarmestTemperatureFlow testObject(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  {
    Model m;
    SetpointManagerWarmestTemperatureFlow spm(m);

    AirLoopHVAC airLoop(m);
    auto node = airLoop.supplyOutletNode();

    EXPECT_TRUE(spm.addToNode(node));
    EXPECT_EQ(1u, node.setpointManagers().size());

    spm.remove();
    EXPECT_EQ(0u, node.setpointManagers().size());
  }
}
