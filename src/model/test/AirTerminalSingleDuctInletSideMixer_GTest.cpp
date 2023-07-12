/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirTerminalSingleDuctInletSideMixer.hpp"
#include "../AirTerminalSingleDuctInletSideMixer_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../ThermalZone.hpp"
#include "../DesignSpecificationOutdoorAir.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);
      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_addToNode) {
  Model m;
  AirTerminalSingleDuctInletSideMixer testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctInletSideMixer>();
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObjectClone.addToNode(inletNode));
  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObjectClone));
  EXPECT_EQ((unsigned)10, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddToNodeWithThermalZone) {
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);
  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());

  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddAirTerminalToPlantLoopAddDemandBranchForComponent) {
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);
  PlantLoop plantLoop(model);

  airLoop.addBranchForZone(thermalZone);
  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  testObject.addToNode(inletNode);

  EXPECT_FALSE(plantLoop.addDemandBranchForComponent(testObject));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddObjectByAirLoopAddBranchForZoneWithThermalZone) {
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  EXPECT_TRUE(airLoop.addBranchForZone(thermalZone, testObject));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());

  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddObjectByAirLoopAddBranchForHVACComponent) {
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);

  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObject));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddObjectByAirLoopAddBranchForHVACComponentWithThermalZone) {
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);

  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObject));
  EXPECT_EQ((unsigned)10, airLoop.demandComponents().size());

  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddToNodeTwoSameObjects) {
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(testObject.addToNode(inletNode));

  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_IsRemovable) {
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  EXPECT_TRUE(testObject.isRemovable());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_Remove) {
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);

  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_RemoveObjectWithThermalZone) {
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);

  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_NewFields_Control_OA) {
  // Test for #3599 - Adding 'Control for Outdoor' Air and 'Per Person Ventilation Flow Rate'
  Model model;
  AirTerminalSingleDuctInletSideMixer atu = AirTerminalSingleDuctInletSideMixer(model);

  EXPECT_TRUE(atu.controlForOutdoorAir());
  EXPECT_EQ("CurrentOccupancy", atu.perPersonVentilationRateMode());

  EXPECT_TRUE(atu.setControlForOutdoorAir(false));
  EXPECT_FALSE(atu.controlForOutdoorAir());

  EXPECT_TRUE(atu.setPerPersonVentilationRateMode("DesignOccupancy"));
  EXPECT_EQ("DesignOccupancy", atu.perPersonVentilationRateMode());
  EXPECT_FALSE(atu.setPerPersonVentilationRateMode("BADENUM"));
  EXPECT_EQ("DesignOccupancy", atu.perPersonVentilationRateMode());
}
