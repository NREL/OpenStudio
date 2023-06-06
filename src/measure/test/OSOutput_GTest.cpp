/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "MeasureFixture.hpp"

#include "../OSOutput.hpp"

#include <vector>
#include <map>

using namespace openstudio;
using namespace openstudio::measure;

TEST_F(MeasureFixture, OSOutput) {

  OSOutput boolOutput = OSOutput::makeBoolOutput("bool");
  OSOutput doubleOutput = OSOutput::makeDoubleOutput("double");
  OSOutput integerOutput = OSOutput::makeIntegerOutput("integer");
  OSOutput stringOutput = OSOutput::makeStringOutput("string");

  EXPECT_EQ(OSOutputType::Boolean, boolOutput.type().value());
  EXPECT_EQ(OSOutputType::Integer, integerOutput.type().value());
  EXPECT_EQ(OSOutputType::Double, doubleOutput.type().value());
  EXPECT_EQ(OSOutputType::String, stringOutput.type().value());

  std::stringstream os;
  os << boolOutput;
  os << integerOutput;
  os << doubleOutput;
  os << stringOutput;
}
