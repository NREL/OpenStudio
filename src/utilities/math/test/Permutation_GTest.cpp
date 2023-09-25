/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../Permutation.hpp"

#include <algorithm>

using openstudio::permutation;
using openstudio::permute;

TEST(Permutation, IntegerSort) {
  std::vector<int> original;
  original.push_back(8);
  original.push_back(1);
  original.push_back(3);
  original.push_back(9);
  std::vector<int> sorted(original);
  std::sort(sorted.begin(), sorted.end());
  std::vector<unsigned> order = permutation(original, sorted);

  ASSERT_EQ(static_cast<unsigned>(4), order.size());
  EXPECT_EQ(static_cast<unsigned>(2), order[0]);
  EXPECT_EQ(static_cast<unsigned>(0), order[1]);
  EXPECT_EQ(static_cast<unsigned>(1), order[2]);
  EXPECT_EQ(static_cast<unsigned>(3), order[3]);

  std::vector<int> another = permute(original, order);
  EXPECT_TRUE(sorted == another);
}
