/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVACOutdoorAirSystem.hpp"
#include "../ControllerOutdoorAir.hpp"
#include "../PlantLoop.hpp"
#include "../Model.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../HumidifierSteamElectric.hpp"
#include "../HumidifierSteamElectric_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../Schedule.hpp"
#include "../ScheduleConstant.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, HumidifierSteamElectric_HumidifierSteamElectric) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      HumidifierSteamElectric humidifier(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;
  HumidifierSteamElectric humidifier(m);

  EXPECT_FALSE(humidifier.availabilitySchedule());
  EXPECT_FALSE(humidifier.ratedCapacity());
  EXPECT_TRUE(humidifier.isRatedCapacityAutosized());
  EXPECT_TRUE(humidifier.ratedPower());
  EXPECT_EQ(10200, humidifier.ratedPower().get());
  EXPECT_FALSE(humidifier.isRatedPowerAutosized());
  EXPECT_FALSE(humidifier.ratedFanPower());
  EXPECT_FALSE(humidifier.standbyPower());

  ScheduleConstant schedule(m);
  EXPECT_TRUE(humidifier.setAvailabilitySchedule(schedule));
  EXPECT_TRUE(humidifier.availabilitySchedule());
  humidifier.resetAvailabilitySchedule();
  EXPECT_FALSE(humidifier.availabilitySchedule());

  EXPECT_TRUE(humidifier.setRatedCapacity(1000));
  EXPECT_TRUE(humidifier.ratedCapacity());
  EXPECT_EQ(1000, humidifier.ratedCapacity().get());
  EXPECT_FALSE(humidifier.isRatedCapacityAutosized());
  humidifier.autosizeRatedCapacity();
  EXPECT_TRUE(humidifier.isRatedCapacityAutosized());

  humidifier.autosizeRatedPower();
  EXPECT_FALSE(humidifier.ratedPower());
  EXPECT_TRUE(humidifier.isRatedPowerAutosized());
  humidifier.resetRatedPower();
  EXPECT_FALSE(humidifier.ratedPower());
  EXPECT_FALSE(humidifier.isRatedPowerAutosized());

  EXPECT_TRUE(humidifier.setRatedFanPower(10));
  EXPECT_TRUE(humidifier.ratedFanPower());
  humidifier.resetRatedFanPower();
  EXPECT_FALSE(humidifier.ratedFanPower());

  EXPECT_TRUE(humidifier.setStandbyPower(12));
  EXPECT_TRUE(humidifier.standbyPower());
  humidifier.resetStandbyPower();
  EXPECT_FALSE(humidifier.standbyPower());
}

TEST_F(ModelFixture, HumidifierSteamElectric_addToNode) {
  Model m;
  HumidifierSteamElectric testObject(m);

  AirLoopHVAC airLoop(m);
  ControllerOutdoorAir controllerOutdoorAir(m);
  AirLoopHVACOutdoorAirSystem outdoorAirSystem(m, controllerOutdoorAir);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  outdoorAirSystem.addToNode(supplyOutletNode);

  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_TRUE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)7, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_TRUE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.demandComponents().size());

  HumidifierSteamElectric testObject2(m);
  HumidifierSteamElectric testObject3(m);

  if (boost::optional<Node> OANode = outdoorAirSystem.outboardOANode()) {
    EXPECT_TRUE(testObject2.addToNode(*OANode));
    EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.oaComponents().size());
  }

  if (boost::optional<Node> reliefNode = outdoorAirSystem.outboardReliefNode()) {
    EXPECT_TRUE(testObject3.addToNode(*reliefNode));
    EXPECT_EQ((unsigned)3, airLoop.supplyComponents().size());
    EXPECT_EQ((unsigned)3, outdoorAirSystem.reliefComponents().size());
  }

  auto testObjectClone = testObject.clone(m).cast<HumidifierSteamElectric>();
  supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)5, airLoop.supplyComponents().size());
}
