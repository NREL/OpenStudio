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
