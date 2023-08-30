/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../AirGap.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, AirGap_Ctor) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      model::Model m;

      model::AirGap airGap(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, AirGap_Getters_Setters) {
  Model m;
  AirGap airGap(m);

  // Check Default
  EXPECT_DOUBLE_EQ(0.1, airGap.thermalResistance());
  EXPECT_TRUE(airGap.setThermalResistance(0.3));
  EXPECT_DOUBLE_EQ(0.3, airGap.thermalResistance());
  airGap.resetThermalResistance();
  EXPECT_DOUBLE_EQ(0.1, airGap.thermalResistance());
}
