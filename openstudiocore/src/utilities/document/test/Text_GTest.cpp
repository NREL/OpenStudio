/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <utilities/document/Text.hpp>

#include <utilities/core/Path.hpp>
#include <utilities/core/Containers.hpp>

#include <resources.hxx>

#include <iomanip>

using openstudio::Text;
using openstudio::OptionalText;
using openstudio::toPath;
using openstudio::StringVector;

TEST(Text, ConstructGetSet)
{
  std::string firstParagraph("Let me not stay a jot for dinner; go get it ready.");
  StringVector paragraphs;
  paragraphs.push_back("How now! what art thou?");
  paragraphs.push_back("A man, sir.");
  paragraphs.push_back("What dost thou profess? what wouldst thou with us?");
  std::string lastParagraph("I do profess to be no less than I seem; to serve<br/> \
                            him truly that will put me in trust: to love him<br/> \
                            that is honest; to converse with him that is wise,<br/> \
                            and says little; to fear judgment; to fight when I<br/> \
                            cannot choose; and to eat no fist.");
  
  Text text(firstParagraph);
  StringVector textParagraphs = text.paragraphs();
  ASSERT_EQ(static_cast<unsigned>(1),textParagraphs.size());
  EXPECT_EQ("Let me not stay a jot for dinner; go get it ready.",textParagraphs[0]);

  text = Text(paragraphs);
  text.insert(0,firstParagraph);
  textParagraphs = text.paragraphs();
  EXPECT_EQ(static_cast<unsigned>(4),textParagraphs.size());
  EXPECT_EQ(firstParagraph,textParagraphs[0]);
  text.replace(textParagraphs.size(),lastParagraph);
  textParagraphs = text.paragraphs();
  EXPECT_EQ(static_cast<unsigned>(5),textParagraphs.size());
  EXPECT_ANY_THROW(text.insert(textParagraphs.size() + 2,lastParagraph));
}
