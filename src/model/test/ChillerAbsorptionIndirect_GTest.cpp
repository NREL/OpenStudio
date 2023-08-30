/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../ChillerAbsorptionIndirect.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ChillerAbsorptionIndirect) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ChillerAbsorptionIndirect chiller(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ChillerAbsorptionIndirect_PlantLoopConnections) {
  model::Model model;
  model::ChillerAbsorptionIndirect chiller(model);

  // Chilled Water Loop: on the supply side
  {
    model::PlantLoop chwLoop(model);
    auto node = chwLoop.supplyOutletNode();
    EXPECT_TRUE(chiller.addToNode(node));
    auto plant = chiller.plantLoop();
    EXPECT_TRUE(plant);
    if (plant) {
      EXPECT_EQ(chwLoop.handle(), plant->handle());
    }
    // PlantLoop has 5 components on the supply side by default (3 Nodes, One splitter, One mixer)
    EXPECT_EQ(7u, chwLoop.supplyComponents().size());

    // test the convenience function
    auto plant_bis = chiller.chilledWaterLoop();
    EXPECT_TRUE(plant_bis);
    if (plant) {
      EXPECT_EQ(chwLoop.handle(), plant_bis->handle());
    }
  }

  // Condenser Loop: on the demand side
  {
    model::PlantLoop cndwLoop(model);
    auto node = cndwLoop.demandInletNode();
    EXPECT_TRUE(chiller.addToNode(node));
    auto plant = chiller.secondaryPlantLoop();
    EXPECT_TRUE(plant);
    if (plant) {
      EXPECT_EQ(cndwLoop.handle(), plant->handle());
    }

    EXPECT_EQ(7u, cndwLoop.demandComponents().size());

    // test the convenience function
    auto plant_bis = chiller.condenserWaterLoop();
    EXPECT_TRUE(plant_bis);
    if (plant) {
      EXPECT_EQ(cndwLoop.handle(), plant_bis->handle());
    }
  }

  // Generator Loop: on the demand side
  {
    model::PlantLoop generatorLoop(model);
    auto node = generatorLoop.demandOutletNode();
    EXPECT_TRUE(chiller.addToTertiaryNode(node));
    auto plant = chiller.tertiaryPlantLoop();
    EXPECT_TRUE(plant);
    if (plant) {
      EXPECT_EQ(generatorLoop.handle(), plant->handle());
    }
    // test the convenience function
    auto plant_bis = chiller.generatorLoop();
    EXPECT_TRUE(plant_bis);
    if (plant) {
      EXPECT_EQ(generatorLoop.handle(), plant_bis->handle());
    }

    EXPECT_EQ(7u, generatorLoop.demandComponents().size());

    EXPECT_TRUE(chiller.removeFromTertiaryPlantLoop());
    plant = chiller.tertiaryPlantLoop();
    EXPECT_FALSE(plant);
    EXPECT_EQ(5u, generatorLoop.demandComponents().size());
  }
}

/* I have overriden the base class WaterToWaterComponent addToNode() and addToTertiaryNode()
 * addToNode will call addToTertiaryNode behind the scenes when needed
 * that is if you try to add it to the demand side of a plant loop when it already has a condenser water loop
 * it should add it to the tertiary(=Generator) loop
 * This should work with addDemandBranchForComponent too
 * AddToTertiaryNode is overriden to not work when trying to add to a supply side node */
TEST_F(ModelFixture, ChillerAbsorptionIndirect_PlantLoopConnections_addToNodeOverride) {
  model::Model model;
  model::ChillerAbsorptionIndirect chiller(model);

  model::PlantLoop chwLoop(model);

  model::PlantLoop cndwLoop(model);
  model::PlantLoop cndwLoop2(model);
  auto c_demand_node2 = cndwLoop2.demandOutletNode();

  model::PlantLoop generatorLoop(model);
  auto h_supply_node = generatorLoop.supplyOutletNode();
  auto h_demand_node = generatorLoop.demandInletNode();

  // Connect to the chw loop
  EXPECT_TRUE(chwLoop.addSupplyBranchForComponent(chiller));
  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  EXPECT_FALSE(chiller.condenserWaterLoop());
  EXPECT_FALSE(chiller.generatorLoop());

  // Connect to the condenser loop
  EXPECT_TRUE(cndwLoop.addDemandBranchForComponent(chiller));

  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop, chiller.condenserWaterLoop().get());

  EXPECT_FALSE(chiller.generatorLoop());

  // Have a chw loop and no generator loop: should connect the hr loop if trying to add to demand side
  EXPECT_TRUE(chiller.setGeneratorHeatSourceType("Steam"));
  EXPECT_EQ("Steam", chiller.generatorHeatSourceType());
  EXPECT_TRUE(generatorLoop.addDemandBranchForComponent(chiller));
  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());
  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop, chiller.condenserWaterLoop().get());
  ASSERT_TRUE(chiller.generatorLoop());
  EXPECT_EQ(generatorLoop, chiller.generatorLoop().get());
  // This should have switched the generator heat source type to "HotWater"
  EXPECT_EQ("HotWater", chiller.generatorHeatSourceType());
  // And Disallow switching to "Steam"
  EXPECT_FALSE(chiller.setGeneratorHeatSourceType("Steam"));
  EXPECT_EQ("HotWater", chiller.generatorHeatSourceType());

  // Have a condenser loop and a generator loop: should reconnect the condenser loop
  // Try with addToNode instead
  EXPECT_TRUE(chiller.addToNode(c_demand_node2));

  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop2, chiller.condenserWaterLoop().get());

  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  ASSERT_TRUE(chiller.generatorLoop());
  EXPECT_EQ(generatorLoop, chiller.generatorLoop().get());

  // Disconnect the tertiary (generator) loop
  EXPECT_TRUE(chiller.removeFromTertiaryPlantLoop());
  EXPECT_FALSE(chiller.generatorLoop());
  // This should have switched the generator heat source type to "Steam"
  EXPECT_EQ("Steam", chiller.generatorHeatSourceType());

  // Shouldn't accept tertiary connection to the supply side
  EXPECT_FALSE(chiller.addToTertiaryNode(h_supply_node));
  EXPECT_FALSE(chiller.generatorLoop());

  // Try addToNode to the demand side of generator plant loop, should work to connect the generator loop
  EXPECT_TRUE(chiller.addToNode(h_demand_node));

  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop2, chiller.condenserWaterLoop().get());

  ASSERT_TRUE(chiller.generatorLoop());
  EXPECT_EQ(generatorLoop, chiller.generatorLoop().get());
  // This should have switched the generator heat source type to "HotWater"
  EXPECT_EQ("HotWater", chiller.generatorHeatSourceType());
}
