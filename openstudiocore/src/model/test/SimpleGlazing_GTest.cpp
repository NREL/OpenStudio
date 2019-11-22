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

