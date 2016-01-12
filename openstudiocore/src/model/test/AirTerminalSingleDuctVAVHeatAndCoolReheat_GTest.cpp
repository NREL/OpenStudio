/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../AirTerminalSingleDuctVAVHeatAndCoolReheat.hpp"
#include "../AirTerminalSingleDuctVAVHeatAndCoolReheat_Impl.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"
#include "../CoilHeatingGas.hpp"
#include "../CoilHeatingGas_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,AirTerminalSingleDuctVAVHeatAndCoolReheat)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
    {
      Model m;
      Schedule schedule = m.alwaysOnDiscreteSchedule();
      CoilHeatingGas coil(m,schedule);
      AirTerminalSingleDuctVAVHeatAndCoolReheat terminal(m,coil);
      terminal.reheatCoil();
      exit(0);
    } ,
    ::testing::ExitedWithCode(0), "" );

  {
    Model m;
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    CoilHeatingGas coil(m,schedule);
    AirTerminalSingleDuctVAVHeatAndCoolReheat terminal(m,coil);

    ThermalZone zone(m);
    AirLoopHVAC airLoopHVAC(m);

    airLoopHVAC.addBranchForZone(zone,terminal);
    ASSERT_EQ(9u,airLoopHVAC.demandComponents().size());
    ASSERT_EQ(1u,zone.equipment().size());

    ASSERT_EQ(1u,terminal.getImpl<model::detail::HVACComponent_Impl>()->children().size());
    AirTerminalSingleDuctVAVHeatAndCoolReheat terminal2 = terminal.clone(m).cast<AirTerminalSingleDuctVAVHeatAndCoolReheat>();
    ASSERT_EQ(1u,terminal2.getImpl<model::detail::HVACComponent_Impl>()->children().size());

    Model m2;
    terminal2 = terminal.clone(m2).cast<AirTerminalSingleDuctVAVHeatAndCoolReheat>();
    ASSERT_EQ(1u,terminal2.getImpl<model::detail::HVACComponent_Impl>()->children().size());

    terminal.remove();
    ASSERT_TRUE(terminal.handle().isNull());
  }
}

