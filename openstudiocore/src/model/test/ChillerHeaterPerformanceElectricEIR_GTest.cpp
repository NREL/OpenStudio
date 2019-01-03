/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../Model.hpp"

#include "../ChillerHeaterPerformanceElectricEIR.hpp"
#include "../ChillerHeaterPerformanceElectricEIR_Impl.hpp"

#include "../CentralHeatPumpSystemModule.hpp"
#include "../CentralHeatPumpSystemModule_Impl.hpp"
#include "../CentralHeatPumpSystem.hpp"
#include "../CentralHeatPumpSystem_Impl.hpp"

// Curves
#include "../Curve.hpp"
#include "../CurveBiquadratic.hpp"
#include "../CurveBiquadratic_Impl.hpp"
#include "../CurveCubic.hpp"
#include "../CurveCubic_Impl.hpp"
#include "../CurveBicubic.hpp"
#include "../CurveBicubic_Impl.hpp"
#include "../CurveQuadratic.hpp"
#include "../CurveQuadratic_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,ChillerHeaterPerformanceElectricEIR_ChillerHeaterPerformanceElectricEIR)
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
TEST_F(ModelFixture,ChillerHeaterPerformanceElectricEIR_SettersGetters)
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
  ASSERT_TRUE(ch_heater.setCoolingModeCoolingCapacityFunctionOfTemperatureCurve(coolmode_ccFT));
  ASSERT_EQ(coolmode_ccFT, ch_heater.coolingModeCoolingCapacityFunctionOfTemperatureCurve());

  // Return type: bool
  // biquadratic
  CurveBiquadratic coolmode_eicoFT(model);
  ASSERT_TRUE(ch_heater.setCoolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(coolmode_eicoFT));
  ASSERT_EQ(coolmode_eicoFT, ch_heater.coolingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve());

  // Return type: bool
  // Bicubic or quadratic
  CurveBicubic coolmode_eicoFPLR(model);
  ASSERT_TRUE(ch_heater.setCoolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(coolmode_eicoFPLR));
  ASSERT_EQ(coolmode_eicoFPLR, ch_heater.coolingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setCoolingModeCoolingCapacityOptimumPartLoadRatio(0.87));
  ASSERT_EQ(0.87, ch_heater.coolingModeCoolingCapacityOptimumPartLoadRatio());

  // Return type: bool
  // "LeavingCondenser" or "EnteringCondenser"
  ASSERT_TRUE(ch_heater.setHeatingModeTemperatureCurveCondenserWaterIndependentVariable("EnteringCondenser"));
  ASSERT_EQ("EnteringCondenser", ch_heater.heatingModeTemperatureCurveCondenserWaterIndependentVariable());

  // Return type: bool
  // biquadratic
  CurveBiquadratic heatmode_ccFT(model);
  ASSERT_TRUE(ch_heater.setHeatingModeCoolingCapacityFunctionOfTemperatureCurve(heatmode_ccFT));
  ASSERT_EQ(heatmode_ccFT, ch_heater.heatingModeCoolingCapacityFunctionOfTemperatureCurve());

  // Return type: bool
  // biquadratic
  CurveBiquadratic heatmode_eicoFT(model);
  ASSERT_TRUE(ch_heater.setHeatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve(heatmode_eicoFT));
  ASSERT_EQ(heatmode_eicoFT, ch_heater.heatingModeElectricInputToCoolingOutputRatioFunctionOfTemperatureCurve());

  // Return type: bool
  // Bicubic or quadratic
  CurveBicubic heatmode_eicoFPLR(model);
  ASSERT_TRUE(ch_heater.setHeatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve(heatmode_eicoFPLR));
  ASSERT_EQ(heatmode_eicoFPLR, ch_heater.heatingModeElectricInputToCoolingOutputRatioFunctionOfPartLoadRatioCurve());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setHeatingModeCoolingCapacityOptimumPartLoadRatio(0.91));
  ASSERT_EQ(0.91, ch_heater.heatingModeCoolingCapacityOptimumPartLoadRatio());

  // Return type: bool
  ASSERT_TRUE(ch_heater.setSizingFactor(1.05));
  ASSERT_EQ(1.05, ch_heater.sizingFactor());

}

TEST_F(ModelFixture,ChillerHeaterPerformanceElectricEIR_CloneWithoutModule)
{
  Model model;

  ChillerHeaterPerformanceElectricEIR ch_heater(model);
  Curve c = ch_heater.coolingModeCoolingCapacityFunctionOfTemperatureCurve();
  ASSERT_EQ(1u, model.getModelObjects<ChillerHeaterPerformanceElectricEIR>().size());

  // Clone the ChillerHeater into the same model
  ChillerHeaterPerformanceElectricEIR  ch_heaterClone = ch_heater.clone(model).cast<ChillerHeaterPerformanceElectricEIR>();
  Curve c1 = ch_heaterClone.coolingModeCoolingCapacityFunctionOfTemperatureCurve();
  ASSERT_EQ(2u, model.getModelObjects<ChillerHeaterPerformanceElectricEIR>().size());
  // Check that it points to the same curve
  ASSERT_EQ(c.handle(), c1.handle());


  // Clone into another model
  Model model2;
  ChillerHeaterPerformanceElectricEIR  ch_heaterClone2 = ch_heater.clone(model2).cast<ChillerHeaterPerformanceElectricEIR>();
  ASSERT_EQ(1u, model2.getModelObjects<ChillerHeaterPerformanceElectricEIR>().size());
  Curve c2 = ch_heaterClone2.coolingModeCoolingCapacityFunctionOfTemperatureCurve();
  ASSERT_EQ(c.name(), c2.name());

}

