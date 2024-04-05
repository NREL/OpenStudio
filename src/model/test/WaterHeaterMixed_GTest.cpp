/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../WaterHeaterMixed.hpp"
#include "../WaterHeaterMixed_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../PipeAdiabatic.hpp"
#include "../PipeAdiabatic_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"

#include "../ScheduleConstant.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, WaterHeaterMixed_WaterHeaterMixed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      model::Model m;
      model::WaterHeaterMixed waterHeaterMixed(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  model::Model m;
  model::WaterHeaterMixed waterHeaterMixed(m);

  ASSERT_TRUE(waterHeaterMixed.heaterThermalEfficiency());
  EXPECT_EQ(0.8, waterHeaterMixed.heaterThermalEfficiency().get());
  EXPECT_TRUE(waterHeaterMixed.setHeaterThermalEfficiency(1.3));
  EXPECT_EQ(1.3, waterHeaterMixed.heaterThermalEfficiency().get());
}

TEST_F(ModelFixture, WaterHeaterMixed_NewFields) {
  Model m;

  WaterHeaterMixed wh(m);

  // Test defaults
  EXPECT_EQ("IndirectHeatPrimarySetpoint", wh.sourceSideFlowControlMode());
  EXPECT_FALSE(wh.indirectAlternateSetpointTemperatureSchedule());
  EXPECT_EQ("General", wh.endUseSubcategory());

  EXPECT_TRUE(wh.setSourceSideFlowControlMode("StorageTank"));
  // Shouldn't accept it, it should be set via the schedule method
  EXPECT_FALSE(wh.setSourceSideFlowControlMode("IndirectHeatAlternateSetpoint"));
  EXPECT_FALSE(wh.setSourceSideFlowControlMode("BadValue"));

  ScheduleConstant sch(m);
  EXPECT_TRUE(wh.setIndirectAlternateSetpointTemperatureSchedule(sch));
  ASSERT_TRUE(wh.indirectAlternateSetpointTemperatureSchedule());
  EXPECT_EQ(sch, wh.indirectAlternateSetpointTemperatureSchedule().get());
  EXPECT_EQ("IndirectHeatAlternateSetpoint", wh.sourceSideFlowControlMode());

  // Reset the schedule also resets the flow control mode
  wh.resetIndirectAlternateSetpointTemperatureSchedule();
  EXPECT_FALSE(wh.indirectAlternateSetpointTemperatureSchedule());
  EXPECT_EQ("IndirectHeatPrimarySetpoint", wh.sourceSideFlowControlMode());

  EXPECT_TRUE(wh.setIndirectAlternateSetpointTemperatureSchedule(sch));
  // Changing the flow control should reset the schedule
  EXPECT_TRUE(wh.setSourceSideFlowControlMode("StorageTank"));
  EXPECT_FALSE(wh.indirectAlternateSetpointTemperatureSchedule());

  EXPECT_TRUE(wh.setEndUseSubcategory("SomethingElse"));
  EXPECT_EQ("SomethingElse", wh.endUseSubcategory());
}

