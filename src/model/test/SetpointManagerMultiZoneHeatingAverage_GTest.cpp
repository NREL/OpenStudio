/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SetpointManagerMultiZoneHeatingAverage.hpp"
#include "../SetpointManagerMultiZoneHeatingAverage_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerMultiZoneHeatingAverage_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SetpointManagerMultiZoneHeatingAverage testObject(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHeatingAverage_addToNode) {
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHeatingAverage spm_1(m);
  spm_1.setControlVariable("Temperature");

  SetpointManagerMultiZoneHeatingAverage spm_2(m);
  spm_2.setControlVariable("Temperature");

  SetpointManagerMultiZoneHeatingAverage spm_3(m);
  spm_3.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMultiZoneHeatingAverage> SetpointManagerMultiZoneHeatingAverages =
    m.getConcreteModelObjects<SetpointManagerMultiZoneHeatingAverage>();
  EXPECT_EQ(3, SetpointManagerMultiZoneHeatingAverages.size());

  EXPECT_EQ(testObject, spm_1.setpointNode().get());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode().get());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  SetpointManagerMultiZoneHeatingAverages = m.getConcreteModelObjects<SetpointManagerMultiZoneHeatingAverage>();
  EXPECT_EQ(2, SetpointManagerMultiZoneHeatingAverages.size());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHeatingAverage_remove) {
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHeatingAverage spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMultiZoneHeatingAverage> SetpointManagerMultiZoneHeatingAverages =
    m.getConcreteModelObjects<SetpointManagerMultiZoneHeatingAverage>();
  EXPECT_EQ(1, SetpointManagerMultiZoneHeatingAverages.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  SetpointManagerMultiZoneHeatingAverages = m.getConcreteModelObjects<SetpointManagerMultiZoneHeatingAverage>();
  EXPECT_EQ(0, SetpointManagerMultiZoneHeatingAverages.size());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHeatingAverage_clone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHeatingAverage testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerMultiZoneHeatingAverage>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ("Temperature", testObjectClone.controlVariable());
  EXPECT_DOUBLE_EQ(20., testObjectClone.minimumSetpointTemperature());
  EXPECT_DOUBLE_EQ(50., testObjectClone.maximumSetpointTemperature());
}

TEST_F(ModelFixture, SetpointManagerMultiZoneHeatingAverage_customDataClone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerMultiZoneHeatingAverage testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  testObject.setMinimumSetpointTemperature(0.012);
  testObject.setMaximumSetpointTemperature(0.005);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerMultiZoneHeatingAverage>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ("Temperature", testObjectClone.controlVariable());
  EXPECT_DOUBLE_EQ(0.012, testObjectClone.minimumSetpointTemperature());
  EXPECT_DOUBLE_EQ(0.005, testObjectClone.maximumSetpointTemperature());
}
