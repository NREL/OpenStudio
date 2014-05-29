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

#include "../Permutation.hpp"

#include <algorithm>

using openstudio::permutation;
using openstudio::permute;

TEST(Permutation, IntegerSort)
{
  std::vector<int> original;
  original.push_back(8);
  original.push_back(1);
  original.push_back(3);
  original.push_back(9);
  std::vector<int> sorted(original);
  std::sort(sorted.begin(),sorted.end());
  std::vector<unsigned> order = permutation(original,sorted);

  ASSERT_EQ(static_cast<unsigned>(4),order.size());
  EXPECT_EQ(static_cast<unsigned>(2),order[0]);
  EXPECT_EQ(static_cast<unsigned>(0),order[1]);
  EXPECT_EQ(static_cast<unsigned>(1),order[2]);
  EXPECT_EQ(static_cast<unsigned>(3),order[3]);

  std::vector<int> another = permute(original,order);
  EXPECT_TRUE(sorted == another);
}
