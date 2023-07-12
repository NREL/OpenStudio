/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../WaterUseConnections.hpp"
#include "../WaterUseConnections_Impl.hpp"
#include "../WaterUseEquipment.hpp"
#include "../WaterUseEquipmentDefinition.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, WaterUseConnections_WaterUseConnections) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      //create a model to use in testing this code.
      Model m;

      WaterUseConnections waterUseConnections(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, WaterUseConnections_addRemoveEquipment) {
  Model m;
  WaterUseConnections waterUseConnections(m);

  WaterUseEquipmentDefinition definition(m);

  WaterUseEquipment e1(definition);
  WaterUseEquipment e2(definition);
  WaterUseEquipment e3(definition);

  waterUseConnections.addWaterUseEquipment(e1);
  waterUseConnections.addWaterUseEquipment(e2);
  waterUseConnections.addWaterUseEquipment(e3);

  EXPECT_EQ(3u, waterUseConnections.waterUseEquipment().size());

  waterUseConnections.removeWaterUseEquipment(e2);

  EXPECT_EQ(2u, waterUseConnections.waterUseEquipment().size());
}

TEST_F(ModelFixture, WaterUseConnections_addToNode) {
  Model m;
  WaterUseConnections testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<WaterUseConnections>();
  demandOutletNode = plantLoop.demandOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, WaterUseConnections_DrainWaterHeatExchanger) {
  Model m;
  WaterUseConnections waterUseConnections(m);

  ASSERT_EQ("None", waterUseConnections.drainWaterHeatExchangerType());
  ASSERT_EQ("Plant", waterUseConnections.drainWaterHeatExchangerDestination());
  ASSERT_FALSE(waterUseConnections.drainWaterHeatExchangerUFactorTimesArea());

  EXPECT_FALSE(waterUseConnections.setDrainWaterHeatExchangerType("Idea1"));
  ASSERT_EQ("None", waterUseConnections.drainWaterHeatExchangerType());
  EXPECT_TRUE(waterUseConnections.setDrainWaterHeatExchangerType("CounterFlow"));
  EXPECT_FALSE(waterUseConnections.setDrainWaterHeatExchangerDestination("Eqiupment"));
  ASSERT_EQ("Plant", waterUseConnections.drainWaterHeatExchangerDestination());
  EXPECT_TRUE(waterUseConnections.setDrainWaterHeatExchangerDestination("PlantAndEquipment"));
  EXPECT_FALSE(waterUseConnections.setDrainWaterHeatExchangerUFactorTimesArea(-1.5));
  EXPECT_TRUE(waterUseConnections.setDrainWaterHeatExchangerUFactorTimesArea(2.5));

  ASSERT_EQ("CounterFlow", waterUseConnections.drainWaterHeatExchangerType());
  ASSERT_EQ("PlantAndEquipment", waterUseConnections.drainWaterHeatExchangerDestination());
  ASSERT_TRUE(waterUseConnections.drainWaterHeatExchangerUFactorTimesArea());
  ASSERT_EQ(2.5, waterUseConnections.drainWaterHeatExchangerUFactorTimesArea().get());

  waterUseConnections.resetDrainWaterHeatExchangerUFactorTimesArea();
  ASSERT_FALSE(waterUseConnections.drainWaterHeatExchangerUFactorTimesArea());
}
