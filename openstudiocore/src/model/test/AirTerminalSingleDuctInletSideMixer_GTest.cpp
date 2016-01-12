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
#include "../AirTerminalSingleDuctInletSideMixer.hpp"
#include "../AirTerminalSingleDuctInletSideMixer_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../ThermalZone.hpp"
#include "../DesignSpecificationOutdoorAir.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);
    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,AirTerminalSingleDuctInletSideMixer_addToNode) {
  Model m; 
  AirTerminalSingleDuctInletSideMixer testObject(m);

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

  AirTerminalSingleDuctInletSideMixer testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctInletSideMixer>();
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObjectClone.addToNode(inletNode));
  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObjectClone));
  EXPECT_EQ( (unsigned)10, airLoop.demandComponents().size() );
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddToNodeWithThermalZone)
{
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);
  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddAirTerminalToPlantLoopAddDemandBranchForComponent)
{
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);
  PlantLoop plantLoop(model);

  airLoop.addBranchForZone(thermalZone);
  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  testObject.addToNode(inletNode);

  EXPECT_FALSE(plantLoop.addDemandBranchForComponent(testObject));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddObjectByAirLoopAddBranchForZoneWithThermalZone)
{
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  EXPECT_TRUE(airLoop.addBranchForZone(thermalZone, testObject));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddObjectByAirLoopAddBranchForHVACComponent)
{
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);

  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObject));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddObjectByAirLoopAddBranchForHVACComponentWithThermalZone)
{
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);

  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObject));
  EXPECT_EQ((unsigned)10, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_AddToNodeTwoSameObjects)
{
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(testObject.addToNode(inletNode));
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_IsRemovable)
{
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  EXPECT_TRUE(testObject.isRemovable());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_Remove)
{
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);

  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctInletSideMixer_RemoveObjectWithThermalZone)
{
  Model model;
  AirTerminalSingleDuctInletSideMixer testObject = AirTerminalSingleDuctInletSideMixer(model);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);

  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
}


