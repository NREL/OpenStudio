/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../RefrigerationCondenserWaterCooled.hpp"
#include "../RefrigerationCondenserWaterCooled_Impl.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_RefrigerationCondenserWaterCooled) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_iddObjectType) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  EXPECT_EQ(refrigerationCondenserWaterCooled.iddObjectType(), IddObjectType::OS_Refrigeration_Condenser_WaterCooled);
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_addToNode) {
  Model m;
  RefrigerationCondenserWaterCooled testObject(m);

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

  auto testObjectClone = testObject.clone(m).cast<RefrigerationCondenserWaterCooled>();
  demandOutletNode = plantLoop.demandOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_RatedEffectiveTotalHeatRejectionRate) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  EXPECT_EQ(58000.0, refrigerationCondenserWaterCooled.ratedEffectiveTotalHeatRejectionRate().get());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setRatedEffectiveTotalHeatRejectionRate(12.0));
  EXPECT_EQ(12.0, refrigerationCondenserWaterCooled.ratedEffectiveTotalHeatRejectionRate().get());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setRatedEffectiveTotalHeatRejectionRate(1.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setRatedEffectiveTotalHeatRejectionRate(0.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setRatedEffectiveTotalHeatRejectionRate(-1.0));
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_RatedCondensingTemperature) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  EXPECT_EQ(29.4, refrigerationCondenserWaterCooled.ratedCondensingTemperature());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setRatedCondensingTemperature(12.0));
  EXPECT_EQ(12.0, refrigerationCondenserWaterCooled.ratedCondensingTemperature());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setRatedEffectiveTotalHeatRejectionRate(1.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setRatedEffectiveTotalHeatRejectionRate(0.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setRatedEffectiveTotalHeatRejectionRate(-1.0));
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_RatedSubcoolingTemperatureDifference) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  EXPECT_EQ(0.0, refrigerationCondenserWaterCooled.ratedSubcoolingTemperatureDifference());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setRatedSubcoolingTemperatureDifference(12.0));
  EXPECT_EQ(12.0, refrigerationCondenserWaterCooled.ratedSubcoolingTemperatureDifference());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setRatedEffectiveTotalHeatRejectionRate(1.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setRatedEffectiveTotalHeatRejectionRate(0.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setRatedEffectiveTotalHeatRejectionRate(-1.0));
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_RatedWaterInletTemperature) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  EXPECT_EQ(10.0, refrigerationCondenserWaterCooled.ratedWaterInletTemperature());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setRatedWaterInletTemperature(12.0));
  EXPECT_EQ(12.0, refrigerationCondenserWaterCooled.ratedWaterInletTemperature());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setRatedWaterInletTemperature(1.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setRatedWaterInletTemperature(0.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setRatedWaterInletTemperature(-1.0));
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_Connections) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  model::Node inletNode(m);

  model::Node outletNode(m);

  m.connect(inletNode, inletNode.outletPort(), refrigerationCondenserWaterCooled, refrigerationCondenserWaterCooled.inletPort());
  m.connect(refrigerationCondenserWaterCooled, refrigerationCondenserWaterCooled.outletPort(), outletNode, outletNode.inletPort());

  ASSERT_TRUE(refrigerationCondenserWaterCooled.inletModelObject());
  ASSERT_TRUE(refrigerationCondenserWaterCooled.outletModelObject());

  EXPECT_EQ(inletNode.handle(), refrigerationCondenserWaterCooled.inletModelObject()->handle());
  EXPECT_EQ(outletNode.handle(), refrigerationCondenserWaterCooled.outletModelObject()->handle());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_PlantLoopAddDemandBranchForComponent) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  PlantLoop plantLoop = PlantLoop(m);
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(refrigerationCondenserWaterCooled));

  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  ASSERT_TRUE(refrigerationCondenserWaterCooled.inletModelObject());
  ASSERT_TRUE(refrigerationCondenserWaterCooled.outletModelObject());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_WaterCooledLoopFlowType) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  EXPECT_EQ("ConstantFlow", refrigerationCondenserWaterCooled.waterCooledLoopFlowType());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterCooledLoopFlowType("VariableFlow"));
  EXPECT_EQ("VariableFlow", refrigerationCondenserWaterCooled.waterCooledLoopFlowType());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterCooledLoopFlowType("ConstantFlow"));
  refrigerationCondenserWaterCooled.resetWaterCooledLoopFlowType();
  EXPECT_EQ("VariableFlow", refrigerationCondenserWaterCooled.waterCooledLoopFlowType());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_WaterOutletTemperatureScheduleName) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  ScheduleCompact schedule(m);

  refrigerationCondenserWaterCooled.setWaterOutletTemperatureSchedule(schedule);
  ASSERT_EQ(schedule, refrigerationCondenserWaterCooled.waterOutletTemperatureSchedule().get());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_WaterDesignFlowRate) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  EXPECT_EQ(0.0025, refrigerationCondenserWaterCooled.waterDesignFlowRate().get());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterDesignFlowRate(12.0));
  EXPECT_EQ(12.0, refrigerationCondenserWaterCooled.waterDesignFlowRate().get());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterDesignFlowRate(1.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setWaterDesignFlowRate(0.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setWaterDesignFlowRate(-1.0));
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_WaterMaximumFlowRate) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  EXPECT_EQ(0.003, refrigerationCondenserWaterCooled.waterMaximumFlowRate().get());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterMaximumFlowRate(12.0));
  EXPECT_EQ(12.0, refrigerationCondenserWaterCooled.waterMaximumFlowRate().get());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterMaximumFlowRate(1.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setWaterMaximumFlowRate(0.0));
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setWaterMaximumFlowRate(-1.0));
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_WaterMaximumWaterOutletTemperature) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  EXPECT_EQ(55.0, refrigerationCondenserWaterCooled.waterMaximumWaterOutletTemperature());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterMaximumWaterOutletTemperature(12.0));
  EXPECT_EQ(12.0, refrigerationCondenserWaterCooled.waterMaximumWaterOutletTemperature());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterMaximumWaterOutletTemperature(10.0));
  EXPECT_EQ(10.0, refrigerationCondenserWaterCooled.waterMaximumWaterOutletTemperature());
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setWaterMaximumWaterOutletTemperature(9.5));

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterMaximumWaterOutletTemperature(60.0));
  EXPECT_EQ(60.0, refrigerationCondenserWaterCooled.waterMaximumWaterOutletTemperature());
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setWaterMaximumWaterOutletTemperature(60.5));

  refrigerationCondenserWaterCooled.resetWaterMaximumWaterOutletTemperature();
  EXPECT_EQ(55.0, refrigerationCondenserWaterCooled.waterMaximumWaterOutletTemperature());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_WaterMinimumWaterInletTemperature) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  EXPECT_EQ(10.0, refrigerationCondenserWaterCooled.waterMinimumWaterInletTemperature());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterMinimumWaterInletTemperature(12.0));
  EXPECT_EQ(12.0, refrigerationCondenserWaterCooled.waterMinimumWaterInletTemperature());

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterMinimumWaterInletTemperature(10.0));
  EXPECT_EQ(10.0, refrigerationCondenserWaterCooled.waterMinimumWaterInletTemperature());
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setWaterMinimumWaterInletTemperature(9.5));

  EXPECT_TRUE(refrigerationCondenserWaterCooled.setWaterMinimumWaterInletTemperature(30.0));
  EXPECT_EQ(30.0, refrigerationCondenserWaterCooled.waterMinimumWaterInletTemperature());
  EXPECT_FALSE(refrigerationCondenserWaterCooled.setWaterMinimumWaterInletTemperature(30.5));

  refrigerationCondenserWaterCooled.resetWaterMinimumWaterInletTemperature();
  EXPECT_EQ(10.0, refrigerationCondenserWaterCooled.waterMinimumWaterInletTemperature());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_EndUseCategory) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  EXPECT_EQ("General", refrigerationCondenserWaterCooled.endUseSubcategory());

  refrigerationCondenserWaterCooled.setEndUseSubcategory("Something else");
  EXPECT_EQ("Something else", refrigerationCondenserWaterCooled.endUseSubcategory());

  refrigerationCondenserWaterCooled.resetEndUseSubcategory();
  EXPECT_EQ("General", refrigerationCondenserWaterCooled.endUseSubcategory());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_CondenserRefrigerantOperatingChargeInventory) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  refrigerationCondenserWaterCooled.setCondenserRefrigerantOperatingChargeInventory(12.0);
  EXPECT_EQ(12.0, refrigerationCondenserWaterCooled.condenserRefrigerantOperatingChargeInventory().get());

  refrigerationCondenserWaterCooled.setCondenserRefrigerantOperatingChargeInventory(-12.0);
  EXPECT_EQ(-12.0, refrigerationCondenserWaterCooled.condenserRefrigerantOperatingChargeInventory().get());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_CondensateReceiverRefrigerantInventory) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  refrigerationCondenserWaterCooled.setCondensateReceiverRefrigerantInventory(12.0);
  EXPECT_EQ(12.0, refrigerationCondenserWaterCooled.condensateReceiverRefrigerantInventory().get());

  refrigerationCondenserWaterCooled.setCondensateReceiverRefrigerantInventory(-12.0);
  EXPECT_EQ(-12.0, refrigerationCondenserWaterCooled.condensateReceiverRefrigerantInventory().get());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_CondensatePipingRefrigerantInventory) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  refrigerationCondenserWaterCooled.setCondensatePipingRefrigerantInventory(12.0);
  EXPECT_EQ(12.0, refrigerationCondenserWaterCooled.condensatePipingRefrigerantInventory().get());

  refrigerationCondenserWaterCooled.setCondensatePipingRefrigerantInventory(-12.0);
  EXPECT_EQ(-12.0, refrigerationCondenserWaterCooled.condensatePipingRefrigerantInventory().get());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_Remove) {
  Model model;
  RefrigerationCondenserWaterCooled testObject = RefrigerationCondenserWaterCooled(model);
  ScheduleCompact schedule(model);

  std::vector<RefrigerationCondenserWaterCooled> refrigerationWaterCooledCondensers =
    model.getConcreteModelObjects<RefrigerationCondenserWaterCooled>();
  EXPECT_EQ(1, refrigerationWaterCooledCondensers.size());

  std::vector<Schedule> refrigerationSchedules = model.getModelObjects<Schedule>();
  EXPECT_EQ(1, refrigerationSchedules.size());

  testObject.setWaterOutletTemperatureSchedule(schedule);

  testObject.remove();

  refrigerationWaterCooledCondensers = model.getConcreteModelObjects<RefrigerationCondenserWaterCooled>();
  EXPECT_EQ(0, refrigerationWaterCooledCondensers.size());

  refrigerationSchedules = model.getModelObjects<Schedule>();
  EXPECT_EQ(1, refrigerationSchedules.size());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_CloneOneModelWithDefaultData) {
  Model m;

  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);

  auto refrigerationCondenserWaterCooledClone = refrigerationCondenserWaterCooled.clone(m).cast<RefrigerationCondenserWaterCooled>();

  EXPECT_NE(refrigerationCondenserWaterCooledClone.handle(), refrigerationCondenserWaterCooled.handle());

  EXPECT_EQ(58000.0, refrigerationCondenserWaterCooledClone.ratedEffectiveTotalHeatRejectionRate().get());
  EXPECT_EQ(29.4, refrigerationCondenserWaterCooledClone.ratedCondensingTemperature());
  EXPECT_EQ(0.0, refrigerationCondenserWaterCooledClone.ratedSubcoolingTemperatureDifference());
  EXPECT_EQ(10.0, refrigerationCondenserWaterCooledClone.ratedWaterInletTemperature());
  EXPECT_EQ("ConstantFlow", refrigerationCondenserWaterCooledClone.waterCooledLoopFlowType());
  EXPECT_EQ(0.0025, refrigerationCondenserWaterCooledClone.waterDesignFlowRate().get());
  EXPECT_EQ(0.003, refrigerationCondenserWaterCooledClone.waterMaximumFlowRate().get());
  EXPECT_EQ(55.0, refrigerationCondenserWaterCooledClone.waterMaximumWaterOutletTemperature());
  EXPECT_EQ(10.0, refrigerationCondenserWaterCooledClone.waterMinimumWaterInletTemperature());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_CloneOneModelWithCustomData) {
  Model m;

  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);
  refrigerationCondenserWaterCooled.setRatedEffectiveTotalHeatRejectionRate(52000.0);
  refrigerationCondenserWaterCooled.setRatedCondensingTemperature(25.0);
  refrigerationCondenserWaterCooled.setRatedSubcoolingTemperatureDifference(3.0);
  refrigerationCondenserWaterCooled.setRatedWaterInletTemperature(11.0);
  refrigerationCondenserWaterCooled.setWaterCooledLoopFlowType("VariableFlow");
  refrigerationCondenserWaterCooled.setWaterDesignFlowRate(0.005);
  refrigerationCondenserWaterCooled.setWaterMaximumFlowRate(0.005);
  refrigerationCondenserWaterCooled.setWaterMaximumWaterOutletTemperature(50.0);
  refrigerationCondenserWaterCooled.setWaterMinimumWaterInletTemperature(15.0);

  auto refrigerationCondenserWaterCooledClone = refrigerationCondenserWaterCooled.clone(m).cast<RefrigerationCondenserWaterCooled>();

  EXPECT_NE(refrigerationCondenserWaterCooledClone.handle(), refrigerationCondenserWaterCooled.handle());

  EXPECT_EQ(52000.0, refrigerationCondenserWaterCooledClone.ratedEffectiveTotalHeatRejectionRate().get());
  EXPECT_EQ(25.0, refrigerationCondenserWaterCooledClone.ratedCondensingTemperature());
  EXPECT_EQ(3.0, refrigerationCondenserWaterCooledClone.ratedSubcoolingTemperatureDifference());
  EXPECT_EQ(11.0, refrigerationCondenserWaterCooledClone.ratedWaterInletTemperature());
  EXPECT_EQ("VariableFlow", refrigerationCondenserWaterCooledClone.waterCooledLoopFlowType());
  EXPECT_EQ(0.005, refrigerationCondenserWaterCooledClone.waterDesignFlowRate().get());
  EXPECT_EQ(0.005, refrigerationCondenserWaterCooledClone.waterMaximumFlowRate().get());
  EXPECT_EQ(50.0, refrigerationCondenserWaterCooledClone.waterMaximumWaterOutletTemperature());
  EXPECT_EQ(15.0, refrigerationCondenserWaterCooledClone.waterMinimumWaterInletTemperature());
}

