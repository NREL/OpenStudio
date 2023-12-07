/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirConditionerVariableRefrigerantFlow.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/AirConditioner_VariableRefrigerantFlow_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlow) {
  Model model;
  AirConditionerVariableRefrigerantFlow vrf(model);

  // TODO
  EXPECT_TRUE(vrf.setMaximumOutdoorTemperatureinHeatRecoveryMode(20));
  EXPECT_TRUE(vrf.setInitialHeatRecoveryCoolingCapacityFraction(0.5));
  // TODO

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  ZoneHVACTerminalUnitVariableRefrigerantFlow term(model);
  EXPECT_TRUE(vrf.addTerminal(term));
  workspace = ft.translateModel(model);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow).size());

  IdfObject idf_vrf = workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow)[0];
  EXPECT_EQ(vrf.nameString(), idf_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::Name, false).get());
  // TODO
  EXPECT_EQ(20, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::MaximumCondenserInletNodeTemperatureinHeatRecoveryMode, false).get());
  EXPECT_EQ(0.5, idf_vrf.getDouble(AirConditioner_VariableRefrigerantFlowFields::InitialHeatRecoveryCoolingCapacityFraction, false).get());
  // TODO
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlow_harcodedCondenserType) {

  // Lambda to dry up code
  auto translateAndCheckOneVRFAndCondenserType = [](const Model& m, const std::string& expectedCondenserType, unsigned expectedNumberOfErrors) {
    ForwardTranslator ft;

    // Translate
    Workspace w = ft.translateModel(m);

    // No Errors
    EXPECT_EQ(expectedNumberOfErrors, ft.errors().size());

    std::vector<WorkspaceObject> objs = w.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow);
    EXPECT_EQ(1u, objs.size());

    WorkspaceObject i_vrf = objs[0];

    EXPECT_EQ(i_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CondenserType).get(), expectedCondenserType);
  };

  std::vector<std::string> noWaterCondenserTypes({"AirCooled", "EvaporativelyCooled"});

  for (const std::string& condenserType : noWaterCondenserTypes) {
    for (bool hasPlantLoop : {false, true}) {

      Model m;
      AirConditionerVariableRefrigerantFlow vrf(m);
      vrf.setCondenserType(condenserType);

      // need at least one terminal
      auto vrfTerminal = ZoneHVACTerminalUnitVariableRefrigerantFlow(m, false);
      EXPECT_TRUE(vrf.addTerminal(vrfTerminal));

      if (hasPlantLoop) {
        PlantLoop p(m);
        EXPECT_TRUE(p.addDemandBranchForComponent(vrf));

        // One Error, still harcoded
        translateAndCheckOneVRFAndCondenserType(m, condenserType, 1);

      } else {
        // No Error
        translateAndCheckOneVRFAndCondenserType(m, condenserType, 0);
      }
    }
  }

  // WaterCooled
  std::string condenserType("WaterCooled");

  for (bool hasPlantLoop : {false, true}) {

    Model m;
    AirConditionerVariableRefrigerantFlow vrf(m);
    vrf.setCondenserType(condenserType);

    auto vrfTerminal = ZoneHVACTerminalUnitVariableRefrigerantFlow(m, false);
    EXPECT_TRUE(vrf.addTerminal(vrfTerminal));

    if (hasPlantLoop) {
      PlantLoop p(m);
      EXPECT_TRUE(p.addDemandBranchForComponent(vrf));

      // No Error
      translateAndCheckOneVRFAndCondenserType(m, condenserType, 0);

    } else {
      // One Error, still harcoded
      translateAndCheckOneVRFAndCondenserType(m, condenserType, 1);
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlow_defaultedCondenserType) {

  ForwardTranslator ft;

  // No Plant Loop => AirCooled
  {
    Model m;
    AirConditionerVariableRefrigerantFlow vrf(m);
    vrf.resetCondenserType();

    EXPECT_TRUE(vrf.isCondenserTypeDefaulted());
    EXPECT_EQ("AirCooled", vrf.condenserType());

    auto vrfTerminal = ZoneHVACTerminalUnitVariableRefrigerantFlow(m, false);
    EXPECT_TRUE(vrf.addTerminal(vrfTerminal));

    // Translate
    Workspace w = ft.translateModel(m);

    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> objs = w.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow);
    EXPECT_EQ(1u, objs.size());

    WorkspaceObject i_vrf = objs[0];

    EXPECT_EQ(i_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CondenserType).get(), "AirCooled");
  }

  // Plant Loop => WaterCooled
  {
    Model m;
    AirConditionerVariableRefrigerantFlow vrf(m);
    vrf.resetCondenserType();

    auto vrfTerminal = ZoneHVACTerminalUnitVariableRefrigerantFlow(m, false);
    EXPECT_TRUE(vrf.addTerminal(vrfTerminal));

    PlantLoop p(m);
    EXPECT_TRUE(p.addDemandBranchForComponent(vrf));

    EXPECT_TRUE(vrf.isCondenserTypeDefaulted());
    EXPECT_EQ("WaterCooled", vrf.condenserType());

    // Translate
    Workspace w = ft.translateModel(m);

    // No Errors
    EXPECT_EQ(0u, ft.errors().size());

    std::vector<WorkspaceObject> objs = w.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow);
    EXPECT_EQ(1u, objs.size());

    WorkspaceObject i_vrf = objs[0];

    EXPECT_EQ(i_vrf.getString(AirConditioner_VariableRefrigerantFlowFields::CondenserType).get(), "WaterCooled");
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirConditionerVariableRefrigerantFlow_NoTerminals) {
  Model model;
  AirConditionerVariableRefrigerantFlow vrf(model);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(model);

  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow).size());
  EXPECT_EQ(9u, workspace.getObjectsByType(IddObjectType::Curve_Biquadratic).size());
  EXPECT_EQ(11u, workspace.getObjectsByType(IddObjectType::Curve_Cubic).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_VariableRefrigerantFlow).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_VariableRefrigerantFlow).size());
  EXPECT_EQ(0u, workspace.getObjectsByType(IddObjectType::Fan_VariableVolume).size());

  ASSERT_EQ(1, ft.warnings().size());
  EXPECT_NE(std::string::npos, ft.warnings().front().logMessage().find("will not be translated as it has no terminals"));
  EXPECT_EQ(0, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow).size());

  ZoneHVACTerminalUnitVariableRefrigerantFlow term(model);
  EXPECT_TRUE(vrf.addTerminal(term));
  workspace = ft.translateModel(model);

  EXPECT_EQ(0, ft.errors().size());
  EXPECT_EQ(1, workspace.getObjectsByType(IddObjectType::AirConditioner_VariableRefrigerantFlow).size());
}
