/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../CoilSystemIntegratedHeatPumpAirSource.hpp"
#include "../CoilSystemIntegratedHeatPumpAirSource_Impl.hpp"
#include "../CoilCoolingDXVariableSpeed.hpp"
#include "../CoilCoolingDXVariableSpeed_Impl.hpp"
#include "../CoilHeatingDXVariableSpeed.hpp"
#include "../CoilHeatingDXVariableSpeed_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_CoilSystemIntegratedHeatPumpAirSource) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
      CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
      CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil);

  EXPECT_TRUE(coilSystem.spaceCoolingCoil().optionalCast<CoilCoolingDXVariableSpeed>());
  EXPECT_TRUE(coilSystem.spaceHeatingCoil().optionalCast<CoilHeatingDXVariableSpeed>());
  EXPECT_FALSE(coilSystem.dedicatedWaterHeatingCoil());
  EXPECT_FALSE(coilSystem.scwhCoil());
  EXPECT_FALSE(coilSystem.scdwhCoolingCoil());
  EXPECT_FALSE(coilSystem.scdwhWaterHeatingCoil());
  EXPECT_FALSE(coilSystem.shdwhHeatingCoil());
  EXPECT_FALSE(coilSystem.shdwhWaterHeatingCoil());
  EXPECT_EQ(20.0, coilSystem.indoorTemperatureLimitForSCWHMode());
  EXPECT_EQ(27.0, coilSystem.ambientTemperatureLimitForSCWHMode());
  EXPECT_EQ(20.0, coilSystem.indoorTemperatureAboveWhichWHHasHigherPriority());
  EXPECT_EQ(20.0, coilSystem.ambientTemperatureAboveWhichWHHasHigherPriority());
  EXPECT_EQ(0, coilSystem.flagtoIndicateLoadControlInSCWHMode());
  EXPECT_EQ(1, coilSystem.minimumSpeedLevelForSCWHMode());
  EXPECT_EQ(0.0, coilSystem.maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode());
  EXPECT_EQ(1, coilSystem.minimumSpeedLevelForSCDWHMode());
  EXPECT_EQ(360.0, coilSystem.maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode());
  EXPECT_EQ(1, coilSystem.minimumSpeedLevelForSHDWHMode());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_SetGetFields) {
  Model m;

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil);

  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed dedicatedWaterHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scwhCoil(m);
  CoilCoolingDXVariableSpeed scdwhCoolingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed scdwhWaterHeatingCoil(m);
  CoilHeatingDXVariableSpeed shdwhHeatingCoil(m);
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeed shdwhWaterHeatingCoil(m);

  EXPECT_TRUE(coilSystem.setDedicatedWaterHeatingCoil(dedicatedWaterHeatingCoil));
  EXPECT_TRUE(coilSystem.setSCWHCoil(scwhCoil));
  EXPECT_TRUE(coilSystem.setSCDWHCoolingCoil(scdwhCoolingCoil));
  EXPECT_TRUE(coilSystem.setSCDWHWaterHeatingCoil(scdwhWaterHeatingCoil));
  EXPECT_TRUE(coilSystem.setSHDWHHeatingCoil(shdwhHeatingCoil));
  EXPECT_TRUE(coilSystem.setSHDWHWaterHeatingCoil(shdwhWaterHeatingCoil));
  EXPECT_TRUE(coilSystem.setIndoorTemperatureLimitForSCWHMode(21.0));
  EXPECT_TRUE(coilSystem.setAmbientTemperatureLimitForSCWHMode(28.0));
  EXPECT_TRUE(coilSystem.setIndoorTemperatureAboveWhichWHHasHigherPriority(22.0));
  EXPECT_TRUE(coilSystem.setAmbientTemperatureAboveWhichWHHasHigherPriority(23.0));
  EXPECT_TRUE(coilSystem.setFlagtoIndicateLoadControlInSCWHMode(1));
  EXPECT_TRUE(coilSystem.setMinimumSpeedLevelForSCWHMode(2));
  EXPECT_TRUE(coilSystem.setMaximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode(1.0));
  EXPECT_TRUE(coilSystem.setMinimumSpeedLevelForSCDWHMode(3));
  EXPECT_TRUE(coilSystem.setMaximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode(380.0));
  EXPECT_TRUE(coilSystem.setMinimumSpeedLevelForSHDWHMode(4));

  EXPECT_EQ(spaceCoolingCoil.name().get(), coilSystem.spaceCoolingCoil().name().get());
  EXPECT_EQ(spaceHeatingCoil.name().get(), coilSystem.spaceHeatingCoil().name().get());
  ASSERT_TRUE(coilSystem.dedicatedWaterHeatingCoil());
  ASSERT_TRUE(coilSystem.scwhCoil());
  ASSERT_TRUE(coilSystem.scdwhCoolingCoil());
  ASSERT_TRUE(coilSystem.scdwhWaterHeatingCoil());
  ASSERT_TRUE(coilSystem.shdwhHeatingCoil());
  ASSERT_TRUE(coilSystem.shdwhWaterHeatingCoil());
  EXPECT_EQ(dedicatedWaterHeatingCoil, coilSystem.dedicatedWaterHeatingCoil().get());
  EXPECT_EQ(scwhCoil, coilSystem.scwhCoil().get());
  EXPECT_EQ(scdwhCoolingCoil, coilSystem.scdwhCoolingCoil().get());
  EXPECT_EQ(scdwhWaterHeatingCoil, coilSystem.scdwhWaterHeatingCoil().get());
  EXPECT_EQ(shdwhHeatingCoil, coilSystem.shdwhHeatingCoil().get());
  EXPECT_EQ(shdwhWaterHeatingCoil, coilSystem.shdwhWaterHeatingCoil().get());

  EXPECT_EQ(21.0, coilSystem.indoorTemperatureLimitForSCWHMode());
  EXPECT_EQ(28.0, coilSystem.ambientTemperatureLimitForSCWHMode());
  EXPECT_EQ(22.0, coilSystem.indoorTemperatureAboveWhichWHHasHigherPriority());
  EXPECT_EQ(23.0, coilSystem.ambientTemperatureAboveWhichWHHasHigherPriority());
  EXPECT_EQ(1, coilSystem.flagtoIndicateLoadControlInSCWHMode());
  EXPECT_EQ(2, coilSystem.minimumSpeedLevelForSCWHMode());
  EXPECT_EQ(1.0, coilSystem.maximumWaterFlowVolumeBeforeSwitchingfromSCDWHtoSCWHMode());
  EXPECT_EQ(3, coilSystem.minimumSpeedLevelForSCDWHMode());
  EXPECT_EQ(380.0, coilSystem.maximumRunningTimeBeforeAllowingElectricResistanceHeatUseDuringSHDWHMode());
  EXPECT_EQ(4, coilSystem.minimumSpeedLevelForSHDWHMode());

  ASSERT_TRUE(spaceCoolingCoil.containingHVACComponent());
  EXPECT_TRUE(spaceCoolingCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(spaceHeatingCoil.containingHVACComponent());
  EXPECT_TRUE(spaceHeatingCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(scdwhCoolingCoil.containingHVACComponent());
  EXPECT_TRUE(scdwhCoolingCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(shdwhHeatingCoil.containingHVACComponent());
  EXPECT_TRUE(shdwhHeatingCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());

  coilSystem.resetDedicatedWaterHeatingCoil();
  coilSystem.resetSCWHCoil();
  coilSystem.resetSCDWHCoolingCoil();
  coilSystem.resetSCDWHWaterHeatingCoil();
  coilSystem.resetSHDWHHeatingCoil();
  coilSystem.resetSHDWHWaterHeatingCoil();

  EXPECT_FALSE(coilSystem.dedicatedWaterHeatingCoil());
  EXPECT_FALSE(coilSystem.scwhCoil());
  EXPECT_FALSE(coilSystem.scdwhCoolingCoil());
  EXPECT_FALSE(coilSystem.scdwhWaterHeatingCoil());
  EXPECT_FALSE(coilSystem.shdwhHeatingCoil());
  EXPECT_FALSE(coilSystem.shdwhWaterHeatingCoil());

  ASSERT_TRUE(spaceCoolingCoil.containingHVACComponent());
  ASSERT_TRUE(spaceHeatingCoil.containingHVACComponent());
  ASSERT_FALSE(scdwhCoolingCoil.containingHVACComponent());
  ASSERT_FALSE(shdwhHeatingCoil.containingHVACComponent());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_Clone) {
  Model m;

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil);

  CoilSystemIntegratedHeatPumpAirSource coilSystemClone = coilSystem.clone(m).cast<CoilSystemIntegratedHeatPumpAirSource>();
  ASSERT_TRUE(coilSystemClone.spaceHeatingCoil().optionalCast<StraightComponent>());

  Model m2;
  CoilSystemIntegratedHeatPumpAirSource coilSystemClone2 = coilSystem.clone(m2).cast<CoilSystemIntegratedHeatPumpAirSource>();
  ASSERT_TRUE(coilSystemClone2.spaceHeatingCoil().optionalCast<StraightComponent>());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_Remove) {
  Model m;

  auto size = m.modelObjects().size();

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil);

  EXPECT_EQ(size + 7, m.modelObjects().size());  // 4: CoilSystem, Coils, Curves, SpeedDatas
  EXPECT_FALSE(coilSystem.remove().empty());
  EXPECT_EQ(size + 2, m.modelObjects().size());  // 1: Curve
}
