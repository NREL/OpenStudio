/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "IddFixture.hpp"

#include "../CommentRegex.hpp"
#include "../Comments.hpp"

using namespace openstudio::commentRegex;
using openstudio::makeComment;
using openstudio::makeIdfEditorComment;

TEST_F(IddFixture,CommentRegex_Lines) {
  std::string line;
  boost::smatch matches;

  // whitespaceOnlyLine
  EXPECT_TRUE(boost::regex_match(line,matches,whitespaceOnlyLine()));
  EXPECT_FALSE(matches.empty());
  ASSERT_EQ(static_cast<unsigned>(1),matches.size());
  EXPECT_TRUE(matches[0].matched);

  line = "     \t \t";
  EXPECT_TRUE(boost::regex_match(line,matches,whitespaceOnlyLine()));
  EXPECT_FALSE(matches.empty());
  ASSERT_EQ(static_cast<unsigned>(1),matches.size());
  EXPECT_TRUE(matches[0].matched);

  line = "  \n";
  EXPECT_FALSE(boost::regex_match(line,matches,whitespaceOnlyLine()));

  line = "\n\n\n";
  EXPECT_FALSE(boost::regex_match(line,matches,whitespaceOnlyLine()));

  line = "Text is not whitespace.";
  EXPECT_FALSE(boost::regex_match(line,matches,whitespaceOnlyLine()));

  line = "! Comment is not whitespace.";
  EXPECT_FALSE(boost::regex_match(line,matches,whitespaceOnlyLine()));

  // commentWhitespaceOnlyLine
  line = "";
  EXPECT_TRUE(boost::regex_match(line,matches,commentWhitespaceOnlyLine()));
  EXPECT_FALSE(matches.empty());
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  EXPECT_TRUE(matches[0].matched);
  EXPECT_FALSE(matches[1].matched);

  line = "     \t \t";
  EXPECT_TRUE(boost::regex_match(line,matches,commentWhitespaceOnlyLine()));
  EXPECT_FALSE(matches.empty());
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  EXPECT_TRUE(matches[0].matched);
  EXPECT_FALSE(matches[1].matched);

  line = "  ";
  EXPECT_TRUE(boost::regex_match(line,matches,commentWhitespaceOnlyLine()));
  EXPECT_FALSE(matches.empty());
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  EXPECT_TRUE(matches[0].matched);
  EXPECT_FALSE(matches[1].matched);

  line = "  \t !A plain comment, no space";
  EXPECT_TRUE(boost::regex_match(line,matches,commentWhitespaceOnlyLine()));
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  std::string match(matches[1].first,matches[1].second);
  EXPECT_EQ("A plain comment, no space",match);

  line = "! A plain comment with space";
  EXPECT_TRUE(boost::regex_match(line,matches,commentWhitespaceOnlyLine()));
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  match = std::string(matches[1].first,matches[1].second);
  EXPECT_EQ(" A plain comment with space",match);

  line = "  \t\t !-IdfEditor-style comments are ok too.";
  EXPECT_TRUE(boost::regex_match(line,matches,commentWhitespaceOnlyLine()));
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  // cppcheck-suppress arrayIndexOutOfBounds
  match = std::string(matches[1].first,matches[1].second);
  EXPECT_EQ("-IdfEditor-style comments are ok too.",match);

  line = " !- One last boring comment, with some math: 1 + 1 = (1^1)*2.";
  EXPECT_TRUE(boost::regex_match(line,matches,commentWhitespaceOnlyLine()));
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  // cppcheck-suppress arrayIndexOutOfBounds
  match = std::string(matches[1].first,matches[1].second);
  EXPECT_EQ("- One last boring comment, with some math: 1 + 1 = (1^1)*2.",match);

  line = " Prefix to comment is not ok. ! Even with a comment behind it.";
  EXPECT_FALSE(boost::regex_match(line,matches,commentWhitespaceOnlyLine()));

  line = "! Comment good, \n ! Multiple lines bad.";
  EXPECT_FALSE(boost::regex_match(line,matches,commentWhitespaceOnlyLine()));

  // editorCommentWhitespaceOnlyLine
  line = "";
  EXPECT_TRUE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));
  EXPECT_FALSE(matches.empty());
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  EXPECT_TRUE(matches[0].matched);
  // cppcheck-suppress arrayIndexOutOfBounds
  EXPECT_FALSE(matches[1].matched);

  line = "     \t \t";
  EXPECT_TRUE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));
  EXPECT_FALSE(matches.empty());
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  EXPECT_TRUE(matches[0].matched);
  // cppcheck-suppress arrayIndexOutOfBounds
  EXPECT_FALSE(matches[1].matched);

  line = "  ";
  EXPECT_TRUE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));
  EXPECT_FALSE(matches.empty());
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  EXPECT_TRUE(matches[0].matched);
  // cppcheck-suppress arrayIndexOutOfBounds
  EXPECT_FALSE(matches[1].matched);

  line = "  \t !A plain comment, no space";
  EXPECT_FALSE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));

  line = "! A plain comment with space";
  EXPECT_FALSE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));

  line = "  \t\t !-IdfEditor-style comments are ok too.";
  EXPECT_TRUE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  // cppcheck-suppress arrayIndexOutOfBounds
  match = std::string(matches[1].first,matches[1].second);
  EXPECT_EQ("IdfEditor-style comments are ok too.",match);

  line = " !- One last boring comment, with some math: 1 + 1 = (1^1)*2.";
  EXPECT_TRUE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  // cppcheck-suppress arrayIndexOutOfBounds
  match = std::string(matches[1].first,matches[1].second);
  EXPECT_EQ(" One last boring comment, with some math: 1 + 1 = (1^1)*2.",match);

  line = " Prefix to comment is not ok. ! Even with a comment behind it.";
  EXPECT_FALSE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));

  line = "! Comment good, \n ! Multiple lines bad.";
  EXPECT_FALSE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));
}

