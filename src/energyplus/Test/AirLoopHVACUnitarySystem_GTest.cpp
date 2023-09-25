/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVACUnitarySystem.hpp"
#include "../../model/Node.hpp"

#include "../../model/CoilCoolingDXSingleSpeed.hpp"
#include "../../model/CoilHeatingDXSingleSpeed.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/CoilHeatingDesuperheater.hpp"
#include "../../model/CoilHeatingGasMultiStage.hpp"
#include "../../model/CoilHeatingElectricMultiStage.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit.hpp"
#include "../../model/CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData.hpp"
#include "../../model/FanSystemModel.hpp"

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_MultiStage_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_MultiStage_FieldEnums.hxx>
#include <utilities/idd/UnitarySystemPerformance_Multispeed_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

/**
 * Tests only the controlType of the ForwardTranslator which I exposed after the fact
 **/
TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACUnitarySystem_ControlType) {
  Model m;

  AirLoopHVAC airLoop(m);
  AirLoopHVACUnitarySystem unitary(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  // test if Setpoint
  unitary.setControlType("Setpoint");

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC).size());

  IdfObject idf_unitary = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem)[0];

  ASSERT_EQ("Setpoint", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::ControlType).get());

  // test if load (make sure nothing is hardcoded)
  unitary.setControlType("Load");

  workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC).size());

  idf_unitary = workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem)[0];

  ASSERT_EQ("Load", idf_unitary.getString(AirLoopHVAC_UnitarySystemFields::ControlType).get());
}

std::vector<std::string> getUnitaryNodes(const Workspace& workspace) {
  WorkspaceObjectVector idfUnitaries(workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem));
  if (idfUnitaries.empty()) {
    return {};
  }

  auto& idfUnitary = idfUnitaries[0];

  return {
    idfUnitary.getString(AirLoopHVAC_UnitarySystemFields::AirInletNodeName).get(),
    idfUnitary.getString(AirLoopHVAC_UnitarySystemFields::AirOutletNodeName).get(),
  };
}

std::vector<std::string> getCoolingCoilNodes(const Workspace& workspace) {
  WorkspaceObjectVector idfCoolingCoils(workspace.getObjectsByType(IddObjectType::Coil_Cooling_DX_SingleSpeed));
  if (idfCoolingCoils.empty()) {
    return {};
  }

  auto& idfCoolingCoil = idfCoolingCoils[0];

  return {
    idfCoolingCoil.getString(Coil_Cooling_DX_SingleSpeedFields::AirInletNodeName).get(),
    idfCoolingCoil.getString(Coil_Cooling_DX_SingleSpeedFields::AirOutletNodeName).get(),
  };
}

std::vector<std::string> getHeatingCoilNodes(const Workspace& workspace) {
  WorkspaceObjectVector idfHeatingCoils(workspace.getObjectsByType(IddObjectType::Coil_Heating_DX_SingleSpeed));
  if (idfHeatingCoils.empty()) {
    return {};
  }

  auto& idfHeatingCoil = idfHeatingCoils[0];

  return {
    idfHeatingCoil.getString(Coil_Heating_DX_SingleSpeedFields::AirInletNodeName).get(),
    idfHeatingCoil.getString(Coil_Heating_DX_SingleSpeedFields::AirOutletNodeName).get(),
  };
}

std::vector<std::string> getFanNodes(const Workspace& workspace) {
  WorkspaceObjectVector idfFans(workspace.getObjectsByType(IddObjectType::Fan_ConstantVolume));
  if (idfFans.empty()) {
    return {};
  }

  auto& idfFan = idfFans[0];

  return {
    idfFan.getString(Fan_ConstantVolumeFields::AirInletNodeName).get(),
    idfFan.getString(Fan_ConstantVolumeFields::AirOutletNodeName).get(),
  };
}

