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
#include <model/SetpointManagerScheduled.hpp>
#include <model/SetpointManagerScheduled_Impl.hpp>
#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/PlantLoop.hpp>
#include <model/ScheduleConstant.hpp>

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerScheduled_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    ScheduleConstant tempSch(m);
    tempSch.setValue(50);
    SetpointManagerScheduled testObject(m,tempSch);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerScheduled_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);

  SetpointManagerScheduled spm_1(m,tempSch);
  spm_1.setControlVariable("MaximumTemperature");

  SetpointManagerScheduled spm_2(m,tempSch);
  spm_2.setControlVariable("MinimumTemperature");

  SetpointManagerScheduled spm_3(m,tempSch);
  spm_3.setControlVariable("Temperature");

  SetpointManagerScheduled spm_4(m,tempSch);
  spm_4.setControlVariable("Temperature");

  SetpointManagerScheduled spm_5(m,tempSch);
  spm_5.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_TRUE(spm_3.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(spm_5.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(3, _setpointManagers.size());
  std::vector<SetpointManagerScheduled> setpointManagerScheduleds = m.getModelObjects<SetpointManagerScheduled>();
  EXPECT_EQ(5, setpointManagerScheduleds.size());

  EXPECT_EQ(testObject, spm_3.setpointNode());
  EXPECT_TRUE(spm_4.addToNode(testObject));
  EXPECT_EQ(testObject, spm_4.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_3) == _setpointManagers.end());
  EXPECT_EQ(3, _setpointManagers.size());
  setpointManagerScheduleds = m.getModelObjects<SetpointManagerScheduled>();
  EXPECT_EQ(4, setpointManagerScheduleds.size());
}

TEST_F(ModelFixture, SetpointManagerScheduled_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);

  SetpointManagerScheduled spm(m,tempSch);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerScheduled> setpointManagerScheduleds = m.getModelObjects<SetpointManagerScheduled>();
  EXPECT_EQ(1, setpointManagerScheduleds.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerScheduleds = m.getModelObjects<SetpointManagerScheduled>();
  EXPECT_EQ(0, setpointManagerScheduleds.size());
}

TEST_F(ModelFixture, SetpointManagerScheduled_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);

  SetpointManagerScheduled testObject(m,tempSch);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerScheduled testObjectClone = testObject.clone(m).cast<SetpointManagerScheduled>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.schedule(), testObjectClone.schedule());
}

TEST_F(ModelFixture, SetpointManagerScheduled_controlVariables)
{
  Model m;
  ScheduleConstant tempSch(m);
  tempSch.setValue(50);
  SetpointManagerScheduled spm(m,tempSch);

  EXPECT_TRUE(spm.setControlVariable("Temperature"));
  EXPECT_EQ("Temperature", spm.controlVariable());
  EXPECT_TRUE(spm.setControlVariable("MaximumTemperature"));
  EXPECT_EQ("MaximumTemperature", spm.controlVariable());
  EXPECT_TRUE(spm.setControlVariable("MinimumTemperature"));
  EXPECT_EQ("MinimumTemperature", spm.controlVariable());

  EXPECT_TRUE(spm.setControlVariable("HumidityRatio"));
  EXPECT_EQ("HumidityRatio", spm.controlVariable());
  EXPECT_TRUE(spm.setControlVariable("MaximumHumidityRatio"));
  EXPECT_EQ("MaximumHumidityRatio", spm.controlVariable());
  EXPECT_TRUE(spm.setControlVariable("MinimumHumidityRatio"));
  EXPECT_EQ("MinimumHumidityRatio", spm.controlVariable());

  EXPECT_TRUE(spm.setControlVariable("MassFlowRate"));
  EXPECT_EQ("MassFlowRate", spm.controlVariable());
  EXPECT_TRUE(spm.setControlVariable("MaximumMassFlowRate"));
  EXPECT_EQ("MaximumMassFlowRate", spm.controlVariable());
  EXPECT_TRUE(spm.setControlVariable("MinimumMassFlowRate"));
  EXPECT_EQ("MinimumMassFlowRate", spm.controlVariable());
}
