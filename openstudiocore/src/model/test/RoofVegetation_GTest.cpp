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

#include "../RoofVegetation.hpp"
#include "../RoofVegetation_Impl.hpp"
#include "../StandardsInformationMaterial.hpp"

#include "../../utilities/units/Quantity.hpp"
#include "../../utilities/units/Unit.hpp"
#include "../../utilities/units/UnitFactory.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture,RoofVegetation_HeightofPlants_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getHeightofPlants(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(roofVegetation.setHeightofPlants(testQ));
  Quantity q = roofVegetation.getHeightofPlants(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_LeafAreaIndex_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getLeafAreaIndex(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(roofVegetation.setLeafAreaIndex(testQ));
  Quantity q = roofVegetation.getLeafAreaIndex(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_LeafReflectivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getLeafReflectivity(true).units(); // Get IP units.

  // Bounds: 0.05 <= value <= 0.5

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(roofVegetation.setLeafReflectivity(testQ));

  double value2(0.05);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(roofVegetation.setLeafReflectivity(testQ2));
  Quantity q2 = roofVegetation.getLeafReflectivity(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_LeafEmissivity_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getLeafEmissivity(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(roofVegetation.setLeafEmissivity(testQ));
  Quantity q = roofVegetation.getLeafEmissivity(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_MinimumStomatalResistance_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getMinimumStomatalResistance(true).units(); // Get IP units.
  // TODO: Check that value is appropriate (within bounds)
  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_TRUE(roofVegetation.setMinimumStomatalResistance(testQ));
  Quantity q = roofVegetation.getMinimumStomatalResistance(true);
  EXPECT_NEAR(value,q.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_Thickness_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getThickness(true).units(); // Get IP units.

  // Bounds: 0.05 < value <= 0.7 meters
  // Bounds: 1.97 < value <= 27.56 inches

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(roofVegetation.setThickness(testQ));

  double value2(2.0);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(roofVegetation.setThickness(testQ2));
  Quantity q2 = roofVegetation.getThickness(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_ConductivityofDrySoil_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit siUnits = roofVegetation.getConductivityofDrySoil(false).units(); // Get SI units.
  Unit ipUnits = roofVegetation.getConductivityofDrySoil(true).units(); // Get IP units.

  // \units W / m - K
  // \ip - units Btu - in / hr - ft2 - R
  // Bounds: 0.2 <= value <= 1.5 W / m - K
  // to convert from SI to IP multiply by 6.9334713

  double value(1.0);
  EXPECT_TRUE(roofVegetation.setConductivityofDrySoil(value));

  Quantity testQ(value, siUnits);
  EXPECT_TRUE(roofVegetation.setConductivityofDrySoil(testQ));

  testQ = Quantity(value, ipUnits);
  EXPECT_FALSE(roofVegetation.setConductivityofDrySoil(testQ));

  double value2(0.2);
  Quantity testQ2(value2, siUnits);
  EXPECT_TRUE(roofVegetation.setConductivityofDrySoil(testQ2));
  Quantity q2 = roofVegetation.getConductivityofDrySoil(true);
  EXPECT_NEAR(value2, q2.value(), 6.9334713*0.2);
  EXPECT_EQ(ipUnits.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_DensityofDrySoil_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getDensityofDrySoil(true).units(); // Get IP units.

  // Bounds: 300 <= value <= 2000 kg/m^3
  // Bounds: 18.73 <= value <= 124.86 lb_m/ft^3

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(roofVegetation.setDensityofDrySoil(testQ));

  double value2(20);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(roofVegetation.setDensityofDrySoil(testQ2));
  Quantity q2 = roofVegetation.getDensityofDrySoil(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_SpecificHeatofDrySoil_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  boost::optional<Unit> osiUnits = createUnit("m^2/s^2*K");
  ASSERT_TRUE(osiUnits);
  Unit siUnits = roofVegetation.getSpecificHeatofDrySoil(false).units(); // Get SI units.
  EXPECT_EQ(osiUnits->standardString(),siUnits.standardString());

  // m^2/s^2*K is equivalent to J/kg*K
  osiUnits = createUnit("J/kg*K");
  ASSERT_TRUE(osiUnits);
  EXPECT_EQ(osiUnits->standardString(),siUnits.standardString());

  boost::optional<Unit> oipUnits = createUnit("Btu/R*lb_m");
  ASSERT_TRUE(oipUnits);
  Unit ipUnits = roofVegetation.getSpecificHeatofDrySoil(true).units(); // Get IP units.
  EXPECT_EQ(oipUnits->standardString(),ipUnits.standardString());

  // Bounds: 500 < value <= 2000 m^2/s^2*K
  // Bounds: 0.1194229483 < value <= 0.4776917933 ft^2/h^2*R

  double value(1.0);
  Quantity testQ(value,ipUnits);
  EXPECT_FALSE(roofVegetation.setSpecificHeatofDrySoil(testQ));

  double value2(0.2);
  Quantity testQ2(value2,ipUnits);
  EXPECT_TRUE(roofVegetation.setSpecificHeatofDrySoil(testQ2));
  Quantity q2 = roofVegetation.getSpecificHeatofDrySoil(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(ipUnits.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_ThermalAbsorptance_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getThermalAbsorptance(true).units(); // Get IP units.

  // Bounds: 0.8 < value <= 1.0

  double value(0.1);
  Quantity testQ(value,units);
  EXPECT_FALSE(roofVegetation.setThermalAbsorptance(testQ));

  double value2(1.0);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(roofVegetation.setThermalAbsorptance(testQ2));
  Quantity q2 = roofVegetation.getThermalAbsorptance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_SolarAbsorptance_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getSolarAbsorptance(true).units(); // Get IP units.

  // Bounds: 0.4 <= value <= 0.9

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(roofVegetation.setSolarAbsorptance(testQ));

  double value2(0.4);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(roofVegetation.setSolarAbsorptance(testQ2));
  Quantity q2 = roofVegetation.getSolarAbsorptance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_VisibleAbsorptance_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getVisibleAbsorptance(true).units(); // Get IP units.

  // Bounds: 0.5 < value <= 1.0

  double value(0.5);
  Quantity testQ(value,units);
  EXPECT_FALSE(roofVegetation.setVisibleAbsorptance(testQ));

  double value2(1.0);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(roofVegetation.setVisibleAbsorptance(testQ2));
  Quantity q2 = roofVegetation.getVisibleAbsorptance(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_SaturationVolumetricMoistureContentoftheSoilLayer_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getSaturationVolumetricMoistureContentoftheSoilLayer(true).units(); // Get IP units.

  // Bounds: 0.1 < value <= 0.5

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(roofVegetation.setSaturationVolumetricMoistureContentoftheSoilLayer(testQ));

  double value2(0.11);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(roofVegetation.setSaturationVolumetricMoistureContentoftheSoilLayer(testQ2));
  Quantity q2 = roofVegetation.getSaturationVolumetricMoistureContentoftheSoilLayer(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_ResidualVolumetricMoistureContentoftheSoilLayer_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getResidualVolumetricMoistureContentoftheSoilLayer(true).units(); // Get IP units.

  // Bounds: 0.01 <= value <= 0.1

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(roofVegetation.setResidualVolumetricMoistureContentoftheSoilLayer(testQ));

  double value2(0.1);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(roofVegetation.setResidualVolumetricMoistureContentoftheSoilLayer(testQ2));
  Quantity q2 = roofVegetation.getResidualVolumetricMoistureContentoftheSoilLayer(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}

TEST_F(ModelFixture,RoofVegetation_InitialVolumetricMoistureContentoftheSoilLayer_Quantity) {
  Model model;
  // TODO: Check constructor.
  RoofVegetation roofVegetation(model);

  Unit units = roofVegetation.getInitialVolumetricMoistureContentoftheSoilLayer(true).units(); // Get IP units.

  // Bounds: 0.05 < value <= 0.5

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(roofVegetation.setInitialVolumetricMoistureContentoftheSoilLayer(testQ));

  double value2(0.5);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(roofVegetation.setInitialVolumetricMoistureContentoftheSoilLayer(testQ2));
  Quantity q2 = roofVegetation.getInitialVolumetricMoistureContentoftheSoilLayer(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
}




TEST_F(ModelFixture, RoofVegetation_StandardsInformation) {
  Model model;

  RoofVegetation roofVegetation(model);
  StandardsInformationMaterial info = roofVegetation.standardsInformation();

}
