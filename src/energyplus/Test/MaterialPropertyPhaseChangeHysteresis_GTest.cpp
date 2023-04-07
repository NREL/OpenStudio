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
#include "../../model/MaterialPropertyPhaseChangeHysteresis.hpp"
#include "../../model/MaterialPropertyPhaseChangeHysteresis_Impl.hpp"
#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/MaterialProperty_PhaseChangeHysteresis_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_MaterialPropertyPhaseChangeHysteresis) {

  ForwardTranslator ft;

  Model model;
  StandardOpaqueMaterial material(model);
  boost::optional<MaterialPropertyPhaseChangeHysteresis> optphaseChangeHysteresis = material.createMaterialPropertyPhaseChangeHysteresis();
  auto phaseChangeHysteresis = optphaseChangeHysteresis.get();

  phaseChangeHysteresis.setLatentHeatduringtheEntirePhaseChangeProcess(11000);
  phaseChangeHysteresis.setLiquidStateThermalConductivity(2.5);
  phaseChangeHysteresis.setLiquidStateDensity(2300.0);
  phaseChangeHysteresis.setLiquidStateSpecificHeat(2100.0);
  phaseChangeHysteresis.setHighTemperatureDifferenceofMeltingCurve(1.5);
  phaseChangeHysteresis.setPeakMeltingTemperature(24.0);
  phaseChangeHysteresis.setLowTemperatureDifferenceofMeltingCurve(1.1);
  phaseChangeHysteresis.setSolidStateThermalConductivity(1.9);
  phaseChangeHysteresis.setSolidStateDensity(2400.0);
  phaseChangeHysteresis.setSolidStateSpecificHeat(2010.0);
  phaseChangeHysteresis.setHighTemperatureDifferenceofFreezingCurve(1.2);
  phaseChangeHysteresis.setPeakFreezingTemperature(21.0);
  phaseChangeHysteresis.setLowTemperatureDifferenceofFreezingCurve(1.05);

  Workspace workspace = ft.translateModel(model);

  WorkspaceObjectVector idfObjs = workspace.getObjectsByType(IddObjectType::MaterialProperty_PhaseChangeHysteresis);
  ASSERT_EQ(1u, idfObjs.size());

  WorkspaceObject idf_matProp(idfObjs[0]);

  EXPECT_EQ(material.nameString(), idf_matProp.getString(MaterialProperty_PhaseChangeHysteresisFields::Name).get());
  EXPECT_EQ(phaseChangeHysteresis.latentHeatduringtheEntirePhaseChangeProcess(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::LatentHeatduringtheEntirePhaseChangeProcess).get());
  EXPECT_EQ(phaseChangeHysteresis.liquidStateThermalConductivity(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::LiquidStateThermalConductivity).get());
  EXPECT_EQ(phaseChangeHysteresis.liquidStateDensity(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::LiquidStateDensity).get());
  EXPECT_EQ(phaseChangeHysteresis.liquidStateSpecificHeat(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::LiquidStateSpecificHeat).get());
  EXPECT_EQ(phaseChangeHysteresis.highTemperatureDifferenceofMeltingCurve(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::HighTemperatureDifferenceofMeltingCurve).get());
  EXPECT_EQ(phaseChangeHysteresis.peakMeltingTemperature(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::PeakMeltingTemperature).get());
  EXPECT_EQ(phaseChangeHysteresis.lowTemperatureDifferenceofMeltingCurve(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::LowTemperatureDifferenceofMeltingCurve).get());
  EXPECT_EQ(phaseChangeHysteresis.solidStateThermalConductivity(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::SolidStateThermalConductivity).get());
  EXPECT_EQ(phaseChangeHysteresis.solidStateDensity(), idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::SolidStateDensity).get());
  EXPECT_EQ(phaseChangeHysteresis.solidStateSpecificHeat(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::SolidStateSpecificHeat).get());
  EXPECT_EQ(phaseChangeHysteresis.highTemperatureDifferenceofFreezingCurve(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::HighTemperatureDifferenceofFreezingCurve).get());
  EXPECT_EQ(phaseChangeHysteresis.peakFreezingTemperature(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::PeakFreezingTemperature).get());
  EXPECT_EQ(phaseChangeHysteresis.lowTemperatureDifferenceofFreezingCurve(),
            idf_matProp.getDouble(MaterialProperty_PhaseChangeHysteresisFields::LowTemperatureDifferenceofFreezingCurve).get());
}
