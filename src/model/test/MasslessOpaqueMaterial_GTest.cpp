/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
  EXPECT_EQ(.9, mat.thermalAbsorptance());
  EXPECT_TRUE(mat.setThermalAbsorptance(0.45));
  EXPECT_EQ(0.45, mat.thermalAbsorptance());
  mat.resetThermalAbsorptance();
  EXPECT_EQ(.9, mat.thermalAbsorptance());

  // Solar Absorptance:  Double
  // Check Idd default: .7
  EXPECT_EQ(.7, mat.solarAbsorptance());
  EXPECT_TRUE(mat.setSolarAbsorptance(0.35));
  EXPECT_EQ(0.35, mat.solarAbsorptance());
  mat.resetSolarAbsorptance();
  EXPECT_EQ(.7, mat.solarAbsorptance());

  // Visible Absorptance:  Double
  // Check Idd default: .7
  EXPECT_EQ(.7, mat.visibleAbsorptance());
  EXPECT_TRUE(mat.setVisibleAbsorptance(0.36));
  EXPECT_EQ(0.36, mat.visibleAbsorptance());
  mat.resetVisibleAbsorptance();
  EXPECT_EQ(.7, mat.visibleAbsorptance());
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
