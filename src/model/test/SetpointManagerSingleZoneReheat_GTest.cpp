/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../SetpointManagerSingleZoneReheat.hpp"
#include "../SetpointManagerSingleZoneReheat_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../ThermalZone.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerSingleZoneReheat_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SetpointManagerSingleZoneReheat testObject(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, SetpointManagerSingleZoneReheat_addToNode) {
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerSingleZoneReheat spm_1(m);
  spm_1.setControlVariable("Temperature");

  SetpointManagerSingleZoneReheat spm_2(m);
  spm_2.setControlVariable("Temperature");

  SetpointManagerSingleZoneReheat spm_3(m);
  spm_3.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_FALSE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerSingleZoneReheat> setpointManagerSingleZoneReheats = m.getConcreteModelObjects<SetpointManagerSingleZoneReheat>();
  EXPECT_EQ(3, setpointManagerSingleZoneReheats.size());

  EXPECT_EQ(testObject, spm_1.setpointNode().get());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode().get());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerSingleZoneReheats = m.getConcreteModelObjects<SetpointManagerSingleZoneReheat>();
  EXPECT_EQ(2, setpointManagerSingleZoneReheats.size());
}

TEST_F(ModelFixture, SetpointManagerSingleZoneReheat_remove) {
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerSingleZoneReheat spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerSingleZoneReheat> setpointManagerSingleZoneReheats = m.getConcreteModelObjects<SetpointManagerSingleZoneReheat>();
  EXPECT_EQ(1, setpointManagerSingleZoneReheats.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerSingleZoneReheats = m.getConcreteModelObjects<SetpointManagerSingleZoneReheat>();
  EXPECT_EQ(0, setpointManagerSingleZoneReheats.size());
}

TEST_F(ModelFixture, SetpointManagerSingleZoneReheat_clone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerSingleZoneReheat testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerSingleZoneReheat>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.maximumSupplyAirTemperature(), testObjectClone.maximumSupplyAirTemperature());
  EXPECT_EQ(testObject.minimumSupplyAirTemperature(), testObjectClone.minimumSupplyAirTemperature());
  EXPECT_EQ(-99.0, testObjectClone.minimumSupplyAirTemperature());
  EXPECT_EQ(99.0, testObjectClone.maximumSupplyAirTemperature());
}

TEST_F(ModelFixture, SetpointManagerSingleZoneReheat_cloneTwoAirloop) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();
  ThermalZone thermalZone(m);

  SetpointManagerSingleZoneReheat testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  testObject.setControlZone(thermalZone);
  ASSERT_TRUE(testObject.setpointNode());
  ASSERT_TRUE(testObject.controlZone());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());
  EXPECT_EQ(thermalZone, testObject.controlZone().get());

  Model m2;

  auto testObjectClone = testObject.clone(m2).cast<SetpointManagerSingleZoneReheat>();
  EXPECT_FALSE(testObjectClone.setpointNode());
  EXPECT_FALSE(testObjectClone.controlZone());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.maximumSupplyAirTemperature(), testObjectClone.maximumSupplyAirTemperature());
  EXPECT_EQ(testObject.minimumSupplyAirTemperature(), testObjectClone.minimumSupplyAirTemperature());
  EXPECT_EQ(-99.0, testObjectClone.minimumSupplyAirTemperature());
  EXPECT_EQ(99.0, testObjectClone.maximumSupplyAirTemperature());

  AirLoopHVAC airloop2(m2);
  ThermalZone thermalZone2(m2);
  Node outletNode2 = airloop2.supplyOutletNode();
  testObjectClone.setControlZone(thermalZone2);
  testObjectClone.addToNode(outletNode2);
  ASSERT_TRUE(testObjectClone.setpointNode());
  ASSERT_TRUE(testObjectClone.controlZone());
  EXPECT_NE(testObject.setpointNode().get(), testObjectClone.setpointNode().get());
  EXPECT_NE(testObject.controlZone().get(), testObjectClone.controlZone().get());
}

TEST_F(ModelFixture, SetpointManagerSingleZoneReheat_customDataClone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();
  ThermalZone thermalZone(m);

  SetpointManagerSingleZoneReheat testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  testObject.setMaximumSupplyAirTemperature(999.9);
  testObject.setMinimumSupplyAirTemperature(999.9);
  testObject.setControlZone(thermalZone);

  ASSERT_TRUE(testObject.setpointNode());
  ASSERT_TRUE(testObject.controlZone());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());
  EXPECT_EQ(thermalZone, testObject.controlZone().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerSingleZoneReheat>();

  EXPECT_FALSE(testObjectClone.setpointNode());
  EXPECT_FALSE(testObjectClone.controlZone());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.maximumSupplyAirTemperature(), testObjectClone.maximumSupplyAirTemperature());
  EXPECT_EQ(testObject.minimumSupplyAirTemperature(), testObjectClone.minimumSupplyAirTemperature());
  EXPECT_EQ(999.9, testObjectClone.maximumSupplyAirTemperature());
  EXPECT_EQ(999.9, testObjectClone.minimumSupplyAirTemperature());
}
