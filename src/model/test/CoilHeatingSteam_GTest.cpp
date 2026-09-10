/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoilHeatingSteam.hpp"
#include "../CoilHeatingSteam_Impl.hpp"
#include "../ControllerWaterCoil.hpp"
#include "../ControllerWaterCoil_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingSteam_CoilHeatingSteam) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      ScheduleCompact s(m);
      CoilHeatingSteam coil(m, s);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilHeatingSteam_addToNode) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingSteam coil(m, s);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);
  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
}

// Add CoilHeatingSteam to AirLoopHVAC and PlantLoop
// Test CoilHeatingSteam::remove and make sure that the loops are intact
TEST_F(ModelFixture, CoilHeatingSteam_remove) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingSteam coil(m, s);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);
  coil.remove();
  ASSERT_EQ((unsigned)2, airLoop.supplyComponents().size());

  ASSERT_TRUE(m.getConcreteModelObjects<CoilHeatingSteam>().empty());

  CoilHeatingSteam coil2(m, s);
  coil2.addToNode(supplyOutletNode);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil2);

  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilHeatingSteam>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingSteam::iddObjectType()).size());

  coil2.remove();
  ASSERT_TRUE(m.getConcreteModelObjects<CoilHeatingSteam>().empty());
}

// Add ONLY to PlantLoop
// Test CoilHeatingSteam::remove and make sure that the plant is intact
TEST_F(ModelFixture, CoilHeatingSteam_remove2) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingSteam coil(m, s);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil);
  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilHeatingSteam>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingSteam::iddObjectType()).size());

  coil.remove();
  ASSERT_TRUE(m.getConcreteModelObjects<CoilHeatingSteam>().empty());
}

// Add ONLY to PlantLoop
// This time use removeDemandBranchWithComponent
TEST_F(ModelFixture, CoilHeatingSteam_remove3) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingSteam coil(m, s);

  PlantLoop plant(m);
  plant.addDemandBranchForComponent(coil);

  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilHeatingSteam>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingSteam::iddObjectType()).size());

  plant.removeDemandBranchWithComponent(coil);
  ASSERT_TRUE(plant.demandComponents(CoilHeatingSteam::iddObjectType()).empty());
  ASSERT_TRUE(m.getConcreteModelObjects<CoilHeatingSteam>().empty());
}

// Add CoilHeatingSteam to AirLoopHVAC and PlantLoop
// Test PlantLoop::removeDemandBranchWithComponent and make sure that the loops are intact
// Test that the coil is still in the model and hooked up to AirLoopHVAC
TEST_F(ModelFixture, CoilHeatingSteam_remove4) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingSteam coil(m, s);

  AirLoopHVAC airLoop(m);
  Node supplyOutletNode = airLoop.supplyOutletNode();

  coil.addToNode(supplyOutletNode);

  PlantLoop plant(m);
  EXPECT_TRUE(plant.addDemandBranchForComponent(coil));

  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilHeatingSteam>().size());
  ASSERT_EQ((unsigned)1, plant.demandComponents(CoilHeatingSteam::iddObjectType()).size());

  plant.removeDemandBranchWithComponent(coil);
  ASSERT_TRUE(plant.demandComponents(CoilHeatingSteam::iddObjectType()).empty());
  ASSERT_EQ((unsigned)3, airLoop.supplyComponents().size());
  ASSERT_EQ((unsigned)1, m.getConcreteModelObjects<CoilHeatingSteam>().size());
}

TEST_F(ModelFixture, CoilHeatingSteam_controller) {
  Model m;
  ScheduleCompact s(m);
  CoilHeatingSteam coil(m, s);

  PlantLoop p(m);

  EXPECT_TRUE(p.addDemandBranchForComponent(coil));
  ASSERT_TRUE(coil.controllerWaterCoil());
}