TEST_F(ModelFixture,ChillerHeaterPerformanceElectricEIR_CloneWithModule)
{
  Model model;

  // Create a CentralHeatPumpSystemModule and get its ChillerHeater (from constructor)
  CentralHeatPumpSystemModule mod(model);
  ChillerHeaterPerformanceElectricEIR ch_heater = mod.chillerHeaterModulesPerformanceComponent();

  Curve c = ch_heater.coolingModeCoolingCapacityFunctionOfTemperatureCurve();
  ASSERT_EQ(1u, model.getModelObjects<ChillerHeaterPerformanceElectricEIR>().size());

  // Clone the ChillerHeater into the same model
  ChillerHeaterPerformanceElectricEIR  ch_heaterClone = ch_heater.clone(model).cast<ChillerHeaterPerformanceElectricEIR>();
  Curve c1 = ch_heaterClone.coolingModeCoolingCapacityFunctionOfTemperatureCurve();
  ASSERT_EQ(2u, model.getModelObjects<ChillerHeaterPerformanceElectricEIR>().size());
  // Check that it points to the same curve
  ASSERT_EQ(c.handle(), c1.handle());

  // Check that it didn't clone the parent Module...
  ASSERT_EQ(1u, model.getModelObjects<CentralHeatPumpSystemModule>().size());

  // Clone into another model
  Model model2;
  ChillerHeaterPerformanceElectricEIR  ch_heaterClone2 = ch_heater.clone(model2).cast<ChillerHeaterPerformanceElectricEIR>();
  ASSERT_EQ(1u, model2.getModelObjects<ChillerHeaterPerformanceElectricEIR>().size());
  Curve c2 = ch_heaterClone2.coolingModeCoolingCapacityFunctionOfTemperatureCurve();
  ASSERT_EQ(c.name(), c2.name());
  // Check that it didn't clone the parent Module...Should be zero
  ASSERT_EQ(0u, model2.getModelObjects<CentralHeatPumpSystemModule>().size());


}


TEST_F(ModelFixture, ChillerHeaterPerformanceElectricEIR_ReverseLookups)
{
  // Nothing set
  {
    Model model;
    ChillerHeaterPerformanceElectricEIR ch_heater(model);
    ASSERT_EQ(0u, ch_heater.centralHeatPumpSystemModules().size());
    ASSERT_EQ(0u, ch_heater.centralHeatPumpSystems().size());
  }

  // A module, no centralHP
  {
    Model model;

    // Create a CentralHeatPumpSystemModule and get its ChillerHeater (from constructor)
    CentralHeatPumpSystemModule mod(model);
    ChillerHeaterPerformanceElectricEIR ch_heater = mod.chillerHeaterModulesPerformanceComponent();

    ASSERT_EQ(1u, ch_heater.centralHeatPumpSystemModules().size());
    ASSERT_EQ(mod.handle(), ch_heater.centralHeatPumpSystemModules()[0].handle());

    ASSERT_EQ(0u, ch_heater.centralHeatPumpSystems().size());

  }


  // A module and a centralHP
  {
    Model model;

    // Create a CentralHeatPumpSystemModule and get its ChillerHeater (from constructor)
    CentralHeatPumpSystem central_hp(model);
    CentralHeatPumpSystemModule mod(model);
    ChillerHeaterPerformanceElectricEIR ch_heater = mod.chillerHeaterModulesPerformanceComponent();
    ASSERT_TRUE(central_hp.addModule(mod));

    ASSERT_EQ(1u, central_hp.modules().size());

    ASSERT_EQ(1u, ch_heater.centralHeatPumpSystemModules().size());
    ASSERT_EQ(mod.handle(), ch_heater.centralHeatPumpSystemModules()[0].handle());

    ASSERT_EQ(1u, ch_heater.centralHeatPumpSystems().size());
    ASSERT_EQ(central_hp.handle(), ch_heater.centralHeatPumpSystems()[0].handle());

  }

  // Two parent modules in two different centralHP
  {
    Model model;

    // Create a CentralHeatPumpSystemModule and get its ChillerHeater (from constructor)
    CentralHeatPumpSystem central_hp(model);
    CentralHeatPumpSystemModule mod(model);
    ChillerHeaterPerformanceElectricEIR ch_heater = mod.chillerHeaterModulesPerformanceComponent();
    ASSERT_TRUE(central_hp.addModule(mod));

    CentralHeatPumpSystem central_hp2(model);
    CentralHeatPumpSystemModule mod2(model);
    ASSERT_TRUE(central_hp2.addModule(mod2));
    mod2.setChillerHeaterModulesPerformanceComponent(ch_heater);

    ASSERT_EQ(2u, ch_heater.centralHeatPumpSystemModules().size());

    ASSERT_EQ(2u, ch_heater.centralHeatPumpSystems().size());

  }

  // Two parent modules in the same centralHP
  {
    Model model;

    // Create a CentralHeatPumpSystemModule and get its ChillerHeater (from constructor)
    CentralHeatPumpSystem central_hp(model);
    CentralHeatPumpSystemModule mod(model);
    ChillerHeaterPerformanceElectricEIR ch_heater = mod.chillerHeaterModulesPerformanceComponent();
    ASSERT_TRUE(central_hp.addModule(mod));

    CentralHeatPumpSystemModule mod2(model);
    ASSERT_TRUE(central_hp.addModule(mod2));
    mod2.setChillerHeaterModulesPerformanceComponent(ch_heater);

    ASSERT_EQ(2u, ch_heater.centralHeatPumpSystemModules().size());

    ASSERT_EQ(1u, ch_heater.centralHeatPumpSystems().size());

  }


}
