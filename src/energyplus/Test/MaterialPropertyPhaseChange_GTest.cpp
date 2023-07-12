/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/MaterialPropertyPhaseChange.hpp"
#include "../../model/MaterialPropertyPhaseChange_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/MaterialProperty_PhaseChange_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <vector>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_MaterialPropertyPhaseChange) {

  ForwardTranslator ft;

  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyPhaseChange> optphaseChange = material.createMaterialPropertyPhaseChange();
  auto phaseChange = optphaseChange.get();

  EXPECT_TRUE(phaseChange.setTemperatureCoefficientforThermalConductivity(14.7));

  phaseChange.removeAllTemperatureEnthalpys();
  std::vector<TemperatureEnthalpy> temperatureEnthalpies{
    {-20, 0.4},
    {24.1, 19260},
    {24.1, 31000},
    {58, 69000},
  };

  EXPECT_TRUE(phaseChange.addTemperatureEnthalpys(temperatureEnthalpies));

  Workspace workspace = ft.translateModel(model);

  WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::MaterialProperty_PhaseChange);
  ASSERT_EQ(1u, idfObjs.size());

  WorkspaceObject idf_matProp(idfObjs[0]);

  EXPECT_EQ("Material 1", idf_matProp.getString(MaterialProperty_PhaseChangeFields::Name).get());
  EXPECT_EQ(phaseChange.temperatureCoefficientforThermalConductivity(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeFields::TemperatureCoefficientforThermalConductivity).get());
  ASSERT_EQ(temperatureEnthalpies.size(), idf_matProp.numExtensibleGroups());
  auto egs = idf_matProp.extensibleGroups();
  for (size_t i = 0; const auto& temperatureEnthalpy : temperatureEnthalpies) {
    auto eg = egs[i];
    EXPECT_EQ(temperatureEnthalpy.temperature(), eg.getDouble(MaterialProperty_PhaseChangeExtensibleFields::Temperature).get());
    EXPECT_EQ(temperatureEnthalpy.enthalpy(), eg.getDouble(MaterialProperty_PhaseChangeExtensibleFields::Enthalpy).get());
    ++i;
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_MaterialPropertyPhaseChange_NotEnoughTemperatureEnthalpyPairs) {

  ForwardTranslator ft;

  Model model;
  StandardOpaqueMaterial material(model);
  std::vector<TemperatureEnthalpy> temperatureEnthalpys;
  TemperatureEnthalpy temperatureEnthalpy1(-20, 0.1);
  temperatureEnthalpys.push_back(temperatureEnthalpy1);
  TemperatureEnthalpy temperatureEnthalpy2(22, 18260);
  temperatureEnthalpys.push_back(temperatureEnthalpy2);
  boost::optional<MaterialPropertyPhaseChange> optphaseChange = material.createMaterialPropertyPhaseChange(temperatureEnthalpys);

  Workspace workspace = ft.translateModel(model);

  WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::MaterialProperty_PhaseChange);
  ASSERT_EQ(0u, idfObjs.size());
  ASSERT_EQ(1, ft.warnings().size());
  EXPECT_EQ("Object of type 'OS:MaterialProperty:PhaseChange' cannot be translated as it has less than 4 temperature-enthalpy pairs.",
            ft.warnings().front().logMessage());
}
