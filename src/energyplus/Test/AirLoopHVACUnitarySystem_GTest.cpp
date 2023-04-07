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

#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/AirLoopHVAC_UnitarySystem_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_SingleSpeed_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Desuperheater_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Gas_MultiStage_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_MultiStage_FieldEnums.hxx>
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
