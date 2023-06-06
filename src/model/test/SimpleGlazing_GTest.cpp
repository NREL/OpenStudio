/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../SimpleGlazing.hpp"
#include "../SimpleGlazing_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, SimpleGlazing_DefaultConstructor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      SimpleGlazing simpleGlazing(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, SimpleGlazing_GettersSetters) {
  Model m;
  SimpleGlazing simpleGlazing(m);

  // Ctor argument defaults
  double uFactor = 0.1;
  double solarHeatGainCoefficient = 0.1;

  // U-Factor:  Double
  // Default in Ctor
  EXPECT_EQ(uFactor, simpleGlazing.uFactor());
  EXPECT_TRUE(simpleGlazing.setUFactor(0.5));
  EXPECT_EQ(0.5, simpleGlazing.uFactor());

  // Solar Heat Gain Coefficient:  Double
  // Default in Ctor
  EXPECT_EQ(solarHeatGainCoefficient, simpleGlazing.solarHeatGainCoefficient());
  EXPECT_TRUE(simpleGlazing.setSolarHeatGainCoefficient(0.68));
  EXPECT_EQ(0.68, simpleGlazing.solarHeatGainCoefficient());

  // Visible Transmittance: Optional Double
  // No Default
  EXPECT_FALSE(simpleGlazing.visibleTransmittance());
  EXPECT_TRUE(simpleGlazing.setVisibleTransmittance(0.45));
  ASSERT_TRUE(simpleGlazing.visibleTransmittance());
  EXPECT_EQ(0.45, simpleGlazing.visibleTransmittance().get());
  simpleGlazing.resetVisibleTransmittance();
  EXPECT_FALSE(simpleGlazing.visibleTransmittance());
}
