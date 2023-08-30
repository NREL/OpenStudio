/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "EnergyPlusFixture.hpp"

#include "../ForwardTranslator.hpp"
#include "../ReverseTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/OutputControlTimestamp.hpp"
#include "../../model/OutputControlTimestamp_Impl.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/IdfObject.hpp"
#include "../../utilities/idf/WorkspaceObject.hpp"

#include <utilities/idd/OutputControl_Timestamp_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::energyplus;
using namespace openstudio::model;
using namespace openstudio;

TEST_F(EnergyPlusFixture, ForwardTranslator_OutputControlTimestamp) {

  ForwardTranslator ft;

  // Create a model
  Model m;

  // Get the unique object
  OutputControlTimestamp outputControlTimestamp = m.getUniqueModelObject<OutputControlTimestamp>();

  // Check all cases where a single output request is True so we know we assigned the fields correctly
  auto boolToString = [](bool b) { return b ? "Yes" : "No"; };

  for (size_t i = 0; i < 2; ++i) {
    bool status[2] = {false};
    status[i] = true;
    EXPECT_TRUE(outputControlTimestamp.setISO8601Format(status[0]));
    EXPECT_TRUE(outputControlTimestamp.setTimestampAtBeginningOfInterval(status[1]));

    Workspace w = ft.translateModel(m);
    WorkspaceObjectVector idfObjs = w.getObjectsByType(IddObjectType::OutputControl_Timestamp);
    ASSERT_EQ(1u, idfObjs.size());

    WorkspaceObject idf_outputControlTimestamp(idfObjs[0]);

    EXPECT_EQ(boolToString(status[0]), idf_outputControlTimestamp.getString(OutputControl_TimestampFields::ISO8601Format).get());
    EXPECT_EQ(boolToString(status[1]), idf_outputControlTimestamp.getString(OutputControl_TimestampFields::TimestampatBeginningofInterval).get());
  }
}

TEST_F(EnergyPlusFixture, ReverseTranslator_OutputControlTimestamp) {

  ReverseTranslator rt;

  Workspace w(StrictnessLevel::Minimal, IddFileType::EnergyPlus);

  // Not there, Model shouldn't have it either
  Model m = rt.translateWorkspace(w);
  EXPECT_FALSE(m.getOptionalUniqueModelObject<OutputControlTimestamp>());

  OptionalWorkspaceObject _i_outputControlTimestamp = w.addObject(IdfObject(IddObjectType::OutputControl_Timestamp));
  ASSERT_TRUE(_i_outputControlTimestamp);

  auto boolToString = [](bool b) { return b ? "Yes" : "No"; };

  for (size_t i = 0; i < 2; ++i) {
    bool status[2] = {false};
    status[i] = true;
    EXPECT_TRUE(_i_outputControlTimestamp->setString(OutputControl_TimestampFields::ISO8601Format, boolToString(status[0])));
    EXPECT_TRUE(_i_outputControlTimestamp->setString(OutputControl_TimestampFields::TimestampatBeginningofInterval, boolToString(status[1])));

    Model m = rt.translateWorkspace(w);

    // // Get the unique object
    OutputControlTimestamp outputControlTimestamp = m.getUniqueModelObject<OutputControlTimestamp>();

    EXPECT_EQ(status[0], outputControlTimestamp.iso8601Format());
    EXPECT_EQ(status[1], outputControlTimestamp.timestampAtBeginningOfInterval());
  }
}
