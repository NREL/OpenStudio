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
#include "../SetpointManagerWarmest.hpp"
#include "../SetpointManagerWarmest_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerWarmest_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SetpointManagerWarmest testObject(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerWarmest_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerWarmest spm_1(m);
  spm_1.setControlVariable("Temperature");

  SetpointManagerWarmest spm_2(m);
  spm_2.setControlVariable("Temperature");

  SetpointManagerWarmest spm_3(m);
  spm_3.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerWarmest> setpointManagerWarmests = m.getModelObjects<SetpointManagerWarmest>();
  EXPECT_EQ(3, setpointManagerWarmests.size());

  EXPECT_EQ(testObject, spm_1.setpointNode());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerWarmests = m.getModelObjects<SetpointManagerWarmest>();
  EXPECT_EQ(2, setpointManagerWarmests.size());
}

TEST_F(ModelFixture, SetpointManagerWarmest_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerWarmest spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerWarmest> setpointManagerWarmests = m.getModelObjects<SetpointManagerWarmest>();
  EXPECT_EQ(1, setpointManagerWarmests.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerWarmests = m.getModelObjects<SetpointManagerWarmest>();
  EXPECT_EQ(0, setpointManagerWarmests.size());
}

TEST_F(ModelFixture, SetpointManagerWarmest_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerWarmest testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerWarmest testObjectClone = testObject.clone(m).cast<SetpointManagerWarmest>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.strategy(), testObjectClone.strategy());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ("MaximumTemperature", testObjectClone.strategy());
  EXPECT_EQ(12.2, testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ(15.6, testObjectClone.maximumSetpointTemperature());
}

TEST_F(ModelFixture, SetpointManagerWarmest_customDataClone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerWarmest testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  testObject.setStrategy("MaximumTemperature");
  testObject.setMaximumSetpointTemperature(999.9);
  testObject.setMinimumSetpointTemperature(999.9);

  SetpointManagerWarmest testObjectClone = testObject.clone(m).cast<SetpointManagerWarmest>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ("MaximumTemperature", testObjectClone.strategy());
  EXPECT_EQ(999.9, testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(999.9, testObjectClone.minimumSetpointTemperature());
}

TEST_F(ModelFixture,SetpointManagerOutdoorAirReset) {
  Model model;

  SetpointManagerWarmest setpointManagerWarmest(model);

  AirLoopHVAC airLoopHVAC(model);

  Node node = airLoopHVAC.supplyOutletNode();

  ASSERT_TRUE(setpointManagerWarmest.addToNode(node));

  ASSERT_TRUE(node.setpointManagerWarmest());

  ASSERT_EQ(setpointManagerWarmest,node.setpointManagerWarmest().get());
}
