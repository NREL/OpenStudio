/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ModelFixture.hpp"

#include "../Model.hpp"

#include "../ChillerHeaterPerformanceElectricEIR.hpp"
#include "../ChillerHeaterPerformanceElectricEIR_Impl.hpp"

// Curves
#include "../Curve.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST(ChillerHeaterPerformanceElectricEIR,ChillerHeaterPerformanceElectricEIR_ChillerHeaterPerformanceElectricEIR)
{
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT (
  {
     Model model;

     ChillerHeaterPerformanceElectricEIR ch_heater(model);

     exit(0);
  } ,
    ::testing::ExitedWithCode(0), "" );
}

// Test the various setters and getters
TEST(ChillerHeaterPerformanceElectricEIR,ChillerHeaterPerformanceElectricEIR_SettersGetters)
{
  Model model;

  ChillerHeaterPerformanceElectricEIR ch_heater(model);

  ASSERT_TRUE(ch_heater.isReferenceCoolingModeEvaporatorCapacityAutosized());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setReferenceCoolingModeEvaporatorCapacity(12345.6));
  ASSERT_TRUE(ch_heater.referenceCoolingModeEvaporatorCapacity());
  ASSERT_EQ(12345.6, ch_heater.referenceCoolingModeEvaporatorCapacity());

  // Return type: void
  ch_heater.autosizeReferenceCoolingModeEvaporatorCapacity();
  ASSERT_TRUE(ch_heater.isReferenceCoolingModeEvaporatorCapacityAutosized());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setReferenceCoolingModeCOP(2.1));
  EXPECT_TRUE(ch_heater.referenceCoolingModeCOP());
  ASSERT_EQ(2.1, ch_heater.referenceCoolingModeCOP());

  // Return type: void
  ch_heater.setReferenceCoolingModeLeavingChilledWaterTemperature(7.1);
  EXPECT_TRUE(ch_heater.referenceCoolingModeLeavingChilledWaterTemperature());
  ASSERT_EQ(7.1, ch_heater.referenceCoolingModeLeavingChilledWaterTemperature());

  // Return type: void
  ch_heater.setReferenceCoolingModeEnteringCondenserFluidTemperature(28.87);
  EXPECT_TRUE(ch_heater.referenceCoolingModeEnteringCondenserFluidTemperature());
  ASSERT_EQ(28.87, ch_heater.referenceCoolingModeEnteringCondenserFluidTemperature());

  // Return type: void
  ch_heater.setReferenceCoolingModeLeavingCondenserWaterTemperature(35.2);
  EXPECT_TRUE(ch_heater.referenceCoolingModeLeavingCondenserWaterTemperature());
  ASSERT_EQ(35.2, ch_heater.referenceCoolingModeLeavingCondenserWaterTemperature());

  // Return type: void
  ch_heater.setReferenceHeatingModeCoolingCapacityRatio(0.71);
  EXPECT_TRUE(ch_heater.referenceHeatingModeCoolingCapacityRatio());
  ASSERT_EQ(0.71, ch_heater.referenceHeatingModeCoolingCapacityRatio());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setReferenceHeatingModeCoolingPowerInputRatio(1.32));
  EXPECT_TRUE(ch_heater.referenceHeatingModeCoolingPowerInputRatio());
  ASSERT_EQ(1.32, ch_heater.referenceHeatingModeCoolingPowerInputRatio());

  // Return type: void
  ch_heater.setReferenceHeatingModeLeavingChilledWaterTemperature(7.1);
  EXPECT_TRUE(ch_heater.referenceHeatingModeLeavingChilledWaterTemperature());
  ASSERT_EQ(7.1, ch_heater.referenceHeatingModeLeavingChilledWaterTemperature());

  // Return type: void
  ch_heater.setReferenceHeatingModeLeavingCondenserWaterTemperature(59.9);
  EXPECT_TRUE(ch_heater.referenceHeatingModeLeavingCondenserWaterTemperature());
  ASSERT_EQ(59.9, ch_heater.referenceHeatingModeLeavingCondenserWaterTemperature());

  // Return type: void
  ch_heater.setReferenceHeatingModeEnteringCondenserFluidTemperature(28.82);
  EXPECT_TRUE(ch_heater.referenceHeatingModeEnteringCondenserFluidTemperature());
  ASSERT_EQ(28.82, ch_heater.referenceHeatingModeEnteringCondenserFluidTemperature());

  // Return type: void
  ch_heater.setHeatingModeEnteringChilledWaterTemperatureLowLimit(5.03);
  EXPECT_TRUE(ch_heater.heatingModeEnteringChilledWaterTemperatureLowLimit());
  ASSERT_EQ(5.03, ch_heater.heatingModeEnteringChilledWaterTemperatureLowLimit());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setChilledWaterFlowModeType("ConstantFlow"));
  ASSERT_EQ("ConstantFlow", ch_heater.chilledWaterFlowModeType());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setDesignChilledWaterFlowRate(0.000365));
  EXPECT_TRUE(ch_heater.designChilledWaterFlowRate());
  ASSERT_EQ(0.000365, ch_heater.designChilledWaterFlowRate());

  // Return type: void
  ch_heater.resetDesignChilledWaterFlowRate();
  // Return type: void
  ch_heater.autosizeDesignChilledWaterFlowRate();
  ASSERT_TRUE(ch_heater.isDesignChilledWaterFlowRateAutosized());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setDesignCondenserWaterFlowRate(0.00037));
  EXPECT_TRUE(ch_heater.designCondenserWaterFlowRate());
  ASSERT_EQ(0.00037, ch_heater.designCondenserWaterFlowRate());

  // Return type: void
  ch_heater.resetDesignCondenserWaterFlowRate();
  // Return type: void
  ch_heater.autosizeDesignCondenserWaterFlowRate();
  ASSERT_TRUE(ch_heater.isDesignCondenserWaterFlowRateAutosized());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setDesignHotWaterFlowRate(0.0003725));
  EXPECT_TRUE(ch_heater.designHotWaterFlowRate());
  ASSERT_EQ(0.0003725, ch_heater.designHotWaterFlowRate());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setCompressorMotorEfficiency(0.98));
  EXPECT_TRUE(ch_heater.compressorMotorEfficiency());
  ASSERT_EQ(0.98, ch_heater.compressorMotorEfficiency());

  // Return type: bool
  // WaterCooled is the only valid type
  ASSERT_TRUE(ch_heater.setCondenserType("WaterCooled"));
  ASSERT_EQ("WaterCooled", ch_heater.condenserType());

  // Return type: bool
  // "LeavingCondenser" or "EnteringCondenser"
  ASSERT_TRUE(ch_heater.setCoolingModeTemperatureCurveCondenserWaterIndependentVariable("LeavingCondenser"));
  ASSERT_EQ("LeavingCondenser", ch_heater.coolingModeTemperatureCurveCondenserWaterIndependentVariable());

  // Return type: bool
  // biquadratic
  CurveBiquadratic coolmode_ccFT(model);
  ASSERT_TRUE(ch_heater.setCoolingModeCoolingCapacityFunctionofTemperatureCurve(coolmode_ccFT));
  ASSERT_EQ(coolmode_ccFT, ch_heater.coolingModeCoolingCapacityFunctionofTemperatureCurve());

  // Return type: bool
  // biquadratic
  CurveBiquadratic coolmode_eicoFT(model);
  ASSERT_TRUE(ch_heater.setCoolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(coolmode_eicoFT));
  ASSERT_EQ(coolmode_eicoFT, ch_heater.coolingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve());

  // Return type: bool
  // Bicubic or quadratic
  CurveCubic coolmode_eicoFPLR(model);
  ASSERT_TRUE(ch_heater.setCoolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(coolmode_eicoFPLR));
  ASSERT_EQ(coolmode_eicoFPLR, ch_heater.coolingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setCoolingModeCoolingCapacityOptimumPartLoadRatio(0.87));
  EXPECT_TRUE(ch_heater.coolingModeCoolingCapacityOptimumPartLoadRatio());
  ASSERT_EQ(0.87, ch_heater.coolingModeCoolingCapacityOptimumPartLoadRatio());

  // Return type: bool
  // "LeavingCondenser" or "EnteringCondenser"
  ASSERT_TRUE(ch_heater.setHeatingModeTemperatureCurveCondenserWaterIndependentVariable("EnteringCondenser"));
  ASSERT_EQ("EnteringCondenser", ch_heater.heatingModeTemperatureCurveCondenserWaterIndependentVariable());

  // Return type: bool
  // biquadratic
  CurveBiquadratic heatmode_ccFT(model);
  ASSERT_TRUE(ch_heater.setHeatingModeCoolingCapacityFunctionofTemperatureCurve(heatmode_ccFT));
  ASSERT_EQ(heatmode_ccFT, ch_heater.heatingModeCoolingCapacityFunctionofTemperatureCurve());

  // Return type: bool
  // biquadratic
  CurveBiquadratic heatmode_eicoFT(model);
  ASSERT_TRUE(ch_heater.setHeatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve(heatmode_eicoFT));
  ASSERT_EQ(heatmode_eicoFT, ch_heater.heatingModeElectricInputtoCoolingOutputRatioFunctionofTemperatureCurve());

  // Return type: bool
  // Bicubic or quadratic
  CurveCubic heatmode_eicoFPLR(model);
  ASSERT_TRUE(ch_heater.setHeatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve(heatmode_eicoFPLR));
  ASSERT_EQ(heatmode_eicoFPLR, ch_heater.heatingModeElectricInputtoCoolingOutputRatioFunctionofPartLoadRatioCurve());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setHeatingModeCoolingCapacityOptimumPartLoadRatio(0.91));
  ASSERT_EQ(0.91, ch_heater.heatingModeCoolingCapacityOptimumPartLoadRatio());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setSizingFactor(1.05));
  ASSERT_EQ(1.05, ch_heater.sizingFactor());

}
