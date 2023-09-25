/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SetpointManagerMultiZoneMaximumHumidityAverage.hpp"
#include "../SetpointManagerMultiZoneMaximumHumidityAverage_Impl.hpp"
#include "../SetpointManagerSingleZoneReheat.hpp"
#include "../SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerMultiZoneMaximumHumidityAverage_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SetpointManagerMultiZoneMaximumHumidityAverage testObject(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, SetpointManagerMultiZoneMaximumHumidityAverage_addToNode) {
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerMultiZoneMaximumHumidityAverage spm_1(m);
  spm_1.setControlVariable("MaximumHumidityRatio");

  SetpointManagerMultiZoneMaximumHumidityAverage spm_2(m);
  spm_2.setControlVariable("MaximumHumidityRatio");

  SetpointManagerMultiZoneMaximumHumidityAverage spm_3(m);
  spm_3.setControlVariable("MaximumHumidityRatio");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMultiZoneMaximumHumidityAverage> SetpointManagerMultiZoneMaximumHumidityAverages =
    m.getConcreteModelObjects<SetpointManagerMultiZoneMaximumHumidityAverage>();
  EXPECT_EQ(3, SetpointManagerMultiZoneMaximumHumidityAverages.size());

  EXPECT_EQ(testObject, spm_1.setpointNode().get());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode().get());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  SetpointManagerMultiZoneMaximumHumidityAverages = m.getConcreteModelObjects<SetpointManagerMultiZoneMaximumHumidityAverage>();
  EXPECT_EQ(2, SetpointManagerMultiZoneMaximumHumidityAverages.size());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneMaximumHumidityAverage_remove) {
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerMultiZoneMaximumHumidityAverage spm(m);
  spm.setControlVariable("MaximumHumidityRatio");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMultiZoneMaximumHumidityAverage> SetpointManagerMultiZoneMaximumHumidityAverages =
    m.getConcreteModelObjects<SetpointManagerMultiZoneMaximumHumidityAverage>();
  EXPECT_EQ(1, SetpointManagerMultiZoneMaximumHumidityAverages.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  SetpointManagerMultiZoneMaximumHumidityAverages = m.getConcreteModelObjects<SetpointManagerMultiZoneMaximumHumidityAverage>();
  EXPECT_EQ(0, SetpointManagerMultiZoneMaximumHumidityAverages.size());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneMaximumHumidityAverage_clone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneMaximumHumidityAverage testObject(m);
  testObject.setControlVariable("MaximumHumidityRatio");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerMultiZoneMaximumHumidityAverage>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointHumidityRatio(), testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(testObject.maximumSetpointHumidityRatio(), testObjectClone.maximumSetpointHumidityRatio());
  EXPECT_EQ("MaximumHumidityRatio", testObjectClone.controlVariable());
  EXPECT_EQ(0.008, testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(0.015, testObjectClone.maximumSetpointHumidityRatio());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneMaximumHumidityAverage_customDataClone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneMaximumHumidityAverage testObject(m);
  testObject.setControlVariable("MaximumHumidityRatio");
  testObject.addToNode(outletNode);
  testObject.setMinimumSetpointHumidityRatio(0.012);
  testObject.setMaximumSetpointHumidityRatio(0.005);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerMultiZoneMaximumHumidityAverage>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointHumidityRatio(), testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(testObject.maximumSetpointHumidityRatio(), testObjectClone.maximumSetpointHumidityRatio());
  EXPECT_EQ("MaximumHumidityRatio", testObjectClone.controlVariable());
  EXPECT_EQ(0.012, testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(0.005, testObjectClone.maximumSetpointHumidityRatio());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneMaximumHumidityAverage_multipleSPMs) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneMaximumHumidityAverage spmHumidity(m);
  spmHumidity.addToNode(outletNode);
  ASSERT_EQ(1u, outletNode.setpointManagers().size());

  SetpointManagerSingleZoneReheat spmTemp(m);
  spmTemp.addToNode(outletNode);
  ASSERT_EQ(2u, outletNode.setpointManagers().size());

  auto node = spmHumidity.setpointNode();
  ASSERT_TRUE(node);
  ASSERT_EQ(outletNode, node.get());

  node = spmTemp.setpointNode();
  ASSERT_TRUE(node);
  ASSERT_EQ(outletNode, node.get());
}
