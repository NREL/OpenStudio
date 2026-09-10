/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../PumpVariableSpeedCondensate.hpp"
#include "../PumpVariableSpeedCondensate_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, PumpVariableSpeedCondensate_PumpVariableSpeedCondensate) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      PumpVariableSpeedCondensate pump(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, PumpVariableSpeedCondensate_connections) {
  Model m;
  PumpVariableSpeedCondensate pump(m);

  Node inletNode(m);
  Node outletNode(m);

  m.connect(inletNode, inletNode.outletPort(), pump, pump.inletPort());
  m.connect(pump, pump.outletPort(), outletNode, outletNode.inletPort());

  ASSERT_TRUE(pump.inletModelObject());
  ASSERT_TRUE(pump.outletModelObject());

  EXPECT_EQ(inletNode.handle(), pump.inletModelObject()->handle());
  EXPECT_EQ(outletNode.handle(), pump.outletModelObject()->handle());
}

TEST_F(ModelFixture, PumpVariableSpeedCondensate_addToNode) {
  Model m;
  PumpVariableSpeedCondensate testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  PumpVariableSpeedCondensate testObject2(m);

  EXPECT_TRUE(testObject2.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.demandComponents().size());

  PlantLoop plantLoop2(m);
  demandOutletNode = plantLoop2.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop2.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<PumpVariableSpeedCondensate>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());
}
