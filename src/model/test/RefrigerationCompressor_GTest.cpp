/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../RefrigerationCompressor.hpp"
#include "../RefrigerationCompressor_Impl.hpp"
#include "../CurveBicubic.hpp"
#include "../CurveBicubic_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RefrigerationCompressor_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model model;
      RefrigerationCompressor testObject = RefrigerationCompressor(model);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, RefrigerationCompressor_Remove) {
  Model model;
  RefrigerationCompressor testObject = RefrigerationCompressor(model);

  std::vector<RefrigerationCompressor> refrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(1, refrigerationCompressors.size());

  testObject.remove();

  refrigerationCompressors = model.getModelObjects<RefrigerationCompressor>();
  EXPECT_EQ(0, refrigerationCompressors.size());
}

TEST_F(ModelFixture, RefrigerationCompressor_CloneOneModelWithDefaultData) {
  Model model;
  RefrigerationCompressor testObject = RefrigerationCompressor(model);
  EXPECT_EQ("Subcritical", testObject.modeofOperation());

  RefrigerationCompressor testObjectClone = testObject.clone(model).cast<RefrigerationCompressor>();

  std::vector<CurveBicubic> refrigerationCompressorCurves = model.getModelObjects<CurveBicubic>();
  for (auto it = refrigerationCompressorCurves.begin(); it != refrigerationCompressorCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_DOUBLE_EQ(18.3, testObjectClone.ratedReturnGasTemperature().get());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone.ratedSubcooling().get());
  EXPECT_EQ("Subcritical", testObjectClone.modeofOperation());
  EXPECT_EQ(testObject.refrigerationCompressorPowerCurve().handle(), testObjectClone.refrigerationCompressorPowerCurve().handle());
  EXPECT_EQ(testObject.refrigerationCompressorCapacityCurve().handle(), testObjectClone.refrigerationCompressorCapacityCurve().handle());
}

TEST_F(ModelFixture, RefrigerationCompressor_CloneOneModelWithCustomData) {
  Model model;
  RefrigerationCompressor testObject = RefrigerationCompressor(model);
  testObject.setRatedReturnGasTemperature(999.0);
  testObject.setRatedSubcooling(999.0);

  CurveBicubic powerCurve = CurveBicubic(model);
  powerCurve.setName("Refrigeration Compressor Power Curve Test");
  powerCurve.setCoefficient1Constant(4451.46);
  powerCurve.setCoefficient2x(-166.108);
  powerCurve.setCoefficient3xPOW2(-5.35664);
  powerCurve.setCoefficient4y(263.553);
  powerCurve.setCoefficient5yPOW2(-0.958849);
  powerCurve.setCoefficient6xTIMESY(8.09252);
  powerCurve.setCoefficient7xPOW3(-0.0239354);
  powerCurve.setCoefficient8yPOW3(0.000633306);
  powerCurve.setCoefficient9xPOW2TIMESY(0.0778798);
  powerCurve.setCoefficient10xTIMESYPOW2(-0.0167386);
  powerCurve.setMinimumValueofx(-23.3);
  powerCurve.setMaximumValueofx(7.2);
  powerCurve.setMinimumValueofy(10.0);
  powerCurve.setMaximumValueofy(60.0);
  powerCurve.setInputUnitTypeforX("Temperature");
  powerCurve.setInputUnitTypeforY("Temperature");
  powerCurve.setOutputUnitType("Power");

  testObject.setRefrigerationCompressorPowerCurve(powerCurve);

  CurveBicubic capacityCurve = CurveBicubic(model);
  capacityCurve.setName("Refrigeration Compressor Capacity Curve Test");
  capacityCurve.setCoefficient1Constant(83249.9);
  capacityCurve.setCoefficient2x(3147.96);
  capacityCurve.setCoefficient3xPOW2(44.9276);
  capacityCurve.setCoefficient4y(-1035.51);
  capacityCurve.setCoefficient5yPOW2(3.74692);
  capacityCurve.setCoefficient6xTIMESY(-37.3907);
  capacityCurve.setCoefficient7xPOW3(0.179897);
  capacityCurve.setCoefficient8yPOW3(-0.0199971);
  capacityCurve.setCoefficient9xPOW2TIMESY(-0.481641);
  capacityCurve.setCoefficient10xTIMESYPOW2(0.0428673);
  capacityCurve.setMinimumValueofx(-23.3);
  capacityCurve.setMaximumValueofx(7.2);
  capacityCurve.setMinimumValueofy(10.0);
  capacityCurve.setMaximumValueofy(60.0);
  capacityCurve.setInputUnitTypeforX("Temperature");
  capacityCurve.setInputUnitTypeforY("Temperature");
  capacityCurve.setOutputUnitType("Capacity");

  testObject.setRefrigerationCompressorCapacityCurve(capacityCurve);

  CurveBicubic transPowerCurve = CurveBicubic(model);
  transPowerCurve.setName("Transcritical Compressor Power Curve Test");
  transPowerCurve.setCoefficient1Constant(4451.46);
  transPowerCurve.setCoefficient2x(-166.108);
  transPowerCurve.setCoefficient3xPOW2(-5.35664);
  transPowerCurve.setCoefficient4y(263.553);
  transPowerCurve.setCoefficient5yPOW2(-0.958849);
  transPowerCurve.setCoefficient6xTIMESY(8.09252);
  transPowerCurve.setCoefficient7xPOW3(-0.0239354);
  transPowerCurve.setCoefficient8yPOW3(0.000633306);
  transPowerCurve.setCoefficient9xPOW2TIMESY(0.0778798);
  transPowerCurve.setCoefficient10xTIMESYPOW2(-0.0167386);
  transPowerCurve.setMinimumValueofx(-23.3);
  transPowerCurve.setMaximumValueofx(7.2);
  transPowerCurve.setMinimumValueofy(10.0);
  transPowerCurve.setMaximumValueofy(60.0);
  transPowerCurve.setInputUnitTypeforX("Temperature");
  transPowerCurve.setInputUnitTypeforY("Temperature");
  transPowerCurve.setOutputUnitType("Power");

  testObject.setTranscriticalCompressorPowerCurve(transPowerCurve);

  CurveBicubic transCapacityCurve = CurveBicubic(model);
  transCapacityCurve.setName("Transcritical Compressor Capacity Curve Test");
  transCapacityCurve.setCoefficient1Constant(83249.9);
  transCapacityCurve.setCoefficient2x(3147.96);
  transCapacityCurve.setCoefficient3xPOW2(44.9276);
  transCapacityCurve.setCoefficient4y(-1035.51);
  transCapacityCurve.setCoefficient5yPOW2(3.74692);
  transCapacityCurve.setCoefficient6xTIMESY(-37.3907);
  transCapacityCurve.setCoefficient7xPOW3(0.179897);
  transCapacityCurve.setCoefficient8yPOW3(-0.0199971);
  transCapacityCurve.setCoefficient9xPOW2TIMESY(-0.481641);
  transCapacityCurve.setCoefficient10xTIMESYPOW2(0.0428673);
  transCapacityCurve.setMinimumValueofx(-23.3);
  transCapacityCurve.setMaximumValueofx(7.2);
  transCapacityCurve.setMinimumValueofy(10.0);
  transCapacityCurve.setMaximumValueofy(60.0);
  transCapacityCurve.setInputUnitTypeforX("Temperature");
  transCapacityCurve.setInputUnitTypeforY("Temperature");
  transCapacityCurve.setOutputUnitType("Capacity");

  testObject.setTranscriticalCompressorCapacityCurve(transCapacityCurve);
  EXPECT_EQ("Transcritical", testObject.modeofOperation());

  RefrigerationCompressor testObjectClone = testObject.clone(model).cast<RefrigerationCompressor>();
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedReturnGasTemperature().get());
  EXPECT_DOUBLE_EQ(999.0, testObjectClone.ratedSubcooling().get());
  EXPECT_EQ("Transcritical", testObjectClone.modeofOperation());
  EXPECT_EQ(testObject.refrigerationCompressorPowerCurve().handle(), testObjectClone.refrigerationCompressorPowerCurve().handle());
  EXPECT_EQ(testObject.refrigerationCompressorCapacityCurve().handle(), testObjectClone.refrigerationCompressorCapacityCurve().handle());
  EXPECT_EQ(powerCurve.handle(), testObjectClone.refrigerationCompressorPowerCurve().handle());
  EXPECT_EQ(capacityCurve.handle(), testObjectClone.refrigerationCompressorCapacityCurve().handle());
  EXPECT_EQ(transPowerCurve.handle(), testObjectClone.transcriticalCompressorPowerCurve().get().handle());
  EXPECT_EQ(transCapacityCurve.handle(), testObjectClone.transcriticalCompressorCapacityCurve().get().handle());
}

