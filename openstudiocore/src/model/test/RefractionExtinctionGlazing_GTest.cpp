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

