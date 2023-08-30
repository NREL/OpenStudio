/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/MaterialPropertyMoisturePenetrationDepthSettings.hpp"
#include "../../model/MaterialPropertyMoisturePenetrationDepthSettings_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/MaterialProperty_MoisturePenetrationDepth_Settings_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_MaterialPropertyMoisturePenetrationDepthSettings) {

  ForwardTranslator ft;

  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> optempd =
    material.createMaterialPropertyMoisturePenetrationDepthSettings(8.9, 0.0069, 0.9066, 0.0404, 22.1121, 0.005, 140);  // drywall

  Workspace workspace = ft.translateModel(model);

  WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::MaterialProperty_MoisturePenetrationDepth_Settings);
  ASSERT_EQ(1u, idfObjs.size());

  WorkspaceObject idf_matProp(idfObjs[0]);

  EXPECT_EQ("Material 1", idf_matProp.getString(MaterialProperty_MoisturePenetrationDepth_SettingsFields::Name).get());
  EXPECT_EQ(8.9, idf_matProp.getDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::WaterVaporDiffusionResistanceFactor).get());
  EXPECT_EQ(0.0069, idf_matProp.getDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficienta).get());
  EXPECT_EQ(0.9066, idf_matProp.getDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientb).get());
  EXPECT_EQ(0.0404, idf_matProp.getDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientc).get());
  EXPECT_EQ(22.1121, idf_matProp.getDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::MoistureEquationCoefficientd).get());
  EXPECT_EQ("Autocalculate", idf_matProp.getString(MaterialProperty_MoisturePenetrationDepth_SettingsFields::SurfaceLayerPenetrationDepth).get());
  EXPECT_EQ("Autocalculate", idf_matProp.getString(MaterialProperty_MoisturePenetrationDepth_SettingsFields::DeepLayerPenetrationDepth).get());
  EXPECT_EQ(0.005, idf_matProp.getDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::CoatingLayerThickness).get());
  EXPECT_EQ(140,
            idf_matProp.getDouble(MaterialProperty_MoisturePenetrationDepth_SettingsFields::CoatingLayerWaterVaporDiffusionResistanceFactor).get());
}
