/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../Primes.hpp"

#include <iostream>

using namespace openstudio;

TEST(Primes, isPrime) {
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