std::vector<std::string> getSuppHeatingCoilNodes(const Workspace& workspace) {
  WorkspaceObjectVector idfSuppHeatingCoils(workspace.getObjectsByType(IddObjectType::Coil_Heating_Desuperheater));
  if (idfSuppHeatingCoils.empty()) {
    return {};
  }

  auto& idfSuppHeatingCoil = idfSuppHeatingCoils[0];

  return {
    idfSuppHeatingCoil.getString(Coil_Heating_DesuperheaterFields::AirInletNodeName).get(),
    idfSuppHeatingCoil.getString(Coil_Heating_DesuperheaterFields::AirOutletNodeName).get(),
  };
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACUnitarySystem_Nodes) {
  for (std::string fanPlacement : {"BlowThrough", "DrawThrough"}) {

    // cooling coil
    {
      Model m;

      CoilCoolingDXSingleSpeed c(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setCoolingCoil(c);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> coolingCoilNodes = getCoolingCoilNodes(workspace);

      EXPECT_EQ(unitaryNodes[0], coolingCoilNodes[0]);
      EXPECT_EQ(coolingCoilNodes[1], unitaryNodes[1]);
    }

    // heating coil
    {
      Model m;

      CoilHeatingDXSingleSpeed h(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setHeatingCoil(h);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> heatingCoilNodes = getHeatingCoilNodes(workspace);

      EXPECT_EQ(unitaryNodes[0], heatingCoilNodes[0]);
      EXPECT_EQ(heatingCoilNodes[1], unitaryNodes[1]);
    }

    // fan
    {
      Model m;

      FanConstantVolume f(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setSupplyFan(f);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> fanNodes = getFanNodes(workspace);

      EXPECT_EQ(unitaryNodes[0], fanNodes[0]);
      EXPECT_EQ(fanNodes[1], unitaryNodes[1]);
    }

    // supp heating coil
    {
      Model m;

      CoilHeatingDesuperheater s(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setSupplementalHeatingCoil(s);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> suppHeatingCoilNodes = getSuppHeatingCoilNodes(workspace);

      EXPECT_EQ(unitaryNodes[0], suppHeatingCoilNodes[0]);
      EXPECT_EQ(suppHeatingCoilNodes[1], unitaryNodes[1]);
    }

    // cooling coil, heating coil
    {
      Model m;

      CoilCoolingDXSingleSpeed c(m);
      CoilHeatingDXSingleSpeed h(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setCoolingCoil(c);
      unitary.setHeatingCoil(h);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> coolingCoilNodes = getCoolingCoilNodes(workspace);
      std::vector<std::string> heatingCoilNodes = getHeatingCoilNodes(workspace);

      EXPECT_EQ(unitaryNodes[0], coolingCoilNodes[0]);
      EXPECT_EQ(coolingCoilNodes[1], heatingCoilNodes[0]);
      EXPECT_EQ(heatingCoilNodes[1], unitaryNodes[1]);
    }

    // cooling coil, fan
    {
      Model m;

      CoilCoolingDXSingleSpeed c(m);
      FanConstantVolume f(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setCoolingCoil(c);
      unitary.setSupplyFan(f);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> coolingCoilNodes = getCoolingCoilNodes(workspace);
      std::vector<std::string> fanNodes = getFanNodes(workspace);

      if (fanPlacement == "BlowThrough") {
        EXPECT_EQ(unitaryNodes[0], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], coolingCoilNodes[0]);
        EXPECT_EQ(coolingCoilNodes[1], unitaryNodes[1]);
      } else if (fanPlacement == "DrawThrough") {
        EXPECT_EQ(unitaryNodes[0], coolingCoilNodes[0]);
        EXPECT_EQ(coolingCoilNodes[1], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], unitaryNodes[1]);
      }
    }

    // cooling coil, supp heating coil
    // Test for #4509 - Bad nodes created for AirLoopHVAC:UnitarySystem with only cooling coil and supplemental heating coil
    {
      Model m;

      CoilCoolingDXSingleSpeed c(m);
      CoilHeatingDesuperheater s(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setCoolingCoil(c);
      unitary.setSupplementalHeatingCoil(s);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> coolingCoilNodes = getCoolingCoilNodes(workspace);
      std::vector<std::string> suppHeatingCoilNodes = getSuppHeatingCoilNodes(workspace);

      EXPECT_EQ(unitaryNodes[0], coolingCoilNodes[0]);
      EXPECT_EQ(coolingCoilNodes[1], suppHeatingCoilNodes[0]);
      EXPECT_EQ(suppHeatingCoilNodes[1], unitaryNodes[1]);
    }

    // heating coil, fan
    {
      Model m;

      CoilHeatingDXSingleSpeed h(m);
      FanConstantVolume f(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setHeatingCoil(h);
      unitary.setSupplyFan(f);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> heatingCoilNodes = getHeatingCoilNodes(workspace);
      std::vector<std::string> fanNodes = getFanNodes(workspace);

      if (fanPlacement == "BlowThrough") {
        EXPECT_EQ(unitaryNodes[0], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], heatingCoilNodes[0]);
        EXPECT_EQ(heatingCoilNodes[1], unitaryNodes[1]);
      } else if (fanPlacement == "DrawThrough") {
        EXPECT_EQ(unitaryNodes[0], heatingCoilNodes[0]);
        EXPECT_EQ(heatingCoilNodes[1], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], unitaryNodes[1]);
      }
    }

    // heating coil, supp heating coil
    {
      Model m;

      CoilHeatingDXSingleSpeed h(m);
      CoilHeatingDesuperheater s(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setHeatingCoil(h);
      unitary.setSupplementalHeatingCoil(s);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> heatingCoilNodes = getHeatingCoilNodes(workspace);
      std::vector<std::string> suppHeatingCoilNodes = getSuppHeatingCoilNodes(workspace);

      EXPECT_EQ(unitaryNodes[0], heatingCoilNodes[0]);
      EXPECT_EQ(heatingCoilNodes[1], suppHeatingCoilNodes[0]);
      EXPECT_EQ(suppHeatingCoilNodes[1], unitaryNodes[1]);
    }

    // fan, supp heating coil
    {
      Model m;

      FanConstantVolume f(m);
      CoilHeatingDesuperheater s(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setSupplyFan(f);
      unitary.setSupplementalHeatingCoil(s);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> fanNodes = getFanNodes(workspace);
      std::vector<std::string> suppHeatingCoilNodes = getSuppHeatingCoilNodes(workspace);

      EXPECT_EQ(unitaryNodes[0], fanNodes[0]);
      EXPECT_EQ(fanNodes[1], suppHeatingCoilNodes[0]);
      EXPECT_EQ(suppHeatingCoilNodes[1], unitaryNodes[1]);
    }

    // cooling coil, heating coil, fan
    {
      Model m;

      CoilCoolingDXSingleSpeed c(m);
      CoilHeatingDXSingleSpeed h(m);
      FanConstantVolume f(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setCoolingCoil(c);
      unitary.setHeatingCoil(h);
      unitary.setSupplyFan(f);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> coolingCoilNodes = getCoolingCoilNodes(workspace);
      std::vector<std::string> heatingCoilNodes = getHeatingCoilNodes(workspace);
      std::vector<std::string> fanNodes = getFanNodes(workspace);

      if (fanPlacement == "BlowThrough") {
        EXPECT_EQ(unitaryNodes[0], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], coolingCoilNodes[0]);
        EXPECT_EQ(coolingCoilNodes[1], heatingCoilNodes[0]);
        EXPECT_EQ(heatingCoilNodes[1], unitaryNodes[1]);
      } else if (fanPlacement == "DrawThrough") {
        EXPECT_EQ(unitaryNodes[0], coolingCoilNodes[0]);
        EXPECT_EQ(coolingCoilNodes[1], heatingCoilNodes[0]);
        EXPECT_EQ(heatingCoilNodes[1], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], unitaryNodes[1]);
      }
    }

    // cooling coil, heating coil, supp heating coil
    {
      Model m;

      CoilCoolingDXSingleSpeed c(m);
      CoilHeatingDXSingleSpeed h(m);
      CoilHeatingDesuperheater s(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setCoolingCoil(c);
      unitary.setHeatingCoil(h);
      unitary.setSupplementalHeatingCoil(s);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> coolingCoilNodes = getCoolingCoilNodes(workspace);
      std::vector<std::string> heatingCoilNodes = getHeatingCoilNodes(workspace);
      std::vector<std::string> suppHeatingCoilNodes = getSuppHeatingCoilNodes(workspace);

      EXPECT_EQ(unitaryNodes[0], coolingCoilNodes[0]);
      EXPECT_EQ(coolingCoilNodes[1], heatingCoilNodes[0]);
      EXPECT_EQ(heatingCoilNodes[1], suppHeatingCoilNodes[0]);
      EXPECT_EQ(suppHeatingCoilNodes[1], unitaryNodes[1]);
    }

    // cooling coil, fan, supp heating coil
    {
      Model m;

      CoilCoolingDXSingleSpeed c(m);
      FanConstantVolume f(m);
      CoilHeatingDesuperheater s(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setCoolingCoil(c);
      unitary.setSupplyFan(f);
      unitary.setSupplementalHeatingCoil(s);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> coolingCoilNodes = getCoolingCoilNodes(workspace);
      std::vector<std::string> fanNodes = getFanNodes(workspace);
      std::vector<std::string> suppHeatingCoilNodes = getSuppHeatingCoilNodes(workspace);

      if (fanPlacement == "BlowThrough") {
        EXPECT_EQ(unitaryNodes[0], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], coolingCoilNodes[0]);
        EXPECT_EQ(coolingCoilNodes[1], suppHeatingCoilNodes[0]);
        EXPECT_EQ(suppHeatingCoilNodes[1], unitaryNodes[1]);
      } else if (fanPlacement == "DrawThrough") {
        EXPECT_EQ(unitaryNodes[0], coolingCoilNodes[0]);
        EXPECT_EQ(coolingCoilNodes[1], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], suppHeatingCoilNodes[0]);
        EXPECT_EQ(suppHeatingCoilNodes[1], unitaryNodes[1]);
      }
    }

    // heating coil, fan, supp heating coil
    {
      Model m;

      CoilHeatingDXSingleSpeed h(m);
      FanConstantVolume f(m);
      CoilHeatingDesuperheater s(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setHeatingCoil(h);
      unitary.setSupplyFan(f);
      unitary.setSupplementalHeatingCoil(s);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> heatingCoilNodes = getHeatingCoilNodes(workspace);
      std::vector<std::string> fanNodes = getFanNodes(workspace);
      std::vector<std::string> suppHeatingCoilNodes = getSuppHeatingCoilNodes(workspace);

      if (fanPlacement == "BlowThrough") {
        EXPECT_EQ(unitaryNodes[0], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], heatingCoilNodes[0]);
        EXPECT_EQ(heatingCoilNodes[1], suppHeatingCoilNodes[0]);
        EXPECT_EQ(suppHeatingCoilNodes[1], unitaryNodes[1]);
      } else if (fanPlacement == "DrawThrough") {
        EXPECT_EQ(unitaryNodes[0], heatingCoilNodes[0]);
        EXPECT_EQ(heatingCoilNodes[1], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], suppHeatingCoilNodes[0]);
        EXPECT_EQ(suppHeatingCoilNodes[1], unitaryNodes[1]);
      }
    }

    // cooling coil, heating coil, fan, supp heating coil
    {
      Model m;

      CoilCoolingDXSingleSpeed c(m);
      CoilHeatingDXSingleSpeed h(m);
      FanConstantVolume f(m);
      CoilHeatingDesuperheater s(m);

      AirLoopHVACUnitarySystem unitary(m);
      unitary.setCoolingCoil(c);
      unitary.setHeatingCoil(h);
      unitary.setSupplyFan(f);
      unitary.setSupplementalHeatingCoil(s);
      unitary.setFanPlacement(fanPlacement);

      AirLoopHVAC airLoop(m);

      Node supplyOutletNode = airLoop.supplyOutletNode();
      unitary.addToNode(supplyOutletNode);

      ForwardTranslator ft;
      Workspace workspace = ft.translateModel(m);

      std::vector<std::string> unitaryNodes = getUnitaryNodes(workspace);
      std::vector<std::string> coolingCoilNodes = getCoolingCoilNodes(workspace);
      std::vector<std::string> heatingCoilNodes = getHeatingCoilNodes(workspace);
      std::vector<std::string> fanNodes = getFanNodes(workspace);
      std::vector<std::string> suppHeatingCoilNodes = getSuppHeatingCoilNodes(workspace);

      if (fanPlacement == "BlowThrough") {
        EXPECT_EQ(unitaryNodes[0], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], coolingCoilNodes[0]);
        EXPECT_EQ(coolingCoilNodes[1], heatingCoilNodes[0]);
        EXPECT_EQ(heatingCoilNodes[1], suppHeatingCoilNodes[0]);
        EXPECT_EQ(suppHeatingCoilNodes[1], unitaryNodes[1]);
      } else if (fanPlacement == "DrawThrough") {
        EXPECT_EQ(unitaryNodes[0], coolingCoilNodes[0]);
        EXPECT_EQ(coolingCoilNodes[1], heatingCoilNodes[0]);
        EXPECT_EQ(heatingCoilNodes[1], fanNodes[0]);
        EXPECT_EQ(fanNodes[1], suppHeatingCoilNodes[0]);
        EXPECT_EQ(suppHeatingCoilNodes[1], unitaryNodes[1]);
      }
    }
  }
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACUnitarySystem_CoilHeatingGasMultiStage) {
  Model m;

  CoilCoolingDXSingleSpeed c(m);
  CoilHeatingGasMultiStage h(m);
  FanConstantVolume f(m);

  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(c);
  unitary.setHeatingCoil(h);
  unitary.setSupplyFan(f);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem).size());
  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::Coil_Heating_Gas_MultiStage).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACUnitarySystem_CoilHeatingElectricMultiStage) {
  Model m;

  CoilCoolingDXSingleSpeed c(m);
  CoilHeatingElectricMultiStage h(m);
  FanConstantVolume f(m);
  CoilHeatingElectricMultiStage s(m);

  AirLoopHVACUnitarySystem unitary(m);
  unitary.setCoolingCoil(c);
  unitary.setHeatingCoil(h);
  unitary.setSupplyFan(f);
  unitary.setSupplementalHeatingCoil(s);

  AirLoopHVAC airLoop(m);

  Node supplyOutletNode = airLoop.supplyOutletNode();
  unitary.addToNode(supplyOutletNode);

  ForwardTranslator ft;
  Workspace workspace = ft.translateModel(m);

  EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem).size());
  EXPECT_EQ(2u, workspace.getObjectsByType(IddObjectType::Coil_Heating_Electric_MultiStage).size());
}