TEST_F(ModelFixture, RefrigerationCompressor_CloneTwoModelsWithDefaultData) {
  Model model;
  RefrigerationCompressor testObject = RefrigerationCompressor(model);

  RefrigerationCompressor testObjectClone = testObject.clone(model).cast<RefrigerationCompressor>();

  Model model2;

  std::vector<CurveBicubic> refrigerationCompressorCurves2 = model2.getModelObjects<CurveBicubic>();
  EXPECT_EQ(0, refrigerationCompressorCurves2.size());

  RefrigerationCompressor testObjectClone2 = testObject.clone(model2).cast<RefrigerationCompressor>();

  std::vector<CurveBicubic> refrigerationCompressorCurves = model.getModelObjects<CurveBicubic>();
  EXPECT_EQ(2, refrigerationCompressorCurves.size());

  refrigerationCompressorCurves2 = model2.getModelObjects<CurveBicubic>();
  EXPECT_EQ(2, refrigerationCompressorCurves2.size());

  for (auto it = refrigerationCompressorCurves.begin(); it != refrigerationCompressorCurves.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  for (auto it = refrigerationCompressorCurves2.begin(); it != refrigerationCompressorCurves2.end(); ++it) {
    EXPECT_TRUE(it->parent());
  }

  EXPECT_DOUBLE_EQ(18.3, testObjectClone2.ratedReturnGasTemperature().get());
  EXPECT_DOUBLE_EQ(0.0, testObjectClone2.ratedSubcooling().get());
  EXPECT_NE(testObject.refrigerationCompressorPowerCurve().handle(), testObjectClone2.refrigerationCompressorPowerCurve().handle());
  EXPECT_NE(testObject.refrigerationCompressorCapacityCurve().handle(), testObjectClone2.refrigerationCompressorCapacityCurve().handle());
  EXPECT_NE(testObjectClone2, testObjectClone);
  EXPECT_NE(testObjectClone2.handle(), testObjectClone.handle());
}
