/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
  // create a model to use
  Model model;

  // create a material object to use
  StandardOpaqueMaterial material(model);
  EXPECT_EQ(1, model.modelObjects().size());

  // new material does not have material property moisture penetration depth settings yet
  EXPECT_FALSE(material.materialPropertyMoisturePenetrationDepthSettings());

  // create a material property moisture penetration depth settings object to use
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd =
    material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);  // drywall
  ASSERT_TRUE(optempd);
  EXPECT_EQ(2, model.modelObjects().size());

  // now the material has material property moisture penetration depth settings
  EXPECT_TRUE(material.materialPropertyMoisturePenetrationDepthSettings());

  // check to make sure the surface layer penetration depth and deep layer penetration depth fields are defaulted as expected
  auto empd = optempd.get();
  EXPECT_TRUE(empd.isSurfaceLayerPenetrationDepthAutocalculated());
  EXPECT_TRUE(empd.isDeepLayerPenetrationDepthAutocalculated());

  // check that creating the material property moisture penetration depth settings when they already exists does nothing and returns nil
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd2 =
    material.createMaterialPropertyMoisturePenetrationDepthSettings(6.6, 0.019, 1, 0, 1, 0, 0);  // carpet
  ASSERT_FALSE(optempd2);
  EXPECT_EQ(2, model.modelObjects().size());
}

// test setting and getting
TEST_F(ModelFixture, MaterialPropertyMoisturePenetrationDepthSettings_SetGetFields) {
  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd =
    material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
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
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd =
    material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
  material.resetMaterialPropertyMoisturePenetrationDepthSettings();
  EXPECT_FALSE(material.materialPropertyMoisturePenetrationDepthSettings());
  EXPECT_EQ(size, model.modelObjects().size());
}

// check that parent remove works
TEST_F(ModelFixture, MaterialPropertyMoisturePenetrationDepthSettings_ParentRemove) {
  Model model;
  auto size = model.modelObjects().size();
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd =
    material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
  EXPECT_FALSE(material.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}

// check that child remove works
TEST_F(ModelFixture, MaterialPropertyMoisturePenetrationDepthSettings_ChildRemove) {
  Model model;
  StandardOpaqueMaterial material(model);
  auto size = model.modelObjects().size();
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd =
    material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
  auto empd = optempd.get();
  EXPECT_EQ(2, model.modelObjects().size());
  EXPECT_FALSE(empd.remove().empty());
  EXPECT_EQ(size, model.modelObjects().size());
}

// check that it points to the material
TEST_F(ModelFixture, MaterialPropertyMoisturePenetrationDepthSettings_MaterialName) {
  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd =
    material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
  auto empd = optempd.get();
  EXPECT_EQ("Material 1", empd.materialName());
  EXPECT_EQ(material.handle(), empd.material().handle());
  StandardOpaqueMaterial material2(model);
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd2 =
    material2.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);
  auto empd2 = optempd2.get();
  EXPECT_EQ("Material 2", empd2.materialName());
  EXPECT_EQ(material2.handle(), empd2.material().handle());
}

TEST_F(ModelFixture, MaterialPropertyMoisturePenetrationDepthSettings_CtorThrow) {
  Model model;
  StandardOpaqueMaterial material(model);

  EXPECT_NO_THROW(MaterialPropertyMoisturePenetrationDepthSettings empd(material, 8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140));
  EXPECT_TRUE(material.materialPropertyMoisturePenetrationDepthSettings());
  EXPECT_THROW(MaterialPropertyMoisturePenetrationDepthSettings empd(material, 8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140),
               openstudio::Exception);
  EXPECT_TRUE(material.materialPropertyMoisturePenetrationDepthSettings());
}
