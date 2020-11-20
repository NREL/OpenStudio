/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
  std::vector<SetpointManagerScheduledDualSetpoint> setpointManagerScheduledDualSetpoints = m.getModelObjects<SetpointManagerScheduledDualSetpoint>();
  EXPECT_EQ(1, setpointManagerScheduledDualSetpoints.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerScheduledDualSetpoints = m.getModelObjects<SetpointManagerScheduledDualSetpoint>();
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
