/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../CoilCoolingLowTempRadiantConstFlow.hpp"
#include "../CoilCoolingLowTempRadiantConstFlow_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingLowTempRadiantConstFlow_SetGetFields) {
  Model model;
  ScheduleConstant coolingHighWaterTempSched(model);
  ScheduleConstant coolingLowWaterTempSched(model);
  ScheduleConstant coolingHighControlTempSched(model);
  ScheduleConstant coolingLowControlTempSched(model);

  coolingHighWaterTempSched.setValue(15.0);
  coolingLowWaterTempSched.setValue(10.0);
  coolingHighControlTempSched.setValue(26.0);
  coolingLowControlTempSched.setValue(22.0);

  CoilCoolingLowTempRadiantConstFlow testCC(model, coolingHighWaterTempSched, coolingLowWaterTempSched, coolingHighControlTempSched,
                                            coolingLowControlTempSched);

  EXPECT_TRUE(testCC.setCoolingHighWaterTemperatureSchedule(coolingHighWaterTempSched));
  boost::optional<Schedule> sch1 = testCC.coolingHighWaterTemperatureSchedule();
  EXPECT_EQ(*sch1, coolingHighWaterTempSched);

  EXPECT_TRUE(testCC.setCoolingLowWaterTemperatureSchedule(coolingLowWaterTempSched));
  boost::optional<Schedule> sch2 = testCC.coolingLowWaterTemperatureSchedule();
  EXPECT_EQ(*sch2, coolingLowWaterTempSched);

  EXPECT_TRUE(testCC.setCoolingHighControlTemperatureSchedule(coolingHighControlTempSched));
  boost::optional<Schedule> sch3 = testCC.coolingHighControlTemperatureSchedule();
  EXPECT_EQ(*sch3, coolingHighControlTempSched);

  EXPECT_TRUE(testCC.setCoolingLowControlTemperatureSchedule(coolingLowControlTempSched));
  boost::optional<Schedule> sch4 = testCC.coolingLowControlTemperatureSchedule();
  EXPECT_EQ(*sch4, coolingLowControlTempSched);

  testCC.setCondensationControlType("Off");
  boost::optional<std::string> str1 = testCC.condensationControlType();
  EXPECT_EQ(*str1, "Off");
  EXPECT_FALSE(testCC.isCondensationControlTypeDefaulted());
  testCC.resetCondensationControlType();
  EXPECT_TRUE(testCC.isCondensationControlTypeDefaulted());

  testCC.setCondensationControlDewpointOffset(2.0);
  boost::optional<double> value1 = testCC.condensationControlDewpointOffset();
  EXPECT_EQ(*value1, 2.0);
  EXPECT_FALSE(testCC.isCondensationControlDewpointOffsetDefaulted());
  testCC.resetCondensationControlDewpointOffset();
  EXPECT_TRUE(testCC.isCondensationControlDewpointOffsetDefaulted());
  //boost::optional<double> value2 = testCC.condensationControlDewpointOffset();
  //EXPECT_TRUE(*value2,1.0);
}

TEST_F(ModelFixture, CoilCoolingLowTempRadiantConstFlow_addToNode) {
  Model m;
  ScheduleConstant coolingHighWaterTempSched(m);
  ScheduleConstant coolingLowWaterTempSched(m);
  ScheduleConstant coolingHighControlTempSched(m);
  ScheduleConstant coolingLowControlTempSched(m);

  CoilCoolingLowTempRadiantConstFlow testObject(m, coolingHighWaterTempSched, coolingLowWaterTempSched, coolingHighControlTempSched,
                                                coolingLowControlTempSched);

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

  auto testObjectClone = testObject.clone(m).cast<CoilCoolingLowTempRadiantConstFlow>();
  demandOutletNode = plantLoop.demandOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.demandComponents().size());
}
