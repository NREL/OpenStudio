/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

TEST_F(ModelFixture,CoilHeatingLowTempRadiantVarFlow_Check_Constructor)
{
  Model model;
  ScheduleConstant heatingControlTemperatureSchedule(model);
  heatingControlTemperatureSchedule.setValue(15.0);

  CoilHeatingLowTempRadiantVarFlow testCoil(model,
                                            heatingControlTemperatureSchedule);

  // Test set and get temperature schedule
  EXPECT_TRUE(testCoil.setHeatingControlTemperatureSchedule(heatingControlTemperatureSchedule));
  boost::optional<Schedule> sch1 = testCoil.heatingControlTemperatureSchedule();
  EXPECT_EQ(*sch1,heatingControlTemperatureSchedule);
}

TEST_F(ModelFixture,CoilHeatingLowTempRadiantVarFlow_Getters_Setters)
{
  Model model;
  ScheduleConstant heatingControlTemperatureSchedule(model);
  heatingControlTemperatureSchedule.setValue(15.0);

  CoilHeatingLowTempRadiantVarFlow testCoil(model,
                                            heatingControlTemperatureSchedule);

  // Field N1 Maximum Hot Water Flow

  EXPECT_TRUE(testCoil.setMaximumHotWaterFlow(100));
  EXPECT_EQ(*testCoil.maximumHotWaterFlow(),100);
  EXPECT_FALSE(testCoil.isMaximumHotWaterFlowDefaulted());
  EXPECT_FALSE(testCoil.isMaximumHotWaterFlowAutosized());

  testCoil.resetMaximumHotWaterFlow();
  EXPECT_TRUE(testCoil.isMaximumHotWaterFlowDefaulted());

  testCoil.autosizeMaximumHotWaterFlow();
  EXPECT_TRUE(testCoil.isMaximumHotWaterFlowAutosized());

  // Field N2 Heating Control Throttling Range

  EXPECT_TRUE(testCoil.setHeatingControlThrottlingRange(1.0));
  boost::optional<double> value = testCoil.heatingControlThrottlingRange();
  EXPECT_EQ(*value,1.0);
  EXPECT_FALSE(testCoil.isHeatingControlThrottlingRangeDefaulted());

  testCoil.resetHeatingControlThrottlingRange();
  value = testCoil.heatingControlThrottlingRange();
  EXPECT_TRUE(testCoil.isHeatingControlThrottlingRangeDefaulted());
  EXPECT_EQ(*value,0.5);
}

TEST_F(ModelFixture,CoilHeatingLowTempRadiantVarFlow_addToNode) {
  Model m;
  ScheduleConstant heatingControlTemperatureSchedule(m);

  CoilHeatingLowTempRadiantVarFlow testObject(m, heatingControlTemperatureSchedule);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)7, plantLoop.demandComponents().size() );

  CoilHeatingLowTempRadiantVarFlow testObjectClone = testObject.clone(m).cast<CoilHeatingLowTempRadiantVarFlow>();
  demandOutletNode = plantLoop.demandOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.demandComponents().size() );
}
