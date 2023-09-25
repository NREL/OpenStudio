/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../GasMixture.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, GasMixture) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;

      GasMixture gasMixture(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, GasMixture_Getters_Setters) {

  Model m;
  GasMixture gasMixture(m);

  // Test defaults
  EXPECT_DOUBLE_EQ(0.003, gasMixture.thickness());
  EXPECT_EQ(4, gasMixture.numberofGasesinMixture());
  EXPECT_EQ("Air", gasMixture.gas1Type());
  EXPECT_DOUBLE_EQ(0.97, gasMixture.gas1Fraction());
  EXPECT_EQ("Argon", gasMixture.gas2Type());
  EXPECT_DOUBLE_EQ(0.01, gasMixture.gas2Fraction());

  // Optional fractions
  EXPECT_EQ("Krypton", gasMixture.gas3Type());
  ASSERT_TRUE(gasMixture.gas3Fraction());
  EXPECT_DOUBLE_EQ(0.01, gasMixture.gas3Fraction().get());

  EXPECT_EQ("Xenon", gasMixture.gas4Type());
  ASSERT_TRUE(gasMixture.gas4Fraction());
  EXPECT_DOUBLE_EQ(0.01, gasMixture.gas4Fraction().get());

  EXPECT_TRUE(gasMixture.setThickness(0.5));
  EXPECT_DOUBLE_EQ(0.5, gasMixture.thickness());

  EXPECT_FALSE(gasMixture.setNumberofGasesinMixture(150));
  EXPECT_EQ(4, gasMixture.numberofGasesinMixture());
  EXPECT_TRUE(gasMixture.setNumberofGasesinMixture(3));
  EXPECT_EQ(3, gasMixture.numberofGasesinMixture());

  EXPECT_TRUE(gasMixture.setGas1Fraction(0.9));
  EXPECT_DOUBLE_EQ(0.9, gasMixture.gas1Fraction());
  EXPECT_TRUE(gasMixture.setGas1Type("Krypton"));
  EXPECT_EQ("Krypton", gasMixture.gas1Type());
  EXPECT_FALSE(gasMixture.setGas1Type("BadValue"));
  EXPECT_EQ("Krypton", gasMixture.gas1Type());

  EXPECT_TRUE(gasMixture.setGas2Fraction(0.05));
  EXPECT_DOUBLE_EQ(0.05, gasMixture.gas2Fraction());
  EXPECT_TRUE(gasMixture.setGas2Type("Air"));
  EXPECT_EQ("Air", gasMixture.gas2Type());
  EXPECT_FALSE(gasMixture.setGas2Type("BadValue"));
  EXPECT_EQ("Air", gasMixture.gas2Type());

  EXPECT_TRUE(gasMixture.setGas3Fraction(0.02));
  ASSERT_TRUE(gasMixture.gas3Fraction());
  EXPECT_DOUBLE_EQ(0.02, gasMixture.gas3Fraction().get());
  EXPECT_TRUE(gasMixture.setGas3Type("Xenon"));
  EXPECT_EQ("Xenon", gasMixture.gas3Type());
  EXPECT_FALSE(gasMixture.setGas3Type("BadValue"));
  EXPECT_EQ("Xenon", gasMixture.gas3Type());

  EXPECT_TRUE(gasMixture.setGas4Fraction(0.035));
  ASSERT_TRUE(gasMixture.gas4Fraction());
  EXPECT_DOUBLE_EQ(0.035, gasMixture.gas4Fraction().get());
  EXPECT_TRUE(gasMixture.setGas4Type("Argon"));
  EXPECT_EQ("Argon", gasMixture.gas4Type());
  EXPECT_FALSE(gasMixture.setGas4Type("BadValue"));
  EXPECT_EQ("Argon", gasMixture.gas4Type());
}
