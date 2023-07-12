/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../SetpointManagerScheduledDualSetpoint.hpp"
#include "../SetpointManagerScheduledDualSetpoint_Impl.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../ScheduleConstant.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerScheduledDualSetpoint_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SetpointManagerScheduledDualSetpoint testObject(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, SetpointManagerScheduledDualSetpoint_addToNode) {
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
  std::vector<SetpointManagerScheduledDualSetpoint> setpointManagerScheduledDualSetpoints =
    m.getConcreteModelObjects<SetpointManagerScheduledDualSetpoint>();
  EXPECT_EQ(3, setpointManagerScheduledDualSetpoints.size());

  EXPECT_EQ(testObject, spm_1.setpointNode().get());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode().get());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerScheduledDualSetpoints = m.getConcreteModelObjects<SetpointManagerScheduledDualSetpoint>();
  EXPECT_EQ(2, setpointManagerScheduledDualSetpoints.size());
}

TEST_F(ModelFixture, SetpointManagerScheduledDualSetpoint_remove) {
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
  std::vector<SetpointManagerScheduledDualSetpoint> setpointManagerScheduledDualSetpoints =
    m.getConcreteModelObjects<SetpointManagerScheduledDualSetpoint>();
  EXPECT_EQ(1, setpointManagerScheduledDualSetpoints.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerScheduledDualSetpoints = m.getConcreteModelObjects<SetpointManagerScheduledDualSetpoint>();
  EXPECT_EQ(0, setpointManagerScheduledDualSetpoints.size());
}

TEST_F(ModelFixture, SetpointManagerScheduledDualSetpoint_clone) {
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

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerScheduledDualSetpoint>();
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
