/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirTerminalSingleDuctVAVHeatAndCoolReheat.hpp"
#include "../AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../CoilHeatingGas.hpp"
#include "../CoilHeatingGas_Impl.hpp"
#include "../Schedule.hpp"
#include "../ScheduleCompact.hpp"
#include "../ScheduleCompact_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirTerminalSingleDuctVAVHeatAndCoolReheat) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Schedule schedule = m.alwaysOnDiscreteSchedule();
      CoilHeatingGas coil(m, schedule);
      AirTerminalSingleDuctVAVHeatAndCoolReheat terminal(m, coil);
      terminal.reheatCoil();
      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  {
    Model m;
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    CoilHeatingGas coil(m, schedule);
    AirTerminalSingleDuctVAVHeatAndCoolReheat terminal(m, coil);

    ThermalZone zone(m);
    AirLoopHVAC airLoopHVAC(m);

    airLoopHVAC.addBranchForZone(zone, terminal);
    ASSERT_EQ(9u, airLoopHVAC.demandComponents().size());
    ASSERT_EQ(1u, zone.equipment().size());

    ASSERT_EQ(1u, terminal.getImpl<model::detail::HVACComponent_Impl>()->children().size());
    auto terminal2 = terminal.clone(m).cast<AirTerminalSingleDuctVAVHeatAndCoolReheat>();
    ASSERT_EQ(1u, terminal2.getImpl<model::detail::HVACComponent_Impl>()->children().size());

    Model m2;
    terminal2 = terminal.clone(m2).cast<AirTerminalSingleDuctVAVHeatAndCoolReheat>();
    ASSERT_EQ(1u, terminal2.getImpl<model::detail::HVACComponent_Impl>()->children().size());

    terminal.remove();
    ASSERT_TRUE(terminal.handle().isNull());
  }
}

TEST_F(ModelFixture, AirTerminalSingleDuctVAVHeatAndCoolReheat_MinimumAirFlowTurndownSchedule) {
  Model m;
  Schedule schedule = m.alwaysOnDiscreteSchedule();
  CoilHeatingGas coil(m, schedule);
  AirTerminalSingleDuctVAVHeatAndCoolReheat terminal(m, coil);

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
