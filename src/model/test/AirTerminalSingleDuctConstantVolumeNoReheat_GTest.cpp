/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirTerminalSingleDuctConstantVolumeNoReheat.hpp"
#include "../AirTerminalSingleDuctConstantVolumeNoReheat_Impl.hpp"
#include "../Schedule.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeNoReheat_AirTerminalSingleDuctConstantVolumeNoReheat) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Schedule s = m.alwaysOnDiscreteSchedule();
      AirTerminalSingleDuctConstantVolumeNoReheat testObject(m, s);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeNoReheat_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctConstantVolumeNoReheat testObject(m, s);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  // Inlet Node, splitter, placeholder node, mixer, outlet node
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  EXPECT_TRUE(testObject.addToNode(inletNode));
  // Inlet Node, splitter, ATU inlet Node, ATU, ATU outlet Node, mixer, outlet node
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctConstantVolumeNoReheat>();

  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObjectClone));
  // 7 initially, plus a branch with ATU inlet Node, ATU, ATU outlet node
  EXPECT_EQ((unsigned)10, airLoop.demandComponents().size());
}

// In E+ 9.0.0, ATU SingleDuctUncontrolled was renamed to SingleDuctConstantVolumeNoReheat
// To be more consistent with the naming convention of others ATU
// So we test that we can instantiate with the old name too
TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeNoReheat_AliasUncontrolled) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctUncontrolled testObject(m, s);

  EXPECT_EQ(testObject.iddObjectType(), AirTerminalSingleDuctConstantVolumeNoReheat::iddObjectType());
}
