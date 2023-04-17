/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
