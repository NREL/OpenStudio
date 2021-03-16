/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../ChillerAbsorption.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ChillerAbsorption) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ChillerAbsorption chiller(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ChillerAbsorption_PlantLoopConnections) {
  model::Model model;
  model::ChillerAbsorption chiller(model);

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
    auto node = generatorLoop.demandInletNode();
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
TEST_F(ModelFixture, ChillerAbsorption_PlantLoopConnections_addToNodeOverride) {
  model::Model model;
  model::ChillerAbsorption chiller(model);

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
