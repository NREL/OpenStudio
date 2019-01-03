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

#include "../ZoneHVACUnitVentilator.hpp"
#include "../ZoneHVACUnitVentilator_Impl.hpp"
#include "../CoilHeatingWater.hpp"
#include "../CoilHeatingWater_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilCoolingWater_Impl.hpp"
#include "../FanVariableVolume.hpp"
#include "../FanVariableVolume_Impl.hpp"
#include "../Schedule.hpp"
#include "../Schedule_Impl.hpp"
#include "../ThermalZone.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ZoneHVACUnitVentilator)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
    Model m;
    ZoneHVACUnitVentilator zonehvac(m);

    exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

TEST_F(ModelFixture, ZoneHVACUnitVentilator_AddAndRemove)
{
  Model m;
  ZoneHVACUnitVentilator zonehvac(m);

  ThermalZone tz(m);
  ASSERT_TRUE(zonehvac.addToThermalZone(tz));
  ASSERT_TRUE(zonehvac.thermalZone());
  ASSERT_EQ(tz,zonehvac.thermalZone().get());
  ASSERT_EQ(1u,tz.equipment().size());
  zonehvac.removeFromThermalZone();
  ASSERT_EQ(0u,tz.equipment().size());

  ZoneHVACUnitVentilator zonehvac2(m);
  zonehvac2.addToThermalZone(tz);
  zonehvac2.remove();
  ASSERT_EQ(0u,tz.equipment().size());
}

TEST_F(ModelFixture, ZoneHVACUnitVentilator_CloneOneModelWithDefaultData)
{
  Model model;
  ZoneHVACUnitVentilator testObject(model);

  ZoneHVACUnitVentilator testObjectClone = testObject.clone(model).cast<ZoneHVACUnitVentilator>();

  EXPECT_TRUE(testObjectClone.isMaximumSupplyAirFlowRateAutosized());
  EXPECT_EQ("VariablePercent", testObjectClone.outdoorAirControlType());
  EXPECT_TRUE(testObjectClone.isMinimumOutdoorAirFlowRateAutosized());
  EXPECT_TRUE(testObjectClone.isMaximumOutdoorAirFlowRateAutosized());
  EXPECT_DOUBLE_EQ(0.001, testObjectClone.heatingConvergenceTolerance());
  EXPECT_DOUBLE_EQ(0.001, testObjectClone.coolingConvergenceTolerance());

  EXPECT_NE(testObject.supplyAirFan().handle(), testObjectClone.supplyAirFan().handle());
}

TEST_F(ModelFixture, ZoneHVACUnitVentilator_CloneOneModelWithCustomData)
{
  Model model;

  FanVariableVolume fan(model);
  CoilHeatingWater heatingCoil(model);
  CoilCoolingWater coolingCoil(model);

  ZoneHVACUnitVentilator testObject(model, fan);
  testObject.setMaximumSupplyAirFlowRate(999);
  testObject.setOutdoorAirControlType("FixedAmount");
  testObject.setMinimumOutdoorAirFlowRate(999);
  testObject.setMaximumOutdoorAirFlowRate(999);
  testObject.setHeatingCoil(heatingCoil);
  testObject.setHeatingConvergenceTolerance(999);
  testObject.setCoolingCoil(coolingCoil);
  testObject.setCoolingConvergenceTolerance(999);

  ZoneHVACUnitVentilator testObjectClone = testObject.clone(model).cast<ZoneHVACUnitVentilator>();

  EXPECT_FALSE(testObjectClone.isMaximumSupplyAirFlowRateAutosized());
  EXPECT_DOUBLE_EQ(999, testObjectClone.maximumSupplyAirFlowRate().get());
  EXPECT_EQ("FixedAmount", testObjectClone.outdoorAirControlType());
  EXPECT_FALSE(testObjectClone.isMinimumOutdoorAirFlowRateAutosized());
  EXPECT_DOUBLE_EQ(999, testObjectClone.minimumOutdoorAirFlowRate().get());
  EXPECT_FALSE(testObjectClone.isMaximumOutdoorAirFlowRateAutosized());
  EXPECT_DOUBLE_EQ(999, testObjectClone.maximumOutdoorAirFlowRate().get());
  EXPECT_NE(fan.handle(), testObjectClone.supplyAirFan().handle());
  ASSERT_TRUE(testObjectClone.heatingCoil());
  EXPECT_NE(heatingCoil.handle(), testObjectClone.heatingCoil()->handle());
  EXPECT_DOUBLE_EQ(999, testObjectClone.heatingConvergenceTolerance());
  ASSERT_TRUE(testObjectClone.coolingCoil());
  EXPECT_NE(coolingCoil.handle(), testObjectClone.coolingCoil()->handle());
  EXPECT_DOUBLE_EQ(999, testObjectClone.coolingConvergenceTolerance());

  EXPECT_NE(testObject.supplyAirFan().handle(), testObjectClone.supplyAirFan().handle());
  EXPECT_NE(testObject.heatingCoil()->handle(), testObjectClone.heatingCoil()->handle());
  EXPECT_NE(testObject.coolingCoil()->handle(), testObjectClone.coolingCoil()->handle());
}

