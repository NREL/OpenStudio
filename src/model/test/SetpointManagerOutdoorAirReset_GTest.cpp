/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../SetpointManagerOutdoorAirReset.hpp"
#include "../SetpointManagerOutdoorAirReset_Impl.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Schedule.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SetpointManagerOutdoorAirReset_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SetpointManagerOutdoorAirReset spm(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirReset_GettersSetters) {
  Model m;
  SetpointManagerOutdoorAirReset spm(m);

  // Control Variable:  String
  // Check Idd default: "Temperature"
  EXPECT_EQ("Temperature", spm.controlVariable());
  // There aren't any other choices possible to test... Temperature is the only one
  EXPECT_TRUE(spm.setControlVariable("Temperature"));

  // Setpoint at Outdoor Low Temperature:  Double
  // Defaults in Ctor
  EXPECT_EQ(22.0, spm.setpointatOutdoorLowTemperature());
  EXPECT_TRUE(spm.setSetpointatOutdoorLowTemperature(10.03));
  EXPECT_EQ(10.03, spm.setpointatOutdoorLowTemperature());

  // Outdoor Low Temperature:  Double
  // Defaults in Ctor
  EXPECT_EQ(10.0, spm.outdoorLowTemperature());
  EXPECT_TRUE(spm.setOutdoorLowTemperature(12.03));
  EXPECT_EQ(12.03, spm.outdoorLowTemperature());

  // Setpoint at Outdoor High Temperature:  Double
  // Defaults in Ctor
  EXPECT_EQ(10.00, spm.setpointatOutdoorHighTemperature());
  EXPECT_TRUE(spm.setSetpointatOutdoorHighTemperature(14.03));
  EXPECT_EQ(14.03, spm.setpointatOutdoorHighTemperature());

  // Outdoor High Temperature:  Double
  // No Default
  EXPECT_EQ(24.0, spm.outdoorHighTemperature());
  EXPECT_TRUE(spm.setOutdoorHighTemperature(25.03));
  EXPECT_EQ(25.03, spm.outdoorHighTemperature());

  // Setpoint Node or NodeList Name: Optional Object
  // see addToNode

  // Schedule Name: Optional Object
  // No Default
  EXPECT_FALSE(spm.schedule());
  Schedule sch(m.alwaysOnDiscreteSchedule());
  EXPECT_TRUE(spm.setSchedule(sch));
  ASSERT_TRUE(spm.schedule());
  EXPECT_EQ(sch, spm.schedule().get());

  // Setpoint at Outdoor Low Temperature 2: Optional Double
  // No Default
  EXPECT_TRUE(spm.setSetpointatOutdoorLowTemperature2(10.03));
  ASSERT_TRUE(spm.setpointatOutdoorLowTemperature2());
  EXPECT_EQ(10.03, spm.setpointatOutdoorLowTemperature2().get());
  spm.resetSetpointatOutdoorLowTemperature2();
  EXPECT_FALSE(spm.setpointatOutdoorLowTemperature2());

  // Outdoor Low Temperature 2: Optional Double
  // No Default
  EXPECT_TRUE(spm.setOutdoorLowTemperature2(10.03));
  ASSERT_TRUE(spm.outdoorLowTemperature2());
  EXPECT_EQ(10.03, spm.outdoorLowTemperature2().get());
  spm.resetOutdoorLowTemperature2();
  EXPECT_FALSE(spm.outdoorLowTemperature2());

  // Setpoint at Outdoor High Temperature 2: Optional Double
  // No Default
  EXPECT_TRUE(spm.setSetpointatOutdoorHighTemperature2(10.03));
  ASSERT_TRUE(spm.setpointatOutdoorHighTemperature2());
  EXPECT_EQ(10.03, spm.setpointatOutdoorHighTemperature2().get());
  spm.resetSetpointatOutdoorHighTemperature2();
  EXPECT_FALSE(spm.setpointatOutdoorHighTemperature2());

  // Outdoor High Temperature 2: Optional Double
  // No Default
  EXPECT_TRUE(spm.setOutdoorHighTemperature2(10.03));
  ASSERT_TRUE(spm.outdoorHighTemperature2());
  EXPECT_EQ(10.03, spm.outdoorHighTemperature2().get());
  spm.resetOutdoorHighTemperature2();
  EXPECT_FALSE(spm.outdoorHighTemperature2());
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirReset_addToNode) {
  Model m;
  AirLoopHVAC airloop(m);
  PlantLoop plantLoop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerOutdoorAirReset spm_1(m);
  spm_1.setControlVariable("Temperature");

  SetpointManagerOutdoorAirReset spm_2(m);
  spm_2.setControlVariable("Temperature");

  SetpointManagerOutdoorAirReset spm_3(m);
  spm_3.setControlVariable("Temperature");

  EXPECT_TRUE(spm_1.addToNode(testObject));

  Node plantNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(spm_3.addToNode(plantNode));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerOutdoorAirReset> setpointManagerOutdoorAirResets = m.getConcreteModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(3, setpointManagerOutdoorAirResets.size());

  EXPECT_EQ(testObject, spm_1.setpointNode().get());
  EXPECT_TRUE(spm_2.addToNode(testObject));
  EXPECT_EQ(testObject, spm_2.setpointNode().get());

  _setpointManagers = testObject.setpointManagers();
  EXPECT_TRUE(std::find(_setpointManagers.begin(), _setpointManagers.end(), spm_1) == _setpointManagers.end());
  EXPECT_EQ(1, _setpointManagers.size());
  setpointManagerOutdoorAirResets = m.getConcreteModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(2, setpointManagerOutdoorAirResets.size());
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirReset_remove) {
  Model m;
  AirLoopHVAC airloop(m);
  Node testObject = airloop.supplyOutletNode();

  SetpointManagerOutdoorAirReset spm(m);
  spm.setControlVariable("Temperature");

  EXPECT_TRUE(spm.addToNode(testObject));

  std::vector<SetpointManager> _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(1, _setpointManagers.size());
  std::vector<SetpointManagerOutdoorAirReset> setpointManagerOutdoorAirResets = m.getConcreteModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(1, setpointManagerOutdoorAirResets.size());

  spm.remove();

  _setpointManagers = testObject.setpointManagers();
  EXPECT_EQ(0, _setpointManagers.size());
  setpointManagerOutdoorAirResets = m.getConcreteModelObjects<SetpointManagerOutdoorAirReset>();
  EXPECT_EQ(0, setpointManagerOutdoorAirResets.size());
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirReset_clone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  SetpointManagerOutdoorAirReset testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerOutdoorAirReset>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.setpointatOutdoorLowTemperature(), testObjectClone.setpointatOutdoorLowTemperature());
  EXPECT_EQ(testObject.outdoorLowTemperature(), testObjectClone.outdoorLowTemperature());
  EXPECT_EQ(testObject.setpointatOutdoorHighTemperature(), testObjectClone.setpointatOutdoorHighTemperature());
  EXPECT_EQ(testObject.outdoorHighTemperature(), testObjectClone.outdoorHighTemperature());
  EXPECT_EQ("Temperature", testObjectClone.controlVariable());
  EXPECT_EQ(22.0, testObjectClone.setpointatOutdoorLowTemperature());
  EXPECT_EQ(10.0, testObjectClone.outdoorLowTemperature());
  EXPECT_EQ(10.0, testObjectClone.setpointatOutdoorHighTemperature());
  EXPECT_EQ(24.0, testObjectClone.outdoorHighTemperature());
}

