/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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