TEST_F(ModelFixture, RefrigerationCondenserWaterCooled_CloneTwoModelsWithDefaultData) {
  Model m;
  RefrigerationCondenserWaterCooled refrigerationCondenserWaterCooled = RefrigerationCondenserWaterCooled(m);
  auto refrigerationCondenserWaterCooledClone = refrigerationCondenserWaterCooled.clone(m).cast<RefrigerationCondenserWaterCooled>();

  Model m2;
  auto refrigerationCondenserWaterCooledClone2 = refrigerationCondenserWaterCooled.clone(m2).cast<RefrigerationCondenserWaterCooled>();

  EXPECT_NE(refrigerationCondenserWaterCooledClone.handle(), refrigerationCondenserWaterCooled.handle());
  EXPECT_NE(refrigerationCondenserWaterCooledClone2.handle(), refrigerationCondenserWaterCooled.handle());
  EXPECT_NE(refrigerationCondenserWaterCooledClone2.handle(), refrigerationCondenserWaterCooledClone.handle());

  EXPECT_EQ(58000.0, refrigerationCondenserWaterCooledClone2.ratedEffectiveTotalHeatRejectionRate().get());
  EXPECT_EQ(29.4, refrigerationCondenserWaterCooledClone2.ratedCondensingTemperature());
  EXPECT_EQ(0.0, refrigerationCondenserWaterCooledClone2.ratedSubcoolingTemperatureDifference());
  EXPECT_EQ(10.0, refrigerationCondenserWaterCooledClone2.ratedWaterInletTemperature());
  EXPECT_EQ("ConstantFlow", refrigerationCondenserWaterCooledClone2.waterCooledLoopFlowType());
  EXPECT_EQ(0.0025, refrigerationCondenserWaterCooledClone2.waterDesignFlowRate().get());
  EXPECT_EQ(0.003, refrigerationCondenserWaterCooledClone2.waterMaximumFlowRate().get());
  EXPECT_EQ(55.0, refrigerationCondenserWaterCooledClone2.waterMaximumWaterOutletTemperature());
  EXPECT_EQ(10.0, refrigerationCondenserWaterCooledClone2.waterMinimumWaterInletTemperature());
}
