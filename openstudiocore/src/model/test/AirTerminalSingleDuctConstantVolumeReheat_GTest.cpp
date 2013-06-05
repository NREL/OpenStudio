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
#include <model/CoilCoolingWater.hpp>
#include <model/AirLoopHVACZoneSplitter.hpp>
#include <model/AirLoopHVACZoneSplitter_Impl.hpp>

#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

//using namespace openstudio;
using namespace openstudio::model; 

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_AirTerminalSingleDuctConstantVolumeReheat)
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

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_addToNode)
{
  /*
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);
  
  //test add to and remove from Thermal zone
  
  ThermalZone thermalZone(model);
  EXPECT_TRUE(testObject.addToThermalZone(thermalZone));
  boost::optional<ThermalZone> testThermalZone = testObject.thermalZone();
  EXPECT_EQ(*(testThermalZone),testObject.thermalZone());
  
  // Check inlet and outlet nodes
  EXPECT_TRUE(testObject.inletNode());
  EXPECT_TRUE(testObject.outletNode());

  testObject.removeFromThermalZone();
  EXPECT_FALSE(testObject.thermalZone());
*/

  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirLoopHVAC airLoop(model);

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));

  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());
  
  // inlet and outlet ports
  EXPECT_TRUE(testObject.inletPort());
  EXPECT_TRUE(testObject.outletPort());
        
}

TEST_F(ModelFixture, AirTerminalSingleDuctConstantVolumeReheat_clone_with_default_data)
{
  Model model;
  ScheduleCompact schedule(model);
  CoilHeatingElectric coil = CoilHeatingElectric(model,schedule);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(model,schedule,coil);

  AirTerminalSingleDuctConstantVolumeReheat testObjectClone = testObject.clone(model).cast<AirTerminalSingleDuctConstantVolumeReheat>();
  EXPECT_TRUE(testObjectClone.isMaximumAirFlowRateAutosized());
  EXPECT_TRUE(testObjectClone.isMaximumHotWaterorSteamFlowRateAutosized());
  EXPECT_NEAR(0.0, testObjectClone.minimumHotWaterorSteamFlowRate(),1.0E-6);
  EXPECT_NEAR(0.0001, testObjectClone.convergenceTolerance(),1.0E-6);
  EXPECT_NEAR(35.0, testObjectClone.maximumReheatAirTemperature(),1.0E-6);

//  Model m2;

//  DistrictCooling DistrictCoolingClone2 = DistrictCooling.clone(m2).cast<DistrictCooling>();

//  ASSERT_EQ(1234,DistrictCoolingClone2.nominalCapacity());
}

TEST_F(ModelFixture,AirTerminalSingleDuctConstantVolumeReheat_MaximumAirFlowRate) 
{
  Model m;
  ScheduleCompact s(m);
  CoilHeatingElectric coil = CoilHeatingElectric(m,s);
  AirTerminalSingleDuctConstantVolumeReheat testObject = AirTerminalSingleDuctConstantVolumeReheat(m,s,coil);

  EXPECT_TRUE(testObject.isMaximumHotWaterorSteamFlowRateAutosized());

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

