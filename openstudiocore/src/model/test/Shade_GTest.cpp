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

#include "../Shade.hpp"
#include "../Shade_Impl.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,Shade_SolarTransmittance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getSolarTransmittance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(shade.setSolarTransmittance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(shade.setSolarTransmittance(testQ2));
  Quantity q2 = shade.getSolarTransmittance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Shade_SolarReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getSolarReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(shade.setSolarReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(shade.setSolarReflectance(testQ2));
  Quantity q2 = shade.getSolarReflectance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Shade_VisibleTransmittance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getVisibleTransmittance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(shade.setVisibleTransmittance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(shade.setVisibleTransmittance(testQ2));
  Quantity q2 = shade.getVisibleTransmittance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Shade_VisibleReflectance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getVisibleReflectance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(shade.setVisibleReflectance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(shade.setVisibleReflectance(testQ2));
  Quantity q2 = shade.getVisibleReflectance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Shade_ThermalHemisphericalEmissivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getThermalHemisphericalEmissivity(true).units(); // Get IP units.

  // Bounds: 0.0 < value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(shade.setThermalHemisphericalEmissivity(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(shade.setThermalHemisphericalEmissivity(testQ2));
  Quantity q2 = shade.getThermalHemisphericalEmissivity(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Shade_ThermalTransmittance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getThermalTransmittance(true).units(); // Get IP units.

  // Bounds: 0.0 <= value < 1.0

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(shade.setThermalTransmittance(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(shade.setThermalTransmittance(testQ2));
  Quantity q2 = shade.getThermalTransmittance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,Shade_Thickness_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getThickness(true).units(); // Get IP units.

  // \minimum> 0
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(shade.setThickness(testQ));
  Quantity q = shade.getThickness(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Shade_Conductivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getConductivity(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(shade.setConductivity(testQ));
  Quantity q = shade.getConductivity(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Shade_ShadetoGlassDistance_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getShadetoGlassDistance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(shade.setShadetoGlassDistance(testQ));
  Quantity q = shade.getShadetoGlassDistance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Shade_TopOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getTopOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(shade.setTopOpeningMultiplier(testQ));
  Quantity q = shade.getTopOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Shade_BottomOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getBottomOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(shade.setBottomOpeningMultiplier(testQ));
  Quantity q = shade.getBottomOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Shade_LeftSideOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getLeftSideOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(shade.setLeftSideOpeningMultiplier(testQ));
  Quantity q = shade.getLeftSideOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Shade_RightSideOpeningMultiplier_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getRightSideOpeningMultiplier(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(shade.setRightSideOpeningMultiplier(testQ));
  Quantity q = shade.getRightSideOpeningMultiplier(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,Shade_AirflowPermeability_Quantity) {
  Model model;
  // TODO: Check constructor.
  Shade shade(model);

  Unit units = shade.getAirflowPermeability(true).units(); // Get IP units.

  // Bounds: 0.0 <= value <= 0.8

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(shade.setAirflowPermeability(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(shade.setAirflowPermeability(testQ2));
  Quantity q2 = shade.getAirflowPermeability(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

