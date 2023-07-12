/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirLoopHVAC.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilCoolingWater.hpp"
#include "../ScheduleCompact.hpp"

using namespace openstudio;

TEST_F(ModelFixture, CoilCoolingWater_CoilCoolingWater) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      model::Model m;

      model::ScheduleCompact s(m);

      model::CoilCoolingWater coil(m, s);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilCoolingWater_addToNode) {
  model::Model m;

  model::ScheduleCompact s(m);

  model::CoilCoolingWater coil(m, s);

  model::AirLoopHVAC airLoop(m);

  model::Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);

  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
}
