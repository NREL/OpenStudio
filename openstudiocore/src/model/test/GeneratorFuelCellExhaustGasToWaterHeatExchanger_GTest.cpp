/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>

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
  EXPECT_EQ(1.0, exhaustHX.method2Parameterhxs0());
  EXPECT_EQ(0.0, exhaustHX.method2Parameterhxs1());
  EXPECT_EQ(0.0, exhaustHX.method2Parameterhxs2());
  EXPECT_EQ(0.0, exhaustHX.method2Parameterhxs3());
  EXPECT_EQ(0.0, exhaustHX.method2Parameterhxs4());
  ASSERT_TRUE(exhaustHX.setHeatExchangerCalculationMethod("FundementalUAeff"));
  EXPECT_EQ(0.0, exhaustHX.method3FAdjustmentFactor());
  EXPECT_EQ(1.0, exhaustHX.method3GasArea());
  EXPECT_EQ(1.0, exhaustHX.method3h0GasCoefficient());
  EXPECT_EQ(1.0, exhaustHX.method3h0WaterCoefficient());
  EXPECT_EQ(1.0, exhaustHX.method3mCoefficient());
  EXPECT_EQ(1.0, exhaustHX.method3nCoefficient());
  EXPECT_EQ(1.0, exhaustHX.method3NdotGasRefCoefficient());
  EXPECT_EQ(1.0, exhaustHX.method3NdotWaterrefCoefficient());
  EXPECT_EQ(1.0, exhaustHX.method3WaterArea());
  ASSERT_TRUE(exhaustHX.setHeatExchangerCalculationMethod("Condensing"));
  EXPECT_EQ(35.0, exhaustHX.method4CondensationThreshold());
  EXPECT_EQ(1.0, exhaustHX.method4hxl1Coefficient());
  EXPECT_EQ(1.0, exhaustHX.method4hxl2Coefficient());
  //check bad method
  ASSERT_FALSE(exhaustHX.setHeatExchangerCalculationMethod("MadeUp"));
  //check reset method
  exhaustHX.resetHeatExchangerCalculationMethod();
  EXPECT_EQ("FixedEffectiveness", exhaustHX.heatExchangerCalculationMethod());
  EXPECT_EQ(1.0, exhaustHX.method1HeatExchangerEffectiveness());
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
  EXPECT_EQ(1.0, exhaustHX.method1HeatExchangerEffectiveness());
  ASSERT_TRUE(exhaustHX.exhaustOutletAirNode());
  EXPECT_EQ(airNode, exhaustHX.exhaustOutletAirNode());
  exhaustHX.resetExhaustOutletAirNode();
  ASSERT_FALSE(exhaustHX.exhaustOutletAirNode());
}
