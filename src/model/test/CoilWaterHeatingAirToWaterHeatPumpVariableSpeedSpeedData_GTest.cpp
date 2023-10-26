/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"

#include "../CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData.hpp"
#include "../CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_Impl.hpp"

#include "../CurveBiquadratic.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveCubic.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData coil(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData_GettersSetters) {
  Model m;
  CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData coilwhSpeed(m);
  coilwhSpeed.setName("My CoilWaterHeatingAirToWaterHeatPumpVariableSpeedSpeedData");

  // Rated Water Heating Capacity: Required Double
  EXPECT_TRUE(coilwhSpeed.setRatedWaterHeatingCapacity(1000.0));
  EXPECT_EQ(1000.0, coilwhSpeed.ratedWaterHeatingCapacity());
  // Bad Value
  EXPECT_FALSE(coilwhSpeed.setRatedWaterHeatingCapacity(-10.0));
  EXPECT_EQ(1000.0, coilwhSpeed.ratedWaterHeatingCapacity());

  // Rated Water Heating COP: Required Double
  EXPECT_TRUE(coilwhSpeed.setRatedWaterHeatingCOP(4.15));
  EXPECT_EQ(4.15, coilwhSpeed.ratedWaterHeatingCOP());
  // Bad Value
  EXPECT_FALSE(coilwhSpeed.setRatedWaterHeatingCOP(-10.0));
  EXPECT_EQ(4.15, coilwhSpeed.ratedWaterHeatingCOP());

  // Rated Sensible Heat Ratio: Required Double
  EXPECT_TRUE(coilwhSpeed.setRatedSensibleHeatRatio(0.75));
  EXPECT_EQ(0.75, coilwhSpeed.ratedSensibleHeatRatio());
  // Bad Value
  EXPECT_FALSE(coilwhSpeed.setRatedSensibleHeatRatio(-9.5));
  EXPECT_EQ(0.75, coilwhSpeed.ratedSensibleHeatRatio());

  // Reference Unit Rated Air Flow Rate: Required Double
  EXPECT_TRUE(coilwhSpeed.setReferenceUnitRatedAirFlowRate(1.1));
  EXPECT_EQ(1.1, coilwhSpeed.referenceUnitRatedAirFlowRate());
  // Bad Value
  EXPECT_FALSE(coilwhSpeed.setReferenceUnitRatedAirFlowRate(-10.0));
  EXPECT_EQ(1.1, coilwhSpeed.referenceUnitRatedAirFlowRate());

  // Reference Unit Rated Water Flow Rate: Required Double
  EXPECT_TRUE(coilwhSpeed.setReferenceUnitRatedWaterFlowRate(0.91));
  EXPECT_EQ(0.91, coilwhSpeed.referenceUnitRatedWaterFlowRate());
  // Bad Value
  EXPECT_FALSE(coilwhSpeed.setReferenceUnitRatedWaterFlowRate(-10.0));
  EXPECT_EQ(0.91, coilwhSpeed.referenceUnitRatedWaterFlowRate());

  // Reference Unit Water Pump Input Power At Rated Conditions: Required Double
  EXPECT_TRUE(coilwhSpeed.setReferenceUnitWaterPumpInputPowerAtRatedConditions(100.1));
  EXPECT_EQ(100.1, coilwhSpeed.referenceUnitWaterPumpInputPowerAtRatedConditions());
  // Bad Value
  EXPECT_FALSE(coilwhSpeed.setReferenceUnitWaterPumpInputPowerAtRatedConditions(-10.0));
  EXPECT_EQ(100.1, coilwhSpeed.referenceUnitWaterPumpInputPowerAtRatedConditions());

  // Total Water Heating Capacity Function of Temperature Curve Name: Required Object
  {
    CurveBiquadratic water_heating_cap_ft(m);
    EXPECT_TRUE(coilwhSpeed.setTotalWaterHeatingCapacityFunctionofTemperatureCurve(water_heating_cap_ft));
    EXPECT_EQ(water_heating_cap_ft, coilwhSpeed.totalWaterHeatingCapacityFunctionofTemperatureCurve());
  }

  // Total Water Heating Capacity Function of Air Flow Fraction Curve Name: Required Object
  {
    CurveQuadratic water_heating_cap_fairflow(m);
    EXPECT_TRUE(coilwhSpeed.setTotalWaterHeatingCapacityFunctionofAirFlowFractionCurve(water_heating_cap_fairflow));
    EXPECT_EQ(water_heating_cap_fairflow, coilwhSpeed.totalWaterHeatingCapacityFunctionofAirFlowFractionCurve());
  }

  // Total Water Heating Capacity Function of Water Flow Fraction Curve Name: Required Object
  {
    CurveCubic water_heating_cap_fwaterflow(m);
    EXPECT_TRUE(coilwhSpeed.setTotalWaterHeatingCapacityFunctionofWaterFlowFractionCurve(water_heating_cap_fwaterflow));
    EXPECT_EQ(water_heating_cap_fwaterflow, coilwhSpeed.totalWaterHeatingCapacityFunctionofWaterFlowFractionCurve());
  }

  // COP Function of Temperature Curve Name: Required Object
  {
    CurveBiquadratic cop_ft(m);
    EXPECT_TRUE(coilwhSpeed.setCOPFunctionofTemperatureCurve(cop_ft));
    EXPECT_EQ(cop_ft, coilwhSpeed.copFunctionofTemperatureCurve());
  }

  // COP Function of Air Flow Fraction Curve Name: Required Object
  {
    CurveCubic cop_fairflow(m);
    EXPECT_TRUE(coilwhSpeed.setCOPFunctionofAirFlowFractionCurve(cop_fairflow));
    EXPECT_EQ(cop_fairflow, coilwhSpeed.copFunctionofAirFlowFractionCurve());
  }

  // COP Function of Water Flow Fraction Curve Name: Required Object
  {
    CurveQuadratic cop_fwaterflow(m);
    EXPECT_TRUE(coilwhSpeed.setCOPFunctionofWaterFlowFractionCurve(cop_fwaterflow));
    EXPECT_EQ(cop_fwaterflow, coilwhSpeed.copFunctionofWaterFlowFractionCurve());
  }
}
