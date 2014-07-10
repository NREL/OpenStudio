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

#include "../GasMixture.hpp"
#include "../GasMixture_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,GasMixture_Thickness_Quantity) {
  Model model;
  // TODO: Check constructor.
  GasMixture gasMixture(model);

  Unit units = gasMixture.getThickness(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gasMixture.setThickness(testQ));
  Quantity q = gasMixture.getThickness(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,GasMixture_Gas1Fraction_Quantity) {
  Model model;
  // TODO: Check constructor.
  GasMixture gasMixture(model);

  Unit units = gasMixture.getGas1Fraction(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gasMixture.setGas1Fraction(testQ));
  Quantity q = gasMixture.getGas1Fraction(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,GasMixture_Gas2Fraction_Quantity) {
  Model model;
  // TODO: Check constructor.
  GasMixture gasMixture(model);

  Unit units = gasMixture.getGas2Fraction(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gasMixture.setGas2Fraction(testQ));
  Quantity q = gasMixture.getGas2Fraction(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,GasMixture_Gas3Fraction_Quantity) {
  Model model;
  // TODO: Check constructor.
  GasMixture gasMixture(model);

  Unit units = gasMixture.getGas3Fraction(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gasMixture.setGas3Fraction(testQ));
  OSOptionalQuantity q = gasMixture.getGas3Fraction(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,GasMixture_Gas4Fraction_Quantity) {
  Model model;
  // TODO: Check constructor.
  GasMixture gasMixture(model);

  Unit units = gasMixture.getGas4Fraction(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(gasMixture.setGas4Fraction(testQ));
  OSOptionalQuantity q = gasMixture.getGas4Fraction(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

