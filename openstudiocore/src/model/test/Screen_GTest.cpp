/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

