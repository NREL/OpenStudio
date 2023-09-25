/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../EvaporativeFluidCoolerTwoSpeed.hpp"
#include "../EvaporativeFluidCoolerTwoSpeed_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, EvaporativeFluidCoolerTwoSpeed_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;

      EvaporativeFluidCoolerTwoSpeed testObject = EvaporativeFluidCoolerTwoSpeed(model);

      EXPECT_TRUE(testObject.isHighFanSpeedAirFlowRateAutosized());
      EXPECT_TRUE(testObject.isHighFanSpeedFanPowerAutosized());
      EXPECT_TRUE(testObject.isLowFanSpeedAirFlowRateAutosized());
      EXPECT_DOUBLE_EQ(0.5, testObject.lowFanSpeedAirFlowRateSizingFactor());
      EXPECT_TRUE(testObject.isLowFanSpeedFanPowerAutosized());
      EXPECT_DOUBLE_EQ(0.5, testObject.lowFanSpeedFanPowerSizingFactor());
      EXPECT_DOUBLE_EQ(0.002208, testObject.designSprayWaterFlowRate());
      EXPECT_EQ("UFactorTimesAreaAndDesignWaterFlowRate", testObject.performanceInputMethod());
      EXPECT_DOUBLE_EQ(1.25, testObject.heatRejectionCapacityandNominalCapacitySizingRatio());
      EXPECT_TRUE(testObject.isLowSpeedUserSpecifiedDesignCapacityAutosized());
      EXPECT_TRUE(testObject.isLowSpeedStandardDesignCapacityAutosized());
      EXPECT_DOUBLE_EQ(0.5, testObject.lowSpeedStandardCapacitySizingFactor());
      EXPECT_TRUE(testObject.isHighFanSpeedUfactorTimesAreaValueAutosized());
      EXPECT_TRUE(testObject.isLowFanSpeedUfactorTimesAreaValueAutosized());
      EXPECT_DOUBLE_EQ(0.6, testObject.lowFanSpeedUFactorTimesAreaSizingFactor());
      EXPECT_DOUBLE_EQ(0.5, testObject.lowSpeedUserSpecifiedDesignCapacitySizingFactor());
      EXPECT_DOUBLE_EQ(1.0, testObject.highSpeedSizingFactor());
      EXPECT_EQ("SaturatedExit", testObject.evaporationLossMode());
      EXPECT_DOUBLE_EQ(0.008, testObject.driftLossPercent());
      EXPECT_EQ("ConcentrationRatio", testObject.blowdownCalculationMode());
      EXPECT_DOUBLE_EQ(3.0, testObject.blowdownConcentrationRatio());
      EXPECT_TRUE(testObject.isDesignWaterFlowRateAutosized());

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}
