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

#include "../StandardGlazing.hpp"
#include "../StandardGlazing_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,StandardGlazing_Thickness_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getThickness(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(standardGlazing.setThickness(testQ));
  Quantity q = standardGlazing.getThickness(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,StandardGlazing_SolarTransmittanceatNormalIncidence_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getSolarTransmittanceatNormalIncidence(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(standardGlazing.setSolarTransmittanceatNormalIncidence(testQ));
  OSOptionalQuantity q = standardGlazing.getSolarTransmittanceatNormalIncidence(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,StandardGlazing_FrontSideSolarReflectanceatNormalIncidence_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getFrontSideSolarReflectanceatNormalIncidence(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(standardGlazing.setFrontSideSolarReflectanceatNormalIncidence(testQ));
  OSOptionalQuantity q = standardGlazing.getFrontSideSolarReflectanceatNormalIncidence(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,StandardGlazing_BackSideSolarReflectanceatNormalIncidence_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getBackSideSolarReflectanceatNormalIncidence(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(standardGlazing.setBackSideSolarReflectanceatNormalIncidence(testQ));
  OSOptionalQuantity q = standardGlazing.getBackSideSolarReflectanceatNormalIncidence(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,StandardGlazing_VisibleTransmittanceatNormalIncidence_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getVisibleTransmittanceatNormalIncidence(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(standardGlazing.setVisibleTransmittanceatNormalIncidence(testQ));
  OSOptionalQuantity q = standardGlazing.getVisibleTransmittanceatNormalIncidence(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,StandardGlazing_FrontSideVisibleReflectanceatNormalIncidence_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getFrontSideVisibleReflectanceatNormalIncidence(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(standardGlazing.setFrontSideVisibleReflectanceatNormalIncidence(testQ));
  OSOptionalQuantity q = standardGlazing.getFrontSideVisibleReflectanceatNormalIncidence(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,StandardGlazing_BackSideVisibleReflectanceatNormalIncidence_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getBackSideVisibleReflectanceatNormalIncidence(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(standardGlazing.setBackSideVisibleReflectanceatNormalIncidence(testQ));
  OSOptionalQuantity q = standardGlazing.getBackSideVisibleReflectanceatNormalIncidence(true);
  ASSERT_TRUE(q.isSet());
  EXPECT_NEAR(value,q.get().value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,StandardGlazing_InfraredTransmittanceatNormalIncidence_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getInfraredTransmittanceatNormalIncidence(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(standardGlazing.setInfraredTransmittanceatNormalIncidence(testQ));
  Quantity q = standardGlazing.getInfraredTransmittanceatNormalIncidence(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,StandardGlazing_FrontSideInfraredHemisphericalEmissivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getFrontSideInfraredHemisphericalEmissivity(true).units(); // Get IP units.

  // Bounds: 0.0 < value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(standardGlazing.setFrontSideInfraredHemisphericalEmissivity(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(standardGlazing.setFrontSideInfraredHemisphericalEmissivity(testQ2));
  Quantity q2 = standardGlazing.getFrontSideInfraredHemisphericalEmissivity(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,StandardGlazing_BackSideInfraredHemisphericalEmissivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getBackSideInfraredHemisphericalEmissivity(true).units(); // Get IP units.

  // Bounds: 0.0 < value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(standardGlazing.setBackSideInfraredHemisphericalEmissivity(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(standardGlazing.setBackSideInfraredHemisphericalEmissivity(testQ2));
  Quantity q2 = standardGlazing.getBackSideInfraredHemisphericalEmissivity(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,StandardGlazing_Conductivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getConductivity(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(standardGlazing.setConductivity(testQ));
  Quantity q = standardGlazing.getConductivity(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,StandardGlazing_DirtCorrectionFactorforSolarandVisibleTransmittance_Quantity) {
  Model model;
  // TODO: Check constructor.
  StandardGlazing standardGlazing(model);

  Unit units = standardGlazing.getDirtCorrectionFactorforSolarandVisibleTransmittance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(standardGlazing.setDirtCorrectionFactorforSolarandVisibleTransmittance(testQ));
  Quantity q = standardGlazing.getDirtCorrectionFactorforSolarandVisibleTransmittance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}
