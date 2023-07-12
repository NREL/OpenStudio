/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../PlantLoop.hpp"
#include "../Model.hpp"
#include "../ChillerElectricReformulatedEIR.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBicubic.hpp"
#include "../Mixer.hpp"
#include "../Mixer_Impl.hpp"
#include "../Splitter.hpp"
#include "../Splitter_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;

TEST_F(ModelFixture, ChillerElectricReformulatedEIR_ChillerElectricReformulatedEIR) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      model::Model m;

      model::CurveBiquadratic ccFofT(m);
      model::CurveBiquadratic eirToCorfOfT(m);
      model::CurveBicubic eiToCorfOfPlr(m);

      model::ChillerElectricReformulatedEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

// Add to the end of an empty supply side and check that it is placed correctly.
TEST_F(ModelFixture, ChillerElectricReformulatedEIR_addToNode1) {
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveBicubic eiToCorfOfPlr(m);

  model::ChillerElectricReformulatedEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  model::Node supplyOutletNode = plantLoop.supplyOutletNode();
  model::Mixer supplyMixer = plantLoop.supplyMixer();

  EXPECT_TRUE(chiller.addToNode(supplyOutletNode));

  EXPECT_EQ(7u, plantLoop.supplyComponents().size());

  ASSERT_TRUE(chiller.supplyOutletModelObject());

  EXPECT_EQ(supplyOutletNode, chiller.supplyOutletModelObject().get());

  boost::optional<model::ModelObject> mo = chiller.supplyInletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->inletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Mixer> mixer = mo->optionalCast<model::Mixer>();

  ASSERT_TRUE(mixer);
}

// Add to the front of an empty supply side and check that it is placed correctly.
TEST_F(ModelFixture, ChillerElectricReformulatedEIR_addToNode2) {
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveBicubic eiToCorfOfPlr(m);

  model::ChillerElectricReformulatedEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  model::Node supplyInletNode = plantLoop.supplyInletNode();

  EXPECT_TRUE(chiller.addToNode(supplyInletNode));

  EXPECT_EQ(7u, plantLoop.supplyComponents().size());

  boost::optional<model::ModelObject> mo = chiller.supplyInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(supplyInletNode, mo.get());

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(plantLoop.supplySplitter(), mo.get());
}

// Add to the middle of the existing branch.
TEST_F(ModelFixture, ChillerElectricReformulatedEIR_addToNode3) {
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveBicubic eiToCorfOfPlr(m);

  model::ChillerElectricReformulatedEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  model::Mixer supplyMixer = plantLoop.supplyMixer();
  model::Splitter supplySplitter = plantLoop.supplySplitter();

  boost::optional<model::ModelObject> mo = supplyMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  EXPECT_TRUE(chiller.addToNode(node.get()));

  EXPECT_EQ(7u, plantLoop.supplyComponents().size());

  mo = chiller.supplyInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(), mo.get());

  mo = node->inletModelObject();

  EXPECT_TRUE(mo);

  EXPECT_EQ(mo.get(), supplySplitter);

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(supplyMixer, mo.get());

  mo = supplySplitter.outletModelObject(0);

  boost::optional<model::ModelObject> mo2 = supplyMixer.inletModelObject(0);

  ASSERT_TRUE(mo);
  ASSERT_TRUE(mo2);

  boost::optional<model::HVACComponent> comp = mo->optionalCast<model::HVACComponent>();
  boost::optional<model::HVACComponent> comp2 = mo2->optionalCast<model::HVACComponent>();

  ASSERT_TRUE(comp);
  ASSERT_TRUE(comp2);

  std::vector<model::ModelObject> comps = plantLoop.supplyComponents(comp.get(), comp2.get());

  ASSERT_EQ(3u, comps.size());
}

// Add to new branch
TEST_F(ModelFixture, ChillerElectricReformulatedEIR_PlantLoop_addSupplyBranch) {
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveBicubic eiToCorfOfPlr(m);

  model::ChillerElectricReformulatedEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  ASSERT_TRUE(plantLoop.addSupplyBranchForComponent(chiller));

  EXPECT_EQ(7u, plantLoop.supplyComponents().size());

  model::Mixer supplyMixer = plantLoop.supplyMixer();

  boost::optional<model::ModelObject> mo = supplyMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(), mo.get());

  mo = chiller.supplyOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(supplyMixer, mo.get());
}

