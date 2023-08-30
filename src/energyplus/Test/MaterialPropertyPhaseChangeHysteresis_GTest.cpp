/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
