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
#include "../SetpointManagerSingleZoneOneStageHeating.hpp"
#include "../SetpointManagerSingleZoneOneStageHeating_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../ThermalZone.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerSingleZoneOneStageHeating_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SetpointManagerSingleZoneOneStageHeating testObject(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerSingleZoneOneStageHeating_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerSingleZoneOneStageHeating spm_1(m);
  spm_1.setControlVariable("Temperature");

  SetpointManagerSingleZoneOneStageHeating spm_2(m);
  spm_2.setControlVariable("Temperature");

  SetpointManagerSingleZoneOneStageHeating spm_3(m);
  spm_3.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerSingleZoneOneStageHeating> SetpointManagerSingleZoneOneStageHeatings = m.getModelObjects<SetpointManagerSingleZoneOneStageHeating>();
  EXPECT_EQ(3, SetpointManagerSingleZoneOneStageHeatings.size());

  EXPECT_EQ(testObject, spm_1.setpointNode());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  SetpointManagerSingleZoneOneStageHeatings = m.getModelObjects<SetpointManagerSingleZoneOneStageHeating>();
  EXPECT_EQ(2, SetpointManagerSingleZoneOneStageHeatings.size());
}

TEST_F(ModelFixture, SetpointManagerSingleZoneOneStageHeating_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerSingleZoneOneStageHeating spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerSingleZoneOneStageHeating> SetpointManagerSingleZoneOneStageHeatings = m.getModelObjects<SetpointManagerSingleZoneOneStageHeating>();
  EXPECT_EQ(1, SetpointManagerSingleZoneOneStageHeatings.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  SetpointManagerSingleZoneOneStageHeatings = m.getModelObjects<SetpointManagerSingleZoneOneStageHeating>();
  EXPECT_EQ(0, SetpointManagerSingleZoneOneStageHeatings.size());
}

TEST_F(ModelFixture, SetpointManagerSingleZoneOneStageHeating_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerSingleZoneOneStageHeating testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerSingleZoneOneStageHeating testObjectClone = testObject.clone(m).cast<SetpointManagerSingleZoneOneStageHeating>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.heatingStageOnSupplyAirSetpointTemperature(), testObjectClone.heatingStageOnSupplyAirSetpointTemperature());
  EXPECT_EQ(testObject.heatingStageOffSupplyAirSetpointTemperature(), testObjectClone.heatingStageOffSupplyAirSetpointTemperature());
  EXPECT_EQ(-99.0, testObjectClone.heatingStageOffSupplyAirSetpointTemperature());
  EXPECT_EQ(99.0, testObjectClone.heatingStageOnSupplyAirSetpointTemperature());
}

TEST_F(ModelFixture, SetpointManagerSingleZoneOneStageHeating_cloneTwoAirloop)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();
  ThermalZone thermalZone(m);

  SetpointManagerSingleZoneOneStageHeating testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  testObject.setControlZone(thermalZone);
  ASSERT_TRUE(testObject.setpointNode());
  ASSERT_TRUE(testObject.controlZone());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());
  EXPECT_EQ(thermalZone, testObject.controlZone().get());

  Model m2;

  SetpointManagerSingleZoneOneStageHeating testObjectClone = testObject.clone(m2).cast<SetpointManagerSingleZoneOneStageHeating>();
  EXPECT_FALSE(testObjectClone.setpointNode());
  EXPECT_FALSE(testObjectClone.controlZone());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.heatingStageOnSupplyAirSetpointTemperature(), testObjectClone.heatingStageOnSupplyAirSetpointTemperature());
  EXPECT_EQ(testObject.heatingStageOffSupplyAirSetpointTemperature(), testObjectClone.heatingStageOffSupplyAirSetpointTemperature());
  EXPECT_EQ(-99.0, testObjectClone.heatingStageOffSupplyAirSetpointTemperature());
  EXPECT_EQ(99.0, testObjectClone.heatingStageOnSupplyAirSetpointTemperature());

  AirLoopHVAC airloop2(m2);
  ThermalZone thermalZone2(m2);
  Node outletNode2 = airloop2.supplyOutletNode();
  testObjectClone.setControlZone(thermalZone2);
  testObjectClone.addToNode(outletNode2);
  ASSERT_TRUE(testObjectClone.setpointNode());
  ASSERT_TRUE(testObjectClone.controlZone());
  EXPECT_NE(testObject.setpointNode().get(), testObjectClone.setpointNode().get());
  EXPECT_NE(testObject.controlZone().get(), testObjectClone.controlZone().get());
}

TEST_F(ModelFixture, SetpointManagerSingleZoneOneStageHeating_customDataClone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();
  ThermalZone thermalZone(m);

  SetpointManagerSingleZoneOneStageHeating testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  testObject.setHeatingStageOnSupplyAirSetpointTemperature(999.9);
  testObject.setHeatingStageOffSupplyAirSetpointTemperature(999.9);
  testObject.setControlZone(thermalZone);

  ASSERT_TRUE(testObject.setpointNode());
  ASSERT_TRUE(testObject.controlZone());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());
  EXPECT_EQ(thermalZone, testObject.controlZone().get());

  SetpointManagerSingleZoneOneStageHeating testObjectClone = testObject.clone(m).cast<SetpointManagerSingleZoneOneStageHeating>();

  EXPECT_FALSE(testObjectClone.setpointNode());
  EXPECT_FALSE(testObjectClone.controlZone());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.heatingStageOnSupplyAirSetpointTemperature(), testObjectClone.heatingStageOnSupplyAirSetpointTemperature());
  EXPECT_EQ(testObject.heatingStageOffSupplyAirSetpointTemperature(), testObjectClone.heatingStageOffSupplyAirSetpointTemperature());
  EXPECT_EQ(999.9, testObjectClone.heatingStageOnSupplyAirSetpointTemperature());
  EXPECT_EQ(999.9, testObjectClone.heatingStageOffSupplyAirSetpointTemperature());
}
