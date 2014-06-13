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

#include <model/test/ModelFixture.hpp>
#include <model/SetpointManagerFollowOutdoorAirTemperature.hpp>
#include <model/SetpointManagerFollowOutdoorAirTemperature_Impl.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/PlantLoop.hpp>

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerFollowOutdoorAirTemperature_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SetpointManagerFollowOutdoorAirTemperature testObject(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerFollowOutdoorAirTemperature_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerFollowOutdoorAirTemperature spm_1(m);
  spm_1.setControlVariable("MaximumTemperature");

  SetpointManagerFollowOutdoorAirTemperature spm_2(m);
  spm_2.setControlVariable("MinimumTemperature");

  SetpointManagerFollowOutdoorAirTemperature spm_3(m);
  spm_3.setControlVariable("Temperature");

  SetpointManagerFollowOutdoorAirTemperature spm_4(m);
  spm_4.setControlVariable("Temperature");

  SetpointManagerFollowOutdoorAirTemperature spm_5(m);
  spm_5.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_TRUE(spm_3.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(spm_5.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(3, _setpointManagers.size());
  std::vector<SetpointManagerFollowOutdoorAirTemperature> setpointManagerFollowOutdoorAirTemperatures = m.getModelObjects<SetpointManagerFollowOutdoorAirTemperature>();
  EXPECT_EQ(5, setpointManagerFollowOutdoorAirTemperatures.size());

  EXPECT_EQ(testObject, spm_3.setpointNode());
  EXPECT_TRUE(spm_4.addToNode(testObject));
  EXPECT_EQ(testObject, spm_4.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_3) == _setpointManagers.end());
  EXPECT_EQ(3, _setpointManagers.size());
  setpointManagerFollowOutdoorAirTemperatures = m.getModelObjects<SetpointManagerFollowOutdoorAirTemperature>();
  EXPECT_EQ(4, setpointManagerFollowOutdoorAirTemperatures.size());
}

TEST_F(ModelFixture, SetpointManagerFollowOutdoorAirTemperature_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerFollowOutdoorAirTemperature spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerFollowOutdoorAirTemperature> setpointManagerFollowOutdoorAirTemperatures = m.getModelObjects<SetpointManagerFollowOutdoorAirTemperature>();
  EXPECT_EQ(1, setpointManagerFollowOutdoorAirTemperatures.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerFollowOutdoorAirTemperatures = m.getModelObjects<SetpointManagerFollowOutdoorAirTemperature>();
  EXPECT_EQ(0, setpointManagerFollowOutdoorAirTemperatures.size());
}

TEST_F(ModelFixture, SetpointManagerFollowOutdoorAirTemperature_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerFollowOutdoorAirTemperature testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerFollowOutdoorAirTemperature testObjectClone = testObject.clone(m).cast<SetpointManagerFollowOutdoorAirTemperature>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.referenceTemperatureType(), testObjectClone.referenceTemperatureType());
  EXPECT_EQ(testObject.offsetTemperatureDifference(), testObjectClone.offsetTemperatureDifference());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ("OutdoorAirWetBulb", testObjectClone.referenceTemperatureType());
  EXPECT_EQ(1.5, testObjectClone.offsetTemperatureDifference());
  EXPECT_EQ(80.0, testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(6.0, testObjectClone.minimumSetpointTemperature());
}

TEST_F(ModelFixture, SetpointManagerFollowOutdoorAirTemperature_customDataClone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerFollowOutdoorAirTemperature testObject(m);
  testObject.setControlVariable("MaximumTemperature");
  testObject.addToNode(outletNode);
  testObject.setReferenceTemperatureType("OutdoorAirDryBulb");
  testObject.setOffsetTemperatureDifference(999.9);
  testObject.setMaximumSetpointTemperature(999.9);
  testObject.setMinimumSetpointTemperature(999.9);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerFollowOutdoorAirTemperature testObjectClone = testObject.clone(m).cast<SetpointManagerFollowOutdoorAirTemperature>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.referenceTemperatureType(), testObjectClone.referenceTemperatureType());
  EXPECT_EQ(testObject.offsetTemperatureDifference(), testObjectClone.offsetTemperatureDifference());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ("MaximumTemperature", testObjectClone.controlVariable());
  EXPECT_EQ("OutdoorAirDryBulb", testObjectClone.referenceTemperatureType());
  EXPECT_EQ(999.9, testObjectClone.offsetTemperatureDifference());
  EXPECT_EQ(999.9, testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(999.9, testObjectClone.minimumSetpointTemperature());
}
