/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../CoolingTowerTwoSpeed.hpp"
#include "../CoolingTowerTwoSpeed_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Schedule.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, CoolingTowerTwoSpeed_constructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoolingTowerTwoSpeed coolingTowerTwoSpeed(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;

  CoolingTowerTwoSpeed coolingTowerTwoSpeed(m);

  EXPECT_FALSE(coolingTowerTwoSpeed.designWaterFlowRate());
  EXPECT_TRUE(coolingTowerTwoSpeed.isDesignWaterFlowRateAutosized());

  EXPECT_FALSE(coolingTowerTwoSpeed.highFanSpeedAirFlowRate());
  EXPECT_TRUE(coolingTowerTwoSpeed.isHighFanSpeedAirFlowRateAutosized());

  EXPECT_FALSE(coolingTowerTwoSpeed.highFanSpeedFanPower());
  EXPECT_TRUE(coolingTowerTwoSpeed.isHighFanSpeedFanPowerAutosized());

  EXPECT_FALSE(coolingTowerTwoSpeed.highFanSpeedUFactorTimesAreaValue());
  EXPECT_TRUE(coolingTowerTwoSpeed.isHighFanSpeedUFactorTimesAreaValueAutosized());

  EXPECT_FALSE(coolingTowerTwoSpeed.lowFanSpeedAirFlowRate());
  EXPECT_TRUE(coolingTowerTwoSpeed.isLowFanSpeedAirFlowRateAutosized());

  EXPECT_EQ(0.5, coolingTowerTwoSpeed.lowFanSpeedAirFlowRateSizingFactor());
  EXPECT_TRUE(coolingTowerTwoSpeed.isLowFanSpeedAirFlowRateSizingFactorDefaulted());

  EXPECT_FALSE(coolingTowerTwoSpeed.lowFanSpeedFanPower());
  EXPECT_TRUE(coolingTowerTwoSpeed.isLowFanSpeedFanPowerAutosized());

  EXPECT_EQ(0.16, coolingTowerTwoSpeed.lowFanSpeedFanPowerSizingFactor());
  EXPECT_TRUE(coolingTowerTwoSpeed.isLowFanSpeedFanPowerSizingFactorDefaulted());

  EXPECT_FALSE(coolingTowerTwoSpeed.lowFanSpeedUFactorTimesAreaValue());
  EXPECT_TRUE(coolingTowerTwoSpeed.isLowFanSpeedUFactorTimesAreaValueAutosized());

  EXPECT_EQ(0.6, coolingTowerTwoSpeed.lowFanSpeedUFactorTimesAreaSizingFactor());
  EXPECT_TRUE(coolingTowerTwoSpeed.isLowFanSpeedUFactorTimesAreaSizingFactorDefaulted());

  ASSERT_TRUE(coolingTowerTwoSpeed.freeConvectionRegimeAirFlowRate());
  EXPECT_EQ(0.0, coolingTowerTwoSpeed.freeConvectionRegimeAirFlowRate().get());
  EXPECT_TRUE(coolingTowerTwoSpeed.isFreeConvectionRegimeAirFlowRateDefaulted());
  EXPECT_FALSE(coolingTowerTwoSpeed.isFreeConvectionRegimeAirFlowRateAutosized());

  EXPECT_EQ(0.1, coolingTowerTwoSpeed.freeConvectionRegimeAirFlowRateSizingFactor());
  EXPECT_TRUE(coolingTowerTwoSpeed.isFreeConvectionRegimeAirFlowRateSizingFactorDefaulted());

  ASSERT_TRUE(coolingTowerTwoSpeed.freeConvectionRegimeUFactorTimesAreaValue());
  EXPECT_EQ(0.0, coolingTowerTwoSpeed.freeConvectionRegimeUFactorTimesAreaValue().get());
  EXPECT_TRUE(coolingTowerTwoSpeed.isFreeConvectionRegimeUFactorTimesAreaValueDefaulted());
  EXPECT_FALSE(coolingTowerTwoSpeed.isFreeConvectionRegimeUFactorTimesAreaValueAutosized());

  EXPECT_EQ(0.1, coolingTowerTwoSpeed.freeConvectionUFactorTimesAreaValueSizingFactor());
  EXPECT_TRUE(coolingTowerTwoSpeed.isFreeConvectionUFactorTimesAreaValueSizingFactorDefaulted());

  EXPECT_EQ("UFactorTimesAreaAndDesignWaterFlowRate", coolingTowerTwoSpeed.performanceInputMethod());
  EXPECT_TRUE(coolingTowerTwoSpeed.isPerformanceInputMethodDefaulted());

  EXPECT_EQ(1.25, coolingTowerTwoSpeed.heatRejectionCapacityandNominalCapacitySizingRatio());
  EXPECT_TRUE(coolingTowerTwoSpeed.isHeatRejectionCapacityandNominalCapacitySizingRatioDefaulted());

  EXPECT_FALSE(coolingTowerTwoSpeed.highSpeedNominalCapacity());

  EXPECT_FALSE(coolingTowerTwoSpeed.lowSpeedNominalCapacity());
  EXPECT_FALSE(coolingTowerTwoSpeed.isLowSpeedNominalCapacityAutosized());

  EXPECT_EQ(0.5, coolingTowerTwoSpeed.lowSpeedNominalCapacitySizingFactor());
  EXPECT_TRUE(coolingTowerTwoSpeed.isLowSpeedNominalCapacitySizingFactorDefaulted());

  EXPECT_FALSE(coolingTowerTwoSpeed.freeConvectionNominalCapacity());
  EXPECT_FALSE(coolingTowerTwoSpeed.isFreeConvectionNominalCapacityAutosized());

  EXPECT_EQ(0.1, coolingTowerTwoSpeed.freeConvectionNominalCapacitySizingFactor());
  EXPECT_TRUE(coolingTowerTwoSpeed.isFreeConvectionNominalCapacitySizingFactorDefaulted());

  EXPECT_EQ(0.0, coolingTowerTwoSpeed.basinHeaterCapacity());
  EXPECT_TRUE(coolingTowerTwoSpeed.isBasinHeaterCapacityDefaulted());

  EXPECT_EQ(2.0, coolingTowerTwoSpeed.basinHeaterSetpointTemperature());
  EXPECT_TRUE(coolingTowerTwoSpeed.isBasinHeaterSetpointTemperatureDefaulted());

  EXPECT_FALSE(coolingTowerTwoSpeed.basinHeaterOperatingSchedule());

  EXPECT_EQ("SaturatedExit", coolingTowerTwoSpeed.evaporationLossMode());
  EXPECT_TRUE(coolingTowerTwoSpeed.isEvaporationLossModeDefaulted());

  EXPECT_EQ(0.2, coolingTowerTwoSpeed.evaporationLossFactor());
  EXPECT_TRUE(coolingTowerTwoSpeed.isEvaporationLossFactorDefaulted());

  EXPECT_EQ(0.008, coolingTowerTwoSpeed.driftLossPercent());
  EXPECT_TRUE(coolingTowerTwoSpeed.isDriftLossPercentDefaulted());

  EXPECT_EQ("ConcentrationRatio", coolingTowerTwoSpeed.blowdownCalculationMode());
  EXPECT_TRUE(coolingTowerTwoSpeed.isBlowdownCalculationModeDefaulted());

  EXPECT_EQ(3.0, coolingTowerTwoSpeed.blowdownConcentrationRatio());
  EXPECT_TRUE(coolingTowerTwoSpeed.isBlowdownConcentrationRatioDefaulted());

  EXPECT_FALSE(coolingTowerTwoSpeed.blowdownMakeupWaterUsageSchedule());

  EXPECT_EQ(1, coolingTowerTwoSpeed.numberofCells());
  EXPECT_TRUE(coolingTowerTwoSpeed.isNumberofCellsDefaulted());

  EXPECT_EQ("MinimalCell", coolingTowerTwoSpeed.cellControl());
  EXPECT_TRUE(coolingTowerTwoSpeed.isCellControlDefaulted());

  EXPECT_EQ(0.33, coolingTowerTwoSpeed.cellMinimumWaterFlowRateFraction());
  EXPECT_TRUE(coolingTowerTwoSpeed.isCellMinimumWaterFlowRateFractionDefaulted());

  EXPECT_EQ(2.5, coolingTowerTwoSpeed.cellMaximumWaterFlowRateFraction());
  EXPECT_TRUE(coolingTowerTwoSpeed.isCellMaximumWaterFlowRateFractionDefaulted());

  EXPECT_EQ(1.0, coolingTowerTwoSpeed.sizingFactor());

  EXPECT_EQ(35.0, coolingTowerTwoSpeed.designInletAirDryBulbTemperature());

  EXPECT_EQ(25.6, coolingTowerTwoSpeed.designInletAirWetBulbTemperature());

  EXPECT_FALSE(coolingTowerTwoSpeed.designApproachTemperature());
  EXPECT_TRUE(coolingTowerTwoSpeed.isDesignApproachTemperatureAutosized());

  EXPECT_FALSE(coolingTowerTwoSpeed.designRangeTemperature());
  EXPECT_TRUE(coolingTowerTwoSpeed.isDesignRangeTemperatureAutosized());

  EXPECT_EQ("General", coolingTowerTwoSpeed.endUseSubcategory());
}

TEST_F(ModelFixture, CoolingTowerTwoSpeed_addToNode) {
  Model m;
  CoolingTowerTwoSpeed coolingTowerTwoSpeed(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(coolingTowerTwoSpeed.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(coolingTowerTwoSpeed.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(coolingTowerTwoSpeed.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(coolingTowerTwoSpeed.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  auto testObjectClone = coolingTowerTwoSpeed.clone(m).cast<CoolingTowerTwoSpeed>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.supplyComponents().size());
}
