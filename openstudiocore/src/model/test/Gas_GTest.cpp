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

