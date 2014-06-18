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

#include "../RefractionExtinctionGlazing.hpp"
#include "../RefractionExtinctionGlazing_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,RefractionExtinctionGlazing_Thickness_Quantity) {
  Model model;
  // TODO: Check constructor.
  RefractionExtinctionGlazing refractionExtinctionGlazing(model);

  Unit units = refractionExtinctionGlazing.getThickness(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(refractionExtinctionGlazing.setThickness(testQ));
  Quantity q = refractionExtinctionGlazing.getThickness(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RefractionExtinctionGlazing_SolarIndexofRefraction_Quantity) {
  Model model;
  // TODO: Check constructor.
  RefractionExtinctionGlazing refractionExtinctionGlazing(model);

  Unit units = refractionExtinctionGlazing.getSolarIndexofRefraction(true).units(); // Get IP units.
  
  // Bounds: 1.0 < value

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(refractionExtinctionGlazing.setSolarIndexofRefraction(testQ));

  double value2(1.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(refractionExtinctionGlazing.setSolarIndexofRefraction(testQ2));
  Quantity q2 = refractionExtinctionGlazing.getSolarIndexofRefraction(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RefractionExtinctionGlazing_SolarExtinctionCoefficient_Quantity) {
  Model model;
  // TODO: Check constructor.
  RefractionExtinctionGlazing refractionExtinctionGlazing(model);

  Unit units = refractionExtinctionGlazing.getSolarExtinctionCoefficient(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(refractionExtinctionGlazing.setSolarExtinctionCoefficient(testQ));
  Quantity q = refractionExtinctionGlazing.getSolarExtinctionCoefficient(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RefractionExtinctionGlazing_VisibleIndexofRefraction_Quantity) {
  Model model;
  // TODO: Check constructor.
  RefractionExtinctionGlazing refractionExtinctionGlazing(model);

  Unit units = refractionExtinctionGlazing.getVisibleIndexofRefraction(true).units(); // Get IP units.

  // Bounds: 1.0 < value

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(refractionExtinctionGlazing.setVisibleIndexofRefraction(testQ));

  double value2(1.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(refractionExtinctionGlazing.setVisibleIndexofRefraction(testQ2));
  Quantity q2 = refractionExtinctionGlazing.getVisibleIndexofRefraction(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RefractionExtinctionGlazing_VisibleExtinctionCoefficient_Quantity) {
  Model model;
  // TODO: Check constructor.
  RefractionExtinctionGlazing refractionExtinctionGlazing(model);

  Unit units = refractionExtinctionGlazing.getVisibleExtinctionCoefficient(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(refractionExtinctionGlazing.setVisibleExtinctionCoefficient(testQ));
  Quantity q = refractionExtinctionGlazing.getVisibleExtinctionCoefficient(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RefractionExtinctionGlazing_InfraredTransmittanceatNormalIncidence_Quantity) {
  Model model;
  // TODO: Check constructor.
  RefractionExtinctionGlazing refractionExtinctionGlazing(model);

  Unit units = refractionExtinctionGlazing.getInfraredTransmittanceatNormalIncidence(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(refractionExtinctionGlazing.setInfraredTransmittanceatNormalIncidence(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(refractionExtinctionGlazing.setInfraredTransmittanceatNormalIncidence(testQ2));
  Quantity q2 = refractionExtinctionGlazing.getInfraredTransmittanceatNormalIncidence(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RefractionExtinctionGlazing_InfraredHemisphericalEmissivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  RefractionExtinctionGlazing refractionExtinctionGlazing(model);

  Unit units = refractionExtinctionGlazing.getInfraredHemisphericalEmissivity(true).units(); // Get IP units.
  
  // Bounds: 0.0 < value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(refractionExtinctionGlazing.setInfraredHemisphericalEmissivity(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(refractionExtinctionGlazing.setInfraredHemisphericalEmissivity(testQ2));
  Quantity q2 = refractionExtinctionGlazing.getInfraredHemisphericalEmissivity(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RefractionExtinctionGlazing_Conductivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  RefractionExtinctionGlazing refractionExtinctionGlazing(model);

  Unit units = refractionExtinctionGlazing.getConductivity(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(refractionExtinctionGlazing.setConductivity(testQ));
  Quantity q = refractionExtinctionGlazing.getConductivity(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RefractionExtinctionGlazing_DirtCorrectionFactorforSolarandVisibleTransmittance_Quantity) {
  Model model;
  // TODO: Check constructor.
  RefractionExtinctionGlazing refractionExtinctionGlazing(model);

  Unit units = refractionExtinctionGlazing.getDirtCorrectionFactorforSolarandVisibleTransmittance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(refractionExtinctionGlazing.setDirtCorrectionFactorforSolarandVisibleTransmittance(testQ));
  Quantity q = refractionExtinctionGlazing.getDirtCorrectionFactorforSolarandVisibleTransmittance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

