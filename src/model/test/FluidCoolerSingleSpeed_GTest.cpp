/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"

#include "../FluidCoolerSingleSpeed.hpp"
#include "../FluidCoolerSingleSpeed_Impl.hpp"

using namespace openstudio;
using namespace openstudio::model;

TEST_F(ModelFixture, FluidCoolerSingleSpeed_FluidCoolerSingleSpeed) {
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";

  ASSERT_EXIT(
    {
      Model m;
      FluidCoolerSingleSpeed testObject(m);

      exit(0);
    },
    ::testing::ExitedWithCode(0), "");
}
