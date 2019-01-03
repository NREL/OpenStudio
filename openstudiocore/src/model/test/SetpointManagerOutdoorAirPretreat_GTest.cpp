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
#include "../SetpointManagerOutdoorAirPretreat.hpp"
#include "../SetpointManagerOutdoorAirPretreat_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerOutdoorAirPretreat_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    SetpointManagerOutdoorAirPretreat testObject(m);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirPretreat_nodes)
{
  Model m;
  AirLoopHVAC airloop(m);
  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m,controllerOutdoorAir);

  Node supplyOutletNode = airloop.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);

  SetpointManagerOutdoorAirPretreat testObject(m);
  testObject.addToNode(supplyOutletNode);

  ASSERT_TRUE(testObject.setpointNode());

  boost::optional<Node> referenceNode;
  boost::optional<Node> mixedAirNode;
  boost::optional<Node> outdoorAirNode;
  boost::optional<Node> returnAirNode;

  if(outdoorAirSystem.mixedAirModelObject()) {
    if( (mixedAirNode = outdoorAirSystem.mixedAirModelObject()->optionalCast<Node>()) ) {
      referenceNode = mixedAirNode;
      testObject.setMixedAirStreamNode(*mixedAirNode);
      testObject.setReferenceSetpointNode(*mixedAirNode);
    }
  }

  if(outdoorAirSystem.outdoorAirModelObject()) {
    if( (outdoorAirNode = outdoorAirSystem.outdoorAirModelObject()->optionalCast<Node>()) ) {
      testObject.setOutdoorAirStreamNode(*outdoorAirNode);
    }
  }

  if(outdoorAirSystem.returnAirModelObject()) {
    if( (returnAirNode = outdoorAirSystem.returnAirModelObject()->optionalCast<Node>()) ) {
      testObject.setReturnAirStreamNode(*returnAirNode);
    }
  }

  ASSERT_TRUE(referenceNode);
  ASSERT_TRUE(mixedAirNode);
  ASSERT_TRUE(outdoorAirNode);
  ASSERT_TRUE(returnAirNode);
  ASSERT_TRUE(testObject.referenceSetpointNode());
  ASSERT_TRUE(testObject.mixedAirStreamNode());
  ASSERT_TRUE(testObject.outdoorAirStreamNode());
  ASSERT_TRUE(testObject.returnAirStreamNode());

  EXPECT_EQ(*referenceNode, testObject.referenceSetpointNode().get());
  EXPECT_EQ(*mixedAirNode, testObject.mixedAirStreamNode().get());
  EXPECT_EQ(*outdoorAirNode, testObject.outdoorAirStreamNode().get());
  EXPECT_EQ(*returnAirNode, testObject.returnAirStreamNode().get());

  EXPECT_EQ(testObject.referenceSetpointNode().get(), testObject.mixedAirStreamNode().get());
  EXPECT_EQ(airloop.supplyOutletNode(), testObject.referenceSetpointNode().get());
  EXPECT_EQ(airloop.supplyOutletNode(), testObject.mixedAirStreamNode().get());
  EXPECT_EQ(airloop.supplyInletNode(), testObject.returnAirStreamNode().get());
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirPretreat_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerOutdoorAirPretreat spm_1(m);
  spm_1.setControlVariable("MaximumHumidityRatio");

  SetpointManagerOutdoorAirPretreat spm_2(m);
  spm_2.setControlVariable("MinimumHumidityRatio");

  SetpointManagerOutdoorAirPretreat spm_3(m);
  spm_3.setControlVariable("Temperature");

  SetpointManagerOutdoorAirPretreat spm_4(m);
  spm_4.setControlVariable("Temperature");

  SetpointManagerOutdoorAirPretreat spm_5(m);
  spm_5.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_TRUE(spm_3.addToNode(testObject));

  EXPECT_TRUE(spm_3.loop());
  EXPECT_TRUE(spm_3.airLoopHVAC());
  EXPECT_FALSE(spm_3.airLoopHVACOutdoorAirSystem());
  EXPECT_FALSE(spm_3.plantLoop());

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_5.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(3, _setpointManagers.size());
  std::vector<SetpointManagerOutdoorAirPretreat> setpointManagerOutdoorAirPretreats = m.getModelObjects<SetpointManagerOutdoorAirPretreat>();
  EXPECT_EQ(5, setpointManagerOutdoorAirPretreats.size());

  EXPECT_EQ(testObject, spm_3.setpointNode());
  EXPECT_TRUE(spm_4.addToNode(testObject));
  EXPECT_EQ(testObject, spm_4.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_3) == _setpointManagers.end());
  EXPECT_EQ(3, _setpointManagers.size());
  setpointManagerOutdoorAirPretreats = m.getModelObjects<SetpointManagerOutdoorAirPretreat>();
  EXPECT_EQ(4, setpointManagerOutdoorAirPretreats.size());
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirPretreat_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerOutdoorAirPretreat spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerOutdoorAirPretreat> setpointManagerOutdoorAirPretreats = m.getModelObjects<SetpointManagerOutdoorAirPretreat>();
  EXPECT_EQ(1, setpointManagerOutdoorAirPretreats.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerOutdoorAirPretreats = m.getModelObjects<SetpointManagerOutdoorAirPretreat>();
  EXPECT_EQ(0, setpointManagerOutdoorAirPretreats.size());
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirPretreat_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerOutdoorAirPretreat testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerOutdoorAirPretreat testObjectClone = testObject.clone(m).cast<SetpointManagerOutdoorAirPretreat>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(testObject.minimumSetpointHumidityRatio(), testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(testObject.maximumSetpointHumidityRatio(), testObjectClone.maximumSetpointHumidityRatio());
  EXPECT_EQ("Temperature", testObjectClone.controlVariable());
  EXPECT_EQ(-99.0, testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ(99.0, testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(0.00001, testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(1.0, testObjectClone.maximumSetpointHumidityRatio());
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirPretreat_customDataClone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerOutdoorAirPretreat testObject(m);
  testObject.setControlVariable("HumidityRatio");
  testObject.addToNode(outletNode);
  testObject.setMinimumSetpointTemperature(999.9);
  testObject.setMaximumSetpointTemperature(999.9);
  testObject.setMinimumSetpointHumidityRatio(1.0);
  testObject.setMaximumSetpointHumidityRatio(0.00001);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerOutdoorAirPretreat testObjectClone = testObject.clone(m).cast<SetpointManagerOutdoorAirPretreat>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(testObject.minimumSetpointHumidityRatio(), testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(testObject.maximumSetpointHumidityRatio(), testObjectClone.maximumSetpointHumidityRatio());
  EXPECT_EQ("HumidityRatio", testObjectClone.controlVariable());
  EXPECT_EQ(999.9, testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ(999.9, testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(1.0, testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(0.00001, testObjectClone.maximumSetpointHumidityRatio());
}
