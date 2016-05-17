/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
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

