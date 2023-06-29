/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "../FloatCompare.hpp"

#include <iostream>

TEST(FloatCompare, Equal) {
  std::cout << "Machine epsilon for double is: " << std::numeric_limits<double>::epsilon() << '\n';

  EXPECT_TRUE(openstudio::equal(1.0 + 1.0, 2.0));
  EXPECT_TRUE(openstudio::equal(std::pow(400.0, 2), 160000.0));
  EXPECT_TRUE(openstudio::equal(std::numeric_limits<double>::min(), -std::numeric_limits<double>::min()));
}
