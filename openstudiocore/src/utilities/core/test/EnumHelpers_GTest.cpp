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

#include "../Enum.hpp"
#include "../EnumHelpers.hpp"

using openstudio::isMember;

namespace openstudio {

namespace enums {
  OPENSTUDIO_ENUM(TestEnum, ((first)) ((second)) ((third)) );

  OPENSTUDIO_ENUM(TestEnum2, ((first)(first)(1)) ((second)) ((third)) );
  OPENSTUDIO_ENUM(TestEnum3, ((first)) ((second)(My Second)(2)) ((third)) ((fourth)(fourth)(5))
    ((SIXTH)) );


  OPENSTUDIO_ENUM7(Test7Enum,
     ((first)(first)(1)),
     ((second)),
     ((third)),
     ((fourth)),
     ((fifth)),
     ((sixth)),
     ((seventh)(bob)) );
}
} // openstudio

TEST(EnumHelpers, isMember)
{
  openstudio::enums::TestEnum te("third");
  openstudio::StringVector members;
  members.push_back("first");
  members.push_back("elephant"); // bad values are ignored
  EXPECT_FALSE(isMember(te,members));
  members.push_back("third");
  EXPECT_TRUE(isMember(te,members));
}
