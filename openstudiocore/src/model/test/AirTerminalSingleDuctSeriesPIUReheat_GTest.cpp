/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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
#include "../AirTerminalSingleDuctSeriesPIUReheat.hpp"
#include "../AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../AirLoopHVAC_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../FanConstantVolume.hpp"
#include "../FanConstantVolume_Impl.hpp"
#include "../CoilHeatingElectric.hpp"
#include "../CoilHeatingElectric_Impl.hpp"
#include "../ScheduleRuleset.hpp"
#include "../ScheduleRuleset_Impl.hpp"
#include "../Model.hpp"
#include "../Model_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,AirTerminalSingleDuctSeriesPIUReheat)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  
  ASSERT_EXIT ( 
  {  
    Model m; 
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m,schedule);
    CoilHeatingElectric coil(m,schedule);
    AirTerminalSingleDuctSeriesPIUReheat terminal(m,fan,coil);
    
    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );

  {
    Model m; 
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m,schedule);
    CoilHeatingElectric coil(m,schedule);
    AirTerminalSingleDuctSeriesPIUReheat terminal(m,fan,coil);

    EXPECT_NO_THROW(terminal.reheatCoil());
    EXPECT_NO_THROW(terminal.fan());

    ThermalZone zone(m);
    AirLoopHVAC airSystem(m);

    EXPECT_TRUE(airSystem.addBranchForZone(zone,terminal));
  }

  {
    Model m; 
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m,schedule);
    CoilHeatingElectric coil(m,schedule);
    AirTerminalSingleDuctSeriesPIUReheat terminal(m,fan,coil);

    terminal.reheatCoil();
    terminal.fan();

    AirTerminalSingleDuctSeriesPIUReheat terminalClone = terminal.clone(m).cast<AirTerminalSingleDuctSeriesPIUReheat>();
    ASSERT_NO_THROW(terminalClone.reheatCoil());
    ASSERT_NO_THROW(terminalClone.fan());

    EXPECT_NE(terminalClone.reheatCoil(),terminal.reheatCoil());
    EXPECT_NE(terminalClone.fan(),terminal.fan());
  }

  // test that setAvailabilitySchedule also set PIU fan schedule
  {
    Model m; 
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m,schedule);
    CoilHeatingElectric coil(m,schedule);
    AirTerminalSingleDuctSeriesPIUReheat terminal(m,fan,coil);

    AirLoopHVAC airLoopHVAC(m);
    airLoopHVAC.addBranchForHVACComponent(terminal);

    ScheduleRuleset hvacSchedule(m);
    airLoopHVAC.setAvailabilitySchedule(hvacSchedule);

    auto fanSchedule = fan.availabilitySchedule();
    ASSERT_EQ(hvacSchedule.handle(),fanSchedule.handle()); 
  }

  // test that addToNode (by proxy addBranchForHVACComponent) sets the fan schedule to match system availabilitySchedule
  {
    Model m; 
    Schedule schedule = m.alwaysOnDiscreteSchedule();
    FanConstantVolume fan(m,schedule);
    CoilHeatingElectric coil(m,schedule);
    AirTerminalSingleDuctSeriesPIUReheat terminal(m,fan,coil);

    AirLoopHVAC airLoopHVAC(m);

    ScheduleRuleset hvacSchedule(m);
    airLoopHVAC.setAvailabilitySchedule(hvacSchedule);

    airLoopHVAC.addBranchForHVACComponent(terminal);
    auto fanSchedule = fan.availabilitySchedule();
    ASSERT_EQ(hvacSchedule.handle(),fanSchedule.handle()); 
  }
}

