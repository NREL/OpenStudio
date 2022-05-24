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
#include "../CoolingTowerSingleSpeed.hpp"
#include "../CoolingTowerSingleSpeed_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Schedule.hpp"

using namespace openstudio::model;

//Test construction of the coil

TEST_F(ModelFixture, CoolingTowerSingleSpeed_CoolingTowerSingleSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      //create a model to use in testing this code.
      Model m;
      CoolingTowerSingleSpeed coolingTowerSingleSpeed(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;

  CoolingTowerSingleSpeed coolingTowerSingleSpeed(m);

  EXPECT_FALSE(coolingTowerSingleSpeed.designWaterFlowRate());
  EXPECT_TRUE(coolingTowerSingleSpeed.isDesignWaterFlowRateAutosized());

  EXPECT_FALSE(coolingTowerSingleSpeed.designAirFlowRate());
  EXPECT_TRUE(coolingTowerSingleSpeed.isDesignAirFlowRateAutosized());

  EXPECT_FALSE(coolingTowerSingleSpeed.fanPoweratDesignAirFlowRate());
  EXPECT_TRUE(coolingTowerSingleSpeed.isFanPoweratDesignAirFlowRateAutosized());

  EXPECT_FALSE(coolingTowerSingleSpeed.uFactorTimesAreaValueatDesignAirFlowRate());
  EXPECT_TRUE(coolingTowerSingleSpeed.isUFactorTimesAreaValueatDesignAirFlowRateAutosized());

  EXPECT_FALSE(coolingTowerSingleSpeed.airFlowRateinFreeConvectionRegime());
  EXPECT_FALSE(coolingTowerSingleSpeed.isAirFlowRateinFreeConvectionRegimeDefaulted());
  EXPECT_TRUE(coolingTowerSingleSpeed.isAirFlowRateinFreeConvectionRegimeAutosized());

  EXPECT_FALSE(coolingTowerSingleSpeed.uFactorTimesAreaValueatFreeConvectionAirFlowRate());
  EXPECT_FALSE(coolingTowerSingleSpeed.isUFactorTimesAreaValueatFreeConvectionAirFlowRateDefaulted());
  EXPECT_TRUE(coolingTowerSingleSpeed.isUFactorTimesAreaValueatFreeConvectionAirFlowRateAutosized());

  EXPECT_EQ("UFactorTimesAreaAndDesignWaterFlowRate", coolingTowerSingleSpeed.performanceInputMethod());
  EXPECT_FALSE(coolingTowerSingleSpeed.isPerformanceInputMethodDefaulted());

  EXPECT_FALSE(coolingTowerSingleSpeed.nominalCapacity());

  ASSERT_TRUE(coolingTowerSingleSpeed.freeConvectionCapacity());
  EXPECT_EQ(0.0, coolingTowerSingleSpeed.freeConvectionCapacity().get());

  EXPECT_EQ(0.0, coolingTowerSingleSpeed.basinHeaterCapacity());
  EXPECT_FALSE(coolingTowerSingleSpeed.isBasinHeaterCapacityDefaulted());

  EXPECT_EQ(2.0, coolingTowerSingleSpeed.basinHeaterSetpointTemperature());
  EXPECT_FALSE(coolingTowerSingleSpeed.isBasinHeaterSetpointTemperatureDefaulted());

  EXPECT_FALSE(coolingTowerSingleSpeed.basinHeaterOperatingSchedule());

  EXPECT_EQ("LossFactor", coolingTowerSingleSpeed.evaporationLossMode());
  EXPECT_FALSE(coolingTowerSingleSpeed.isEvaporationLossModeDefaulted());

  EXPECT_EQ(0.2, coolingTowerSingleSpeed.evaporationLossFactor());
  EXPECT_FALSE(coolingTowerSingleSpeed.isEvaporationLossFactorDefaulted());

  EXPECT_EQ(0.008, coolingTowerSingleSpeed.driftLossPercent());
  EXPECT_FALSE(coolingTowerSingleSpeed.isDriftLossPercentDefaulted());

  EXPECT_EQ("ConcentrationRatio", coolingTowerSingleSpeed.blowdownCalculationMode());
  EXPECT_FALSE(coolingTowerSingleSpeed.isBlowdownCalculationModeDefaulted());

  EXPECT_EQ(3.0, coolingTowerSingleSpeed.blowdownConcentrationRatio());
  EXPECT_FALSE(coolingTowerSingleSpeed.isBlowdownConcentrationRatioDefaulted());

  EXPECT_FALSE(coolingTowerSingleSpeed.blowdownMakeupWaterUsageSchedule());

  EXPECT_EQ("FanCycling", coolingTowerSingleSpeed.capacityControl());
  EXPECT_FALSE(coolingTowerSingleSpeed.isCapacityControlDefaulted());

  EXPECT_EQ(1, coolingTowerSingleSpeed.numberofCells());
  EXPECT_FALSE(coolingTowerSingleSpeed.isNumberofCellsDefaulted());

  EXPECT_EQ("MinimalCell", coolingTowerSingleSpeed.cellControl());
  EXPECT_FALSE(coolingTowerSingleSpeed.isCellControlDefaulted());

  EXPECT_EQ(0.33, coolingTowerSingleSpeed.cellMinimumWaterFlowRateFraction());
  EXPECT_FALSE(coolingTowerSingleSpeed.isCellMinimumWaterFlowRateFractionDefaulted());

  EXPECT_EQ(2.5, coolingTowerSingleSpeed.cellMaximumWaterFlowRateFraction());
  EXPECT_FALSE(coolingTowerSingleSpeed.isCellMaximumWaterFlowRateFractionDefaulted());

  EXPECT_EQ(1.0, coolingTowerSingleSpeed.sizingFactor());
  EXPECT_FALSE(coolingTowerSingleSpeed.isSizingFactorDefaulted());

  EXPECT_EQ(0.1, coolingTowerSingleSpeed.freeConvectionAirFlowRateSizingFactor());

  EXPECT_EQ(0.1, coolingTowerSingleSpeed.freeConvectionUFactorTimesAreaValueSizingFactor());

  EXPECT_EQ(1.25, coolingTowerSingleSpeed.heatRejectionCapacityAndNominalCapacitySizingRatio());

  EXPECT_EQ(0.1, coolingTowerSingleSpeed.freeConvectionNominalCapacitySizingFactor());

  EXPECT_EQ(35.0, coolingTowerSingleSpeed.designInletAirDryBulbTemperature());

  EXPECT_EQ(25.6, coolingTowerSingleSpeed.designInletAirWetBulbTemperature());

  EXPECT_FALSE(coolingTowerSingleSpeed.designApproachTemperature());
  EXPECT_TRUE(coolingTowerSingleSpeed.isDesignApproachTemperatureAutosized());

  EXPECT_FALSE(coolingTowerSingleSpeed.designRangeTemperature());
  EXPECT_TRUE(coolingTowerSingleSpeed.isDesignRangeTemperatureAutosized());

  EXPECT_EQ("General", coolingTowerSingleSpeed.endUseSubcategory());
}

TEST_F(ModelFixture, CoolingTowerSingleSpeed_addToNode) {
  Model m;
  CoolingTowerSingleSpeed testObject(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(testObject.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(testObject.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(testObject.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  auto testObjectClone = testObject.clone(m).cast<CoolingTowerSingleSpeed>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.supplyComponents().size());
}
