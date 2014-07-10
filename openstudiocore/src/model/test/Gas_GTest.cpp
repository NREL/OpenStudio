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

#include "../Gas.hpp"
#include "../Gas_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,Gas_Thickness_Quantity) {
  Model model;
  // TODO: Check constructor.
  Gas gas(model);

  Unit units = gas.getThickness(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gas.setThickness(testQ));
  Quantity q = gas.getThickness(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Gas_ConductivityCoefficientA_Quantity) {
  Model model;
  // TODO: Check constructor.
  Gas gas(model);

  Unit units = gas.getConductivityCoefficientA(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gas.setConductivityCoefficientA(testQ));
  OSOptionalQuantity q = gas.getConductivityCoefficientA(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Gas_ConductivityCoefficientB_Quantity) {
  Model model;
  // TODO: Check constructor.
  Gas gas(model);

  Unit units = gas.getConductivityCoefficientB(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gas.setConductivityCoefficientB(testQ));
  OSOptionalQuantity q = gas.getConductivityCoefficientB(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Gas_ViscosityCoefficientA_Quantity) {
  Model model;
  // TODO: Check constructor.
  Gas gas(model);

  Unit units = gas.getViscosityCoefficientA(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gas.setViscosityCoefficientA(testQ));
  OSOptionalQuantity q = gas.getViscosityCoefficientA(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Gas_ViscosityCoefficientB_Quantity) {
  Model model;
  // TODO: Check constructor.
  Gas gas(model);

  Unit units = gas.getViscosityCoefficientB(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gas.setViscosityCoefficientB(testQ));
  OSOptionalQuantity q = gas.getViscosityCoefficientB(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Gas_SpecificHeatCoefficientA_Quantity) {
  Model model;
  // TODO: Check constructor.
  Gas gas(model);

  Unit units = gas.getSpecificHeatCoefficientA(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gas.setSpecificHeatCoefficientA(testQ));
  OSOptionalQuantity q = gas.getSpecificHeatCoefficientA(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Gas_SpecificHeatCoefficientB_Quantity) {
  Model model;
  // TODO: Check constructor.
  Gas gas(model);

  Unit units = gas.getSpecificHeatCoefficientB(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gas.setSpecificHeatCoefficientB(testQ));
  OSOptionalQuantity q = gas.getSpecificHeatCoefficientB(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Gas_MolecularWeight_Quantity) {
  Model model;
  // TODO: Check constructor.
  Gas gas(model);

  Unit units = gas.getMolecularWeight(true).units(); // Get IP units.

  // Bounds: 20 <= value <= 200)

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(gas.setMolecularWeight(testQ));

  double value2(20.0);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(gas.setMolecularWeight(testQ2));
  OSOptionalQuantity q = gas.getMolecularWeight(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value2,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

