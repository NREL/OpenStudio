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

#include "../Screen.hpp"
#include "../Screen_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,Screen_DiffuseSolarReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Screen screen(model);

  Unit units = screen.getDiffuseSolarReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(screen.setDiffuseSolarReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(screen.setDiffuseSolarReflectance(testQ2));
  Quantity q2 = screen.getDiffuseSolarReflectance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Screen_DiffuseVisibleReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Screen screen(model);

  Unit units = screen.getDiffuseVisibleReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(screen.setDiffuseVisibleReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(screen.setDiffuseVisibleReflectance(testQ2));
  Quantity q2 = screen.getDiffuseVisibleReflectance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Screen_ThermalHemisphericalEmissivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  Screen screen(model);

  Unit units = screen.getThermalHemisphericalEmissivity(true).units(); // Get IP units.

  // Bounds: 0.0 < value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(screen.setThermalHemisphericalEmissivity(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(screen.setThermalHemisphericalEmissivity(testQ2));
  Quantity q2 = screen.getThermalHemisphericalEmissivity(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Screen_Conductivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  Screen screen(model);

  Unit units = screen.getConductivity(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(screen.setConductivity(testQ));
  Quantity q = screen.getConductivity(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Screen_ScreenMaterialSpacing_Quantity) {
  Model model;
  // TODO: Check constructor.
  Screen screen(model);

  Unit units = screen.getScreenMaterialSpacing(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(screen.setScreenMaterialSpacing(testQ));
  Quantity q = screen.getScreenMaterialSpacing(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Screen_ScreenMaterialDiameter_Quantity) {
  Model model;
  // TODO: Check constructor.
  Screen screen(model);

  Unit units = screen.getScreenMaterialDiameter(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(screen.setScreenMaterialDiameter(testQ));
  Quantity q = screen.getScreenMaterialDiameter(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Screen_ScreentoGlassDistance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Screen screen(model);

  Unit units = screen.getScreentoGlassDistance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(screen.setScreentoGlassDistance(testQ));
  Quantity q = screen.getScreentoGlassDistance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Screen_TopOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Screen screen(model);

  Unit units = screen.getTopOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(screen.setTopOpeningMultiplier(testQ));
  Quantity q = screen.getTopOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Screen_BottomOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Screen screen(model);

  Unit units = screen.getBottomOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(screen.setBottomOpeningMultiplier(testQ));
  Quantity q = screen.getBottomOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Screen_LeftSideOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Screen screen(model);

  Unit units = screen.getLeftSideOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(screen.setLeftSideOpeningMultiplier(testQ));
  Quantity q = screen.getLeftSideOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Screen_RightSideOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Screen screen(model);

  Unit units = screen.getRightSideOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(screen.setRightSideOpeningMultiplier(testQ));
  Quantity q = screen.getRightSideOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

