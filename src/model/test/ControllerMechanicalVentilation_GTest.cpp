/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>
#include "ModelFixture.hpp"
#include "../Model.hpp"
#include "../ControllerMechanicalVentilation.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, ControllerMechanicalVentilation) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      model::Model m;

      model::ControllerMechanicalVentilation controller(m);
      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}

TEST_F(ModelFixture, ControllerMechanicalVentilation_SystemOutdoorAirMethod) {

  Model m;
  ControllerMechanicalVentilation c(m);
  EXPECT_TRUE(c.isSystemOutdoorAirMethodDefaulted());
  EXPECT_EQ("Standard62.1VentilationRateProcedure", c.systemOutdoorAirMethod());
}
