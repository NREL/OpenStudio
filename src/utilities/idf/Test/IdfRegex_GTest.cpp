/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "IdfFixture.hpp"

#include "../IdfRegex.hpp"
#include "../../idd/CommentRegex.hpp"

using openstudio::commentRegex::commentWhitespaceOnlyBlock;

TEST_F(IdfFixture, MultipleFieldsPerLine) {}

TEST_F(IdfFixture, CommentsAndWhitespace) {
  std::string testString;
  testString = "\n\n! A comment block\n! Of two lines.\n\n\n\n! More comments.";
  boost::match_results<std::string::const_iterator> m;
  EXPECT_TRUE(boost::regex_match(testString, m, commentWhitespaceOnlyBlock()));

  testString = "! A comment line\n code() and other text ! comment\n\n";
  EXPECT_FALSE(boost::regex_match(testString, m, commentWhitespaceOnlyBlock()));

  testString = "The quick brown fox jumped over the lazy dog.";
  EXPECT_FALSE(boost::regex_match(testString, m, commentWhitespaceOnlyBlock()));

  testString = "";
  EXPECT_TRUE(boost::regex_match(testString, m, commentWhitespaceOnlyBlock()));

  testString = "\n\n\n";
  EXPECT_TRUE(boost::regex_match(testString, m, commentWhitespaceOnlyBlock()));

  testString = "    \n\n   ";
  EXPECT_TRUE(boost::regex_match(testString, m, commentWhitespaceOnlyBlock()));
}
