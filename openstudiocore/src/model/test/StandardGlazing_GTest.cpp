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
