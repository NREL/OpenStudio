/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "BCLFixture.hpp"

#include "../../core/StringStreamLogSink.hpp"

#include "../BCLComponent.hpp"

using namespace openstudio;

TEST_F(BCLFixture, BCLComponent) {
  std::string dir = toString(resourcesPath()) + "/utilities/BCL/Components/philadelphia pa [724086 TMY2-13739]/";
  BCLComponent component(dir);

  // "_" has been replaced by " "
  EXPECT_EQ("philadelphia pa [724086 TMY2-13739]", component.name());
}

TEST_F(BCLFixture, BCLComponent_WrongType_float) {
  // Test for #323
  openstudio::path compDir = resourcesPath() / "utilities/BCL/Components/323_wrongXMLType_float";

  StringStreamLogSink sink;
  sink.setLogLevel(Error);

  EXPECT_THROW(BCLComponent{compDir}, openstudio::Exception);
  ASSERT_EQ(1, sink.logMessages().size());
  EXPECT_EQ("Please double check your XML, you have an attribute named 'Effective R-value' with a datatype of 'float' but the value isn't a double: "
            "'THIS IS A MISTAKE'.",
            sink.logMessages()[0].logMessage());
}

TEST_F(BCLFixture, BCLComponent_WrongType_int) {
  // Test for #323
  openstudio::path compDir = resourcesPath() / "utilities/BCL/Components/323_wrongXMLType_int";

  StringStreamLogSink sink;
  sink.setLogLevel(Error);

  EXPECT_THROW(BCLComponent{compDir}, openstudio::Exception);
  ASSERT_EQ(1, sink.logMessages().size());
  EXPECT_EQ("Please double check your XML, you have an attribute named 'Effective R-value' with a datatype of 'int' but the value isn't an integer: "
            "'THIS IS A MISTAKE'.",
            sink.logMessages()[0].logMessage());
}
