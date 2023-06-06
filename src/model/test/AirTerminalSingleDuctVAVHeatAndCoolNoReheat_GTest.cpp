/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirTerminalSingleDuctVAVHeatAndCoolNoReheat.hpp"
#include "../AirTerminalSingleDuctVAVHeatAndCoolNoReheat_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalSingleDuctVAVHeatAndCoolNoReheat) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      AirTerminalSingleDuctVAVHeatAndCoolNoReheat terminal(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVHeatAndCoolNoReheat_MinimumAirFlowTurndownSchedule) {
  Model m;
  AirTerminalSingleDuctVAVHeatAndCoolNoReheat terminal(m);

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