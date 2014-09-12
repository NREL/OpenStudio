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
#include <model/ZoneControlThermostatStagedDualSetpoint.hpp>
#include <model/ZoneControlThermostatStagedDualSetpoint_Impl.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/ThermalZone.hpp>
#include <model/ThermalZone_Impl.hpp>

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneControlThermostatStagedDualSetpoint_DefaultConstructors)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
    Model m;
    ZoneControlThermostatStagedDualSetpoint thermostat(m);

    exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, ZoneControlThermostatStagedDualSetpoint_SetGet)
{
  Model m;
  ZoneControlThermostatStagedDualSetpoint thermostat(m);

  ThermalZone zone(m);
  ASSERT_TRUE(zone.setThermostat(thermostat));

  ASSERT_TRUE(thermostat.thermalZone());
  ASSERT_EQ(thermostat.handle(),zone.thermostat()->handle());
  ASSERT_TRUE(thermostat.thermalZone());
  ASSERT_EQ(zone.handle(),thermostat.thermalZone()->handle());
}

