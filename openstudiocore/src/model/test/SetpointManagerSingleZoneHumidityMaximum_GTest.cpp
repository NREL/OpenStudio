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

#include "../SetpointManagerSingleZoneHumidityMaximum.hpp"
#include "../SetpointManagerSingleZoneHumidityMaximum_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../ThermalZone.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerSingleZoneHumidityMaximum_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SetpointManagerSingleZoneHumidityMaximum testObject(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerSingleZoneHumidityMaximum_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerSingleZoneHumidityMaximum spm_1(m);
  spm_1.setControlVariable("Temperature");

  SetpointManagerSingleZoneHumidityMaximum spm_2(m);
  spm_2.setControlVariable("Temperature");

  SetpointManagerSingleZoneHumidityMaximum spm_3(m);
  spm_3.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerSingleZoneHumidityMaximum> setpointManagerSingleZoneHumidityMaximums = m.getModelObjects<SetpointManagerSingleZoneHumidityMaximum>();
  EXPECT_EQ(3, setpointManagerSingleZoneHumidityMaximums.size());

  EXPECT_EQ(testObject, spm_1.setpointNode());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerSingleZoneHumidityMaximums = m.getModelObjects<SetpointManagerSingleZoneHumidityMaximum>();
  EXPECT_EQ(2, setpointManagerSingleZoneHumidityMaximums.size());
}

TEST_F(ModelFixture, SetpointManagerSingleZoneHumidityMaximum_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerSingleZoneHumidityMaximum spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerSingleZoneHumidityMaximum> setpointManagerSingleZoneHumidityMaximums = m.getModelObjects<SetpointManagerSingleZoneHumidityMaximum>();
  EXPECT_EQ(1, setpointManagerSingleZoneHumidityMaximums.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerSingleZoneHumidityMaximums = m.getModelObjects<SetpointManagerSingleZoneHumidityMaximum>();
  EXPECT_EQ(0, setpointManagerSingleZoneHumidityMaximums.size());
}

TEST_F(ModelFixture, SetpointManagerSingleZoneHumidityMaximum_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerSingleZoneHumidityMaximum testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerSingleZoneHumidityMaximum testObjectClone = testObject.clone(m).cast<SetpointManagerSingleZoneHumidityMaximum>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
}

TEST_F(ModelFixture, SetpointManagerSingleZoneHumidityMaximum_cloneTwoAirloop)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();
  ThermalZone thermalZone(m);

  SetpointManagerSingleZoneHumidityMaximum testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  testObject.setControlZone(thermalZone);
  ASSERT_TRUE(testObject.setpointNode());
  ASSERT_TRUE(testObject.controlZone());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());
  EXPECT_EQ(thermalZone, testObject.controlZone().get());

  Model m2;

  SetpointManagerSingleZoneHumidityMaximum testObjectClone = testObject.clone(m2).cast<SetpointManagerSingleZoneHumidityMaximum>();
  EXPECT_FALSE(testObjectClone.setpointNode());
  EXPECT_FALSE(testObjectClone.controlZone());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());

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

TEST_F(ModelFixture, SetpointManagerSingleZoneHumidityMaximum_customDataClone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();
  ThermalZone thermalZone(m);

  SetpointManagerSingleZoneHumidityMaximum testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  testObject.setControlZone(thermalZone);

  ASSERT_TRUE(testObject.setpointNode());
  ASSERT_TRUE(testObject.controlZone());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());
  EXPECT_EQ(thermalZone, testObject.controlZone().get());

  SetpointManagerSingleZoneHumidityMaximum testObjectClone = testObject.clone(m).cast<SetpointManagerSingleZoneHumidityMaximum>();

  EXPECT_FALSE(testObjectClone.setpointNode());
  EXPECT_FALSE(testObjectClone.controlZone());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
}