// Add to the end of an empty demand side and check that it is placed correctly.
TEST_F(ModelFixture, ChillerElectricReformulatedEIR_addToDemandNode1) {
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveBicubic eiToCorfOfPlr(m);

  model::ChillerElectricReformulatedEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  model::Node demandOutletNode = plantLoop.demandOutletNode();
  model::Mixer demandMixer = plantLoop.demandMixer();

  EXPECT_TRUE(chiller.addToNode(demandOutletNode));

  EXPECT_EQ(7u, plantLoop.demandComponents().size());

  ASSERT_TRUE(chiller.demandOutletModelObject());

  EXPECT_EQ(demandOutletNode, chiller.demandOutletModelObject().get());

  boost::optional<model::ModelObject> mo = chiller.demandInletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->inletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Mixer> mixer = mo->optionalCast<model::Mixer>();

  ASSERT_TRUE(mixer);

  ASSERT_TRUE(!chiller.remove().empty());

  EXPECT_EQ(5u, plantLoop.demandComponents().size());
}

// Add to the front of an empty demand side and check that it is placed correctly.
TEST_F(ModelFixture, ChillerElectricReformulatedEIR_addToDemandNode2) {
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveBicubic eiToCorfOfPlr(m);

  model::ChillerElectricReformulatedEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  model::Node demandInletNode = plantLoop.demandInletNode();

  EXPECT_TRUE(chiller.addToNode(demandInletNode));

  EXPECT_EQ(7u, plantLoop.demandComponents().size());

  boost::optional<model::ModelObject> mo = chiller.demandInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(demandInletNode, mo.get());

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(plantLoop.demandSplitter(), mo.get());

  ASSERT_TRUE(!chiller.remove().empty());

  EXPECT_EQ(5u, plantLoop.demandComponents().size());
}

// Add to the middle of the existing branch.
TEST_F(ModelFixture, ChillerElectricReformulatedEIR_addToDemandNode3) {
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveBicubic eiToCorfOfPlr(m);

  model::ChillerElectricReformulatedEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  model::Mixer demandMixer = plantLoop.demandMixer();

  boost::optional<model::ModelObject> mo = demandMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  EXPECT_TRUE(chiller.addToNode(node.get()));

  EXPECT_EQ(7u, plantLoop.demandComponents().size());

  mo = chiller.demandInletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(), mo.get());

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(demandMixer, mo.get());

  ASSERT_TRUE(!chiller.remove().empty());

  EXPECT_EQ(5u, plantLoop.demandComponents().size());
}

