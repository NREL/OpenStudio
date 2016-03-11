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
#include "../SetpointManagerMultiZoneHumidityMinimum.hpp"
#include "../SetpointManagerMultiZoneHumidityMinimum_Impl.hpp"
#include "../SetpointManagerSingleZoneReheat.hpp"
#include "../SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMinimum_DefaultConstructor)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    SetpointManagerMultiZoneHumidityMinimum testObject(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMinimum_addToNode)
{
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHumidityMinimum spm_1(m);
  spm_1.setControlVariable("MinimumHumidityRatio");

  SetpointManagerMultiZoneHumidityMinimum spm_2(m);
  spm_2.setControlVariable("MinimumHumidityRatio");

  SetpointManagerMultiZoneHumidityMinimum spm_3(m);
  spm_3.setControlVariable("MinimumHumidityRatio");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMultiZoneHumidityMinimum> setpointManagerMultiZoneHumidityMinimums = m.getModelObjects<SetpointManagerMultiZoneHumidityMinimum>();
  EXPECT_EQ(3, setpointManagerMultiZoneHumidityMinimums.size());

  EXPECT_EQ(testObject, spm_1.setpointNode());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerMultiZoneHumidityMinimums = m.getModelObjects<SetpointManagerMultiZoneHumidityMinimum>();
  EXPECT_EQ(2, setpointManagerMultiZoneHumidityMinimums.size());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMinimum_remove)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHumidityMinimum spm(m);
  spm.setControlVariable("MinimumHumidityRatio");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMultiZoneHumidityMinimum> setpointManagerMultiZoneHumidityMinimums = m.getModelObjects<SetpointManagerMultiZoneHumidityMinimum>();
  EXPECT_EQ(1, setpointManagerMultiZoneHumidityMinimums.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerMultiZoneHumidityMinimums = m.getModelObjects<SetpointManagerMultiZoneHumidityMinimum>();
  EXPECT_EQ(0, setpointManagerMultiZoneHumidityMinimums.size());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMinimum_clone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHumidityMinimum testObject(m);
  testObject.setControlVariable("MinimumHumidityRatio");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerMultiZoneHumidityMinimum testObjectClone = testObject.clone(m).cast<SetpointManagerMultiZoneHumidityMinimum>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointHumidityRatio(), testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(testObject.maximumSetpointHumidityRatio(), testObjectClone.maximumSetpointHumidityRatio());
  EXPECT_EQ("MinimumHumidityRatio", testObjectClone.controlVariable());
  EXPECT_EQ(0.005, testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(0.012, testObjectClone.maximumSetpointHumidityRatio());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMinimum_customDataClone)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHumidityMinimum testObject(m);
  testObject.setControlVariable("MinimumHumidityRatio");
  testObject.addToNode(outletNode);
  testObject.setMinimumSetpointHumidityRatio(0.012);
  testObject.setMaximumSetpointHumidityRatio(0.005);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  SetpointManagerMultiZoneHumidityMinimum testObjectClone = testObject.clone(m).cast<SetpointManagerMultiZoneHumidityMinimum>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointHumidityRatio(), testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(testObject.maximumSetpointHumidityRatio(), testObjectClone.maximumSetpointHumidityRatio());
  EXPECT_EQ("MinimumHumidityRatio", testObjectClone.controlVariable());
  EXPECT_EQ(0.012, testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(0.005, testObjectClone.maximumSetpointHumidityRatio());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHumidityMinimum_multipleSPMs)
{
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHumidityMinimum spmHumidity(m);
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

