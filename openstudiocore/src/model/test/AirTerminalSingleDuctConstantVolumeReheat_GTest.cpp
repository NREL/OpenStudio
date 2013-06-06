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



  /*ASSERT_EQ("autosize",testObject.MaximumAirFlowRate().get());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);*/

  // TODO: Check that value is appropriate (within bounds)
  /*ASSERT_EQ("Autosize",coil.getRatedHighSpeedTotalCoolingCapacity().get());

  coil.setBasinHeaterSetpointTemperature(35);
  ASSERT_EQ(35,coil.getBasinHeaterSetpointTemperature().get());

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(airTerminalSingleDuctConstantVolumeReheat.setMaximumAirFlowRate(testQ));
  OSOptionalQuantity q = airTerminalSingleDuctConstantVolumeReheat.getMaximumAirFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);*/
}
/*
TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_MaximumHotWaterorSteamFlowRate_Quantity) {
  Model model;
  // TODO: Check constructor.
  AirTerminalSingleDuctConstantVolumeReheat airTerminalSingleDuctConstantVolumeReheat(model);

  Unit units = airTerminalSingleDuctConstantVolumeReheat.getMaximumHotWaterorSteamFlowRate(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(airTerminalSingleDuctConstantVolumeReheat.setMaximumHotWaterorSteamFlowRate(testQ));
  OSOptionalQuantity q = airTerminalSingleDuctConstantVolumeReheat.getMaximumHotWaterorSteamFlowRate(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_MinimumHotWaterorSteamFlowRate_Quantity) {
  Model model;
  // TODO: Check constructor.
  AirTerminalSingleDuctConstantVolumeReheat airTerminalSingleDuctConstantVolumeReheat(model);

  Unit units = airTerminalSingleDuctConstantVolumeReheat.getMinimumHotWaterorSteamFlowRate(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(airTerminalSingleDuctConstantVolumeReheat.setMinimumHotWaterorSteamFlowRate(testQ));
  Quantity q = airTerminalSingleDuctConstantVolumeReheat.getMinimumHotWaterorSteamFlowRate(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_ConvergenceTolerance_Quantity) {
  Model model;
  // TODO: Check constructor.
  AirTerminalSingleDuctConstantVolumeReheat airTerminalSingleDuctConstantVolumeReheat(model);

  Unit units = airTerminalSingleDuctConstantVolumeReheat.getConvergenceTolerance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(airTerminalSingleDuctConstantVolumeReheat.setConvergenceTolerance(testQ));
  Quantity q = airTerminalSingleDuctConstantVolumeReheat.getConvergenceTolerance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_MaximumReheatAirTemperature_Quantity) {
  Model model;
  // TODO: Check constructor.
  AirTerminalSingleDuctConstantVolumeReheat airTerminalSingleDuctConstantVolumeReheat(model);

  Unit units = airTerminalSingleDuctConstantVolumeReheat.getMaximumReheatAirTemperature(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(airTerminalSingleDuctConstantVolumeReheat.setMaximumReheatAirTemperature(testQ));
  Quantity q = airTerminalSingleDuctConstantVolumeReheat.getMaximumReheatAirTemperature(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}*/

