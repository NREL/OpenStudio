/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
