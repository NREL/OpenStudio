/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SetpointManagerFollowGroundTemperature.hpp"
#include "../SetpointManagerFollowGroundTemperature_Impl.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerFollowGroundTemperature_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SetpointManagerFollowGroundTemperature testObject(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, SetpointManagerFollowGroundTemperature_addToNode) {
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerFollowGroundTemperature spm_1(m);
  spm_1.setControlVariable("MaximumTemperature");

  SetpointManagerFollowGroundTemperature spm_2(m);
  spm_2.setControlVariable("MinimumTemperature");

  SetpointManagerFollowGroundTemperature spm_3(m);
  spm_3.setControlVariable("Temperature");

  SetpointManagerFollowGroundTemperature spm_4(m);
  spm_4.setControlVariable("Temperature");

  SetpointManagerFollowGroundTemperature spm_5(m);
  spm_5.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_TRUE(spm_3.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(spm_5.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(3, _setpointManagers.size());
  std::vector<SetpointManagerFollowGroundTemperature> SetpointManagerFollowGroundTemperatures =
    m.getConcreteModelObjects<SetpointManagerFollowGroundTemperature>();
  EXPECT_EQ(5, SetpointManagerFollowGroundTemperatures.size());

  EXPECT_EQ(testObject, spm_3.setpointNode().get());
  EXPECT_TRUE(spm_4.addToNode(testObject));
  EXPECT_EQ(testObject, spm_4.setpointNode().get());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_3) == _setpointManagers.end());
  EXPECT_EQ(3, _setpointManagers.size());
  SetpointManagerFollowGroundTemperatures = m.getConcreteModelObjects<SetpointManagerFollowGroundTemperature>();
  EXPECT_EQ(4, SetpointManagerFollowGroundTemperatures.size());
}

TEST_F(ModelFixture, SetpointManagerFollowGroundTemperature_remove) {
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerFollowGroundTemperature spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerFollowGroundTemperature> SetpointManagerFollowGroundTemperatures =
    m.getConcreteModelObjects<SetpointManagerFollowGroundTemperature>();
  EXPECT_EQ(1, SetpointManagerFollowGroundTemperatures.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  SetpointManagerFollowGroundTemperatures = m.getConcreteModelObjects<SetpointManagerFollowGroundTemperature>();
  EXPECT_EQ(0, SetpointManagerFollowGroundTemperatures.size());
}

TEST_F(ModelFixture, SetpointManagerFollowGroundTemperature_clone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerFollowGroundTemperature testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerFollowGroundTemperature>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.offsetTemperatureDifference(), testObjectClone.offsetTemperatureDifference());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_DOUBLE_EQ(0, testObjectClone.offsetTemperatureDifference());
  EXPECT_DOUBLE_EQ(80.0, testObjectClone.maximumSetpointTemperature());
  EXPECT_DOUBLE_EQ(10.0, testObjectClone.minimumSetpointTemperature());
}

TEST_F(ModelFixture, SetpointManagerFollowGroundTemperature_customDataClone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerFollowGroundTemperature testObject(m);
  testObject.setControlVariable("MaximumTemperature");
  testObject.addToNode(outletNode);
  testObject.setOffsetTemperatureDifference(999.9);
  testObject.setMaximumSetpointTemperature(999.9);
  testObject.setMinimumSetpointTemperature(999.9);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerFollowGroundTemperature>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.offsetTemperatureDifference(), testObjectClone.offsetTemperatureDifference());
  EXPECT_EQ(testObject.maximumSetpointTemperature(), testObjectClone.maximumSetpointTemperature());
  EXPECT_EQ(testObject.minimumSetpointTemperature(), testObjectClone.minimumSetpointTemperature());
  EXPECT_EQ("MaximumTemperature", testObjectClone.controlVariable());
  EXPECT_DOUBLE_EQ(999.9, testObjectClone.offsetTemperatureDifference());
  EXPECT_DOUBLE_EQ(999.9, testObjectClone.maximumSetpointTemperature());
  EXPECT_DOUBLE_EQ(999.9, testObjectClone.minimumSetpointTemperature());
}

TEST_F(ModelFixture, SetpointManagerFollowGroundTemperature_referenceObjectType) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerFollowGroundTemperature testObject(m);
  EXPECT_EQ(testObject.referenceGroundTemperatureObjectType(), "Site:GroundTemperature:BuildingSurface");
  EXPECT_TRUE(testObject.setReferenceGroundTemperatureObjectType("Site:GroundTemperature:Deep"));
  EXPECT_EQ(testObject.referenceGroundTemperatureObjectType(), "Site:GroundTemperature:Deep");
  EXPECT_FALSE(testObject.setReferenceGroundTemperatureObjectType("bogus"));
  EXPECT_EQ(testObject.referenceGroundTemperatureObjectType(), "Site:GroundTemperature:Deep");
}
