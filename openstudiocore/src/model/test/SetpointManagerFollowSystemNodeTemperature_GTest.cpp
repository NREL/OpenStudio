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

