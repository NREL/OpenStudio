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
#include "../SetpointManagerFollowSystemNodeTemperature.hpp"
#include "../SetpointManagerFollowSystemNodeTemperature_Impl.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerFollowSystemNodeTemperature_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SetpointManagerFollowSystemNodeTemperature testObject(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerFollowSystemNodeTemperature_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerFollowSystemNodeTemperature spm_1(m);
  spm_1.setControlVariable("MaximumTemperature");

  SetpointManagerFollowSystemNodeTemperature spm_2(m);
  spm_2.setControlVariable("MinimumTemperature");

  SetpointManagerFollowSystemNodeTemperature spm_3(m);
  spm_3.setControlVariable("Temperature");

  SetpointManagerFollowSystemNodeTemperature spm_4(m);
  spm_4.setControlVariable("Temperature");

  SetpointManagerFollowSystemNodeTemperature spm_5(m);
  spm_5.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_TRUE(spm_3.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(spm_5.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(3, _setpointManagers.size());
  std::vector<SetpointManagerFollowSystemNodeTemperature> SetpointManagerFollowSystemNodeTemperatures = m.getModelObjects<SetpointManagerFollowSystemNodeTemperature>();
  EXPECT_EQ(5, SetpointManagerFollowSystemNodeTemperatures.size());

  EXPECT_EQ(testObject, spm_3.setpointNode());
  EXPECT_TRUE(spm_4.addToNode(testObject));
  EXPECT_EQ(testObject, spm_4.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_3) == _setpointManagers.end());
  EXPECT_EQ(3, _setpointManagers.size());
  SetpointManagerFollowSystemNodeTemperatures = m.getModelObjects<SetpointManagerFollowSystemNodeTemperature>();
  EXPECT_EQ(4, SetpointManagerFollowSystemNodeTemperatures.size());
}

TEST_F(ModelFixture, SetpointManagerFollowSystemNodeTemperature_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerFollowSystemNodeTemperature spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerFollowSystemNodeTemperature> SetpointManagerFollowSystemNodeTemperatures = m.getModelObjects<SetpointManagerFollowSystemNodeTemperature>();
  EXPECT_EQ(1, SetpointManagerFollowSystemNodeTemperatures.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  SetpointManagerFollowSystemNodeTemperatures = m.getModelObjects<SetpointManagerFollowSystemNodeTemperature>();
  EXPECT_EQ(0, SetpointManagerFollowSystemNodeTemperatures.size());
}

TEST_F(ModelFixture, SetpointManagerFollowSystemNodeTemperature_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  AirLoopHVAC airloop2(m);
  Node outletNode2 = airloop2.supplyOutletNode();

  SetpointManagerFollowSystemNodeTemperature testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.setReferenceNode(outletNode2);
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());
  ASSERT_TRUE(testObject.referenceNode());

  SetpointManagerFollowSystemNodeTemperature testObjectClone = testObject.clone(m).cast<SetpointManagerFollowSystemNodeTemperature>();
  EXPECT_FALSE(testObjectClone.setpointNode());
  EXPECT_FALSE(testObjectClone.referenceNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.referenceTemperatureType(), testObjectClone.referenceTemperatureType());
  EXPECT_EQ(testObject.offsetTemperatureDifference(), testObjectClone.offsetTemperatureDifference());
  EXPECT_EQ(testObject.maximumLimitSetpointTemperature(), testObjectClone.maximumLimitSetpointTemperature());
  EXPECT_EQ(testObject.minimumLimitSetpointTemperature(), testObjectClone.minimumLimitSetpointTemperature());
  EXPECT_EQ("NodeDryBulb", testObjectClone.referenceTemperatureType());
  EXPECT_DOUBLE_EQ(0, testObjectClone.offsetTemperatureDifference());
  EXPECT_DOUBLE_EQ(100.0, testObjectClone.maximumLimitSetpointTemperature());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.minimumLimitSetpointTemperature());
}

TEST_F(ModelFixture, SetpointManagerFollowSystemNodeTemperature_customDataClone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  AirLoopHVAC airloop2(m);
  Node outletNode2 = airloop2.supplyOutletNode();

  SetpointManagerFollowSystemNodeTemperature testObject(m);
  testObject.setControlVariable("MaximumTemperature");
  testObject.setReferenceNode(outletNode2);
  testObject.addToNode(outletNode);
  testObject.setReferenceTemperatureType("NodeWetBulb");
  testObject.setOffsetTemperatureDifference(999.9);
  testObject.setMaximumLimitSetpointTemperature(999.9);
  testObject.setMinimumLimitSetpointTemperature(999.9);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());
  ASSERT_TRUE(testObject.referenceNode());

  SetpointManagerFollowSystemNodeTemperature testObjectClone = testObject.clone(m).cast<SetpointManagerFollowSystemNodeTemperature>();
  EXPECT_FALSE(testObjectClone.setpointNode());
  EXPECT_FALSE(testObjectClone.referenceNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.referenceTemperatureType(), testObjectClone.referenceTemperatureType());
  EXPECT_EQ(testObject.offsetTemperatureDifference(), testObjectClone.offsetTemperatureDifference());
  EXPECT_EQ(testObject.maximumLimitSetpointTemperature(), testObjectClone.maximumLimitSetpointTemperature());
  EXPECT_EQ(testObject.minimumLimitSetpointTemperature(), testObjectClone.minimumLimitSetpointTemperature());
  EXPECT_EQ("MaximumTemperature", testObjectClone.controlVariable());
  EXPECT_EQ("NodeWetBulb", testObjectClone.referenceTemperatureType());
  EXPECT_DOUBLE_EQ(999.9, testObjectClone.offsetTemperatureDifference());
  EXPECT_DOUBLE_EQ(999.9, testObjectClone.maximumLimitSetpointTemperature());
  EXPECT_DOUBLE_EQ(999.9, testObjectClone.minimumLimitSetpointTemperature());
}

