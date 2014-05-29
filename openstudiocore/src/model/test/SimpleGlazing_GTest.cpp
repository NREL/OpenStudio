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

#include "../SimpleGlazing.hpp"
#include "../SimpleGlazing_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,SimpleGlazing_UFactor_Quantity) {
  Model model;
  // TODO: Check constructor.
  SimpleGlazing simpleGlazing(model);

  Unit units = simpleGlazing.getUFactor(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(simpleGlazing.setUFactor(testQ));
  Quantity q = simpleGlazing.getUFactor(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,SimpleGlazing_SolarHeatGainCoefficient_Quantity) {
  Model model;
  // TODO: Check constructor.
  SimpleGlazing simpleGlazing(model);

  Unit units = simpleGlazing.getSolarHeatGainCoefficient(true).units(); // Get IP units.

  // Bounds: 0.0 < value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(simpleGlazing.setSolarHeatGainCoefficient(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(simpleGlazing.setSolarHeatGainCoefficient(testQ2));
  Quantity q2 = simpleGlazing.getSolarHeatGainCoefficient(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,SimpleGlazing_VisibleTransmittance_Quantity) {
  Model model;
  // TODO: Check constructor.
  SimpleGlazing simpleGlazing(model);

  Unit units = simpleGlazing.getVisibleTransmittance(true).units(); // Get IP units.

  // Bounds: 0.0 < value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(simpleGlazing.setVisibleTransmittance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(simpleGlazing.setVisibleTransmittance(testQ2));
  OSOptionalQuantity q = simpleGlazing.getVisibleTransmittance(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value2,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

