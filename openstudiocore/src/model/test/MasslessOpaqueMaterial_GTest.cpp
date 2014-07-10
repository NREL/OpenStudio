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

#include "../MasslessOpaqueMaterial.hpp"
#include "../MasslessOpaqueMaterial_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,MasslessOpaqueMaterial_ThermalResistance_Quantity) {
  Model model;
  // TODO: Check constructor.
  MasslessOpaqueMaterial masslessOpaqueMaterial(model);

  Unit units = masslessOpaqueMaterial.getThermalResistance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(masslessOpaqueMaterial.setThermalResistance(testQ));
  Quantity q = masslessOpaqueMaterial.getThermalResistance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,MasslessOpaqueMaterial_ThermalAbsorptance_Quantity) {
  Model model;
  // TODO: Check constructor.
  MasslessOpaqueMaterial masslessOpaqueMaterial(model);

  Unit units = masslessOpaqueMaterial.getThermalAbsorptance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value <= 0.99999

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(masslessOpaqueMaterial.setThermalAbsorptance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(masslessOpaqueMaterial.setThermalAbsorptance(testQ2));
  Quantity q2 = masslessOpaqueMaterial.getThermalAbsorptance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,MasslessOpaqueMaterial_SolarAbsorptance_Quantity) {
  Model model;
  // TODO: Check constructor.
  MasslessOpaqueMaterial masslessOpaqueMaterial(model);

  Unit units = masslessOpaqueMaterial.getSolarAbsorptance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(masslessOpaqueMaterial.setSolarAbsorptance(testQ));
  Quantity q = masslessOpaqueMaterial.getSolarAbsorptance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,MasslessOpaqueMaterial_VisibleAbsorptance_Quantity) {
  Model model;
  // TODO: Check constructor.
  MasslessOpaqueMaterial masslessOpaqueMaterial(model);

  Unit units = masslessOpaqueMaterial.getVisibleAbsorptance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value <= 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(masslessOpaqueMaterial.setVisibleAbsorptance(testQ));
  Quantity q = masslessOpaqueMaterial.getVisibleAbsorptance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

