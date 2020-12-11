/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
#include "../CoilCoolingLowTempRadiantVarFlow.hpp"
#include "../CoilCoolingLowTempRadiantVarFlow_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilCoolingLowTempRadiantVarFlow_Check_Constructor) {
  Model model;
  ScheduleConstant coolingControlTemperatureSchedule(model);
  coolingControlTemperatureSchedule.setValue(15.0);

  CoilCoolingLowTempRadiantVarFlow testCoil(model, coolingControlTemperatureSchedule);

  // Test set and get temperature schedule
  EXPECT_TRUE(testCoil.setCoolingControlTemperatureSchedule(coolingControlTemperatureSchedule));
  boost::optional<Schedule> sch1 = testCoil.coolingControlTemperatureSchedule();
  EXPECT_EQ(*sch1, coolingControlTemperatureSchedule);
}

TEST_F(ModelFixture, CoilCoolingLowTempRadiantVarFlow_Getters_Setters) {
  Model model;
  ScheduleConstant coolingControlTemperatureSchedule(model);
  coolingControlTemperatureSchedule.setValue(15.0);

  CoilCoolingLowTempRadiantVarFlow testCoil(model, coolingControlTemperatureSchedule);

  // Field N1 Maximum Hot Water Flow

  EXPECT_TRUE(testCoil.setMaximumColdWaterFlow(100));
  EXPECT_EQ(*testCoil.maximumColdWaterFlow(), 100);
  EXPECT_FALSE(testCoil.isMaximumColdWaterFlowDefaulted());
  EXPECT_FALSE(testCoil.isMaximumColdWaterFlowAutosized());

  testCoil.resetMaximumColdWaterFlow();
  EXPECT_TRUE(testCoil.isMaximumColdWaterFlowDefaulted());

  testCoil.autosizeMaximumColdWaterFlow();
  EXPECT_TRUE(testCoil.isMaximumColdWaterFlowAutosized());

  // Field N2 Cooling Control Throttling Range

  EXPECT_TRUE(testCoil.setCoolingControlThrottlingRange(1.0));
  boost::optional<double> value = testCoil.coolingControlThrottlingRange();
  EXPECT_EQ(*value, 1.0);
  EXPECT_FALSE(testCoil.isCoolingControlThrottlingRangeDefaulted());

  testCoil.resetCoolingControlThrottlingRange();
  value = testCoil.coolingControlThrottlingRange();
  EXPECT_TRUE(testCoil.isCoolingControlThrottlingRangeDefaulted());
  EXPECT_EQ(*value, 0.5);

  // Field A6 Condensation Control Type

  testCoil.setCondensationControlType("VariableOff");
  boost::optional<std::string> str = testCoil.condensationControlType();
  EXPECT_EQ(*str, "VariableOff");
  EXPECT_FALSE(testCoil.isCondensationControlTypeDefaulted());

  testCoil.resetCondensationControlType();
  str = testCoil.condensationControlType();
  EXPECT_EQ(*str, "SimpleOff");
  EXPECT_TRUE(testCoil.isCondensationControlTypeDefaulted());

  // Field N3 Condensation Control Dewpoint Offset

  testCoil.setCondensationControlDewpointOffset(0.5);
  value = testCoil.condensationControlDewpointOffset();
  EXPECT_EQ(*value, 0.5);
  EXPECT_FALSE(testCoil.isCondensationControlDewpointOffsetDefaulted());

  testCoil.resetCondensationControlDewpointOffset();
  value = testCoil.condensationControlDewpointOffset();
  EXPECT_EQ(*value, 1.0);
  EXPECT_TRUE(testCoil.isCondensationControlDewpointOffsetDefaulted());

  EXPECT_EQ("CoolingDesignCapacity", testCoil.coolingDesignCapacityMethod());
  EXPECT_TRUE(testCoil.setCoolingDesignCapacityMethod("CapacityPerFloorArea"));
  EXPECT_EQ("CapacityPerFloorArea", testCoil.coolingDesignCapacityMethod());
  EXPECT_FALSE(testCoil.setCoolingDesignCapacityMethod("BADENUM"));
  EXPECT_EQ("CapacityPerFloorArea", testCoil.coolingDesignCapacityMethod());

  EXPECT_TRUE(testCoil.isCoolingDesignCapacityAutosized());
  EXPECT_TRUE(testCoil.setCoolingDesignCapacity(1000.05));
  EXPECT_FALSE(testCoil.isCoolingDesignCapacityAutosized());
  ASSERT_TRUE(testCoil.coolingDesignCapacity());
  EXPECT_EQ(1000.05, testCoil.coolingDesignCapacity().get());
  testCoil.autosizeCoolingDesignCapacity();
  EXPECT_TRUE(testCoil.isCoolingDesignCapacityAutosized());

  EXPECT_EQ(0, testCoil.coolingDesignCapacityPerFloorArea());
  EXPECT_TRUE(testCoil.setCoolingDesignCapacityPerFloorArea(11.05));
  EXPECT_EQ(11.05, testCoil.coolingDesignCapacityPerFloorArea());

  EXPECT_EQ(1.0, testCoil.fractionofAutosizedCoolingDesignCapacity());
  EXPECT_TRUE(testCoil.setFractionofAutosizedCoolingDesignCapacity(1.05));
  EXPECT_EQ(1.05, testCoil.fractionofAutosizedCoolingDesignCapacity());
  EXPECT_FALSE(testCoil.setFractionofAutosizedCoolingDesignCapacity(-0.05));
  EXPECT_EQ(1.05, testCoil.fractionofAutosizedCoolingDesignCapacity());
}

TEST_F(ModelFixture, CoilCoolingLowTempRadiantVarFlow_addToNode) {
  Model m;
  ScheduleConstant coolingControlTemperatureSchedule(m);
  CoilCoolingLowTempRadiantVarFlow testObject(m, coolingControlTemperatureSchedule);

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

  CoilCoolingLowTempRadiantVarFlow testObjectClone = testObject.clone(m).cast<CoilCoolingLowTempRadiantVarFlow>();
  demandOutletNode = plantLoop.demandOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.demandComponents().size());
}