// Add to new demand branch
TEST_F(ModelFixture, ChillerElectricReformulatedEIR_PlantLoop_addDemandBranch) {
  model::Model m;

  model::PlantLoop plantLoop(m);

  model::CurveBiquadratic ccFofT(m);
  model::CurveBiquadratic eirToCorfOfT(m);
  model::CurveBicubic eiToCorfOfPlr(m);

  model::ChillerElectricReformulatedEIR chiller(m, ccFofT, eirToCorfOfT, eiToCorfOfPlr);

  ASSERT_TRUE(plantLoop.addDemandBranchForComponent(chiller));

  EXPECT_EQ(7u, plantLoop.demandComponents().size());

  model::Mixer demandMixer = plantLoop.demandMixer();

  boost::optional<model::ModelObject> mo = demandMixer.inletModelObject(0);

  ASSERT_TRUE(mo);

  boost::optional<model::Node> node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(node.get(), mo.get());

  mo = chiller.demandOutletModelObject();

  ASSERT_TRUE(mo);

  node = mo->optionalCast<model::Node>();

  ASSERT_TRUE(node);

  mo = node->outletModelObject();

  ASSERT_TRUE(mo);

  ASSERT_EQ(demandMixer, mo.get());

  ASSERT_TRUE(plantLoop.removeDemandBranchWithComponent(chiller));

  EXPECT_EQ(5u, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, ChillerElectricReformulatedEIR_PlantLoopConnections) {
  model::Model model;
  model::ChillerElectricReformulatedEIR chiller(model);

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

  // Heat Recovery Loop: on the demand side
  {
    model::PlantLoop hrLoop(model);
    auto node = hrLoop.demandOutletNode();
    EXPECT_TRUE(chiller.addToTertiaryNode(node));
    auto plant = chiller.tertiaryPlantLoop();
    EXPECT_TRUE(plant);
    if (plant) {
      EXPECT_EQ(hrLoop.handle(), plant->handle());
    }
    // test the convenience function
    auto plant_bis = chiller.heatRecoveryLoop();
    EXPECT_TRUE(plant_bis);
    if (plant) {
      EXPECT_EQ(hrLoop.handle(), plant_bis->handle());
    }

    EXPECT_EQ(7u, hrLoop.demandComponents().size());

    EXPECT_TRUE(chiller.removeFromTertiaryPlantLoop());
    plant = chiller.tertiaryPlantLoop();
    EXPECT_FALSE(plant);
    EXPECT_EQ(5u, hrLoop.demandComponents().size());
  }
}

/* I have overriden the base class WaterToWaterComponent addToNode() and addToTertiaryNode()
 * addToNode will call addToTertiaryNode behind the scenes when needed
 * that is if you try to add it to the demand side of a plant loop when it already has a condenser water loop
 * it should add it to the tertiary(=HR) loop
 * This should work with addDemandBranchForComponent too
 * AddToTertiaryNode is overriden to not work when trying to add to a supply side node */
TEST_F(ModelFixture, ChillerElectricReformulatedEIR_PlantLoopConnections_addToNodeOverride) {
  model::Model model;
  model::ChillerElectricReformulatedEIR chiller(model);

  model::PlantLoop chwLoop(model);

  model::PlantLoop cndwLoop(model);
  model::PlantLoop cndwLoop2(model);
  auto c_demand_node2 = cndwLoop2.demandOutletNode();

  model::PlantLoop hrLoop(model);
  auto h_supply_node = hrLoop.supplyOutletNode();
  auto h_demand_node = hrLoop.demandInletNode();

  // Connect to the chw loop
  EXPECT_TRUE(chwLoop.addSupplyBranchForComponent(chiller));
  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  EXPECT_FALSE(chiller.condenserWaterLoop());
  EXPECT_FALSE(chiller.heatRecoveryLoop());

  // Connect to the condenser loop
  EXPECT_TRUE(cndwLoop.addDemandBranchForComponent(chiller));

  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop, chiller.condenserWaterLoop().get());

  EXPECT_FALSE(chiller.heatRecoveryLoop());

  // Have a chw loop and no hr loop: should connect the hr loop if trying to add to demand side
  EXPECT_TRUE(hrLoop.addDemandBranchForComponent(chiller));
  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());
  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop, chiller.condenserWaterLoop().get());
  ASSERT_TRUE(chiller.heatRecoveryLoop());
  EXPECT_EQ(hrLoop, chiller.heatRecoveryLoop().get());

  // Have a condenser loop and a hr loop: should reconnect the condenser loop
  // Try with addToNode instead
  EXPECT_TRUE(chiller.addToNode(c_demand_node2));

  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop2, chiller.condenserWaterLoop().get());

  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  ASSERT_TRUE(chiller.heatRecoveryLoop());
  EXPECT_EQ(hrLoop, chiller.heatRecoveryLoop().get());

  // Disconnect the tertiary (hr) loop
  EXPECT_TRUE(chiller.removeFromTertiaryPlantLoop());
  EXPECT_FALSE(chiller.heatRecoveryLoop());

  // Shouldn't accept tertiary connection to the supply side
  EXPECT_FALSE(chiller.addToTertiaryNode(h_supply_node));
  EXPECT_FALSE(chiller.heatRecoveryLoop());

  // Try addToNode to the demand side of hr plant loop, should work to connect the hr loop
  EXPECT_TRUE(chiller.addToNode(h_demand_node));

  ASSERT_TRUE(chiller.chilledWaterLoop());
  EXPECT_EQ(chwLoop, chiller.chilledWaterLoop().get());

  ASSERT_TRUE(chiller.condenserWaterLoop());
  EXPECT_EQ(cndwLoop2, chiller.condenserWaterLoop().get());

  ASSERT_TRUE(chiller.heatRecoveryLoop());
  EXPECT_EQ(hrLoop, chiller.heatRecoveryLoop().get());
}
