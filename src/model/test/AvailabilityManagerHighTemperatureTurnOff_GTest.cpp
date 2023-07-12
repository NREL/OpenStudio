/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AvailabilityManagerHighTemperatureTurnOff.hpp"
#include "../AvailabilityManagerHighTemperatureTurnOff_Impl.hpp"

#include "../Node.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AvailabilityManagerHighTemperatureTurnOff_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      AvailabilityManagerHighTemperatureTurnOff avm(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AvailabilityManagerHighTemperatureTurnOff_Setters_Getters) {
  Model m;

  AvailabilityManagerHighTemperatureTurnOff avm(m);

  // Test Sensor node fields
  // Random node
  Node n(m);
  ASSERT_FALSE(avm.sensorNode());
  EXPECT_TRUE(avm.setSensorNode(n));
  ASSERT_TRUE(avm.sensorNode());
  ASSERT_EQ(n.handle(), avm.sensorNode()->handle());
  avm.resetSensorNode();
  ASSERT_FALSE(avm.sensorNode());

  // AirLoop Node
  AirLoopHVAC airloop(m);
  Node airLoop_Node = airloop.supplyOutletNode();
  EXPECT_TRUE(avm.setSensorNode(airLoop_Node));
  ASSERT_TRUE(avm.sensorNode());
  ASSERT_EQ(airLoop_Node.handle(), avm.sensorNode()->handle());

  // PlantLoop Node
  PlantLoop plantLoop(m);
  Node plantLoop_Node = plantLoop.supplyOutletNode();
  EXPECT_TRUE(avm.setSensorNode(plantLoop_Node));
  ASSERT_TRUE(avm.sensorNode());
  ASSERT_EQ(plantLoop_Node.handle(), avm.sensorNode()->handle());

  // Test temperature
  EXPECT_TRUE(avm.setTemperature(60.19));
  ASSERT_EQ(60.19, avm.temperature());
}
