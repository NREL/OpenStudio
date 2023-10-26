/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../RoofVegetation.hpp"
#include "../StandardsInformationMaterial.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, RoofVegetation_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      RoofVegetation roofVegetation(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, RoofVegetation_GettersSetters) {
  Model m;
  RoofVegetation roofVegetation(m);

  // Height of Plants:  Double
  // Check Idd default: .2
  EXPECT_EQ(.2, roofVegetation.heightofPlants());
  EXPECT_TRUE(roofVegetation.setHeightofPlants(0.1));
  EXPECT_EQ(0.1, roofVegetation.heightofPlants());
  roofVegetation.resetHeightofPlants();
  EXPECT_EQ(.2, roofVegetation.heightofPlants());

  // Leaf Area Index:  Double
  // Check Idd default: 1.0
  EXPECT_EQ(1.0, roofVegetation.leafAreaIndex());
  EXPECT_TRUE(roofVegetation.setLeafAreaIndex(0.5));
  EXPECT_EQ(0.5, roofVegetation.leafAreaIndex());
  roofVegetation.resetLeafAreaIndex();
  EXPECT_EQ(1.0, roofVegetation.leafAreaIndex());

  // Leaf Reflectivity:  Double
  // Check Idd default: 0.22
  EXPECT_EQ(0.22, roofVegetation.leafReflectivity());
  EXPECT_TRUE(roofVegetation.setLeafReflectivity(0.14));
  EXPECT_EQ(0.14, roofVegetation.leafReflectivity());
  roofVegetation.resetLeafReflectivity();
  EXPECT_EQ(0.22, roofVegetation.leafReflectivity());

  // Leaf Emissivity:  Double
  // Check Idd default: 0.95
  EXPECT_EQ(0.95, roofVegetation.leafEmissivity());
  EXPECT_TRUE(roofVegetation.setLeafEmissivity(0.88));
  EXPECT_EQ(0.88, roofVegetation.leafEmissivity());
  roofVegetation.resetLeafEmissivity();
  EXPECT_EQ(0.95, roofVegetation.leafEmissivity());

  // Minimum Stomatal Resistance:  Double
  // Check Idd default: 180.0
  EXPECT_EQ(180.0, roofVegetation.minimumStomatalResistance());
  EXPECT_TRUE(roofVegetation.setMinimumStomatalResistance(115.0));
  EXPECT_EQ(115.0, roofVegetation.minimumStomatalResistance());
  roofVegetation.resetMinimumStomatalResistance();
  EXPECT_EQ(180.0, roofVegetation.minimumStomatalResistance());

  // Soil Layer Name:  String
  // Check Idd default: "Green Roof Soil"
  EXPECT_EQ("Green Roof Soil", roofVegetation.soilLayerName());
  EXPECT_TRUE(roofVegetation.setSoilLayerName("Another name"));
  EXPECT_EQ("Another name", roofVegetation.soilLayerName());

  // Roughness:  String
  // Check Idd default: "MediumRough"
  // Note JM 2018-12-12: Actually Ctor arg (in hpp) overrides to Smooth
  EXPECT_EQ("Smooth", roofVegetation.roughness());
  // Test a valid choice
  EXPECT_TRUE(roofVegetation.setRoughness("VeryRough"));
  EXPECT_EQ("VeryRough", roofVegetation.roughness());
  // Test an invalid choice
  EXPECT_FALSE(roofVegetation.setRoughness("BadChoice"));
  EXPECT_EQ("VeryRough", roofVegetation.roughness());

  // Thickness:  Double
  // Check Idd default: 0.1
  EXPECT_EQ(0.1, roofVegetation.thickness());
  EXPECT_TRUE(roofVegetation.setThickness(0.08));
  EXPECT_EQ(0.08, roofVegetation.thickness());
  roofVegetation.resetThickness();
  EXPECT_EQ(0.1, roofVegetation.thickness());

  // Conductivity of Dry Soil:  Double
  // Check Idd default: 0.35
  EXPECT_EQ(0.35, roofVegetation.conductivityofDrySoil());
  EXPECT_TRUE(roofVegetation.setConductivityofDrySoil(0.28));
  EXPECT_EQ(0.28, roofVegetation.conductivityofDrySoil());
  roofVegetation.resetConductivityofDrySoil();
  EXPECT_EQ(0.35, roofVegetation.conductivityofDrySoil());

  // Density of Dry Soil:  Double
  // Check Idd default: 1100.0
  EXPECT_EQ(1100.0, roofVegetation.densityofDrySoil());
  EXPECT_TRUE(roofVegetation.setDensityofDrySoil(700.0));
  EXPECT_EQ(700.0, roofVegetation.densityofDrySoil());
  roofVegetation.resetDensityofDrySoil();
  EXPECT_EQ(1100.0, roofVegetation.densityofDrySoil());

  // Specific Heat of Dry Soil:  Double
  // Check Idd default: 1200.0
  EXPECT_EQ(1200.0, roofVegetation.specificHeatofDrySoil());
  EXPECT_TRUE(roofVegetation.setSpecificHeatofDrySoil(850.0));
  EXPECT_EQ(850.0, roofVegetation.specificHeatofDrySoil());
  roofVegetation.resetSpecificHeatofDrySoil();
  EXPECT_EQ(1200.0, roofVegetation.specificHeatofDrySoil());

  // Thermal Absorptance:  Double
  // Check Idd default: .9
  EXPECT_EQ(.9, roofVegetation.thermalAbsorptance().get());
  EXPECT_TRUE(roofVegetation.setThermalAbsorptance(0.85));
  EXPECT_EQ(0.85, roofVegetation.thermalAbsorptance().get());
  roofVegetation.resetThermalAbsorptance();
  EXPECT_EQ(.9, roofVegetation.thermalAbsorptance().get());

  // Solar Absorptance:  Double
  // Check Idd default: .70
  EXPECT_EQ(.70, roofVegetation.solarAbsorptance().get());
  EXPECT_TRUE(roofVegetation.setSolarAbsorptance(0.55));
  EXPECT_EQ(0.55, roofVegetation.solarAbsorptance().get());
  roofVegetation.resetSolarAbsorptance();
  EXPECT_EQ(.70, roofVegetation.solarAbsorptance().get());

  // Visible Absorptance:  Double
  // Check Idd default: .75
  EXPECT_EQ(.75, roofVegetation.visibleAbsorptance().get());
  EXPECT_TRUE(roofVegetation.setVisibleAbsorptance(0.63));
  EXPECT_EQ(0.63, roofVegetation.visibleAbsorptance().get());
  roofVegetation.resetVisibleAbsorptance();
  EXPECT_EQ(.75, roofVegetation.visibleAbsorptance().get());

  // Saturation Volumetric Moisture Content of the Soil Layer:  Double
  // Check Idd default: 0.3
  EXPECT_EQ(0.3, roofVegetation.saturationVolumetricMoistureContentoftheSoilLayer());
  EXPECT_TRUE(roofVegetation.setSaturationVolumetricMoistureContentoftheSoilLayer(0.2));
  EXPECT_EQ(0.2, roofVegetation.saturationVolumetricMoistureContentoftheSoilLayer());
  roofVegetation.resetSaturationVolumetricMoistureContentoftheSoilLayer();
  EXPECT_EQ(0.3, roofVegetation.saturationVolumetricMoistureContentoftheSoilLayer());

  // Residual Volumetric Moisture Content of the Soil Layer:  Double
  // Check Idd default: 0.01
  EXPECT_EQ(0.01, roofVegetation.residualVolumetricMoistureContentoftheSoilLayer());
  EXPECT_TRUE(roofVegetation.setResidualVolumetricMoistureContentoftheSoilLayer(0.01));
  EXPECT_EQ(0.01, roofVegetation.residualVolumetricMoistureContentoftheSoilLayer());
  roofVegetation.resetResidualVolumetricMoistureContentoftheSoilLayer();
  EXPECT_EQ(0.01, roofVegetation.residualVolumetricMoistureContentoftheSoilLayer());

  // Initial Volumetric Moisture Content of the Soil Layer:  Double
  // Check Idd default: 0.1
  EXPECT_EQ(0.1, roofVegetation.initialVolumetricMoistureContentoftheSoilLayer());
  EXPECT_TRUE(roofVegetation.setInitialVolumetricMoistureContentoftheSoilLayer(0.08));
  EXPECT_EQ(0.08, roofVegetation.initialVolumetricMoistureContentoftheSoilLayer());
  roofVegetation.resetInitialVolumetricMoistureContentoftheSoilLayer();
  EXPECT_EQ(0.1, roofVegetation.initialVolumetricMoistureContentoftheSoilLayer());

  // Moisture Diffusion Calculation Method:  String
  // Check Idd default: "Advanced"
  EXPECT_EQ("Advanced", roofVegetation.moistureDiffusionCalculationMethod());
  // Test a valid choice
  EXPECT_TRUE(roofVegetation.setMoistureDiffusionCalculationMethod("Simple"));
  EXPECT_EQ("Simple", roofVegetation.moistureDiffusionCalculationMethod());
  // Test an invalid choice
  EXPECT_FALSE(roofVegetation.setMoistureDiffusionCalculationMethod("BadChoice"));
  EXPECT_EQ("Simple", roofVegetation.moistureDiffusionCalculationMethod());
}

TEST_F(ModelFixture, RoofVegetation_StandardsInformation) {
  Model model;

  RoofVegetation roofVegetation(model);
  StandardsInformationMaterial info = roofVegetation.standardsInformation();
}
