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
#include "../CoilChillerAirSourceVariableSpeed.hpp"
#include "../CoilChillerAirSourceVariableSpeed_Impl.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpVariableSpeed.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpVariableSpeed_Impl.hpp"
#include "../CoilCoolingWater.hpp"
#include "../CoilCoolingWater_Impl.hpp"
#include "../ThermalStorageIceDetailed.hpp"
#include "../ThermalStorageIceDetailed_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

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
  EXPECT_FALSE(coilSystem.spaceHeatingCoil().optionalCast<CoilHeatingDXVariableSpeed>());
  EXPECT_FALSE(coilSystem.dedicatedWaterHeatingCoil());
  EXPECT_FALSE(coilSystem.scwhCoil());
  EXPECT_FALSE(coilSystem.scdwhCoolingCoil());
  EXPECT_FALSE(coilSystem.scdwhWaterHeatingCoil());
  EXPECT_FALSE(coilSystem.shdwhHeatingCoil());
  EXPECT_FALSE(coilSystem.shdwhWaterHeatingCoil());
  EXPECT_FALSE(coilSystem.enhancedDehumidificationCoolingCoil());
  EXPECT_FALSE(coilSystem.gridResponseCoolingCoil());
  EXPECT_FALSE(coilSystem.gridResponseHeatingCoil());
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
  EXPECT_EQ(1.0, coilSystem.sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil());
  EXPECT_EQ(1.0, coilSystem.sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil());
  EXPECT_EQ(1.0, coilSystem.sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil());
  EXPECT_EQ(1.0, coilSystem.sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil());
  EXPECT_EQ(0.15, coilSystem.sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil());
  EXPECT_EQ(1.0, coilSystem.sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil());
  EXPECT_EQ(0.1, coilSystem.sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil());
  EXPECT_EQ(0.9, coilSystem.sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil());
  EXPECT_EQ(0.9, coilSystem.sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil());
  EXPECT_EQ(0.9, coilSystem.sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil());
  EXPECT_FALSE(coilSystem.chillerCoil());
  EXPECT_EQ("Single", coilSystem.chillerCoilBelongstoaSingleorSeparateUnit());
  EXPECT_EQ(1, coilSystem.chillerCoilCompressorRunSpeed());
  EXPECT_EQ(1.0, coilSystem.sizingRatioofChillerCoiltoSpaceCoolingCoil());
  EXPECT_FALSE(coilSystem.supplementalChillerCoil());
  EXPECT_EQ(1.0, coilSystem.airFlowRatioofWaterCoiltotheSpaceCoolingCoil());
  EXPECT_EQ(1.0, coilSystem.waterFlowRatioofWaterCoiltotheChillerCoil());
  EXPECT_FALSE(coilSystem.storageTank());
  EXPECT_EQ(0.9, coilSystem.iceFractionBelowWhichChargingStarts());
  EXPECT_EQ(-0.5, coilSystem.chillerEnteringTemperatureatZeroTankFraction());
  EXPECT_FALSE(coilSystem.temperatureDeviationCurve());
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
  CoilCoolingDXVariableSpeed enhancedDehumidificationCoolingCoil(m);
  CoilCoolingDXVariableSpeed gridResponseCoolingCoil(m);
  CoilHeatingDXVariableSpeed gridResponseHeatingCoil(m);
  CoilChillerAirSourceVariableSpeed chillerCoil(m);
  CoilCoolingWater supplementalChillerCoil(m);
  ThermalStorageIceDetailed thermalStorage(m);
  auto curve = CurveQuadratic(m);

  EXPECT_TRUE(coilSystem.setDedicatedWaterHeatingCoil(dedicatedWaterHeatingCoil));
  EXPECT_TRUE(coilSystem.setSCWHCoil(scwhCoil));
  EXPECT_TRUE(coilSystem.setSCDWHCoolingCoil(scdwhCoolingCoil));
  EXPECT_TRUE(coilSystem.setSCDWHWaterHeatingCoil(scdwhWaterHeatingCoil));
  EXPECT_TRUE(coilSystem.setSHDWHHeatingCoil(shdwhHeatingCoil));
  EXPECT_TRUE(coilSystem.setSHDWHWaterHeatingCoil(shdwhWaterHeatingCoil));
  EXPECT_TRUE(coilSystem.setEnhancedDehumidificationCoolingCoil(enhancedDehumidificationCoolingCoil));
  EXPECT_TRUE(coilSystem.setGridResponseCoolingCoil(gridResponseCoolingCoil));
  EXPECT_TRUE(coilSystem.setGridResponseHeatingCoil(gridResponseHeatingCoil));
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
  EXPECT_TRUE(coilSystem.setSizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil(1.5));
  EXPECT_TRUE(coilSystem.setSizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil(2.0));
  EXPECT_TRUE(coilSystem.setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil(2.5));
  EXPECT_TRUE(coilSystem.setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil(3.0));
  EXPECT_TRUE(coilSystem.setSizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(3.5));
  EXPECT_TRUE(coilSystem.setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil(4.0));
  EXPECT_TRUE(coilSystem.setSizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil(4.5));
  EXPECT_TRUE(coilSystem.setSizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil(4.9));
  EXPECT_TRUE(coilSystem.setSizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil(1.2));
  EXPECT_TRUE(coilSystem.setSizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil(1.4));
  EXPECT_TRUE(coilSystem.setChillerCoil(chillerCoil));
  EXPECT_TRUE(coilSystem.setChillerCoilBelongstoaSingleorSeparateUnit("Separate"));
  EXPECT_TRUE(coilSystem.setChillerCoilCompressorRunSpeed(2));
  EXPECT_TRUE(coilSystem.setSizingRatioofChillerCoiltoSpaceCoolingCoil(1.6));
  EXPECT_TRUE(coilSystem.setSupplementalChillerCoil(supplementalChillerCoil));
  EXPECT_TRUE(coilSystem.setAirFlowRatioofWaterCoiltotheSpaceCoolingCoil(1.25));
  EXPECT_TRUE(coilSystem.setWaterFlowRatioofWaterCoiltotheChillerCoil(1.75));
  EXPECT_TRUE(coilSystem.setStorageTank(thermalStorage));
  EXPECT_TRUE(coilSystem.setIceFractionBelowWhichChargingStarts(0.8));
  EXPECT_TRUE(coilSystem.setChillerEnteringTemperatureatZeroTankFraction(-0.4));
  EXPECT_TRUE(coilSystem.setTemperatureDeviationCurve(curve));

  EXPECT_EQ(spaceCoolingCoil.name().get(), coilSystem.spaceCoolingCoil().name().get());
  EXPECT_EQ(spaceHeatingCoil.name().get(), coilSystem.spaceCHeatingCoil().name().get());
  EXPECT_TRUE(coilSystem.dedicatedWaterHeatingCoil());
  EXPECT_TRUE(coilSystem.scwhCoil());
  EXPECT_TRUE(coilSystem.scdwhCoolingCoil());
  EXPECT_TRUE(coilSystem.scdwhWaterHeatingCoil());
  EXPECT_TRUE(coilSystem.shdwhHeatingCoil());
  EXPECT_TRUE(coilSystem.shdwhWaterHeatingCoil());
  EXPECT_TRUE(coilSystem.enhancedDehumidificationCoolingCoil());
  EXPECT_TRUE(coilSystem.gridResponseCoolingCoil());
  EXPECT_TRUE(coilSystem.gridResponseHeatingCoil());
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
  EXPECT_EQ(1.5, coilSystem.sizingRatioofSpaceHeatingCoiltoSpaceCoolingCoil());
  EXPECT_EQ(2.0, coilSystem.sizingRatioofDedicatedWaterHeatingCoiltoSpaceCoolingCoil());
  EXPECT_EQ(2.5, coilSystem.sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithFullCondensingtoSpaceCoolingCoil());
  EXPECT_EQ(3.0, coilSystem.sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingCoolingCapacitytoSpaceCoolingCoil());
  EXPECT_EQ(3.5, coilSystem.sizingRatioofCombinedSpaceCoolingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil());
  EXPECT_EQ(4.0, coilSystem.sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingSpaceHeatingCapacitytoSpaceCoolingCoil());
  EXPECT_EQ(4.5, coilSystem.sizingRatioofCombinedSpaceHeatingandWaterHeatingCoilwithDesuperheatingWaterHeatingCapacitytoSpaceCoolingCoil());
  EXPECT_EQ(4.9, coilSystem.sizingRatioofEnhancedDehumidificationCoiltoSpaceCoolingCoil());
  EXPECT_EQ(1.2, coilSystem.sizingRatioofGridResponseCoolingCoiltoSpaceCoolingCoil());
  EXPECT_EQ(1.4, coilSystem.sizingRatioofGridResponseHeatingCoiltoSpaceCoolingCoil());
  EXPECT_TRUE(coilSystem.chillerCoil());
  EXPECT_EQ("Separate", coilSystem.chillerCoilBelongstoaSingleorSeparateUnit());
  EXPECT_EQ(2, coilSystem.chillerCoilCompressorRunSpeed());
  EXPECT_EQ(1.6, coilSystem.sizingRatioofChillerCoiltoSpaceCoolingCoil());
  EXPECT_TRUE(coilSystem.supplementalChillerCoil());
  EXPECT_EQ(1.25, coilSystem.airFlowRatioofWaterCoiltotheSpaceCoolingCoil());
  EXPECT_EQ(1.75, coilSystem.waterFlowRatioofWaterCoiltotheChillerCoil());
  EXPECT_TRUE(coilSystem.storageTank());
  EXPECT_EQ(0.8, coilSystem.iceFractionBelowWhichChargingStarts());
  EXPECT_EQ(-0.4, coilSystem.chillerEnteringTemperatureatZeroTankFraction());
  EXPECT_TRUE(coilSystem.temperatureDeviationCurve());

  ASSERT_TRUE(spaceCoolingCoil.containingHVACComponent());
  EXPECT_TRUE(spaceCoolingCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(spaceHeatingCoil.containingHVACComponent());
  EXPECT_TRUE(spaceHeatingCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(scdwhCoolingCoil.containingHVACComponent());
  EXPECT_TRUE(scdwhCoolingCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(shdwhHeatingCoil.containingHVACComponent());
  EXPECT_TRUE(shdwhHeatingCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(enhancedDehumidificationCoolingCoil.containingHVACComponent());
  EXPECT_TRUE(enhancedDehumidificationCoolingCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(gridResponseCoolingCoil.containingHVACComponent());
  EXPECT_TRUE(gridResponseCoolingCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(gridResponseHeatingCoil.containingHVACComponent());
  EXPECT_TRUE(gridResponseHeatingCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(chillerCoil.containingHVACComponent());
  EXPECT_TRUE(chillerCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(supplementalChillerCoil.containingHVACComponent());
  EXPECT_TRUE(supplementalChillerCoil.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  ASSERT_TRUE(thermalStorage.containingHVACComponent());
  EXPECT_TRUE(thermalStorage.containingHVACComponent().get().optionalCast<CoilSystemIntegratedHeatPumpAirSource>());

  coilSystem.resetDedicatedWaterHeatingCoil();
  coilSystem.resetSCWHCoil();
  coilSystem.resetSCDWHCoolingCoil();
  coilSystem.resetSCDWHWaterHeatingCoil();
  coilSystem.resetSHDWHHeatingCoil();
  coilSystem.resetSHDWHWaterHeatingCoil();
  coilSystem.resetEnhancedDehumidificationCoolingCoil();
  coilSystem.resetGridResponseCoolingCoil();
  coilSystem.resetGridResponseHeatingCoil();
  coilSystem.resetChillerCoil();
  coilSystem.resetSupplementalChillerCoil();
  coilSystem.resetStorageTank();
  coilSystem.resetTemperatureDeviationCurve();

  EXPECT_FALSE(coilSystem.dedicatedWaterHeatingCoil());
  EXPECT_FALSE(coilSystem.scwhCoil());
  EXPECT_FALSE(coilSystem.scdwhCoolingCoil());
  EXPECT_FALSE(coilSystem.scdwhWaterHeatingCoil());
  EXPECT_FALSE(coilSystem.shdwhHeatingCoil());
  EXPECT_FALSE(coilSystem.shdwhWaterHeatingCoil());
  EXPECT_FALSE(coilSystem.enhancedDehumidificationCoolingCoil());
  EXPECT_FALSE(coilSystem.gridResponseCoolingCoil());
  EXPECT_FALSE(coilSystem.gridResponseHeatingCoil());
  EXPECT_FALSE(coilSystem.chillerCoil());
  EXPECT_FALSE(coilSystem.supplementalChillerCoil());
  EXPECT_FALSE(coilSystem.storageTank());
  EXPECT_FALSE(coilSystem.temperatureDeviationCurve());

  ASSERT_TRUE(spaceCoolingCoil.containingHVACComponent());
  ASSERT_TRUE(spaceHeatingCoil.containingHVACComponent());
  ASSERT_FALSE(scdwhCoolingCoil.containingHVACComponent());
  ASSERT_FALSE(shdwhHeatingCoil.containingHVACComponent());
  ASSERT_FALSE(enhancedDehumidificationCoolingCoil.containingHVACComponent());
  ASSERT_FALSE(gridResponseCoolingCoil.containingHVACComponent());
  ASSERT_FALSE(gridResponseHeatingCoil.containingHVACComponent());
  ASSERT_FALSE(chillerCoil.containingHVACComponent());
  ASSERT_FALSE(supplementalChillerCoil.containingHVACComponent());
  ASSERT_FALSE(thermalStorage.containingHVACComponent());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_Clone) {
  Model m;

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil);

  CoilSystemIntegratedHeatPumpAirSource coilSystemClone = coilSystem.clone(m).cast<CoilSystemIntegratedHeatPumpAirSource>();
  ASSERT_TRUE(coilSystemClone.spaceHeatingCoil());

  Model m2;
  CoilSystemIntegratedHeatPumpAirSource coilSystemClone2 = coilSystem.clone(m2).cast<CoilSystemIntegratedHeatPumpAirSource>();
  ASSERT_TRUE(coilSystemClone2.spaceHeatingCoil());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_Remove) {
  Model m;

  auto size = m.modelObjects().size();

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil);

  EXPECT_EQ(size + 4, m.modelObjects().size());  // 4: CoilSystem, Coil, Curve, SpeedData
  EXPECT_FALSE(coilSystem.remove().empty());
  EXPECT_EQ(size + 1, m.modelObjects().size());  // 1: Curve
}
