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

#include "../FloatCompare.hpp"

#include <iostream>

TEST(FloatCompare, Equal)
{
  std::cout << "Machine epsilon for double is: " 
            << std::numeric_limits<double>::epsilon() << std::endl;

  EXPECT_TRUE(openstudio::equal(1.0+1.0,2.0));
  EXPECT_TRUE(openstudio::equal(std::pow(400.0,2),160000.0));
  EXPECT_TRUE(openstudio::equal(std::numeric_limits<double>::min(),-std::numeric_limits<double>::min()));
}
