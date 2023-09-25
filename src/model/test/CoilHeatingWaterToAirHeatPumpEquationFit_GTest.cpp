/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../CoilHeatingWaterToAirHeatPumpEquationFit.hpp"
#include "../CoilHeatingWaterToAirHeatPumpEquationFit_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilHeatingWaterToAirHeatPumpEquationFit_Test) {
  Model model;
  // test constructor.
  CoilHeatingWaterToAirHeatPumpEquationFit coilHeatingWaterToAirHPEquationFit(model);

  // test rated air flow rate getter and setter
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedAirFlowRate(0.1));
  boost::optional<double> testRatedAirFlowRate = coilHeatingWaterToAirHPEquationFit.ratedAirFlowRate();
  EXPECT_EQ(*testRatedAirFlowRate, 0.1);

  // test if rated air flow rate is defaulted to autosize
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedAirFlowRateDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedAirFlowRate();
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.ratedAirFlowRate());
  coilHeatingWaterToAirHPEquationFit.autosizeRatedAirFlowRate();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedAirFlowRateAutosized());

  // test rated water flow rate getter and setter
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedWaterFlowRate(0.1));
  boost::optional<double> testRatedWaterFlowRate = coilHeatingWaterToAirHPEquationFit.ratedWaterFlowRate();
  EXPECT_EQ(*testRatedWaterFlowRate, 0.1);

  // test if rated water flow rate autosize
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedWaterFlowRateDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedWaterFlowRate();
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.ratedWaterFlowRate());
  coilHeatingWaterToAirHPEquationFit.autosizeRatedWaterFlowRate();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedWaterFlowRateAutosized());

  // test rated heating capacity
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedHeatingCapacity(200));
  boost::optional<double> testRatedHeatingCapacity = coilHeatingWaterToAirHPEquationFit.ratedHeatingCapacity();
  EXPECT_EQ(*testRatedHeatingCapacity, 200);

  // test if rated heating capacity autosize
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCapacityDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedHeatingCapacity();
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.ratedHeatingCapacity());
  coilHeatingWaterToAirHPEquationFit.autosizeRatedHeatingCapacity();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCapacityAutosized());

  // test rated heatingCOP
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedHeatingCoefficientofPerformance(4));
  boost::optional<double> testRatedHeatingCOP = coilHeatingWaterToAirHPEquationFit.ratedHeatingCoefficientofPerformance();
  EXPECT_EQ(*testRatedHeatingCOP, 4);

  // test if rated HeatingCOP default
  EXPECT_FALSE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCoefficientofPerformanceDefaulted());
  coilHeatingWaterToAirHPEquationFit.resetRatedHeatingCoefficientofPerformance();
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.ratedHeatingCoefficientofPerformance());
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.isRatedHeatingCoefficientofPerformanceDefaulted());

  // test rated entering water temperature
  EXPECT_EQ(20, coilHeatingWaterToAirHPEquationFit.ratedEnteringWaterTemperature());
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedEnteringWaterTemperature(25));
  EXPECT_EQ(25, coilHeatingWaterToAirHPEquationFit.ratedEnteringWaterTemperature());

  // test rated entering air dry bulb temperature
  EXPECT_EQ(20, coilHeatingWaterToAirHPEquationFit.ratedEnteringAirDryBulbTemperature());
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatedEnteringAirDryBulbTemperature(26));
  EXPECT_EQ(26, coilHeatingWaterToAirHPEquationFit.ratedEnteringAirDryBulbTemperature());

  // test ratio of rated heating capacity to rated cooling capacity
  EXPECT_EQ(1.0, coilHeatingWaterToAirHPEquationFit.ratioofRatedHeatingCapacitytoRatedCoolingCapacity());
  EXPECT_TRUE(coilHeatingWaterToAirHPEquationFit.setRatioofRatedHeatingCapacitytoRatedCoolingCapacity(1.2));
  EXPECT_EQ(1.2, coilHeatingWaterToAirHPEquationFit.ratioofRatedHeatingCapacitytoRatedCoolingCapacity());
}
