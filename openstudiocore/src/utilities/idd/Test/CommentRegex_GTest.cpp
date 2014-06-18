/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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
  match = std::string(matches[1].first,matches[1].second);
  EXPECT_EQ("-IdfEditor-style comments are ok too.",match);

  line = " !- One last boring comment, with some math: 1 + 1 = (1^1)*2.";
  EXPECT_TRUE(boost::regex_match(line,matches,commentWhitespaceOnlyLine()));
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
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
  EXPECT_FALSE(matches[1].matched);

  line = "     \t \t";
  EXPECT_TRUE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));
  EXPECT_FALSE(matches.empty());
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  EXPECT_TRUE(matches[0].matched);
  EXPECT_FALSE(matches[1].matched);

  line = "  ";
  EXPECT_TRUE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));
  EXPECT_FALSE(matches.empty());
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  EXPECT_TRUE(matches[0].matched);
  EXPECT_FALSE(matches[1].matched);

  line = "  \t !A plain comment, no space";
  EXPECT_FALSE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));

  line = "! A plain comment with space";
  EXPECT_FALSE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));

  line = "  \t\t !-IdfEditor-style comments are ok too.";
  EXPECT_TRUE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
  match = std::string(matches[1].first,matches[1].second);
  EXPECT_EQ("IdfEditor-style comments are ok too.",match);

  line = " !- One last boring comment, with some math: 1 + 1 = (1^1)*2.";
  EXPECT_TRUE(boost::regex_match(line,matches,editorCommentWhitespaceOnlyLine()));
  ASSERT_EQ(static_cast<unsigned>(2),matches.size());
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
