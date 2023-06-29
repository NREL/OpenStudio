/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SetpointManagerMultiZoneMinimumHumidityAverage.hpp"
#include "../SetpointManagerMultiZoneMinimumHumidityAverage_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerMultiZoneMinimumHumidityAverage_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SetpointManagerMultiZoneMinimumHumidityAverage testObject(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, SetpointManagerMultiZoneMinimumHumidityAverage_addToNode) {
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerMultiZoneMinimumHumidityAverage spm_1(m);
  spm_1.setControlVariable("MinimumHumidityRatio");

  SetpointManagerMultiZoneMinimumHumidityAverage spm_2(m);
  spm_2.setControlVariable("MinimumHumidityRatio");

  SetpointManagerMultiZoneMinimumHumidityAverage spm_3(m);
  spm_3.setControlVariable("MinimumHumidityRatio");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMultiZoneMinimumHumidityAverage> setpointManagerMultiZoneMinimumHumidityAverages =
    m.getConcreteModelObjects<SetpointManagerMultiZoneMinimumHumidityAverage>();
  EXPECT_EQ(3, setpointManagerMultiZoneMinimumHumidityAverages.size());

  EXPECT_EQ(testObject, spm_1.setpointNode().get());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode().get());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerMultiZoneMinimumHumidityAverages = m.getConcreteModelObjects<SetpointManagerMultiZoneMinimumHumidityAverage>();
  EXPECT_EQ(2, setpointManagerMultiZoneMinimumHumidityAverages.size());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneMinimumHumidityAverage_remove) {
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerMultiZoneMinimumHumidityAverage spm(m);
  spm.setControlVariable("MinimumHumidityRatio");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMultiZoneMinimumHumidityAverage> SetpointManagerMultiZoneMinimumHumidityAverages =
    m.getConcreteModelObjects<SetpointManagerMultiZoneMinimumHumidityAverage>();
  EXPECT_EQ(1, SetpointManagerMultiZoneMinimumHumidityAverages.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  SetpointManagerMultiZoneMinimumHumidityAverages = m.getConcreteModelObjects<SetpointManagerMultiZoneMinimumHumidityAverage>();
  EXPECT_EQ(0, SetpointManagerMultiZoneMinimumHumidityAverages.size());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneMinimumHumidityAverage_clone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneMinimumHumidityAverage testObject(m);
  testObject.setControlVariable("MinimumHumidityRatio");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerMultiZoneMinimumHumidityAverage>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointHumidityRatio(), testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(testObject.maximumSetpointHumidityRatio(), testObjectClone.maximumSetpointHumidityRatio());
  EXPECT_EQ("MinimumHumidityRatio", testObjectClone.controlVariable());
  EXPECT_EQ(0.005, testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(0.012, testObjectClone.maximumSetpointHumidityRatio());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneMinimumHumidityAverage_customDataClone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneMinimumHumidityAverage testObject(m);
  testObject.setControlVariable("MinimumHumidityRatio");
  testObject.addToNode(outletNode);
  testObject.setMinimumSetpointHumidityRatio(0.012);
  testObject.setMaximumSetpointHumidityRatio(0.005);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerMultiZoneMinimumHumidityAverage>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointHumidityRatio(), testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(testObject.maximumSetpointHumidityRatio(), testObjectClone.maximumSetpointHumidityRatio());
  EXPECT_EQ("MinimumHumidityRatio", testObjectClone.controlVariable());
  EXPECT_EQ(0.012, testObjectClone.minimumSetpointHumidityRatio());
  EXPECT_EQ(0.005, testObjectClone.maximumSetpointHumidityRatio());
}
