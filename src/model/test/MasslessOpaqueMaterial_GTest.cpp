/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../MasslessOpaqueMaterial.hpp"
#include "../MasslessOpaqueMaterial_Impl.hpp"
#include "../StandardsInformationMaterial.hpp"
#include "../StandardsInformationMaterial_Impl.hpp"

//#include <utilities/embedded_files.hxx>

using namespace openstudio;
using namespace openstudio::model;

//TEST_F(ModelFixture, Material_EmbeddedFile) {
//ASSERT_TRUE(::openstudiomodel::embedded_files::hasFile(":/Resources/standards/OpenStudio_Standards_materials_merged.json"));
//}

TEST_F(ModelFixture, MasslessOpaqueMaterial_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      MasslessOpaqueMaterial mat(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, MasslessOpaqueMaterial_GettersSetters) {
  Model m;
  MasslessOpaqueMaterial mat(m);

  // Roughness:  String
  // Default in Ctor argument
  EXPECT_EQ("Smooth", mat.roughness());
  // Test a valid choice
  EXPECT_TRUE(mat.setRoughness("VeryRough"));
  EXPECT_EQ("VeryRough", mat.roughness());
  // Test an invalid choice
  EXPECT_FALSE(mat.setRoughness("BadChoice"));
  EXPECT_EQ("VeryRough", mat.roughness());

  // Thermal Resistance:  Double
  // Default in Ctor argument
  EXPECT_EQ(0.1, mat.thermalResistance());
  EXPECT_TRUE(mat.setThermalResistance(1.0));
  EXPECT_EQ(1.0, mat.thermalResistance());

  // Thermal Absorptance:  Double
  // Check Idd default: .9
  EXPECT_EQ(.9, mat.thermalAbsorptance().get());
  EXPECT_TRUE(mat.setThermalAbsorptance(0.45));
  EXPECT_EQ(0.45, mat.thermalAbsorptance().get());
  mat.resetThermalAbsorptance();
  EXPECT_EQ(.9, mat.thermalAbsorptance().get());

  // Solar Absorptance:  Double
  // Check Idd default: .7
  EXPECT_EQ(.7, mat.solarAbsorptance().get());
  EXPECT_TRUE(mat.setSolarAbsorptance(0.35));
  EXPECT_EQ(0.35, mat.solarAbsorptance().get());
  mat.resetSolarAbsorptance();
  EXPECT_EQ(.7, mat.solarAbsorptance().get());

  // Visible Absorptance:  Double
  // Check Idd default: .7
  EXPECT_EQ(.7, mat.visibleAbsorptance().get());
  EXPECT_TRUE(mat.setVisibleAbsorptance(0.36));
  EXPECT_EQ(0.36, mat.visibleAbsorptance().get());
  mat.resetVisibleAbsorptance();
  EXPECT_EQ(.7, mat.visibleAbsorptance().get());
}

TEST_F(ModelFixture, MasslessOpaqueMaterial_StandardsInformation) {
  Model model;

  MasslessOpaqueMaterial masslessOpaqueMaterial(model);
  StandardsInformationMaterial info = masslessOpaqueMaterial.standardsInformation();

  std::vector<std::string> suggestedMatStandards = info.suggestedMaterialStandards();
  EXPECT_LT(0u, suggestedMatStandards.size());  // For now only "CEC Title24-2013"
  EXPECT_LT(0u, info.suggestedCompositeFramingMaterials().size());
  EXPECT_LT(0u, info.suggestedCompositeFramingConfigurations().size());
  EXPECT_LT(0u, info.suggestedCompositeFramingDepths().size());
  EXPECT_LT(0u, info.suggestedCompositeFramingSizes().size());
  EXPECT_LT(0u, info.suggestedCompositeCavityInsulations().size());

  EXPECT_TRUE(std::find(suggestedMatStandards.begin(), suggestedMatStandards.end(), "CEC Title24-2013") != suggestedMatStandards.end());
  info.setMaterialStandard("CEC Title24-2013");
  ASSERT_TRUE(info.materialStandard());
  EXPECT_EQ("CEC Title24-2013", info.materialStandard().get());

  EXPECT_LT(0u, info.suggestedCompositeFramingMaterials().size());
  EXPECT_LT(0u, info.suggestedCompositeFramingConfigurations().size());
  EXPECT_LT(0u, info.suggestedCompositeFramingDepths().size());
  EXPECT_LT(0u, info.suggestedCompositeFramingSizes().size());
  EXPECT_LT(0u, info.suggestedCompositeCavityInsulations().size());

  info.setMaterialStandard("Non-Existent Standard");
  ASSERT_TRUE(info.materialStandard());
  EXPECT_EQ("Non-Existent Standard", info.materialStandard().get());

  EXPECT_EQ(0u, info.suggestedCompositeFramingMaterials().size());
  EXPECT_EQ(0u, info.suggestedCompositeFramingConfigurations().size());
  EXPECT_EQ(0u, info.suggestedCompositeFramingDepths().size());
  EXPECT_EQ(0u, info.suggestedCompositeFramingSizes().size());
  EXPECT_EQ(0u, info.suggestedCompositeCavityInsulations().size());
}
