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
#include "../AirTerminalSingleDuctVAVReheat.hpp"
#include "../AirTerminalSingleDuctVAVReheat_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../BoilerHotWater.hpp"
#include "../BoilerHotWater_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../Schedule.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../HVACTemplates.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../PlantLoop.hpp"
#include "../PlantLoop_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingWater_Impl.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture,AirTerminalSingleDuctVAVReheat_AirTerminalSingleDuctVAVReheat)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    Schedule s = m.alwaysOnDiscreteSchedule();
    CoilHeatingElectric coil = CoilHeatingElectric(m,s);
    AirTerminalSingleDuctVAVReheat testObject(m,s,coil);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,AirTerminalSingleDuctVAVReheat_addToNode) {
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  CoilHeatingElectric coil = CoilHeatingElectric(m,s);
  AirTerminalSingleDuctVAVReheat testObject(m,s,coil);

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

  AirTerminalSingleDuctVAVReheat testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctVAVReheat>();
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObjectClone.addToNode(inletNode));
  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObjectClone));
  EXPECT_EQ( (unsigned)10, airLoop.demandComponents().size() );
}

TEST_F(ModelFixture,AirTerminalSingleDuctVAVReheat_remove) {
  Model m;

  auto airLoopHVAC = addSystemType7(m).cast<AirLoopHVAC>();

  ThermalZone zone1(m);
  ThermalZone zone2(m);
  ThermalZone zone3(m);

  airLoopHVAC.addBranchForZone(zone1);
  airLoopHVAC.addBranchForZone(zone2);
  airLoopHVAC.addBranchForZone(zone3);

  auto terminal1 = zone1.airLoopHVACTerminal();
  EXPECT_TRUE(terminal1);
  auto terminal2 = zone2.airLoopHVACTerminal();
  EXPECT_TRUE(terminal2);
  auto terminal3 = zone3.airLoopHVACTerminal();
  EXPECT_TRUE(terminal3);

  auto coil1 = terminal1->cast<AirTerminalSingleDuctVAVReheat>().reheatCoil();
  auto coil2 = terminal2->cast<AirTerminalSingleDuctVAVReheat>().reheatCoil();
  auto coil3 = terminal3->cast<AirTerminalSingleDuctVAVReheat>().reheatCoil();

  auto terminals = airLoopHVAC.demandComponents(AirTerminalSingleDuctVAVReheat::iddObjectType());
  EXPECT_EQ(3u,terminals.size());

  auto boiler = m.getModelObjects<BoilerHotWater>().front();
  auto plant = boiler.plantLoop().get();
  auto coils = plant.demandComponents(CoilHeatingWater::iddObjectType());
  EXPECT_EQ(4u,coils.size());

  airLoopHVAC.removeBranchForZone(zone1);
  airLoopHVAC.removeBranchForZone(zone2);
  terminal3->remove();

  coils = plant.demandComponents(CoilHeatingWater::iddObjectType());
  EXPECT_EQ(1u,coils.size());

  EXPECT_TRUE(terminal1->handle().isNull());
  EXPECT_TRUE(terminal2->handle().isNull());
  EXPECT_TRUE(terminal3->handle().isNull());

  EXPECT_TRUE(coil1.handle().isNull());
  EXPECT_TRUE(coil2.handle().isNull());
  EXPECT_TRUE(coil3.handle().isNull());
}

