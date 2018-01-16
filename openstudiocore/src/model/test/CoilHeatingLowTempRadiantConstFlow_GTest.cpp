/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../ScheduleConstant.hpp"
#include "../ScheduleConstant_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

#include "../CoilHeatingLowTempRadiantConstFlow.hpp"
#include "../CoilHeatingLowTempRadiantConstFlow_Impl.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,CoilHeatingLowTempRadiantConstFlow_SetGetFields) {
  Model model;
  ScheduleConstant heatingHighWaterTempSched(model);
  ScheduleConstant heatingLowWaterTempSched(model);
  ScheduleConstant heatingHighControlTempSched(model);
  ScheduleConstant heatingLowControlTempSched(model);

  CoilHeatingLowTempRadiantConstFlow testHC(model,
                                            heatingHighWaterTempSched,
                                            heatingLowWaterTempSched,
                                            heatingHighControlTempSched,
                                            heatingLowControlTempSched);
  heatingHighWaterTempSched.setValue(50.0);
  EXPECT_TRUE(testHC.setHeatingHighWaterTemperatureSchedule(heatingHighWaterTempSched));
  boost::optional<Schedule> sch1 = testHC.heatingHighWaterTemperatureSchedule();
  EXPECT_EQ(*sch1,heatingHighWaterTempSched);

  heatingLowWaterTempSched.setValue(30.0);
  EXPECT_TRUE(testHC.setHeatingLowWaterTemperatureSchedule(heatingLowWaterTempSched));
  boost::optional<Schedule> sch2 = testHC.heatingLowWaterTemperatureSchedule();
  EXPECT_EQ(*sch2,heatingLowWaterTempSched);

  heatingHighControlTempSched.setValue(21.0);
  EXPECT_TRUE(testHC.setHeatingHighControlTemperatureSchedule(heatingHighControlTempSched));
  boost::optional<Schedule> sch3 = testHC.heatingHighControlTemperatureSchedule();
  EXPECT_EQ(*sch3,heatingHighControlTempSched);

  heatingLowControlTempSched.setValue(15.0);
  EXPECT_TRUE(testHC.setHeatingLowControlTemperatureSchedule(heatingLowControlTempSched));
  boost::optional<Schedule> sch4 = testHC.heatingLowControlTemperatureSchedule();
  EXPECT_EQ(*sch4,heatingLowControlTempSched);

}

TEST_F(ModelFixture,CoilHeatingLowTempRadiantConstFlow_addToNode) {
  Model m;
  ScheduleConstant heatingHighWaterTempSched(m);
  ScheduleConstant heatingLowWaterTempSched(m);
  ScheduleConstant heatingHighControlTempSched(m);
  ScheduleConstant heatingLowControlTempSched(m);

  CoilHeatingLowTempRadiantConstFlow testObject(m,
                                                heatingHighWaterTempSched,
                                                heatingLowWaterTempSched,
                                                heatingHighControlTempSched,
                                                heatingLowControlTempSched);

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

  CoilHeatingLowTempRadiantConstFlow testObjectClone = testObject.clone(m).cast<CoilHeatingLowTempRadiantConstFlow>();
  demandOutletNode = plantLoop.demandOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)9, plantLoop.demandComponents().size() );
}
