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
#include "../../model/OutputJSON.hpp"
#include "../../model/OutputJSON_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/Output_JSON_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputJSON) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // Get the unique object
  OutputJSON outputJSON = m.getUniqueModelObject<OutputJSON>();

  // Check all cases where a single output request is True so we know we assigned the fields correctly
  auto boolToString = [](bool b) { return b ? "Yes" : "No"; };

  for (int i = 0; i < 3; ++i) {
    bool status[] = {false, false, false};
    status[i] = true;
    bool json = status[0];
    bool cbor = status[1];
    bool msgpack = status[2];
    EXPECT_TRUE(outputJSON.setOutputJSON(json));
    EXPECT_TRUE(outputJSON.setOutputCBOR(cbor));
    EXPECT_TRUE(outputJSON.setOutputMessagePack(msgpack));

    Workspace w = ft.translateModel(m);

    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::Output_JSON);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_json(idfObjs[0]);

    EXPECT_EQ(boolToString(json), idf_json.getString(Output_JSONFields::OutputJSON).get());
    EXPECT_EQ(boolToString(cbor), idf_json.getString(Output_JSONFields::OutputCBOR).get());
    EXPECT_EQ(boolToString(msgpack), idf_json.getString(Output_JSONFields::OutputMessagePack).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputJSON) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::None, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  Model m = rt.translateWorkspace(w);
  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputJSON>());

  OptionalWorkspaceObject _i_outputJSON = w.addObject(IdfObject(IddObjectType::Output_JSON));
  ASSERT_TRUE(_i_outputJSON);

  auto boolToString = [](bool b) { return b ? "Yes" : "No"; };

  for (int i = 0; i < 3; ++i) {
    bool status[] = {false, false, false};
    status[i] = true;
    bool json = status[0];
    bool cbor = status[1];
    bool msgpack = status[2];

    EXPECT_TRUE(_i_outputJSON->setString(Output_JSONFields::OutputJSON, boolToString(json)));
    EXPECT_TRUE(_i_outputJSON->setString(Output_JSONFields::OutputCBOR, boolToString(cbor)));
    EXPECT_TRUE(_i_outputJSON->setString(Output_JSONFields::OutputMessagePack, boolToString(msgpack)));

    Model m = rt.translateWorkspace(w);

    // Get the unique object
    OutputJSON outputJSON = m.getUniqueModelObject<OutputJSON>();
    EXPECT_EQ(json, outputJSON.outputJSON());
    EXPECT_EQ(cbor, outputJSON.outputCBOR());
    EXPECT_EQ(msgpack, outputJSON.outputMessagePack());
  }
}
