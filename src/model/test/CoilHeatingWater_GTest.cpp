/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingWater_Impl.hpp"
#include "../ControllerWaterCoil.hpp"
#include "../ControllerWaterCoil_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingWater_CoilHeatingWater) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ScheduleCompact s(m);
      CoilHeatingWater coil(m, s);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilHeatingWater_addToNode) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);
  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
}

// Add CoilHeatingWater to AirLoopHVAC and PlantLoop
// Test CoilHeatingWater::remove and make sure that the loops are intact
TEST_F(ModelFixture, CoilHeatingWater_remove) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);
  coil.remove();
  ASSERT_EQ((unsigned)2, airLoop.supplyComponents().size());

  ASSERT_TRUE(m.getConcreteModelObjects<CoilHeatingWater>().empty());

  CoilHeatingWater coil2(m, s);
  coil2.addToNode(supplyOutletNode);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil2);

  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilHeatingWater>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingWater::iddObjectType()).size());

  coil2.remove();
  ASSERT_TRUE(m.getConcreteModelObjects<CoilHeatingWater>().empty());
}

// Add ONLY to PlantLoop
// Test CoilHeatingWater::remove and make sure that the plant is intact
TEST_F(ModelFixture, CoilHeatingWater_remove2) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil);
  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilHeatingWater>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingWater::iddObjectType()).size());

  coil.remove();
  ASSERT_TRUE(m.getConcreteModelObjects<CoilHeatingWater>().empty());
}

// Add ONLY to PlantLoop
// This time use removeDemandBranchWithComponent
TEST_F(ModelFixture, CoilHeatingWater_remove3) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil);

  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilHeatingWater>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingWater::iddObjectType()).size());

  plant.removeDemandBranchWithComponent(coil);
  ASSERT_TRUE(plant.demandComponents(CoilHeatingWater::iddObjectType()).empty());
  ASSERT_TRUE(m.getConcreteModelObjects<CoilHeatingWater>().empty());
}

// Add CoilHeatingWater to AirLoopHVAC and PlantLoop
// Test PlantLoop::removeDemandBranchWithComponent and make sure that the loops are intact
// Test that the coil is still in the model and hooked up to AirLoopHVAC
TEST_F(ModelFixture, CoilHeatingWater_remove4) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);

  PlantLoop plant(m);
  EXPECT_TRUE(plant.addDemandBranchForComponent(coil));

  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilHeatingWater>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingWater::iddObjectType()).size());

  plant.removeDemandBranchWithComponent(coil);
  ASSERT_TRUE(plant.demandComponents(CoilHeatingWater::iddObjectType()).empty());
  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilHeatingWater>().size());
}

TEST_F(ModelFixture, CoilHeatingWater_controller) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil(m, s);

  PlantLoop p(m);

  EXPECT_TRUE(p.addDemandBranchForComponent(coil));
  ASSERT_TRUE(coil.controllerWaterCoil());
}
