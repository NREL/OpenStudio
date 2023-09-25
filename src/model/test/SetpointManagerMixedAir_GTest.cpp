/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SetpointManagerMixedAir.hpp"
#include "../SetpointManagerMixedAir_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Schedule.hpp"
#include "../FanVariableVolume.hpp"
#include "../CoilHeatingElectric.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerMixedAir_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SetpointManagerMixedAir testObject(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, SetpointManagerMixedAir_addToNode) {
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node supplyNode = airloop.supplyOutletNode();
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanVariableVolume fan(m, s);
  CoilHeatingElectric coil(m, s);

  coil.addToNode(supplyNode);
  fan.addToNode(supplyNode);
  Node node = coil.outletModelObject()->cast<Node>();

  SetpointManagerMixedAir spm_1(m);
  spm_1.setControlVariable("Temperature");

  SetpointManagerMixedAir spm_2(m);
  spm_2.setControlVariable("Temperature");

  SetpointManagerMixedAir spm_3(m);
  spm_3.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(node));

  EXPECT_TRUE(spm_1.fanInletNode());
  EXPECT_TRUE(spm_1.fanOutletNode());
  EXPECT_TRUE(spm_1.referenceSetpointNode());

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = node.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMixedAir> setpointManagerMixedAirs = m.getConcreteModelObjects<SetpointManagerMixedAir>();
  EXPECT_EQ(3, setpointManagerMixedAirs.size());

  EXPECT_EQ(node, spm_1.setpointNode().get());
  EXPECT_TRUE(spm_2.addToNode(node));
  EXPECT_EQ(node, spm_2.setpointNode().get());

  _setpointManagers = node.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerMixedAirs = m.getConcreteModelObjects<SetpointManagerMixedAir>();
  EXPECT_EQ(2, setpointManagerMixedAirs.size());
}

TEST_F(ModelFixture, SetpointManagerMixedAir_updateFanInletOutletNodes) {
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node supplyNode = airloop.supplyOutletNode();
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanVariableVolume fan(m, s);
  CoilHeatingElectric coil1(m, s);
  CoilHeatingElectric coil2(m, s);
  CoilHeatingElectric coil3(m, s);

  coil1.addToNode(supplyNode);
  coil2.addToNode(supplyNode);
  coil3.addToNode(supplyNode);

  Node node1 = coil1.outletModelObject()->cast<Node>();
  Node node2 = coil2.outletModelObject()->cast<Node>();
  Node node3 = coil3.outletModelObject()->cast<Node>();

  SetpointManagerMixedAir spm_1(m);
  spm_1.setControlVariable("Temperature");
  spm_1.addToNode(node1);

  SetpointManagerMixedAir spm_2(m);
  spm_2.setControlVariable("Temperature");
  spm_2.addToNode(node2);

  SetpointManagerMixedAir spm_3(m);
  spm_3.setControlVariable("Temperature");
  spm_3.addToNode(node3);

  EXPECT_FALSE(spm_1.fanInletNode());
  EXPECT_FALSE(spm_1.fanOutletNode());
  EXPECT_FALSE(spm_2.fanInletNode());
  EXPECT_FALSE(spm_2.fanOutletNode());
  EXPECT_FALSE(spm_3.fanInletNode());
  EXPECT_FALSE(spm_3.fanOutletNode());

  fan.addToNode(supplyNode);

  EXPECT_TRUE(spm_1.fanInletNode());
  EXPECT_TRUE(spm_1.fanOutletNode());
  EXPECT_TRUE(spm_2.fanInletNode());
  EXPECT_TRUE(spm_2.fanOutletNode());
  EXPECT_TRUE(spm_3.fanInletNode());
  EXPECT_TRUE(spm_3.fanOutletNode());
}

TEST_F(ModelFixture, SetpointManagerMixedAir_remove) {
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerMixedAir spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerMixedAir> setpointManagerMixedAirs = m.getConcreteModelObjects<SetpointManagerMixedAir>();
  EXPECT_EQ(1, setpointManagerMixedAirs.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerMixedAirs = m.getConcreteModelObjects<SetpointManagerMixedAir>();
  EXPECT_EQ(0, setpointManagerMixedAirs.size());
}

TEST_F(ModelFixture, SetpointManagerMixedAir_clone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node supplyNode = airloop.supplyOutletNode();
  Schedule s = m.alwaysOnDiscreteSchedule();
  FanVariableVolume fan(m, s);
  CoilHeatingElectric coil(m, s);

  coil.addToNode(supplyNode);
  fan.addToNode(supplyNode);
  Node node = coil.outletModelObject()->cast<Node>();

  SetpointManagerMixedAir testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(node);
  ASSERT_TRUE(testObject.setpointNode());
  ASSERT_TRUE(testObject.fanInletNode());
  ASSERT_TRUE(testObject.fanOutletNode());
  ASSERT_TRUE(testObject.referenceSetpointNode());
  EXPECT_EQ(node, testObject.setpointNode().get());
  EXPECT_EQ(supplyNode, testObject.referenceSetpointNode().get());
  EXPECT_EQ(fan.inletModelObject()->cast<Node>(), testObject.fanInletNode().get());
  EXPECT_EQ(fan.outletModelObject()->cast<Node>(), testObject.fanOutletNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerMixedAir>();
  EXPECT_FALSE(testObjectClone.setpointNode());
  EXPECT_FALSE(testObjectClone.fanInletNode());
  EXPECT_FALSE(testObjectClone.fanOutletNode());
  EXPECT_FALSE(testObjectClone.referenceSetpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
}

TEST_F(ModelFixture, SetpointManagerMixedAir_customDataClone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node supplyNode = airloop.supplyOutletNode();

  SetpointManagerMixedAir testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(supplyNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(supplyNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerMixedAir>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
}
