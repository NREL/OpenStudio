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
#include <model/SetpointManagerScheduledDualSetpoint.hpp>
#include <model/SetpointManagerScheduledDualSetpoint_Impl.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/PlantLoop.hpp>
#include <model/ScheduleConstant.hpp>

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerScheduledDualSetpoint_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SetpointManagerScheduledDualSetpoint testObject(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerScheduledDualSetpoint_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);

  SetpointManagerScheduledDualSetpoint spm_1(m);
  spm_1.setControlVariable("Temperature");
  spm_1.setHighSetpointSchedule(tempSch);
  spm_1.setLowSetpointSchedule(tempSch);

  SetpointManagerScheduledDualSetpoint spm_2(m);
  spm_2.setControlVariable("Temperature");
  spm_2.setHighSetpointSchedule(tempSch);
  spm_2.setLowSetpointSchedule(tempSch);

  SetpointManagerScheduledDualSetpoint spm_3(m);
  spm_3.setControlVariable("Temperature");
  spm_3.setHighSetpointSchedule(tempSch);
  spm_3.setLowSetpointSchedule(tempSch);

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerScheduledDualSetpoint> setpointManagerScheduledDualSetpoints = m.getModelObjects<SetpointManagerScheduledDualSetpoint>();
  EXPECT_EQ(3, setpointManagerScheduledDualSetpoints.size());

  EXPECT_EQ(testObject, spm_1.setpointNode());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerScheduledDualSetpoints = m.getModelObjects<SetpointManagerScheduledDualSetpoint>();
  EXPECT_EQ(2, setpointManagerScheduledDualSetpoints.size());
}

TEST_F(ModelFixture, SetpointManagerScheduledDualSetpoint_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);

  SetpointManagerScheduledDualSetpoint spm(m);
  spm.setControlVariable("Temperature");
  spm.setHighSetpointSchedule(tempSch);
  spm.setLowSetpointSchedule(tempSch);

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerScheduledDualSetpoint> setpointManagerScheduledDualSetpoints = m.getModelObjects<SetpointManagerScheduledDualSetpoint>();
  EXPECT_EQ(1, setpointManagerScheduledDualSetpoints.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerScheduledDualSetpoints = m.getModelObjects<SetpointManagerScheduledDualSetpoint>();
  EXPECT_EQ(0, setpointManagerScheduledDualSetpoints.size());
}

TEST_F(ModelFixture, SetpointManagerScheduledDualSetpoint_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);

  SetpointManagerScheduledDualSetpoint testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.setHighSetpointSchedule(tempSch);
  testObject.setLowSetpointSchedule(tempSch);
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  ASSERT_TRUE(testObject.highSetpointSchedule());
  ASSERT_TRUE(testObject.lowSetpointSchedule());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerScheduledDualSetpoint testObjectClone = testObject.clone(m).cast<SetpointManagerScheduledDualSetpoint>();
  EXPECT_FALSE(testObjectClone.setpointNode());
  ASSERT_TRUE(testObjectClone.highSetpointSchedule());
  ASSERT_TRUE(testObjectClone.lowSetpointSchedule());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.highSetpointSchedule().get(), testObjectClone.highSetpointSchedule().get());
  EXPECT_EQ(tempSch, testObject.highSetpointSchedule().get());
  EXPECT_EQ(tempSch, testObject.lowSetpointSchedule().get());
  EXPECT_EQ(tempSch, testObjectClone.highSetpointSchedule().get());
  EXPECT_EQ(tempSch, testObjectClone.lowSetpointSchedule().get());
}
