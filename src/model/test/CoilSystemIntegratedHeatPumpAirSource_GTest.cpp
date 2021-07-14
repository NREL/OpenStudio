/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../WaterHeaterHeatPump.hpp"
#include "../AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "../Schedule.hpp"
#include "../FanConstantVolume.hpp"
#include "../CoilHeatingElectric.hpp"

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

  EXPECT_EQ(spaceCoolingCoil, coilSystem.spaceCoolingCoil());
  EXPECT_EQ(spaceHeatingCoil, coilSystem.spaceHeatingCoil());
  EXPECT_EQ(dedicatedWaterHeatingCoil, coilSystem.dedicatedWaterHeatingCoil().get());
  EXPECT_EQ(scwhCoil, coilSystem.scwhCoil().get());
  EXPECT_EQ(scdwhCoolingCoil, coilSystem.scdwhCoolingCoil().get());
  EXPECT_EQ(scdwhWaterHeatingCoil, coilSystem.scdwhWaterHeatingCoil().get());
  EXPECT_EQ(shdwhHeatingCoil, coilSystem.shdwhHeatingCoil().get());
  EXPECT_EQ(shdwhWaterHeatingCoil, coilSystem.shdwhWaterHeatingCoil().get());

  // Test containingHVACComponent
  ASSERT_TRUE(spaceCoolingCoil.containingHVACComponent());
  ASSERT_TRUE(spaceCoolingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, spaceCoolingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(spaceHeatingCoil.containingHVACComponent());
  ASSERT_TRUE(spaceHeatingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, spaceHeatingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(dedicatedWaterHeatingCoil.containingHVACComponent());
  ASSERT_TRUE(dedicatedWaterHeatingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, dedicatedWaterHeatingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(scwhCoil.containingHVACComponent());
  ASSERT_TRUE(scwhCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, scwhCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(scdwhCoolingCoil.containingHVACComponent());
  ASSERT_TRUE(scdwhCoolingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, scdwhCoolingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(scdwhWaterHeatingCoil.containingHVACComponent());
  ASSERT_TRUE(scdwhWaterHeatingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, scdwhWaterHeatingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(shdwhHeatingCoil.containingHVACComponent());
  ASSERT_TRUE(shdwhHeatingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, shdwhHeatingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  ASSERT_TRUE(shdwhWaterHeatingCoil.containingHVACComponent());
  ASSERT_TRUE(shdwhWaterHeatingCoil.containingHVACComponent()->optionalCast<CoilSystemIntegratedHeatPumpAirSource>());
  EXPECT_EQ(coilSystem, shdwhWaterHeatingCoil.containingHVACComponent()->cast<CoilSystemIntegratedHeatPumpAirSource>());

  // Test regular fields
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
  EXPECT_EQ(spaceHeatingCoil.name().get(), coilSystem.spaceHeatingCoil().name().get());
  ASSERT_TRUE(coilSystem.dedicatedWaterHeatingCoil());
  ASSERT_TRUE(coilSystem.scwhCoil());
  ASSERT_TRUE(coilSystem.scdwhCoolingCoil());
  ASSERT_TRUE(coilSystem.scdwhWaterHeatingCoil());
  ASSERT_TRUE(coilSystem.shdwhHeatingCoil());
  ASSERT_TRUE(coilSystem.shdwhWaterHeatingCoil());
  ASSERT_TRUE(coilSystem.enhancedDehumidificationCoolingCoil());
  ASSERT_TRUE(coilSystem.gridResponseCoolingCoil());
  ASSERT_TRUE(coilSystem.gridResponseHeatingCoil());
  EXPECT_EQ(dedicatedWaterHeatingCoil, coilSystem.dedicatedWaterHeatingCoil().get());
  EXPECT_EQ(scwhCoil, coilSystem.scwhCoil().get());
  EXPECT_EQ(scdwhCoolingCoil, coilSystem.scdwhCoolingCoil().get());
  EXPECT_EQ(scdwhWaterHeatingCoil, coilSystem.scdwhWaterHeatingCoil().get());
  EXPECT_EQ(shdwhHeatingCoil, coilSystem.shdwhHeatingCoil().get());
  EXPECT_EQ(shdwhWaterHeatingCoil, coilSystem.shdwhWaterHeatingCoil().get());
  EXPECT_EQ(enhancedDehumidificationCoolingCoil, coilSystem.enhancedDehumidificationCoolingCoil().get());
  EXPECT_EQ(gridResponseCoolingCoil, coilSystem.gridResponseCoolingCoil().get());
  EXPECT_EQ(gridResponseHeatingCoil, coilSystem.gridResponseHeatingCoil().get());

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

  size_t nCoilCoolingDXVariableSpeed = 1;
  size_t nCoilHeatingDXVariableSpeed = 1;
  size_t nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed = 0;
  EXPECT_EQ(nCoilCoolingDXVariableSpeed, m.getConcreteModelObjects<CoilCoolingDXVariableSpeed>().size());
  EXPECT_EQ(nCoilHeatingDXVariableSpeed, m.getConcreteModelObjects<CoilHeatingDXVariableSpeed>().size());
  EXPECT_EQ(nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());

  // Cloning clones the coils too.

  {
    CoilSystemIntegratedHeatPumpAirSource coilSystemClone = coilSystem.clone(m).cast<CoilSystemIntegratedHeatPumpAirSource>();

    EXPECT_EQ(2 * nCoilCoolingDXVariableSpeed, m.getConcreteModelObjects<CoilCoolingDXVariableSpeed>().size());
    EXPECT_EQ(2 * nCoilHeatingDXVariableSpeed, m.getConcreteModelObjects<CoilHeatingDXVariableSpeed>().size());
    EXPECT_EQ(2 * nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed,
              m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());

    // It should have reassigned to the cloned coils too
    EXPECT_NE(coilSystem.spaceCoolingCoil(), coilSystemClone.spaceCoolingCoil());
    EXPECT_NE(coilSystem.spaceHeatingCoil(), coilSystemClone.spaceHeatingCoil());
    /*     EXPECT_NE(coilSystem.dedicatedWaterHeatingCoil(), coilSystemClone.dedicatedWaterHeatingCoil());
    EXPECT_NE(coilSystem.scwhCoil(), coilSystemClone.scwhCoil());
    EXPECT_NE(coilSystem.scdwhCoolingCoil(), coilSystemClone.scdwhCoolingCoil());
    EXPECT_NE(coilSystem.scdwhWaterHeatingCoil(), coilSystemClone.scdwhWaterHeatingCoil());
    EXPECT_NE(coilSystem.shdwhHeatingCoil(), coilSystemClone.shdwhHeatingCoil());
    EXPECT_NE(coilSystem.shdwhWaterHeatingCoil(), coilSystemClone.shdwhWaterHeatingCoil()); */
  }

  {
    Model m2;
    CoilSystemIntegratedHeatPumpAirSource coilSystemClone = coilSystem.clone(m2).cast<CoilSystemIntegratedHeatPumpAirSource>();

    EXPECT_EQ(nCoilCoolingDXVariableSpeed, m2.getConcreteModelObjects<CoilCoolingDXVariableSpeed>().size());
    EXPECT_EQ(nCoilHeatingDXVariableSpeed, m2.getConcreteModelObjects<CoilHeatingDXVariableSpeed>().size());
    EXPECT_EQ(nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed, m2.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());

    EXPECT_NO_THROW(coilSystemClone.spaceCoolingCoil());
    EXPECT_NO_THROW(coilSystemClone.spaceHeatingCoil());
    /*     EXPECT_NO_THROW(coilSystemClone.dedicatedWaterHeatingCoil());
    EXPECT_NO_THROW(coilSystemClone.scwhCoil());
    EXPECT_NO_THROW(coilSystemClone.scdwhCoolingCoil());
    EXPECT_NO_THROW(coilSystemClone.scdwhWaterHeatingCoil());
    EXPECT_NO_THROW(coilSystemClone.shdwhHeatingCoil());
    EXPECT_NO_THROW(coilSystemClone.shdwhWaterHeatingCoil()); */
  }
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_Remove) {
  Model m;

  CoilCoolingDXVariableSpeed spaceCoolingCoil(m);
  CoilHeatingDXVariableSpeed spaceHeatingCoil(m);
  CoilSystemIntegratedHeatPumpAirSource coilSystem(m, spaceCoolingCoil, spaceHeatingCoil);

  size_t nCoilCoolingDXVariableSpeed = 1;
  size_t nCoilHeatingDXVariableSpeed = 1;
  size_t nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed = 0;
  EXPECT_EQ(nCoilCoolingDXVariableSpeed, m.getConcreteModelObjects<CoilCoolingDXVariableSpeed>().size());
  EXPECT_EQ(nCoilHeatingDXVariableSpeed, m.getConcreteModelObjects<CoilHeatingDXVariableSpeed>().size());
  EXPECT_EQ(nCoilWaterHeatingAirToWaterHeatPumpVariableSpeed, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());

  EXPECT_FALSE(coilSystem.remove().empty());
  EXPECT_EQ(0, m.getConcreteModelObjects<CoilCoolingDXVariableSpeed>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<CoilHeatingDXVariableSpeed>().size());
  EXPECT_EQ(0, m.getConcreteModelObjects<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>().size());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_DefaultConstructor) {
  Model m;

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);

  EXPECT_TRUE(coilSystem.spaceCoolingCoil().optionalCast<CoilCoolingDXVariableSpeed>());
  EXPECT_TRUE(coilSystem.spaceHeatingCoil().optionalCast<CoilHeatingDXVariableSpeed>());
  EXPECT_TRUE(coilSystem.dedicatedWaterHeatingCoil().get().optionalCast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>());
  EXPECT_TRUE(coilSystem.scwhCoil().get().optionalCast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>());
  EXPECT_TRUE(coilSystem.scdwhCoolingCoil().get().optionalCast<CoilCoolingDXVariableSpeed>());
  EXPECT_TRUE(coilSystem.scdwhWaterHeatingCoil().get().optionalCast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>());
  EXPECT_TRUE(coilSystem.shdwhHeatingCoil().get().optionalCast<CoilHeatingDXVariableSpeed>());
  EXPECT_TRUE(coilSystem.shdwhWaterHeatingCoil().get().optionalCast<CoilWaterHeatingAirToWaterHeatPumpVariableSpeed>());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_containingHVACComponent_AirLoopHVACUnitaryHeatPumpAirToAir) {

  Model m;

  Schedule s = m.alwaysOnDiscreteSchedule();
  FanConstantVolume supplyFan(m, s);
  CoilHeatingElectric coilHeatingElectric(m, s);

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);
  EXPECT_FALSE(coilSystem.containingHVACComponent());

  AirLoopHVACUnitaryHeatPumpAirToAir unitary(m, s, supplyFan, coilSystem, coilSystem, coilHeatingElectric);
  EXPECT_EQ(coilSystem.handle(), unitary.coolingCoil().handle());
  EXPECT_EQ(coilSystem.handle(), unitary.heatingCoil().handle());

  // Test containingHVAC
  ASSERT_TRUE(coilSystem.containingHVACComponent());
  EXPECT_EQ(unitary.handle(), coilSystem.containingHVACComponent()->handle());
}

TEST_F(ModelFixture, CoilSystemIntegratedHeatPumpAirSource_containingHVACComponent_WaterHeaterHeatPumpPumpedCondenser) {

  Model m;

  CoilSystemIntegratedHeatPumpAirSource coilSystem(m);
  EXPECT_FALSE(coilSystem.containingHVACComponent());

  WaterHeaterHeatPump hpwh(m);
  EXPECT_TRUE(hpwh.setDXCoil(coilSystem));
  EXPECT_EQ(coilSystem.handle(), hpwh.dXCoil().handle());

  // Test containingHVAC
  ASSERT_TRUE(coilSystem.containingHVACComponent());
  EXPECT_EQ(hpwh.handle(), coilSystem.containingHVACComponent()->handle());
}
