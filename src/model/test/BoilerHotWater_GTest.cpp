/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../BoilerHotWater.hpp"
#include "../BoilerHotWater_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../../utilities/data/DataEnums.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, BoilerHotWater_BoilerHotWater) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      BoilerHotWater boiler(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  BoilerHotWater boiler(m);

  EXPECT_EQ(0.0, boiler.onCycleParasiticElectricLoad());
  EXPECT_EQ(0.0, boiler.offCycleParasiticFuelLoad());

  EXPECT_TRUE(boiler.setOnCycleParasiticElectricLoad(0.5));
  EXPECT_EQ(0.5, boiler.onCycleParasiticElectricLoad());

  EXPECT_TRUE(boiler.setOffCycleParasiticFuelLoad(0.8));
  EXPECT_EQ(0.8, boiler.offCycleParasiticFuelLoad());
}

TEST_F(ModelFixture, BoilerHotWater_connections) {
  Model m;
  BoilerHotWater boiler(m);

  Node inletNode(m);
  Node outletNode(m);

  m.connect(inletNode, inletNode.outletPort(), boiler, boiler.inletPort());
  m.connect(boiler, boiler.outletPort(), outletNode, outletNode.inletPort());

  ASSERT_TRUE(boiler.inletModelObject());
  ASSERT_TRUE(boiler.outletModelObject());

  EXPECT_EQ(inletNode.handle(), boiler.inletModelObject()->handle());
  EXPECT_EQ(outletNode.handle(), boiler.outletModelObject()->handle());
}

TEST_F(ModelFixture, BoilerHotWater_addToNode) {
  Model m;
  BoilerHotWater testObject(m);

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
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<BoilerHotWater>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.supplyComponents().size());
}

TEST_F(ModelFixture, BoilerHotWater_remove) {
  Model m;

  PlantLoop plant(m);
  BoilerHotWater b1(m);
  BoilerHotWater b2(m);
  EXPECT_FALSE(b1.loop());
  EXPECT_FALSE(b2.loop());

  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 3u);

  EXPECT_TRUE(plant.addSupplyBranchForComponent(b1));
  EXPECT_GT(plant.supplyComponents(b1, plant.supplyOutletNode()).size(), 0u);
  EXPECT_TRUE(b1.loop());
  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 4u);
  {
    auto node = b1.outletModelObject()->cast<Node>();
    EXPECT_TRUE(b2.addToNode(node));
    EXPECT_GT(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
    EXPECT_TRUE(b2.loop());
  }

  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 5u);

  EXPECT_GT(plant.supplyComponents(b1, plant.supplyOutletNode()).size(), 0u);
  EXPECT_GT(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
  EXPECT_EQ(plant.demandComponents(b1, plant.demandOutletNode()).size(), 0u);
  EXPECT_EQ(plant.demandComponents(b2, plant.demandOutletNode()).size(), 0u);

  auto n1 = b1.outletModelObject().get();
  auto n2 = b2.outletModelObject().get();

  b1.remove();
  EXPECT_EQ(plant.supplyComponents(b1, plant.supplyOutletNode()).size(), 0u);
  EXPECT_GT(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 4u);
  b2.remove();
  EXPECT_EQ(plant.supplyComponents(b2, plant.supplyOutletNode()).size(), 0u);
  EXPECT_EQ(plant.supplyComponents(Node::iddObjectType()).size(), 3u);

  EXPECT_TRUE(n1.handle().isNull());
  EXPECT_TRUE(n2.handle().isNull());
}

TEST_F(ModelFixture, BoilerHotWater_HeatCoolFuelTypes) {

  using namespace openstudio;

  Model m;

  PlantLoop p(m);
  BoilerHotWater b(m);
  p.addSupplyBranchForComponent(b);

  for (auto& fuelTypeString : BoilerHotWater::validFuelTypeValues()) {
    EXPECT_TRUE(b.setFuelType(fuelTypeString));

    const FuelType fuelType(fuelTypeString);
    const AppGFuelType appGFuelType = convertFuelTypeToAppG(fuelType);

    EXPECT_EQ(ComponentType(ComponentType::Heating), b.componentType());
    EXPECT_EQ(0, b.coolingFuelTypes().size());
    ASSERT_EQ(1, b.heatingFuelTypes().size());
    EXPECT_EQ(fuelType, b.heatingFuelTypes().front());
    ASSERT_EQ(1, b.appGHeatingFuelTypes().size());
    EXPECT_EQ(appGFuelType, b.appGHeatingFuelTypes().front());

    EXPECT_EQ(ComponentType(ComponentType::Heating), p.componentType());
    EXPECT_EQ(0, p.coolingFuelTypes().size());
    ASSERT_EQ(1, p.heatingFuelTypes().size());
    EXPECT_EQ(fuelType, p.heatingFuelTypes().front());
    ASSERT_EQ(1, p.appGHeatingFuelTypes().size());
    EXPECT_EQ(appGFuelType, p.appGHeatingFuelTypes().front());
  }
}
