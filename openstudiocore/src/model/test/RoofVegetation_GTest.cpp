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

#include "../RoofVegetation.hpp"
#include "../RoofVegetation_Impl.hpp"

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

  Unit units = roofVegetation.getConductivityofDrySoil(true).units(); // Get IP units.

  // Bounds: 0.2 <= value <= 1.5

  double value(1.0);
  Quantity testQ(value,units);
  EXPECT_FALSE(roofVegetation.setConductivityofDrySoil(testQ));

  double value2(0.2);
  Quantity testQ2(value2,units);
  EXPECT_TRUE(roofVegetation.setConductivityofDrySoil(testQ2));
  Quantity q2 = roofVegetation.getConductivityofDrySoil(true);
  EXPECT_NEAR(value2,q2.value(),1.0E-8);
  EXPECT_EQ(units.standardString(),q2.units().standardString());
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

