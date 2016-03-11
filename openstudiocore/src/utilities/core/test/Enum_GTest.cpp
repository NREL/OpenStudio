/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#include "../Enum.hpp"
#include "../String.hpp"
#include "../Containers.hpp"

#include <iostream>

OPENSTUDIO_ENUM(TestEnum4, ((first)) ((second)) ((third)) );

namespace openstudio {

OPENSTUDIO_ENUM(TestEnum5, ((first)) ((second)) ((third)) );

namespace enums {
  OPENSTUDIO_ENUM(TestEnum, ((first)) ((second)) ((third)) );

  OPENSTUDIO_ENUM(TestEnum2, ((first)(first)(1)) ((second)) ((third)) );
  OPENSTUDIO_ENUM(TestEnum3, ((first)) ((second)(My Second)(2)) ((third)) ((fourth)(fourth)(5))
    ((SIXTH)) );


}
} // openstudio


TEST(Enum, EnumThrows)
{
  EXPECT_THROW(openstudio::enums::TestEnum("forth"), std::runtime_error);
  EXPECT_NO_THROW(openstudio::enums::TestEnum("third"));
  EXPECT_THROW(openstudio::enums::TestEnum(3), std::runtime_error);
  EXPECT_THROW(openstudio::enums::TestEnum3(4), std::runtime_error);
  EXPECT_THROW(openstudio::enums::TestEnum3("bob"), std::runtime_error);
  EXPECT_NO_THROW(openstudio::enums::TestEnum3("my second"));
}


TEST(Enum, EnumDescription)
{
  openstudio::enums::TestEnum3 te5("my second");
  EXPECT_EQ("second", te5.valueName());
  EXPECT_EQ("TestEnum3", te5.enumName());
  EXPECT_EQ(static_cast<openstudio::enums::TestEnum3::domain>(2), te5.value());
  EXPECT_EQ("My Second", te5.valueDescription());
}

TEST(Enum, EnumInterface)
{
  openstudio::enums::TestEnum te1("THIRD");
  openstudio::enums::TestEnum2 te2("SeConD");
  openstudio::enums::TestEnum3 te3(3);
  openstudio::enums::TestEnum3 te4("SIXTH");
  EXPECT_THROW(openstudio::enums::TestEnum3 te5("Seventh"), std::runtime_error); // does not exist

  EXPECT_EQ("third", te1.valueName());
  EXPECT_EQ("TestEnum", te1.enumName());
  EXPECT_EQ(2, te2.value());

  EXPECT_EQ("second", te2.valueName());
  EXPECT_EQ("TestEnum2", te2.enumName());
  EXPECT_EQ(static_cast<openstudio::enums::TestEnum2::domain>(2), te2.value());

  EXPECT_EQ("third", te3.valueName());
  EXPECT_EQ("TestEnum3", te3.enumName());
  EXPECT_EQ(static_cast<openstudio::enums::TestEnum3::domain>(3), te3.value());

  EXPECT_EQ("SIXTH", te4.valueName());
  EXPECT_EQ("TestEnum3", te4.enumName());
  EXPECT_EQ(static_cast<openstudio::enums::TestEnum3::domain>(6), te4.value());

  // Integer comparison
  EXPECT_TRUE(te4 > 3);
}

TEST(Enum, GetValues) {
  openstudio::IntSet values = openstudio::enums::TestEnum3::getValues();
  ASSERT_EQ(static_cast<unsigned>(5),values.size());
  EXPECT_FALSE(values.find(0) == values.end());
  EXPECT_FALSE(values.find(2) == values.end());
  EXPECT_FALSE(values.find(3) == values.end());
  EXPECT_FALSE(values.find(5) == values.end());
  EXPECT_FALSE(values.find(6) == values.end());
}

TEST(Enum, GetDescriptions) {
  std::map<int, std::string> descriptions = openstudio::enums::TestEnum3::getDescriptions();
  ASSERT_EQ(static_cast<unsigned>(2), descriptions.size());
  EXPECT_EQ(descriptions[2], "My Second");
  EXPECT_EQ(descriptions[5], "fourth");
}

TEST(Enum, GetNames) {
  std::map<int, std::string> names = openstudio::enums::TestEnum3::getNames();
  ASSERT_EQ(static_cast<unsigned>(5), names.size());
  EXPECT_EQ(names[0], "first");
  EXPECT_EQ(names[2], "second");
  EXPECT_EQ(names[3], "third");
  EXPECT_EQ(names[5], "fourth");
  EXPECT_EQ(names[6], "SIXTH");

}
