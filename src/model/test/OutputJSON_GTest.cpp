/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../OutputJSON.hpp"
#include "../OutputJSON_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, OutputJSON_GettersSetters) {

  Model model;
  auto outputJSON = model.getUniqueModelObject<OutputJSON>();

  // Option Type: Required String
  EXPECT_TRUE(outputJSON.setOptionType("TimeSeries"));
  EXPECT_EQ("TimeSeries", outputJSON.optionType());
  // Bad Value
  EXPECT_FALSE(outputJSON.setOptionType("BADENUM"));
  EXPECT_EQ("TimeSeries", outputJSON.optionType());

  // Output JSON: Required Boolean
  EXPECT_TRUE(outputJSON.setOutputJSON(true));
  EXPECT_TRUE(outputJSON.outputJSON());
  EXPECT_TRUE(outputJSON.setOutputJSON(false));
  EXPECT_FALSE(outputJSON.outputJSON());

  // Output CBOR: Required Boolean
  EXPECT_TRUE(outputJSON.setOutputCBOR(true));
  EXPECT_TRUE(outputJSON.outputCBOR());
  EXPECT_TRUE(outputJSON.setOutputCBOR(false));
  EXPECT_FALSE(outputJSON.outputCBOR());

  // Output MessagePack: Required Boolean
  EXPECT_TRUE(outputJSON.setOutputMessagePack(true));
  EXPECT_TRUE(outputJSON.outputMessagePack());
  EXPECT_TRUE(outputJSON.setOutputMessagePack(false));
  EXPECT_FALSE(outputJSON.outputMessagePack());

}  // End of Getter_Setters test
