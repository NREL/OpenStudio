/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
