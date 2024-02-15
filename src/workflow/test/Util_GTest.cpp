/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../Util.hpp"

#include <map>
#include <string>

class WorkflowFixture : public testing::Test
{
};

TEST_F(WorkflowFixture, Util_sanitizeKey) {

  const std::map<std::string, std::string> hash_with_sanitized_keys = {
    {"Regular", "Regular"},
    {"key1!@#", "key1"},
    {"key2.{}\\", "key2"},
    {"key3_ ", "key3"},
    {"invalid|key", "invalid_key"},
    {"another key", "another key"},
    {"special@char", "special_char"},
    {"key.with.dots", "key_with_dots"},
    {"key with spaces", "key with spaces"},
    {"slashes\\/included", "slashes_included"},
    {"pipe|character", "pipe_character"},
    {"!exclamation", "_exclamation"},
    {"@at_symbol", "_at_symbol"},
    {"#hash_symbol", "_hash_symbol"},
    {"$dollar_sign", "_dollar_sign"},
    {"%percent_sign", "_percent_sign"},
    {"^caret", "_caret"},
    {"&ampersand", "_ampersand"},
    {"*asterisk", "_asterisk"},
    {"(open_parenthesis)", "_open_parenthesis"},
    {"{open_brace}", "_open_brace"},
    {"}close_brace{", "_close_brace"},
    {"\\backslash\\", "_backslash"},
    {"[open_square]", "_open_square"},
    {"]close_square", "_close_square"},
    {";semicolon;", "_semicolon"},
    {":colon:", "_colon"},
    {"'single_quote'", "_single_quote"},
    {"\"double_quote\"", "_double_quote"},
    {",comma,", "_comma"},
    {"<less_than>", "_less_than"},
    {".period.", "_period"},
    {">greater_than<", "_greater_than"},
    {"/forward_slash/", "_forward_slash"},
    {"?question_mark?", "_question_mark"},
    {"=equal_sign=", "_equal_sign"},
    {"+plus_sign+", "_plus_sign"},
    {"___", ""},  // Consecutive underscores to be squeezed
    {"key_with___underscores", "key_with_underscores"},
    {"key_with__underscores", "key_with_underscores"},
    {"key_with_underscores", "key_with_underscores"},
    {"__underscored__key__", "_underscored_key"},          // Multiple underscores within the key
    {"___double___underscores___", "_double_underscores"}  // Multiple consecutive underscores
  };

  for (const auto& [originalKey, expectedKey] : hash_with_sanitized_keys) {
    const std::string sanitizedKey = openstudio::workflow::util::sanitizeKey(originalKey);
    // Check if the sanitized key matches the expected value
    EXPECT_EQ(sanitizedKey, expectedKey) << "Error: Key '" << originalKey << "' was not sanitized as expected.";
  }
}
