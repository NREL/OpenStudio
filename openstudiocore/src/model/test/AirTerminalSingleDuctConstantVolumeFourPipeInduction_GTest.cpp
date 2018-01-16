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
#include "../AirTerminalSingleDuctConstantVolumeFourPipeInduction.hpp"
#include "../AirTerminalSingleDuctConstantVolumeFourPipeInduction_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilCoolingWater.hpp"
#include "../Schedule.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../PortList.hpp"
#include "../PortList_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeFourPipeInduction_AirTerminalSingleDuctConstantVolumeFourPipeInduction)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    Schedule s = m.alwaysOnDiscreteSchedule();
    CoilHeatingWater heatingCoil(m,s);
    AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m,heatingCoil);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeFourPipeInduction_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m,s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m,heatingCoil);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)2, airLoop.supplyComponents().size() );

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.supplyComponents().size() );

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ( (unsigned)5, plantLoop.demandComponents().size() );

  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObjectClone.addToNode(inletNode));
  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObjectClone));
  EXPECT_EQ( (unsigned)10, airLoop.demandComponents().size() );
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_remove)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m,s);
  CoilCoolingWater coolingCoil(m,s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m,heatingCoil);

  testObject.setCoolingCoil(coolingCoil);

  AirLoopHVAC airLoop(m);
  ThermalZone thermalZone(m);
  PlantLoop plantLoop(m);

  // KSB: I don't think it is the greatest idea to test these private methods,
  // but this area has resulted in a simulation error so it needs to be tested
  EXPECT_FALSE(thermalZone.getImpl<detail::ThermalZone_Impl>()->exhaustPortList().getTarget(3));
  EXPECT_FALSE(thermalZone.getImpl<detail::ThermalZone_Impl>()->inletPortList().getTarget(3));

  airLoop.addBranchForZone(thermalZone, testObject);
  plantLoop.addDemandBranchForComponent(heatingCoil);
  plantLoop.addDemandBranchForComponent(coolingCoil);

  EXPECT_TRUE(thermalZone.getImpl<detail::ThermalZone_Impl>()->exhaustPortList().getTarget(3));
  EXPECT_TRUE(thermalZone.getImpl<detail::ThermalZone_Impl>()->inletPortList().getTarget(3));

  EXPECT_EQ((unsigned)10, plantLoop.demandComponents().size());
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());

  testObject.remove();

  EXPECT_FALSE(thermalZone.getImpl<detail::ThermalZone_Impl>()->exhaustPortList().getTarget(3));
  EXPECT_TRUE(thermalZone.getImpl<detail::ThermalZone_Impl>()->inletPortList().getTarget(3));

  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeFourPipeInduction_clone)
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingWater heatingCoil(m,s);
  CoilCoolingWater coolingCoil(m,s);
  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObject(m,heatingCoil);

  testObject.setCoolingCoil(coolingCoil);

  AirLoopHVAC airLoop(m);
  ThermalZone thermalZone(m);
  PlantLoop plantLoop(m);

  airLoop.addBranchForZone(thermalZone, testObject);
  plantLoop.addDemandBranchForComponent(heatingCoil);
  plantLoop.addDemandBranchForComponent(coolingCoil);

  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
  ASSERT_NO_THROW(testObjectClone.heatingCoil());
  ASSERT_TRUE(testObjectClone.coolingCoil());
  EXPECT_FALSE(testObjectClone.airLoopHVAC());

  EXPECT_NE(testObjectClone.heatingCoil(),testObject.heatingCoil());
  EXPECT_NE(testObjectClone.coolingCoil().get(),testObject.coolingCoil().get());

  Model m2;

  AirTerminalSingleDuctConstantVolumeFourPipeInduction testObjectClone2 = testObject.clone(m2).cast<AirTerminalSingleDuctConstantVolumeFourPipeInduction>();
  ASSERT_NO_THROW(testObjectClone2.heatingCoil());
  ASSERT_TRUE(testObjectClone2.coolingCoil());
  EXPECT_FALSE(testObjectClone.airLoopHVAC());

  EXPECT_NE(testObjectClone2.heatingCoil(),testObject.heatingCoil());
  EXPECT_NE(testObjectClone2.coolingCoil().get(),testObject.coolingCoil().get());
}
