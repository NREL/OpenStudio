/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <model/test/ModelFixture.hpp>

#include <model/AirTerminalSingleDuctSeriesPIUReheat.hpp>
#include <model/AirTerminalSingleDuctSeriesPIUReheat_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>
#include <model/FanConstantVolume.hpp>
#include <model/FanConstantVolume_Impl.hpp>
#include <model/CoilHeatingElectric.hpp>
#include <model/CoilHeatingElectric_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

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
}

