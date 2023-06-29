/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../Optional.hpp"

#include <sstream>

using openstudio::OptionalInt;
using openstudio::OptionalDouble;
using openstudio::OptionalString;
using openstudio::operator<<;

namespace {

TEST(OptionalTest, builtInTypes) {
  std::stringstream ss;

  OptionalInt i;
  EXPECT_FALSE(i);
  ss << i;
  EXPECT_EQ("", ss.str());
  ss.str("");
  i = 1;
  EXPECT_TRUE(i);
  ss << i;
  EXPECT_EQ("1", ss.str());
  ss.str("");
  int j = *i + 1;
  EXPECT_EQ(2, j);

  OptionalDouble d;
  EXPECT_FALSE(d);
  ss << d;
  EXPECT_EQ("", ss.str());
  ss.str("");
  d = 1.0;
  EXPECT_TRUE(d);
  ss << d;
  EXPECT_EQ("1", ss.str());
  ss.str("");
  double d2 = *d + 1.0;
  EXPECT_EQ(2.0, d2);
}

// cppcheck-suppress syntaxError
TEST(OptionalTest, stdTypes) {
  std::stringstream ss;

  OptionalString str;
  EXPECT_FALSE(str);
  ss << str;
  EXPECT_EQ("", ss.str());
  ss.str("");
  str = "Hello World!";
  ASSERT_TRUE(str);
  EXPECT_EQ("Hello World!", *str);
  ss << str;
  EXPECT_EQ("Hello World!", ss.str());
  ss.str("");
}

}  // namespace
