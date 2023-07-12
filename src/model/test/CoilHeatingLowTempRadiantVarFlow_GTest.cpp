/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

#include "../CoilHeatingLowTempRadiantVarFlow.hpp"
#include "../CoilHeatingLowTempRadiantVarFlow_Impl.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingLowTempRadiantVarFlow_Check_Constructor) {
  Model model;
  ScheduleConstant heatingControlTemperatureSchedule(model);
  heatingControlTemperatureSchedule.setValue(15.0);

  CoilHeatingLowTempRadiantVarFlow testCoil(model, heatingControlTemperatureSchedule);

  // Test set and get temperature schedule
  EXPECT_TRUE(testCoil.setHeatingControlTemperatureSchedule(heatingControlTemperatureSchedule));
  boost::optional<Schedule> sch1 = testCoil.heatingControlTemperatureSchedule();
  EXPECT_EQ(*sch1, heatingControlTemperatureSchedule);
}

TEST_F(ModelFixture, CoilHeatingLowTempRadiantVarFlow_Getters_Setters) {
  Model model;
  ScheduleConstant heatingControlTemperatureSchedule(model);
  heatingControlTemperatureSchedule.setValue(15.0);

  CoilHeatingLowTempRadiantVarFlow testCoil(model, heatingControlTemperatureSchedule);

  // Field N1 Maximum Hot Water Flow

  EXPECT_TRUE(testCoil.setMaximumHotWaterFlow(100));
  EXPECT_EQ(*testCoil.maximumHotWaterFlow(), 100);
  EXPECT_FALSE(testCoil.isMaximumHotWaterFlowDefaulted());
  EXPECT_FALSE(testCoil.isMaximumHotWaterFlowAutosized());

  testCoil.resetMaximumHotWaterFlow();
  EXPECT_TRUE(testCoil.isMaximumHotWaterFlowDefaulted());

  testCoil.autosizeMaximumHotWaterFlow();
  EXPECT_TRUE(testCoil.isMaximumHotWaterFlowAutosized());

  // Field N2 Heating Control Throttling Range

  EXPECT_TRUE(testCoil.setHeatingControlThrottlingRange(1.0));
  boost::optional<double> value = testCoil.heatingControlThrottlingRange();
  EXPECT_EQ(*value, 1.0);
  EXPECT_FALSE(testCoil.isHeatingControlThrottlingRangeDefaulted());

  testCoil.resetHeatingControlThrottlingRange();
  value = testCoil.heatingControlThrottlingRange();
  EXPECT_TRUE(testCoil.isHeatingControlThrottlingRangeDefaulted());
  EXPECT_EQ(*value, 0.5);

  EXPECT_EQ("HeatingDesignCapacity", testCoil.heatingDesignCapacityMethod());
  EXPECT_TRUE(testCoil.setHeatingDesignCapacityMethod("CapacityPerFloorArea"));
  EXPECT_EQ("CapacityPerFloorArea", testCoil.heatingDesignCapacityMethod());
  EXPECT_FALSE(testCoil.setHeatingDesignCapacityMethod("BADENUM"));
  EXPECT_EQ("CapacityPerFloorArea", testCoil.heatingDesignCapacityMethod());

  EXPECT_TRUE(testCoil.isHeatingDesignCapacityAutosized());
  EXPECT_TRUE(testCoil.setHeatingDesignCapacity(1000.05));
  EXPECT_FALSE(testCoil.isHeatingDesignCapacityAutosized());
  ASSERT_TRUE(testCoil.heatingDesignCapacity());
  EXPECT_EQ(1000.05, testCoil.heatingDesignCapacity().get());
  testCoil.autosizeHeatingDesignCapacity();
  EXPECT_TRUE(testCoil.isHeatingDesignCapacityAutosized());

  EXPECT_EQ(0, testCoil.heatingDesignCapacityPerFloorArea());
  EXPECT_TRUE(testCoil.setHeatingDesignCapacityPerFloorArea(11.05));
  EXPECT_EQ(11.05, testCoil.heatingDesignCapacityPerFloorArea());

  EXPECT_EQ(1.0, testCoil.fractionofAutosizedHeatingDesignCapacity());
  EXPECT_TRUE(testCoil.setFractionofAutosizedHeatingDesignCapacity(1.05));
  EXPECT_EQ(1.05, testCoil.fractionofAutosizedHeatingDesignCapacity());
  EXPECT_FALSE(testCoil.setFractionofAutosizedHeatingDesignCapacity(-0.05));
  EXPECT_EQ(1.05, testCoil.fractionofAutosizedHeatingDesignCapacity());
}

TEST_F(ModelFixture, CoilHeatingLowTempRadiantVarFlow_addToNode) {
  Model m;
  ScheduleConstant heatingControlTemperatureSchedule(m);

  CoilHeatingLowTempRadiantVarFlow testObject(m, heatingControlTemperatureSchedule);

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

  auto testObjectClone = testObject.clone(m).cast<CoilHeatingLowTempRadiantVarFlow>();
  demandOutletNode = plantLoop.demandOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.demandComponents().size());
}