TEST_F(ModelFixture, SetpointManagerOutdoorAirReset_customDataClone) {
  Model m;
  AirLoopHVAC airloop(m);
  Node outletNode = airloop.supplyOutletNode();

  Schedule s(m.alwaysOnDiscreteSchedule());

  SetpointManagerOutdoorAirReset testObject(m);
  testObject.setControlVariable("Temperature");
  testObject.addToNode(outletNode);
  testObject.setSetpointatOutdoorLowTemperature(999.9);
  testObject.setOutdoorLowTemperature(999.9);
  testObject.setSetpointatOutdoorHighTemperature(999.9);
  testObject.setOutdoorHighTemperature(999.9);
  testObject.setSchedule(s);
  testObject.setSetpointatOutdoorLowTemperature2(999.9);
  ASSERT_TRUE(testObject.setpointNode());
  EXPECT_EQ(outletNode, testObject.setpointNode().get());

  auto testObjectClone = testObject.clone(m).cast<SetpointManagerOutdoorAirReset>();
  EXPECT_FALSE(testObjectClone.setpointNode());

  EXPECT_NE(testObject, testObjectClone);
  EXPECT_EQ(testObject.controlVariable(), testObjectClone.controlVariable());
  EXPECT_EQ(testObject.setpointatOutdoorLowTemperature(), testObjectClone.setpointatOutdoorLowTemperature());
  EXPECT_EQ(testObject.outdoorLowTemperature(), testObjectClone.outdoorLowTemperature());
  EXPECT_EQ(testObject.setpointatOutdoorHighTemperature(), testObjectClone.setpointatOutdoorHighTemperature());
  EXPECT_EQ(testObject.outdoorHighTemperature(), testObjectClone.outdoorHighTemperature());
  EXPECT_EQ(testObject.schedule().get(), testObjectClone.schedule().get());
  EXPECT_EQ(testObject.setpointatOutdoorLowTemperature2().get(), testObjectClone.setpointatOutdoorLowTemperature2().get());
  EXPECT_EQ("Temperature", testObjectClone.controlVariable());
  EXPECT_EQ(999.9, testObjectClone.setpointatOutdoorLowTemperature());
  EXPECT_EQ(999.9, testObjectClone.outdoorLowTemperature());
  EXPECT_EQ(999.9, testObjectClone.setpointatOutdoorHighTemperature());
  EXPECT_EQ(999.9, testObjectClone.outdoorHighTemperature());
  EXPECT_EQ(s, testObjectClone.schedule().get());
  EXPECT_EQ(999.9, testObjectClone.setpointatOutdoorLowTemperature2().get());
}
