/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>

#include <model/AirTerminalSingleDuctConstantVolumeReheat.hpp>
#include <model/AirTerminalSingleDuctConstantVolumeReheat_Impl.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/ScheduleCompact.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/CoilHeatingElectric.hpp>
#include <model/CoilHeatingGas.hpp>
#include <model/CoilHeatingWater.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/PlantLoop.hpp>
#include <model/PlantLoop_Impl.hpp>
#include <model/ConnectorMixer.hpp>
#include <model/ConnectorMixer_Impl.hpp>
#include <model/ConnectorSplitter.hpp>
#include <model/ConnectorSplitter_Impl.hpp>
#include <model/Splitter.hpp>
#include <model/Splitter_Impl.hpp>

//using namespace openstudio;
using namespace openstudio::model; 

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_DefaultConstructorWithCoilHeatingElectric)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    ScheduleCompact schedule(model);
    CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
    AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_DefaultConstructorWithCoilHeatingGas)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    ScheduleCompact schedule(model);
    CoilHeatingGas coil = CoilHeatingGas(model,schedule);
    AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_DefaultConstructorWithCoilHeatingWater)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model model;
    ScheduleCompact schedule(model);
    CoilHeatingWater coil = CoilHeatingWater(model,schedule);
    AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_AddToNode)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));

  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_AddToNodeAirLoopSupplySide)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());  
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_AddToNodePlantLoop)
{
  Model model;
  Schedule schedule = model.alwaysOnDiscreteSchedule();
  CoilHeatingWater coil = CoilHeatingWater(model,schedule);
  PlantLoop plantLoop(model);
  
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  EXPECT_EQ( (unsigned)5,plantLoop.demandComponents().size() );

  Node demandInletNode = plantLoop.demandSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(demandInletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());  

  Node supplyInletNode = plantLoop.supplySplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(supplyInletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());  
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_AddToNodeWithThermalZone)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);
  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_AddAirTerminalWaterHeatingCoilToPlant)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingWater coil = CoilHeatingWater(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);
  PlantLoop plantLoop(model);

  airLoop.addBranchForZone(thermalZone);
  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  testObject.addToNode(inletNode);

  EXPECT_TRUE(plantLoop.addDemandBranchForComponent(coil));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());
  EXPECT_NE((unsigned)7, airLoop.demandComponents().size());
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());
  EXPECT_NE((unsigned)5, plantLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_RemoveAirTerminalWaterHeatingCoilFromPlant)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingWater coil = CoilHeatingWater(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);
  PlantLoop plantLoop(model);

  airLoop.addBranchForZone(thermalZone);
  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  testObject.addToNode(inletNode);

  plantLoop.addDemandBranchForComponent(coil);
  EXPECT_TRUE(plantLoop.removeDemandBranchWithComponent(coil));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());
  EXPECT_NE((unsigned)7, airLoop.demandComponents().size());
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());
  EXPECT_NE((unsigned)7, plantLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_AddObjectByAirLoopAddBranchForZoneWithThermalZone)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  EXPECT_TRUE(airLoop.addBranchForZone(thermalZone, testObject));
  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());
  EXPECT_NE((unsigned)7, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_AddObjectByAirLoopAddBranchForHVACComponent)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);

  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObject));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
  EXPECT_NE((unsigned)9, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_AddObjectByAirLoopAddBranchForHVACComponentWithThermalZone)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);

  EXPECT_TRUE(airLoop.addBranchForHVACComponent(testObject));
  EXPECT_EQ((unsigned)10, airLoop.demandComponents().size());
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_AddToNodeTwoSameObjects)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);
  inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  EXPECT_FALSE(testObject.addToNode(inletNode));
  
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());   
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_IsRemovable)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  EXPECT_TRUE(testObject.isRemovable());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_Remove)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);

  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_RemoveObjectWithThermalZone)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);
  ThermalZone thermalZone(model);

  airLoop.addBranchForZone(thermalZone);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();
  testObject.addToNode(inletNode);

  EXPECT_EQ((unsigned)9, airLoop.demandComponents().size());

  testObject.remove();
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_CloneOneModelWithDefaultData)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirTerminalSingleDuctConstantVolumeReheat testObjectClone = testObject.clone(model).cast<AirTerminalSingleDuctConstantVolumeReheat>();
  EXPECT_TRUE(testObjectClone.isMaximumAirFlowRateAutosized());
  EXPECT_TRUE(testObjectClone.isMaximumHotWaterorSteamFlowRateAutosized());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.minimumHotWaterorSteamFlowRate());
  EXPECT_DOUBLE_EQ(0.001, testObjectClone.convergenceTolerance());
  EXPECT_DOUBLE_EQ(35.0, testObjectClone.maximumReheatAirTemperature());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_CloneOneModelWithCustomData)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);
  testObject.setMaximumAirFlowRate(999.0);
  testObject.setMaximumHotWaterorSteamFlowRate(999.0);
  testObject.setMinimumHotWaterorSteamFlowRate(999.0);
  testObject.setConvergenceTolerance(1.0);
  testObject.setMaximumReheatAirTemperature(999.0);

  AirTerminalSingleDuctConstantVolumeReheat testObjectClone = testObject.clone(model).cast<AirTerminalSingleDuctConstantVolumeReheat>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.maximumAirFlowRate().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.maximumHotWaterorSteamFlowRate().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.minimumHotWaterorSteamFlowRate());
  EXPECT_DOUBLE_EQ(1.0, testObjectClone.convergenceTolerance());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.maximumReheatAirTemperature());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_CloneTwoModelsWithDefaultData)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirTerminalSingleDuctConstantVolumeReheat testObjectClone = testObject.clone(model).cast<AirTerminalSingleDuctConstantVolumeReheat>();

  Model model2;

  AirTerminalSingleDuctConstantVolumeReheat testObjectClone2 = testObject.clone(model2).cast<AirTerminalSingleDuctConstantVolumeReheat>();
  EXPECT_TRUE(testObjectClone2.isMaximumAirFlowRateAutosized());
  EXPECT_TRUE(testObjectClone2.isMaximumHotWaterorSteamFlowRateAutosized());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone2.minimumHotWaterorSteamFlowRate());
  EXPECT_DOUBLE_EQ(0.001, testObjectClone2.convergenceTolerance());
  EXPECT_DOUBLE_EQ(35.0, testObjectClone2.maximumReheatAirTemperature());
  EXPECT_NE(testObjectClone2, testObjectClone);
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_SetNewReheatCoil)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coilElec = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coilElec);

  CoilHeatingGas coilGas = CoilHeatingGas(model,schedule);

  EXPECT_TRUE(testObject.setReheatCoil(coilGas));
  testObject.resetReheatCoil();

  CoilHeatingWater coilWater = CoilHeatingWater(model,schedule);
  EXPECT_TRUE(testObject.setReheatCoil(coilWater));
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_MaximumAirFlowRate) 
{
  Model m;
  ScheduleCompact s(m);
  CoilHeatingElectric coil = CoilHeatingElectric(m,s);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(m,s,coil);

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

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_MaximumHotWaterorSteamFlowRate) 
{
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil = CoilHeatingWater(m,s);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(m,s,coil);

  EXPECT_TRUE(testObject.isMaximumHotWaterorSteamFlowRateAutosized());

  testObject.setMaximumHotWaterorSteamFlowRate(999.0);
  EXPECT_DOUBLE_EQ(999.0, testObject.maximumHotWaterorSteamFlowRate().get());

  testObject.setMaximumHotWaterorSteamFlowRate(0.0);
  EXPECT_DOUBLE_EQ(0.0, testObject.maximumHotWaterorSteamFlowRate().get());

  EXPECT_FALSE(testObject.setMaximumHotWaterorSteamFlowRate(-1.0));

  testObject.resetMaximumHotWaterorSteamFlowRate();
  EXPECT_FALSE(testObject.isMaximumHotWaterorSteamFlowRateAutosized());

  testObject.autosizeMaximumHotWaterorSteamFlowRate();
  EXPECT_TRUE(testObject.isMaximumHotWaterorSteamFlowRateAutosized());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_MinimumHotWaterorSteamFlowRate) 
{
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil = CoilHeatingWater(m,s);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(m,s,coil);

  testObject.setMinimumHotWaterorSteamFlowRate(999.0);
  EXPECT_DOUBLE_EQ(999.0, testObject.minimumHotWaterorSteamFlowRate());

  testObject.setMinimumHotWaterorSteamFlowRate(0.0);
  EXPECT_DOUBLE_EQ(0.0, testObject.minimumHotWaterorSteamFlowRate());

  EXPECT_FALSE(testObject.setMinimumHotWaterorSteamFlowRate(-1.0));

  testObject.resetMinimumHotWaterorSteamFlowRate();
  EXPECT_TRUE(testObject.isMinimumHotWaterorSteamFlowRateDefaulted());
  EXPECT_DOUBLE_EQ(0.0, testObject.minimumHotWaterorSteamFlowRate());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_ConvergenceTolerance) 
{
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil = CoilHeatingWater(m,s);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(m,s,coil);

  testObject.setConvergenceTolerance(999.0);
  EXPECT_DOUBLE_EQ(999.0, testObject.convergenceTolerance());

  EXPECT_FALSE(testObject.setConvergenceTolerance(0.0));

  EXPECT_TRUE(testObject.setConvergenceTolerance(1.0));
  EXPECT_DOUBLE_EQ(1.0, testObject.convergenceTolerance());

  EXPECT_FALSE(testObject.setConvergenceTolerance(-1.0));

  testObject.resetConvergenceTolerance();
  EXPECT_TRUE(testObject.isConvergenceToleranceDefaulted());
  EXPECT_DOUBLE_EQ(0.001, testObject.convergenceTolerance());
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_MaximumReheatAirTemperature) 
{
  Model m;
  ScheduleCompact s(m);
  CoilHeatingWater coil = CoilHeatingWater(m,s);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(m,s,coil);

  testObject.setMaximumReheatAirTemperature(999.0);
  EXPECT_DOUBLE_EQ(999.0, testObject.maximumReheatAirTemperature());

  EXPECT_FALSE(testObject.setMaximumReheatAirTemperature(0.0));
  
  EXPECT_TRUE(testObject.setMaximumReheatAirTemperature(35.0));
  EXPECT_DOUBLE_EQ(35.0, testObject.maximumReheatAirTemperature());

  EXPECT_FALSE(testObject.setMaximumReheatAirTemperature(-1.0));

  testObject.resetMaximumReheatAirTemperature();
  EXPECT_TRUE(testObject.isMaximumReheatAirTemperatureDefaulted());
  EXPECT_DOUBLE_EQ(35.0, testObject.maximumReheatAirTemperature());
}

