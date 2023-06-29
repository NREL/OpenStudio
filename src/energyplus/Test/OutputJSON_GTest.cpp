/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
  auto outputJSON = m.getUniqueModelObject<OutputJSON>();

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

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

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
    auto outputJSON = m.getUniqueModelObject<OutputJSON>();
    EXPECT_EQ(json, outputJSON.outputJSON());
    EXPECT_EQ(cbor, outputJSON.outputCBOR());
    EXPECT_EQ(msgpack, outputJSON.outputMessagePack());
  }
}
