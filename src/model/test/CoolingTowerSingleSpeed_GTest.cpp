/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
