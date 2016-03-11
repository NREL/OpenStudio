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
#include "../SetpointManagerColdest.hpp"
#include "../SetpointManagerColdest_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerColdest_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SetpointManagerColdest testObject(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerColdest_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerColdest spm_1(m);
  spm_1.setControlVariable("Temperature");

  SetpointManagerColdest spm_2(m);
  spm_2.setControlVariable("Temperature");

  SetpointManagerColdest spm_3(m);
  spm_3.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerColdest> SetpointManagerColdests = m.getModelObjects<SetpointManagerColdest>();
  EXPECT_EQ(3, SetpointManagerColdests.size());

  EXPECT_EQ(testObject, spm_1.setpointNode());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  SetpointManagerColdests = m.getModelObjects<SetpointManagerColdest>();
  EXPECT_EQ(2, SetpointManagerColdests.size());
}

TEST_F(ModelFixture, SetpointManagerColdest_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerColdest spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerColdest> SetpointManagerColdests = m.getModelObjects<SetpointManagerColdest>();
  EXPECT_EQ(1, SetpointManagerColdests.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  SetpointManagerColdests = m.getModelObjects<SetpointManagerColdest>();
  EXPECT_EQ(0, SetpointManagerColdests.size());
}

TEST_F(ModelFixture, SetpointManagerColdest_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerColdest testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerColdest testObjectClone = testObject.clone(m).cast<SetpointManagerColdest>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.strategy(), testObjectClone.strategy());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ("MinimumTemperature", testObjectClone.strategy());
  EXPECT_DOUBLE_EQ(20, testObjectClone.minimumSetpointTemperature());
  EXPECT_DOUBLE_EQ(50, testObjectClone.maximumSetpointTemperature());
}

TEST_F(ModelFixture, SetpointManagerColdest_customDataClone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerColdest testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  testObject.setStrategy("MinimumTemperature");
  testObject.setMaximumSetpointTemperature(999.9);
  testObject.setMinimumSetpointTemperature(999.9);

  SetpointManagerColdest testObjectClone = testObject.clone(m).cast<SetpointManagerColdest>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ("MinimumTemperature", testObjectClone.strategy());
  EXPECT_DOUBLE_EQ(999.9, testObjectClone.maximumSetpointTemperature());
  EXPECT_DOUBLE_EQ(999.9, testObjectClone.minimumSetpointTemperature());
}
