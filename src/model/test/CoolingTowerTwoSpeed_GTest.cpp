/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
