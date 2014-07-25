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

#include "../Primes.hpp"

#include <iostream>

using namespace openstudio;

TEST(Primes, isPrime)
{
  EXPECT_FALSE(isPrime<int>(0));
  EXPECT_FALSE(isPrime<int>(1));
  EXPECT_TRUE(isPrime<int>(2));
  EXPECT_TRUE(isPrime<int>(3));
  EXPECT_FALSE(isPrime<int>(4));
  EXPECT_TRUE(isPrime<int>(5));
  EXPECT_FALSE(isPrime<int>(6));
  EXPECT_TRUE(isPrime<int>(7));
  EXPECT_FALSE(isPrime<int>(8));
  EXPECT_FALSE(isPrime<int>(9));
  EXPECT_TRUE(isPrime<int>(11));
  EXPECT_TRUE(isPrime<int>(13));
  EXPECT_FALSE(isPrime<int>(15));
  EXPECT_TRUE(isPrime<int>(17));
  EXPECT_TRUE(isPrime<int>(19));
  EXPECT_FALSE(isPrime<int>(21));
  EXPECT_TRUE(isPrime<int>(23));
  EXPECT_FALSE(isPrime<int>(25));
}
