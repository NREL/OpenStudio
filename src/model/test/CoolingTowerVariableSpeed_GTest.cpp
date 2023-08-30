/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../CoolingTowerVariableSpeed.hpp"
#include "../CoolingTowerVariableSpeed_Impl.hpp"
#include "../AirLoopHVAC.hpp"
#include "../PlantLoop.hpp"
#include "../Node.hpp"
#include "../Node_Impl.hpp"
#include "../AirLoopHVACZoneSplitter.hpp"
#include "../Schedule.hpp"

using namespace openstudio::model;

TEST_F(ModelFixture, CoolingTowerVariableSpeed_constructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoolingTowerVariableSpeed coolingTowerVariableSpeed(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");

  Model m;

  CoolingTowerVariableSpeed coolingTowerVariableSpeed(m);

  ASSERT_TRUE(coolingTowerVariableSpeed.modelType());
  EXPECT_EQ("CoolToolsCrossFlow", coolingTowerVariableSpeed.modelType().get());

  EXPECT_FALSE(coolingTowerVariableSpeed.modelCoefficient());

  ASSERT_TRUE(coolingTowerVariableSpeed.designInletAirWetBulbTemperature());
  EXPECT_EQ(25.5556, coolingTowerVariableSpeed.designInletAirWetBulbTemperature().get());

  ASSERT_TRUE(coolingTowerVariableSpeed.designApproachTemperature());
  EXPECT_EQ(3.8889, coolingTowerVariableSpeed.designApproachTemperature().get());

  ASSERT_TRUE(coolingTowerVariableSpeed.designRangeTemperature());
  EXPECT_EQ(5.5556, coolingTowerVariableSpeed.designRangeTemperature().get());

  EXPECT_FALSE(coolingTowerVariableSpeed.designWaterFlowRate());
  EXPECT_TRUE(coolingTowerVariableSpeed.isDesignWaterFlowRateAutosized());

  EXPECT_FALSE(coolingTowerVariableSpeed.designAirFlowRate());
  EXPECT_TRUE(coolingTowerVariableSpeed.isDesignAirFlowRateAutosized());

  EXPECT_FALSE(coolingTowerVariableSpeed.designFanPower());
  EXPECT_TRUE(coolingTowerVariableSpeed.isDesignFanPowerAutosized());

  ASSERT_TRUE(coolingTowerVariableSpeed.minimumAirFlowRateRatio());
  EXPECT_EQ(0.2, coolingTowerVariableSpeed.minimumAirFlowRateRatio().get());

  ASSERT_TRUE(coolingTowerVariableSpeed.fractionofTowerCapacityinFreeConvectionRegime());
  EXPECT_EQ(0.125, coolingTowerVariableSpeed.fractionofTowerCapacityinFreeConvectionRegime().get());

  ASSERT_TRUE(coolingTowerVariableSpeed.basinHeaterCapacity());
  EXPECT_EQ(0.0, coolingTowerVariableSpeed.basinHeaterCapacity().get());

  ASSERT_TRUE(coolingTowerVariableSpeed.basinHeaterSetpointTemperature());
  EXPECT_EQ(2.0, coolingTowerVariableSpeed.basinHeaterSetpointTemperature().get());

  EXPECT_FALSE(coolingTowerVariableSpeed.basinHeaterOperatingSchedule());

  EXPECT_EQ("SaturatedExit", coolingTowerVariableSpeed.evaporationLossMode());
  EXPECT_FALSE(coolingTowerVariableSpeed.isEvaporationLossModeDefaulted());

  EXPECT_EQ(0.2, coolingTowerVariableSpeed.evaporationLossFactor());
  EXPECT_TRUE(coolingTowerVariableSpeed.isEvaporationLossFactorDefaulted());

  ASSERT_TRUE(coolingTowerVariableSpeed.driftLossPercent());
  EXPECT_EQ(0.008, coolingTowerVariableSpeed.driftLossPercent().get());

  EXPECT_EQ("ConcentrationRatio", coolingTowerVariableSpeed.blowdownCalculationMode());
  EXPECT_FALSE(coolingTowerVariableSpeed.isBlowdownCalculationModeDefaulted());

  ASSERT_TRUE(coolingTowerVariableSpeed.blowdownConcentrationRatio());
  EXPECT_EQ(3.0, coolingTowerVariableSpeed.blowdownConcentrationRatio().get());

  EXPECT_FALSE(coolingTowerVariableSpeed.blowdownMakeupWaterUsageSchedule());

  EXPECT_FALSE(coolingTowerVariableSpeed.numberofCells());

  EXPECT_EQ("MinimalCell", coolingTowerVariableSpeed.cellControl());
  EXPECT_TRUE(coolingTowerVariableSpeed.isCellControlDefaulted());

  EXPECT_FALSE(coolingTowerVariableSpeed.cellMinimumWaterFlowRateFraction());

  EXPECT_FALSE(coolingTowerVariableSpeed.cellMaximumWaterFlowRateFraction());

  ASSERT_TRUE(coolingTowerVariableSpeed.sizingFactor());
  EXPECT_EQ(1.0, coolingTowerVariableSpeed.sizingFactor().get());

  EXPECT_EQ("General", coolingTowerVariableSpeed.endUseSubcategory());

  EXPECT_TRUE(coolingTowerVariableSpeed.fanPowerRatioFunctionofAirFlowRateRatioCurve());
}

TEST_F(ModelFixture, CoolingTowerVariableSpeed_addToNode) {
  Model m;
  CoolingTowerVariableSpeed coolingTowerVariableSpeed(m);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();

  EXPECT_FALSE(coolingTowerVariableSpeed.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)2, airLoop.supplyComponents().size());

  Node inletNode = airLoop.zoneSplitter().lastOutletModelObject()->cast<Node>();

  EXPECT_FALSE(coolingTowerVariableSpeed.addToNode(inletNode));
  EXPECT_EQ((unsigned)5, airLoop.demandComponents().size());

  PlantLoop plantLoop(m);
  supplyOutletNode = plantLoop.supplyOutletNode();
  EXPECT_TRUE(coolingTowerVariableSpeed.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)7, plantLoop.supplyComponents().size());

  Node demandOutletNode = plantLoop.demandOutletNode();
  EXPECT_FALSE(coolingTowerVariableSpeed.addToNode(demandOutletNode));
  EXPECT_EQ((unsigned)5, plantLoop.demandComponents().size());

  auto testObjectClone = coolingTowerVariableSpeed.clone(m).cast<CoolingTowerVariableSpeed>();
  supplyOutletNode = plantLoop.supplyOutletNode();

  EXPECT_TRUE(testObjectClone.addToNode(supplyOutletNode));
  EXPECT_EQ((unsigned)9, plantLoop.supplyComponents().size());
}
