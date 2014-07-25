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
#include "IdfFixture.hpp"

#include "../IdfRegex.hpp"
#include "../../idd/CommentRegex.hpp"

using openstudio::commentRegex::commentWhitespaceOnlyBlock;

TEST_F(IdfFixture, MultipleFieldsPerLine) 
{

}

TEST_F(IdfFixture, CommentsAndWhitespace)
{
  std::string testString;
  testString = "\n\n! A comment block\n! Of two lines.\n\n\n\n! More comments.";
  boost::match_results<std::string::const_iterator> m;
  EXPECT_TRUE(boost::regex_match(testString,m,commentWhitespaceOnlyBlock()));

  testString = "! A comment line\n code() and other text ! comment\n\n";
  EXPECT_FALSE(boost::regex_match(testString,m,commentWhitespaceOnlyBlock()));

  testString = "The quick brown fox jumped over the lazy dog.";
  EXPECT_FALSE(boost::regex_match(testString,m,commentWhitespaceOnlyBlock()));

  testString = "";
  EXPECT_TRUE(boost::regex_match(testString,m,commentWhitespaceOnlyBlock()));

  testString = "\n\n\n";
  EXPECT_TRUE(boost::regex_match(testString,m,commentWhitespaceOnlyBlock()));

  testString = "    \n\n   ";
  EXPECT_TRUE(boost::regex_match(testString,m,commentWhitespaceOnlyBlock()));
}
