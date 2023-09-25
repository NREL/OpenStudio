/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../Enum.hpp"
#include "../EnumHelpers.hpp"

using openstudio::isMember;

namespace openstudio {

namespace enums {
  OPENSTUDIO_ENUM(TestEnum, ((first))((second))((third)));

  OPENSTUDIO_ENUM(TestEnum2, ((first)(first)(1))((second))((third)));
  OPENSTUDIO_ENUM(TestEnum3, ((first))((second)(My Second)(2))((third))((fourth)(fourth)(5))((SIXTH)));

}  // namespace enums
}  // namespace openstudio

TEST(EnumHelpers, isMember) {
  openstudio::enums::TestEnum te("third");
  openstudio::StringVector members;
  members.push_back("first");
  members.push_back("elephant");  // bad values are ignored
  EXPECT_FALSE(isMember(te, members));
  members.push_back("third");
  EXPECT_TRUE(isMember(te, members));
}
