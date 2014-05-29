/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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
#include "../AirTerminalSingleDuctVAVNoReheat.hpp"
#include "../AirTerminalSingleDuctVAVNoReheat_Impl.hpp"
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

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    Schedule schedule = model.alwaysOnDiscreteSchedule();
    
    AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,AirTerminalSingleDuctVAVNoReheat_addToNode) {
  Model m; 
  Schedule s = m.alwaysOnDiscreteSchedule();
  AirTerminalSingleDuctVAVNoReheat testObject(m,s);

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

  AirTerminalSingleDuctVAVNoReheat testObjectClone = testObject.clone(m).cast<AirTerminalSingleDuctVAVNoReheat>();
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObjectClone.addToNode(inletNode));
  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObjectClone));
  EXPECT_EQ( (unsigned)10, airLoop.demandComponents().size() );
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_AddToNodeWithThermalZone)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);
  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_AddAirTerminalToPlantLoopAddDemandBranchForComponent)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();

  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);
  PlantLoop plantLoop(model);

  airLoop.addBranchForZone(thermalZone);
  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  testObject.addToNode(inletNode);

  EXPECT_FALSE(plantLoop.addDemandBranchForComponent(testObject));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());
  EXPECT_NE((unsigned)7, airLoop.demandComponents().size());
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
  EXPECT_NE((unsigned)7, plantLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_AddObjectByAirLoopAddBranchForZoneWithThermalZone)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  EXPECT_TRUE(airLoop.addBranchForZone(thermalZone, testObject));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());
  EXPECT_NE((unsigned)7, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_AddObjectByAirLoopAddBranchForHVACComponent)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

  AirLoopHVAC airLoop(model);

  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObject));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
  EXPECT_NE((unsigned)9, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_AddObjectByAirLoopAddBranchForHVACComponentWithThermalZone)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);

  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObject));
  EXPECT_EQ((unsigned)10, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_AddToNodeTwoSameObjects)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

  AirLoopHVAC airLoop(model);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(testObject.addToNode(inletNode));
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_IsRemovable)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

  EXPECT_TRUE(testObject.isRemovable());
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_Remove)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

  AirLoopHVAC airLoop(model);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);

  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_RemoveObjectWithThermalZone)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);

  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_CloneOneModelWithDefaultData)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

  AirTerminalSingleDuctVAVNoReheat testObjectClone = testObject.clone(model).cast<AirTerminalSingleDuctVAVNoReheat>();
  EXPECT_TRUE(testObjectClone.isMaximumAirFlowRateAutosized());
  EXPECT_EQ("Constant", testObjectClone.zoneMinimumAirFlowInputMethod().get());
  EXPECT_DOUBLE_EQ(0.3, testObjectClone.constantMinimumAirFlowFraction().get());
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_CloneOneModelWithCustomData)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  Schedule minAirFlowSchedule = model.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);
  testObject.setMaximumAirFlowRate(999.0);
  testObject.setZoneMinimumAirFlowInputMethod("Scheduled");
  testObject.setConstantMinimumAirFlowFraction(999.0);
  testObject.setFixedMinimumAirFlowRate(1.0);
  testObject.setMinimumAirFlowFractionSchedule(minAirFlowSchedule);

  AirTerminalSingleDuctVAVNoReheat testObjectClone = testObject.clone(model).cast<AirTerminalSingleDuctVAVNoReheat>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.maximumAirFlowRate().get());
  EXPECT_EQ("Scheduled", testObjectClone.zoneMinimumAirFlowInputMethod().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.constantMinimumAirFlowFraction().get());
  EXPECT_DOUBLE_EQ(1.0, testObjectClone.fixedMinimumAirFlowRate().get());
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_CloneTwoModelsWithDefaultData)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(model,schedule);

  AirTerminalSingleDuctVAVNoReheat testObjectClone = testObject.clone(model).cast<AirTerminalSingleDuctVAVNoReheat>();

  Model model2;

  AirTerminalSingleDuctVAVNoReheat testObjectClone2 = testObject.clone(model2).cast<AirTerminalSingleDuctVAVNoReheat>();
  EXPECT_TRUE(testObjectClone2.isMaximumAirFlowRateAutosized());
  EXPECT_EQ("Constant", testObjectClone2.zoneMinimumAirFlowInputMethod().get());
  EXPECT_DOUBLE_EQ(0.3, testObjectClone2.constantMinimumAirFlowFraction().get());
  EXPECT_NE(testObjectClone2, testObjectClone);
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_MaximumAirFlowRate) 
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(m,s);

  EXPECT_TRUE(testObject.isMaximumAirFlowRateAutosized());

  testObject.setMaximumAirFlowRate(999.0);
  EXPECT_DOUBLE_EQ(999.0, testObject.maximumAirFlowRate().get());

  testObject.setMaximumAirFlowRate(0.0);
  EXPECT_DOUBLE_EQ(0.0, testObject.maximumAirFlowRate().get());

  EXPECT_FALSE(testObject.setMaximumAirFlowRate(-1.0));

  testObject.resetMaximumAirFlowRate();
  EXPECT_FALSE(testObject.isMaximumAirFlowRateAutosized());

  testObject.autosizeMaximumAirFlowRate();
  EXPECT_TRUE(testObject.isMaximumAirFlowRateAutosized());
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_ZoneMinimumAirFlowInputMethod) 
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(m,s);

  EXPECT_EQ("Constant", testObject.zoneMinimumAirFlowInputMethod().get());

  EXPECT_TRUE(testObject.setZoneMinimumAirFlowInputMethod("FixedFlowRate"));
  EXPECT_EQ("FixedFlowRate", testObject.zoneMinimumAirFlowInputMethod().get());

  EXPECT_TRUE(testObject.setZoneMinimumAirFlowInputMethod("Scheduled"));
  EXPECT_EQ("Scheduled", testObject.zoneMinimumAirFlowInputMethod().get());

  EXPECT_FALSE(testObject.setZoneMinimumAirFlowInputMethod("Not Valid"));
  EXPECT_EQ("Scheduled", testObject.zoneMinimumAirFlowInputMethod().get());

  testObject.resetZoneMinimumAirFlowInputMethod();
  EXPECT_EQ("", testObject.zoneMinimumAirFlowInputMethod().get());
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_ConstantMinimumAirFlowFraction) 
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(m,s);

  EXPECT_DOUBLE_EQ(0.3, testObject.constantMinimumAirFlowFraction().get());

  testObject.setConstantMinimumAirFlowFraction(999.0);
  EXPECT_DOUBLE_EQ(999.0, testObject.constantMinimumAirFlowFraction().get());

  testObject.setConstantMinimumAirFlowFraction(0.0);
  EXPECT_DOUBLE_EQ(0.0, testObject.constantMinimumAirFlowFraction().get());

  EXPECT_TRUE(testObject.setConstantMinimumAirFlowFraction(-1.0));

  testObject.resetConstantMinimumAirFlowFraction();
  EXPECT_TRUE(testObject.isConstantMinimumAirFlowFractionDefaulted());
  EXPECT_DOUBLE_EQ(0.3, testObject.constantMinimumAirFlowFraction().get());
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_FixedMinimumAirFlowRate) 
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(m,s);

  EXPECT_TRUE(testObject.isFixedMinimumAirFlowRateDefaulted());

  testObject.setFixedMinimumAirFlowRate(999.0);
  EXPECT_DOUBLE_EQ(999.0, testObject.fixedMinimumAirFlowRate().get());

  testObject.setFixedMinimumAirFlowRate(0.0);
  EXPECT_DOUBLE_EQ(0.0, testObject.fixedMinimumAirFlowRate().get());

  EXPECT_TRUE(testObject.setFixedMinimumAirFlowRate(-1.0));

  testObject.resetFixedMinimumAirFlowRate();
  EXPECT_TRUE(testObject.isFixedMinimumAirFlowRateDefaulted());
  EXPECT_DOUBLE_EQ(0.0, testObject.fixedMinimumAirFlowRate().get());
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_MinimumAirFlowFractionSchedule) 
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(m,s);

  ScheduleCompact alwaysOnSchedule(m);
  alwaysOnSchedule.setName("ALWAYS_ON");
  alwaysOnSchedule.setString(3,"Through: 12/31");
  alwaysOnSchedule.setString(4,"For: AllDays");
  alwaysOnSchedule.setString(5,"Until: 24:00");
  alwaysOnSchedule.setString(6,"1");

  EXPECT_FALSE(testObject.minimumAirFlowFractionSchedule());
  EXPECT_TRUE(testObject.setMinimumAirFlowFractionSchedule(alwaysOnSchedule));
  EXPECT_TRUE(testObject.minimumAirFlowFractionSchedule());
  EXPECT_EQ(alwaysOnSchedule, testObject.minimumAirFlowFractionSchedule().get());
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVNoReheat_DesignSpecificationOutdoorAirObject) 
{
  Model m;
  Schedule s = m.alwaysOnDiscreteSchedule();
  DesignSpecificationOutdoorAir oa = DesignSpecificationOutdoorAir(m);
  
  AirTerminalSingleDuctVAVNoReheat testObject = AirTerminalSingleDuctVAVNoReheat(m,s);

  EXPECT_FALSE(testObject.designSpecificationOutdoorAirObject());
  EXPECT_TRUE(testObject.setDesignSpecificationOutdoorAirObject(oa));
  EXPECT_TRUE(testObject.designSpecificationOutdoorAirObject());
  EXPECT_EQ(oa, testObject.designSpecificationOutdoorAirObject().get());
}