TEST_F(ModelFixture, ZoneHVACUnitVentilator_CloneTwoModelsWithDefaultData)
{
  Model model;
  ZoneHVACUnitVentilator testObject(model);

  ZoneHVACUnitVentilator testObjectClone = testObject.clone(model).cast<ZoneHVACUnitVentilator>();

  Model model2;

  ZoneHVACUnitVentilator testObjectClone2 = testObject.clone(model2).cast<ZoneHVACUnitVentilator>();

  EXPECT_TRUE(testObjectClone.isMaximumSupplyAirFlowRateAutosized());
  EXPECT_EQ("VariablePercent", testObjectClone.outdoorAirControlType());
  EXPECT_TRUE(testObjectClone.isMinimumOutdoorAirFlowRateAutosized());
  EXPECT_TRUE(testObjectClone.isMaximumOutdoorAirFlowRateAutosized());
  EXPECT_DOUBLE_EQ(0.001, testObjectClone.heatingConvergenceTolerance());
  EXPECT_DOUBLE_EQ(0.001, testObjectClone.coolingConvergenceTolerance());

  EXPECT_NE(testObject.supplyAirFan().handle(), testObjectClone.supplyAirFan().handle());
  EXPECT_NE(testObjectClone.supplyAirFan().handle(), testObjectClone2.supplyAirFan().handle());

  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}

TEST_F(ModelFixture, ZoneHVACUnitVentilator_CloneTwoModelWithCustomData)
{
  Model model;

  FanVariableVolume fan(model);
  CoilHeatingWater heatingCoil(model);
  CoilCoolingWater coolingCoil(model);

  ZoneHVACUnitVentilator testObject(model, fan);
  testObject.setMaximumSupplyAirFlowRate(999);
  testObject.setOutdoorAirControlType("FixedAmount");
  testObject.setMinimumOutdoorAirFlowRate(999);
  testObject.setMaximumOutdoorAirFlowRate(999);
  testObject.setHeatingCoil(heatingCoil);
  testObject.setHeatingConvergenceTolerance(999);
  testObject.setCoolingCoil(coolingCoil);
  testObject.setCoolingConvergenceTolerance(999);

  ZoneHVACUnitVentilator testObjectClone = testObject.clone(model).cast<ZoneHVACUnitVentilator>();

  Model model2;
  ZoneHVACUnitVentilator testObjectClone2 = testObject.clone(model2).cast<ZoneHVACUnitVentilator>();

  EXPECT_FALSE(testObjectClone2.isMaximumSupplyAirFlowRateAutosized());
  EXPECT_DOUBLE_EQ(999, testObjectClone2.maximumSupplyAirFlowRate().get());
  EXPECT_EQ("FixedAmount", testObjectClone2.outdoorAirControlType());
  EXPECT_FALSE(testObjectClone2.isMinimumOutdoorAirFlowRateAutosized());
  EXPECT_DOUBLE_EQ(999, testObjectClone2.minimumOutdoorAirFlowRate().get());
  EXPECT_FALSE(testObjectClone2.isMaximumOutdoorAirFlowRateAutosized());
  EXPECT_DOUBLE_EQ(999, testObjectClone2.maximumOutdoorAirFlowRate().get());
  EXPECT_NE(fan.handle(), testObjectClone2.supplyAirFan().handle());
  ASSERT_TRUE(testObjectClone2.heatingCoil());
  EXPECT_NE(heatingCoil.handle(), testObjectClone2.heatingCoil()->handle());
  EXPECT_DOUBLE_EQ(999, testObjectClone2.heatingConvergenceTolerance());
  ASSERT_TRUE(testObjectClone2.coolingCoil());
  EXPECT_NE(coolingCoil.handle(), testObjectClone2.coolingCoil()->handle());
  EXPECT_DOUBLE_EQ(999, testObjectClone2.coolingConvergenceTolerance());

  EXPECT_NE(testObject.supplyAirFan().handle(), testObjectClone2.supplyAirFan().handle());
  EXPECT_NE(testObject.heatingCoil()->handle(), testObjectClone2.heatingCoil()->handle());
  EXPECT_NE(testObject.coolingCoil()->handle(), testObjectClone2.coolingCoil()->handle());

  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());

}
