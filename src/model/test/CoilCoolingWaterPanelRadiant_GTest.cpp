/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../CoilCoolingWaterPanelRadiant.hpp"
#include "../CoilCoolingWaterPanelRadiant_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Splitter.hpp"
#include "../Splitter_Impl.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingWaterPanelRadiant_addToNode) {
  Model m;
  CoilCoolingWaterPanelRadiant testObject(m);

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

  CoilCoolingWaterPanelRadiant testObject2(m);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(testObject2));
  EXPECT_EQ(9u, plantLoop.demandComponents().size());
}

TEST_F(ModelFixture, CoilCoolingWaterPanelRadiant_SetGetFields) {
  Model m;
  CoilCoolingWaterPanelRadiant coil(m);

  EXPECT_EQ(5.0, coil.ratedInletWaterTemperature());
  EXPECT_EQ(24.0, coil.ratedInletSpaceTemperature());
  EXPECT_EQ(0.063, coil.ratedWaterMassFlowRate());
  EXPECT_EQ("CoolingDesignCapacity", coil.coolingDesignCapacityMethod());
  EXPECT_FALSE(coil.coolingDesignCapacity());
  EXPECT_TRUE(coil.isCoolingDesignCapacityAutosized());
  EXPECT_EQ(0.0, coil.coolingDesignCapacityPerFloorArea());
  EXPECT_EQ(1.0, coil.fractionofAutosizedCoolingDesignCapacity());
  EXPECT_FALSE(coil.maximumChilledWaterFlowRate());
  EXPECT_TRUE(coil.isMaximumChilledWaterFlowRateAutosized());
  EXPECT_EQ("MeanAirTemperature", coil.controlType());
  EXPECT_EQ(0.5, coil.coolingControlThrottlingRange());
  EXPECT_FALSE(coil.coolingControlTemperatureSchedule());
  EXPECT_EQ("SimpleOff", coil.condensationControlType());
  EXPECT_EQ(1.0, coil.condensationControlDewpointOffset());

  EXPECT_TRUE(coil.setRatedInletWaterTemperature(6.5));
  EXPECT_TRUE(coil.setRatedInletSpaceTemperature(25.5));
  EXPECT_TRUE(coil.setRatedWaterMassFlowRate(0.08));
  EXPECT_TRUE(coil.setCoolingDesignCapacityMethod("CapacityPerFloorArea"));
  EXPECT_TRUE(coil.setCoolingDesignCapacity(10.0));
  EXPECT_TRUE(coil.setCoolingDesignCapacityPerFloorArea(0.5));
  EXPECT_TRUE(coil.setFractionofAutosizedCoolingDesignCapacity(1.5));
  EXPECT_TRUE(coil.setMaximumChilledWaterFlowRate(5.0));
  EXPECT_TRUE(coil.setControlType("MeanRadiantTemperature"));
  EXPECT_TRUE(coil.setCoolingControlThrottlingRange(0.7));
  ScheduleConstant sched(m);
  sched.setValue(0.5);
  EXPECT_TRUE(coil.setCoolingControlTemperatureSchedule(sched));
  EXPECT_TRUE(coil.setCondensationControlType("VariableOff"));
  EXPECT_TRUE(coil.setCondensationControlDewpointOffset(1.25));

  EXPECT_EQ(6.5, coil.ratedInletWaterTemperature());
  EXPECT_EQ(25.5, coil.ratedInletSpaceTemperature());
  EXPECT_EQ(0.08, coil.ratedWaterMassFlowRate());
  EXPECT_EQ("CapacityPerFloorArea", coil.coolingDesignCapacityMethod());
  EXPECT_TRUE(coil.coolingDesignCapacity());
  EXPECT_EQ(10.0, coil.coolingDesignCapacity().get());
  EXPECT_FALSE(coil.isCoolingDesignCapacityAutosized());
  EXPECT_EQ(0.5, coil.coolingDesignCapacityPerFloorArea());
  EXPECT_EQ(1.5, coil.fractionofAutosizedCoolingDesignCapacity());
  EXPECT_TRUE(coil.maximumChilledWaterFlowRate());
  EXPECT_EQ(5.0, coil.maximumChilledWaterFlowRate().get());
  EXPECT_FALSE(coil.isMaximumChilledWaterFlowRateAutosized());
  EXPECT_EQ("MeanRadiantTemperature", coil.controlType());
  EXPECT_EQ(0.7, coil.coolingControlThrottlingRange());
  EXPECT_TRUE(coil.coolingControlTemperatureSchedule());
  EXPECT_EQ("VariableOff", coil.condensationControlType());
  EXPECT_EQ(1.25, coil.condensationControlDewpointOffset());

  coil.resetCoolingControlTemperatureSchedule();
  EXPECT_FALSE(coil.coolingControlTemperatureSchedule());
}
