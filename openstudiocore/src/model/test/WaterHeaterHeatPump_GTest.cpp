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
#include "../WaterHeaterHeatPump.hpp"
#include "../WaterHeaterHeatPump_Impl.hpp"
#include "../FanOnOff.hpp"
#include "../FanOnOff_Impl.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPump.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPump_Impl.hpp"
#include "../HVACComponent.hpp"
#include "../HVACComponent_Impl.hpp"
#include "../ThermalZone.hpp"
#include "../ThermalZone_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,WaterHeaterHeatPump)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  // Create
  ASSERT_EXIT ( 
  {  
     Model m; 
     WaterHeaterHeatPump hpwh(m); 

     exit(0); 
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture,WaterHeaterHeatPump_Relationships)
{
  Model m;
  WaterHeaterHeatPump hpwh(m);

  auto dxCoil = hpwh.dXCoil();
  ASSERT_FALSE(dxCoil.handle().isNull());
  auto fan = hpwh.fan();
  ASSERT_FALSE(fan.handle().isNull());
  auto tank = hpwh.tank();
  ASSERT_FALSE(tank.handle().isNull());

  auto zoneHVAC = tank.containingZoneHVACComponent();
  ASSERT_TRUE(zoneHVAC);
  EXPECT_EQ(hpwh,zoneHVAC.get());
}

TEST_F(ModelFixture,WaterHeaterHeatPump_Remove)
{
  Model m;
  WaterHeaterHeatPump hpwh(m);

  auto dxCoil = hpwh.dXCoil();
  auto fan = hpwh.fan();
  auto tank = hpwh.tank();
  
  hpwh.remove();

  EXPECT_TRUE(hpwh.handle().isNull());
  EXPECT_TRUE(dxCoil.handle().isNull());
  EXPECT_TRUE(fan.handle().isNull());
  EXPECT_TRUE(tank.handle().isNull());
}

TEST_F(ModelFixture,WaterHeaterHeatPump_ThermalZone)
{
  Model m;
  WaterHeaterHeatPump hpwh(m);

  ThermalZone zone(m);
  EXPECT_TRUE(hpwh.addToThermalZone(zone));
  EXPECT_EQ(1u,zone.equipment().size());

  hpwh.remove();
  EXPECT_TRUE(hpwh.handle().isNull());
  EXPECT_TRUE(zone.equipment().empty());
}

TEST_F(ModelFixture,WaterHeaterHeatPump_SystemConnections)
{
  Model m;
  WaterHeaterHeatPump hpwh(m);

  PlantLoop plant(m);
  auto demandCount = plant.supplyComponents().size();
  auto tank = hpwh.tank();
  EXPECT_TRUE(plant.addSupplyBranchForComponent(tank));
  EXPECT_TRUE(plant.supplyComponent(tank.handle()));

  hpwh.remove();

  EXPECT_EQ(demandCount,plant.supplyComponents().size());
  EXPECT_TRUE(hpwh.handle().isNull());
  EXPECT_TRUE(tank.handle().isNull());
}

