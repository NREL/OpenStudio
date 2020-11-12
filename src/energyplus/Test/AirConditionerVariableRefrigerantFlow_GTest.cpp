/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/AirConditionerVariableRefrigerantFlow.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/Node.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/AirConditioner_VariableRefrigerantFlow_FieldEnums.hxx>

#include <resources.hxx>

#include <sstream>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslatorAirConditionerVariableRefrigerantFlow_harcodedCondenserType) {

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

TEST_F(EnergyPlusFixture, ForwardTranslatorAirConditionerVariableRefrigerantFlow_defaultedCondenserType) {

  ForwardTranslator ft;

  // No Plant Loop => AirCooled
  {
    Model m;
    AirConditionerVariableRefrigerantFlow vrf(m);
    vrf.resetCondenserType();

    EXPECT_TRUE(vrf.isCondenserTypeDefaulted());
    EXPECT_EQ("AirCooled", vrf.condenserType());

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
