/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
