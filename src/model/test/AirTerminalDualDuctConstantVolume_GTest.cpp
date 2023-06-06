/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirTerminalDualDuctConstantVolume.hpp"
#include "../AirTerminalDualDuctConstantVolume_Impl.hpp"

#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../AirLoopHVACZoneSplitter_Impl.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalDualDuctConstantVolume) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      AirTerminalDualDuctConstantVolume atu(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AirTerminalDualDuctConstantVolume_DefaultCtor) {

  Model m;
  AirTerminalDualDuctConstantVolume atu(m);

  // Should be autosized, have alwaysOn avail sch, and not connected
  EXPECT_TRUE(atu.isMaximumAirFlowRateAutosized());
  EXPECT_FALSE(atu.hotAirInletNode());
  EXPECT_FALSE(atu.coldAirInletNode());
  EXPECT_EQ(m.alwaysOnDiscreteSchedule().handle(), atu.availabilitySchedule().handle());
}

TEST_F(ModelFixture, AirTerminalDualDuctConstantVolume_SettersGetters) {

  Model m;
  AirTerminalDualDuctConstantVolume atu(m);

  ScheduleCompact sch2(m);
  EXPECT_TRUE(atu.setAvailabilitySchedule(sch2));
  EXPECT_EQ(sch2.handle(), atu.availabilitySchedule().handle());

  EXPECT_TRUE(atu.setMaximumAirFlowRate(0.1));
  EXPECT_FALSE(atu.isMaximumAirFlowRateAutosized());
  ASSERT_TRUE(atu.maximumAirFlowRate());
  EXPECT_EQ(0.1, atu.maximumAirFlowRate().get());
  atu.autosizeMaximumAirFlowRate();
  EXPECT_TRUE(atu.isMaximumAirFlowRateAutosized());
}

TEST_F(ModelFixture, AirTerminalDualDuctConstantVolume_AddToAirLoopHVAC) {

  // Add terminal
  {
    Model m;
    AirTerminalDualDuctConstantVolume atu(m);

    AirLoopHVAC a(m, true);
    ASSERT_TRUE(a.isDualDuct());
    {
      auto t_zoneSplitters = a.zoneSplitters();
      EXPECT_EQ(1u, t_zoneSplitters.size());
      EXPECT_EQ(a.zoneSplitter(), t_zoneSplitters.front());
      EXPECT_EQ(1u, a.demandInletNodes().size());
    }

    EXPECT_TRUE(a.addBranchForHVACComponent(atu));
    EXPECT_EQ(2u, a.demandInletNodes().size());
    EXPECT_EQ(2u, a.zoneSplitters().size());

    EXPECT_EQ(10u, a.demandComponents().size());

    // Check that inletModelObject(0) is the hotAirInletNode
    ASSERT_TRUE(atu.hotAirInletNode());
    ASSERT_TRUE(atu.inletModelObject(0));
    EXPECT_EQ(atu.hotAirInletNode()->handle(), atu.inletModelObject(0)->handle());

    // Check that inletModelObject(1) is the coldAirInletNode
    ASSERT_TRUE(atu.coldAirInletNode());
    ASSERT_TRUE(atu.inletModelObject(1));
    EXPECT_EQ(atu.coldAirInletNode()->handle(), atu.inletModelObject(1)->handle());
  }

  {
    // Add terminal with zone
    Model m;
    AirTerminalDualDuctConstantVolume atu(m);

    AirLoopHVAC a(m, true);
    ASSERT_TRUE(a.isDualDuct());
    {
      auto t_zoneSplitters = a.zoneSplitters();
      EXPECT_EQ(1u, t_zoneSplitters.size());
      EXPECT_EQ(a.zoneSplitter(), t_zoneSplitters.front());
      EXPECT_EQ(1u, a.demandInletNodes().size());
    }

    ThermalZone zone(m);
    EXPECT_TRUE(a.addBranchForZone(zone, atu));
    {
      auto t_zoneSplitters = a.zoneSplitters();
      EXPECT_EQ(2u, t_zoneSplitters.size());
      EXPECT_EQ(a.zoneSplitter(), t_zoneSplitters.front());
      EXPECT_EQ(2u, a.demandInletNodes().size());
    }

    EXPECT_EQ(12u, a.demandComponents().size());

    // Remove the terminal
    atu.remove();
    EXPECT_EQ(7u, a.demandComponents().size());

    // Make sure we can't add the same zone again
    EXPECT_EQ(1u, zone.airLoopHVACs().size());
    EXPECT_FALSE(a.addBranchForZone(zone));

    // Remove the whole branch
    EXPECT_TRUE(a.removeBranchForZone(zone));
    EXPECT_EQ(5u, a.demandComponents().size());

    AirTerminalDualDuctConstantVolume atu2(m);
    EXPECT_TRUE(a.addBranchForZone(zone, atu2));
    EXPECT_EQ(12u, a.demandComponents().size());

    // Remove the whole branch again, this time there is a terminal to worry about
    EXPECT_TRUE(a.removeBranchForZone(zone));
    EXPECT_EQ(5u, a.demandComponents().size());

    // Do it again and then see that the terminal duplication is happening when we add additional branches
    AirTerminalDualDuctConstantVolume atu3(m);

    EXPECT_TRUE(a.addBranchForZone(zone, atu3));
    EXPECT_EQ(12u, a.demandComponents().size());

    ThermalZone zone2(m);
    EXPECT_TRUE(a.addBranchForZone(zone2));

    {
      auto terminals = subsetCastVector<AirTerminalDualDuctConstantVolume>(a.demandComponents());
      EXPECT_EQ(2u, terminals.size());
    }
  }
}
