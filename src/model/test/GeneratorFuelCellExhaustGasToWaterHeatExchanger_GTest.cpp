/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../GeneratorFuelCellExhaustGasToWaterHeatExchanger.hpp"
#include "../GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl.hpp"

#include "../Node.hpp"
#include "../Node_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;
using std::string;

TEST_F(ModelFixture, FuelCellExhaustGasToWaterHeatExchanger) {
  Model model;

  GeneratorFuelCellExhaustGasToWaterHeatExchanger exhaustHX(model);
  EXPECT_EQ(0.0004, exhaustHX.heatRecoveryWaterMaximumFlowRate());
  EXPECT_EQ("Condensing", exhaustHX.heatExchangerCalculationMethod());
  //EXPECT_EQ(1.0, exhaustHX.method1HeatExchangerEffectiveness());
  ASSERT_FALSE(exhaustHX.exhaustOutletAirNode());
  //check set node
  Node airNode(model);
  ASSERT_TRUE(exhaustHX.setExhaustOutletAirNode(airNode));
  ASSERT_TRUE(exhaustHX.exhaustOutletAirNode());
  exhaustHX.resetExhaustOutletAirNode();
  ASSERT_FALSE(exhaustHX.exhaustOutletAirNode());
  //check calculation methods
  ASSERT_TRUE(exhaustHX.setHeatExchangerCalculationMethod("EmpiricalUAeff"));
  EXPECT_EQ(1.0, exhaustHX.method2Parameterhxs0().get());
  EXPECT_EQ(0.0, exhaustHX.method2Parameterhxs1().get());
  EXPECT_EQ(0.0, exhaustHX.method2Parameterhxs2().get());
  EXPECT_EQ(0.0, exhaustHX.method2Parameterhxs3().get());
  EXPECT_EQ(0.0, exhaustHX.method2Parameterhxs4().get());
  ASSERT_TRUE(exhaustHX.setHeatExchangerCalculationMethod("FundementalUAeff"));
  EXPECT_EQ(0.0, exhaustHX.method3FAdjustmentFactor().get());
  EXPECT_EQ(1.0, exhaustHX.method3GasArea().get());
  EXPECT_EQ(1.0, exhaustHX.method3h0GasCoefficient().get());
  EXPECT_EQ(1.0, exhaustHX.method3h0WaterCoefficient().get());
  EXPECT_EQ(1.0, exhaustHX.method3mCoefficient().get());
  EXPECT_EQ(1.0, exhaustHX.method3nCoefficient().get());
  EXPECT_EQ(1.0, exhaustHX.method3NdotGasRefCoefficient().get());
  EXPECT_EQ(1.0, exhaustHX.method3NdotWaterrefCoefficient().get());
  EXPECT_EQ(1.0, exhaustHX.method3WaterArea().get());
  ASSERT_TRUE(exhaustHX.setHeatExchangerCalculationMethod("Condensing"));
  EXPECT_EQ(35.0, exhaustHX.method4CondensationThreshold().get());
  EXPECT_EQ(1.0, exhaustHX.method4hxl1Coefficient().get());
  EXPECT_EQ(1.0, exhaustHX.method4hxl2Coefficient().get());
  //check bad method
  ASSERT_FALSE(exhaustHX.setHeatExchangerCalculationMethod("MadeUp"));
  //check reset method
  exhaustHX.resetHeatExchangerCalculationMethod();
  EXPECT_EQ("FixedEffectiveness", exhaustHX.heatExchangerCalculationMethod());
  EXPECT_EQ(1.0, exhaustHX.method1HeatExchangerEffectiveness().get());
  //check reset method1 params
  exhaustHX.resetMethod1HeatExchangerEffectiveness();
  ASSERT_FALSE(exhaustHX.method1HeatExchangerEffectiveness());
  //check reset method2 params
  exhaustHX.resetMethod2Parameterhxs0();
  exhaustHX.resetMethod2Parameterhxs1();
  exhaustHX.resetMethod2Parameterhxs2();
  exhaustHX.resetMethod2Parameterhxs3();
  exhaustHX.resetMethod2Parameterhxs4();
  ASSERT_FALSE(exhaustHX.method2Parameterhxs0());
  ASSERT_FALSE(exhaustHX.method2Parameterhxs1());
  ASSERT_FALSE(exhaustHX.method2Parameterhxs2());
  ASSERT_FALSE(exhaustHX.method2Parameterhxs3());
  ASSERT_FALSE(exhaustHX.method2Parameterhxs4());
  //check reset method3 params
  exhaustHX.resetMethod3FAdjustmentFactor();
  exhaustHX.resetMethod3GasArea();
  exhaustHX.resetMethod3h0GasCoefficient();
  exhaustHX.resetMethod3h0WaterCoefficient();
  exhaustHX.resetMethod3mCoefficient();
  exhaustHX.resetMethod3nCoefficient();
  exhaustHX.resetMethod3NdotGasRefCoefficient();
  exhaustHX.resetMethod3NdotWaterrefCoefficient();
  exhaustHX.resetMethod3WaterArea();
  ASSERT_FALSE(exhaustHX.method3FAdjustmentFactor());
  ASSERT_FALSE(exhaustHX.method3GasArea());
  ASSERT_FALSE(exhaustHX.method3h0GasCoefficient());
  ASSERT_FALSE(exhaustHX.method3h0WaterCoefficient());
  ASSERT_FALSE(exhaustHX.method3mCoefficient());
  ASSERT_FALSE(exhaustHX.method3nCoefficient());
  ASSERT_FALSE(exhaustHX.method3NdotGasRefCoefficient());
  ASSERT_FALSE(exhaustHX.method3NdotWaterrefCoefficient());
  ASSERT_FALSE(exhaustHX.method3WaterArea());
  //check reset method4 params
  exhaustHX.resetMethod4CondensationThreshold();
  exhaustHX.resetMethod4hxl1Coefficient();
  exhaustHX.resetMethod4hxl2Coefficient();
  ASSERT_FALSE(exhaustHX.method4CondensationThreshold());
  ASSERT_FALSE(exhaustHX.method4hxl1Coefficient());
  ASSERT_FALSE(exhaustHX.method4hxl2Coefficient());
}

TEST_F(ModelFixture, FuelCellExhaustGasToWaterHeatExchanger2) {
  Model model;

  Node airNode(model);
  GeneratorFuelCellExhaustGasToWaterHeatExchanger exhaustHX(model, airNode);
  EXPECT_EQ(0.0004, exhaustHX.heatRecoveryWaterMaximumFlowRate());
  EXPECT_EQ("FixedEffectiveness", exhaustHX.heatExchangerCalculationMethod());
  EXPECT_EQ(1.0, exhaustHX.method1HeatExchangerEffectiveness().get());
  ASSERT_TRUE(exhaustHX.exhaustOutletAirNode());
  EXPECT_EQ(airNode, exhaustHX.exhaustOutletAirNode().get());
  exhaustHX.resetExhaustOutletAirNode();
  ASSERT_FALSE(exhaustHX.exhaustOutletAirNode());
}
