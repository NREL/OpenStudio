/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../Model.hpp"
#include "../Model_Impl.hpp"

#include "../MaterialPropertyMoisturePenetrationDepthSettings.hpp"
#include "../MaterialPropertyMoisturePenetrationDepthSettings_Impl.hpp"

#include "../StandardOpaqueMaterial.hpp"
#include "../StandardOpaqueMaterial_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, MaterialPropertyMoisturePenetrationDepthSettings_MaterialPropertyMoisturePenetrationDepthSettings) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      // create a model to use
      Model model;

      // create a material object to use
      StandardOpaqueMaterial material(model);
      
      exit(0);
    },
    ::testing::ExitedWithCode(0),
    ""
  );
    
  // create a model to use
  Model model;
  
  // create a material object to use
  StandardOpaqueMaterial material(model);
  EXPECT_EQ(1, model.modelObjects().size());
  
  // new material does not have material property moisture penetration depth settings yet
  EXPECT_TRUE(!material.materialPropertyMoisturePenetrationDepthSettings());

  // create a material property moisture penetration depth settings object to use
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd = material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140); // drywall
  ASSERT_TRUE(optempd);
  EXPECT_EQ(2, model.modelObjects().size());

  // now the material has material property moisture penetration depth settings
  EXPECT_TRUE(material.materialPropertyMoisturePenetrationDepthSettings());
  
  // check to make sure the surface layer penetration depth and deep layer penetration depth fields are defaulted as expected
  auto empd = optempd.get();
  EXPECT_TRUE(empd.isSurfaceLayerPenetrationDepthAutocalculated());
  EXPECT_TRUE(empd.isDeepLayerPenetrationDepthAutocalculated());

  // check that creating the material property moisture penetration depth settings when they already exists does nothing and returns nil
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd2 = material.createMaterialPropertyMoisturePenetrationDepthSettings(6.6, 0.019, 1, 0, 1, 0, 0); // carpet
  ASSERT_FALSE(optempd2);
  EXPECT_EQ(2, model.modelObjects().size());
}

