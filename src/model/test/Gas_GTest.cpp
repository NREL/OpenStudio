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

#include "../Gas.hpp"
#include "../Gas_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, Gas_Gas) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      Gas gas(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, Gas_GettersSetters) {

  Model m;
  Gas gas(m);

  // Gas Type:  String
  // No Default
  // Test a valid choice
  EXPECT_TRUE(gas.setGasType("Air"));
  EXPECT_EQ("Air", gas.gasType());
  // Test an invalid choice
  EXPECT_FALSE(gas.setGasType("BadChoice"));
  EXPECT_EQ("Air", gas.gasType());

  // Thickness:  Double
  // No Default
  EXPECT_TRUE(gas.setThickness(1.1));
  EXPECT_EQ(1.1, gas.thickness());

  // Conductivity Coefficient A: Optional Double
  // No Default
  EXPECT_TRUE(gas.setConductivityCoefficientA(10.03));
  ASSERT_TRUE(gas.conductivityCoefficientA());
  EXPECT_EQ(10.03, gas.conductivityCoefficientA().get());
  gas.resetConductivityCoefficientA();
  EXPECT_FALSE(gas.conductivityCoefficientA());

  // Conductivity Coefficient B: Optional Double
  // No Default
  EXPECT_TRUE(gas.setConductivityCoefficientB(11.03));
  ASSERT_TRUE(gas.conductivityCoefficientB());
  EXPECT_EQ(11.03, gas.conductivityCoefficientB().get());
  gas.resetConductivityCoefficientB();
  EXPECT_FALSE(gas.conductivityCoefficientB());

  // Conductivity Coefficient C: Optional Double
  // No Default
  EXPECT_TRUE(gas.setConductivityCoefficientC(12.03));
  ASSERT_TRUE(gas.conductivityCoefficientC());
  EXPECT_EQ(12.03, gas.conductivityCoefficientC().get());
  gas.resetConductivityCoefficientC();
  EXPECT_FALSE(gas.conductivityCoefficientC());

  // Viscosity Coefficient A: Optional Double
  // No Default
  EXPECT_TRUE(gas.setViscosityCoefficientA(1.0));
  ASSERT_TRUE(gas.viscosityCoefficientA());
  EXPECT_EQ(1.0, gas.viscosityCoefficientA().get());
  gas.resetViscosityCoefficientA();
  EXPECT_FALSE(gas.viscosityCoefficientA());

  // Viscosity Coefficient B: Optional Double
  // No Default
  EXPECT_TRUE(gas.setViscosityCoefficientB(13.03));
  ASSERT_TRUE(gas.viscosityCoefficientB());
  EXPECT_EQ(13.03, gas.viscosityCoefficientB().get());
  gas.resetViscosityCoefficientB();
  EXPECT_FALSE(gas.viscosityCoefficientB());

  // Viscosity Coefficient C: Optional Double
  // No Default
  EXPECT_TRUE(gas.setViscosityCoefficientC(14.03));
  ASSERT_TRUE(gas.viscosityCoefficientC());
  EXPECT_EQ(14.03, gas.viscosityCoefficientC().get());
  gas.resetViscosityCoefficientC();
  EXPECT_FALSE(gas.viscosityCoefficientC());

  // Specific Heat Coefficient A: Optional Double
  // No Default
  EXPECT_TRUE(gas.setSpecificHeatCoefficientA(1.3));
  ASSERT_TRUE(gas.specificHeatCoefficientA());
  EXPECT_EQ(1.3, gas.specificHeatCoefficientA().get());
  gas.resetSpecificHeatCoefficientA();
  EXPECT_FALSE(gas.specificHeatCoefficientA());

  // Specific Heat Coefficient B: Optional Double
  // No Default
  EXPECT_TRUE(gas.setSpecificHeatCoefficientB(15.03));
  ASSERT_TRUE(gas.specificHeatCoefficientB());
  EXPECT_EQ(15.03, gas.specificHeatCoefficientB().get());
  gas.resetSpecificHeatCoefficientB();
  EXPECT_FALSE(gas.specificHeatCoefficientB());

  // Specific Heat Coefficient C: Optional Double
  // No Default
  EXPECT_TRUE(gas.setSpecificHeatCoefficientC(16.03));
  ASSERT_TRUE(gas.specificHeatCoefficientC());
  EXPECT_EQ(16.03, gas.specificHeatCoefficientC().get());
  gas.resetSpecificHeatCoefficientC();
  EXPECT_FALSE(gas.specificHeatCoefficientC());

  // Molecular Weight: Optional Double
  // No Default
  EXPECT_TRUE(gas.setMolecularWeight(110.0));
  ASSERT_TRUE(gas.molecularWeight());
  EXPECT_EQ(110.0, gas.molecularWeight().get());
  gas.resetMolecularWeight();
  EXPECT_FALSE(gas.molecularWeight());

  // Specific Heat Ratio: Optional Double
  // No Default
  EXPECT_TRUE(gas.setSpecificHeatRatio(2.0));
  ASSERT_TRUE(gas.specificHeatRatio());
  EXPECT_EQ(2.0, gas.specificHeatRatio().get());
  gas.resetSpecificHeatRatio();
  EXPECT_FALSE(gas.specificHeatRatio());
}
