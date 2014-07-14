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

#include "../Optional.hpp"

#include <sstream>

using openstudio::OptionalInt;
using openstudio::OptionalDouble;
using openstudio::OptionalString;
using openstudio::operator<<;

namespace
{

  TEST(OptionalTest, builtInTypes)
  {
    std::stringstream ss;

    OptionalInt i;
    EXPECT_FALSE(i);
    ss << i;
    EXPECT_EQ("",ss.str()); ss.str("");
    i = 1;
    EXPECT_TRUE(i);
    ss << i;
    EXPECT_EQ("1",ss.str()); ss.str("");
    int j = *i + 1;
    EXPECT_EQ(2,j);

    OptionalDouble d;
    EXPECT_FALSE(d);
    ss << d;
    EXPECT_EQ("",ss.str()); ss.str("");
    d = 1.0;
    EXPECT_TRUE(d);
    ss << d;
    EXPECT_EQ("1",ss.str()); ss.str("");
    double d2 = *d + 1.0;
    EXPECT_EQ(2.0,d2);
  }

  TEST(OptionalTest, stdTypes)
  {
    std::stringstream ss;

    OptionalString str;
    EXPECT_FALSE(str);
    ss << str;
    EXPECT_EQ("",ss.str()); ss.str("");
    str = "Hello World!";
    ASSERT_TRUE(str);
    EXPECT_EQ("Hello World!",*str);
    ss << str;
    EXPECT_EQ("Hello World!",ss.str()); ss.str("");
  }

}
