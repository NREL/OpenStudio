/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../SetpointManagerScheduled.hpp"
#include "../SetpointManagerScheduled_Impl.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../ScheduleConstant.hpp"

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
