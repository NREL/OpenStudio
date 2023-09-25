/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirTerminalDualDuctVAV.hpp"
#include "../AirTerminalDualDuctVAV_Impl.hpp"
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
#include "../ScheduleCompact_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalDualDuctVAV) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      AirTerminalDualDuctVAV terminal(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  // Add terminal
  {
    Model m;
    AirTerminalDualDuctVAV terminal(m);

    AirLoopHVAC airLoopHVAC(m, true);
    ASSERT_TRUE(airLoopHVAC.isDualDuct());
    {
      auto t_zoneSplitters = airLoopHVAC.zoneSplitters();
      EXPECT_EQ(1u, t_zoneSplitters.size());
      EXPECT_EQ(airLoopHVAC.zoneSplitter(), t_zoneSplitters.front());
      EXPECT_EQ(1u, airLoopHVAC.demandInletNodes().size());
    }

    EXPECT_TRUE(airLoopHVAC.addBranchForHVACComponent(terminal));
    EXPECT_EQ(2u, airLoopHVAC.demandInletNodes().size());
    EXPECT_EQ(2u, airLoopHVAC.zoneSplitters().size());

    EXPECT_EQ(10u, airLoopHVAC.demandComponents().size());
  }

  {
    // Add terminal with zone
    Model m;
    AirTerminalDualDuctVAV terminal(m);

    AirLoopHVAC airLoopHVAC(m, true);
    ASSERT_TRUE(airLoopHVAC.isDualDuct());
    {
      auto t_zoneSplitters = airLoopHVAC.zoneSplitters();
      EXPECT_EQ(1u, t_zoneSplitters.size());
      EXPECT_EQ(airLoopHVAC.zoneSplitter(), t_zoneSplitters.front());
      EXPECT_EQ(1u, airLoopHVAC.demandInletNodes().size());
    }

    ThermalZone zone(m);
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone, terminal));
    {
      auto t_zoneSplitters = airLoopHVAC.zoneSplitters();
      EXPECT_EQ(2u, t_zoneSplitters.size());
      EXPECT_EQ(airLoopHVAC.zoneSplitter(), t_zoneSplitters.front());
      EXPECT_EQ(2u, airLoopHVAC.demandInletNodes().size());
    }

    EXPECT_EQ(12u, airLoopHVAC.demandComponents().size());

    // Remove the terminal
    terminal.remove();
    EXPECT_EQ(7u, airLoopHVAC.demandComponents().size());

    // Make sure we cant add the same zone again
    EXPECT_FALSE(airLoopHVAC.addBranchForZone(zone));

    // Remove the whole branch
    EXPECT_TRUE(airLoopHVAC.removeBranchForZone(zone));
    EXPECT_EQ(5u, airLoopHVAC.demandComponents().size());

    AirTerminalDualDuctVAV terminal2(m);
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone, terminal2));
    EXPECT_EQ(12u, airLoopHVAC.demandComponents().size());

    // Remove the whole branch again, this time there is a terminal to worry about
    EXPECT_TRUE(airLoopHVAC.removeBranchForZone(zone));
    EXPECT_EQ(5u, airLoopHVAC.demandComponents().size());

    // Do it again and then see that the terminal duplication is happening when we add additional branches
    AirTerminalDualDuctVAV terminal3(m);

    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone, terminal3));
    EXPECT_EQ(12u, airLoopHVAC.demandComponents().size());

    ThermalZone zone2(m);
    EXPECT_TRUE(airLoopHVAC.addBranchForZone(zone2));

    {
      auto terminals = subsetCastVector<AirTerminalDualDuctVAV>(airLoopHVAC.demandComponents());
      EXPECT_EQ(2u, terminals.size());
    }
  }
}

TEST_F(ModelFixture, AirTerminalDualDuctVAV_MinimumAirFlowTurndownSchedule) {
  Model m;
  AirTerminalDualDuctVAV terminal(m);

  ScheduleCompact alwaysOnSchedule(m);
  alwaysOnSchedule.setName("ALWAYS_ON");
  alwaysOnSchedule.setString(3, "Through: 12/31");
  alwaysOnSchedule.setString(4, "For: AllDays");
  alwaysOnSchedule.setString(5, "Until: 24:00");
  alwaysOnSchedule.setString(6, "1");

  EXPECT_FALSE(terminal.minimumAirFlowTurndownSchedule());
  EXPECT_TRUE(terminal.setMinimumAirFlowTurndownSchedule(alwaysOnSchedule));
  EXPECT_TRUE(terminal.minimumAirFlowTurndownSchedule());
  EXPECT_EQ(alwaysOnSchedule, terminal.minimumAirFlowTurndownSchedule().get());
  terminal.resetMinimumAirFlowTurndownSchedule();
  EXPECT_FALSE(terminal.minimumAirFlowTurndownSchedule());
}
