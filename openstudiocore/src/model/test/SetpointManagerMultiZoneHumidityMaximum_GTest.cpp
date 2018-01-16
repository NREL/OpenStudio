/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SetpointManagerMultiZoneHumidityMaximum.hpp"
#include "../SetpointManagerMultiZoneHumidityMaximum_Impl.hpp"
#include "../SetpointManagerSingleZoneReheat.hpp"
#include "../SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMaximum_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    SetpointManagerMultiZoneHumidityMaximum testObject(m);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMaximum_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHumidityMaximum spm_1(m);
  spm_1.setControlVariable("MaximumHumidityRatio");

  SetpointManagerMultiZoneHumidityMaximum spm_2(m);
  spm_2.setControlVariable("MaximumHumidityRatio");

  SetpointManagerMultiZoneHumidityMaximum spm_3(m);
  spm_3.setControlVariable("MaximumHumidityRatio");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMultiZoneHumidityMaximum> SetpointManagerMultiZoneHumidityMaximums = m.getModelObjects<SetpointManagerMultiZoneHumidityMaximum>();
  EXPECT_EQ(3, SetpointManagerMultiZoneHumidityMaximums.size());

  EXPECT_EQ(testObject, spm_1.setpointNode());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  SetpointManagerMultiZoneHumidityMaximums = m.getModelObjects<SetpointManagerMultiZoneHumidityMaximum>();
  EXPECT_EQ(2, SetpointManagerMultiZoneHumidityMaximums.size());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMaximum_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHumidityMaximum spm(m);
  spm.setControlVariable("MaximumHumidityRatio");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMultiZoneHumidityMaximum> SetpointManagerMultiZoneHumidityMaximums = m.getModelObjects<SetpointManagerMultiZoneHumidityMaximum>();
  EXPECT_EQ(1, SetpointManagerMultiZoneHumidityMaximums.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  SetpointManagerMultiZoneHumidityMaximums = m.getModelObjects<SetpointManagerMultiZoneHumidityMaximum>();
  EXPECT_EQ(0, SetpointManagerMultiZoneHumidityMaximums.size());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMaximum_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHumidityMaximum testObject(m);
  testObject.setControlVariable("MaximumHumidityRatio");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerMultiZoneHumidityMaximum testObjectClone = testObject.clone(m).cast<SetpointManagerMultiZoneHumidityMaximum>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointHumidityRatio(), testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(testObject.maximumSetpointHumidityRatio(), testObjectClone.maximumSetpointHumidityRatio());
  EXPECT_EQ("MaximumHumidityRatio", testObjectClone.controlVariable());
  EXPECT_EQ(0.008, testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(0.015, testObjectClone.maximumSetpointHumidityRatio());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMaximum_customDataClone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHumidityMaximum testObject(m);
  testObject.setControlVariable("MaximumHumidityRatio");
  testObject.addToNode(outletNode);
  testObject.setMinimumSetpointHumidityRatio(0.012);
  testObject.setMaximumSetpointHumidityRatio(0.005);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerMultiZoneHumidityMaximum testObjectClone = testObject.clone(m).cast<SetpointManagerMultiZoneHumidityMaximum>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointHumidityRatio(), testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(testObject.maximumSetpointHumidityRatio(), testObjectClone.maximumSetpointHumidityRatio());
  EXPECT_EQ("MaximumHumidityRatio", testObjectClone.controlVariable());
  EXPECT_EQ(0.012, testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(0.005, testObjectClone.maximumSetpointHumidityRatio());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMaximum_multipleSPMs)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHumidityMaximum spmHumidity(m);
  spmHumidity.addToNode(outletNode);
  ASSERT_EQ(1u,outletNode.setpointManagers().size());

  SetpointManagerSingleZoneReheat spmTemp(m);
  spmTemp.addToNode(outletNode);
  ASSERT_EQ(2u,outletNode.setpointManagers().size());

  auto node = spmHumidity.setpointNode();
  ASSERT_TRUE(node);
  ASSERT_EQ(outletNode,node.get());

  node = spmTemp.setpointNode();
  ASSERT_TRUE(node);
  ASSERT_EQ(outletNode,node.get());
}