TEST_F(IddFixture,CommentRegex_Blocks) {
  std::string block;
  boost::smatch matches;

  // commentWhitespaceOnlyBlock
  EXPECT_TRUE(boost::regex_match(block,matches,commentWhitespaceOnlyBlock()));
  EXPECT_FALSE(matches.empty());
  EXPECT_EQ(static_cast<unsigned>(1),matches.size());

  block = "  \t\t\n \n    \n\n\t\t\t";
  EXPECT_TRUE(boost::regex_match(block,matches,commentWhitespaceOnlyBlock()));
  EXPECT_FALSE(matches.empty());
  EXPECT_EQ(static_cast<unsigned>(1),matches.size());

  block = "  \n! Hi there, folks!\n\n! Welcome to Colorado!";
  EXPECT_TRUE(boost::regex_match(block,matches,commentWhitespaceOnlyBlock()));
  EXPECT_FALSE(matches.empty());
  EXPECT_EQ(static_cast<unsigned>(1),matches.size());

  block = "!- This comment could have been auto-generated by IdfEditor\n!- But it wasn't\n";
  EXPECT_TRUE(boost::regex_match(block,matches,commentWhitespaceOnlyBlock()));
  EXPECT_FALSE(matches.empty());
  EXPECT_EQ(static_cast<unsigned>(1),matches.size());

  block = "This is a block\n\nThat has text in it\n! Most of which is not commented.";
  EXPECT_FALSE(boost::regex_match(block,matches,commentWhitespaceOnlyBlock()));

  // editorCommentWhitespaceOnlyBlock
  block = "";
  EXPECT_TRUE(boost::regex_match(block,matches,editorCommentWhitespaceOnlyBlock()));
  EXPECT_FALSE(matches.empty());
  EXPECT_EQ(static_cast<unsigned>(1),matches.size());

  block = "  \t\t\n \n    \n\n\t\t\t";
  EXPECT_TRUE(boost::regex_match(block,matches,editorCommentWhitespaceOnlyBlock()));
  EXPECT_FALSE(matches.empty());
  EXPECT_EQ(static_cast<unsigned>(1),matches.size());

  block = "  \n! Hi there, folks!\n\n! Welcome to Colorado!";
  EXPECT_FALSE(boost::regex_match(block,matches,editorCommentWhitespaceOnlyBlock()));

  block = "!- This comment could have been auto-generated by IdfEditor\n!- But it wasn't\n";
  EXPECT_TRUE(boost::regex_match(block,matches,editorCommentWhitespaceOnlyBlock()));
  EXPECT_FALSE(matches.empty());
  EXPECT_EQ(static_cast<unsigned>(1),matches.size());

  block = "This is a block\n\nThat has text in it\n! Most of which is not commented.";
  EXPECT_FALSE(boost::regex_match(block,matches,editorCommentWhitespaceOnlyBlock()));
}

TEST_F(IddFixture,CommentRegex_MakeComment) {
  std::stringstream block;
  std::string comment, editorComment;
  boost::smatch matches;

  block << "One line without endl";
  comment = makeComment(block.str());
  EXPECT_EQ("! One line without endl",comment);
  block.str("");

  block << "One line with endl\n";
  comment = makeComment(block.str());
  EXPECT_EQ("! One line with endl\n",comment);
  block.str("");

  block << "Comment block has two lines \n and no comment marks.";
  comment = makeComment(block.str());
  editorComment = makeIdfEditorComment(block.str());
  EXPECT_EQ("! Comment block has two lines \n! and no comment marks.",comment);
  EXPECT_EQ("!- Comment block has two lines \n!- and no comment marks.",editorComment);
  block.str("");

  block << "The quick brown fox jumped over the lazy dog." << std::endl
        << "!The quick brown fox jumped over the lazy dog." << std::endl
        << "The quick !brown fox jumped over the laxy dog." << std::endl
        << "!- The quick brown fox jumped over the lazy dog." << std::endl;
  comment = makeComment(block.str());
  editorComment = makeIdfEditorComment(block.str());
  block.str("");
  block << "! The quick brown fox jumped over the lazy dog." << std::endl
        << "! The quick brown fox jumped over the lazy dog." << std::endl
        << "! The quick !brown fox jumped over the laxy dog." << std::endl
        << "! The quick brown fox jumped over the lazy dog." << std::endl;
  EXPECT_EQ(block.str(),comment);
  block.str("");
  block << "!- The quick brown fox jumped over the lazy dog." << std::endl
        << "!- The quick brown fox jumped over the lazy dog." << std::endl
        << "!- The quick !brown fox jumped over the laxy dog." << std::endl
        << "!- The quick brown fox jumped over the lazy dog." << std::endl;
  EXPECT_EQ(block.str(),editorComment);
  block.str("");

  block << "" << std::endl
        << "  " << std::endl
        << "!A comment." << std::endl
        << " " << std::endl
        << " " << std::endl
        << "! That continues ..." << std::endl
        << std::endl;
  comment = makeComment(block.str());
  editorComment = makeIdfEditorComment(block.str());
  EXPECT_EQ(block.str(),comment); // retains as-is
  block.str("");
  block << std::endl
        << std::endl
        << "!- A comment." << std::endl
        << "!- " << std::endl
        << "!- " << std::endl
        << "!- That continues ..." << std::endl
        << std::endl;
  EXPECT_EQ(block.str(),editorComment);
  block.str("");

}