// test setting and getting
TEST_F(ModelFixture, MaterialPropertyMoisturePenetrationDepthSettings_SetGetFields) {
  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd = material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
  auto empd = optempd.get();

  // check that the drywall properties were set properly
  EXPECT_EQ(8.9, empd.waterVaporDiffusionResistanceFactor());
  EXPECT_EQ(0.0069, empd.moistureEquationCoefficientA());
  EXPECT_EQ(0.9066, empd.moistureEquationCoefficientB());
  EXPECT_EQ(0.0404, empd.moistureEquationCoefficientC());
  EXPECT_EQ(22.1121, empd.moistureEquationCoefficientD());
  EXPECT_EQ(0.005, empd.coatingLayerThickness());
  EXPECT_EQ(140, empd.coatingLayerWaterVaporDiffusionResistanceFactor());

  // now override the defaults with explicit values
  empd.setWaterVaporDiffusionResistanceFactor(6.6);
  empd.setMoistureEquationCoefficientA(0.019);
  empd.setMoistureEquationCoefficientB(1);
  empd.setMoistureEquationCoefficientC(0);
  empd.setMoistureEquationCoefficientD(1);
  empd.setSurfaceLayerPenetrationDepth(0.1);
  empd.setDeepLayerPenetrationDepth(0);
  empd.setCoatingLayerThickness(0);
  empd.setCoatingLayerWaterVaporDiffusionResistanceFactor(0);

  EXPECT_EQ(6.6, empd.waterVaporDiffusionResistanceFactor());
  EXPECT_EQ(0.019, empd.moistureEquationCoefficientA());
  EXPECT_EQ(1, empd.moistureEquationCoefficientB());
  EXPECT_EQ(0, empd.moistureEquationCoefficientC());
  EXPECT_EQ(1, empd.moistureEquationCoefficientD());
  boost::optional<double> optsurfacelayerpenetrationdepth = empd.surfaceLayerPenetrationDepth();
  boost::optional<double> optdeeplayerpenetrationdepth = empd.deepLayerPenetrationDepth();
  EXPECT_TRUE(optsurfacelayerpenetrationdepth);
  EXPECT_TRUE(optdeeplayerpenetrationdepth);
  auto surfacelayerpenetrationdepth = optsurfacelayerpenetrationdepth.get();
  auto deeplayerpenetrationdepth = optdeeplayerpenetrationdepth.get();
  EXPECT_EQ(0.1, surfacelayerpenetrationdepth);
  EXPECT_EQ(0, deeplayerpenetrationdepth);  
  EXPECT_EQ(0, empd.coatingLayerThickness());
  EXPECT_EQ(0, empd.coatingLayerWaterVaporDiffusionResistanceFactor());

  // test setting back to defaults
  empd.autocalculateSurfaceLayerPenetrationDepth();
  empd.autocalculateDeepLayerPenetrationDepth();
  boost::optional<double> optsurfacelayerpenetrationdepth2 = empd.surfaceLayerPenetrationDepth();
  boost::optional<double> optdeeplayerpenetrationdepth2 = empd.deepLayerPenetrationDepth();
  EXPECT_FALSE(optsurfacelayerpenetrationdepth2);
  EXPECT_FALSE(optdeeplayerpenetrationdepth2);
  EXPECT_TRUE(empd.isSurfaceLayerPenetrationDepthAutocalculated());
  EXPECT_TRUE(empd.isDeepLayerPenetrationDepthAutocalculated());

  // check that all coefficients method works
  empd.setMoistureEquationCoefficients(0.0069, 0.9066, 0.0404, 22.1121);
  EXPECT_EQ(0.0069, empd.moistureEquationCoefficientA());
  EXPECT_EQ(0.9066, empd.moistureEquationCoefficientB());
  EXPECT_EQ(0.0404, empd.moistureEquationCoefficientC());
  EXPECT_EQ(22.1121, empd.moistureEquationCoefficientD());
  auto coefs = empd.moistureEquationCoefficients();
  EXPECT_EQ(0.0069, coefs[0]);
  EXPECT_EQ(0.9066, coefs[1]);
  EXPECT_EQ(0.0404, coefs[2]);
  EXPECT_EQ(22.1121, coefs[3]);
}

// check that parent reset works
TEST_F(ModelFixture, MaterialPropertyMoisturePenetrationDepthSettings_ParentReset) {
  Model model;
  StandardOpaqueMaterial material(model);
  auto size = model.modelObjects().size();
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd = material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
  material.resetMaterialPropertyMoisturePenetrationDepthSettings();
  EXPECT_FALSE(material.materialPropertyMoisturePenetrationDepthSettings());
  EXPECT_EQ(size, model.modelObjects().size());
}

// check that parent remove works
TEST_F(ModelFixture, MaterialPropertyMoisturePenetrationDepthSettings_ParentRemove) {
  Model model;
  auto size = model.modelObjects().size();
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd = material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
  EXPECT_FALSE(material.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}

// check that child remove works
TEST_F(ModelFixture, MaterialPropertyMoisturePenetrationDepthSettings_ChildRemove) {
  Model model;
  StandardOpaqueMaterial material(model);
  auto size = model.modelObjects().size();
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd = material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
  auto empd = optempd.get();
  EXPECT_EQ(2, model.modelObjects().size());
  EXPECT_FALSE(empd.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}

// check that it points to the material
TEST_F(ModelFixture, MaterialPropertyMoisturePenetrationDepthSettings_MaterialName) {
  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd = material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
  auto empd = optempd.get();
  EXPECT_EQ("Material 1", empd.materialName());
  StandardOpaqueMaterial material2(model);
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd2 = material2.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
  auto empd2 = optempd2.get();
  EXPECT_EQ("Material 2", empd2.materialName());
}