TEST_F(ModelFixture, WaterHeaterMixed_PlantLoopConnections) {
  Model m;

  WaterHeaterMixed wh(m);

  PlantLoop p1(m);
  PlantLoop p2(m);

  // plant loop #1
  auto node1 = p1.supplyInletNode();
  EXPECT_TRUE(wh.addToNode(node1));

  EXPECT_EQ(1u, p1.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(0u, p2.supplyComponents(WaterHeaterMixed::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  EXPECT_FALSE(wh.demandInletModelObject());
  EXPECT_FALSE(wh.demandOutletModelObject());

  EXPECT_EQ(p1.supplyInletNode().nameString(), wh.supplyInletModelObject()->cast<Node>().nameString());
  std::string useSideOutletNodeName = wh.supplyOutletModelObject()->cast<Node>().nameString();
  EXPECT_NE("", useSideOutletNodeName);

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());
  EXPECT_FALSE(wh.sourceSidePlantLoop());

  // plant loop #2
  PipeAdiabatic bypass_pipe(m);
  p2.addSupplyBranchForComponent(bypass_pipe);
  ASSERT_TRUE(bypass_pipe.inletModelObject());
  ASSERT_TRUE(bypass_pipe.inletModelObject()->optionalCast<Node>());
  auto node2 = bypass_pipe.inletModelObject()->cast<Node>();
  EXPECT_TRUE(wh.addToSourceSideNode(node2));
  bypass_pipe.remove();

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.sourceSidePlantLoop());
  EXPECT_NE(wh.plantLoop()->handle(), wh.sourceSidePlantLoop()->handle());
  EXPECT_EQ(wh.sourceSidePlantLoop()->handle(), p2.handle());

  EXPECT_EQ(1u, p1.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1u, p2.supplyComponents(WaterHeaterMixed::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  ASSERT_TRUE(wh.demandInletModelObject());
  ASSERT_TRUE(wh.demandOutletModelObject());

  EXPECT_EQ(p1.supplyInletNode().nameString(), wh.supplyInletModelObject()->cast<Node>().nameString());  // doesn't change
  EXPECT_EQ(useSideOutletNodeName, wh.supplyOutletModelObject()->cast<Node>().nameString());             // doesn't change
  EXPECT_NE(p2.supplyInletNode().nameString(), wh.demandInletModelObject()->cast<Node>().nameString());
  EXPECT_NE("", wh.demandOutletModelObject()->cast<Node>().nameString());

  EXPECT_TRUE(wh.removeFromSourceSidePlantLoop());
  EXPECT_FALSE(wh.sourceSidePlantLoop());
}

TEST_F(ModelFixture, WaterHeaterMixed_PlantLoopConnections_addToNodeOverride) {
  Model m;

  WaterHeaterMixed wh(m);

  PlantLoop p1(m);
  PlantLoop p2(m);

  // plant loop #1
  auto node1 = p1.supplyInletNode();
  EXPECT_TRUE(wh.addToNode(node1));

  EXPECT_EQ(1u, p1.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(0u, p2.supplyComponents(WaterHeaterMixed::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  EXPECT_FALSE(wh.demandInletModelObject());
  EXPECT_FALSE(wh.demandOutletModelObject());

  EXPECT_EQ(p1.supplyInletNode().nameString(), wh.supplyInletModelObject()->cast<Node>().nameString());
  std::string useSideOutletNodeName = wh.supplyOutletModelObject()->cast<Node>().nameString();
  EXPECT_NE("", useSideOutletNodeName);

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());
  EXPECT_FALSE(wh.sourceSidePlantLoop());

  // plant loop #2
  PipeAdiabatic bypass_pipe(m);
  p2.addSupplyBranchForComponent(bypass_pipe);
  ASSERT_TRUE(bypass_pipe.inletModelObject());
  ASSERT_TRUE(bypass_pipe.inletModelObject()->optionalCast<Node>());
  auto node2 = bypass_pipe.inletModelObject()->cast<Node>();
  EXPECT_TRUE(wh.addToNode(node2));  // try with addToNode instead
  bypass_pipe.remove();

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.sourceSidePlantLoop());
  EXPECT_NE(wh.plantLoop()->handle(), wh.sourceSidePlantLoop()->handle());
  EXPECT_EQ(wh.sourceSidePlantLoop()->handle(), p2.handle());

  EXPECT_EQ(1u, p1.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1u, p2.supplyComponents(WaterHeaterMixed::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  ASSERT_TRUE(wh.demandInletModelObject());
  ASSERT_TRUE(wh.demandOutletModelObject());

  EXPECT_EQ(p1.supplyInletNode().nameString(), wh.supplyInletModelObject()->cast<Node>().nameString());  // doesn't change
  EXPECT_EQ(useSideOutletNodeName, wh.supplyOutletModelObject()->cast<Node>().nameString());             // doesn't change
  EXPECT_NE(p2.supplyInletNode().nameString(), wh.demandInletModelObject()->cast<Node>().nameString());
  EXPECT_NE("", wh.demandOutletModelObject()->cast<Node>().nameString());

  EXPECT_TRUE(wh.removeFromSourceSidePlantLoop());
  EXPECT_FALSE(wh.sourceSidePlantLoop());
}

TEST_F(ModelFixture, WaterHeaterMixed_PlantLoopConnections_addToNodeOverride_addSupplyBranch) {

  // Since addSupplyBranchForComponent calls addToNode, we should be able to call it twice in a row with two supply side nodes,
  // and connect it to the Use Side PlantLoop first and Source Side PlantLoop second
  Model m;

  WaterHeaterMixed wh(m);

  PlantLoop p1(m);
  PlantLoop p2(m);

  // plant loop #1
  EXPECT_TRUE(p1.addSupplyBranchForComponent(wh));

  EXPECT_EQ(1, p1.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(0, p2.supplyComponents(WaterHeaterMixed::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  EXPECT_FALSE(wh.demandInletModelObject());
  EXPECT_FALSE(wh.demandOutletModelObject());

  std::string useSideOutletNodeName = wh.supplyOutletModelObject()->cast<Node>().nameString();
  EXPECT_NE("", useSideOutletNodeName);

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());
  EXPECT_FALSE(wh.sourceSidePlantLoop());

  // plant loop #2
  EXPECT_TRUE(p2.addSupplyBranchForComponent(wh));

  ASSERT_TRUE(wh.plantLoop());
  EXPECT_EQ(wh.plantLoop()->handle(), p1.handle());
  ASSERT_TRUE(wh.useSidePlantLoop());
  EXPECT_EQ(wh.useSidePlantLoop()->handle(), p1.handle());

  ASSERT_TRUE(wh.secondaryPlantLoop());
  EXPECT_NE(wh.plantLoop()->handle(), wh.secondaryPlantLoop()->handle());
  EXPECT_EQ(wh.secondaryPlantLoop()->handle(), p2.handle());
  ASSERT_TRUE(wh.sourceSidePlantLoop());
  EXPECT_NE(wh.plantLoop()->handle(), wh.sourceSidePlantLoop()->handle());
  EXPECT_EQ(wh.sourceSidePlantLoop()->handle(), p2.handle());

  EXPECT_EQ(1, p1.supplyComponents(WaterHeaterMixed::iddObjectType()).size());
  EXPECT_EQ(1, p2.supplyComponents(WaterHeaterMixed::iddObjectType()).size());

  ASSERT_TRUE(wh.supplyInletModelObject());
  ASSERT_TRUE(wh.supplyOutletModelObject());
  ASSERT_TRUE(wh.demandInletModelObject());
  ASSERT_TRUE(wh.demandOutletModelObject());

  EXPECT_EQ(useSideOutletNodeName, wh.supplyOutletModelObject()->cast<Node>().nameString());  // doesn't change
  EXPECT_NE("", wh.demandOutletModelObject()->cast<Node>().nameString());

  EXPECT_TRUE(wh.removeFromSourceSidePlantLoop());
  EXPECT_FALSE(wh.sourceSidePlantLoop());
}
