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
