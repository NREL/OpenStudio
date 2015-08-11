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
#include "../WaterHeaterHeatPump.hpp"
#include "../WaterHeaterHeatPump_Impl.hpp"
#include "../FanOnOff.hpp"
#include "../FanOnOff_Impl.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,WaterHeaterHeatPump)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT ( 
  {  
     Model m; 
     WaterHeaterHeatPump hpwh(m); 
     hpwh.remove();

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );

  {
    Model m;
    WaterHeaterHeatPump hpwh(m);

    auto tank = hpwh.tank();
    ASSERT_FALSE(tank.handle().isNull());
    auto zoneHVAC = tank.containingZoneHVACComponent();

     auto dxCoil = hpwh.dXCoil();
     EXPECT_FALSE(dxCoil.handle().isNull());
     auto fan = hpwh.fan();
     EXPECT_FALSE(fan.handle().isNull());

    ASSERT_TRUE(zoneHVAC);
    ASSERT_EQ(hpwh,zoneHVAC.get());

    PlantLoop plantLoop(m);
  }
}