TEST_F(EnergyPlusFixture, ForwardTranslator_AirLoopHVACUnitarySystem_VSCoils) {
  // Test for #4715 - Creating a VS WSHP using UnitarySystem object generates incorrect air flow ratios for UnitarySystemPerformance:Multispeed object

  // autosize the supply air flow rates
  {
    Model m;

    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit c(m);
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData cs1(m);
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData cs2(m);
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData cs3(m);
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData cs4(m);
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData cs5(m);
    c.addSpeed(cs1);
    c.addSpeed(cs2);
    c.addSpeed(cs3);
    c.addSpeed(cs4);
    c.addSpeed(cs5);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit h(m);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData hs1(m);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData hs2(m);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData hs3(m);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData hs4(m);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData hs5(m);
    h.addSpeed(hs1);
    h.addSpeed(hs2);
    h.addSpeed(hs3);
    h.addSpeed(hs4);
    h.addSpeed(hs5);
    FanSystemModel f(m);

    AirLoopHVACUnitarySystem unitary(m);
    unitary.setCoolingCoil(c);
    unitary.setHeatingCoil(h);
    unitary.setSupplyFan(f);

    AirLoopHVAC airLoop(m);

    Node supplyOutletNode = airLoop.supplyOutletNode();
    unitary.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(m);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed).size());

    IdfObject idf_perf = workspace.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed)[0];

    EXPECT_EQ(5, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
    EXPECT_EQ(5, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
    EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
    EXPECT_TRUE(idf_perf.isEmpty(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio));

    ASSERT_EQ(5u, idf_perf.numExtensibleGroups());

    auto egs = idf_perf.extensibleGroups();

    IdfExtensibleGroup eg1 = egs[0];
    EXPECT_FALSE(eg1.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_EQ("Autosize", eg1.getString(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
    EXPECT_FALSE(eg1.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
    EXPECT_EQ("Autosize", eg1.getString(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());

    IdfExtensibleGroup eg2 = egs[1];
    EXPECT_FALSE(eg2.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_EQ("Autosize", eg2.getString(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
    EXPECT_FALSE(eg2.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
    EXPECT_EQ("Autosize", eg2.getString(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());

    IdfExtensibleGroup eg3 = egs[2];
    EXPECT_FALSE(eg3.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_EQ("Autosize", eg3.getString(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
    EXPECT_FALSE(eg3.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
    EXPECT_EQ("Autosize", eg3.getString(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());

    IdfExtensibleGroup eg4 = egs[3];
    EXPECT_FALSE(eg4.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_EQ("Autosize", eg4.getString(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
    EXPECT_FALSE(eg4.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
    EXPECT_EQ("Autosize", eg4.getString(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());

    IdfExtensibleGroup eg5 = egs[4];
    EXPECT_FALSE(eg5.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_EQ("Autosize", eg5.getString(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio).get());
    EXPECT_FALSE(eg5.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
    EXPECT_EQ("Autosize", eg5.getString(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio).get());
  }

  // hardsize the supply air flow rates
  {
    Model m;

    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFit c(m);
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData cs1(m);
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData cs2(m);
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData cs3(m);
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData cs4(m);
    CoilCoolingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData cs5(m);
    c.addSpeed(cs1);
    c.addSpeed(cs2);
    c.addSpeed(cs3);
    c.addSpeed(cs4);
    c.addSpeed(cs5);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFit h(m);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData hs1(m);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData hs2(m);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData hs3(m);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData hs4(m);
    CoilHeatingWaterToAirHeatPumpVariableSpeedEquationFitSpeedData hs5(m);
    h.addSpeed(hs1);
    h.addSpeed(hs2);
    h.addSpeed(hs3);
    h.addSpeed(hs4);
    h.addSpeed(hs5);
    FanSystemModel f(m);

    AirLoopHVACUnitarySystem unitary(m);
    unitary.setSupplyAirFlowRateDuringCoolingOperation(10);
    unitary.setSupplyAirFlowRateDuringHeatingOperation(10);
    unitary.setCoolingCoil(c);
    unitary.setHeatingCoil(h);
    unitary.setSupplyFan(f);

    AirLoopHVAC airLoop(m);

    Node supplyOutletNode = airLoop.supplyOutletNode();
    unitary.addToNode(supplyOutletNode);

    ForwardTranslator ft;
    Workspace workspace = ft.translateModel(m);

    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::AirLoopHVAC_UnitarySystem).size());
    EXPECT_EQ(1u, workspace.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed).size());

    IdfObject idf_perf = workspace.getObjectsByType(IddObjectType::UnitarySystemPerformance_Multispeed)[0];

    EXPECT_EQ(5, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforHeating).get());
    EXPECT_EQ(5, idf_perf.getInt(UnitarySystemPerformance_MultispeedFields::NumberofSpeedsforCooling).get());
    EXPECT_EQ("No", idf_perf.getString(UnitarySystemPerformance_MultispeedFields::SingleModeOperation).get());
    EXPECT_TRUE(idf_perf.isEmpty(UnitarySystemPerformance_MultispeedFields::NoLoadSupplyAirFlowRateRatio));

    ASSERT_EQ(5u, idf_perf.numExtensibleGroups());

    auto egs = idf_perf.extensibleGroups();

    IdfExtensibleGroup eg1 = egs[0];
    EXPECT_TRUE(eg1.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_TRUE(eg1.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));

    IdfExtensibleGroup eg2 = egs[1];
    EXPECT_TRUE(eg2.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_TRUE(eg2.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));

    IdfExtensibleGroup eg3 = egs[2];
    EXPECT_TRUE(eg3.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_TRUE(eg3.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));

    IdfExtensibleGroup eg4 = egs[3];
    EXPECT_TRUE(eg4.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_TRUE(eg4.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));

    IdfExtensibleGroup eg5 = egs[4];
    EXPECT_TRUE(eg5.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::HeatingSpeedSupplyAirFlowRatio));
    EXPECT_TRUE(eg5.getDouble(UnitarySystemPerformance_MultispeedExtensibleFields::CoolingSpeedSupplyAirFlowRatio));
  }
}
