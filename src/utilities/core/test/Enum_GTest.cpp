/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../Enum.hpp"
#include "../String.hpp"
#include "../Containers.hpp"

#include <iostream>

OPENSTUDIO_ENUM(TestEnum4, ((first))((second))((third)));

namespace openstudio {

OPENSTUDIO_ENUM(TestEnum5, ((first))((second))((third)));

namespace enums {
  OPENSTUDIO_ENUM(TestEnum, ((first))((second))((third)));

  OPENSTUDIO_ENUM(TestEnum2, ((first)(first)(1))((second))((third)));
  OPENSTUDIO_ENUM(TestEnum3, ((first))((second)(My Second)(2))((third))((fourth)(fourth)(5))((SIXTH)));

}  // namespace enums
}  // namespace openstudio

TEST(Enum, EnumThrows) {
  EXPECT_THROW(openstudio::enums::TestEnum("forth"), std::runtime_error);
  // #1741, we expect an informative error message
  try {
    openstudio::enums::TestEnum("forth");
  } catch (std::runtime_error& e) {
    std::string expectedErrorMessage("Unknown OpenStudio Enum Value 'FORTH' for Enum TestEnum");
    EXPECT_EQ(expectedErrorMessage, std::string(e.what()));
  }

  EXPECT_NO_THROW(openstudio::enums::TestEnum("third"));
  EXPECT_THROW(openstudio::enums::TestEnum(3), std::runtime_error);
  try {
    openstudio::enums::TestEnum(3);
  } catch (std::runtime_error& e) {
    std::string expectedErrorMessage("Unknown OpenStudio Enum Value = 3 for Enum TestEnum");
    EXPECT_EQ(expectedErrorMessage, std::string(e.what()));
  }

  EXPECT_THROW(openstudio::enums::TestEnum3(4), std::runtime_error);
  EXPECT_THROW(openstudio::enums::TestEnum3("bob"), std::runtime_error);
  EXPECT_NO_THROW(openstudio::enums::TestEnum3("my second"));
}

TEST(Enum, EnumDescription) {
  openstudio::enums::TestEnum3 te5("my second");
  EXPECT_EQ("second", te5.valueName());
  EXPECT_EQ("TestEnum3", te5.enumName());
  EXPECT_EQ(static_cast<openstudio::enums::TestEnum3::domain>(2), te5.value());
  EXPECT_EQ("My Second", te5.valueDescription());
}

TEST(Enum, EnumInterface) {
  openstudio::enums::TestEnum te1("THIRD");
  openstudio::enums::TestEnum2 te2("SeConD");
  openstudio::enums::TestEnum3 te3(3);
  openstudio::enums::TestEnum3 te4("SIXTH");
  EXPECT_THROW(openstudio::enums::TestEnum3 te5("Seventh"), std::runtime_error);  // does not exist

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
  ASSERT_EQ(static_cast<unsigned>(5), values.size());
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
